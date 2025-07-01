//
// Observer.h
//
// Library: Foundation
// Package: Notifications
// Module:  NotificationCenter
//
// Definition of the Observer class template.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Observer_INCLUDED
#define Foundation_Observer_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractObserver.h"
#include "Poco/Mutex.h"


namespace Poco {


template <class C, class N>
class POCO_DEPRECATED("use `NObserver` instead") Observer: public AbstractObserver
	/// This template class implements an adapter that sits between
	/// a NotificationCenter and an object receiving notifications
	/// from it. It is quite similar in concept to the
	/// RunnableAdapter, but provides some NotificationCenter
	/// specific additional methods.
	/// See the NotificationCenter class for information on how
	/// to use this template class.
	///
	/// Instead of the Observer class template, you might want to
	/// use the NObserver class template, which uses an AutoPtr to
	/// pass the Notification to the callback function, thus freeing
	/// you from memory management issues.
{
public:
	using Callback = void (C::*)(N *);

	Observer(C& object, Callback method):
		_pObject(&object),
		_method(method)
	{
	}

	Observer(const Observer& observer):
		AbstractObserver(observer),
		_pObject(observer._pObject),
		_method(observer._method)
	{
	}

	~Observer() override = default;

	Observer() = delete;

	Observer& operator = (const Observer& observer)
	{
		if (&observer != this)
		{
			_pObject = observer._pObject;
			_method  = observer._method;
		}
		return *this;
	}

	void notify(Notification *pNf) const override
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		if (_pObject)
		{
			pNf->duplicate();
			(_pObject->*_method)(static_cast<N*>(pNf));
		}
	}

	bool equals(const AbstractObserver& abstractObserver) const override
	{
		const Observer* pObs = dynamic_cast<const Observer*>(&abstractObserver);
		return pObs && pObs->_pObject == _pObject && pObs->_method == _method;
	}

	POCO_DEPRECATED("use `bool accepts(const Notification::Ptr&)` instead")
	bool accepts(Notification* pNf, const char* pName) const override
	{
		return (!pName || pNf->name() == pName) && (dynamic_cast<N*>(pNf) != nullptr);
	}

	bool accepts(const Notification::Ptr& pNf) const override
	{
		return (pNf.cast<N>() != nullptr);
	}

	AbstractObserver *clone() const override
	{
		return new Observer(*this);
	}

	void disable() override
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		_pObject = nullptr;
	}

private:
	C*       _pObject;
	Callback _method;
	mutable Poco::Mutex _mutex;
};


} // namespace Poco


#endif // Foundation_Observer_INCLUDED
