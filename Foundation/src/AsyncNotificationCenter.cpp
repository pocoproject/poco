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
#include <vector>

namespace Poco {

#if (POCO_HAVE_JTHREAD)

AsyncNotificationCenter::AsyncNotificationCenter(AsyncMode mode, std::size_t workersCount) :
	_mode(mode),
	_ra(*this, &AsyncNotificationCenter::dequeue),
	_enqueueThreadStarted(false),
	_enqueueThreadDone(false),
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
#if (POCO_HAVE_JTHREAD)
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


std::vector<NotificationResult> AsyncNotificationCenter::synchronousDispatch(Notification::Ptr pNotification)
{
	poco_check_ptr (pNotification);

	auto observers = observersToNotify(pNotification);
	if (observers.empty())
		return {};

	std::vector<NotificationResult> results;
	results.reserve(observers.size());
	for (auto& o : observers)
	{
		if (!o->acceptsSynchronously())
			continue;

		results.push_back(o->notifySynchronously(pNotification));
	}

	return results;
}


void AsyncNotificationCenter::notifyObservers(Notification::Ptr& pNotification)
{
	poco_check_ptr (pNotification);

#if (POCO_HAVE_JTHREAD)

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
		_listsEmpty = false;
		_listsEmptyCondition.notify_all();
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

	if (_mode == AsyncMode::ENQUEUE || _mode == AsyncMode::BOTH)
	{
		if (_enqueueThreadStarted)
		{
			throw Poco::InvalidAccessException(
				Poco::format("thread already started %s", poco_src_loc));
		}
		_enqueueThread.start(_ra);
		Poco::Stopwatch sw;
		sw.start();
		while (!_enqueueThreadStarted)
		{
			if (sw.elapsedSeconds() > 5)
				throw Poco::TimeoutException(poco_src_loc);
			Thread::sleep(20);
		}
	}

#if (POCO_HAVE_JTHREAD)
	_workerIterator = _lists.begin();

	if (_mode == AsyncMode::NOTIFY || _mode == AsyncMode::BOTH)
	{
		auto dispatch = [this](std::stop_token stopToken, int id) {
			this->dispatchNotifications(stopToken, id);
		};

		for (std::size_t i {0}; i < _workersCount; ++i)
		{
			auto worker = std::jthread(dispatch, i);
			_workers.push_back(std::move(worker));
		}
	}
#endif
}


void AsyncNotificationCenter::stop()
{
	if (_mode == AsyncMode::ENQUEUE || _mode == AsyncMode::BOTH)
	{
		if (_enqueueThreadStarted.exchange(false))
		{
			_nq.wakeUpAll();
			while (!_enqueueThreadDone) Thread::sleep(100);
			_enqueueThread.join();
		}
	}

#if (POCO_HAVE_JTHREAD)
	for (auto& t: _workers)
	{
		t.request_stop();
	}

// TODO: Should the observer lists be cleared here or
//       shall the workers send all of them to observers and then finish?
#endif
}


void AsyncNotificationCenter::dequeue()
{
	Notification::Ptr pNf;
	_enqueueThreadStarted = true;
	_enqueueThreadDone = false;
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
	_enqueueThreadDone = true;
	_enqueueThreadStarted = false;
}

#if (POCO_HAVE_JTHREAD)

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
		if (!hasObserver(*o))
		{
			// Observer is not registered anymore, remove its list
			_workerIterator = _lists.erase(_workerIterator);
			continue;
		}
		auto n = l.front();
		l.pop_front();
		return { {o, n} };
	}
	_listsEmpty = true;
	return {};
}

void AsyncNotificationCenter::dispatchNotifications(std::stop_token& stopToken, int workerId)
{
	while (!stopToken.stop_requested())
	{
		// get next observer and notification
		auto no = nextNotification();
		if (no.has_value())
		{
			auto [o, n] = *no;
			o->notify(n);
			continue;
		}
		// No notification available, wait for a while
		std::unique_lock<std::mutex> lock(_listsMutex);
		_listsEmptyCondition.wait_for(
			lock, 1s,
			[&stopToken, this] { return !_listsEmpty || stopToken.stop_requested(); }
		);
		if (stopToken.stop_requested())
			break;
	}
}

#endif

} // namespace Poco
