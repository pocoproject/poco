
#include "SQLParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "parser/bison_parser.h"
#include "parser/flex_lexer.h"

namespace hsql {

SQLParser::SQLParser() { fprintf(stderr, "SQLParser only has static methods atm! Do not initialize!\n"); }

// static
bool SQLParser::parse(const std::string& sql, SQLParserResult* result) {
  yyscan_t scanner;
  YY_BUFFER_STATE state;

  // The lexer reads up to the first NUL character, so input with an embedded
  // NUL would be parsed only in part.
  const std::string::size_type nul = sql.find('\0');
  if (nul != std::string::npos) {
    static const char nulMessage[] = "embedded NUL character";
    char* copy = static_cast<char*>(malloc(sizeof(nulMessage)));
    if (copy != nullptr) memcpy(copy, nulMessage, sizeof(nulMessage));
    result->reset();
    result->setErrorDetails(copy, 0, static_cast<int>(nul));
    result->setIsValid(false);
    return true;
  }

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
  if (!success) {
    // A statement list is complete before the token after it is read, so a
    // failed parse can hold the statements before the error. They are dropped,
    // so that a part of the input does not pass for the whole; the error
    // details stay.
    const char* message = result->errorMsg();
    char* copy = nullptr;
    if (message != nullptr) {
      const size_t size = strlen(message) + 1;
      copy = static_cast<char*>(malloc(size));
      if (copy != nullptr) memcpy(copy, message, size);
    }
    const int errorLine = result->errorLine();
    const int errorColumn = result->errorColumn();
    result->reset();
    result->setErrorDetails(copy, errorLine, errorColumn);
  }
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
  // The lexer would end the input at an embedded NUL character.
  if (sql.find('\0') != std::string::npos) return false;

  // Initialize the scanner.
  yyscan_t scanner;
  if (hsql_lex_init(&scanner)) {
    fprintf(stderr, "SQLParser: Error when initializing lexer!\n");
    return false;
  }

  YY_BUFFER_STATE state;
  state = hsql__scan_string(sql.c_str(), scanner);

  YYSTYPE yylval;
  YYLTYPE yylloc{};

  // Step through the string until EOF is read. Each lex pass that returns
  // an sval-bearing token (SQL_IDENTIFIER, SQL_STRING, SQL_NAMED_PARAM)
  // allocates yylval.sval via strdup / hsql::substr; free it before the
  // next lex call overwrites yylval. The previous loop shape lexed twice
  // before freeing, which leaked the first sval-bearing token and missed
  // SQL_NAMED_PARAM entirely. The set mirrors bison's `%destructor
  // { free($$); } <sval>` for the same tokens.
  // Note: hsql_lex returns int, but we know its range is within 16 bit.
  while (true) {
    int16_t token = hsql_lex(&yylval, &yylloc, scanner);
    if (token == 0) break;
    tokens->push_back(token);
    if (token == SQL_IDENTIFIER || token == SQL_STRING || token == SQL_NAMED_PARAM) {
      free(yylval.sval);
    }
  }

  hsql__delete_buffer(state, scanner);
  hsql_lex_destroy(scanner);
  return true;
}

}  // namespace hsql
