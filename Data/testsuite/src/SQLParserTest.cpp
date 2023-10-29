//
// SQLParserTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SQLParserTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"

#ifdef POCO_DATA_ENABLE_SQL_PARSER

#include "Poco/Data/SQLParser.h"


using namespace Poco::Data;


SQLParserTest::SQLParserTest(const std::string& name): CppUnit::TestCase(name)
{
}


SQLParserTest::~SQLParserTest()
{
}


void SQLParserTest::testSQLParser()
{
	std::string query = "INSERT INTO Test VALUES ('1', 2, 3.5);"
		"SELECT * FROM Test WHERE First = ?;"
		"UPDATE Test SET value=1 WHERE First = '1';"
		"DELETE FROM Test WHERE First = ?;";

	SQLParserResult result;
	SQLParser::parse(query, &result);
	int ins = 0, sel = 0, upd = 0, del = 0;

	assertTrue(result.isValid());
	assertEqual(4, result.size());
	for (auto i = 0u; i < result.size(); ++i)
	{
		const SQLStatement* stmt = result.getStatement(i);
		//printStatementInfo(stmt);
		switch (stmt->type())
		{
		case kStmtSelect: ++sel; break;
		case kStmtInsert: ++ins; break;
		case kStmtUpdate: ++upd; break;
		case kStmtDelete: ++del; break;
		default: break;
		}
	}
	assertEqual(1, ins);
	assertEqual(1, sel);
	assertEqual(1, upd);
	assertEqual(1, del);
}


void SQLParserTest::setUp()
{
}


void SQLParserTest::tearDown()
{
}


CppUnit::Test* SQLParserTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SQLParserTest");

	CppUnit_addTest(pSuite, SQLParserTest, testSQLParser);

	return pSuite;
}


#endif // POCO_DATA_ENABLE_SQL_PARSER
