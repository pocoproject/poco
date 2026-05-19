//
// MaxKey.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  MaxKey
//
// Definition of the MaxKey sentinel type.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_MaxKey_INCLUDED
#define MongoDB_MaxKey_INCLUDED


#include "Poco/MongoDB/Element.h"
#include "Poco/MongoDB/MongoDB.h"


namespace Poco::MongoDB {


struct MaxKey
	/// Sentinel BSON value (TypeId 0x7F) that compares higher than any other
	/// BSON value. Used by the server for shard-key chunk upper bounds, for
	/// open-ended index range queries, and in the output of administrative
	/// commands such as $indexStats. Has no payload.
{
};


template<>
struct ElementTraits<MaxKey>
{
	enum { TypeId = 0x7F };

	static std::string toString(const MaxKey& /*value*/, int /*indent*/ = 0)
	{
		return "{\"$maxKey\": 1}";
	}
};


template<>
inline void BSONReader::read<MaxKey>(MaxKey& /*to*/)
{
}


template<>
inline void BSONWriter::write<MaxKey>(const MaxKey& /*from*/)
{
}


} // namespace Poco::MongoDB


#endif // MongoDB_MaxKey_INCLUDED
