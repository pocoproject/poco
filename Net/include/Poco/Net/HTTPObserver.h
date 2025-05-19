//
// HTTPObserver.h
//
// Library: Foundation
// Package: Notifications
// Module:  HTTPObserver
//
// Definition of the HTTPObserver class template.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//
#ifndef Net_HTTPObserver_INCLUDED
#define Net_HTTPObserver_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractObserver.h"
#include "Poco/Logger.h"
#include "Poco/Mutex.h"
#include <memory>


namespace Poco {
namespace Net {
	
	
template <class C, class N>
class HTTPObserver: public AbstractObserver
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
	///   - HTTPObserver expects the callback function to accept a const AutoPtr&
	///     instead of a plain pointer as argument, thus simplifying memory
	///     management.
	///
	///   - In addition to dispatching notifications based on the Notification runtime
	///     type, HTTPObserver can also notify subscribers based on the Notification name.
	///     To enable this functionality, a matcher function must be provided.
	///     Null matcher means no matching is performed and all notificiations
	///     of the type subscribed to are dispatched.
{
public:
	using Type = HTTPObserver<C, N>;
	using SharedPtrC = std::shared_ptr<C>;
	using NotificationPtr = AutoPtr<N>;
	using Callback = void (C::*)(const NotificationPtr&);
	using Handler = Callback;
	using Matcher = bool (C::*)(const std::string&) const;

	HTTPObserver() = delete;

	HTTPObserver(SharedPtrC object, Handler method, Matcher matcher = nullptr):
		_pObject(object),
		_handler(method),
		_matcher(matcher)
	{
	}
	
	HTTPObserver(const HTTPObserver& observer):
	AbstractObserver(observer),
	_pObject(observer._pObject),
	_handler(observer._handler),
	_matcher(observer._matcher)
	{
	}
	
	~HTTPObserver()
	{
	}

	HTTPObserver& operator = (const HTTPObserver& observer)
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
		const HTTPObserver* pObs = dynamic_cast<const HTTPObserver*>(&abstractObserver);
		return pObs && pObs->_pObject == _pObject && pObs->_handler == _handler && pObs->_matcher == _matcher;
	}

	POCO_DEPRECATED("use `bool accepts(const Notification::Ptr&)` instead")
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
		return new HTTPObserver(*this);
	}

	virtual void disable()
	{
		// do nothing
	}

protected:

	void handle(const NotificationPtr& ptr) const
	{
		Mutex::ScopedLock lock(_mutex);

		if (_pObject)
			(_pObject.get()->*_handler)(ptr);
	}

	bool match(const Notification::Ptr& ptr) const
	{
		Mutex::ScopedLock l(_mutex);

		return _pObject && (!_matcher || (_pObject.get()->*_matcher)(ptr->name()));
	}

	Mutex& mutex() const
	{
		return _mutex;
	}

private:
	SharedPtrC _pObject;
	Callback _handler;
	Matcher _matcher;
	mutable Poco::Mutex _mutex;
};

}} // namespace Poco:Net


#endif // Net_HTTPObserver_INCLUDED

