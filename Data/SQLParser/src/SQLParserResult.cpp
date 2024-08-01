
#include "SQLParserResult.h"
#include <algorithm>
#include <string.h>

namespace hsql {

SQLParserResult::SQLParserResult() :
  statements_(new std::vector<SQLStatement*>),
  isValid_(false),
  errorMsg_(nullptr),
  errorLine_(-1),
  errorColumn_(-1),
  parameters_(new std::vector<Expr*>)
{
}

SQLParserResult::SQLParserResult(SQLStatement* stmt) :
  statements_(new std::vector<SQLStatement*>),
  isValid_(false),
  errorMsg_(nullptr),
  errorLine_(-1),
  errorColumn_(-1),
  parameters_(new std::vector<Expr*>)
{
  addStatement(stmt);
}

// Move constructor.
SQLParserResult::SQLParserResult(SQLParserResult&& moved) { *this = std::forward<SQLParserResult>(moved); }

SQLParserResult& SQLParserResult::operator=(SQLParserResult&& moved) {
  isValid_ = moved.isValid_;
  errorMsg_ = moved.errorMsg_;
  statements_ = moved.statements_;
  parameters_ = moved.parameters_;

  moved.reset(true);
  return *this;
}

SQLParserResult::~SQLParserResult() { reset(); }

void SQLParserResult::addStatement(SQLStatement* stmt) {
  if (!statements_) statements_ = new std::vector<SQLStatement*>;
  statements_->push_back(stmt);
}

const SQLStatement* SQLParserResult::getStatement(size_t index) const {
  return statements_ ? (*statements_)[index] : nullptr;
}

SQLStatement* SQLParserResult::getMutableStatement(size_t index) {
  return statements_ ? (*statements_)[index] : nullptr;
}

size_t SQLParserResult::size() const { return statements_ ? statements_->size() : 0u; }

bool SQLParserResult::isValid() const { return isValid_; }

const char* SQLParserResult::errorMsg() const { return errorMsg_; }

int SQLParserResult::errorLine() const { return errorLine_; }

int SQLParserResult::errorColumn() const { return errorColumn_; }

void SQLParserResult::setIsValid(bool isValid) { isValid_ = isValid; }

void SQLParserResult::setErrorDetails(char* errorMsg, int errorLine, int errorColumn) {
  if (errorMsg_) free(errorMsg);
  errorMsg_ = errorMsg;
  errorLine_ = errorLine;
  errorColumn_ = errorColumn;
}

const std::vector<SQLStatement*>& SQLParserResult::getStatements() const {
  if (!statements_) statements_ = new std::vector<SQLStatement*>;
  return *statements_;
}

std::vector<SQLStatement*> SQLParserResult::releaseStatements() {
  std::vector<SQLStatement*> copy;
  if (statements_)
  {
    copy = *statements_;
    statements_->clear();
  }
  return copy;
}

void SQLParserResult::reset(bool mv) {
  if (statements_)
  {
    if (!mv)
    {
      for (SQLStatement* statement : *statements_) {
        delete statement;
      }
      delete statements_;
    }
    statements_ = nullptr;
  }

  if (parameters_)
  {
    if (!mv) delete parameters_;
    parameters_ = nullptr;
  }

  isValid_ = false;

  if (errorMsg_)
  {
    if (!mv) free(errorMsg_);
    errorMsg_ = nullptr;
  }
  errorLine_ = -1;
  errorColumn_ = -1;
}

// Does NOT take ownership.
void SQLParserResult::addParameter(Expr* parameter) {
  if (!parameters_) parameters_ = new std::vector<Expr*>;
  parameters_->push_back(parameter);
  std::sort(parameters_->begin(), parameters_->end(), [](const Expr* a, const Expr* b) { return a->ival < b->ival; });
}

const std::vector<Expr*>& SQLParserResult::parameters() {
  if (!parameters_) parameters_ = new std::vector<Expr*>;
  return *parameters_;
}

}  // namespace hsql
