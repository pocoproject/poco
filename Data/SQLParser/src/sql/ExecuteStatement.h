#ifndef SQLPARSER_EXECUTE_STATEMENT_H
#define SQLPARSER_EXECUTE_STATEMENT_H

#include "SQLStatement.h"

namespace hsql {

// Represents SQL Execute statements.
// Example: "EXECUTE ins_prep(100, "test", 2.3);"
struct SQLParser_API ExecuteStatement : SQLStatement {
  ExecuteStatement();
  ~ExecuteStatement() override;

  char* name;
  std::vector<Expr*>* parameters;

  // The return value marker of an ODBC call escape, e.g. the first '?' of
  // "{? = call p(?)}". ODBC counts it as parameter 1 and binds it positionally,
  // so it is kept as a placeholder expression rather than dropped. Null for
  // an EXECUTE statement and for a call escape written without it.
  Expr* returnValue;
};

}  // namespace hsql

#endif
