#ifndef SQLPARSER_CREATE_STATEMENT_H
#define SQLPARSER_CREATE_STATEMENT_H

#include "ColumnType.h"
#include "SQLStatement.h"

#include <ostream>
#include <unordered_set>

namespace hsql {
struct SelectStatement;

enum struct ConstraintType { ForeignKey, NotNull, Null, PrimaryKey, Unique };
std::ostream& operator<<(std::ostream& os, const ConstraintType constraint_type);

// Superclass for both TableConstraint and ColumnDefinition.
struct SQLParser_API TableElement {
  virtual ~TableElement() = default;
};

// Represents definition of a table constraint.
struct SQLParser_API TableConstraint : TableElement {
  TableConstraint(ConstraintType keyType, std::vector<char*>* columnNames);

  ~TableConstraint() override;

  ConstraintType type;
  std::vector<char*>* columnNames;
};

// Table and columns referenced by foreign key constraint on table or column level.
struct SQLParser_API ReferencesSpecification {
  ReferencesSpecification(char* schema, char* table, std::vector<char*>* columns);
  ~ReferencesSpecification();

  char* schema;
  char* table;
  std::vector<char*>* columns;
};

// Foreign key constraint on table level (when specified as table element).
struct SQLParser_API ForeignKeyConstraint : TableConstraint {
  ForeignKeyConstraint(std::vector<char*>* columnNames, ReferencesSpecification* references);
  ~ForeignKeyConstraint() override;

  ReferencesSpecification* references;
};

// Represents definition of a table column
struct SQLParser_API ColumnDefinition : TableElement {
  ColumnDefinition(char* name, ColumnType type, std::unordered_set<ConstraintType>* column_constraints,
                   std::vector<ReferencesSpecification*>* references);
  ~ColumnDefinition() override;

  // By default, columns are nullable. However, we track if a column is explicitly requested to be nullable to
  // notice conflicts with PRIMARY KEY table constraints.
  bool trySetNullableExplicit();

  std::unordered_set<ConstraintType>* column_constraints;
  char* name;
  ColumnType type;
  bool nullable;
  std::vector<ReferencesSpecification*>* references;
};

struct SQLParser_API ColumnConstraints {
  explicit ColumnConstraints();

  std::unordered_set<ConstraintType>* constraints;
  std::vector<ReferencesSpecification*>* references;
};

enum CreateType {
  kCreateTable,
  kCreateTableFromTbl,  // Hyrise file format
  kCreateView,
  kCreateIndex
};

// Represents SQL Create statements.
// Example: "CREATE TABLE students (name TEXT, student_number INTEGER, city TEXT, grade DOUBLE)"
struct SQLParser_API CreateStatement : SQLStatement {
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
