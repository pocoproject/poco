/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */
#line 60 "bison_parser.y"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#line 74 "bison_parser.cpp"
/* Substitute the type names.  */
#define YYSTYPE         HSQL_STYPE
#define YYLTYPE         HSQL_LTYPE
/* Substitute the variable and function names.  */
#define yyparse         hsql_parse
#define yylex           hsql_lex
#define yyerror         hsql_error
#define yydebug         hsql_debug
#define yynerrs         hsql_nerrs

/* First part of user prologue.  */
#line 2 "bison_parser.y"


/**
 * bison_parser.y
 * defines bison_parser.h
 * outputs bison_parser.c
 *
 * Grammar File Spec: http://dinosaur.compilertools.net/bison/bison_6.html
 *
 */
/*********************************
 ** Section 1: C Declarations
 *********************************/

// clang-format on
#include "bison_parser.h"
#include "flex_lexer.h"

#include <stdio.h>
#include <string.h>
#include <string>

  using namespace hsql;

  int yyerror(YYLTYPE * llocp, SQLParserResult * result, yyscan_t scanner, const char* msg) {
    result->setIsValid(false);
    result->setErrorDetails(strdup(msg), llocp->first_line, llocp->first_column);
    return 0;
  }

  // Flattens a possibly qualified name back into one string, taking ownership
  // of both parts. Used where the statement has a single name field, e.g. the
  // procedure of an ODBC call escape.
  char* qualifiedName(TableName name) {
    if (!name.schema) return name.name;
    std::string combined(std::string(name.schema) + "." + name.name);
    free(name.schema);
    free(name.name);
    return strdup(combined.c_str());
  }
  // clang-format off

#line 129 "bison_parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "bison_parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_STRING = 4,                     /* STRING  */
  YYSYMBOL_FLOATVAL = 5,                   /* FLOATVAL  */
  YYSYMBOL_INTVAL = 6,                     /* INTVAL  */
  YYSYMBOL_DOLLAR_PARAM = 7,               /* DOLLAR_PARAM  */
  YYSYMBOL_NAMED_PARAM = 8,                /* NAMED_PARAM  */
  YYSYMBOL_DEALLOCATE = 9,                 /* DEALLOCATE  */
  YYSYMBOL_PARAMETERS = 10,                /* PARAMETERS  */
  YYSYMBOL_INTERSECT = 11,                 /* INTERSECT  */
  YYSYMBOL_TEMPORARY = 12,                 /* TEMPORARY  */
  YYSYMBOL_TIMESTAMP = 13,                 /* TIMESTAMP  */
  YYSYMBOL_DISTINCT = 14,                  /* DISTINCT  */
  YYSYMBOL_NVARCHAR = 15,                  /* NVARCHAR  */
  YYSYMBOL_RESTRICT = 16,                  /* RESTRICT  */
  YYSYMBOL_TRUNCATE = 17,                  /* TRUNCATE  */
  YYSYMBOL_ANALYZE = 18,                   /* ANALYZE  */
  YYSYMBOL_BETWEEN = 19,                   /* BETWEEN  */
  YYSYMBOL_CASCADE = 20,                   /* CASCADE  */
  YYSYMBOL_COLUMNS = 21,                   /* COLUMNS  */
  YYSYMBOL_CONTROL = 22,                   /* CONTROL  */
  YYSYMBOL_DEFAULT = 23,                   /* DEFAULT  */
  YYSYMBOL_EXECUTE = 24,                   /* EXECUTE  */
  YYSYMBOL_EXPLAIN = 25,                   /* EXPLAIN  */
  YYSYMBOL_ENCODING = 26,                  /* ENCODING  */
  YYSYMBOL_INTEGER = 27,                   /* INTEGER  */
  YYSYMBOL_NATURAL = 28,                   /* NATURAL  */
  YYSYMBOL_PREPARE = 29,                   /* PREPARE  */
  YYSYMBOL_SCHEMAS = 30,                   /* SCHEMAS  */
  YYSYMBOL_CHARACTER_VARYING = 31,         /* CHARACTER_VARYING  */
  YYSYMBOL_REAL = 32,                      /* REAL  */
  YYSYMBOL_DECIMAL = 33,                   /* DECIMAL  */
  YYSYMBOL_SMALLINT = 34,                  /* SMALLINT  */
  YYSYMBOL_BIGINT = 35,                    /* BIGINT  */
  YYSYMBOL_SPATIAL = 36,                   /* SPATIAL  */
  YYSYMBOL_VARCHAR = 37,                   /* VARCHAR  */
  YYSYMBOL_VIRTUAL = 38,                   /* VIRTUAL  */
  YYSYMBOL_DESCRIBE = 39,                  /* DESCRIBE  */
  YYSYMBOL_BEFORE = 40,                    /* BEFORE  */
  YYSYMBOL_COLUMN = 41,                    /* COLUMN  */
  YYSYMBOL_CREATE = 42,                    /* CREATE  */
  YYSYMBOL_DELETE = 43,                    /* DELETE  */
  YYSYMBOL_DIRECT = 44,                    /* DIRECT  */
  YYSYMBOL_DOUBLE = 45,                    /* DOUBLE  */
  YYSYMBOL_ESCAPE = 46,                    /* ESCAPE  */
  YYSYMBOL_EXCEPT = 47,                    /* EXCEPT  */
  YYSYMBOL_EXISTS = 48,                    /* EXISTS  */
  YYSYMBOL_EXTRACT = 49,                   /* EXTRACT  */
  YYSYMBOL_CAST = 50,                      /* CAST  */
  YYSYMBOL_FORMAT = 51,                    /* FORMAT  */
  YYSYMBOL_GLOBAL = 52,                    /* GLOBAL  */
  YYSYMBOL_HAVING = 53,                    /* HAVING  */
  YYSYMBOL_IMPORT = 54,                    /* IMPORT  */
  YYSYMBOL_INSERT = 55,                    /* INSERT  */
  YYSYMBOL_ISNULL = 56,                    /* ISNULL  */
  YYSYMBOL_OFFSET = 57,                    /* OFFSET  */
  YYSYMBOL_RENAME = 58,                    /* RENAME  */
  YYSYMBOL_SCHEMA = 59,                    /* SCHEMA  */
  YYSYMBOL_SELECT = 60,                    /* SELECT  */
  YYSYMBOL_SORTED = 61,                    /* SORTED  */
  YYSYMBOL_TABLES = 62,                    /* TABLES  */
  YYSYMBOL_UNLOAD = 63,                    /* UNLOAD  */
  YYSYMBOL_UPDATE = 64,                    /* UPDATE  */
  YYSYMBOL_VALUES = 65,                    /* VALUES  */
  YYSYMBOL_AFTER = 66,                     /* AFTER  */
  YYSYMBOL_ALTER = 67,                     /* ALTER  */
  YYSYMBOL_CROSS = 68,                     /* CROSS  */
  YYSYMBOL_DELTA = 69,                     /* DELTA  */
  YYSYMBOL_FLOAT = 70,                     /* FLOAT  */
  YYSYMBOL_GROUP = 71,                     /* GROUP  */
  YYSYMBOL_INDEX = 72,                     /* INDEX  */
  YYSYMBOL_INNER = 73,                     /* INNER  */
  YYSYMBOL_LIMIT = 74,                     /* LIMIT  */
  YYSYMBOL_LOCAL = 75,                     /* LOCAL  */
  YYSYMBOL_MERGE = 76,                     /* MERGE  */
  YYSYMBOL_MINUS = 77,                     /* MINUS  */
  YYSYMBOL_ORDER = 78,                     /* ORDER  */
  YYSYMBOL_OVER = 79,                      /* OVER  */
  YYSYMBOL_OUTER = 80,                     /* OUTER  */
  YYSYMBOL_RIGHT = 81,                     /* RIGHT  */
  YYSYMBOL_TABLE = 82,                     /* TABLE  */
  YYSYMBOL_UNION = 83,                     /* UNION  */
  YYSYMBOL_USING = 84,                     /* USING  */
  YYSYMBOL_WHERE = 85,                     /* WHERE  */
  YYSYMBOL_CALL = 86,                      /* CALL  */
  YYSYMBOL_CASE = 87,                      /* CASE  */
  YYSYMBOL_CHAR = 88,                      /* CHAR  */
  YYSYMBOL_COPY = 89,                      /* COPY  */
  YYSYMBOL_DATE = 90,                      /* DATE  */
  YYSYMBOL_DATETIME = 91,                  /* DATETIME  */
  YYSYMBOL_DESC = 92,                      /* DESC  */
  YYSYMBOL_DROP = 93,                      /* DROP  */
  YYSYMBOL_ELSE = 94,                      /* ELSE  */
  YYSYMBOL_FILE = 95,                      /* FILE  */
  YYSYMBOL_FROM = 96,                      /* FROM  */
  YYSYMBOL_FULL = 97,                      /* FULL  */
  YYSYMBOL_HASH = 98,                      /* HASH  */
  YYSYMBOL_HINT = 99,                      /* HINT  */
  YYSYMBOL_INTO = 100,                     /* INTO  */
  YYSYMBOL_JOIN = 101,                     /* JOIN  */
  YYSYMBOL_LEFT = 102,                     /* LEFT  */
  YYSYMBOL_LIKE = 103,                     /* LIKE  */
  YYSYMBOL_LOAD = 104,                     /* LOAD  */
  YYSYMBOL_LONG = 105,                     /* LONG  */
  YYSYMBOL_NULL = 106,                     /* NULL  */
  YYSYMBOL_PARTITION = 107,                /* PARTITION  */
  YYSYMBOL_PLAN = 108,                     /* PLAN  */
  YYSYMBOL_SHOW = 109,                     /* SHOW  */
  YYSYMBOL_TEXT = 110,                     /* TEXT  */
  YYSYMBOL_THEN = 111,                     /* THEN  */
  YYSYMBOL_TIME = 112,                     /* TIME  */
  YYSYMBOL_VIEW = 113,                     /* VIEW  */
  YYSYMBOL_WHEN = 114,                     /* WHEN  */
  YYSYMBOL_WITH = 115,                     /* WITH  */
  YYSYMBOL_ADD = 116,                      /* ADD  */
  YYSYMBOL_ALL = 117,                      /* ALL  */
  YYSYMBOL_AND = 118,                      /* AND  */
  YYSYMBOL_ASC = 119,                      /* ASC  */
  YYSYMBOL_END = 120,                      /* END  */
  YYSYMBOL_FOR = 121,                      /* FOR  */
  YYSYMBOL_INT = 122,                      /* INT  */
  YYSYMBOL_NOT = 123,                      /* NOT  */
  YYSYMBOL_OFF = 124,                      /* OFF  */
  YYSYMBOL_SET = 125,                      /* SET  */
  YYSYMBOL_TOP = 126,                      /* TOP  */
  YYSYMBOL_AS = 127,                       /* AS  */
  YYSYMBOL_BY = 128,                       /* BY  */
  YYSYMBOL_IF = 129,                       /* IF  */
  YYSYMBOL_IN = 130,                       /* IN  */
  YYSYMBOL_IS = 131,                       /* IS  */
  YYSYMBOL_OF = 132,                       /* OF  */
  YYSYMBOL_ON = 133,                       /* ON  */
  YYSYMBOL_OR = 134,                       /* OR  */
  YYSYMBOL_TO = 135,                       /* TO  */
  YYSYMBOL_NO = 136,                       /* NO  */
  YYSYMBOL_ARRAY = 137,                    /* ARRAY  */
  YYSYMBOL_CONCAT = 138,                   /* CONCAT  */
  YYSYMBOL_ILIKE = 139,                    /* ILIKE  */
  YYSYMBOL_SECOND = 140,                   /* SECOND  */
  YYSYMBOL_MINUTE = 141,                   /* MINUTE  */
  YYSYMBOL_HOUR = 142,                     /* HOUR  */
  YYSYMBOL_DAY = 143,                      /* DAY  */
  YYSYMBOL_MONTH = 144,                    /* MONTH  */
  YYSYMBOL_YEAR = 145,                     /* YEAR  */
  YYSYMBOL_SECONDS = 146,                  /* SECONDS  */
  YYSYMBOL_MINUTES = 147,                  /* MINUTES  */
  YYSYMBOL_HOURS = 148,                    /* HOURS  */
  YYSYMBOL_DAYS = 149,                     /* DAYS  */
  YYSYMBOL_MONTHS = 150,                   /* MONTHS  */
  YYSYMBOL_YEARS = 151,                    /* YEARS  */
  YYSYMBOL_INTERVAL = 152,                 /* INTERVAL  */
  YYSYMBOL_TRUE = 153,                     /* TRUE  */
  YYSYMBOL_FALSE = 154,                    /* FALSE  */
  YYSYMBOL_BOOLEAN = 155,                  /* BOOLEAN  */
  YYSYMBOL_TRANSACTION = 156,              /* TRANSACTION  */
  YYSYMBOL_BEGIN = 157,                    /* BEGIN  */
  YYSYMBOL_COMMIT = 158,                   /* COMMIT  */
  YYSYMBOL_ROLLBACK = 159,                 /* ROLLBACK  */
  YYSYMBOL_NOWAIT = 160,                   /* NOWAIT  */
  YYSYMBOL_SKIP = 161,                     /* SKIP  */
  YYSYMBOL_LOCKED = 162,                   /* LOCKED  */
  YYSYMBOL_SHARE = 163,                    /* SHARE  */
  YYSYMBOL_RANGE = 164,                    /* RANGE  */
  YYSYMBOL_ROWS = 165,                     /* ROWS  */
  YYSYMBOL_GROUPS = 166,                   /* GROUPS  */
  YYSYMBOL_UNBOUNDED = 167,                /* UNBOUNDED  */
  YYSYMBOL_FOLLOWING = 168,                /* FOLLOWING  */
  YYSYMBOL_PRECEDING = 169,                /* PRECEDING  */
  YYSYMBOL_CURRENT_ROW = 170,              /* CURRENT_ROW  */
  YYSYMBOL_FETCH = 171,                    /* FETCH  */
  YYSYMBOL_NEXT = 172,                     /* NEXT  */
  YYSYMBOL_ONLY = 173,                     /* ONLY  */
  YYSYMBOL_UNIQUE = 174,                   /* UNIQUE  */
  YYSYMBOL_PRIMARY = 175,                  /* PRIMARY  */
  YYSYMBOL_FOREIGN = 176,                  /* FOREIGN  */
  YYSYMBOL_KEY = 177,                      /* KEY  */
  YYSYMBOL_REFERENCES = 178,               /* REFERENCES  */
  YYSYMBOL_OUTERJOIN = 179,                /* OUTERJOIN  */
  YYSYMBOL_WITHIN = 180,                   /* WITHIN  */
  YYSYMBOL_CONNECT = 181,                  /* CONNECT  */
  YYSYMBOL_PRIOR = 182,                    /* PRIOR  */
  YYSYMBOL_START = 183,                    /* START  */
  YYSYMBOL_ODBC_OJ = 184,                  /* ODBC_OJ  */
  YYSYMBOL_NEXT_VALUE_FOR = 185,           /* NEXT_VALUE_FOR  */
  YYSYMBOL_186_ = 186,                     /* '='  */
  YYSYMBOL_EQUALS = 187,                   /* EQUALS  */
  YYSYMBOL_NOTEQUALS = 188,                /* NOTEQUALS  */
  YYSYMBOL_189_ = 189,                     /* '<'  */
  YYSYMBOL_190_ = 190,                     /* '>'  */
  YYSYMBOL_LESS = 191,                     /* LESS  */
  YYSYMBOL_GREATER = 192,                  /* GREATER  */
  YYSYMBOL_LESSEQ = 193,                   /* LESSEQ  */
  YYSYMBOL_GREATEREQ = 194,                /* GREATEREQ  */
  YYSYMBOL_NOTNULL = 195,                  /* NOTNULL  */
  YYSYMBOL_196_ = 196,                     /* '+'  */
  YYSYMBOL_197_ = 197,                     /* '-'  */
  YYSYMBOL_198_ = 198,                     /* '*'  */
  YYSYMBOL_199_ = 199,                     /* '/'  */
  YYSYMBOL_200_ = 200,                     /* '%'  */
  YYSYMBOL_201_ = 201,                     /* '^'  */
  YYSYMBOL_UMINUS = 202,                   /* UMINUS  */
  YYSYMBOL_203_ = 203,                     /* '['  */
  YYSYMBOL_204_ = 204,                     /* ']'  */
  YYSYMBOL_205_ = 205,                     /* '('  */
  YYSYMBOL_206_ = 206,                     /* ')'  */
  YYSYMBOL_207_ = 207,                     /* '.'  */
  YYSYMBOL_208_ = 208,                     /* ';'  */
  YYSYMBOL_209_ = 209,                     /* ','  */
  YYSYMBOL_210_ = 210,                     /* '{'  */
  YYSYMBOL_211_ = 211,                     /* '}'  */
  YYSYMBOL_212_ = 212,                     /* '?'  */
  YYSYMBOL_YYACCEPT = 213,                 /* $accept  */
  YYSYMBOL_input = 214,                    /* input  */
  YYSYMBOL_statement_list = 215,           /* statement_list  */
  YYSYMBOL_statement = 216,                /* statement  */
  YYSYMBOL_preparable_statement = 217,     /* preparable_statement  */
  YYSYMBOL_opt_hints = 218,                /* opt_hints  */
  YYSYMBOL_hint_list = 219,                /* hint_list  */
  YYSYMBOL_hint = 220,                     /* hint  */
  YYSYMBOL_transaction_statement = 221,    /* transaction_statement  */
  YYSYMBOL_opt_transaction_keyword = 222,  /* opt_transaction_keyword  */
  YYSYMBOL_prepare_statement = 223,        /* prepare_statement  */
  YYSYMBOL_prepare_target_query = 224,     /* prepare_target_query  */
  YYSYMBOL_odbc_call_statement = 225,      /* odbc_call_statement  */
  YYSYMBOL_opt_odbc_return = 226,          /* opt_odbc_return  */
  YYSYMBOL_execute_statement = 227,        /* execute_statement  */
  YYSYMBOL_import_statement = 228,         /* import_statement  */
  YYSYMBOL_file_type = 229,                /* file_type  */
  YYSYMBOL_file_path = 230,                /* file_path  */
  YYSYMBOL_opt_import_export_options = 231, /* opt_import_export_options  */
  YYSYMBOL_import_export_options = 232,    /* import_export_options  */
  YYSYMBOL_csv_option = 233,               /* csv_option  */
  YYSYMBOL_export_statement = 234,         /* export_statement  */
  YYSYMBOL_show_statement = 235,           /* show_statement  */
  YYSYMBOL_create_statement = 236,         /* create_statement  */
  YYSYMBOL_opt_not_exists = 237,           /* opt_not_exists  */
  YYSYMBOL_table_elem_commalist = 238,     /* table_elem_commalist  */
  YYSYMBOL_table_elem = 239,               /* table_elem  */
  YYSYMBOL_column_def = 240,               /* column_def  */
  YYSYMBOL_column_type = 241,              /* column_type  */
  YYSYMBOL_opt_time_precision = 242,       /* opt_time_precision  */
  YYSYMBOL_opt_decimal_specification = 243, /* opt_decimal_specification  */
  YYSYMBOL_opt_column_constraints = 244,   /* opt_column_constraints  */
  YYSYMBOL_column_constraints = 245,       /* column_constraints  */
  YYSYMBOL_column_constraint = 246,        /* column_constraint  */
  YYSYMBOL_table_constraint = 247,         /* table_constraint  */
  YYSYMBOL_references_spec = 248,          /* references_spec  */
  YYSYMBOL_drop_statement = 249,           /* drop_statement  */
  YYSYMBOL_opt_exists = 250,               /* opt_exists  */
  YYSYMBOL_alter_statement = 251,          /* alter_statement  */
  YYSYMBOL_alter_action = 252,             /* alter_action  */
  YYSYMBOL_drop_action = 253,              /* drop_action  */
  YYSYMBOL_delete_statement = 254,         /* delete_statement  */
  YYSYMBOL_truncate_statement = 255,       /* truncate_statement  */
  YYSYMBOL_insert_statement = 256,         /* insert_statement  */
  YYSYMBOL_opt_column_list = 257,          /* opt_column_list  */
  YYSYMBOL_update_statement = 258,         /* update_statement  */
  YYSYMBOL_update_clause_commalist = 259,  /* update_clause_commalist  */
  YYSYMBOL_update_clause = 260,            /* update_clause  */
  YYSYMBOL_select_statement = 261,         /* select_statement  */
  YYSYMBOL_select_within_set_operation = 262, /* select_within_set_operation  */
  YYSYMBOL_select_within_set_operation_no_parentheses = 263, /* select_within_set_operation_no_parentheses  */
  YYSYMBOL_select_with_paren = 264,        /* select_with_paren  */
  YYSYMBOL_select_no_paren = 265,          /* select_no_paren  */
  YYSYMBOL_set_operator = 266,             /* set_operator  */
  YYSYMBOL_set_type = 267,                 /* set_type  */
  YYSYMBOL_opt_all = 268,                  /* opt_all  */
  YYSYMBOL_opt_start_with = 269,           /* opt_start_with  */
  YYSYMBOL_opt_connect_by = 270,           /* opt_connect_by  */
  YYSYMBOL_select_clause = 271,            /* select_clause  */
  YYSYMBOL_opt_distinct = 272,             /* opt_distinct  */
  YYSYMBOL_select_list = 273,              /* select_list  */
  YYSYMBOL_opt_from_clause = 274,          /* opt_from_clause  */
  YYSYMBOL_from_clause = 275,              /* from_clause  */
  YYSYMBOL_opt_where = 276,                /* opt_where  */
  YYSYMBOL_opt_group = 277,                /* opt_group  */
  YYSYMBOL_opt_having = 278,               /* opt_having  */
  YYSYMBOL_opt_within_group = 279,         /* opt_within_group  */
  YYSYMBOL_opt_order = 280,                /* opt_order  */
  YYSYMBOL_order_list = 281,               /* order_list  */
  YYSYMBOL_order_desc = 282,               /* order_desc  */
  YYSYMBOL_opt_order_type = 283,           /* opt_order_type  */
  YYSYMBOL_opt_null_ordering = 284,        /* opt_null_ordering  */
  YYSYMBOL_opt_top = 285,                  /* opt_top  */
  YYSYMBOL_opt_limit = 286,                /* opt_limit  */
  YYSYMBOL_expr_list = 287,                /* expr_list  */
  YYSYMBOL_opt_extended_literal_list = 288, /* opt_extended_literal_list  */
  YYSYMBOL_extended_literal_list = 289,    /* extended_literal_list  */
  YYSYMBOL_casted_extended_literal = 290,  /* casted_extended_literal  */
  YYSYMBOL_extended_literal = 291,         /* extended_literal  */
  YYSYMBOL_expr_alias = 292,               /* expr_alias  */
  YYSYMBOL_expr = 293,                     /* expr  */
  YYSYMBOL_operand = 294,                  /* operand  */
  YYSYMBOL_scalar_expr = 295,              /* scalar_expr  */
  YYSYMBOL_unary_expr = 296,               /* unary_expr  */
  YYSYMBOL_binary_expr = 297,              /* binary_expr  */
  YYSYMBOL_logic_expr = 298,               /* logic_expr  */
  YYSYMBOL_in_expr = 299,                  /* in_expr  */
  YYSYMBOL_row_expr_list = 300,            /* row_expr_list  */
  YYSYMBOL_case_expr = 301,                /* case_expr  */
  YYSYMBOL_case_list = 302,                /* case_list  */
  YYSYMBOL_exists_expr = 303,              /* exists_expr  */
  YYSYMBOL_comp_expr = 304,                /* comp_expr  */
  YYSYMBOL_function_expr = 305,            /* function_expr  */
  YYSYMBOL_opt_window = 306,               /* opt_window  */
  YYSYMBOL_opt_partition = 307,            /* opt_partition  */
  YYSYMBOL_opt_frame_clause = 308,         /* opt_frame_clause  */
  YYSYMBOL_frame_type = 309,               /* frame_type  */
  YYSYMBOL_frame_bound = 310,              /* frame_bound  */
  YYSYMBOL_extract_expr = 311,             /* extract_expr  */
  YYSYMBOL_next_value_expr = 312,          /* next_value_expr  */
  YYSYMBOL_cast_expr = 313,                /* cast_expr  */
  YYSYMBOL_datetime_field = 314,           /* datetime_field  */
  YYSYMBOL_datetime_field_plural = 315,    /* datetime_field_plural  */
  YYSYMBOL_duration_field = 316,           /* duration_field  */
  YYSYMBOL_array_expr = 317,               /* array_expr  */
  YYSYMBOL_array_index = 318,              /* array_index  */
  YYSYMBOL_between_expr = 319,             /* between_expr  */
  YYSYMBOL_column_name = 320,              /* column_name  */
  YYSYMBOL_nonreserved_keyword = 321,      /* nonreserved_keyword  */
  YYSYMBOL_literal = 322,                  /* literal  */
  YYSYMBOL_string_literal = 323,           /* string_literal  */
  YYSYMBOL_bool_literal = 324,             /* bool_literal  */
  YYSYMBOL_num_literal = 325,              /* num_literal  */
  YYSYMBOL_int_literal = 326,              /* int_literal  */
  YYSYMBOL_null_literal = 327,             /* null_literal  */
  YYSYMBOL_date_literal = 328,             /* date_literal  */
  YYSYMBOL_interval_literal = 329,         /* interval_literal  */
  YYSYMBOL_param_expr = 330,               /* param_expr  */
  YYSYMBOL_table_ref = 331,                /* table_ref  */
  YYSYMBOL_table_ref_atomic = 332,         /* table_ref_atomic  */
  YYSYMBOL_nonjoin_table_ref_atomic = 333, /* nonjoin_table_ref_atomic  */
  YYSYMBOL_table_value_row_list = 334,     /* table_value_row_list  */
  YYSYMBOL_table_ref_commalist = 335,      /* table_ref_commalist  */
  YYSYMBOL_table_ref_name = 336,           /* table_ref_name  */
  YYSYMBOL_table_ref_name_no_alias = 337,  /* table_ref_name_no_alias  */
  YYSYMBOL_table_name = 338,               /* table_name  */
  YYSYMBOL_opt_index_name = 339,           /* opt_index_name  */
  YYSYMBOL_table_alias = 340,              /* table_alias  */
  YYSYMBOL_opt_table_alias = 341,          /* opt_table_alias  */
  YYSYMBOL_alias = 342,                    /* alias  */
  YYSYMBOL_opt_alias = 343,                /* opt_alias  */
  YYSYMBOL_opt_locking_clause = 344,       /* opt_locking_clause  */
  YYSYMBOL_opt_locking_clause_list = 345,  /* opt_locking_clause_list  */
  YYSYMBOL_locking_clause = 346,           /* locking_clause  */
  YYSYMBOL_row_lock_mode = 347,            /* row_lock_mode  */
  YYSYMBOL_opt_row_lock_policy = 348,      /* opt_row_lock_policy  */
  YYSYMBOL_opt_with_clause = 349,          /* opt_with_clause  */
  YYSYMBOL_with_clause = 350,              /* with_clause  */
  YYSYMBOL_with_description_list = 351,    /* with_description_list  */
  YYSYMBOL_with_description = 352,         /* with_description  */
  YYSYMBOL_join_clause = 353,              /* join_clause  */
  YYSYMBOL_opt_join_type = 354,            /* opt_join_type  */
  YYSYMBOL_join_condition = 355,           /* join_condition  */
  YYSYMBOL_opt_semicolon = 356,            /* opt_semicolon  */
  YYSYMBOL_ident_commalist = 357           /* ident_commalist  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined HSQL_LTYPE_IS_TRIVIAL && HSQL_LTYPE_IS_TRIVIAL \
             && defined HSQL_STYPE_IS_TRIVIAL && HSQL_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  73
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1555

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  213
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  145
/* YYNRULES -- Number of rules.  */
#define YYNRULES  417
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  779

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   448


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   200,     2,     2,
     205,   206,   198,   196,   209,   197,   207,   199,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   208,
     189,   186,   190,   212,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   203,     2,   204,   201,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   210,     2,   211,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   187,   188,   191,   192,   193,   194,   195,   202
};

