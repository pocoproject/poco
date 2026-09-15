//
// HTTPTestSuite.h
//
// Definition of the HTTPTestSuite class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef HTTPTestSuite_INCLUDED
#define HTTPTestSuite_INCLUDED


#include "CppUnit/TestSuite.h"


class HTTPTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // HTTPTestSuite_INCLUDED
