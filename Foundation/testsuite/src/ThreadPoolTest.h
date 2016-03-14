//
// ThreadPoolTest.h
//
// $Id: //poco/1.4/Foundation/testsuite/src/ThreadPoolTest.h#1 $
//
// Definition of the ThreadPoolTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ThreadPoolTest_INCLUDED
#define ThreadPoolTest_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/CppUnit/TestCase.h"
#include "Poco/Event.h"
#include "Poco/Mutex.h"


class ThreadPoolTest: public CppUnit::TestCase
{
public:
	ThreadPoolTest(const std::string& name);
	~ThreadPoolTest();

	void testThreadPool();
	void testThreadPoolUniformDistribution();
	void testThreadPoolCustomDistribution();
	
	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

protected:
	void count();

private:
	void startThreadPoolTest(int affinityPolicy);
	Poco::FastMutex _mutex;
	Poco::Event _event;
	int   _count;
};


#endif // ThreadPoolTest_INCLUDED
