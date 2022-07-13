//
// SocketObserver.h
//
// Library: Foundation
// Package: Notifications
// Module:  NotificationCenter
//
// Definition of the SocketObserver class template.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_SocketObserver_INCLUDED
#define Foundation_SocketObserver_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractObserver.h"
#include "Poco/Mutex.h"
#include "Poco/SharedPtr.h"
#include <unordered_map>


namespace Poco {

namespace Net {

extern std::unordered_map<void*, AtomicCounter> _counter;

template <class C, class N>
class SocketObserver: public AbstractObserver
	/// This template class implements an adapter that sits between
	/// a NotificationCenter and an object receiving notifications
	/// from it. It is quite similar in concept to the
	/// RunnableAdapter, but provides some NotificationCenter
	/// specific additional methods.
	/// See the NotificationCenter class for information on how
	/// to use this template class.
	///
	/// This class template is quite similar to the NObserver class
	/// template. The only difference is that the SocketObserver
	/// manage _pObject lifecycle.
	/// TODO use SharedPtr instead of AutoPtr
{
public:
	typedef AutoPtr<N> NotificationPtr;
	typedef void (C::*Callback)(const NotificationPtr&);

	SocketObserver(C& object, Callback method):
		_pObject(&object),
		_method(method)
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		if (_counter.find(_pObject) != _counter.end())
			_counter[_pObject]++;
		else
			_counter.emplace(_pObject, 1);
	}

	SocketObserver(const SocketObserver& observer):
		AbstractObserver(observer),
		_pObject(observer._pObject),
		_method(observer._method)
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		_counter[_pObject]++;
	}

	~SocketObserver()
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		if (_counter.find(_pObject) != _counter.end())
		{
			if (_counter[_pObject] > 0)
				_counter[_pObject]--;

			if (_counter[_pObject] == 0)
			{
				_counter.erase(_pObject);
				delete _pObject;
				_pObject = nullptr;
			}
		}
	}

	SocketObserver& operator = (const SocketObserver& observer)
	{
		if (&observer != this)
		{
			Poco::Mutex::ScopedLock lock(_mutex);
			if (_pObject != observer._pObject)
			{
				_counter[_pObject]--;
				_counter[observer._pObject]++;
				if (_counter[_pObject] == 0)
				{
					_counter.erase(_pObject);
					if (_pObject)
					{
						delete _pObject;
						_pObject = nullptr;
					}
				}
			}
			_pObject = observer._pObject;
			_method  = observer._method;
		}
		return *this;
	}

	void notify(Notification* pNf) const
	{
		Poco::Mutex::ScopedLock lock(_mutex);

		if (!disabled && _pObject)
		{
			N* pCastNf = dynamic_cast<N*>(pNf);
			if (pCastNf)
			{
				NotificationPtr ptr(pCastNf, true);
				(_pObject->*_method)(ptr);
			}
		}
	}

	bool equals(const AbstractObserver& abstractObserver) const
	{
		const SocketObserver* pObs = dynamic_cast<const SocketObserver*>(&abstractObserver);
		return pObs && pObs->_pObject == _pObject && pObs->_method == _method;
	}

	bool accepts(Notification* pNf, const char* pName = 0) const
	{
		return dynamic_cast<N*>(pNf) && (!pName || pNf->name() == pName);
	}

	AbstractObserver* clone() const
	{
		return new SocketObserver(*this);
	}

	void disable()
	{
		Poco::Mutex::ScopedLock lock(_mutex);

		disabled = true;
	}

private:
	SocketObserver();

	C*       _pObject;
	Callback _method;
	mutable Poco::Mutex _mutex;

	bool disabled = false;
};

}

} // namespace Poco


#endif // Foundation_SocketObserver_INCLUDED

