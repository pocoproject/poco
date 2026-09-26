//
// ClassInfo.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "ClassInfo.h"


ClassInfo::ClassInfo(const std::string& name, 
		const std::string& nameSpace, 
		const std::string& schemaNameSpace,
		const std::string& includeFile, 
		const std::string& dllExportMacro):
	_variables(),
	_methods(),
	_constructors(),
	_destructor(),
	_info(name, nameSpace, schemaNameSpace, "", includeFile, false, false, false),
	_parent(),
	_dllExportMacro(dllExportMacro)
{
	_destructor = Destructor(*this, Utility::AC_PUBLIC, true);
}


ClassInfo::~ClassInfo()
{
}


void ClassInfo::addConstructor(const Constructor& constr)
{
	poco_assert (constr.name() == name());
	_constructors.push_back(constr);
}


void ClassInfo::setDestructor(Utility::Access acc, bool isVirtual)
{
	_destructor = Destructor(*this, acc, isVirtual);
}


void ClassInfo::addFwdDeclare(const std::string& className, const std::string& nameSpace, const std::string& include)
{
	poco_assert_dbg (!className.empty());
	poco_assert_dbg (!include.empty());
	if (!nameSpace.empty() && nameSpace != getNameSpace())
	{
		_fwdDeclarations.insert(nameSpace + className);
	}
	else
		_fwdDeclarations.insert(className);

	addSrcInclude(include, false);
}
