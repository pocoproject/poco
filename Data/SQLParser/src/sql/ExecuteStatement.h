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
};

}  // namespace hsql

#endif
