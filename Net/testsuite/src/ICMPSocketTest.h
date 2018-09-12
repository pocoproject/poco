//
// ICMPSocketTest.h
//
// Definition of the ICMPSocketTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ICMPSocketTest_INCLUDED
#define ICMPSocketTest_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/CppUnit/TestCase.h"


class ICMPSocketTest: public CppUnit::TestCase
{
public:
	ICMPSocketTest(const std::string& name);
	~ICMPSocketTest();

	void testSendToReceiveFrom();
	void testAssign();
	void testMTU();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // ICMPSocketTest_INCLUDED
