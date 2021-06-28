//
// SharedPtr.h
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
#include <cstddef>


namespace Poco {


class ReferenceCounter
	/// Simple ReferenceCounter object, does not delete itself when count reaches 0.
{
public:
	ReferenceCounter(): _cnt(1)
	{
	}

	void duplicate()
	{
		++_cnt;
	}

	int release()
	{
		return --_cnt;
	}

	int referenceCount() const
	{
		return _cnt.value();
	}

private:
	AtomicCounter _cnt;
};


template <class C>
class ReleasePolicy
	/// The default release policy for SharedPtr, which
	/// simply uses the delete operator to delete an object.
{
public:
	static void release(C* pObj) noexcept
		/// Delete the object.
		/// Note that pObj can be nullptr.
	{
		delete pObj;
	}
};


template <class C>
class ReleaseArrayPolicy
	/// The release policy for SharedPtr holding arrays.
{
public:
	static void release(C* pObj) noexcept
		/// Delete the object.
		/// Note that pObj can be nullptr.
	{
		delete [] pObj;
	}
};


template <class C, class RC = ReferenceCounter, class RP = ReleasePolicy<C>>
class SharedPtr
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
	typedef C Type;

	SharedPtr():
		_pCounter(nullptr),
		_ptr(nullptr)
	{
	}

	SharedPtr(C* ptr)
	try:
		_pCounter(ptr ? new RC : nullptr),
		_ptr(ptr)
	{
	}
	catch (...)
	{
		RP::release(ptr);
	}

	template <class Other, class OtherRP>
	SharedPtr(const SharedPtr<Other, RC, OtherRP>& ptr):
		_pCounter(ptr._pCounter),
		_ptr(const_cast<Other*>(ptr.get()))
	{
		if (_pCounter) _pCounter->duplicate();
	}

	SharedPtr(const SharedPtr& ptr):
		_pCounter(ptr._pCounter),
		_ptr(ptr._ptr)
	{
		if (_pCounter) _pCounter->duplicate();
	}

	SharedPtr(SharedPtr&& ptr) noexcept:
		_pCounter(ptr._pCounter),
		_ptr(ptr._ptr)
	{
		ptr._pCounter = nullptr;
		ptr._ptr = nullptr;
	}

	~SharedPtr()
	{
		release();
	}

