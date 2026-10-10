#ifndef SQLPARSER_DELETE_STATEMENT_H
#define SQLPARSER_DELETE_STATEMENT_H

#include "SQLStatement.h"

// Note: Implementations of constructors and destructors can be found in statements.cpp.
namespace hsql {

// Represents SQL Delete statements.
// Example: "DELETE FROM students WHERE grade > 3.0"
// Note: if (expr == nullptr) => delete all rows (truncate)
struct SQLParser_API DeleteStatement : SQLStatement {
  DeleteStatement();
  ~DeleteStatement() override;

  char* schema;
  char* tableName;
  Expr* expr;
};

}  // namespace hsql

#endif
