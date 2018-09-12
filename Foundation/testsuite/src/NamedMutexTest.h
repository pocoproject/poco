//
// NamedMutexTest.h
//
// Definition of the NamedMutexTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NamedMutexTest_INCLUDED
#define NamedMutexTest_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/CppUnit/TestCase.h"


class NamedMutexTest: public CppUnit::TestCase
{
public:
#if POCO != POCO_OS_CYGWIN
	NamedMutexTest(const std::string& name);
	~NamedMutexTest();

	void testLock();
	void testTryLock();
#endif
	
	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // NamedMutexTest_INCLUDED
