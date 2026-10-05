//
// TCPReactorServerTest.h
//
// Definition of the TCPReactorServerTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef TCPReactorServerTest_INCLUDED
#define TCPReactorServerTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class TCPReactorServerTest: public CppUnit::TestCase
{
public:
	TCPReactorServerTest(const std::string& name);
	~TCPReactorServerTest();

	void testSuppliedSocket();
	void testNonBlockingRead();
	void testReadsPerEvent();
	void testReadsHeldData();
	void testDataBeforeErrorIsDelivered();
	void testCloseCallbackOnce();
	void testCallbackCloses();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void callbackCloses(bool nonBlocking);
};


#endif // TCPReactorServerTest_INCLUDED
