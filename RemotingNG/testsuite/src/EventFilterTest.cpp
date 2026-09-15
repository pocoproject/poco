//
// EventFilterTest.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "EventFilterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/EventFilter.h"
#include "Poco/Thread.h"


EventFilterTest::EventFilterTest(const std::string& name): CppUnit::TestCase(name)
{
}


EventFilterTest::~EventFilterTest()
{
}


void EventFilterTest::setUp()
{
}


void EventFilterTest::tearDown()
{
}


void EventFilterTest::testIsGreaterThanFilter()
{
	Poco::RemotingNG::IsGreaterThanFilter<int> filter(100);
	assert (filter.accept(101));
	assert (!filter.accept(100));
	assert (!filter.accept(99));
}


void EventFilterTest::testIsGreaterThanOrEqualToFilter()
{
	Poco::RemotingNG::IsGreaterThanOrEqualToFilter<int> filter(100);
	assert (filter.accept(100));
	assert (filter.accept(101));
	assert (!filter.accept(99));
}


void EventFilterTest::testIsLessThanFilter()
{
	Poco::RemotingNG::IsLessThanFilter<int> filter(100);
	assert (!filter.accept(100));
	assert (!filter.accept(101));
	assert (filter.accept(99));
}


void EventFilterTest::testIsLessThanOrEqualToFilter()
{
	Poco::RemotingNG::IsLessThanOrEqualToFilter<int> filter(100);
	assert (filter.accept(100));
	assert (!filter.accept(101));
	assert (filter.accept(99));
}


void EventFilterTest::testLogicalOrFilter()
{
	Poco::RemotingNG::LogicalOrFilter<int> filter(
		new Poco::RemotingNG::IsLessThanFilter<int>(100),
		new Poco::RemotingNG::IsGreaterThanFilter<int>(100));
	assert (!filter.accept(100));
	assert (filter.accept(101));
	assert (filter.accept(99));
}


void EventFilterTest::testLogicalAndFilter()
{
	Poco::RemotingNG::LogicalAndFilter<int> filter(
		new Poco::RemotingNG::IsLessThanFilter<int>(101),
		new Poco::RemotingNG::IsGreaterThanFilter<int>(99));
	assert (filter.accept(100));
	assert (!filter.accept(101));
	assert (!filter.accept(99));
}


void EventFilterTest::testLogicalXorFilter()
{
	Poco::RemotingNG::LogicalXorFilter<int> filter(
		new Poco::RemotingNG::IsLessThanFilter<int>(101),
		new Poco::RemotingNG::IsGreaterThanFilter<int>(99));
	assert (!filter.accept(100));
	assert (filter.accept(101));
	assert (filter.accept(99));
}


void EventFilterTest::testMinimumDeltaFilter()
{
	Poco::RemotingNG::MinimumDeltaFilter<int> filter(10);
	assert (filter.accept(0));
	assert (!filter.accept(1));
	assert (!filter.accept(2));
	assert (filter.accept(10));
	assert (!filter.accept(11));
	assert (!filter.accept(9));
	assert (filter.accept(0));
}


void EventFilterTest::testMinimumIntervalFilter()
{
	Poco::RemotingNG::MinimumIntervalFilter<int> filter(100000);
	assert (filter.accept(0));
	assert (!filter.accept(0));
	assert (!filter.accept(1));
	Poco::Thread::sleep(110);
	assert (filter.accept(0));
	assert (!filter.accept(0));
}


void EventFilterTest::testMinimumIntervalOrDeltaFilter()
{
	Poco::RemotingNG::MinimumIntervalOrDeltaFilter<int> filter(100000, 10);
	assert (filter.accept(0));
	assert (!filter.accept(0));
	assert (!filter.accept(1));
	assert (filter.accept(10));
	Poco::Thread::sleep(110);
	assert (filter.accept(10));
	assert (!filter.accept(9));
	assert (filter.accept(0));
}


void EventFilterTest::testHysteresisFilter()
{
	Poco::RemotingNG::HysteresisFilter<int> filter(18, 22);
	assert (filter.accept(17));
	assert (!filter.accept(16));
	assert (!filter.accept(18));
	assert (!filter.accept(19));
	assert (!filter.accept(20));
	assert (!filter.accept(21));
	assert (!filter.accept(22));
	assert (filter.accept(23));
	assert (!filter.accept(24));
	assert (!filter.accept(20));
	assert (!filter.accept(18));
	assert (filter.accept(17));
	assert (!filter.accept(20));
	assert (filter.accept(24));
}


CppUnit::Test* EventFilterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("EventFilterTest");

	CppUnit_addTest(pSuite, EventFilterTest, testIsGreaterThanFilter);
	CppUnit_addTest(pSuite, EventFilterTest, testIsGreaterThanOrEqualToFilter);
	CppUnit_addTest(pSuite, EventFilterTest, testIsLessThanFilter);
	CppUnit_addTest(pSuite, EventFilterTest, testIsLessThanOrEqualToFilter);
	CppUnit_addTest(pSuite, EventFilterTest, testLogicalOrFilter);
	CppUnit_addTest(pSuite, EventFilterTest, testLogicalAndFilter);
	CppUnit_addTest(pSuite, EventFilterTest, testLogicalXorFilter);
	CppUnit_addTest(pSuite, EventFilterTest, testMinimumDeltaFilter);
	CppUnit_addTest(pSuite, EventFilterTest, testMinimumDeltaFilter);
	CppUnit_addTest(pSuite, EventFilterTest, testMinimumIntervalFilter);
	CppUnit_addTest(pSuite, EventFilterTest, testMinimumIntervalOrDeltaFilter);
	CppUnit_addTest(pSuite, EventFilterTest, testHysteresisFilter);

	return pSuite;
}