	SharedPtr& assign(C* ptr)
	{
		if (get() != ptr)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	SharedPtr& assign(const SharedPtr& ptr)
	{
		if (&ptr != this)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	template <class Other, class OtherRP>
	SharedPtr& assign(const SharedPtr<Other, RC, OtherRP>& ptr)
	{
		if (ptr.get() != _ptr)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	void reset()
	{
		assign(nullptr);
	}

	void reset(C* ptr)
	{
		assign(ptr);
	}

	void reset(const SharedPtr& ptr)
	{
		assign(ptr);
	}

	template <class Other, class OtherRP>
	void reset(const SharedPtr<Other, RC, OtherRP>& ptr)
	{
		assign<Other, OtherRP>(ptr);
	}

	SharedPtr& operator = (C* ptr)
	{
		return assign(ptr);
	}

	SharedPtr& operator = (const SharedPtr& ptr)
	{
		return assign(ptr);
	}

	SharedPtr& operator = (SharedPtr&& ptr) noexcept
	{
		release();
		_ptr = ptr._ptr;
		ptr._ptr = nullptr;
		_pCounter = ptr._pCounter;
		ptr._pCounter = nullptr;
		return *this;
	}

	template <class Other, class OtherRP>
	SharedPtr& operator = (const SharedPtr<Other, RC, OtherRP>& ptr)
	{
		return assign<Other>(ptr);
	}

	void swap(SharedPtr& ptr)
	{
		std::swap(_ptr, ptr._ptr);
		std::swap(_pCounter, ptr._pCounter);
	}

	template <class Other>
	SharedPtr<Other, RC, RP> cast() const
		/// Casts the SharedPtr via a dynamic cast to the given type.
		/// Returns an SharedPtr containing NULL if the cast fails.
		/// Example: (assume class Sub: public Super)
		///    SharedPtr<Super> super(new Sub());
		///    SharedPtr<Sub> sub = super.cast<Sub>();
		///    poco_assert (sub.get());
	{
		Other* pOther = dynamic_cast<Other*>(_ptr);
		if (pOther)
			return SharedPtr<Other, RC, RP>(_pCounter, pOther);
		return SharedPtr<Other, RC, RP>();
	}

	template <class Other>
	SharedPtr<Other, RC, RP> unsafeCast() const
		/// Casts the SharedPtr via a static cast to the given type.
		/// Example: (assume class Sub: public Super)
		///    SharedPtr<Super> super(new Sub());
		///    SharedPtr<Sub> sub = super.unsafeCast<Sub>();
		///    poco_assert (sub.get());
	{
		Other* pOther = static_cast<Other*>(_ptr);
		return SharedPtr<Other, RC, RP>(_pCounter, pOther);
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
		return _ptr == nullptr;
	}

	bool isNull() const
	{
		return _ptr == nullptr;
	}

	bool operator == (const SharedPtr& ptr) const
	{
		return get() == ptr.get();
	}

	bool operator == (const C* ptr) const
	{
		return get() == ptr;
	}

	bool operator == (C* ptr) const
	{
		return get() == ptr;
	}

	bool operator == (std::nullptr_t ptr) const
	{
		return get() == ptr;
	}

	bool operator != (const SharedPtr& ptr) const
	{
		return get() != ptr.get();
	}

	bool operator != (const C* ptr) const
	{
		return get() != ptr;
	}

	bool operator != (C* ptr) const
	{
		return get() != ptr;
	}

	bool operator != (std::nullptr_t ptr) const
	{
		return get() != ptr;
	}

	bool operator < (const SharedPtr& ptr) const
	{
		return get() < ptr.get();
	}

	bool operator < (const C* ptr) const
	{
		return get() < ptr;
	}

	bool operator < (C* ptr) const
	{
		return get() < ptr;
	}

	bool operator <= (const SharedPtr& ptr) const
	{
		return get() <= ptr.get();
	}

	bool operator <= (const C* ptr) const
	{
		return get() <= ptr;
	}

	bool operator <= (C* ptr) const
	{
		return get() <= ptr;
	}

	bool operator > (const SharedPtr& ptr) const
	{
		return get() > ptr.get();
	}

	bool operator > (const C* ptr) const
	{
		return get() > ptr;
	}

	bool operator > (C* ptr) const
	{
		return get() > ptr;
	}

	bool operator >= (const SharedPtr& ptr) const
	{
		return get() >= ptr.get();
	}

	bool operator >= (const C* ptr) const
	{
		return get() >= ptr;
	}

	bool operator >= (C* ptr) const
	{
		return get() >= ptr;
	}

	int referenceCount() const
	{
		return _pCounter ? _pCounter->referenceCount() : 0;
	}

private:
	C* deref() const
	{
		if (!_ptr)
			throw NullPointerException();

		return _ptr;
	}

	void release() noexcept
	{
		if (_pCounter && _pCounter->release() == 0)
		{
			RP::release(_ptr);
			_ptr = nullptr;

			delete _pCounter;
			_pCounter = nullptr;
		}
	}

	SharedPtr(RC* pCounter, C* ptr): _pCounter(pCounter), _ptr(ptr)
		/// for cast operation
	{
		poco_assert_dbg (_pCounter);
		_pCounter->duplicate();
	}

private:
	RC* _pCounter;
	C*  _ptr;

	template <class OtherC, class OtherRC, class OtherRP> friend class SharedPtr;
};


template <class C, class RC, class RP>
inline void swap(SharedPtr<C, RC, RP>& p1, SharedPtr<C, RC, RP>& p2)
{
	p1.swap(p2);
}


template <typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args)
{
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}


template <typename T>
SharedPtr<T, ReferenceCounter, ReleaseArrayPolicy<T>> makeSharedArray(std::size_t size)
{
	return SharedPtr<T, ReferenceCounter, ReleaseArrayPolicy<T>>(new T[size]);
}


} // namespace Poco


#endif // Foundation_SharedPtr_INCLUDED
