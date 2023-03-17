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
#include <iostream>


using Poco::NDC;


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
	assertTrue (NDC::current().depth() == 1);
	{
		poco_ndc("item2");
		assertTrue (NDC::current().depth() == 2);
		{
			poco_ndc("item3");
			assertTrue (NDC::current().depth() == 3);
			NDC::current().dump(std::cout);
		}
		assertTrue (NDC::current().depth() == 2);
	}
	assertTrue (NDC::current().depth() == 1);
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

	return pSuite;
}
