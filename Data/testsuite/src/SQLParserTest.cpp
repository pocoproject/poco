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


void SQLParserTest::testNonReservedKeywords()
{
	// Date part keywords as function arguments. The abbreviated spellings
	// (mi, hh, ...) lex as identifiers and have always parsed, so before this
	// the behaviour depended on how the date part was spelled.
	{
		std::string query = "SELECT * FROM Test WHERE CreatedAt >= DATEADD(MINUTE, -10, GETDATE());";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());
		assertTrue(result.getStatement(0)->type() == kStmtSelect);
	}

	// Keywords as function names and as column names.
	{
		std::string query = "SELECT ISNULL(A, 0), CHAR(10), FORMAT(A, '00'), YEAR, MONTH FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertEqual(5, pSelect->selectList->size());
		assertTrue(pSelect->selectList->at(0)->type == kExprFunctionRef);
		assertEqual("ISNULL", std::string(pSelect->selectList->at(0)->name));
		assertTrue(pSelect->selectList->at(3)->type == kExprColumnRef);
		assertEqual("YEAR", std::string(pSelect->selectList->at(3)->name));
	}

	// The keyword uses these tokens still have: EXTRACT's datetime field, the
	// ISNULL postfix operator, CAST's column type and the INTERVAL qualifier.
	{
		std::string query = "SELECT EXTRACT(MINUTE FROM D) FROM Test;"
			"SELECT A FROM Test WHERE B ISNULL;"
			"SELECT CAST(A AS INT) FROM Test;"
			"SELECT INTERVAL '1' MINUTE FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(4, result.size());
	}
}


void SQLParserTest::testNextValueFor()
{
	// T-SQL sequence expression. It yields a value like a function call does,
	// so it is kept as one, with the sequence as its single argument.
	{
		std::string query = "SELECT NEXT VALUE FOR mydb.dbo.Seq AS NextId;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertTrue(pSelect->selectList->at(0)->type == kExprFunctionRef);
		assertEqual("NEXT VALUE FOR", std::string(pSelect->selectList->at(0)->name));
		assertEqual(1, pSelect->selectList->at(0)->exprList->size());
		assertEqual("mydb.dbo.Seq", std::string(pSelect->selectList->at(0)->exprList->at(0)->name));
	}

	// NEXT VALUE FOR is lexed as one token, so "value" keeps working as a plain
	// column name and as a bare alias, and FETCH NEXT is unaffected.
	{
		std::string query = "SELECT Id, Value FROM Test WHERE Value = 1;"
			"SELECT COALESCE(MAX(SeqValue) + 1, 1) Value FROM Test;"
			"SELECT A FROM Test ORDER BY A OFFSET 0 ROWS FETCH NEXT 10 ROWS ONLY;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(3, result.size());

		const SelectStatement* pAliased = static_cast<const SelectStatement*>(result.getStatement(1));
		assertEqual("Value", std::string(pAliased->selectList->at(0)->alias));
	}
}


void SQLParserTest::testODBCEscapes()
{
	// ODBC outer join escape - the escape only marks the join for the driver,
	// so the join itself is what ends up in the tree.
	{
		std::string query = "SELECT * FROM {oj Test LEFT OUTER JOIN Other ON Test.Id = Other.Id};";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertNotNull(pSelect->fromTable);
		assertTrue(pSelect->fromTable->type == kTableJoin);
		assertTrue(pSelect->fromTable->join->type == kJoinLeft);
	}

	// ODBC procedure call escape, with and without the "? =" return marker and
	// with a qualified procedure name.
	{
		std::string query = "{? = call SomeProc(?, 'a', 1)};"
			"{call SomeProc(?)};"
			"{ ? = call [mydb].[dbo].[SomeProc]('a') };";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(3, result.size());
		assertTrue(result.getStatement(0)->type() == kStmtExecute);
		assertTrue(result.getStatement(1)->type() == kStmtExecute);
		assertTrue(result.getStatement(2)->type() == kStmtExecute);
	}

	// "oj" outside the escape stays an ordinary identifier, and a plain
	// EXECUTE is unchanged.
	{
		std::string query = "SELECT oj FROM Test oj;"
			"EXECUTE SomeProc(1, 2);";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(2, result.size());
		assertTrue(result.getStatement(0)->type() == kStmtSelect);
		assertTrue(result.getStatement(1)->type() == kStmtExecute);
	}
}


