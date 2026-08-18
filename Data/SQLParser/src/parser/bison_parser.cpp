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
  YYSYMBOL_179_ = 179,                     /* '='  */
  YYSYMBOL_EQUALS = 180,                   /* EQUALS  */
  YYSYMBOL_NOTEQUALS = 181,                /* NOTEQUALS  */
  YYSYMBOL_182_ = 182,                     /* '<'  */
  YYSYMBOL_183_ = 183,                     /* '>'  */
  YYSYMBOL_LESS = 184,                     /* LESS  */
  YYSYMBOL_GREATER = 185,                  /* GREATER  */
  YYSYMBOL_LESSEQ = 186,                   /* LESSEQ  */
  YYSYMBOL_GREATEREQ = 187,                /* GREATEREQ  */
  YYSYMBOL_NOTNULL = 188,                  /* NOTNULL  */
  YYSYMBOL_189_ = 189,                     /* '+'  */
  YYSYMBOL_190_ = 190,                     /* '-'  */
  YYSYMBOL_191_ = 191,                     /* '*'  */
  YYSYMBOL_192_ = 192,                     /* '/'  */
  YYSYMBOL_193_ = 193,                     /* '%'  */
  YYSYMBOL_194_ = 194,                     /* '^'  */
  YYSYMBOL_UMINUS = 195,                   /* UMINUS  */
  YYSYMBOL_196_ = 196,                     /* '['  */
  YYSYMBOL_197_ = 197,                     /* ']'  */
  YYSYMBOL_198_ = 198,                     /* '('  */
  YYSYMBOL_199_ = 199,                     /* ')'  */
  YYSYMBOL_200_ = 200,                     /* '.'  */
  YYSYMBOL_201_ = 201,                     /* ';'  */
  YYSYMBOL_202_ = 202,                     /* ','  */
  YYSYMBOL_203_ = 203,                     /* '?'  */
  YYSYMBOL_YYACCEPT = 204,                 /* $accept  */
  YYSYMBOL_input = 205,                    /* input  */
  YYSYMBOL_statement_list = 206,           /* statement_list  */
  YYSYMBOL_statement = 207,                /* statement  */
  YYSYMBOL_preparable_statement = 208,     /* preparable_statement  */
  YYSYMBOL_opt_hints = 209,                /* opt_hints  */
  YYSYMBOL_hint_list = 210,                /* hint_list  */
  YYSYMBOL_hint = 211,                     /* hint  */
  YYSYMBOL_transaction_statement = 212,    /* transaction_statement  */
  YYSYMBOL_opt_transaction_keyword = 213,  /* opt_transaction_keyword  */
  YYSYMBOL_prepare_statement = 214,        /* prepare_statement  */
  YYSYMBOL_prepare_target_query = 215,     /* prepare_target_query  */
  YYSYMBOL_execute_statement = 216,        /* execute_statement  */
  YYSYMBOL_import_statement = 217,         /* import_statement  */
  YYSYMBOL_file_type = 218,                /* file_type  */
  YYSYMBOL_file_path = 219,                /* file_path  */
  YYSYMBOL_opt_import_export_options = 220, /* opt_import_export_options  */
  YYSYMBOL_import_export_options = 221,    /* import_export_options  */
  YYSYMBOL_csv_option = 222,               /* csv_option  */
  YYSYMBOL_export_statement = 223,         /* export_statement  */
  YYSYMBOL_show_statement = 224,           /* show_statement  */
  YYSYMBOL_create_statement = 225,         /* create_statement  */
  YYSYMBOL_opt_not_exists = 226,           /* opt_not_exists  */
  YYSYMBOL_table_elem_commalist = 227,     /* table_elem_commalist  */
  YYSYMBOL_table_elem = 228,               /* table_elem  */
  YYSYMBOL_column_def = 229,               /* column_def  */
  YYSYMBOL_column_type = 230,              /* column_type  */
  YYSYMBOL_opt_time_precision = 231,       /* opt_time_precision  */
  YYSYMBOL_opt_decimal_specification = 232, /* opt_decimal_specification  */
  YYSYMBOL_opt_column_constraints = 233,   /* opt_column_constraints  */
  YYSYMBOL_column_constraints = 234,       /* column_constraints  */
  YYSYMBOL_column_constraint = 235,        /* column_constraint  */
  YYSYMBOL_table_constraint = 236,         /* table_constraint  */
  YYSYMBOL_references_spec = 237,          /* references_spec  */
  YYSYMBOL_drop_statement = 238,           /* drop_statement  */
  YYSYMBOL_opt_exists = 239,               /* opt_exists  */
  YYSYMBOL_alter_statement = 240,          /* alter_statement  */
  YYSYMBOL_alter_action = 241,             /* alter_action  */
  YYSYMBOL_drop_action = 242,              /* drop_action  */
  YYSYMBOL_delete_statement = 243,         /* delete_statement  */
  YYSYMBOL_truncate_statement = 244,       /* truncate_statement  */
  YYSYMBOL_insert_statement = 245,         /* insert_statement  */
  YYSYMBOL_opt_column_list = 246,          /* opt_column_list  */
  YYSYMBOL_update_statement = 247,         /* update_statement  */
  YYSYMBOL_update_clause_commalist = 248,  /* update_clause_commalist  */
  YYSYMBOL_update_clause = 249,            /* update_clause  */
  YYSYMBOL_select_statement = 250,         /* select_statement  */
  YYSYMBOL_select_within_set_operation = 251, /* select_within_set_operation  */
  YYSYMBOL_select_within_set_operation_no_parentheses = 252, /* select_within_set_operation_no_parentheses  */
  YYSYMBOL_select_with_paren = 253,        /* select_with_paren  */
  YYSYMBOL_select_no_paren = 254,          /* select_no_paren  */
  YYSYMBOL_set_operator = 255,             /* set_operator  */
  YYSYMBOL_set_type = 256,                 /* set_type  */
  YYSYMBOL_opt_all = 257,                  /* opt_all  */
  YYSYMBOL_select_clause = 258,            /* select_clause  */
  YYSYMBOL_opt_distinct = 259,             /* opt_distinct  */
  YYSYMBOL_select_list = 260,              /* select_list  */
  YYSYMBOL_opt_from_clause = 261,          /* opt_from_clause  */
  YYSYMBOL_from_clause = 262,              /* from_clause  */
  YYSYMBOL_opt_where = 263,                /* opt_where  */
  YYSYMBOL_opt_group = 264,                /* opt_group  */
  YYSYMBOL_opt_having = 265,               /* opt_having  */
  YYSYMBOL_opt_order = 266,                /* opt_order  */
  YYSYMBOL_order_list = 267,               /* order_list  */
  YYSYMBOL_order_desc = 268,               /* order_desc  */
  YYSYMBOL_opt_order_type = 269,           /* opt_order_type  */
  YYSYMBOL_opt_null_ordering = 270,        /* opt_null_ordering  */
  YYSYMBOL_opt_top = 271,                  /* opt_top  */
  YYSYMBOL_opt_limit = 272,                /* opt_limit  */
  YYSYMBOL_expr_list = 273,                /* expr_list  */
  YYSYMBOL_opt_extended_literal_list = 274, /* opt_extended_literal_list  */
  YYSYMBOL_extended_literal_list = 275,    /* extended_literal_list  */
  YYSYMBOL_casted_extended_literal = 276,  /* casted_extended_literal  */
  YYSYMBOL_extended_literal = 277,         /* extended_literal  */
  YYSYMBOL_expr_alias = 278,               /* expr_alias  */
  YYSYMBOL_expr = 279,                     /* expr  */
  YYSYMBOL_operand = 280,                  /* operand  */
  YYSYMBOL_scalar_expr = 281,              /* scalar_expr  */
  YYSYMBOL_unary_expr = 282,               /* unary_expr  */
  YYSYMBOL_binary_expr = 283,              /* binary_expr  */
  YYSYMBOL_logic_expr = 284,               /* logic_expr  */
  YYSYMBOL_in_expr = 285,                  /* in_expr  */
  YYSYMBOL_row_expr_list = 286,            /* row_expr_list  */
  YYSYMBOL_case_expr = 287,                /* case_expr  */
  YYSYMBOL_case_list = 288,                /* case_list  */
  YYSYMBOL_exists_expr = 289,              /* exists_expr  */
  YYSYMBOL_comp_expr = 290,                /* comp_expr  */
  YYSYMBOL_function_expr = 291,            /* function_expr  */
  YYSYMBOL_opt_window = 292,               /* opt_window  */
  YYSYMBOL_opt_partition = 293,            /* opt_partition  */
  YYSYMBOL_opt_frame_clause = 294,         /* opt_frame_clause  */
  YYSYMBOL_frame_type = 295,               /* frame_type  */
  YYSYMBOL_frame_bound = 296,              /* frame_bound  */
  YYSYMBOL_extract_expr = 297,             /* extract_expr  */
  YYSYMBOL_cast_expr = 298,                /* cast_expr  */
  YYSYMBOL_datetime_field = 299,           /* datetime_field  */
  YYSYMBOL_datetime_field_plural = 300,    /* datetime_field_plural  */
  YYSYMBOL_duration_field = 301,           /* duration_field  */
  YYSYMBOL_array_expr = 302,               /* array_expr  */
  YYSYMBOL_array_index = 303,              /* array_index  */
  YYSYMBOL_between_expr = 304,             /* between_expr  */
  YYSYMBOL_column_name = 305,              /* column_name  */
  YYSYMBOL_nonreserved_keyword = 306,      /* nonreserved_keyword  */
  YYSYMBOL_literal = 307,                  /* literal  */
  YYSYMBOL_string_literal = 308,           /* string_literal  */
  YYSYMBOL_bool_literal = 309,             /* bool_literal  */
  YYSYMBOL_num_literal = 310,              /* num_literal  */
  YYSYMBOL_int_literal = 311,              /* int_literal  */
  YYSYMBOL_null_literal = 312,             /* null_literal  */
  YYSYMBOL_date_literal = 313,             /* date_literal  */
  YYSYMBOL_interval_literal = 314,         /* interval_literal  */
  YYSYMBOL_param_expr = 315,               /* param_expr  */
  YYSYMBOL_table_ref = 316,                /* table_ref  */
  YYSYMBOL_table_ref_atomic = 317,         /* table_ref_atomic  */
  YYSYMBOL_nonjoin_table_ref_atomic = 318, /* nonjoin_table_ref_atomic  */
  YYSYMBOL_table_value_row_list = 319,     /* table_value_row_list  */
  YYSYMBOL_table_ref_commalist = 320,      /* table_ref_commalist  */
  YYSYMBOL_table_ref_name = 321,           /* table_ref_name  */
  YYSYMBOL_table_ref_name_no_alias = 322,  /* table_ref_name_no_alias  */
  YYSYMBOL_table_name = 323,               /* table_name  */
  YYSYMBOL_opt_index_name = 324,           /* opt_index_name  */
  YYSYMBOL_table_alias = 325,              /* table_alias  */
  YYSYMBOL_opt_table_alias = 326,          /* opt_table_alias  */
  YYSYMBOL_alias = 327,                    /* alias  */
  YYSYMBOL_opt_alias = 328,                /* opt_alias  */
  YYSYMBOL_opt_locking_clause = 329,       /* opt_locking_clause  */
  YYSYMBOL_opt_locking_clause_list = 330,  /* opt_locking_clause_list  */
  YYSYMBOL_locking_clause = 331,           /* locking_clause  */
  YYSYMBOL_row_lock_mode = 332,            /* row_lock_mode  */
  YYSYMBOL_opt_row_lock_policy = 333,      /* opt_row_lock_policy  */
  YYSYMBOL_opt_with_clause = 334,          /* opt_with_clause  */
  YYSYMBOL_with_clause = 335,              /* with_clause  */
  YYSYMBOL_with_description_list = 336,    /* with_description_list  */
  YYSYMBOL_with_description = 337,         /* with_description  */
  YYSYMBOL_join_clause = 338,              /* join_clause  */
  YYSYMBOL_opt_join_type = 339,            /* opt_join_type  */
  YYSYMBOL_join_condition = 340,           /* join_condition  */
  YYSYMBOL_opt_semicolon = 341,            /* opt_semicolon  */
  YYSYMBOL_ident_commalist = 342           /* ident_commalist  */
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
#define YYLAST   1338

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  204
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  139
/* YYNRULES -- Number of rules.  */
#define YYNRULES  398
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  739

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   441


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
       2,     2,     2,     2,     2,     2,     2,   193,     2,     2,
     198,   199,   191,   189,   202,   190,   200,   192,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   201,
     182,   179,   183,   203,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   196,     2,   197,   194,     2,     2,     2,     2,     2,
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
     175,   176,   177,   178,   180,   181,   184,   185,   186,   187,
     188,   195
};

#if HSQL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   379,   379,   399,   405,   412,   416,   420,   421,   422,
     424,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     439,   440,   442,   446,   451,   455,   465,   466,   467,   469,
     469,   475,   481,   483,   487,   499,   505,   522,   537,   539,
     540,   541,   543,   557,   561,   571,   575,   599,   607,   620,
     627,   642,   662,   663,   668,   679,   692,   704,   711,   718,
     727,   728,   730,   734,   739,   740,   742,   750,   751,   752,
     753,   754,   755,   756,   760,   761,   762,   763,   764,   765,
     766,   767,   768,   769,   770,   772,   773,   775,   776,   777,
     779,   780,   782,   786,   790,   795,   803,   804,   805,   806,
     808,   809,   810,   812,   820,   826,   832,   838,   844,   845,
     852,   858,   860,   870,   877,   888,   895,   903,   904,   911,
     918,   922,   927,   937,   941,   945,   957,   957,   959,   960,
     969,   970,   972,   986,   998,  1003,  1007,  1011,  1016,  1017,
    1019,  1029,  1030,  1032,  1034,  1035,  1037,  1039,  1040,  1042,
    1047,  1049,  1050,  1052,  1053,  1055,  1059,  1064,  1066,  1067,
    1068,  1070,  1071,  1093,  1094,  1095,  1097,  1098,  1099,  1100,
    1101,  1102,  1103,  1108,  1112,  1118,  1119,  1121,  1125,  1130,
    1130,  1134,  1135,  1136,  1138,  1147,  1147,  1147,  1147,  1147,
    1149,  1150,  1150,  1150,  1150,  1150,  1150,  1150,  1150,  1151,
    1151,  1155,  1155,  1157,  1158,  1159,  1160,  1161,  1163,  1163,
    1164,  1165,  1166,  1167,  1168,  1169,  1170,  1171,  1172,  1174,
    1175,  1177,  1178,  1179,  1180,  1184,  1185,  1189,  1194,  1201,
    1202,  1203,  1204,  1206,  1207,  1209,  1210,  1212,  1213,  1214,
    1215,  1216,  1217,  1218,  1222,  1223,  1224,  1227,  1232,  1233,
    1237,  1238,  1240,  1241,  1246,  1247,  1248,  1252,  1253,  1254,
    1256,  1257,  1258,  1259,  1260,  1262,  1264,  1266,  1267,  1268,
    1269,  1270,  1271,  1273,  1274,  1275,  1276,  1277,  1278,  1280,
    1280,  1282,  1288,  1293,  1295,  1296,  1297,  1298,  1299,  1305,
    1311,  1318,  1324,  1338,  1339,  1340,  1341,  1342,  1343,  1344,
    1345,  1346,  1347,  1348,  1349,  1350,  1351,  1352,  1353,  1354,
    1355,  1356,  1357,  1359,  1359,  1359,  1359,  1359,  1359,  1359,
    1361,  1363,  1364,  1366,  1367,  1369,  1371,  1373,  1384,  1385,
    1396,  1428,  1433,  1445,  1454,  1454,  1461,  1461,  1463,  1463,
    1471,  1480,  1489,  1493,  1498,  1502,  1507,  1515,  1521,  1525,
    1529,  1540,  1541,  1543,  1543,  1545,  1545,  1547,  1551,  1552,
    1554,  1554,  1560,  1561,  1563,  1567,  1572,  1578,  1585,  1586,
    1587,  1588,  1590,  1591,  1592,  1598,  1598,  1600,  1602,  1606,
    1611,  1621,  1628,  1636,  1645,  1646,  1647,  1648,  1649,  1650,
    1651,  1652,  1653,  1654,  1656,  1662,  1662,  1665,  1669
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
  "'='", "EQUALS", "NOTEQUALS", "'<'", "'>'", "LESS", "GREATER", "LESSEQ",
  "GREATEREQ", "NOTNULL", "'+'", "'-'", "'*'", "'/'", "'%'", "'^'",
  "UMINUS", "'['", "']'", "'('", "')'", "'.'", "';'", "','", "'?'",
  "$accept", "input", "statement_list", "statement",
  "preparable_statement", "opt_hints", "hint_list", "hint",
  "transaction_statement", "opt_transaction_keyword", "prepare_statement",
  "prepare_target_query", "execute_statement", "import_statement",
  "file_type", "file_path", "opt_import_export_options",
  "import_export_options", "csv_option", "export_statement",
  "show_statement", "create_statement", "opt_not_exists",
  "table_elem_commalist", "table_elem", "column_def", "column_type",
  "opt_time_precision", "opt_decimal_specification",
  "opt_column_constraints", "column_constraints", "column_constraint",
  "table_constraint", "references_spec", "drop_statement", "opt_exists",
  "alter_statement", "alter_action", "drop_action", "delete_statement",
  "truncate_statement", "insert_statement", "opt_column_list",
  "update_statement", "update_clause_commalist", "update_clause",
  "select_statement", "select_within_set_operation",
  "select_within_set_operation_no_parentheses", "select_with_paren",
  "select_no_paren", "set_operator", "set_type", "opt_all",
  "select_clause", "opt_distinct", "select_list", "opt_from_clause",
  "from_clause", "opt_where", "opt_group", "opt_having", "opt_order",
  "order_list", "order_desc", "opt_order_type", "opt_null_ordering",
  "opt_top", "opt_limit", "expr_list", "opt_extended_literal_list",
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

