//
// SharedPtr.h
//
// $Id: //poco/1.4/Foundation/include/Poco/SharedPtr.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  SharedPtr
//
// Definition of the SharedPtr template class.
//
// Copyright (c) 2005-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_SharedPtr_INCLUDED
#define Foundation_SharedPtr_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/AtomicCounter.h"
#include <algorithm>
#include <memory>


namespace Poco {



template <class C>
class SharedPtr
	: public std::shared_ptr<C>
	/// SharedPtr is a "smart" pointer for classes implementing
	/// reference counting based garbage collection.
	/// SharedPtr is thus similar to AutoPtr. Unlike the
	/// AutoPtr template, which can only be used with
	/// classes that support reference counting, SharedPtr
	/// can be used with any class. For this to work, a
	/// SharedPtr manages a reference count for the object
	/// it manages.
	/// 
	/// SharedPtr works in the following way:
	/// If an SharedPtr is assigned an ordinary pointer to
	/// an object (via the constructor or the assignment operator),
	/// it takes ownership of the object and the object's reference 
	/// count is initialized to one.
	/// If the SharedPtr is assigned another SharedPtr, the
	/// object's reference count is incremented by one.
	/// The destructor of SharedPtr decrements the object's
	/// reference count by one and deletes the object if the
	/// reference count reaches zero.
	/// SharedPtr supports dereferencing with both the ->
	/// and the * operator. An attempt to dereference a null
	/// SharedPtr results in a NullPointerException being thrown.
	/// SharedPtr also implements all relational operators and
	/// a cast operator in case dynamic casting of the encapsulated data types
	/// is required.
{
public:
	SharedPtr()
		: std::shared_ptr<C>()
	{
	}

	SharedPtr(C* ptr)
		: std::shared_ptr<C>(ptr)
	{
	}

	template <class Other>
	SharedPtr(const SharedPtr<Other>& ptr)
		: std::shared_ptr<C>(ptr)
	{

	}

	SharedPtr(const SharedPtr& ptr)
		: std::shared_ptr<C>(ptr)
	{
	}

	SharedPtr(const std::shared_ptr<C>& ptr)
		: std::shared_ptr<C>(ptr)
	{
	}

	SharedPtr(SharedPtr&& ptr) 
		: std::shared_ptr<C>(ptr)
	{
	}


	SharedPtr& assign(C* ptr)
	{
		if (this->get() != ptr)
		{
			SharedPtr tmp(ptr);
			this->swap(tmp);
		}
		return *this;
	}
	
	SharedPtr& assign(const SharedPtr& ptr)
	{
		if (&ptr != this)
		{
			SharedPtr tmp(ptr);
			this->swap(tmp);
		}
		return *this;
	}
	
	template <class Other>
	SharedPtr& assign(const SharedPtr<Other>& ptr)
	{
		if (ptr.get() != this->get())
		{
			SharedPtr tmp(ptr);
			this->swap(tmp);
		}
		return *this;
	}

	SharedPtr& operator = (C* ptr)
	{
		return assign(ptr);
	}

	SharedPtr& operator = (const SharedPtr& ptr)
	{
		return assign(ptr);
	}

	SharedPtr& operator = (SharedPtr&& ptr)
	{
		std::shared_ptr<C>::operator=(ptr);
		return *this;
	}

	template <class Other>
	SharedPtr& operator = (const SharedPtr<Other>& ptr)
	{
		return assign<Other>(ptr);
	}


	template <class Other>
	SharedPtr<Other> cast() const
		/// Casts the SharedPtr via a dynamic cast to the given type.
		/// Returns an SharedPtr containing NULL if the cast fails.
		/// Example: (assume class Sub: public Super)
		///    SharedPtr<Super> super(new Sub());
		///    SharedPtr<Sub> sub = super.cast<Sub>();
		///    poco_assert (sub.get());
	{
		std::shared_ptr<Other> pOther = std::dynamic_pointer_cast<Other>(*this);
		return pOther;
	}

	template <class Other>
	SharedPtr<Other> unsafeCast() const
		/// Casts the SharedPtr via a static cast to the given type.
		/// Example: (assume class Sub: public Super)
		///    SharedPtr<Super> super(new Sub());
		///    SharedPtr<Sub> sub = super.unsafeCast<Sub>();
		///    poco_assert (sub.get());
	{
		std::shared_ptr<Other> pOther = std::static_pointer_cast<Other>(*this);
		return pOther;
	}

	C* operator -> ()
	{
		return deref();
	}

	const C* operator -> () const
	{
		return deref();
	}

	C& operator * ()
	{
		return *deref();
	}

	const C& operator * () const
	{
		return *deref();
	}

	operator C* ()
	{
		return this->get();
	}
	
	operator const C* () const
	{
		return this->get();
	}


	bool isNull() const
	{
		return this->get() == 0;
	}

	bool operator == (const SharedPtr& ptr) const
	{
		return this->get() == ptr.get();
	}

	bool operator == (const C* ptr) const
	{
		return this->get() == ptr;
	}

	bool operator == (C* ptr) const
	{
		return this->get() == ptr;
	}

	bool operator != (const SharedPtr& ptr) const
	{
		return this->get() != ptr.get();
	}

	bool operator != (const C* ptr) const
	{
		return this->get() != ptr;
	}

	bool operator != (C* ptr) const
	{
		return this->get() != ptr;
	}

	bool operator < (const SharedPtr& ptr) const
	{
		return this->get() < ptr.get();
	}

	bool operator < (const C* ptr) const
	{
		return this->get() < ptr;
	}

	bool operator < (C* ptr) const
	{
		return this->get() < ptr;
	}

	bool operator <= (const SharedPtr& ptr) const
	{
		return this->get() <= ptr.get();
	}

	bool operator <= (const C* ptr) const
	{
		return this->get() <= ptr;
	}

	bool operator <= (C* ptr) const
	{
		return this->get() <= ptr;
	}

	bool operator > (const SharedPtr& ptr) const
	{
		return this->get() > ptr.get();
	}

	bool operator > (const C* ptr) const
	{
		return this->get() > ptr;
	}

	bool operator > (C* ptr) const
	{
		return this->get() > ptr;
	}

	bool operator >= (const SharedPtr& ptr) const
	{
		return this->get() >= ptr.get();
	}

	bool operator >= (const C* ptr) const
	{
		return this->get() >= ptr;
	}

	bool operator >= (C* ptr) const
	{
		return this->get() >= ptr;
	}
	
	int referenceCount() const
	{
		return this->use_count();
	}

protected:
	C* deref() const
	{
		if (!this->operator bool())
			throw NullPointerException();

		return this->get();
	}

	const C* deref() const
	{
		if (!this->operator bool())
			throw NullPointerException();

		return this->get();
	}
	

};


} // namespace Poco


#endif // Foundation_SharedPtr_INCLUDED
