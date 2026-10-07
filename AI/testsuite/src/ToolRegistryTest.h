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
	~ToolRegistryTest() = default;

	void testRegisterAndExecute();
	void testGetDefinitions();
	void testUnregister();
	void testUnknownTool();
	void testExecutorException();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // ToolRegistryTest_INCLUDED
