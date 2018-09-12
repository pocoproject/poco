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
#include "Poco/MemoryPool.h"
#include "Poco/Mutex.h"
#include <atomic>


#if defined(_DEBUG) && defined(POCO_REFCOUNT_DC)
#define ENABLE_REFCOUNT_DC
#endif


#ifdef ENABLE_REFCOUNT_DC

#include "Poco/NestedDiagnosticContext.h"
#include "Poco/OrderedMap.h"
#include "Poco/Mutex.h"


namespace Poco {


//
// RefCountDiagnosticContext
//

class Foundation_API RefCountDiagnosticContext
	/// This utility class is used for debugging and testing purposes
	/// of the reference counting functionality and should not be used
	/// for any other purpose. it is enabled only for debug builds and
	/// if POCO_REFCOUNT_DC macro is defined in Config.h.
	///
	/// See poco_rcdc_* set of macros below for explanation how it is used.
{
public:
	class TraceRecord
	{
	public:
		TraceRecord(void *ptr, int refCount, const char* func, const std::string &backtrace = "");
		TraceRecord(const TraceRecord& other);
		TraceRecord(TraceRecord&& other);
		TraceRecord& operator == (const TraceRecord& other);
		TraceRecord& operator == (TraceRecord&& other);

	private:
		TraceRecord();

		void*       _ptr = 0;
		int         _refCount = 0;
		std::string _functionName;
		std::string _backtrace;

		friend class RefCountDiagnosticContext;
	};

	typedef OrderedMap<void*, std::vector<TraceRecord>> TraceMap;

	RefCountDiagnosticContext(bool full = false);
	~RefCountDiagnosticContext();

	void addEntry(TraceRecord&& record) const;
	void dumpRef(std::ostream &os, bool leakOnly = true) const;
	void dumpAllRef(std::ostream &os) const;
	void dumpLeakRef(std::ostream &os) const;
	void clear() const;

	// definition in Foundation.cpp
	static RefCountDiagnosticContext& get();

private:
	RefCountDiagnosticContext(const RefCountDiagnosticContext&) = delete;
	RefCountDiagnosticContext& operator = (const RefCountDiagnosticContext&) = delete;
	RefCountDiagnosticContext(RefCountDiagnosticContext&&) = delete;
	RefCountDiagnosticContext& operator = (RefCountDiagnosticContext&&) = delete;

	static TraceMap& traceMap() { return _traceMap; }

	// definitions in Foundation.cpp
	static TraceMap      _traceMap;
	static SpinlockMutex _mutex;
	static bool          _full;
};


//
// inlines
//

inline void RefCountDiagnosticContext::addEntry(TraceRecord&& record) const
{
	SpinlockMutex::ScopedLock l(_mutex);
	traceMap()[record._ptr].emplace_back(std::move(record));
}


inline void RefCountDiagnosticContext::dumpAllRef(std::ostream& os) const
{
	dumpRef(os, false);
	if (!_full)
	{
		os << std::endl;
		os << "Full dump requested but not recorded, leaks only (if any) displayed.";
		os << std::endl;
	}
}


inline void RefCountDiagnosticContext::dumpLeakRef(std::ostream& os) const
{
	dumpRef(os, true);
}


inline void RefCountDiagnosticContext::clear() const
{
	SpinlockMutex::ScopedLock l(_mutex);
	traceMap().clear();
}


typedef RefCountDiagnosticContext RCDC;


} // namespace Poco


// Diagnostic utility macros for tracing and managing records of refcount increments/decrements.
// To trim stack backtrace entries from beginning or end, adjust parameters to NDC::backTrace()
// below. Usable only for classes that inherit from [Weak]RefCountedObject.
//
// Note: since access to the internal RefCountDiagnosticContext (static) storage is protected
// by a static mutex, using this functionality partially disables the multi-threaded nature
// of RefCounted access, so it should never be permanently enabled in code, but used as a
// temporary reference counting troubleshooting tool (compile debug, with POCO_REFCOUNT_DC in
// Config.h defined to enable/disable this functionality).
//
// Backtrace works only with g++ at this time; without it this functionality is of limited use.
// Disabled by default for all builds and always (even if POCO_REFCOUNT_DC is defined) disabled
// for non-debug builds.

#define poco_rcdc_log RCDC::get().addEntry(RCDC::TraceRecord((void*)this, _counter, __func__, NDC::backtrace(1, 7)))

#define poco_rcdc_dump_leak(os) RCDC::get().dumpLeakRef(os)

#define poco_rcdc_dump_all(os) RCDC::get().dumpAllRef(os)

#define poco_rcdc_reset RCDC::get().clear();

#else // !ENABLE_REFCOUNT_DC


namespace Poco { typedef void RCDC; }

#define poco_rcdc_log
#define poco_rcdc_dump_leak(os)
#define poco_rcdc_dump_all(os)
#define poco_rcdc_reset

#endif // ENABLE_REFCOUNT_DC


