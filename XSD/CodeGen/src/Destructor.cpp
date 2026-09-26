//
// Destructor.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Destructor.h"
#include "ClassInfo.h"
#include "Poco/String.h"


Destructor::Destructor(const ClassInfo& owner, Utility::Access acc, bool isVirtual):
	AbstractMethod(std::string("~")+owner.name(), acc, false, false, isVirtual, false)
{
}


Destructor::Destructor():
	AbstractMethod(std::string("~"), Utility::AC_PUBLIC, false, false, true, false)
{
}


Destructor::~Destructor()
{
}
