//
// JSONRPCTestSuite.cpp
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "JSONRPCTestSuite.h"
#include "SerializerTest.h"
#include "DeserializerTest.h"
#include "JSONParserTest.h"
#include "RemotingTest.h"


CppUnit::Test* JSONRPCTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JSONRPCTestSuite");

	pSuite->addTest(SerializerTest::suite());
	pSuite->addTest(DeserializerTest::suite());
	pSuite->addTest(JSONParserTest::suite());
	pSuite->addTest(RemotingTestJSONRPC::suite());
	pSuite->addTest(RemotingTestJSONRPCCompressed::suite());
	pSuite->addTest(RemotingTestJSONRPCNonChunked::suite());
	pSuite->addTest(RemotingTestJSONRPCAuth::suite());
	pSuite->addTest(RemotingTestJSONRPCAuthChunked::suite());

	return pSuite;
}
