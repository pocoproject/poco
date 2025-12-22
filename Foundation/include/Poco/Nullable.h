//
// Nullable.h
//
// Library: Foundation
// Package: Core
// Module:  Nullable
//
// Definition of the Nullable template class.
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Nullable_INCLUDED
#define Foundation_Nullable_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include <optional>
#include <iostream>


namespace Poco {

using NullType = std::nullopt_t;

template <typename C>
class Nullable
	/// Nullable is a simple wrapper class for value types
	/// that allows objects or native type variables
	/// to have "null" value.
	///
	/// The class is useful for passing parameters to functions
	/// when parameters are optional and no default values
	/// should be used or when a non-assigned state is needed,
	/// such as in e.g. fetching null values from database.
	///
	/// A Nullable can be default constructed. In this case,
	/// the Nullable will have a Null value and isNull() will
	/// return true. Calling value() (without default value) on
	/// a Null object will throw a NullValueException.
	///
	/// A Nullable can also be constructed from a value.
	/// It is possible to assign a value to a Nullable, and
	/// to reset a Nullable to contain a Null value by calling
	/// clear().
	///
	/// For use with Nullable, the value type should support
	/// default construction.
{
public:
	using Type = C;

	Nullable() = default;
		/// Creates an empty Nullable.

	Nullable(const NullType&)
		/// Creates an empty Nullable.
	{
	}

	Nullable(const C& value):
		/// Creates a Nullable with the given value.
		_optional(value)
	{
	}

	Nullable(C&& value):
		/// Creates a Nullable by moving the given value.
		_optional(std::forward<C>(value))
	{
	}

	Nullable(const Nullable& other):
		/// Creates a Nullable by copying another one.
		_optional(other._optional)
	{
	}

	Nullable(Nullable&& other) noexcept:
		/// Creates a Nullable by moving another one.
		_optional(std::move(other._optional))
	{
		other._optional.reset();
	}

	~Nullable() = default;
		/// Destroys the Nullable.

	Nullable& assign(const C& value)
		/// Assigns a value to the Nullable.
	{
		_optional.emplace(value);
		return *this;
	}

	Nullable& assign(C&& value)
		/// Assigns a value to the Nullable.
	{
		_optional.emplace(std::move(value));
		return *this;
	}

	Nullable& assign(const Nullable& other)
		/// Assigns another Nullable.
	{
		if (&other != this)
			_optional = other._optional;
		return *this;
	}

	Nullable& assign(NullType)
		/// Sets value to null.
	{
		_optional.reset();
		return *this;
	}

	Nullable& operator = (const C& value)
		/// Assigns a value to the Nullable.
	{
		return assign(value);
	}

	Nullable& operator = (C&& value)
		/// Move-assigns a value to the Nullable.
	{
		return assign(std::move(value));
	}

	Nullable& operator = (const Nullable& other)
		/// Assigns another Nullable.
	{
		return assign(other);
	}

	Nullable& operator = (Nullable&& other) noexcept
		/// Moves another Nullable.
	{
		_optional = std::move(other._optional);
		return *this;
	}

	Nullable& operator = (NullType)
		/// Assigns NullType.
	{
		_optional.reset();
		return *this;
	}

	void swap(Nullable& other) noexcept
		/// Swaps this Nullable with other.
	{
		std::swap(_optional, other._optional);
	}

	bool operator == (const Nullable<C>& other) const
		/// Compares two Nullables for equality
	{
		return _optional == other._optional;
	}

	bool operator == (const C& value) const
		/// Compares Nullable with value for equality
	{
		return (_optional.has_value() && _optional.value() == value);
	}

	bool operator == (const NullType&) const
		/// Compares Nullable with NullData for equality
	{
		return !_optional.has_value();
	}

	bool operator != (const C& value) const
		/// Compares Nullable with value for non equality
	{
		return !(*this == value);
	}

	bool operator != (const Nullable<C>& other) const
		/// Compares two Nullables for non equality
	{
		return !(*this == other);
	}

	bool operator != (const NullType&) const
		/// Compares with NullData for non equality
	{
		return _optional.has_value();
	}

	bool operator < (const Nullable<C>& other) const
		/// Compares two Nullable objects. Return true if this object's
		/// value is smaler than the other object's value.
		/// Null value is smaller than a non-null value.
	{
		return _optional < other._optional;
	}

	bool operator > (const Nullable<C>& other) const
		/// Compares two Nullable objects. Return true if this object's
		/// value is greater than the other object's value.
		/// A non-null value is greater than a null value.
	{
		return !(*this == other) && !(*this < other);
	}

	C& value()
		/// Returns the Nullable's value.
		///
		/// Throws a NullValueException if the Nullable is empty.
	{
		if (_optional.has_value())
			return _optional.value();
		else
			throw NullValueException();
	}

	const C& value() const
		/// Returns the Nullable's value.
		///
		/// Throws a NullValueException if the Nullable is empty.
	{
		if (_optional.has_value())
			return _optional.value();
		else
			throw NullValueException();
	}

	const C& value(const C& deflt) const
		/// Returns the Nullable's value, or the
		/// given default value if the Nullable is empty.
	{
		if (_optional.has_value())
			return _optional.value();

		return deflt;
	}

	explicit operator C& ()
		/// Get reference to the value
	{
		return value();
	}

	explicit operator const C& () const
		/// Get const reference to the value
	{
		return value();
	}

	operator const NullType& () const
		/// Get reference to the value
	{
		return _null;
	}

	bool isNull() const
		/// Returns true if the Nullable is empty.
	{
		return !_optional.has_value();
	}

	void clear()
		/// Clears the Nullable.
	{
		_optional.reset();
	}

private:
	std::optional<C> _optional;
	static constexpr NullType _null {std::nullopt};
};


template <typename C>
inline void swap(Nullable<C>& n1, Nullable<C>& n2) noexcept
{
	n1.swap(n2);
}


template <typename C>
std::ostream& operator<<(std::ostream& out, const Nullable<C>& obj)
{
	if (!obj.isNull()) out << obj.value();
	return out;
}


template <typename C>
bool operator == (const NullType&, const Nullable<C>& n)
	/// Returns true if this Nullable is null.
{
	return n.isNull();
}


template <typename C>
bool operator != (const C& c, const Nullable<C>& n)
	/// Compares Nullable with value for non equality
{
	return !(n == c);
}


template <typename C>
bool operator == (const C& c, const Nullable<C>& n)
	/// Compares Nullable with NullData for equality
{
	return (n == c);
}


template <typename C>
bool operator != (const NullType&, const Nullable<C>& n)
	/// Returns true if this Nullable is not null.
{
	return !n.isNull();
}


} // namespace Poco


#endif // Foundation_Nullable_INCLUDED
