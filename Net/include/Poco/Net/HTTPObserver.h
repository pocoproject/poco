//
// HTTPObserver.h
//
// Library: Foundation
// Package: Net
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


#include "Poco/AbstractObserver.h"
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
	/// Note: Despite the similar naming, HTTPObserver is NOT related to the
	/// deprecated Poco::Observer class. HTTPObserver derives directly from
	/// AbstractObserver (like NObserver) and does not trigger deprecation warnings.
	///
	/// HTTPObserver vs NObserver:
	///
	///   - Both use AutoPtr<N> for automatic notification memory management
	///
	///   - NObserver takes a reference (C&) - observer does not own the object
	///
	///   - HTTPObserver takes a shared_ptr<C> - observer shares object ownership,
	///     suitable for cases where the observed object may be destroyed while
	///     still registered (e.g., connection objects using shared_from_this())
	///
	/// HTTPObserver cannot use the addNObserver() convenience method because
	/// it requires shared_ptr ownership semantics. Instead, construct the
	/// HTTPObserver explicitly and register it directly:
	///
	///     reactor.addEventHandler(socket,
	///         HTTPObserver<MyClass, ReadableNotification>(
	///             shared_from_this(), &MyClass::onReadable));
	///
	/// Or with NotificationCenter:
	///
	///     nc.addObserver(
	///         HTTPObserver<MyClass, MyNotification>(
	///             sharedPtr, &MyClass::handleNotification));
	///
	/// The addObserver(const AbstractObserver&) overload accepts any
	/// AbstractObserver subclass and does not trigger deprecation warnings.
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

	[[nodiscard]]
	virtual bool equals(const AbstractObserver& abstractObserver) const
	{
		const HTTPObserver* pObs = dynamic_cast<const HTTPObserver*>(&abstractObserver);
		return pObs && pObs->_pObject == _pObject && pObs->_handler == _handler && pObs->_matcher == _matcher;
	}

	POCO_DEPRECATED("use `bool accepts(const Notification::Ptr&)` instead")
	[[nodiscard]]
	virtual bool accepts(Notification* pNf, const char* pName) const
	{
		return (!pName || pNf->name() == pName) && dynamic_cast<N*>(pNf) != nullptr;
	}

	[[nodiscard]]
	virtual bool accepts(const Notification::Ptr& pNf) const
	{
		return (match(pNf) && (pNf.template cast<N>() != nullptr));
	}

	[[nodiscard]]
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

	[[nodiscard]]
	bool match(const Notification::Ptr& ptr) const
	{
		Mutex::ScopedLock l(_mutex);

		return _pObject && (!_matcher || (_pObject.get()->*_matcher)(ptr->name()));
	}

	[[nodiscard]]
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

