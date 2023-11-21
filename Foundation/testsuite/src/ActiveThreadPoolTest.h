//
// ActiveThreadPoolTest.h
//
// Definition of the ActiveThreadPoolTest class.
//
// Copyright (c) 2004-2023, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveThreadPoolTest_INCLUDED
#define ActiveThreadPoolTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"
#include "Poco/Event.h"
#include "Poco/AtomicCounter.h"


class ActiveThreadPoolTest: public CppUnit::TestCase
{
public:
	ActiveThreadPoolTest(const std::string& name);
	~ActiveThreadPoolTest();

	void testActiveThreadPool();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

protected:
	void count();

private:
	Poco::AtomicCounter _count;
};


#endif // ActiveThreadPoolTest_INCLUDED