#define YYPACT_NINF (-536)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-396)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1001,    54,   136,   170,   207,   136,   255,    -9,    11,   124,
     136,   182,    21,   306,   293,   331,   169,   169,   169,   352,
     157,  -536,   250,  -536,   250,  -536,  -536,  -536,  -536,  -536,
    -536,  -536,  -536,  -536,  -536,  -536,  -536,    -4,  -536,   382,
     190,  -536,   203,   281,  -536,   301,   301,   301,   136,   435,
     136,   333,  -536,   319,    -4,   327,   -14,   319,   319,   319,
     136,  -536,   353,   276,  -536,  -536,  -536,  -536,  -536,  -536,
     776,  -536,   392,  -536,  -536,   401,    84,  -536,   304,  -536,
     497,    24,   527,   422,   556,   136,   136,   495,  -536,   486,
     384,   584,   540,   136,   390,   391,   588,   588,   588,   590,
     136,   136,  -536,   396,   331,  -536,   398,    20,   581,  -536,
    -536,  -536,    -4,   481,   471,    -4,   242,   400,  -536,  -536,
     557,  -536,  -536,   403,   598,  -536,   599,  -536,  -536,    52,
    -536,   405,   404,  -536,  -536,  -536,  -536,  -536,  -536,  -536,
    -536,  -536,  -536,  -536,  -536,  -536,   559,  -536,   472,    10,
     384,   761,  -536,   588,   611,    45,   436,   -30,  -536,  -536,
     523,  -536,  -536,  -536,   -40,   -40,   -40,  -536,  -536,  -536,
    -536,  -536,   614,  -536,   613,  -536,  -536,   761,   542,  -536,
    -536,    84,  -536,  -536,   761,   542,   761,   366,   500,   621,
    -536,  -536,  -536,  -536,  -536,  -536,  -536,  -536,  -536,  -536,
    -536,  -536,  -536,  -536,  -536,   177,  -536,   354,  -536,  -536,
    -536,    24,  -536,   136,   624,   513,    41,   502,   104,  -536,
    -536,  -536,   432,   433,   437,  -536,  -536,   535,  -536,  -536,
    -536,   831,   438,  -536,  -536,  -536,  -536,  -536,  -536,  -536,
    -536,  -536,  -536,  -536,  -536,   986,  -536,   605,   -45,   253,
    -536,  -536,  -536,  -536,  -536,  -536,  -536,  -536,  -536,  -536,
    -536,  -536,  -536,  -536,  -536,   439,  -536,   533,  -536,  -118,
     440,  -536,   761,   584,  -536,   595,  -536,  -536,   442,    65,
    -536,   495,  -536,   445,     7,  -536,   446,   548,   444,  -536,
     183,   242,    -4,   448,  -536,   173,   242,   140,   591,   144,
      34,  -536,   500,  -536,  -536,   520,  -536,  -536,   453,   564,
    -536,  1080,   454,   483,   485,   187,  -536,  -536,  -536,   513,
      26,    30,   603,   354,   761,   761,   341,   -24,   466,   986,
     605,  1106,   761,   -67,   467,   465,    69,   227,   761,   761,
     986,  -536,   986,   -36,   470,   289,   986,   986,   986,   986,
     986,   986,   986,   986,   986,   986,   986,   986,   986,   986,
     986,   613,    39,   136,  -536,   666,    24,   -45,  -536,   319,
      65,   667,   669,   435,   670,   271,  -536,  -536,    24,  -536,
     614,  -536,   199,   495,  -536,   761,  -536,   241,  -536,  -536,
    -536,  -536,   761,  -536,  -536,   687,   500,   499,   761,   761,
    -536,   514,  -536,   531,   201,  -536,  1080,   611,   588,  -536,
    -536,   511,  -536,   512,  -536,  -536,   515,  -536,  -536,   516,
    -536,  -536,  -536,  -536,   517,  -536,  -536,   290,   611,   518,
     519,  -536,    41,  -536,   633,   761,   200,  -536,   521,   622,
     342,   202,   317,   761,   761,  -536,   603,   139,   616,     3,
    -536,   761,  -536,   234,   761,  -536,   604,  1085,  1124,   986,
     525,   605,  -536,   615,   528,  1124,  1124,  1124,  1124,  1142,
    1142,  1142,  1142,   -67,   -67,    60,    60,    60,   -58,   532,
     633,   761,  -536,  -536,   272,   727,   280,  -536,  -536,  -536,
    -536,  -536,   220,   285,  -536,   210,    48,    63,   439,  -536,
     179,  -536,   529,  -536,    63,  -536,   661,  -536,  -536,  -536,
    -536,   731,  -536,  -536,   563,   -45,   -45,   672,  -536,   611,
    -536,   575,  -536,   541,   302,  -536,   733,   735,  -536,   737,
     738,   754,  -536,  -536,   655,  -536,   585,   136,  -536,   290,
    -536,  -536,   303,   611,   611,  -536,   565,  -536,   322,    40,
      44,  -536,   761,  1080,   761,   761,  -536,   332,   307,   571,
    -536,  -536,   641,   574,  -536,   986,  1124,   605,   576,   323,
    -536,  -536,  -536,   324,  -536,  -536,  -536,   769,   435,  -536,
    -536,   774,   580,   582,   417,  -536,  -536,  -536,   678,  -536,
    -536,  -536,   700,   702,   703,   683,   199,  -536,   658,  -536,
    -536,   761,  -536,   109,  -536,  -536,  -536,   592,   330,   593,
     600,   602,  -536,  -536,   384,  -536,  -536,  -536,   334,   345,
     680,   633,   633,   761,   589,  -536,   161,   607,   -45,   343,
    -536,   761,  -536,   596,   603,  1106,   608,   347,  -536,  -536,
     633,  -536,  -536,   292,   761,   351,    63,   606,   199,  -536,
    -536,  -536,   199,   326,   761,   188,  -536,  -536,  -536,   784,
    -536,  -536,  -536,  -536,  -536,   619,   674,   542,  -536,  -536,
     359,    46,  -536,  -536,  -536,   -45,   603,   617,  -536,  -536,
    -536,   361,    63,   623,  -536,   611,  -536,    31,    -7,   640,
     625,  -536,   761,   413,   633,  -536,  -536,   626,  -536,  -536,
    -536,   761,   368,   628,   761,   761,  -536,  -536,  -536,   444,
    -536,  -536,  -536,   629,    73,  -536,  -536,   369,  -536,   611,
     -45,  -536,   -45,  -536,   383,    96,   397,  -536,  -536,  -536,
     374,  -536,  -536,   696,  -536,  -536,  -536,    96,  -536
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     376,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    30,    30,    30,     0,
     396,     3,    21,    19,    21,    18,     8,     9,     7,    11,
      16,    17,    13,    14,    12,    15,    10,     0,   375,     0,
     348,   114,    33,     0,    54,    61,    61,    61,     0,     0,
       0,     0,   347,   109,     0,     0,     0,   109,   109,   109,
       0,    52,     0,   377,   378,    29,    26,    28,    27,     1,
     376,     2,     0,     6,     5,   165,   123,   124,   154,   106,
       0,   176,     0,     0,   352,     0,     0,   148,    37,     0,
     118,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     4,     0,     0,   142,   136,
     137,   135,     0,   139,     0,     0,   172,   349,   320,   323,
     325,   332,   333,     0,     0,   326,     0,   321,   322,     0,
     331,     0,   175,   177,   179,   181,   313,   314,   315,   324,
     316,   317,   318,   319,    32,    31,     0,   351,     0,     0,
     118,     0,   113,     0,     0,     0,     0,   148,   120,   108,
       0,   131,   130,    38,    41,    41,    41,   107,   104,   105,
     380,   379,     0,   325,     0,   163,   141,     0,   154,   127,
     126,   128,   138,   134,     0,   154,     0,     0,   363,     0,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   328,     0,   327,   330,   182,   183,
      34,     0,    60,     0,     0,   376,     0,     0,   284,   312,
     310,   308,     0,     0,     0,   306,   305,     0,   307,   311,
     309,     0,     0,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,     0,   286,     0,   147,   185,
     192,   193,   194,   187,   189,   195,   188,   208,   196,   197,
     198,   199,   191,   186,   201,   288,   202,     0,   397,     0,
       0,   116,     0,     0,   119,     0,   110,   111,     0,     0,
      51,   148,    50,    24,     0,    22,     0,   145,   143,   173,
     361,   172,     0,   153,   155,   160,   172,   167,   169,   166,
       0,   132,   362,   364,   350,     0,   329,   178,     0,     0,
      57,     0,     0,     0,     0,     0,    62,    64,    65,   376,
     142,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   204,     0,   203,     0,     0,   361,     0,     0,     0,
       0,   205,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   142,     0,   117,     0,     0,   122,   121,   109,
       0,     0,     0,     0,     0,     0,    47,    36,     0,    20,
       0,   164,     0,   148,   144,     0,   359,     0,   360,   184,
     125,   129,     0,   159,   158,   161,   363,     0,     0,     0,
     368,     0,   370,     0,   374,   365,     0,     0,     0,    83,
      77,     0,    79,    89,    80,    67,     0,    74,    75,     0,
      71,    72,    78,    81,    86,    76,    68,    91,     0,     0,
       0,    56,     0,    59,   251,     0,   285,   287,     0,     0,
       0,     0,     0,     0,     0,   231,     0,     0,     0,     0,
     200,     0,   190,     0,     0,   219,   220,     0,   215,     0,
       0,     0,   206,     0,   218,   217,   237,   238,   239,   240,
     241,   242,   243,   210,   209,   212,   211,   213,   214,     0,
     251,     0,    35,   398,     0,     0,     0,    48,    45,    43,
      49,    40,     0,     0,    23,   348,   376,   356,     0,   146,
     334,   336,     0,   338,   356,   337,   150,   174,   357,   358,
     156,     0,   157,   133,     0,   170,   168,     0,   371,     0,
     373,     0,   366,     0,     0,    55,     0,     0,    73,     0,
       0,     0,    82,    98,     0,    97,     0,     0,    66,    90,
      92,    94,     0,     0,     0,    63,     0,   244,     0,   142,
       0,   235,     0,     0,     0,     0,   229,     0,     0,     0,
     281,   227,     0,     0,   228,     0,   216,     0,     0,     0,
     207,   282,   248,     0,   115,   112,    39,     0,     0,    46,
      25,     0,     0,     0,     0,   355,   340,   353,     0,   392,
     384,   390,   388,   391,   386,     0,     0,   346,     0,   140,
     162,     0,   369,   374,   372,   180,    58,     0,     0,     0,
       0,     0,    99,    96,   118,    93,    95,   101,     0,     0,
     253,   251,   251,     0,   289,   291,     0,     0,   233,     0,
     232,     0,   236,     0,     0,   283,     0,     0,   223,   221,
     251,    44,    42,   349,     0,     0,   356,   357,     0,   387,
     389,   385,     0,   335,     0,     0,   367,    70,    88,     0,
      84,    69,    85,   103,   100,     0,     0,   154,   245,   246,
       0,     0,   265,   266,   230,   234,     0,     0,   224,   222,
     249,     0,   356,     0,   339,     0,   381,   393,   152,     0,
       0,   102,     0,   256,   251,   290,   292,     0,   225,   342,
     341,     0,     0,     0,     0,     0,   149,   171,    87,   252,
     257,   258,   259,     0,     0,   247,   226,     0,   354,     0,
     394,   382,   151,   250,     0,     0,     0,   264,   254,   343,
       0,   263,   261,     0,   262,   260,   383,     0,   255
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -536,  -536,  -536,   750,  -536,   798,  -536,   443,  -536,   468,
    -536,  -536,  -536,  -536,  -350,   -84,   409,   457,   337,  -536,
    -536,  -536,   488,  -536,   410,  -536,  -354,  -536,  -536,  -536,
    -536,   294,  -536,  -474,  -536,   -50,  -536,  -536,  -536,  -536,
    -536,  -536,  -147,  -536,  -536,   568,  -200,   -72,  -536,    66,
     -37,   -15,  -536,  -536,   -70,  -298,  -536,  -536,  -536,  -122,
    -536,  -536,  -174,  -536,   455,  -536,  -536,  -536,    12,  -326,
    -536,   -89,   634,   627,  -220,  -150,  -206,  -536,  -536,  -536,
    -536,  -536,  -536,  -536,   524,  -536,  -536,  -366,  -461,  -536,
    -536,  -536,  -425,  -536,  -536,  -131,  -536,  -536,  -536,  -536,
    -536,  -536,  -364,   -43,  -536,  -536,   717,   -86,  -536,  -536,
     724,  -536,  -536,  -535,   206,  -536,  -536,  -536,  -536,     0,
    -536,  -536,  -453,  -228,  -536,   459,  -536,   554,  -536,   254,
    -536,  -536,  -536,   755,  -536,  -536,  -536,  -536,  -387
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    21,    22,    73,   284,   285,    23,    66,
      24,   145,    25,    26,    89,   164,   280,   375,   376,    27,
      28,    29,    84,   315,   316,   317,   427,   532,   528,   538,
     539,   540,   318,   541,    30,    93,    31,   276,   277,    32,
      33,    34,   155,    35,   157,   158,    36,   178,   179,   180,
     334,   112,   113,   183,    78,   177,   287,   383,   384,   152,
     599,   706,   116,   293,   294,   395,   512,   108,   188,   288,
     131,   132,   133,   134,   289,   290,   249,   250,   251,   252,
     253,   254,   337,   255,   327,   256,   257,   258,   547,   667,
     713,   714,   728,   259,   260,   202,   203,   204,   261,   262,
     263,   264,   265,   266,   136,   137,   138,   139,   140,   141,
     142,   143,   499,   500,   501,   645,   502,   503,    51,   504,
     148,   585,   586,   587,   389,   301,   302,   303,   404,   522,
      37,    38,    63,    64,   505,   595,   721,    71,   269
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      77,   248,    41,   217,   291,    44,   449,    99,   100,   101,
      52,   296,    56,   165,   166,   310,   497,    95,   498,   572,
     524,   175,   435,   489,    40,   331,   173,   335,   118,   119,
     120,   121,   122,   436,   295,   274,   297,   299,   135,   333,
     176,   542,   181,   185,   311,   181,   705,   624,    87,   695,
      90,   597,   523,   176,   176,   151,    75,   119,   120,   588,
     102,   653,   388,   115,   481,   616,   386,   459,   371,   267,
     443,   346,   386,   338,   123,   278,   306,   326,    55,   724,
     346,   364,    97,    39,   365,   149,   150,    48,   286,   339,
     444,   372,   725,   160,   460,   109,   445,   336,   400,   589,
     168,   169,   724,    76,   590,    75,   214,    49,   388,   548,
     270,   591,   592,   582,   124,   703,   373,   687,   271,   433,
      94,    98,   367,   331,   357,   358,   359,   360,   593,   361,
     125,   110,   603,   594,   457,   569,   458,   215,   361,    40,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   573,   618,   619,   279,   377,
     668,   669,   135,    15,   704,   507,   292,   111,   135,   170,
     401,   374,   273,    42,   440,   441,   126,   127,   128,   680,
     447,   118,   119,   120,   121,   122,   386,   338,   455,   456,
     584,   691,   439,   684,    54,   385,   387,   402,   346,   627,
     560,   399,   495,   339,   126,   385,   379,   588,   216,   380,
      43,   403,   219,   308,   129,   312,   313,   314,   174,    54,
     391,   437,   181,   371,    50,   434,   220,   130,   642,   700,
     497,   561,   498,   715,   564,   625,   221,   696,   480,   622,
     726,   637,   295,   727,   508,   509,   577,   589,   515,   516,
     225,   623,   590,   566,   360,   226,   361,   338,   338,   591,
     592,   506,   338,   726,    53,   393,   727,   124,   452,   520,
     521,   578,   340,   339,   339,   479,   593,   484,   339,   338,
    -393,   594,   497,   125,   498,   438,   497,   228,   498,   493,
     229,   338,   394,   557,   558,   339,   583,   670,   702,   186,
     733,   338,   320,   390,   321,   397,   338,   339,   396,   341,
     387,   365,   738,   554,    60,   109,   187,   339,   681,   485,
     338,   230,   339,   135,   525,    65,   374,    45,   688,   126,
     127,   128,   730,   519,    62,   135,   339,    46,   452,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   110,    69,   689,   588,    61,   342,   562,    70,   635,
     672,   520,   521,   482,   563,    72,   709,   129,    47,   218,
     118,   119,   120,   121,   122,   717,   343,    82,    57,   219,
     130,  -344,   114,   344,   345,    79,   431,   111,    58,   432,
      80,   346,   347,   220,   589,   462,   533,   496,   549,   590,
     550,    81,   626,   221,   628,   629,   591,   592,   320,   559,
     581,   555,   463,   534,   222,   223,   224,   225,   631,    59,
     647,   509,   226,   593,   568,   338,   453,  -393,   594,   454,
      83,   444,   348,   349,   350,   351,   352,   556,    88,   353,
     354,   339,   355,   356,   357,   358,   359,   360,    92,   361,
     338,   655,   630,   227,   228,   325,   124,   229,    91,   338,
     338,   338,    96,   674,   535,   536,   339,   663,   537,   553,
     491,   574,   125,   492,   211,   339,   339,   339,   104,   576,
     103,   675,   492,   298,   580,    67,    68,   211,   230,   231,
     549,   106,   189,   693,   190,   191,   192,   193,   194,   195,
     117,   606,   617,   232,   365,   365,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   126,   127,
     128,   621,   639,   640,   385,   385,   385,   107,  -345,   658,
     636,   144,   659,   664,    85,    86,   365,   614,   218,   118,
     119,   120,   121,   122,   665,   146,   679,   365,   219,   385,
     682,   731,   732,   683,   720,   722,   245,   246,   694,   147,
     699,   385,   220,   385,   247,   734,   735,   718,   729,   130,
     365,   385,   221,   736,   281,   282,   365,   710,   711,   712,
     151,   153,   154,   222,   223,   224,   225,   156,   159,   161,
     162,   226,   163,   167,    54,   176,   172,   677,   182,   184,
     189,   205,   206,   207,   210,   213,   211,   212,   218,   118,
     119,   120,   121,   122,   268,   272,   275,   283,   219,   173,
     114,   300,   227,   228,   304,   124,   229,   309,    15,   319,
     322,   323,   220,   363,   332,   324,   369,   362,   366,   697,
     370,   125,   221,   378,   382,   381,   385,   406,   398,   325,
     392,   407,   428,   222,   223,   224,   225,   230,   231,   408,
     429,   226,   430,    75,   446,    75,   450,   451,   461,   483,
     514,   487,   232,   488,   490,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   126,   127,   128,
     511,   517,   227,   228,   518,   124,   229,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   526,
     527,   125,   546,   529,   530,   531,   543,   544,   552,   459,
     551,   570,   338,   567,   361,   245,   246,   230,   231,   571,
     575,   596,   598,   247,   600,   601,   602,   604,   130,   607,
     605,   608,   232,   609,   610,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   126,   127,   128,
     611,   612,   613,   620,   218,   118,   119,   120,   121,   122,
     632,   633,   634,   641,   219,   638,  -395,   643,   644,   648,
     649,   646,   650,   651,   652,     1,   654,   666,   220,   671,
     690,   657,   660,     2,   676,   245,   246,   537,   221,   661,
       3,   662,   692,   247,   685,     4,   673,   678,   130,   222,
     223,   224,   225,   707,   737,     5,   698,   226,     6,     7,
     105,   701,    74,   494,   708,   716,   719,   486,   723,   579,
       8,     9,   305,   615,   218,   118,   119,   120,   121,   122,
      10,   368,   545,    11,   219,   307,   208,   510,   227,   228,
     442,   124,   229,   209,   686,   513,   405,   656,   220,   171,
       0,     0,     0,     0,     0,    12,     0,   125,   221,    13,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   328,
     223,   224,   225,   230,   231,    14,     0,   226,     0,     0,
       0,    15,     0,     0,     0,     0,     0,     0,   232,     0,
       0,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   126,   127,   128,     0,     0,   227,   228,
       0,   124,   229,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    16,    17,    18,     0,   125,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,   246,   230,   329,     0,     0,     0,     0,   247,
       0,     0,     0,     0,   130,     0,     0,     0,   232,     0,
       0,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   126,   127,   128,     0,     0,     0,   218,
     118,   119,   120,   121,   122,     0,     0,     0,     0,   219,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       1,     0,     0,   220,     0,     0,     0,     0,     2,     0,
       0,   245,   246,   221,     0,     3,     0,     0,     0,   330,
       4,     0,     0,     0,   130,   223,   224,   225,     0,     0,
       5,     0,   226,     6,     7,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     8,     9,     0,     0,     0,
       0,     0,     0,     0,     0,    10,     0,     0,    11,     0,
       0,     0,     0,   227,   228,     0,   124,   229,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      12,     0,   125,   409,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   410,   230,   329,
      14,   411,   412,   413,   414,   415,    15,   416,     0,     0,
       0,     0,     0,   232,     0,   417,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   126,   127,
     128,   341,     0,     0,     0,     0,     0,     0,     0,     0,
     418,     0,     0,     0,     0,     0,     0,     0,    16,    17,
      18,     0,   341,     0,     0,     0,     0,     0,   419,     0,
     420,   421,     0,     0,     0,     0,   245,   246,     0,     0,
     341,     0,     0,     0,   330,   422,     0,     0,   342,   130,
     423,     0,   424,     0,     0,     0,     0,     0,   341,     0,
       0,     0,   425,   565,     0,     0,     0,     0,   448,   342,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,   346,   347,     0,     0,  -396,     0,   448,
       0,     0,     0,     0,     0,   426,     0,   345,     0,     0,
       0,     0,     0,     0,   346,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   346,  -396,   348,   349,   350,   351,   352,     0,
       0,   353,   354,   345,   355,   356,   357,   358,   359,   360,
     346,   361,     0,     0,     0,   348,   349,   350,   351,   352,
       0,     0,   353,   354,     0,   355,   356,   357,   358,   359,
     360,     0,   361,  -396,  -396,  -396,   351,   352,     0,     0,
     353,   354,     0,   355,   356,   357,   358,   359,   360,     0,
     361,     0,     0,     0,  -396,  -396,     0,     0,  -396,  -396,
       0,   355,   356,   357,   358,   359,   360,     0,   361
};

