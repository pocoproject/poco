#ifndef SQLPARSER_EXPORT_STATEMENT_H
#define SQLPARSER_EXPORT_STATEMENT_H

#include "ImportStatement.h"
#include "SQLStatement.h"
#include "SelectStatement.h"

namespace hsql {
// Represents SQL Export statements.
struct SQLParser_API ExportStatement : SQLStatement {
  ExportStatement(ImportType type);
  ~ExportStatement() override;

  // ImportType is used for compatibility reasons
  ImportType type;
  char* filePath;
  char* schema;
  char* tableName;
  SelectStatement* select;
};

}  // namespace hsql

#endif
