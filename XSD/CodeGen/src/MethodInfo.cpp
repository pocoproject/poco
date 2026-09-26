//
// MethodInfo.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MethodInfo.h"
#include "Poco/String.h"


MethodInfo::MethodInfo(const std::string& name, Utility::Access acc, bool isConst, bool isStatic, bool isVirtual, bool isAbstract):
	AbstractMethod(name, acc, isConst, isStatic, isVirtual, isAbstract)
{
}


MethodInfo::~MethodInfo()
{
}