static const yytype_int16 yycheck[] =
{
      37,   151,     2,   150,   178,     5,   332,    57,    58,    59,
      10,   185,    12,    97,    98,   215,   382,    54,   382,   480,
     407,   107,   320,   373,     3,   231,     6,   247,     4,     5,
       6,     7,     8,     3,   184,   157,   186,   187,    81,   245,
      14,   428,   112,   115,     3,   115,    53,     3,    48,     3,
      50,   504,   406,    14,    14,    85,    60,     5,     6,    28,
      60,   596,   290,    78,   362,   539,     3,   103,     3,   153,
      94,   138,     3,   118,    50,   115,   207,   227,    12,     6,
     138,   199,    96,    29,   202,    85,    86,    96,   174,   134,
     114,    26,    19,    93,   130,    11,   120,   247,    64,    68,
     100,   101,     6,    37,    73,    60,    96,    96,   336,   435,
      65,    80,    81,    65,    90,    84,    51,   652,   155,   319,
      54,   135,   272,   329,   191,   192,   193,   194,    97,   196,
     106,    47,   519,   102,   340,   461,   342,   127,   196,     3,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   481,   543,   544,   198,   281,
     621,   622,   205,   115,   133,   385,   181,    83,   211,   103,
     136,   106,   202,     3,   324,   325,   152,   153,   154,   640,
     330,     4,     5,     6,     7,     8,     3,   118,   338,   339,
     127,   665,   323,   646,   198,   202,   127,   163,   138,   553,
     197,    57,     3,   134,   152,   202,   199,    28,   198,   202,
       3,   177,    13,   213,   190,   174,   175,   176,   198,   198,
     292,   191,   292,     3,   100,   199,    27,   203,   578,   682,
     596,   451,   596,   694,   454,   191,    37,   191,   199,   199,
     167,   567,   392,   170,     3,     4,    26,    68,   398,   399,
      51,   549,    73,   459,   194,    56,   196,   118,   118,    80,
      81,   383,   118,   167,    82,    92,   170,    90,   199,   160,
     161,    51,    19,   134,   134,   361,    97,   366,   134,   118,
     101,   102,   648,   106,   648,   322,   652,    88,   652,   378,
      91,   118,   119,   443,   444,   134,   496,   623,   685,    57,
     725,   118,   198,   291,   200,   165,   118,   134,   296,    56,
     127,   202,   737,   111,    21,    11,    74,   134,   644,   369,
     118,   122,   134,   366,   408,   156,   106,    72,   654,   152,
     153,   154,   719,   132,     3,   378,   134,    82,   199,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,    47,     0,   165,    28,    62,   103,   123,   201,   565,
     199,   160,   161,   363,   130,   115,   692,   190,   113,     3,
       4,     5,     6,     7,     8,   701,   123,    96,    72,    13,
     203,   202,    78,   130,   131,     3,   199,    83,    82,   202,
     200,   138,   139,    27,    68,   106,   106,   198,   198,    73,
     200,   198,   552,    37,   554,   555,    80,    81,   198,   446,
     200,    94,   123,   123,    48,    49,    50,    51,   111,   113,
       3,     4,    56,    97,   461,   118,   199,   101,   102,   202,
     129,   114,   179,   180,   181,   182,   183,   120,     3,   186,
     187,   134,   189,   190,   191,   192,   193,   194,   129,   196,
     118,   601,   120,    87,    88,   114,    90,    91,   125,   118,
     118,   118,   135,   120,   174,   175,   134,   614,   178,   127,
     199,   199,   106,   202,   202,   134,   134,   134,   202,   199,
     127,   631,   202,   117,   199,    17,    18,   202,   122,   123,
     198,    99,   200,   667,   140,   141,   142,   143,   144,   145,
       3,   199,   199,   137,   202,   202,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   199,   199,   199,   202,   202,   202,   126,   202,   199,
     567,     4,   202,   199,    46,    47,   202,   537,     3,     4,
       5,     6,     7,     8,   199,   123,   199,   202,    13,   202,
     199,   168,   169,   202,   704,   705,   190,   191,   199,     3,
     199,   202,    27,   202,   198,   168,   169,   199,   199,   203,
     202,   202,    37,   199,   165,   166,   202,   164,   165,   166,
      85,    95,   198,    48,    49,    50,    51,     3,    48,   199,
     199,    56,     4,     3,   198,    14,   198,   634,   117,   128,
     200,   198,     4,     4,   199,   133,   202,    48,     3,     4,
       5,     6,     7,     8,     3,   179,    93,     3,    13,     6,
      78,   121,    87,    88,     3,    90,    91,     3,   115,   127,
     198,   198,    27,   100,   196,   198,    41,   198,   198,   676,
     198,   106,    37,   198,    96,   199,   202,   127,    57,   114,
     202,   198,   198,    48,    49,    50,    51,   122,   123,    95,
     177,    56,   177,    60,   198,    60,   199,   202,   198,     3,
     171,     4,   137,     4,     4,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
       3,   177,    87,    88,   163,    90,    91,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   198,
     198,   106,    79,   198,   198,   198,   198,   198,    96,   103,
     199,   106,   118,   198,   196,   190,   191,   122,   123,   197,
       3,   202,    71,   198,     3,   172,    64,   162,   203,     6,
     199,     6,   137,     6,     6,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
       6,   106,   177,   198,     3,     4,     5,     6,     7,     8,
     199,   130,   198,     4,    13,   199,     0,     3,   198,   101,
      80,   199,    80,    80,   101,     9,   128,   107,    27,   200,
       6,   199,   199,    17,   198,   190,   191,   178,    37,   199,
      24,   199,   128,   198,   198,    29,   199,   199,   203,    48,
      49,    50,    51,   173,   118,    39,   199,    56,    42,    43,
      70,   198,    24,   380,   199,   199,   198,   370,   199,   492,
      54,    55,   205,   539,     3,     4,     5,     6,     7,     8,
      64,   273,   432,    67,    13,   211,   129,   392,    87,    88,
     326,    90,    91,   129,   648,   396,   302,   603,    27,   104,
      -1,    -1,    -1,    -1,    -1,    89,    -1,   106,    37,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,   122,   123,   109,    -1,    56,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,    87,    88,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,   159,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   191,   122,   123,    -1,    -1,    -1,    -1,   198,
      -1,    -1,    -1,    -1,   203,    -1,    -1,    -1,   137,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    27,    -1,    -1,    -1,    -1,    17,    -1,
      -1,   190,   191,    37,    -1,    24,    -1,    -1,    -1,   198,
      29,    -1,    -1,    -1,   203,    49,    50,    51,    -1,    -1,
      39,    -1,    56,    42,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    87,    88,    -1,    90,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,   106,    13,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,   122,   123,
     109,    31,    32,    33,    34,    35,   115,    37,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    45,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,
     159,    -1,    56,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      90,    91,    -1,    -1,    -1,    -1,   190,   191,    -1,    -1,
      56,    -1,    -1,    -1,   198,   105,    -1,    -1,   103,   203,
     110,    -1,   112,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,   122,   118,    -1,    -1,    -1,    -1,   123,   103,
      -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,
      -1,    -1,    -1,   138,   139,    -1,    -1,   103,    -1,   123,
      -1,    -1,    -1,    -1,    -1,   155,    -1,   131,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   179,   180,   181,   182,   183,    -1,
      -1,   186,   187,   131,   189,   190,   191,   192,   193,   194,
     138,   196,    -1,    -1,    -1,   179,   180,   181,   182,   183,
      -1,    -1,   186,   187,    -1,   189,   190,   191,   192,   193,
     194,    -1,   196,   179,   180,   181,   182,   183,    -1,    -1,
     186,   187,    -1,   189,   190,   191,   192,   193,   194,    -1,
     196,    -1,    -1,    -1,   182,   183,    -1,    -1,   186,   187,
      -1,   189,   190,   191,   192,   193,   194,    -1,   196
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     9,    17,    24,    29,    39,    42,    43,    54,    55,
      64,    67,    89,    93,   109,   115,   157,   158,   159,   205,
     206,   207,   208,   212,   214,   216,   217,   223,   224,   225,
     238,   240,   243,   244,   245,   247,   250,   334,   335,    29,
       3,   323,     3,     3,   323,    72,    82,   113,    96,    96,
     100,   322,   323,    82,   198,   253,   323,    72,    82,   113,
      21,    62,     3,   336,   337,   156,   213,   213,   213,     0,
     201,   341,   115,   209,   209,    60,   253,   254,   258,     3,
     200,   198,    96,   129,   226,   226,   226,   323,     3,   218,
     323,   125,   129,   239,   253,   254,   135,    96,   135,   239,
     239,   239,   323,   127,   202,   207,    99,   126,   271,    11,
      47,    83,   255,   256,    78,   255,   266,     3,     4,     5,
       6,     7,     8,    50,    90,   106,   152,   153,   154,   190,
     203,   274,   275,   276,   277,   307,   308,   309,   310,   311,
     312,   313,   314,   315,     4,   215,   123,     3,   324,   323,
     323,    85,   263,    95,   198,   246,     3,   248,   249,    48,
     323,   199,   199,     4,   219,   219,   219,     3,   323,   323,
     253,   337,   198,     6,   198,   311,    14,   259,   251,   252,
     253,   258,   117,   257,   128,   251,    57,    74,   272,   200,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   299,   300,   301,   198,     4,     4,   310,   314,
     199,   202,    48,   133,    96,   127,   198,   246,     3,    13,
      27,    37,    48,    49,    50,    51,    56,    87,    88,    91,
     122,   123,   137,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   190,   191,   198,   279,   280,
     281,   282,   283,   284,   285,   287,   289,   290,   291,   297,
     298,   302,   303,   304,   305,   306,   307,   219,     3,   342,
      65,   254,   179,   202,   263,    93,   241,   242,   115,   198,
     220,   220,   220,     3,   210,   211,   311,   260,   273,   278,
     279,   266,   255,   267,   268,   279,   266,   279,   117,   279,
     121,   329,   330,   331,     3,   277,   299,   276,   323,     3,
     250,     3,   174,   175,   176,   227,   228,   229,   236,   127,
     198,   200,   198,   198,   198,   114,   279,   288,    48,   123,
     198,   280,   196,   280,   254,   278,   279,   286,   118,   134,
      19,    56,   103,   123,   130,   131,   138,   139,   179,   180,
     181,   182,   183,   186,   187,   189,   190,   191,   192,   193,
     194,   196,   198,   100,   199,   202,   198,   279,   249,    41,
     198,     3,    26,    51,   106,   221,   222,   263,   198,   199,
     202,   199,    96,   261,   262,   202,     3,   127,   327,   328,
     272,   251,   202,    92,   119,   269,   272,   165,    57,    57,
      64,   136,   163,   177,   332,   331,   127,   198,    95,    13,
      27,    31,    32,    33,    34,    35,    37,    45,    70,    88,
      90,    91,   105,   110,   112,   122,   155,   230,   198,   177,
     177,   199,   202,   250,   199,   259,     3,   191,   254,   299,
     279,   279,   288,    94,   114,   120,   198,   279,   123,   273,
     199,   202,   199,   199,   202,   279,   279,   280,   280,   103,
     130,   198,   106,   123,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   311,
     199,   259,   323,     3,   275,   239,   221,     4,     4,   218,
       4,   199,   202,   275,   211,     3,   198,   291,   306,   316,
     317,   318,   320,   321,   323,   338,   263,   278,     3,     4,
     268,     3,   270,   329,   171,   279,   279,   177,   163,   132,
     160,   161,   333,   230,   342,   219,   198,   198,   232,   198,
     198,   198,   231,   106,   123,   174,   175,   178,   233,   234,
     235,   237,   342,   198,   198,   228,    79,   292,   273,   198,
     200,   199,    96,   127,   111,    94,   120,   279,   279,   254,
     197,   278,   123,   130,   278,   118,   280,   198,   254,   273,
     106,   197,   292,   273,   199,     3,   199,    26,    51,   222,
     199,   200,    65,   250,   127,   325,   326,   327,    28,    68,
      73,    80,    81,    97,   102,   339,   202,   326,    71,   264,
       3,   172,    64,   342,   162,   199,   199,     6,     6,     6,
       6,     6,   106,   177,   323,   235,   237,   199,   342,   342,
     198,   199,   199,   259,     3,   191,   279,   230,   279,   279,
     120,   111,   199,   130,   198,   280,   254,   273,   199,   199,
     199,     4,   218,     3,   198,   319,   199,     3,   101,    80,
      80,    80,   101,   317,   128,   279,   333,   199,   199,   202,
     199,   199,   199,   246,   199,   199,   107,   293,   292,   292,
     273,   200,   199,   199,   120,   279,   198,   254,   199,   199,
     292,   273,   199,   202,   326,   198,   318,   317,   273,   165,
       6,   237,   128,   266,   199,     3,   191,   254,   199,   199,
     326,   198,   342,    84,   133,    53,   265,   173,   199,   273,
     164,   165,   166,   294,   295,   292,   199,   273,   199,   198,
     279,   340,   279,   199,     6,    19,   167,   170,   296,   199,
     342,   168,   169,   296,   168,   169,   199,   118,   296
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   204,   205,   206,   206,   207,   207,   207,   207,   207,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     209,   209,   210,   210,   211,   211,   212,   212,   212,   213,
     213,   214,   215,   216,   216,   217,   217,   218,   219,   220,
     220,   220,   221,   221,   221,   221,   221,   221,   222,   222,
     223,   223,   224,   224,   224,   225,   225,   225,   225,   225,
     226,   226,   227,   227,   228,   228,   229,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   231,   231,   232,   232,   232,
     233,   233,   234,   234,   234,   234,   235,   235,   235,   235,
     236,   236,   236,   237,   238,   238,   238,   238,   239,   239,
     240,   241,   242,   243,   244,   245,   245,   246,   246,   247,
     248,   248,   249,   250,   250,   250,   251,   251,   252,   252,
     253,   253,   254,   254,   255,   256,   256,   256,   257,   257,
     258,   259,   259,   260,   261,   261,   262,   263,   263,   264,
     264,   265,   265,   266,   266,   267,   267,   268,   269,   269,
     269,   270,   270,   271,   271,   271,   272,   272,   272,   272,
     272,   272,   272,   273,   273,   274,   274,   275,   275,   276,
     276,   277,   277,   277,   278,   279,   279,   279,   279,   279,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   281,   281,   282,   282,   282,   282,   282,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   284,
     284,   285,   285,   285,   285,   285,   285,   286,   286,   287,
     287,   287,   287,   288,   288,   289,   289,   290,   290,   290,
     290,   290,   290,   290,   291,   291,   291,   291,   291,   291,
     292,   292,   293,   293,   294,   294,   294,   295,   295,   295,
     296,   296,   296,   296,   296,   297,   298,   299,   299,   299,
     299,   299,   299,   300,   300,   300,   300,   300,   300,   301,
     301,   302,   303,   304,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   307,   307,   307,   307,   307,   307,   307,
     308,   309,   309,   310,   310,   311,   312,   313,   314,   314,
     314,   315,   315,   315,   316,   316,   317,   317,   318,   318,
     318,   318,   319,   319,   320,   320,   321,   322,   323,   323,
     323,   324,   324,   325,   325,   326,   326,   327,   327,   327,
     328,   328,   329,   329,   330,   330,   331,   331,   332,   332,
     332,   332,   333,   333,   333,   334,   334,   335,   336,   336,
     337,   338,   338,   338,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   340,   341,   341,   342,   342
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
       7,     1,     0,     1,     1,     0,     2,     2,     0,     4,
       0,     2,     0,     3,     0,     1,     3,     3,     1,     1,
       0,     0,     2,     2,     4,     0,     2,     2,     4,     2,
       4,     8,     0,     1,     3,     1,     0,     1,     3,     1,
       6,     1,     2,     2,     2,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     2,     2,     2,     3,     4,     1,     3,
       3,     3,     3,     3,     3,     3,     4,     3,     3,     3,
       3,     5,     6,     5,     6,     7,     8,     3,     3,     4,
       6,     3,     5,     4,     5,     4,     5,     3,     3,     3,
       3,     3,     3,     3,     4,     6,     6,     8,     4,     6,
       6,     0,     3,     0,     2,     5,     0,     1,     1,     1,
       2,     2,     2,     2,     1,     6,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     5,     1,     3,     1,     3,     1,     5,
       7,     5,     7,     1,     1,     1,     1,     1,     1,     1,
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
#line 2247 "bison_parser.cpp"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2253 "bison_parser.cpp"
        break;

    case YYSYMBOL_FLOATVAL: /* FLOATVAL  */
#line 196 "bison_parser.y"
            { }
#line 2259 "bison_parser.cpp"
        break;

    case YYSYMBOL_INTVAL: /* INTVAL  */
#line 196 "bison_parser.y"
            { }
#line 2265 "bison_parser.cpp"
        break;

    case YYSYMBOL_DOLLAR_PARAM: /* DOLLAR_PARAM  */
#line 196 "bison_parser.y"
            { }
#line 2271 "bison_parser.cpp"
        break;

    case YYSYMBOL_NAMED_PARAM: /* NAMED_PARAM  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2277 "bison_parser.cpp"
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
#line 2290 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2296 "bison_parser.cpp"
        break;

    case YYSYMBOL_preparable_statement: /* preparable_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2302 "bison_parser.cpp"
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
#line 2315 "bison_parser.cpp"
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
#line 2328 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint: /* hint  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2334 "bison_parser.cpp"
        break;

    case YYSYMBOL_transaction_statement: /* transaction_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).transaction_stmt)); }
#line 2340 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_statement: /* prepare_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).prep_stmt)); }
#line 2346 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_target_query: /* prepare_target_query  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2352 "bison_parser.cpp"
        break;

    case YYSYMBOL_execute_statement: /* execute_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2358 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_statement: /* import_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_stmt)); }
#line 2364 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_type: /* file_type  */
#line 196 "bison_parser.y"
            { }
#line 2370 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_path: /* file_path  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2376 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_import_export_options: /* opt_import_export_options  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2382 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_export_options: /* import_export_options  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2388 "bison_parser.cpp"
        break;

    case YYSYMBOL_csv_option: /* csv_option  */
#line 218 "bison_parser.y"
            {
  free(((*yyvaluep).csv_option_t)->second);
  delete (((*yyvaluep).csv_option_t));
}
#line 2397 "bison_parser.cpp"
        break;

    case YYSYMBOL_export_statement: /* export_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).export_stmt)); }
#line 2403 "bison_parser.cpp"
        break;

    case YYSYMBOL_show_statement: /* show_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).show_stmt)); }
#line 2409 "bison_parser.cpp"
        break;

    case YYSYMBOL_create_statement: /* create_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).create_stmt)); }
#line 2415 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_not_exists: /* opt_not_exists  */
#line 196 "bison_parser.y"
            { }
#line 2421 "bison_parser.cpp"
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
#line 2434 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem: /* table_elem  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table_element_t)); }
#line 2440 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_def: /* column_def  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).column_t)); }
#line 2446 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_type: /* column_type  */
#line 196 "bison_parser.y"
            { }
#line 2452 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_time_precision: /* opt_time_precision  */
#line 196 "bison_parser.y"
            { }
#line 2458 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_decimal_specification: /* opt_decimal_specification  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).ival_pair)); }
#line 2464 "bison_parser.cpp"
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
#line 2486 "bison_parser.cpp"
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
#line 2508 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraint: /* column_constraint  */
#line 196 "bison_parser.y"
            { }
#line 2514 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_constraint: /* table_constraint  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table_constraint_t)); }
#line 2520 "bison_parser.cpp"
        break;

    case YYSYMBOL_references_spec: /* references_spec  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).references_spec_t)); }
#line 2526 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_statement: /* drop_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).drop_stmt)); }
#line 2532 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_exists: /* opt_exists  */
#line 196 "bison_parser.y"
            { }
#line 2538 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_statement: /* alter_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alter_stmt)); }
#line 2544 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_action: /* alter_action  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alter_action_t)); }
#line 2550 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_action: /* drop_action  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).drop_action_t)); }
#line 2556 "bison_parser.cpp"
        break;

    case YYSYMBOL_delete_statement: /* delete_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2562 "bison_parser.cpp"
        break;

    case YYSYMBOL_truncate_statement: /* truncate_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2568 "bison_parser.cpp"
        break;

    case YYSYMBOL_insert_statement: /* insert_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).insert_stmt)); }
#line 2574 "bison_parser.cpp"
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
#line 2587 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_statement: /* update_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).update_stmt)); }
#line 2593 "bison_parser.cpp"
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
#line 2606 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause: /* update_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).update_t)); }
#line 2612 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_statement: /* select_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2618 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation: /* select_within_set_operation  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2624 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation_no_parentheses: /* select_within_set_operation_no_parentheses  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2630 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_with_paren: /* select_with_paren  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2636 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_no_paren: /* select_no_paren  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2642 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_operator: /* set_operator  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2648 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_type: /* set_type  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2654 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_all: /* opt_all  */
#line 196 "bison_parser.y"
            { }
#line 2660 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_clause: /* select_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2666 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_distinct: /* opt_distinct  */
#line 196 "bison_parser.y"
            { }
#line 2672 "bison_parser.cpp"
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
#line 2685 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_from_clause: /* opt_from_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2691 "bison_parser.cpp"
        break;

    case YYSYMBOL_from_clause: /* from_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2697 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_where: /* opt_where  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2703 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_group: /* opt_group  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).group_t)); }
#line 2709 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_having: /* opt_having  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2715 "bison_parser.cpp"
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
#line 2728 "bison_parser.cpp"
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
#line 2741 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_desc: /* order_desc  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).order)); }
#line 2747 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order_type: /* opt_order_type  */
#line 196 "bison_parser.y"
            { }
#line 2753 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_null_ordering: /* opt_null_ordering  */
#line 196 "bison_parser.y"
            { }
#line 2759 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_top: /* opt_top  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 2765 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_limit: /* opt_limit  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 2771 "bison_parser.cpp"
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
#line 2784 "bison_parser.cpp"
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
#line 2797 "bison_parser.cpp"
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
#line 2810 "bison_parser.cpp"
        break;

    case YYSYMBOL_casted_extended_literal: /* casted_extended_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2816 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal: /* extended_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2822 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_alias: /* expr_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2828 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr: /* expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2834 "bison_parser.cpp"
        break;

    case YYSYMBOL_operand: /* operand  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2840 "bison_parser.cpp"
        break;

    case YYSYMBOL_scalar_expr: /* scalar_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2846 "bison_parser.cpp"
        break;

    case YYSYMBOL_unary_expr: /* unary_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2852 "bison_parser.cpp"
        break;

    case YYSYMBOL_binary_expr: /* binary_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2858 "bison_parser.cpp"
        break;

    case YYSYMBOL_logic_expr: /* logic_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2864 "bison_parser.cpp"
        break;

    case YYSYMBOL_in_expr: /* in_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2870 "bison_parser.cpp"
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
#line 2883 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_expr: /* case_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2889 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_list: /* case_list  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2895 "bison_parser.cpp"
        break;

    case YYSYMBOL_exists_expr: /* exists_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2901 "bison_parser.cpp"
        break;

    case YYSYMBOL_comp_expr: /* comp_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2907 "bison_parser.cpp"
        break;

    case YYSYMBOL_function_expr: /* function_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2913 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_window: /* opt_window  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).window_description)); }
#line 2919 "bison_parser.cpp"
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
#line 2932 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_frame_clause: /* opt_frame_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).frame_description)); }
#line 2938 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_type: /* frame_type  */
#line 196 "bison_parser.y"
            { }
#line 2944 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_bound: /* frame_bound  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).frame_bound)); }
#line 2950 "bison_parser.cpp"
        break;

    case YYSYMBOL_extract_expr: /* extract_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2956 "bison_parser.cpp"
        break;

    case YYSYMBOL_cast_expr: /* cast_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2962 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field: /* datetime_field  */
#line 196 "bison_parser.y"
            { }
#line 2968 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field_plural: /* datetime_field_plural  */
#line 196 "bison_parser.y"
            { }
#line 2974 "bison_parser.cpp"
        break;

    case YYSYMBOL_duration_field: /* duration_field  */
#line 196 "bison_parser.y"
            { }
#line 2980 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_expr: /* array_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2986 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_index: /* array_index  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2992 "bison_parser.cpp"
        break;

    case YYSYMBOL_between_expr: /* between_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2998 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_name: /* column_name  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3004 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonreserved_keyword: /* nonreserved_keyword  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3010 "bison_parser.cpp"
        break;

    case YYSYMBOL_literal: /* literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3016 "bison_parser.cpp"
        break;

    case YYSYMBOL_string_literal: /* string_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3022 "bison_parser.cpp"
        break;

    case YYSYMBOL_bool_literal: /* bool_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3028 "bison_parser.cpp"
        break;

    case YYSYMBOL_num_literal: /* num_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3034 "bison_parser.cpp"
        break;

    case YYSYMBOL_int_literal: /* int_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3040 "bison_parser.cpp"
        break;

    case YYSYMBOL_null_literal: /* null_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3046 "bison_parser.cpp"
        break;

    case YYSYMBOL_date_literal: /* date_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3052 "bison_parser.cpp"
        break;

    case YYSYMBOL_interval_literal: /* interval_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3058 "bison_parser.cpp"
        break;

    case YYSYMBOL_param_expr: /* param_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3064 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref: /* table_ref  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3070 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_atomic: /* table_ref_atomic  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3076 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonjoin_table_ref_atomic: /* nonjoin_table_ref_atomic  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3082 "bison_parser.cpp"
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
#line 3095 "bison_parser.cpp"
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
#line 3108 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name: /* table_ref_name  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3114 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name_no_alias: /* table_ref_name_no_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3120 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_name: /* table_name  */
#line 197 "bison_parser.y"
            {
  free(((*yyvaluep).table_name).name);
  free(((*yyvaluep).table_name).schema);
}
#line 3129 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_index_name: /* opt_index_name  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3135 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_alias: /* table_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3141 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_table_alias: /* opt_table_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3147 "bison_parser.cpp"
        break;

    case YYSYMBOL_alias: /* alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3153 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_alias: /* opt_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3159 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause: /* opt_locking_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3165 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause_list: /* opt_locking_clause_list  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3171 "bison_parser.cpp"
        break;

    case YYSYMBOL_locking_clause: /* locking_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_t)); }
#line 3177 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_lock_mode: /* row_lock_mode  */
#line 196 "bison_parser.y"
            { }
#line 3183 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_row_lock_policy: /* opt_row_lock_policy  */
#line 196 "bison_parser.y"
            { }
#line 3189 "bison_parser.cpp"
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
#line 3202 "bison_parser.cpp"
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
#line 3215 "bison_parser.cpp"
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
#line 3228 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description: /* with_description  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).with_description_t)); }
#line 3234 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_clause: /* join_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3240 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_join_type: /* opt_join_type  */
#line 196 "bison_parser.y"
            { }
#line 3246 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_condition: /* join_condition  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3252 "bison_parser.cpp"
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
#line 3265 "bison_parser.cpp"
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

#line 3373 "bison_parser.cpp"

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
#line 379 "bison_parser.y"
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
#line 3603 "bison_parser.cpp"
    break;

  case 3: /* statement_list: statement  */
#line 399 "bison_parser.y"
                           {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyval.stmt_vec) = new std::vector<SQLStatement*>();
  (yyval.stmt_vec)->push_back((yyvsp[0].statement));
}
#line 3614 "bison_parser.cpp"
    break;

  case 4: /* statement_list: statement_list ';' statement  */