#if HSQL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   394,   394,   414,   420,   427,   431,   435,   436,   437,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   455,   456,   458,   462,   467,   471,   481,   482,   483,
     485,   485,   491,   497,   503,   508,   513,   513,   515,   519,
     531,   537,   554,   569,   571,   572,   573,   575,   589,   593,
     603,   607,   631,   639,   652,   659,   674,   694,   695,   700,
     711,   724,   736,   743,   750,   759,   760,   762,   766,   771,
     772,   774,   782,   783,   784,   785,   786,   787,   788,   792,
     793,   794,   795,   796,   797,   798,   799,   800,   801,   802,
     804,   805,   807,   808,   809,   811,   812,   814,   818,   822,
     827,   835,   836,   837,   838,   840,   841,   842,   844,   852,
     858,   864,   870,   876,   877,   884,   890,   892,   902,   909,
     920,   927,   935,   936,   943,   950,   954,   959,   969,   973,
     977,   989,   989,   991,   992,  1001,  1002,  1004,  1018,  1030,
    1035,  1039,  1043,  1048,  1049,  1053,  1054,  1056,  1057,  1059,
    1071,  1072,  1074,  1076,  1077,  1079,  1081,  1082,  1084,  1089,
    1091,  1092,  1097,  1098,  1100,  1101,  1103,  1107,  1112,  1114,
    1115,  1116,  1118,  1119,  1145,  1146,  1147,  1149,  1150,  1151,
    1152,  1153,  1154,  1155,  1160,  1164,  1170,  1171,  1173,  1177,
    1182,  1182,  1186,  1187,  1188,  1190,  1199,  1199,  1199,  1199,
    1199,  1201,  1202,  1202,  1202,  1202,  1202,  1202,  1202,  1202,
    1202,  1203,  1203,  1207,  1207,  1211,  1213,  1215,  1216,  1217,
    1218,  1219,  1221,  1221,  1222,  1223,  1224,  1225,  1226,  1227,
    1228,  1229,  1230,  1232,  1233,  1235,  1236,  1237,  1238,  1242,
    1243,  1247,  1252,  1259,  1260,  1261,  1262,  1264,  1265,  1267,
    1268,  1270,  1271,  1272,  1273,  1274,  1275,  1276,  1280,  1281,
    1285,  1288,  1293,  1294,  1298,  1299,  1301,  1302,  1307,  1308,
    1309,  1313,  1314,  1315,  1317,  1318,  1319,  1320,  1321,  1323,
    1328,  1334,  1336,  1337,  1338,  1339,  1340,  1341,  1343,  1344,
    1345,  1346,  1347,  1348,  1350,  1350,  1352,  1358,  1363,  1365,
    1366,  1367,  1368,  1369,  1375,  1381,  1388,  1394,  1408,  1409,
    1410,  1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,
    1420,  1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,
    1430,  1432,  1432,  1432,  1432,  1432,  1432,  1432,  1434,  1436,
    1437,  1439,  1440,  1442,  1444,  1446,  1457,  1458,  1469,  1501,
    1506,  1518,  1527,  1527,  1534,  1534,  1538,  1540,  1540,  1548,
    1557,  1566,  1570,  1575,  1579,  1584,  1592,  1598,  1602,  1606,
    1617,  1618,  1620,  1620,  1622,  1622,  1624,  1628,  1629,  1631,
    1631,  1637,  1638,  1640,  1644,  1649,  1655,  1662,  1663,  1664,
    1665,  1667,  1668,  1669,  1675,  1675,  1677,  1679,  1683,  1688,
    1698,  1705,  1713,  1722,  1723,  1724,  1725,  1726,  1727,  1728,
    1729,  1730,  1731,  1733,  1739,  1739,  1742,  1746
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER", "STRING",
  "FLOATVAL", "INTVAL", "DOLLAR_PARAM", "NAMED_PARAM", "DEALLOCATE",
  "PARAMETERS", "INTERSECT", "TEMPORARY", "TIMESTAMP", "DISTINCT",
  "NVARCHAR", "RESTRICT", "TRUNCATE", "ANALYZE", "BETWEEN", "CASCADE",
  "COLUMNS", "CONTROL", "DEFAULT", "EXECUTE", "EXPLAIN", "ENCODING",
  "INTEGER", "NATURAL", "PREPARE", "SCHEMAS", "CHARACTER_VARYING", "REAL",
  "DECIMAL", "SMALLINT", "BIGINT", "SPATIAL", "VARCHAR", "VIRTUAL",
  "DESCRIBE", "BEFORE", "COLUMN", "CREATE", "DELETE", "DIRECT", "DOUBLE",
  "ESCAPE", "EXCEPT", "EXISTS", "EXTRACT", "CAST", "FORMAT", "GLOBAL",
  "HAVING", "IMPORT", "INSERT", "ISNULL", "OFFSET", "RENAME", "SCHEMA",
  "SELECT", "SORTED", "TABLES", "UNLOAD", "UPDATE", "VALUES", "AFTER",
  "ALTER", "CROSS", "DELTA", "FLOAT", "GROUP", "INDEX", "INNER", "LIMIT",
  "LOCAL", "MERGE", "MINUS", "ORDER", "OVER", "OUTER", "RIGHT", "TABLE",
  "UNION", "USING", "WHERE", "CALL", "CASE", "CHAR", "COPY", "DATE",
  "DATETIME", "DESC", "DROP", "ELSE", "FILE", "FROM", "FULL", "HASH",
  "HINT", "INTO", "JOIN", "LEFT", "LIKE", "LOAD", "LONG", "NULL",
  "PARTITION", "PLAN", "SHOW", "TEXT", "THEN", "TIME", "VIEW", "WHEN",
  "WITH", "ADD", "ALL", "AND", "ASC", "END", "FOR", "INT", "NOT", "OFF",
  "SET", "TOP", "AS", "BY", "IF", "IN", "IS", "OF", "ON", "OR", "TO", "NO",
  "ARRAY", "CONCAT", "ILIKE", "SECOND", "MINUTE", "HOUR", "DAY", "MONTH",
  "YEAR", "SECONDS", "MINUTES", "HOURS", "DAYS", "MONTHS", "YEARS",
  "INTERVAL", "TRUE", "FALSE", "BOOLEAN", "TRANSACTION", "BEGIN", "COMMIT",
  "ROLLBACK", "NOWAIT", "SKIP", "LOCKED", "SHARE", "RANGE", "ROWS",
  "GROUPS", "UNBOUNDED", "FOLLOWING", "PRECEDING", "CURRENT_ROW", "FETCH",
  "NEXT", "ONLY", "UNIQUE", "PRIMARY", "FOREIGN", "KEY", "REFERENCES",
  "OUTERJOIN", "WITHIN", "CONNECT", "PRIOR", "START", "ODBC_OJ",
  "NEXT_VALUE_FOR", "'='", "EQUALS", "NOTEQUALS", "'<'", "'>'", "LESS",
  "GREATER", "LESSEQ", "GREATEREQ", "NOTNULL", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'^'", "UMINUS", "'['", "']'", "'('", "')'", "'.'", "';'", "','",
  "'{'", "'}'", "'?'", "$accept", "input", "statement_list", "statement",
  "preparable_statement", "opt_hints", "hint_list", "hint",
  "transaction_statement", "opt_transaction_keyword", "prepare_statement",
  "prepare_target_query", "odbc_call_statement", "opt_odbc_return",
  "execute_statement", "import_statement", "file_type", "file_path",
  "opt_import_export_options", "import_export_options", "csv_option",
  "export_statement", "show_statement", "create_statement",
  "opt_not_exists", "table_elem_commalist", "table_elem", "column_def",
  "column_type", "opt_time_precision", "opt_decimal_specification",
  "opt_column_constraints", "column_constraints", "column_constraint",
  "table_constraint", "references_spec", "drop_statement", "opt_exists",
  "alter_statement", "alter_action", "drop_action", "delete_statement",
  "truncate_statement", "insert_statement", "opt_column_list",
  "update_statement", "update_clause_commalist", "update_clause",
  "select_statement", "select_within_set_operation",
  "select_within_set_operation_no_parentheses", "select_with_paren",
  "select_no_paren", "set_operator", "set_type", "opt_all",
  "opt_start_with", "opt_connect_by", "select_clause", "opt_distinct",
  "select_list", "opt_from_clause", "from_clause", "opt_where",
  "opt_group", "opt_having", "opt_within_group", "opt_order", "order_list",
  "order_desc", "opt_order_type", "opt_null_ordering", "opt_top",
  "opt_limit", "expr_list", "opt_extended_literal_list",
  "extended_literal_list", "casted_extended_literal", "extended_literal",
  "expr_alias", "expr", "operand", "scalar_expr", "unary_expr",
  "binary_expr", "logic_expr", "in_expr", "row_expr_list", "case_expr",
  "case_list", "exists_expr", "comp_expr", "function_expr", "opt_window",
  "opt_partition", "opt_frame_clause", "frame_type", "frame_bound",
  "extract_expr", "next_value_expr", "cast_expr", "datetime_field",
  "datetime_field_plural", "duration_field", "array_expr", "array_index",
  "between_expr", "column_name", "nonreserved_keyword", "literal",
  "string_literal", "bool_literal", "num_literal", "int_literal",
  "null_literal", "date_literal", "interval_literal", "param_expr",
  "table_ref", "table_ref_atomic", "nonjoin_table_ref_atomic",
  "table_value_row_list", "table_ref_commalist", "table_ref_name",
  "table_ref_name_no_alias", "table_name", "opt_index_name", "table_alias",
  "opt_table_alias", "alias", "opt_alias", "opt_locking_clause",
  "opt_locking_clause_list", "locking_clause", "row_lock_mode",
  "opt_row_lock_policy", "opt_with_clause", "with_clause",
  "with_description_list", "with_description", "join_clause",
  "opt_join_type", "join_condition", "opt_semicolon", "ident_commalist", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-653)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-415)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     650,    54,   106,   127,   131,   106,    30,    -5,    81,   107,
     106,   148,    24,   275,    49,   270,    32,    32,    32,    66,
     282,    82,  -653,   214,  -653,   214,  -653,  -653,  -653,  -653,
    -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,    -3,
    -653,   335,   152,  -653,   158,   286,  -653,   249,   249,   249,
     106,   364,   106,   266,  -653,   257,    -3,   307,   -22,   257,
     257,   257,   106,  -653,   301,   199,  -653,  -653,  -653,  -653,
    -653,   295,   391,  -653,   294,  -653,   380,  -653,  -653,   361,
      61,  -653,   355,  -653,   492,    88,   493,   398,   522,   106,
     106,   455,  -653,   441,   359,   558,   519,   106,   363,   365,
     566,   566,   566,   569,   106,   106,  -653,   370,   270,  -653,
     106,  -653,   371,    22,   563,  -653,  -653,  -653,    -3,   462,
     450,    -3,    50,   374,  -653,  -653,   586,  -653,  -653,   377,
     579,  -653,   580,  -653,  -653,    58,  -653,   379,   378,  -653,
    -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,
    -653,  -653,   538,  -653,   456,   -10,   359,   939,  -653,   566,
     587,    19,   420,   -27,  -653,  -653,   514,  -653,  -653,  -653,
     -70,   -70,   -70,  -653,  -653,  -653,  -653,  -653,    60,   605,
    -653,   939,  -653,  -653,   939,   532,  -653,  -653,    61,  -653,
    -653,   939,   532,   939,   451,   490,   609,  -653,  -653,  -653,
    -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,
    -653,  -653,   168,  -653,   373,  -653,  -653,  -653,    88,  -653,
     106,   610,   499,    77,   488,     6,  -653,  -653,  -653,   412,
     414,   415,  -653,  -653,   634,  -653,  -653,  -653,  1091,   418,
    -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,
    -653,  -653,  -653,  -653,  1243,  -653,   106,  1243,  -653,   787,
     303,  1279,  -653,  -653,  -653,  -653,  -653,  -653,  -653,  -653,
    -653,  -653,  -653,  -653,  -653,  -653,  -653,   443,   419,  -653,
     525,  -653,   198,   421,  -653,   939,   558,  -653,   588,  -653,
    -653,   422,   213,  -653,   455,  -653,    88,  -653,   423,   216,
    -653,     3,   534,   426,  -653,    52,    50,    -3,   434,  -653,
     162,    50,   -33,   574,     2,   150,  -653,   490,  -653,  -653,
     517,  -653,  -653,   440,   551,  -653,  1400,   445,   474,   475,
     226,  -653,  -653,  -653,   499,    27,    26,   593,   373,   939,
     939,   271,   185,   449,  1243,   787,  1174,   939,   452,  -653,
     291,   454,   448,    53,   230,   939,   939,  1243,  -653,  1243,
     -30,   453,    92,  1243,  1243,  1243,  1243,  1243,  1243,  1243,
    1243,  1243,  1243,  1243,  1243,  1243,  1243,  1243,   656,  -653,
      37,   106,  -653,   662,    88,   303,  -653,   257,   213,   664,
     665,   364,   666,   237,  -653,  -653,   460,    88,  -653,   605,
    -653,   860,   455,  -653,   939,  -653,   115,  -653,  -653,  -653,
    -653,   939,  -653,  -653,   669,   490,   502,   939,   939,  -653,
     498,  -653,   513,   183,  -653,  1400,   587,   566,  -653,  -653,
     472,  -653,   473,  -653,  -653,   476,  -653,  -653,   481,  -653,
    -653,  -653,  -653,   482,  -653,  -653,   256,   587,   483,   486,
    -653,    77,  -653,   601,   939,    97,  -653,   489,   598,    94,
      85,   298,   939,   939,  -653,   593,   149,   594,  -122,  -653,
     939,  -653,   -42,   939,  -653,   578,   702,  1121,  1243,   494,
     787,  -653,   592,   452,  1121,  1121,  1121,  1121,   334,   334,
     334,   334,   291,   291,   139,   139,   139,   -40,   496,   601,
     939,  -653,  -653,   238,   698,   254,  -653,  -653,  -653,  -653,
    -653,   244,   491,   260,  -653,   109,   860,   -16,    65,   419,
    -653,   273,  -653,   497,  -653,    65,  -653,   520,  -653,  -653,
    -653,  -653,   704,  -653,  -653,   536,   303,   303,   645,  -653,
     587,  -653,   548,  -653,   505,   261,  -653,   706,   707,  -653,
     709,   710,   712,  -653,  -653,   613,  -653,   543,   106,  -653,
     256,  -653,  -653,   265,   587,   587,  -653,   518,  -653,   274,
      40,    28,  -653,   939,  1400,   939,   939,  -653,   276,   217,
     535,  -653,  -653,   608,   537,  -653,  1243,  1121,   787,   539,
     287,  -653,  -653,  -653,   297,  -653,  -653,  -653,   740,   364,
    -653,  -653,  -653,   743,   195,   542,   544,   369,  -653,  -653,
    -653,   648,  -653,  -653,  -653,   671,   672,   673,   653,   860,
    -653,   640,   581,  -653,   939,  -653,   -84,  -653,  -653,  -653,
     554,   302,   555,   557,   560,  -653,  -653,   359,  -653,  -653,
    -653,   320,   337,   657,   589,   601,   939,   561,  -653,   154,
     564,   303,   281,  -653,   939,  -653,   562,   593,  1174,   567,
     338,  -653,  -653,   601,  -653,  -653,   268,  -653,   939,   339,
      65,   584,  1012,  -653,  -653,  -653,   860,   296,   939,   644,
     725,   191,  -653,  -653,  -653,   791,  -653,  -653,  -653,  -653,
    -653,   620,   674,   532,   728,   601,  -653,   344,    39,  -653,
    -653,  -653,   303,   593,   595,  -653,  -653,  -653,   345,    65,
     599,  -653,   587,  -653,   343,   303,   939,   675,  -653,   637,
     606,  -653,   939,   284,   616,  -653,   601,  -653,  -653,   607,
    -653,  -653,  -653,   939,   346,   617,   939,   303,   939,  -653,
    -653,   426,  -653,  -653,  -653,   612,    34,   733,  -653,  -653,
     350,  -653,   587,   303,  -653,   -19,  -653,   245,    38,   293,
    -653,  -653,   695,  -653,   356,   939,  -653,  -653,  -653,   708,
    -653,  -653,   939,  -653,   303,    38,   357,  -653,  -653
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,    31,    31,    37,
       0,   415,     3,    22,    20,    22,    19,    18,     8,     9,
       7,    11,    16,    17,    13,    14,    12,    15,    10,     0,
     394,     0,   367,   119,    38,     0,    59,    66,    66,    66,
       0,     0,     0,     0,   366,   114,     0,     0,     0,   114,
     114,   114,     0,    57,     0,   396,   397,    30,    27,    29,
      28,     0,     0,     1,   395,     2,     0,     6,     5,   176,
     128,   129,   165,   111,     0,   187,     0,     0,   371,     0,
       0,   157,    42,     0,   123,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,    36,
       0,     4,     0,     0,   151,   141,   142,   140,     0,   144,
       0,     0,   183,   368,   338,   341,   343,   350,   351,     0,
       0,   344,     0,   339,   340,     0,   349,     0,   186,   188,
     190,   192,   331,   332,   333,   342,   334,   335,   336,   337,
      33,    32,     0,   370,     0,     0,   123,     0,   118,     0,
       0,     0,     0,   157,   125,   113,     0,   136,   135,    43,
      46,    46,    46,   112,   109,   110,   399,   398,     0,     0,
     343,     0,   174,   150,     0,   165,   132,   131,   133,   143,
     139,     0,   165,     0,     0,   382,     0,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   346,     0,   345,   348,   193,   194,    39,     0,    65,
       0,     0,   395,     0,     0,   299,   327,   325,   323,     0,
       0,     0,   321,   320,     0,   322,   326,   324,     0,     0,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   330,   328,     0,   329,     0,     0,   301,     0,
     156,   196,   203,   204,   205,   198,   200,   206,   199,   222,
     207,   208,   210,   209,   211,   202,   197,   213,   303,   214,
       0,   416,     0,     0,   121,     0,     0,   124,     0,   115,
     116,     0,     0,    56,   157,    55,   187,    35,    25,     0,
      23,     0,   154,   152,   184,   380,   183,     0,   164,   166,
     171,   183,   178,   180,   177,     0,   137,   381,   383,   369,
       0,   347,   189,     0,     0,    62,     0,     0,     0,     0,
       0,    67,    69,    70,   395,   151,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   218,     0,   217,   280,
     216,     0,     0,   380,     0,     0,     0,     0,   219,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   215,
     151,     0,   122,     0,     0,   127,   126,   114,     0,     0,
       0,     0,     0,     0,    52,    41,     0,     0,    21,     0,
     175,     0,   157,   153,     0,   378,     0,   379,   195,   130,
     134,     0,   170,   169,   172,   382,     0,     0,     0,   387,
       0,   389,     0,   393,   384,     0,     0,     0,    88,    82,
       0,    84,    94,    85,    72,     0,    79,    80,     0,    76,
      77,    83,    86,    91,    81,    73,    96,     0,     0,     0,
      61,     0,    64,   265,     0,   300,   302,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,   212,
       0,   201,     0,     0,   233,   234,     0,   229,     0,     0,
       0,   220,     0,   232,   231,   251,   252,   253,   254,   255,
     256,   257,   224,   223,   226,   225,   227,   228,     0,   265,
       0,    40,   417,     0,     0,     0,    53,    50,    48,    54,
      45,     0,     0,     0,    24,   367,     0,   395,   375,     0,
     155,   352,   354,     0,   357,   375,   355,   146,   185,   376,
     377,   167,     0,   168,   138,     0,   181,   179,     0,   390,
       0,   392,     0,   385,     0,     0,    60,     0,     0,    78,
       0,     0,     0,    87,   103,     0,   102,     0,     0,    71,
      95,    97,    99,     0,     0,     0,    68,     0,   258,     0,
     151,     0,   249,     0,     0,     0,     0,   243,     0,     0,
       0,   296,   241,     0,     0,   242,     0,   230,     0,     0,
       0,   221,   297,   262,     0,   120,   117,    44,     0,     0,
      51,    34,    26,     0,   412,     0,     0,     0,   374,   359,
     372,     0,   411,   403,   409,   407,   410,   405,     0,     0,
     365,     0,   148,   173,     0,   388,   393,   391,   191,    63,
       0,     0,     0,     0,     0,   104,   101,   123,    98,   100,
     106,     0,     0,   267,   163,   265,     0,   304,   306,     0,
       0,   247,     0,   246,     0,   250,     0,     0,   298,     0,
       0,   237,   235,   265,    49,    47,   368,   356,     0,     0,
     375,   376,     0,   406,   408,   404,     0,   353,     0,     0,
     159,     0,   386,    75,    93,     0,    89,    74,    90,   108,
     105,     0,     0,   165,     0,   265,   260,     0,     0,   279,
     281,   244,   248,     0,     0,   238,   236,   263,     0,   375,
       0,   358,     0,   400,   412,   145,     0,     0,   149,     0,
       0,   107,     0,   270,     0,   259,   265,   305,   307,     0,
     239,   361,   360,     0,     0,     0,     0,   147,     0,   182,
      92,   266,   271,   272,   273,     0,     0,     0,   261,   240,
       0,   373,     0,   413,   401,   161,   264,     0,     0,     0,
     278,   268,     0,   362,     0,     0,   158,   277,   275,     0,
     276,   274,     0,   402,   160,     0,     0,   269,   162
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -653,  -653,  -653,   753,  -653,   803,  -653,   430,  -653,   468,
    -653,  -653,  -653,  -653,  -653,  -653,  -368,   -88,   348,   442,
     323,  -653,  -653,  -653,   461,  -653,   393,  -653,  -377,  -653,
    -653,  -653,  -653,   285,  -653,  -500,  -653,   -53,  -653,  -653,
    -653,  -653,  -653,  -653,  -153,  -653,  -653,   556,  -207,   -89,
    -653,   210,   -39,    -7,  -653,  -653,  -653,  -653,   -75,  -313,
    -653,  -653,  -653,  -133,  -653,  -653,  -653,  -181,    76,   438,
    -653,  -653,  -653,    20,  -338,   559,  -269,   632,   639,  -235,
    -156,  -218,  -653,  -653,  -653,  -653,  -653,  -653,  -653,   511,
    -653,  -653,  -385,  -478,  -653,  -653,  -653,  -652,  -653,  -653,
    -653,  -145,  -653,  -653,  -653,  -653,  -653,  -653,  -383,   -52,
    -653,  -653,   718,   -94,  -653,  -653,   719,  -653,  -653,  -451,
     184,  -653,  -653,  -653,  -653,     0,  -653,  -653,  -464,  -239,
    -653,   444,  -653,   540,  -653,   232,  -653,  -653,  -653,   754,
    -653,  -653,  -653,  -653,  -400
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    20,    21,    22,    23,    77,   299,   300,    24,    68,
      25,   151,    26,    72,    27,    28,    93,   170,   293,   393,
     394,    29,    30,    31,    88,   330,   331,   332,   446,   553,
     549,   559,   560,   561,   333,   562,    32,    97,    33,   289,
     290,    34,    35,    36,   161,    37,   163,   164,    38,   185,
     186,   187,   351,   118,   119,   190,   622,   680,    82,   184,
     302,   402,   403,   158,   718,   766,   695,   122,   308,   309,
     414,   533,   114,   195,   303,   137,   138,   139,   140,   304,
     305,   261,   262,   263,   264,   265,   266,   354,   267,   342,
     268,   269,   270,   568,   693,   745,   746,   761,   271,   272,
     273,   209,   210,   211,   274,   275,   276,   277,   278,   279,
     142,   143,   144,   145,   146,   147,   148,   149,   520,   521,
     522,   669,   523,   524,    53,   525,   154,   608,   609,   610,
     408,   316,   317,   318,   423,   543,    39,    40,    65,    66,
     526,   618,   754,    75,   282
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      81,   260,    43,   224,   306,    46,   103,   104,   105,   468,
      54,   311,    58,   171,   172,   325,   518,    99,   519,   182,
     346,   593,   454,   508,   352,   301,   545,    42,   180,   455,
     287,   647,   192,   141,   765,   310,   348,   312,   314,   350,
     757,   183,   727,   188,   757,   291,   188,   563,   544,   605,
      91,   183,    94,   758,   183,   405,   405,    79,   157,   418,
     639,   620,   106,   125,   126,   604,   407,   500,   405,   321,
      62,   280,   115,   478,   101,   121,   541,   542,   341,    79,
     326,   583,   581,    41,   283,   355,   221,   404,   584,   155,
     156,    50,   124,   125,   126,   127,   128,   166,   363,    15,
     479,   356,    47,   353,   174,   175,   769,   193,   116,    42,
     178,    63,    48,   102,   407,   503,   569,   222,   529,   530,
     355,   355,   284,   777,   194,   383,   346,   452,   513,   385,
      44,   518,   416,   519,    45,   292,   356,   356,   129,   476,
     626,   477,   590,    49,   117,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     141,   395,   594,   378,   641,   642,   141,   696,   677,   528,
     355,   355,   124,   125,   126,   127,   128,    51,   130,   406,
     406,   307,   286,   459,   460,   707,   356,   356,    67,   466,
     404,   721,   607,   458,   131,   223,   575,   650,   481,   474,
     475,   759,    56,   355,   760,   759,   711,    52,   760,   400,
     132,   335,   355,   336,   419,   482,   389,   725,   410,   356,
     323,   574,    57,   611,   456,   714,   648,   181,   356,    56,
      55,   665,   188,   453,   518,   582,   519,   728,   585,   390,
     132,   133,   134,   499,   141,   732,   645,   389,   748,    80,
     660,   327,   328,   329,   412,   310,   349,   646,   130,   471,
     587,   536,   537,   612,   391,   296,    98,   355,   613,   527,
     598,   297,   355,    64,   131,   614,   615,   363,    71,   462,
     355,   413,    73,   356,   498,   135,   420,   518,   356,   519,
      74,   518,   616,   519,  -414,   599,   356,   617,   457,   463,
     136,   611,   570,     1,   571,   464,   578,   579,   697,   355,
     606,     2,   734,   421,   335,   540,   603,   176,     3,   392,
     132,   133,   134,     4,   611,   356,   409,   422,   654,    76,
     708,   415,   141,     5,   504,   355,     6,     7,    83,   546,
     377,   612,   378,   541,   542,   141,   613,    59,     8,     9,
     392,   356,   764,   614,   615,   471,   719,    60,    10,    84,
     699,    11,   554,    85,   612,   135,   115,    92,   658,   613,
     616,   611,   671,   530,  -412,   617,   614,   615,    87,   555,
     136,   501,    86,    12,   741,   340,    96,    13,    61,   355,
     358,    95,   576,   616,   355,   750,   653,  -412,   617,   355,
     755,   701,   116,    14,   382,   356,   667,   383,   108,    15,
     356,   612,   463,   767,   768,   356,   613,   649,   577,   651,
     652,   355,   398,   614,   615,   399,   580,   735,   107,   363,
     556,   557,   450,   120,   558,   451,   472,   356,   117,   473,
     616,   589,   100,   510,   595,   617,   511,   218,   742,   743,
     744,    16,    17,    18,   225,   124,   125,   126,   127,   128,
     597,   770,   771,   511,   226,   362,   602,   629,   681,   218,
     383,   640,   363,   570,   383,   196,   736,   110,   227,   112,
     644,   109,  -363,   404,   689,    69,    70,   113,   228,   374,
     375,   376,   377,   662,   378,   123,   404,   150,   702,   229,
     230,   231,   232,   663,    19,  -364,   404,   233,   684,    89,
      90,   685,   723,   197,   198,   199,   200,   201,   202,   294,
     295,   152,   715,  -415,  -415,   153,   690,  -415,  -415,   383,
     372,   373,   374,   375,   376,   377,   159,   378,   234,   235,
     157,   130,   236,   691,   706,   709,   383,   404,   710,   659,
     726,   731,   751,   404,   404,   383,   763,   131,   637,   404,
     737,   162,   773,   778,   160,   383,   411,   165,   313,   167,
     169,   168,   173,   237,   238,    56,   179,   183,   191,   189,
     753,   196,   212,   213,   214,   217,   219,   218,   239,   220,
     281,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   132,   133,   134,   285,   288,   298,   774,
     120,   315,   319,   324,    15,   334,   310,   337,   704,   338,
     339,   347,   379,   252,   380,   381,   384,   388,   397,   387,
     401,   417,   253,   254,   255,   404,   256,   225,   124,   125,
     126,   127,   128,   411,   425,   426,   427,   226,   257,   258,
     447,   448,   449,    79,   465,   378,   259,   470,   480,     1,
     469,   227,   180,   136,   729,   502,   512,     2,   506,   507,
     509,   228,   532,   535,     3,   538,   539,   547,   548,     4,
     567,   550,   229,   230,   231,   232,   551,   552,   564,     5,
     233,   565,     6,     7,   573,   572,   355,   478,   591,   588,
     592,   596,   601,   621,     8,     9,   619,   623,   624,   625,
     627,   628,   630,   631,    10,   632,   633,    11,   634,   635,
     636,   234,   235,   643,   130,   236,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   656,    12,
     131,   655,   657,    13,   664,   661,   666,   668,   340,   672,
     670,   673,   674,   675,   676,   678,   237,   238,   358,    14,
     683,   686,   679,   687,   692,    15,   688,   703,   698,   694,
     700,   239,   716,   705,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   132,   133,   134,   712,
     225,   124,   125,   126,   127,   128,   717,   720,   558,   724,
     226,   730,   722,   738,   733,   359,   252,    16,    17,    18,
     739,   762,   740,   749,   227,   253,   254,   255,   756,   256,
     586,   747,   752,   772,   228,   467,   775,   111,    78,   514,
     505,   257,   258,   362,   600,   229,   230,   231,   232,   259,
     363,   364,   386,   233,   566,   638,   136,    79,   776,   531,
     322,   320,   461,   215,   216,   396,   713,   424,   682,   534,
      19,     0,   177,   515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   226,   234,   235,     0,   130,   236,     0,
       0,     0,     0,     0,     0,     0,     0,   227,   365,   366,
     367,   368,   369,   131,     0,   370,   371,   228,   372,   373,
     374,   375,   376,   377,     0,   378,     0,     0,     0,   237,
     238,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   239,     0,     0,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   132,
     133,   134,   225,   124,   125,   126,   127,   128,   235,     0,
       0,   236,   226,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,     0,     0,     0,   227,     0,   253,   254,
     255,     0,   256,     0,     0,     0,   228,     0,     0,     0,
       0,     0,   237,     0,   257,   258,     0,   229,   230,   231,
     232,     0,   259,     0,     0,   233,     0,     0,     0,   136,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,     0,     0,     0,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   226,   234,   235,     0,   130,
     236,     0,   252,     0,     0,     0,     0,     0,     0,   227,
       0,   253,     0,   255,   516,   131,     0,     0,     0,   228,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,   238,   232,     0,   517,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   239,     0,     0,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   132,   133,   134,   225,   124,   125,   126,   127,   128,
     235,     0,     0,   236,   226,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,     0,     0,     0,   227,     0,
     253,   254,   255,     0,   256,     0,     0,     0,   228,     0,
       0,     0,     0,     0,   237,     0,   257,   258,     0,   343,
     230,   231,   232,     0,   259,     0,     0,   233,     0,     0,
       0,   136,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   358,   234,   235,
       0,   130,   236,     0,   252,     0,     0,     0,     0,     0,
       0,     0,     0,   253,     0,   255,     0,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,   344,     0,     0,   517,     0,     0,
       0,     0,     0,     0,  -415,     0,     0,     0,   239,     0,
     358,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   132,   133,   134,   225,   124,   125,   126,
     127,   128,   362,     0,     0,     0,   226,     0,     0,   363,
    -415,     0,     0,   252,     0,     0,     0,     0,     0,     0,
     227,     0,   253,   254,   255,     0,   256,   359,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,   257,   258,
       0,     0,   230,   231,   232,     0,   345,   467,   357,   233,
       0,     0,     0,   136,     0,   362,     0,  -415,  -415,  -415,
     368,   369,   363,   364,   370,   371,     0,   372,   373,   374,
     375,   376,   377,     0,   378,     0,     0,     0,     0,     0,
     234,   235,     0,   130,   236,   358,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     365,   366,   367,   368,   369,   237,   344,   370,   371,     0,
     372,   373,   374,   375,   376,   377,     0,   378,     0,     0,
     239,     0,   359,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   132,   133,   134,     0,     0,
       0,     0,   360,     0,     0,     0,     0,     0,     0,   361,
     362,     0,     0,   428,     0,   252,     0,   363,   364,     0,
       0,     0,     0,     0,   253,   254,   255,   429,   256,     0,
       0,   430,   431,   432,   433,   434,     0,   435,     0,     0,
     257,   258,     0,     0,     0,   436,     0,     0,   345,     0,
       0,     0,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,   366,   367,   368,   369,
     437,     0,   370,   371,     0,   372,   373,   374,   375,   376,
     377,     0,   378,     0,     0,     0,     0,     0,   438,     0,
     439,   440,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   441,     0,     0,     0,     0,
     442,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   444,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   445
};

