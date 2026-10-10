//
// SPSCQueueTest.h
//
// Definition of the SPSCQueueTest class.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SPSCQueueTest_INCLUDED
#define SPSCQueueTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class SPSCQueueTest: public CppUnit::TestCase
{
public:
	SPSCQueueTest(const std::string& name);
	~SPSCQueueTest();

	void testBasicPushPop();
	void testCapacity();
	void testFull();
	void testEmpty();
	void testMoveSemantics();
	void testEmplace();
	void testSPSCThreaded();
	void testHighThroughput();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // SPSCQueueTest_INCLUDED
