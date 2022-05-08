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
#include "Poco/Net/SocketAddress.h"
#include "CppUnit/TestCase.h"


class DatagramSocketTest: public CppUnit::TestCase
{
public:
	DatagramSocketTest(const std::string& name);
	~DatagramSocketTest();

	void testEcho();
	void testMoveDatagramSocket();
	void testEchoBuffer();
	void testReceiveFromAvailable();
	void testSendToReceiveFrom();
	void testUnbound();
	void testReuseAddressPortWildcard();
	void testReuseAddressPortSpecific();
	void testBroadcast();
	void testGatherScatterFixed();
	void testGatherScatterVariable();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	static Poco::UInt16 getFreePort(Poco::Net::SocketAddress::Family family, std::uint16_t port);

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
