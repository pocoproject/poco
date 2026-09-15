//
// JSONRPCTestSuite.h
//
// Definition of the JSONRPCTestSuite class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef JSONRPCTestSuite_INCLUDED
#define JSONRPCTestSuite_INCLUDED


#include "CppUnit/TestSuite.h"


class JSONRPCTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // JSONRPCTestSuite_INCLUDED
