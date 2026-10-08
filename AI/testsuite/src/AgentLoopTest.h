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
	~AgentLoopTest() = default;

	void testTextOnly();
	void testToolCallAndResult();
	void testMaxRounds();
	void testProviderThrowsPoco();
	void testProviderThrowsNullPointer();
	void testProviderThrowsStd();
	void testToolResultCap();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // AgentLoopTest_INCLUDED
