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
#include "Poco/AtomicCounter.h"

#if defined(_DEBUG) && defined(POCO_REFCOUNT_NDC)

#include "Poco/NestedDiagnosticContext.h"

// Diagnostic utility macro; it collects stack backtraces at every refcount increment/decrement.
// To trim stack entries from beginning or end, adjust paramters to NDC::backTrace() here.
// Works only with g++ at this time.
#define poco_rc_ndc_dbg NDC::TraceRecord tr((void*)this, referenceCount(), "", NDC::backTrace(1, 7)); \
						NDC::TraceEntry entry({(uint64_t)this, tr}); NDC::addEntry(entry);

#else

#define poco_rc_ndc_dbg

#endif


namespace Poco {


class Foundation_API RefCountedObject
	/// A base class for objects that employ
	/// reference counting based garbage collection.
	///
	/// Reference-counted objects inhibit construction
	/// by copying and assignment.
{
public:
	RefCountedObject();
		/// Creates the RefCountedObject.
		/// The initial reference count is one.

	void duplicate() const;
		/// Increments the object's reference count.

	void release() const throw();
		/// Decrements the object's reference count
		/// and deletes the object if the count
		/// reaches zero.

	int referenceCount() const;
		/// Returns the reference count.

protected:
	virtual ~RefCountedObject();
		/// Destroys the RefCountedObject.

private:
	RefCountedObject(const RefCountedObject&);
	RefCountedObject& operator = (const RefCountedObject&);
	RefCountedObject(RefCountedObject&&);
	RefCountedObject& operator = (RefCountedObject&&);

	mutable AtomicCounter _counter;
};


//
// inlines
//
inline int RefCountedObject::referenceCount() const
{
	return _counter.value();
}


inline void RefCountedObject::duplicate() const
{
	++_counter;
	poco_rc_ndc_dbg;
}


inline void RefCountedObject::release() const throw()
{
	try
	{
		poco_rc_ndc_dbg;
		poco_assert_dbg(referenceCount() > 0);
		if (--_counter == 0) delete this;
	}
	catch (...)
	{
		poco_unexpected();
	}
}


} // namespace Poco


#endif // Foundation_RefCountedObject_INCLUDED
