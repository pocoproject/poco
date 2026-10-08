//
// FeaturesTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef FeaturesTest_INCLUDED
#define FeaturesTest_INCLUDED


#include "CppUnit/TestCase.h"


class FeaturesTest: public CppUnit::TestCase
	/// Covers the core API added for the standalone MCP server: per-request context
	/// threaded into tool handlers, structured tool output, and the generic
	/// resources/prompts registries.
{
public:
	FeaturesTest(const std::string& name);
	~FeaturesTest();

	void testContextHandlerIdentity();
	void testStructuredContent();
	void testResources();
	void testPrompts();
	void testToolsOnlyHasNoResourceCaps();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // FeaturesTest_INCLUDED
