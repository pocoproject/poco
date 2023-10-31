//
// SQLParserTest.h
//
// Definition of the SQLParserTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLParserTest_INCLUDED
#define SQLParserTest_INCLUDED

#include "Poco/Config.h"

#ifndef POCO_DATA_NO_SQL_PARSER

#include "Poco/Data/Data.h"
#include "CppUnit/TestCase.h"


class SQLParserTest: public CppUnit::TestCase
{
public:
	SQLParserTest(const std::string& name);
	~SQLParserTest();

	void testSQLParser();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif //  POCO_DATA_NO_SQL_PARSER


#endif // SQLParserTest_INCLUDED
