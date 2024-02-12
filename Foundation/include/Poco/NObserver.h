//
// NObserver.h
//
// Library: Foundation
// Package: Notifications
// Module:  NObserver
//
// Definition of the NObserver class template.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NObserver_INCLUDED
#define Foundation_NObserver_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractObserver.h"
#include "Poco/Mutex.h"


namespace Poco {


template <class C, class N>
class NObserver: public AbstractObserver
	/// This template class implements an adapter that sits between
	/// a NotificationCenter and an object receiving notifications
	/// from it. It is quite similar in concept to the
	/// RunnableAdapter, but provides some NotificationCenter
	/// specific additional methods.
	/// See the NotificationCenter class for information on how
	/// to use this template class.
	///
	/// This class template is quite similar to the Observer class
	/// template. The differences are:
	///
	///   - NObserver expects the callback function to accept a const AutoPtr&
	///     instead of a plain pointer as argument, thus simplifying memory
	///     management.
	///
	///   - In addition to dispatching notifications based on the Notification runtime
	///     type, NObserver can also notify subscribers based on the Notification name.
	///     To enable this functionality, a matcher function must be provided.
	///     Null matcher means no matching is performed and all notificiations
	///     of the type subscribed to are dispatched.
{
public:
	using Type = NObserver<C, N>;
	using NotificationPtr = AutoPtr<N>;
	using Callback = void (C::*)(const NotificationPtr&);
	using Handler = Callback;
	using Matcher = bool (C::*)(const std::string&) const;

	NObserver() = delete;

	NObserver(C& object, Handler method, Matcher matcher = nullptr):
		_pObject(&object),
		_handler(method),
		_matcher(matcher)
	{
	}

	NObserver(const NObserver& observer):
		AbstractObserver(observer),
		_pObject(observer._pObject),
		_handler(observer._handler),
		_matcher(observer._matcher)
	{
	}

	~NObserver()
	{
	}

	NObserver& operator = (const NObserver& observer)
	{
		if (&observer != this)
		{
			_pObject = observer._pObject;
			_handler  = observer._handler;
			_matcher  = observer._matcher;
		}
		return *this;
	}

	virtual void notify(Notification* pNf) const
	{
		handle(NotificationPtr(static_cast<N*>(pNf), true));
	}

	virtual bool equals(const AbstractObserver& abstractObserver) const
	{
		const NObserver* pObs = dynamic_cast<const NObserver*>(&abstractObserver);
		return pObs && pObs->_pObject == _pObject && pObs->_handler == _handler && pObs->_matcher == _matcher;
	}

	[[deprecated("use `bool accepts(const Notification::Ptr&)` instead")]]
	virtual bool accepts(Notification* pNf, const char* pName) const
	{
		return (!pName || pNf->name() == pName) && dynamic_cast<N*>(pNf) != nullptr;
	}

	virtual bool accepts(const Notification::Ptr& pNf) const
	{
		return (match(pNf) && (pNf.template cast<N>() != nullptr));
	}

	virtual AbstractObserver* clone() const
	{
		return new NObserver(*this);
	}

	virtual void disable()
	{
		Poco::Mutex::ScopedLock lock(_mutex);

		_pObject = nullptr;
	}

protected:

	void handle(const NotificationPtr& ptr) const
	{
		Mutex::ScopedLock lock(_mutex);

		if (_pObject)
			(_pObject->*_handler)(ptr);
	}

	bool match(const Notification::Ptr& ptr) const
	{
		Mutex::ScopedLock l(_mutex);

		return _pObject && (!_matcher || (_pObject->*_matcher)(ptr->name()));
	}

	Mutex& mutex() const
	{
		return _mutex;
	}

private:
	C*       _pObject;
	Callback _handler;
	Matcher _matcher;
	mutable Poco::Mutex _mutex;
};


} // namespace Poco


#endif // Foundation_NObserver_INCLUDED
