//
// FunctionPriorityDelegate.h
//
// Library: Foundation
// Package: Events
// Module:  FunctionPriorityDelegate
//
// Implementation of the FunctionPriorityDelegate template.
//
// Copyright (c) 2006-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FunctionPriorityDelegate_INCLUDED
#define Foundation_FunctionPriorityDelegate_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractPriorityDelegate.h"
#include "Poco/Mutex.h"
#include <type_traits>


namespace Poco {


namespace Detail {

/// Helper to select the correct function pointer type based on template parameters.
template <class TArgs, bool hasSender, bool senderIsConst>
struct FunctionPriorityDelegateNotifyType;

template <class TArgs>
struct FunctionPriorityDelegateNotifyType<TArgs, true, true>
{
	using Type = void (*)(const void*, TArgs&);
};

template <class TArgs>
struct FunctionPriorityDelegateNotifyType<TArgs, true, false>
{
	using Type = void (*)(void*, TArgs&);
};

template <class TArgs, bool senderIsConst>
struct FunctionPriorityDelegateNotifyType<TArgs, false, senderIsConst>
{
	using Type = void (*)(TArgs&);
};

template <>
struct FunctionPriorityDelegateNotifyType<void, true, true>
{
	using Type = void (*)(const void*);
};

template <>
struct FunctionPriorityDelegateNotifyType<void, true, false>
{
	using Type = void (*)(void*);
};

template <bool senderIsConst>
struct FunctionPriorityDelegateNotifyType<void, false, senderIsConst>
{
	using Type = void (*)();
};

} // namespace Detail


template <class TArgs, bool hasSender = true, bool senderIsConst = true>
class FunctionPriorityDelegate: public AbstractPriorityDelegate<TArgs>
	/// Wraps a freestanding function or static member function
	/// for use as a PriorityDelegate.
{
public:
	using NotifyFunction = typename Detail::FunctionPriorityDelegateNotifyType<TArgs, hasSender, senderIsConst>::Type;

	FunctionPriorityDelegate(NotifyFunction function, int prio):
		AbstractPriorityDelegate<TArgs>(prio),
		_function(function)
	{
	}

	FunctionPriorityDelegate(const FunctionPriorityDelegate& delegate):
		AbstractPriorityDelegate<TArgs>(delegate),
		_function(delegate._function)
	{
	}

	FunctionPriorityDelegate& operator = (const FunctionPriorityDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_function = delegate._function;
			this->_priority = delegate._priority;
		}
		return *this;
	}

	~FunctionPriorityDelegate() = default;

	FunctionPriorityDelegate() = delete;

	bool notify(const void* sender, TArgs& arguments)
	{
		Mutex::ScopedLock lock(_mutex);
		if (_function)
		{
			if constexpr (hasSender)
			{
				if constexpr (senderIsConst)
				{
					(*_function)(sender, arguments);
				}
				else
				{
					(*_function)(const_cast<void*>(sender), arguments);
				}
			}
			else
			{
				(*_function)(arguments);
			}
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		const FunctionPriorityDelegate* pOtherDelegate = dynamic_cast<const FunctionPriorityDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _function == pOtherDelegate->_function;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionPriorityDelegate(*this);
	}

	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_function = nullptr;
	}

protected:
	NotifyFunction _function;
	Mutex _mutex;
};


template <bool hasSender, bool senderIsConst>
class FunctionPriorityDelegate<void, hasSender, senderIsConst>: public AbstractPriorityDelegate<void>
	/// Specialization for void arguments (no TArgs parameter).
{
public:
	using NotifyFunction = typename Detail::FunctionPriorityDelegateNotifyType<void, hasSender, senderIsConst>::Type;

	FunctionPriorityDelegate(NotifyFunction function, int prio):
		AbstractPriorityDelegate<void>(prio),
		_function(function)
	{
	}

	FunctionPriorityDelegate(const FunctionPriorityDelegate& delegate):
		AbstractPriorityDelegate<void>(delegate),
		_function(delegate._function)
	{
	}

	FunctionPriorityDelegate& operator = (const FunctionPriorityDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_function = delegate._function;
			this->_priority = delegate._priority;
		}
		return *this;
	}

	~FunctionPriorityDelegate() override = default;

	FunctionPriorityDelegate() = delete;

	bool notify(const void* sender) override
	{
		Mutex::ScopedLock lock(_mutex);
		if (_function)
		{
			if constexpr (hasSender)
			{
				if constexpr (senderIsConst)
				{
					(*_function)(sender);
				}
				else
				{
					(*_function)(const_cast<void*>(sender));
				}
			}
			else
			{
				(*_function)();
			}
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<void>& other) const override
	{
		const FunctionPriorityDelegate* pOtherDelegate = dynamic_cast<const FunctionPriorityDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _function == pOtherDelegate->_function;
	}

	AbstractDelegate<void>* clone() const override
	{
		return new FunctionPriorityDelegate(*this);
	}

	void disable() override
	{
		Mutex::ScopedLock lock(_mutex);
		_function = nullptr;
	}

protected:
	NotifyFunction _function;
	Mutex _mutex;
};


} // namespace Poco


#endif // Foundation_FunctionPriorityDelegate_INCLUDED
