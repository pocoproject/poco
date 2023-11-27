#include "Expr.h"
#include <stdio.h>
#include <string.h>
#include "SelectStatement.h"

namespace hsql {

FrameBound::FrameBound(int64_t offset, FrameBoundType type, bool unbounded)
    : offset{offset}, type{type}, unbounded{unbounded} {}

FrameDescription::FrameDescription(FrameType type, FrameBound* start, FrameBound* end)
    : type{type}, start{start}, end{end} {}

FrameDescription::~FrameDescription() {
  delete start;
  delete end;
}

WindowDescription::WindowDescription(std::vector<Expr*>* partitionList, std::vector<OrderDescription*>* orderList,
                                     FrameDescription* frameDescription)
    : partitionList{partitionList}, orderList{orderList}, frameDescription{frameDescription} {}

WindowDescription::~WindowDescription() {
  if (partitionList) {
    for (Expr* e : *partitionList) {
      delete e;
    }
    delete partitionList;
  }

  if (orderList) {
    for (OrderDescription* orderDescription : *orderList) {
      delete orderDescription;
    }
    delete orderList;
  }

  delete frameDescription;
}

Expr::Expr(ExprType type)
    : type(type),
      expr(nullptr),
      expr2(nullptr),
      exprList(nullptr),
      select(nullptr),
      name(nullptr),
      table(nullptr),
      alias(nullptr),
      fval(0),
      ival(0),
      ival2(0),
      datetimeField(kDatetimeNone),
      columnType(DataType::UNKNOWN, 0),
      isBoolLiteral(false),
      opType(kOpNone),
      distinct(false),
      windowDescription(nullptr) {}

Expr::~Expr() {
  delete expr;
  delete expr2;
  delete select;
  delete windowDescription;

  free(name);
  free(table);
  free(alias);

  if (exprList) {
    for (Expr* e : *exprList) {
      delete e;
    }
    delete exprList;
  }
}

Expr* Expr::make(ExprType type) {
  Expr* e = new Expr(type);
  return e;
}

Expr* Expr::makeOpUnary(OperatorType op, Expr* expr) {
  Expr* e = new Expr(kExprOperator);
  e->opType = op;
  e->expr = expr;
  e->expr2 = nullptr;
  return e;
}

Expr* Expr::makeOpBinary(Expr* expr1, OperatorType op, Expr* expr2) {
  Expr* e = new Expr(kExprOperator);
  e->opType = op;
  e->expr = expr1;
  e->expr2 = expr2;
  return e;
}

Expr* Expr::makeBetween(Expr* expr, Expr* left, Expr* right) {
  Expr* e = new Expr(kExprOperator);
  e->expr = expr;
  e->opType = kOpBetween;
  e->exprList = new std::vector<Expr*>();
  e->exprList->push_back(left);
  e->exprList->push_back(right);
  return e;
}

Expr* Expr::makeCaseList(Expr* caseListElement) {
  Expr* e = new Expr(kExprOperator);
  // Case list expressions are temporary and will be integrated into the case
  // expressions exprList - thus assign operator type kOpNone
  e->opType = kOpNone;
  e->exprList = new std::vector<Expr*>();
  e->exprList->push_back(caseListElement);
  return e;
}

Expr* Expr::makeCaseListElement(Expr* when, Expr* then) {
  Expr* e = new Expr(kExprOperator);
  e->opType = kOpCaseListElement;
  e->expr = when;
  e->expr2 = then;
  return e;
}

Expr* Expr::caseListAppend(Expr* caseList, Expr* caseListElement) {
  caseList->exprList->push_back(caseListElement);
  return caseList;
}

Expr* Expr::makeCase(Expr* expr, Expr* caseList, Expr* elseExpr) {
  Expr* e = new Expr(kExprOperator);
  e->opType = kOpCase;
  e->expr = expr;
  e->expr2 = elseExpr;
  e->exprList = caseList->exprList;
  caseList->exprList = nullptr;
  delete caseList;
  return e;
}

Expr* Expr::makeLiteral(int64_t val) {
  Expr* e = new Expr(kExprLiteralInt);
  e->ival = val;
  return e;
}

Expr* Expr::makeLiteral(double value) {
  Expr* e = new Expr(kExprLiteralFloat);
  e->fval = value;
  return e;
}

Expr* Expr::makeLiteral(char* string) {
  Expr* e = new Expr(kExprLiteralString);
  e->name = string;
  return e;
}

Expr* Expr::makeLiteral(bool val) {
  Expr* e = new Expr(kExprLiteralInt);
  e->ival = (int)val;
  e->isBoolLiteral = true;
  return e;
}

Expr* Expr::makeNullLiteral() {
  Expr* e = new Expr(kExprLiteralNull);
  return e;
}

Expr* Expr::makeDateLiteral(char* string) {
  Expr* e = new Expr(kExprLiteralDate);
  e->name = string;
  return e;
}

Expr* Expr::makeIntervalLiteral(int64_t duration, DatetimeField unit) {
  Expr* e = new Expr(kExprLiteralInterval);
  e->ival = duration;
  e->datetimeField = unit;
  return e;
}

Expr* Expr::makeColumnRef(char* name) {
  Expr* e = new Expr(kExprColumnRef);
  e->name = name;
  return e;
}

Expr* Expr::makeColumnRef(char* table, char* name) {
  Expr* e = new Expr(kExprColumnRef);
  e->name = name;
  e->table = table;
  return e;
}

Expr* Expr::makeStar(void) {
  Expr* e = new Expr(kExprStar);
  return e;
}

Expr* Expr::makeStar(char* table) {
  Expr* e = new Expr(kExprStar);
  e->table = table;
  return e;
}

Expr* Expr::makeFunctionRef(char* func_name, std::vector<Expr*>* exprList, bool distinct, WindowDescription* window) {
  Expr* e = new Expr(kExprFunctionRef);
  e->name = func_name;
  e->exprList = exprList;
  e->distinct = distinct;
  e->windowDescription = window;
  return e;
}

Expr* Expr::makeArray(std::vector<Expr*>* exprList) {
  Expr* e = new Expr(kExprArray);
  e->exprList = exprList;
  return e;
}

Expr* Expr::makeArrayIndex(Expr* expr, int64_t index) {
  Expr* e = new Expr(kExprArrayIndex);
  e->expr = expr;
  e->ival = index;
  return e;
}

Expr* Expr::makeParameter(int id) {
  Expr* e = new Expr(kExprParameter);
  e->ival = id;
  return e;
}

Expr* Expr::makeSelect(SelectStatement* select) {
  Expr* e = new Expr(kExprSelect);
  e->select = select;
  return e;
}

Expr* Expr::makeExists(SelectStatement* select) {
  Expr* e = new Expr(kExprOperator);
  e->opType = kOpExists;
  e->select = select;
  return e;
}

Expr* Expr::makeInOperator(Expr* expr, std::vector<Expr*>* exprList) {
  Expr* e = new Expr(kExprOperator);
  e->opType = kOpIn;
  e->expr = expr;
  e->exprList = exprList;

  return e;
}

Expr* Expr::makeInOperator(Expr* expr, SelectStatement* select) {
  Expr* e = new Expr(kExprOperator);
  e->opType = kOpIn;
  e->expr = expr;
  e->select = select;

  return e;
}

Expr* Expr::makeExtract(DatetimeField datetimeField, Expr* expr) {
  Expr* e = new Expr(kExprExtract);
  e->datetimeField = datetimeField;
  e->expr = expr;
  return e;
}

Expr* Expr::makeCast(Expr* expr, ColumnType columnType) {
  Expr* e = new Expr(kExprCast);
  e->columnType = columnType;
  e->expr = expr;
  return e;
}

bool Expr::isType(ExprType exprType) const { return exprType == type; }

bool Expr::isLiteral() const {
  return isType(kExprLiteralInt) || isType(kExprLiteralFloat) || isType(kExprLiteralString) || isType(kExprParameter) ||
         isType(kExprLiteralNull) || isType(kExprLiteralDate) || isType(kExprLiteralInterval);
}

bool Expr::hasAlias() const { return alias != nullptr; }

bool Expr::hasTable() const { return table != nullptr; }

const char* Expr::getName() const {
  if (alias)
    return alias;
  else
    return name;
}

char* substr(const char* source, int from, int to) {
  int len = to - from;
  char* copy = (char*)malloc(len + 1);
  ;
#if defined(_WIN32) || defined(_WIN64)
#pragma warning(disable : 4996)
#endif
  strncpy(copy, source + from, len);
  copy[len] = '\0';
  return copy;
#if defined(_WIN32) || defined(_WIN64)
#pragma warning(default : 4996)
#endif
}
}  // namespace hsql
