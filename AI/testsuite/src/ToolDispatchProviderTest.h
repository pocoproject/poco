//
// ToolDispatchProviderTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ToolDispatchProviderTest_INCLUDED
#define ToolDispatchProviderTest_INCLUDED


#include "CppUnit/TestCase.h"


class ToolDispatchProviderTest: public CppUnit::TestCase
	/// Exercises ToolDispatchProvider over a provider that records what it
	/// is asked.
{
public:
	ToolDispatchProviderTest(const std::string& name);
	~ToolDispatchProviderTest() = default;

	void testRuleMatch();
	void testNoMatchFallsThrough();
	void testAnswerFromResults();
	void testEarlierResultsDoNotCount();
	void testNoToolsKeepsHistory();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // ToolDispatchProviderTest_INCLUDED
