//
// MutexTest.h
//
// Definition of the MutexTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MutexTest_INCLUDED
#define MutexTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class MutexTest: public CppUnit::TestCase
{
public:
	MutexTest(const std::string& name);
	~MutexTest();

	void testScopedLockWithUnlockTryLock();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // MutexTest_INCLUDED
