//
// SSHTest.h
//
// Definition of the SSHTest class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSHTest_INCLUDED
#define SSHTest_INCLUDED


#include "Poco/SSH/SSH.h"
#include "CppUnit/TestCase.h"


class SSHTest: public CppUnit::TestCase
{
public:
	SSHTest(const std::string& name);
	~SSHTest();

	void testHostKeyGeneration();
	void testHostKeyReuse();
	void testServerStartStop();
	void testClientConnect();
	void testPasswordAuth();
	void testPasswordAuthFail();
	void testPublicKeyAuthUserNameMustEqualKeyFileName();
	void testPublicKeyAuthKeepsSessionWhenKeyFileCannotBeRead();
	void testPublicKeyAuthTakesTildeAsFileName();
	void testPublicKeyAuthNonAsciiUserNameUsesItsOwnKeyFile();
	void testPublicKeyAuthLogsUserNameOnOneLine();
	void testSessionHandling();
	void testServerShutdownWithActiveSession();
	void testClientDisconnect();
	void testMaxConnections();
	void testReadLineKeepsNonAsciiBytes();
	void testReadLineBackspaceErasesUtf8Sequence();
	void testReadLineMaxLength();
	void testReadLineTimeout();
	void testReadLineWithoutTimeLimit();
	void testReadLineNegativeTimeout();
	void testServerReportsChosenPort();
	void testServerStopTimeout();
	void testServerStopWaitsForSessionNotStarted();
	void testServerStopRequestsSessionsOnce();
	void testServerDestructorWarnsOnce();
	void testServerStopDuringPasswordCheck();
	void testServerStopDuringKeyExchange();
	void testServerStopDuringAuthentication();
	void testServerStopDuringChannelOpening();
	void testServerStopDuringReadLine();
	void testServerStopDuringStreamWrite();
	void testServerStopDuringStreamWriteToFullConnection();
	void testServerStopDuringStreamWriteBeforeChannelClose();
	void testLoginResumesAfterTimedOutWaits();
	void testServerStopDuringRawRead();
	void testHandlerReadWaitsWithoutTimeLimit();
	void testHandlerReadAfterStreamWriteWaitsWithoutTimeLimit();
	void testServerStopDuringReadLineBeforeRawRead();
	void testReadLineEchoWaitsForWindow();
	void testReadLineTimeoutEndsEchoWaitingForWindow();
	void testStreamWriteSendsAllDataBeforeSessionEnds();
	void testServerStopDuringStreamWriteToReadingClient();
	void testWriteFailsWhenStopCheckReturnsTrue();
	void testLoginGraceTime();
	void testLoginGraceTimeLimitsAuthentication();
	void testLoginGraceTimeLimitsChannelOpening();
	void testLoginGraceTimeZeroSetsNoLimit();
	void testLoginGraceTimeEndsWithChannelOpening();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	std::string _testDir;
};


#endif // SSHTest_INCLUDED