void SQLParserTest::testTopWithExpression()
{
	// T-SQL requires the parentheses exactly when TOP takes an expression
	// rather than a constant, so TOP (?) is the documented form for a
	// parameterised row count. The bare form stays literal-only, since
	// "TOP A" would be ambiguous with the select list.
	{
		std::string query = "SELECT TOP (?) * FROM Test;"
			"SELECT TOP (:limit) * FROM Test;"
			"SELECT TOP (N + 1) * FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(3, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertNotNull(pSelect->limit);
		assertTrue(pSelect->limit->limit->type == kExprParameter);
		assertNull(pSelect->limit->offset);
	}

	// Both literal forms keep yielding the same LimitDescription as before.
	{
		std::string query = "SELECT TOP (10) * FROM Test;"
			"SELECT TOP 10 * FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(2, result.size());

		for (std::size_t i = 0; i < 2; ++i)
		{
			const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(i));
			assertNotNull(pSelect->limit);
			assertTrue(pSelect->limit->limit->type == kExprLiteralInt);
			assertEqual(10, pSelect->limit->limit->ival);
		}
	}
}


void SQLParserTest::testHierarchicalQuery()
{
	// CONNECT BY on its own, with the LEVEL pseudo column. LEVEL is not a
	// keyword, so it stays an ordinary column reference.
	{
		std::string query = "SELECT LEVEL FROM Dual CONNECT BY LEVEL <= 5;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertNotNull(pSelect->connectBy);
		assertTrue(pSelect->connectBy->opType == kOpLessEq);
		assertNull(pSelect->startWith);
	}

	// START WITH plus CONNECT BY PRIOR - PRIOR binds to the operand after it,
	// so the condition stays a comparison of PRIOR Id against ParentId.
	{
		std::string query = "SELECT Id FROM Test START WITH Id = 1 CONNECT BY PRIOR Id = ParentId;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertNotNull(pSelect->startWith);
		assertNotNull(pSelect->connectBy);
		assertTrue(pSelect->connectBy->expr->type == kExprOperator);
		assertTrue(pSelect->connectBy->expr->opType == kOpPrior);
	}

	// CONNECT and START stay usable as ordinary names, and a statement without
	// the clauses leaves both null.
	{
		std::string query = "SELECT Connect, Start FROM Test WHERE Connect = 1;"
			"SELECT A FROM Test WHERE B = 1 GROUP BY A ORDER BY A;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(2, result.size());

		const SelectStatement* pPlain = static_cast<const SelectStatement*>(result.getStatement(1));
		assertNull(pPlain->startWith);
		assertNull(pPlain->connectBy);
	}
}


void SQLParserTest::testWithinGroup()
{
	// LISTAGG(X, ', ') WITHIN GROUP (ORDER BY Y) - ordered-set aggregate. The
	// sort order belongs to the aggregate itself, so it is kept in
	// withinGroupOrder, separate from the window description an OVER produces.
	{
		std::string query = "SELECT LISTAGG(A, ', ') WITHIN GROUP (ORDER BY B) FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertTrue(pSelect->selectList->at(0)->type == kExprFunctionRef);
		assertEqual("LISTAGG", std::string(pSelect->selectList->at(0)->name));
		assertNotNull(pSelect->selectList->at(0)->withinGroupOrder);
		assertEqual(1, pSelect->selectList->at(0)->withinGroupOrder->size());
		assertNull(pSelect->selectList->at(0)->windowDescription);
	}

	// Several sort keys with direction, and an alias on the aggregate.
	{
		std::string query = "SELECT LISTAGG(A, ', ') WITHIN GROUP (ORDER BY B DESC, C ASC) AS Lst FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertEqual(2, pSelect->selectList->at(0)->withinGroupOrder->size());
		assertEqual("Lst", std::string(pSelect->selectList->at(0)->alias));
	}

	// Plain aggregates, OVER windows, GROUP BY and ORDER BY are unaffected.
	{
		std::string query = "SELECT COUNT(A) FROM Test;"
			"SELECT COUNT(*) OVER (PARTITION BY A ORDER BY B) FROM Test;"
			"SELECT A FROM Test GROUP BY A ORDER BY A;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(3, result.size());

		const SelectStatement* pPlain = static_cast<const SelectStatement*>(result.getStatement(0));
		assertNull(pPlain->selectList->at(0)->withinGroupOrder);
		const SelectStatement* pWindow = static_cast<const SelectStatement*>(result.getStatement(1));
		assertNotNull(pWindow->selectList->at(0)->windowDescription);
		assertNull(pWindow->selectList->at(0)->withinGroupOrder);
	}
}


