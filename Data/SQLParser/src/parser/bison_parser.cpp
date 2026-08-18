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
#line 48 "bison_parser.y"

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

  using namespace hsql;

  int yyerror(YYLTYPE * llocp, SQLParserResult * result, yyscan_t scanner, const char* msg) {
    result->setIsValid(false);
    result->setErrorDetails(strdup(msg), llocp->first_line, llocp->first_column);
    return 0;
  }
  // clang-format off

#line 117 "bison_parser.cpp"

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
  YYSYMBOL_184_ = 184,                     /* '='  */
  YYSYMBOL_EQUALS = 185,                   /* EQUALS  */
  YYSYMBOL_NOTEQUALS = 186,                /* NOTEQUALS  */
  YYSYMBOL_187_ = 187,                     /* '<'  */
  YYSYMBOL_188_ = 188,                     /* '>'  */
  YYSYMBOL_LESS = 189,                     /* LESS  */
  YYSYMBOL_GREATER = 190,                  /* GREATER  */
  YYSYMBOL_LESSEQ = 191,                   /* LESSEQ  */
  YYSYMBOL_GREATEREQ = 192,                /* GREATEREQ  */
  YYSYMBOL_NOTNULL = 193,                  /* NOTNULL  */
  YYSYMBOL_194_ = 194,                     /* '+'  */
  YYSYMBOL_195_ = 195,                     /* '-'  */
  YYSYMBOL_196_ = 196,                     /* '*'  */
  YYSYMBOL_197_ = 197,                     /* '/'  */
  YYSYMBOL_198_ = 198,                     /* '%'  */
  YYSYMBOL_199_ = 199,                     /* '^'  */
  YYSYMBOL_UMINUS = 200,                   /* UMINUS  */
  YYSYMBOL_201_ = 201,                     /* '['  */
  YYSYMBOL_202_ = 202,                     /* ']'  */
  YYSYMBOL_203_ = 203,                     /* '('  */
  YYSYMBOL_204_ = 204,                     /* ')'  */
  YYSYMBOL_205_ = 205,                     /* '.'  */
  YYSYMBOL_206_ = 206,                     /* ';'  */
  YYSYMBOL_207_ = 207,                     /* ','  */
  YYSYMBOL_208_ = 208,                     /* '?'  */
  YYSYMBOL_YYACCEPT = 209,                 /* $accept  */
  YYSYMBOL_input = 210,                    /* input  */
  YYSYMBOL_statement_list = 211,           /* statement_list  */
  YYSYMBOL_statement = 212,                /* statement  */
  YYSYMBOL_preparable_statement = 213,     /* preparable_statement  */
  YYSYMBOL_opt_hints = 214,                /* opt_hints  */
  YYSYMBOL_hint_list = 215,                /* hint_list  */
  YYSYMBOL_hint = 216,                     /* hint  */
  YYSYMBOL_transaction_statement = 217,    /* transaction_statement  */
  YYSYMBOL_opt_transaction_keyword = 218,  /* opt_transaction_keyword  */
  YYSYMBOL_prepare_statement = 219,        /* prepare_statement  */
  YYSYMBOL_prepare_target_query = 220,     /* prepare_target_query  */
  YYSYMBOL_execute_statement = 221,        /* execute_statement  */
  YYSYMBOL_import_statement = 222,         /* import_statement  */
  YYSYMBOL_file_type = 223,                /* file_type  */
  YYSYMBOL_file_path = 224,                /* file_path  */
  YYSYMBOL_opt_import_export_options = 225, /* opt_import_export_options  */
  YYSYMBOL_import_export_options = 226,    /* import_export_options  */
  YYSYMBOL_csv_option = 227,               /* csv_option  */
  YYSYMBOL_export_statement = 228,         /* export_statement  */
  YYSYMBOL_show_statement = 229,           /* show_statement  */
  YYSYMBOL_create_statement = 230,         /* create_statement  */
  YYSYMBOL_opt_not_exists = 231,           /* opt_not_exists  */
  YYSYMBOL_table_elem_commalist = 232,     /* table_elem_commalist  */
  YYSYMBOL_table_elem = 233,               /* table_elem  */
  YYSYMBOL_column_def = 234,               /* column_def  */
  YYSYMBOL_column_type = 235,              /* column_type  */
  YYSYMBOL_opt_time_precision = 236,       /* opt_time_precision  */
  YYSYMBOL_opt_decimal_specification = 237, /* opt_decimal_specification  */
  YYSYMBOL_opt_column_constraints = 238,   /* opt_column_constraints  */
  YYSYMBOL_column_constraints = 239,       /* column_constraints  */
  YYSYMBOL_column_constraint = 240,        /* column_constraint  */
  YYSYMBOL_table_constraint = 241,         /* table_constraint  */
  YYSYMBOL_references_spec = 242,          /* references_spec  */
  YYSYMBOL_drop_statement = 243,           /* drop_statement  */
  YYSYMBOL_opt_exists = 244,               /* opt_exists  */
  YYSYMBOL_alter_statement = 245,          /* alter_statement  */
  YYSYMBOL_alter_action = 246,             /* alter_action  */
  YYSYMBOL_drop_action = 247,              /* drop_action  */
  YYSYMBOL_delete_statement = 248,         /* delete_statement  */
  YYSYMBOL_truncate_statement = 249,       /* truncate_statement  */
  YYSYMBOL_insert_statement = 250,         /* insert_statement  */
  YYSYMBOL_opt_column_list = 251,          /* opt_column_list  */
  YYSYMBOL_update_statement = 252,         /* update_statement  */
  YYSYMBOL_update_clause_commalist = 253,  /* update_clause_commalist  */
  YYSYMBOL_update_clause = 254,            /* update_clause  */
  YYSYMBOL_select_statement = 255,         /* select_statement  */
  YYSYMBOL_select_within_set_operation = 256, /* select_within_set_operation  */
  YYSYMBOL_select_within_set_operation_no_parentheses = 257, /* select_within_set_operation_no_parentheses  */
  YYSYMBOL_select_with_paren = 258,        /* select_with_paren  */
  YYSYMBOL_select_no_paren = 259,          /* select_no_paren  */
  YYSYMBOL_set_operator = 260,             /* set_operator  */
  YYSYMBOL_set_type = 261,                 /* set_type  */
  YYSYMBOL_opt_all = 262,                  /* opt_all  */
  YYSYMBOL_opt_start_with = 263,           /* opt_start_with  */
  YYSYMBOL_opt_connect_by = 264,           /* opt_connect_by  */
  YYSYMBOL_select_clause = 265,            /* select_clause  */
  YYSYMBOL_opt_distinct = 266,             /* opt_distinct  */
  YYSYMBOL_select_list = 267,              /* select_list  */
  YYSYMBOL_opt_from_clause = 268,          /* opt_from_clause  */
  YYSYMBOL_from_clause = 269,              /* from_clause  */
  YYSYMBOL_opt_where = 270,                /* opt_where  */
  YYSYMBOL_opt_group = 271,                /* opt_group  */
  YYSYMBOL_opt_having = 272,               /* opt_having  */
  YYSYMBOL_opt_within_group = 273,         /* opt_within_group  */
  YYSYMBOL_opt_order = 274,                /* opt_order  */
  YYSYMBOL_order_list = 275,               /* order_list  */
  YYSYMBOL_order_desc = 276,               /* order_desc  */
  YYSYMBOL_opt_order_type = 277,           /* opt_order_type  */
  YYSYMBOL_opt_null_ordering = 278,        /* opt_null_ordering  */
  YYSYMBOL_opt_top = 279,                  /* opt_top  */
  YYSYMBOL_opt_limit = 280,                /* opt_limit  */
  YYSYMBOL_expr_list = 281,                /* expr_list  */
  YYSYMBOL_opt_extended_literal_list = 282, /* opt_extended_literal_list  */
  YYSYMBOL_extended_literal_list = 283,    /* extended_literal_list  */
  YYSYMBOL_casted_extended_literal = 284,  /* casted_extended_literal  */
  YYSYMBOL_extended_literal = 285,         /* extended_literal  */
  YYSYMBOL_expr_alias = 286,               /* expr_alias  */
  YYSYMBOL_expr = 287,                     /* expr  */
  YYSYMBOL_operand = 288,                  /* operand  */
  YYSYMBOL_scalar_expr = 289,              /* scalar_expr  */
  YYSYMBOL_unary_expr = 290,               /* unary_expr  */
  YYSYMBOL_binary_expr = 291,              /* binary_expr  */
  YYSYMBOL_logic_expr = 292,               /* logic_expr  */
  YYSYMBOL_in_expr = 293,                  /* in_expr  */
  YYSYMBOL_row_expr_list = 294,            /* row_expr_list  */
  YYSYMBOL_case_expr = 295,                /* case_expr  */
  YYSYMBOL_case_list = 296,                /* case_list  */
  YYSYMBOL_exists_expr = 297,              /* exists_expr  */
  YYSYMBOL_comp_expr = 298,                /* comp_expr  */
  YYSYMBOL_function_expr = 299,            /* function_expr  */
  YYSYMBOL_opt_window = 300,               /* opt_window  */
  YYSYMBOL_opt_partition = 301,            /* opt_partition  */
  YYSYMBOL_opt_frame_clause = 302,         /* opt_frame_clause  */
  YYSYMBOL_frame_type = 303,               /* frame_type  */
  YYSYMBOL_frame_bound = 304,              /* frame_bound  */
  YYSYMBOL_extract_expr = 305,             /* extract_expr  */
  YYSYMBOL_cast_expr = 306,                /* cast_expr  */
  YYSYMBOL_datetime_field = 307,           /* datetime_field  */
  YYSYMBOL_datetime_field_plural = 308,    /* datetime_field_plural  */
  YYSYMBOL_duration_field = 309,           /* duration_field  */
  YYSYMBOL_array_expr = 310,               /* array_expr  */
  YYSYMBOL_array_index = 311,              /* array_index  */
  YYSYMBOL_between_expr = 312,             /* between_expr  */
  YYSYMBOL_column_name = 313,              /* column_name  */
  YYSYMBOL_nonreserved_keyword = 314,      /* nonreserved_keyword  */
  YYSYMBOL_literal = 315,                  /* literal  */
  YYSYMBOL_string_literal = 316,           /* string_literal  */
  YYSYMBOL_bool_literal = 317,             /* bool_literal  */
  YYSYMBOL_num_literal = 318,              /* num_literal  */
  YYSYMBOL_int_literal = 319,              /* int_literal  */
  YYSYMBOL_null_literal = 320,             /* null_literal  */
  YYSYMBOL_date_literal = 321,             /* date_literal  */
  YYSYMBOL_interval_literal = 322,         /* interval_literal  */
  YYSYMBOL_param_expr = 323,               /* param_expr  */
  YYSYMBOL_table_ref = 324,                /* table_ref  */
  YYSYMBOL_table_ref_atomic = 325,         /* table_ref_atomic  */
  YYSYMBOL_nonjoin_table_ref_atomic = 326, /* nonjoin_table_ref_atomic  */
  YYSYMBOL_table_value_row_list = 327,     /* table_value_row_list  */
  YYSYMBOL_table_ref_commalist = 328,      /* table_ref_commalist  */
  YYSYMBOL_table_ref_name = 329,           /* table_ref_name  */
  YYSYMBOL_table_ref_name_no_alias = 330,  /* table_ref_name_no_alias  */
  YYSYMBOL_table_name = 331,               /* table_name  */
  YYSYMBOL_opt_index_name = 332,           /* opt_index_name  */
  YYSYMBOL_table_alias = 333,              /* table_alias  */
  YYSYMBOL_opt_table_alias = 334,          /* opt_table_alias  */
  YYSYMBOL_alias = 335,                    /* alias  */
  YYSYMBOL_opt_alias = 336,                /* opt_alias  */
  YYSYMBOL_opt_locking_clause = 337,       /* opt_locking_clause  */
  YYSYMBOL_opt_locking_clause_list = 338,  /* opt_locking_clause_list  */
  YYSYMBOL_locking_clause = 339,           /* locking_clause  */
  YYSYMBOL_row_lock_mode = 340,            /* row_lock_mode  */
  YYSYMBOL_opt_row_lock_policy = 341,      /* opt_row_lock_policy  */
  YYSYMBOL_opt_with_clause = 342,          /* opt_with_clause  */
  YYSYMBOL_with_clause = 343,              /* with_clause  */
  YYSYMBOL_with_description_list = 344,    /* with_description_list  */
  YYSYMBOL_with_description = 345,         /* with_description  */
  YYSYMBOL_join_clause = 346,              /* join_clause  */
  YYSYMBOL_opt_join_type = 347,            /* opt_join_type  */
  YYSYMBOL_join_condition = 348,           /* join_condition  */
  YYSYMBOL_opt_semicolon = 349,            /* opt_semicolon  */
  YYSYMBOL_ident_commalist = 350           /* ident_commalist  */
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
#define YYFINAL  69
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1518

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  209
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  142
/* YYNRULES -- Number of rules.  */
#define YYNRULES  408
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  760

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   446


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
       2,     2,     2,     2,     2,     2,     2,   198,     2,     2,
     203,   204,   196,   194,   207,   195,   205,   197,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   206,
     187,   184,   188,   208,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   201,     2,   202,   199,     2,     2,     2,     2,     2,
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
     175,   176,   177,   178,   179,   180,   181,   182,   183,   185,
     186,   189,   190,   191,   192,   193,   200
};

