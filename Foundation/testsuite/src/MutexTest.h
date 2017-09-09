//
// MutexTest.h
//
// Definition of the MutexTest class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MutexTest_INCLUDED
#define MutexTest_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/CppUnit/TestCase.h"


class MutexTest: public CppUnit::TestCase
{
public:
	MutexTest(const std::string& name);
	~MutexTest();

	void testMutexRecursion();
	void testRecursiveMutexRecursion();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // MutexTest_INCLUDED
