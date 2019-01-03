//
// StdFunctionDelegate.h
//
// Library: Foundation
// Package: Events
// Module:  StdFunctionDelegate
//
// Implementation of the StdFunctionDelegate template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_StdFunctionDelegate_INCLUDED
#define Foundation_StdFunctionDelegate_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractDelegate.h"
#include "Poco/Mutex.h"
#include <functional>
#include <atomic>


namespace Poco {


template <class TArgs, bool hasSender = true, bool senderIsConst = true>
class StdFunctionDelegate: public Poco::AbstractDelegate<TArgs>
    /// Wraps a std::function or lambda
    /// for use as a Delegate.
{
public:
    typedef std::function<void (const void*, TArgs&)> NotifyMethod;

	StdFunctionDelegate() = delete;

    StdFunctionDelegate(NotifyMethod method):
        _receiverMethod(method), _id(++id_generator)
    {
    }

    StdFunctionDelegate(const StdFunctionDelegate& delegate):
        Poco::AbstractDelegate<TArgs>(delegate),
        _receiverMethod(delegate._receiverMethod),
		_id(delegate._id)
    {
    }

    ~StdFunctionDelegate()
    {
    }

    StdFunctionDelegate& operator = (const StdFunctionDelegate& delegate)
    {
        if (&delegate != this)
        {
            this->_pTarget        = delegate._pTarget;
            this->_receiverMethod = delegate._receiverMethod;
			this->_id             = delegate._id;
        }
        return *this;
    }

    bool notify(const void* sender, TArgs& arguments)
    {
        Poco::Mutex::ScopedLock lock(_mutex);
        if (_receiverMethod)
        {
            _receiverMethod(sender, arguments);
            return true;
        }
        else return false;
    }


    bool equals(const Poco::AbstractDelegate<TArgs>& other) const
    {
        const StdFunctionDelegate* pOtherDelegate = dynamic_cast<const StdFunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _id == pOtherDelegate->_id;
    }

    Poco::AbstractDelegate<TArgs>* clone() const
    {
        return new StdFunctionDelegate(*this);
    }

    void disable()
    {
        Poco::Mutex::ScopedLock lock(_mutex);
        _receiverMethod = nullptr;
    }

protected:
    NotifyMethod _receiverMethod;
    Poco::Mutex _mutex;
	int _id;

private:
	static std::atomic_int id_generator;
};


template <class TArgs, bool hasSender, bool senderIsConst>
std::atomic_int StdFunctionDelegate<TArgs, hasSender, senderIsConst>::id_generator;


} // namespace Poco


#endif // Foundation_StdFunctionDelegate_INCLUDED
