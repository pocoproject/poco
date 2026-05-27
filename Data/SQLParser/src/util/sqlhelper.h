#ifndef SQLPARSER_SQLHELPER_H
#define SQLPARSER_SQLHELPER_H

#include "../sqlparser_win.h"
#include "../sql/statements.h"

namespace hsql {

// Prints a summary of the given SQLStatement.
void SQLParser_API printStatementInfo(const SQLStatement* stmt);

// Prints a summary of the given SelectStatement with the given indentation.
void SQLParser_API printSelectStatementInfo(const SelectStatement* stmt, uintmax_t num_indent);

// Prints a summary of the given ImportStatement with the given indentation.
void SQLParser_API printImportStatementInfo(const ImportStatement* stmt, uintmax_t num_indent);

// Prints a summary of the given CopyStatement with the given indentation.
void SQLParser_API printExportStatementInfo(const ExportStatement* stmt, uintmax_t num_indent);

// Prints a summary of the given InsertStatement with the given indentation.
void SQLParser_API printInsertStatementInfo(const InsertStatement* stmt, uintmax_t num_indent);

// Prints a summary of the given CreateStatement with the given indentation.
void SQLParser_API printCreateStatementInfo(const CreateStatement* stmt, uintmax_t num_indent);

// Prints a summary of the given TransactionStatement with the given indentation.
void SQLParser_API printTransactionStatementInfo(const TransactionStatement* stmt, uintmax_t num_indent);

// Prints a summary of the given Expression with the given indentation.
void SQLParser_API printExpression(Expr* expr, uintmax_t num_indent);

// Prints an ORDER BY clause
void SQLParser_API printOrderBy(const std::vector<OrderDescription*>* expr, uintmax_t num_indent);

// Prints WindowDescription.
void SQLParser_API printWindowDescription(WindowDescription* window_description, uintmax_t num_indent);

}  // namespace hsql

#endif
