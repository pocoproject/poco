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
	~AnthropicProviderTest();

	void testRequestBody();
	void testEmptySystemPrompt();
	void testURIJoin();
	void testStreamedText();
	void testStreamedToolUse();
	void testThinkingRoundTrip();
	void testCutStream();
	void testErrorEvent();
	void testHttpErrors();
	void testMalformedToolInput();
	void testAppendToolResult();
	void testListModels();
	void testCancelBeforeHeaders();
	void testCancelDuringStream();
	void testConcurrentCalls();
	void testCancelStress();
	void testCacheMarkers();
	void testRequestMembersAndHeaders();
	void testCancelDuringUpload();
	void testCancelDuringConnect();
	void testCancelDuringHandshake();
	void testCancelDescriptorNotInherited();
	void testCutToolCall();
	void testThinkingSignatures();
	void testBlocksAsTheyCame();
	void testRefusal();
	void testUnreachable();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // AnthropicProviderTest_INCLUDED
