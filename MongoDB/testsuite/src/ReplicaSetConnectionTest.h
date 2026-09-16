//
// ReplicaSetConnectionTest.h
//
// Definition of the ReplicaSetConnectionTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ReplicaSetConnectionTest_INCLUDED
#define ReplicaSetConnectionTest_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "CppUnit/TestCase.h"


class ReplicaSetConnectionTest: public CppUnit::TestCase
{
public:
	ReplicaSetConnectionTest(const std::string& name);
	~ReplicaSetConnectionTest() override;

	void testServerHelloAndPing();
	void testScriptedErrorReply();
	void testNoReplyTimesOut();
	void testCloseConnectionAndStopListening();
	void testPartialMessageDoesNotStall();
	void testFindServerErrorBounded();
	void testFindNoReplyBounded();
	void testFindSucceedsAfterServerErrors();
	void testFindResentAfterNoReply();
	void testInsertNotResentAfterNoReply();
	void testInsertNotResentAfterConnectionClosed();
	void testInsertResentAfterNotWritablePrimary();
	void testInsertNotResentAfterStateChange();
	void testWriteErrorsReplyReturned();
	void testWriteConcernErrorReturned();
	void testGetMoreNotResentAfterNoReply();
	void testGetMoreNotResentAfterNotPrimary();
	void testKillCursorsNotResent();
	void testKillAcceptsCursorNotFound();
	void testKillWithUnconfirmedReplyReleasesCursor();
	void testAggregateWithOutputStageNotResent();
	void testAggregateResentAfterConnectionLoss();
	void testConnectFailureRetriedOnSecondServer();
	void testStaleConnectionFindRecoversInsertFails();
	void testMalformedReplyNotResent();
	void testUnsupportedElementTypeNotResent();
	void testKillCursorsNotResentAfterNotPrimary();
	void testNoSuitableServer();
	void testFindResentAfterNotExecutedReply();
	void testInsertResentAfterLegacyNotPrimary();
	void testCountResentAfterNoReply();
	void testNoSuitableServerAfterErrorReply();
	void testNoSuitableServerAfterNoReply();
	void testUnlistedErrorReplyReturned();
	void testCodelessErrorReplyReturned();
	void testCursorRequestsCarryCursorId();
	void testAttemptBoundScalesWithServerCount();
	void testSafeReadsResentAndOtherCommandsNot();
	void testCursorFollowsResentFind();
	void testGetMoreCursorNotFoundReturned();
	void testOneWayRequestDelivered();
	void testReadResponseWithoutConnection();
	void testReconnectAfterConnectionClosed();
	void testKillWithForeignIdReleasesCursor();
	void testFindResentAfterNoReplyWithConfigTimeouts();
	void testSocketTimeoutFallsBackToConfig();
	void testHandBuiltBodyClassifiedByFirstElement();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();
};


#endif // ReplicaSetConnectionTest_INCLUDED
