//
// Range.h
//
// Library: Data
// Package: DataCore
// Module:  Range
//
// Definition of the Range class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_Range_INCLUDED
#define SQL_Range_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/SQL/Limit.h"


namespace Poco {
namespace SQL {


class Poco_SQL_API Range
	/// Range stores information how many rows a query should return.
{
public:
	Range(Limit::SizeT lowValue, Limit::SizeT upValue, bool hardLimit);
		/// Creates the Range. lowValue must be smaller equal than upValue

	~Range();
		/// Destroys the Limit.

	const Limit& lower() const;
		/// Returns the lower limit

	const Limit& upper() const;
		/// Returns the upper limit

private:
	Limit _lower;
	Limit _upper;
};


//
// inlines
//
inline const Limit& Range::lower() const
{
	return _lower;
}


inline const Limit& Range::upper() const
{
	return _upper;
}


namespace Keywords {


template <typename T>
Limit limit(T lim, bool hard = false)
	/// Creates an upperLimit
{
	return Limit(static_cast<Limit::SizeT>(lim), hard, false);
}


template <typename T>
Limit upperLimit(T lim, bool hard = false)
{
	return limit(lim, hard);
}


template <typename T>
Limit lowerLimit(T lim)
{
	return Limit(static_cast<Limit::SizeT>(lim), true, true);
}


template <typename T>
Range range(T low, T upp, bool hard = false)
{
	return Range(static_cast<Limit::SizeT>(low), static_cast<Limit::SizeT>(upp), hard);
}


} // namespace Keywords


} } // namespace Poco::SQL


#endif // Data_Range_INCLUDED
