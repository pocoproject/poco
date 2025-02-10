//
// AsyncObserver.h
//
// Library: Foundation
// Package: Notifications
// Module:  AsyncObserver
//
// Definition of the AsyncObserver class template.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering d.o.o.,
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_AsyncObserver_INCLUDED
#define Foundation_AsyncObserver_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/NObserver.h"
#include "Poco/Thread.h"
#include "Poco/Stopwatch.h"
#include "Poco/Debugger.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Format.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/NotificationQueue.h"


namespace Poco {


template <class C, class N>
class AsyncObserver: public NObserver<C, N>
	/// AsyncObserver notifies subscribers in a dedicated thread (as opposed
	/// to (N)Observer classes, which notify subscribers synchronously).
	/// In order to become active and process notifications, the start()
	/// method must be called.
	///
	/// This class is meant to be used with the NotificationCenter only.
	/// Notification processing thread can be started only once, and copying
	/// should be done before `start()` is called.
{
public:
	using Type = AsyncObserver<C, N>;
	using Matcher = typename NObserver<C, N>::Matcher;
	using Handler = typename NObserver<C, N>::Handler;
	using NotificationPtr = typename NObserver<C, N>::NotificationPtr;

	AsyncObserver() = delete;

	AsyncObserver(C& object, Handler handler, Matcher matcher = nullptr):
		NObserver<C, N>(object, handler, matcher),
		_ra(*this, &AsyncObserver::dequeue),
		_started(false),
		_done(false)
	{
	}

	AsyncObserver(const AsyncObserver& observer):
		NObserver<C, N>(observer),
		_ra(*this, &AsyncObserver::dequeue),
		_started(false),
		_done(false)
	{
		poco_assert(observer._nq.empty());
	}

	~AsyncObserver()
	{
		disable();
	}

	AsyncObserver& operator = (const AsyncObserver& observer)
	{
		if (&observer != this)
		{
			poco_assert(observer._nq.empty());
			setObject(observer._pObject);
			setHandler(observer._handler);
			setMatcher(observer._matcher);
			_started = false;
			_done =false;
		}
		return *this;
	}

	virtual void notify(Notification* pNf) const
	{
		_nq.enqueueNotification(NotificationPtr(static_cast<N*>(pNf), true));
	}

	virtual AbstractObserver* clone() const
	{
		return new AsyncObserver(*this);
	}

	virtual void start()
	{
		Poco::ScopedLock l(this->mutex());
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

	virtual void disable()
	{
		if (!_started.exchange(false)) return;
		_nq.wakeUpAll();
		while (!_done) Thread::sleep(100);
		_thread.join();
		NObserver<C, N>::disable();
	}

	virtual int backlog() const
	{
		return _nq.size();
	}

private:
	void dequeue()
	{
		Notification::Ptr pNf;
		_started = true;
		_done = false;
		while ((pNf = _nq.waitDequeueNotification()))
		{
			try
			{
				this->handle(pNf.unsafeCast<N>());
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

	using Adapter = RunnableAdapter<AsyncObserver<C, N>>;

	Thread _thread;
	mutable NotificationQueue _nq;
	Adapter _ra;
	std::atomic<bool> _started;
	std::atomic<bool> _done;
};


} // namespace Poco


#endif // Foundation_AsyncObserver_INCLUDED
