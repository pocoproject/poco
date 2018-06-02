//
// UDPSocketReader.h
//
// Library: Net
// Package: UDP
// Module:  UDPSocketReader
//
// Definition of the UDPSocketReader class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_UDPSocketReader_INCLUDED
#define Net_UDPSocketReader_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/DatagramSocket.h"


namespace Poco {
namespace Net {


template <std::size_t S = POCO_UDP_BUF_SIZE>
class UDPSocketReader
	/// UDPSocketReader is responsible for UDP socket I/O operations.
	/// When data or error is detected on a socket, the reader is invoked
	/// to do the I/O. After the data is read, it is passed on, to one
	/// of the handlers for processing. Errors are also passed to a
	/// handler for handling (if any configured).
	/// Depending on settings, data senders may be notified of the handler's
	/// data and error backlogs.
{
private:
	class Counter
	{
	public:
		Counter(): val(0)
		{
		}

		operator AtomicCounter::ValueType ()
		{
			return static_cast<AtomicCounter::ValueType>(val);
		}

		Counter& operator=(AtomicCounter::ValueType value)
		{
			val = static_cast<Poco::Int32>(value);
			return *this;
		}

		Poco::Int32 operator++()
		{
			return ++val;
		}

		Counter& operator++(int)
		{
			return val++;
		}

	private:
		Poco::Int32 val;
	};

public:
	UDPSocketReader(typename UDPHandlerImpl<S>::List& handlers, int backlogThreshold = 0):
		_handlers(handlers),
		_handler(_handlers.begin()),
		_backlogThreshold(backlogThreshold)
		/// Creates the UDPSocketReader.
	{
		poco_assert(_handler != _handlers.end());
	}

	UDPSocketReader(typename UDPHandlerImpl<S>::List& handlers, const UDPServerParams& serverParams):
		_handlers(handlers),
		_handler(_handlers.begin()),
		_backlogThreshold(serverParams.backlogThreshold())
		/// Creates the UDPSocketReader.
	{
		poco_assert(_handler != _handlers.end());
	}

	~UDPSocketReader()
		/// Destroys the UDPSocketReader.
	{
	}

	void read(DatagramSocket& sock)
		/// Reads data from the socket and passes it to the next handler.
		/// Errors are also passed to the handler. If object is configured
		/// for replying to sender and data or error backlog threshold is
		/// exceeded, sender is notified of the current backlog size.
	{
		typedef typename UDPHandlerImpl<S>::MsgSizeT RT;
		char* p = 0;
		struct sockaddr* pSA = 0;
		poco_socklen_t* pAL = 0;
		poco_socket_t sockfd = sock.impl()->sockfd();
		nextHandler();
		try
		{
			p = handler().next(sockfd);
			if (p)
			{
				Poco::UInt16 off = handler().offset();
				poco_socklen_t* pAL = reinterpret_cast<poco_socklen_t*>(p + sizeof(RT));
				*pAL = SocketAddress::MAX_ADDRESS_LENGTH;
				struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(p + sizeof(RT) + sizeof(poco_socklen_t));
				RT ret = sock.receiveFrom(p + off, S - off - 1, &pSA, &pAL);
				if (ret < 0)
				{
					AtomicCounter::ValueType errors = setError(sock.impl()->sockfd(), p, Error::getMessage(Error::last()));
					if (_backlogThreshold > 0 && errors > _backlogThreshold && errors != _errorBacklog[sockfd])
					{
						Poco::Int32 err = static_cast<Poco::Int32>(errors);
						sock.sendTo(&err, sizeof(Poco::Int32), SocketAddress(pSA, *pAL));
						_errorBacklog[sockfd] = errors;
					}
					return;
				}
				AtomicCounter::ValueType data = handler().setData(p, ret);
				p[off + ret] = 0; // for ascii convenience, zero-terminate
				if (_backlogThreshold > 0 && data > _backlogThreshold && data != _dataBacklog[sockfd])
				{
					Poco::Int32 d = static_cast<Poco::Int32>(data);
					sock.sendTo(&d, sizeof(Poco::Int32), SocketAddress(pSA, *pAL));
					_dataBacklog[sockfd] = data;
				}
			}
			else return;
		}
		catch (Poco::Exception& exc)
		{
			AtomicCounter::ValueType errors = setError(sock.impl()->sockfd(), p, exc.displayText());
			if (_backlogThreshold > 0 && errors > _backlogThreshold && errors != _errorBacklog[sockfd] && pSA && pAL)
			{
				Poco::Int32 err = static_cast<Poco::Int32>(errors);
				sock.sendTo(&err, sizeof(Poco::Int32), SocketAddress(pSA, *pAL));
				_errorBacklog[sockfd] = errors;
			}
		}
		handler().notify();
	}

	bool handlerStopped() const
		/// Returns true if all handlers are stopped.
	{
		bool stopped = true;
		typename UDPHandlerImpl<S>::List::iterator it = _handlers.begin();
		typename UDPHandlerImpl<S>::List::iterator end = _handlers.end();
		for (; it != end; ++it) stopped = stopped && (*it)->stopped();
		return stopped;
	}

	void stopHandler()
		/// Stops all handlers.
	{
		typename UDPHandlerImpl<S>::List::iterator it = _handlers.begin();
		typename UDPHandlerImpl<S>::List::iterator end = _handlers.end();
		for (; it != end; ++it) (*it)->stop();
	}

	bool handlerDone() const
		/// Returns true if all handlers are done processing data.
	{
		bool done = true;
		typename UDPHandlerImpl<S>::List::iterator it = _handlers.begin();
		typename UDPHandlerImpl<S>::List::iterator end = _handlers.end();
		for (; it != end; ++it) done = done && (*it)->done();
		return done;
	}

	AtomicCounter::ValueType setError(poco_socket_t sock, char* buf = 0, const std::string& err = "")
		/// Sets error to the provided buffer buf. If the buffer is null, a new buffer is obtained
		/// from handler.
		/// If successful, returns the handler's eror backlog size, otherwise returns zero.
	{
		if (!buf) buf = handler().next(sock);
		if (buf) return handler().setError(buf, err.empty() ? Error::getMessage(Error::last()) : err);
		return 0;
	}

private:
	void nextHandler()
		/// Re-points the handler iterator to the next handler in
		/// round-robin fashion.
	{
		poco_assert_dbg (_handler != _handlers.end());
		if (++_handler == _handlers.end()) _handler = _handlers.begin();
	}

	UDPHandlerImpl<S>& handler()
		/// Returns the reference to the current handler.
	{
		poco_assert_dbg (_handler != _handlers.end());
		return **_handler;
	}

	typedef typename UDPHandlerImpl<S>::List           HandlerList;
	typedef typename UDPHandlerImpl<S>::List::iterator HandlerIterator;
	typedef std::map<poco_socket_t, Counter>           CounterMap;

	HandlerList&    _handlers;
	HandlerIterator _handler;
	CounterMap      _dataBacklog;
	CounterMap      _errorBacklog;
	int             _backlogThreshold;
};


} } // namespace Poco::Net


#endif // Net_UDPSocketReader_INCLUDED
