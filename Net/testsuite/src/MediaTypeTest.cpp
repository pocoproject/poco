//
// MediaTypeTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MediaTypeTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/MediaType.h"


using Poco::Net::MediaType;


MediaTypeTest::MediaTypeTest(const std::string& name): CppUnit::TestCase(name)
{
}


MediaTypeTest::~MediaTypeTest()
{
}


void MediaTypeTest::testParse()
{
	MediaType mt1("text/plain");
	assertTrue (mt1.getType() == "text");
	assertTrue (mt1.getSubType() == "plain");
	assertTrue (mt1.parameters().empty());

	MediaType mt2("text/xml;charset=us-ascii");
	assertTrue (mt2.getType() == "text");
	assertTrue (mt2.getSubType() == "xml");
	assertTrue (mt2.parameters().size() == 1);
	assertTrue (mt2.getParameter("charset") == "us-ascii");

	MediaType mt3("application/test; param1=value1; param2=\"value 2\"");
	assertTrue (mt3.getType() == "application");
	assertTrue (mt3.getSubType() == "test");
	assertTrue (mt3.parameters().size() == 2);
	assertTrue (mt3.getParameter("param1") == "value1");
	assertTrue (mt3.getParameter("PARAM2") == "value 2");

	MediaType mt4("multipart/mixed; boundary=\"MIME_boundary_01234567\"");
	assertTrue (mt4.getType() == "multipart");
	assertTrue (mt4.getSubType() == "mixed");
	assertTrue (mt4.parameters().size() == 1);
	assertTrue (mt4.getParameter("boundary") == "MIME_boundary_01234567");
}


void MediaTypeTest::testToString()
{
	MediaType mt1("text", "plain");
	assertTrue (mt1.toString() == "text/plain");

	mt1.setParameter("charset", "iso-8859-1");
	assertTrue (mt1.toString() == "text/plain; charset=iso-8859-1");

	MediaType mt2("application", "test");
	mt2.setParameter("param1", "value1");
	mt2.setParameter("param2", "value 2");
	assertTrue (mt2.toString() == "application/test; param1=value1; param2=\"value 2\"");
}


void MediaTypeTest::testMatch()
{
	MediaType mt1("Text/Plain");
	MediaType mt2("text/plain");
	MediaType mt3("text/xml");
	assertTrue (mt1.matches(mt2));
	assertTrue (!mt1.matches(mt3));
	assertTrue (mt1.matches("text"));
	assertTrue (mt2.matches("text"));
	assertTrue (mt3.matches("text"));
}


void MediaTypeTest::testMatchRange()
{
	MediaType mt1("Text/Plain");
	MediaType mt2("text/plain");
	MediaType mt3("text/xml");
	MediaType mt4("image/jpg");
	MediaType mt5("text/*");
	MediaType mt6("*/*");
	assertTrue (mt1.matchesRange(mt5));
	assertTrue (mt2.matchesRange(mt5));
	assertTrue (mt3.matchesRange(mt5));
	assertTrue (!mt4.matchesRange(mt5));
	assertTrue (mt1.matchesRange(mt6));
	assertTrue (mt2.matchesRange(mt6));
	assertTrue (mt3.matchesRange(mt6));
	assertTrue (mt4.matchesRange(mt6));

	assertTrue (mt5.matchesRange(mt1));
	assertTrue (mt5.matchesRange(mt2));
	assertTrue (mt5.matchesRange(mt3));
	assertTrue (!mt5.matchesRange(mt4));

	assertTrue (mt1.matchesRange("text", "*"));
	assertTrue (mt2.matchesRange("text", "*"));
	assertTrue (mt3.matchesRange("text", "*"));
	assertTrue (!mt4.matchesRange("text", "*"));

	assertTrue (mt1.matchesRange("*"));
	assertTrue (mt4.matchesRange("*"));
}


void MediaTypeTest::setUp()
{
}


void MediaTypeTest::tearDown()
{
}


CppUnit::Test* MediaTypeTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MediaTypeTest");

	CppUnit_addTest(pSuite, MediaTypeTest, testParse);
	CppUnit_addTest(pSuite, MediaTypeTest, testToString);
	CppUnit_addTest(pSuite, MediaTypeTest, testMatch);
	CppUnit_addTest(pSuite, MediaTypeTest, testMatchRange);

	return pSuite;
}
