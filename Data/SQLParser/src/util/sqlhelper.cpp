
#include "sqlhelper.h"
#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace hsql {

void printOperatorExpression(Expr* expr, uintmax_t num_indent);
void printAlias(Alias* alias, uintmax_t num_indent);

std::ostream& operator<<(std::ostream& os, const OperatorType& op);
std::ostream& operator<<(std::ostream& os, const DatetimeField& datetime);
std::ostream& operator<<(std::ostream& os, const FrameBound& frame_bound);

std::string indent(uintmax_t num_indent) { return std::string(num_indent, '\t'); }
void inprint(int64_t val, uintmax_t num_indent) { std::cout << indent(num_indent).c_str() << val << "  " << std::endl; }
void inprint(double val, uintmax_t num_indent) { std::cout << indent(num_indent).c_str() << val << std::endl; }
void inprint(const char* val, uintmax_t num_indent) { std::cout << indent(num_indent).c_str() << val << std::endl; }
void inprint(const char* val, const char* val2, uintmax_t num_indent) {
  std::cout << indent(num_indent).c_str() << val << "->" << val2 << std::endl;
}
void inprintC(char val, uintmax_t num_indent) { std::cout << indent(num_indent).c_str() << val << std::endl; }
void inprint(const OperatorType& op, uintmax_t num_indent) { std::cout << indent(num_indent) << op << std::endl; }
void inprint(const ColumnType& colType, uintmax_t num_indent) {
  std::cout << indent(num_indent) << colType << std::endl;
}
void inprint(const DatetimeField& colType, uintmax_t num_indent) {
  std::cout << indent(num_indent) << colType << std::endl;
}

void printTableRefInfo(TableRef* table, uintmax_t num_indent) {
  switch (table->type) {
    case kTableName:
      inprint(table->name, num_indent);
      if (table->schema) {
        inprint("Schema", num_indent + 1);
        inprint(table->schema, num_indent + 2);
      }
      break;
    case kTableSelect:
      printSelectStatementInfo(table->select, num_indent);
      break;
    case kTableJoin:
      inprint("Join Table", num_indent);
      inprint("Left", num_indent + 1);
      printTableRefInfo(table->join->left, num_indent + 2);
      inprint("Right", num_indent + 1);
      printTableRefInfo(table->join->right, num_indent + 2);
      inprint("Join Condition", num_indent + 1);
      printExpression(table->join->condition, num_indent + 2);
      break;
    case kTableCrossProduct:
      for (TableRef* tbl : *table->list) printTableRefInfo(tbl, num_indent);
      break;
  }

  if (table->alias) {
    printAlias(table->alias, num_indent);
  }
}

void printAlias(Alias* alias, uintmax_t num_indent) {
  inprint("Alias", num_indent + 1);
  inprint(alias->name, num_indent + 2);

  if (alias->columns) {
    for (char* column : *(alias->columns)) {
      inprint(column, num_indent + 3);
    }
  }
}

void printOperatorExpression(Expr* expr, uintmax_t num_indent) {
  if (expr == nullptr) {
    inprint("null", num_indent);
    return;
  }

  inprint(expr->opType, num_indent);

  printExpression(expr->expr, num_indent + 1);
  if (expr->expr2) {
    printExpression(expr->expr2, num_indent + 1);
  } else if (expr->exprList) {
    for (Expr* e : *expr->exprList) printExpression(e, num_indent + 1);
  }
}

