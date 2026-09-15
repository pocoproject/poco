//
// RemotingTestSuite.h
//
// Definition of the RemotingTestSuite class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingTestSuite_INCLUDED
#define RemotingTestSuite_INCLUDED


#ifndef CppUnit_TestSuite_INCLUDED
#include "CppUnit/TestSuite.h"
#endif


class RemotingTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // RemotingTestSuite_INCLUDED
