//
// BulkExtraction.h
//
// Library: Data
// Package: DataCore
// Module:  Bulk
//
// Definition of the BulkExtraction class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_Bulk_INCLUDED
#define Data_Bulk_INCLUDED


#include "Poco/Void.h"
#include "Poco/Data/Limit.h"


namespace Poco {
namespace Data {


class Data_API Bulk
{
public:
	Bulk() = delete;

	Bulk(const Limit& limit);
		/// Creates the Bulk.

	Bulk(Poco::UInt32 value);
		/// Creates the Bulk.

	~Bulk();
		/// Destroys the bulk.

	[[nodiscard]]
	const Limit& limit() const;
		/// Returns the limit asociated with this bulk object.

	[[nodiscard]]
	Poco::UInt32 size() const;
		/// Returns the value of the limit asociated with
		/// this bulk object.

private:
	Limit _limit;
};


///
/// inlines
///
inline const Limit& Bulk::limit() const
{
	return _limit;
}


inline Poco::UInt32 Bulk::size() const
{
	return _limit.value();
}


namespace Keywords {

[[nodiscard]]
inline Bulk bulk(const Limit& limit = Limit(Limit::LIMIT_UNLIMITED, false, false))
	/// Convenience function for creation of bulk.
{
	return {limit};
}


inline void bulk([[maybe_unused]] Void v)
	/// Dummy bulk function. Used for bulk binding creation
	/// (see BulkBinding) and bulk extraction signalling to Statement.
{
}


} // namespace Keywords


using BulkFnType = void (*)(Void);


} } // namespace Poco::Data


#endif // Data_Bulk_INCLUDED