#if HSQL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   382,   382,   402,   408,   415,   419,   423,   424,   425,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     442,   443,   445,   449,   454,   458,   468,   469,   470,   472,
     472,   478,   484,   486,   490,   502,   508,   525,   540,   542,
     543,   544,   546,   560,   564,   574,   578,   602,   610,   623,
     630,   645,   665,   666,   671,   682,   695,   707,   714,   721,
     730,   731,   733,   737,   742,   743,   745,   753,   754,   755,
     756,   757,   758,   759,   763,   764,   765,   766,   767,   768,
     769,   770,   771,   772,   773,   775,   776,   778,   779,   780,
     782,   783,   785,   789,   793,   798,   806,   807,   808,   809,
     811,   812,   813,   815,   823,   829,   835,   841,   847,   848,
     855,   861,   863,   873,   880,   891,   898,   906,   907,   914,
     921,   925,   930,   940,   944,   948,   960,   960,   962,   963,
     972,   973,   975,   989,  1001,  1006,  1010,  1014,  1019,  1020,
    1024,  1025,  1027,  1028,  1030,  1042,  1043,  1045,  1047,  1048,
    1050,  1052,  1053,  1055,  1060,  1062,  1063,  1068,  1069,  1071,
    1072,  1074,  1078,  1083,  1085,  1086,  1087,  1089,  1090,  1112,
    1113,  1114,  1116,  1117,  1118,  1119,  1120,  1121,  1122,  1127,
    1131,  1137,  1138,  1140,  1144,  1149,  1149,  1153,  1154,  1155,
    1157,  1166,  1166,  1166,  1166,  1166,  1168,  1169,  1169,  1169,
    1169,  1169,  1169,  1169,  1169,  1170,  1170,  1174,  1174,  1178,
    1180,  1182,  1183,  1184,  1185,  1186,  1188,  1188,  1189,  1190,
    1191,  1192,  1193,  1194,  1195,  1196,  1197,  1199,  1200,  1202,
    1203,  1204,  1205,  1209,  1210,  1214,  1219,  1226,  1227,  1228,
    1229,  1231,  1232,  1234,  1235,  1237,  1238,  1239,  1240,  1241,
    1242,  1243,  1247,  1248,  1252,  1255,  1260,  1261,  1265,  1266,
    1268,  1269,  1274,  1275,  1276,  1280,  1281,  1282,  1284,  1285,
    1286,  1287,  1288,  1290,  1292,  1294,  1295,  1296,  1297,  1298,
    1299,  1301,  1302,  1303,  1304,  1305,  1306,  1308,  1308,  1310,
    1316,  1321,  1323,  1324,  1325,  1326,  1327,  1333,  1339,  1346,
    1352,  1366,  1367,  1368,  1369,  1370,  1371,  1372,  1373,  1374,
    1375,  1376,  1377,  1378,  1379,  1380,  1381,  1382,  1383,  1384,
    1385,  1386,  1387,  1389,  1389,  1389,  1389,  1389,  1389,  1389,
    1391,  1393,  1394,  1396,  1397,  1399,  1401,  1403,  1414,  1415,
    1426,  1458,  1463,  1475,  1484,  1484,  1491,  1491,  1493,  1493,
    1501,  1510,  1519,  1523,  1528,  1532,  1537,  1545,  1551,  1555,
    1559,  1570,  1571,  1573,  1573,  1575,  1575,  1577,  1581,  1582,
    1584,  1584,  1590,  1591,  1593,  1597,  1602,  1608,  1615,  1616,
    1617,  1618,  1620,  1621,  1622,  1628,  1628,  1630,  1632,  1636,
    1641,  1651,  1658,  1666,  1675,  1676,  1677,  1678,  1679,  1680,
    1681,  1682,  1683,  1684,  1686,  1692,  1692,  1695,  1699
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
  "OUTERJOIN", "WITHIN", "CONNECT", "PRIOR", "START", "'='", "EQUALS",
  "NOTEQUALS", "'<'", "'>'", "LESS", "GREATER", "LESSEQ", "GREATEREQ",
  "NOTNULL", "'+'", "'-'", "'*'", "'/'", "'%'", "'^'", "UMINUS", "'['",
  "']'", "'('", "')'", "'.'", "';'", "','", "'?'", "$accept", "input",
  "statement_list", "statement", "preparable_statement", "opt_hints",
  "hint_list", "hint", "transaction_statement", "opt_transaction_keyword",
  "prepare_statement", "prepare_target_query", "execute_statement",
  "import_statement", "file_type", "file_path",
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
  "extract_expr", "cast_expr", "datetime_field", "datetime_field_plural",
  "duration_field", "array_expr", "array_index", "between_expr",
  "column_name", "nonreserved_keyword", "literal", "string_literal",
  "bool_literal", "num_literal", "int_literal", "null_literal",
  "date_literal", "interval_literal", "param_expr", "table_ref",
  "table_ref_atomic", "nonjoin_table_ref_atomic", "table_value_row_list",
  "table_ref_commalist", "table_ref_name", "table_ref_name_no_alias",
  "table_name", "opt_index_name", "table_alias", "opt_table_alias",
  "alias", "opt_alias", "opt_locking_clause", "opt_locking_clause_list",
  "locking_clause", "row_lock_mode", "opt_row_lock_policy",
  "opt_with_clause", "with_clause", "with_description_list",
  "with_description", "join_clause", "opt_join_type", "join_condition",
  "opt_semicolon", "ident_commalist", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-519)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-406)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     858,    46,   102,   116,   130,   102,    13,    92,   126,   146,
     102,   180,    20,   220,    52,   265,   131,   131,   131,   272,
     108,  -519,   213,  -519,   213,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,   -27,  -519,   343,
     115,  -519,   178,   291,  -519,   263,   263,   263,   102,   396,
     102,   305,  -519,   279,   -27,   270,   -25,   279,   279,   279,
     102,  -519,   299,   232,  -519,  -519,  -519,  -519,  -519,  -519,
     373,  -519,   345,  -519,  -519,   321,    59,  -519,   188,  -519,
     460,   211,   477,   362,   492,   102,   102,   421,  -519,   438,
     337,   538,   494,   102,   340,   342,   544,   544,   544,   546,
     102,   102,  -519,   361,   265,  -519,   380,    21,   552,  -519,
    -519,  -519,   -27,   467,   457,   -27,   252,   381,  -519,  -519,
     412,  -519,  -519,   384,   584,  -519,   585,  -519,  -519,    48,
    -519,   386,   385,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,   543,  -519,   461,   -14,
     337,   843,  -519,   544,   590,   216,   411,   -23,  -519,  -519,
     504,  -519,  -519,  -519,   -46,   -46,   -46,  -519,  -519,  -519,
    -519,  -519,   595,  -519,   593,  -519,  -519,   843,   522,  -519,
    -519,    59,  -519,  -519,   843,   522,   843,   428,   480,   601,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,    85,  -519,   280,  -519,  -519,
    -519,   211,  -519,   102,   602,   491,    75,   481,   -94,  -519,
    -519,  -519,   404,   409,   410,  -519,  -519,   612,  -519,  -519,
    -519,   913,   413,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  1073,  -519,  1073,  -519,
     682,   123,  1147,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,   442,   419,  -519,
     526,  -519,   173,   424,  -519,   843,   538,  -519,   587,  -519,
    -519,   426,    95,  -519,   421,  -519,   427,   189,  -519,   429,
     536,   430,  -519,    69,   252,   -27,   431,  -519,   151,   252,
     140,   577,    22,   147,  -519,   480,  -519,  -519,   508,  -519,
    -519,   437,   547,  -519,  1363,   440,   464,   468,   196,  -519,
    -519,  -519,   491,    16,    25,   586,   280,   843,   843,   204,
      10,   441,  1073,   682,  1226,   843,   447,   159,   446,   444,
      63,   197,   843,   843,  1073,  -519,  1073,   110,   449,   207,
    1073,  1073,  1073,  1073,  1073,  1073,  1073,  1073,  1073,  1073,
    1073,  1073,  1073,  1073,  1073,   593,  -519,    27,   102,  -519,
     650,   211,   123,  -519,   279,    95,   651,   652,   396,   653,
     241,  -519,  -519,   211,  -519,   595,  -519,  1090,   421,  -519,
     843,  -519,   391,  -519,  -519,  -519,  -519,   843,  -519,  -519,
     655,   480,   483,   843,   843,  -519,   482,  -519,   501,   104,
    -519,  1363,   590,   544,  -519,  -519,   462,  -519,   463,  -519,
    -519,   469,  -519,  -519,   470,  -519,  -519,  -519,  -519,   471,
    -519,  -519,   -47,   590,   472,   473,  -519,    75,  -519,   588,
     843,   208,  -519,   465,   574,   249,   193,   248,   843,   843,
    -519,   586,   -54,   568,   182,  -519,   843,  -519,   255,   843,
    -519,   559,  1126,  1244,  1073,   475,   682,  -519,   573,   447,
    1244,  1244,  1244,  1244,   153,   153,   153,   153,   159,   159,
      11,    11,    11,   -78,   478,   588,   843,  -519,  -519,   242,
     678,   253,  -519,  -519,  -519,  -519,  -519,    96,   257,  -519,
     333,    19,    55,   419,  -519,    35,  -519,   476,  -519,    55,
    -519,   499,  -519,  -519,  -519,  -519,   681,  -519,  -519,   519,
     123,   123,   628,  -519,   590,  -519,   531,  -519,   490,   264,
    -519,   690,   691,  -519,   692,   695,   698,  -519,  -519,   599,
    -519,   529,   102,  -519,   -47,  -519,  -519,   276,   590,   590,
    -519,   505,  -519,   283,    28,    29,  -519,   843,  1363,   843,
     843,  -519,   205,   201,   503,  -519,  -519,   580,   509,  -519,
    1073,  1244,   682,   507,   285,  -519,  -519,  -519,   287,  -519,
    -519,  -519,   709,   396,  -519,  -519,   711,   512,   513,   414,
    -519,  -519,  -519,   615,  -519,  -519,  -519,   640,   641,   642,
     622,  1090,  -519,   609,   548,  -519,   843,  -519,   -64,  -519,
    -519,  -519,   521,   289,   523,   524,   532,  -519,  -519,   337,
    -519,  -519,  -519,   293,   295,   630,   560,   588,   843,   534,
    -519,   -53,   537,   123,   254,  -519,   843,  -519,   540,   586,
    1226,   541,   297,  -519,  -519,   588,  -519,  -519,   334,   843,
     301,    55,   545,  1090,  -519,  -519,  -519,  1090,   235,   843,
     616,   675,   141,  -519,  -519,  -519,   741,  -519,  -519,  -519,
    -519,  -519,   572,   623,   522,   696,   588,  -519,   303,    32,
    -519,  -519,  -519,   123,   586,   564,  -519,  -519,  -519,   307,
      55,   571,  -519,   590,  -519,   370,   123,   843,   643,  -519,
     603,   575,  -519,   843,   363,   578,  -519,   588,  -519,  -519,
     576,  -519,  -519,  -519,   843,   313,   579,   843,   123,   843,
    -519,  -519,   430,  -519,  -519,  -519,   581,    37,   697,  -519,
    -519,   317,  -519,   590,   123,  -519,   -22,  -519,   284,    38,
     290,  -519,  -519,   649,  -519,   318,   843,  -519,  -519,  -519,
     660,  -519,  -519,   843,  -519,   123,    38,   319,  -519,  -519
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     386,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    30,    30,    30,     0,
     406,     3,    21,    19,    21,    18,     8,     9,     7,    11,
      16,    17,    13,    14,    12,    15,    10,     0,   385,     0,
     358,   114,    33,     0,    54,    61,    61,    61,     0,     0,
       0,     0,   357,   109,     0,     0,     0,   109,   109,   109,
       0,    52,     0,   387,   388,    29,    26,    28,    27,     1,
     386,     2,     0,     6,     5,   171,   123,   124,   160,   106,
       0,   182,     0,     0,   362,     0,     0,   152,    37,     0,
     118,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     4,     0,     0,   146,   136,
     137,   135,     0,   139,     0,     0,   178,   359,   330,   333,
     335,   342,   343,     0,     0,   336,     0,   331,   332,     0,
     341,     0,   181,   183,   185,   187,   323,   324,   325,   334,
     326,   327,   328,   329,    32,    31,     0,   361,     0,     0,
     118,     0,   113,     0,     0,     0,     0,   152,   120,   108,
       0,   131,   130,    38,    41,    41,    41,   107,   104,   105,
     390,   389,     0,   335,     0,   169,   145,     0,   160,   127,
     126,   128,   138,   134,     0,   160,     0,     0,   373,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   338,     0,   337,   340,   188,   189,
      34,     0,    60,     0,     0,   386,     0,     0,   292,   320,
     318,   316,     0,     0,     0,   314,   313,     0,   315,   319,
     317,     0,     0,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   321,     0,   322,     0,   294,
       0,   151,   191,   198,   199,   200,   193,   195,   201,   194,
     216,   202,   203,   204,   205,   197,   192,   207,   296,   208,
       0,   407,     0,     0,   116,     0,     0,   119,     0,   110,
     111,     0,     0,    51,   152,    50,    24,     0,    22,     0,
     149,   147,   179,   371,   178,     0,   159,   161,   166,   178,
     173,   175,   172,     0,   132,   372,   374,   360,     0,   339,
     184,     0,     0,    57,     0,     0,     0,     0,     0,    62,
      64,    65,   386,   146,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   212,     0,   211,   210,     0,     0,
     371,     0,     0,     0,     0,   213,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   209,   146,     0,   117,
       0,     0,   122,   121,   109,     0,     0,     0,     0,     0,
       0,    47,    36,     0,    20,     0,   170,     0,   152,   148,
       0,   369,     0,   370,   190,   125,   129,     0,   165,   164,
     167,   373,     0,     0,     0,   378,     0,   380,     0,   384,
     375,     0,     0,     0,    83,    77,     0,    79,    89,    80,
      67,     0,    74,    75,     0,    71,    72,    78,    81,    86,
      76,    68,    91,     0,     0,     0,    56,     0,    59,   259,
       0,   293,   295,     0,     0,     0,     0,     0,     0,     0,
     239,     0,     0,     0,     0,   206,     0,   196,     0,     0,
     227,   228,     0,   223,     0,     0,     0,   214,     0,   226,
     225,   245,   246,   247,   248,   249,   250,   251,   218,   217,
     220,   219,   221,   222,     0,   259,     0,    35,   408,     0,
       0,     0,    48,    45,    43,    49,    40,     0,     0,    23,
     358,   386,   366,     0,   150,   344,   346,     0,   348,   366,
     347,   141,   180,   367,   368,   162,     0,   163,   133,     0,
     176,   174,     0,   381,     0,   383,     0,   376,     0,     0,
      55,     0,     0,    73,     0,     0,     0,    82,    98,     0,
      97,     0,     0,    66,    90,    92,    94,     0,     0,     0,
      63,     0,   252,     0,   146,     0,   243,     0,     0,     0,
       0,   237,     0,     0,     0,   289,   235,     0,     0,   236,
       0,   224,     0,     0,     0,   215,   290,   256,     0,   115,
     112,    39,     0,     0,    46,    25,     0,     0,     0,     0,
     365,   350,   363,     0,   402,   394,   400,   398,   401,   396,
       0,     0,   356,     0,   143,   168,     0,   379,   384,   382,
     186,    58,     0,     0,     0,     0,     0,    99,    96,   118,
      93,    95,   101,     0,     0,   261,   158,   259,     0,   297,
     299,     0,     0,   241,     0,   240,     0,   244,     0,     0,
     291,     0,     0,   231,   229,   259,    44,    42,   359,     0,
       0,   366,   367,     0,   397,   399,   395,     0,   345,     0,
       0,   154,     0,   377,    70,    88,     0,    84,    69,    85,
     103,   100,     0,     0,   160,     0,   259,   254,     0,     0,
     273,   274,   238,   242,     0,     0,   232,   230,   257,     0,
     366,     0,   349,     0,   391,   403,   140,     0,     0,   144,
       0,     0,   102,     0,   264,     0,   253,   259,   298,   300,
       0,   233,   352,   351,     0,     0,     0,     0,   142,     0,
     177,    87,   260,   265,   266,   267,     0,     0,     0,   255,
     234,     0,   364,     0,   404,   392,   156,   258,     0,     0,
       0,   272,   262,     0,   353,     0,     0,   153,   271,   269,
       0,   270,   268,     0,   393,   155,     0,     0,   263,   157
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -519,  -519,  -519,   713,  -519,   760,  -519,   401,  -519,   452,
    -519,  -519,  -519,  -519,  -356,   -79,   309,   415,   292,  -519,
    -519,  -519,   466,  -519,   350,  -519,  -363,  -519,  -519,  -519,
    -519,   247,  -519,  -498,  -519,   -43,  -519,  -519,  -519,  -519,
    -519,  -519,  -146,  -519,  -519,   516,  -205,   -89,  -519,   240,
     -37,   -33,  -519,  -519,  -519,  -519,   -65,  -299,  -519,  -519,
    -519,  -132,  -519,  -519,  -519,  -176,    43,   400,  -519,  -519,
    -519,    67,  -328,  -519,   -12,   589,   594,  -211,  -150,  -191,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,   474,  -519,  -519,
    -375,  -447,  -519,  -519,  -519,  -386,  -519,  -519,  -140,  -519,
    -519,  -519,  -519,  -519,  -519,  -367,   -76,  -519,  -519,   669,
     -86,  -519,  -519,   672,  -519,  -519,  -518,   149,  -519,  -519,
    -519,  -519,     1,  -519,  -519,  -457,  -186,  -519,   405,  -519,
     506,  -519,   202,  -519,  -519,  -519,   705,  -519,  -519,  -519,
    -519,  -404
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    21,    22,    73,   287,   288,    23,    66,
      24,   145,    25,    26,    89,   164,   283,   380,   381,    27,
      28,    29,    84,   318,   319,   320,   432,   537,   533,   543,
     544,   545,   321,   546,    30,    93,    31,   279,   280,    32,
      33,    34,   155,    35,   157,   158,    36,   178,   179,   180,
     338,   112,   113,   183,   604,   661,    78,   177,   290,   388,
     389,   152,   699,   747,   676,   116,   296,   297,   400,   517,
     108,   188,   291,   131,   132,   133,   134,   292,   293,   252,
     253,   254,   255,   256,   257,   341,   258,   330,   259,   260,
     261,   552,   674,   726,   727,   742,   262,   263,   202,   203,
     204,   264,   265,   266,   267,   268,   269,   136,   137,   138,
     139,   140,   141,   142,   143,   504,   505,   506,   650,   507,
     508,    51,   509,   148,   590,   591,   592,   394,   304,   305,
     306,   409,   527,    37,    38,    63,    64,   510,   600,   735,
      71,   272
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      77,   251,   294,    41,   217,   135,    44,   454,   529,   299,
     313,    52,   502,    56,    99,   100,   101,    95,   165,   166,
     503,   175,   494,    40,   440,   277,   185,   173,   441,   547,
     176,   746,   629,    75,   298,   708,   300,   302,   577,   339,
     334,   176,   176,   738,   738,   115,   621,   181,   528,    87,
     181,    90,   602,   119,   120,   336,   739,   337,   391,   538,
     350,   102,   151,   593,   342,   342,   391,   309,   486,   281,
     109,    97,   391,    60,   270,    39,   539,   329,   314,   404,
     343,   343,   214,   658,   587,    45,   149,   150,   289,   118,
     119,   120,   121,   122,   160,    46,   525,   526,   376,   376,
     340,   168,   169,   594,   448,    40,   110,   393,   595,   323,
      98,   324,   553,   215,    61,   596,   597,   438,   274,    42,
     608,   377,   582,   365,   449,   372,    47,   540,   541,   135,
     450,   542,   598,    43,    15,   135,  -403,   599,   574,   695,
     342,   334,   111,   370,   623,   624,   378,   583,   295,   350,
     457,   680,   382,   462,   393,   463,   343,   282,   578,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   702,   124,    54,   445,   446,   512,
     677,   342,   589,   452,   276,   390,   444,   342,    48,   216,
     392,   125,   460,   461,   692,   632,   392,   343,   688,   109,
     126,   379,   379,   343,   740,   740,   396,   741,   741,   345,
     364,   405,   365,   464,   311,   118,   119,   120,   121,   122,
     439,   442,    49,    54,   174,   630,   502,   647,   709,   706,
     181,   485,   627,   713,   503,   110,   524,   126,   127,   128,
     465,   342,  -354,   398,   642,   566,    50,   298,   569,   315,
     316,   317,    55,   520,   521,   628,   511,   343,   342,   342,
     729,   123,    53,   593,   525,   526,   114,   457,    62,   342,
     399,   111,    69,   571,   343,   343,    75,    76,   502,   484,
     129,   273,   502,   406,   349,   343,   503,    65,   443,   715,
     503,   350,    57,   130,    94,   135,   588,   350,   562,   563,
     678,   124,    58,   594,   559,   402,   700,   135,   595,   186,
     407,   342,   636,   467,    70,   596,   597,   125,   328,   342,
      80,   689,   342,   342,   408,   635,   187,   343,    72,   745,
     468,   490,   598,    59,   530,   343,  -403,   599,   343,   343,
    -406,  -406,   560,   170,  -406,  -406,    79,   359,   360,   361,
     362,   363,   364,   750,   365,   361,   362,   363,   364,   489,
     365,   395,   449,   126,   127,   128,   401,   342,   561,   487,
     758,   498,   342,  -405,   682,   722,   558,   369,   567,   640,
     370,    81,     1,   343,   565,   568,   731,    82,   343,   390,
       2,   736,    83,   384,   513,   514,   385,     3,   593,    88,
     436,   458,     4,   437,   459,    96,   129,   631,    92,   633,
     634,   554,     5,   555,   564,     6,     7,   652,   514,   130,
     190,   191,   192,   193,   194,   195,   103,     8,     9,   573,
      91,   218,   118,   119,   120,   121,   122,    10,   594,   104,
      11,   219,  -355,   595,   106,   496,   579,   107,   497,   211,
     596,   597,   748,   749,   716,   220,   662,   581,   751,   752,
     497,   585,    12,   117,   211,   221,    13,   598,   611,    67,
      68,   370,   599,   670,   284,   285,   222,   223,   224,   225,
     622,   144,    14,   370,   226,   146,   683,   626,    15,   644,
     390,   645,   390,   665,   390,   147,   666,   671,   704,   672,
     370,   687,   370,   717,   390,   690,   151,   707,   691,   696,
     390,   712,    85,    86,   390,   227,   228,   732,   124,   229,
     370,   744,   754,   759,   390,   370,   397,   723,   724,   725,
      16,    17,    18,   153,   125,   641,   323,   554,   586,   189,
     154,   156,   159,   619,   161,   301,   162,   718,   163,   167,
     230,   231,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    54,   232,   176,   734,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     126,   127,   128,   172,   182,   184,   189,   205,   206,   207,
     210,   212,   211,   271,   213,   275,   755,   278,   286,   173,
     114,   303,   685,   298,   307,   312,    15,   325,   322,   245,
     246,   247,   326,   327,   335,   218,   118,   119,   120,   121,
     122,   366,   367,   248,   249,   219,   368,   371,   374,   375,
     383,   250,   387,   386,   403,   411,   130,   390,   397,   220,
     412,   434,   413,   433,   451,   435,    75,   710,   365,   221,
     455,   456,   466,   488,   519,   492,   493,   495,   516,   522,
     222,   223,   224,   225,   523,   531,   532,   551,   226,   556,
     557,   464,   534,   535,   536,   548,   549,   342,   572,   575,
     576,   580,   603,   601,   605,   218,   118,   119,   120,   121,
     122,   606,   607,   609,   610,   219,   612,   613,   614,   227,
     228,   615,   124,   229,   616,   617,   618,   637,   625,   220,
     638,   643,   639,   646,   648,   649,   653,   651,   125,   221,
     654,   655,   656,   657,   659,   664,   328,   667,   668,   660,
     222,   223,   224,   225,   230,   231,   669,   673,   226,   679,
     675,   681,    75,   684,   697,   686,   698,   701,   693,   232,
     542,   703,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   126,   127,   128,   705,   711,   227,
     228,   719,   124,   229,   714,   743,   720,   753,   756,   721,
     730,   728,   733,   105,    74,   737,   499,   550,   125,   584,
     491,   620,   373,   245,   246,   247,   757,   515,   208,   308,
     310,   209,   694,   447,   230,   231,   518,   248,   249,   171,
     663,   410,     0,     0,     0,   250,     0,     0,     0,   232,
     130,     0,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   126,   127,   128,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   218,   118,   119,   120,
     121,   122,     0,     0,     0,     0,   219,     0,     0,     0,
       0,     0,     0,   245,   246,   247,     0,     1,     0,     0,
     220,     0,     0,     0,     0,     2,     0,   248,   249,     0,
     221,     0,     3,     0,     0,   250,     0,     4,     0,     0,
     130,   222,   223,   224,   225,     0,     0,     5,     0,   226,
       6,     7,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     8,     9,     0,     0,   218,   118,   119,   120,
     121,   122,    10,     0,     0,    11,   219,     0,     0,     0,
     227,   228,     0,   124,   229,     0,     0,     0,     0,     0,
     220,     0,     0,     0,     0,     0,     0,    12,     0,   125,
     221,    13,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   331,   223,   224,   225,   230,   231,    14,     0,   226,
       0,     0,     0,    15,     0,     0,     0,     0,     0,     0,
     232,     0,     0,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   126,   127,   128,     0,     0,
     227,   228,     0,   124,   229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    16,    17,    18,     0,   125,
       0,     0,     0,     0,   245,   246,   247,     0,     0,     0,
       0,     0,     0,     0,     0,   230,   332,     0,   248,   249,
       0,     0,     0,     0,     0,     0,   250,     0,     0,     0,
     232,   130,     0,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   126,   127,   128,     0,     0,
       0,     0,     0,     0,     0,     0,   218,   118,   119,   120,
     121,   122,     0,     0,     0,     0,   219,     0,     0,     0,
       0,     0,     0,   500,   245,   246,   247,     0,     0,     0,
     220,     0,     0,   219,     0,     0,     0,     0,   248,   249,
     221,     0,     0,     0,     0,     0,   333,   220,     0,     0,
       0,   130,   223,   224,   225,     0,     0,   221,     0,   226,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   225,     0,     0,     0,     0,   226,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     227,   228,     0,   124,   229,     0,   344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   228,   125,
       0,   229,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   230,   332,     0,     0,     0,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
     232,     0,   230,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   126,   127,   128,     0,   346,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,     0,     0,   570,     0,     0,     0,     0,   453,
     346,     0,     0,     0,   245,   246,   247,   349,     0,     0,
       0,     0,     0,     0,   350,   351,     0,     0,   248,   249,
     347,   245,     0,   247,     0,     0,   333,   348,   349,     0,
       0,   130,   345,     0,     0,   350,   351,     0,     0,     0,
       0,     0,     0,   501,     0,     0,     0,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     352,   353,   354,   355,   356,     0,     0,   357,   358,     0,
     359,   360,   361,   362,   363,   364,     0,   365,     0,   346,
       0,   352,   353,   354,   355,   356,     0,     0,   357,   358,
       0,   359,   360,   361,   362,   363,   364,  -406,   365,   453,
       0,     0,     0,     0,     0,     0,     0,   349,     0,     0,
       0,     0,     0,     0,   350,   351,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   349,   414,     0,     0,     0,
       0,     0,   350,  -406,     0,     0,     0,     0,     0,     0,
     415,     0,     0,     0,   416,   417,   418,   419,   420,     0,
     421,     0,     0,     0,     0,     0,     0,     0,   422,     0,
     352,   353,   354,   355,   356,     0,     0,   357,   358,     0,
     359,   360,   361,   362,   363,   364,     0,   365,  -406,  -406,
    -406,   355,   356,   423,     0,   357,   358,     0,   359,   360,
     361,   362,   363,   364,     0,   365,     0,     0,     0,     0,
       0,   424,     0,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,   428,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431
};

