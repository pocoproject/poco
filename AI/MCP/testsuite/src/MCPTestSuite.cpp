//
// MCPTestSuite.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MCPTestSuite.h"
#include "EnvelopeTest.h"
#include "DiscoverTest.h"
#include "ToolsTest.h"
#include "FeaturesTest.h"
#include "StreamIngressTest.h"
#include "StreamClientTest.h"
#include "ProcessClientTest.h"


CppUnit::Test* MCPTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MCPTestSuite");

	pSuite->addTest(EnvelopeTest::suite());
	pSuite->addTest(DiscoverTest::suite());
	pSuite->addTest(ToolsTest::suite());
	pSuite->addTest(FeaturesTest::suite());
	pSuite->addTest(StreamIngressTest::suite());
	pSuite->addTest(StreamClientTest::suite());
	pSuite->addTest(ProcessClientTest::suite());

	return pSuite;
}
