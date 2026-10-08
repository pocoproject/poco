//
// SyslogTest.h
//
// Definition of the SyslogTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SyslogTest_INCLUDED
#define SyslogTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class SyslogTest: public CppUnit::TestCase
{
public:
	SyslogTest(const std::string& name);
	~SyslogTest();

	void testListener();
	void testChannelFacility();
	void testChannelOpenClose();
	void testOldBSD();
	void testStructuredData();
	void testBSDWithoutTimestamp();
	void testTimestamp();
	void testTCPOctetCounting();
	void testTCPNewline();
	void testTCPMixedFraming();
	void testTCPSplitFrames();
	void testTCPCoalescedFrames();
	void testTCPOversize();
	void testTCPExactSize();
	void testTCPGarbage();
	void testTCPUnterminated();
	void testTCPManyConnections();
	void testTCPOpenTwice();
	void testTCPTwoSockets();
	void testTCPCloseWithClients();
	void testTCPPort();
	void testTCPOpenFailed();
	void testTCPMaxConnections();
	void testTCPIdleTimeout();
	void testTCPIdleTimeoutQueueFull();
	void testTCPMaxQueued();
	void testTCPChannel();
	void testTCPChannelToListener();
	void testTCPChannelReconnect();
	void testTCPChannelServerClosed();
	void testTCPChannelResend();
	void testTCPChannelServerAway();
	void testTCPChannelServerNotReading();
	void testTCPChannelServerAwayReported();
	void testTCPChannelServerCloses();
	void testTCPChannelTarget();
	void testTCPChannelHostName();
	void testTCPChannelSwitchTransport();
	void testTCPChannelProperties();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // SyslogTest_INCLUDED