void printExpression(Expr* expr, uintmax_t num_indent) {
  if (!expr) return;
  switch (expr->type) {
    case kExprStar:
      inprint("*", num_indent);
      break;
    case kExprColumnRef:
      inprint(expr->name, num_indent);
      if (expr->table) {
        inprint("Table:", num_indent + 1);
        inprint(expr->table, num_indent + 2);
      }
      break;
    // case kExprTableColumnRef: inprint(expr->table, expr->name, num_indent); break;
    case kExprLiteralFloat:
      inprint(expr->fval, num_indent);
      break;
    case kExprLiteralInt:
      inprint(expr->ival, num_indent);
      break;
    case kExprLiteralString:
      inprint(expr->name, num_indent);
      break;
    case kExprLiteralDate:
      inprint(expr->name, num_indent);
      break;
    case kExprLiteralNull:
      inprint("NULL", num_indent);
      break;
    case kExprLiteralInterval:
      inprint("INTERVAL", num_indent);
      inprint(expr->ival, num_indent + 1);
      inprint(expr->datetimeField, num_indent + 1);
      break;
    case kExprFunctionRef:
      inprint(expr->name, num_indent);
      for (Expr* e : *expr->exprList) {
        printExpression(e, num_indent + 1);
      }

      if (expr->windowDescription) {
        printWindowDescription(expr->windowDescription, num_indent + 1);
      }
      break;
    case kExprExtract:
      inprint("EXTRACT", num_indent);
      inprint(expr->datetimeField, num_indent + 1);
      printExpression(expr->expr, num_indent + 1);
      break;
    case kExprCast:
      inprint("CAST", num_indent);
      inprint(expr->columnType, num_indent + 1);
      printExpression(expr->expr, num_indent + 1);
      break;
    case kExprOperator:
      printOperatorExpression(expr, num_indent);
      break;
    case kExprSelect:
      printSelectStatementInfo(expr->select, num_indent);
      break;
    case kExprParameter:
      inprint(expr->ival, num_indent);
      break;
    case kExprArray:
      for (Expr* e : *expr->exprList) {
        printExpression(e, num_indent + 1);
      }
      break;
    case kExprArrayIndex:
      printExpression(expr->expr, num_indent + 1);
      inprint(expr->ival, num_indent);
      break;
    default:
      std::cerr << "Unrecognized expression type " << expr->type << std::endl;
      return;
  }
  if (expr->alias) {
    inprint("Alias", num_indent + 1);
    inprint(expr->alias, num_indent + 2);
  }
}

void printOrderBy(const std::vector<OrderDescription*>* expr, uintmax_t num_indent) {
  if (!expr) return;
  for (const auto& order_description : *expr) {
    printExpression(order_description->expr, num_indent);
    if (order_description->type == kOrderAsc) {
      inprint("ascending", num_indent);
    } else {
      inprint("descending", num_indent);
    }
  }
}

void printWindowDescription(WindowDescription* window_description, uintmax_t num_indent) {
  inprint("OVER", num_indent);
  if (window_description->partitionList) {
    inprint("PARTITION BY", num_indent + 1);
    for (const auto e : *window_description->partitionList) {
      printExpression(e, num_indent + 2);
    }
  }

  if (window_description->orderList) {
    inprint("ORDER BY", num_indent + 1);
    printOrderBy(window_description->orderList, num_indent + 2);
  }

  std::stringstream stream;
  switch (window_description->frameDescription->type) {
    case kRows:
      stream << "ROWS";
      break;
    case kRange:
      stream << "RANGE";
      break;
    case kGroups:
      stream << "GROUPS";
      break;
  }
  stream << " BETWEEN " << *window_description->frameDescription->start << " AND "
         << *window_description->frameDescription->end;
  inprint(stream.str().c_str(), num_indent + 1);
}

