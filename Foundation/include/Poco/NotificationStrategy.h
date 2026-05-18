//
// NotificationStrategy.h
//
// Library: Foundation
// Package: Events
// Module:  NotificationStrategy
//
// Definition of the NotificationStrategy interface.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NotificationStrategy_INCLUDED
#define Foundation_NotificationStrategy_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace Poco {


template <class TArgs, class TDelegate>
class NotificationStrategy
	/// The interface that all notification strategies must implement.
	///
	/// Note: Event is based on policy-driven design, so every strategy implementation
	/// must provide all the methods from this interface (otherwise: compile errors)
	/// but does not need to inherit from NotificationStrategy.
{
public:
	using DelegateHandle = TDelegate*;
	using DelegatePtr    = SharedPtr<TDelegate>;
	using Delegates      = std::vector<DelegatePtr>;

	NotificationStrategy() = default;

	virtual ~NotificationStrategy() = default;

	virtual void notify(const void* sender, TArgs& arguments) = 0;
		/// Sends a notification to all registered delegates.

	virtual DelegateHandle add(const TDelegate& delegate) = 0;
		/// Adds a delegate to the strategy.

	virtual void remove(const TDelegate& delegate) = 0;
		/// Removes a delegate from the strategy, if found.
		/// Does nothing if the delegate has not been added.

	virtual void remove(DelegateHandle delegateHandle) = 0;
		/// Removes a delegate from the strategy, if found.
		/// Does nothing if the delegate has not been added.

	virtual DelegatePtr detach(const TDelegate& delegate) = 0;
		/// Removes a matching delegate from the strategy without disabling
		/// it, and returns the detached delegate (or an empty DelegatePtr
		/// if no match). The caller is responsible for calling disable()
		/// on the returned delegate. AbstractEvent uses this to avoid
		/// holding the event mutex across Delegate::disable() — see
		/// AbstractEvent::operator -= for the lock-order rationale.

	virtual DelegatePtr detach(DelegateHandle delegateHandle) = 0;
		/// Same as detach(const TDelegate&), keyed by handle.

	virtual Delegates detachAll() = 0;
		/// Removes all delegates from the strategy without disabling them
		/// and returns them, so the caller can disable() each outside its
		/// own mutex. AbstractEvent::clear() uses this for the same reason
		/// detach() exists.

	virtual void clear() = 0;
		/// Removes all delegates from the strategy.

	virtual bool empty() const = 0;
		/// Returns false if the strategy contains at least one delegate.
};


template <class TDelegate>
class NotificationStrategy<void, TDelegate>
	/// The interface that all notification strategies must implement.
	///
	/// Note: Event is based on policy-driven design, so every strategy implementation
	/// must provide all the methods from this interface (otherwise: compile errors)
	/// but does not need to inherit from NotificationStrategy.
{
public:
	using DelegateHandle = TDelegate*;
	using DelegatePtr    = SharedPtr<TDelegate>;
	using Delegates      = std::vector<DelegatePtr>;

	NotificationStrategy() = default;

	virtual ~NotificationStrategy() = default;

	virtual void notify(const void* sender) = 0;
		/// Sends a notification to all registered delegates.

	virtual DelegateHandle add(const TDelegate& delegate) = 0;
		/// Adds a delegate to the strategy.

	virtual void remove(const TDelegate& delegate) = 0;
		/// Removes a delegate from the strategy, if found.
		/// Does nothing if the delegate has not been added.

	virtual void remove(DelegateHandle delegateHandle) = 0;
		/// Removes a delegate from the strategy, if found.
		/// Does nothing if the delegate has not been added.

	virtual DelegatePtr detach(const TDelegate& delegate) = 0;
		/// See the TArgs specialization above.

	virtual DelegatePtr detach(DelegateHandle delegateHandle) = 0;
		/// See the TArgs specialization above.

	virtual Delegates detachAll() = 0;
		/// See the TArgs specialization above.

	virtual void clear() = 0;
		/// Removes all delegates from the strategy.

	virtual bool empty() const = 0;
		/// Returns false if the strategy contains at least one delegate.
};


} // namespace Poco


#endif // Foundation_NotificationStrategy_INCLUDED
