
#include "SQLParser.h"
#include <stdio.h>
#include <string>
#include "parser/bison_parser.h"
#include "parser/flex_lexer.h"

namespace hsql {

SQLParser::SQLParser() { fprintf(stderr, "SQLParser only has static methods atm! Do not initialize!\n"); }

// static
bool SQLParser::parse(const std::string& sql, SQLParserResult* result) {
  yyscan_t scanner;
  YY_BUFFER_STATE state;

  if (hsql_lex_init(&scanner)) {
    // Couldn't initialize the lexer.
    fprintf(stderr, "SQLParser: Error when initializing lexer!\n");
    return false;
  }
  const char* text = sql.c_str();
  state = hsql__scan_string(text, scanner);

  // Parse the tokens.
  // If parsing fails, the result will contain an error object.
  int ret = hsql_parse(result, scanner);
  bool success = (ret == 0);
  result->setIsValid(success);

  hsql__delete_buffer(state, scanner);
  hsql_lex_destroy(scanner);

  return true;
}

// static
bool SQLParser::parseSQLString(const char* sql, SQLParserResult* result) { return parse(sql, result); }

bool SQLParser::parseSQLString(const std::string& sql, SQLParserResult* result) { return parse(sql, result); }

// static
bool SQLParser::tokenize(const std::string& sql, std::vector<int16_t>* tokens) {
  // Initialize the scanner.
  yyscan_t scanner;
  if (hsql_lex_init(&scanner)) {
    fprintf(stderr, "SQLParser: Error when initializing lexer!\n");
    return false;
  }

  YY_BUFFER_STATE state;
  state = hsql__scan_string(sql.c_str(), scanner);

  YYSTYPE yylval;
  YYLTYPE yylloc;

  // Step through the string until EOF is read.
  // Note: hsql_lex returns int, but we know that its range is within 16 bit.
  int16_t token = hsql_lex(&yylval, &yylloc, scanner);
  while (token != 0) {
    tokens->push_back(token);
    token = hsql_lex(&yylval, &yylloc, scanner);

    if (token == SQL_IDENTIFIER || token == SQL_STRING) {
      free(yylval.sval);
    }
  }

  hsql__delete_buffer(state, scanner);
  hsql_lex_destroy(scanner);
  return true;
}

}  // namespace hsql
