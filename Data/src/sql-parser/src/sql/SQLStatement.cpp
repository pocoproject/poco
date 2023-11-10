
#include "SQLStatement.h"

namespace hsql {

// SQLStatement
SQLStatement::SQLStatement(StatementType type) : hints(nullptr), type_(type) {}

SQLStatement::~SQLStatement() {
  if (hints) {
    for (Expr* hint : *hints) {
      delete hint;
    }
  }
  delete hints;
}

StatementType SQLStatement::type() const { return type_; }

bool SQLStatement::isType(StatementType type) const { return (type_ == type); }

bool SQLStatement::is(StatementType type) const { return isType(type); }

}  // namespace hsql
