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


namespace Poco {


AsyncNotificationCenter::AsyncNotificationCenter(): _ra(*this, &AsyncNotificationCenter::dequeue),
	_started(false),
	_done(false)
{
	start();
}


AsyncNotificationCenter::~AsyncNotificationCenter()
{
	stop();
}


void AsyncNotificationCenter::postNotification(Notification::Ptr pNotification)
{
	_nq.enqueueNotification(pNotification);
}


int AsyncNotificationCenter::backlog() const
{
	return _nq.size();
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
}


void AsyncNotificationCenter::stop()
{
	if (!_started.exchange(false)) return;
	_nq.wakeUpAll();
	while (!_done) Thread::sleep(100);
	_thread.join();
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


} // namespace Poco
