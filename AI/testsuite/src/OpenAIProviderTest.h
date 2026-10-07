//
// OpenAIProviderTest.h
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef OpenAIProviderTest_INCLUDED
#define OpenAIProviderTest_INCLUDED


#include "CppUnit/TestCase.h"


class OpenAIProviderTest: public CppUnit::TestCase
{
public:
	OpenAIProviderTest(const std::string& name);
	~OpenAIProviderTest() = default;

	void testAppendAssistantTextOnly();
	void testAppendAssistantWithToolCalls();
	void testAppendToolResult();
	void testAppendToolResultSeparateMessages();
	void testToOpenAITools();
	void testEmbed();
	void testEmbedError();
	void testListModels();
	void testListModelsError();
	void testListModelsMalformed();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // OpenAIProviderTest_INCLUDED
