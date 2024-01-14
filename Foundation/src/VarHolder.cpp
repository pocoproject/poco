//
// VarHolder.cpp
//
// Library: Foundation
// Package: Core
// Module:  VarHolder
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Dynamic/VarHolder.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Dynamic/Struct.h"
#include "Poco/JSONString.h"


namespace Poco {
namespace Dynamic {


#if defined(POCO_OS_FAMILY_WINDOWS)

template class Foundation_API Struct<std::string>;
template class Foundation_API Struct<int>;

template class Foundation_API Struct<std::string, Poco::OrderedMap<std::string, Var>, Poco::OrderedSet<std::string>>;
template class Foundation_API Struct<int, OrderedMap<int, Var>, OrderedSet<int>>;

#else

template class Struct<std::string>;
template class Struct<int>;

template class Struct<std::string, Poco::OrderedMap<std::string, Var>, Poco::OrderedSet<std::string>>;
template class Struct<int, OrderedMap<int, Var>, OrderedSet<int>>;

#endif


VarHolder::VarHolder()
{
}


VarHolder::~VarHolder()
{
}


namespace Impl {


void escape(std::string& target, const std::string& source)
{
	target = toJSON(source);
}


bool isJSONString(const Var& any)
{
	return any.type() == typeid(std::string) ||
		any.type() == typeid(char) ||
		any.type() == typeid(char*) ||
		any.type() == typeid(Poco::DateTime) ||
		any.type() == typeid(Poco::LocalDateTime) ||
		any.type() == typeid(Poco::Timestamp) ||
		any.type() == typeid(Poco::UUID);
}


void appendJSONString(std::string& val, const Var& any)
{
	std::string json;
	escape(json, any.convert<std::string>());
	val.append(json);
}


void appendJSONKey(std::string& val, const Var& any)
{
	return appendJSONString(val, any);
}


void appendJSONValue(std::string& val, const Var& any, bool wrap)
{
	if (any.isEmpty())
	{
		val.append("null");
	}
	else if (any.isString() && any.extract<std::string>().empty())
	{
		val.append("\"\"");
	}
	else
	{
		bool isStr = wrap && isJSONString(any);
		if (isStr)
		{
			appendJSONString(val, any.convert<std::string>());
		}
		else
		{
			val.append(any.convert<std::string>());
		}
	}
}


} // namespace Impl


} } // namespace Poco::Dynamic
