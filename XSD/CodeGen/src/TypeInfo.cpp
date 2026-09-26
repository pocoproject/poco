//
// TypeInfo.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TypeInfo.h"


TypeInfo::TypeInfo():
	_name(),
	_nameSpace(),
	_schemaNameSpace(),
	_fullName(),
	_xsdType(),
	_includeFile(),
	_isSystemInclude(false),
	_isVector(false),
	_isNullable(false),
	_isScalar(false)
{
}


TypeInfo::TypeInfo(const std::string& name, const std::string& nameSpace, const std::string& schemaNameSpace, const std::string& xsdType, const std::string& includeFile, bool isSystemInclude, bool isVector, bool isScalar):
	_name(name),
	_nameSpace(nameSpace),
	_schemaNameSpace(schemaNameSpace),
	_fullName(nameSpace + "::" + name),
	_xsdType(xsdType),
	_includeFile(includeFile),
	_isSystemInclude(isSystemInclude),
	_isVector(isVector),
	_isNullable(false),
	_isScalar(isScalar)
{
}


TypeInfo::~TypeInfo()
{
}
