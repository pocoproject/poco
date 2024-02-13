#ifndef SQLPARSER_SQLPARSER_H
#define SQLPARSER_SQLPARSER_H

#include "SQLParserResult.h"
#include "sql/statements.h"

namespace hsql {

// Static methods used to parse SQL strings.
class SQLParser_API SQLParser {
 public:
  // Parses a given constant character SQL string into the result object.
  // Returns true if the lexer and parser could run without internal errors.
  // This does NOT mean that the SQL string was valid SQL. To check that
  // you need to check result->isValid();
  static bool parse(const std::string& sql, SQLParserResult* result);

  // Run tokenization on the given string and store the tokens in the output vector.
  static bool tokenize(const std::string& sql, std::vector<int16_t>* tokens);

  // Deprecated.
  // Old method to parse SQL strings. Replaced by parse().
  static bool parseSQLString(const char* sql, SQLParserResult* result);

  // Deprecated.
  // Old method to parse SQL strings. Replaced by parse().
  static bool parseSQLString(const std::string& sql, SQLParserResult* result);

 private:
  SQLParser();
};

}  // namespace hsql

#endif