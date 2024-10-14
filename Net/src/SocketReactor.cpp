//
// SocketReactor.cpp
//
// Library: Net
// Package: Reactor
// Module:  SocketReactor
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketReactor.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Thread.h"
#include "Poco/Stopwatch.h"
#include "Poco/Exception.h"


using Poco::Exception;
using Poco::ErrorHandler;


namespace Poco {
namespace Net {


SocketReactor::SocketReactor(): _threadAffinity(-1),
	_stop(false),
	_pReadableNotification(new ReadableNotification(this)),
	_pWritableNotification(new WritableNotification(this)),
	_pErrorNotification(new ErrorNotification(this)),
	_pTimeoutNotification(new TimeoutNotification(this)),
	_pShutdownNotification(new ShutdownNotification(this))
{
}


SocketReactor::SocketReactor(const Poco::Timespan& pollTimeout, int threadAffinity):
	_threadAffinity(threadAffinity),
	_stop(false),
	_pReadableNotification(new ReadableNotification(this)),
	_pWritableNotification(new WritableNotification(this)),
	_pErrorNotification(new ErrorNotification(this)),
	_pTimeoutNotification(new TimeoutNotification(this)),
	_pShutdownNotification(new ShutdownNotification(this))
{
	_params.pollTimeout = pollTimeout;
}

SocketReactor::SocketReactor(const Params& params, int threadAffinity):
	_params(params),
	_threadAffinity(threadAffinity),
	_stop(false),
	_pReadableNotification(new ReadableNotification(this)),
	_pWritableNotification(new WritableNotification(this)),
	_pErrorNotification(new ErrorNotification(this)),
	_pTimeoutNotification(new TimeoutNotification(this)),
	_pShutdownNotification(new ShutdownNotification(this))
{

}


SocketReactor::~SocketReactor()
{
}


void SocketReactor::run()
{
	if (_threadAffinity >= 0)
	{
		Poco::Thread* pThread = Thread::current();
		if (pThread) pThread->setAffinity(_threadAffinity);
	}
	Poco::Stopwatch sw;
	if (_params.throttle) sw.start();
	PollSet::SocketModeMap sm;
	while (!_stop)
	{
		try
		{
			if (hasSocketHandlers())
			{
				sm = _pollSet.poll(_params.pollTimeout);
				if (_stop) break;
				for (const auto& s : sm)
				{
					try
					{
						if (s.second & PollSet::POLL_READ)
						{
							dispatch(s.first, _pReadableNotification);
						}
						if (s.second & PollSet::POLL_WRITE)
						{
							dispatch(s.first, _pWritableNotification);
						}
						if (s.second & PollSet::POLL_ERROR)
						{
							dispatch(s.first, _pErrorNotification);
						}
					}
					catch (Exception& exc)
					{
						onError(s.first, exc.code(), exc.displayText());
						ErrorHandler::handle(exc);
					}
					catch (std::exception& exc)
					{
						onError(s.first, 0, exc.what());
						ErrorHandler::handle(exc);
					}
					catch (...)
					{
						onError(s.first, 0, "unknown exception");
						ErrorHandler::handle();
					}
				}
				if (0 == sm.size())
				{
					onTimeout();
					if (_params.throttle && _params.pollTimeout == 0)
					{
						if ((sw.elapsed()/1000) > _params.sleepLimit) sleep();
					}
				}
				else if (_params.throttle) sw.restart();
			}
			else sleep();
		}
		catch (Exception& exc)
		{
			onError(exc.code(), exc.displayText());
			ErrorHandler::handle(exc);
		}
		catch (std::exception& exc)
		{
			onError(0, exc.what());
			ErrorHandler::handle(exc);
		}
		catch (...)
		{
			onError(0, "unknown exception");
			ErrorHandler::handle();
		}
	}
	onShutdown();
}


void SocketReactor::sleep()
{
	if (_params.sleep < _params.sleepLimit) ++_params.sleep;
	_event.tryWait(_params.sleep);
}


void SocketReactor::stop()
{
	if (_stop.exchange(true)) return;
	wakeUp();
}


void SocketReactor::wakeUp()
{
	_pollSet.wakeUp();
	_event.set();
}


bool SocketReactor::hasSocketHandlers()
{
	if (!_pollSet.empty())
	{
		ScopedLock lock(_mutex);
		for (auto& p: _handlers)
		{
			if (p.second->accepts(_pReadableNotification) ||
				p.second->accepts(_pWritableNotification) ||
				p.second->accepts(_pErrorNotification)) return true;
		}
	}

	return false;
}


void SocketReactor::addEventHandler(const Socket& socket, const Poco::AbstractObserver& observer)
{
	NotifierPtr pNotifier = getNotifier(socket, true);

	if (!pNotifier->hasObserver(observer)) pNotifier->addObserver(this, observer);

	int mode = 0;
	if (pNotifier->accepts(_pReadableNotification)) mode |= PollSet::POLL_READ;
	if (pNotifier->accepts(_pWritableNotification)) mode |= PollSet::POLL_WRITE;
	if (pNotifier->accepts(_pErrorNotification))    mode |= PollSet::POLL_ERROR;
	if (mode) _pollSet.add(socket, mode);
}


bool SocketReactor::hasEventHandler(const Socket& socket, const Poco::AbstractObserver& observer)
{
	NotifierPtr pNotifier = getNotifier(socket);
	if (!pNotifier) return false;
	if (pNotifier->hasObserver(observer)) return true;
	return false;
}


SocketReactor::NotifierPtr SocketReactor::getNotifier(const Socket& socket, bool makeNew)
{
	const SocketImpl* pImpl = socket.impl();
	if (pImpl == nullptr) return 0;
	poco_socket_t sockfd = pImpl->sockfd();
	ScopedLock lock(_mutex);

	EventHandlerMap::iterator it = _handlers.find(sockfd);
	if (it != _handlers.end()) return it->second;
	else if (makeNew) return (_handlers[sockfd] = new SocketNotifier(socket));

	return 0;
}


void SocketReactor::removeEventHandler(const Socket& socket, const Poco::AbstractObserver& observer)
{
	const SocketImpl* pImpl = socket.impl();
	if (pImpl == nullptr) return;
	NotifierPtr pNotifier = getNotifier(socket);
	if (pNotifier && pNotifier->hasObserver(observer))
	{
		if(pNotifier->countObservers() == 1)
		{
			{
				ScopedLock lock(_mutex);
				_handlers.erase(pImpl->sockfd());
			}
			_pollSet.remove(socket);
		}
		pNotifier->removeObserver(this, observer);

		if (pNotifier->countObservers() > 0 && socket.impl()->sockfd() > 0)
		{
			int mode = 0;
			if (pNotifier->accepts(_pReadableNotification)) mode |= PollSet::POLL_READ;
			if (pNotifier->accepts(_pWritableNotification)) mode |= PollSet::POLL_WRITE;
			if (pNotifier->accepts(_pErrorNotification))    mode |= PollSet::POLL_ERROR;
			_pollSet.update(socket, mode);
		}
	}
}


void SocketReactor::onTimeout()
{
	dispatch(_pTimeoutNotification);
}


void SocketReactor::onShutdown()
{
	dispatch(_pShutdownNotification);
}


void SocketReactor::dispatch(const Socket& socket, SocketNotification* pNotification)
{
	NotifierPtr pNotifier = getNotifier(socket);
	if (!pNotifier) return;
	dispatch(pNotifier, pNotification);
}


void SocketReactor::dispatch(SocketNotification* pNotification)
{
	std::vector<NotifierPtr> delegates;
	{
		ScopedLock lock(_mutex);
		delegates.reserve(_handlers.size());
		for (EventHandlerMap::iterator it = _handlers.begin(); it != _handlers.end(); ++it)
			delegates.push_back(it->second);
	}
	for (std::vector<NotifierPtr>::iterator it = delegates.begin(); it != delegates.end(); ++it)
	{
		dispatch(*it, pNotification);
	}
}


} } // namespace Poco::Net
