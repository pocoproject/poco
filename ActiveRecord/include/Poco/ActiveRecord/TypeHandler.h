//
// TypeHandler.h
//
// Library: ActiveRecord
// Package: ActiveRecord
// Module:  TypeHandler
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecord_TypeHandler_INCLUDED
#define ActiveRecord_TypeHandler_INCLUDED


#include "Poco/Data/TypeHandler.h"
#include "Poco/ThreadLocal.h"
#include "Poco/UUID.h"
#include <map>


namespace Poco {
namespace Data {


template <>
class TypeHandler<Poco::UUID>
{
public:
	using UUIDMap = std::map<std::size_t, std::string>;

	static std::size_t size()
	{
		return 1;
	}

	static void bind(std::size_t pos, const Poco::UUID& uuid, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		static Poco::ThreadLocal<UUIDMap> uuidMap;
		std::string& uuidString = (*uuidMap)[pos];
		uuidString = uuid.toString();
		TypeHandler<std::string>::bind(pos++, uuidString, pBinder, dir);
	}

	static void extract(std::size_t pos, Poco::UUID& uuid, const Poco::UUID& deflt, AbstractExtractor::Ptr pExtr)
	{
		std::string defltString = deflt.toString();
		std::string uuidString;
		TypeHandler<std::string>::extract(pos++, uuidString, defltString, pExtr);
		uuid.parse(uuidString);
	}

	static void prepare(std::size_t pos, const Poco::UUID& uuid, AbstractPreparator::Ptr pPrep)
	{
		static Poco::ThreadLocal<UUIDMap> uuidMap;
		std::string& uuidString = (*uuidMap)[pos];
		uuidString = uuid.toString();
		TypeHandler<std::string>::prepare(pos++, uuidString, pPrep);
	}
};


} } // namespace Poco::Data


#endif // ActiveRecord_TypeHandler_INCLUDED

