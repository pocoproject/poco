#ifndef SQLPARSER_ALTER_STATEMENT_H
#define SQLPARSER_ALTER_STATEMENT_H

#include "SQLStatement.h"

// Note: Implementations of constructors and destructors can be found in statements.cpp.
namespace hsql {

enum ActionType {
  DropColumn,
};

struct AlterAction {
  AlterAction(ActionType type);
  ActionType type;
  virtual ~AlterAction();
};

struct DropColumnAction : AlterAction {
  DropColumnAction(char* column_name);
  char* columnName;
  bool ifExists;

  ~DropColumnAction() override;
};

// Represents SQL Alter Table statements.
// Example "ALTER TABLE students DROP COLUMN name;"
struct AlterStatement : SQLStatement {
  AlterStatement(char* name, AlterAction* action);
  ~AlterStatement() override;

  char* schema;
  bool ifTableExists;
  char* name;
  AlterAction* action;
};
}  // namespace hsql

#endif
