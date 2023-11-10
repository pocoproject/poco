
#include "SQLParser.h"
#include "sql_asserts.h"
#include "thirdparty/microtest/microtest.h"

using hsql::kExprLiteralInt;
using hsql::kExprParameter;

using hsql::kStmtDrop;
using hsql::kStmtExecute;
using hsql::kStmtInsert;
using hsql::kStmtPrepare;
using hsql::kStmtSelect;

using hsql::kDropPreparedStatement;

using hsql::DropStatement;
using hsql::ExecuteStatement;
using hsql::InsertStatement;
using hsql::PrepareStatement;
using hsql::SelectStatement;

TEST(PrepareSingleStatementTest) {
  TEST_PARSE_SINGLE_SQL("PREPARE test FROM 'SELECT * FROM students WHERE grade = ?';", kStmtPrepare, PrepareStatement,
                        result, prepare);

  ASSERT_STREQ(prepare->name, "test");
  ASSERT_STREQ(prepare->query, "SELECT * FROM students WHERE grade = ?");

  TEST_PARSE_SINGLE_SQL(prepare->query, kStmtSelect, SelectStatement, result2, select);

  ASSERT_EQ(result2.parameters().size(), 1);
  ASSERT(select->whereClause->expr2->isType(kExprParameter))
  ASSERT_EQ(select->whereClause->expr2->ival, 0)
}

TEST(DeallocatePrepareStatementTest) {
  TEST_PARSE_SINGLE_SQL("DEALLOCATE PREPARE test;", kStmtDrop, DropStatement, result, drop);

  ASSERT_EQ(drop->type, kDropPreparedStatement);
  ASSERT_STREQ(drop->name, "test");
}

TEST(StatementWithParameters) {
  TEST_PARSE_SINGLE_SQL("SELECT * FROM test WHERE a = ? AND b = ?", kStmtSelect, SelectStatement, result, stmt);

  const hsql::Expr* eq1 = stmt->whereClause->expr;
  const hsql::Expr* eq2 = stmt->whereClause->expr2;

  ASSERT_EQ(result.parameters().size(), 2);

  ASSERT_EQ(eq1->opType, hsql::kOpEquals)
  ASSERT(eq1->expr->isType(hsql::kExprColumnRef))
  ASSERT(eq1->expr2->isType(kExprParameter))
  ASSERT_EQ(eq1->expr2->ival, 0)
  ASSERT_EQ(result.parameters()[0], eq1->expr2);

  ASSERT_EQ(eq2->opType, hsql::kOpEquals)
  ASSERT(eq2->expr->isType(hsql::kExprColumnRef))
  ASSERT(eq2->expr2->isType(kExprParameter))
  ASSERT_EQ(eq2->expr2->ival, 1)
  ASSERT_EQ(result.parameters()[1], eq2->expr2);
}

TEST(ExecuteStatementTest) {
  TEST_PARSE_SINGLE_SQL("EXECUTE test(1, 2);", kStmtExecute, ExecuteStatement, result, stmt);

  ASSERT_STREQ(stmt->name, "test");
  ASSERT_EQ(stmt->parameters->size(), 2);
}

TEST(ExecuteStatementTestNoParam) {
  TEST_PARSE_SINGLE_SQL("EXECUTE test();", kStmtExecute, ExecuteStatement, result, stmt);

  ASSERT_STREQ(stmt->name, "test");
  ASSERT_EQ(stmt->parameters, 0);
}

TEST(ExecuteStatementTestNoParamList) {
  TEST_PARSE_SINGLE_SQL("EXECUTE test;", kStmtExecute, ExecuteStatement, result, stmt);

  ASSERT_STREQ(stmt->name, "test");
  ASSERT_EQ(stmt->parameters, 0);
}