namespace Poco {


//
// RefCountedObject
//


class RefCounter
	/// A class for thread-safe atomic reference counting.
{
public:
	RefCounter();
		/// Creates RefCounter.

	~RefCounter();
		/// Destroys RefCounter.

	int operator++ ();
		/// Prefix increment.

	int operator++ (int);
		/// Postfix increment.

	int operator-- ();
		/// Prefix decrement.

	int operator-- (int);
		/// Postfix decrement.

	operator int();
		/// Returns the current value as int.

private:
	RefCounter(const RefCounter&) = delete;
	RefCounter(RefCounter&&) = delete;
	RefCounter& operator = (const RefCounter&) = delete;
	RefCounter& operator = (RefCounter&&) = delete;

	mutable std::atomic<int> _counter;
};

//
// inlines
//

inline int RefCounter::operator++()
{
#ifdef ENABLE_REFCOUNT_DC
	int c = _counter.fetch_add(1, std::memory_order_relaxed) + 1;
	poco_rcdc_log;
	return c;
#endif
	return _counter.fetch_add(1, std::memory_order_relaxed) + 1;
}


inline int RefCounter::operator++(int)
{
#ifdef ENABLE_REFCOUNT_DC
	int c = _counter.fetch_add(1, std::memory_order_relaxed);
	poco_rcdc_log;
	return c;
#endif
	return _counter.fetch_add(1, std::memory_order_relaxed);
}


inline int RefCounter::operator--()
{
#ifdef ENABLE_REFCOUNT_DC
	int c = _counter.fetch_sub(1, std::memory_order_acquire) - 1;
	poco_rcdc_log;
	return c;
#endif
	return _counter.fetch_sub(1, std::memory_order_acquire) - 1;
}


inline int RefCounter::operator--(int)
{
#ifdef ENABLE_REFCOUNT_DC
	int c = _counter.fetch_sub(1, std::memory_order_acquire);
	poco_rcdc_log;
	return c;
#endif
	return _counter.fetch_sub(1, std::memory_order_acquire);
}


inline RefCounter::operator int()
{
	return _counter;
}


//
// RefCountedObject
//

class Foundation_API RefCountedObject
	/// A class for thread-safe strong reference counting.
	///
	/// It maintains the reference count, and deletes itself
	/// when the count reaches zero; it follows that a
	/// RefCountedObject must always be created on the heap.
{
public:
	RefCountedObject();
		/// Creates RefCountedObject

	void duplicate() const;
		/// Increments the reference counter.

	void release();
		/// Decrements the reference counter.
		/// If counter value after decrement is zero,
		/// this object is deleted.

	int referenceCount() const;
		/// Returns the reference count.

protected:
	virtual ~RefCountedObject();
		/// Destroys RefCountedObject

private:
	RefCountedObject(const RefCountedObject&) = delete;
	RefCountedObject(RefCountedObject&&) = delete;
	RefCountedObject& operator = (const RefCountedObject&) = delete;
	RefCountedObject& operator = (RefCountedObject&&) = delete;

	mutable RefCounter _counter;

	friend class RefCountDiagnosticContext;
};

//
// inlines
//

inline void RefCountedObject::duplicate() const
{
	_counter++;
}


inline void RefCountedObject::release()
{
	if (--_counter == 0) delete this;
}


inline int RefCountedObject::referenceCount() const
{
	return _counter;
}


typedef RefCountedObject RCO;


//
// WeakRefCounter
//


// global memory pool for weak counters;
// see definition in Foundation.cpp
template<typename T>
extern FastMemoryPool<T>& getFastMemoryPool();


class Foundation_API WeakRefCounter
	/// A class for thread-safe strong and weak reference
	/// counting.
	///
	/// WeakRefCounter is created on the heap from WeakRefCountedObject
	/// and may outlive the WeakRefCountedObject that created it.
	/// It maintains two reference counts, "strong" and "weak"; deletes
	/// itself when both counts reach zero. This is the only reference
	/// counting facility accessed by WeakRefPtr.
	///
	/// Note: this class has a very specific library-internal purpose;
	/// it is not meant for general use.
{
public:
	WeakRefCounter();
		/// Creates WeakRefCounter.

	void duplicate();
		/// Increments strong counter.

	int release();
		/// Decrements strong counter.
		/// If, after decrement, both strong and weak
		/// counters are zero, it deletes this object.

	WeakRefCounter* duplicateWeak();
		/// Increments weak counter.

	WeakRefCounter* releaseWeak();
		/// Decrements weak counter.
		/// If, after decrement, both strong and weak
		/// counters are zero, it deletes this object.

	int count() const;
		/// Rteturns the current value of the strong
		/// reference count.

	int weakCount() const;
		/// Rteturns the current value of the weak
		/// reference count.

private:
	~WeakRefCounter();
		/// Destroys WeakRefCounter.

	void* operator new(std::size_t);
		/// Returns a block of memory from FastMemoryPool.

	void operator delete(void* ptr);
		/// Returns the block of memory to the FastMemoryPool.

	void* operator new [] (std::size_t);
		/// Not used; must never be accessed.
		/// throws InvalidAccessException.

	void operator delete [] (void* ptr);
		/// Not used; does nothing.

	WeakRefCounter(const WeakRefCounter&);
	WeakRefCounter(WeakRefCounter&&);
	WeakRefCounter& operator = (const WeakRefCounter&);
	WeakRefCounter& operator = (WeakRefCounter&&);

	mutable RefCounter _counter;
	mutable RefCounter _weakCounter;

	friend class WeakRefCountedObject;
	template <typename T, class M> friend class FastMemoryPool;
};

//
// inlines
//

inline void WeakRefCounter::duplicate()
{
	_counter++;
}


inline int WeakRefCounter::release()
{
	if (--_counter == 0 && _weakCounter == 0)
	{
		delete this;
		return 0;
	}
	return _counter;
}


inline WeakRefCounter* WeakRefCounter::duplicateWeak()
{
	_weakCounter++;
	return this;
}


inline WeakRefCounter* WeakRefCounter::releaseWeak()
{
	if (--_weakCounter == 0 && _counter == 0)
	{
		delete this;
		return nullptr;
	}
	return this;
}


inline int WeakRefCounter::count() const
{
	return _counter;
}


inline int WeakRefCounter::weakCount() const
{
	return _weakCounter;
}


//
// RefCountedObject
//

class Foundation_API WeakRefCountedObject
	/// A base class for objects that employ weak/strong
	/// reference counting based garbage collection.
	///
	/// Reference-counted objects inhibit construction
	/// by copying and assignment.
	///
	///  The semantics of the RefCountedImpl object
	///  and involved classes are as follows:
	///
	///    - WeakRefCountedObject (WRCO) is used by WeakRefPtr and
	///      RefPtr; RefPtr accesses only WRCO, WeakRefPtr accesses
	///      only WeakRefCounter (which is created from this class
	///      and may outlive it if "strong" count reaches zero before
	///      the weak one)
	///
	///    - WRCO must be created on the heap and should never be
	///      created on the stack (if a user class inherits from WRCO,
	///      it should have protected destructor)
	///
	///    - both reference counts are maintained by WeakRefCounter,
	///      which deletes itself under normal circumstances
	///      (see below for an exception to normal circumstances)
	///
	///    - WeakRefCounter may "outlive" this object if weak reference
	///      count is greater than zero at the time when the strong
	///      reference count reaches zero
	///
	///    - WeakRefCounter will always delete itself. except in one "corner"
	///      case when a WRCO object is created outside of RefPtr/WealRefPtr
	///      wrappers and deleted using `delete` operator, without release()
	///      ever be called; since not calling release() means that the counter
	///      was never notified of the reference count change, it follows that
	///      it is the responsibility of this object to delete the counter
	///
	///    - reference counting is thread-safe; reference counted objects,
	///      as well as the smart pointers "wrapping" them, however, are not
	///      thread-safe; access to null counter pointer means there is a
	///      bug somewhere in user application (there are debug build checks
	///      indicating such circumstances, but no non-debug checks,
	///      so - thread carefully!)
	///
	///    - mutexes are not used, so there is no performance difference between
	///      single- and multi-threaded modes of operation; note that, depending
	///      on the underlying atomics implementation, locking may still happen
	///      in both modes
{
public:
	void duplicate() const;
		/// Increments the object's reference count.

	void release() const throw();
		/// Decrements the object's reference count;
		/// deletes the object if the count reaches
		/// zero.

	int referenceCount() const;
		/// Returns the reference count.

	WeakRefCounter* counter() const;
		/// Return pointer to counter.

protected:

	WeakRefCountedObject();
		/// Creates the RefCountedObjectImpl.
		/// The initial reference count is one.;

	virtual ~WeakRefCountedObject();
		/// Destroys the RefCountedObjectImpl.

private:
	WeakRefCountedObject(const WeakRefCountedObject&) = delete;
	WeakRefCountedObject& operator = (const WeakRefCountedObject&) = delete;
	WeakRefCountedObject(WeakRefCountedObject&&) = delete;
	WeakRefCountedObject& operator = (WeakRefCountedObject&&) = delete;

	mutable std::atomic<WeakRefCounter*> _pCounter;

	friend class RefCountDiagnosticContext;
	template <typename T, class M> friend class FastMemoryPool;
};


typedef WeakRefCountedObject WRCO;


//
// inlines
//

inline void WeakRefCountedObject::duplicate() const
{
	poco_assert_dbg(_pCounter.load() != 0);
	_pCounter.load()->duplicate();
}


inline void WeakRefCountedObject::release() const throw()
{
	poco_assert_dbg(_pCounter.load() != 0);
	if(_pCounter.load()->release() == 0)
	{
		// _pCounter has deleted itself, disarm
		// the delete in the destructor
		_pCounter = 0;
		delete this;
		return;
	}
}


inline int WeakRefCountedObject::referenceCount() const
{
	return _pCounter.load()->count();
}


inline WeakRefCounter* WeakRefCountedObject::counter() const
{
	return _pCounter;
}


} // namespace Poco


#endif // Foundation_RefCountedObject_INCLUDED
