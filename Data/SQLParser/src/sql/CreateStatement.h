#ifndef SQLPARSER_CREATE_STATEMENT_H
#define SQLPARSER_CREATE_STATEMENT_H

#include "ColumnType.h"
#include "SQLStatement.h"

#include <ostream>
#include <unordered_set>

// Note: Implementations of constructors and destructors can be found in statements.cpp.
namespace hsql {
struct SQLParser_API SelectStatement;

enum struct ConstraintType { None, NotNull, Null, PrimaryKey, Unique };

// Superclass for both TableConstraint and Column Definition
struct SQLParser_API TableElement {
  virtual ~TableElement() {}
};

// Represents definition of a table constraint
struct SQLParser_API TableConstraint : TableElement {
  TableConstraint(ConstraintType keyType, std::vector<char*>* columnNames);

  ~TableConstraint() override;

  ConstraintType type;
  std::vector<char*>* columnNames;
};

// Represents definition of a table column
struct SQLParser_API ColumnDefinition : TableElement {
  ColumnDefinition(char* name, ColumnType type, std::unordered_set<ConstraintType>* column_constraints);

  ~ColumnDefinition() override;

  // By default, columns are nullable. However, we track if a column is explicitly requested to be nullable to
  // notice conflicts with PRIMARY KEY table constraints.
  bool trySetNullableExplicit() {
    if (column_constraints->count(ConstraintType::NotNull) || column_constraints->count(ConstraintType::PrimaryKey)) {
      if (column_constraints->count(ConstraintType::Null)) {
        return false;
      }
      nullable = false;
    }

    return true;
  }

  std::unordered_set<ConstraintType>* column_constraints;
  char* name;
  ColumnType type;
  bool nullable;
};

enum CreateType {
  kCreateTable,
  kCreateTableFromTbl,  // Hyrise file format
  kCreateView,
  kCreateIndex
};

// Represents SQL Create statements.
// Example: "CREATE TABLE students (name TEXT, student_number INTEGER, city TEXT, grade DOUBLE)"
struct CreateStatement : SQLStatement {
  CreateStatement(CreateType type);
  ~CreateStatement() override;

  void setColumnDefsAndConstraints(std::vector<TableElement*>* tableElements);

  CreateType type;
  bool ifNotExists;                                 // default: false
  char* filePath;                                   // default: nullptr
  char* schema;                                     // default: nullptr
  char* tableName;                                  // default: nullptr
  char* indexName;                                  // default: nullptr
  std::vector<char*>* indexColumns;                 // default: nullptr
  std::vector<ColumnDefinition*>* columns;          // default: nullptr
  std::vector<TableConstraint*>* tableConstraints;  // default: nullptr
  std::vector<char*>* viewColumns;
  SelectStatement* select;
};

}  // namespace hsql

#endif
