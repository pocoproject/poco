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
#include "sql/SelectStatement.h"


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


void SQLParserTest::testTopWithParentheses()
{
	std::string query = "SELECT TOP (10) * FROM Test;";
	SQLParserResult result;
	SQLParser::parse(query, &result);
	assertTrue(result.isValid());
	assertEqual(1, result.size());
	assertTrue(result.getStatement(0)->type() == kStmtSelect);
}


void SQLParserTest::testOffsetFetchNext()
{
	std::string query = "SELECT * FROM Test ORDER BY Id OFFSET 0 ROWS FETCH NEXT 10 ROWS ONLY;";
	SQLParserResult result;
	SQLParser::parse(query, &result);
	assertTrue(result.isValid());
	assertEqual(1, result.size());
	assertTrue(result.getStatement(0)->type() == kStmtSelect);
}


void SQLParserTest::testBracketedIdentifiers()
{
	std::string query = "SELECT * FROM [dbo].[Test];";
	SQLParserResult result;
	SQLParser::parse(query, &result);
	assertTrue(result.isValid());
	assertEqual(1, result.size());
	assertTrue(result.getStatement(0)->type() == kStmtSelect);
}


void SQLParserTest::testThreePartTableName()
{
	std::string query = "SELECT * FROM mydb.dbo.Test;";
	SQLParserResult result;
	SQLParser::parse(query, &result);
	assertTrue(result.isValid());
	assertEqual(1, result.size());
	assertTrue(result.getStatement(0)->type() == kStmtSelect);
}


void SQLParserTest::testArrayLiteralNotShadowedByBracketIdentifier()
{
	// ARRAY[foo] - single bare-identifier element. Regression case: the
	// bracket-quoted-identifier lexer rule (testBracketedIdentifiers) would
	// otherwise swallow the whole "[foo]" as one SQL_IDENTIFIER token,
	// leaving the parser without the '[' / ']' tokens array_expr needs.
	{
		std::string query = "SELECT ARRAY[foo] FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());
		assertTrue(result.getStatement(0)->type() == kStmtSelect);
	}

	// ARRAY[1,2,3] - multi-element array literal still parses (already
	// safe against the bracket-identifier rule since content starts with a
	// digit, but pinned here so the afterarray lexer state doesn't regress it).
	{
		std::string query = "SELECT ARRAY[1,2,3] FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());
		assertTrue(result.getStatement(0)->type() == kStmtSelect);
	}

	// operand[int_literal] array indexing still parses (unaffected, but
	// pinned since it shares the '[' ']' tokens with the two cases above).
	{
		std::string query = "SELECT col[0] FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());
		assertTrue(result.getStatement(0)->type() == kStmtSelect);
	}
}


void SQLParserTest::testDialectStatements()
{
	// The upstream suite (linux-sqlparser-upstream-tests) already asserts the
	// tree shape for these forms. What is asserted here is the contract Poco
	// depends on: the statement parses, yields one statement, and classifies as
	// SELECT so Statement::isSelect() can be trusted.
	static const char* const selects[] =
	{
		// Non-reserved keywords as names and as function names.
		"SELECT * FROM Test WHERE CreatedAt >= DATEADD(MINUTE, -10, GETDATE());",
		"SELECT ISNULL(A, 0), CHAR(10), FORMAT(A, '00'), YEAR, MONTH FROM Test;",
		"SELECT Test.YEAR, dbo.Test.MONTH FROM dbo.Test;",
		"SELECT A AS year FROM Test AS next;",
		"SELECT * FROM start WHERE connect = 1;",
		// T-SQL sequence expression.
		"SELECT NEXT VALUE FOR Seq;",
		"SELECT NEXT VALUE FOR MyDb.dbo.Seq AS NextId;",
		// Parenthesized TOP taking an expression.
		"SELECT TOP (?) * FROM Test;",
		"SELECT TOP (:limit) * FROM Test;",
		"SELECT TOP (N + 1) * FROM Test;",
		// Oracle hierarchical queries, either clause order, with and without NOCYCLE.
		"SELECT Level FROM Dual CONNECT BY Level <= 5;",
		"SELECT Id FROM Test START WITH Id = 1 CONNECT BY PRIOR Id = ParentId;",
		"SELECT Id FROM Test CONNECT BY PRIOR Id = ParentId START WITH Id = 1;",
		"SELECT Id FROM Test CONNECT BY NOCYCLE PRIOR Id = ParentId;",
		// Ordered-set aggregates.
		"SELECT ListAgg(A, ', ') WITHIN GROUP (ORDER BY B) FROM Test;",
		"SELECT dbo.ListAgg(A, ', ') WITHIN GROUP (ORDER BY B DESC) AS L FROM Test;",
		// Oracle outer join marker.
		"SELECT * FROM Test, Other WHERE Test.Id (+) = Other.Id;",
		"SELECT * FROM Test, Other WHERE Test.Id = Other.Id (+);",
		// Table value constructor and table-valued functions in FROM.
		"SELECT * FROM (VALUES (0, 'Any'), (1, 'One')) AS V(Id, Name);",
		"SELECT Value FROM String_Split('a,b', ',');",
		"SELECT * FROM dbo.Fn_Split('a', ',') F;",
		// Row constructor on the left of IN, both right-hand forms.
		"SELECT * FROM Test WHERE (A, B) IN (SELECT X, Y FROM Other);",
		"SELECT * FROM Test WHERE (A, B) IN ((1, 2), (3, 4));",
		"SELECT * FROM Test WHERE (A, B) NOT IN ((1, 2), (3, 4));",
		// Multi-part column references and string literal aliases.
		"SELECT dbo.Test.A, MyDb.dbo.Test.B FROM MyDb.dbo.Test;",
		"SELECT dbo.Test.*, MyDb.dbo.Test.* FROM MyDb.dbo.Test;",
		"SELECT A AS 'Coil Id' FROM Test AS 'my table';",
		// ODBC outer join escape.
		"SELECT * FROM {oj Test LEFT OUTER JOIN Other ON Test.Id = Other.Id};",
		// Scientific notation numeric literals.
		"SELECT 1.5e3, 1.5E+3, 5e-1, .5e1, 2e10 FROM Test;"
	};

	for (const char* query: selects)
	{
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue (result.isValid());
		assertEqual(1, result.size());
		assertTrue (result.getStatement(0)->type() == kStmtSelect);
	}

	// ODBC procedure call escapes are execute statements, not selects.
	static const char* const executes[] =
	{
		"{? = call some_proc(?, 'a', 1)};",
		"{call some_proc(?)};",
		"{call some_proc};",
		"{ ? = call [MyDb].[dbo].[some_proc]('a') };"
	};

	for (const char* query: executes)
	{
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue (result.isValid());
		assertEqual(1, result.size());
		assertTrue (result.getStatement(0)->type() == kStmtExecute);
	}

	// A window function or an ordered-set aggregate is not a table reference,
	// and a parenthesized TOP holding a placeholder cannot be combined with
	// LIMIT: the placeholder is also held by the parameter list.
	static const char* const invalid[] =
	{
		"SELECT * FROM Count(*) OVER (PARTITION BY A);",
		"SELECT * FROM ListAgg(A, ', ') WITHIN GROUP (ORDER BY B);",
		"SELECT TOP (?) * FROM Test LIMIT 5;"
	};

	for (const char* query: invalid)
	{
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue (!result.isValid());
	}
}


