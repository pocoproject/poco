//
// DatagramSocketTest.h
//
// Definition of the DatagramSocketTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DatagramSocketTest_INCLUDED
#define DatagramSocketTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class DatagramSocketTest: public CppUnit::TestCase
{
public:
	DatagramSocketTest(const std::string& name);
	~DatagramSocketTest();

	void testEcho();
	void testEchoBuffer();
	void testSendToReceiveFrom();
	void testUnbound();
	void testBroadcast();
	void testGatherScatterFixed();
	void testGatherScatterVariable();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	// "STRF" are sendto/recvfrom versions of the same functionality
	void testGatherScatterFixedWin();
	void testGatherScatterSTRFFixedWin();
	void testGatherScatterVariableWin();
	void testGatherScatterSTRFVariableWin();

	void testGatherScatterFixedUNIX();
	void testGatherScatterSTRFFixedUNIX();
	void testGatherScatterVariableUNIX();
	void testGatherScatterSTRFVariableUNIX();
};


#endif // DatagramSocketTest_INCLUDED
