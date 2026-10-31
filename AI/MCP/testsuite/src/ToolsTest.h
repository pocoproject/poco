//
// ToolsTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ToolsTest_INCLUDED
#define ToolsTest_INCLUDED


#include "CppUnit/TestCase.h"


class ToolsTest: public CppUnit::TestCase
{
public:
	ToolsTest(const std::string& name);
	~ToolsTest();

	void testToolsList();
	void testSchemasAreCopied();
	void testConcurrentReplies();
	void testToolsCall();
	void testCallErrors();
	void testHandlerThrowIsToolError();
	void testRegistrationRefused();
	void testHeaderAnnotations();
	void testToolResultForms();
	void testToolResultFromObject();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // ToolsTest_INCLUDED