static const yytype_int16 yycheck[] =
{
      39,   157,     2,   156,   185,     5,    59,    60,    61,   347,
      10,   192,    12,   101,   102,   222,   401,    56,   401,   113,
     238,   499,   335,   391,   259,   181,   426,     3,     6,     3,
     163,     3,   121,    85,    53,   191,   254,   193,   194,   257,
       6,    14,     3,   118,     6,   115,   121,   447,   425,    65,
      50,    14,    52,    19,    14,     3,     3,    60,    85,    57,
     560,   525,    62,     5,     6,   516,   305,   380,     3,   214,
      21,   159,    11,   103,    96,    82,   160,   161,   234,    60,
       3,   123,   204,    29,    65,   118,    96,   209,   130,    89,
      90,    96,     4,     5,     6,     7,     8,    97,   138,   115,
     130,   134,    72,   259,   104,   105,   758,    57,    47,     3,
     110,    62,    82,   135,   353,   384,   454,   127,     3,     4,
     118,   118,   161,   775,    74,   209,   344,   334,   397,   285,
       3,   516,   165,   516,     3,   205,   134,   134,    50,   357,
     540,   359,   480,   113,    83,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     212,   294,   500,   203,   564,   565,   218,   645,   619,   404,
     118,   118,     4,     5,     6,     7,     8,    96,    90,   127,
     127,   188,   209,   339,   340,   663,   134,   134,   156,   345,
     209,   691,   127,   338,   106,   205,   111,   574,   106,   355,
     356,   167,   205,   118,   170,   167,   670,   100,   170,   206,
     152,   205,   118,   207,    64,   123,     3,   695,   307,   134,
     220,   127,    12,    28,   198,   676,   198,   205,   134,   205,
      82,   599,   307,   206,   619,   470,   619,   198,   473,    26,
     152,   153,   154,   206,   296,   709,   206,     3,   726,    39,
     588,   174,   175,   176,    92,   411,   256,   570,    90,   206,
     478,   417,   418,    68,    51,   205,    56,   118,    73,   402,
      26,   211,   118,     3,   106,    80,    81,   138,   212,    94,
     118,   119,     0,   134,   378,   197,   136,   672,   134,   672,
     208,   676,    97,   676,     0,    51,   134,   102,   337,   114,
     212,    28,   205,     9,   207,   120,   462,   463,   646,   118,
     517,    17,   712,   163,   205,   132,   207,   107,    24,   106,
     152,   153,   154,    29,    28,   134,   306,   177,   111,   115,
     668,   311,   384,    39,   387,   118,    42,    43,     3,   427,
     201,    68,   203,   160,   161,   397,    73,    72,    54,    55,
     106,   134,   752,    80,    81,   206,   165,    82,    64,   207,
     206,    67,   106,   205,    68,   197,    11,     3,   586,    73,
      97,    28,     3,     4,   101,   102,    80,    81,   129,   123,
     212,   381,    96,    89,   722,   114,   129,    93,   113,   118,
      56,   125,    94,    97,   118,   733,   120,   101,   102,   118,
     738,   120,    47,   109,   206,   134,   211,   209,   209,   115,
     134,    68,   114,   168,   169,   134,    73,   573,   120,   575,
     576,   118,   206,    80,    81,   209,   465,    84,   127,   138,
     174,   175,   206,    78,   178,   209,   206,   134,    83,   209,
      97,   480,   135,   206,   206,   102,   209,   209,   164,   165,
     166,   157,   158,   159,     3,     4,     5,     6,     7,     8,
     206,   168,   169,   209,    13,   131,   206,   206,   624,   209,
     209,   206,   138,   205,   209,   207,   133,    86,    27,    99,
     206,   186,   209,   209,   637,    17,    18,   126,    37,   198,
     199,   200,   201,   206,   203,     3,   209,     4,   654,    48,
      49,    50,    51,   206,   210,   209,   209,    56,   206,    48,
      49,   209,   693,   140,   141,   142,   143,   144,   145,   171,
     172,   123,   678,   189,   190,     3,   206,   193,   194,   209,
     196,   197,   198,   199,   200,   201,    95,   203,    87,    88,
      85,    90,    91,   206,   206,   206,   209,   209,   209,   588,
     206,   206,   206,   209,   209,   209,   206,   106,   558,   209,
     716,     3,   206,   206,   205,   209,   209,    48,   117,   206,
       4,   206,     3,   122,   123,   205,   205,    14,   128,   117,
     736,   207,   205,     4,     4,   206,    48,   209,   137,   133,
       3,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   186,    93,     3,   765,
      78,   121,     3,     3,   115,   127,   772,   205,   657,   205,
     205,   203,   179,   172,   205,   100,   205,   205,   205,    41,
      96,    57,   181,   182,   183,   209,   185,     3,     4,     5,
       6,     7,     8,   209,   127,   205,    95,    13,   197,   198,
     205,   177,   177,    60,   205,   203,   205,   209,   205,     9,
     206,    27,     6,   212,   703,     3,   206,    17,     4,     4,
       4,    37,     3,   171,    24,   177,   163,   205,   205,    29,
      79,   205,    48,    49,    50,    51,   205,   205,   205,    39,
      56,   205,    42,    43,    96,   206,   118,   103,   106,   205,
     204,     3,   211,   183,    54,    55,   209,     3,   172,    64,
     162,   206,     6,     6,    64,     6,     6,    67,     6,   106,
     177,    87,    88,   205,    90,    91,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   130,    89,
     106,   206,   205,    93,     4,   206,     3,   205,   114,   101,
     206,    80,    80,    80,   101,   115,   122,   123,    56,   109,
     206,   206,   181,   206,   107,   115,   206,   205,   207,   180,
     206,   137,   128,   206,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   205,
       3,     4,     5,     6,     7,     8,    71,     6,   178,    71,
      13,   206,   128,   128,   205,   103,   172,   157,   158,   159,
     173,    78,   206,   206,    27,   181,   182,   183,   206,   185,
     118,   205,   205,   128,    37,   123,   118,    74,    25,   399,
     388,   197,   198,   131,   511,    48,    49,    50,    51,   205,
     138,   139,   286,    56,   451,   560,   212,    60,   772,   411,
     218,   212,   341,   135,   135,   296,   672,   317,   626,   415,
     210,    -1,   108,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    87,    88,    -1,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,   186,   187,
     188,   189,   190,   106,    -1,   193,   194,    37,   196,   197,
     198,   199,   200,   201,    -1,   203,    -1,    -1,    -1,   122,
     123,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,     3,     4,     5,     6,     7,     8,    88,    -1,
      -1,    91,    13,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,   181,   182,
     183,    -1,   185,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   197,   198,    -1,    48,    49,    50,
      51,    -1,   205,    -1,    -1,    56,    -1,    -1,    -1,   212,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    87,    88,    -1,    90,
      91,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,   181,    -1,   183,   184,   106,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,    51,    -1,   205,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,     3,     4,     5,     6,     7,     8,
      88,    -1,    -1,    91,    13,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
     181,   182,   183,    -1,   185,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   197,   198,    -1,    48,
      49,    50,    51,    -1,   205,    -1,    -1,    56,    -1,    -1,
      -1,   212,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    87,    88,
      -1,    90,    91,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,   183,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,    -1,    -1,   205,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,   137,    -1,
      56,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,     3,     4,     5,     6,
       7,     8,   131,    -1,    -1,    -1,    13,    -1,    -1,   138,
     139,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,   181,   182,   183,    -1,   185,   103,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
      -1,    -1,    49,    50,    51,    -1,   205,   123,    19,    56,
      -1,    -1,    -1,   212,    -1,   131,    -1,   186,   187,   188,
     189,   190,   138,   139,   193,   194,    -1,   196,   197,   198,
     199,   200,   201,    -1,   203,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    90,    91,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,   187,   188,   189,   190,   122,   123,   193,   194,    -1,
     196,   197,   198,   199,   200,   201,    -1,   203,    -1,    -1,
     137,    -1,   103,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,    -1,    -1,    13,    -1,   172,    -1,   138,   139,    -1,
      -1,    -1,    -1,    -1,   181,   182,   183,    27,   185,    -1,
      -1,    31,    32,    33,    34,    35,    -1,    37,    -1,    -1,
     197,   198,    -1,    -1,    -1,    45,    -1,    -1,   205,    -1,
      -1,    -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,   187,   188,   189,   190,
      70,    -1,   193,   194,    -1,   196,   197,   198,   199,   200,
     201,    -1,   203,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,
     110,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     9,    17,    24,    29,    39,    42,    43,    54,    55,
      64,    67,    89,    93,   109,   115,   157,   158,   159,   210,
     214,   215,   216,   217,   221,   223,   225,   227,   228,   234,
     235,   236,   249,   251,   254,   255,   256,   258,   261,   349,
     350,    29,     3,   338,     3,     3,   338,    72,    82,   113,
      96,    96,   100,   337,   338,    82,   205,   264,   338,    72,
      82,   113,    21,    62,     3,   351,   352,   156,   222,   222,
     222,   212,   226,     0,   208,   356,   115,   218,   218,    60,
     264,   265,   271,     3,   207,   205,    96,   129,   237,   237,
     237,   338,     3,   229,   338,   125,   129,   250,   264,   265,
     135,    96,   135,   250,   250,   250,   338,   127,   209,   186,
      86,   216,    99,   126,   285,    11,    47,    83,   266,   267,
      78,   266,   280,     3,     4,     5,     6,     7,     8,    50,
      90,   106,   152,   153,   154,   197,   212,   288,   289,   290,
     291,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       4,   224,   123,     3,   339,   338,   338,    85,   276,    95,
     205,   257,     3,   259,   260,    48,   338,   206,   206,     4,
     230,   230,   230,     3,   338,   338,   264,   352,   338,   205,
       6,   205,   326,    14,   272,   262,   263,   264,   271,   117,
     268,   128,   262,    57,    74,   286,   207,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   314,
     315,   316,   205,     4,     4,   325,   329,   206,   209,    48,
     133,    96,   127,   205,   257,     3,    13,    27,    37,    48,
      49,    50,    51,    56,    87,    88,    91,   122,   123,   137,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   172,   181,   182,   183,   185,   197,   198,   205,
     293,   294,   295,   296,   297,   298,   299,   301,   303,   304,
     305,   311,   312,   313,   317,   318,   319,   320,   321,   322,
     230,     3,   357,    65,   265,   186,   209,   276,    93,   252,
     253,   115,   205,   231,   231,   231,   205,   211,     3,   219,
     220,   293,   273,   287,   292,   293,   280,   266,   281,   282,
     293,   280,   293,   117,   293,   121,   344,   345,   346,     3,
     291,   314,   290,   338,     3,   261,     3,   174,   175,   176,
     238,   239,   240,   247,   127,   205,   207,   205,   205,   205,
     114,   293,   302,    48,   123,   205,   294,   203,   294,   338,
     294,   265,   292,   293,   300,   118,   134,    19,    56,   103,
     123,   130,   131,   138,   139,   186,   187,   188,   189,   190,
     193,   194,   196,   197,   198,   199,   200,   201,   203,   179,
     205,   100,   206,   209,   205,   293,   260,    41,   205,     3,
      26,    51,   106,   232,   233,   276,   288,   205,   206,   209,
     206,    96,   274,   275,   209,     3,   127,   342,   343,   286,
     262,   209,    92,   119,   283,   286,   165,    57,    57,    64,
     136,   163,   177,   347,   346,   127,   205,    95,    13,    27,
      31,    32,    33,    34,    35,    37,    45,    70,    88,    90,
      91,   105,   110,   112,   122,   155,   241,   205,   177,   177,
     206,   209,   261,   206,   272,     3,   198,   265,   314,   293,
     293,   302,    94,   114,   120,   205,   293,   123,   287,   206,
     209,   206,   206,   209,   293,   293,   294,   294,   103,   130,
     205,   106,   123,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   326,   206,
     272,   338,     3,   289,   250,   232,     4,     4,   229,     4,
     206,   209,   206,   289,   220,     3,   184,   205,   305,   321,
     331,   332,   333,   335,   336,   338,   353,   276,   292,     3,
       4,   282,     3,   284,   344,   171,   293,   293,   177,   163,
     132,   160,   161,   348,   241,   357,   230,   205,   205,   243,
     205,   205,   205,   242,   106,   123,   174,   175,   178,   244,
     245,   246,   248,   357,   205,   205,   239,    79,   306,   287,
     205,   207,   206,    96,   127,   111,    94,   120,   293,   293,
     265,   204,   292,   123,   130,   292,   118,   294,   205,   265,
     287,   106,   204,   306,   287,   206,     3,   206,    26,    51,
     233,   211,   206,   207,   332,    65,   261,   127,   340,   341,
     342,    28,    68,    73,    80,    81,    97,   102,   354,   209,
     341,   183,   269,     3,   172,    64,   357,   162,   206,   206,
       6,     6,     6,     6,     6,   106,   177,   338,   246,   248,
     206,   357,   357,   205,   206,   206,   272,     3,   198,   293,
     241,   293,   293,   120,   111,   206,   130,   205,   294,   265,
     287,   206,   206,   206,     4,   229,     3,   211,   205,   334,
     206,     3,   101,    80,    80,    80,   101,   332,   115,   181,
     270,   293,   348,   206,   206,   209,   206,   206,   206,   257,
     206,   206,   107,   307,   180,   279,   306,   287,   207,   206,
     206,   120,   293,   205,   265,   206,   206,   306,   287,   206,
     209,   341,   205,   333,   332,   293,   128,    71,   277,   165,
       6,   248,   128,   280,    71,   306,   206,     3,   198,   265,
     206,   206,   341,   205,   357,    84,   133,   293,   128,   173,
     206,   287,   164,   165,   166,   308,   309,   205,   306,   206,
     287,   206,   205,   293,   355,   287,   206,     6,    19,   167,
     170,   310,    78,   206,   357,    53,   278,   168,   169,   310,
     168,   169,   128,   206,   293,   118,   281,   310,   206
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   213,   214,   215,   215,   216,   216,   216,   216,   216,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   218,   218,   219,   219,   220,   220,   221,   221,   221,
     222,   222,   223,   224,   225,   225,   226,   226,   227,   227,
     228,   228,   229,   230,   231,   231,   231,   232,   232,   232,
     232,   232,   232,   233,   233,   234,   234,   235,   235,   235,
     236,   236,   236,   236,   236,   237,   237,   238,   238,   239,
     239,   240,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     242,   242,   243,   243,   243,   244,   244,   245,   245,   245,
     245,   246,   246,   246,   246,   247,   247,   247,   248,   249,
     249,   249,   249,   250,   250,   251,   252,   253,   254,   255,
     256,   256,   257,   257,   258,   259,   259,   260,   261,   261,
     261,   262,   262,   263,   263,   264,   264,   265,   265,   266,
     267,   267,   267,   268,   268,   269,   269,   270,   270,   271,
     272,   272,   273,   274,   274,   275,   276,   276,   277,   277,
     278,   278,   279,   279,   280,   280,   281,   281,   282,   283,
     283,   283,   284,   284,   285,   285,   285,   286,   286,   286,
     286,   286,   286,   286,   287,   287,   288,   288,   289,   289,
     290,   290,   291,   291,   291,   292,   293,   293,   293,   293,
     293,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   295,   295,   295,   296,   296,   296,   296,
     296,   296,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   298,   298,   299,   299,   299,   299,   299,
     299,   300,   300,   301,   301,   301,   301,   302,   302,   303,
     303,   304,   304,   304,   304,   304,   304,   304,   305,   305,
     305,   305,   305,   305,   306,   306,   307,   307,   308,   308,
     308,   309,   309,   309,   310,   310,   310,   310,   310,   311,
     312,   313,   314,   314,   314,   314,   314,   314,   315,   315,
     315,   315,   315,   315,   316,   316,   317,   318,   319,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   322,   322,   322,   322,   322,   322,   322,   323,   324,
     324,   325,   325,   326,   327,   328,   329,   329,   329,   330,
     330,   330,   331,   331,   332,   332,   332,   333,   333,   333,
     333,   334,   334,   335,   335,   336,   337,   338,   338,   338,
     339,   339,   340,   340,   341,   341,   342,   342,   342,   343,
     343,   344,   344,   345,   345,   346,   346,   347,   347,   347,
     347,   348,   348,   348,   349,   349,   350,   351,   351,   352,
     353,   353,   353,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   355,   356,   356,   357,   357
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     3,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     0,     1,     3,     1,     4,     2,     2,     2,
       1,     0,     4,     1,     8,     5,     2,     0,     2,     5,
       7,     6,     1,     1,     4,     3,     0,     4,     2,     4,
       2,     3,     1,     2,     2,     5,     5,     2,     3,     2,
       8,     7,     6,     9,     7,     3,     0,     1,     3,     1,
       1,     3,     1,     1,     4,     4,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     4,
       3,     0,     5,     3,     0,     1,     0,     1,     2,     1,
       2,     2,     1,     1,     2,     5,     4,     6,     3,     4,
       4,     3,     4,     2,     0,     5,     1,     4,     4,     2,
       8,     5,     3,     0,     5,     1,     3,     3,     2,     2,
       6,     1,     1,     1,     3,     3,     3,     4,     6,     2,
       1,     1,     1,     1,     0,     3,     0,     3,     0,     9,
       1,     0,     1,     1,     0,     2,     2,     0,     4,     0,
       2,     0,     7,     0,     3,     0,     1,     3,     3,     1,
       1,     0,     0,     2,     2,     4,     0,     2,     2,     4,
       2,     4,     8,     0,     1,     3,     1,     0,     1,     3,
       1,     6,     1,     2,     2,     2,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     2,     2,     2,     2,     2,
       3,     4,     1,     3,     3,     3,     3,     3,     3,     3,
       4,     3,     3,     3,     3,     5,     6,     5,     6,     7,
       8,     3,     3,     4,     6,     3,     5,     4,     5,     4,
       5,     3,     3,     3,     3,     3,     3,     3,     4,     7,
       6,     8,     4,     6,     6,     0,     3,     0,     2,     5,
       0,     1,     1,     1,     2,     2,     2,     2,     1,     6,
       2,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     5,     1,
       3,     1,     3,     1,     5,     7,     5,     7,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     3,     2,     1,
       1,     1,     1,     3,     1,     1,     3,     1,     4,     2,
       5,     3,     5,     1,     3,     2,     1,     1,     3,     5,
       1,     0,     1,     5,     1,     0,     2,     2,     1,     1,
       0,     1,     0,     1,     2,     3,     5,     1,     3,     1,
       2,     2,     1,     0,     1,     0,     2,     1,     3,     3,
       4,     6,     8,     1,     2,     1,     2,     1,     2,     1,
       1,     1,     0,     1,     1,     0,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = SQL_HSQL_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == SQL_HSQL_EMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, result, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use SQL_HSQL_error or SQL_HSQL_UNDEF. */
#define YYERRCODE SQL_HSQL_UNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if HSQL_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined HSQL_LTYPE_IS_TRIVIAL && HSQL_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, hsql::SQLParserResult* result, yyscan_t scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (result);
  YY_USE (scanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, hsql::SQLParserResult* result, yyscan_t scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, result, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, hsql::SQLParserResult* result, yyscan_t scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !HSQL_DEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !HSQL_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, hsql::SQLParserResult* result, yyscan_t scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (result);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_IDENTIFIER: /* IDENTIFIER  */
#line 221 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2340 "bison_parser.cpp"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 221 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2346 "bison_parser.cpp"
        break;

    case YYSYMBOL_FLOATVAL: /* FLOATVAL  */
#line 208 "bison_parser.y"
            { }
#line 2352 "bison_parser.cpp"
        break;

    case YYSYMBOL_INTVAL: /* INTVAL  */
#line 208 "bison_parser.y"
            { }
#line 2358 "bison_parser.cpp"
        break;

    case YYSYMBOL_DOLLAR_PARAM: /* DOLLAR_PARAM  */
#line 208 "bison_parser.y"
            { }
#line 2364 "bison_parser.cpp"
        break;

    case YYSYMBOL_NAMED_PARAM: /* NAMED_PARAM  */
#line 221 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2370 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement_list: /* statement_list  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).stmt_vec)) {
    for (auto ptr : *(((*yyvaluep).stmt_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).stmt_vec));
}
#line 2383 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2389 "bison_parser.cpp"
        break;

    case YYSYMBOL_preparable_statement: /* preparable_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2395 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_hints: /* opt_hints  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2408 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint_list: /* hint_list  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2421 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint: /* hint  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2427 "bison_parser.cpp"
        break;

    case YYSYMBOL_transaction_statement: /* transaction_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).transaction_stmt)); }
#line 2433 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_statement: /* prepare_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).prep_stmt)); }
#line 2439 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_target_query: /* prepare_target_query  */
#line 221 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2445 "bison_parser.cpp"
        break;

    case YYSYMBOL_odbc_call_statement: /* odbc_call_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2451 "bison_parser.cpp"
        break;

    case YYSYMBOL_execute_statement: /* execute_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2457 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_statement: /* import_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).import_stmt)); }
#line 2463 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_type: /* file_type  */
#line 208 "bison_parser.y"
            { }
#line 2469 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_path: /* file_path  */
#line 221 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2475 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_import_export_options: /* opt_import_export_options  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2481 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_export_options: /* import_export_options  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2487 "bison_parser.cpp"
        break;

    case YYSYMBOL_csv_option: /* csv_option  */
#line 230 "bison_parser.y"
            {
  free(((*yyvaluep).csv_option_t)->second);
  delete (((*yyvaluep).csv_option_t));
}
#line 2496 "bison_parser.cpp"
        break;

    case YYSYMBOL_export_statement: /* export_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).export_stmt)); }
#line 2502 "bison_parser.cpp"
        break;

    case YYSYMBOL_show_statement: /* show_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).show_stmt)); }
#line 2508 "bison_parser.cpp"
        break;

    case YYSYMBOL_create_statement: /* create_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).create_stmt)); }
#line 2514 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_not_exists: /* opt_not_exists  */
#line 208 "bison_parser.y"
            { }
#line 2520 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem_commalist: /* table_elem_commalist  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).table_element_vec)) {
    for (auto ptr : *(((*yyvaluep).table_element_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_element_vec));
}
#line 2533 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem: /* table_elem  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).table_element_t)); }
#line 2539 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_def: /* column_def  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).column_t)); }
#line 2545 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_type: /* column_type  */
#line 208 "bison_parser.y"
            { }
#line 2551 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_time_precision: /* opt_time_precision  */
#line 208 "bison_parser.y"
            { }
#line 2557 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_decimal_specification: /* opt_decimal_specification  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).ival_pair)); }
#line 2563 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_column_constraints: /* opt_column_constraints  */
#line 234 "bison_parser.y"
            {
  // ColumnConstraints owns heap-allocated members (constraints, references).
  // The happy path transfers those pointers into a ColumnDefinition and then
  // deletes the wrapper; bison only reaches this destructor on parse error,
  // when the inner pointers are still owned by the wrapper and must be
  // released here to avoid leaks.
  if (((*yyvaluep).column_constraints_t)) {
    delete ((*yyvaluep).column_constraints_t)->constraints;
    if (((*yyvaluep).column_constraints_t)->references) {
      for (auto ptr : *(((*yyvaluep).column_constraints_t)->references)) {
        delete ptr;
      }
    }
    delete ((*yyvaluep).column_constraints_t)->references;
  }
  delete (((*yyvaluep).column_constraints_t));
}
#line 2585 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraints: /* column_constraints  */
#line 234 "bison_parser.y"
            {
  // ColumnConstraints owns heap-allocated members (constraints, references).
  // The happy path transfers those pointers into a ColumnDefinition and then
  // deletes the wrapper; bison only reaches this destructor on parse error,
  // when the inner pointers are still owned by the wrapper and must be
  // released here to avoid leaks.
  if (((*yyvaluep).column_constraints_t)) {
    delete ((*yyvaluep).column_constraints_t)->constraints;
    if (((*yyvaluep).column_constraints_t)->references) {
      for (auto ptr : *(((*yyvaluep).column_constraints_t)->references)) {
        delete ptr;
      }
    }
    delete ((*yyvaluep).column_constraints_t)->references;
  }
  delete (((*yyvaluep).column_constraints_t));
}
#line 2607 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraint: /* column_constraint  */
#line 208 "bison_parser.y"
            { }
#line 2613 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_constraint: /* table_constraint  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).table_constraint_t)); }
#line 2619 "bison_parser.cpp"
        break;

    case YYSYMBOL_references_spec: /* references_spec  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).references_spec_t)); }
#line 2625 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_statement: /* drop_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).drop_stmt)); }
#line 2631 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_exists: /* opt_exists  */
#line 208 "bison_parser.y"
            { }
#line 2637 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_statement: /* alter_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).alter_stmt)); }
#line 2643 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_action: /* alter_action  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).alter_action_t)); }
#line 2649 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_action: /* drop_action  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).drop_action_t)); }
#line 2655 "bison_parser.cpp"
        break;

    case YYSYMBOL_delete_statement: /* delete_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2661 "bison_parser.cpp"
        break;

    case YYSYMBOL_truncate_statement: /* truncate_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2667 "bison_parser.cpp"
        break;

    case YYSYMBOL_insert_statement: /* insert_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).insert_stmt)); }
#line 2673 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_column_list: /* opt_column_list  */
#line 213 "bison_parser.y"
            {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 2686 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_statement: /* update_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).update_stmt)); }
#line 2692 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause_commalist: /* update_clause_commalist  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).update_vec)) {
    for (auto ptr : *(((*yyvaluep).update_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).update_vec));
}
#line 2705 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause: /* update_clause  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).update_t)); }
#line 2711 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_statement: /* select_statement  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2717 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation: /* select_within_set_operation  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2723 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation_no_parentheses: /* select_within_set_operation_no_parentheses  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2729 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_with_paren: /* select_with_paren  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2735 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_no_paren: /* select_no_paren  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2741 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_operator: /* set_operator  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2747 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_type: /* set_type  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2753 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_all: /* opt_all  */
#line 208 "bison_parser.y"
            { }
#line 2759 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_start_with: /* opt_start_with  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2765 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_connect_by: /* opt_connect_by  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2771 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_clause: /* select_clause  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2777 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_distinct: /* opt_distinct  */
#line 208 "bison_parser.y"
            { }
#line 2783 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_list: /* select_list  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2796 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_from_clause: /* opt_from_clause  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2802 "bison_parser.cpp"
        break;

    case YYSYMBOL_from_clause: /* from_clause  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2808 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_where: /* opt_where  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2814 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_group: /* opt_group  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).group_t)); }
#line 2820 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_having: /* opt_having  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2826 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_within_group: /* opt_within_group  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 2839 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order: /* opt_order  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 2852 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_list: /* order_list  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 2865 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_desc: /* order_desc  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).order)); }
#line 2871 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order_type: /* opt_order_type  */
#line 208 "bison_parser.y"
            { }
#line 2877 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_null_ordering: /* opt_null_ordering  */
#line 208 "bison_parser.y"
            { }
#line 2883 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_top: /* opt_top  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 2889 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_limit: /* opt_limit  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 2895 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_list: /* expr_list  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2908 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_extended_literal_list: /* opt_extended_literal_list  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2921 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal_list: /* extended_literal_list  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2934 "bison_parser.cpp"
        break;

    case YYSYMBOL_casted_extended_literal: /* casted_extended_literal  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2940 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal: /* extended_literal  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2946 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_alias: /* expr_alias  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2952 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr: /* expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2958 "bison_parser.cpp"
        break;

    case YYSYMBOL_operand: /* operand  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2964 "bison_parser.cpp"
        break;

    case YYSYMBOL_scalar_expr: /* scalar_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2970 "bison_parser.cpp"
        break;

    case YYSYMBOL_unary_expr: /* unary_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2976 "bison_parser.cpp"
        break;

    case YYSYMBOL_binary_expr: /* binary_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2982 "bison_parser.cpp"
        break;

    case YYSYMBOL_logic_expr: /* logic_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2988 "bison_parser.cpp"
        break;

    case YYSYMBOL_in_expr: /* in_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2994 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_expr_list: /* row_expr_list  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3007 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_expr: /* case_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3013 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_list: /* case_list  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3019 "bison_parser.cpp"
        break;

    case YYSYMBOL_exists_expr: /* exists_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3025 "bison_parser.cpp"
        break;

    case YYSYMBOL_comp_expr: /* comp_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3031 "bison_parser.cpp"
        break;

    case YYSYMBOL_function_expr: /* function_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3037 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_window: /* opt_window  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).window_description)); }
#line 3043 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_partition: /* opt_partition  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3056 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_frame_clause: /* opt_frame_clause  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).frame_description)); }
#line 3062 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_type: /* frame_type  */
#line 208 "bison_parser.y"
            { }
#line 3068 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_bound: /* frame_bound  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).frame_bound)); }
#line 3074 "bison_parser.cpp"
        break;

    case YYSYMBOL_extract_expr: /* extract_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3080 "bison_parser.cpp"
        break;

    case YYSYMBOL_next_value_expr: /* next_value_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3086 "bison_parser.cpp"
        break;

    case YYSYMBOL_cast_expr: /* cast_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3092 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field: /* datetime_field  */
#line 208 "bison_parser.y"
            { }
#line 3098 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field_plural: /* datetime_field_plural  */
#line 208 "bison_parser.y"
            { }
#line 3104 "bison_parser.cpp"
        break;

    case YYSYMBOL_duration_field: /* duration_field  */
#line 208 "bison_parser.y"
            { }
#line 3110 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_expr: /* array_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3116 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_index: /* array_index  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3122 "bison_parser.cpp"
        break;

    case YYSYMBOL_between_expr: /* between_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3128 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_name: /* column_name  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3134 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonreserved_keyword: /* nonreserved_keyword  */
#line 221 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3140 "bison_parser.cpp"
        break;

    case YYSYMBOL_literal: /* literal  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3146 "bison_parser.cpp"
        break;

    case YYSYMBOL_string_literal: /* string_literal  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3152 "bison_parser.cpp"
        break;

    case YYSYMBOL_bool_literal: /* bool_literal  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3158 "bison_parser.cpp"
        break;

    case YYSYMBOL_num_literal: /* num_literal  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3164 "bison_parser.cpp"
        break;

    case YYSYMBOL_int_literal: /* int_literal  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3170 "bison_parser.cpp"
        break;

    case YYSYMBOL_null_literal: /* null_literal  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3176 "bison_parser.cpp"
        break;

    case YYSYMBOL_date_literal: /* date_literal  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3182 "bison_parser.cpp"
        break;

    case YYSYMBOL_interval_literal: /* interval_literal  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3188 "bison_parser.cpp"
        break;

    case YYSYMBOL_param_expr: /* param_expr  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3194 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref: /* table_ref  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3200 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_atomic: /* table_ref_atomic  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3206 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonjoin_table_ref_atomic: /* nonjoin_table_ref_atomic  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3212 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_value_row_list: /* table_value_row_list  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3225 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_commalist: /* table_ref_commalist  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).table_vec)) {
    for (auto ptr : *(((*yyvaluep).table_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_vec));
}
#line 3238 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name: /* table_ref_name  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3244 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name_no_alias: /* table_ref_name_no_alias  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3250 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_name: /* table_name  */
#line 209 "bison_parser.y"
            {
  free(((*yyvaluep).table_name).name);
  free(((*yyvaluep).table_name).schema);
}
#line 3259 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_index_name: /* opt_index_name  */
#line 221 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3265 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_alias: /* table_alias  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3271 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_table_alias: /* opt_table_alias  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3277 "bison_parser.cpp"
        break;

    case YYSYMBOL_alias: /* alias  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3283 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_alias: /* opt_alias  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3289 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause: /* opt_locking_clause  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3295 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause_list: /* opt_locking_clause_list  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3301 "bison_parser.cpp"
        break;

    case YYSYMBOL_locking_clause: /* locking_clause  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).locking_t)); }
#line 3307 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_lock_mode: /* row_lock_mode  */
#line 208 "bison_parser.y"
            { }
#line 3313 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_row_lock_policy: /* opt_row_lock_policy  */
#line 208 "bison_parser.y"
            { }
#line 3319 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_with_clause: /* opt_with_clause  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3332 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_clause: /* with_clause  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3345 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description_list: /* with_description_list  */
#line 222 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3358 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description: /* with_description  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).with_description_t)); }
#line 3364 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_clause: /* join_clause  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3370 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_join_type: /* opt_join_type  */
#line 208 "bison_parser.y"
            { }
#line 3376 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_condition: /* join_condition  */
#line 251 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3382 "bison_parser.cpp"
        break;

    case YYSYMBOL_ident_commalist: /* ident_commalist  */
#line 213 "bison_parser.y"
            {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 3395 "bison_parser.cpp"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (hsql::SQLParserResult* result, yyscan_t scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined HSQL_LTYPE_IS_TRIVIAL && HSQL_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = SQL_HSQL_EMPTY; /* Cause a token to be read.  */


/* User initialization code.  */
#line 108 "bison_parser.y"
{
  // Initialize
  yylloc.first_column = 0;
  yylloc.last_column = 0;
  yylloc.first_line = 0;
  yylloc.last_line = 0;
  yylloc.total_column = 0;
  yylloc.string_length = 0;
}

#line 3503 "bison_parser.cpp"

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == SQL_HSQL_EMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= SQL_YYEOF)
    {
      yychar = SQL_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == SQL_HSQL_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = SQL_HSQL_UNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = SQL_HSQL_EMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* input: statement_list opt_semicolon  */
#line 394 "bison_parser.y"
                                     {
  for (SQLStatement* stmt : *(yyvsp[-1].stmt_vec)) {
    // Transfers ownership of the statement.
    result->addStatement(stmt);
  }

  unsigned param_id = 0;
  for (void* param : yyloc.param_list) {
    if (param) {
      Expr* expr = (Expr*)param;
      if (expr->type == kExprParameter) {
        expr->ival = param_id++;
      }
      result->addParameter(expr);
    }
  }
    delete (yyvsp[-1].stmt_vec);
  }
#line 3733 "bison_parser.cpp"
    break;

  case 3: /* statement_list: statement  */
#line 414 "bison_parser.y"
                           {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyval.stmt_vec) = new std::vector<SQLStatement*>();
  (yyval.stmt_vec)->push_back((yyvsp[0].statement));
}
#line 3744 "bison_parser.cpp"
    break;

  case 4: /* statement_list: statement_list ';' statement  */
#line 420 "bison_parser.y"
                               {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyvsp[-2].stmt_vec)->push_back((yyvsp[0].statement));
  (yyval.stmt_vec) = (yyvsp[-2].stmt_vec);
}
#line 3755 "bison_parser.cpp"
    break;

  case 5: /* statement: prepare_statement opt_hints  */
#line 427 "bison_parser.y"
                                        {
  (yyval.statement) = (yyvsp[-1].prep_stmt);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3764 "bison_parser.cpp"
    break;

  case 6: /* statement: preparable_statement opt_hints  */
#line 431 "bison_parser.y"
                                 {
  (yyval.statement) = (yyvsp[-1].statement);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3773 "bison_parser.cpp"
    break;

  case 7: /* statement: show_statement  */
#line 435 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].show_stmt); }
#line 3779 "bison_parser.cpp"
    break;

  case 8: /* statement: import_statement  */
#line 436 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].import_stmt); }
#line 3785 "bison_parser.cpp"
    break;

  case 9: /* statement: export_statement  */
#line 437 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].export_stmt); }
#line 3791 "bison_parser.cpp"
    break;

  case 10: /* preparable_statement: select_statement  */
#line 439 "bison_parser.y"
                                        { (yyval.statement) = (yyvsp[0].select_stmt); }
#line 3797 "bison_parser.cpp"
    break;

  case 11: /* preparable_statement: create_statement  */
#line 440 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].create_stmt); }
#line 3803 "bison_parser.cpp"
    break;

  case 12: /* preparable_statement: insert_statement  */
#line 441 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].insert_stmt); }
#line 3809 "bison_parser.cpp"
    break;

  case 13: /* preparable_statement: delete_statement  */
#line 442 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3815 "bison_parser.cpp"
    break;

  case 14: /* preparable_statement: truncate_statement  */
#line 443 "bison_parser.y"
                     { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3821 "bison_parser.cpp"
    break;

  case 15: /* preparable_statement: update_statement  */
#line 444 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].update_stmt); }
#line 3827 "bison_parser.cpp"
    break;

  case 16: /* preparable_statement: drop_statement  */
#line 445 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].drop_stmt); }
#line 3833 "bison_parser.cpp"
    break;

  case 17: /* preparable_statement: alter_statement  */
#line 446 "bison_parser.y"
                  { (yyval.statement) = (yyvsp[0].alter_stmt); }
#line 3839 "bison_parser.cpp"
    break;

  case 18: /* preparable_statement: execute_statement  */
#line 447 "bison_parser.y"
                    { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 3845 "bison_parser.cpp"
    break;

  case 19: /* preparable_statement: odbc_call_statement  */
#line 448 "bison_parser.y"
                      { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 3851 "bison_parser.cpp"
    break;

  case 20: /* preparable_statement: transaction_statement  */
#line 449 "bison_parser.y"
                        { (yyval.statement) = (yyvsp[0].transaction_stmt); }
#line 3857 "bison_parser.cpp"
    break;

  case 21: /* opt_hints: WITH HINT '(' hint_list ')'  */
#line 455 "bison_parser.y"
                                        { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 3863 "bison_parser.cpp"
    break;

  case 22: /* opt_hints: %empty  */
#line 456 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 3869 "bison_parser.cpp"
    break;

  case 23: /* hint_list: hint  */
#line 458 "bison_parser.y"
                 {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 3878 "bison_parser.cpp"
    break;

  case 24: /* hint_list: hint_list ',' hint  */
#line 462 "bison_parser.y"
                     {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 3887 "bison_parser.cpp"
    break;

  case 25: /* hint: IDENTIFIER  */
#line 467 "bison_parser.y"
                  {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[0].sval);
}
#line 3896 "bison_parser.cpp"
    break;

  case 26: /* hint: IDENTIFIER '(' extended_literal_list ')'  */
#line 471 "bison_parser.y"
                                           {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[-3].sval);
  (yyval.expr)->exprList = (yyvsp[-1].expr_vec);
}
#line 3906 "bison_parser.cpp"
    break;

  case 27: /* transaction_statement: BEGIN opt_transaction_keyword  */
#line 481 "bison_parser.y"
                                                      { (yyval.transaction_stmt) = new TransactionStatement(kBeginTransaction); }
#line 3912 "bison_parser.cpp"
    break;

  case 28: /* transaction_statement: ROLLBACK opt_transaction_keyword  */
#line 482 "bison_parser.y"
                                   { (yyval.transaction_stmt) = new TransactionStatement(kRollbackTransaction); }
#line 3918 "bison_parser.cpp"
    break;

  case 29: /* transaction_statement: COMMIT opt_transaction_keyword  */
#line 483 "bison_parser.y"
                                 { (yyval.transaction_stmt) = new TransactionStatement(kCommitTransaction); }
#line 3924 "bison_parser.cpp"
    break;

  case 32: /* prepare_statement: PREPARE IDENTIFIER FROM prepare_target_query  */
#line 491 "bison_parser.y"
                                                                 {
  (yyval.prep_stmt) = new PrepareStatement();
  (yyval.prep_stmt)->name = (yyvsp[-2].sval);
  (yyval.prep_stmt)->query = (yyvsp[0].sval);
}
#line 3934 "bison_parser.cpp"
    break;

  case 34: /* odbc_call_statement: '{' opt_odbc_return CALL table_name '(' opt_extended_literal_list ')' '}'  */
#line 503 "bison_parser.y"
                                                                                                {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = qualifiedName((yyvsp[-4].table_name));
  (yyval.exec_stmt)->parameters = (yyvsp[-2].expr_vec);
}
#line 3944 "bison_parser.cpp"
    break;

  case 35: /* odbc_call_statement: '{' opt_odbc_return CALL table_name '}'  */
#line 508 "bison_parser.y"
                                          {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = qualifiedName((yyvsp[-1].table_name));
}
#line 3953 "bison_parser.cpp"
    break;

  case 38: /* execute_statement: EXECUTE IDENTIFIER  */
#line 515 "bison_parser.y"
                                       {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[0].sval);
}
#line 3962 "bison_parser.cpp"
    break;

  case 39: /* execute_statement: EXECUTE IDENTIFIER '(' opt_extended_literal_list ')'  */
#line 519 "bison_parser.y"
                                                       {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[-3].sval);
  (yyval.exec_stmt)->parameters = (yyvsp[-1].expr_vec);
}
#line 3972 "bison_parser.cpp"
    break;

  case 40: /* import_statement: IMPORT FROM file_type FILE file_path INTO table_name  */
#line 531 "bison_parser.y"
                                                                        {
  (yyval.import_stmt) = new ImportStatement((yyvsp[-4].import_type_t));
  (yyval.import_stmt)->filePath = (yyvsp[-2].sval);
  (yyval.import_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.import_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 3983 "bison_parser.cpp"
    break;

  case 41: /* import_statement: COPY table_name FROM file_path opt_import_export_options opt_where  */
#line 537 "bison_parser.y"
                                                                     {
  (yyval.import_stmt) = new ImportStatement((yyvsp[-1].import_export_option_t)->format);
  (yyval.import_stmt)->filePath = (yyvsp[-2].sval);
  (yyval.import_stmt)->schema = (yyvsp[-4].table_name).schema;
  (yyval.import_stmt)->tableName = (yyvsp[-4].table_name).name;
  (yyval.import_stmt)->whereClause = (yyvsp[0].expr);
  if ((yyvsp[-1].import_export_option_t)->encoding) {
    (yyval.import_stmt)->encoding = (yyvsp[-1].import_export_option_t)->encoding;
    (yyvsp[-1].import_export_option_t)->encoding = nullptr;
  }
  if ((yyvsp[-1].import_export_option_t)->csv_options) {
    (yyval.import_stmt)->csv_options = (yyvsp[-1].import_export_option_t)->csv_options;
    (yyvsp[-1].import_export_option_t)->csv_options = nullptr;
  }
  delete (yyvsp[-1].import_export_option_t);
}
#line 4004 "bison_parser.cpp"
    break;

  case 42: /* file_type: IDENTIFIER  */
#line 554 "bison_parser.y"
                       {
  if (strcasecmp((yyvsp[0].sval), "csv") == 0) {
    (yyval.import_type_t) = kImportCSV;
  } else if (strcasecmp((yyvsp[0].sval), "tbl") == 0) {
    (yyval.import_type_t) = kImportTbl;
  } else if (strcasecmp((yyvsp[0].sval), "binary") == 0 || strcasecmp((yyvsp[0].sval), "bin") == 0) {
    (yyval.import_type_t) = kImportBinary;
  } else {
    free((yyvsp[0].sval));
    yyerror(&yyloc, result, scanner, "File type is unknown.");
    YYERROR;
  }
  free((yyvsp[0].sval));
}
#line 4023 "bison_parser.cpp"
    break;

  case 43: /* file_path: STRING  */
#line 569 "bison_parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 4029 "bison_parser.cpp"
    break;

  case 44: /* opt_import_export_options: WITH '(' import_export_options ')'  */
#line 571 "bison_parser.y"
                                                               { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 4035 "bison_parser.cpp"
    break;

  case 45: /* opt_import_export_options: '(' import_export_options ')'  */
#line 572 "bison_parser.y"
                                { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 4041 "bison_parser.cpp"
    break;

  case 46: /* opt_import_export_options: %empty  */
#line 573 "bison_parser.y"
              { (yyval.import_export_option_t) = new ImportExportOptions{}; }
#line 4047 "bison_parser.cpp"
    break;

  case 47: /* import_export_options: import_export_options ',' FORMAT file_type  */
#line 575 "bison_parser.y"
                                                                   {
  if ((yyvsp[-3].import_export_option_t)->format != kImportAuto) {
    delete (yyvsp[-3].import_export_option_t);
    yyerror(&yyloc, result, scanner, "File type must only be provided once.");
    YYERROR;
  }
  if ((yyvsp[-3].import_export_option_t)->csv_options && (yyvsp[0].import_type_t) != kImportCSV && (yyvsp[0].import_type_t) != kImportAuto) {
    delete (yyvsp[-3].import_export_option_t);
    yyerror(&yyloc, result, scanner, "CSV options (DELIMITER, NULL, QUOTE) are only allowed for CSV files.");
    YYERROR;
  }
  (yyvsp[-3].import_export_option_t)->format = (yyvsp[0].import_type_t);
  (yyval.import_export_option_t) = (yyvsp[-3].import_export_option_t);
}
#line 4066 "bison_parser.cpp"
    break;

  case 48: /* import_export_options: FORMAT file_type  */
#line 589 "bison_parser.y"
                   {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->format = (yyvsp[0].import_type_t);
}
#line 4075 "bison_parser.cpp"
    break;

  case 49: /* import_export_options: import_export_options ',' ENCODING STRING  */
#line 593 "bison_parser.y"
                                            {
  if ((yyvsp[-3].import_export_option_t)->encoding) {
    delete (yyvsp[-3].import_export_option_t);
    free((yyvsp[0].sval));
    yyerror(&yyloc, result, scanner, "Encoding type must only be provided once.");
    YYERROR;
  }
  (yyvsp[-3].import_export_option_t)->encoding = (yyvsp[0].sval);
  (yyval.import_export_option_t) = (yyvsp[-3].import_export_option_t);
}
#line 4090 "bison_parser.cpp"
    break;

  case 50: /* import_export_options: ENCODING STRING  */
#line 603 "bison_parser.y"
                  {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->encoding = (yyvsp[0].sval);
}
#line 4099 "bison_parser.cpp"
    break;

  case 51: /* import_export_options: import_export_options ',' csv_option  */
#line 607 "bison_parser.y"
                                       {
  if ((yyvsp[-2].import_export_option_t)->format != kImportAuto && (yyvsp[-2].import_export_option_t)->format != kImportCSV) {
    delete (yyvsp[-2].import_export_option_t);
    free((yyvsp[0].csv_option_t)->second);
    delete (yyvsp[0].csv_option_t);
    yyerror(&yyloc, result, scanner, "CSV options (DELIMITER, NULL, QUOTE) are only allowed for CSV files.");
    YYERROR;
  }

  if ((yyvsp[-2].import_export_option_t)->csv_options == nullptr) {
    (yyvsp[-2].import_export_option_t)->csv_options = new CsvOptions{};
  }

  if (!(yyvsp[-2].import_export_option_t)->csv_options->accept_csv_option((yyvsp[0].csv_option_t))) {
    free((yyvsp[0].csv_option_t)->second);
    delete (yyvsp[0].csv_option_t);
    delete (yyvsp[-2].import_export_option_t);
    yyerror(&yyloc, result, scanner, "CSV options (DELIMITER, NULL, QUOTE) cannot be provided more than once.");
    YYERROR;
  }

  delete (yyvsp[0].csv_option_t);
  (yyval.import_export_option_t) = (yyvsp[-2].import_export_option_t);
}
#line 4128 "bison_parser.cpp"
    break;

  case 52: /* import_export_options: csv_option  */
#line 631 "bison_parser.y"
             {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->csv_options = new CsvOptions{};
  (yyval.import_export_option_t)->csv_options->accept_csv_option((yyvsp[0].csv_option_t));

  delete (yyvsp[0].csv_option_t);
}
#line 4140 "bison_parser.cpp"
    break;

  case 53: /* csv_option: IDENTIFIER STRING  */
#line 639 "bison_parser.y"
                               {
  if (strcasecmp((yyvsp[-1].sval), "DELIMITER") == 0) {
    (yyval.csv_option_t) = new std::pair<CsvOptionType, char*>(CsvOptionType::Delimiter, (yyvsp[0].sval));
  } else if (strcasecmp((yyvsp[-1].sval), "QUOTE") == 0) {
    (yyval.csv_option_t) = new std::pair<CsvOptionType, char*>(CsvOptionType::Quote, (yyvsp[0].sval));
  } else {
    free((yyvsp[-1].sval));
    free((yyvsp[0].sval));
    yyerror(&yyloc, result, scanner, "Unknown CSV option.");
    YYERROR;
  }
  free((yyvsp[-1].sval));
}
#line 4158 "bison_parser.cpp"
    break;

  case 54: /* csv_option: NULL STRING  */
#line 652 "bison_parser.y"
              { (yyval.csv_option_t) = new std::pair<CsvOptionType, char*>(CsvOptionType::Null, (yyvsp[0].sval)); }
#line 4164 "bison_parser.cpp"
    break;

  case 55: /* export_statement: COPY table_name TO file_path opt_import_export_options  */
#line 659 "bison_parser.y"
                                                                          {
  (yyval.export_stmt) = new ExportStatement((yyvsp[0].import_export_option_t)->format);
  (yyval.export_stmt)->filePath = (yyvsp[-1].sval);
  (yyval.export_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.export_stmt)->tableName = (yyvsp[-3].table_name).name;
  if ((yyvsp[0].import_export_option_t)->encoding) {
    (yyval.export_stmt)->encoding = (yyvsp[0].import_export_option_t)->encoding;
    (yyvsp[0].import_export_option_t)->encoding = nullptr;
  }
  if ((yyvsp[0].import_export_option_t)->csv_options) {
    (yyval.export_stmt)->csv_options = (yyvsp[0].import_export_option_t)->csv_options;
    (yyvsp[0].import_export_option_t)->csv_options = nullptr;
  }
  delete (yyvsp[0].import_export_option_t);
}
#line 4184 "bison_parser.cpp"
    break;

  case 56: /* export_statement: COPY select_with_paren TO file_path opt_import_export_options  */
#line 674 "bison_parser.y"
                                                                {
  (yyval.export_stmt) = new ExportStatement((yyvsp[0].import_export_option_t)->format);
  (yyval.export_stmt)->filePath = (yyvsp[-1].sval);
  (yyval.export_stmt)->select = (yyvsp[-3].select_stmt);
  if ((yyvsp[0].import_export_option_t)->encoding) {
    (yyval.export_stmt)->encoding = (yyvsp[0].import_export_option_t)->encoding;
    (yyvsp[0].import_export_option_t)->encoding = nullptr;
  }
  if ((yyvsp[0].import_export_option_t)->csv_options) {
    (yyval.export_stmt)->csv_options = (yyvsp[0].import_export_option_t)->csv_options;
    (yyvsp[0].import_export_option_t)->csv_options = nullptr;
  }
  delete (yyvsp[0].import_export_option_t);
}
#line 4203 "bison_parser.cpp"
    break;

  case 57: /* show_statement: SHOW TABLES  */
#line 694 "bison_parser.y"
                             { (yyval.show_stmt) = new ShowStatement(kShowTables); }
#line 4209 "bison_parser.cpp"
    break;

  case 58: /* show_statement: SHOW COLUMNS table_name  */
#line 695 "bison_parser.y"
                          {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4219 "bison_parser.cpp"
    break;

  case 59: /* show_statement: DESCRIBE table_name  */
#line 700 "bison_parser.y"
                      {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4229 "bison_parser.cpp"
    break;

  case 60: /* create_statement: CREATE TABLE opt_not_exists table_name FROM IDENTIFIER FILE file_path  */
#line 711 "bison_parser.y"
                                                                                         {
  (yyval.create_stmt) = new CreateStatement(kCreateTableFromTbl);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-5].bval);
  (yyval.create_stmt)->schema = (yyvsp[-4].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-4].table_name).name;
  if (strcasecmp((yyvsp[-2].sval), "tbl") != 0) {
    free((yyvsp[-2].sval));
    yyerror(&yyloc, result, scanner, "File type is unknown.");
    YYERROR;
  }
  free((yyvsp[-2].sval));
  (yyval.create_stmt)->filePath = (yyvsp[0].sval);
}
#line 4247 "bison_parser.cpp"
    break;

  case 61: /* create_statement: CREATE TABLE opt_not_exists table_name '(' table_elem_commalist ')'  */
#line 724 "bison_parser.y"
                                                                      {
  (yyval.create_stmt) = new CreateStatement(kCreateTable);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-4].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->setColumnDefsAndConstraints((yyvsp[-1].table_element_vec));
  delete (yyvsp[-1].table_element_vec);
  if (result->errorMsg()) {
    delete (yyval.create_stmt);
    YYERROR;
  }
}
#line 4264 "bison_parser.cpp"
    break;

  case 62: /* create_statement: CREATE TABLE opt_not_exists table_name AS select_statement  */
#line 736 "bison_parser.y"
                                                             {
  (yyval.create_stmt) = new CreateStatement(kCreateTable);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-3].bval);
  (yyval.create_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4276 "bison_parser.cpp"
    break;

  case 63: /* create_statement: CREATE INDEX opt_not_exists opt_index_name ON table_name '(' ident_commalist ')'  */
#line 743 "bison_parser.y"
                                                                                   {
  (yyval.create_stmt) = new CreateStatement(kCreateIndex);
  (yyval.create_stmt)->indexName = (yyvsp[-5].sval);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-6].bval);
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->indexColumns = (yyvsp[-1].str_vec);
}
#line 4288 "bison_parser.cpp"
    break;

  case 64: /* create_statement: CREATE VIEW opt_not_exists table_name opt_column_list AS select_statement  */
#line 750 "bison_parser.y"
                                                                            {
  (yyval.create_stmt) = new CreateStatement(kCreateView);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-4].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->viewColumns = (yyvsp[-2].str_vec);
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4301 "bison_parser.cpp"
    break;

  case 65: /* opt_not_exists: IF NOT EXISTS  */
#line 759 "bison_parser.y"
                               { (yyval.bval) = true; }
#line 4307 "bison_parser.cpp"
    break;

  case 66: /* opt_not_exists: %empty  */
#line 760 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4313 "bison_parser.cpp"
    break;

  case 67: /* table_elem_commalist: table_elem  */
#line 762 "bison_parser.y"
                                  {
  (yyval.table_element_vec) = new std::vector<TableElement*>();
  (yyval.table_element_vec)->push_back((yyvsp[0].table_element_t));
}
#line 4322 "bison_parser.cpp"
    break;

  case 68: /* table_elem_commalist: table_elem_commalist ',' table_elem  */
#line 766 "bison_parser.y"
                                      {
  (yyvsp[-2].table_element_vec)->push_back((yyvsp[0].table_element_t));
  (yyval.table_element_vec) = (yyvsp[-2].table_element_vec);
}
#line 4331 "bison_parser.cpp"
    break;

  case 69: /* table_elem: column_def  */
#line 771 "bison_parser.y"
                        { (yyval.table_element_t) = (yyvsp[0].column_t); }
#line 4337 "bison_parser.cpp"
    break;

  case 70: /* table_elem: table_constraint  */
#line 772 "bison_parser.y"
                   { (yyval.table_element_t) = (yyvsp[0].table_constraint_t); }
#line 4343 "bison_parser.cpp"
    break;

  case 71: /* column_def: IDENTIFIER column_type opt_column_constraints  */
#line 774 "bison_parser.y"
                                                           {
  (yyval.column_t) = new ColumnDefinition((yyvsp[-2].sval), (yyvsp[-1].column_type_t), (yyvsp[0].column_constraints_t)->constraints, (yyvsp[0].column_constraints_t)->references);
  if (!(yyval.column_t)->trySetNullableExplicit()) {
    yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{(yyvsp[-2].sval)}).c_str());
  }
  delete (yyvsp[0].column_constraints_t);
}
#line 4355 "bison_parser.cpp"
    break;

  case 72: /* column_type: BIGINT  */
#line 782 "bison_parser.y"
                     { (yyval.column_type_t) = ColumnType{DataType::BIGINT}; }
#line 4361 "bison_parser.cpp"
    break;

  case 73: /* column_type: BOOLEAN  */
#line 783 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::BOOLEAN}; }
#line 4367 "bison_parser.cpp"
    break;

  case 74: /* column_type: CHAR '(' INTVAL ')'  */
#line 784 "bison_parser.y"
                      { (yyval.column_type_t) = ColumnType{DataType::CHAR, (yyvsp[-1].ival)}; }
#line 4373 "bison_parser.cpp"
    break;

  case 75: /* column_type: CHARACTER_VARYING '(' INTVAL ')'  */
#line 785 "bison_parser.y"
                                   { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4379 "bison_parser.cpp"
    break;

  case 76: /* column_type: DATE  */
#line 786 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::DATE}; }
#line 4385 "bison_parser.cpp"
    break;

  case 77: /* column_type: DATETIME  */
#line 787 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4391 "bison_parser.cpp"
    break;

  case 78: /* column_type: DECIMAL opt_decimal_specification  */
#line 788 "bison_parser.y"
                                    {
  (yyval.column_type_t) = ColumnType{DataType::DECIMAL, 0, (yyvsp[0].ival_pair)->first, (yyvsp[0].ival_pair)->second};
  delete (yyvsp[0].ival_pair);
}
#line 4400 "bison_parser.cpp"
    break;

  case 79: /* column_type: DOUBLE  */
#line 792 "bison_parser.y"
         { (yyval.column_type_t) = ColumnType{DataType::DOUBLE}; }
#line 4406 "bison_parser.cpp"
    break;

  case 80: /* column_type: FLOAT  */
#line 793 "bison_parser.y"
        { (yyval.column_type_t) = ColumnType{DataType::FLOAT}; }
#line 4412 "bison_parser.cpp"
    break;

  case 81: /* column_type: INT  */
#line 794 "bison_parser.y"
      { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4418 "bison_parser.cpp"
    break;

  case 82: /* column_type: INTEGER  */
#line 795 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4424 "bison_parser.cpp"
    break;

  case 83: /* column_type: LONG  */
#line 796 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::LONG}; }
#line 4430 "bison_parser.cpp"
    break;

  case 84: /* column_type: REAL  */
#line 797 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::REAL}; }
#line 4436 "bison_parser.cpp"
    break;

  case 85: /* column_type: SMALLINT  */
#line 798 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::SMALLINT}; }
#line 4442 "bison_parser.cpp"
    break;

  case 86: /* column_type: TEXT  */
#line 799 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::TEXT}; }
#line 4448 "bison_parser.cpp"
    break;

  case 87: /* column_type: TIME opt_time_precision  */
#line 800 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::TIME, 0, (yyvsp[0].ival)}; }
#line 4454 "bison_parser.cpp"
    break;

  case 88: /* column_type: TIMESTAMP  */
#line 801 "bison_parser.y"
            { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4460 "bison_parser.cpp"
    break;

  case 89: /* column_type: VARCHAR '(' INTVAL ')'  */
#line 802 "bison_parser.y"
                         { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4466 "bison_parser.cpp"
    break;

  case 90: /* opt_time_precision: '(' INTVAL ')'  */
#line 804 "bison_parser.y"
                                    { (yyval.ival) = (yyvsp[-1].ival); }
#line 4472 "bison_parser.cpp"
    break;

  case 91: /* opt_time_precision: %empty  */
#line 805 "bison_parser.y"
              { (yyval.ival) = 0; }
#line 4478 "bison_parser.cpp"
    break;

  case 92: /* opt_decimal_specification: '(' INTVAL ',' INTVAL ')'  */
#line 807 "bison_parser.y"
                                                      { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-3].ival), (yyvsp[-1].ival)}; }
#line 4484 "bison_parser.cpp"
    break;

  case 93: /* opt_decimal_specification: '(' INTVAL ')'  */
#line 808 "bison_parser.y"
                 { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-1].ival), 0}; }
#line 4490 "bison_parser.cpp"
    break;

  case 94: /* opt_decimal_specification: %empty  */
#line 809 "bison_parser.y"
              { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{0, 0}; }
#line 4496 "bison_parser.cpp"
    break;

  case 95: /* opt_column_constraints: column_constraints  */
#line 811 "bison_parser.y"
                                            { (yyval.column_constraints_t) = (yyvsp[0].column_constraints_t); }
#line 4502 "bison_parser.cpp"
    break;

  case 96: /* opt_column_constraints: %empty  */
#line 812 "bison_parser.y"
              { (yyval.column_constraints_t) = new ColumnConstraints(); }
#line 4508 "bison_parser.cpp"
    break;

  case 97: /* column_constraints: column_constraint  */
#line 814 "bison_parser.y"
                                       {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
}
#line 4517 "bison_parser.cpp"
    break;

  case 98: /* column_constraints: column_constraints column_constraint  */
#line 818 "bison_parser.y"
                                       {
  (yyvsp[-1].column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4526 "bison_parser.cpp"
    break;

  case 99: /* column_constraints: references_spec  */
#line 822 "bison_parser.y"
                  {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyval.column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
}
#line 4536 "bison_parser.cpp"
    break;

  case 100: /* column_constraints: column_constraints references_spec  */
#line 827 "bison_parser.y"
                                     {
  // Multiple foreign keys for the same column could be possible, so we do not raise an error in that case.
  // Think of foreign keys referenced on multiple levels (returned item references sold item references items).
  (yyvsp[-1].column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyvsp[-1].column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4548 "bison_parser.cpp"
    break;

  case 101: /* column_constraint: PRIMARY KEY  */
#line 835 "bison_parser.y"
                                { (yyval.column_constraint_t) = ConstraintType::PrimaryKey; }
#line 4554 "bison_parser.cpp"
    break;

  case 102: /* column_constraint: UNIQUE  */
#line 836 "bison_parser.y"
         { (yyval.column_constraint_t) = ConstraintType::Unique; }
#line 4560 "bison_parser.cpp"
    break;

  case 103: /* column_constraint: NULL  */
#line 837 "bison_parser.y"
       { (yyval.column_constraint_t) = ConstraintType::Null; }
#line 4566 "bison_parser.cpp"
    break;

  case 104: /* column_constraint: NOT NULL  */
#line 838 "bison_parser.y"
           { (yyval.column_constraint_t) = ConstraintType::NotNull; }
#line 4572 "bison_parser.cpp"
    break;

  case 105: /* table_constraint: PRIMARY KEY '(' ident_commalist ')'  */
#line 840 "bison_parser.y"
                                                       { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::PrimaryKey, (yyvsp[-1].str_vec)); }
#line 4578 "bison_parser.cpp"
    break;

  case 106: /* table_constraint: UNIQUE '(' ident_commalist ')'  */
#line 841 "bison_parser.y"
                                 { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::Unique, (yyvsp[-1].str_vec)); }
#line 4584 "bison_parser.cpp"
    break;

  case 107: /* table_constraint: FOREIGN KEY '(' ident_commalist ')' references_spec  */
#line 842 "bison_parser.y"
                                                      { (yyval.table_constraint_t) = new ForeignKeyConstraint((yyvsp[-2].str_vec), (yyvsp[0].references_spec_t)); }
#line 4590 "bison_parser.cpp"
    break;

  case 108: /* references_spec: REFERENCES table_name opt_column_list  */
#line 844 "bison_parser.y"
                                                        { (yyval.references_spec_t) = new ReferencesSpecification((yyvsp[-1].table_name).schema, (yyvsp[-1].table_name).name, (yyvsp[0].str_vec)); }
#line 4596 "bison_parser.cpp"
    break;

  case 109: /* drop_statement: DROP TABLE opt_exists table_name  */
#line 852 "bison_parser.y"
                                                  {
  (yyval.drop_stmt) = new DropStatement(kDropTable);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4607 "bison_parser.cpp"
    break;

  case 110: /* drop_statement: DROP VIEW opt_exists table_name  */
#line 858 "bison_parser.y"
                                  {
  (yyval.drop_stmt) = new DropStatement(kDropView);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4618 "bison_parser.cpp"
    break;

  case 111: /* drop_statement: DEALLOCATE PREPARE IDENTIFIER  */
#line 864 "bison_parser.y"
                                {
  (yyval.drop_stmt) = new DropStatement(kDropPreparedStatement);
  (yyval.drop_stmt)->ifExists = false;
  (yyval.drop_stmt)->name = (yyvsp[0].sval);
}
#line 4628 "bison_parser.cpp"
    break;

  case 112: /* drop_statement: DROP INDEX opt_exists IDENTIFIER  */
#line 870 "bison_parser.y"
                                   {
  (yyval.drop_stmt) = new DropStatement(kDropIndex);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->indexName = (yyvsp[0].sval);
}
#line 4638 "bison_parser.cpp"
    break;

  case 113: /* opt_exists: IF EXISTS  */
#line 876 "bison_parser.y"
                       { (yyval.bval) = true; }
#line 4644 "bison_parser.cpp"
    break;

  case 114: /* opt_exists: %empty  */
#line 877 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4650 "bison_parser.cpp"
    break;

  case 115: /* alter_statement: ALTER TABLE opt_exists table_name alter_action  */
#line 884 "bison_parser.y"
                                                                 {
  (yyval.alter_stmt) = new AlterStatement((yyvsp[-1].table_name).name, (yyvsp[0].alter_action_t));
  (yyval.alter_stmt)->ifTableExists = (yyvsp[-2].bval);
  (yyval.alter_stmt)->schema = (yyvsp[-1].table_name).schema;
}
#line 4660 "bison_parser.cpp"
    break;

  case 116: /* alter_action: drop_action  */
#line 890 "bison_parser.y"
                           { (yyval.alter_action_t) = (yyvsp[0].drop_action_t); }
#line 4666 "bison_parser.cpp"
    break;

  case 117: /* drop_action: DROP COLUMN opt_exists IDENTIFIER  */
#line 892 "bison_parser.y"
                                                {
  (yyval.drop_action_t) = new DropColumnAction((yyvsp[0].sval));
  (yyval.drop_action_t)->ifExists = (yyvsp[-1].bval);
}
#line 4675 "bison_parser.cpp"
    break;

  case 118: /* delete_statement: DELETE FROM table_name opt_where  */
#line 902 "bison_parser.y"
                                                    {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[-1].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[-1].table_name).name;
  (yyval.delete_stmt)->expr = (yyvsp[0].expr);
}
#line 4686 "bison_parser.cpp"
    break;

  case 119: /* truncate_statement: TRUNCATE table_name  */
#line 909 "bison_parser.y"
                                         {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 4696 "bison_parser.cpp"
    break;

  case 120: /* insert_statement: INSERT INTO table_name opt_column_list VALUES '(' extended_literal_list ')'  */
#line 920 "bison_parser.y"
                                                                                               {
  (yyval.insert_stmt) = new InsertStatement(kInsertValues);
  (yyval.insert_stmt)->schema = (yyvsp[-5].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-5].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-4].str_vec);
  (yyval.insert_stmt)->values = (yyvsp[-1].expr_vec);
}
#line 4708 "bison_parser.cpp"
    break;

  case 121: /* insert_statement: INSERT INTO table_name opt_column_list select_no_paren  */
#line 927 "bison_parser.y"
                                                         {
  (yyval.insert_stmt) = new InsertStatement(kInsertSelect);
  (yyval.insert_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-1].str_vec);
  (yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4720 "bison_parser.cpp"
    break;

  case 122: /* opt_column_list: '(' ident_commalist ')'  */
#line 935 "bison_parser.y"
                                          { (yyval.str_vec) = (yyvsp[-1].str_vec); }
#line 4726 "bison_parser.cpp"
    break;

  case 123: /* opt_column_list: %empty  */
#line 936 "bison_parser.y"
              { (yyval.str_vec) = nullptr; }
#line 4732 "bison_parser.cpp"
    break;

  case 124: /* update_statement: UPDATE table_ref_name_no_alias SET update_clause_commalist opt_where  */
#line 943 "bison_parser.y"
                                                                                        {
  (yyval.update_stmt) = new UpdateStatement();
  (yyval.update_stmt)->table = (yyvsp[-3].table);
  (yyval.update_stmt)->updates = (yyvsp[-1].update_vec);
  (yyval.update_stmt)->where = (yyvsp[0].expr);
}
#line 4743 "bison_parser.cpp"
    break;

  case 125: /* update_clause_commalist: update_clause  */
#line 950 "bison_parser.y"
                                        {
  (yyval.update_vec) = new std::vector<UpdateClause*>();
  (yyval.update_vec)->push_back((yyvsp[0].update_t));
}
#line 4752 "bison_parser.cpp"
    break;

  case 126: /* update_clause_commalist: update_clause_commalist ',' update_clause  */
#line 954 "bison_parser.y"
                                            {
  (yyvsp[-2].update_vec)->push_back((yyvsp[0].update_t));
  (yyval.update_vec) = (yyvsp[-2].update_vec);
}
#line 4761 "bison_parser.cpp"
    break;

  case 127: /* update_clause: IDENTIFIER '=' expr  */
#line 959 "bison_parser.y"
                                    {
  (yyval.update_t) = new UpdateClause();
  (yyval.update_t)->column = (yyvsp[-2].sval);
  (yyval.update_t)->value = (yyvsp[0].expr);
}
#line 4771 "bison_parser.cpp"
    break;

  case 128: /* select_statement: opt_with_clause select_with_paren  */
#line 969 "bison_parser.y"
                                                     {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4780 "bison_parser.cpp"
    break;

  case 129: /* select_statement: opt_with_clause select_no_paren  */
#line 973 "bison_parser.y"
                                  {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4789 "bison_parser.cpp"
    break;

  case 130: /* select_statement: opt_with_clause select_with_paren set_operator select_within_set_operation opt_order opt_limit  */
#line 977 "bison_parser.y"
                                                                                                 {
  (yyval.select_stmt) = (yyvsp[-4].select_stmt);
  if ((yyval.select_stmt)->setOperations == nullptr) {
    (yyval.select_stmt)->setOperations = new std::vector<SetOperation*>();
  }
  (yyval.select_stmt)->setOperations->push_back((yyvsp[-3].set_operator_t));
  (yyval.select_stmt)->setOperations->back()->nestedSelectStatement = (yyvsp[-2].select_stmt);
  (yyval.select_stmt)->setOperations->back()->resultOrder = (yyvsp[-1].order_vec);
  (yyval.select_stmt)->setOperations->back()->resultLimit = (yyvsp[0].limit);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-5].with_description_vec);
}
#line 4805 "bison_parser.cpp"
    break;

  case 133: /* select_within_set_operation_no_parentheses: select_clause  */
#line 991 "bison_parser.y"
                                                           { (yyval.select_stmt) = (yyvsp[0].select_stmt); }
#line 4811 "bison_parser.cpp"
    break;

  case 134: /* select_within_set_operation_no_parentheses: select_clause set_operator select_within_set_operation  */
#line 992 "bison_parser.y"
                                                         {
  (yyval.select_stmt) = (yyvsp[-2].select_stmt);
  if ((yyval.select_stmt)->setOperations == nullptr) {
    (yyval.select_stmt)->setOperations = new std::vector<SetOperation*>();
  }
  (yyval.select_stmt)->setOperations->push_back((yyvsp[-1].set_operator_t));
  (yyval.select_stmt)->setOperations->back()->nestedSelectStatement = (yyvsp[0].select_stmt);
}
#line 4824 "bison_parser.cpp"
    break;

  case 135: /* select_with_paren: '(' select_no_paren ')'  */
#line 1001 "bison_parser.y"
                                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4830 "bison_parser.cpp"
    break;

  case 136: /* select_with_paren: '(' select_with_paren ')'  */
#line 1002 "bison_parser.y"
                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4836 "bison_parser.cpp"
    break;

  case 137: /* select_no_paren: select_clause opt_order opt_limit opt_locking_clause  */
#line 1004 "bison_parser.y"
                                                                       {
  (yyval.select_stmt) = (yyvsp[-3].select_stmt);
  (yyval.select_stmt)->order = (yyvsp[-2].order_vec);

  // Limit could have been set by TOP.
  if ((yyvsp[-1].limit)) {
    delete (yyval.select_stmt)->limit;
    (yyval.select_stmt)->limit = (yyvsp[-1].limit);
  }

  if ((yyvsp[0].locking_clause_vec)) {
    (yyval.select_stmt)->lockings = (yyvsp[0].locking_clause_vec);
  }
}
#line 4855 "bison_parser.cpp"
    break;

  case 138: /* select_no_paren: select_clause set_operator select_within_set_operation opt_order opt_limit opt_locking_clause  */
#line 1018 "bison_parser.y"
                                                                                                {
  (yyval.select_stmt) = (yyvsp[-5].select_stmt);
  if ((yyval.select_stmt)->setOperations == nullptr) {
    (yyval.select_stmt)->setOperations = new std::vector<SetOperation*>();
  }
  (yyval.select_stmt)->setOperations->push_back((yyvsp[-4].set_operator_t));
  (yyval.select_stmt)->setOperations->back()->nestedSelectStatement = (yyvsp[-3].select_stmt);
  (yyval.select_stmt)->setOperations->back()->resultOrder = (yyvsp[-2].order_vec);
  (yyval.select_stmt)->setOperations->back()->resultLimit = (yyvsp[-1].limit);
  (yyval.select_stmt)->lockings = (yyvsp[0].locking_clause_vec);
}
#line 4871 "bison_parser.cpp"
    break;

  case 139: /* set_operator: set_type opt_all  */
#line 1030 "bison_parser.y"
                                {
  (yyval.set_operator_t) = (yyvsp[-1].set_operator_t);
  (yyval.set_operator_t)->isAll = (yyvsp[0].bval);
}
#line 4880 "bison_parser.cpp"
    break;

  case 140: /* set_type: UNION  */
#line 1035 "bison_parser.y"
                 {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetUnion;
}
#line 4889 "bison_parser.cpp"
    break;

  case 141: /* set_type: INTERSECT  */
#line 1039 "bison_parser.y"
            {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetIntersect;
}
#line 4898 "bison_parser.cpp"
    break;

  case 142: /* set_type: EXCEPT  */
#line 1043 "bison_parser.y"
         {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetExcept;
}
#line 4907 "bison_parser.cpp"
    break;

  case 143: /* opt_all: ALL  */
#line 1048 "bison_parser.y"
              { (yyval.bval) = true; }
#line 4913 "bison_parser.cpp"
    break;

  case 144: /* opt_all: %empty  */
#line 1049 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4919 "bison_parser.cpp"
    break;

  case 145: /* opt_start_with: START WITH expr  */
#line 1053 "bison_parser.y"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 4925 "bison_parser.cpp"
    break;

  case 146: /* opt_start_with: %empty  */
#line 1054 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4931 "bison_parser.cpp"
    break;

  case 147: /* opt_connect_by: CONNECT BY expr  */
#line 1056 "bison_parser.y"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 4937 "bison_parser.cpp"
    break;

  case 148: /* opt_connect_by: %empty  */
#line 1057 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4943 "bison_parser.cpp"
    break;

  case 149: /* select_clause: SELECT opt_top opt_distinct select_list opt_from_clause opt_where opt_start_with opt_connect_by opt_group  */
#line 1059 "bison_parser.y"
                                                                                                                          {
  (yyval.select_stmt) = new SelectStatement();
  (yyval.select_stmt)->limit = (yyvsp[-7].limit);
  (yyval.select_stmt)->selectDistinct = (yyvsp[-6].bval);
  (yyval.select_stmt)->selectList = (yyvsp[-5].expr_vec);
  (yyval.select_stmt)->fromTable = (yyvsp[-4].table);
  (yyval.select_stmt)->whereClause = (yyvsp[-3].expr);
  (yyval.select_stmt)->startWith = (yyvsp[-2].expr);
  (yyval.select_stmt)->connectBy = (yyvsp[-1].expr);
  (yyval.select_stmt)->groupBy = (yyvsp[0].group_t);
}
#line 4959 "bison_parser.cpp"
    break;

  case 150: /* opt_distinct: DISTINCT  */
#line 1071 "bison_parser.y"
                        { (yyval.bval) = true; }
#line 4965 "bison_parser.cpp"
    break;

  case 151: /* opt_distinct: %empty  */
#line 1072 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4971 "bison_parser.cpp"
    break;

  case 153: /* opt_from_clause: from_clause  */
#line 1076 "bison_parser.y"
                              { (yyval.table) = (yyvsp[0].table); }
#line 4977 "bison_parser.cpp"
    break;

  case 154: /* opt_from_clause: %empty  */
#line 1077 "bison_parser.y"
              { (yyval.table) = nullptr; }
#line 4983 "bison_parser.cpp"
    break;

  case 155: /* from_clause: FROM table_ref  */
#line 1079 "bison_parser.y"
                             { (yyval.table) = (yyvsp[0].table); }
#line 4989 "bison_parser.cpp"
    break;

  case 156: /* opt_where: WHERE expr  */
#line 1081 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 4995 "bison_parser.cpp"
    break;

  case 157: /* opt_where: %empty  */
#line 1082 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 5001 "bison_parser.cpp"
    break;

  case 158: /* opt_group: GROUP BY expr_list opt_having  */
#line 1084 "bison_parser.y"
                                          {
  (yyval.group_t) = new GroupByDescription();
  (yyval.group_t)->columns = (yyvsp[-1].expr_vec);
  (yyval.group_t)->having = (yyvsp[0].expr);
}
#line 5011 "bison_parser.cpp"
    break;

  case 159: /* opt_group: %empty  */
#line 1089 "bison_parser.y"
              { (yyval.group_t) = nullptr; }
#line 5017 "bison_parser.cpp"
    break;

  case 160: /* opt_having: HAVING expr  */
#line 1091 "bison_parser.y"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 5023 "bison_parser.cpp"
    break;

  case 161: /* opt_having: %empty  */
#line 1092 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 5029 "bison_parser.cpp"
    break;

  case 162: /* opt_within_group: WITHIN GROUP '(' ORDER BY order_list ')'  */
#line 1097 "bison_parser.y"
                                                            { (yyval.order_vec) = (yyvsp[-1].order_vec); }
#line 5035 "bison_parser.cpp"
    break;

  case 163: /* opt_within_group: %empty  */
#line 1098 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 5041 "bison_parser.cpp"
    break;

  case 164: /* opt_order: ORDER BY order_list  */
#line 1100 "bison_parser.y"
                                { (yyval.order_vec) = (yyvsp[0].order_vec); }
#line 5047 "bison_parser.cpp"
    break;

  case 165: /* opt_order: %empty  */
#line 1101 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 5053 "bison_parser.cpp"
    break;

  case 166: /* order_list: order_desc  */
#line 1103 "bison_parser.y"
                        {
  (yyval.order_vec) = new std::vector<OrderDescription*>();
  (yyval.order_vec)->push_back((yyvsp[0].order));
}
#line 5062 "bison_parser.cpp"
    break;

  case 167: /* order_list: order_list ',' order_desc  */
#line 1107 "bison_parser.y"
                            {
  (yyvsp[-2].order_vec)->push_back((yyvsp[0].order));
  (yyval.order_vec) = (yyvsp[-2].order_vec);
}
#line 5071 "bison_parser.cpp"
    break;

  case 168: /* order_desc: expr opt_order_type opt_null_ordering  */
#line 1112 "bison_parser.y"
                                                   { (yyval.order) = new OrderDescription((yyvsp[-1].order_type), (yyvsp[-2].expr), (yyvsp[0].null_ordering_t)); }
#line 5077 "bison_parser.cpp"
    break;

  case 169: /* opt_order_type: ASC  */
#line 1114 "bison_parser.y"
                     { (yyval.order_type) = kOrderAsc; }
#line 5083 "bison_parser.cpp"
    break;

  case 170: /* opt_order_type: DESC  */
#line 1115 "bison_parser.y"
       { (yyval.order_type) = kOrderDesc; }
#line 5089 "bison_parser.cpp"
    break;

  case 171: /* opt_order_type: %empty  */
#line 1116 "bison_parser.y"
              { (yyval.order_type) = kOrderAsc; }
#line 5095 "bison_parser.cpp"
    break;

  case 172: /* opt_null_ordering: %empty  */
#line 1118 "bison_parser.y"
                                { (yyval.null_ordering_t) = NullOrdering::Undefined; }
#line 5101 "bison_parser.cpp"
    break;

  case 173: /* opt_null_ordering: IDENTIFIER IDENTIFIER  */
#line 1119 "bison_parser.y"
                        {
  auto null_ordering = NullOrdering::Undefined;
  if (strcasecmp((yyvsp[-1].sval), "nulls") == 0) {
    if (strcasecmp((yyvsp[0].sval), "first") == 0) {
      null_ordering = NullOrdering::First;
    } else if (strcasecmp((yyvsp[0].sval), "last") == 0) {
      null_ordering = NullOrdering::Last;
    }
  }
  free((yyvsp[-1].sval));
  free((yyvsp[0].sval));

  if (null_ordering == NullOrdering::Undefined) {
    yyerror(&yyloc, result, scanner, "Expected NULLS FIRST or NULLS LAST ordering.");
    YYERROR;
  }

  (yyval.null_ordering_t) = null_ordering;
}
#line 5125 "bison_parser.cpp"
    break;

  case 174: /* opt_top: TOP int_literal  */
#line 1145 "bison_parser.y"
                          { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 5131 "bison_parser.cpp"
    break;

  case 175: /* opt_top: TOP '(' expr ')'  */
#line 1146 "bison_parser.y"
                   { (yyval.limit) = new LimitDescription((yyvsp[-1].expr), nullptr); }
#line 5137 "bison_parser.cpp"
    break;

  case 176: /* opt_top: %empty  */
#line 1147 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 5143 "bison_parser.cpp"
    break;

  case 177: /* opt_limit: LIMIT expr  */
#line 1149 "bison_parser.y"
                       { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 5149 "bison_parser.cpp"
    break;

  case 178: /* opt_limit: OFFSET expr  */
#line 1150 "bison_parser.y"
              { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 5155 "bison_parser.cpp"
    break;

  case 179: /* opt_limit: LIMIT expr OFFSET expr  */
#line 1151 "bison_parser.y"
                         { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 5161 "bison_parser.cpp"
    break;

  case 180: /* opt_limit: LIMIT ALL  */
#line 1152 "bison_parser.y"
            { (yyval.limit) = new LimitDescription(nullptr, nullptr); }
#line 5167 "bison_parser.cpp"
    break;

  case 181: /* opt_limit: LIMIT ALL OFFSET expr  */
#line 1153 "bison_parser.y"
                        { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 5173 "bison_parser.cpp"
    break;

  case 182: /* opt_limit: OFFSET expr ROWS FETCH NEXT expr ROWS ONLY  */
#line 1154 "bison_parser.y"
                                             { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[-6].expr)); }
#line 5179 "bison_parser.cpp"
    break;

  case 183: /* opt_limit: %empty  */
#line 1155 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 5185 "bison_parser.cpp"
    break;

  case 184: /* expr_list: expr_alias  */
#line 1160 "bison_parser.y"
                       {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5194 "bison_parser.cpp"
    break;

  case 185: /* expr_list: expr_list ',' expr_alias  */
#line 1164 "bison_parser.y"
                           {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5203 "bison_parser.cpp"
    break;

  case 186: /* opt_extended_literal_list: extended_literal_list  */
#line 1170 "bison_parser.y"
                                                  { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5209 "bison_parser.cpp"
    break;

  case 187: /* opt_extended_literal_list: %empty  */
#line 1171 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5215 "bison_parser.cpp"
    break;

  case 188: /* extended_literal_list: casted_extended_literal  */
#line 1173 "bison_parser.y"
                                                {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5224 "bison_parser.cpp"
    break;

  case 189: /* extended_literal_list: extended_literal_list ',' casted_extended_literal  */
#line 1177 "bison_parser.y"
                                                    {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5233 "bison_parser.cpp"
    break;

  case 191: /* casted_extended_literal: CAST '(' extended_literal AS column_type ')'  */
#line 1182 "bison_parser.y"
                                                                                          {
  (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t));
}
#line 5241 "bison_parser.cpp"
    break;

  case 192: /* extended_literal: literal  */
#line 1186 "bison_parser.y"
                           { (yyval.expr) = (yyvsp[0].expr); }
#line 5247 "bison_parser.cpp"
    break;

  case 193: /* extended_literal: '-' num_literal  */
#line 1187 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5253 "bison_parser.cpp"
    break;

  case 194: /* extended_literal: '-' interval_literal  */
#line 1188 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5259 "bison_parser.cpp"
    break;

  case 195: /* expr_alias: expr opt_alias  */
#line 1190 "bison_parser.y"
                            {
  (yyval.expr) = (yyvsp[-1].expr);
  if ((yyvsp[0].alias_t)) {
    (yyval.expr)->alias = (yyvsp[0].alias_t)->name;
    (yyvsp[0].alias_t)->name = nullptr;
    delete (yyvsp[0].alias_t);
  }
}
#line 5272 "bison_parser.cpp"
    break;

  case 201: /* operand: '(' expr ')'  */
#line 1201 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[-1].expr); }
#line 5278 "bison_parser.cpp"
    break;

  case 212: /* operand: '(' select_no_paren ')'  */
#line 1203 "bison_parser.y"
                                         {
  (yyval.expr) = Expr::makeSelect((yyvsp[-1].select_stmt));
}
#line 5286 "bison_parser.cpp"
    break;

  case 215: /* scalar_expr: column_name OUTERJOIN  */
#line 1211 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpUnary(kOpOuterJoin, (yyvsp[-1].expr)); }
#line 5292 "bison_parser.cpp"
    break;

  case 216: /* unary_expr: '-' operand  */
#line 1213 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5298 "bison_parser.cpp"
    break;

  case 217: /* unary_expr: PRIOR operand  */
#line 1215 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpUnary(kOpPrior, (yyvsp[0].expr)); }
#line 5304 "bison_parser.cpp"
    break;

  case 218: /* unary_expr: NOT operand  */
#line 1216 "bison_parser.y"
              { (yyval.expr) = Expr::makeOpUnary(kOpNot, (yyvsp[0].expr)); }
#line 5310 "bison_parser.cpp"
    break;

  case 219: /* unary_expr: operand ISNULL  */
#line 1217 "bison_parser.y"
                 { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-1].expr)); }
#line 5316 "bison_parser.cpp"
    break;

  case 220: /* unary_expr: operand IS NULL  */
#line 1218 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-2].expr)); }
#line 5322 "bison_parser.cpp"
    break;

  case 221: /* unary_expr: operand IS NOT NULL  */
#line 1219 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeOpUnary(kOpIsNull, (yyvsp[-3].expr))); }
#line 5328 "bison_parser.cpp"
    break;

  case 223: /* binary_expr: operand '-' operand  */
#line 1221 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpMinus, (yyvsp[0].expr)); }
#line 5334 "bison_parser.cpp"
    break;

  case 224: /* binary_expr: operand '+' operand  */
#line 1222 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPlus, (yyvsp[0].expr)); }
#line 5340 "bison_parser.cpp"
    break;

  case 225: /* binary_expr: operand '/' operand  */
#line 1223 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpSlash, (yyvsp[0].expr)); }
#line 5346 "bison_parser.cpp"
    break;

  case 226: /* binary_expr: operand '*' operand  */
#line 1224 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAsterisk, (yyvsp[0].expr)); }
#line 5352 "bison_parser.cpp"
    break;

  case 227: /* binary_expr: operand '%' operand  */
#line 1225 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPercentage, (yyvsp[0].expr)); }
#line 5358 "bison_parser.cpp"
    break;

  case 228: /* binary_expr: operand '^' operand  */
#line 1226 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpCaret, (yyvsp[0].expr)); }
#line 5364 "bison_parser.cpp"
    break;

  case 229: /* binary_expr: operand LIKE operand  */
#line 1227 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLike, (yyvsp[0].expr)); }
#line 5370 "bison_parser.cpp"
    break;

  case 230: /* binary_expr: operand NOT LIKE operand  */
#line 1228 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-3].expr), kOpNotLike, (yyvsp[0].expr)); }
#line 5376 "bison_parser.cpp"
    break;

  case 231: /* binary_expr: operand ILIKE operand  */
#line 1229 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpILike, (yyvsp[0].expr)); }
#line 5382 "bison_parser.cpp"
    break;

  case 232: /* binary_expr: operand CONCAT operand  */
#line 1230 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpConcat, (yyvsp[0].expr)); }
#line 5388 "bison_parser.cpp"
    break;

  case 233: /* logic_expr: expr AND expr  */
#line 1232 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAnd, (yyvsp[0].expr)); }
#line 5394 "bison_parser.cpp"
    break;

  case 234: /* logic_expr: expr OR expr  */
#line 1233 "bison_parser.y"
               { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpOr, (yyvsp[0].expr)); }
#line 5400 "bison_parser.cpp"
    break;

  case 235: /* in_expr: operand IN '(' expr_list ')'  */
#line 1235 "bison_parser.y"
                                       { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].expr_vec)); }
#line 5406 "bison_parser.cpp"
    break;

  case 236: /* in_expr: operand NOT IN '(' expr_list ')'  */
#line 1236 "bison_parser.y"
                                   { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].expr_vec))); }
#line 5412 "bison_parser.cpp"
    break;

  case 237: /* in_expr: operand IN '(' select_no_paren ')'  */
#line 1237 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].select_stmt)); }
#line 5418 "bison_parser.cpp"
    break;

  case 238: /* in_expr: operand NOT IN '(' select_no_paren ')'  */
#line 1238 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].select_stmt))); }
#line 5424 "bison_parser.cpp"
    break;

  case 239: /* in_expr: '(' row_expr_list ')' IN '(' select_no_paren ')'  */
#line 1242 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeInOperator(Expr::makeArray((yyvsp[-5].expr_vec)), (yyvsp[-1].select_stmt)); }
#line 5430 "bison_parser.cpp"
    break;

  case 240: /* in_expr: '(' row_expr_list ')' NOT IN '(' select_no_paren ')'  */
#line 1243 "bison_parser.y"
                                                       { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator(Expr::makeArray((yyvsp[-6].expr_vec)), (yyvsp[-1].select_stmt))); }
