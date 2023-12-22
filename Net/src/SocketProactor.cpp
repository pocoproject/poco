//
// SocketProactor.cpp
//
// Library: Net
// Package: Sockets
// Module:  SocketProactor
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketProactor.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/DatagramSocketImpl.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#ifdef POCO_OS_FAMILY_WINDOWS
#ifdef max
#undef max
#endif // max
#endif // POCO_OS_FAMILY_WINDOWS
#include <limits>


using Poco::Exception;
using Poco::ErrorHandler;


namespace Poco {
namespace Net {


//
// Worker
//

class Worker
	/// Worker is a utility class that executes work (functions).
	/// Workload can be permanent (executed on every doWork() call),
	/// or "one-shot" (scheduled for a single execution at a point
	/// in the future).
{
public:
	using MutexType = SocketProactor::MutexType;
	using ScopedLock = SocketProactor::ScopedLock;
	using Work = SocketProactor::Work;
	using WorkEntry = std::pair<Work, Poco::Timestamp>;
	using WorkList = std::deque<WorkEntry>;

	void addWork(const Work& ch, Timestamp::TimeDiff ms = SocketProactor::PERMANENT_COMPLETION_HANDLER)
	{
		addWork(Work(ch), ms);
	}

	void addWork(Work&& ch, Timestamp::TimeDiff ms, int pos = -1)
	{
		auto pch = SocketProactor::PERMANENT_COMPLETION_HANDLER;
		Poco::Timestamp expires = (ms != pch) ? Timestamp() + (ms * 1000) : Timestamp(pch);
		if (pos == -1 || (pos + 1) > _funcList.size())
		{
			ScopedLock lock(_mutex);
			_funcList.push_back({std::move(ch), expires});
		}
		else
		{
			if (pos < 0)
				throw Poco::InvalidArgumentException("SocketProactor::addWork()");
			ScopedLock lock(_mutex);
			_funcList.insert(_funcList.begin() + pos, {std::move(ch), expires});
		}

	}

	void removeWork()
	{
		ScopedLock lock(_mutex);
		_funcList.clear();
	}

	int scheduledWork()
	{
		int cnt = 0;
		ScopedLock lock(_mutex);
		WorkList::iterator it = _funcList.begin();
		for (; it != _funcList.end(); ++it)
		{
			if (!isPermanent(it->second)) ++cnt;
		}
		return cnt;
	}

	int removeScheduledWork(int count)
	{
		auto isScheduled = [](const Timestamp &ts)
		{ return !isPermanent(ts); };
		return removeWork(isScheduled, count);
	}

	int permanentWork()
	{
		int cnt = 0;
		ScopedLock lock(_mutex);
		WorkList::iterator it = _funcList.begin();
		for (; it != _funcList.end(); ++it)
		{
			if (isPermanent(it->second))
				++cnt;
		}
		return cnt;
	}


	int removePermanentWork(int count)
	{
		auto perm = [](const Timestamp &ts)
		{ return isPermanent(ts); };
		return removeWork(perm, count);
	}

	static bool isPermanent(const Timestamp &entry)
	{
		return entry == Timestamp(SocketProactor::PERMANENT_COMPLETION_HANDLER);
	}

	int doWork(bool handleOne, bool expiredOnly)
	{
		std::unique_ptr<Work> pCH;
		int handled = 0;
		{
			ScopedLock lock(_mutex);
			WorkList::iterator it = _funcList.begin();
			try
			{
				while (it != _funcList.end())
				{
					std::size_t prevSize = 0;
					bool alwaysRun = isPermanent(it->second) && !expiredOnly;
					bool isExpired = !alwaysRun && (Timestamp() >= it->second);
					if (isExpired)
					{
						pCH.reset(new Work(std::move(it->first)));
						it = _funcList.erase(it);
					}
					else if (alwaysRun)
					{
						pCH.reset(new Work(it->first));
						++it;
					}
					else ++it;
					prevSize = _funcList.size();

					if (pCH)
					{
						(*pCH)();
						pCH.reset();
						++handled;
						if (handleOne) break;
					}
					// handler call may add or remove handlers;
					// if so, we must start from the beginning
					if (prevSize != _funcList.size())
						it = _funcList.begin();
				}
			}
			catch (Exception& exc)
			{
				ErrorHandler::handle(exc);
			}
			catch (std::exception& exc)
			{
				ErrorHandler::handle(exc);
			}
			catch (...)
			{
				ErrorHandler::handle();
			}
		}
		return handled;
	}

	int runOne()
	{
		try
		{
			while (0 == doWork(true, false));
			return 1;
		}
		catch(...) {}
		return 0;
	}

private:
	template <typename F>
	int removeWork(F isType, int count)
		/// Removes `count` functions of the specified type;
		/// if count is -1, removes all the functions of the
		/// specified type.
	{
		int removed = 0;
		ScopedLock lock(_mutex);
		int left = count > -1 ? count : static_cast<int>(_funcList.size());
		WorkList::iterator it = _funcList.begin();
		while (left && it != _funcList.end())
		{
			if (isType(it->second))
			{
				++removed;
				it = _funcList.erase((it));
				--left;
			}
			else ++it;
		}
		return removed;
	}

	WorkList  _funcList;
	MutexType _mutex;
};


//
// SocketProactor
//

const Timestamp::TimeDiff SocketProactor::PERMANENT_COMPLETION_HANDLER =
	std::numeric_limits<Timestamp::TimeDiff>::max();


SocketProactor::SocketProactor(bool worker):
	_isRunning(false),
	_isStopped(false),
	_stop(false),
	_timeout(0),
	_maxTimeout(DEFAULT_MAX_TIMEOUT_MS),
	_pThread(nullptr),
	_ioCompletion(_maxTimeout),
	_pWorker(worker ? new Worker : nullptr)
{
}


SocketProactor::SocketProactor(const Poco::Timespan& timeout, bool worker):
	_isRunning(false),
	_isStopped(false),
	_stop(false),
	_timeout(0),
	_maxTimeout(static_cast<long>(timeout.totalMilliseconds())),
	_pThread(nullptr),
	_ioCompletion(_maxTimeout),
	_pWorker(worker ? new Worker : nullptr)
{
}


SocketProactor::~SocketProactor()
{
	_ioCompletion.stop();
	wait();
	for (auto& pS : _writeHandlers)
	{
		for (auto& pH : pS.second)
		{
			if (pH->_pBuf && pH->_owner)
				delete pH->_pBuf;
		}
	}
}


void SocketProactor::wait()
{
	_ioCompletion.wakeUp();
	_ioCompletion.wait();
}


bool SocketProactor::hasHandlers(SubscriberMap& handlers, int sockfd)
{
	Poco::Mutex::ScopedLock l(_writeMutex);
	if (handlers.end() == handlers.find(sockfd))
		return false;
	return true;
}


int SocketProactor::poll(int* pHandled)
{
	int handled = 0;
	int worked = 0;
	PollSet::SocketModeMap sm = _pollSet.poll(_timeout);
	if (sm.size() > 0)
	{
		auto it = sm.begin();
		auto end = sm.end();
		for (; it != end; ++it)
		{
			if (it->second & PollSet::POLL_READ)
			{
				Socket sock = it->first;
				if (hasHandlers(_readHandlers, static_cast<int>(sock.impl()->sockfd())))
					handled += receive(sock);
			}
			if (it->second & PollSet::POLL_WRITE)
			{
				Socket sock = it->first;
				if (hasHandlers(_writeHandlers, static_cast<int>(sock.impl()->sockfd())))
					handled += send(sock);
			}
			if (it->second & PollSet::POLL_ERROR)
			{
				Socket sock = it->first;
				handled += error(sock);
			}
		}
	}

	if (_pWorker)
	{
		if (hasSocketHandlers() && handled) worked = doWork();
		else worked = doWork(false, true);
	}

	if (pHandled) *pHandled = handled;
	return worked;
}


void SocketProactor::addReceiveFrom(Socket sock, Buffer& buf, Poco::Net::SocketAddress& addr, Callback&& onCompletion)
{
	if (!sock.isDatagram())
		throw Poco::InvalidArgumentException("SocketProactor::addSend(): UDP socket required");
	std::unique_ptr<Handler> pHandler(new Handler);
	pHandler->_pAddr = std::addressof(addr);
	pHandler->_pBuf = std::addressof(buf);
	pHandler->_onCompletion = std::move(onCompletion);

	Poco::Mutex::ScopedLock l(_readMutex);
	_readHandlers[sock.impl()->sockfd()].push_back(std::move(pHandler));
	if (!has(sock)) addSocket(sock, PollSet::POLL_READ);
}


void SocketProactor::addSendTo(Socket sock, const Buffer& message, const SocketAddress& addr, Callback&& onCompletion)
{
	if (!sock.isDatagram())
		throw Poco::InvalidArgumentException("SocketProactor::addSend(): UDP socket required");
	Buffer* pMessage = nullptr;
	SocketAddress* pAddr = nullptr;
	try
	{
		pMessage = new Buffer(message);
		pAddr = new SocketAddress(addr);
	}
	catch(...)
	{
		delete pMessage;
		delete pAddr;
		throw;
	}
	addSend(sock, pMessage, pAddr, std::move(onCompletion), true);
}


void SocketProactor::addSendTo(Socket sock, Buffer&& message, const SocketAddress&& addr, Callback&& onCompletion)
{
	if (!sock.isDatagram())
		throw Poco::InvalidArgumentException("SocketProactor::addSend(): UDP socket required");
	Buffer* pMessage = nullptr;
	SocketAddress* pAddr = nullptr;
	try
	{
		pMessage = new Buffer(std::move(message));
		pAddr = new SocketAddress(std::move(addr));
	}
	catch(...)
	{
		delete pMessage;
		delete pAddr;
		throw;
	}
	addSend(sock, pMessage, pAddr, std::move(onCompletion), true);
}


void SocketProactor::addReceive(Socket sock, Buffer& buf, Callback&& onCompletion)
{
	if (!sock.isStream())
		throw Poco::InvalidArgumentException("SocketProactor::addSend(): TCP socket required");
	std::unique_ptr<Handler> pHandler(new Handler);
	pHandler->_pAddr = nullptr;
	pHandler->_pBuf = std::addressof(buf);
	pHandler->_onCompletion = std::move(onCompletion);

	Poco::Mutex::ScopedLock l(_readMutex);
	_readHandlers[sock.impl()->sockfd()].push_back(std::move(pHandler));
	if (!has(sock)) addSocket(sock, PollSet::POLL_READ);
}


void SocketProactor::addSend(Socket sock, const Buffer& message, Callback&& onCompletion)
{
	if (!sock.isStream())
		throw Poco::InvalidArgumentException("SocketProactor::addSend(): TCP socket required");
	Buffer* pMessage = nullptr;
	try
	{
		pMessage = new Buffer(message);
	}
	catch(...)
	{
		delete pMessage;
		throw;
	}
	addSend(sock, pMessage, nullptr, std::move(onCompletion), true);
}


void SocketProactor::addSend(Socket sock, Buffer&& message, Callback&& onCompletion)
{
	if (!sock.isStream())
		throw Poco::InvalidArgumentException("SocketProactor::addSend(): TCP socket required");
	Buffer* pMessage = nullptr;
	try
	{
		pMessage = new Buffer(std::move(message));
	}
	catch(...)
	{
		delete pMessage;
		throw;
	}
	addSend(sock, pMessage, nullptr, std::move(onCompletion), true);
}


void SocketProactor::addSend(Socket sock, Buffer* pMessage, SocketAddress* pAddr, Callback&& onCompletion, bool own)
{
	std::unique_ptr<Handler> pHandler(new Handler);
	pHandler->_pAddr = pAddr;
	pHandler->_pBuf = pMessage;
	pHandler->_onCompletion = std::move(onCompletion);
	pHandler->_owner = own;

	Poco::Mutex::ScopedLock l(_writeMutex);
	_writeHandlers[sock.impl()->sockfd()].push_back(std::move(pHandler));
	if (!has(sock)) addSocket(sock, PollSet::POLL_WRITE);
}


int SocketProactor::error(Socket& sock)
{
	int cnt = errorImpl(sock, _readHandlers, _readMutex);
	cnt += errorImpl(sock, _writeHandlers, _writeMutex);
	return cnt;
}


int SocketProactor::send(Socket& sock)
{
	Poco::Mutex::ScopedLock l(_writeMutex);
	auto hIt = _writeHandlers.find(sock.impl()->sockfd());
	if (hIt == _writeHandlers.end()) return 0;
	IOHandlerList& handlers = hIt->second;
	int handled = static_cast<int>(handlers.size());
	auto it = handlers.begin();
	auto end = handlers.end();
	while (it != end)
	{
		if (sock.isDatagram())
			sendTo(*sock.impl(), it);
		else if (sock.isStream())
			send(*sock.impl(), it);
		else
		{
			deleteHandler(handlers, it);
			throw Poco::InvalidArgumentException("Unknown socket type.");
		}
		deleteHandler(handlers, it);

		// end iterator is invalidated when the last member
		// is removed, so make sure we don't check for it
		if (handlers.empty()) break;
	}
	handled -= static_cast<int>(handlers.size());
	if (handled) _ioCompletion.wakeUp();
	return handled;
}


void SocketProactor::sendTo(SocketImpl& sock, IOHandlerIt& it)
{
	Buffer* pBuf = (*it)->_pBuf;
	if (pBuf && pBuf->size())
	{
		SocketAddress *pAddr = (*it)->_pAddr;
		int n = 0, err = 0;
		try
		{
			n = sock.sendTo(&(*pBuf)[0], static_cast<int>(pBuf->size()), *pAddr);
		}
		catch(std::exception&)
		{
			err = Socket::lastError();
		}
		enqueueIONotification(std::move((*it)->_onCompletion), n, err);
	}
	else
	{
		if (!pBuf)
			throw Poco::NullPointerException("SocketProactor::sendTo(): null buffer");
		else if (pBuf->empty())
			throw Poco::InvalidArgumentException("SocketProactor::sendTo(): empty buffer");
		else // we shouldn't be here
			throw Poco::InvalidAccessException("SocketProactor::sendTo(): unexpected error");
	}
}


void SocketProactor::send(SocketImpl& sock, IOHandlerIt& it)
{
	Buffer* pBuf = (*it)->_pBuf;
	if (pBuf && pBuf->size())
	{
		int n = 0, err = 0;
		try
		{
			n = sock.sendBytes(&(*pBuf)[0], static_cast<int>(pBuf->size()));
		}
		catch(std::exception&)
		{
			err = Socket::lastError();
		}
		enqueueIONotification(std::move((*it)->_onCompletion), n, err);
	}
	else
	{
		if (!pBuf)
			throw Poco::NullPointerException("SocketProactor::sendTo(): null buffer");
		else if (pBuf->empty())
			throw Poco::InvalidArgumentException("SocketProactor::sendTo(): empty buffer");
		else // we shouldn't be here
			throw Poco::InvalidAccessException("SocketProactor::sendTo(): unexpected error");
	}
}


int SocketProactor::receive(Socket& sock)
{
	Poco::Mutex::ScopedLock l(_readMutex);
	auto hIt = _readHandlers.find(sock.impl()->sockfd());
	if (hIt == _readHandlers.end()) return 0;
	IOHandlerList& handlers = hIt->second;
	int handled = static_cast<int>(handlers.size());
	int avail = 0;
	auto it = handlers.begin();
	auto end = handlers.end();
	for (; it != end;)
	{
		if ((avail = sock.available()))
		{
			if (sock.isDatagram())
				receiveFrom(*sock.impl(), it, avail);
			else if (sock.isStream())
				receive(*sock.impl(), it, avail);
			else
				throw Poco::InvalidArgumentException("Unknown socket type.");

			++it;
			handlers.pop_front();
			// end iterator is invalidated when the last member
			// is removed, so make sure we don't check for it
			if (handlers.size() == 0) break;
		}
		else break;
	}
	handled -= static_cast<int>(handlers.size());
	if (handled) _ioCompletion.wakeUp();
	return handled;
}


void SocketProactor::receiveFrom(SocketImpl& sock, IOHandlerIt& it, int available)
{
	Buffer *pBuf = (*it)->_pBuf;
	SocketAddress *pAddr = (*it)->_pAddr;
	SocketAddress addr = *pAddr;
	poco_check_ptr(pBuf);
	if (pBuf->size() < available) pBuf->resize(available);
	int n = 0, err = 0;
	try
	{
		n = sock.receiveFrom(&(*pBuf)[0], available, *pAddr);
	}
	catch(std::exception&)
	{
		err = Socket::lastError();
	}
	enqueueIONotification(std::move((*it)->_onCompletion), n, err);
}


void SocketProactor::receive(SocketImpl& sock, IOHandlerIt& it, int available)
{
	Buffer *pBuf = (*it)->_pBuf;
	poco_check_ptr(pBuf);
	if (pBuf->size() < available) pBuf->resize(available);
	int n = 0, err = 0;
	try
	{
		n = sock.receiveBytes(&(*pBuf)[0], available);
	}
	catch(std::exception&)
	{
		err = Socket::lastError();
	}
	enqueueIONotification(std::move((*it)->_onCompletion), n, err);
}


int SocketProactor::doWork(bool handleOne, bool expiredOnly)
{
	return worker().doWork(handleOne, expiredOnly);
}


int SocketProactor::runOne()
{
	return worker().runOne();
}


void SocketProactor::sleep(bool isAtWork)
{
	try
	{
		if (isAtWork)
		{
			_timeout = 0;
			return;
		}
		else
		{
			if (_timeout < _maxTimeout) ++_timeout;
		}
		if (_pThread) _pThread->trySleep(_timeout);
		else Thread::sleep(_timeout);
	}
	catch (Exception& exc)
	{
		ErrorHandler::handle(exc);
	}
	catch (std::exception& exc)
	{
		ErrorHandler::handle(exc);
	}
	catch (...)
	{
		ErrorHandler::handle();
	}
}


void SocketProactor::run()
{
	_pThread = Thread::current();
	_ioCompletion.start();
	int handled = 0;
	if (!_isStopped) _stop = false;
	_isStopped = false;
	while (!_stop)
	{
		this->sleep(poll(&handled) || handled);
		_isRunning = true;
	}
	_isRunning = false;
	onShutdown();
}


bool SocketProactor::hasSocketHandlers() const
{
	if (_readHandlers.size() || _writeHandlers.size())
		return true;
	return false;
}


void SocketProactor::stop()
{
	// the reason for two flags is to prevent a race
	// when stop() is called before run() (which sets
	// stop to false before entering the polling loop
	// in order to allow multiple run()/stop() cycles)
	_stop = true;
	_isStopped = true;
}


void SocketProactor::wakeUp()
{
	if (_pThread) _pThread->wakeUp();
}


void SocketProactor::setTimeout(const Poco::Timespan& timeout)
{
	_timeout = static_cast<long>(timeout.totalMilliseconds());
}


Poco::Timespan SocketProactor::getTimeout() const
{
	return _maxTimeout;
}


Worker& SocketProactor::worker()
{
	poco_check_ptr(_pWorker);
	return *_pWorker;
}


void SocketProactor::addWork(const Work& ch, Timestamp::TimeDiff ms)
{
	worker().addWork(Work(ch), ms);
}


void SocketProactor::addWork(Work&& ch, Timestamp::TimeDiff ms, int pos)
{
	worker().addWork(std::move(ch), ms, pos);
}


void SocketProactor::removeWork()
{
	worker().removeWork();
}


int SocketProactor::scheduledWork()
{
	return worker().scheduledWork();
}


int SocketProactor::removeScheduledWork(int count)
{
	return worker().removeScheduledWork(count);
}


int SocketProactor::permanentWork()
{
	return worker().permanentWork();
}


int SocketProactor::removePermanentWork(int count)
{
	return worker().removePermanentWork(count);
}


bool SocketProactor::has(const Socket& sock) const
{
	return _pollSet.has(sock);
}


bool SocketProactor::ioCompletionInProgress() const
{
	return _ioCompletion.queueSize();
}


void SocketProactor::onShutdown()
{
	_pollSet.wakeUp();
	_ioCompletion.stop();
	_ioCompletion.wait();
}


void SocketProactor::deleteHandler(IOHandlerList& handlers, IOHandlerList::iterator& it)
{
	if ((*it)->_owner)
	{
		if ((*it)->_pBuf)
		{
			delete (*it)->_pBuf;
			(*it)->_pBuf = nullptr;
		}
		if ((*it)->_pAddr)
		{
			delete (*it)->_pAddr;
			(*it)->_pAddr = nullptr;
		}
	}
	++it;
	handlers.pop_front();
}


} } // namespace Poco::Net
