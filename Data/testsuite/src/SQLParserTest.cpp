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
#include <set>
#include <cstdint>
#include "SQLParser.h"
#include "sql/AlterStatement.h"
#include "sql/DropStatement.h"
#include "sql/DeleteStatement.h"


namespace Poco::Data {

namespace Parser = hsql; // namespace Poco::Data::Parser

} // namespace Poco::Data


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
		os << result.errorMsg() << " (L" << result.errorLine() << ":C" << result.errorColumn() << ")\n";
		failmsg(os.str());
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


void SQLParserTest::testInvalidSQL()
{
	std::string query = "SELECT FROM WHERE ;";
	SQLParserResult result;
	SQLParser::parse(query, &result);
	assertTrue (!result.isValid());
	assertTrue (result.errorMsg() != nullptr && std::string(result.errorMsg()).length() > 0);
}


void SQLParserTest::testResetClearsParameters()
{
	// Reuses the testSQLParser query (8 statements, 2 '?' parameters) and
	// reparses N times with reset() between. Pins the lifecycle path the
	// upstream commit 38d1a66 hardened (sval leaks across iterations).
	std::string query = "INSERT INTO Test VALUES ('1', 2, 3.5);"
		"SELECT * FROM Test WHERE First = ?;"
		"UPDATE Test SET value=1 WHERE First = '1';"
		"DELETE FROM Test WHERE First = ?;"
		"DROP TABLE table_name;"
		"ALTER TABLE mytable DROP COLUMN IF EXISTS mycolumn;"
		"PREPARE prep_inst FROM 'INSERT INTO test VALUES (?, ?, ?)';"
		"EXECUTE prep_inst(1, 2, 3);";

	SQLParserResult result;
	for (int i = 0; i < 5; ++i)
	{
		SQLParser::parse(query, &result);
		assertTrue (result.isValid());
		assertEqual(8, result.size());
		assertEqual(2, static_cast<int>(result.parameters().size()));
		result.reset();
	}
	SQLParser::parse(query, &result);
	assertTrue (result.isValid());
	assertEqual(8, result.size());
	assertEqual(2, static_cast<int>(result.parameters().size()));
}


void SQLParserTest::testNamedParameter()
{
	std::string query = "SELECT * FROM t WHERE u = :user AND a = :age";
	SQLParserResult result;
	SQLParser::parse(query, &result);
	assertTrue (result.isValid());
	assertEqual(1, result.size());
	assertEqual(2, static_cast<int>(result.parameters().size()));

	std::set<std::string> names;
	for (const Expr* e: result.parameters())
	{
		assertTrue (e->type == kExprParameterNamed);
		assertTrue (e->name != nullptr);
		names.insert(std::string(e->name));
	}
	std::set<std::string> expected;
	expected.insert("user");
	expected.insert("age");
	assertTrue (names == expected);
}


void SQLParserTest::testAlterDropColumnIfExists()
{
	std::string query = "ALTER TABLE mytable DROP COLUMN IF EXISTS mycolumn";
	SQLParserResult result;
	SQLParser::parse(query, &result);
	assertTrue (result.isValid());
	assertEqual(1, result.size());

	const SQLStatement* stmt = result.getStatement(0);
	assertTrue (stmt->type() == kStmtAlter);
	const AlterStatement* alter = static_cast<const AlterStatement*>(stmt);
	assertTrue (alter->name == std::string("mytable"));

	const DropColumnAction* action = static_cast<const DropColumnAction*>(alter->action);
	assertTrue (action->columnName == std::string("mycolumn"));
	assertTrue (action->ifExists);
}


void SQLParserTest::testDropDiscrimination()
{
	std::string query = "DROP TABLE t; DROP INDEX idx_t;";
	SQLParserResult result;
	SQLParser::parse(query, &result);
	assertTrue (result.isValid());
	assertEqual(2, result.size());

	const SQLStatement* s0 = result.getStatement(0);
	assertTrue (s0->type() == kStmtDrop);
	const DropStatement* d0 = static_cast<const DropStatement*>(s0);
	assertTrue (d0->type == kDropTable);
	assertTrue (d0->name == std::string("t"));

	const SQLStatement* s1 = result.getStatement(1);
	assertTrue (s1->type() == kStmtDrop);
	const DropStatement* d1 = static_cast<const DropStatement*>(s1);
	assertTrue (d1->type == kDropIndex);
}


void SQLParserTest::testDeleteShape()
{
	// MemoryDB.cpp:1232 specifically branches on d->expr == nullptr for
	// the truncate-detect optimization. Pin both shapes here so a parser
	// regression that flipped the meaning would break this test, not
	// just MemoryDB's truncate behavior under load.
	std::string query = "DELETE FROM t WHERE id=1; DELETE FROM t;";
	SQLParserResult result;
	SQLParser::parse(query, &result);
	assertTrue (result.isValid());
	assertEqual(2, result.size());

	const DeleteStatement* d0 = static_cast<const DeleteStatement*>(result.getStatement(0));
	assertTrue (d0->expr != nullptr);

	const DeleteStatement* d1 = static_cast<const DeleteStatement*>(result.getStatement(1));
	assertTrue (d1->expr == nullptr);
	assertTrue (d1->tableName == std::string("t"));
}


void SQLParserTest::testComments()
{
	// hsql lexer supports -- line comments; the C-style /* ... */ form is
	// NOT supported by this lexer build, so we exercise only the line-
	// comment variant. MemoryDB.cpp's trace-hook skipLeading*Comments is
	// the production path that must handle both - that path is tested
	// downstream in MemoryDBTest::testCommentPrefixedWithoutRowidRejected.
	std::string query = "-- header\nSELECT * FROM t;\n-- trailing\nINSERT INTO t VALUES(1);";
	SQLParserResult result;
	SQLParser::parse(query, &result);
	assertTrue (result.isValid());
	assertEqual(2, result.size());
	assertTrue (result.getStatement(0)->type() == kStmtSelect);
	assertTrue (result.getStatement(1)->type() == kStmtInsert);
}


void SQLParserTest::testTokenize()
{
	// Backstops the SQLParser::tokenize loop rewrite landed in upstream
	// commit 38d1a66 (sval-per-iteration leaks, NAMED_PARAM inclusion).
	std::string query = "SELECT id FROM t WHERE name = :who AND age = $1";
	std::vector<int16_t> tokens;
	try
	{
		SQLParser::tokenize(query, &tokens);
		assertTrue (!tokens.empty());
	}
	catch (...)
	{
		failmsg("SQLParser::tokenize threw an exception");
	}
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
	CppUnit_addTest(pSuite, SQLParserTest, testInvalidSQL);
	CppUnit_addTest(pSuite, SQLParserTest, testResetClearsParameters);
	CppUnit_addTest(pSuite, SQLParserTest, testNamedParameter);
	CppUnit_addTest(pSuite, SQLParserTest, testAlterDropColumnIfExists);
	CppUnit_addTest(pSuite, SQLParserTest, testDropDiscrimination);
	CppUnit_addTest(pSuite, SQLParserTest, testDeleteShape);
	CppUnit_addTest(pSuite, SQLParserTest, testComments);
	CppUnit_addTest(pSuite, SQLParserTest, testTokenize);

	return pSuite;
}


#endif //  POCO_DATA_NO_SQL_PARSER
