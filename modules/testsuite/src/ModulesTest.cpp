//
// ModulesTest.cpp
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ModulesTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"

import Poco;

ModulesTest::ModulesTest(const std::string& name): CppUnit::TestCase(name)
{
}


ModulesTest::~ModulesTest()
{
}


void ModulesTest::testModule()
{
	Poco::LocalDateTime now;

	std::string str = Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::ISO8601_FORMAT);
	Poco::DateTime dt;
	int tzd;
	Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, str, dt, tzd);
	dt.makeUTC(tzd);
	Poco::LocalDateTime ldt(tzd, dt);
	Poco::URI uri1("http://www.appinf.com:81/sample?example-query#somewhere");

	std::println("Scheme:    {}", uri1.getScheme());
	std::println("Authority: {}". uri1.getAuthority());
	std::println("Path:      {}", uri1.getPath());
	std::println("Query:     {}", uri1.getQuery());
	std::println("Fragment:  {}", uri1.getFragment());

	Poco::URI uri2;
	uri2.setScheme("https");
	uri2.setAuthority("www.appinf.com");
	uri2.setPath("/another sample");

	std::println("{}", uri1.toString());
}


CppUnit::Test* ModulesTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ModulesTest");

	CppUnit_addTest(pSuite, ModulesTest, testModule);

	return pSuite;
}
