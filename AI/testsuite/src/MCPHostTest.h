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
{
public:
	MCPHostTest(const std::string& name);
	~MCPHostTest();

	void testRegisterIntoOverHTTP();
	void testRegisterIntoOverStreams();
	void testNamesTheModelsRefuse();
	void testCollisions();
	void testResultPassesThrough();
	void testAddServerFailure();
	void testDuplicateLabel();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // MCPHostTest_INCLUDED
