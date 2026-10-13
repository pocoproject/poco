//
// MCPHostTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MCPHostTest_INCLUDED
#define MCPHostTest_INCLUDED


#include "CppUnit/TestCase.h"


class MCPHostTest: public CppUnit::TestCase
	/// Exercises MCPHost against an in-test MCP server behind TestHTTPHost.
{
public:
	MCPHostTest(const std::string& name);
	~MCPHostTest();

	void testRegisterAndCall();
	void testToolErrorBecomesErrorString();
	void testCollisionSkipped();
	void testDeadServerThrows();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // MCPHostTest_INCLUDED