#line 405 "bison_parser.y"
                               {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyvsp[-2].stmt_vec)->push_back((yyvsp[0].statement));
  (yyval.stmt_vec) = (yyvsp[-2].stmt_vec);
}
#line 3625 "bison_parser.cpp"
    break;

  case 5: /* statement: prepare_statement opt_hints  */
#line 412 "bison_parser.y"
                                        {
  (yyval.statement) = (yyvsp[-1].prep_stmt);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3634 "bison_parser.cpp"
    break;

  case 6: /* statement: preparable_statement opt_hints  */
#line 416 "bison_parser.y"
                                 {
  (yyval.statement) = (yyvsp[-1].statement);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3643 "bison_parser.cpp"
    break;

  case 7: /* statement: show_statement  */
#line 420 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].show_stmt); }
#line 3649 "bison_parser.cpp"
    break;

  case 8: /* statement: import_statement  */
#line 421 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].import_stmt); }
#line 3655 "bison_parser.cpp"
    break;

  case 9: /* statement: export_statement  */
#line 422 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].export_stmt); }
#line 3661 "bison_parser.cpp"
    break;

  case 10: /* preparable_statement: select_statement  */
#line 424 "bison_parser.y"
                                        { (yyval.statement) = (yyvsp[0].select_stmt); }
#line 3667 "bison_parser.cpp"
    break;

  case 11: /* preparable_statement: create_statement  */
#line 425 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].create_stmt); }
#line 3673 "bison_parser.cpp"
    break;

  case 12: /* preparable_statement: insert_statement  */
#line 426 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].insert_stmt); }
#line 3679 "bison_parser.cpp"
    break;

  case 13: /* preparable_statement: delete_statement  */
#line 427 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3685 "bison_parser.cpp"
    break;

  case 14: /* preparable_statement: truncate_statement  */
#line 428 "bison_parser.y"
                     { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3691 "bison_parser.cpp"
    break;

  case 15: /* preparable_statement: update_statement  */
#line 429 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].update_stmt); }
#line 3697 "bison_parser.cpp"
    break;

  case 16: /* preparable_statement: drop_statement  */
#line 430 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].drop_stmt); }
#line 3703 "bison_parser.cpp"
    break;

  case 17: /* preparable_statement: alter_statement  */
#line 431 "bison_parser.y"
                  { (yyval.statement) = (yyvsp[0].alter_stmt); }
#line 3709 "bison_parser.cpp"
    break;

  case 18: /* preparable_statement: execute_statement  */
#line 432 "bison_parser.y"
                    { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 3715 "bison_parser.cpp"
    break;

  case 19: /* preparable_statement: transaction_statement  */
#line 433 "bison_parser.y"
                        { (yyval.statement) = (yyvsp[0].transaction_stmt); }
#line 3721 "bison_parser.cpp"
    break;

  case 20: /* opt_hints: WITH HINT '(' hint_list ')'  */
#line 439 "bison_parser.y"
                                        { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 3727 "bison_parser.cpp"
    break;

  case 21: /* opt_hints: %empty  */
#line 440 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 3733 "bison_parser.cpp"
    break;

  case 22: /* hint_list: hint  */
#line 442 "bison_parser.y"
                 {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 3742 "bison_parser.cpp"
    break;

  case 23: /* hint_list: hint_list ',' hint  */
#line 446 "bison_parser.y"
                     {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 3751 "bison_parser.cpp"
    break;

  case 24: /* hint: IDENTIFIER  */
#line 451 "bison_parser.y"
                  {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[0].sval);
}
#line 3760 "bison_parser.cpp"
    break;

  case 25: /* hint: IDENTIFIER '(' extended_literal_list ')'  */
#line 455 "bison_parser.y"
                                           {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[-3].sval);
  (yyval.expr)->exprList = (yyvsp[-1].expr_vec);
}
#line 3770 "bison_parser.cpp"
    break;

  case 26: /* transaction_statement: BEGIN opt_transaction_keyword  */
#line 465 "bison_parser.y"
                                                      { (yyval.transaction_stmt) = new TransactionStatement(kBeginTransaction); }
#line 3776 "bison_parser.cpp"
    break;

  case 27: /* transaction_statement: ROLLBACK opt_transaction_keyword  */
#line 466 "bison_parser.y"
                                   { (yyval.transaction_stmt) = new TransactionStatement(kRollbackTransaction); }
#line 3782 "bison_parser.cpp"
    break;

  case 28: /* transaction_statement: COMMIT opt_transaction_keyword  */
#line 467 "bison_parser.y"
                                 { (yyval.transaction_stmt) = new TransactionStatement(kCommitTransaction); }
#line 3788 "bison_parser.cpp"
    break;

  case 31: /* prepare_statement: PREPARE IDENTIFIER FROM prepare_target_query  */
#line 475 "bison_parser.y"
                                                                 {
  (yyval.prep_stmt) = new PrepareStatement();
  (yyval.prep_stmt)->name = (yyvsp[-2].sval);
  (yyval.prep_stmt)->query = (yyvsp[0].sval);
}
#line 3798 "bison_parser.cpp"
    break;

  case 33: /* execute_statement: EXECUTE IDENTIFIER  */
#line 483 "bison_parser.y"
                                       {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[0].sval);
}
#line 3807 "bison_parser.cpp"
    break;

  case 34: /* execute_statement: EXECUTE IDENTIFIER '(' opt_extended_literal_list ')'  */
#line 487 "bison_parser.y"
                                                       {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[-3].sval);
  (yyval.exec_stmt)->parameters = (yyvsp[-1].expr_vec);
}
#line 3817 "bison_parser.cpp"
    break;

  case 35: /* import_statement: IMPORT FROM file_type FILE file_path INTO table_name  */
#line 499 "bison_parser.y"
                                                                        {
  (yyval.import_stmt) = new ImportStatement((yyvsp[-4].import_type_t));
  (yyval.import_stmt)->filePath = (yyvsp[-2].sval);
  (yyval.import_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.import_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 3828 "bison_parser.cpp"
    break;

  case 36: /* import_statement: COPY table_name FROM file_path opt_import_export_options opt_where  */
#line 505 "bison_parser.y"
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
#line 3849 "bison_parser.cpp"
    break;

  case 37: /* file_type: IDENTIFIER  */
#line 522 "bison_parser.y"
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
#line 3868 "bison_parser.cpp"
    break;

  case 38: /* file_path: STRING  */
#line 537 "bison_parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 3874 "bison_parser.cpp"
    break;

  case 39: /* opt_import_export_options: WITH '(' import_export_options ')'  */
#line 539 "bison_parser.y"
                                                               { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 3880 "bison_parser.cpp"
    break;

  case 40: /* opt_import_export_options: '(' import_export_options ')'  */
#line 540 "bison_parser.y"
                                { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 3886 "bison_parser.cpp"
    break;

  case 41: /* opt_import_export_options: %empty  */
#line 541 "bison_parser.y"
              { (yyval.import_export_option_t) = new ImportExportOptions{}; }
#line 3892 "bison_parser.cpp"
    break;

  case 42: /* import_export_options: import_export_options ',' FORMAT file_type  */
#line 543 "bison_parser.y"
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
#line 3911 "bison_parser.cpp"
    break;

  case 43: /* import_export_options: FORMAT file_type  */
#line 557 "bison_parser.y"
                   {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->format = (yyvsp[0].import_type_t);
}
#line 3920 "bison_parser.cpp"
    break;

  case 44: /* import_export_options: import_export_options ',' ENCODING STRING  */
#line 561 "bison_parser.y"
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
#line 3935 "bison_parser.cpp"
    break;

  case 45: /* import_export_options: ENCODING STRING  */
#line 571 "bison_parser.y"
                  {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->encoding = (yyvsp[0].sval);
}
#line 3944 "bison_parser.cpp"
    break;

  case 46: /* import_export_options: import_export_options ',' csv_option  */
#line 575 "bison_parser.y"
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
#line 3973 "bison_parser.cpp"
    break;

  case 47: /* import_export_options: csv_option  */
#line 599 "bison_parser.y"
             {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->csv_options = new CsvOptions{};
  (yyval.import_export_option_t)->csv_options->accept_csv_option((yyvsp[0].csv_option_t));

  delete (yyvsp[0].csv_option_t);
}
#line 3985 "bison_parser.cpp"
    break;

  case 48: /* csv_option: IDENTIFIER STRING  */
#line 607 "bison_parser.y"
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
#line 4003 "bison_parser.cpp"
    break;

  case 49: /* csv_option: NULL STRING  */
#line 620 "bison_parser.y"
              { (yyval.csv_option_t) = new std::pair<CsvOptionType, char*>(CsvOptionType::Null, (yyvsp[0].sval)); }
#line 4009 "bison_parser.cpp"
    break;

  case 50: /* export_statement: COPY table_name TO file_path opt_import_export_options  */
#line 627 "bison_parser.y"
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
#line 4029 "bison_parser.cpp"
    break;

  case 51: /* export_statement: COPY select_with_paren TO file_path opt_import_export_options  */
#line 642 "bison_parser.y"
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
#line 4048 "bison_parser.cpp"
    break;

  case 52: /* show_statement: SHOW TABLES  */
#line 662 "bison_parser.y"
                             { (yyval.show_stmt) = new ShowStatement(kShowTables); }
#line 4054 "bison_parser.cpp"
    break;

  case 53: /* show_statement: SHOW COLUMNS table_name  */
#line 663 "bison_parser.y"
                          {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4064 "bison_parser.cpp"
    break;

  case 54: /* show_statement: DESCRIBE table_name  */
#line 668 "bison_parser.y"
                      {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4074 "bison_parser.cpp"
    break;

  case 55: /* create_statement: CREATE TABLE opt_not_exists table_name FROM IDENTIFIER FILE file_path  */
#line 679 "bison_parser.y"
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
#line 4092 "bison_parser.cpp"
    break;

  case 56: /* create_statement: CREATE TABLE opt_not_exists table_name '(' table_elem_commalist ')'  */
#line 692 "bison_parser.y"
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
#line 4109 "bison_parser.cpp"
    break;

  case 57: /* create_statement: CREATE TABLE opt_not_exists table_name AS select_statement  */
#line 704 "bison_parser.y"
                                                             {
  (yyval.create_stmt) = new CreateStatement(kCreateTable);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-3].bval);
  (yyval.create_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4121 "bison_parser.cpp"
    break;

  case 58: /* create_statement: CREATE INDEX opt_not_exists opt_index_name ON table_name '(' ident_commalist ')'  */
#line 711 "bison_parser.y"
                                                                                   {
  (yyval.create_stmt) = new CreateStatement(kCreateIndex);
  (yyval.create_stmt)->indexName = (yyvsp[-5].sval);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-6].bval);
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->indexColumns = (yyvsp[-1].str_vec);
}
#line 4133 "bison_parser.cpp"
    break;

  case 59: /* create_statement: CREATE VIEW opt_not_exists table_name opt_column_list AS select_statement  */
#line 718 "bison_parser.y"
                                                                            {
  (yyval.create_stmt) = new CreateStatement(kCreateView);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-4].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->viewColumns = (yyvsp[-2].str_vec);
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4146 "bison_parser.cpp"
    break;

  case 60: /* opt_not_exists: IF NOT EXISTS  */
#line 727 "bison_parser.y"
                               { (yyval.bval) = true; }
#line 4152 "bison_parser.cpp"
    break;

  case 61: /* opt_not_exists: %empty  */
#line 728 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4158 "bison_parser.cpp"
    break;

  case 62: /* table_elem_commalist: table_elem  */
#line 730 "bison_parser.y"
                                  {
  (yyval.table_element_vec) = new std::vector<TableElement*>();
  (yyval.table_element_vec)->push_back((yyvsp[0].table_element_t));
}
#line 4167 "bison_parser.cpp"
    break;

  case 63: /* table_elem_commalist: table_elem_commalist ',' table_elem  */
#line 734 "bison_parser.y"
                                      {
  (yyvsp[-2].table_element_vec)->push_back((yyvsp[0].table_element_t));
  (yyval.table_element_vec) = (yyvsp[-2].table_element_vec);
}
#line 4176 "bison_parser.cpp"
    break;

  case 64: /* table_elem: column_def  */
#line 739 "bison_parser.y"
                        { (yyval.table_element_t) = (yyvsp[0].column_t); }
#line 4182 "bison_parser.cpp"
    break;

  case 65: /* table_elem: table_constraint  */
#line 740 "bison_parser.y"
                   { (yyval.table_element_t) = (yyvsp[0].table_constraint_t); }
#line 4188 "bison_parser.cpp"
    break;

  case 66: /* column_def: IDENTIFIER column_type opt_column_constraints  */
#line 742 "bison_parser.y"
                                                           {
  (yyval.column_t) = new ColumnDefinition((yyvsp[-2].sval), (yyvsp[-1].column_type_t), (yyvsp[0].column_constraints_t)->constraints, (yyvsp[0].column_constraints_t)->references);
  if (!(yyval.column_t)->trySetNullableExplicit()) {
    yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{(yyvsp[-2].sval)}).c_str());
  }
  delete (yyvsp[0].column_constraints_t);
}
#line 4200 "bison_parser.cpp"
    break;

  case 67: /* column_type: BIGINT  */
#line 750 "bison_parser.y"
                     { (yyval.column_type_t) = ColumnType{DataType::BIGINT}; }
#line 4206 "bison_parser.cpp"
    break;

  case 68: /* column_type: BOOLEAN  */
#line 751 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::BOOLEAN}; }
#line 4212 "bison_parser.cpp"
    break;

  case 69: /* column_type: CHAR '(' INTVAL ')'  */
#line 752 "bison_parser.y"
                      { (yyval.column_type_t) = ColumnType{DataType::CHAR, (yyvsp[-1].ival)}; }
#line 4218 "bison_parser.cpp"
    break;

  case 70: /* column_type: CHARACTER_VARYING '(' INTVAL ')'  */
#line 753 "bison_parser.y"
                                   { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4224 "bison_parser.cpp"
    break;

  case 71: /* column_type: DATE  */
#line 754 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::DATE}; }
#line 4230 "bison_parser.cpp"
    break;

  case 72: /* column_type: DATETIME  */
#line 755 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4236 "bison_parser.cpp"
    break;

  case 73: /* column_type: DECIMAL opt_decimal_specification  */
#line 756 "bison_parser.y"
                                    {
  (yyval.column_type_t) = ColumnType{DataType::DECIMAL, 0, (yyvsp[0].ival_pair)->first, (yyvsp[0].ival_pair)->second};
  delete (yyvsp[0].ival_pair);
}
#line 4245 "bison_parser.cpp"
    break;

  case 74: /* column_type: DOUBLE  */
#line 760 "bison_parser.y"
         { (yyval.column_type_t) = ColumnType{DataType::DOUBLE}; }
#line 4251 "bison_parser.cpp"
    break;

  case 75: /* column_type: FLOAT  */
#line 761 "bison_parser.y"
        { (yyval.column_type_t) = ColumnType{DataType::FLOAT}; }
#line 4257 "bison_parser.cpp"
    break;

  case 76: /* column_type: INT  */
#line 762 "bison_parser.y"
      { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4263 "bison_parser.cpp"
    break;

  case 77: /* column_type: INTEGER  */
#line 763 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4269 "bison_parser.cpp"
    break;

  case 78: /* column_type: LONG  */
#line 764 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::LONG}; }
#line 4275 "bison_parser.cpp"
    break;

  case 79: /* column_type: REAL  */
#line 765 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::REAL}; }
#line 4281 "bison_parser.cpp"
    break;

  case 80: /* column_type: SMALLINT  */
#line 766 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::SMALLINT}; }
#line 4287 "bison_parser.cpp"
    break;

  case 81: /* column_type: TEXT  */
#line 767 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::TEXT}; }
#line 4293 "bison_parser.cpp"
    break;

  case 82: /* column_type: TIME opt_time_precision  */
#line 768 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::TIME, 0, (yyvsp[0].ival)}; }
#line 4299 "bison_parser.cpp"
    break;

  case 83: /* column_type: TIMESTAMP  */
#line 769 "bison_parser.y"
            { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4305 "bison_parser.cpp"
    break;

  case 84: /* column_type: VARCHAR '(' INTVAL ')'  */
#line 770 "bison_parser.y"
                         { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4311 "bison_parser.cpp"
    break;

  case 85: /* opt_time_precision: '(' INTVAL ')'  */
#line 772 "bison_parser.y"
                                    { (yyval.ival) = (yyvsp[-1].ival); }
#line 4317 "bison_parser.cpp"
    break;

  case 86: /* opt_time_precision: %empty  */
#line 773 "bison_parser.y"
              { (yyval.ival) = 0; }
#line 4323 "bison_parser.cpp"
    break;

  case 87: /* opt_decimal_specification: '(' INTVAL ',' INTVAL ')'  */
#line 775 "bison_parser.y"
                                                      { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-3].ival), (yyvsp[-1].ival)}; }
#line 4329 "bison_parser.cpp"
    break;

  case 88: /* opt_decimal_specification: '(' INTVAL ')'  */
#line 776 "bison_parser.y"
                 { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-1].ival), 0}; }
#line 4335 "bison_parser.cpp"
    break;

  case 89: /* opt_decimal_specification: %empty  */
#line 777 "bison_parser.y"
              { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{0, 0}; }
#line 4341 "bison_parser.cpp"
    break;

  case 90: /* opt_column_constraints: column_constraints  */
#line 779 "bison_parser.y"
                                            { (yyval.column_constraints_t) = (yyvsp[0].column_constraints_t); }
#line 4347 "bison_parser.cpp"
    break;

  case 91: /* opt_column_constraints: %empty  */
#line 780 "bison_parser.y"
              { (yyval.column_constraints_t) = new ColumnConstraints(); }
#line 4353 "bison_parser.cpp"
    break;

  case 92: /* column_constraints: column_constraint  */
#line 782 "bison_parser.y"
                                       {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
}
#line 4362 "bison_parser.cpp"
    break;

  case 93: /* column_constraints: column_constraints column_constraint  */
#line 786 "bison_parser.y"
                                       {
  (yyvsp[-1].column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4371 "bison_parser.cpp"
    break;

  case 94: /* column_constraints: references_spec  */
#line 790 "bison_parser.y"
                  {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyval.column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
}
#line 4381 "bison_parser.cpp"
    break;

  case 95: /* column_constraints: column_constraints references_spec  */
#line 795 "bison_parser.y"
                                     {
  // Multiple foreign keys for the same column could be possible, so we do not raise an error in that case.
  // Think of foreign keys referenced on multiple levels (returned item references sold item references items).
  (yyvsp[-1].column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyvsp[-1].column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4393 "bison_parser.cpp"
    break;

  case 96: /* column_constraint: PRIMARY KEY  */
#line 803 "bison_parser.y"
                                { (yyval.column_constraint_t) = ConstraintType::PrimaryKey; }
#line 4399 "bison_parser.cpp"
    break;

  case 97: /* column_constraint: UNIQUE  */
#line 804 "bison_parser.y"
         { (yyval.column_constraint_t) = ConstraintType::Unique; }
#line 4405 "bison_parser.cpp"
    break;

  case 98: /* column_constraint: NULL  */
#line 805 "bison_parser.y"
       { (yyval.column_constraint_t) = ConstraintType::Null; }
#line 4411 "bison_parser.cpp"
    break;

  case 99: /* column_constraint: NOT NULL  */
#line 806 "bison_parser.y"
           { (yyval.column_constraint_t) = ConstraintType::NotNull; }
#line 4417 "bison_parser.cpp"
    break;

  case 100: /* table_constraint: PRIMARY KEY '(' ident_commalist ')'  */
#line 808 "bison_parser.y"
                                                       { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::PrimaryKey, (yyvsp[-1].str_vec)); }
#line 4423 "bison_parser.cpp"
    break;

  case 101: /* table_constraint: UNIQUE '(' ident_commalist ')'  */
#line 809 "bison_parser.y"
                                 { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::Unique, (yyvsp[-1].str_vec)); }
#line 4429 "bison_parser.cpp"
    break;

  case 102: /* table_constraint: FOREIGN KEY '(' ident_commalist ')' references_spec  */
#line 810 "bison_parser.y"
                                                      { (yyval.table_constraint_t) = new ForeignKeyConstraint((yyvsp[-2].str_vec), (yyvsp[0].references_spec_t)); }
#line 4435 "bison_parser.cpp"
    break;

  case 103: /* references_spec: REFERENCES table_name opt_column_list  */
#line 812 "bison_parser.y"
                                                        { (yyval.references_spec_t) = new ReferencesSpecification((yyvsp[-1].table_name).schema, (yyvsp[-1].table_name).name, (yyvsp[0].str_vec)); }
#line 4441 "bison_parser.cpp"
    break;

  case 104: /* drop_statement: DROP TABLE opt_exists table_name  */
#line 820 "bison_parser.y"
                                                  {
  (yyval.drop_stmt) = new DropStatement(kDropTable);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4452 "bison_parser.cpp"
    break;

  case 105: /* drop_statement: DROP VIEW opt_exists table_name  */
#line 826 "bison_parser.y"
                                  {
  (yyval.drop_stmt) = new DropStatement(kDropView);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4463 "bison_parser.cpp"
    break;

  case 106: /* drop_statement: DEALLOCATE PREPARE IDENTIFIER  */
#line 832 "bison_parser.y"
                                {
  (yyval.drop_stmt) = new DropStatement(kDropPreparedStatement);
  (yyval.drop_stmt)->ifExists = false;
  (yyval.drop_stmt)->name = (yyvsp[0].sval);
}
#line 4473 "bison_parser.cpp"
    break;

  case 107: /* drop_statement: DROP INDEX opt_exists IDENTIFIER  */
#line 838 "bison_parser.y"
                                   {
  (yyval.drop_stmt) = new DropStatement(kDropIndex);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->indexName = (yyvsp[0].sval);
}
#line 4483 "bison_parser.cpp"
    break;

  case 108: /* opt_exists: IF EXISTS  */
#line 844 "bison_parser.y"
                       { (yyval.bval) = true; }
#line 4489 "bison_parser.cpp"
    break;

  case 109: /* opt_exists: %empty  */
#line 845 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4495 "bison_parser.cpp"
    break;

  case 110: /* alter_statement: ALTER TABLE opt_exists table_name alter_action  */
#line 852 "bison_parser.y"
                                                                 {
  (yyval.alter_stmt) = new AlterStatement((yyvsp[-1].table_name).name, (yyvsp[0].alter_action_t));
  (yyval.alter_stmt)->ifTableExists = (yyvsp[-2].bval);
  (yyval.alter_stmt)->schema = (yyvsp[-1].table_name).schema;
}
#line 4505 "bison_parser.cpp"
    break;

  case 111: /* alter_action: drop_action  */
#line 858 "bison_parser.y"
                           { (yyval.alter_action_t) = (yyvsp[0].drop_action_t); }
#line 4511 "bison_parser.cpp"
    break;

  case 112: /* drop_action: DROP COLUMN opt_exists IDENTIFIER  */
#line 860 "bison_parser.y"
                                                {
  (yyval.drop_action_t) = new DropColumnAction((yyvsp[0].sval));
  (yyval.drop_action_t)->ifExists = (yyvsp[-1].bval);
}
#line 4520 "bison_parser.cpp"
    break;

  case 113: /* delete_statement: DELETE FROM table_name opt_where  */
#line 870 "bison_parser.y"
                                                    {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[-1].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[-1].table_name).name;
  (yyval.delete_stmt)->expr = (yyvsp[0].expr);
}
#line 4531 "bison_parser.cpp"
    break;

  case 114: /* truncate_statement: TRUNCATE table_name  */
#line 877 "bison_parser.y"
                                         {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 4541 "bison_parser.cpp"
    break;

  case 115: /* insert_statement: INSERT INTO table_name opt_column_list VALUES '(' extended_literal_list ')'  */
#line 888 "bison_parser.y"
                                                                                               {
  (yyval.insert_stmt) = new InsertStatement(kInsertValues);
  (yyval.insert_stmt)->schema = (yyvsp[-5].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-5].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-4].str_vec);
  (yyval.insert_stmt)->values = (yyvsp[-1].expr_vec);
}
#line 4553 "bison_parser.cpp"
    break;

  case 116: /* insert_statement: INSERT INTO table_name opt_column_list select_no_paren  */
#line 895 "bison_parser.y"
                                                         {
  (yyval.insert_stmt) = new InsertStatement(kInsertSelect);
  (yyval.insert_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-1].str_vec);
  (yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4565 "bison_parser.cpp"
    break;

  case 117: /* opt_column_list: '(' ident_commalist ')'  */
#line 903 "bison_parser.y"
                                          { (yyval.str_vec) = (yyvsp[-1].str_vec); }
#line 4571 "bison_parser.cpp"
    break;

  case 118: /* opt_column_list: %empty  */
#line 904 "bison_parser.y"
              { (yyval.str_vec) = nullptr; }
#line 4577 "bison_parser.cpp"
    break;

  case 119: /* update_statement: UPDATE table_ref_name_no_alias SET update_clause_commalist opt_where  */
#line 911 "bison_parser.y"
                                                                                        {
  (yyval.update_stmt) = new UpdateStatement();
  (yyval.update_stmt)->table = (yyvsp[-3].table);
  (yyval.update_stmt)->updates = (yyvsp[-1].update_vec);
  (yyval.update_stmt)->where = (yyvsp[0].expr);
}
#line 4588 "bison_parser.cpp"
    break;

  case 120: /* update_clause_commalist: update_clause  */
#line 918 "bison_parser.y"
                                        {
  (yyval.update_vec) = new std::vector<UpdateClause*>();
  (yyval.update_vec)->push_back((yyvsp[0].update_t));
}
#line 4597 "bison_parser.cpp"
    break;

  case 121: /* update_clause_commalist: update_clause_commalist ',' update_clause  */
#line 922 "bison_parser.y"
                                            {
  (yyvsp[-2].update_vec)->push_back((yyvsp[0].update_t));
  (yyval.update_vec) = (yyvsp[-2].update_vec);
}
#line 4606 "bison_parser.cpp"
    break;

  case 122: /* update_clause: IDENTIFIER '=' expr  */
#line 927 "bison_parser.y"
                                    {
  (yyval.update_t) = new UpdateClause();
  (yyval.update_t)->column = (yyvsp[-2].sval);
  (yyval.update_t)->value = (yyvsp[0].expr);
}
#line 4616 "bison_parser.cpp"
    break;

  case 123: /* select_statement: opt_with_clause select_with_paren  */
#line 937 "bison_parser.y"
                                                     {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4625 "bison_parser.cpp"
    break;

  case 124: /* select_statement: opt_with_clause select_no_paren  */
#line 941 "bison_parser.y"
                                  {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4634 "bison_parser.cpp"
    break;

  case 125: /* select_statement: opt_with_clause select_with_paren set_operator select_within_set_operation opt_order opt_limit  */
#line 945 "bison_parser.y"
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
#line 4650 "bison_parser.cpp"
    break;

  case 128: /* select_within_set_operation_no_parentheses: select_clause  */
#line 959 "bison_parser.y"
                                                           { (yyval.select_stmt) = (yyvsp[0].select_stmt); }
#line 4656 "bison_parser.cpp"
    break;

  case 129: /* select_within_set_operation_no_parentheses: select_clause set_operator select_within_set_operation  */
#line 960 "bison_parser.y"
                                                         {
  (yyval.select_stmt) = (yyvsp[-2].select_stmt);
  if ((yyval.select_stmt)->setOperations == nullptr) {
    (yyval.select_stmt)->setOperations = new std::vector<SetOperation*>();
  }
  (yyval.select_stmt)->setOperations->push_back((yyvsp[-1].set_operator_t));
  (yyval.select_stmt)->setOperations->back()->nestedSelectStatement = (yyvsp[0].select_stmt);
}
#line 4669 "bison_parser.cpp"
    break;

  case 130: /* select_with_paren: '(' select_no_paren ')'  */
#line 969 "bison_parser.y"
                                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4675 "bison_parser.cpp"
    break;

  case 131: /* select_with_paren: '(' select_with_paren ')'  */
#line 970 "bison_parser.y"
                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4681 "bison_parser.cpp"
    break;

  case 132: /* select_no_paren: select_clause opt_order opt_limit opt_locking_clause  */
#line 972 "bison_parser.y"
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
#line 4700 "bison_parser.cpp"
    break;

  case 133: /* select_no_paren: select_clause set_operator select_within_set_operation opt_order opt_limit opt_locking_clause  */
#line 986 "bison_parser.y"
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
#line 4716 "bison_parser.cpp"
    break;

  case 134: /* set_operator: set_type opt_all  */
#line 998 "bison_parser.y"
                                {
  (yyval.set_operator_t) = (yyvsp[-1].set_operator_t);
  (yyval.set_operator_t)->isAll = (yyvsp[0].bval);
}
#line 4725 "bison_parser.cpp"
    break;

  case 135: /* set_type: UNION  */
#line 1003 "bison_parser.y"
                 {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetUnion;
}
#line 4734 "bison_parser.cpp"
    break;

  case 136: /* set_type: INTERSECT  */
#line 1007 "bison_parser.y"
            {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetIntersect;
}
#line 4743 "bison_parser.cpp"
    break;

  case 137: /* set_type: EXCEPT  */
#line 1011 "bison_parser.y"
         {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetExcept;
}
#line 4752 "bison_parser.cpp"
    break;

  case 138: /* opt_all: ALL  */
#line 1016 "bison_parser.y"
              { (yyval.bval) = true; }
#line 4758 "bison_parser.cpp"
    break;

  case 139: /* opt_all: %empty  */
#line 1017 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4764 "bison_parser.cpp"
    break;

  case 140: /* select_clause: SELECT opt_top opt_distinct select_list opt_from_clause opt_where opt_group  */
#line 1019 "bison_parser.y"
                                                                                            {
  (yyval.select_stmt) = new SelectStatement();
  (yyval.select_stmt)->limit = (yyvsp[-5].limit);
  (yyval.select_stmt)->selectDistinct = (yyvsp[-4].bval);
  (yyval.select_stmt)->selectList = (yyvsp[-3].expr_vec);
  (yyval.select_stmt)->fromTable = (yyvsp[-2].table);
  (yyval.select_stmt)->whereClause = (yyvsp[-1].expr);
  (yyval.select_stmt)->groupBy = (yyvsp[0].group_t);
}
#line 4778 "bison_parser.cpp"
    break;

  case 141: /* opt_distinct: DISTINCT  */
#line 1029 "bison_parser.y"
                        { (yyval.bval) = true; }
#line 4784 "bison_parser.cpp"
    break;

  case 142: /* opt_distinct: %empty  */
#line 1030 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4790 "bison_parser.cpp"
    break;

  case 144: /* opt_from_clause: from_clause  */
#line 1034 "bison_parser.y"
                              { (yyval.table) = (yyvsp[0].table); }
#line 4796 "bison_parser.cpp"
    break;

  case 145: /* opt_from_clause: %empty  */
#line 1035 "bison_parser.y"
              { (yyval.table) = nullptr; }
#line 4802 "bison_parser.cpp"
    break;

  case 146: /* from_clause: FROM table_ref  */
#line 1037 "bison_parser.y"
                             { (yyval.table) = (yyvsp[0].table); }
#line 4808 "bison_parser.cpp"
    break;

  case 147: /* opt_where: WHERE expr  */
#line 1039 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 4814 "bison_parser.cpp"
    break;

  case 148: /* opt_where: %empty  */
#line 1040 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4820 "bison_parser.cpp"
    break;

  case 149: /* opt_group: GROUP BY expr_list opt_having  */
#line 1042 "bison_parser.y"
                                          {
  (yyval.group_t) = new GroupByDescription();
  (yyval.group_t)->columns = (yyvsp[-1].expr_vec);
  (yyval.group_t)->having = (yyvsp[0].expr);
}
#line 4830 "bison_parser.cpp"
    break;

  case 150: /* opt_group: %empty  */
#line 1047 "bison_parser.y"
              { (yyval.group_t) = nullptr; }
#line 4836 "bison_parser.cpp"
    break;

  case 151: /* opt_having: HAVING expr  */
#line 1049 "bison_parser.y"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 4842 "bison_parser.cpp"
    break;

  case 152: /* opt_having: %empty  */
#line 1050 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4848 "bison_parser.cpp"
    break;

  case 153: /* opt_order: ORDER BY order_list  */
#line 1052 "bison_parser.y"
                                { (yyval.order_vec) = (yyvsp[0].order_vec); }
#line 4854 "bison_parser.cpp"
    break;

  case 154: /* opt_order: %empty  */
#line 1053 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 4860 "bison_parser.cpp"
    break;

  case 155: /* order_list: order_desc  */
#line 1055 "bison_parser.y"
                        {
  (yyval.order_vec) = new std::vector<OrderDescription*>();
  (yyval.order_vec)->push_back((yyvsp[0].order));
}
#line 4869 "bison_parser.cpp"
    break;

  case 156: /* order_list: order_list ',' order_desc  */
#line 1059 "bison_parser.y"
                            {
  (yyvsp[-2].order_vec)->push_back((yyvsp[0].order));
  (yyval.order_vec) = (yyvsp[-2].order_vec);
}
#line 4878 "bison_parser.cpp"
    break;

  case 157: /* order_desc: expr opt_order_type opt_null_ordering  */
#line 1064 "bison_parser.y"
                                                   { (yyval.order) = new OrderDescription((yyvsp[-1].order_type), (yyvsp[-2].expr), (yyvsp[0].null_ordering_t)); }
#line 4884 "bison_parser.cpp"
    break;

  case 158: /* opt_order_type: ASC  */
#line 1066 "bison_parser.y"
                     { (yyval.order_type) = kOrderAsc; }
#line 4890 "bison_parser.cpp"
    break;

  case 159: /* opt_order_type: DESC  */
#line 1067 "bison_parser.y"
       { (yyval.order_type) = kOrderDesc; }
#line 4896 "bison_parser.cpp"
    break;

  case 160: /* opt_order_type: %empty  */
#line 1068 "bison_parser.y"
              { (yyval.order_type) = kOrderAsc; }
#line 4902 "bison_parser.cpp"
    break;

  case 161: /* opt_null_ordering: %empty  */
#line 1070 "bison_parser.y"
                                { (yyval.null_ordering_t) = NullOrdering::Undefined; }
#line 4908 "bison_parser.cpp"
    break;

  case 162: /* opt_null_ordering: IDENTIFIER IDENTIFIER  */
#line 1071 "bison_parser.y"
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
#line 4932 "bison_parser.cpp"
    break;

  case 163: /* opt_top: TOP int_literal  */
#line 1093 "bison_parser.y"
                          { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 4938 "bison_parser.cpp"
    break;

  case 164: /* opt_top: TOP '(' int_literal ')'  */
#line 1094 "bison_parser.y"
                          { (yyval.limit) = new LimitDescription((yyvsp[-1].expr), nullptr); }
#line 4944 "bison_parser.cpp"
    break;

  case 165: /* opt_top: %empty  */
#line 1095 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 4950 "bison_parser.cpp"
    break;

  case 166: /* opt_limit: LIMIT expr  */
#line 1097 "bison_parser.y"
                       { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 4956 "bison_parser.cpp"
    break;

  case 167: /* opt_limit: OFFSET expr  */
#line 1098 "bison_parser.y"
              { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 4962 "bison_parser.cpp"
    break;

  case 168: /* opt_limit: LIMIT expr OFFSET expr  */
#line 1099 "bison_parser.y"
                         { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 4968 "bison_parser.cpp"
    break;

  case 169: /* opt_limit: LIMIT ALL  */
#line 1100 "bison_parser.y"
            { (yyval.limit) = new LimitDescription(nullptr, nullptr); }
#line 4974 "bison_parser.cpp"
    break;

  case 170: /* opt_limit: LIMIT ALL OFFSET expr  */
#line 1101 "bison_parser.y"
                        { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 4980 "bison_parser.cpp"
    break;

  case 171: /* opt_limit: OFFSET expr ROWS FETCH NEXT expr ROWS ONLY  */
#line 1102 "bison_parser.y"
                                             { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[-6].expr)); }
#line 4986 "bison_parser.cpp"
    break;

  case 172: /* opt_limit: %empty  */
#line 1103 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 4992 "bison_parser.cpp"
    break;

  case 173: /* expr_list: expr_alias  */
#line 1108 "bison_parser.y"
                       {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5001 "bison_parser.cpp"
    break;

  case 174: /* expr_list: expr_list ',' expr_alias  */
#line 1112 "bison_parser.y"
                           {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5010 "bison_parser.cpp"
    break;

  case 175: /* opt_extended_literal_list: extended_literal_list  */
#line 1118 "bison_parser.y"
                                                  { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5016 "bison_parser.cpp"
    break;

  case 176: /* opt_extended_literal_list: %empty  */
#line 1119 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5022 "bison_parser.cpp"
    break;

  case 177: /* extended_literal_list: casted_extended_literal  */
#line 1121 "bison_parser.y"
                                                {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5031 "bison_parser.cpp"
    break;

  case 178: /* extended_literal_list: extended_literal_list ',' casted_extended_literal  */
#line 1125 "bison_parser.y"
                                                    {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5040 "bison_parser.cpp"
    break;

  case 180: /* casted_extended_literal: CAST '(' extended_literal AS column_type ')'  */
#line 1130 "bison_parser.y"
                                                                                          {
  (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t));
}
#line 5048 "bison_parser.cpp"
    break;

  case 181: /* extended_literal: literal  */
#line 1134 "bison_parser.y"
                           { (yyval.expr) = (yyvsp[0].expr); }
#line 5054 "bison_parser.cpp"
    break;

  case 182: /* extended_literal: '-' num_literal  */
#line 1135 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5060 "bison_parser.cpp"
    break;

  case 183: /* extended_literal: '-' interval_literal  */
#line 1136 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5066 "bison_parser.cpp"
    break;

  case 184: /* expr_alias: expr opt_alias  */
#line 1138 "bison_parser.y"
                            {
  (yyval.expr) = (yyvsp[-1].expr);
  if ((yyvsp[0].alias_t)) {
    (yyval.expr)->alias = (yyvsp[0].alias_t)->name;
    (yyvsp[0].alias_t)->name = nullptr;
    delete (yyvsp[0].alias_t);
  }
}
#line 5079 "bison_parser.cpp"
    break;

  case 190: /* operand: '(' expr ')'  */
#line 1149 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[-1].expr); }
#line 5085 "bison_parser.cpp"
    break;

  case 200: /* operand: '(' select_no_paren ')'  */
#line 1151 "bison_parser.y"
                                         {
  (yyval.expr) = Expr::makeSelect((yyvsp[-1].select_stmt));
}
#line 5093 "bison_parser.cpp"
    break;

  case 203: /* unary_expr: '-' operand  */
#line 1157 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5099 "bison_parser.cpp"
    break;

  case 204: /* unary_expr: NOT operand  */
#line 1158 "bison_parser.y"
              { (yyval.expr) = Expr::makeOpUnary(kOpNot, (yyvsp[0].expr)); }
#line 5105 "bison_parser.cpp"
    break;

  case 205: /* unary_expr: operand ISNULL  */
#line 1159 "bison_parser.y"
                 { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-1].expr)); }
#line 5111 "bison_parser.cpp"
    break;

  case 206: /* unary_expr: operand IS NULL  */
#line 1160 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-2].expr)); }
#line 5117 "bison_parser.cpp"
    break;

  case 207: /* unary_expr: operand IS NOT NULL  */
#line 1161 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeOpUnary(kOpIsNull, (yyvsp[-3].expr))); }
#line 5123 "bison_parser.cpp"
    break;

  case 209: /* binary_expr: operand '-' operand  */
#line 1163 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpMinus, (yyvsp[0].expr)); }
#line 5129 "bison_parser.cpp"
    break;

  case 210: /* binary_expr: operand '+' operand  */
#line 1164 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPlus, (yyvsp[0].expr)); }
#line 5135 "bison_parser.cpp"
    break;

  case 211: /* binary_expr: operand '/' operand  */
#line 1165 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpSlash, (yyvsp[0].expr)); }
#line 5141 "bison_parser.cpp"
    break;

  case 212: /* binary_expr: operand '*' operand  */
#line 1166 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAsterisk, (yyvsp[0].expr)); }
#line 5147 "bison_parser.cpp"
    break;

  case 213: /* binary_expr: operand '%' operand  */
#line 1167 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPercentage, (yyvsp[0].expr)); }
#line 5153 "bison_parser.cpp"
    break;

  case 214: /* binary_expr: operand '^' operand  */
#line 1168 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpCaret, (yyvsp[0].expr)); }
#line 5159 "bison_parser.cpp"
    break;

  case 215: /* binary_expr: operand LIKE operand  */
#line 1169 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLike, (yyvsp[0].expr)); }
#line 5165 "bison_parser.cpp"
    break;

  case 216: /* binary_expr: operand NOT LIKE operand  */
#line 1170 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-3].expr), kOpNotLike, (yyvsp[0].expr)); }
#line 5171 "bison_parser.cpp"
    break;

  case 217: /* binary_expr: operand ILIKE operand  */
#line 1171 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpILike, (yyvsp[0].expr)); }
#line 5177 "bison_parser.cpp"
    break;

  case 218: /* binary_expr: operand CONCAT operand  */
#line 1172 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpConcat, (yyvsp[0].expr)); }
#line 5183 "bison_parser.cpp"
    break;

  case 219: /* logic_expr: expr AND expr  */
#line 1174 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAnd, (yyvsp[0].expr)); }
#line 5189 "bison_parser.cpp"
    break;

  case 220: /* logic_expr: expr OR expr  */
#line 1175 "bison_parser.y"
               { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpOr, (yyvsp[0].expr)); }
#line 5195 "bison_parser.cpp"
    break;

  case 221: /* in_expr: operand IN '(' expr_list ')'  */
#line 1177 "bison_parser.y"
                                       { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].expr_vec)); }
#line 5201 "bison_parser.cpp"
    break;

  case 222: /* in_expr: operand NOT IN '(' expr_list ')'  */
#line 1178 "bison_parser.y"
                                   { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].expr_vec))); }
#line 5207 "bison_parser.cpp"
    break;

  case 223: /* in_expr: operand IN '(' select_no_paren ')'  */
#line 1179 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].select_stmt)); }
#line 5213 "bison_parser.cpp"
    break;

  case 224: /* in_expr: operand NOT IN '(' select_no_paren ')'  */
#line 1180 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].select_stmt))); }
#line 5219 "bison_parser.cpp"
    break;

  case 225: /* in_expr: '(' row_expr_list ')' IN '(' select_no_paren ')'  */
#line 1184 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeInOperator(Expr::makeArray((yyvsp[-5].expr_vec)), (yyvsp[-1].select_stmt)); }
#line 5225 "bison_parser.cpp"
    break;

  case 226: /* in_expr: '(' row_expr_list ')' NOT IN '(' select_no_paren ')'  */
#line 1185 "bison_parser.y"
                                                       { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator(Expr::makeArray((yyvsp[-6].expr_vec)), (yyvsp[-1].select_stmt))); }
#line 5231 "bison_parser.cpp"
    break;

  case 227: /* row_expr_list: expr_alias ',' expr_alias  */
#line 1189 "bison_parser.y"
                                          {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[-2].expr));
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5241 "bison_parser.cpp"
    break;

  case 228: /* row_expr_list: row_expr_list ',' expr_alias  */
#line 1194 "bison_parser.y"
                               {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5250 "bison_parser.cpp"
    break;

  case 229: /* case_expr: CASE expr case_list END  */
#line 1201 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-2].expr), (yyvsp[-1].expr), nullptr); }
#line 5256 "bison_parser.cpp"
    break;

  case 230: /* case_expr: CASE expr case_list ELSE expr END  */
#line 1202 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-4].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5262 "bison_parser.cpp"
    break;

  case 231: /* case_expr: CASE case_list END  */
#line 1203 "bison_parser.y"
                     { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-1].expr), nullptr); }
