#ifndef SQLPARSER_PREPARE_STATEMENT_H
#define SQLPARSER_PREPARE_STATEMENT_H

#include "SQLStatement.h"

namespace hsql {

// Represents SQL Prepare statements.
// Example: PREPARE test FROM 'SELECT * FROM test WHERE a = ?;'
struct PrepareStatement : SQLStatement {
  PrepareStatement();
  ~PrepareStatement() override;

  char* name;

  // The query that is supposed to be prepared.
  char* query;
};

}  // namespace hsql

#endif
