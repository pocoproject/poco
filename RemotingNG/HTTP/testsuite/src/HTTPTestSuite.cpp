//
// HTTPTestSuite.cpp
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "HTTPTestSuite.h"
#include "RemotingTest.h"


CppUnit::Test* HTTPTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPTestSuite");

	pSuite->addTest(RemotingTestHTTP::suite());
	pSuite->addTest(RemotingTestHTTPCompressed::suite());
	pSuite->addTest(RemotingTestHTTPNonChunked::suite());
	pSuite->addTest(RemotingTestHTTPAuth::suite());
	pSuite->addTest(RemotingTestHTTPAuthChunked::suite());

	return pSuite;
}
