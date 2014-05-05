//
// VarHolder.cpp
//
// $Id: //poco/svn/Foundation/src/VarHolder.cpp#3 $
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


namespace Poco {
namespace Dynamic {


VarHolder::VarHolder()
{
}


VarHolder::~VarHolder()
{
}


namespace Impl {


bool isJSONString(const Var& any)
{
	return any.type() == typeid(std::string) || 
		any.type() == typeid(char) || 
		any.type() == typeid(char*) || 
		any.type() == typeid(Poco::DateTime) || 
		any.type() == typeid(Poco::LocalDateTime) || 
		any.type() == typeid(Poco::Timestamp);
}


void appendJSONString(std::string& val, const Var& any)
{
	val.append(1, '"');
	val.append(any.convert<std::string>());
	val.append(1, '"');
}


void appendJSONKey(std::string& val, const Var& any)
{
	return appendJSONString(val, any);
}


void appendJSONValue(std::string& val, const Var& any)
{
	if (any.isEmpty()) val.append("null");
	else 
	{
		bool isStr = isJSONString(any);
		if (isStr) val.append(1, '"');
		val.append(any.convert<std::string>());
		if (isStr) val.append(1, '"');
	}
}


} // namespace Impl


} } // namespace Poco::Dynamic
