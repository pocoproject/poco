#ifndef __PARSER_TYPEDEF_H__
#define __PARSER_TYPEDEF_H__

#include <vector>

#ifndef YYtypeDEF_YY_SCANNER_T
#define YYtypeDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

#define YYSTYPE HSQL_STYPE
#define YYLTYPE HSQL_LTYPE

struct HSQL_CUST_LTYPE {
  int first_line;
  int first_column;
  int last_line;
  int last_column;

  int total_column;

  // Length of the string in the SQL query string
  int string_length;

  // Parameters.
  // int param_id;
  std::vector<void*> param_list;
};

#define HSQL_LTYPE HSQL_CUST_LTYPE
#define HSQL_LTYPE_IS_DECLARED 1

#endif