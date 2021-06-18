//
// IDTraits.h
//
// Library: ActiveRecord
// Package: ActiveRecord
// Module:  IDTraits
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecord_IDTraits_INCLUDED
#define ActiveRecord_IDTraits_INCLUDED


#include "Poco/Types.h"
#include "Poco/UUID.h"
#include "Poco/NumberFormatter.h"


namespace Poco {
namespace ActiveRecord {


template <typename T>
class IDTraits
	/// Traits for ID/index types.
	/// See specializations for details.
{
};


template <>
class IDTraits<Poco::UInt64>
{
public:
	static const Poco::UInt64 INVALID_ID;

	static bool isValid(Poco::UInt64 id)
	{
		return id != INVALID_ID;
	}

	static std::string toString(Poco::UInt64 id)
	{
		return Poco::NumberFormatter::format(id);
	}
};


template <>
class IDTraits<Poco::Int64>
{
public:
	static const Poco::Int64 INVALID_ID;

	static bool isValid(Poco::Int64 id)
	{
		return id != INVALID_ID;
	}

	static std::string toString(Poco::Int64 id)
	{
		return Poco::NumberFormatter::format(id);
	}
};


template <>
class IDTraits<Poco::UInt32>
{
public:
	static const Poco::UInt32 INVALID_ID;

	static bool isValid(Poco::UInt32 id)
	{
		return id != INVALID_ID;
	}

	static std::string toString(Poco::UInt32 id)
	{
		return Poco::NumberFormatter::format(id);
	}
};


template <>
class IDTraits<Poco::Int32>
{
public:
	static const Poco::Int32 INVALID_ID;

	static bool isValid(Poco::Int32 id)
	{
		return id != INVALID_ID;
	}

	static std::string toString(Poco::Int32 id)
	{
		return Poco::NumberFormatter::format(id);
	}
};


template <>
class IDTraits<Poco::UInt16>
{
public:
	static const Poco::UInt16 INVALID_ID;

	static bool isValid(Poco::UInt16 id)
	{
		return id != INVALID_ID;
	}

	static std::string toString(Poco::UInt16 id)
	{
		return Poco::NumberFormatter::format(id);
	}
};


template <>
class IDTraits<Poco::Int16>
{
public:
	static const Poco::Int16 INVALID_ID;

	static bool isValid(Poco::Int16 id)
	{
		return id != INVALID_ID;
	}

	static std::string toString(Poco::Int16 id)
	{
		return Poco::NumberFormatter::format(id);
	}
};


template <>
class IDTraits<std::string>
{
public:
	static const std::string INVALID_ID;

	static bool isValid(const std::string& id)
	{
		return !id.empty();
	}

	static std::string toString(const std::string& id)
	{
		return id;
	}
};


template <>
class IDTraits<Poco::UUID>
{
public:
	static const Poco::UUID INVALID_ID;

	static bool isValid(const Poco::UUID& id)
	{
		return !id.isNull();
	}

	static std::string toString(const Poco::UUID& id)
	{
		return id.toString();
	}
};


} } // namespace Poco::ActiveRecord


#endif // ActiveRecord_IDTraits_INCLUDED
