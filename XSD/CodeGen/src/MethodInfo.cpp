//
// MethodInfo.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
