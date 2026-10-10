//
// MinKey.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  MinKey
//
// Definition of the MinKey sentinel type.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_MinKey_INCLUDED
#define MongoDB_MinKey_INCLUDED


#include "Poco/MongoDB/Element.h"
#include "Poco/MongoDB/MongoDB.h"


namespace Poco::MongoDB {


struct MinKey
	/// Sentinel BSON value (TypeId 0xFF) that compares lower than any other
	/// BSON value. Used by the server for shard-key chunk lower bounds, for
	/// open-ended index range queries, and in the output of administrative
	/// commands such as $indexStats. Has no payload.
{
};


template<>
struct ElementTraits<MinKey>
{
	enum { TypeId = 0xFF };

	static std::string toString(const MinKey& /*value*/, int /*indent*/ = 0)
	{
		return "{\"$minKey\": 1}";
	}
};


template<>
inline void BSONReader::read<MinKey>(MinKey& /*to*/)
{
}


template<>
inline void BSONWriter::write<MinKey>(const MinKey& /*from*/)
{
}


} // namespace Poco::MongoDB


#endif // MongoDB_MinKey_INCLUDED
