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


#if defined(_DEBUG) && defined(POCO_REFCOUNT_NDC)


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
	/// for any other purpose. It is created on the heap and safely
	/// deleted by a custom atexit() handler.
	///
	/// See poco_rcdc_* set of macros below for explanation how it is used.
{
public:
	class TraceRecord
	{
	public:
		TraceRecord(void *ptr, int refCount, int threshold, const char* func, const std::string &backtrace = "");
		TraceRecord(const TraceRecord& other);
		TraceRecord(TraceRecord&& other);
		TraceRecord& operator == (const TraceRecord& other);
		TraceRecord& operator == (TraceRecord&& other);

	private:
		TraceRecord();

		void*       _ptr = 0;
		int         _refCount = 0;
		// threshold depends on whether duplicate()/release()
		// were ever called; if not, it is 0, because all
		// we've ever seen was refCount=1 in constructor
		int         _threshold = 0;
		std::string _functionName;
		std::string _backtrace;

		friend class RefCountDiagnosticContext;
	};

	typedef OrderedMap<void*, std::vector<TraceRecord>> TraceMap;
	typedef std::atomic<RefCountDiagnosticContext*>     Ptr;

	RefCountDiagnosticContext(bool full = false);
	~RefCountDiagnosticContext();

	void addEntry(TraceRecord&& record) const;
	void dumpRef(std::ostream &os, bool leakOnly = true) const;
	void dumpAllRef(std::ostream &os) const;
	void dumpLeakRef(std::ostream &os) const;
	void clear() const;

	static RefCountDiagnosticContext* get();

private:
	RefCountDiagnosticContext(const RefCountDiagnosticContext&) = delete;
	RefCountDiagnosticContext& operator = (const RefCountDiagnosticContext&) = delete;
	RefCountDiagnosticContext(RefCountDiagnosticContext&&) = delete;
	RefCountDiagnosticContext& operator = (RefCountDiagnosticContext&&) = delete;

	static TraceMap& traceMap() { return _traceMap; }

	static TraceMap _traceMap;
	static Mutex    _mutex;
	static Ptr      _pRCDC;
	static bool     _full;

	friend void atExitHandler();
};


//
// inlines
//

inline void RefCountDiagnosticContext::addEntry(TraceRecord&& record) const
{
	Mutex::ScopedLock l(_mutex);
	// if not full recording and this is the last record (ie. it is being
	// garbage collected) for this pointer, remove it;
	// full recording can get quite big and keeping collected garbage
	// records does not make much sense in most scenarios, but it is possible
	// by setting `_full` flag to true
	auto found = traceMap().find(record._ptr);
	if (found != traceMap().end() && !_full && record._refCount == record._threshold)
	{
		// memory has been released and full recording is disabled,
		// remove entry
		traceMap().erase(found);
	}
	else // otherwise, record it
	{
		traceMap()[record._ptr].emplace_back(std::move(record));
	}
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
	Mutex::ScopedLock l(_mutex);
	traceMap().clear();
}


typedef RefCountDiagnosticContext RCDC;


} // namespace Poco


// Diagnostic utility macros for tracing and managing records of refcount increments/decrements.
// To trim stack entries from beginning or end, adjust parameters to NDC::backTrace() below.
// Meaningfully usable only from classes that conform to the [Weak]RefCountedObject interface
// and functionality.
//
// Note: since access to the internal RefCountDiagnosticContext (static) storage is protected
// by a static mutex, using this functionality partially disables the multi-threaded nature
// of RefCounted access, so it should not be permanently enabled in code, but used as a temporary
// reference counting troubleshooting tool (compile with POCO_REFCOUNT_NDC in Config.h defined
// to globally enable/disable this functionality).
//
// Backtrace works only with g++ at this time; without it this functionality is of limited use.
// Disabled by default and always (even if POCO_REFCOUNT_NDC is defined) disabled for non-debug
// builds.

#define poco_rcdc_log(t) if (RCDC* p=RCDC::get()) p->addEntry(RCDC::TraceRecord((void*)this, referenceCount(), t, __func__, NDC::backtrace(1, 7)))

#define poco_rcdc_dump_leak(os) if (RCDC* p=RCDC::get()) p->dumpLeakRef(os)

#define poco_rcdc_dump_all(os) if (RCDC* p=RCDC::get()) p->dumpAllRef(os)

#define poco_rcdc_reset if (RCDC* p=RCDC::get()) p->clear();

#else // !(_DEBUG && POCO_REFCOUNT_NDC)


namespace Poco { typedef void RCDC; }

#define poco_rcdc_log(t)
#define poco_rcdc_dump_leak(os)
#define poco_rcdc_dump_all(os)
#define poco_rcdc_reset

#endif // _DEBUG && POCO_REFCOUNT_NDC


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
	typedef void MutexType;

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
	friend class RefCountDiagnosticContext;
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
	typedef M MutexType;

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
class RefCountedObjectImpl
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
	typedef typename T::MutexType MutexType;

	void duplicate() const
		/// Increments the object's reference count.
	{
		poco_assert_dbg(_counter.load() != 0);
		_counter.load()->duplicate();
		poco_rcdc_log(1);
	}

	void release() const throw()
		/// Decrements the object's reference count;
		/// deletes the object if the count reaches
		/// zero.
	{
		poco_assert_dbg(_counter.load() != 0);
		poco_rcdc_log(1);
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
		/// This function is not thread-safe.
	{
		T* pCounter = _counter.load();
		poco_assert_dbg (pCounter != 0);
		return pCounter ? pCounter->count() : 0;
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
		poco_rcdc_log(0);
	}

	virtual ~RefCountedObjectImpl()
		/// Destroys the RefCountedObjectImpl.
	{
		poco_rcdc_log(1);
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

	friend class RefCountDiagnosticContext;
};


// MS linker has trouble with duplicate symbols unless these are defined
// externally as fully specialized and inlined; unfortunately, this means
// same code in two places (ie. defeats the main purpose of templates).
// While this code could be factored out in standalone functions, it involves
// `delete this`, shapes everything in even uglier way and further complicates
// development and troubleshooting. So, for now, we live with this workaround
// annoyance.

#ifdef POCO_COMPILER_MSVC

template <>
inline RefCountedObjectImpl<RefCounter>::RefCountedObjectImpl() : _counter(new RefCounter)
{
	poco_rcdc_log(0);
}


template <>
inline RefCountedObjectImpl<RefCounter>::~RefCountedObjectImpl()
{
	poco_rcdc_log(1);
	delete _counter.load();
}


template <>
inline void RefCountedObjectImpl<RefCounter>::duplicate() const
{
	poco_assert_dbg(_counter.load() != 0);
	_counter.load()->duplicate();
	poco_rcdc_log(1);
}


template <>
inline void RefCountedObjectImpl<RefCounter>::release() const throw()
{
	poco_assert_dbg(_counter.load() != 0);
	poco_rcdc_log(1);
	if(_counter.load()->release() == 0)
	{
		_counter.store(0);
		delete this;
	}
}


template <>
inline int RefCountedObjectImpl<RefCounter>::referenceCount() const
{
	T* pCounter = _counter.load();
	poco_assert_dbg (pCounter != 0);
	return pCounter ? pCounter->count() : 0;
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