void SQLParserTest::testCarriageReturn()
{
	// CR is whitespace, including inside the multi-word tokens. A statement read
	// from a CRLF file must parse exactly like the LF form.
	static const char* const queries[] =
	{
		"SELECT A\r\nFROM Test\r\nWHERE B = 1;",
		"SELECT NEXT VALUE\r\nFOR Seq;",
		"SELECT Sum(A) OVER (ROWS BETWEEN CURRENT\r\nROW AND UNBOUNDED FOLLOWING) FROM Test;",
		"SELECT Array\r\n[Foo] FROM Test;",
		"SELECT Id FROM Test START\r\nWITH Id = 1 CONNECT\r\nBY PRIOR Id = ParentId;",
		"SELECT * FROM Test, Other WHERE Test.Id (\r\n+\r\n) = Other.Id;"
	};

	for (const char* query: queries)
	{
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue (result.isValid());
		assertEqual(1, result.size());
	}

	// A line comment ends at its newline. The clause after the comment carries
	// the FROM target, so a swallowed line is a parse error rather than a
	// silently shorter statement.
	static const char* const commented[] =
	{
		"SELECT A FROM\n-- comment\n  Test;",
		"SELECT A FROM -- comment\n\nTest;",
		"SELECT A FROM\r\n-- comment\r\n  Test;",
		"SELECT A FROM -- comment   \n   Test;"
	};

	for (const char* query: commented)
	{
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue (result.isValid());
		assertEqual(1, result.size());
		assertTrue (result.getStatement(0)->type() == kStmtSelect);
	}

	// A clause a shorter statement could do without needs the tree checked
	// instead: with an inclusive comment condition this parses as valid with
	// no WHERE at all.
	SQLParserResult whereResult;
	SQLParser::parse("SELECT A FROM Test -- comment   \n   WHERE B IN (1, 2);", &whereResult);
	assertTrue (whereResult.isValid());
	assertEqual(1, whereResult.size());
	const SelectStatement* commentedSelect =
		static_cast<const SelectStatement*>(whereResult.getStatement(0));
	assertTrue (commentedSelect->whereClause != nullptr);
}


void SQLParserTest::testODBCCallParameters()
{
	// ODBC numbers the return marker as parameter 1 and binds it positionally,
	// so it has to reach the parameter list like any other placeholder -
	// Utility::boundSQL derives its arity check from that list.
	SQLParserResult result;
	SQLParser::parse("{? = call some_proc(?, ?)};", &result);
	assertTrue (result.isValid());
	assertEqual(3, static_cast<int>(result.parameters().size()));

	result.reset();
	SQLParser::parse("{call some_proc(?, ?)};", &result);
	assertTrue (result.isValid());
	assertEqual(2, static_cast<int>(result.parameters().size()));
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
	CppUnit_addTest(pSuite, SQLParserTest, testTopWithParentheses);
	CppUnit_addTest(pSuite, SQLParserTest, testOffsetFetchNext);
	CppUnit_addTest(pSuite, SQLParserTest, testBracketedIdentifiers);
	CppUnit_addTest(pSuite, SQLParserTest, testThreePartTableName);
	CppUnit_addTest(pSuite, SQLParserTest, testArrayLiteralNotShadowedByBracketIdentifier);
	CppUnit_addTest(pSuite, SQLParserTest, testDialectStatements);
	CppUnit_addTest(pSuite, SQLParserTest, testCarriageReturn);
	CppUnit_addTest(pSuite, SQLParserTest, testODBCCallParameters);
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
