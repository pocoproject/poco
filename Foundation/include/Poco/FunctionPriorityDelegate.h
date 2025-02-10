//
// FunctionPriorityDelegate.h
//
// Library: Foundation
// Package: Events
// Module:  FunctionPriorityDelegate
//
// Implementation of the FunctionPriorityDelegate template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FunctionPriorityDelegate_INCLUDED
#define Foundation_FunctionPriorityDelegate_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractPriorityDelegate.h"
#include "Poco/Mutex.h"


namespace Poco {


template <class TArgs, bool useSender = true, bool senderIsConst = true>
class FunctionPriorityDelegate: public AbstractPriorityDelegate<TArgs>
	/// Wraps a freestanding function or static member function
	/// for use as a PriorityDelegate.
{
public:
	using NotifyFunction = void (*)(const void *, TArgs &);

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
			(*_function)(sender, arguments);
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
		_function = 0;
	}

protected:
	NotifyFunction _function;
	Mutex _mutex;
};


template <class TArgs>
class FunctionPriorityDelegate<TArgs, true, false>: public AbstractPriorityDelegate<TArgs>
{
public:
	using NotifyFunction = void (*)(void *, TArgs &);

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
			(*_function)(const_cast<void*>(sender), arguments);
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
		_function = 0;
	}

protected:
	NotifyFunction _function;
	Mutex _mutex;
};


template <class TArgs>
class FunctionPriorityDelegate<TArgs, false>: public AbstractPriorityDelegate<TArgs>
{
public:
	using NotifyFunction = void (*)(TArgs &);

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
			(*_function)(arguments);
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
		_function = 0;
	}

protected:
	NotifyFunction _function;
	Mutex _mutex;
};


template <>
class FunctionPriorityDelegate<void, true, true>: public AbstractPriorityDelegate<void>
	/// Wraps a freestanding function or static member function
	/// for use as a PriorityDelegate.
{
public:
	using NotifyFunction = void (*)(const void *);

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

	FunctionPriorityDelegate() = delete;

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

	bool notify(const void* sender) override
	{
		Mutex::ScopedLock lock(_mutex);
		if (_function)
		{
			(*_function)(sender);
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


template <>
class FunctionPriorityDelegate<void, true, false>: public AbstractPriorityDelegate<void>
{
public:
	using NotifyFunction = void (*)(void *);

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
			(*_function)(const_cast<void*>(sender));
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


template <>
class FunctionPriorityDelegate<void, false>: public AbstractPriorityDelegate<void>
{
public:
	using NotifyFunction = void (*)();

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
			(*_function)();
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
