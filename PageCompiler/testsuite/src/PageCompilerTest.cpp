//
// PageCompilerTest.cpp
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PageCompilerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTMLForm.h"
#include "TestHandler.h"
#include "TestHandlerNoForm.h"
#include "StringifyHandler.h"
#include "StringifyHandlerNoForm.h"
#include "StringifyEscapeHandler.h"
#include <sstream>


PageCompilerTest::PageCompilerTest(const std::string& name): CppUnit::TestCase(name)
{
}


PageCompilerTest::~PageCompilerTest()
{
}


void PageCompilerTest::testHandler()
{
	// Verify that the original page generates a concrete, linkable HTTP handler.
	AI::Test::TestHandler handler;
}


void PageCompilerTest::testHandlerNoForm()
{
	// Verify compilation with form disabled and request used in a pre-response scriptlet.
	AI::Test::TestHandlerNoForm handler;
}


void PageCompilerTest::testStringify()
{
	AI::Test::StringifyHandler handler;
	Poco::Net::HTMLForm form;
	form.set("name", "Poco");
	std::ostringstream ostr;

	handler.stringify(ostr, form);

	assertEquals (std::string("<p>Hello, Poco!</p>\n"), ostr.str());
}


void PageCompilerTest::testStringifyNoForm()
{
	AI::Test::StringifyHandlerNoForm handler;
	std::ostringstream ostr;

	handler.stringify(ostr);

	assertEquals (std::string("<p>Hello, World!</p>\n"), ostr.str());
}


void PageCompilerTest::testStringifyEscape()
{
	AI::Test::StringifyEscapeHandler handler;
	Poco::Net::HTMLForm form;
	std::ostringstream ostr;

	handler.stringify(ostr, form);

	assertEquals (std::string("<p>&lt;a&amp;b&gt;</p>\n"), ostr.str());
}


void PageCompilerTest::setUp()
{
}


void PageCompilerTest::tearDown()
{
}


CppUnit::Test* PageCompilerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PageCompilerTest");

	CppUnit_addTest(pSuite, PageCompilerTest, testHandler);
	CppUnit_addTest(pSuite, PageCompilerTest, testHandlerNoForm);
	CppUnit_addTest(pSuite, PageCompilerTest, testStringify);
	CppUnit_addTest(pSuite, PageCompilerTest, testStringifyNoForm);
	CppUnit_addTest(pSuite, PageCompilerTest, testStringifyEscape);

	return pSuite;
}
