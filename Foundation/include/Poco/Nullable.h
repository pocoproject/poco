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
#include <algorithm>
#include <iostream>


namespace Poco {


enum NullType
{
	NULL_GENERIC = 0
};


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
{
public:
	Nullable(): _isNull(true)
		/// Creates an empty Nullable.
	{
		construct();
	}

	Nullable(const NullType&): _isNull(true)
		/// Creates an empty Nullable.
	{
		construct();
	}

	Nullable(const C& val): _isNull(true)
		/// Creates a Nullable with the given value.
	{
		construct(&val);
	}

	Nullable(const Nullable& other): _isNull(true)
		/// Creates a Nullable by copying another one.
	{
		construct(other);
	}

	~Nullable()
		/// Destroys the Nullable.
	{
		destruct();
	}

	Nullable& assign(const C& val)
		/// Assigns a value to the Nullable.
	{
		construct(&val);
		return *this;
	}

	Nullable& assign(const Nullable& other)
		/// Assigns another Nullable.
	{
		if (&other != this) construct(other);
		return *this;
	}

	Nullable& assign(NullType)
		/// Sets value to null.
	{
		destruct();
		return *this;
	}

	Nullable& operator = (const C& value)
		/// Assigns a value to the Nullable.
	{
		return assign(value);
	}

	Nullable& operator = (const Nullable& other)
		/// Assigns another Nullable.
	{
		return assign(other);
	}

	Nullable& operator = (NullType)
		/// Assigns another Nullable.
	{
		destruct();
		return *this;
	}

	bool operator == (const Nullable<C>& other) const
		/// Compares two Nullables for equality
	{
		return (_isNull && other._isNull) ||
			(!_isNull && !other._isNull && value() == other.value());
	}

	bool operator == (const C& val) const
		/// Compares Nullable with value for equality
	{
		return (!_isNull && value() == val);
	}

	bool operator == (const NullType&) const
		/// Compares Nullable with NullData for equality
	{
		return _isNull;
	}

	bool operator != (const C& val) const
		/// Compares Nullable with value for non equality
	{
		return !(*this == val);
	}

	bool operator != (const Nullable<C>& other) const
		/// Compares two Nullables for non equality
	{
		return !(*this == other);
	}

	bool operator != (const NullType&) const
		/// Compares with NullData for non equality
	{
		return !_isNull;
	}

	bool operator < (const Nullable<C>& other) const
		/// Compares two Nullable objects. Return true if this object's
		/// value is smaller than the other object's value.
		/// Null value is smaller than a non-null value.
	{
		if (_isNull && other._isNull) return false;

		if (!_isNull && !other._isNull)
			return (value() < other.value());

		if (_isNull && !other._isNull) return true;

		return false;
	}

	bool operator <= (const Nullable<C>& other) const
		/// Compares two Nullable objects. Return true if this object's
		/// value is smaller or equal than the other object's value.
		/// Null value is smaller than a non-null value.
	{
		if (_isNull && other._isNull) return true;

		if (!_isNull && !other._isNull)
			return ((value() < other.value()) || (value() == other.value()));

		if (_isNull && !other._isNull) return true;

		return false;
	}

	bool operator > (const Nullable<C>& other) const
		/// Compares two Nullable objects. Return true if this object's
		/// value is greater than the other object's value.
		/// A non-null value is greater than a null value.
	{
		return !(*this == other) && !(*this < other);
	}

	bool operator >= (const Nullable<C>& other) const
		/// Compares two Nullable objects. Return true if this object's
		/// value is greater or equal than the other object's value.
		/// A non-null value is greater than a null value.
	{
		return (*this > other) || (*this == other);
	}

	C& value()
		/// Returns the Nullable's value.
		///
		/// Throws a NullValueException if the Nullable is empty.
	{
		if (!_isNull)
			return *reinterpret_cast<C*>(_value);
		else
			throw NullValueException();
	}

	const C& value() const
		/// Returns the Nullable's value.
		///
		/// Throws a NullValueException if the Nullable is empty.
	{
		if (!_isNull)
			return *reinterpret_cast<const C*>(_value);
		else
			throw NullValueException();
	}

	const C& value(const C& deflt) const
		/// Returns the Nullable's value, or the
		/// given default value if the Nullable is empty.
	{
		return _isNull ? deflt : *reinterpret_cast<const C*>(_value);
	}

	operator C& ()
		/// Get reference to the value
	{
		return value();
	}

	operator const C& () const
		/// Get const reference to the value
	{
		return value();
	}

	operator NullType& ()
		/// Get reference to the value
	{
		return _null;
	}

	bool isNull() const
		/// Returns true if the Nullable is empty.
	{
		return _isNull;
	}

	void clear()
		/// Clears the Nullable.
	{
		destruct();
	}

private:
	void construct(const Nullable& val)
	{
		if (!val._isNull) construct(&val.value());
		else destruct();
	}

	void construct(const C* pVal = 0)
	{
		destruct();
		if (pVal)
		{
			new(_value) C(*pVal);
			_isNull = false;
		}
	}

	void destruct(bool init = true)
	{
		if (!_isNull)
		{
			value().~C();
			_isNull = true;
		}
	}

	char     _value[sizeof(C)];
	bool     _isNull;
	NullType _null;
};


template <typename C>
inline void swap(Nullable<C>& n1, Nullable<C>& n2)
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