void SQLParserTest::testOracleOuterJoin()
{
	// WHERE T.Id (+) = U.Id - Oracle's legacy outer join marker, kept in the
	// tree as a unary operator on the column it follows.
	{
		std::string query = "SELECT A FROM Test T, Other U WHERE T.Id (+) = U.Id;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertNotNull(pSelect->whereClause);
		assertTrue(pSelect->whereClause->opType == kOpEquals);
		assertTrue(pSelect->whereClause->expr->type == kExprOperator);
		assertTrue(pSelect->whereClause->expr->opType == kOpOuterJoin);
		assertTrue(pSelect->whereClause->expr->expr->type == kExprColumnRef);
	}

	// Right-hand side, no spaces, and spaces inside the marker.
	{
		std::string query = "SELECT A FROM Test T, Other U WHERE T.Id = U.Id (+);"
			"SELECT A FROM Test T, Other U WHERE T.Id(+)=U.Id;"
			"SELECT A FROM Test T, Other U WHERE T.Id ( + ) = U.Id;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(3, result.size());
	}

	// Only a lone '+' between parentheses is the marker; arithmetic and
	// parenthesized expressions are unaffected.
	{
		std::string query = "SELECT A + 1 FROM Test;"
			"SELECT (A + B) FROM Test;"
			"SELECT A FROM Test WHERE X = (1 + 2);";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(3, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertTrue(pSelect->selectList->at(0)->opType == kOpPlus);
	}
}


void SQLParserTest::testTableValueConstructor()
{
	// FROM (VALUES (...), (...)) AS t(cols) - each row is kept as an array
	// expression, the column names come from the alias.
	{
		std::string query = "SELECT * FROM (VALUES (0, 'Any'), (1, 'One')) AS V(Id, Name);";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertNotNull(pSelect->fromTable);
		assertTrue(pSelect->fromTable->type == kTableValues);
		assertNotNull(pSelect->fromTable->values);
		assertEqual(2, pSelect->fromTable->values->size());
		assertTrue(pSelect->fromTable->values->at(0)->type == kExprArray);
		assertEqual(2, pSelect->fromTable->values->at(0)->exprList->size());

		assertNotNull(pSelect->fromTable->alias);
		assertEqual("V", std::string(pSelect->fromTable->alias->name));
		assertNotNull(pSelect->fromTable->alias->columns);
		assertEqual(2, pSelect->fromTable->alias->columns->size());
	}

	// A single row, a subquery in FROM and INSERT ... VALUES are unaffected.
	{
		std::string query = "SELECT * FROM (VALUES (1)) AS V(X);"
			"SELECT A FROM (SELECT 1 AS A) X;"
			"INSERT INTO Test VALUES (1, 'a');";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(3, result.size());
		assertTrue(static_cast<const SelectStatement*>(result.getStatement(1))->fromTable->type == kTableSelect);
		assertTrue(result.getStatement(2)->type() == kStmtInsert);
	}
}


void SQLParserTest::testTableValuedFunction()
{
	// FROM STRING_SPLIT(s, ',') - the table reference is the function call
	// itself, kept as kTableFunc with the call in TableRef::func.
	{
		std::string query = "SELECT Value FROM STRING_SPLIT('a,b', ',');";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertNotNull(pSelect->fromTable);
		assertTrue(pSelect->fromTable->type == kTableFunc);
		assertNotNull(pSelect->fromTable->func);
		assertEqual("STRING_SPLIT", std::string(pSelect->fromTable->func->name));
		assertEqual(2, pSelect->fromTable->func->exprList->size());
		assertEqual("STRING_SPLIT", std::string(pSelect->fromTable->getName()));
	}

	// Aliased, schema qualified, and nested in a subquery.
	{
		std::string query = "SELECT * FROM STRING_SPLIT('a,b', ',') AS S;"
			"SELECT * FROM dbo.fn_split('a', ',') F;"
			"SELECT A FROM Test WHERE Id IN (SELECT CAST(Value AS INT) FROM STRING_SPLIT('1,2', ','));";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(3, result.size());

		const SelectStatement* pAliased = static_cast<const SelectStatement*>(result.getStatement(0));
		assertEqual("S", std::string(pAliased->fromTable->getName()));
	}

	// A window function and an ordered-set aggregate are not table references,
	// so only a plain call is accepted there - both stay valid in a select list.
	{
		SQLParserResult windowResult;
		SQLParser::parse("SELECT * FROM COUNT(*) OVER (PARTITION BY A);", &windowResult);
		assertTrue(!windowResult.isValid());

		SQLParserResult aggregateResult;
		SQLParser::parse("SELECT * FROM LISTAGG(A, ', ') WITHIN GROUP (ORDER BY B);", &aggregateResult);
		assertTrue(!aggregateResult.isValid());

		SQLParserResult selectListResult;
		SQLParser::parse("SELECT COUNT(*) OVER (PARTITION BY A) FROM Test;", &selectListResult);
		assertTrue(selectListResult.isValid());
	}

	// Plain table references and subqueries in FROM are unchanged.
	{
		std::string query = "SELECT * FROM Test A, Other B WHERE A.Id = B.Id;"
			"SELECT * FROM (SELECT 1) X;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(2, result.size());
		assertTrue(static_cast<const SelectStatement*>(result.getStatement(0))->fromTable->type == kTableCrossProduct);
		assertTrue(static_cast<const SelectStatement*>(result.getStatement(1))->fromTable->type == kTableSelect);
	}
}


void SQLParserTest::testRowConstructorIn()
{
	// (A, B) IN (SELECT X, Y FROM ...) - row constructor on the left of IN.
	// The tuple is represented as an array expression, so no element is lost.
	{
		std::string query = "SELECT C FROM Test WHERE (A, B) IN (SELECT X, Y FROM Other);";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertNotNull(pSelect->whereClause);
		assertTrue(pSelect->whereClause->opType == kOpIn);
		assertTrue(pSelect->whereClause->expr->type == kExprArray);
		assertEqual(2, pSelect->whereClause->expr->exprList->size());
	}

	// NOT IN form, and the unchanged single-operand forms.
	{
		std::string query = "SELECT C FROM Test WHERE (A, B, C) NOT IN (SELECT X, Y, Z FROM Other);"
			"SELECT C FROM Test WHERE (A) IN (SELECT X FROM Other);"
			"SELECT C FROM Test WHERE A IN (1, 2, 3);";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(3, result.size());

		const SelectStatement* pSingle = static_cast<const SelectStatement*>(result.getStatement(1));
		assertTrue(pSingle->whereClause->expr->type == kExprColumnRef);
	}
}


void SQLParserTest::testMultiPartColumnName()
{
	// Three- and four-part column references, the column-side counterpart of
	// the multi-part table names from #5429. The qualifiers are folded into
	// the single table slot Expr has.
	{
		std::string query = "SELECT dbo.Test.A, mydb.dbo.Test.B FROM mydb.dbo.Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertEqual(2, pSelect->selectList->size());
		assertTrue(pSelect->selectList->at(0)->type == kExprColumnRef);
		assertEqual("dbo.Test", std::string(pSelect->selectList->at(0)->table));
		assertEqual("A", std::string(pSelect->selectList->at(0)->name));
		assertEqual("mydb.dbo.Test", std::string(pSelect->selectList->at(1)->table));
		assertEqual("B", std::string(pSelect->selectList->at(1)->name));
	}

	// Multi-part qualified star, and bracket-quoted names on both sides of a
	// join condition - the shape this was found as.
	{
		std::string query = "SELECT mydb.dbo.Test.* FROM mydb.dbo.Test;"
			"SELECT * FROM [mydb].[dbo].Test INNER JOIN [mydb].[dbo].Other "
			"ON [mydb].[dbo].Test.Id = [mydb].[dbo].Other.Id;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(2, result.size());
	}

	// One- and two-part references and schema-qualified function calls are
	// unchanged - they share the leading IDENTIFIER '.' IDENTIFIER prefix.
	{
		std::string query = "SELECT C, A.B, A.*, sys.uuid() FROM X A;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertEqual(4, pSelect->selectList->size());
		assertNull(pSelect->selectList->at(0)->table);
		assertEqual("A", std::string(pSelect->selectList->at(1)->table));
		assertTrue(pSelect->selectList->at(2)->type == kExprStar);
		assertTrue(pSelect->selectList->at(3)->type == kExprFunctionRef);
	}
}


void SQLParserTest::testStringAlias()
{
	// T-SQL accepts a string literal as an alias, e.g. SELECT A AS 'My Col'.
	{
		std::string query = "SELECT A AS 'Coil Id', B AS 'X' FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertEqual(2, pSelect->selectList->size());
		assertEqual("Coil Id", std::string(pSelect->selectList->at(0)->alias));
		assertEqual("X", std::string(pSelect->selectList->at(1)->alias));
	}

	// Only the AS form is accepted, so a bare string in the select list is
	// still a string literal, not an alias for the expression before it.
	{
		std::string query = "SELECT 'literal', A AS B FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertEqual(2, pSelect->selectList->size());
		assertTrue(pSelect->selectList->at(0)->type == kExprLiteralString);
		assertNull(pSelect->selectList->at(0)->alias);
	}
}


void SQLParserTest::testScientificNotationLiteral()
{
	// In a select list a missing exponent rule does not fail the parse: the
	// mantissa is scanned as a number and the exponent as an identifier, so
	// "SELECT 1.5e3" used to parse as "SELECT 1.5 AS e3" - one literal with an
	// alias. Pin both the literal type and the absence of an alias.
	{
		std::string query = "SELECT 1.5e3 FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());
		assertTrue(result.getStatement(0)->type() == kStmtSelect);

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertEqual(1, pSelect->selectList->size());
		assertTrue(pSelect->selectList->at(0)->type == kExprLiteralFloat);
		assertEqualDelta(1500.0, pSelect->selectList->at(0)->fval, 0.0);
		assertNull(pSelect->selectList->at(0)->alias);
	}

	// Outside a select list there is no alias to absorb the exponent, so the
	// same literal was a syntax error.
	{
		std::string query = "SELECT * FROM Test WHERE Value = 0.5e-2;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());
		assertTrue(result.getStatement(0)->type() == kStmtSelect);
	}

	// Signed and leading-dot exponents, and an exponent without a fraction.
	{
		std::string query = "SELECT 1.5E+3, 5e-1, .5e1, 2e10 FROM Test;";
		SQLParserResult result;
		SQLParser::parse(query, &result);
		assertTrue(result.isValid());
		assertEqual(1, result.size());

		const SelectStatement* pSelect = static_cast<const SelectStatement*>(result.getStatement(0));
		assertEqual(4, pSelect->selectList->size());
		for (const auto* pExpr: *pSelect->selectList)
		{
			assertTrue(pExpr->type == kExprLiteralFloat);
			assertNull(pExpr->alias);
		}
	}
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
	CppUnit_addTest(pSuite, SQLParserTest, testNonReservedKeywords);
	CppUnit_addTest(pSuite, SQLParserTest, testNextValueFor);
	CppUnit_addTest(pSuite, SQLParserTest, testODBCEscapes);
	CppUnit_addTest(pSuite, SQLParserTest, testTopWithExpression);
	CppUnit_addTest(pSuite, SQLParserTest, testHierarchicalQuery);
	CppUnit_addTest(pSuite, SQLParserTest, testWithinGroup);
	CppUnit_addTest(pSuite, SQLParserTest, testOracleOuterJoin);
	CppUnit_addTest(pSuite, SQLParserTest, testTableValueConstructor);
	CppUnit_addTest(pSuite, SQLParserTest, testTableValuedFunction);
	CppUnit_addTest(pSuite, SQLParserTest, testRowConstructorIn);
	CppUnit_addTest(pSuite, SQLParserTest, testMultiPartColumnName);
	CppUnit_addTest(pSuite, SQLParserTest, testStringAlias);
	CppUnit_addTest(pSuite, SQLParserTest, testScientificNotationLiteral);
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
