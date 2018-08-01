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
#include <memory>
#include <algorithm>
#include <utility>


namespace Poco {


template <class C>
class SharedPtr
	/// As of POCO C++ Libraries Release 2.0, SharedPtr is a small wrapper around
	/// std::shared_ptr, providing basic backwards compatibility to the 1.x SharedPtr
	/// implementation.
	///
	/// Note: ReleasePolicy is no longer supported.
{
public:
	typedef C Type;

	SharedPtr()
	{
	}

	SharedPtr(C* ptr):
		_ptr(ptr)
	{
	}

	SharedPtr(const std::shared_ptr<C>& ptr):
		_ptr(ptr)
	{
	}

	template <class Other>
	SharedPtr(const SharedPtr<Other>& ptr):
		_ptr(std::dynamic_pointer_cast<C>(ptr.shared_ptr()))
	{
	}

	SharedPtr(const SharedPtr& ptr):
		_ptr(ptr._ptr)
	{
	}

	SharedPtr(SharedPtr&& ptr):
		_ptr(std::move(ptr._ptr))
	{
	}

	SharedPtr(std::shared_ptr<C>&& ptr):
		_ptr(std::move(ptr))
	{
	}

	~SharedPtr() noexcept
	{
	}

	SharedPtr& assign(C* ptr)
	{
		_ptr.reset(ptr);
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

	template <class Other>
	SharedPtr& assign(const SharedPtr<Other>& ptr)
	{
		if (ptr.get() != _ptr.get())
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	void reset() noexcept
	{
		_ptr.reset();
	}

	void reset(C* ptr)
	{
		_ptr.reset(ptr);
	}

	void reset(const SharedPtr& ptr)
	{
		assign(ptr);
	}

	template <class Other>
	void reset(const SharedPtr<Other>& ptr)
	{
		assign<Other>(ptr);
	}

	SharedPtr& operator = (C* ptr)
	{
		return assign(ptr);
	}

	SharedPtr& operator = (const SharedPtr& ptr)
	{
		return assign(ptr);
	}

	template <class Other>
	SharedPtr& operator = (const SharedPtr<Other>& ptr)
	{
		return assign<Other>(ptr);
	}

	SharedPtr& operator = (SharedPtr&& ptr)
	{
		_ptr = std::move(ptr._ptr);
		return *this;
	}

	SharedPtr& operator = (std::shared_ptr<C>&& ptr)
	{
		_ptr = std::move(ptr);
		return *this;
	}

	void swap(SharedPtr& ptr)
	{
		std::swap(_ptr, ptr._ptr);
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
		return SharedPtr(std::dynamic_pointer_cast<Other>(_ptr));
	}

	template <class Other>
	SharedPtr<Other> unsafeCast() const
		/// Casts the SharedPtr via a static cast to the given type.
		/// Example: (assume class Sub: public Super)
		///    SharedPtr<Super> super(new Sub());
		///    SharedPtr<Sub> sub = super.unsafeCast<Sub>();
		///    poco_assert (sub.get());
	{
		return SharedPtr(std::static_pointer_cast<Other>(_ptr));
	}

	operator std::shared_ptr<C>()
	{
		return _ptr;
	}

	const std::shared_ptr<C>& shared_ptr() const
	{
		return _ptr;
	}

	std::shared_ptr<C>& shared_ptr()
	{
		return _ptr;
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
		return _ptr.get();
	}

	const C* get() const
	{
		return _ptr.get();
	}

	operator C* ()
	{
		return _ptr.get();
	}

	operator const C* () const
	{
		return _ptr.get();
	}

	bool operator ! () const
	{
		return _ptr == 0;
	}

	bool isNull() const
	{
		return _ptr == 0;
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

	long referenceCount() const
	{
		return _ptr.use_count();
	}

protected:
	C* deref() const
	{
		if (!_ptr)
			throw NullPointerException();

		return _ptr.get();
	}

protected:
	std::shared_ptr<C> _ptr;
};


template <class C>
inline void swap(SharedPtr<C>& p1, SharedPtr<C>& p2)
{
	p1.swap(p2);
}


} // namespace Poco


#endif // Foundation_SharedPtr_INCLUDED
