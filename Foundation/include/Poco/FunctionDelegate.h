//
// FunctionDelegate.h
//
// Library: Foundation
// Package: Events
// Module:  FunctionDelegate
//
// Implementation of the FunctionDelegate template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FunctionDelegate_INCLUDED
#define Foundation_FunctionDelegate_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractDelegate.h"
#include "Poco/Mutex.h"


namespace Poco {


template <class TArgs, bool hasSender = true, bool senderIsConst = true>
class FunctionDelegate: public AbstractDelegate<TArgs>
	/// Wraps a freestanding function or static member function
	/// for use as a Delegate.
{
public:
	using NotifyFunction = void (*)(const void *, TArgs &);

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
			(*_function)(sender, arguments);
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _function == pOtherDelegate->_function;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionDelegate(*this);
	}

	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_function = 0;
	}

protected:
	NotifyFunction _function;
	Mutex _mutex;
};


template <class TArgs>
class FunctionDelegate<TArgs, true, false>: public AbstractDelegate<TArgs>
{
public:
	using NotifyFunction = void (*)(void *, TArgs &);

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
			(*_function)(const_cast<void*>(sender), arguments);
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _function == pOtherDelegate->_function;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionDelegate(*this);
	}

	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_function = 0;
	}

protected:
	NotifyFunction _function;
	Mutex _mutex;
};


template <class TArgs, bool senderIsConst>
class FunctionDelegate<TArgs, false, senderIsConst>: public AbstractDelegate<TArgs>
{
public:
	using NotifyFunction = void (*)(TArgs &);

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

	bool notify(const void* /*sender*/, TArgs& arguments)
	{
		Mutex::ScopedLock lock(_mutex);
		if (_function)
		{
			(*_function)(arguments);
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _function == pOtherDelegate->_function;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionDelegate(*this);
	}

	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_function = 0;
	}

protected:
	NotifyFunction _function;
	Mutex _mutex;
};


template <>
class FunctionDelegate<void, true, true>: public AbstractDelegate<void>
	/// Wraps a freestanding function or static member function
	/// for use as a Delegate.
{
public:
	using NotifyFunction = void (*)(const void *);

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

	bool notify(const void *sender) override
	{
		Mutex::ScopedLock lock(_mutex);
		if (_function)
		{
			(*_function)(sender);
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<void> &other) const override
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _function == pOtherDelegate->_function;
	}

	AbstractDelegate<void> *clone() const override
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


template <>
class FunctionDelegate<void, true, false>: public AbstractDelegate<void>
{
public:
	using NotifyFunction = void (*)(void *);

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

	bool notify(const void *sender) override
	{
		Mutex::ScopedLock lock(_mutex);
		if (_function)
		{
			(*_function)(const_cast<void*>(sender));
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<void> &other) const override
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _function == pOtherDelegate->_function;
	}

	AbstractDelegate<void> *clone() const override
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


template <bool senderIsConst>
class FunctionDelegate<void, false, senderIsConst>: public AbstractDelegate<void>
{
public:
	using NotifyFunction = void (*)();

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

	bool notify(const void * /*sender*/) override
	{
		Mutex::ScopedLock lock(_mutex);
		if (_function)
		{
			(*_function)();
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<void> &other) const override
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _function == pOtherDelegate->_function;
	}

	AbstractDelegate<void> *clone() const override
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