void printSelectStatementInfo(const SelectStatement* stmt, uintmax_t num_indent) {
  inprint("SelectStatement", num_indent);
  inprint("Fields:", num_indent + 1);
  for (Expr* expr : *stmt->selectList) printExpression(expr, num_indent + 2);

  if (stmt->fromTable) {
    inprint("Sources:", num_indent + 1);
    printTableRefInfo(stmt->fromTable, num_indent + 2);
  }

  if (stmt->whereClause) {
    inprint("Search Conditions:", num_indent + 1);
    printExpression(stmt->whereClause, num_indent + 2);
  }

  if (stmt->groupBy) {
    inprint("GroupBy:", num_indent + 1);
    for (Expr* expr : *stmt->groupBy->columns) printExpression(expr, num_indent + 2);
    if (stmt->groupBy->having) {
      inprint("Having:", num_indent + 1);
      printExpression(stmt->groupBy->having, num_indent + 2);
    }
  }
  if (stmt->lockings) {
    inprint("Lock Info:", num_indent + 1);
    for (LockingClause* lockingClause : *stmt->lockings) {
      inprint("Type", num_indent + 2);
      if (lockingClause->rowLockMode == RowLockMode::ForUpdate) {
        inprint("FOR UPDATE", num_indent + 3);
      } else if (lockingClause->rowLockMode == RowLockMode::ForNoKeyUpdate) {
        inprint("FOR NO KEY UPDATE", num_indent + 3);
      } else if (lockingClause->rowLockMode == RowLockMode::ForShare) {
        inprint("FOR SHARE", num_indent + 3);
      } else if (lockingClause->rowLockMode == RowLockMode::ForKeyShare) {
        inprint("FOR KEY SHARE", num_indent + 3);
      }
      if (lockingClause->tables) {
        inprint("Target tables:", num_indent + 2);
        for (char* dtable : *lockingClause->tables) {
          inprint(dtable, num_indent + 3);
        }
      }
      if (lockingClause->rowLockWaitPolicy != RowLockWaitPolicy::None) {
        inprint("Waiting policy: ", num_indent + 2);
        if (lockingClause->rowLockWaitPolicy == RowLockWaitPolicy::NoWait)
          inprint("NOWAIT", num_indent + 3);
        else
          inprint("SKIP LOCKED", num_indent + 3);
      }
    }
  }

  if (stmt->setOperations) {
    for (SetOperation* setOperation : *stmt->setOperations) {
      switch (setOperation->setType) {
        case SetType::kSetIntersect:
          inprint("Intersect:", num_indent + 1);
          break;
        case SetType::kSetUnion:
          inprint("Union:", num_indent + 1);
          break;
        case SetType::kSetExcept:
          inprint("Except:", num_indent + 1);
          break;
      }

      printSelectStatementInfo(setOperation->nestedSelectStatement, num_indent + 2);

      if (setOperation->resultOrder) {
        inprint("SetResultOrderBy:", num_indent + 1);
        printOrderBy(setOperation->resultOrder, num_indent + 2);
      }

      if (setOperation->resultLimit) {
        if (setOperation->resultLimit->limit) {
          inprint("SetResultLimit:", num_indent + 1);
          printExpression(setOperation->resultLimit->limit, num_indent + 2);
        }

        if (setOperation->resultLimit->offset) {
          inprint("SetResultOffset:", num_indent + 1);
          printExpression(setOperation->resultLimit->offset, num_indent + 2);
        }
      }
    }
  }

  if (stmt->order) {
    inprint("OrderBy:", num_indent + 1);
    printOrderBy(stmt->order, num_indent + 2);
  }

  if (stmt->limit && stmt->limit->limit) {
    inprint("Limit:", num_indent + 1);
    printExpression(stmt->limit->limit, num_indent + 2);
  }

  if (stmt->limit && stmt->limit->offset) {
    inprint("Offset:", num_indent + 1);
    printExpression(stmt->limit->offset, num_indent + 2);
  }
}

void printImportStatementInfo(const ImportStatement* stmt, uintmax_t num_indent) {
  inprint("ImportStatement", num_indent);
  inprint(stmt->filePath, num_indent + 1);
  switch (stmt->type) {
    case ImportType::kImportCSV:
      inprint("CSV", num_indent + 1);
      break;
    case ImportType::kImportTbl:
      inprint("TBL", num_indent + 1);
      break;
    case ImportType::kImportBinary:
      inprint("BINARY", num_indent + 1);
      break;
    case ImportType::kImportAuto:
      inprint("AUTO", num_indent + 1);
      break;
  }
  inprint(stmt->tableName, num_indent + 1);
  if (stmt->whereClause) {
    inprint("WHERE:", num_indent + 1);
    printExpression(stmt->whereClause, num_indent + 2);
  }
}

void printExportStatementInfo(const ExportStatement* stmt, uintmax_t num_indent) {
  inprint("ExportStatement", num_indent);
  inprint(stmt->filePath, num_indent + 1);
  switch (stmt->type) {
    case ImportType::kImportCSV:
      inprint("CSV", num_indent + 1);
      break;
    case ImportType::kImportTbl:
      inprint("TBL", num_indent + 1);
      break;
    case ImportType::kImportBinary:
      inprint("BINARY", num_indent + 1);
      break;
    case ImportType::kImportAuto:
      inprint("AUTO", num_indent + 1);
      break;
  }

  if (stmt->tableName) {
    inprint(stmt->tableName, num_indent + 1);
  } else {
    printSelectStatementInfo(stmt->select, num_indent + 1);
  }
}

void printCreateStatementInfo(const CreateStatement* stmt, uintmax_t num_indent) {
  inprint("CreateStatement", num_indent);
  inprint(stmt->tableName, num_indent + 1);
  if (stmt->filePath) inprint(stmt->filePath, num_indent + 1);
}