#line 5268 "bison_parser.cpp"
    break;

  case 232: /* case_expr: CASE case_list ELSE expr END  */
#line 1204 "bison_parser.y"
                               { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5274 "bison_parser.cpp"
    break;

  case 233: /* case_list: WHEN expr THEN expr  */
#line 1206 "bison_parser.y"
                                { (yyval.expr) = Expr::makeCaseList(Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5280 "bison_parser.cpp"
    break;

  case 234: /* case_list: case_list WHEN expr THEN expr  */
#line 1207 "bison_parser.y"
                                { (yyval.expr) = Expr::caseListAppend((yyvsp[-4].expr), Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5286 "bison_parser.cpp"
    break;

  case 235: /* exists_expr: EXISTS '(' select_no_paren ')'  */
#line 1209 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeExists((yyvsp[-1].select_stmt)); }
#line 5292 "bison_parser.cpp"
    break;

  case 236: /* exists_expr: NOT EXISTS '(' select_no_paren ')'  */
#line 1210 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeExists((yyvsp[-1].select_stmt))); }
#line 5298 "bison_parser.cpp"
    break;

  case 237: /* comp_expr: operand '=' operand  */
#line 1212 "bison_parser.y"
                                { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5304 "bison_parser.cpp"
    break;

  case 238: /* comp_expr: operand EQUALS operand  */
#line 1213 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5310 "bison_parser.cpp"
    break;

  case 239: /* comp_expr: operand NOTEQUALS operand  */
#line 1214 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpNotEquals, (yyvsp[0].expr)); }
#line 5316 "bison_parser.cpp"
    break;

  case 240: /* comp_expr: operand '<' operand  */
#line 1215 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLess, (yyvsp[0].expr)); }
#line 5322 "bison_parser.cpp"
    break;

  case 241: /* comp_expr: operand '>' operand  */
#line 1216 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreater, (yyvsp[0].expr)); }
#line 5328 "bison_parser.cpp"
    break;

  case 242: /* comp_expr: operand LESSEQ operand  */
#line 1217 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLessEq, (yyvsp[0].expr)); }
#line 5334 "bison_parser.cpp"
    break;

  case 243: /* comp_expr: operand GREATEREQ operand  */
#line 1218 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreaterEq, (yyvsp[0].expr)); }
#line 5340 "bison_parser.cpp"
    break;

  case 244: /* function_expr: IDENTIFIER '(' ')' opt_window  */
#line 1222 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 5346 "bison_parser.cpp"
    break;

  case 245: /* function_expr: IDENTIFIER '(' opt_distinct expr_list ')' opt_window  */
#line 1223 "bison_parser.y"
                                                       { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description)); }
#line 5352 "bison_parser.cpp"
    break;

  case 246: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' ')' opt_window  */
#line 1224 "bison_parser.y"
                                               {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), (yyvsp[-5].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description));
}
#line 5360 "bison_parser.cpp"
    break;

  case 247: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' opt_distinct expr_list ')' opt_window  */
