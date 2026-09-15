//
// RemotingTestSuite.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "RemotingTestSuite.h"
#include "RemotingTest.h"
#include "EventFilterTest.h"
#include "URIUtilityTest.h"


CppUnit::Test* RemotingTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestSuite");

	pSuite->addTest(RemotingTest::suite());
	pSuite->addTest(EventFilterTest::suite());
	pSuite->addTest(URIUtilityTest::suite());

	return pSuite;
}
