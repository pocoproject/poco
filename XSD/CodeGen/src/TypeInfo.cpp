//
// TypeInfo.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "TypeInfo.h"


TypeInfo::TypeInfo():
	_name(),
	_nameSpace(),
	_fullName(),
	_includeFile(),
	_isSystemInclude(false),
	_isVector(false),
	_isNullable(false)
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
