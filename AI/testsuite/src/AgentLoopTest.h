//
// AgentLoopTest.h
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AgentLoopTest_INCLUDED
#define AgentLoopTest_INCLUDED


#include "CppUnit/TestCase.h"


class AgentLoopTest: public CppUnit::TestCase
{
public:
	AgentLoopTest(const std::string& name);
	~AgentLoopTest();

	void testTextOnly();
	void testToolRound();
	void testDoneOnEveryExit();
	void testProviderError();
	void testNoAnswer();
	void testCancelled();
	void testCancelDuringTools();
	void testToolResultCutAtCharacter();
	void testFinalRound();
	void testToolsUnchangedAcrossRounds();
	void testApprover();
	void testRequestContext();
	void testUnknownTool();
	void testConstruction();
	void testCutByTokenLimit();
	void testToolAddedDuringRun();
	void testApproverThrows();
	void testToolReplacedDuringRun();
	void testCallbackThrows();
	void testCallbackThrowsDuringCall();
	void testCallbackThrowsAtRoundLimit();
	void testResultLimits();
	void testDoneCarriesTheOutcome();
	void testToolSeesTheCancel();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // AgentLoopTest_INCLUDED