#line 5436 "bison_parser.cpp"
    break;

  case 241: /* row_expr_list: expr_alias ',' expr_alias  */
#line 1247 "bison_parser.y"
                                          {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[-2].expr));
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5446 "bison_parser.cpp"
    break;

  case 242: /* row_expr_list: row_expr_list ',' expr_alias  */
#line 1252 "bison_parser.y"
                               {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5455 "bison_parser.cpp"
    break;

  case 243: /* case_expr: CASE expr case_list END  */
#line 1259 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-2].expr), (yyvsp[-1].expr), nullptr); }
#line 5461 "bison_parser.cpp"
    break;

  case 244: /* case_expr: CASE expr case_list ELSE expr END  */
#line 1260 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-4].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5467 "bison_parser.cpp"
    break;

  case 245: /* case_expr: CASE case_list END  */
#line 1261 "bison_parser.y"
                     { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-1].expr), nullptr); }
#line 5473 "bison_parser.cpp"
    break;

  case 246: /* case_expr: CASE case_list ELSE expr END  */
#line 1262 "bison_parser.y"
                               { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5479 "bison_parser.cpp"
    break;

  case 247: /* case_list: WHEN expr THEN expr  */
#line 1264 "bison_parser.y"
                                { (yyval.expr) = Expr::makeCaseList(Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5485 "bison_parser.cpp"
    break;

  case 248: /* case_list: case_list WHEN expr THEN expr  */
#line 1265 "bison_parser.y"
                                { (yyval.expr) = Expr::caseListAppend((yyvsp[-4].expr), Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5491 "bison_parser.cpp"
    break;

  case 249: /* exists_expr: EXISTS '(' select_no_paren ')'  */
#line 1267 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeExists((yyvsp[-1].select_stmt)); }
#line 5497 "bison_parser.cpp"
    break;

  case 250: /* exists_expr: NOT EXISTS '(' select_no_paren ')'  */
#line 1268 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeExists((yyvsp[-1].select_stmt))); }
#line 5503 "bison_parser.cpp"
    break;

  case 251: /* comp_expr: operand '=' operand  */
#line 1270 "bison_parser.y"
                                { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5509 "bison_parser.cpp"
    break;

  case 252: /* comp_expr: operand EQUALS operand  */
#line 1271 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5515 "bison_parser.cpp"
    break;

  case 253: /* comp_expr: operand NOTEQUALS operand  */
#line 1272 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpNotEquals, (yyvsp[0].expr)); }
#line 5521 "bison_parser.cpp"
    break;

  case 254: /* comp_expr: operand '<' operand  */
#line 1273 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLess, (yyvsp[0].expr)); }
#line 5527 "bison_parser.cpp"
    break;

  case 255: /* comp_expr: operand '>' operand  */
#line 1274 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreater, (yyvsp[0].expr)); }
#line 5533 "bison_parser.cpp"
    break;

  case 256: /* comp_expr: operand LESSEQ operand  */
#line 1275 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLessEq, (yyvsp[0].expr)); }
#line 5539 "bison_parser.cpp"
    break;

  case 257: /* comp_expr: operand GREATEREQ operand  */
#line 1276 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreaterEq, (yyvsp[0].expr)); }
#line 5545 "bison_parser.cpp"
    break;

  case 258: /* function_expr: IDENTIFIER '(' ')' opt_window  */
#line 1280 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 5551 "bison_parser.cpp"
    break;

  case 259: /* function_expr: IDENTIFIER '(' opt_distinct expr_list ')' opt_within_group opt_window  */
#line 1281 "bison_parser.y"
                                                                        {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-6].sval), (yyvsp[-3].expr_vec), (yyvsp[-4].bval), (yyvsp[0].window_description));
  (yyval.expr)->withinGroupOrder = (yyvsp[-1].order_vec);
}
#line 5560 "bison_parser.cpp"
    break;

  case 260: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' ')' opt_window  */
#line 1285 "bison_parser.y"
                                               {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), (yyvsp[-5].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description));
}
#line 5568 "bison_parser.cpp"
    break;

  case 261: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' opt_distinct expr_list ')' opt_window  */
