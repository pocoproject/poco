//
// Destructor.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef CodeGen_Destructor_H_INCLUDED
#define CodeGen_Destructor_H_INCLUDED


#include "AbstractMethod.h"


class ClassInfo;


class Destructor: public AbstractMethod
{
public:
	Destructor(const ClassInfo& owner, Utility::Access acc, bool isVirtual);

	virtual ~Destructor();

private:
	Destructor();

	friend class ClassInfo;
};


#endif // CodeGen_Destructor_H_INCLUDED
