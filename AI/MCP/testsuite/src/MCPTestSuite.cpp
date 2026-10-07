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
#include "HandshakeTest.h"
#include "ToolsTest.h"
#include "StreamIngressTest.h"
#include "StdIOTest.h"
#include "FeaturesTest.h"


CppUnit::Test* MCPTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MCPTestSuite");

	pSuite->addTest(EnvelopeTest::suite());
	pSuite->addTest(HandshakeTest::suite());
	pSuite->addTest(ToolsTest::suite());
	pSuite->addTest(StreamIngressTest::suite());
	pSuite->addTest(StdIOTest::suite());
	pSuite->addTest(FeaturesTest::suite());

	return pSuite;
}
