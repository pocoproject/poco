//
// TextEncodingTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TextEncodingTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/TextEncoding.h"
#include "Poco/Latin1Encoding.h"
#include "Poco/Latin2Encoding.h"
#include "Poco/Latin9Encoding.h"
#include "Poco/Windows1250Encoding.h"
#include "Poco/Windows1251Encoding.h"
#include "Poco/Windows1252Encoding.h"
#include "Poco/UTF8Encoding.h"


using namespace Poco;


TextEncodingTest::TextEncodingTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


TextEncodingTest::~TextEncodingTest()
{
}


void TextEncodingTest::testTextEncoding()
{
	TextEncoding& ascii = TextEncoding::byName("ASCII");
	assertTrue (std::string("US-ASCII") == ascii.canonicalName());

	TextEncoding& utf8 = TextEncoding::byName("utf8");
	assertTrue (std::string("UTF-8") == utf8.canonicalName());
	
	TextEncoding& latin1 = TextEncoding::byName("latin1");
	assertTrue (std::string("ISO-8859-1") == latin1.canonicalName());
	
	TextEncoding& latin2 = TextEncoding::byName("latin2");
	assertTrue (std::string("ISO-8859-2") == latin2.canonicalName());

	TextEncoding& latin9 = TextEncoding::byName("latin9");
	assertTrue (std::string("ISO-8859-15") == latin9.canonicalName());

	TextEncoding& cp1250 = TextEncoding::byName("CP1250");
	assertTrue (std::string("windows-1250") == cp1250.canonicalName());

	TextEncoding& cp1251 = TextEncoding::byName("CP1251");
	assertTrue (std::string("windows-1251") == cp1251.canonicalName());

	TextEncoding& cp1252 = TextEncoding::byName("CP1252");
	assertTrue (std::string("windows-1252") == cp1252.canonicalName());


	TextEncoding& glob = TextEncoding::global();
	assertTrue (std::string("UTF-8") == glob.canonicalName());
	
	TextEncoding::global(new Latin1Encoding);
	TextEncoding& glob2 = TextEncoding::global();
	assertTrue (std::string("ISO-8859-1") == glob2.canonicalName());

	TextEncoding::global(new Latin2Encoding);
	TextEncoding& glob3 = TextEncoding::global();
	assertTrue (std::string("ISO-8859-2") == glob3.canonicalName());

	TextEncoding::global(new Latin9Encoding);
	TextEncoding& glob4 = TextEncoding::global();
	assertTrue (std::string("ISO-8859-15") == glob4.canonicalName());

	TextEncoding::global(new Windows1250Encoding);
	TextEncoding& glob5 = TextEncoding::global();
	assertTrue (std::string("windows-1250") == glob5.canonicalName());

	TextEncoding::global(new Windows1251Encoding);
	TextEncoding& glob6 = TextEncoding::global();
	assertTrue (std::string("windows-1251") == glob6.canonicalName());

	TextEncoding::global(new Windows1252Encoding);
	TextEncoding& glob7 = TextEncoding::global();
	assertTrue (std::string("windows-1252") == glob7.canonicalName());

	TextEncoding::global(new UTF8Encoding);
	TextEncoding& glob8 = TextEncoding::global();
	assertTrue (std::string("UTF-8") == glob8.canonicalName());
}


void TextEncodingTest::testTextEncodingManager()
{
	TextEncodingRegistry registry;

	assertTrue (registry.has("us-ascii"));
	assertTrue (registry.has("US-ASCII"));
	assertTrue (registry.has("ascii"));
	assertTrue (registry.has("ASCII"));

	assertTrue (registry.has("utf-8"));
	assertTrue (registry.has("UTF-8"));

	assertTrue (registry.has("utf-16"));
	assertTrue (registry.has("UTF-16"));

	assertTrue (registry.has("utf-32"));
	assertTrue (registry.has("UTF-32"));

	assertTrue (registry.has("iso-8859-1"));
	assertTrue (registry.has("ISO-8859-1"));

	assertTrue (registry.has("iso-8859-2"));
	assertTrue (registry.has("ISO-8859-2"));

	assertTrue (registry.has("iso-8859-15"));
	assertTrue (registry.has("ISO-8859-15"));

	assertTrue (registry.has("windows-1250"));
	assertTrue (registry.has("WINDOWS-1250"));

	assertTrue (registry.has("windows-1251"));
	assertTrue (registry.has("WINDOWS-1251"));

	assertTrue (registry.has("windows-1252"));
	assertTrue (registry.has("WINDOWS-1252"));
}


void TextEncodingTest::setUp()
{
}


void TextEncodingTest::tearDown()
{
}


CppUnit::Test* TextEncodingTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TextEncodingTest");

	CppUnit_addTest(pSuite, TextEncodingTest, testTextEncoding);
	CppUnit_addTest(pSuite, TextEncodingTest, testTextEncodingManager);

	return pSuite;
}
