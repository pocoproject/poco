//
// RESTTestSuite.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "RESTTestSuite.h"
#include "JSONParserTest.h"
#include "JSONSerializerTest.h"
#include "JSONDeserializerTest.h"
#include "PathSerializerTest.h"
#include "PathDeserializerTest.h"
#include "HeaderSerializerTest.h"
#include "HeaderDeserializerTest.h"
#include "FormSerializerTest.h"
#include "FormDeserializerTest.h"
#include "RawSerializerTest.h"
#include "RawDeserializerTest.h"
#include "SerializerTest.h"
#include "DeserializerTest.h"
#include "RemotingTest.h"


CppUnit::Test* RESTTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RESTTestSuite");

	pSuite->addTest(JSONParserTest::suite());
	pSuite->addTest(JSONSerializerTest::suite());
	pSuite->addTest(JSONDeserializerTest::suite());
	pSuite->addTest(PathSerializerTest::suite());
	pSuite->addTest(PathDeserializerTest::suite());
	pSuite->addTest(HeaderSerializerTest::suite());
	pSuite->addTest(HeaderDeserializerTest::suite());
	pSuite->addTest(FormSerializerTest::suite());
	pSuite->addTest(FormDeserializerTest::suite());
	pSuite->addTest(RawSerializerTest::suite());
	pSuite->addTest(RawDeserializerTest::suite());
	pSuite->addTest(SerializerTest::suite());
	pSuite->addTest(DeserializerTest::suite());
	pSuite->addTest(RemotingTestREST::suite());
	pSuite->addTest(RemotingTestRESTChunked::suite());
	pSuite->addTest(RemotingTestRESTCompressed::suite());
	pSuite->addTest(RemotingTestRESTChunkedCompressed::suite());
	pSuite->addTest(RemotingTestRESTAuth::suite());
	pSuite->addTest(RemotingTestAuth::suite());

	return pSuite;
}
