//
// MPSCQueueTest.h
//
// Definition of the MPSCQueueTest class.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MPSCQueueTest_INCLUDED
#define MPSCQueueTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class MPSCQueueTest: public CppUnit::TestCase
{
public:
	MPSCQueueTest(const std::string& name);
	~MPSCQueueTest();

	void testBasicPushPop();
	void testCapacity();
	void testFull();
	void testEmpty();
	void testMoveSemantics();
	void testEmplace();
	void testSingleProducerSingleConsumer();
	void testMultipleProducers();
	void testHighContention();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // MPSCQueueTest_INCLUDED
