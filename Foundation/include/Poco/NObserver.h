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
#include "Poco/AutoPtr.h"
#include "Poco/Mutex.h"

#include <atomic>
#include <functional>

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
	///
	/// Thread Safety:
	///
	/// The handle() method uses atomic load for _pObject and does not
	/// hold any lock during callback invocation. This prevents
	/// lock-order-inversion deadlocks when handlers call back into
	/// NotificationCenter (e.g., to remove themselves during disconnect).
	///
	/// Callers must ensure:
	///   - The observed object outlives all pending notifications.
	///     This is inherent to the observer pattern: if object C is
	///     destroyed while notifications are in flight (e.g., queued
	///     in AsyncObserver), undefined behavior occurs.
	///   - Assignment operators should not be used concurrently with
	///     notification delivery. Observer instances are typically
	///     created once and registered; concurrent modification during
	///     active notification dispatch is not supported.
{
public:
	using Type = NObserver<C, N>;
	using NotificationPtr = AutoPtr<N>;
	using Handler = void (C::*)(const NotificationPtr&);
	using SyncHandler = NotificationResult (C::*)(const NotificationPtr&);
	using Matcher = bool (C::*)(const std::string&) const;
	using MatcherFunc = std::function<bool(const std::string&)>;

	NObserver() = delete;

	NObserver(C& object, Handler method, Matcher matcher = nullptr, SyncHandler syncMethod = nullptr):
		_pObject(&object),
		_handler(method),
		_syncHandler(syncMethod),
		_matcher(matcher)
	{
	}

	NObserver(C& object, Handler method, MatcherFunc matcherFunc, SyncHandler syncMethod = nullptr):
		_pObject(&object),
		_handler(method),
		_syncHandler(syncMethod),
		_matcher(nullptr),
		_matcherFunc(matcherFunc)
	{
	}

	NObserver(const NObserver& observer):
		AbstractObserver(observer),
		_pObject(observer._pObject.load()),
		_handler(observer._handler),
		_syncHandler(observer._syncHandler),
		_matcher(observer._matcher),
		_matcherFunc(observer._matcherFunc)
	{
	}

	NObserver(NObserver&& observer):
		AbstractObserver(observer),
		_pObject(observer._pObject.exchange(nullptr)),
		_handler(std::move(observer._handler)),
		_syncHandler(std::move(observer._syncHandler)),
		_matcher(std::move(observer._matcher)),
		_matcherFunc(std::move(observer._matcherFunc))
	{
	}

	~NObserver() override = default;

	NObserver& operator = (const NObserver& observer)
	{
		if (&observer != this)
		{
			_pObject = observer._pObject;
			_handler = observer._handler;
			_syncHandler = observer._syncHandler;
			_matcher = observer._matcher;
			_matcherFunc = observer._matcherFunc;
		}
		return *this;
	}

	NObserver& operator = (NObserver&& observer)
	{
		if (&observer != this)
		{
			_pObject = observer._pObject.exchange(nullptr);
			_handler = std::move(observer._handler);
			_syncHandler = std::move(observer._syncHandler);
			_matcher = std::move(observer._matcher);
			_matcherFunc = std::move(observer._matcherFunc);
		}
		return *this;
	}

	void notify(Notification* pNf) const override
	{
		handle(NotificationPtr(static_cast<N*>(pNf), true));
	}

	NotificationResult notifySync(Notification* pNf) const override
	{
		return handleSync(NotificationPtr(static_cast<N*>(pNf), true));
	}

	bool equals(const AbstractObserver& abstractObserver) const override
	{
		const auto* pObs = dynamic_cast<const NObserver*>(&abstractObserver);
		return pObs && pObs->_pObject == _pObject && pObs->_handler == _handler;
	}

	POCO_DEPRECATED("use `bool accepts(const Notification::Ptr&)` instead with matcher function if needed")
	bool accepts(Notification* pNf, const char* pName) const override
	{
		return (!pName || pNf->name() == pName) && dynamic_cast<N*>(pNf) != nullptr;
	}

	bool accepts(const Notification::Ptr& pNf) const override
	{
		if (hasMatcher())
			return match(pNf);
		else
			return pNf.template cast<N>() != nullptr;
	}

	bool acceptsSync() const override
	{
		return _pObject != nullptr && _syncHandler != nullptr;
	}

	AbstractObserver* clone() const override
	{
		return new NObserver(*this);
	}

	void disable() override
	{
		_pObject = nullptr;
	}

protected:

	void handle(const NotificationPtr& ptr) const
	{
		// Read atomically, call handler without holding lock to avoid
		// lock-order-inversion deadlock with handlers that call back
		// into NotificationCenter (e.g., during disconnect handling)
		C* pObject = _pObject.load();
		if (pObject != nullptr)
			(pObject->*_handler)(ptr);
	}

	NotificationResult handleSync(const NotificationPtr& ptr) const
	{
		C* pObject = _pObject.load();
		SyncHandler syncHandler = _syncHandler;

		if (pObject == nullptr || syncHandler == nullptr)
			return {};

		return (pObject->*syncHandler)(ptr);
	}

	bool hasMatcher() const
	{
		return _pObject.load() != nullptr &&
			   (_matcher != nullptr || _matcherFunc != nullptr);
	}

	bool match(const Notification::Ptr& ptr) const
	{
		C* pObject = _pObject.load();
		if (pObject == nullptr)
			return false;

		if (_matcher)
			return (pObject->*_matcher)(ptr->name());

		if (_matcherFunc)
			return _matcherFunc(ptr->name());

		return false;
	}

private:
	std::atomic<C*> _pObject {nullptr};
	Handler _handler {nullptr};
	SyncHandler _syncHandler {nullptr};
	Matcher _matcher {nullptr};
	MatcherFunc _matcherFunc;
};


} // namespace Poco


#endif // Foundation_NObserver_INCLUDED
