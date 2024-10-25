//
// JSONFormatterTest.h
//
// Definition of the JSONFormatterTest class.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JSONFormatterTest_INCLUDED
#define JSONFormatterTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class JSONFormatterTest: public CppUnit::TestCase
{
public:
	JSONFormatterTest(const std::string& name);
	~JSONFormatterTest();

	void testJSONFormatter();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // JSONFormatterTest_INCLUDED