static const yytype_int16 yycheck[] =
{
      37,   151,   178,     2,   150,    81,     5,   335,   412,   185,
     215,    10,   387,    12,    57,    58,    59,    54,    97,    98,
     387,   107,   378,     3,   323,   157,   115,     6,     3,   433,
      14,    53,     3,    60,   184,     3,   186,   187,   485,   250,
     231,    14,    14,     6,     6,    78,   544,   112,   411,    48,
     115,    50,   509,     5,     6,   246,    19,   248,     3,   106,
     138,    60,    85,    28,   118,   118,     3,   207,   367,   115,
      11,    96,     3,    21,   153,    29,   123,   227,     3,    57,
     134,   134,    96,   601,    65,    72,    85,    86,   174,     4,
       5,     6,     7,     8,    93,    82,   160,   161,     3,     3,
     250,   100,   101,    68,    94,     3,    47,   293,    73,   203,
     135,   205,   440,   127,    62,    80,    81,   322,   155,     3,
     524,    26,    26,   201,   114,   275,   113,   174,   175,   205,
     120,   178,    97,     3,   115,   211,   101,   102,   466,   657,
     118,   332,    83,   207,   548,   549,    51,    51,   181,   138,
     204,   204,   284,   344,   340,   346,   134,   203,   486,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   672,    90,   203,   327,   328,   390,
     627,   118,   127,   333,   207,   207,   326,   118,    96,   203,
     127,   106,   342,   343,   651,   558,   127,   134,   645,    11,
     152,   106,   106,   134,   167,   167,   295,   170,   170,    56,
     199,    64,   201,   103,   213,     4,     5,     6,     7,     8,
     204,   196,    96,   203,   203,   196,   601,   583,   196,   676,
     295,   204,   204,   690,   601,    47,   132,   152,   153,   154,
     130,   118,   207,    92,   572,   456,   100,   397,   459,   174,
     175,   176,    12,   403,   404,   554,   388,   134,   118,   118,
     707,    50,    82,    28,   160,   161,    78,   204,     3,   118,
     119,    83,     0,   464,   134,   134,    60,    37,   653,   365,
     195,    65,   657,   136,   131,   134,   653,   156,   325,   693,
     657,   138,    72,   208,    54,   371,   501,   138,   448,   449,
     628,    90,    82,    68,   111,   165,   165,   383,    73,    57,
     163,   118,   111,   106,   206,    80,    81,   106,   114,   118,
     205,   649,   118,   118,   177,   120,    74,   134,   115,   733,
     123,   374,    97,   113,   413,   134,   101,   102,   134,   134,
     187,   188,    94,   103,   191,   192,     3,   194,   195,   196,
     197,   198,   199,   739,   201,   196,   197,   198,   199,   371,
     201,   294,   114,   152,   153,   154,   299,   118,   120,   368,
     756,   383,   118,     0,   120,   703,   127,   204,   123,   570,
     207,   203,     9,   134,   202,   130,   714,    96,   134,   207,
      17,   719,   129,   204,     3,     4,   207,    24,    28,     3,
     204,   204,    29,   207,   207,   135,   195,   557,   129,   559,
     560,   203,    39,   205,   451,    42,    43,     3,     4,   208,
     140,   141,   142,   143,   144,   145,   127,    54,    55,   466,
     125,     3,     4,     5,     6,     7,     8,    64,    68,   207,
      67,    13,   207,    73,    99,   204,   204,   126,   207,   207,
      80,    81,   168,   169,    84,    27,   606,   204,   168,   169,
     207,   204,    89,     3,   207,    37,    93,    97,   204,    17,
      18,   207,   102,   619,   165,   166,    48,    49,    50,    51,
     204,     4,   109,   207,    56,   123,   636,   204,   115,   204,
     207,   204,   207,   204,   207,     3,   207,   204,   674,   204,
     207,   204,   207,   133,   207,   204,    85,   204,   207,   659,
     207,   204,    46,    47,   207,    87,    88,   204,    90,    91,
     207,   204,   204,   204,   207,   207,   207,   164,   165,   166,
     157,   158,   159,    95,   106,   572,   203,   203,   205,   205,
     203,     3,    48,   542,   204,   117,   204,   697,     4,     3,
     122,   123,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   203,   137,    14,   717,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   203,   117,   128,   205,   203,     4,     4,
     204,    48,   207,     3,   133,   184,   746,    93,     3,     6,
      78,   121,   639,   753,     3,     3,   115,   203,   127,   181,
     182,   183,   203,   203,   201,     3,     4,     5,     6,     7,
       8,   179,   203,   195,   196,    13,   100,   203,    41,   203,
     203,   203,    96,   204,    57,   127,   208,   207,   207,    27,
     203,   177,    95,   203,   203,   177,    60,   684,   201,    37,
     204,   207,   203,     3,   171,     4,     4,     4,     3,   177,
      48,    49,    50,    51,   163,   203,   203,    79,    56,   204,
      96,   103,   203,   203,   203,   203,   203,   118,   203,   106,
     202,     3,   183,   207,     3,     3,     4,     5,     6,     7,
       8,   172,    64,   162,   204,    13,     6,     6,     6,    87,
      88,     6,    90,    91,     6,   106,   177,   204,   203,    27,
     130,   204,   203,     4,     3,   203,   101,   204,   106,    37,
      80,    80,    80,   101,   115,   204,   114,   204,   204,   181,
      48,    49,    50,    51,   122,   123,   204,   107,    56,   205,
     180,   204,    60,   203,   128,   204,    71,     6,   203,   137,
     178,   128,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,    71,   204,    87,
      88,   128,    90,    91,   203,    78,   173,   128,   118,   204,
     204,   203,   203,    70,    24,   204,   385,   437,   106,   497,
     375,   544,   276,   181,   182,   183,   753,   397,   129,   205,
     211,   129,   653,   329,   122,   123,   401,   195,   196,   104,
     608,   305,    -1,    -1,    -1,   203,    -1,    -1,    -1,   137,
     208,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,     9,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    17,    -1,   195,   196,    -1,
      37,    -1,    24,    -1,    -1,   203,    -1,    29,    -1,    -1,
     208,    48,    49,    50,    51,    -1,    -1,    39,    -1,    56,
      42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    -1,    -1,     3,     4,     5,     6,
       7,     8,    64,    -1,    -1,    67,    13,    -1,    -1,    -1,
      87,    88,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,   106,
      37,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,   122,   123,   109,    -1,    56,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      87,    88,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,   158,   159,    -1,   106,
      -1,    -1,    -1,    -1,   181,   182,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,    -1,   195,   196,
      -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,    -1,
     137,   208,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,     3,   181,   182,   183,    -1,    -1,    -1,
      27,    -1,    -1,    13,    -1,    -1,    -1,    -1,   195,   196,
      37,    -1,    -1,    -1,    -1,    -1,   203,    27,    -1,    -1,
      -1,   208,    49,    50,    51,    -1,    -1,    37,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    90,    91,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,   106,
      -1,    91,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,   122,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,   103,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    -1,    -1,   118,    -1,    -1,    -1,    -1,   123,
     103,    -1,    -1,    -1,   181,   182,   183,   131,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,    -1,   195,   196,
     123,   181,    -1,   183,    -1,    -1,   203,   130,   131,    -1,
      -1,   208,    56,    -1,    -1,   138,   139,    -1,    -1,    -1,
      -1,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,   185,   186,   187,   188,    -1,    -1,   191,   192,    -1,
     194,   195,   196,   197,   198,   199,    -1,   201,    -1,   103,
      -1,   184,   185,   186,   187,   188,    -1,    -1,   191,   192,
      -1,   194,   195,   196,   197,   198,   199,   103,   201,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    13,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    31,    32,    33,    34,    35,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
     184,   185,   186,   187,   188,    -1,    -1,   191,   192,    -1,
     194,   195,   196,   197,   198,   199,    -1,   201,   184,   185,
     186,   187,   188,    70,    -1,   191,   192,    -1,   194,   195,
     196,   197,   198,   199,    -1,   201,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     9,    17,    24,    29,    39,    42,    43,    54,    55,
      64,    67,    89,    93,   109,   115,   157,   158,   159,   210,
     211,   212,   213,   217,   219,   221,   222,   228,   229,   230,
     243,   245,   248,   249,   250,   252,   255,   342,   343,    29,
       3,   331,     3,     3,   331,    72,    82,   113,    96,    96,
     100,   330,   331,    82,   203,   258,   331,    72,    82,   113,
      21,    62,     3,   344,   345,   156,   218,   218,   218,     0,
     206,   349,   115,   214,   214,    60,   258,   259,   265,     3,
     205,   203,    96,   129,   231,   231,   231,   331,     3,   223,
     331,   125,   129,   244,   258,   259,   135,    96,   135,   244,
     244,   244,   331,   127,   207,   212,    99,   126,   279,    11,
      47,    83,   260,   261,    78,   260,   274,     3,     4,     5,
       6,     7,     8,    50,    90,   106,   152,   153,   154,   195,
     208,   282,   283,   284,   285,   315,   316,   317,   318,   319,
     320,   321,   322,   323,     4,   220,   123,     3,   332,   331,
     331,    85,   270,    95,   203,   251,     3,   253,   254,    48,
     331,   204,   204,     4,   224,   224,   224,     3,   331,   331,
     258,   345,   203,     6,   203,   319,    14,   266,   256,   257,
     258,   265,   117,   262,   128,   256,    57,    74,   280,   205,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   307,   308,   309,   203,     4,     4,   318,   322,
     204,   207,    48,   133,    96,   127,   203,   251,     3,    13,
      27,    37,    48,    49,    50,    51,    56,    87,    88,    91,
     122,   123,   137,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   181,   182,   183,   195,   196,
     203,   287,   288,   289,   290,   291,   292,   293,   295,   297,
     298,   299,   305,   306,   310,   311,   312,   313,   314,   315,
     224,     3,   350,    65,   259,   184,   207,   270,    93,   246,
     247,   115,   203,   225,   225,   225,     3,   215,   216,   319,
     267,   281,   286,   287,   274,   260,   275,   276,   287,   274,
     287,   117,   287,   121,   337,   338,   339,     3,   285,   307,
     284,   331,     3,   255,     3,   174,   175,   176,   232,   233,
     234,   241,   127,   203,   205,   203,   203,   203,   114,   287,
     296,    48,   123,   203,   288,   201,   288,   288,   259,   286,
     287,   294,   118,   134,    19,    56,   103,   123,   130,   131,
     138,   139,   184,   185,   186,   187,   188,   191,   192,   194,
     195,   196,   197,   198,   199,   201,   179,   203,   100,   204,
     207,   203,   287,   254,    41,   203,     3,    26,    51,   106,
     226,   227,   270,   203,   204,   207,   204,    96,   268,   269,
     207,     3,   127,   335,   336,   280,   256,   207,    92,   119,
     277,   280,   165,    57,    57,    64,   136,   163,   177,   340,
     339,   127,   203,    95,    13,    27,    31,    32,    33,    34,
      35,    37,    45,    70,    88,    90,    91,   105,   110,   112,
     122,   155,   235,   203,   177,   177,   204,   207,   255,   204,
     266,     3,   196,   259,   307,   287,   287,   296,    94,   114,
     120,   203,   287,   123,   281,   204,   207,   204,   204,   207,
     287,   287,   288,   288,   103,   130,   203,   106,   123,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   319,   204,   266,   331,     3,   283,
     244,   226,     4,     4,   223,     4,   204,   207,   283,   216,
       3,   203,   299,   314,   324,   325,   326,   328,   329,   331,
     346,   270,   286,     3,     4,   276,     3,   278,   337,   171,
     287,   287,   177,   163,   132,   160,   161,   341,   235,   350,
     224,   203,   203,   237,   203,   203,   203,   236,   106,   123,
     174,   175,   178,   238,   239,   240,   242,   350,   203,   203,
     233,    79,   300,   281,   203,   205,   204,    96,   127,   111,
      94,   120,   287,   287,   259,   202,   286,   123,   130,   286,
     118,   288,   203,   259,   281,   106,   202,   300,   281,   204,
       3,   204,    26,    51,   227,   204,   205,    65,   255,   127,
     333,   334,   335,    28,    68,    73,    80,    81,    97,   102,
     347,   207,   334,   183,   263,     3,   172,    64,   350,   162,
     204,   204,     6,     6,     6,     6,     6,   106,   177,   331,
     240,   242,   204,   350,   350,   203,   204,   204,   266,     3,
     196,   287,   235,   287,   287,   120,   111,   204,   130,   203,
     288,   259,   281,   204,   204,   204,     4,   223,     3,   203,
     327,   204,     3,   101,    80,    80,    80,   101,   325,   115,
     181,   264,   287,   341,   204,   204,   207,   204,   204,   204,
     251,   204,   204,   107,   301,   180,   273,   300,   281,   205,
     204,   204,   120,   287,   203,   259,   204,   204,   300,   281,
     204,   207,   334,   203,   326,   325,   287,   128,    71,   271,
     165,     6,   242,   128,   274,    71,   300,   204,     3,   196,
     259,   204,   204,   334,   203,   350,    84,   133,   287,   128,
     173,   204,   281,   164,   165,   166,   302,   303,   203,   300,
     204,   281,   204,   203,   287,   348,   281,   204,     6,    19,
     167,   170,   304,    78,   204,   350,    53,   272,   168,   169,
     304,   168,   169,   128,   204,   287,   118,   275,   304,   204
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   209,   210,   211,   211,   212,   212,   212,   212,   212,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     214,   214,   215,   215,   216,   216,   217,   217,   217,   218,
     218,   219,   220,   221,   221,   222,   222,   223,   224,   225,
     225,   225,   226,   226,   226,   226,   226,   226,   227,   227,
     228,   228,   229,   229,   229,   230,   230,   230,   230,   230,
     231,   231,   232,   232,   233,   233,   234,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   236,   236,   237,   237,   237,
     238,   238,   239,   239,   239,   239,   240,   240,   240,   240,
     241,   241,   241,   242,   243,   243,   243,   243,   244,   244,
     245,   246,   247,   248,   249,   250,   250,   251,   251,   252,
     253,   253,   254,   255,   255,   255,   256,   256,   257,   257,
     258,   258,   259,   259,   260,   261,   261,   261,   262,   262,
     263,   263,   264,   264,   265,   266,   266,   267,   268,   268,
     269,   270,   270,   271,   271,   272,   272,   273,   273,   274,
     274,   275,   275,   276,   277,   277,   277,   278,   278,   279,
     279,   279,   280,   280,   280,   280,   280,   280,   280,   281,
     281,   282,   282,   283,   283,   284,   284,   285,   285,   285,
     286,   287,   287,   287,   287,   287,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   289,   289,   289,
     290,   290,   290,   290,   290,   290,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   292,   292,   293,
     293,   293,   293,   293,   293,   294,   294,   295,   295,   295,
     295,   296,   296,   297,   297,   298,   298,   298,   298,   298,
     298,   298,   299,   299,   299,   299,   299,   299,   300,   300,
     301,   301,   302,   302,   302,   303,   303,   303,   304,   304,
     304,   304,   304,   305,   306,   307,   307,   307,   307,   307,
     307,   308,   308,   308,   308,   308,   308,   309,   309,   310,
     311,   312,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   315,   315,   315,   315,   315,   315,   315,
     316,   317,   317,   318,   318,   319,   320,   321,   322,   322,
     322,   323,   323,   323,   324,   324,   325,   325,   326,   326,
     326,   326,   327,   327,   328,   328,   329,   330,   331,   331,
     331,   332,   332,   333,   333,   334,   334,   335,   335,   335,
     336,   336,   337,   337,   338,   338,   339,   339,   340,   340,
     340,   340,   341,   341,   341,   342,   342,   343,   344,   344,
     345,   346,   346,   346,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   348,   349,   349,   350,   350
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     3,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     0,     1,     3,     1,     4,     2,     2,     2,     1,
       0,     4,     1,     2,     5,     7,     6,     1,     1,     4,
       3,     0,     4,     2,     4,     2,     3,     1,     2,     2,
       5,     5,     2,     3,     2,     8,     7,     6,     9,     7,
       3,     0,     1,     3,     1,     1,     3,     1,     1,     4,
       4,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     4,     3,     0,     5,     3,     0,
       1,     0,     1,     2,     1,     2,     2,     1,     1,     2,
       5,     4,     6,     3,     4,     4,     3,     4,     2,     0,
       5,     1,     4,     4,     2,     8,     5,     3,     0,     5,
       1,     3,     3,     2,     2,     6,     1,     1,     1,     3,
       3,     3,     4,     6,     2,     1,     1,     1,     1,     0,
       3,     0,     3,     0,     9,     1,     0,     1,     1,     0,
       2,     2,     0,     4,     0,     2,     0,     7,     0,     3,
       0,     1,     3,     3,     1,     1,     0,     0,     2,     2,
       4,     0,     2,     2,     4,     2,     4,     8,     0,     1,
       3,     1,     0,     1,     3,     1,     6,     1,     2,     2,
       2,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     2,
       2,     2,     2,     2,     3,     4,     1,     3,     3,     3,
       3,     3,     3,     3,     4,     3,     3,     3,     3,     5,
       6,     5,     6,     7,     8,     3,     3,     4,     6,     3,
       5,     4,     5,     4,     5,     3,     3,     3,     3,     3,
       3,     3,     4,     7,     6,     8,     4,     6,     6,     0,
       3,     0,     2,     5,     0,     1,     1,     1,     2,     2,
       2,     2,     1,     6,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     5,     1,     3,     1,     3,     1,     5,     7,     5,
       7,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       2,     1,     1,     1,     1,     3,     1,     1,     1,     4,
       2,     5,     3,     5,     1,     3,     2,     1,     1,     3,
       5,     1,     0,     1,     5,     1,     0,     2,     2,     1,
       1,     0,     1,     0,     1,     2,     3,     5,     1,     3,
       1,     2,     2,     1,     0,     1,     0,     2,     1,     3,
       3,     4,     6,     8,     1,     2,     1,     2,     1,     2,
       1,     1,     1,     0,     1,     1,     0,     1,     3
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
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2303 "bison_parser.cpp"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2309 "bison_parser.cpp"
        break;

    case YYSYMBOL_FLOATVAL: /* FLOATVAL  */
#line 196 "bison_parser.y"
            { }
#line 2315 "bison_parser.cpp"
        break;

    case YYSYMBOL_INTVAL: /* INTVAL  */
#line 196 "bison_parser.y"
            { }
#line 2321 "bison_parser.cpp"
        break;

    case YYSYMBOL_DOLLAR_PARAM: /* DOLLAR_PARAM  */
#line 196 "bison_parser.y"
            { }
#line 2327 "bison_parser.cpp"
        break;

    case YYSYMBOL_NAMED_PARAM: /* NAMED_PARAM  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2333 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement_list: /* statement_list  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).stmt_vec)) {
    for (auto ptr : *(((*yyvaluep).stmt_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).stmt_vec));
}
#line 2346 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2352 "bison_parser.cpp"
        break;

    case YYSYMBOL_preparable_statement: /* preparable_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2358 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_hints: /* opt_hints  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2371 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint_list: /* hint_list  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2384 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint: /* hint  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2390 "bison_parser.cpp"
        break;

    case YYSYMBOL_transaction_statement: /* transaction_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).transaction_stmt)); }
#line 2396 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_statement: /* prepare_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).prep_stmt)); }
#line 2402 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_target_query: /* prepare_target_query  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2408 "bison_parser.cpp"
        break;

    case YYSYMBOL_execute_statement: /* execute_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2414 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_statement: /* import_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_stmt)); }
#line 2420 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_type: /* file_type  */
#line 196 "bison_parser.y"
            { }
#line 2426 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_path: /* file_path  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2432 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_import_export_options: /* opt_import_export_options  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2438 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_export_options: /* import_export_options  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2444 "bison_parser.cpp"
        break;

    case YYSYMBOL_csv_option: /* csv_option  */
#line 218 "bison_parser.y"
            {
  free(((*yyvaluep).csv_option_t)->second);
  delete (((*yyvaluep).csv_option_t));
}
#line 2453 "bison_parser.cpp"
        break;

    case YYSYMBOL_export_statement: /* export_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).export_stmt)); }
#line 2459 "bison_parser.cpp"
        break;

    case YYSYMBOL_show_statement: /* show_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).show_stmt)); }
#line 2465 "bison_parser.cpp"
        break;

    case YYSYMBOL_create_statement: /* create_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).create_stmt)); }
#line 2471 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_not_exists: /* opt_not_exists  */
#line 196 "bison_parser.y"
            { }
#line 2477 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem_commalist: /* table_elem_commalist  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).table_element_vec)) {
    for (auto ptr : *(((*yyvaluep).table_element_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_element_vec));
}
#line 2490 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem: /* table_elem  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table_element_t)); }
#line 2496 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_def: /* column_def  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).column_t)); }
#line 2502 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_type: /* column_type  */
#line 196 "bison_parser.y"
            { }
#line 2508 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_time_precision: /* opt_time_precision  */
#line 196 "bison_parser.y"
            { }
#line 2514 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_decimal_specification: /* opt_decimal_specification  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).ival_pair)); }
#line 2520 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_column_constraints: /* opt_column_constraints  */
#line 222 "bison_parser.y"
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
#line 2542 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraints: /* column_constraints  */
#line 222 "bison_parser.y"
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
#line 2564 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraint: /* column_constraint  */
#line 196 "bison_parser.y"
            { }
#line 2570 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_constraint: /* table_constraint  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table_constraint_t)); }
#line 2576 "bison_parser.cpp"
        break;

    case YYSYMBOL_references_spec: /* references_spec  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).references_spec_t)); }
#line 2582 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_statement: /* drop_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).drop_stmt)); }
#line 2588 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_exists: /* opt_exists  */
#line 196 "bison_parser.y"
            { }
#line 2594 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_statement: /* alter_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alter_stmt)); }
#line 2600 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_action: /* alter_action  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alter_action_t)); }
#line 2606 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_action: /* drop_action  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).drop_action_t)); }
#line 2612 "bison_parser.cpp"
        break;

    case YYSYMBOL_delete_statement: /* delete_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2618 "bison_parser.cpp"
        break;

    case YYSYMBOL_truncate_statement: /* truncate_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2624 "bison_parser.cpp"
        break;

    case YYSYMBOL_insert_statement: /* insert_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).insert_stmt)); }
#line 2630 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_column_list: /* opt_column_list  */
#line 201 "bison_parser.y"
            {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 2643 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_statement: /* update_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).update_stmt)); }
#line 2649 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause_commalist: /* update_clause_commalist  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).update_vec)) {
    for (auto ptr : *(((*yyvaluep).update_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).update_vec));
}
#line 2662 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause: /* update_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).update_t)); }
#line 2668 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_statement: /* select_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2674 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation: /* select_within_set_operation  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2680 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation_no_parentheses: /* select_within_set_operation_no_parentheses  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2686 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_with_paren: /* select_with_paren  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2692 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_no_paren: /* select_no_paren  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2698 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_operator: /* set_operator  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2704 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_type: /* set_type  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2710 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_all: /* opt_all  */
#line 196 "bison_parser.y"
            { }
#line 2716 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_start_with: /* opt_start_with  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2722 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_connect_by: /* opt_connect_by  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2728 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_clause: /* select_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2734 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_distinct: /* opt_distinct  */
#line 196 "bison_parser.y"
            { }
#line 2740 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_list: /* select_list  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2753 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_from_clause: /* opt_from_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2759 "bison_parser.cpp"
        break;

    case YYSYMBOL_from_clause: /* from_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2765 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_where: /* opt_where  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2771 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_group: /* opt_group  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).group_t)); }
#line 2777 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_having: /* opt_having  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2783 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_within_group: /* opt_within_group  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 2796 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order: /* opt_order  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 2809 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_list: /* order_list  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 2822 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_desc: /* order_desc  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).order)); }
#line 2828 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order_type: /* opt_order_type  */
#line 196 "bison_parser.y"
            { }
#line 2834 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_null_ordering: /* opt_null_ordering  */
#line 196 "bison_parser.y"
            { }
#line 2840 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_top: /* opt_top  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 2846 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_limit: /* opt_limit  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 2852 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_list: /* expr_list  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2865 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_extended_literal_list: /* opt_extended_literal_list  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2878 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal_list: /* extended_literal_list  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2891 "bison_parser.cpp"
        break;

    case YYSYMBOL_casted_extended_literal: /* casted_extended_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2897 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal: /* extended_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2903 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_alias: /* expr_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2909 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr: /* expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2915 "bison_parser.cpp"
        break;

    case YYSYMBOL_operand: /* operand  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2921 "bison_parser.cpp"
        break;

    case YYSYMBOL_scalar_expr: /* scalar_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2927 "bison_parser.cpp"
        break;

    case YYSYMBOL_unary_expr: /* unary_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2933 "bison_parser.cpp"
        break;

    case YYSYMBOL_binary_expr: /* binary_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2939 "bison_parser.cpp"
        break;

    case YYSYMBOL_logic_expr: /* logic_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2945 "bison_parser.cpp"
        break;

    case YYSYMBOL_in_expr: /* in_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2951 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_expr_list: /* row_expr_list  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2964 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_expr: /* case_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2970 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_list: /* case_list  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2976 "bison_parser.cpp"
        break;

    case YYSYMBOL_exists_expr: /* exists_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2982 "bison_parser.cpp"
        break;

    case YYSYMBOL_comp_expr: /* comp_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2988 "bison_parser.cpp"
        break;

    case YYSYMBOL_function_expr: /* function_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2994 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_window: /* opt_window  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).window_description)); }
#line 3000 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_partition: /* opt_partition  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3013 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_frame_clause: /* opt_frame_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).frame_description)); }
#line 3019 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_type: /* frame_type  */
#line 196 "bison_parser.y"
            { }
#line 3025 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_bound: /* frame_bound  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).frame_bound)); }
#line 3031 "bison_parser.cpp"
        break;

    case YYSYMBOL_extract_expr: /* extract_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3037 "bison_parser.cpp"
        break;

    case YYSYMBOL_cast_expr: /* cast_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3043 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field: /* datetime_field  */
#line 196 "bison_parser.y"
            { }
#line 3049 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field_plural: /* datetime_field_plural  */
#line 196 "bison_parser.y"
            { }
#line 3055 "bison_parser.cpp"
        break;

    case YYSYMBOL_duration_field: /* duration_field  */
#line 196 "bison_parser.y"
            { }
#line 3061 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_expr: /* array_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3067 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_index: /* array_index  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3073 "bison_parser.cpp"
        break;

    case YYSYMBOL_between_expr: /* between_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3079 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_name: /* column_name  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3085 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonreserved_keyword: /* nonreserved_keyword  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3091 "bison_parser.cpp"
        break;

    case YYSYMBOL_literal: /* literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3097 "bison_parser.cpp"
        break;

    case YYSYMBOL_string_literal: /* string_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3103 "bison_parser.cpp"
        break;

    case YYSYMBOL_bool_literal: /* bool_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3109 "bison_parser.cpp"
        break;

    case YYSYMBOL_num_literal: /* num_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3115 "bison_parser.cpp"
        break;

    case YYSYMBOL_int_literal: /* int_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3121 "bison_parser.cpp"
        break;

    case YYSYMBOL_null_literal: /* null_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3127 "bison_parser.cpp"
        break;

    case YYSYMBOL_date_literal: /* date_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3133 "bison_parser.cpp"
        break;

    case YYSYMBOL_interval_literal: /* interval_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3139 "bison_parser.cpp"
        break;

    case YYSYMBOL_param_expr: /* param_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3145 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref: /* table_ref  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3151 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_atomic: /* table_ref_atomic  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3157 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonjoin_table_ref_atomic: /* nonjoin_table_ref_atomic  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3163 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_value_row_list: /* table_value_row_list  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3176 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_commalist: /* table_ref_commalist  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).table_vec)) {
    for (auto ptr : *(((*yyvaluep).table_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_vec));
}
#line 3189 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name: /* table_ref_name  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3195 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name_no_alias: /* table_ref_name_no_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3201 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_name: /* table_name  */
#line 197 "bison_parser.y"
            {
  free(((*yyvaluep).table_name).name);
  free(((*yyvaluep).table_name).schema);
}
#line 3210 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_index_name: /* opt_index_name  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3216 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_alias: /* table_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3222 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_table_alias: /* opt_table_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3228 "bison_parser.cpp"
        break;

    case YYSYMBOL_alias: /* alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3234 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_alias: /* opt_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3240 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause: /* opt_locking_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3246 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause_list: /* opt_locking_clause_list  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3252 "bison_parser.cpp"
        break;

    case YYSYMBOL_locking_clause: /* locking_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_t)); }
#line 3258 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_lock_mode: /* row_lock_mode  */
#line 196 "bison_parser.y"
            { }
#line 3264 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_row_lock_policy: /* opt_row_lock_policy  */
#line 196 "bison_parser.y"
            { }
#line 3270 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_with_clause: /* opt_with_clause  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3283 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_clause: /* with_clause  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3296 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description_list: /* with_description_list  */
#line 210 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3309 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description: /* with_description  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).with_description_t)); }
#line 3315 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_clause: /* join_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3321 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_join_type: /* opt_join_type  */
#line 196 "bison_parser.y"
            { }
#line 3327 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_condition: /* join_condition  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3333 "bison_parser.cpp"
        break;

    case YYSYMBOL_ident_commalist: /* ident_commalist  */
#line 201 "bison_parser.y"
            {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 3346 "bison_parser.cpp"
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
#line 96 "bison_parser.y"
{
  // Initialize
  yylloc.first_column = 0;
  yylloc.last_column = 0;
  yylloc.first_line = 0;
  yylloc.last_line = 0;
  yylloc.total_column = 0;
  yylloc.string_length = 0;
}

#line 3454 "bison_parser.cpp"

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
#line 382 "bison_parser.y"
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
#line 3684 "bison_parser.cpp"
    break;

  case 3: /* statement_list: statement  */
#line 402 "bison_parser.y"
                           {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyval.stmt_vec) = new std::vector<SQLStatement*>();
  (yyval.stmt_vec)->push_back((yyvsp[0].statement));
}
#line 3695 "bison_parser.cpp"
    break;

  case 4: /* statement_list: statement_list ';' statement  */
#line 408 "bison_parser.y"
                               {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyvsp[-2].stmt_vec)->push_back((yyvsp[0].statement));
  (yyval.stmt_vec) = (yyvsp[-2].stmt_vec);
}
#line 3706 "bison_parser.cpp"
    break;

  case 5: /* statement: prepare_statement opt_hints  */
#line 415 "bison_parser.y"
                                        {
  (yyval.statement) = (yyvsp[-1].prep_stmt);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3715 "bison_parser.cpp"
    break;

  case 6: /* statement: preparable_statement opt_hints  */
#line 419 "bison_parser.y"
                                 {
  (yyval.statement) = (yyvsp[-1].statement);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3724 "bison_parser.cpp"
    break;

  case 7: /* statement: show_statement  */
#line 423 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].show_stmt); }
#line 3730 "bison_parser.cpp"
    break;

  case 8: /* statement: import_statement  */
#line 424 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].import_stmt); }
#line 3736 "bison_parser.cpp"
    break;

  case 9: /* statement: export_statement  */
#line 425 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].export_stmt); }
#line 3742 "bison_parser.cpp"
    break;

  case 10: /* preparable_statement: select_statement  */
#line 427 "bison_parser.y"
                                        { (yyval.statement) = (yyvsp[0].select_stmt); }
#line 3748 "bison_parser.cpp"
    break;

  case 11: /* preparable_statement: create_statement  */
#line 428 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].create_stmt); }
#line 3754 "bison_parser.cpp"
    break;

  case 12: /* preparable_statement: insert_statement  */
#line 429 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].insert_stmt); }
#line 3760 "bison_parser.cpp"
    break;

  case 13: /* preparable_statement: delete_statement  */
#line 430 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3766 "bison_parser.cpp"
    break;

  case 14: /* preparable_statement: truncate_statement  */
#line 431 "bison_parser.y"
                     { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3772 "bison_parser.cpp"
    break;

  case 15: /* preparable_statement: update_statement  */
#line 432 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].update_stmt); }
#line 3778 "bison_parser.cpp"
    break;

  case 16: /* preparable_statement: drop_statement  */
#line 433 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].drop_stmt); }
#line 3784 "bison_parser.cpp"
    break;

  case 17: /* preparable_statement: alter_statement  */
#line 434 "bison_parser.y"
                  { (yyval.statement) = (yyvsp[0].alter_stmt); }
#line 3790 "bison_parser.cpp"
    break;

  case 18: /* preparable_statement: execute_statement  */
#line 435 "bison_parser.y"
                    { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 3796 "bison_parser.cpp"
    break;

  case 19: /* preparable_statement: transaction_statement  */
#line 436 "bison_parser.y"
                        { (yyval.statement) = (yyvsp[0].transaction_stmt); }
#line 3802 "bison_parser.cpp"
    break;

  case 20: /* opt_hints: WITH HINT '(' hint_list ')'  */
#line 442 "bison_parser.y"
                                        { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 3808 "bison_parser.cpp"
    break;

  case 21: /* opt_hints: %empty  */
#line 443 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 3814 "bison_parser.cpp"
    break;

  case 22: /* hint_list: hint  */
#line 445 "bison_parser.y"
                 {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 3823 "bison_parser.cpp"
    break;

  case 23: /* hint_list: hint_list ',' hint  */
#line 449 "bison_parser.y"
                     {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 3832 "bison_parser.cpp"
    break;

  case 24: /* hint: IDENTIFIER  */
#line 454 "bison_parser.y"
                  {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[0].sval);
}
#line 3841 "bison_parser.cpp"
    break;

  case 25: /* hint: IDENTIFIER '(' extended_literal_list ')'  */
#line 458 "bison_parser.y"
                                           {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[-3].sval);
  (yyval.expr)->exprList = (yyvsp[-1].expr_vec);
}
#line 3851 "bison_parser.cpp"
    break;

  case 26: /* transaction_statement: BEGIN opt_transaction_keyword  */
#line 468 "bison_parser.y"
                                                      { (yyval.transaction_stmt) = new TransactionStatement(kBeginTransaction); }
#line 3857 "bison_parser.cpp"
    break;

  case 27: /* transaction_statement: ROLLBACK opt_transaction_keyword  */
#line 469 "bison_parser.y"
                                   { (yyval.transaction_stmt) = new TransactionStatement(kRollbackTransaction); }
#line 3863 "bison_parser.cpp"
    break;

  case 28: /* transaction_statement: COMMIT opt_transaction_keyword  */
#line 470 "bison_parser.y"
                                 { (yyval.transaction_stmt) = new TransactionStatement(kCommitTransaction); }
#line 3869 "bison_parser.cpp"
    break;

  case 31: /* prepare_statement: PREPARE IDENTIFIER FROM prepare_target_query  */
#line 478 "bison_parser.y"
                                                                 {
  (yyval.prep_stmt) = new PrepareStatement();
  (yyval.prep_stmt)->name = (yyvsp[-2].sval);
  (yyval.prep_stmt)->query = (yyvsp[0].sval);
}
#line 3879 "bison_parser.cpp"
    break;

  case 33: /* execute_statement: EXECUTE IDENTIFIER  */
#line 486 "bison_parser.y"
                                       {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[0].sval);
}
#line 3888 "bison_parser.cpp"
    break;

  case 34: /* execute_statement: EXECUTE IDENTIFIER '(' opt_extended_literal_list ')'  */
#line 490 "bison_parser.y"
                                                       {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[-3].sval);
  (yyval.exec_stmt)->parameters = (yyvsp[-1].expr_vec);
}
#line 3898 "bison_parser.cpp"
    break;

  case 35: /* import_statement: IMPORT FROM file_type FILE file_path INTO table_name  */
#line 502 "bison_parser.y"
                                                                        {
  (yyval.import_stmt) = new ImportStatement((yyvsp[-4].import_type_t));
  (yyval.import_stmt)->filePath = (yyvsp[-2].sval);
  (yyval.import_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.import_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 3909 "bison_parser.cpp"
    break;

  case 36: /* import_statement: COPY table_name FROM file_path opt_import_export_options opt_where  */
#line 508 "bison_parser.y"
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
#line 3930 "bison_parser.cpp"
    break;

  case 37: /* file_type: IDENTIFIER  */
#line 525 "bison_parser.y"
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
#line 3949 "bison_parser.cpp"
    break;

  case 38: /* file_path: STRING  */
#line 540 "bison_parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 3955 "bison_parser.cpp"
    break;

  case 39: /* opt_import_export_options: WITH '(' import_export_options ')'  */
#line 542 "bison_parser.y"
                                                               { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 3961 "bison_parser.cpp"
    break;

  case 40: /* opt_import_export_options: '(' import_export_options ')'  */
#line 543 "bison_parser.y"
                                { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 3967 "bison_parser.cpp"
    break;

  case 41: /* opt_import_export_options: %empty  */
#line 544 "bison_parser.y"
              { (yyval.import_export_option_t) = new ImportExportOptions{}; }
#line 3973 "bison_parser.cpp"
    break;

  case 42: /* import_export_options: import_export_options ',' FORMAT file_type  */
#line 546 "bison_parser.y"
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
#line 3992 "bison_parser.cpp"
    break;

  case 43: /* import_export_options: FORMAT file_type  */
#line 560 "bison_parser.y"
                   {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->format = (yyvsp[0].import_type_t);
}
#line 4001 "bison_parser.cpp"
    break;

  case 44: /* import_export_options: import_export_options ',' ENCODING STRING  */
#line 564 "bison_parser.y"
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
#line 4016 "bison_parser.cpp"
    break;

  case 45: /* import_export_options: ENCODING STRING  */
#line 574 "bison_parser.y"
                  {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->encoding = (yyvsp[0].sval);
}
#line 4025 "bison_parser.cpp"
    break;

  case 46: /* import_export_options: import_export_options ',' csv_option  */
#line 578 "bison_parser.y"
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
#line 4054 "bison_parser.cpp"
    break;

  case 47: /* import_export_options: csv_option  */
#line 602 "bison_parser.y"
             {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->csv_options = new CsvOptions{};
  (yyval.import_export_option_t)->csv_options->accept_csv_option((yyvsp[0].csv_option_t));

  delete (yyvsp[0].csv_option_t);
}
#line 4066 "bison_parser.cpp"
    break;

  case 48: /* csv_option: IDENTIFIER STRING  */
#line 610 "bison_parser.y"
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
#line 4084 "bison_parser.cpp"
    break;

  case 49: /* csv_option: NULL STRING  */
#line 623 "bison_parser.y"
              { (yyval.csv_option_t) = new std::pair<CsvOptionType, char*>(CsvOptionType::Null, (yyvsp[0].sval)); }
#line 4090 "bison_parser.cpp"
    break;

  case 50: /* export_statement: COPY table_name TO file_path opt_import_export_options  */
#line 630 "bison_parser.y"
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
#line 4110 "bison_parser.cpp"
    break;

  case 51: /* export_statement: COPY select_with_paren TO file_path opt_import_export_options  */
#line 645 "bison_parser.y"
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
#line 4129 "bison_parser.cpp"
    break;

  case 52: /* show_statement: SHOW TABLES  */
#line 665 "bison_parser.y"
                             { (yyval.show_stmt) = new ShowStatement(kShowTables); }
#line 4135 "bison_parser.cpp"
    break;

  case 53: /* show_statement: SHOW COLUMNS table_name  */
#line 666 "bison_parser.y"
                          {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4145 "bison_parser.cpp"
    break;

  case 54: /* show_statement: DESCRIBE table_name  */
#line 671 "bison_parser.y"
                      {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4155 "bison_parser.cpp"
    break;

  case 55: /* create_statement: CREATE TABLE opt_not_exists table_name FROM IDENTIFIER FILE file_path  */
#line 682 "bison_parser.y"
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
#line 4173 "bison_parser.cpp"
    break;

  case 56: /* create_statement: CREATE TABLE opt_not_exists table_name '(' table_elem_commalist ')'  */
#line 695 "bison_parser.y"
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
#line 4190 "bison_parser.cpp"
    break;

  case 57: /* create_statement: CREATE TABLE opt_not_exists table_name AS select_statement  */
#line 707 "bison_parser.y"
                                                             {
  (yyval.create_stmt) = new CreateStatement(kCreateTable);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-3].bval);
  (yyval.create_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4202 "bison_parser.cpp"
    break;

  case 58: /* create_statement: CREATE INDEX opt_not_exists opt_index_name ON table_name '(' ident_commalist ')'  */
#line 714 "bison_parser.y"
                                                                                   {
  (yyval.create_stmt) = new CreateStatement(kCreateIndex);
  (yyval.create_stmt)->indexName = (yyvsp[-5].sval);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-6].bval);
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->indexColumns = (yyvsp[-1].str_vec);
}
#line 4214 "bison_parser.cpp"
    break;

  case 59: /* create_statement: CREATE VIEW opt_not_exists table_name opt_column_list AS select_statement  */
#line 721 "bison_parser.y"
                                                                            {
  (yyval.create_stmt) = new CreateStatement(kCreateView);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-4].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->viewColumns = (yyvsp[-2].str_vec);
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4227 "bison_parser.cpp"
    break;

  case 60: /* opt_not_exists: IF NOT EXISTS  */
#line 730 "bison_parser.y"
                               { (yyval.bval) = true; }
#line 4233 "bison_parser.cpp"
    break;

  case 61: /* opt_not_exists: %empty  */
#line 731 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4239 "bison_parser.cpp"
    break;

  case 62: /* table_elem_commalist: table_elem  */
#line 733 "bison_parser.y"
                                  {
  (yyval.table_element_vec) = new std::vector<TableElement*>();
  (yyval.table_element_vec)->push_back((yyvsp[0].table_element_t));
}
#line 4248 "bison_parser.cpp"
    break;

  case 63: /* table_elem_commalist: table_elem_commalist ',' table_elem  */
#line 737 "bison_parser.y"
                                      {
  (yyvsp[-2].table_element_vec)->push_back((yyvsp[0].table_element_t));
  (yyval.table_element_vec) = (yyvsp[-2].table_element_vec);
}
#line 4257 "bison_parser.cpp"
    break;

  case 64: /* table_elem: column_def  */
#line 742 "bison_parser.y"
                        { (yyval.table_element_t) = (yyvsp[0].column_t); }
#line 4263 "bison_parser.cpp"
    break;

  case 65: /* table_elem: table_constraint  */
#line 743 "bison_parser.y"
                   { (yyval.table_element_t) = (yyvsp[0].table_constraint_t); }
#line 4269 "bison_parser.cpp"
    break;

  case 66: /* column_def: IDENTIFIER column_type opt_column_constraints  */
#line 745 "bison_parser.y"
                                                           {
  (yyval.column_t) = new ColumnDefinition((yyvsp[-2].sval), (yyvsp[-1].column_type_t), (yyvsp[0].column_constraints_t)->constraints, (yyvsp[0].column_constraints_t)->references);
  if (!(yyval.column_t)->trySetNullableExplicit()) {
    yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{(yyvsp[-2].sval)}).c_str());
  }
  delete (yyvsp[0].column_constraints_t);
}
#line 4281 "bison_parser.cpp"
    break;

  case 67: /* column_type: BIGINT  */
#line 753 "bison_parser.y"
                     { (yyval.column_type_t) = ColumnType{DataType::BIGINT}; }
#line 4287 "bison_parser.cpp"
    break;

  case 68: /* column_type: BOOLEAN  */
#line 754 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::BOOLEAN}; }
#line 4293 "bison_parser.cpp"
    break;

  case 69: /* column_type: CHAR '(' INTVAL ')'  */
#line 755 "bison_parser.y"
                      { (yyval.column_type_t) = ColumnType{DataType::CHAR, (yyvsp[-1].ival)}; }
#line 4299 "bison_parser.cpp"
    break;

  case 70: /* column_type: CHARACTER_VARYING '(' INTVAL ')'  */
#line 756 "bison_parser.y"
                                   { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4305 "bison_parser.cpp"
    break;

  case 71: /* column_type: DATE  */
#line 757 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::DATE}; }
#line 4311 "bison_parser.cpp"
    break;

  case 72: /* column_type: DATETIME  */
#line 758 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4317 "bison_parser.cpp"
    break;

  case 73: /* column_type: DECIMAL opt_decimal_specification  */
#line 759 "bison_parser.y"
                                    {
  (yyval.column_type_t) = ColumnType{DataType::DECIMAL, 0, (yyvsp[0].ival_pair)->first, (yyvsp[0].ival_pair)->second};
  delete (yyvsp[0].ival_pair);
}
#line 4326 "bison_parser.cpp"
    break;

  case 74: /* column_type: DOUBLE  */
#line 763 "bison_parser.y"
         { (yyval.column_type_t) = ColumnType{DataType::DOUBLE}; }
#line 4332 "bison_parser.cpp"
    break;

  case 75: /* column_type: FLOAT  */
#line 764 "bison_parser.y"
        { (yyval.column_type_t) = ColumnType{DataType::FLOAT}; }
#line 4338 "bison_parser.cpp"
    break;

  case 76: /* column_type: INT  */
#line 765 "bison_parser.y"
      { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4344 "bison_parser.cpp"
    break;

  case 77: /* column_type: INTEGER  */
#line 766 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4350 "bison_parser.cpp"
    break;

  case 78: /* column_type: LONG  */
#line 767 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::LONG}; }
#line 4356 "bison_parser.cpp"
    break;

  case 79: /* column_type: REAL  */
#line 768 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::REAL}; }
#line 4362 "bison_parser.cpp"
    break;

  case 80: /* column_type: SMALLINT  */
#line 769 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::SMALLINT}; }
#line 4368 "bison_parser.cpp"
    break;

  case 81: /* column_type: TEXT  */
#line 770 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::TEXT}; }
#line 4374 "bison_parser.cpp"
    break;

  case 82: /* column_type: TIME opt_time_precision  */
#line 771 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::TIME, 0, (yyvsp[0].ival)}; }
#line 4380 "bison_parser.cpp"
    break;

  case 83: /* column_type: TIMESTAMP  */
#line 772 "bison_parser.y"
            { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4386 "bison_parser.cpp"
    break;

  case 84: /* column_type: VARCHAR '(' INTVAL ')'  */
#line 773 "bison_parser.y"
                         { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4392 "bison_parser.cpp"
    break;

  case 85: /* opt_time_precision: '(' INTVAL ')'  */
#line 775 "bison_parser.y"
                                    { (yyval.ival) = (yyvsp[-1].ival); }
#line 4398 "bison_parser.cpp"
    break;

  case 86: /* opt_time_precision: %empty  */
#line 776 "bison_parser.y"
              { (yyval.ival) = 0; }
#line 4404 "bison_parser.cpp"
    break;

  case 87: /* opt_decimal_specification: '(' INTVAL ',' INTVAL ')'  */
#line 778 "bison_parser.y"
                                                      { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-3].ival), (yyvsp[-1].ival)}; }
#line 4410 "bison_parser.cpp"
    break;

  case 88: /* opt_decimal_specification: '(' INTVAL ')'  */
#line 779 "bison_parser.y"
                 { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-1].ival), 0}; }
#line 4416 "bison_parser.cpp"
    break;

  case 89: /* opt_decimal_specification: %empty  */
#line 780 "bison_parser.y"
              { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{0, 0}; }
#line 4422 "bison_parser.cpp"
    break;

  case 90: /* opt_column_constraints: column_constraints  */
#line 782 "bison_parser.y"
                                            { (yyval.column_constraints_t) = (yyvsp[0].column_constraints_t); }
#line 4428 "bison_parser.cpp"
    break;

  case 91: /* opt_column_constraints: %empty  */
#line 783 "bison_parser.y"
              { (yyval.column_constraints_t) = new ColumnConstraints(); }
#line 4434 "bison_parser.cpp"
    break;

  case 92: /* column_constraints: column_constraint  */
#line 785 "bison_parser.y"
                                       {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
}
#line 4443 "bison_parser.cpp"
    break;

  case 93: /* column_constraints: column_constraints column_constraint  */
#line 789 "bison_parser.y"
                                       {
  (yyvsp[-1].column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4452 "bison_parser.cpp"
    break;

  case 94: /* column_constraints: references_spec  */
#line 793 "bison_parser.y"
                  {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyval.column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
}
#line 4462 "bison_parser.cpp"
    break;

  case 95: /* column_constraints: column_constraints references_spec  */
#line 798 "bison_parser.y"
                                     {
  // Multiple foreign keys for the same column could be possible, so we do not raise an error in that case.
  // Think of foreign keys referenced on multiple levels (returned item references sold item references items).
  (yyvsp[-1].column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyvsp[-1].column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4474 "bison_parser.cpp"
    break;

  case 96: /* column_constraint: PRIMARY KEY  */
#line 806 "bison_parser.y"
                                { (yyval.column_constraint_t) = ConstraintType::PrimaryKey; }
#line 4480 "bison_parser.cpp"
    break;

  case 97: /* column_constraint: UNIQUE  */
#line 807 "bison_parser.y"
         { (yyval.column_constraint_t) = ConstraintType::Unique; }
#line 4486 "bison_parser.cpp"
    break;

  case 98: /* column_constraint: NULL  */
#line 808 "bison_parser.y"
       { (yyval.column_constraint_t) = ConstraintType::Null; }
#line 4492 "bison_parser.cpp"
    break;

  case 99: /* column_constraint: NOT NULL  */
#line 809 "bison_parser.y"
           { (yyval.column_constraint_t) = ConstraintType::NotNull; }
#line 4498 "bison_parser.cpp"
    break;

  case 100: /* table_constraint: PRIMARY KEY '(' ident_commalist ')'  */
#line 811 "bison_parser.y"
                                                       { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::PrimaryKey, (yyvsp[-1].str_vec)); }
#line 4504 "bison_parser.cpp"
    break;

  case 101: /* table_constraint: UNIQUE '(' ident_commalist ')'  */
#line 812 "bison_parser.y"
                                 { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::Unique, (yyvsp[-1].str_vec)); }
#line 4510 "bison_parser.cpp"
    break;

  case 102: /* table_constraint: FOREIGN KEY '(' ident_commalist ')' references_spec  */
#line 813 "bison_parser.y"
                                                      { (yyval.table_constraint_t) = new ForeignKeyConstraint((yyvsp[-2].str_vec), (yyvsp[0].references_spec_t)); }
#line 4516 "bison_parser.cpp"
    break;

  case 103: /* references_spec: REFERENCES table_name opt_column_list  */
#line 815 "bison_parser.y"
                                                        { (yyval.references_spec_t) = new ReferencesSpecification((yyvsp[-1].table_name).schema, (yyvsp[-1].table_name).name, (yyvsp[0].str_vec)); }
#line 4522 "bison_parser.cpp"
    break;

  case 104: /* drop_statement: DROP TABLE opt_exists table_name  */
#line 823 "bison_parser.y"
                                                  {
  (yyval.drop_stmt) = new DropStatement(kDropTable);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4533 "bison_parser.cpp"
    break;

  case 105: /* drop_statement: DROP VIEW opt_exists table_name  */
#line 829 "bison_parser.y"
                                  {
  (yyval.drop_stmt) = new DropStatement(kDropView);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4544 "bison_parser.cpp"
    break;

  case 106: /* drop_statement: DEALLOCATE PREPARE IDENTIFIER  */
#line 835 "bison_parser.y"
                                {
  (yyval.drop_stmt) = new DropStatement(kDropPreparedStatement);
  (yyval.drop_stmt)->ifExists = false;
  (yyval.drop_stmt)->name = (yyvsp[0].sval);
}
#line 4554 "bison_parser.cpp"
    break;

  case 107: /* drop_statement: DROP INDEX opt_exists IDENTIFIER  */
#line 841 "bison_parser.y"
                                   {
  (yyval.drop_stmt) = new DropStatement(kDropIndex);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->indexName = (yyvsp[0].sval);
}
#line 4564 "bison_parser.cpp"
    break;

  case 108: /* opt_exists: IF EXISTS  */
#line 847 "bison_parser.y"
                       { (yyval.bval) = true; }
#line 4570 "bison_parser.cpp"
    break;

  case 109: /* opt_exists: %empty  */
#line 848 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4576 "bison_parser.cpp"
    break;

  case 110: /* alter_statement: ALTER TABLE opt_exists table_name alter_action  */
#line 855 "bison_parser.y"
                                                                 {
  (yyval.alter_stmt) = new AlterStatement((yyvsp[-1].table_name).name, (yyvsp[0].alter_action_t));
  (yyval.alter_stmt)->ifTableExists = (yyvsp[-2].bval);
  (yyval.alter_stmt)->schema = (yyvsp[-1].table_name).schema;
}
#line 4586 "bison_parser.cpp"
    break;

  case 111: /* alter_action: drop_action  */
#line 861 "bison_parser.y"
                           { (yyval.alter_action_t) = (yyvsp[0].drop_action_t); }
#line 4592 "bison_parser.cpp"
    break;

  case 112: /* drop_action: DROP COLUMN opt_exists IDENTIFIER  */
#line 863 "bison_parser.y"
                                                {
  (yyval.drop_action_t) = new DropColumnAction((yyvsp[0].sval));
  (yyval.drop_action_t)->ifExists = (yyvsp[-1].bval);
}
#line 4601 "bison_parser.cpp"
    break;

  case 113: /* delete_statement: DELETE FROM table_name opt_where  */
#line 873 "bison_parser.y"
                                                    {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[-1].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[-1].table_name).name;
  (yyval.delete_stmt)->expr = (yyvsp[0].expr);
}
#line 4612 "bison_parser.cpp"
    break;

  case 114: /* truncate_statement: TRUNCATE table_name  */
#line 880 "bison_parser.y"
                                         {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 4622 "bison_parser.cpp"
    break;

  case 115: /* insert_statement: INSERT INTO table_name opt_column_list VALUES '(' extended_literal_list ')'  */
#line 891 "bison_parser.y"
                                                                                               {
  (yyval.insert_stmt) = new InsertStatement(kInsertValues);
  (yyval.insert_stmt)->schema = (yyvsp[-5].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-5].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-4].str_vec);
  (yyval.insert_stmt)->values = (yyvsp[-1].expr_vec);
}
#line 4634 "bison_parser.cpp"
    break;

  case 116: /* insert_statement: INSERT INTO table_name opt_column_list select_no_paren  */
#line 898 "bison_parser.y"
                                                         {
  (yyval.insert_stmt) = new InsertStatement(kInsertSelect);
  (yyval.insert_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-1].str_vec);
  (yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4646 "bison_parser.cpp"
    break;

  case 117: /* opt_column_list: '(' ident_commalist ')'  */
#line 906 "bison_parser.y"
                                          { (yyval.str_vec) = (yyvsp[-1].str_vec); }
#line 4652 "bison_parser.cpp"
    break;

  case 118: /* opt_column_list: %empty  */
#line 907 "bison_parser.y"
              { (yyval.str_vec) = nullptr; }
#line 4658 "bison_parser.cpp"
    break;

  case 119: /* update_statement: UPDATE table_ref_name_no_alias SET update_clause_commalist opt_where  */
#line 914 "bison_parser.y"
                                                                                        {
  (yyval.update_stmt) = new UpdateStatement();
  (yyval.update_stmt)->table = (yyvsp[-3].table);
  (yyval.update_stmt)->updates = (yyvsp[-1].update_vec);
  (yyval.update_stmt)->where = (yyvsp[0].expr);
}
#line 4669 "bison_parser.cpp"
    break;

  case 120: /* update_clause_commalist: update_clause  */
#line 921 "bison_parser.y"
                                        {
  (yyval.update_vec) = new std::vector<UpdateClause*>();
  (yyval.update_vec)->push_back((yyvsp[0].update_t));
}
#line 4678 "bison_parser.cpp"
    break;

  case 121: /* update_clause_commalist: update_clause_commalist ',' update_clause  */
#line 925 "bison_parser.y"
                                            {
  (yyvsp[-2].update_vec)->push_back((yyvsp[0].update_t));
  (yyval.update_vec) = (yyvsp[-2].update_vec);
}
#line 4687 "bison_parser.cpp"
    break;

  case 122: /* update_clause: IDENTIFIER '=' expr  */
#line 930 "bison_parser.y"
                                    {
  (yyval.update_t) = new UpdateClause();
  (yyval.update_t)->column = (yyvsp[-2].sval);
  (yyval.update_t)->value = (yyvsp[0].expr);
}
#line 4697 "bison_parser.cpp"
    break;

  case 123: /* select_statement: opt_with_clause select_with_paren  */
#line 940 "bison_parser.y"
                                                     {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4706 "bison_parser.cpp"
    break;

  case 124: /* select_statement: opt_with_clause select_no_paren  */
#line 944 "bison_parser.y"
                                  {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4715 "bison_parser.cpp"
    break;

  case 125: /* select_statement: opt_with_clause select_with_paren set_operator select_within_set_operation opt_order opt_limit  */
#line 948 "bison_parser.y"
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
#line 4731 "bison_parser.cpp"
    break;

  case 128: /* select_within_set_operation_no_parentheses: select_clause  */
#line 962 "bison_parser.y"
                                                           { (yyval.select_stmt) = (yyvsp[0].select_stmt); }
#line 4737 "bison_parser.cpp"
    break;

  case 129: /* select_within_set_operation_no_parentheses: select_clause set_operator select_within_set_operation  */
#line 963 "bison_parser.y"
                                                         {
  (yyval.select_stmt) = (yyvsp[-2].select_stmt);
  if ((yyval.select_stmt)->setOperations == nullptr) {
    (yyval.select_stmt)->setOperations = new std::vector<SetOperation*>();
  }
  (yyval.select_stmt)->setOperations->push_back((yyvsp[-1].set_operator_t));
  (yyval.select_stmt)->setOperations->back()->nestedSelectStatement = (yyvsp[0].select_stmt);
}
#line 4750 "bison_parser.cpp"
    break;

  case 130: /* select_with_paren: '(' select_no_paren ')'  */
#line 972 "bison_parser.y"
                                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4756 "bison_parser.cpp"
    break;

  case 131: /* select_with_paren: '(' select_with_paren ')'  */
#line 973 "bison_parser.y"
                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4762 "bison_parser.cpp"
    break;

  case 132: /* select_no_paren: select_clause opt_order opt_limit opt_locking_clause  */
#line 975 "bison_parser.y"
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
#line 4781 "bison_parser.cpp"
    break;

  case 133: /* select_no_paren: select_clause set_operator select_within_set_operation opt_order opt_limit opt_locking_clause  */
#line 989 "bison_parser.y"
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
#line 4797 "bison_parser.cpp"
    break;

  case 134: /* set_operator: set_type opt_all  */
#line 1001 "bison_parser.y"
                                {
  (yyval.set_operator_t) = (yyvsp[-1].set_operator_t);
  (yyval.set_operator_t)->isAll = (yyvsp[0].bval);
}
#line 4806 "bison_parser.cpp"
    break;

  case 135: /* set_type: UNION  */
#line 1006 "bison_parser.y"
                 {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetUnion;
}
#line 4815 "bison_parser.cpp"
    break;

  case 136: /* set_type: INTERSECT  */
#line 1010 "bison_parser.y"
            {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetIntersect;
}
#line 4824 "bison_parser.cpp"
    break;

  case 137: /* set_type: EXCEPT  */
#line 1014 "bison_parser.y"
         {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetExcept;
}
#line 4833 "bison_parser.cpp"
    break;

  case 138: /* opt_all: ALL  */
#line 1019 "bison_parser.y"
              { (yyval.bval) = true; }
#line 4839 "bison_parser.cpp"
    break;

  case 139: /* opt_all: %empty  */
#line 1020 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4845 "bison_parser.cpp"
    break;

  case 140: /* opt_start_with: START WITH expr  */
#line 1024 "bison_parser.y"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 4851 "bison_parser.cpp"
    break;

  case 141: /* opt_start_with: %empty  */
#line 1025 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4857 "bison_parser.cpp"
    break;

  case 142: /* opt_connect_by: CONNECT BY expr  */
#line 1027 "bison_parser.y"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 4863 "bison_parser.cpp"
    break;

  case 143: /* opt_connect_by: %empty  */
#line 1028 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4869 "bison_parser.cpp"
    break;

  case 144: /* select_clause: SELECT opt_top opt_distinct select_list opt_from_clause opt_where opt_start_with opt_connect_by opt_group  */
#line 1030 "bison_parser.y"
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
#line 4885 "bison_parser.cpp"
    break;

  case 145: /* opt_distinct: DISTINCT  */
#line 1042 "bison_parser.y"
                        { (yyval.bval) = true; }
#line 4891 "bison_parser.cpp"
    break;

  case 146: /* opt_distinct: %empty  */
#line 1043 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4897 "bison_parser.cpp"
    break;

  case 148: /* opt_from_clause: from_clause  */
#line 1047 "bison_parser.y"
                              { (yyval.table) = (yyvsp[0].table); }
#line 4903 "bison_parser.cpp"
    break;

  case 149: /* opt_from_clause: %empty  */
#line 1048 "bison_parser.y"
              { (yyval.table) = nullptr; }
#line 4909 "bison_parser.cpp"
    break;

  case 150: /* from_clause: FROM table_ref  */
#line 1050 "bison_parser.y"
                             { (yyval.table) = (yyvsp[0].table); }
#line 4915 "bison_parser.cpp"
    break;

  case 151: /* opt_where: WHERE expr  */
#line 1052 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 4921 "bison_parser.cpp"
    break;

  case 152: /* opt_where: %empty  */
#line 1053 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4927 "bison_parser.cpp"
    break;

  case 153: /* opt_group: GROUP BY expr_list opt_having  */
#line 1055 "bison_parser.y"
                                          {
  (yyval.group_t) = new GroupByDescription();
  (yyval.group_t)->columns = (yyvsp[-1].expr_vec);
  (yyval.group_t)->having = (yyvsp[0].expr);
}
#line 4937 "bison_parser.cpp"
    break;

  case 154: /* opt_group: %empty  */
#line 1060 "bison_parser.y"
              { (yyval.group_t) = nullptr; }
#line 4943 "bison_parser.cpp"
    break;

  case 155: /* opt_having: HAVING expr  */
#line 1062 "bison_parser.y"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 4949 "bison_parser.cpp"
    break;

  case 156: /* opt_having: %empty  */
#line 1063 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4955 "bison_parser.cpp"
    break;

  case 157: /* opt_within_group: WITHIN GROUP '(' ORDER BY order_list ')'  */
#line 1068 "bison_parser.y"
                                                            { (yyval.order_vec) = (yyvsp[-1].order_vec); }
#line 4961 "bison_parser.cpp"
    break;

  case 158: /* opt_within_group: %empty  */
#line 1069 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 4967 "bison_parser.cpp"
    break;

  case 159: /* opt_order: ORDER BY order_list  */
#line 1071 "bison_parser.y"
                                { (yyval.order_vec) = (yyvsp[0].order_vec); }
#line 4973 "bison_parser.cpp"
    break;

  case 160: /* opt_order: %empty  */
#line 1072 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 4979 "bison_parser.cpp"
    break;

  case 161: /* order_list: order_desc  */
#line 1074 "bison_parser.y"
                        {
  (yyval.order_vec) = new std::vector<OrderDescription*>();
  (yyval.order_vec)->push_back((yyvsp[0].order));
}
#line 4988 "bison_parser.cpp"
    break;

  case 162: /* order_list: order_list ',' order_desc  */
#line 1078 "bison_parser.y"
                            {
  (yyvsp[-2].order_vec)->push_back((yyvsp[0].order));
  (yyval.order_vec) = (yyvsp[-2].order_vec);
}
#line 4997 "bison_parser.cpp"
    break;

  case 163: /* order_desc: expr opt_order_type opt_null_ordering  */
#line 1083 "bison_parser.y"
                                                   { (yyval.order) = new OrderDescription((yyvsp[-1].order_type), (yyvsp[-2].expr), (yyvsp[0].null_ordering_t)); }
#line 5003 "bison_parser.cpp"
    break;

  case 164: /* opt_order_type: ASC  */
#line 1085 "bison_parser.y"
                     { (yyval.order_type) = kOrderAsc; }
#line 5009 "bison_parser.cpp"
    break;

  case 165: /* opt_order_type: DESC  */
#line 1086 "bison_parser.y"
       { (yyval.order_type) = kOrderDesc; }
#line 5015 "bison_parser.cpp"
    break;

  case 166: /* opt_order_type: %empty  */
#line 1087 "bison_parser.y"
              { (yyval.order_type) = kOrderAsc; }
#line 5021 "bison_parser.cpp"
    break;

  case 167: /* opt_null_ordering: %empty  */
#line 1089 "bison_parser.y"
                                { (yyval.null_ordering_t) = NullOrdering::Undefined; }
#line 5027 "bison_parser.cpp"
    break;

  case 168: /* opt_null_ordering: IDENTIFIER IDENTIFIER  */
#line 1090 "bison_parser.y"
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
#line 5051 "bison_parser.cpp"
    break;

  case 169: /* opt_top: TOP int_literal  */
#line 1112 "bison_parser.y"
                          { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 5057 "bison_parser.cpp"
    break;

  case 170: /* opt_top: TOP '(' int_literal ')'  */
#line 1113 "bison_parser.y"
                          { (yyval.limit) = new LimitDescription((yyvsp[-1].expr), nullptr); }
#line 5063 "bison_parser.cpp"
    break;

  case 171: /* opt_top: %empty  */
#line 1114 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 5069 "bison_parser.cpp"
    break;

  case 172: /* opt_limit: LIMIT expr  */
#line 1116 "bison_parser.y"
                       { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 5075 "bison_parser.cpp"
    break;

  case 173: /* opt_limit: OFFSET expr  */
#line 1117 "bison_parser.y"
              { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 5081 "bison_parser.cpp"
    break;

  case 174: /* opt_limit: LIMIT expr OFFSET expr  */
#line 1118 "bison_parser.y"
                         { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 5087 "bison_parser.cpp"
    break;

  case 175: /* opt_limit: LIMIT ALL  */
#line 1119 "bison_parser.y"
            { (yyval.limit) = new LimitDescription(nullptr, nullptr); }
#line 5093 "bison_parser.cpp"
    break;

  case 176: /* opt_limit: LIMIT ALL OFFSET expr  */
#line 1120 "bison_parser.y"
                        { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 5099 "bison_parser.cpp"
    break;

  case 177: /* opt_limit: OFFSET expr ROWS FETCH NEXT expr ROWS ONLY  */
#line 1121 "bison_parser.y"
                                             { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[-6].expr)); }
#line 5105 "bison_parser.cpp"
    break;

  case 178: /* opt_limit: %empty  */
#line 1122 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 5111 "bison_parser.cpp"
    break;

  case 179: /* expr_list: expr_alias  */
#line 1127 "bison_parser.y"
                       {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5120 "bison_parser.cpp"
    break;

  case 180: /* expr_list: expr_list ',' expr_alias  */
#line 1131 "bison_parser.y"
                           {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5129 "bison_parser.cpp"
    break;

  case 181: /* opt_extended_literal_list: extended_literal_list  */
#line 1137 "bison_parser.y"
                                                  { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5135 "bison_parser.cpp"
    break;

  case 182: /* opt_extended_literal_list: %empty  */
#line 1138 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5141 "bison_parser.cpp"
    break;

  case 183: /* extended_literal_list: casted_extended_literal  */
#line 1140 "bison_parser.y"
                                                {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5150 "bison_parser.cpp"
    break;

  case 184: /* extended_literal_list: extended_literal_list ',' casted_extended_literal  */
#line 1144 "bison_parser.y"
                                                    {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5159 "bison_parser.cpp"
    break;

  case 186: /* casted_extended_literal: CAST '(' extended_literal AS column_type ')'  */
#line 1149 "bison_parser.y"
                                                                                          {
  (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t));
}
#line 5167 "bison_parser.cpp"
    break;

  case 187: /* extended_literal: literal  */
#line 1153 "bison_parser.y"
                           { (yyval.expr) = (yyvsp[0].expr); }
#line 5173 "bison_parser.cpp"
    break;

  case 188: /* extended_literal: '-' num_literal  */
#line 1154 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5179 "bison_parser.cpp"
    break;

  case 189: /* extended_literal: '-' interval_literal  */
#line 1155 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5185 "bison_parser.cpp"
    break;

  case 190: /* expr_alias: expr opt_alias  */
#line 1157 "bison_parser.y"
                            {
  (yyval.expr) = (yyvsp[-1].expr);
  if ((yyvsp[0].alias_t)) {
    (yyval.expr)->alias = (yyvsp[0].alias_t)->name;
    (yyvsp[0].alias_t)->name = nullptr;
    delete (yyvsp[0].alias_t);
  }
}
#line 5198 "bison_parser.cpp"
    break;

  case 196: /* operand: '(' expr ')'  */
#line 1168 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[-1].expr); }
#line 5204 "bison_parser.cpp"
    break;

  case 206: /* operand: '(' select_no_paren ')'  */
#line 1170 "bison_parser.y"
                                         {
  (yyval.expr) = Expr::makeSelect((yyvsp[-1].select_stmt));
}
#line 5212 "bison_parser.cpp"
    break;

  case 209: /* scalar_expr: column_name OUTERJOIN  */
#line 1178 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpUnary(kOpOuterJoin, (yyvsp[-1].expr)); }
#line 5218 "bison_parser.cpp"
    break;

  case 210: /* unary_expr: '-' operand  */
#line 1180 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5224 "bison_parser.cpp"
    break;

  case 211: /* unary_expr: PRIOR operand  */
#line 1182 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpUnary(kOpPrior, (yyvsp[0].expr)); }
#line 5230 "bison_parser.cpp"
    break;

  case 212: /* unary_expr: NOT operand  */
#line 1183 "bison_parser.y"
              { (yyval.expr) = Expr::makeOpUnary(kOpNot, (yyvsp[0].expr)); }
#line 5236 "bison_parser.cpp"
    break;

  case 213: /* unary_expr: operand ISNULL  */
#line 1184 "bison_parser.y"
                 { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-1].expr)); }
#line 5242 "bison_parser.cpp"
    break;

  case 214: /* unary_expr: operand IS NULL  */
#line 1185 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-2].expr)); }
#line 5248 "bison_parser.cpp"
    break;

  case 215: /* unary_expr: operand IS NOT NULL  */
#line 1186 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeOpUnary(kOpIsNull, (yyvsp[-3].expr))); }
#line 5254 "bison_parser.cpp"
    break;

  case 217: /* binary_expr: operand '-' operand  */
#line 1188 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpMinus, (yyvsp[0].expr)); }
#line 5260 "bison_parser.cpp"
    break;

  case 218: /* binary_expr: operand '+' operand  */
#line 1189 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPlus, (yyvsp[0].expr)); }
#line 5266 "bison_parser.cpp"
    break;

  case 219: /* binary_expr: operand '/' operand  */
#line 1190 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpSlash, (yyvsp[0].expr)); }
#line 5272 "bison_parser.cpp"
    break;

  case 220: /* binary_expr: operand '*' operand  */
#line 1191 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAsterisk, (yyvsp[0].expr)); }
#line 5278 "bison_parser.cpp"
    break;

  case 221: /* binary_expr: operand '%' operand  */
#line 1192 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPercentage, (yyvsp[0].expr)); }
#line 5284 "bison_parser.cpp"
    break;

  case 222: /* binary_expr: operand '^' operand  */
#line 1193 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpCaret, (yyvsp[0].expr)); }
#line 5290 "bison_parser.cpp"
    break;

  case 223: /* binary_expr: operand LIKE operand  */
#line 1194 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLike, (yyvsp[0].expr)); }
#line 5296 "bison_parser.cpp"
    break;

  case 224: /* binary_expr: operand NOT LIKE operand  */
#line 1195 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-3].expr), kOpNotLike, (yyvsp[0].expr)); }
#line 5302 "bison_parser.cpp"
    break;

  case 225: /* binary_expr: operand ILIKE operand  */
#line 1196 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpILike, (yyvsp[0].expr)); }
#line 5308 "bison_parser.cpp"
    break;

  case 226: /* binary_expr: operand CONCAT operand  */
#line 1197 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpConcat, (yyvsp[0].expr)); }
#line 5314 "bison_parser.cpp"
    break;

  case 227: /* logic_expr: expr AND expr  */
#line 1199 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAnd, (yyvsp[0].expr)); }
#line 5320 "bison_parser.cpp"
    break;

  case 228: /* logic_expr: expr OR expr  */
#line 1200 "bison_parser.y"
               { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpOr, (yyvsp[0].expr)); }
#line 5326 "bison_parser.cpp"
    break;

  case 229: /* in_expr: operand IN '(' expr_list ')'  */
#line 1202 "bison_parser.y"
                                       { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].expr_vec)); }
#line 5332 "bison_parser.cpp"
    break;

  case 230: /* in_expr: operand NOT IN '(' expr_list ')'  */
#line 1203 "bison_parser.y"
                                   { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].expr_vec))); }
#line 5338 "bison_parser.cpp"
    break;

  case 231: /* in_expr: operand IN '(' select_no_paren ')'  */
#line 1204 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].select_stmt)); }
#line 5344 "bison_parser.cpp"
    break;

  case 232: /* in_expr: operand NOT IN '(' select_no_paren ')'  */
#line 1205 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].select_stmt))); }
#line 5350 "bison_parser.cpp"
    break;

  case 233: /* in_expr: '(' row_expr_list ')' IN '(' select_no_paren ')'  */
#line 1209 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeInOperator(Expr::makeArray((yyvsp[-5].expr_vec)), (yyvsp[-1].select_stmt)); }
#line 5356 "bison_parser.cpp"
    break;

  case 234: /* in_expr: '(' row_expr_list ')' NOT IN '(' select_no_paren ')'  */
#line 1210 "bison_parser.y"
                                                       { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator(Expr::makeArray((yyvsp[-6].expr_vec)), (yyvsp[-1].select_stmt))); }
#line 5362 "bison_parser.cpp"
    break;

  case 235: /* row_expr_list: expr_alias ',' expr_alias  */
#line 1214 "bison_parser.y"
                                          {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[-2].expr));
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5372 "bison_parser.cpp"
    break;

  case 236: /* row_expr_list: row_expr_list ',' expr_alias  */
#line 1219 "bison_parser.y"
                               {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5381 "bison_parser.cpp"
    break;

  case 237: /* case_expr: CASE expr case_list END  */
#line 1226 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-2].expr), (yyvsp[-1].expr), nullptr); }
#line 5387 "bison_parser.cpp"
    break;

  case 238: /* case_expr: CASE expr case_list ELSE expr END  */
#line 1227 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-4].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5393 "bison_parser.cpp"
    break;

  case 239: /* case_expr: CASE case_list END  */
#line 1228 "bison_parser.y"
                     { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-1].expr), nullptr); }
#line 5399 "bison_parser.cpp"
    break;

  case 240: /* case_expr: CASE case_list ELSE expr END  */
#line 1229 "bison_parser.y"
                               { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5405 "bison_parser.cpp"
    break;

  case 241: /* case_list: WHEN expr THEN expr  */
#line 1231 "bison_parser.y"
                                { (yyval.expr) = Expr::makeCaseList(Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5411 "bison_parser.cpp"
    break;

  case 242: /* case_list: case_list WHEN expr THEN expr  */
#line 1232 "bison_parser.y"
                                { (yyval.expr) = Expr::caseListAppend((yyvsp[-4].expr), Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5417 "bison_parser.cpp"
    break;

  case 243: /* exists_expr: EXISTS '(' select_no_paren ')'  */
#line 1234 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeExists((yyvsp[-1].select_stmt)); }
#line 5423 "bison_parser.cpp"
    break;

  case 244: /* exists_expr: NOT EXISTS '(' select_no_paren ')'  */
#line 1235 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeExists((yyvsp[-1].select_stmt))); }
#line 5429 "bison_parser.cpp"
    break;

  case 245: /* comp_expr: operand '=' operand  */
#line 1237 "bison_parser.y"
                                { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5435 "bison_parser.cpp"
    break;

  case 246: /* comp_expr: operand EQUALS operand  */
#line 1238 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5441 "bison_parser.cpp"
    break;

  case 247: /* comp_expr: operand NOTEQUALS operand  */
#line 1239 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpNotEquals, (yyvsp[0].expr)); }
#line 5447 "bison_parser.cpp"
    break;

  case 248: /* comp_expr: operand '<' operand  */
#line 1240 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLess, (yyvsp[0].expr)); }
#line 5453 "bison_parser.cpp"
    break;

  case 249: /* comp_expr: operand '>' operand  */
#line 1241 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreater, (yyvsp[0].expr)); }
#line 5459 "bison_parser.cpp"
    break;

  case 250: /* comp_expr: operand LESSEQ operand  */
#line 1242 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLessEq, (yyvsp[0].expr)); }
#line 5465 "bison_parser.cpp"
    break;

  case 251: /* comp_expr: operand GREATEREQ operand  */
#line 1243 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreaterEq, (yyvsp[0].expr)); }
#line 5471 "bison_parser.cpp"
    break;

  case 252: /* function_expr: IDENTIFIER '(' ')' opt_window  */
#line 1247 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 5477 "bison_parser.cpp"
    break;

  case 253: /* function_expr: IDENTIFIER '(' opt_distinct expr_list ')' opt_within_group opt_window  */
#line 1248 "bison_parser.y"
                                                                        {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-6].sval), (yyvsp[-3].expr_vec), (yyvsp[-4].bval), (yyvsp[0].window_description));
  (yyval.expr)->withinGroupOrder = (yyvsp[-1].order_vec);
}
#line 5486 "bison_parser.cpp"
    break;

  case 254: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' ')' opt_window  */
#line 1252 "bison_parser.y"
                                               {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), (yyvsp[-5].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description));
}
#line 5494 "bison_parser.cpp"
    break;

  case 255: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' opt_distinct expr_list ')' opt_window  */
