//
// AsyncNotificationCenter.cpp
//
// Library: Foundation
// Package: Notifications
// Module:  AsyncNotificationCenter
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering d.o.o.,
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AsyncNotificationCenter.h"

#include "Poco/AbstractObserver.h"
#include "Poco/Stopwatch.h"
#include "Poco/Debugger.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Format.h"

namespace Poco {

#if (POCO_HAVE_CPP20_COMPILER)

AsyncNotificationCenter::AsyncNotificationCenter(AsyncMode mode, std::size_t workersCount) :
	_mode(mode),
	_ra(*this, &AsyncNotificationCenter::dequeue),
	_started(false),
	_done(false),
	_workersCount(workersCount)
{
	start();
}

#else

AsyncNotificationCenter::AsyncNotificationCenter() :
	_ra(*this, &AsyncNotificationCenter::dequeue),
	_started(false),
	_done(false)
{
	start();
}

#endif

AsyncNotificationCenter::~AsyncNotificationCenter()
{
	stop();
}


void AsyncNotificationCenter::postNotification(Notification::Ptr pNotification)
{
#if (POCO_HAVE_CPP20_COMPILER)
	if (_mode == AsyncMode::ENQUEUE || _mode == AsyncMode::BOTH)
	{
		_nq.enqueueNotification(pNotification);
	}
	else
	{
		// Notification enqueue is synchronous
		notifyObservers(pNotification);
	}
#else
	_nq.enqueueNotification(pNotification);
#endif
}


int AsyncNotificationCenter::backlog() const
{
	return _nq.size();
}

void AsyncNotificationCenter::notifyObservers(Notification::Ptr& pNotification)
{
	poco_check_ptr (pNotification);

#if (POCO_HAVE_CPP20_COMPILER)

	if (_mode == AsyncMode::NOTIFY || _mode == AsyncMode::BOTH)
	{
		// Notification is asynchronous, add it to the lists
		// for appropriate observers.
		std::unique_lock<std::mutex> lock(_listsMutex);
		auto observers = observersToNotify(pNotification);
		if (observers.empty())
			return;
		for (auto& o : observers)
		{
			auto& list = _lists[o];
			list.push_back(pNotification);
		}
		_listsCondition.notify_all();
	}
	else
	{
		// Notification is synchronous
		NotificationCenter::notifyObservers(pNotification);
	}
#else
	NotificationCenter::notifyObservers(pNotification);
#endif
}


void AsyncNotificationCenter::start()
{
	Poco::ScopedLock l(mutex());
	if (_started)
	{
		throw Poco::InvalidAccessException(
			Poco::format("thread already started %s", poco_src_loc));
	}
	_thread.start(_ra);
	Poco::Stopwatch sw;
	sw.start();
	while (!_started)
	{
		if (sw.elapsedSeconds() > 5)
			throw Poco::TimeoutException(poco_src_loc);
		Thread::sleep(100);
	}

#if (POCO_HAVE_CPP20_COMPILER)
	_workerIterator = _lists.begin();

	auto dispatch = [this](std::stop_token stopToken) {
		this->dispatchNotifications(stopToken);
	};

	for (std::size_t i {0}; i < _workersCount; ++i)
	{
		auto worker = std::jthread(dispatch);
		_workers.push_back(std::move(worker));
	}
#endif
}


void AsyncNotificationCenter::stop()
{
	if (!_started.exchange(false)) return;
	_nq.wakeUpAll();
	while (!_done) Thread::sleep(100);
	_thread.join();

#if (POCO_HAVE_CPP20_COMPILER)
	for (auto& t: _workers)
	{
		t.request_stop();
	}
#endif
}


void AsyncNotificationCenter::dequeue()
{
	Notification::Ptr pNf;
	_started = true;
	_done = false;
	while ((pNf = _nq.waitDequeueNotification()))
	{
		try
		{
			notifyObservers(pNf);
		}
		catch (Poco::Exception& ex)
		{
			Poco::ErrorHandler::handle(ex);
		}
		catch (std::exception& ex)
		{
			Poco::ErrorHandler::handle(ex);
		}
		catch (...)
		{
			Poco::ErrorHandler::handle();
		}
	}
	_done = true;
	_started = false;
}

#if (POCO_HAVE_CPP20_COMPILER)

std::optional<AsyncNotificationCenter::NotificationTuple> AsyncNotificationCenter::nextNotification()
{
	std::unique_lock<std::mutex> lock(_listsMutex);

	for (std::size_t i {0}; i < _lists.size(); ++i)
	{
		if (_lists.empty())
			break;

		if (_workerIterator == _lists.end())
			_workerIterator = _lists.begin();

		auto& o = _workerIterator->first;
		auto& l = _workerIterator->second;
		if (l.empty())
		{
			++_workerIterator;
			continue;
		};
		if (!hasObserver(*o)) {
			// Observer is not registered anymore, remove its list
			_workerIterator = _lists.erase(_workerIterator);
			continue;
		}
		auto n = l.front();
		l.pop_front();
		return { {o, n} };
	}
	return {};
}

void AsyncNotificationCenter::dispatchNotifications(std::stop_token& stopToken)
{
	while (!stopToken.stop_requested())
	{
		// get next observer and notification
		auto no = nextNotification();
		if (no.has_value())
		{
			auto [a, n] = *no;
			a->notify(n);
			continue;
		}
		// No notification available, wait for a while
		std::unique_lock<std::mutex> lock(_listsMutex);
		_listsCondition.wait_for(
			lock, 1s,
			[&stopToken] { return stopToken.stop_requested(); }
		);
		if (stopToken.stop_requested())
			break;
	}
}

#endif

} // namespace Poco
