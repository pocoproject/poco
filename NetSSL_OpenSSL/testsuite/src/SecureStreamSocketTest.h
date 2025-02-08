//
// SecureStreamSocketTest.h
//
// Definition of the SecureStreamSocketTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SecureStreamSocketTest_INCLUDED
#define SecureStreamSocketTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class SecureStreamSocketTest: public CppUnit::TestCase
{
public:
	SecureStreamSocketTest(const std::string& name);
	~SecureStreamSocketTest();

	void testSendReceive();
	void testPeek();
	void testNB();
	void testSendFile();
	void testSendFileLarge();
	void testSendFileRange();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // SecureStreamSocketTest
