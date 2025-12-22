//
// SpinlockMutexTest.h
//
// Definition of the SpinlockMutexTest class.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SpinlockMutexTest_INCLUDED
#define SpinlockMutexTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class SpinlockMutexTest: public CppUnit::TestCase
{
public:
	SpinlockMutexTest(const std::string& name);
	~SpinlockMutexTest();

	void testLock();
	void testTryLock();
	void testLockTimeout();
	void testTryLockTimeout();
	void testAdaptiveSpinning();
	void testContention();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // SpinlockMutexTest_INCLUDED
