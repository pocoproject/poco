//
// EventFilterTest.h
//
// Definition of the EventFilterTest class.
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef EventFilterTest_INCLUDED
#define EventFilterTest_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "CppUnit/TestCase.h"


class EventFilterTest: public CppUnit::TestCase
{
public:
	EventFilterTest(const std::string& name);
	~EventFilterTest();

	void testIsGreaterThanFilter();
	void testIsGreaterThanOrEqualToFilter();
	void testIsLessThanFilter();
	void testIsLessThanOrEqualToFilter();
	void testLogicalOrFilter();
	void testLogicalAndFilter();
	void testLogicalXorFilter();
	void testMinimumDeltaFilter();
	void testMinimumIntervalFilter();
	void testMinimumIntervalOrDeltaFilter();
	void testHysteresisFilter();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // EventFilterTest_INCLUDED
