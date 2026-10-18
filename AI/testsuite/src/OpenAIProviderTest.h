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
	~OpenAIProviderTest();

	void testRequestBody();
	void testNoTools();
	void testStreamedText();
	void testStreamedToolCalls();
	void testFinishWithoutDone();
	void testCutStream();
	void testErrorInStream();
	void testHttpErrors();
	void testMalformedArguments();
	void testAppendToolResult();
	void testListModels();
	void testEmbeddings();
	void testMalformedEmbeddings();
	void testCancelBeforeHeaders();
	void testCancelDuringStream();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // OpenAIProviderTest_INCLUDED
