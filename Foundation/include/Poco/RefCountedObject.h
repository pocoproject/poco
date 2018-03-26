//
// RefCountedObject.h
//
// Library: Foundation
// Package: Core
// Module:  RefCountedObject
//
// Definition of the RefCountedObject class.
//
// Copyright (c) 2004-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_RefCountedObject_INCLUDED
#define Foundation_RefCountedObject_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Mutex.h"
#include <atomic>

/*
#if defined(_DEBUG) && defined(POCO_REFCOUNT_NDC)

#include "Poco/NestedDiagnosticContext.h"

// Diagnostic utility macro; it collects stack backtraces at every refcount increment/decrement.
// To trim stack entries from beginning or end, adjust paramters to NDC::backTrace() here.
// Usable only from classes that conform to the (Weak)RefCountedObject interface.
// Works only with g++ at this time.
#define poco_rc_ndc_dbg Poco::NDC::TraceRecord tr((void*)this, referenceCount(), "", Poco::NDC::backTrace(1, 7)); \
						Poco::NDC::TraceEntry entry({(uint64_t)this, tr}); Poco::NDC::addEntry(entry);

#else

#define poco_rc_ndc_dbg

#endif
*/

namespace Poco {


typedef std::atomic<int> RefCounterType;


//
// RefCounter
//


class Foundation_API RefCounter
	/// A class for thread-safe strong reference counting.
	///
	/// RefCounter is created from RefCountedObject and used
	/// internally as template parameter for RefCountedObjectImpl.
	/// It maintains the reference count and deletes itself when
	/// the count reaches zero.
{
public:
	RefCounter();

	void duplicate() const;

	int release();

	int count() const;

private:
	~RefCounter();

	RefCounter(const RefCounter&) = delete;
	RefCounter(RefCounter&&) = delete;
	RefCounter& operator = (const RefCounter&) = delete;
	RefCounter& operator = (RefCounter&&) = delete;

	mutable RefCounterType _counter;
	template <class T> friend class RefCountedObjectImpl;
};

//
// WeakRefCounter
//

template <typename M = Mutex>
class WeakRefCounter
	/// A class for thread-safe strong and weak reference
	/// counting.
	///
	/// WeakRefCounter is created from WeakRefCountedObject and
	/// used internally as template parameter for RefCountedObjectImpl.
	/// It maintains two reference counts, "strong" and "weak" and
	/// deletes itself when both counts reach zero.
	/// WeakRefCounter can be instantiated with Mutex (default, thread-safe),
	/// or NullMutex (no synchronization access from multiple threads).
{
public:
	typedef typename M::ScopedLock ScopedLock;

	WeakRefCounter(): _counter(1)
	{
	}

	void duplicate()
	{
		++_counter;
	}

	int release()
	{
		int counter = --_counter;
		if (counter == 0)
		{
			if (!_pWeakCounter || *_pWeakCounter == 0)
			{
				delete this;
				return 0;
			}
		}
		return counter;
	}

	WeakRefCounter* duplicateWeak()
	{
		ScopedLock l(_mutex);
		if (!_pWeakCounter) _pWeakCounter = new RefCounterType(1);
		else                ++*_pWeakCounter;
		return this;
	}

	WeakRefCounter* releaseWeak()
	{
		ScopedLockWithUnlock<M> l(_mutex);

		poco_check_ptr(_pWeakCounter);
		if (--*_pWeakCounter > 0) return this;
		poco_assert (*_pWeakCounter == 0);

		if(_counter.load() == 0)
		{
			l.unlock();
			delete this;
		}

		return 0;
	}

	int count() const
	{
		return _counter;
	}

	int weakCount() const
	{
		ScopedLock l(_mutex);
		if (_pWeakCounter) return *_pWeakCounter;
		return 0;
	}

private:
	~WeakRefCounter()
	{
		delete _pWeakCounter;
	}

	WeakRefCounter(const WeakRefCounter&);
	WeakRefCounter(WeakRefCounter&&);
	WeakRefCounter& operator = (const WeakRefCounter&);
	WeakRefCounter& operator = (WeakRefCounter&&);

	RefCounterType  _counter;
	RefCounterType* _pWeakCounter = 0;
	mutable M       _mutex;

	template <class T> friend class RefCountedObjectImpl;
};


//
// RefCountedObject
//


template <typename T>
class RefCountedObjectImpl//: public RefCountedObjectBase
	/// A base class for objects that employ
	/// reference counting based garbage collection.
	///
	/// Reference-counted objects inhibit construction
	/// by copying and assignment.
	///
	///  The semantic of the RefCountedImpl object
	///  and involved classes are:
	///
	///    - RefCountedObjectImpl can be instantiated as
	///      RefCountedObject (RCO) or WeakRefCountedobject (WRCO)
	///
	///    - RCO and WRCO must be created on the heap and can not be
	///      created on the stack
	///
	///    - RCO has one reference count; when the count reaches
	///      zero, object deletes itself
	///
	///    - WRCO has two reference counts, "strong" and "weak";
	///      when the strong count reaches zero, this object deletes
	///      itself; the reference counter, however, continues to exist
	///      until the last weak reference is released (see below)
	///
	///    - counts are maintained by objects (RefCounter and WeakRefCounter)
	///      used to instantiate the RefCountedObjectImpl template;
	///      these objects delete itself under normal circumstances
	///      (see below for an exception to normal circumstances)
	///
	///    - when instantiated using WeakRefCounter, the counter object
	///      may "outlive" this object if weak reference count is greater
	///      than zero at the time when strong reference count reaches zero
	///
	///    - there is a "corner" case when a [W]RCO object may be created as
	///      a pointer and deleted using delete operator, without release()
	///      ever be called; since this means that counter was never notified
	///      of the reference count change, it follows that it is the
	///      responsibility of the [W]RCO to delete the counter
{
public:
	typedef typename std::atomic<T*> CounterType;

	void duplicate() const
		/// Increments the object's reference count.
	{
		_counter.load()->duplicate();
	}

	void release() const throw()
		/// Decrements the object's reference count;
		/// deletes the object if the count reaches
		/// zero.
	{
		if(_counter.load()->release() == 0)
		{
			// disarm _counter delete in destructor
			// (_counter has deleted itself)
			_counter.store(0);
			delete this;
		}
	}

	int referenceCount() const
		/// Returns the reference count.
	{
		return _counter.load()->count();
	}

	T* counter() const
	{
		return _counter;
	}

protected:
	RefCountedObjectImpl(): _counter(new T)
		/// Creates the RefCountedObjectImpl.
		/// The initial reference count is one.
	{
	}

	virtual ~RefCountedObjectImpl()
		/// Destroys the RefCountedObjectImpl.
	{
		// prevent leak if release() was never called
		// (eg. this object new-ed into bare pointer
		// and deleted from outside using delete)
		// to prevent double delete, release() flags
		// access by setting the counter pointer to 0
		delete _counter.load();
	}

private:
	RefCountedObjectImpl(const RefCountedObjectImpl&) = delete;
	RefCountedObjectImpl& operator = (const RefCountedObjectImpl&) = delete;
	RefCountedObjectImpl(RefCountedObjectImpl&&) = delete;
	RefCountedObjectImpl& operator = (RefCountedObjectImpl&&) = delete;

	mutable CounterType _counter;
};


#ifdef POCO_COMPILER_MSVC

template <>
inline RefCountedObjectImpl<RefCounter>::RefCountedObjectImpl() : _counter(new RefCounter)
{
}


template <>
inline RefCountedObjectImpl<RefCounter>::~RefCountedObjectImpl()
{
	delete _counter.load();
}


template <>
inline void RefCountedObjectImpl<RefCounter>::duplicate() const
{
	_counter.load()->duplicate();
}


template <>
inline void RefCountedObjectImpl<RefCounter>::release() const throw()
{
	if (_counter.load()->release() == 0)
	{
		_counter.store(0);
		delete this;
	}
}


template <>
inline int RefCountedObjectImpl<RefCounter>::referenceCount() const
{
	return _counter.load()->count();
}


template <>
inline RefCounter* RefCountedObjectImpl<RefCounter>::counter() const
{
	return _counter;
}

#endif // POCO_COMPILER_MSVC


typedef RefCountedObjectImpl<WeakRefCounter<Mutex>> WeakRefCountedObject;
typedef WeakRefCountedObject WRCO;

typedef RefCountedObjectImpl<RefCounter> RefCountedObject;
typedef RefCountedObject RCO;


} // namespace Poco


#endif // Foundation_RefCountedObject_INCLUDED
