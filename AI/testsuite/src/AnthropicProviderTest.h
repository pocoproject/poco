//
// AnthropicProviderTest.h
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AnthropicProviderTest_INCLUDED
#define AnthropicProviderTest_INCLUDED


#include "CppUnit/TestCase.h"


class AnthropicProviderTest: public CppUnit::TestCase
{
public:
	AnthropicProviderTest(const std::string& name);
	~AnthropicProviderTest() = default;

	void testAppendAssistantTextOnly();
	void testAppendAssistantWithToolCalls();
	void testAppendToolResult();
	void testAppendToolResultMerge();
	void testToAnthropicTools();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // AnthropicProviderTest_INCLUDED
