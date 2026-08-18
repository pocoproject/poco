#ifndef SQLPARSER_TABLEREF_H
#define SQLPARSER_TABLEREF_H

#include <stdio.h>
#include <vector>
#include "Expr.h"

namespace hsql {

struct SelectStatement;
struct JoinDefinition;
struct TableRef;

// Possible table reference types.
enum TableRefType { kTableName, kTableSelect, kTableJoin, kTableCrossProduct, kTableFunc, kTableValues };

struct SQLParser_API TableName {
  char* schema;
  char* name;
};

struct SQLParser_API Alias {
  Alias(char* name, std::vector<char*>* columns = nullptr);
  ~Alias();

  char* name;
  std::vector<char*>* columns;
};

// Holds reference to tables. Can be either table names or a select statement.
struct SQLParser_API TableRef {
  TableRef(TableRefType type);
  virtual ~TableRef();

  TableRefType type;

  char* schema;
  char* name;
  Alias* alias;

  SelectStatement* select;
  std::vector<TableRef*>* list;
  JoinDefinition* join;

  // Set for kTableFunc: the table-valued function call this reference is,
  // e.g. FROM STRING_SPLIT(s, ','). name mirrors the function name so
  // getName() keeps working for every table reference type.
  Expr* func;

  // Set for kTableValues: the rows of a table value constructor, e.g.
  // FROM (VALUES (1, 'a'), (2, 'b')) AS t(id, name). Every element is one row,
  // held as an array expression.
  std::vector<Expr*>* values;

  // Returns true if a schema is set.
  bool hasSchema() const;

  // Returns the alias, if it is set. Otherwise the name.
  const char* getName() const;
};

// Possible types of joins.
enum JoinType { kJoinInner, kJoinFull, kJoinLeft, kJoinRight, kJoinCross, kJoinNatural };

// Definition of a join construct.
struct SQLParser_API JoinDefinition {
  JoinDefinition();
  virtual ~JoinDefinition();

  TableRef* left;
  TableRef* right;
  Expr* condition;
  std::vector<char*>* namedColumns;

  JoinType type;
};

}  // namespace hsql
#endif
