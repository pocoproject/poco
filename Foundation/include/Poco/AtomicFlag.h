//
// AtomicFlag.h
//
// Library: Foundation
// Package: Core
// Module:  AtomicFlag
//
// Definition of the AtomicFlag class.
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_AtomicFlag_INCLUDED
#define Foundation_AtomicFlag_INCLUDED


#include "Poco/Foundation.h"
#include <atomic>


namespace Poco {


class Foundation_API AtomicFlag
	/// This class implements an atomic boolean flag by wrapping
	/// the std::atomic_flag. It is guaranteed to be thread-safe
	/// and lock-free.
	///
	/// Only default-construction is allowed, objects of this
	/// class are not copyable, assignable or movable.
	///
	/// Note that this class is not a replacement for (atomic)
	/// bool type because its value can not be read without also
	/// being set to true. However, after being set (either
	/// explicitly, through the set() call or implicitly, via
	/// operator bool()), it can be reset and reused.
	///
	/// The class is useful in scenarios such as busy-wait or
	/// one-off code blocks, e.g.:
	///
	/// class MyClass
	/// {
	/// public:
	///     void myFunc()
	///     {
	///         if (!_beenHere) doOnce();
	///         doMany();
	///     }
	///
	///     void doOnce() { /* executed once */ }
	///
	///     void doMany() { /* executed multiple times */ }
	///
	/// private:
	///     AtomicFlag _beenHere;
	/// }
	///
	/// MyClass myClass;
	/// int i = 0;
	/// while (++i < 10) myClass.myFunc();
{
public:
	AtomicFlag() = default;
		/// Creates AtomicFlag.

	~AtomicFlag() = default;
		/// Destroys AtomicFlag.

	bool set();
		/// Sets the flag to true and returns previously
		/// held value.

	void reset();
		/// Resets the flag to false.

	operator bool ();
		/// Sets the flag to true and returns previously
		/// held value.

private:
	AtomicFlag(const AtomicFlag&) = delete;
	AtomicFlag& operator = (const AtomicFlag&) = delete;
	AtomicFlag(AtomicFlag&&) = delete;
	AtomicFlag& operator = (AtomicFlag&&) = delete;

	std::atomic_flag _flag = ATOMIC_FLAG_INIT;
};


//
// inlines
//


inline bool AtomicFlag::set()
{
	return _flag.test_and_set(std::memory_order_acquire);
}


inline void AtomicFlag::reset()
{
	_flag.clear(std::memory_order_release);
}


inline AtomicFlag::operator bool ()
{
	return set();
}


} // namespace Poco


#endif // Foundation_AtomicFlag_INCLUDED

