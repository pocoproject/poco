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
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ActiveObserver_INCLUDED
#define Foundation_ActiveObserver_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/NObserver.h"
#include "Poco/Thread.h"
#include "Poco/Stopwatch.h"
#include "Poco/Debugger.h"
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
	using Matcher = bool (C::*)(const std::string&);

	AsyncObserver() = delete;

	AsyncObserver(C& object, Handler handler, Matcher matcher = nullptr):
		NObserver(object, handler),
		_matcher(matcher),
		_ra(*this, &AsyncObserver::dequeue),
		_started(false),
		_done(false)
	{
	}

	AsyncObserver(const AsyncObserver& observer):
		NObserver(observer),
		_matcher(observer._matcher),
		_ra(*this, &AsyncObserver::dequeue),
		_started(false),
		_done(false)
	{
		poco_assert(observer._nq.size() == 0);
	}

	~AsyncObserver()
	{
		disable();
	}

	AsyncObserver& operator = (const AsyncObserver& observer)
	{
		if (&observer != this)
		{
			poco_assert(observer._nq.size() == 0);
			_pObject = observer._pObject;
			_handler  = observer._handler;
			_matcher = observer._matcher;
			_started = false;
			_done =false;
		}
		return *this;
	}

	void notify(Notification* pNf) const
	{
		Poco::ScopedLockWithUnlock l(_mutex);
		if (_pObject)
		{
			if (!_matcher || (_pObject->*_matcher)(pNf->name()))
			{
				l.unlock();
				N* pCastNf = dynamic_cast<N*>(pNf);
				if (pCastNf)
				{
					NotificationPtr ptr(pCastNf, true);
					_nq.enqueueNotification(ptr);
				}
			}
		}
	}

	virtual bool equals(const AbstractObserver& abstractObserver) const
	{
		const AsyncObserver* pObs = dynamic_cast<const AsyncObserver*>(&abstractObserver);
		return pObs && pObs->_pObject == _pObject && pObs->_method == _method && pObs->_matcher == _matcher;
	}

	virtual bool accepts(Notification* pNf, const char* pName = nullptr) const
	{
		if (!dynamic_cast<N*>(pNf)) return false;

		Poco::ScopedLock l(_mutex);
		return _pObject && _matcher && (_pObject->*_matcher)(pNf->name());
	}

	virtual AbstractObserver* clone() const
	{
		return new AsyncObserver(*this);
	}

	virtual void start()
	{
		Poco::ScopedLock l(_mutex);
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
		NObserver::disable();
	}

	virtual int backlog() const
	{
		return _nq.size();
	}

private:
	void dequeue()
	{
		NotificationPtr pNf;
		_started = true;
		while (pNf = _nq.waitDequeueNotification())
		{
			try
			{
				_mutex.lock();
				try
				{
					(_pObject->*_method)(pNf);
					_mutex.unlock();
				}
				catch (Poco::SystemException&) { break; }
				catch (...) { _mutex.unlock(); }
			} catch (Poco::SystemException&) {}
		}
		_done = true;
		_started = false;
	}

	using Adapter = RunnableAdapter<AsyncObserver<C, N>>;

	Thread _thread;
	Matcher _matcher;
	mutable NotificationQueue _nq;
	Adapter _ra;
	std::atomic<bool> _started;
	std::atomic<bool> _done;
};


} // namespace Poco


#endif // Foundation_ActiveObserver_INCLUDED
