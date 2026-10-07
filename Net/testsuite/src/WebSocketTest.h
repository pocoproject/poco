//
// WebSocketTest.h
//
// Definition of the WebSocketTest class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef WebSocketTest_INCLUDED
#define WebSocketTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class WebSocketTest: public CppUnit::TestCase
{
public:
	WebSocketTest(const std::string& name);
	~WebSocketTest();

	void testWebSocket();
	void testWebSocketLarge();
	void testWebSocketLargeInOneFrame();
	void testWebSocketNB();
	void testPeerCloseAfterPartialHeader();
	void testPeerCloseAfterPartialHeaderNB();
	void testMalformedFrameUnmaskedClient();
	void testMalformedFrameRSV1Default();
	void testMalformedFrameRSV2Default();
	void testMalformedFrameRSV3Default();
	void testMalformedFrameAllowedRSV1();
	void testMalformedFrameAllowedRSV1RejectRSV2();
	void testMalformedFrameReservedOpcode03();
	void testMalformedFrameReservedControlOpcode0B();
	void testMalformedFrameFragmentedControlPing();
	void testMalformedFrameControlPing125Accepted();
	void testMalformedFrameControlPing126Rejected();
	void testMalformedFrameControlPing127Rejected();
	void testMalformedFrameClientRejectsMaskedServerFrame();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void testOneLargeFrame(int msgSize);
	void peerCloseAfterPartialHeader(bool blocking);
	int sendServerFrame(const std::string& frameBytes, int allowedRSV = 0, int bufferSize = 256, int* pFlags = nullptr, int* pReceived = nullptr);
};


#endif // WebSocketTest_INCLUDED
