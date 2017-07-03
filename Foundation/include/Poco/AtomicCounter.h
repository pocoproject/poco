//
// AtomicCounter.h
//
// $Id: //poco/1.4/Foundation/include/Poco/AtomicCounter.h#4 $
//
// Library: Foundation
// Package: Core
// Module:  AtomicCounter
//
// Definition of the AtomicCounter class.
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_AtomicCounter_INCLUDED
#define Foundation_AtomicCounter_INCLUDED


#include "Poco/Foundation.h"
#include <atomic>
#include "Poco/Mutex.h"


namespace Poco {


class Foundation_API AtomicCounter
#if defined(POCO_ENABLE_CPP11)
	: public std::atomic<int>
#endif
	/// This class implements a simple counter, which
	/// provides atomic operations that are safe to
	/// use in a multithreaded environment.
	///
	/// Typical usage of AtomicCounter is for implementing
	/// reference counting and similar things.
	///
	/// On some platforms, the implementation of AtomicCounter
	/// is based on atomic primitives specific to the platform
	/// (such as InterlockedIncrement, etc. on Windows), and
	/// thus very efficient. On platforms that do not support
	/// atomic primitives, operations are guarded by a FastMutex.
	///
	/// The following platforms currently have atomic
	/// primitives:
	///   - Windows
	///   - Mac OS X
	///   - GCC 4.1+ (Intel platforms only)
{
public:
	typedef int ValueType; /// The underlying integer type.
	
	AtomicCounter();
		/// Creates a new AtomicCounter and initializes it to zero.
		
	explicit AtomicCounter(ValueType initialValue);
		/// Creates a new AtomicCounter and initializes it with
		/// the given value.
	
	AtomicCounter(const AtomicCounter& counter);
		/// Creates the counter by copying another one.
	
	~AtomicCounter();
		/// Destroys the AtomicCounter.

	AtomicCounter& operator = (const AtomicCounter& counter);
		/// Assigns the value of another AtomicCounter.
		
	AtomicCounter& operator = (ValueType value);
		/// Assigns a value to the counter.

	ValueType value() const;
		/// Returns the value of the counter.
		
	bool operator ! () const;
		/// Returns true if the counter is zero, false otherwise.

};


//
// inlines
//


//
// C++11
//
inline AtomicCounter::ValueType AtomicCounter::value() const
{
	return load();
}


inline bool AtomicCounter::operator ! () const
{
	return load() == 0;
	return !*this;


} // namespace Poco


#endif // Foundation_AtomicCounter_INCLUDED