#line 1288 "bison_parser.y"
                                                                      {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-7].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description));
}
#line 5576 "bison_parser.cpp"
    break;

  case 262: /* function_expr: nonreserved_keyword '(' ')' opt_window  */
#line 1293 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 5582 "bison_parser.cpp"
    break;

  case 263: /* function_expr: nonreserved_keyword '(' opt_distinct expr_list ')' opt_window  */
#line 1294 "bison_parser.y"
                                                                { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description)); }
#line 5588 "bison_parser.cpp"
    break;

  case 264: /* opt_window: OVER '(' opt_partition opt_order opt_frame_clause ')'  */
#line 1298 "bison_parser.y"
                                                                   { (yyval.window_description) = new WindowDescription((yyvsp[-3].expr_vec), (yyvsp[-2].order_vec), (yyvsp[-1].frame_description)); }
#line 5594 "bison_parser.cpp"
    break;

  case 265: /* opt_window: %empty  */
#line 1299 "bison_parser.y"
              { (yyval.window_description) = nullptr; }
#line 5600 "bison_parser.cpp"
    break;

  case 266: /* opt_partition: PARTITION BY expr_list  */
#line 1301 "bison_parser.y"
                                       { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5606 "bison_parser.cpp"
    break;

  case 267: /* opt_partition: %empty  */
#line 1302 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5612 "bison_parser.cpp"
    break;

  case 268: /* opt_frame_clause: frame_type frame_bound  */
#line 1307 "bison_parser.y"
                                          { (yyval.frame_description) = new FrameDescription{(yyvsp[-1].frame_type), (yyvsp[0].frame_bound), new FrameBound{0, kCurrentRow, false}}; }
#line 5618 "bison_parser.cpp"
    break;

  case 269: /* opt_frame_clause: frame_type BETWEEN frame_bound AND frame_bound  */
#line 1308 "bison_parser.y"
                                                 { (yyval.frame_description) = new FrameDescription{(yyvsp[-4].frame_type), (yyvsp[-2].frame_bound), (yyvsp[0].frame_bound)}; }
#line 5624 "bison_parser.cpp"
    break;

  case 270: /* opt_frame_clause: %empty  */
#line 1309 "bison_parser.y"
              {
  (yyval.frame_description) = new FrameDescription{kRange, new FrameBound{0, kPreceding, true}, new FrameBound{0, kCurrentRow, false}};
}
#line 5632 "bison_parser.cpp"
    break;

  case 271: /* frame_type: RANGE  */
#line 1313 "bison_parser.y"
                   { (yyval.frame_type) = kRange; }
#line 5638 "bison_parser.cpp"
    break;

  case 272: /* frame_type: ROWS  */
#line 1314 "bison_parser.y"
       { (yyval.frame_type) = kRows; }
#line 5644 "bison_parser.cpp"
    break;

  case 273: /* frame_type: GROUPS  */
#line 1315 "bison_parser.y"
         { (yyval.frame_type) = kGroups; }
#line 5650 "bison_parser.cpp"
    break;

  case 274: /* frame_bound: UNBOUNDED PRECEDING  */
#line 1317 "bison_parser.y"
                                  { (yyval.frame_bound) = new FrameBound{0, kPreceding, true}; }
#line 5656 "bison_parser.cpp"
    break;

  case 275: /* frame_bound: INTVAL PRECEDING  */
#line 1318 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kPreceding, false}; }
#line 5662 "bison_parser.cpp"
    break;

  case 276: /* frame_bound: UNBOUNDED FOLLOWING  */
#line 1319 "bison_parser.y"
                      { (yyval.frame_bound) = new FrameBound{0, kFollowing, true}; }
#line 5668 "bison_parser.cpp"
    break;

  case 277: /* frame_bound: INTVAL FOLLOWING  */
#line 1320 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kFollowing, false}; }
#line 5674 "bison_parser.cpp"
    break;

  case 278: /* frame_bound: CURRENT_ROW  */
#line 1321 "bison_parser.y"
              { (yyval.frame_bound) = new FrameBound{0, kCurrentRow, false}; }
#line 5680 "bison_parser.cpp"
    break;

  case 279: /* extract_expr: EXTRACT '(' datetime_field FROM expr ')'  */
#line 1323 "bison_parser.y"
                                                        { (yyval.expr) = Expr::makeExtract((yyvsp[-3].datetime_field), (yyvsp[-1].expr)); }
#line 5686 "bison_parser.cpp"
    break;

  case 280: /* next_value_expr: NEXT_VALUE_FOR table_name  */
#line 1328 "bison_parser.y"
                                            {
  auto args = new std::vector<Expr*>();
  args->push_back(Expr::makeColumnRef(qualifiedName((yyvsp[0].table_name))));
  (yyval.expr) = Expr::makeFunctionRef(strdup("NEXT VALUE FOR"), args, false, nullptr);
}
#line 5696 "bison_parser.cpp"
    break;

  case 281: /* cast_expr: CAST '(' expr AS column_type ')'  */
#line 1334 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t)); }
#line 5702 "bison_parser.cpp"
    break;

  case 282: /* datetime_field: SECOND  */
#line 1336 "bison_parser.y"
                        { (yyval.datetime_field) = kDatetimeSecond; }
#line 5708 "bison_parser.cpp"
    break;

  case 283: /* datetime_field: MINUTE  */
#line 1337 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMinute; }
#line 5714 "bison_parser.cpp"
    break;

  case 284: /* datetime_field: HOUR  */
#line 1338 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeHour; }
#line 5720 "bison_parser.cpp"
    break;

  case 285: /* datetime_field: DAY  */
#line 1339 "bison_parser.y"
      { (yyval.datetime_field) = kDatetimeDay; }
#line 5726 "bison_parser.cpp"
    break;

  case 286: /* datetime_field: MONTH  */
#line 1340 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeMonth; }
#line 5732 "bison_parser.cpp"
    break;

  case 287: /* datetime_field: YEAR  */
#line 1341 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeYear; }
#line 5738 "bison_parser.cpp"
    break;

  case 288: /* datetime_field_plural: SECONDS  */
#line 1343 "bison_parser.y"
                                { (yyval.datetime_field) = kDatetimeSecond; }
#line 5744 "bison_parser.cpp"
    break;

  case 289: /* datetime_field_plural: MINUTES  */
#line 1344 "bison_parser.y"
          { (yyval.datetime_field) = kDatetimeMinute; }
#line 5750 "bison_parser.cpp"
    break;

  case 290: /* datetime_field_plural: HOURS  */
#line 1345 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeHour; }
#line 5756 "bison_parser.cpp"
    break;

  case 291: /* datetime_field_plural: DAYS  */
#line 1346 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeDay; }
#line 5762 "bison_parser.cpp"
    break;

  case 292: /* datetime_field_plural: MONTHS  */
#line 1347 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMonth; }
#line 5768 "bison_parser.cpp"
    break;

  case 293: /* datetime_field_plural: YEARS  */
#line 1348 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeYear; }
#line 5774 "bison_parser.cpp"
    break;

  case 296: /* array_expr: ARRAY '[' expr_list ']'  */
#line 1352 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeArray((yyvsp[-1].expr_vec)); }
#line 5780 "bison_parser.cpp"
    break;

  case 297: /* array_index: operand '[' int_literal ']'  */
#line 1358 "bison_parser.y"
                                          {
  (yyval.expr) = Expr::makeArrayIndex((yyvsp[-3].expr), (yyvsp[-1].expr)->ival);
  delete (yyvsp[-1].expr);
}
#line 5789 "bison_parser.cpp"
    break;

  case 298: /* between_expr: operand BETWEEN operand AND operand  */
#line 1363 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeBetween((yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 5795 "bison_parser.cpp"
    break;

  case 299: /* column_name: IDENTIFIER  */
#line 1365 "bison_parser.y"
                         { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 5801 "bison_parser.cpp"
    break;

  case 300: /* column_name: IDENTIFIER '.' IDENTIFIER  */
#line 1366 "bison_parser.y"
                            { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 5807 "bison_parser.cpp"
    break;

  case 301: /* column_name: '*'  */
#line 1367 "bison_parser.y"
      { (yyval.expr) = Expr::makeStar(); }
#line 5813 "bison_parser.cpp"
    break;

  case 302: /* column_name: IDENTIFIER '.' '*'  */
#line 1368 "bison_parser.y"
                     { (yyval.expr) = Expr::makeStar((yyvsp[-2].sval)); }
#line 5819 "bison_parser.cpp"
    break;

  case 303: /* column_name: nonreserved_keyword  */
#line 1369 "bison_parser.y"
                      { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 5825 "bison_parser.cpp"
    break;

  case 304: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1375 "bison_parser.y"
                                           {
  std::string combined(std::string((yyvsp[-4].sval)) + "." + (yyvsp[-2].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeColumnRef(strdup(combined.c_str()), (yyvsp[0].sval));
}
#line 5836 "bison_parser.cpp"
    break;

  case 305: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1381 "bison_parser.y"
                                                          {
  std::string combined(std::string((yyvsp[-6].sval)) + "." + (yyvsp[-4].sval) + "." + (yyvsp[-2].sval));
  free((yyvsp[-6].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeColumnRef(strdup(combined.c_str()), (yyvsp[0].sval));
}
#line 5848 "bison_parser.cpp"
    break;

  case 306: /* column_name: IDENTIFIER '.' IDENTIFIER '.' '*'  */
#line 1388 "bison_parser.y"
                                    {
  std::string combined(std::string((yyvsp[-4].sval)) + "." + (yyvsp[-2].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeStar(strdup(combined.c_str()));
}
#line 5859 "bison_parser.cpp"
    break;

  case 307: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER '.' '*'  */
#line 1394 "bison_parser.y"
                                                   {
  std::string combined(std::string((yyvsp[-6].sval)) + "." + (yyvsp[-4].sval) + "." + (yyvsp[-2].sval));
  free((yyvsp[-6].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeStar(strdup(combined.c_str()));
}
#line 5871 "bison_parser.cpp"
    break;

  case 308: /* nonreserved_keyword: SECOND  */
#line 1408 "bison_parser.y"
                             { (yyval.sval) = strdup("SECOND"); }
#line 5877 "bison_parser.cpp"
    break;

  case 309: /* nonreserved_keyword: MINUTE  */
#line 1409 "bison_parser.y"
         { (yyval.sval) = strdup("MINUTE"); }
#line 5883 "bison_parser.cpp"
    break;

  case 310: /* nonreserved_keyword: HOUR  */
#line 1410 "bison_parser.y"
       { (yyval.sval) = strdup("HOUR"); }
#line 5889 "bison_parser.cpp"
    break;

  case 311: /* nonreserved_keyword: DAY  */
#line 1411 "bison_parser.y"
      { (yyval.sval) = strdup("DAY"); }
#line 5895 "bison_parser.cpp"
    break;

  case 312: /* nonreserved_keyword: MONTH  */
#line 1412 "bison_parser.y"
        { (yyval.sval) = strdup("MONTH"); }
#line 5901 "bison_parser.cpp"
    break;

  case 313: /* nonreserved_keyword: YEAR  */
#line 1413 "bison_parser.y"
       { (yyval.sval) = strdup("YEAR"); }
#line 5907 "bison_parser.cpp"
    break;

  case 314: /* nonreserved_keyword: SECONDS  */
#line 1414 "bison_parser.y"
          { (yyval.sval) = strdup("SECONDS"); }
#line 5913 "bison_parser.cpp"
    break;

  case 315: /* nonreserved_keyword: MINUTES  */
#line 1415 "bison_parser.y"
          { (yyval.sval) = strdup("MINUTES"); }
#line 5919 "bison_parser.cpp"
    break;

  case 316: /* nonreserved_keyword: HOURS  */
#line 1416 "bison_parser.y"
        { (yyval.sval) = strdup("HOURS"); }
#line 5925 "bison_parser.cpp"
    break;

  case 317: /* nonreserved_keyword: DAYS  */
#line 1417 "bison_parser.y"
       { (yyval.sval) = strdup("DAYS"); }
#line 5931 "bison_parser.cpp"
    break;

  case 318: /* nonreserved_keyword: MONTHS  */
#line 1418 "bison_parser.y"
         { (yyval.sval) = strdup("MONTHS"); }
#line 5937 "bison_parser.cpp"
    break;

  case 319: /* nonreserved_keyword: YEARS  */
#line 1419 "bison_parser.y"
        { (yyval.sval) = strdup("YEARS"); }
#line 5943 "bison_parser.cpp"
    break;

  case 320: /* nonreserved_keyword: ISNULL  */
#line 1420 "bison_parser.y"
         { (yyval.sval) = strdup("ISNULL"); }
#line 5949 "bison_parser.cpp"
    break;

  case 321: /* nonreserved_keyword: FORMAT  */
#line 1421 "bison_parser.y"
         { (yyval.sval) = strdup("FORMAT"); }
#line 5955 "bison_parser.cpp"
    break;

  case 322: /* nonreserved_keyword: CHAR  */
#line 1422 "bison_parser.y"
       { (yyval.sval) = strdup("CHAR"); }
#line 5961 "bison_parser.cpp"
    break;

  case 323: /* nonreserved_keyword: VARCHAR  */
#line 1423 "bison_parser.y"
          { (yyval.sval) = strdup("VARCHAR"); }
#line 5967 "bison_parser.cpp"
    break;

  case 324: /* nonreserved_keyword: INT  */
#line 1424 "bison_parser.y"
      { (yyval.sval) = strdup("INT"); }
#line 5973 "bison_parser.cpp"
    break;

  case 325: /* nonreserved_keyword: INTEGER  */
#line 1425 "bison_parser.y"
          { (yyval.sval) = strdup("INTEGER"); }
#line 5979 "bison_parser.cpp"
    break;

  case 326: /* nonreserved_keyword: DATETIME  */
#line 1426 "bison_parser.y"
           { (yyval.sval) = strdup("DATETIME"); }
#line 5985 "bison_parser.cpp"
    break;

  case 327: /* nonreserved_keyword: TIMESTAMP  */
#line 1427 "bison_parser.y"
            { (yyval.sval) = strdup("TIMESTAMP"); }
#line 5991 "bison_parser.cpp"
    break;

  case 328: /* nonreserved_keyword: CONNECT  */
#line 1428 "bison_parser.y"
          { (yyval.sval) = strdup("CONNECT"); }
#line 5997 "bison_parser.cpp"
    break;

  case 329: /* nonreserved_keyword: START  */
#line 1429 "bison_parser.y"
        { (yyval.sval) = strdup("START"); }
#line 6003 "bison_parser.cpp"
    break;

  case 330: /* nonreserved_keyword: NEXT  */
#line 1430 "bison_parser.y"
       { (yyval.sval) = strdup("NEXT"); }
#line 6009 "bison_parser.cpp"
    break;

  case 338: /* string_literal: STRING  */
#line 1434 "bison_parser.y"
                        { (yyval.expr) = Expr::makeLiteral((yyvsp[0].sval)); }
#line 6015 "bison_parser.cpp"
    break;

  case 339: /* bool_literal: TRUE  */
#line 1436 "bison_parser.y"
                    { (yyval.expr) = Expr::makeLiteral(true); }
#line 6021 "bison_parser.cpp"
    break;

  case 340: /* bool_literal: FALSE  */
#line 1437 "bison_parser.y"
        { (yyval.expr) = Expr::makeLiteral(false); }
#line 6027 "bison_parser.cpp"
    break;

  case 341: /* num_literal: FLOATVAL  */
#line 1439 "bison_parser.y"
                       { (yyval.expr) = Expr::makeLiteral((yyvsp[0].fval)); }
#line 6033 "bison_parser.cpp"
    break;

  case 343: /* int_literal: INTVAL  */
#line 1442 "bison_parser.y"
                     { (yyval.expr) = Expr::makeLiteral((yyvsp[0].ival)); }
#line 6039 "bison_parser.cpp"
    break;

  case 344: /* null_literal: NULL  */
#line 1444 "bison_parser.y"
                    { (yyval.expr) = Expr::makeNullLiteral(); }
#line 6045 "bison_parser.cpp"
    break;

  case 345: /* date_literal: DATE STRING  */
#line 1446 "bison_parser.y"
                           {
  int day{0}, month{0}, year{0}, chars_parsed{0};
  // If the whole string is parsed, chars_parsed points to the terminating null byte after the last character
  if (sscanf((yyvsp[0].sval), "%4d-%2d-%2d%n", &day, &month, &year, &chars_parsed) != 3 || (yyvsp[0].sval)[chars_parsed] != 0) {
    free((yyvsp[0].sval));
    yyerror(&yyloc, result, scanner, "Found incorrect date format. Expected format: YYYY-MM-DD");
    YYERROR;
  }
  (yyval.expr) = Expr::makeDateLiteral((yyvsp[0].sval));
}
#line 6060 "bison_parser.cpp"
    break;

  case 346: /* interval_literal: INTVAL duration_field  */
#line 1457 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeIntervalLiteral((yyvsp[-1].ival), (yyvsp[0].datetime_field)); }
#line 6066 "bison_parser.cpp"
    break;

  case 347: /* interval_literal: INTERVAL STRING datetime_field  */
#line 1458 "bison_parser.y"
                                 {
  int duration{0}, chars_parsed{0};
  // If the whole string is parsed, chars_parsed points to the terminating null byte after the last character
  if (sscanf((yyvsp[-1].sval), "%d%n", &duration, &chars_parsed) != 1 || (yyvsp[-1].sval)[chars_parsed] != 0) {
    free((yyvsp[-1].sval));
    yyerror(&yyloc, result, scanner, "Found incorrect interval format. Expected format: INTEGER");
    YYERROR;
  }
  free((yyvsp[-1].sval));
  (yyval.expr) = Expr::makeIntervalLiteral(duration, (yyvsp[0].datetime_field));
}
#line 6082 "bison_parser.cpp"
    break;

  case 348: /* interval_literal: INTERVAL STRING  */
#line 1469 "bison_parser.y"
                  {
  int duration{0}, chars_parsed{0};
  // 'seconds' and 'minutes' are the longest accepted interval qualifiers (7 chars) + null byte
  char unit_string[8];
  // If the whole string is parsed, chars_parsed points to the terminating null byte after the last character
  if (sscanf((yyvsp[0].sval), "%d %7s%n", &duration, unit_string, &chars_parsed) != 2 || (yyvsp[0].sval)[chars_parsed] != 0) {
    free((yyvsp[0].sval));
    yyerror(&yyloc, result, scanner, "Found incorrect interval format. Expected format: INTEGER INTERVAL_QUALIIFIER");
    YYERROR;
  }
  free((yyvsp[0].sval));

  DatetimeField unit;
  if (strcasecmp(unit_string, "second") == 0 || strcasecmp(unit_string, "seconds") == 0) {
    unit = kDatetimeSecond;
  } else if (strcasecmp(unit_string, "minute") == 0 || strcasecmp(unit_string, "minutes") == 0) {
    unit = kDatetimeMinute;
  } else if (strcasecmp(unit_string, "hour") == 0 || strcasecmp(unit_string, "hours") == 0) {
    unit = kDatetimeHour;
  } else if (strcasecmp(unit_string, "day") == 0 || strcasecmp(unit_string, "days") == 0) {
    unit = kDatetimeDay;
  } else if (strcasecmp(unit_string, "month") == 0 || strcasecmp(unit_string, "months") == 0) {
    unit = kDatetimeMonth;
  } else if (strcasecmp(unit_string, "year") == 0 || strcasecmp(unit_string, "years") == 0) {
    unit = kDatetimeYear;
  } else {
    yyerror(&yyloc, result, scanner, "Interval qualifier is unknown.");
    YYERROR;
  }
  (yyval.expr) = Expr::makeIntervalLiteral(duration, unit);
}
#line 6118 "bison_parser.cpp"
    break;

  case 349: /* param_expr: '?'  */
#line 1501 "bison_parser.y"
                 {
  (yyval.expr) = Expr::makeParameter(yylloc.total_column);
  (yyval.expr)->ival2 = yylloc.total_column - 1;  // source column (0-based) of the '?' token
  yyloc.param_list.push_back((yyval.expr));
}
#line 6128 "bison_parser.cpp"
    break;

  case 350: /* param_expr: DOLLAR_PARAM  */
#line 1506 "bison_parser.y"
               {
  if ((yyvsp[0].ival) < 1) {
    yyerror(&yyloc, result, scanner, "$0 is not a valid positional parameter.");
    YYERROR;
  }
  (yyval.expr) = Expr::makeDollarParameter((yyvsp[0].ival));
  // length of $N token: 1 for '$' + digit count of N
  int64_t dollarLen = 1;
  for (int64_t v = (yyvsp[0].ival); v > 0; v /= 10) ++dollarLen;
  (yyval.expr)->ival2 = yylloc.total_column - dollarLen;
  yyloc.param_list.push_back((yyval.expr));
}
#line 6145 "bison_parser.cpp"
    break;

  case 351: /* param_expr: NAMED_PARAM  */
#line 1518 "bison_parser.y"
              {
  (yyval.expr) = Expr::makeNamedParameter((yyvsp[0].sval));
  (yyval.expr)->ival2 = yylloc.total_column - 1 - (int64_t)strlen((yyvsp[0].sval));
  yyloc.param_list.push_back((yyval.expr));
}
#line 6155 "bison_parser.cpp"
    break;

  case 353: /* table_ref: table_ref_commalist ',' table_ref_atomic  */
#line 1527 "bison_parser.y"
                                                                        {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  auto tbl = new TableRef(kTableCrossProduct);
  tbl->list = (yyvsp[-2].table_vec);
  (yyval.table) = tbl;
}
#line 6166 "bison_parser.cpp"
    break;

  case 356: /* table_ref_atomic: ODBC_OJ table_ref_atomic '}'  */
#line 1538 "bison_parser.y"
                               { (yyval.table) = (yyvsp[-1].table); }
#line 6172 "bison_parser.cpp"
    break;

  case 358: /* nonjoin_table_ref_atomic: '(' select_statement ')' opt_table_alias  */
#line 1540 "bison_parser.y"
                                                                                     {
  auto tbl = new TableRef(kTableSelect);
  tbl->select = (yyvsp[-2].select_stmt);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6183 "bison_parser.cpp"
    break;

  case 359: /* nonjoin_table_ref_atomic: function_expr opt_table_alias  */
#line 1548 "bison_parser.y"
                                {
  auto tbl = new TableRef(kTableFunc);
  tbl->func = (yyvsp[-1].expr);
  if ((yyvsp[-1].expr)->name) tbl->name = strdup((yyvsp[-1].expr)->name);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6195 "bison_parser.cpp"
    break;

  case 360: /* nonjoin_table_ref_atomic: '(' VALUES table_value_row_list ')' opt_table_alias  */
#line 1557 "bison_parser.y"
                                                      {
  auto tbl = new TableRef(kTableValues);
  tbl->values = (yyvsp[-2].expr_vec);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6206 "bison_parser.cpp"
    break;

  case 361: /* table_value_row_list: '(' expr_list ')'  */
#line 1566 "bison_parser.y"
                                         {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
}
#line 6215 "bison_parser.cpp"
    break;

  case 362: /* table_value_row_list: table_value_row_list ',' '(' expr_list ')'  */
#line 1570 "bison_parser.y"
                                             {
  (yyvsp[-4].expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
  (yyval.expr_vec) = (yyvsp[-4].expr_vec);
}
#line 6224 "bison_parser.cpp"
    break;

  case 363: /* table_ref_commalist: table_ref_atomic  */
#line 1575 "bison_parser.y"
                                       {
  (yyval.table_vec) = new std::vector<TableRef*>();
  (yyval.table_vec)->push_back((yyvsp[0].table));
}
#line 6233 "bison_parser.cpp"
    break;

  case 364: /* table_ref_commalist: table_ref_commalist ',' table_ref_atomic  */
#line 1579 "bison_parser.y"
                                           {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  (yyval.table_vec) = (yyvsp[-2].table_vec);
}
#line 6242 "bison_parser.cpp"
    break;

  case 365: /* table_ref_name: table_name opt_table_alias  */
#line 1584 "bison_parser.y"
                                            {
  auto tbl = new TableRef(kTableName);
  tbl->schema = (yyvsp[-1].table_name).schema;
  tbl->name = (yyvsp[-1].table_name).name;
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6254 "bison_parser.cpp"
    break;

  case 366: /* table_ref_name_no_alias: table_name  */
#line 1592 "bison_parser.y"
                                     {
  (yyval.table) = new TableRef(kTableName);
  (yyval.table)->schema = (yyvsp[0].table_name).schema;
  (yyval.table)->name = (yyvsp[0].table_name).name;
}
#line 6264 "bison_parser.cpp"
    break;

  case 367: /* table_name: IDENTIFIER  */
#line 1598 "bison_parser.y"
                        {
  (yyval.table_name).schema = nullptr;
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6273 "bison_parser.cpp"
    break;

  case 368: /* table_name: IDENTIFIER '.' IDENTIFIER  */
#line 1602 "bison_parser.y"
                            {
  (yyval.table_name).schema = (yyvsp[-2].sval);
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6282 "bison_parser.cpp"
    break;

  case 369: /* table_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1606 "bison_parser.y"
                                           {
  // Three-part (database.schema.table) name. TableName has no separate
  // database slot, so fold database+schema into schema as "database.schema" -
  // callers here only need the statement to parse, not the individual parts.
  std::string combined(std::string((yyvsp[-4].sval)) + "." + (yyvsp[-2].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.table_name).schema = strdup(combined.c_str());
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6297 "bison_parser.cpp"
    break;

  case 370: /* opt_index_name: IDENTIFIER  */
#line 1617 "bison_parser.y"
                            { (yyval.sval) = (yyvsp[0].sval); }
#line 6303 "bison_parser.cpp"
    break;

  case 371: /* opt_index_name: %empty  */
#line 1618 "bison_parser.y"
              { (yyval.sval) = nullptr; }
#line 6309 "bison_parser.cpp"
    break;

  case 373: /* table_alias: AS IDENTIFIER '(' ident_commalist ')'  */
#line 1620 "bison_parser.y"
                                                            { (yyval.alias_t) = new Alias((yyvsp[-3].sval), (yyvsp[-1].str_vec)); }
#line 6315 "bison_parser.cpp"
    break;

  case 375: /* opt_table_alias: %empty  */
#line 1622 "bison_parser.y"
                                            { (yyval.alias_t) = nullptr; }
#line 6321 "bison_parser.cpp"
    break;

  case 376: /* alias: AS IDENTIFIER  */
#line 1624 "bison_parser.y"
                      { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6327 "bison_parser.cpp"
    break;

  case 377: /* alias: AS STRING  */
#line 1628 "bison_parser.y"
            { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6333 "bison_parser.cpp"
    break;

  case 378: /* alias: IDENTIFIER  */
#line 1629 "bison_parser.y"
             { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6339 "bison_parser.cpp"
    break;

  case 380: /* opt_alias: %empty  */
#line 1631 "bison_parser.y"
                                { (yyval.alias_t) = nullptr; }
#line 6345 "bison_parser.cpp"
    break;

  case 381: /* opt_locking_clause: opt_locking_clause_list  */
#line 1637 "bison_parser.y"
                                             { (yyval.locking_clause_vec) = (yyvsp[0].locking_clause_vec); }
#line 6351 "bison_parser.cpp"
    break;

  case 382: /* opt_locking_clause: %empty  */
#line 1638 "bison_parser.y"
              { (yyval.locking_clause_vec) = nullptr; }
#line 6357 "bison_parser.cpp"
    break;

  case 383: /* opt_locking_clause_list: locking_clause  */
#line 1640 "bison_parser.y"
                                         {
  (yyval.locking_clause_vec) = new std::vector<LockingClause*>();
  (yyval.locking_clause_vec)->push_back((yyvsp[0].locking_t));
}
#line 6366 "bison_parser.cpp"
    break;

  case 384: /* opt_locking_clause_list: opt_locking_clause_list locking_clause  */
#line 1644 "bison_parser.y"
                                         {
  (yyvsp[-1].locking_clause_vec)->push_back((yyvsp[0].locking_t));
  (yyval.locking_clause_vec) = (yyvsp[-1].locking_clause_vec);
}
#line 6375 "bison_parser.cpp"
    break;

  case 385: /* locking_clause: FOR row_lock_mode opt_row_lock_policy  */
#line 1649 "bison_parser.y"
                                                       {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-1].lock_mode_t);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
  (yyval.locking_t)->tables = nullptr;
}
#line 6386 "bison_parser.cpp"
    break;

  case 386: /* locking_clause: FOR row_lock_mode OF ident_commalist opt_row_lock_policy  */
#line 1655 "bison_parser.y"
                                                           {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-3].lock_mode_t);
  (yyval.locking_t)->tables = (yyvsp[-1].str_vec);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
}
#line 6397 "bison_parser.cpp"
    break;

  case 387: /* row_lock_mode: UPDATE  */
#line 1662 "bison_parser.y"
                       { (yyval.lock_mode_t) = RowLockMode::ForUpdate; }
#line 6403 "bison_parser.cpp"
    break;

  case 388: /* row_lock_mode: NO KEY UPDATE  */
#line 1663 "bison_parser.y"
                { (yyval.lock_mode_t) = RowLockMode::ForNoKeyUpdate; }
#line 6409 "bison_parser.cpp"
    break;

  case 389: /* row_lock_mode: SHARE  */
#line 1664 "bison_parser.y"
        { (yyval.lock_mode_t) = RowLockMode::ForShare; }
#line 6415 "bison_parser.cpp"
    break;

  case 390: /* row_lock_mode: KEY SHARE  */
#line 1665 "bison_parser.y"
            { (yyval.lock_mode_t) = RowLockMode::ForKeyShare; }
#line 6421 "bison_parser.cpp"
    break;

  case 391: /* opt_row_lock_policy: SKIP LOCKED  */
#line 1667 "bison_parser.y"
                                  { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::SkipLocked; }
#line 6427 "bison_parser.cpp"
    break;

  case 392: /* opt_row_lock_policy: NOWAIT  */
#line 1668 "bison_parser.y"
         { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::NoWait; }
#line 6433 "bison_parser.cpp"
    break;

  case 393: /* opt_row_lock_policy: %empty  */
#line 1669 "bison_parser.y"
              { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::None; }
#line 6439 "bison_parser.cpp"
    break;

  case 395: /* opt_with_clause: %empty  */
#line 1675 "bison_parser.y"
                                            { (yyval.with_description_vec) = nullptr; }
#line 6445 "bison_parser.cpp"
    break;

  case 396: /* with_clause: WITH with_description_list  */
#line 1677 "bison_parser.y"
                                         { (yyval.with_description_vec) = (yyvsp[0].with_description_vec); }
#line 6451 "bison_parser.cpp"
    break;

  case 397: /* with_description_list: with_description  */
#line 1679 "bison_parser.y"
                                         {
  (yyval.with_description_vec) = new std::vector<WithDescription*>();
  (yyval.with_description_vec)->push_back((yyvsp[0].with_description_t));
}
#line 6460 "bison_parser.cpp"
    break;

  case 398: /* with_description_list: with_description_list ',' with_description  */
#line 1683 "bison_parser.y"
                                             {
  (yyvsp[-2].with_description_vec)->push_back((yyvsp[0].with_description_t));
  (yyval.with_description_vec) = (yyvsp[-2].with_description_vec);
}
#line 6469 "bison_parser.cpp"
    break;

  case 399: /* with_description: IDENTIFIER AS select_with_paren  */
#line 1688 "bison_parser.y"
                                                   {
  (yyval.with_description_t) = new WithDescription();
  (yyval.with_description_t)->alias = (yyvsp[-2].sval);
  (yyval.with_description_t)->select = (yyvsp[0].select_stmt);
}
#line 6479 "bison_parser.cpp"
    break;

  case 400: /* join_clause: table_ref_atomic NATURAL JOIN nonjoin_table_ref_atomic  */
#line 1698 "bison_parser.y"
                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = kJoinNatural;
  (yyval.table)->join->left = (yyvsp[-3].table);
  (yyval.table)->join->right = (yyvsp[0].table);
}
#line 6491 "bison_parser.cpp"
    break;

  case 401: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic ON join_condition  */
#line 1705 "bison_parser.y"
                                                                         {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (JoinType)(yyvsp[-4].join_type);
  (yyval.table)->join->left = (yyvsp[-5].table);
  (yyval.table)->join->right = (yyvsp[-2].table);
  (yyval.table)->join->condition = (yyvsp[0].expr);
}
#line 6504 "bison_parser.cpp"
    break;

  case 402: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic USING '(' ident_commalist ')'  */
#line 1713 "bison_parser.y"
                                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (yyvsp[-6].join_type);
  (yyval.table)->join->left = (yyvsp[-7].table);
  (yyval.table)->join->right = (yyvsp[-4].table);
  (yyval.table)->join->namedColumns = (yyvsp[-1].str_vec);
}
#line 6517 "bison_parser.cpp"
    break;

  case 403: /* opt_join_type: INNER  */
#line 1722 "bison_parser.y"
                      { (yyval.join_type) = kJoinInner; }
#line 6523 "bison_parser.cpp"
    break;

  case 404: /* opt_join_type: LEFT OUTER  */
#line 1723 "bison_parser.y"
             { (yyval.join_type) = kJoinLeft; }
#line 6529 "bison_parser.cpp"
    break;

  case 405: /* opt_join_type: LEFT  */
#line 1724 "bison_parser.y"
       { (yyval.join_type) = kJoinLeft; }
#line 6535 "bison_parser.cpp"
    break;

  case 406: /* opt_join_type: RIGHT OUTER  */
#line 1725 "bison_parser.y"
              { (yyval.join_type) = kJoinRight; }
#line 6541 "bison_parser.cpp"
    break;

  case 407: /* opt_join_type: RIGHT  */
#line 1726 "bison_parser.y"
        { (yyval.join_type) = kJoinRight; }
#line 6547 "bison_parser.cpp"
    break;

  case 408: /* opt_join_type: FULL OUTER  */
#line 1727 "bison_parser.y"
             { (yyval.join_type) = kJoinFull; }
#line 6553 "bison_parser.cpp"
    break;

  case 409: /* opt_join_type: OUTER  */
#line 1728 "bison_parser.y"
        { (yyval.join_type) = kJoinFull; }
#line 6559 "bison_parser.cpp"
    break;

  case 410: /* opt_join_type: FULL  */
#line 1729 "bison_parser.y"
       { (yyval.join_type) = kJoinFull; }
#line 6565 "bison_parser.cpp"
    break;

  case 411: /* opt_join_type: CROSS  */
#line 1730 "bison_parser.y"
        { (yyval.join_type) = kJoinCross; }
#line 6571 "bison_parser.cpp"
    break;

  case 412: /* opt_join_type: %empty  */
#line 1731 "bison_parser.y"
                       { (yyval.join_type) = kJoinInner; }
#line 6577 "bison_parser.cpp"
    break;

  case 416: /* ident_commalist: IDENTIFIER  */
#line 1742 "bison_parser.y"
                             {
  (yyval.str_vec) = new std::vector<char*>();
  (yyval.str_vec)->push_back((yyvsp[0].sval));
}
#line 6586 "bison_parser.cpp"
    break;

  case 417: /* ident_commalist: ident_commalist ',' IDENTIFIER  */
#line 1746 "bison_parser.y"
                                 {
  (yyvsp[-2].str_vec)->push_back((yyvsp[0].sval));
  (yyval.str_vec) = (yyvsp[-2].str_vec);
}
#line 6595 "bison_parser.cpp"
    break;


#line 6599 "bison_parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == SQL_HSQL_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, result, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= SQL_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == SQL_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, result, scanner);
          yychar = SQL_HSQL_EMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != SQL_HSQL_EMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1752 "bison_parser.y"


/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */

    // clang-format on
