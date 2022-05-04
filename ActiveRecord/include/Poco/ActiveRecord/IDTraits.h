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


#include "Poco/ActiveRecord/ActiveRecordLib.h"
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
	static constexpr Poco::UInt64 INVALID_ID = std::numeric_limits<Poco::UInt64>::max();

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
	static constexpr Poco::Int64 INVALID_ID = std::numeric_limits<Poco::Int64>::max();

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
	static constexpr Poco::UInt32 INVALID_ID = std::numeric_limits<Poco::UInt32>::max();

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
	static const Poco::Int32 INVALID_ID = std::numeric_limits<Poco::Int32>::max();

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
	static constexpr Poco::UInt16 INVALID_ID = std::numeric_limits<Poco::UInt16>::max();

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
	static constexpr Poco::Int16 INVALID_ID = std::numeric_limits<Poco::Int16>::max();

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
	static ActiveRecordLib_API const std::string INVALID_ID;

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
	static ActiveRecordLib_API const Poco::UUID INVALID_ID;

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
