//
// SQLParserTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SQLParserTest.h"

#ifndef POCO_DATA_NO_SQL_PARSER

#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include <sstream>
#include "SQLParser.h"


namespace Poco {
namespace Data {

namespace Parser = hsql; // namespace Poco::Data::Parser

}}


using namespace Poco::Data::Parser;


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
		"DELETE FROM Test WHERE First = ?;"
		"DROP TABLE table_name;"
		"ALTER TABLE mytable DROP COLUMN IF EXISTS mycolumn;"
		"PREPARE prep_inst FROM 'INSERT INTO test VALUES (?, ?, ?)';"
		"EXECUTE prep_inst(1, 2, 3);";

	SQLParserResult result;
	SQLParser::parse(query, &result);
	int ins = 0, sel = 0, upd = 0, del = 0, drop = 0, alter = 0, prepare = 0, execute = 0;

	if (!result.isValid())
	{
		std::ostringstream os;
		os << "Given string is not a valid SQL query.\n";
		os << result.errorMsg() << "(L" << result.errorLine() << result.errorColumn() << ")\n";
		fail(os.str());
	}
	assertTrue(result.isValid());
	assertEqual(8, result.size());
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
		case kStmtDrop: ++drop; break;
		case kStmtAlter: ++alter; break;
		case kStmtPrepare: ++prepare; break;
		case kStmtExecute: ++execute; break;
		default: break;
		}
	}
	assertEqual(1, ins);
	assertEqual(1, sel);
	assertEqual(1, upd);
	assertEqual(1, del);
	assertEqual(1, drop);
	assertEqual(1, alter);
	assertEqual(1, prepare);
	assertEqual(1, execute);

	result.reset();
	SQLParser::parse(query, &result);
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


#endif //  POCO_DATA_NO_SQL_PARSER
