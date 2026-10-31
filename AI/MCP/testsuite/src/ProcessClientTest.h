//
// ProcessClientTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ProcessClientTest_INCLUDED
#define ProcessClientTest_INCLUDED


#include "CppUnit/TestCase.h"


class ProcessClientTest: public CppUnit::TestCase
{
public:
	ProcessClientTest(const std::string& name);
	~ProcessClientTest();

	void testRoundTrip();
	void testTimeout();
	void testServerEndsMidRequest();
	void testWriteToEndedServer();
	void testWriteTimeoutCancels();
	void testReaderOutlivesClient();
	void testServerLeavesItsGroup();
	void testNoiseOnStandardOutput();
	void testCloseEndsServer();
	void testStubbornServerIsKilled();
	void testTerminationRequest();
	void testEnvironmentAndDirectory();
	void testClearedEnvironment();
	void testOversizedLine();
	void testFloodBeforeTheReply();
	void testServerStopsReading();
	void testProcessKeepsOutputOpen();
	void testCommandThatCannotRun();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	static std::string serverPath();
};


#endif // ProcessClientTest_INCLUDED
