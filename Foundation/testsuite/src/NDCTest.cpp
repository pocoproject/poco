//
// NDCTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NDCTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/NestedDiagnosticContext.h"
#include "Poco/ActiveThreadPool.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/Format.h"
#include "Poco/Path.h"
#include <iostream>
#include <sstream>


using Poco::NDC;
using Poco::ActiveThreadPool;
using Poco::RunnableAdapter;
using Poco::Path;


NDCTest::NDCTest(const std::string& name): CppUnit::TestCase(name)
{
}


NDCTest::~NDCTest()
{
}


void NDCTest::testNDC()
{
	NDC ndc;
	assertTrue (ndc.depth() == 0);
	ndc.push("item1");
	assertTrue (ndc.toString() == "item1");
	assertTrue (ndc.depth() == 1);
	ndc.push("item2");
	assertTrue (ndc.toString() == "item1:item2");
	assertTrue (ndc.depth() == 2);
	ndc.pop();
	assertTrue (ndc.depth() == 1);
	assertTrue (ndc.toString() == "item1");
	ndc.pop();
	assertTrue (ndc.depth() == 0);
}


void NDCTest::testNDCScope()
{
	poco_ndc("item1");
	auto line1 = __LINE__ - 1;
	assertTrue (NDC::current().depth() == 1);

	{
		poco_ndc("item2");
		auto line2 = __LINE__ - 1;
		assertTrue (NDC::current().depth() == 2);

		{
			poco_ndc("item3");
			auto line3 = __LINE__ - 1;
			assertTrue (NDC::current().depth() == 3);

 			std::ostringstream ostr1;
			NDC::current().dump(ostr1);
			assertEqual (ostr1.str(), Poco::format(
				"\"item1\" (in \"%s\", line %d)\n"
				"\"item2\" (in \"%s\", line %d)\n"
				"\"item3\" (in \"%s\", line %d)",
				std::string(__FILE__), line1,
				std::string(__FILE__), line2,
				std::string(__FILE__), line3));

			std::ostringstream ostr2;
			NDC::current().dump(ostr2, "\n", true);
			std::string fileName = Path(__FILE__).getFileName();
			assertEqual(ostr2.str(), Poco::format(
				"\"item1\" (in \"%s\", line %d)\n"
				"\"item2\" (in \"%s\", line %d)\n"
				"\"item3\" (in \"%s\", line %d)",
				fileName, line1,
				fileName, line2,
				fileName, line3));
		}
		assertTrue (NDC::current().depth() == 2);
	}
	assertTrue (NDC::current().depth() == 1);
}


void NDCTest::testNDCMultiThread()
{
	ActiveThreadPool pool;
	RunnableAdapter<NDCTest> ra(*this, &NDCTest::runInThread);
	for (int i = 0; i < 1000; i++)
	{
		pool.start(ra);
	}
	pool.joinAll();
}


void NDCTest::runInThread()
{
	testNDC();
	testNDCScope();
}


void NDCTest::setUp()
{
}


void NDCTest::tearDown()
{
}


CppUnit::Test* NDCTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NDCTest");

	CppUnit_addTest(pSuite, NDCTest, testNDC);
	CppUnit_addTest(pSuite, NDCTest, testNDCScope);
	CppUnit_addTest(pSuite, NDCTest, testNDCMultiThread);

	return pSuite;
}
