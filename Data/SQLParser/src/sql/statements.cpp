#include "statements.h"

#include <stdint.h>

#include "AlterStatement.h"
#include "ImportExportOptions.h"

namespace hsql {

ColumnType::ColumnType(DataType data_type, int64_t length, int64_t precision, int64_t scale)
    : data_type(data_type), length(length), precision(precision), scale(scale) {}

bool operator==(const ColumnType& lhs, const ColumnType& rhs) {
  if (lhs.data_type != rhs.data_type) {
    return false;
  }
  return lhs.length == rhs.length && lhs.precision == rhs.precision && lhs.scale == rhs.scale;
}

bool operator!=(const ColumnType& lhs, const ColumnType& rhs) { return !(lhs == rhs); }

std::ostream& operator<<(std::ostream& stream, const ColumnType& column_type) {
  switch (column_type.data_type) {
    case DataType::UNKNOWN:
      stream << "UNKNOWN";
      break;
    case DataType::INT:
      stream << "INT";
      break;
    case DataType::BIGINT:
      stream << "BIGINT";
      break;
    case DataType::LONG:
      stream << "LONG";
      break;
    case DataType::FLOAT:
      stream << "FLOAT";
      break;
    case DataType::DOUBLE:
      stream << "DOUBLE";
      break;
    case DataType::REAL:
      stream << "REAL";
      break;
    case DataType::CHAR:
      stream << "CHAR(" << column_type.length << ")";
      break;
    case DataType::VARCHAR:
      stream << "VARCHAR(" << column_type.length << ")";
      break;
    case DataType::DECIMAL:
      stream << "DECIMAL";
      break;
    case DataType::TEXT:
      stream << "TEXT";
      break;
    case DataType::DATETIME:
      stream << "DATETIME";
      break;
    case DataType::DATE:
      stream << "DATE";
      break;
    case DataType::TIME:
      stream << "TIME";
      break;
    case DataType::SMALLINT:
      stream << "SMALLINT";
      break;
    case DataType::BOOLEAN:
      stream << "BOOLEAN";
      break;
  }
  return stream;
}

// DeleteStatement
DeleteStatement::DeleteStatement() : SQLStatement(kStmtDelete), schema(nullptr), tableName(nullptr), expr(nullptr) {}

DeleteStatement::~DeleteStatement() {
  free(schema);
  free(tableName);
  delete expr;
}

// DropStatement
DropStatement::DropStatement(DropType type)
    : SQLStatement(kStmtDrop), type(type), schema(nullptr), name(nullptr), indexName(nullptr) {}

DropStatement::~DropStatement() {
  free(schema);
  free(name);
  free(indexName);
}

// AlterStatement and supportive classes

AlterAction::AlterAction(ActionType type) : type(type) {}

AlterAction::~AlterAction() = default;

DropColumnAction::DropColumnAction(char* column_name)
    : AlterAction(ActionType::DropColumn), columnName(column_name), ifExists(false) {}

DropColumnAction::~DropColumnAction() { free(columnName); }

AlterStatement::AlterStatement(char* name, AlterAction* action)
    : SQLStatement(kStmtAlter), schema(nullptr), ifTableExists(false), name(name), action(action) {}

AlterStatement::~AlterStatement() {
  free(schema);
  free(name);
  delete action;
}

// TransactionStatement
TransactionStatement::TransactionStatement(TransactionCommand command)
    : SQLStatement(kStmtTransaction), command(command) {}

TransactionStatement::~TransactionStatement() {}

// ExecuteStatement
ExecuteStatement::ExecuteStatement() : SQLStatement(kStmtExecute), name(nullptr), parameters(nullptr) {}

ExecuteStatement::~ExecuteStatement() {
  free(name);

  if (parameters) {
    for (Expr* param : *parameters) {
      delete param;
    }
    delete parameters;
  }
}

// ExportStatement
ExportStatement::ExportStatement(ImportType type)
    : SQLStatement(kStmtExport),
      type(type),
      filePath(nullptr),
      schema(nullptr),
      tableName(nullptr),
      select(nullptr),
      encoding(nullptr),
      csv_options(nullptr) {}

ExportStatement::~ExportStatement() {
  free(filePath);
  free(schema);
  free(tableName);
  delete select;
  free(encoding);
  delete csv_options;
}

CsvOptions::CsvOptions() : delimiter(nullptr), null(nullptr), quote(nullptr) {}
CsvOptions::~CsvOptions() {
  free(delimiter);
  free(null);
  free(quote);
}

bool CsvOptions::accept_csv_option(std::pair<CsvOptionType, char*>* option) {
  switch (option->first) {
    case CsvOptionType::Delimiter:
      if (delimiter != nullptr) {
        return false;
      }
      delimiter = option->second;
      break;
    case CsvOptionType::Null:
      if (null != nullptr) {
        return false;
      }
      null = option->second;
      break;
    case CsvOptionType::Quote:
      if (quote != nullptr) {
        return false;
      }
      quote = option->second;
      break;
  }

  return true;
}

ImportExportOptions::ImportExportOptions() : format(kImportAuto), encoding(nullptr), csv_options(nullptr) {}

ImportExportOptions::~ImportExportOptions() {
  free(encoding);
  delete csv_options;
}

// ImportStatement
ImportStatement::ImportStatement(ImportType type)
    : SQLStatement(kStmtImport),
      type(type),
      filePath(nullptr),
      schema(nullptr),
      tableName(nullptr),
      whereClause(nullptr),
      encoding(nullptr),
      csv_options(nullptr) {}

ImportStatement::~ImportStatement() {
  free(filePath);
  free(schema);
  free(tableName);
  delete whereClause;
  free(encoding);
  delete csv_options;
}

// InsertStatement
InsertStatement::InsertStatement(InsertType type)
    : SQLStatement(kStmtInsert),
      type(type),
      schema(nullptr),
      tableName(nullptr),
      columns(nullptr),
      values(nullptr),
      select(nullptr) {}

InsertStatement::~InsertStatement() {
  free(schema);
  free(tableName);
  delete select;

  if (columns) {
    for (char* column : *columns) {
      free(column);
    }
    delete columns;
  }

  if (values) {
    for (Expr* expr : *values) {
      delete expr;
    }
    delete values;
  }
}

// ShowStatament
ShowStatement::ShowStatement(ShowType type) : SQLStatement(kStmtShow), type(type), schema(nullptr), name(nullptr) {}

ShowStatement::~ShowStatement() {
  free(schema);
  free(name);
}

// SelectStatement.h

// OrderDescription
OrderDescription::OrderDescription(OrderType type, Expr* expr, NullOrdering null_ordering)
    : type(type), expr(expr), null_ordering(null_ordering) {}

OrderDescription::~OrderDescription() { delete expr; }

// LimitDescription
LimitDescription::LimitDescription(Expr* limit, Expr* offset) : limit(limit), offset(offset) {}

LimitDescription::~LimitDescription() {
  delete limit;
  delete offset;
}

// GroypByDescription
GroupByDescription::GroupByDescription() : columns(nullptr), having(nullptr) {}

GroupByDescription::~GroupByDescription() {
  delete having;

  if (columns) {
    for (Expr* expr : *columns) {
      delete expr;
    }
    delete columns;
  }
}

WithDescription::~WithDescription() {
  free(alias);
  delete select;
}

// SelectStatement
SelectStatement::SelectStatement()
    : SQLStatement(kStmtSelect),
      fromTable(nullptr),
      selectDistinct(false),
      selectList(nullptr),
      whereClause(nullptr),
      groupBy(nullptr),
      setOperations(nullptr),
      order(nullptr),
      withDescriptions(nullptr),
      limit(nullptr),
      lockings(nullptr) {}

SelectStatement::~SelectStatement() {
  delete fromTable;
  delete whereClause;
  delete groupBy;
  delete limit;

  // Delete each element in the select list.
  if (selectList) {
    for (Expr* expr : *selectList) {
      delete expr;
    }
    delete selectList;
  }

  if (order) {
    for (OrderDescription* desc : *order) {
      delete desc;
    }
    delete order;
  }

  if (withDescriptions) {
    for (WithDescription* desc : *withDescriptions) {
      delete desc;
    }
    delete withDescriptions;
  }

  if (setOperations) {
    for (SetOperation* setOperation : *setOperations) {
      delete setOperation;
    }
    delete setOperations;
  }

  if (lockings) {
    for (LockingClause* lockingClause : *lockings) {
      if (lockingClause->tables) {
        for (char* dtable : *lockingClause->tables) {
          free(dtable);
        }
        delete lockingClause->tables;
      }
      delete lockingClause;
    }
    delete lockings;
  }
}

// UpdateStatement
UpdateStatement::UpdateStatement() : SQLStatement(kStmtUpdate), table(nullptr), updates(nullptr), where(nullptr) {}

UpdateStatement::~UpdateStatement() {
  delete table;
  delete where;

  if (updates) {
    for (UpdateClause* update : *updates) {
      free(update->column);
      delete update->value;
      delete update;
    }
    delete updates;
  }
}

// Alias
Alias::Alias(char* name, std::vector<char*>* columns) : name(name), columns(columns) {}

Alias::~Alias() {
  free(name);
  if (columns) {
    for (char* column : *columns) {
      free(column);
    }
    delete columns;
  }
}

// TableRef
TableRef::TableRef(TableRefType type)
    : type(type), schema(nullptr), name(nullptr), alias(nullptr), select(nullptr), list(nullptr), join(nullptr) {}

TableRef::~TableRef() {
  free(schema);
  free(name);

  delete select;
  delete join;
  delete alias;

  if (list) {
    for (TableRef* table : *list) {
      delete table;
    }
    delete list;
  }
}

bool TableRef::hasSchema() const { return schema != nullptr; }

const char* TableRef::getName() const {
  if (alias)
    return alias->name;
  else
    return name;
}

// JoinDefinition
JoinDefinition::JoinDefinition()
    : left(nullptr), right(nullptr), condition(nullptr), namedColumns(nullptr), type(kJoinInner) {}

JoinDefinition::~JoinDefinition() {
  delete left;
  delete right;
  delete condition;

  if (namedColumns) {
    for (auto* column : *namedColumns) {
      free(column);
    }
    delete namedColumns;
  }
}

SetOperation::SetOperation() : nestedSelectStatement(nullptr), resultOrder(nullptr), resultLimit(nullptr) {}

SetOperation::~SetOperation() {
  delete nestedSelectStatement;
  delete resultLimit;

  if (resultOrder) {
    for (OrderDescription* desc : *resultOrder) {
      delete desc;
    }
    delete resultOrder;
  }
}

}  // namespace hsql
