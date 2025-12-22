#ifndef SQLPARSER_DROP_STATEMENT_H
#define SQLPARSER_DROP_STATEMENT_H

#include "SQLStatement.h"

// Note: Implementations of constructors and destructors can be found in statements.cpp.
namespace hsql {

enum DropType { kDropTable, kDropSchema, kDropIndex, kDropView, kDropPreparedStatement };

// Represents SQL Delete statements.
// Example "DROP TABLE students;"
struct SQLParser_API DropStatement : SQLStatement {
  DropStatement(DropType type);
  ~DropStatement() override;

  DropType type;
  bool ifExists;
  char* schema;
  char* name;
  char* indexName;
};

}  // namespace hsql
#endif