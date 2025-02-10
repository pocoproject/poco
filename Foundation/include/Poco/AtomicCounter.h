//
// AtomicCounter.h
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


namespace Poco {


class Foundation_API AtomicCounter
	/// This class implements a simple counter, which
	/// provides atomic operations that are safe to
	/// use in a multithreaded environment.
	///
	/// Typical usage of AtomicCounter is for implementing
	/// reference counting and similar functionality.
{
public:
	using ValueType = int; /// The underlying integer type.

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

	operator ValueType () const;
		/// Converts the AtomicCounter to ValueType.

	ValueType value() const;
		/// Returns the value of the counter.

	ValueType operator ++ (); // prefix
		/// Increments the counter and returns the result.

	ValueType operator ++ (int); // postfix
		/// Increments the counter and returns the previous value.

	ValueType operator -- (); // prefix
		/// Decrements the counter and returns the result.

	ValueType operator -- (int); // postfix
		/// Decrements the counter and returns the previous value.

	bool operator ! () const;
		/// Returns true if the counter is zero, false otherwise.

private:
	std::atomic<int> _counter;
};


//
// inlines
//

inline AtomicCounter::operator AtomicCounter::ValueType () const
{
	return _counter.load();
}


inline AtomicCounter::ValueType AtomicCounter::value() const
{
	return _counter.load();
}


inline AtomicCounter::ValueType AtomicCounter::operator ++ () // prefix
{
	return ++_counter;
}


inline AtomicCounter::ValueType AtomicCounter::operator ++ (int) // postfix
{
	return _counter++;
}


inline AtomicCounter::ValueType AtomicCounter::operator -- () // prefix
{
	return --_counter;
}


inline AtomicCounter::ValueType AtomicCounter::operator -- (int) // postfix
{
	return _counter--;
}


inline bool AtomicCounter::operator ! () const
{
	return _counter.load() == 0;
}


} // namespace Poco


#endif // Foundation_AtomicCounter_INCLUDED
