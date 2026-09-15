//
// RESTTestSuite.h
//
// Definition of the RESTTestSuite class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef RESTTestSuite_INCLUDED
#define RESTTestSuite_INCLUDED


#include "CppUnit/TestSuite.h"


class RESTTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // RESTTestSuite_INCLUDED
