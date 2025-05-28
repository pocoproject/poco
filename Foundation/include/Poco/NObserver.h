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
{
public:
	using Type = NObserver<C, N>;
	using NotificationPtr = AutoPtr<N>;
	using Handler = void (C::*)(const NotificationPtr&);
	using Matcher = bool (C::*)(const std::string&) const;
	using MatcherFunc = std::function<bool(const std::string&)>;

	NObserver() = delete;

	NObserver(C& object, Handler method, Matcher matcher = nullptr):
		_pObject(&object),
		_handler(method)
	{
		if (matcher)
		{
			_matcherFunc = [this, matcher](const std::string& s) -> bool
			{
				if (!_pObject)
					return false;
				return (_pObject->*_matcher)(s);
			};
		}
	}

	NObserver(C& object, Handler method, MatcherFunc matcherFunc):
		_pObject(&object),
		_handler(method),
		_matcherFunc(matcherFunc)
	{
	}

	NObserver(const NObserver& observer):
		AbstractObserver(observer),
		_pObject(observer._pObject),
		_handler(observer._handler),
		_matcherFunc(observer._matcherFunc)
	{
	}

	NObserver(NObserver&& observer):
		AbstractObserver(observer),
		_pObject(std::move(observer._pObject)),
		_handler(std::move(observer._handler)),
		_matcherFunc(std::move(observer._matcherFunc))
	{
		std::cout << "NOBserver moved" << std::endl;
	}

	~NObserver() override = default;

	NObserver& operator = (const NObserver& observer)
	{
		if (&observer != this)
		{
			_pObject = observer._pObject;
			_handler = observer._handler;
			_matcherFunc = observer._matcherFunc;
		}
		return *this;
	}

	NObserver& operator = (NObserver&& observer)
	{
		if (&observer != this)
		{
			_pObject = std::move(observer._pObject);
			_handler = std::move(observer._handler);
			_matcherFunc = std::move(observer._matcherFunc);
		}
		std::cout << "NOBserver move assigned" << std::endl;
		return *this;
	}

	void notify(Notification* pNf) const override
	{
		handle(NotificationPtr(static_cast<N*>(pNf), true));
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

	AbstractObserver* clone() const override
	{
		return new NObserver(*this);
	}

	void disable() override
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

	bool hasMatcher() const
	{
		return _pObject && _matcherFunc != nullptr;
	}

	bool match(const Notification::Ptr& ptr) const
	{
		Mutex::ScopedLock l(_mutex);		
		return _matcherFunc(ptr->name());
	}

	Mutex& mutex() const
	{
		return _mutex;
	}

private:
	C*       _pObject {nullptr};
	Handler _handler {nullptr};
	SyncHandler _syncHandler {nullptr};
	MatcherFunc _matcherFunc;

	mutable Poco::Mutex _mutex;
};


} // namespace Poco


#endif // Foundation_NObserver_INCLUDED
