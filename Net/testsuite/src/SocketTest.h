//
// SocketTest.h
//
// Definition of the SocketTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SocketTest_INCLUDED
#define SocketTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class SocketTest: public CppUnit::TestCase
{
public:
	SocketTest(const std::string& name);
	~SocketTest() override;

	void testEcho();
	void testPeek();
	void testMoveStreamSocket();
	void testPoll();
	void testPollModes();
	void testPollStress();
	void testAvailable();
	void testFIFOBuffer();
	void testConnect();
	void testConnectRefused();
	void testConnectRefusedNB();
	void testNonBlocking();
	void testAddress();
	void testAssign();
	void testTimeout();
	void testBufferSize();
	void testOptions();
	void testOptionsAfterShutdown();
	void testKeepAliveParams();

#if defined(POCO_TEST_DEPRECATED)
	void testSelect();
	void testSelect2();
	void testSelect3();
#endif

	void testEchoUnixLocal();
	void testUnixLocalAbstract();
	void testUseFd();
	void testSendFile();
	void testSendFileLarge();
	void testSendFileRange();
	void testCloseOnExec();
	void testReceiveNonBlockingIgnoresTimeout();
	void testPollNegativeTimeoutBelowOneMillisecond();
#if defined(POCO_OS_FAMILY_UNIX)
	void testReceiveInterruptedWithTimeout();
	void testReceiveWaitAllInterruptedKeepsTimeout();
	void testSendInterruptedWithTimeout();
	void testReceiveFromInterruptedWithTimeout();
	void testAcceptInterruptedWithTimeout();
	void testReceiveInterruptedNoTimeout();
	void testPollInterruptedNoTimeout();
	void testPollClosedDescriptor();
	void testPollAtFdLimit();
#endif

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();

private:
	void onReadable(bool& b);
	void onWritable(bool& b);

	int _readableToNot = 0;
	int _notToReadable = 0;
	int _writableToNot = 0;
	int _notToWritable = 0;
};


#endif // SocketTest_INCLUDED
