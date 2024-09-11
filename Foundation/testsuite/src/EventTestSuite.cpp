//
// EventTestSuite.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "EventTestSuite.h"
#if defined(POCO_TEST_DEPRECATED)
#include "FIFOEventTest.h"
#endif
#include "BasicEventTest.h"
#include "PriorityEventTest.h"

CppUnit::Test* EventTestSuite::suite()
{
	auto* pSuite = new CppUnit::TestSuite("EventTestSuite");

	pSuite->addTest(BasicEventTest::suite());
	pSuite->addTest(PriorityEventTest::suite());

#if defined(POCO_TEST_DEPRECATED)
	pSuite->addTest(FIFOEventTest::suite());
#endif

	return pSuite;
}
