#include "CreateStatement.h"
#include "SelectStatement.h"

namespace hsql {

std::ostream& operator<<(std::ostream& os, const ConstraintType constraint_type) {
  switch (constraint_type) {
    case ConstraintType::Null:
      os << "NULL";
      break;
    case ConstraintType::NotNull:
      os << "NOT NULL";
      break;
    case ConstraintType::ForeignKey:
      os << "FOREIGN KEY";
      break;
    case ConstraintType::PrimaryKey:
      os << "PRIMARY KEY";
      break;
    case ConstraintType::Unique:
      os << "UNIQUE";
      break;
  }
  return os;
}

// Constraints
TableConstraint::TableConstraint(ConstraintType type, std::vector<char*>* columnNames)
    : type(type), columnNames(columnNames) {}

TableConstraint::~TableConstraint() {
  for (auto* column : *columnNames) {
    free(column);
  }
  delete columnNames;
}

// Foreign key constraint
ReferencesSpecification::ReferencesSpecification(char* schema, char* table, std::vector<char*>* columns)
    : schema{schema}, table{table}, columns{columns} {};

ReferencesSpecification::~ReferencesSpecification() {
  free(schema);
  free(table);
  if (columns) {
    for (auto* column : *columns) {
      free(column);
    }
    delete columns;
  }
}

ForeignKeyConstraint::ForeignKeyConstraint(std::vector<char*>* columnNames, ReferencesSpecification* references)
    : TableConstraint(ConstraintType::ForeignKey, columnNames), references{references} {}

ForeignKeyConstraint::~ForeignKeyConstraint() { delete references; }

ColumnConstraints::ColumnConstraints()
    : constraints{new std::unordered_set<ConstraintType>()}, references{new std::vector<ReferencesSpecification*>} {}

// ColumnDefinition
ColumnDefinition::ColumnDefinition(char* name, ColumnType type, std::unordered_set<ConstraintType>* column_constraints,
                                   std::vector<ReferencesSpecification*>* references)
    : column_constraints(column_constraints), name(name), type(type), nullable(true), references(references) {}

ColumnDefinition::~ColumnDefinition() {
  free(name);
  delete column_constraints;
  if (references) {
    for (auto* ref : *references) {
      delete ref;
    }
  }
  delete references;
}

bool ColumnDefinition::trySetNullableExplicit() {
  if (column_constraints->count(ConstraintType::NotNull) || column_constraints->count(ConstraintType::PrimaryKey)) {
    if (column_constraints->count(ConstraintType::Null)) {
      return false;
    }
    nullable = false;
  }

  return true;
}

// CreateStatemnet
CreateStatement::CreateStatement(CreateType type)
    : SQLStatement(kStmtCreate),
      type(type),
      ifNotExists(false),
      filePath(nullptr),
      schema(nullptr),
      tableName(nullptr),
      indexName(nullptr),
      indexColumns(nullptr),
      columns(nullptr),
      tableConstraints(nullptr),
      viewColumns(nullptr),
      select(nullptr) {}

CreateStatement::~CreateStatement() {
  free(filePath);
  free(schema);
  free(tableName);
  free(indexName);
  delete select;

  if (columns) {
    for (auto* def : *columns) {
      delete def;
    }
    delete columns;
  }

  if (tableConstraints) {
    for (auto* def : *tableConstraints) {
      delete def;
    }
    delete tableConstraints;
  }

  if (indexColumns) {
    for (char* column : *indexColumns) {
      free(column);
    }
    delete indexColumns;
  }

  if (viewColumns) {
    for (char* column : *viewColumns) {
      free(column);
    }
    delete viewColumns;
  }
}

void CreateStatement::setColumnDefsAndConstraints(std::vector<TableElement*>* tableElements) {
  columns = new std::vector<ColumnDefinition*>();
  tableConstraints = new std::vector<TableConstraint*>();

  for (auto tableElem : *tableElements) {
    if (auto* colDef = dynamic_cast<ColumnDefinition*>(tableElem)) {
      columns->emplace_back(colDef);
    } else if (auto* tableConstraint = dynamic_cast<TableConstraint*>(tableElem)) {
      tableConstraints->emplace_back(tableConstraint);
    }
  }
}

}  // namespace hsql
