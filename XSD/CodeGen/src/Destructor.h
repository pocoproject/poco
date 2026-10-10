//
// Destructor.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_Destructor_H_INCLUDED
#define CodeGen_Destructor_H_INCLUDED


#include "AbstractMethod.h"


class ClassInfo;


class Destructor: public AbstractMethod
{
public:
	Destructor(const ClassInfo& owner, Utility::Access acc, bool isVirtual);

private:
	Destructor();

	friend class ClassInfo;
};


#endif // CodeGen_Destructor_H_INCLUDED
