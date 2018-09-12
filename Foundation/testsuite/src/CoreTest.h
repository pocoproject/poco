//
// CoreTest.h
//
// Definition of the CoreTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CoreTest_INCLUDED
#define CoreTest_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/CppUnit/TestCase.h"


class CoreTest: public CppUnit::TestCase
{
public:
	CoreTest(const std::string& name);
	~CoreTest();

	void testPlatform();
	void testFixedLength();
	void testBugcheck();
	void testFPE();
	void testEnvironment();
	void testBuffer();
	void testAtomicCounter();
	void testAtomicFlag();
	void testNullable();
	void testAscii();
	void testChecksum64();
	void testMakeUnique();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

protected:
	int _readableToNot;
	int _writableToNot;
	int _notToReadable;
	int _notToWritable;
};


#endif // CoreTest_INCLUDED
