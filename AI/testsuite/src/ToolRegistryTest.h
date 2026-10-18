//
// ToolRegistryTest.h
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ToolRegistryTest_INCLUDED
#define ToolRegistryTest_INCLUDED


#include "CppUnit/TestCase.h"


class ToolRegistryTest: public CppUnit::TestCase
{
public:
	ToolRegistryTest(const std::string& name);
	~ToolRegistryTest();

	void testRegisterAndExecute();
	void testReplace();
	void testRegisterIfAbsent();
	void testUnknownTool();
	void testExecutorThrows();
	void testToolsInOrder();
	void testRegistrationRefused();
	void testToolResultText();
	void testContextReachesTheHandler();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // ToolRegistryTest_INCLUDED