void printInsertStatementInfo(const InsertStatement* stmt, uintmax_t num_indent) {
  inprint("InsertStatement", num_indent);
  inprint(stmt->tableName, num_indent + 1);
  if (stmt->columns) {
    inprint("Columns", num_indent + 1);
    for (char* col_name : *stmt->columns) {
      inprint(col_name, num_indent + 2);
    }
  }
  switch (stmt->type) {
    case kInsertValues:
      inprint("Values", num_indent + 1);
      for (Expr* expr : *stmt->values) {
        printExpression(expr, num_indent + 2);
      }
      break;
    case kInsertSelect:
      printSelectStatementInfo(stmt->select, num_indent + 1);
      break;
  }
}

void printTransactionStatementInfo(const TransactionStatement* stmt, uintmax_t num_indent) {
  inprint("TransactionStatement", num_indent);
  switch (stmt->command) {
    case kBeginTransaction:
      inprint("BEGIN", num_indent + 1);
      break;
    case kCommitTransaction:
      inprint("COMMIT", num_indent + 1);
      break;
    case kRollbackTransaction:
      inprint("ROLLBACK", num_indent + 1);
      break;
  }
}

void printStatementInfo(const SQLStatement* stmt) {
  switch (stmt->type()) {
    case kStmtSelect:
      printSelectStatementInfo((const SelectStatement*)stmt, 0);
      break;
    case kStmtInsert:
      printInsertStatementInfo((const InsertStatement*)stmt, 0);
      break;
    case kStmtCreate:
      printCreateStatementInfo((const CreateStatement*)stmt, 0);
      break;
    case kStmtImport:
      printImportStatementInfo((const ImportStatement*)stmt, 0);
      break;
    case kStmtExport:
      printExportStatementInfo((const ExportStatement*)stmt, 0);
      break;
    case kStmtTransaction:
      printTransactionStatementInfo((const TransactionStatement*)stmt, 0);
      break;
    default:
      break;
  }
}

std::ostream& operator<<(std::ostream& os, const OperatorType& op) {
  static const std::map<const OperatorType, const std::string> operatorToToken = {
      {kOpNone, "None"},     {kOpBetween, "BETWEEN"},
      {kOpCase, "CASE"},     {kOpCaseListElement, "CASE LIST ELEMENT"},
      {kOpPlus, "+"},        {kOpMinus, "-"},
      {kOpAsterisk, "*"},    {kOpSlash, "/"},
      {kOpPercentage, "%"},  {kOpCaret, "^"},
      {kOpEquals, "="},      {kOpNotEquals, "!="},
      {kOpLess, "<"},        {kOpLessEq, "<="},
      {kOpGreater, ">"},     {kOpGreaterEq, ">="},
      {kOpLike, "LIKE"},     {kOpNotLike, "NOT LIKE"},
      {kOpILike, "ILIKE"},   {kOpAnd, "AND"},
      {kOpOr, "OR"},         {kOpIn, "IN"},
      {kOpConcat, "CONCAT"}, {kOpNot, "NOT"},
      {kOpUnaryMinus, "-"},  {kOpIsNull, "IS NULL"},
      {kOpExists, "EXISTS"}};

  const auto found = operatorToToken.find(op);
  if (found == operatorToToken.cend()) {
    return os << static_cast<uint64_t>(op);
  } else {
    return os << (*found).second;
  }
}

std::ostream& operator<<(std::ostream& os, const DatetimeField& datetime) {
  static const std::map<const DatetimeField, const std::string> operatorToToken = {
      {kDatetimeNone, "None"}, {kDatetimeSecond, "SECOND"}, {kDatetimeMinute, "MINUTE"}, {kDatetimeHour, "HOUR"},
      {kDatetimeDay, "DAY"},   {kDatetimeMonth, "MONTH"},   {kDatetimeYear, "YEAR"}};

  const auto found = operatorToToken.find(datetime);
  if (found == operatorToToken.cend()) {
    return os << static_cast<uint64_t>(datetime);
  } else {
    return os << (*found).second;
  }
}

std::ostream& operator<<(std::ostream& os, const FrameBound& frame_bound) {
  if (frame_bound.type == kCurrentRow) {
    os << "CURRENT ROW";
    return os;
  }

  if (frame_bound.unbounded) {
    os << "UNBOUNDED";
  } else {
    os << frame_bound.offset;
  }

  os << " ";

  if (frame_bound.type == kPreceding) {
    os << "PRECEDING";
  } else {
    os << "FOLLOWING";
  }

  return os;
}

}  // namespace hsql
