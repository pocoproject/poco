//
// PriorityStrategy.h
//
// Library: Foundation
// Package: Events
// Module:  PrioritytStrategy
//
// Implementation of the DefaultStrategy template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_PriorityStrategy_INCLUDED
#define Foundation_PriorityStrategy_INCLUDED


#include "Poco/NotificationStrategy.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace Poco {


template <class TArgs, class TDelegate>
class PriorityStrategy: public NotificationStrategy<TArgs, TDelegate>
	/// NotificationStrategy for PriorityEvent.
	///
	/// Delegates are kept in a std::vector<>, ordered
	/// by their priority.
{
public:
	using DelegateHandle = TDelegate*;
	using DelegatePtr = SharedPtr<TDelegate>;
	using Delegates = std::vector<DelegatePtr>;
	using Iterator = typename Delegates::iterator;

public:
	PriorityStrategy() = default;
	PriorityStrategy(const PriorityStrategy& s) = default;
	PriorityStrategy(PriorityStrategy&& s) = default;
	~PriorityStrategy() = default;

	void notify(const void* sender, TArgs& arguments)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			(*it)->notify(sender, arguments);
		}
	}

	DelegateHandle add(const TDelegate& delegate)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if ((*it)->priority() > delegate.priority())
			{
				DelegatePtr pDelegate(static_cast<TDelegate*>(delegate.clone()));
				_delegates.insert(it, pDelegate);
				return pDelegate.get();
			}
		}
		DelegatePtr pDelegate(static_cast<TDelegate*>(delegate.clone()));
		_delegates.push_back(pDelegate);
		return pDelegate.get();
	}

	void remove(const TDelegate& delegate)
	{
		DelegatePtr p = detach(delegate);
		if (p) p->disable();
	}

	void remove(DelegateHandle delegateHandle)
	{
		DelegatePtr p = detach(delegateHandle);
		if (p) p->disable();
	}

	DelegatePtr detach(const TDelegate& delegate)
		/// Removes a matching delegate from the internal list without
		/// disabling it. See DefaultStrategy::detach.
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if (delegate.equals(**it))
			{
				DelegatePtr p = *it;
				_delegates.erase(it);
				return p;
			}
		}
		return DelegatePtr();
	}

	DelegatePtr detach(DelegateHandle delegateHandle)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if (*it == delegateHandle)
			{
				DelegatePtr p = *it;
				_delegates.erase(it);
				return p;
			}
		}
		return DelegatePtr();
	}

	Delegates detachAll()
	{
		Delegates out;
		out.swap(_delegates);
		return out;
	}

	PriorityStrategy& operator = (const PriorityStrategy& s)
	{
		if (this != &s)
		{
			_delegates = s._delegates;
		}
		return *this;
	}

	void clear()
	{
		Delegates detached = detachAll();
		for (Iterator it = detached.begin(); it != detached.end(); ++it)
		{
			(*it)->disable();
		}
	}

	[[nodiscard]]
	bool empty() const
	{
		return _delegates.empty();
	}

protected:
	Delegates _delegates;
};


template <class TDelegate>
class PriorityStrategy<void, TDelegate>
	/// NotificationStrategy for PriorityEvent.
	///
	/// Delegates are kept in a std::vector<>, ordered
	/// by their priority.
{
public:
	using DelegateHandle = TDelegate*;
	using DelegatePtr = SharedPtr<TDelegate>;
	using Delegates = std::vector<DelegatePtr>;
	using Iterator = typename Delegates::iterator;

public:
	PriorityStrategy() = default;

	PriorityStrategy(const PriorityStrategy& s):
		_delegates(s._delegates)
	{
	}

	~PriorityStrategy() = default;

	void notify(const void* sender)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			(*it)->notify(sender);
		}
	}

	DelegateHandle add(const TDelegate& delegate)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if ((*it)->priority() > delegate.priority())
			{
				DelegatePtr pDelegate(static_cast<TDelegate*>(delegate.clone()));
				_delegates.insert(it, pDelegate);
				return pDelegate.get();
			}
		}
		DelegatePtr pDelegate(static_cast<TDelegate*>(delegate.clone()));
		_delegates.push_back(pDelegate);
		return pDelegate.get();
	}

	void remove(const TDelegate& delegate)
	{
		DelegatePtr p = detach(delegate);
		if (p) p->disable();
	}

	void remove(DelegateHandle delegateHandle)
	{
		DelegatePtr p = detach(delegateHandle);
		if (p) p->disable();
	}

	DelegatePtr detach(const TDelegate& delegate)
		/// See the TArgs specialization above.
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if (delegate.equals(**it))
			{
				DelegatePtr p = *it;
				_delegates.erase(it);
				return p;
			}
		}
		return DelegatePtr();
	}

	DelegatePtr detach(DelegateHandle delegateHandle)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if (*it == delegateHandle)
			{
				DelegatePtr p = *it;
				_delegates.erase(it);
				return p;
			}
		}
		return DelegatePtr();
	}

	Delegates detachAll()
	{
		Delegates out;
		out.swap(_delegates);
		return out;
	}

	PriorityStrategy& operator = (const PriorityStrategy& s)
	{
		if (this != &s)
		{
			_delegates = s._delegates;
		}
		return *this;
	}

	void clear()
	{
		Delegates detached = detachAll();
		for (Iterator it = detached.begin(); it != detached.end(); ++it)
		{
			(*it)->disable();
		}
	}

	[[nodiscard]]
	bool empty() const
	{
		return _delegates.empty();
	}

protected:
	Delegates _delegates;
};


} // namespace Poco


#endif // Foundation_PriorityStrategy_INCLUDED
