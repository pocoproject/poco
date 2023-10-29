#ifndef SQLPARSER_IMPORT_STATEMENT_H
#define SQLPARSER_IMPORT_STATEMENT_H

#include "SQLStatement.h"

namespace hsql {
enum ImportType {
  kImportCSV,
  kImportTbl,  // Hyrise file format
  kImportBinary,
  kImportAuto
};

// Represents SQL Import statements.
struct SQLParser_API ImportStatement : SQLStatement {
  ImportStatement(ImportType type);
  ~ImportStatement() override;

  ImportType type;
  char* filePath;
  char* schema;
  char* tableName;
  Expr* whereClause;
};

}  // namespace hsql

#endif
