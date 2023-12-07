#include "thirdparty/microtest/microtest.h"

#include "SQLParser.h"
#include "util/sqlhelper.h"

#include "sql_asserts.h"

#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

using namespace hsql;

std::string readFileContents(std::string file_path) {
  std::ifstream t(file_path.c_str());
  std::string text((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
  return text;
}

TEST(TPCHQueryGrammarTests) {
  std::vector<std::string> files = {
      "test/queries/tpc-h-01.sql", "test/queries/tpc-h-02.sql", "test/queries/tpc-h-03.sql",
      "test/queries/tpc-h-04.sql", "test/queries/tpc-h-05.sql", "test/queries/tpc-h-06.sql",
      "test/queries/tpc-h-07.sql", "test/queries/tpc-h-08.sql", "test/queries/tpc-h-09.sql",
      "test/queries/tpc-h-10.sql", "test/queries/tpc-h-11.sql", "test/queries/tpc-h-12.sql",
      "test/queries/tpc-h-13.sql", "test/queries/tpc-h-14.sql", "test/queries/tpc-h-15.sql",
      "test/queries/tpc-h-16.sql", "test/queries/tpc-h-17.sql", "test/queries/tpc-h-18.sql",
      "test/queries/tpc-h-19.sql", "test/queries/tpc-h-20.sql", "test/queries/tpc-h-21.sql",
      "test/queries/tpc-h-22.sql",
  };

  int testsFailed = 0;
  std::string concatenated = "";
  for (const std::string& file_path : files) {
    std::string query = readFileContents(file_path);

    concatenated += query;
    if (concatenated.back() != ';') concatenated += ';';

    SQLParserResult result;
    SQLParser::parse(query.c_str(), &result);
    if (!result.isValid()) {
      mt::printFailed(file_path.c_str());
      printf("%s           %s (L%d:%d)%s\n", mt::red(), result.errorMsg(), result.errorLine(), result.errorColumn(),
             mt::def());
      ++testsFailed;
    } else {
      mt::printOk(file_path.c_str());
    }
  }

  SQLParserResult result;
  SQLParser::parse(concatenated.c_str(), &result);
  if (!result.isValid()) {
    mt::printFailed("TPCHAllConcatenated");
    printf("%s           %s (L%d:%d)%s\n", mt::red(), result.errorMsg(), result.errorLine(), result.errorColumn(),
           mt::def());
    ++testsFailed;
  } else {
    mt::printOk("TPCHAllConcatenated");
  }

  ASSERT_EQ(testsFailed, 0);
}

TEST(TPCHQueryDetailTest) {
  std::string query = readFileContents("test/queries/tpc-h-20.sql");

  SQLParserResult result;
  SQLParser::parse(query.c_str(), &result);
  ASSERT(result.isValid());
  ASSERT_EQ(result.size(), 1);

  const SQLStatement* stmt20 = result.getStatement(0);
  ASSERT_EQ(stmt20->type(), kStmtSelect);

  const SelectStatement* select20 = (const SelectStatement*)stmt20;
  ASSERT_EQ(select20->selectList->size(), 2);
  ASSERT_STREQ(select20->selectList->at(0)->getName(), "S_NAME");
  ASSERT_STREQ(select20->selectList->at(1)->getName(), "S_ADDRESS");

  // Test WHERE Clause.
  Expr* where = select20->whereClause;
  ASSERT_NOTNULL(where);
  ASSERT(where->isType(kExprOperator));
  ASSERT_EQ(where->opType, kOpAnd);

  Expr* andExpr2 = where->expr;
  ASSERT_NOTNULL(andExpr2);
  ASSERT(andExpr2->isType(kExprOperator));
  ASSERT_EQ(andExpr2->opType, kOpAnd);

  // Test IN expression.
  Expr* inExpr = andExpr2->expr;
  ASSERT_NOTNULL(inExpr);
  ASSERT(inExpr->isType(kExprOperator));
  ASSERT_EQ(inExpr->opType, kOpIn);

  ASSERT_STREQ(inExpr->expr->getName(), "S_SUPPKEY");
  ASSERT_NOTNULL(inExpr->select);
  ASSERT_EQ(inExpr->select->selectList->size(), 1);
  ASSERT(inExpr->select->selectList->at(0)->isType(kExprColumnRef));
  ASSERT_STREQ(inExpr->select->selectList->at(0)->getName(), "PS_SUPPKEY");

  // Test ORDER BY clause.
  ASSERT_NOTNULL(select20->order);
  ASSERT_EQ(select20->order->size(), 1);
  ASSERT(select20->order->at(0)->expr->isType(kExprColumnRef));
  ASSERT_STREQ(select20->order->at(0)->expr->getName(), "S_NAME");
}
