//
// RefPtr.h
//
// Library: Foundation
// Package: Core
// Module:  RefPtr
//
// Definition of the RefPtr template class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_RefPtr_INCLUDED
#define Foundation_RefPtr_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"


namespace Poco {


template <class C>
class RefPtr
	/// RefPtr is a "smart" pointer for classes implementing
	/// reference counting based garbage collection.
	/// To be usable with the RefPtr template, a class must
	/// implement the following behaviour:
	/// A class must maintain a reference count.
	/// The constructors of the object initialize the reference
	/// count to one.
	/// The class must implement following public methods:
	///
	///     void duplicate()
	///     (increments the reference count by one)
	///
	///     void release()
	///     (decrements the reference count by one, if the reference
	///      count reaches zero, deletes the object)
	///
	/// For a conforming implementation, see RefCountedObject.
	/// Unless there are specific reasons not to, it is recommended
	/// that RefPtr-held objects inherit from RefCountedObject.
	///
	/// RefPtr works in the following way:
	/// If an RefPtr is assigned an ordinary pointer to
	/// an object (via the constructor or the assignment operator),
	/// it takes ownership of the object and the object's reference
	/// count remains unchanged.
	/// If the RefPtr is assigned another RefPtr, the
	/// object's reference count is incremented by one by
	/// calling duplicate() on its object.
	/// The destructor of RefPtr calls release() on its
	/// object.
	/// RefPtr supports dereferencing with both the ->
	/// and the * operator. An attempt to dereference a null
	/// RefPtr results in a NullPointerException being thrown.
	/// RefPtr also implements all relational operators.
	/// Note that RefPtr allows casting of its encapsulated data types.
	///
	/// RefPtr supports weak pointers to its internally held pointers.
	/// For semantics, see WeakRefPtr documentation.
	///
{
public:
	RefPtr(): _ptr(0)
	{
	}

	RefPtr(C* ptr): _ptr(ptr)
	{
	}

	RefPtr(C* ptr, bool shared): _ptr(ptr)
	{
		if (shared && _ptr) _ptr->duplicate();
	}

	RefPtr(const RefPtr& ptr): _ptr(ptr._ptr)
	{
		if (_ptr) _ptr->duplicate();
	}

	RefPtr(RefPtr&& ptr) : _ptr(ptr._ptr)
	{
		ptr._ptr = 0;
	}

	template <class Other>
	RefPtr(const RefPtr<Other>& ptr): _ptr(const_cast<Other*>(ptr.get()))
	{
		if (_ptr) _ptr->duplicate();
	}

	template <class Other>
	RefPtr(RefPtr<Other>&& ptr): _ptr(ptr.get())
	{
		ptr.resetMove();
	}

	~RefPtr()
	{
		release();
	}

	RefPtr& assign(C* ptr)
	{
		if (_ptr != ptr)
		{
			release();
			_ptr = ptr;
		}
		return *this;
	}

	RefPtr& assign(C* ptr, bool shared)
	{
		if (_ptr != ptr)
		{
			release();
			_ptr = ptr;
			if (shared && _ptr) _ptr->duplicate();
		}
		return *this;
	}

	RefPtr& assign(const RefPtr& ptr)
	{
		if (&ptr != this)
		{
			release();
			_ptr = ptr._ptr;
			if (_ptr) _ptr->duplicate();
		}
		return *this;
	}

	template <class Other>
	RefPtr& assign(const RefPtr<Other>& ptr)
	{
		if (&ptr != (RefPtr<Other>*)this)
		{
			release();
			_ptr = const_cast<Other*>(ptr.get());
			if (_ptr) _ptr->duplicate();
		}
		return *this;
	}

	void resetMove()
	{
		if (_ptr) _ptr = 0;
	}

	void reset()
	{
		if (_ptr)
		{
			release();
			_ptr = 0;
		}
	}

	void reset(C* ptr)
	{
		assign(ptr);
	}

	void reset(C* ptr, bool shared)
	{
		assign(ptr, shared);
	}

	void reset(const RefPtr& ptr)
	{
		assign(ptr);
	}

	template <class Other>
	void reset(const RefPtr<Other>& ptr)
	{
		assign<Other>(ptr);
	}

	RefPtr& operator = (C* ptr)
	{
		return assign(ptr);
	}

	RefPtr& operator = (const RefPtr& ptr)
	{
		return assign(ptr);
	}

	template <class Other>
	RefPtr& operator = (const RefPtr<Other>& ptr)
	{
		return assign<Other>(ptr);
	}

	void moveAssign(C** pptr)
		/// Move-assigns pointer to this RefPtr.
		/// The pointer to pointer must not be null,
		/// but the dereferenced pointer may be null.
		/// Self move-assignment is no-op
	{
		poco_check_ptr (pptr);

		if (*pptr != _ptr)
		{
			reset();
			if (*pptr)
			{
				_ptr = *pptr;
				*pptr = 0;
			}
		}
	}

	RefPtr& operator = (RefPtr&& refPtr)
	{
		moveAssign(&refPtr._ptr);
		return *this;
	}

	template <class Other>
	RefPtr& operator = (RefPtr<Other>&& otherRefPtr)
	{
		Other* pOther = otherRefPtr.get();
		if (pOther != _ptr)
		{
			reset();
			if (pOther)
			{
				_ptr = pOther;
				otherRefPtr.resetMove();
			}
		}
		return *this;
	}

	void swap(RefPtr& ptr)
	{
		std::swap(_ptr, ptr._ptr);
	}

	template <class Other>
	RefPtr<Other> cast() const
		/// Casts the RefPtr via a dynamic cast to the given type.
		/// Returns an RefPtr containing NULL if the cast fails.
		/// Example: (assume class Sub: public Super)
		///    RefPtr<Super> super(new Sub());
		///    RefPtr<Sub> sub = super.cast<Sub>();
		///    poco_assert (sub.get());
	{
		return RefPtr<Other>(dynamic_cast<Other*>(_ptr), true);
	}

	template <class Other>
	RefPtr<Other> unsafeCast() const
		/// Casts the RefPtr via a static cast to the given type.
		/// Example: (assume class Sub: public Super)
		///    RefPtr<Super> super(new Sub());
		///    RefPtr<Sub> sub = super.unsafeCast<Sub>();
		///    poco_assert (sub.get());
	{
		return RefPtr<Other>(static_cast<Other*>(_ptr), true);
	}

	C* operator -> ()
	{
		if (_ptr)
			return _ptr;
		else
			throw NullPointerException();
	}

	const C* operator -> () const
	{
		if (_ptr)
			return _ptr;
		else
			throw NullPointerException();
	}

	C& operator * ()
	{
		if (_ptr)
			return *_ptr;
		else
			throw NullPointerException();
	}

	const C& operator * () const
	{
		if (_ptr)
			return *_ptr;
		else
			throw NullPointerException();
	}

	C* get()
	{
		return _ptr;
	}

	const C* get() const
	{
		return _ptr;
	}

	operator C* ()
	{
		return _ptr;
	}

	operator const C* () const
	{
		return _ptr;
	}

	bool operator ! () const
	{
		return _ptr == 0;
	}

	bool isNull() const
	{
		return _ptr == 0;
	}

	C* duplicate()
	{
		if (_ptr) _ptr->duplicate();
		return _ptr;
	}

	bool operator == (const RefPtr& ptr) const
	{
		return _ptr == ptr._ptr;
	}

	bool operator == (const C* ptr) const
	{
		return _ptr == ptr;
	}

	bool operator == (C* ptr) const
	{
		return _ptr == ptr;
	}

	bool operator != (const RefPtr& ptr) const
	{
		return _ptr != ptr._ptr;
	}

	bool operator != (const C* ptr) const
	{
		return _ptr != ptr;
	}

	bool operator != (C* ptr) const
	{
		return _ptr != ptr;
	}

	bool operator < (const RefPtr& ptr) const
	{
		return _ptr < ptr._ptr;
	}

	bool operator < (const C* ptr) const
	{
		return _ptr < ptr;
	}

	bool operator < (C* ptr) const
	{
		return _ptr < ptr;
	}

	bool operator <= (const RefPtr& ptr) const
	{
		return _ptr <= ptr._ptr;
	}

	bool operator <= (const C* ptr) const
	{
		return _ptr <= ptr;
	}

	bool operator <= (C* ptr) const
	{
		return _ptr <= ptr;
	}

	bool operator > (const RefPtr& ptr) const
	{
		return _ptr > ptr._ptr;
	}

	bool operator > (const C* ptr) const
	{
		return _ptr > ptr;
	}

	bool operator > (C* ptr) const
	{
		return _ptr > ptr;
	}

	bool operator >= (const RefPtr& ptr) const
	{
		return _ptr >= ptr._ptr;
	}

	bool operator >= (const C* ptr) const
	{
		return _ptr >= ptr;
	}

	bool operator >= (C* ptr) const
	{
		return _ptr >= ptr;
	}

private:
	void release()
	{
		if (_ptr) _ptr->release();
	}

	C* _ptr = 0;
};


template <class C>
inline void swap(RefPtr<C>& p1, RefPtr<C>& p2)
{
	p1.swap(p2);
}


} // namespace Poco


#endif // Foundation_RefPtr_INCLUDED