#line 1227 "bison_parser.y"
                                                                      {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-7].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description));
}
#line 5368 "bison_parser.cpp"
    break;

  case 248: /* function_expr: nonreserved_keyword '(' ')' opt_window  */
#line 1232 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 5374 "bison_parser.cpp"
    break;

  case 249: /* function_expr: nonreserved_keyword '(' opt_distinct expr_list ')' opt_window  */
#line 1233 "bison_parser.y"
                                                                { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description)); }
#line 5380 "bison_parser.cpp"
    break;

  case 250: /* opt_window: OVER '(' opt_partition opt_order opt_frame_clause ')'  */
#line 1237 "bison_parser.y"
                                                                   { (yyval.window_description) = new WindowDescription((yyvsp[-3].expr_vec), (yyvsp[-2].order_vec), (yyvsp[-1].frame_description)); }
#line 5386 "bison_parser.cpp"
    break;

  case 251: /* opt_window: %empty  */
#line 1238 "bison_parser.y"
              { (yyval.window_description) = nullptr; }
#line 5392 "bison_parser.cpp"
    break;

  case 252: /* opt_partition: PARTITION BY expr_list  */
#line 1240 "bison_parser.y"
                                       { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5398 "bison_parser.cpp"
    break;

  case 253: /* opt_partition: %empty  */
#line 1241 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5404 "bison_parser.cpp"
    break;

  case 254: /* opt_frame_clause: frame_type frame_bound  */
#line 1246 "bison_parser.y"
                                          { (yyval.frame_description) = new FrameDescription{(yyvsp[-1].frame_type), (yyvsp[0].frame_bound), new FrameBound{0, kCurrentRow, false}}; }
#line 5410 "bison_parser.cpp"
    break;

  case 255: /* opt_frame_clause: frame_type BETWEEN frame_bound AND frame_bound  */
#line 1247 "bison_parser.y"
                                                 { (yyval.frame_description) = new FrameDescription{(yyvsp[-4].frame_type), (yyvsp[-2].frame_bound), (yyvsp[0].frame_bound)}; }
#line 5416 "bison_parser.cpp"
    break;

  case 256: /* opt_frame_clause: %empty  */
#line 1248 "bison_parser.y"
              {
  (yyval.frame_description) = new FrameDescription{kRange, new FrameBound{0, kPreceding, true}, new FrameBound{0, kCurrentRow, false}};
}
#line 5424 "bison_parser.cpp"
    break;

  case 257: /* frame_type: RANGE  */
#line 1252 "bison_parser.y"
                   { (yyval.frame_type) = kRange; }
#line 5430 "bison_parser.cpp"
    break;

  case 258: /* frame_type: ROWS  */
#line 1253 "bison_parser.y"
       { (yyval.frame_type) = kRows; }
#line 5436 "bison_parser.cpp"
    break;

  case 259: /* frame_type: GROUPS  */
#line 1254 "bison_parser.y"
         { (yyval.frame_type) = kGroups; }
#line 5442 "bison_parser.cpp"
    break;

  case 260: /* frame_bound: UNBOUNDED PRECEDING  */
#line 1256 "bison_parser.y"
                                  { (yyval.frame_bound) = new FrameBound{0, kPreceding, true}; }
#line 5448 "bison_parser.cpp"
    break;

  case 261: /* frame_bound: INTVAL PRECEDING  */
#line 1257 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kPreceding, false}; }
#line 5454 "bison_parser.cpp"
    break;

  case 262: /* frame_bound: UNBOUNDED FOLLOWING  */
#line 1258 "bison_parser.y"
                      { (yyval.frame_bound) = new FrameBound{0, kFollowing, true}; }
#line 5460 "bison_parser.cpp"
    break;

  case 263: /* frame_bound: INTVAL FOLLOWING  */
#line 1259 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kFollowing, false}; }
#line 5466 "bison_parser.cpp"
    break;

  case 264: /* frame_bound: CURRENT_ROW  */
#line 1260 "bison_parser.y"
              { (yyval.frame_bound) = new FrameBound{0, kCurrentRow, false}; }
#line 5472 "bison_parser.cpp"
    break;

  case 265: /* extract_expr: EXTRACT '(' datetime_field FROM expr ')'  */
#line 1262 "bison_parser.y"
                                                        { (yyval.expr) = Expr::makeExtract((yyvsp[-3].datetime_field), (yyvsp[-1].expr)); }
#line 5478 "bison_parser.cpp"
    break;

  case 266: /* cast_expr: CAST '(' expr AS column_type ')'  */
#line 1264 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t)); }
#line 5484 "bison_parser.cpp"
    break;

  case 267: /* datetime_field: SECOND  */
#line 1266 "bison_parser.y"
                        { (yyval.datetime_field) = kDatetimeSecond; }
#line 5490 "bison_parser.cpp"
    break;

  case 268: /* datetime_field: MINUTE  */
#line 1267 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMinute; }
#line 5496 "bison_parser.cpp"
    break;

  case 269: /* datetime_field: HOUR  */
#line 1268 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeHour; }
#line 5502 "bison_parser.cpp"
    break;

  case 270: /* datetime_field: DAY  */
#line 1269 "bison_parser.y"
      { (yyval.datetime_field) = kDatetimeDay; }
#line 5508 "bison_parser.cpp"
    break;

  case 271: /* datetime_field: MONTH  */
#line 1270 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeMonth; }
#line 5514 "bison_parser.cpp"
    break;

  case 272: /* datetime_field: YEAR  */
#line 1271 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeYear; }
#line 5520 "bison_parser.cpp"
    break;

  case 273: /* datetime_field_plural: SECONDS  */
#line 1273 "bison_parser.y"
                                { (yyval.datetime_field) = kDatetimeSecond; }
#line 5526 "bison_parser.cpp"
    break;

  case 274: /* datetime_field_plural: MINUTES  */
#line 1274 "bison_parser.y"
          { (yyval.datetime_field) = kDatetimeMinute; }
#line 5532 "bison_parser.cpp"
    break;

  case 275: /* datetime_field_plural: HOURS  */
#line 1275 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeHour; }
#line 5538 "bison_parser.cpp"
    break;

  case 276: /* datetime_field_plural: DAYS  */
#line 1276 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeDay; }
#line 5544 "bison_parser.cpp"
    break;

  case 277: /* datetime_field_plural: MONTHS  */
#line 1277 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMonth; }
#line 5550 "bison_parser.cpp"
    break;

  case 278: /* datetime_field_plural: YEARS  */
#line 1278 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeYear; }
#line 5556 "bison_parser.cpp"
    break;

  case 281: /* array_expr: ARRAY '[' expr_list ']'  */
#line 1282 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeArray((yyvsp[-1].expr_vec)); }
#line 5562 "bison_parser.cpp"
    break;

  case 282: /* array_index: operand '[' int_literal ']'  */
#line 1288 "bison_parser.y"
                                          {
  (yyval.expr) = Expr::makeArrayIndex((yyvsp[-3].expr), (yyvsp[-1].expr)->ival);
  delete (yyvsp[-1].expr);
}
#line 5571 "bison_parser.cpp"
    break;

  case 283: /* between_expr: operand BETWEEN operand AND operand  */
#line 1293 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeBetween((yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 5577 "bison_parser.cpp"
    break;

  case 284: /* column_name: IDENTIFIER  */
#line 1295 "bison_parser.y"
                         { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 5583 "bison_parser.cpp"
    break;

  case 285: /* column_name: IDENTIFIER '.' IDENTIFIER  */
#line 1296 "bison_parser.y"
                            { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 5589 "bison_parser.cpp"
    break;

  case 286: /* column_name: '*'  */
#line 1297 "bison_parser.y"
      { (yyval.expr) = Expr::makeStar(); }
#line 5595 "bison_parser.cpp"
    break;

  case 287: /* column_name: IDENTIFIER '.' '*'  */
#line 1298 "bison_parser.y"
                     { (yyval.expr) = Expr::makeStar((yyvsp[-2].sval)); }
#line 5601 "bison_parser.cpp"
    break;

  case 288: /* column_name: nonreserved_keyword  */
#line 1299 "bison_parser.y"
                      { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 5607 "bison_parser.cpp"
    break;

  case 289: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1305 "bison_parser.y"
                                           {
  std::string combined(std::string((yyvsp[-4].sval)) + "." + (yyvsp[-2].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeColumnRef(strdup(combined.c_str()), (yyvsp[0].sval));
}
#line 5618 "bison_parser.cpp"
    break;

  case 290: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1311 "bison_parser.y"
                                                          {
  std::string combined(std::string((yyvsp[-6].sval)) + "." + (yyvsp[-4].sval) + "." + (yyvsp[-2].sval));
  free((yyvsp[-6].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeColumnRef(strdup(combined.c_str()), (yyvsp[0].sval));
}
#line 5630 "bison_parser.cpp"
    break;

  case 291: /* column_name: IDENTIFIER '.' IDENTIFIER '.' '*'  */
#line 1318 "bison_parser.y"
                                    {
  std::string combined(std::string((yyvsp[-4].sval)) + "." + (yyvsp[-2].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeStar(strdup(combined.c_str()));
}
#line 5641 "bison_parser.cpp"
    break;

  case 292: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER '.' '*'  */
#line 1324 "bison_parser.y"
                                                   {
  std::string combined(std::string((yyvsp[-6].sval)) + "." + (yyvsp[-4].sval) + "." + (yyvsp[-2].sval));
  free((yyvsp[-6].sval));
  free((yyvsp[-4].sval));
  free((yyvsp[-2].sval));
  (yyval.expr) = Expr::makeStar(strdup(combined.c_str()));
}
#line 5653 "bison_parser.cpp"
    break;

  case 293: /* nonreserved_keyword: SECOND  */
#line 1338 "bison_parser.y"
                             { (yyval.sval) = strdup("SECOND"); }
#line 5659 "bison_parser.cpp"
    break;

  case 294: /* nonreserved_keyword: MINUTE  */
#line 1339 "bison_parser.y"
         { (yyval.sval) = strdup("MINUTE"); }
#line 5665 "bison_parser.cpp"
    break;

  case 295: /* nonreserved_keyword: HOUR  */
#line 1340 "bison_parser.y"
       { (yyval.sval) = strdup("HOUR"); }
#line 5671 "bison_parser.cpp"
    break;

  case 296: /* nonreserved_keyword: DAY  */
#line 1341 "bison_parser.y"
      { (yyval.sval) = strdup("DAY"); }
#line 5677 "bison_parser.cpp"
    break;

  case 297: /* nonreserved_keyword: MONTH  */
#line 1342 "bison_parser.y"
        { (yyval.sval) = strdup("MONTH"); }
#line 5683 "bison_parser.cpp"
    break;

  case 298: /* nonreserved_keyword: YEAR  */
#line 1343 "bison_parser.y"
       { (yyval.sval) = strdup("YEAR"); }
#line 5689 "bison_parser.cpp"
    break;

  case 299: /* nonreserved_keyword: SECONDS  */
#line 1344 "bison_parser.y"
          { (yyval.sval) = strdup("SECONDS"); }
#line 5695 "bison_parser.cpp"
    break;

  case 300: /* nonreserved_keyword: MINUTES  */
#line 1345 "bison_parser.y"
          { (yyval.sval) = strdup("MINUTES"); }
#line 5701 "bison_parser.cpp"
    break;

  case 301: /* nonreserved_keyword: HOURS  */
#line 1346 "bison_parser.y"
        { (yyval.sval) = strdup("HOURS"); }
#line 5707 "bison_parser.cpp"
    break;

  case 302: /* nonreserved_keyword: DAYS  */
#line 1347 "bison_parser.y"
       { (yyval.sval) = strdup("DAYS"); }
#line 5713 "bison_parser.cpp"
    break;

  case 303: /* nonreserved_keyword: MONTHS  */
#line 1348 "bison_parser.y"
         { (yyval.sval) = strdup("MONTHS"); }
#line 5719 "bison_parser.cpp"
    break;

  case 304: /* nonreserved_keyword: YEARS  */
#line 1349 "bison_parser.y"
        { (yyval.sval) = strdup("YEARS"); }
#line 5725 "bison_parser.cpp"
    break;

  case 305: /* nonreserved_keyword: ISNULL  */
#line 1350 "bison_parser.y"
         { (yyval.sval) = strdup("ISNULL"); }
#line 5731 "bison_parser.cpp"
    break;

  case 306: /* nonreserved_keyword: FORMAT  */
#line 1351 "bison_parser.y"
         { (yyval.sval) = strdup("FORMAT"); }
#line 5737 "bison_parser.cpp"
    break;

  case 307: /* nonreserved_keyword: CHAR  */
#line 1352 "bison_parser.y"
       { (yyval.sval) = strdup("CHAR"); }
#line 5743 "bison_parser.cpp"
    break;

  case 308: /* nonreserved_keyword: VARCHAR  */
#line 1353 "bison_parser.y"
          { (yyval.sval) = strdup("VARCHAR"); }
#line 5749 "bison_parser.cpp"
    break;

  case 309: /* nonreserved_keyword: INT  */
#line 1354 "bison_parser.y"
      { (yyval.sval) = strdup("INT"); }
#line 5755 "bison_parser.cpp"
    break;

  case 310: /* nonreserved_keyword: INTEGER  */
#line 1355 "bison_parser.y"
          { (yyval.sval) = strdup("INTEGER"); }
#line 5761 "bison_parser.cpp"
    break;

  case 311: /* nonreserved_keyword: DATETIME  */
#line 1356 "bison_parser.y"
           { (yyval.sval) = strdup("DATETIME"); }
#line 5767 "bison_parser.cpp"
    break;

  case 312: /* nonreserved_keyword: TIMESTAMP  */
#line 1357 "bison_parser.y"
            { (yyval.sval) = strdup("TIMESTAMP"); }
#line 5773 "bison_parser.cpp"
    break;

  case 320: /* string_literal: STRING  */
#line 1361 "bison_parser.y"
                        { (yyval.expr) = Expr::makeLiteral((yyvsp[0].sval)); }
#line 5779 "bison_parser.cpp"
    break;

  case 321: /* bool_literal: TRUE  */
#line 1363 "bison_parser.y"
                    { (yyval.expr) = Expr::makeLiteral(true); }
#line 5785 "bison_parser.cpp"
    break;

  case 322: /* bool_literal: FALSE  */
#line 1364 "bison_parser.y"
        { (yyval.expr) = Expr::makeLiteral(false); }
#line 5791 "bison_parser.cpp"
    break;

  case 323: /* num_literal: FLOATVAL  */
#line 1366 "bison_parser.y"
                       { (yyval.expr) = Expr::makeLiteral((yyvsp[0].fval)); }
#line 5797 "bison_parser.cpp"
    break;

  case 325: /* int_literal: INTVAL  */
#line 1369 "bison_parser.y"
                     { (yyval.expr) = Expr::makeLiteral((yyvsp[0].ival)); }
#line 5803 "bison_parser.cpp"
    break;

  case 326: /* null_literal: NULL  */
#line 1371 "bison_parser.y"
                    { (yyval.expr) = Expr::makeNullLiteral(); }
#line 5809 "bison_parser.cpp"
    break;

  case 327: /* date_literal: DATE STRING  */
#line 1373 "bison_parser.y"
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
#line 5824 "bison_parser.cpp"
    break;

  case 328: /* interval_literal: INTVAL duration_field  */
#line 1384 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeIntervalLiteral((yyvsp[-1].ival), (yyvsp[0].datetime_field)); }
#line 5830 "bison_parser.cpp"
    break;

  case 329: /* interval_literal: INTERVAL STRING datetime_field  */
#line 1385 "bison_parser.y"
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
#line 5846 "bison_parser.cpp"
    break;

  case 330: /* interval_literal: INTERVAL STRING  */
#line 1396 "bison_parser.y"
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
#line 5882 "bison_parser.cpp"
    break;

  case 331: /* param_expr: '?'  */
#line 1428 "bison_parser.y"
                 {
  (yyval.expr) = Expr::makeParameter(yylloc.total_column);
  (yyval.expr)->ival2 = yylloc.total_column - 1;  // source column (0-based) of the '?' token
  yyloc.param_list.push_back((yyval.expr));
}
#line 5892 "bison_parser.cpp"
    break;

  case 332: /* param_expr: DOLLAR_PARAM  */
#line 1433 "bison_parser.y"
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
#line 5909 "bison_parser.cpp"
    break;

  case 333: /* param_expr: NAMED_PARAM  */
#line 1445 "bison_parser.y"
              {
  (yyval.expr) = Expr::makeNamedParameter((yyvsp[0].sval));
  (yyval.expr)->ival2 = yylloc.total_column - 1 - (int64_t)strlen((yyvsp[0].sval));
  yyloc.param_list.push_back((yyval.expr));
}
#line 5919 "bison_parser.cpp"
    break;

  case 335: /* table_ref: table_ref_commalist ',' table_ref_atomic  */
#line 1454 "bison_parser.y"
                                                                        {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  auto tbl = new TableRef(kTableCrossProduct);
  tbl->list = (yyvsp[-2].table_vec);
  (yyval.table) = tbl;
}
#line 5930 "bison_parser.cpp"
    break;

  case 339: /* nonjoin_table_ref_atomic: '(' select_statement ')' opt_table_alias  */
#line 1463 "bison_parser.y"
                                                                                     {
  auto tbl = new TableRef(kTableSelect);
  tbl->select = (yyvsp[-2].select_stmt);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 5941 "bison_parser.cpp"
    break;

  case 340: /* nonjoin_table_ref_atomic: function_expr opt_table_alias  */
#line 1471 "bison_parser.y"
                                {
  auto tbl = new TableRef(kTableFunc);
  tbl->func = (yyvsp[-1].expr);
  if ((yyvsp[-1].expr)->name) tbl->name = strdup((yyvsp[-1].expr)->name);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 5953 "bison_parser.cpp"
    break;

  case 341: /* nonjoin_table_ref_atomic: '(' VALUES table_value_row_list ')' opt_table_alias  */
#line 1480 "bison_parser.y"
                                                      {
  auto tbl = new TableRef(kTableValues);
  tbl->values = (yyvsp[-2].expr_vec);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 5964 "bison_parser.cpp"
    break;

  case 342: /* table_value_row_list: '(' expr_list ')'  */
#line 1489 "bison_parser.y"
                                         {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
}
#line 5973 "bison_parser.cpp"
    break;

  case 343: /* table_value_row_list: table_value_row_list ',' '(' expr_list ')'  */
#line 1493 "bison_parser.y"
                                             {
  (yyvsp[-4].expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
  (yyval.expr_vec) = (yyvsp[-4].expr_vec);
}
#line 5982 "bison_parser.cpp"
    break;

  case 344: /* table_ref_commalist: table_ref_atomic  */
#line 1498 "bison_parser.y"
                                       {
  (yyval.table_vec) = new std::vector<TableRef*>();
  (yyval.table_vec)->push_back((yyvsp[0].table));
}
#line 5991 "bison_parser.cpp"
    break;

  case 345: /* table_ref_commalist: table_ref_commalist ',' table_ref_atomic  */
#line 1502 "bison_parser.y"
                                           {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  (yyval.table_vec) = (yyvsp[-2].table_vec);
}
#line 6000 "bison_parser.cpp"
    break;

  case 346: /* table_ref_name: table_name opt_table_alias  */
#line 1507 "bison_parser.y"
                                            {
  auto tbl = new TableRef(kTableName);
  tbl->schema = (yyvsp[-1].table_name).schema;
  tbl->name = (yyvsp[-1].table_name).name;
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6012 "bison_parser.cpp"
    break;

  case 347: /* table_ref_name_no_alias: table_name  */
#line 1515 "bison_parser.y"
                                     {
  (yyval.table) = new TableRef(kTableName);
  (yyval.table)->schema = (yyvsp[0].table_name).schema;
  (yyval.table)->name = (yyvsp[0].table_name).name;
}
#line 6022 "bison_parser.cpp"
    break;

  case 348: /* table_name: IDENTIFIER  */
#line 1521 "bison_parser.y"
                        {
  (yyval.table_name).schema = nullptr;
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6031 "bison_parser.cpp"
    break;

  case 349: /* table_name: IDENTIFIER '.' IDENTIFIER  */
#line 1525 "bison_parser.y"
                            {
  (yyval.table_name).schema = (yyvsp[-2].sval);
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6040 "bison_parser.cpp"
    break;

  case 350: /* table_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1529 "bison_parser.y"
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
#line 6055 "bison_parser.cpp"
    break;

  case 351: /* opt_index_name: IDENTIFIER  */
#line 1540 "bison_parser.y"
                            { (yyval.sval) = (yyvsp[0].sval); }
#line 6061 "bison_parser.cpp"
    break;

  case 352: /* opt_index_name: %empty  */
#line 1541 "bison_parser.y"
              { (yyval.sval) = nullptr; }
#line 6067 "bison_parser.cpp"
    break;

  case 354: /* table_alias: AS IDENTIFIER '(' ident_commalist ')'  */
#line 1543 "bison_parser.y"
                                                            { (yyval.alias_t) = new Alias((yyvsp[-3].sval), (yyvsp[-1].str_vec)); }
#line 6073 "bison_parser.cpp"
    break;

  case 356: /* opt_table_alias: %empty  */
#line 1545 "bison_parser.y"
                                            { (yyval.alias_t) = nullptr; }
#line 6079 "bison_parser.cpp"
    break;

  case 357: /* alias: AS IDENTIFIER  */
#line 1547 "bison_parser.y"
                      { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6085 "bison_parser.cpp"
    break;

  case 358: /* alias: AS STRING  */
#line 1551 "bison_parser.y"
            { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6091 "bison_parser.cpp"
    break;

  case 359: /* alias: IDENTIFIER  */
#line 1552 "bison_parser.y"
             { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6097 "bison_parser.cpp"
    break;

  case 361: /* opt_alias: %empty  */
#line 1554 "bison_parser.y"
                                { (yyval.alias_t) = nullptr; }
#line 6103 "bison_parser.cpp"
    break;

  case 362: /* opt_locking_clause: opt_locking_clause_list  */
#line 1560 "bison_parser.y"
                                             { (yyval.locking_clause_vec) = (yyvsp[0].locking_clause_vec); }
#line 6109 "bison_parser.cpp"
    break;

  case 363: /* opt_locking_clause: %empty  */
#line 1561 "bison_parser.y"
              { (yyval.locking_clause_vec) = nullptr; }
#line 6115 "bison_parser.cpp"
    break;

  case 364: /* opt_locking_clause_list: locking_clause  */
#line 1563 "bison_parser.y"
                                         {
  (yyval.locking_clause_vec) = new std::vector<LockingClause*>();
  (yyval.locking_clause_vec)->push_back((yyvsp[0].locking_t));
}
#line 6124 "bison_parser.cpp"
    break;

  case 365: /* opt_locking_clause_list: opt_locking_clause_list locking_clause  */
#line 1567 "bison_parser.y"
                                         {
  (yyvsp[-1].locking_clause_vec)->push_back((yyvsp[0].locking_t));
  (yyval.locking_clause_vec) = (yyvsp[-1].locking_clause_vec);
}
#line 6133 "bison_parser.cpp"
    break;

  case 366: /* locking_clause: FOR row_lock_mode opt_row_lock_policy  */
#line 1572 "bison_parser.y"
                                                       {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-1].lock_mode_t);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
  (yyval.locking_t)->tables = nullptr;
}
#line 6144 "bison_parser.cpp"
    break;

  case 367: /* locking_clause: FOR row_lock_mode OF ident_commalist opt_row_lock_policy  */
#line 1578 "bison_parser.y"
                                                           {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-3].lock_mode_t);
  (yyval.locking_t)->tables = (yyvsp[-1].str_vec);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
}
#line 6155 "bison_parser.cpp"
    break;

  case 368: /* row_lock_mode: UPDATE  */
#line 1585 "bison_parser.y"
                       { (yyval.lock_mode_t) = RowLockMode::ForUpdate; }
#line 6161 "bison_parser.cpp"
    break;

  case 369: /* row_lock_mode: NO KEY UPDATE  */
#line 1586 "bison_parser.y"
                { (yyval.lock_mode_t) = RowLockMode::ForNoKeyUpdate; }
#line 6167 "bison_parser.cpp"
    break;

  case 370: /* row_lock_mode: SHARE  */
#line 1587 "bison_parser.y"
        { (yyval.lock_mode_t) = RowLockMode::ForShare; }
#line 6173 "bison_parser.cpp"
    break;

  case 371: /* row_lock_mode: KEY SHARE  */
#line 1588 "bison_parser.y"
            { (yyval.lock_mode_t) = RowLockMode::ForKeyShare; }
#line 6179 "bison_parser.cpp"
    break;

  case 372: /* opt_row_lock_policy: SKIP LOCKED  */
#line 1590 "bison_parser.y"
                                  { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::SkipLocked; }
#line 6185 "bison_parser.cpp"
    break;

  case 373: /* opt_row_lock_policy: NOWAIT  */
#line 1591 "bison_parser.y"
         { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::NoWait; }
#line 6191 "bison_parser.cpp"
    break;

  case 374: /* opt_row_lock_policy: %empty  */
#line 1592 "bison_parser.y"
              { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::None; }
#line 6197 "bison_parser.cpp"
    break;

  case 376: /* opt_with_clause: %empty  */
#line 1598 "bison_parser.y"
                                            { (yyval.with_description_vec) = nullptr; }
#line 6203 "bison_parser.cpp"
    break;

  case 377: /* with_clause: WITH with_description_list  */
#line 1600 "bison_parser.y"
                                         { (yyval.with_description_vec) = (yyvsp[0].with_description_vec); }
#line 6209 "bison_parser.cpp"
    break;

  case 378: /* with_description_list: with_description  */
#line 1602 "bison_parser.y"
                                         {
  (yyval.with_description_vec) = new std::vector<WithDescription*>();
  (yyval.with_description_vec)->push_back((yyvsp[0].with_description_t));
}
#line 6218 "bison_parser.cpp"
    break;

  case 379: /* with_description_list: with_description_list ',' with_description  */
#line 1606 "bison_parser.y"
                                             {
  (yyvsp[-2].with_description_vec)->push_back((yyvsp[0].with_description_t));
  (yyval.with_description_vec) = (yyvsp[-2].with_description_vec);
}
#line 6227 "bison_parser.cpp"
    break;

  case 380: /* with_description: IDENTIFIER AS select_with_paren  */
#line 1611 "bison_parser.y"
                                                   {
  (yyval.with_description_t) = new WithDescription();
  (yyval.with_description_t)->alias = (yyvsp[-2].sval);
  (yyval.with_description_t)->select = (yyvsp[0].select_stmt);
}
#line 6237 "bison_parser.cpp"
    break;

  case 381: /* join_clause: table_ref_atomic NATURAL JOIN nonjoin_table_ref_atomic  */
#line 1621 "bison_parser.y"
                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = kJoinNatural;
  (yyval.table)->join->left = (yyvsp[-3].table);
  (yyval.table)->join->right = (yyvsp[0].table);
}
#line 6249 "bison_parser.cpp"
    break;

  case 382: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic ON join_condition  */
#line 1628 "bison_parser.y"
                                                                         {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (JoinType)(yyvsp[-4].join_type);
  (yyval.table)->join->left = (yyvsp[-5].table);
  (yyval.table)->join->right = (yyvsp[-2].table);
  (yyval.table)->join->condition = (yyvsp[0].expr);
}
#line 6262 "bison_parser.cpp"
    break;

  case 383: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic USING '(' ident_commalist ')'  */
#line 1636 "bison_parser.y"
                                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (yyvsp[-6].join_type);
  (yyval.table)->join->left = (yyvsp[-7].table);
  (yyval.table)->join->right = (yyvsp[-4].table);
  (yyval.table)->join->namedColumns = (yyvsp[-1].str_vec);
}
#line 6275 "bison_parser.cpp"
    break;

  case 384: /* opt_join_type: INNER  */
#line 1645 "bison_parser.y"
                      { (yyval.join_type) = kJoinInner; }
#line 6281 "bison_parser.cpp"
    break;

  case 385: /* opt_join_type: LEFT OUTER  */
#line 1646 "bison_parser.y"
             { (yyval.join_type) = kJoinLeft; }
#line 6287 "bison_parser.cpp"
    break;

  case 386: /* opt_join_type: LEFT  */
#line 1647 "bison_parser.y"
       { (yyval.join_type) = kJoinLeft; }
#line 6293 "bison_parser.cpp"
    break;

  case 387: /* opt_join_type: RIGHT OUTER  */
#line 1648 "bison_parser.y"
              { (yyval.join_type) = kJoinRight; }
#line 6299 "bison_parser.cpp"
    break;

  case 388: /* opt_join_type: RIGHT  */
#line 1649 "bison_parser.y"
        { (yyval.join_type) = kJoinRight; }
#line 6305 "bison_parser.cpp"
    break;

  case 389: /* opt_join_type: FULL OUTER  */
#line 1650 "bison_parser.y"
             { (yyval.join_type) = kJoinFull; }
#line 6311 "bison_parser.cpp"
    break;

  case 390: /* opt_join_type: OUTER  */
#line 1651 "bison_parser.y"
        { (yyval.join_type) = kJoinFull; }
#line 6317 "bison_parser.cpp"
    break;

  case 391: /* opt_join_type: FULL  */
#line 1652 "bison_parser.y"
       { (yyval.join_type) = kJoinFull; }
#line 6323 "bison_parser.cpp"
    break;

  case 392: /* opt_join_type: CROSS  */
#line 1653 "bison_parser.y"
        { (yyval.join_type) = kJoinCross; }
#line 6329 "bison_parser.cpp"
    break;

  case 393: /* opt_join_type: %empty  */
#line 1654 "bison_parser.y"
                       { (yyval.join_type) = kJoinInner; }
#line 6335 "bison_parser.cpp"
    break;

  case 397: /* ident_commalist: IDENTIFIER  */
#line 1665 "bison_parser.y"
                             {
  (yyval.str_vec) = new std::vector<char*>();
  (yyval.str_vec)->push_back((yyvsp[0].sval));
}
#line 6344 "bison_parser.cpp"
    break;

  case 398: /* ident_commalist: ident_commalist ',' IDENTIFIER  */
#line 1669 "bison_parser.y"
                                 {
  (yyvsp[-2].str_vec)->push_back((yyvsp[0].sval));
  (yyval.str_vec) = (yyvsp[-2].str_vec);
}
#line 6353 "bison_parser.cpp"
    break;


#line 6357 "bison_parser.cpp"

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

#line 1675 "bison_parser.y"


/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */

    // clang-format on
