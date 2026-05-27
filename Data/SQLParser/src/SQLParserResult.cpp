
#include "SQLParserResult.h"
#include <algorithm>

namespace hsql {

SQLParserResult::SQLParserResult() : isValid_(false), errorMsg_(nullptr) {}

SQLParserResult::SQLParserResult(SQLStatement* stmt) : isValid_(false), errorMsg_(nullptr) { addStatement(stmt); }

// Move constructor.
SQLParserResult::SQLParserResult(SQLParserResult&& moved) { *this = std::forward<SQLParserResult>(moved); }

SQLParserResult& SQLParserResult::operator=(SQLParserResult&& moved) {
  isValid_ = moved.isValid_;
  errorMsg_ = moved.errorMsg_;
  statements_ = std::move(moved.statements_);

  moved.errorMsg_ = nullptr;
  moved.reset();
  return *this;
}

SQLParserResult::~SQLParserResult() { reset(); }

void SQLParserResult::addStatement(SQLStatement* stmt) { statements_.push_back(stmt); }

const SQLStatement* SQLParserResult::getStatement(size_t index) const { return statements_[index]; }

SQLStatement* SQLParserResult::getMutableStatement(size_t index) { return statements_[index]; }

size_t SQLParserResult::size() const { return statements_.size(); }

bool SQLParserResult::isValid() const { return isValid_; }

const char* SQLParserResult::errorMsg() const { return errorMsg_; }

int SQLParserResult::errorLine() const { return errorLine_; }

int SQLParserResult::errorColumn() const { return errorColumn_; }

void SQLParserResult::setIsValid(bool isValid) { isValid_ = isValid; }

void SQLParserResult::setErrorDetails(char* errorMsg, int errorLine, int errorColumn) {
  errorMsg_ = errorMsg;
  errorLine_ = errorLine;
  errorColumn_ = errorColumn;
}

const std::vector<SQLStatement*>& SQLParserResult::getStatements() const { return statements_; }

std::vector<SQLStatement*> SQLParserResult::releaseStatements() {
  std::vector<SQLStatement*> copy = statements_;

  statements_.clear();

  return copy;
}

void SQLParserResult::reset() {
  for (SQLStatement* statement : statements_) {
    delete statement;
  }
  statements_.clear();

  isValid_ = false;

  free(errorMsg_);
  errorMsg_ = nullptr;
  errorLine_ = -1;
  errorColumn_ = -1;
}

// Does NOT take ownership.
void SQLParserResult::addParameter(Expr* parameter) {
  parameters_.push_back(parameter);
  std::sort(parameters_.begin(), parameters_.end(), [](const Expr* a, const Expr* b) { return a->ival < b->ival; });
}

const std::vector<Expr*>& SQLParserResult::parameters() { return parameters_; }

}  // namespace hsql
