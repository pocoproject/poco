//
// AITestSuite.cpp
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AITestSuite.h"
#include "ToolRegistryTest.h"
#include "AgentLoopTest.h"
#include "AnthropicProviderTest.h"
#include "OpenAIProviderTest.h"
#include "ToolDispatchProviderTest.h"
#include "MCPHostTest.h"


CppUnit::Test* AITestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AITestSuite");

	pSuite->addTest(ToolRegistryTest::suite());
	pSuite->addTest(AgentLoopTest::suite());
	pSuite->addTest(AnthropicProviderTest::suite());
	pSuite->addTest(OpenAIProviderTest::suite());
	pSuite->addTest(ToolDispatchProviderTest::suite());
	pSuite->addTest(MCPHostTest::suite());

	return pSuite;
}