#line 1255 "bison_parser.y"
                                                                      {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-7].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description));
}
#line 5502 "bison_parser.cpp"
    break;

  case 256: /* function_expr: nonreserved_keyword '(' ')' opt_window  */
#line 1260 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 5508 "bison_parser.cpp"
    break;

  case 257: /* function_expr: nonreserved_keyword '(' opt_distinct expr_list ')' opt_window  */
#line 1261 "bison_parser.y"
                                                                { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description)); }
#line 5514 "bison_parser.cpp"
    break;

  case 258: /* opt_window: OVER '(' opt_partition opt_order opt_frame_clause ')'  */
#line 1265 "bison_parser.y"
                                                                   { (yyval.window_description) = new WindowDescription((yyvsp[-3].expr_vec), (yyvsp[-2].order_vec), (yyvsp[-1].frame_description)); }
#line 5520 "bison_parser.cpp"
    break;

  case 259: /* opt_window: %empty  */
#line 1266 "bison_parser.y"
              { (yyval.window_description) = nullptr; }
#line 5526 "bison_parser.cpp"
    break;

  case 260: /* opt_partition: PARTITION BY expr_list  */
#line 1268 "bison_parser.y"
                                       { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5532 "bison_parser.cpp"
    break;

  case 261: /* opt_partition: %empty  */
#line 1269 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5538 "bison_parser.cpp"
    break;

  case 262: /* opt_frame_clause: frame_type frame_bound  */
#line 1274 "bison_parser.y"
                                          { (yyval.frame_description) = new FrameDescription{(yyvsp[-1].frame_type), (yyvsp[0].frame_bound), new FrameBound{0, kCurrentRow, false}}; }
#line 5544 "bison_parser.cpp"
    break;

  case 263: /* opt_frame_clause: frame_type BETWEEN frame_bound AND frame_bound  */
#line 1275 "bison_parser.y"
                                                 { (yyval.frame_description) = new FrameDescription{(yyvsp[-4].frame_type), (yyvsp[-2].frame_bound), (yyvsp[0].frame_bound)}; }
#line 5550 "bison_parser.cpp"
    break;

  case 264: /* opt_frame_clause: %empty  */
#line 1276 "bison_parser.y"
              {
  (yyval.frame_description) = new FrameDescription{kRange, new FrameBound{0, kPreceding, true}, new FrameBound{0, kCurrentRow, false}};
}
#line 5558 "bison_parser.cpp"
    break;

  case 265: /* frame_type: RANGE  */
#line 1280 "bison_parser.y"
                   { (yyval.frame_type) = kRange; }
#line 5564 "bison_parser.cpp"
    break;

  case 266: /* frame_type: ROWS  */
#line 1281 "bison_parser.y"
       { (yyval.frame_type) = kRows; }
#line 5570 "bison_parser.cpp"
    break;

  case 267: /* frame_type: GROUPS  */
#line 1282 "bison_parser.y"
         { (yyval.frame_type) = kGroups; }
#line 5576 "bison_parser.cpp"
    break;

  case 268: /* frame_bound: UNBOUNDED PRECEDING  */
#line 1284 "bison_parser.y"
                                  { (yyval.frame_bound) = new FrameBound{0, kPreceding, true}; }
#line 5582 "bison_parser.cpp"
    break;

  case 269: /* frame_bound: INTVAL PRECEDING  */
#line 1285 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kPreceding, false}; }
#line 5588 "bison_parser.cpp"
    break;

  case 270: /* frame_bound: UNBOUNDED FOLLOWING  */
#line 1286 "bison_parser.y"
                      { (yyval.frame_bound) = new FrameBound{0, kFollowing, true}; }
#line 5594 "bison_parser.cpp"
    break;

  case 271: /* frame_bound: INTVAL FOLLOWING  */
#line 1287 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kFollowing, false}; }
#line 5600 "bison_parser.cpp"
    break;

  case 272: /* frame_bound: CURRENT_ROW  */
#line 1288 "bison_parser.y"
              { (yyval.frame_bound) = new FrameBound{0, kCurrentRow, false}; }
#line 5606 "bison_parser.cpp"
    break;

  case 273: /* extract_expr: EXTRACT '(' datetime_field FROM expr ')'  */
#line 1290 "bison_parser.y"
                                                        { (yyval.expr) = Expr::makeExtract((yyvsp[-3].datetime_field), (yyvsp[-1].expr)); }
#line 5612 "bison_parser.cpp"
    break;

  case 274: /* cast_expr: CAST '(' expr AS column_type ')'  */
#line 1292 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t)); }
#line 5618 "bison_parser.cpp"
    break;

  case 275: /* datetime_field: SECOND  */
#line 1294 "bison_parser.y"
                        { (yyval.datetime_field) = kDatetimeSecond; }
#line 5624 "bison_parser.cpp"
    break;

  case 276: /* datetime_field: MINUTE  */
#line 1295 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMinute; }
#line 5630 "bison_parser.cpp"
    break;

  case 277: /* datetime_field: HOUR  */
#line 1296 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeHour; }
#line 5636 "bison_parser.cpp"
    break;

  case 278: /* datetime_field: DAY  */
#line 1297 "bison_parser.y"
      { (yyval.datetime_field) = kDatetimeDay; }
#line 5642 "bison_parser.cpp"
    break;

  case 279: /* datetime_field: MONTH  */
#line 1298 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeMonth; }
#line 5648 "bison_parser.cpp"
    break;

  case 280: /* datetime_field: YEAR  */
#line 1299 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeYear; }
#line 5654 "bison_parser.cpp"
    break;

  case 281: /* datetime_field_plural: SECONDS  */
#line 1301 "bison_parser.y"
                                { (yyval.datetime_field) = kDatetimeSecond; }
#line 5660 "bison_parser.cpp"
    break;

  case 282: /* datetime_field_plural: MINUTES  */
#line 1302 "bison_parser.y"
          { (yyval.datetime_field) = kDatetimeMinute; }
#line 5666 "bison_parser.cpp"
    break;

  case 283: /* datetime_field_plural: HOURS  */
#line 1303 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeHour; }
#line 5672 "bison_parser.cpp"
    break;

  case 284: /* datetime_field_plural: DAYS  */
#line 1304 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeDay; }
#line 5678 "bison_parser.cpp"
    break;

  case 285: /* datetime_field_plural: MONTHS  */
#line 1305 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMonth; }
#line 5684 "bison_parser.cpp"
    break;

  case 286: /* datetime_field_plural: YEARS  */
#line 1306 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeYear; }
#line 5690 "bison_parser.cpp"
    break;

  case 289: /* array_expr: ARRAY '[' expr_list ']'  */
#line 1310 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeArray((yyvsp[-1].expr_vec)); }
#line 5696 "bison_parser.cpp"
    break;

  case 290: /* array_index: operand '[' int_literal ']'  */
#line 1316 "bison_parser.y"
                                          {
  (yyval.expr) = Expr::makeArrayIndex((yyvsp[-3].expr), (yyvsp[-1].expr)->ival);
  delete (yyvsp[-1].expr);
}
#line 5705 "bison_parser.cpp"
    break;

  case 291: /* between_expr: operand BETWEEN operand AND operand  */
#line 1321 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeBetween((yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 5711 "bison_parser.cpp"
    break;

  case 292: /* column_name: IDENTIFIER  */
#line 1323 "bison_parser.y"
                         { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 5717 "bison_parser.cpp"
    break;

  case 293: /* column_name: IDENTIFIER '.' IDENTIFIER  */
#line 1324 "bison_parser.y"
                            { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 5723 "bison_parser.cpp"
    break;

  case 294: /* column_name: '*'  */
#line 1325 "bison_parser.y"
      { (yyval.expr) = Expr::makeStar(); }
#line 5729 "bison_parser.cpp"
    break;

  case 295: /* column_name: IDENTIFIER '.' '*'  */
#line 1326 "bison_parser.y"
                     { (yyval.expr) = Expr::makeStar((yyvsp[-2].sval)); }
#line 5735 "bison_parser.cpp"
    break;

  case 296: /* column_name: nonreserved_keyword  */
#line 1327 "bison_parser.y"
                      { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 5741 "bison_parser.cpp"
    break;

  case 297: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1333 "bison_parser.y"
                                           {
  std::string combined(std::string((yyvsp[-4].sval)) + "." + (yyvsp[-2].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeColumnRef(strdup(combined.c_str()), (yyvsp[0].sval));
}
#line 5752 "bison_parser.cpp"
    break;

  case 298: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1339 "bison_parser.y"
                                                          {
  std::string combined(std::string((yyvsp[-6].sval)) + "." + (yyvsp[-4].sval) + "." + (yyvsp[-2].sval));
  free((yyvsp[-6].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeColumnRef(strdup(combined.c_str()), (yyvsp[0].sval));
}
#line 5764 "bison_parser.cpp"
    break;

  case 299: /* column_name: IDENTIFIER '.' IDENTIFIER '.' '*'  */
#line 1346 "bison_parser.y"
                                    {
  std::string combined(std::string((yyvsp[-4].sval)) + "." + (yyvsp[-2].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeStar(strdup(combined.c_str()));
}
#line 5775 "bison_parser.cpp"
    break;

  case 300: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER '.' '*'  */
#line 1352 "bison_parser.y"
                                                   {
  std::string combined(std::string((yyvsp[-6].sval)) + "." + (yyvsp[-4].sval) + "." + (yyvsp[-2].sval));
  free((yyvsp[-6].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeStar(strdup(combined.c_str()));
}
#line 5787 "bison_parser.cpp"
    break;

  case 301: /* nonreserved_keyword: SECOND  */
#line 1366 "bison_parser.y"
                             { (yyval.sval) = strdup("SECOND"); }
#line 5793 "bison_parser.cpp"
    break;

  case 302: /* nonreserved_keyword: MINUTE  */
#line 1367 "bison_parser.y"
         { (yyval.sval) = strdup("MINUTE"); }
#line 5799 "bison_parser.cpp"
    break;

  case 303: /* nonreserved_keyword: HOUR  */
#line 1368 "bison_parser.y"
       { (yyval.sval) = strdup("HOUR"); }
#line 5805 "bison_parser.cpp"
    break;

  case 304: /* nonreserved_keyword: DAY  */
#line 1369 "bison_parser.y"
      { (yyval.sval) = strdup("DAY"); }
#line 5811 "bison_parser.cpp"
    break;

  case 305: /* nonreserved_keyword: MONTH  */
#line 1370 "bison_parser.y"
        { (yyval.sval) = strdup("MONTH"); }
#line 5817 "bison_parser.cpp"
    break;

  case 306: /* nonreserved_keyword: YEAR  */
#line 1371 "bison_parser.y"
       { (yyval.sval) = strdup("YEAR"); }
#line 5823 "bison_parser.cpp"
    break;

  case 307: /* nonreserved_keyword: SECONDS  */
#line 1372 "bison_parser.y"
          { (yyval.sval) = strdup("SECONDS"); }
#line 5829 "bison_parser.cpp"
    break;

  case 308: /* nonreserved_keyword: MINUTES  */
#line 1373 "bison_parser.y"
          { (yyval.sval) = strdup("MINUTES"); }
#line 5835 "bison_parser.cpp"
    break;

  case 309: /* nonreserved_keyword: HOURS  */
#line 1374 "bison_parser.y"
        { (yyval.sval) = strdup("HOURS"); }
#line 5841 "bison_parser.cpp"
    break;

  case 310: /* nonreserved_keyword: DAYS  */
#line 1375 "bison_parser.y"
       { (yyval.sval) = strdup("DAYS"); }
#line 5847 "bison_parser.cpp"
    break;

  case 311: /* nonreserved_keyword: MONTHS  */
#line 1376 "bison_parser.y"
         { (yyval.sval) = strdup("MONTHS"); }
#line 5853 "bison_parser.cpp"
    break;

  case 312: /* nonreserved_keyword: YEARS  */
#line 1377 "bison_parser.y"
        { (yyval.sval) = strdup("YEARS"); }
#line 5859 "bison_parser.cpp"
    break;

  case 313: /* nonreserved_keyword: ISNULL  */
#line 1378 "bison_parser.y"
         { (yyval.sval) = strdup("ISNULL"); }
#line 5865 "bison_parser.cpp"
    break;

  case 314: /* nonreserved_keyword: FORMAT  */
#line 1379 "bison_parser.y"
         { (yyval.sval) = strdup("FORMAT"); }
#line 5871 "bison_parser.cpp"
    break;

  case 315: /* nonreserved_keyword: CHAR  */
#line 1380 "bison_parser.y"
       { (yyval.sval) = strdup("CHAR"); }
#line 5877 "bison_parser.cpp"
    break;

  case 316: /* nonreserved_keyword: VARCHAR  */
#line 1381 "bison_parser.y"
          { (yyval.sval) = strdup("VARCHAR"); }
#line 5883 "bison_parser.cpp"
    break;

  case 317: /* nonreserved_keyword: INT  */
#line 1382 "bison_parser.y"
      { (yyval.sval) = strdup("INT"); }
#line 5889 "bison_parser.cpp"
    break;

  case 318: /* nonreserved_keyword: INTEGER  */
#line 1383 "bison_parser.y"
          { (yyval.sval) = strdup("INTEGER"); }
#line 5895 "bison_parser.cpp"
    break;

  case 319: /* nonreserved_keyword: DATETIME  */
#line 1384 "bison_parser.y"
           { (yyval.sval) = strdup("DATETIME"); }
#line 5901 "bison_parser.cpp"
    break;

  case 320: /* nonreserved_keyword: TIMESTAMP  */
#line 1385 "bison_parser.y"
            { (yyval.sval) = strdup("TIMESTAMP"); }
#line 5907 "bison_parser.cpp"
    break;

  case 321: /* nonreserved_keyword: CONNECT  */
#line 1386 "bison_parser.y"
          { (yyval.sval) = strdup("CONNECT"); }
#line 5913 "bison_parser.cpp"
    break;

  case 322: /* nonreserved_keyword: START  */
#line 1387 "bison_parser.y"
        { (yyval.sval) = strdup("START"); }
#line 5919 "bison_parser.cpp"
    break;

  case 330: /* string_literal: STRING  */
#line 1391 "bison_parser.y"
                        { (yyval.expr) = Expr::makeLiteral((yyvsp[0].sval)); }
#line 5925 "bison_parser.cpp"
    break;

  case 331: /* bool_literal: TRUE  */
#line 1393 "bison_parser.y"
                    { (yyval.expr) = Expr::makeLiteral(true); }
#line 5931 "bison_parser.cpp"
    break;

  case 332: /* bool_literal: FALSE  */
#line 1394 "bison_parser.y"
        { (yyval.expr) = Expr::makeLiteral(false); }
#line 5937 "bison_parser.cpp"
    break;

  case 333: /* num_literal: FLOATVAL  */
#line 1396 "bison_parser.y"
                       { (yyval.expr) = Expr::makeLiteral((yyvsp[0].fval)); }
#line 5943 "bison_parser.cpp"
    break;

  case 335: /* int_literal: INTVAL  */
#line 1399 "bison_parser.y"
                     { (yyval.expr) = Expr::makeLiteral((yyvsp[0].ival)); }
#line 5949 "bison_parser.cpp"
    break;

  case 336: /* null_literal: NULL  */
#line 1401 "bison_parser.y"
                    { (yyval.expr) = Expr::makeNullLiteral(); }
#line 5955 "bison_parser.cpp"
    break;

  case 337: /* date_literal: DATE STRING  */
#line 1403 "bison_parser.y"
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
#line 5970 "bison_parser.cpp"
    break;

  case 338: /* interval_literal: INTVAL duration_field  */
#line 1414 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeIntervalLiteral((yyvsp[-1].ival), (yyvsp[0].datetime_field)); }
#line 5976 "bison_parser.cpp"
    break;

  case 339: /* interval_literal: INTERVAL STRING datetime_field  */
#line 1415 "bison_parser.y"
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
#line 5992 "bison_parser.cpp"
    break;

  case 340: /* interval_literal: INTERVAL STRING  */
#line 1426 "bison_parser.y"
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
#line 6028 "bison_parser.cpp"
    break;

  case 341: /* param_expr: '?'  */
#line 1458 "bison_parser.y"
                 {
  (yyval.expr) = Expr::makeParameter(yylloc.total_column);
  (yyval.expr)->ival2 = yylloc.total_column - 1;  // source column (0-based) of the '?' token
  yyloc.param_list.push_back((yyval.expr));
}
#line 6038 "bison_parser.cpp"
    break;

  case 342: /* param_expr: DOLLAR_PARAM  */
#line 1463 "bison_parser.y"
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
#line 6055 "bison_parser.cpp"
    break;

  case 343: /* param_expr: NAMED_PARAM  */
#line 1475 "bison_parser.y"
              {
  (yyval.expr) = Expr::makeNamedParameter((yyvsp[0].sval));
  (yyval.expr)->ival2 = yylloc.total_column - 1 - (int64_t)strlen((yyvsp[0].sval));
  yyloc.param_list.push_back((yyval.expr));
}
#line 6065 "bison_parser.cpp"
    break;

  case 345: /* table_ref: table_ref_commalist ',' table_ref_atomic  */
#line 1484 "bison_parser.y"
                                                                        {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  auto tbl = new TableRef(kTableCrossProduct);
  tbl->list = (yyvsp[-2].table_vec);
  (yyval.table) = tbl;
}
#line 6076 "bison_parser.cpp"
    break;

  case 349: /* nonjoin_table_ref_atomic: '(' select_statement ')' opt_table_alias  */
#line 1493 "bison_parser.y"
                                                                                     {
  auto tbl = new TableRef(kTableSelect);
  tbl->select = (yyvsp[-2].select_stmt);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6087 "bison_parser.cpp"
    break;

  case 350: /* nonjoin_table_ref_atomic: function_expr opt_table_alias  */
#line 1501 "bison_parser.y"
                                {
  auto tbl = new TableRef(kTableFunc);
  tbl->func = (yyvsp[-1].expr);
  if ((yyvsp[-1].expr)->name) tbl->name = strdup((yyvsp[-1].expr)->name);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6099 "bison_parser.cpp"
    break;

  case 351: /* nonjoin_table_ref_atomic: '(' VALUES table_value_row_list ')' opt_table_alias  */
#line 1510 "bison_parser.y"
                                                      {
  auto tbl = new TableRef(kTableValues);
  tbl->values = (yyvsp[-2].expr_vec);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6110 "bison_parser.cpp"
    break;

  case 352: /* table_value_row_list: '(' expr_list ')'  */
#line 1519 "bison_parser.y"
                                         {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
}
#line 6119 "bison_parser.cpp"
    break;

  case 353: /* table_value_row_list: table_value_row_list ',' '(' expr_list ')'  */
#line 1523 "bison_parser.y"
                                             {
  (yyvsp[-4].expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
  (yyval.expr_vec) = (yyvsp[-4].expr_vec);
}
#line 6128 "bison_parser.cpp"
    break;

  case 354: /* table_ref_commalist: table_ref_atomic  */
#line 1528 "bison_parser.y"
                                       {
  (yyval.table_vec) = new std::vector<TableRef*>();
  (yyval.table_vec)->push_back((yyvsp[0].table));
}
#line 6137 "bison_parser.cpp"
    break;

  case 355: /* table_ref_commalist: table_ref_commalist ',' table_ref_atomic  */
#line 1532 "bison_parser.y"
                                           {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  (yyval.table_vec) = (yyvsp[-2].table_vec);
}
#line 6146 "bison_parser.cpp"
    break;

  case 356: /* table_ref_name: table_name opt_table_alias  */
#line 1537 "bison_parser.y"
                                            {
  auto tbl = new TableRef(kTableName);
  tbl->schema = (yyvsp[-1].table_name).schema;
  tbl->name = (yyvsp[-1].table_name).name;
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6158 "bison_parser.cpp"
    break;

  case 357: /* table_ref_name_no_alias: table_name  */
#line 1545 "bison_parser.y"
                                     {
  (yyval.table) = new TableRef(kTableName);
  (yyval.table)->schema = (yyvsp[0].table_name).schema;
  (yyval.table)->name = (yyvsp[0].table_name).name;
}
#line 6168 "bison_parser.cpp"
    break;

  case 358: /* table_name: IDENTIFIER  */
#line 1551 "bison_parser.y"
                        {
  (yyval.table_name).schema = nullptr;
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6177 "bison_parser.cpp"
    break;

  case 359: /* table_name: IDENTIFIER '.' IDENTIFIER  */
#line 1555 "bison_parser.y"
                            {
  (yyval.table_name).schema = (yyvsp[-2].sval);
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6186 "bison_parser.cpp"
    break;

  case 360: /* table_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1559 "bison_parser.y"
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
#line 6201 "bison_parser.cpp"
    break;

  case 361: /* opt_index_name: IDENTIFIER  */
#line 1570 "bison_parser.y"
                            { (yyval.sval) = (yyvsp[0].sval); }
#line 6207 "bison_parser.cpp"
    break;

  case 362: /* opt_index_name: %empty  */
#line 1571 "bison_parser.y"
              { (yyval.sval) = nullptr; }
#line 6213 "bison_parser.cpp"
    break;

  case 364: /* table_alias: AS IDENTIFIER '(' ident_commalist ')'  */
#line 1573 "bison_parser.y"
                                                            { (yyval.alias_t) = new Alias((yyvsp[-3].sval), (yyvsp[-1].str_vec)); }
#line 6219 "bison_parser.cpp"
    break;

  case 366: /* opt_table_alias: %empty  */
#line 1575 "bison_parser.y"
                                            { (yyval.alias_t) = nullptr; }
#line 6225 "bison_parser.cpp"
    break;

  case 367: /* alias: AS IDENTIFIER  */
#line 1577 "bison_parser.y"
                      { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6231 "bison_parser.cpp"
    break;

  case 368: /* alias: AS STRING  */
#line 1581 "bison_parser.y"
            { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6237 "bison_parser.cpp"
    break;

  case 369: /* alias: IDENTIFIER  */
#line 1582 "bison_parser.y"
             { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6243 "bison_parser.cpp"
    break;

  case 371: /* opt_alias: %empty  */
#line 1584 "bison_parser.y"
                                { (yyval.alias_t) = nullptr; }
#line 6249 "bison_parser.cpp"
    break;

  case 372: /* opt_locking_clause: opt_locking_clause_list  */
#line 1590 "bison_parser.y"
                                             { (yyval.locking_clause_vec) = (yyvsp[0].locking_clause_vec); }
#line 6255 "bison_parser.cpp"
    break;

  case 373: /* opt_locking_clause: %empty  */
#line 1591 "bison_parser.y"
              { (yyval.locking_clause_vec) = nullptr; }
#line 6261 "bison_parser.cpp"
    break;

  case 374: /* opt_locking_clause_list: locking_clause  */
#line 1593 "bison_parser.y"
                                         {
  (yyval.locking_clause_vec) = new std::vector<LockingClause*>();
  (yyval.locking_clause_vec)->push_back((yyvsp[0].locking_t));
}
#line 6270 "bison_parser.cpp"
    break;

  case 375: /* opt_locking_clause_list: opt_locking_clause_list locking_clause  */
#line 1597 "bison_parser.y"
                                         {
  (yyvsp[-1].locking_clause_vec)->push_back((yyvsp[0].locking_t));
  (yyval.locking_clause_vec) = (yyvsp[-1].locking_clause_vec);
}
#line 6279 "bison_parser.cpp"
    break;

  case 376: /* locking_clause: FOR row_lock_mode opt_row_lock_policy  */
#line 1602 "bison_parser.y"
                                                       {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-1].lock_mode_t);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
  (yyval.locking_t)->tables = nullptr;
}
#line 6290 "bison_parser.cpp"
    break;

  case 377: /* locking_clause: FOR row_lock_mode OF ident_commalist opt_row_lock_policy  */
#line 1608 "bison_parser.y"
                                                           {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-3].lock_mode_t);
  (yyval.locking_t)->tables = (yyvsp[-1].str_vec);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
}
#line 6301 "bison_parser.cpp"
    break;

  case 378: /* row_lock_mode: UPDATE  */
#line 1615 "bison_parser.y"
                       { (yyval.lock_mode_t) = RowLockMode::ForUpdate; }
#line 6307 "bison_parser.cpp"
    break;

  case 379: /* row_lock_mode: NO KEY UPDATE  */
#line 1616 "bison_parser.y"
                { (yyval.lock_mode_t) = RowLockMode::ForNoKeyUpdate; }
#line 6313 "bison_parser.cpp"
    break;

  case 380: /* row_lock_mode: SHARE  */
#line 1617 "bison_parser.y"
        { (yyval.lock_mode_t) = RowLockMode::ForShare; }
#line 6319 "bison_parser.cpp"
    break;

  case 381: /* row_lock_mode: KEY SHARE  */
#line 1618 "bison_parser.y"
            { (yyval.lock_mode_t) = RowLockMode::ForKeyShare; }
#line 6325 "bison_parser.cpp"
    break;

  case 382: /* opt_row_lock_policy: SKIP LOCKED  */
#line 1620 "bison_parser.y"
                                  { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::SkipLocked; }
#line 6331 "bison_parser.cpp"
    break;

  case 383: /* opt_row_lock_policy: NOWAIT  */
#line 1621 "bison_parser.y"
         { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::NoWait; }
#line 6337 "bison_parser.cpp"
    break;

  case 384: /* opt_row_lock_policy: %empty  */
#line 1622 "bison_parser.y"
              { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::None; }
#line 6343 "bison_parser.cpp"
    break;

  case 386: /* opt_with_clause: %empty  */
#line 1628 "bison_parser.y"
                                            { (yyval.with_description_vec) = nullptr; }
#line 6349 "bison_parser.cpp"
    break;

  case 387: /* with_clause: WITH with_description_list  */
#line 1630 "bison_parser.y"
                                         { (yyval.with_description_vec) = (yyvsp[0].with_description_vec); }
#line 6355 "bison_parser.cpp"
    break;

  case 388: /* with_description_list: with_description  */
#line 1632 "bison_parser.y"
                                         {
  (yyval.with_description_vec) = new std::vector<WithDescription*>();
  (yyval.with_description_vec)->push_back((yyvsp[0].with_description_t));
}
#line 6364 "bison_parser.cpp"
    break;

  case 389: /* with_description_list: with_description_list ',' with_description  */
#line 1636 "bison_parser.y"
                                             {
  (yyvsp[-2].with_description_vec)->push_back((yyvsp[0].with_description_t));
  (yyval.with_description_vec) = (yyvsp[-2].with_description_vec);
}
#line 6373 "bison_parser.cpp"
    break;

  case 390: /* with_description: IDENTIFIER AS select_with_paren  */
#line 1641 "bison_parser.y"
                                                   {
  (yyval.with_description_t) = new WithDescription();
  (yyval.with_description_t)->alias = (yyvsp[-2].sval);
  (yyval.with_description_t)->select = (yyvsp[0].select_stmt);
}
#line 6383 "bison_parser.cpp"
    break;

  case 391: /* join_clause: table_ref_atomic NATURAL JOIN nonjoin_table_ref_atomic  */
#line 1651 "bison_parser.y"
                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = kJoinNatural;
  (yyval.table)->join->left = (yyvsp[-3].table);
  (yyval.table)->join->right = (yyvsp[0].table);
}
#line 6395 "bison_parser.cpp"
    break;

  case 392: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic ON join_condition  */
#line 1658 "bison_parser.y"
                                                                         {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (JoinType)(yyvsp[-4].join_type);
  (yyval.table)->join->left = (yyvsp[-5].table);
  (yyval.table)->join->right = (yyvsp[-2].table);
  (yyval.table)->join->condition = (yyvsp[0].expr);
}
#line 6408 "bison_parser.cpp"
    break;

  case 393: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic USING '(' ident_commalist ')'  */
#line 1666 "bison_parser.y"
                                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (yyvsp[-6].join_type);
  (yyval.table)->join->left = (yyvsp[-7].table);
  (yyval.table)->join->right = (yyvsp[-4].table);
  (yyval.table)->join->namedColumns = (yyvsp[-1].str_vec);
}
#line 6421 "bison_parser.cpp"
    break;

  case 394: /* opt_join_type: INNER  */
#line 1675 "bison_parser.y"
                      { (yyval.join_type) = kJoinInner; }
#line 6427 "bison_parser.cpp"
    break;

  case 395: /* opt_join_type: LEFT OUTER  */
#line 1676 "bison_parser.y"
             { (yyval.join_type) = kJoinLeft; }
#line 6433 "bison_parser.cpp"
    break;

  case 396: /* opt_join_type: LEFT  */
#line 1677 "bison_parser.y"
       { (yyval.join_type) = kJoinLeft; }
#line 6439 "bison_parser.cpp"
    break;

  case 397: /* opt_join_type: RIGHT OUTER  */
#line 1678 "bison_parser.y"
              { (yyval.join_type) = kJoinRight; }
#line 6445 "bison_parser.cpp"
    break;

  case 398: /* opt_join_type: RIGHT  */
#line 1679 "bison_parser.y"
        { (yyval.join_type) = kJoinRight; }
#line 6451 "bison_parser.cpp"
    break;

  case 399: /* opt_join_type: FULL OUTER  */
#line 1680 "bison_parser.y"
             { (yyval.join_type) = kJoinFull; }
#line 6457 "bison_parser.cpp"
    break;

  case 400: /* opt_join_type: OUTER  */
#line 1681 "bison_parser.y"
        { (yyval.join_type) = kJoinFull; }
#line 6463 "bison_parser.cpp"
    break;

  case 401: /* opt_join_type: FULL  */
#line 1682 "bison_parser.y"
       { (yyval.join_type) = kJoinFull; }
#line 6469 "bison_parser.cpp"
    break;

  case 402: /* opt_join_type: CROSS  */
#line 1683 "bison_parser.y"
        { (yyval.join_type) = kJoinCross; }
#line 6475 "bison_parser.cpp"
    break;

  case 403: /* opt_join_type: %empty  */
#line 1684 "bison_parser.y"
                       { (yyval.join_type) = kJoinInner; }
#line 6481 "bison_parser.cpp"
    break;

  case 407: /* ident_commalist: IDENTIFIER  */
#line 1695 "bison_parser.y"
                             {
  (yyval.str_vec) = new std::vector<char*>();
  (yyval.str_vec)->push_back((yyvsp[0].sval));
}
#line 6490 "bison_parser.cpp"
    break;

  case 408: /* ident_commalist: ident_commalist ',' IDENTIFIER  */
#line 1699 "bison_parser.y"
                                 {
  (yyvsp[-2].str_vec)->push_back((yyvsp[0].sval));
  (yyval.str_vec) = (yyvsp[-2].str_vec);
}
#line 6499 "bison_parser.cpp"
    break;


#line 6503 "bison_parser.cpp"

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

#line 1705 "bison_parser.y"


/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */

    // clang-format on
