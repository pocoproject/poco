//
// Destructor.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
