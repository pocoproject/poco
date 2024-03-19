//
// UDPHandler.h
//
// Library: Net
// Package: UDP
// Module:  UDPHandler
//
// Definition of the UDPHandler class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_UDPHandler_INCLUDED
#define Net_UDPHandler_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketDefs.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/MemoryPool.h"
#include "Poco/Event.h"
#include "Poco/Mutex.h"
#include "Poco/StringTokenizer.h"
#include <deque>
#include <cstring>
#include <atomic>
#include <map>

namespace Poco {
namespace Net {


using UDPMsgSizeT = int;
constexpr std::size_t POCO_UDP_BUF_SIZE = 1472 + sizeof(UDPMsgSizeT) + SocketAddress::MAX_ADDRESS_LENGTH;


template <std::size_t S = POCO_UDP_BUF_SIZE, class TMutex = Poco::FastMutex>
class UDPHandlerImpl: public Runnable, public RefCountedObject
	/// UDP handler handles the data that arrives to the UDP server.
	/// The class is thread-safe and runs in its own thread, so many handlers
	/// can be used in parallel. Handler manages and provides the storage
	/// (fixed-size memory blocks of S size) to the reader, which signals back
	/// to the handler when there is data or error ready for processing.
	/// Typically, user will inherit from this class and override processData()
	/// and processError() members to do the actual work. To auto-start the handler,
	/// the inheriting class can call start() in the constructor.
{
public:
	using MsgSizeT = UDPMsgSizeT;
	using Ptr = AutoPtr<UDPHandlerImpl>;
	using List = std::vector<Ptr>;
	using DFMutex = TMutex;

	static constexpr MsgSizeT BUF_STATUS_IDLE  = 0;
	static constexpr MsgSizeT BUF_STATUS_BUSY  = -1;
	static constexpr MsgSizeT BUF_STATUS_ERROR = -2;

	UDPHandlerImpl(std::size_t bufListSize = 1000, std::ostream* pErr = nullptr):
		_thread("UDPHandlerImpl"),
		_stop(false),
		_done(false),
		_bufListSize(bufListSize),
		_blockSize(S),
		_dataBacklog(0),
		_errorBacklog(0),
		_pErr(pErr)
		/// Creates the UDPHandlerImpl.
	{
	}

	~UDPHandlerImpl() override
		/// Destroys the UDPHandlerImpl.
	{
		stop();
		_thread.join();
	}

	std::size_t blockSize() const
		/// Returns the memory block size.
	{
		return _blockSize;
	}

	char* next(poco_socket_t sock)
		/// Creates the next BufList entry, and returns
		/// the pointers to the newly created guard/buffer.
		/// If mutex lock times out, returns null pointer.
	{
		char* ret = nullptr;
		if (_mutex.tryLock(10))
		{
			if (_buffers[sock].size() < _bufListSize) // building buffer list
			{
				makeNext(sock, &ret);
			}
			else if (payloadSize(*_bufIt[sock]) != 0) // busy
			{
				makeNext(sock, &ret);
			}
			else if (payloadSize(*_bufIt[sock]) == 0) // available
			{
				setBusy(*_bufIt[sock]);
				ret = *_bufIt[sock];
				if (++_bufIt[sock] == _buffers[sock].end())
				{
					_bufIt[sock] = _buffers[sock].begin();
				}
			}
			else // last resort, full scan
			{
				auto& buffers { _buffers[sock] };
				auto it {buffers.begin()};
				for (; it != buffers.end(); ++it)
				{
					if (payloadSize(*_bufIt[sock]) == 0) // available
					{
						setBusy(*it);
						ret = *it;
						_bufIt[sock] = it;
						if (++_bufIt[sock] == _buffers[sock].end())
						{
							_bufIt[sock] = _buffers[sock].begin();
						}
						break;
					}
				}
				if (it == buffers.end()) makeNext(sock, &ret);
			}
			_mutex.unlock();
		}
		return ret;
	}

	void notify()
		/// Sets the data ready event.
	{
		_dataReady.set();
	}

	void run() override
		/// Does the work.
	{
		while (!_stop)
		{
			_dataReady.wait();
			if (_stop) break;
			if (_mutex.tryLock(10))
			{
				if (!_stop)
				{
					for (const auto& buf: _buffers)
					{
						for (auto* list: buf.second)
						{
							if (hasData(list))
							{
								processData(list);
								--_dataBacklog;
								setIdle(list);
							}
							else if (isError(list))
							{
								processError(list);
								++_errorBacklog;
							}
						}
					}
				}
				_mutex.unlock();
			}
		}
		_done = true;
	}

	void stop()
		/// Signals the handler to stop.
	{
		_stop = true;
		_dataReady.set();
	}

	bool stopped() const
		/// Returns true if the handler was signalled to stop.
	{
		return _stop == true;
	}

	bool done() const
		/// Returns true if handler is done (ie. run() thread
		/// entrypoint end was reached).
	{
		return _done;
	}

	void setBusy(char* pBuf)
		/// Flags the buffer as busy (usually done before buffer
		/// is passed to the reader.
	{
		setStatus(pBuf, BUF_STATUS_BUSY);
	}

	void setIdle(char* pBuf)
		/// Flags the buffer as idle, ie. not used by reader or
		/// waiting to be processed, so ready to be reused for
		/// reading.
	{
		setStatus(pBuf, BUF_STATUS_IDLE);
	}

	AtomicCounter::ValueType setData(char* pBuf, MsgSizeT sz)
		/// Flags the buffer as containing data.
	{
		setStatus(pBuf, sz);
		return ++_dataBacklog;
	}

	AtomicCounter::ValueType setError(char* pBuf, const std::string& err)
		/// Sets the error into the buffer.
	{
		std::size_t availLen = S - errorOffset();
		std::memset(pBuf + errorOffset(), 0, availLen);
		std::size_t msgLen = err.length();
		if (msgLen)
		{
			if (msgLen >= availLen) msgLen = availLen;
			std::memcpy(pBuf + errorOffset(), err.data(), msgLen);
		}
		setStatus(pBuf, BUF_STATUS_ERROR);
		return --_errorBacklog;
	}

	bool hasData(const char* pBuf)
		/// Returns true if buffer contains data.
	{
		typename DFMutex::ScopedLock l(_dfMutex);
		return payloadSize(pBuf) > 0;
	}

	bool isError(const char* pBuf)
		/// Returns true if buffer contains error.
	{
		typename DFMutex::ScopedLock l(_dfMutex);
		return *reinterpret_cast<const MsgSizeT*>(pBuf) == BUF_STATUS_ERROR;
	}

	static constexpr Poco::UInt16 errorOffset()
		/// Returns offset of address length.
	{
		return sizeof(MsgSizeT);
	}

	static constexpr Poco::UInt16 addressLengthOffset()
		/// Returns offset of address length.
	{
		return sizeof(MsgSizeT);
	}

	static constexpr Poco::UInt16 addressOffset()
		/// Returns offset of address.
	{
		return addressLengthOffset() + sizeof(poco_socklen_t);
	}

	static constexpr Poco::UInt16 payloadOffset()
		/// Returns buffer data offset.
	{
		return addressOffset() + SocketAddress::MAX_ADDRESS_LENGTH;
	}

	static MsgSizeT payloadSize(const char* buf)
	{
		return *reinterpret_cast<const MsgSizeT*>(buf);
	}

	static SocketAddress address(const char* buf)
	{
		const auto* len = reinterpret_cast<const poco_socklen_t*>(buf + addressLengthOffset());
		const auto* pSA = reinterpret_cast<const struct sockaddr*>(buf + addressOffset());
		poco_assert(*len <= SocketAddress::MAX_ADDRESS_LENGTH);

		return SocketAddress(pSA, *len);
	}

	static const char* payload(const char* buf)
		/// Returns pointer to payload.
		///
		/// Total message size is S.
		///
		/// Data message layout is as follows:
		///
		/// +------------------------+------------------------+-----------------------------------+--------- ~ ---+
		/// | sizeof(MsgSizeT) bytes | sizeof(poco_socklen_t) | SocketAddress::MAX_ADDRESS_LENGTH |    payload    |
		/// +------------------------+------------------------+-----------------------------------+--------- ~ ---+
	{
		return buf + payloadOffset();
	}

	static Poco::StringTokenizer payload(const char* buf, char delimiter)
		/// Returns tokenized payload.
		/// Used when multiple logical messages are contained in a
		/// single physical message. Messages must be ASCII, as well as
		/// unambiguously delimited in order for this function to succeed.
	{
		return Poco::StringTokenizer(payload(buf), std::string(1, delimiter), StringTokenizer::TOK_IGNORE_EMPTY);
	}

	static const char* error(const char* buf)
		/// Returns pointer to the error message payload.
		///
		/// Total message size is S.
		///
		/// Error message layout is as follows:
		///
		/// +------------------------+--------- ~ ---+
		/// | sizeof(MsgSizeT) bytes |    payload    |
		/// +------------------------+--------- ~ ---+
	{
		return buf + sizeof(MsgSizeT);
	}

	virtual void processData(char*)
		/// Called when data is received by reader.
		///
		/// No-op here, must be overridden by inheriting
		/// class in order to do useful work.
	{
	};

	virtual void processError(char* buf)
		/// Called when error is detected by reader.
		///
		/// Only functional if stream pointer is provided
		/// to the handler, otherwise it must be overridden
		/// by inheriting class in order to do useful work.
	{
		if (_pErr) *_pErr << error(buf) << std::endl;
		setIdle(buf);
	}

	void start()
		/// Starts the handler run in thread.
	{
		_thread.start(*this);
	}

private:
	using BufList = std::deque<char*>;
	using BufMap = std::map<poco_socket_t, BufList>;
	using BLIt = typename BufList::iterator;
	using BufIt = std::map<poco_socket_t, BLIt>;
	using BufArray = std::array<char, S>;
	using MemPool = Poco::FastMemoryPool<BufArray>;

	void setStatusImpl(char* pBuf, MsgSizeT status)
	{
		*reinterpret_cast<MsgSizeT*>(pBuf) = status;
	}

	void setStatus(char* pBuf, MsgSizeT status)
	{
		typename DFMutex::ScopedLock l(_dfMutex);
		setStatusImpl(pBuf, status);
	}

	void makeNext(poco_socket_t sock, char** ret)
	{
		_buffers[sock].push_back(reinterpret_cast<char*>(_memPool.get()));
		setStatusImpl(_buffers[sock].back(), BUF_STATUS_BUSY);
		_bufIt[sock] = _buffers[sock].begin();
		*ret = _buffers[sock].back();
	}

	Poco::Event       _dataReady;
	Poco::Thread      _thread;
	std::atomic<bool> _stop;
	std::atomic<bool> _done;
	BufMap            _buffers;
	BufIt             _bufIt;
	std::size_t       _bufListSize;
	const std::size_t _blockSize;
	MemPool           _memPool;
	AtomicCounter     _dataBacklog;
	AtomicCounter     _errorBacklog;
	Poco::FastMutex   _mutex;
	DFMutex           _dfMutex;
	std::ostream*     _pErr;
};


using UDPHandler = UDPHandlerImpl<>;


} } // namespace Poco::Net


#endif // Net_UDPHandler_INCLUDED
