//
// FunctionDelegate.h
//
// Library: Foundation
// Package: Events
// Module:  FunctionDelegate
//
// Implementation of the FunctionDelegate template.
//
// Copyright (c) 2006-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FunctionDelegate_INCLUDED
#define Foundation_FunctionDelegate_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractDelegate.h"
#include "Poco/Mutex.h"
#include <type_traits>


namespace Poco {


namespace Detail {

/// Helper to select the correct function pointer type based on template parameters.
template <class TArgs, bool hasSender, bool senderIsConst>
struct FunctionDelegateNotifyType;

template <class TArgs>
struct FunctionDelegateNotifyType<TArgs, true, true>
{
	using Type = void (*)(const void*, TArgs&);
};

template <class TArgs>
struct FunctionDelegateNotifyType<TArgs, true, false>
{
	using Type = void (*)(void*, TArgs&);
};

template <class TArgs, bool senderIsConst>
struct FunctionDelegateNotifyType<TArgs, false, senderIsConst>
{
	using Type = void (*)(TArgs&);
};

template <>
struct FunctionDelegateNotifyType<void, true, true>
{
	using Type = void (*)(const void*);
};

template <>
struct FunctionDelegateNotifyType<void, true, false>
{
	using Type = void (*)(void*);
};

template <bool senderIsConst>
struct FunctionDelegateNotifyType<void, false, senderIsConst>
{
	using Type = void (*)();
};

} // namespace Detail


template <class TArgs, bool hasSender = true, bool senderIsConst = true>
class FunctionDelegate: public AbstractDelegate<TArgs>
	/// Wraps a freestanding function or static member function
	/// for use as a Delegate.
{
public:
	using NotifyFunction = typename Detail::FunctionDelegateNotifyType<TArgs, hasSender, senderIsConst>::Type;

	FunctionDelegate(NotifyFunction function):
		_function(function)
	{
	}

	FunctionDelegate(const FunctionDelegate& delegate):
		AbstractDelegate<TArgs>(delegate),
		_function(delegate._function)
	{
	}

	~FunctionDelegate() = default;

	FunctionDelegate() = delete;

	FunctionDelegate& operator = (const FunctionDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_function = delegate._function;
		}
		return *this;
	}

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

	[[nodiscard]]
	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _function == pOtherDelegate->_function;
	}

	[[nodiscard]]
	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionDelegate(*this);
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
class FunctionDelegate<void, hasSender, senderIsConst>: public AbstractDelegate<void>
	/// Specialization for void arguments (no TArgs parameter).
{
public:
	using NotifyFunction = typename Detail::FunctionDelegateNotifyType<void, hasSender, senderIsConst>::Type;

	FunctionDelegate(NotifyFunction function):
		_function(function)
	{
	}

	FunctionDelegate(const FunctionDelegate& delegate):
		AbstractDelegate<void>(delegate),
		_function(delegate._function)
	{
	}

	~FunctionDelegate() override = default;

	FunctionDelegate() = delete;

	FunctionDelegate& operator = (const FunctionDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_function = delegate._function;
		}
		return *this;
	}

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

	[[nodiscard]]
	bool equals(const AbstractDelegate<void>& other) const override
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _function == pOtherDelegate->_function;
	}

	[[nodiscard]]
	AbstractDelegate<void>* clone() const override
	{
		return new FunctionDelegate(*this);
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


#endif // Foundation_FunctionDelegate_INCLUDED
