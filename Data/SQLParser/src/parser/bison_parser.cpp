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
  YYSYMBOL_case_expr = 286,                /* case_expr  */
  YYSYMBOL_case_list = 287,                /* case_list  */
  YYSYMBOL_exists_expr = 288,              /* exists_expr  */
  YYSYMBOL_comp_expr = 289,                /* comp_expr  */
  YYSYMBOL_function_expr = 290,            /* function_expr  */
  YYSYMBOL_opt_window = 291,               /* opt_window  */
  YYSYMBOL_opt_partition = 292,            /* opt_partition  */
  YYSYMBOL_opt_frame_clause = 293,         /* opt_frame_clause  */
  YYSYMBOL_frame_type = 294,               /* frame_type  */
  YYSYMBOL_frame_bound = 295,              /* frame_bound  */
  YYSYMBOL_extract_expr = 296,             /* extract_expr  */
  YYSYMBOL_cast_expr = 297,                /* cast_expr  */
  YYSYMBOL_datetime_field = 298,           /* datetime_field  */
  YYSYMBOL_datetime_field_plural = 299,    /* datetime_field_plural  */
  YYSYMBOL_duration_field = 300,           /* duration_field  */
  YYSYMBOL_array_expr = 301,               /* array_expr  */
  YYSYMBOL_array_index = 302,              /* array_index  */
  YYSYMBOL_between_expr = 303,             /* between_expr  */
  YYSYMBOL_column_name = 304,              /* column_name  */
  YYSYMBOL_nonreserved_keyword = 305,      /* nonreserved_keyword  */
  YYSYMBOL_literal = 306,                  /* literal  */
  YYSYMBOL_string_literal = 307,           /* string_literal  */
  YYSYMBOL_bool_literal = 308,             /* bool_literal  */
  YYSYMBOL_num_literal = 309,              /* num_literal  */
  YYSYMBOL_int_literal = 310,              /* int_literal  */
  YYSYMBOL_null_literal = 311,             /* null_literal  */
  YYSYMBOL_date_literal = 312,             /* date_literal  */
  YYSYMBOL_interval_literal = 313,         /* interval_literal  */
  YYSYMBOL_param_expr = 314,               /* param_expr  */
  YYSYMBOL_table_ref = 315,                /* table_ref  */
  YYSYMBOL_table_ref_atomic = 316,         /* table_ref_atomic  */
  YYSYMBOL_nonjoin_table_ref_atomic = 317, /* nonjoin_table_ref_atomic  */
  YYSYMBOL_table_ref_commalist = 318,      /* table_ref_commalist  */
  YYSYMBOL_table_ref_name = 319,           /* table_ref_name  */
  YYSYMBOL_table_ref_name_no_alias = 320,  /* table_ref_name_no_alias  */
  YYSYMBOL_table_name = 321,               /* table_name  */
  YYSYMBOL_opt_index_name = 322,           /* opt_index_name  */
  YYSYMBOL_table_alias = 323,              /* table_alias  */
  YYSYMBOL_opt_table_alias = 324,          /* opt_table_alias  */
  YYSYMBOL_alias = 325,                    /* alias  */
  YYSYMBOL_opt_alias = 326,                /* opt_alias  */
  YYSYMBOL_opt_locking_clause = 327,       /* opt_locking_clause  */
  YYSYMBOL_opt_locking_clause_list = 328,  /* opt_locking_clause_list  */
  YYSYMBOL_locking_clause = 329,           /* locking_clause  */
  YYSYMBOL_row_lock_mode = 330,            /* row_lock_mode  */
  YYSYMBOL_opt_row_lock_policy = 331,      /* opt_row_lock_policy  */
  YYSYMBOL_opt_with_clause = 332,          /* opt_with_clause  */
  YYSYMBOL_with_clause = 333,              /* with_clause  */
  YYSYMBOL_with_description_list = 334,    /* with_description_list  */
  YYSYMBOL_with_description = 335,         /* with_description  */
  YYSYMBOL_join_clause = 336,              /* join_clause  */
  YYSYMBOL_opt_join_type = 337,            /* opt_join_type  */
  YYSYMBOL_join_condition = 338,           /* join_condition  */
  YYSYMBOL_opt_semicolon = 339,            /* opt_semicolon  */
  YYSYMBOL_ident_commalist = 340           /* ident_commalist  */
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
#define YYLAST   1277

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  204
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  137
/* YYNRULES -- Number of rules.  */
#define YYNRULES  386
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  698

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
       0,   378,   378,   398,   404,   411,   415,   419,   420,   421,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     438,   439,   441,   445,   450,   454,   464,   465,   466,   468,
     468,   474,   480,   482,   486,   498,   504,   521,   536,   538,
     539,   540,   542,   556,   560,   570,   574,   598,   606,   619,
     626,   641,   661,   662,   667,   678,   691,   703,   710,   717,
     726,   727,   729,   733,   738,   739,   741,   749,   750,   751,
     752,   753,   754,   755,   759,   760,   761,   762,   763,   764,
     765,   766,   767,   768,   769,   771,   772,   774,   775,   776,
     778,   779,   781,   785,   789,   794,   802,   803,   804,   805,
     807,   808,   809,   811,   819,   825,   831,   837,   843,   844,
     851,   857,   859,   869,   876,   887,   894,   902,   903,   910,
     917,   921,   926,   936,   940,   944,   956,   956,   958,   959,
     968,   969,   971,   985,   997,  1002,  1006,  1010,  1015,  1016,
    1018,  1028,  1029,  1031,  1033,  1034,  1036,  1038,  1039,  1041,
    1046,  1048,  1049,  1051,  1052,  1054,  1058,  1063,  1065,  1066,
    1067,  1069,  1070,  1092,  1093,  1094,  1096,  1097,  1098,  1099,
    1100,  1101,  1102,  1107,  1111,  1117,  1118,  1120,  1124,  1129,
    1129,  1133,  1134,  1135,  1137,  1146,  1146,  1146,  1146,  1146,
    1148,  1149,  1149,  1149,  1149,  1149,  1149,  1149,  1149,  1150,
    1150,  1154,  1154,  1156,  1157,  1158,  1159,  1160,  1162,  1162,
    1163,  1164,  1165,  1166,  1167,  1168,  1169,  1170,  1171,  1173,
    1174,  1176,  1177,  1178,  1179,  1183,  1184,  1185,  1186,  1188,
    1189,  1191,  1192,  1194,  1195,  1196,  1197,  1198,  1199,  1200,
    1204,  1205,  1206,  1209,  1214,  1215,  1219,  1220,  1222,  1223,
    1228,  1229,  1230,  1234,  1235,  1236,  1238,  1239,  1240,  1241,
    1242,  1244,  1246,  1248,  1249,  1250,  1251,  1252,  1253,  1255,
    1256,  1257,  1258,  1259,  1260,  1262,  1262,  1264,  1270,  1275,
    1277,  1278,  1279,  1280,  1281,  1289,  1290,  1291,  1292,  1293,
    1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,  1302,  1303,
    1304,  1305,  1306,  1307,  1308,  1310,  1310,  1310,  1310,  1310,
    1310,  1310,  1312,  1314,  1315,  1317,  1318,  1320,  1322,  1324,
    1335,  1336,  1347,  1379,  1384,  1396,  1405,  1405,  1412,  1412,
    1414,  1414,  1421,  1425,  1430,  1438,  1444,  1448,  1452,  1463,
    1464,  1466,  1466,  1468,  1468,  1470,  1474,  1475,  1477,  1477,
    1483,  1484,  1486,  1490,  1495,  1501,  1508,  1509,  1510,  1511,
    1513,  1514,  1515,  1521,  1521,  1523,  1525,  1529,  1534,  1544,
    1551,  1559,  1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,
    1576,  1577,  1579,  1585,  1585,  1588,  1592
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
  "binary_expr", "logic_expr", "in_expr", "case_expr", "case_list",
  "exists_expr", "comp_expr", "function_expr", "opt_window",
  "opt_partition", "opt_frame_clause", "frame_type", "frame_bound",
  "extract_expr", "cast_expr", "datetime_field", "datetime_field_plural",
  "duration_field", "array_expr", "array_index", "between_expr",
  "column_name", "nonreserved_keyword", "literal", "string_literal",
  "bool_literal", "num_literal", "int_literal", "null_literal",
  "date_literal", "interval_literal", "param_expr", "table_ref",
  "table_ref_atomic", "nonjoin_table_ref_atomic", "table_ref_commalist",
  "table_ref_name", "table_ref_name_no_alias", "table_name",
  "opt_index_name", "table_alias", "opt_table_alias", "alias", "opt_alias",
  "opt_locking_clause", "opt_locking_clause_list", "locking_clause",
  "row_lock_mode", "opt_row_lock_policy", "opt_with_clause", "with_clause",
  "with_description_list", "with_description", "join_clause",
  "opt_join_type", "join_condition", "opt_semicolon", "ident_commalist", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-607)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-384)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     958,    52,    91,   110,   123,    91,    14,    64,    84,    88,
      91,   125,    19,   284,    31,   187,    49,    49,    49,   209,
      39,  -607,   117,  -607,   117,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,   -22,  -607,   292,
      59,  -607,   135,   210,  -607,   242,   242,   242,    91,   343,
      91,   234,  -607,   250,   -22,   216,   -35,   250,   250,   250,
      91,  -607,   248,   181,  -607,  -607,  -607,  -607,  -607,  -607,
     733,  -607,   290,  -607,  -607,   272,   226,  -607,   251,  -607,
     397,    67,   413,   298,   422,    91,    91,   350,  -607,   344,
     245,   444,   408,    91,   294,   303,   505,   505,   505,   513,
      91,    91,  -607,   320,   187,  -607,   322,    18,   507,  -607,
    -607,  -607,   -22,   405,   395,   -22,    38,   324,  -607,  -607,
     514,  -607,  -607,   330,   526,  -607,   527,  -607,  -607,    44,
    -607,   333,   331,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,  -607,   487,  -607,   403,   -39,
     245,   718,  -607,   505,   534,   203,   359,   -38,  -607,  -607,
     451,  -607,  -607,  -607,   -51,   -51,   -51,  -607,  -607,  -607,
    -607,  -607,   544,  -607,   545,  -607,  -607,   718,   480,  -607,
    -607,   226,  -607,  -607,   718,   480,   718,   336,   439,   558,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,    61,  -607,   327,  -607,  -607,
    -607,    67,  -607,    91,   559,   448,    53,   437,   -84,  -607,
    -607,  -607,   373,   374,   375,  -607,  -607,   492,  -607,  -607,
    -607,   788,   378,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,   943,  -607,   562,   -29,   222,
    -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,  -607,  -607,  -607,   379,  -607,   476,  -607,   -15,
     380,  -607,   718,   444,  -607,   540,  -607,  -607,   386,    50,
    -607,   350,  -607,   387,    34,  -607,   388,   490,   389,  -607,
     116,    38,   -22,   390,  -607,   -10,    38,   182,   531,   138,
      16,  -607,   439,  -607,  -607,   463,  -607,  -607,   396,   498,
    -607,  1037,   398,   418,   420,   108,  -607,  -607,  -607,   448,
      11,    32,   541,   327,   718,   718,   236,   217,   402,   943,
    1063,   718,    92,   404,   -16,   718,   718,   943,  -607,   943,
     -43,   406,    43,   943,   943,   943,   943,   943,   943,   943,
     943,   943,   943,   943,   943,   943,   943,   943,   545,    25,
      91,  -607,   599,    67,   -29,  -607,   250,    50,   601,   603,
     343,   604,   178,  -607,  -607,    67,  -607,   544,  -607,    20,
     350,  -607,   718,  -607,   194,  -607,  -607,  -607,  -607,   718,
    -607,  -607,   606,   439,   445,   718,   718,  -607,   440,  -607,
     456,    40,  -607,  1037,   534,   505,  -607,  -607,   423,  -607,
     425,  -607,  -607,   426,  -607,  -607,   427,  -607,  -607,  -607,
    -607,   429,  -607,  -607,   190,   534,   430,   432,  -607,    53,
    -607,   547,   718,   433,  -607,   421,   551,   254,   244,   218,
     718,   718,  -607,   541,   548,   -44,  -607,  -607,  -607,   530,
    1042,  1081,   943,   468,   562,  -607,   561,   473,  1081,  1081,
    1081,  1081,   363,   363,   363,   363,    92,    92,    93,    93,
      93,   -75,   474,   547,   718,  -607,  -607,   197,   667,   208,
    -607,  -607,  -607,  -607,  -607,   152,   230,  -607,   448,  -607,
     174,  -607,   470,  -607,    41,  -607,   602,  -607,  -607,  -607,
    -607,   671,  -607,  -607,   503,   -29,   -29,   612,  -607,   534,
    -607,   515,  -607,   479,   232,  -607,   673,   674,  -607,   675,
     680,   681,  -607,  -607,   582,  -607,   512,    91,  -607,   190,
    -607,  -607,   238,   534,   534,  -607,   493,  -607,   239,    26,
    -607,   718,  1037,   718,   718,  -607,   302,   256,   494,  -607,
     943,  1081,   562,   495,   246,  -607,  -607,  -607,   247,  -607,
    -607,  -607,   688,   343,  -607,  -607,   497,   596,  -607,  -607,
    -607,   618,   620,   621,   616,    20,   315,  -607,  -607,  -607,
     590,  -607,  -607,   718,  -607,   167,  -607,  -607,  -607,   520,
     252,   521,   528,   529,  -607,  -607,   245,  -607,  -607,  -607,
     253,   258,   622,   547,   547,   718,   131,   533,   -29,   310,
    -607,   718,  -607,  1063,   535,   263,  -607,  -607,   547,  -607,
    -607,    41,    20,  -607,  -607,  -607,    20,   180,   532,   718,
     183,  -607,  -607,  -607,   729,  -607,  -607,  -607,  -607,  -607,
     560,   608,   480,  -607,  -607,   304,  -607,  -607,  -607,   -29,
    -607,  -607,  -607,  -607,  -607,   224,   534,   -20,   564,   542,
    -607,   718,   348,   547,   546,   718,   308,   718,  -607,  -607,
    -607,   389,  -607,  -607,  -607,   549,    24,  -607,   534,   -29,
    -607,  -607,   -29,  -607,    98,    22,   155,  -607,  -607,   309,
    -607,  -607,   625,  -607,  -607,  -607,    22,  -607
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     364,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    30,    30,    30,     0,
     384,     3,    21,    19,    21,    18,     8,     9,     7,    11,
      16,    17,    13,    14,    12,    15,    10,     0,   363,     0,
     336,   114,    33,     0,    54,    61,    61,    61,     0,     0,
       0,     0,   335,   109,     0,     0,     0,   109,   109,   109,
       0,    52,     0,   365,   366,    29,    26,    28,    27,     1,
     364,     2,     0,     6,     5,   165,   123,   124,   154,   106,
       0,   176,     0,     0,   340,     0,     0,   148,    37,     0,
     118,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     4,     0,     0,   142,   136,
     137,   135,     0,   139,     0,     0,   172,   337,   312,   315,
     317,   324,   325,     0,     0,   318,     0,   313,   314,     0,
     323,     0,   175,   177,   179,   181,   305,   306,   307,   316,
     308,   309,   310,   311,    32,    31,     0,   339,     0,     0,
     118,     0,   113,     0,     0,     0,     0,   148,   120,   108,
       0,   131,   130,    38,    41,    41,    41,   107,   104,   105,
     368,   367,     0,   317,     0,   163,   141,     0,   154,   127,
     126,   128,   138,   134,     0,   154,     0,     0,   351,     0,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   320,     0,   319,   322,   182,   183,
      34,     0,    60,     0,     0,   364,     0,     0,   280,   304,
     302,   300,     0,     0,     0,   298,   297,     0,   299,   303,
     301,     0,     0,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   282,     0,   147,   185,
     192,   193,   194,   187,   189,   195,   188,   208,   196,   197,
     198,   199,   191,   186,   201,   284,   202,     0,   385,     0,
       0,   116,     0,     0,   119,     0,   110,   111,     0,     0,
      51,   148,    50,    24,     0,    22,     0,   145,   143,   173,
     349,   172,     0,   153,   155,   160,   172,   167,   169,   166,
       0,   132,   350,   352,   338,     0,   321,   178,     0,     0,
      57,     0,     0,     0,     0,     0,    62,    64,    65,   364,
     142,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     204,     0,   203,     0,     0,     0,     0,     0,   205,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
       0,   117,     0,     0,   122,   121,   109,     0,     0,     0,
       0,     0,     0,    47,    36,     0,    20,     0,   164,     0,
     148,   144,     0,   347,     0,   348,   184,   125,   129,     0,
     159,   158,   161,   351,     0,     0,     0,   356,     0,   358,
       0,   362,   353,     0,     0,     0,    83,    77,     0,    79,
      89,    80,    67,     0,    74,    75,     0,    71,    72,    78,
      81,    86,    76,    68,    91,     0,     0,     0,    56,     0,
      59,   247,     0,   281,   283,     0,     0,     0,     0,     0,
       0,     0,   227,     0,     0,     0,   200,   190,   219,   220,
       0,   215,     0,     0,     0,   206,     0,   218,   217,   233,
     234,   235,   236,   237,   238,   239,   210,   209,   212,   211,
     213,   214,     0,   247,     0,    35,   386,     0,     0,     0,
      48,    45,    43,    49,    40,     0,     0,    23,   364,   146,
     326,   328,     0,   330,   344,   329,   150,   174,   345,   346,
     156,     0,   157,   133,     0,   170,   168,     0,   359,     0,
     361,     0,   354,     0,     0,    55,     0,     0,    73,     0,
       0,     0,    82,    98,     0,    97,     0,     0,    66,    90,
      92,    94,     0,     0,     0,    63,     0,   240,     0,   142,
     231,     0,     0,     0,     0,   225,     0,     0,     0,   277,
       0,   216,     0,     0,     0,   207,   278,   244,     0,   115,
     112,    39,     0,     0,    46,    25,     0,     0,   380,   372,
     378,   376,   379,   374,     0,     0,     0,   343,   334,   341,
       0,   140,   162,     0,   357,   362,   360,   180,    58,     0,
       0,     0,     0,     0,    99,    96,   118,    93,    95,   101,
       0,     0,   249,   247,   247,     0,     0,     0,   229,     0,
     228,     0,   232,   279,     0,     0,   223,   221,   247,    44,
      42,   344,     0,   375,   377,   373,     0,   327,   345,     0,
       0,   355,    70,    88,     0,    84,    69,    85,   103,   100,
       0,     0,   154,   241,   242,     0,   261,   262,   226,   230,
     224,   222,   245,   331,   369,   381,     0,   152,     0,     0,
     102,     0,   252,   247,     0,     0,     0,     0,   149,   171,
      87,   248,   253,   254,   255,     0,     0,   243,     0,   382,
     370,   342,   151,   246,     0,     0,     0,   260,   250,     0,
     259,   257,     0,   258,   256,   371,     0,   251
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -607,  -607,  -607,   669,  -607,   716,  -607,   369,  -607,   446,
    -607,  -607,  -607,  -607,  -351,   -83,   326,   382,   262,  -607,
    -607,  -607,   428,  -607,   325,  -607,  -361,  -607,  -607,  -607,
    -607,   227,  -607,  -475,  -607,   -46,  -607,  -607,  -607,  -607,
    -607,  -607,  -146,  -607,  -607,   478,  -208,   -88,  -607,    66,
     -48,   -27,  -607,  -607,   -86,  -304,  -607,  -607,  -607,  -136,
    -607,  -607,  -176,  -607,   370,  -607,  -607,  -607,     3,  -326,
    -607,  -271,   550,   553,   381,  -150,  -214,  -607,  -607,  -607,
    -607,  -607,  -607,   438,  -607,  -607,  -607,  -441,  -607,  -607,
    -607,  -606,  -607,  -607,  -162,  -607,  -607,  -607,  -607,  -607,
    -607,  -607,   -61,  -607,  -607,   641,   -89,  -607,  -607,   642,
    -607,  -607,  -516,   151,  -607,  -607,  -607,    -2,  -607,  -607,
     156,   488,  -607,   391,  -607,   477,  -607,   195,  -607,  -607,
    -607,   677,  -607,  -607,  -607,  -607,  -363
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    21,    22,    73,   284,   285,    23,    66,
      24,   145,    25,    26,    89,   164,   280,   372,   373,    27,
      28,    29,    84,   315,   316,   317,   424,   522,   518,   528,
     529,   530,   318,   531,    30,    93,    31,   276,   277,    32,
      33,    34,   155,    35,   157,   158,    36,   178,   179,   180,
      77,   112,   113,   183,    78,   177,   287,   380,   381,   152,
     581,   668,   116,   293,   294,   392,   502,   108,   188,   288,
     131,   132,   133,   134,   289,   290,   249,   250,   251,   252,
     253,   254,   255,   327,   256,   257,   258,   537,   642,   675,
     676,   688,   259,   260,   202,   203,   204,   261,   262,   263,
     264,   265,   266,   136,   137,   138,   139,   140,   141,   142,
     143,   489,   490,   491,   492,   493,    51,   494,   148,   577,
     578,   579,   386,   301,   302,   303,   401,   512,    37,    38,
      63,    64,   495,   574,   680,    71,   269
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      41,   248,   291,    44,   217,   445,    95,   310,    52,   296,
      56,    99,   100,   101,   165,   166,   432,   330,   175,   482,
     135,   274,    40,    40,   173,   176,   181,   185,   684,   181,
     684,   332,   557,   667,   295,   433,   297,   299,    75,   176,
     176,   514,   513,   685,   383,   306,    87,   151,    90,   119,
     120,   115,    60,   368,   598,   474,   311,   214,   102,   627,
     452,    97,   532,   343,   278,   118,   119,   120,   121,   122,
     267,   118,   119,   120,   121,   122,   369,   326,    55,   692,
     397,    39,   390,   149,   150,   286,    45,   453,   215,   335,
     697,   160,   477,    61,    40,   186,    46,   334,   168,   169,
      98,   370,   335,    76,   486,   336,   538,   271,   335,   391,
     655,   430,   187,    42,   320,   330,   321,   123,   336,   383,
      94,   358,   364,   450,   336,   451,    43,    47,   554,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   135,   374,   585,   279,   558,   455,
     135,   124,   398,   549,   292,   368,   371,   124,   382,   216,
      48,   436,   643,   644,   273,   660,   456,   125,   576,   170,
     600,   601,   509,   125,   437,   438,    54,   652,   562,   399,
      49,   607,   382,   447,   361,   448,   449,   362,    50,   686,
      62,   686,   687,   400,   687,   396,   126,   498,   499,   333,
     510,   511,   567,   563,   388,    65,   181,    53,   567,    69,
     431,   308,   620,   126,   127,   128,   174,    54,   488,   126,
     127,   128,   677,   434,   473,   604,   615,   312,   313,   314,
     343,   343,    72,   376,   335,   605,   377,   109,   551,   295,
      70,   337,   568,   384,   496,   505,   506,   569,   568,   335,
     336,   129,   567,   569,   570,   571,   335,   129,   371,    80,
     570,   571,   109,    75,   130,   336,   690,   691,   270,   472,
     130,   572,   336,   110,   435,  -381,   573,   572,   338,   645,
     566,  -381,   573,   354,   355,   356,   357,   357,   358,   358,
     546,   547,   568,   666,   387,    79,   523,   569,   110,   393,
     335,   335,   135,   657,   570,   571,    82,   428,   664,   111,
     429,   440,   544,   524,   135,   689,   336,   336,   628,   499,
     478,   572,   515,   693,   694,   339,   573,   510,   511,   114,
     646,   441,   441,    81,   111,   671,   613,   442,   545,   218,
     118,   119,   120,   121,   122,   340,    88,   394,   658,   219,
     325,    96,   341,   342,   335,   543,    57,   665,   475,    91,
     343,   344,   335,   220,   525,   526,    58,   611,   527,   362,
     336,    83,   335,   221,   335,   103,  -332,   484,   336,    92,
     485,   542,  -333,   104,   222,   223,   224,   225,   336,   106,
     336,   606,   226,   608,   609,   548,   559,    59,   107,   211,
     117,   345,   346,   347,   348,   349,   553,   561,   350,   351,
     485,   352,   353,   354,   355,   356,   357,   144,   358,   338,
     335,   146,   610,   227,   228,   147,   124,   229,   335,   565,
     648,   588,   211,   630,   362,   151,   336,   599,   603,   153,
     362,   382,   125,   154,   336,   617,   618,   156,   382,   382,
     638,   633,   639,   298,   634,   362,   159,   640,   230,   231,
     362,   649,   651,    67,    68,   382,   662,   190,   191,   192,
     193,   194,   195,   232,    85,    86,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   126,   127,
     128,   281,   282,   161,   342,   218,   118,   119,   120,   121,
     122,   343,   162,   663,   614,   219,   382,   681,   695,   163,
     362,   362,   672,   673,   674,   679,   167,   682,    54,   220,
     172,   176,   182,   184,   189,   596,   245,   246,   205,   221,
     206,   207,   210,   211,   247,   212,   213,   268,   272,   130,
     222,   223,   224,   225,   275,  -384,  -384,   283,   226,  -384,
    -384,   173,   352,   353,   354,   355,   356,   357,   114,   358,
     300,   304,   309,    15,   319,   218,   118,   119,   120,   121,
     122,   322,   323,   324,   331,   219,   360,   359,   363,   227,
     228,   366,   124,   229,   367,   375,   379,   378,   395,   220,
     403,   382,   389,   405,   404,   426,   425,   427,   125,   221,
     443,    75,   476,   446,   454,   480,   325,   481,   483,   501,
     222,   223,   224,   225,   230,   231,   504,   507,   226,   508,
     540,   516,    75,   517,   519,   520,   536,   521,   533,   232,
     534,   539,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   126,   127,   128,   541,   335,   227,
     228,   452,   124,   229,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   552,   555,   125,   358,
     560,   556,   575,   580,   582,   583,   584,   586,   587,   589,
     590,   591,   245,   246,   230,   231,   592,   593,   594,   595,
     247,   602,   619,   612,   616,   130,   621,   622,   623,   232,
     624,   625,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   126,   127,   128,   626,   629,   632,
     635,   218,   118,   119,   120,   121,   122,   636,   637,   641,
     656,   219,   647,  -383,   650,   659,   661,   669,   527,   105,
      74,   670,     1,   696,   678,   220,   487,   564,   683,   479,
       2,   365,   245,   246,   535,   221,   597,     3,   305,   500,
     247,   307,     4,   497,   439,   130,   222,   223,   224,   225,
     208,   209,     5,   654,   226,     6,     7,   653,   385,   402,
     631,   171,     0,     0,   503,     0,     0,     8,     9,     0,
       0,   218,   118,   119,   120,   121,   122,    10,     0,     0,
      11,   219,     0,     0,     0,   227,   228,     0,   124,   229,
       0,     0,     0,     0,     0,   220,     0,     0,     0,     0,
       0,     0,    12,     0,   125,   221,    13,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   328,   223,   224,   225,
     230,   231,    14,     0,   226,     0,     0,     0,    15,     0,
       0,     0,     0,     0,     0,   232,     0,     0,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     126,   127,   128,     0,     0,   227,   228,     0,   124,   229,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      16,    17,    18,     0,   125,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,   246,
     230,   329,     0,     0,     0,     0,   247,     0,     0,     0,
       0,   130,     0,     0,     0,   232,     0,     0,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     126,   127,   128,     0,     0,     0,   218,   118,   119,   120,
     121,   122,     0,     0,     0,     0,   219,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     0,     0,
     220,     0,     0,     0,     0,     2,     0,     0,   245,   246,
     221,     0,     3,     0,     0,     0,   247,     4,     0,     0,
       0,   130,   223,   224,   225,     0,     0,     5,     0,   226,
       6,     7,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     8,     9,     0,     0,     0,     0,     0,     0,
       0,     0,    10,     0,     0,    11,     0,     0,     0,     0,
     227,   228,     0,   124,   229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    12,     0,   125,
     406,    13,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   407,   230,   329,    14,   408,   409,
     410,   411,   412,    15,   413,     0,     0,     0,     0,     0,
     232,     0,   414,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   126,   127,   128,   338,     0,
       0,     0,     0,     0,     0,     0,     0,   415,     0,     0,
       0,     0,     0,     0,     0,    16,    17,    18,     0,   338,
       0,     0,     0,     0,     0,   416,     0,   417,   418,     0,
       0,     0,     0,   245,   246,     0,     0,   338,     0,     0,
       0,   247,   419,     0,     0,   339,   130,   420,     0,   421,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   422,
     550,     0,     0,     0,     0,   444,   339,     0,     0,     0,
       0,     0,     0,   342,     0,     0,     0,     0,     0,     0,
     343,   344,     0,     0,  -384,     0,   444,     0,     0,     0,
       0,     0,   423,     0,   342,     0,     0,     0,     0,     0,
       0,   343,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,     0,   343,
    -384,   345,   346,   347,   348,   349,     0,     0,   350,   351,
       0,   352,   353,   354,   355,   356,   357,     0,   358,     0,
       0,     0,   345,   346,   347,   348,   349,     0,     0,   350,
     351,     0,   352,   353,   354,   355,   356,   357,     0,   358,
    -384,  -384,  -384,   348,   349,     0,     0,   350,   351,     0,
     352,   353,   354,   355,   356,   357,     0,   358
};

static const yytype_int16 yycheck[] =
{
       2,   151,   178,     5,   150,   331,    54,   215,    10,   185,
      12,    57,    58,    59,    97,    98,   320,   231,   107,   370,
      81,   157,     3,     3,     6,    14,   112,   115,     6,   115,
       6,   245,   473,    53,   184,     3,   186,   187,    60,    14,
      14,   404,   403,    19,     3,   207,    48,    85,    50,     5,
       6,    78,    21,     3,   529,   359,     3,    96,    60,   575,
     103,    96,   425,   138,   115,     4,     5,     6,     7,     8,
     153,     4,     5,     6,     7,     8,    26,   227,    12,   685,
      64,    29,    92,    85,    86,   174,    72,   130,   127,   118,
     696,    93,   363,    62,     3,    57,    82,   247,   100,   101,
     135,    51,   118,    37,   375,   134,   432,   155,   118,   119,
     626,   319,    74,     3,   198,   329,   200,    50,   134,     3,
      54,   196,   272,   337,   134,   339,     3,   113,   454,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   205,   281,   509,   198,   474,   106,
     211,    90,   136,   197,   181,     3,   106,    90,   202,   198,
      96,   323,   603,   604,   202,   640,   123,   106,   127,   103,
     533,   534,   132,   106,   324,   325,   198,   618,    26,   163,
      96,   542,   202,   199,   199,   335,   336,   202,   100,   167,
       3,   167,   170,   177,   170,    57,   152,     3,     4,   247,
     160,   161,    28,    51,   292,   156,   292,    82,    28,     0,
     199,   213,   563,   152,   153,   154,   198,   198,   198,   152,
     153,   154,   663,   191,   199,   199,   552,   174,   175,   176,
     138,   138,   115,   199,   118,   539,   202,    11,   452,   389,
     201,    19,    68,   127,   380,   395,   396,    73,    68,   118,
     134,   190,    28,    73,    80,    81,   118,   190,   106,   200,
      80,    81,    11,    60,   203,   134,   168,   169,    65,   358,
     203,    97,   134,    47,   322,   101,   102,    97,    56,   605,
     488,   101,   102,   191,   192,   193,   194,   194,   196,   196,
     440,   441,    68,   656,   291,     3,   106,    73,    47,   296,
     118,   118,   363,   629,    80,    81,    96,   199,    84,    83,
     202,    94,    94,   123,   375,   678,   134,   134,     3,     4,
     366,    97,   405,   168,   169,   103,   102,   160,   161,    78,
     199,   114,   114,   198,    83,   661,   550,   120,   120,     3,
       4,     5,     6,     7,     8,   123,     3,   165,   165,    13,
     114,   135,   130,   131,   118,   111,    72,   133,   360,   125,
     138,   139,   118,    27,   174,   175,    82,   111,   178,   202,
     134,   129,   118,    37,   118,   127,   202,   199,   134,   129,
     202,   127,   202,   202,    48,    49,    50,    51,   134,    99,
     134,   541,    56,   543,   544,   443,   199,   113,   126,   202,
       3,   179,   180,   181,   182,   183,   454,   199,   186,   187,
     202,   189,   190,   191,   192,   193,   194,     4,   196,    56,
     118,   123,   120,    87,    88,     3,    90,    91,   118,   199,
     120,   199,   202,   583,   202,    85,   134,   199,   199,    95,
     202,   202,   106,   198,   134,   199,   199,     3,   202,   202,
     596,   199,   199,   117,   202,   202,    48,   199,   122,   123,
     202,   611,   199,    17,    18,   202,   642,   140,   141,   142,
     143,   144,   145,   137,    46,    47,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   165,   166,   199,   131,     3,     4,     5,     6,     7,
       8,   138,   199,   199,   552,    13,   202,   199,   199,     4,
     202,   202,   164,   165,   166,   665,     3,   667,   198,    27,
     198,    14,   117,   128,   200,   527,   190,   191,   198,    37,
       4,     4,   199,   202,   198,    48,   133,     3,   179,   203,
      48,    49,    50,    51,    93,   182,   183,     3,    56,   186,
     187,     6,   189,   190,   191,   192,   193,   194,    78,   196,
     121,     3,     3,   115,   127,     3,     4,     5,     6,     7,
       8,   198,   198,   198,   196,    13,   100,   198,   198,    87,
      88,    41,    90,    91,   198,   198,    96,   199,    57,    27,
     127,   202,   202,    95,   198,   177,   198,   177,   106,    37,
     198,    60,     3,   199,   198,     4,   114,     4,     4,     3,
      48,    49,    50,    51,   122,   123,   171,   177,    56,   163,
     199,   198,    60,   198,   198,   198,    79,   198,   198,   137,
     198,   198,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,    96,   118,    87,
      88,   103,    90,    91,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   198,   106,   106,   196,
       3,   197,   202,    71,     3,   172,    64,   162,   199,     6,
       6,     6,   190,   191,   122,   123,     6,     6,   106,   177,
     198,   198,     4,   199,   199,   203,   199,   101,    80,   137,
      80,    80,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   101,   128,   199,
     199,     3,     4,     5,     6,     7,     8,   199,   199,   107,
     198,    13,   199,     0,   199,     6,   128,   173,   178,    70,
      24,   199,     9,   118,   198,    27,   377,   485,   199,   367,
      17,   273,   190,   191,   429,    37,   529,    24,   205,   389,
     198,   211,    29,   382,   326,   203,    48,    49,    50,    51,
     129,   129,    39,   622,    56,    42,    43,   621,   290,   302,
     585,   104,    -1,    -1,   393,    -1,    -1,    54,    55,    -1,
      -1,     3,     4,     5,     6,     7,     8,    64,    -1,    -1,
      67,    13,    -1,    -1,    -1,    87,    88,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,   106,    37,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
     122,   123,   109,    -1,    56,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,    87,    88,    -1,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,   158,   159,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
     122,   123,    -1,    -1,    -1,    -1,   198,    -1,    -1,    -1,
      -1,   203,    -1,    -1,    -1,   137,    -1,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    17,    -1,    -1,   190,   191,
      37,    -1,    24,    -1,    -1,    -1,   198,    29,    -1,    -1,
      -1,   203,    49,    50,    51,    -1,    -1,    39,    -1,    56,
      42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      87,    88,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,   106,
      13,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,   122,   123,   109,    31,    32,
      33,    34,    35,   115,    37,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    45,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,   158,   159,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    90,    91,    -1,
      -1,    -1,    -1,   190,   191,    -1,    -1,    56,    -1,    -1,
      -1,   198,   105,    -1,    -1,   103,   203,   110,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     118,    -1,    -1,    -1,    -1,   123,   103,    -1,    -1,    -1,
      -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,    -1,    -1,   103,    -1,   123,    -1,    -1,    -1,
      -1,    -1,   155,    -1,   131,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,   179,   180,   181,   182,   183,    -1,    -1,   186,   187,
      -1,   189,   190,   191,   192,   193,   194,    -1,   196,    -1,
      -1,    -1,   179,   180,   181,   182,   183,    -1,    -1,   186,
     187,    -1,   189,   190,   191,   192,   193,   194,    -1,   196,
     179,   180,   181,   182,   183,    -1,    -1,   186,   187,    -1,
     189,   190,   191,   192,   193,   194,    -1,   196
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     9,    17,    24,    29,    39,    42,    43,    54,    55,
      64,    67,    89,    93,   109,   115,   157,   158,   159,   205,
     206,   207,   208,   212,   214,   216,   217,   223,   224,   225,
     238,   240,   243,   244,   245,   247,   250,   332,   333,    29,
       3,   321,     3,     3,   321,    72,    82,   113,    96,    96,
     100,   320,   321,    82,   198,   253,   321,    72,    82,   113,
      21,    62,     3,   334,   335,   156,   213,   213,   213,     0,
     201,   339,   115,   209,   209,    60,   253,   254,   258,     3,
     200,   198,    96,   129,   226,   226,   226,   321,     3,   218,
     321,   125,   129,   239,   253,   254,   135,    96,   135,   239,
     239,   239,   321,   127,   202,   207,    99,   126,   271,    11,
      47,    83,   255,   256,    78,   255,   266,     3,     4,     5,
       6,     7,     8,    50,    90,   106,   152,   153,   154,   190,
     203,   274,   275,   276,   277,   306,   307,   308,   309,   310,
     311,   312,   313,   314,     4,   215,   123,     3,   322,   321,
     321,    85,   263,    95,   198,   246,     3,   248,   249,    48,
     321,   199,   199,     4,   219,   219,   219,     3,   321,   321,
     253,   335,   198,     6,   198,   310,    14,   259,   251,   252,
     253,   258,   117,   257,   128,   251,    57,    74,   272,   200,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   298,   299,   300,   198,     4,     4,   309,   313,
     199,   202,    48,   133,    96,   127,   198,   246,     3,    13,
      27,    37,    48,    49,    50,    51,    56,    87,    88,    91,
     122,   123,   137,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   190,   191,   198,   279,   280,
     281,   282,   283,   284,   285,   286,   288,   289,   290,   296,
     297,   301,   302,   303,   304,   305,   306,   219,     3,   340,
      65,   254,   179,   202,   263,    93,   241,   242,   115,   198,
     220,   220,   220,     3,   210,   211,   310,   260,   273,   278,
     279,   266,   255,   267,   268,   279,   266,   279,   117,   279,
     121,   327,   328,   329,     3,   277,   298,   276,   321,     3,
     250,     3,   174,   175,   176,   227,   228,   229,   236,   127,
     198,   200,   198,   198,   198,   114,   279,   287,    48,   123,
     280,   196,   280,   254,   279,   118,   134,    19,    56,   103,
     123,   130,   131,   138,   139,   179,   180,   181,   182,   183,
     186,   187,   189,   190,   191,   192,   193,   194,   196,   198,
     100,   199,   202,   198,   279,   249,    41,   198,     3,    26,
      51,   106,   221,   222,   263,   198,   199,   202,   199,    96,
     261,   262,   202,     3,   127,   325,   326,   272,   251,   202,
      92,   119,   269,   272,   165,    57,    57,    64,   136,   163,
     177,   330,   329,   127,   198,    95,    13,    27,    31,    32,
      33,    34,    35,    37,    45,    70,    88,    90,    91,   105,
     110,   112,   122,   155,   230,   198,   177,   177,   199,   202,
     250,   199,   259,     3,   191,   254,   298,   279,   279,   287,
      94,   114,   120,   198,   123,   273,   199,   199,   279,   279,
     280,   280,   103,   130,   198,   106,   123,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   310,   199,   259,   321,     3,   275,   239,   221,
       4,     4,   218,     4,   199,   202,   275,   211,   198,   315,
     316,   317,   318,   319,   321,   336,   263,   278,     3,     4,
     268,     3,   270,   327,   171,   279,   279,   177,   163,   132,
     160,   161,   331,   230,   340,   219,   198,   198,   232,   198,
     198,   198,   231,   106,   123,   174,   175,   178,   233,   234,
     235,   237,   340,   198,   198,   228,    79,   291,   273,   198,
     199,    96,   127,   111,    94,   120,   279,   279,   254,   197,
     118,   280,   198,   254,   273,   106,   197,   291,   273,   199,
       3,   199,    26,    51,   222,   199,   250,    28,    68,    73,
      80,    81,    97,   102,   337,   202,   127,   323,   324,   325,
      71,   264,     3,   172,    64,   340,   162,   199,   199,     6,
       6,     6,     6,     6,   106,   177,   321,   235,   237,   199,
     340,   340,   198,   199,   199,   259,   279,   230,   279,   279,
     120,   111,   199,   280,   254,   273,   199,   199,   199,     4,
     218,   199,   101,    80,    80,    80,   101,   316,     3,   128,
     279,   331,   199,   199,   202,   199,   199,   199,   246,   199,
     199,   107,   292,   291,   291,   273,   199,   199,   120,   279,
     199,   199,   291,   324,   317,   316,   198,   273,   165,     6,
     237,   128,   266,   199,    84,   133,   340,    53,   265,   173,
     199,   273,   164,   165,   166,   293,   294,   291,   198,   279,
     338,   199,   279,   199,     6,    19,   167,   170,   295,   340,
     168,   169,   295,   168,   169,   199,   118,   295
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
     284,   285,   285,   285,   285,   286,   286,   286,   286,   287,
     287,   288,   288,   289,   289,   289,   289,   289,   289,   289,
     290,   290,   290,   290,   290,   290,   291,   291,   292,   292,
     293,   293,   293,   294,   294,   294,   295,   295,   295,   295,
     295,   296,   297,   298,   298,   298,   298,   298,   298,   299,
     299,   299,   299,   299,   299,   300,   300,   301,   302,   303,
     304,   304,   304,   304,   304,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   306,   306,   306,   306,   306,
     306,   306,   307,   308,   308,   309,   309,   310,   311,   312,
     313,   313,   313,   314,   314,   314,   315,   315,   316,   316,
     317,   317,   318,   318,   319,   320,   321,   321,   321,   322,
     322,   323,   323,   324,   324,   325,   325,   325,   326,   326,
     327,   327,   328,   328,   329,   329,   330,   330,   330,   330,
     331,   331,   331,   332,   332,   333,   334,   334,   335,   336,
     336,   336,   337,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   338,   339,   339,   340,   340
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
       3,     5,     6,     5,     6,     4,     6,     3,     5,     4,
       5,     4,     5,     3,     3,     3,     3,     3,     3,     3,
       4,     6,     6,     8,     4,     6,     6,     0,     3,     0,
       2,     5,     0,     1,     1,     1,     2,     2,     2,     2,
       1,     6,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     5,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     3,     2,     1,     1,     1,     1,     3,     1,     1,
       1,     4,     1,     3,     2,     1,     1,     3,     5,     1,
       0,     1,     5,     1,     0,     2,     2,     1,     1,     0,
       1,     0,     1,     2,     3,     5,     1,     3,     1,     2,
       2,     1,     0,     1,     0,     2,     1,     3,     3,     4,
       6,     8,     1,     2,     1,     2,     1,     2,     1,     1,
       1,     0,     1,     1,     0,     1,     3
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
#line 2217 "bison_parser.cpp"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2223 "bison_parser.cpp"
        break;

    case YYSYMBOL_FLOATVAL: /* FLOATVAL  */
#line 196 "bison_parser.y"
            { }
#line 2229 "bison_parser.cpp"
        break;

    case YYSYMBOL_INTVAL: /* INTVAL  */
#line 196 "bison_parser.y"
            { }
#line 2235 "bison_parser.cpp"
        break;

    case YYSYMBOL_DOLLAR_PARAM: /* DOLLAR_PARAM  */
#line 196 "bison_parser.y"
            { }
#line 2241 "bison_parser.cpp"
        break;

    case YYSYMBOL_NAMED_PARAM: /* NAMED_PARAM  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2247 "bison_parser.cpp"
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
#line 2260 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2266 "bison_parser.cpp"
        break;

    case YYSYMBOL_preparable_statement: /* preparable_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2272 "bison_parser.cpp"
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
#line 2285 "bison_parser.cpp"
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
#line 2298 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint: /* hint  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2304 "bison_parser.cpp"
        break;

    case YYSYMBOL_transaction_statement: /* transaction_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).transaction_stmt)); }
#line 2310 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_statement: /* prepare_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).prep_stmt)); }
#line 2316 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_target_query: /* prepare_target_query  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2322 "bison_parser.cpp"
        break;

    case YYSYMBOL_execute_statement: /* execute_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2328 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_statement: /* import_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_stmt)); }
#line 2334 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_type: /* file_type  */
#line 196 "bison_parser.y"
            { }
#line 2340 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_path: /* file_path  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2346 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_import_export_options: /* opt_import_export_options  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2352 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_export_options: /* import_export_options  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2358 "bison_parser.cpp"
        break;

    case YYSYMBOL_csv_option: /* csv_option  */
#line 218 "bison_parser.y"
            {
  free(((*yyvaluep).csv_option_t)->second);
  delete (((*yyvaluep).csv_option_t));
}
#line 2367 "bison_parser.cpp"
        break;

    case YYSYMBOL_export_statement: /* export_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).export_stmt)); }
#line 2373 "bison_parser.cpp"
        break;

    case YYSYMBOL_show_statement: /* show_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).show_stmt)); }
#line 2379 "bison_parser.cpp"
        break;

    case YYSYMBOL_create_statement: /* create_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).create_stmt)); }
#line 2385 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_not_exists: /* opt_not_exists  */
#line 196 "bison_parser.y"
            { }
#line 2391 "bison_parser.cpp"
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
#line 2404 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem: /* table_elem  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table_element_t)); }
#line 2410 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_def: /* column_def  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).column_t)); }
#line 2416 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_type: /* column_type  */
#line 196 "bison_parser.y"
            { }
#line 2422 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_time_precision: /* opt_time_precision  */
#line 196 "bison_parser.y"
            { }
#line 2428 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_decimal_specification: /* opt_decimal_specification  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).ival_pair)); }
#line 2434 "bison_parser.cpp"
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
#line 2456 "bison_parser.cpp"
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
#line 2478 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraint: /* column_constraint  */
#line 196 "bison_parser.y"
            { }
#line 2484 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_constraint: /* table_constraint  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table_constraint_t)); }
#line 2490 "bison_parser.cpp"
        break;

    case YYSYMBOL_references_spec: /* references_spec  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).references_spec_t)); }
#line 2496 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_statement: /* drop_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).drop_stmt)); }
#line 2502 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_exists: /* opt_exists  */
#line 196 "bison_parser.y"
            { }
#line 2508 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_statement: /* alter_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alter_stmt)); }
#line 2514 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_action: /* alter_action  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alter_action_t)); }
#line 2520 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_action: /* drop_action  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).drop_action_t)); }
#line 2526 "bison_parser.cpp"
        break;

    case YYSYMBOL_delete_statement: /* delete_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2532 "bison_parser.cpp"
        break;

    case YYSYMBOL_truncate_statement: /* truncate_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2538 "bison_parser.cpp"
        break;

    case YYSYMBOL_insert_statement: /* insert_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).insert_stmt)); }
#line 2544 "bison_parser.cpp"
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
#line 2557 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_statement: /* update_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).update_stmt)); }
#line 2563 "bison_parser.cpp"
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
#line 2576 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause: /* update_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).update_t)); }
#line 2582 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_statement: /* select_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2588 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation: /* select_within_set_operation  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2594 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation_no_parentheses: /* select_within_set_operation_no_parentheses  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2600 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_with_paren: /* select_with_paren  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2606 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_no_paren: /* select_no_paren  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2612 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_operator: /* set_operator  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2618 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_type: /* set_type  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2624 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_all: /* opt_all  */
#line 196 "bison_parser.y"
            { }
#line 2630 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_clause: /* select_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2636 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_distinct: /* opt_distinct  */
#line 196 "bison_parser.y"
            { }
#line 2642 "bison_parser.cpp"
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
#line 2655 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_from_clause: /* opt_from_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2661 "bison_parser.cpp"
        break;

    case YYSYMBOL_from_clause: /* from_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2667 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_where: /* opt_where  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2673 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_group: /* opt_group  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).group_t)); }
#line 2679 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_having: /* opt_having  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2685 "bison_parser.cpp"
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
#line 2698 "bison_parser.cpp"
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
#line 2711 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_desc: /* order_desc  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).order)); }
#line 2717 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order_type: /* opt_order_type  */
#line 196 "bison_parser.y"
            { }
#line 2723 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_null_ordering: /* opt_null_ordering  */
#line 196 "bison_parser.y"
            { }
#line 2729 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_top: /* opt_top  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 2735 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_limit: /* opt_limit  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 2741 "bison_parser.cpp"
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
#line 2754 "bison_parser.cpp"
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
#line 2767 "bison_parser.cpp"
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
#line 2780 "bison_parser.cpp"
        break;

    case YYSYMBOL_casted_extended_literal: /* casted_extended_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2786 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal: /* extended_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2792 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_alias: /* expr_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2798 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr: /* expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2804 "bison_parser.cpp"
        break;

    case YYSYMBOL_operand: /* operand  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2810 "bison_parser.cpp"
        break;

    case YYSYMBOL_scalar_expr: /* scalar_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2816 "bison_parser.cpp"
        break;

    case YYSYMBOL_unary_expr: /* unary_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2822 "bison_parser.cpp"
        break;

    case YYSYMBOL_binary_expr: /* binary_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2828 "bison_parser.cpp"
        break;

    case YYSYMBOL_logic_expr: /* logic_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2834 "bison_parser.cpp"
        break;

    case YYSYMBOL_in_expr: /* in_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2840 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_expr: /* case_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2846 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_list: /* case_list  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2852 "bison_parser.cpp"
        break;

    case YYSYMBOL_exists_expr: /* exists_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2858 "bison_parser.cpp"
        break;

    case YYSYMBOL_comp_expr: /* comp_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2864 "bison_parser.cpp"
        break;

    case YYSYMBOL_function_expr: /* function_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2870 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_window: /* opt_window  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).window_description)); }
#line 2876 "bison_parser.cpp"
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
#line 2889 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_frame_clause: /* opt_frame_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).frame_description)); }
#line 2895 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_type: /* frame_type  */
#line 196 "bison_parser.y"
            { }
#line 2901 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_bound: /* frame_bound  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).frame_bound)); }
#line 2907 "bison_parser.cpp"
        break;

    case YYSYMBOL_extract_expr: /* extract_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2913 "bison_parser.cpp"
        break;

    case YYSYMBOL_cast_expr: /* cast_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2919 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field: /* datetime_field  */
#line 196 "bison_parser.y"
            { }
#line 2925 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field_plural: /* datetime_field_plural  */
#line 196 "bison_parser.y"
            { }
#line 2931 "bison_parser.cpp"
        break;

    case YYSYMBOL_duration_field: /* duration_field  */
#line 196 "bison_parser.y"
            { }
#line 2937 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_expr: /* array_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2943 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_index: /* array_index  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2949 "bison_parser.cpp"
        break;

    case YYSYMBOL_between_expr: /* between_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2955 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_name: /* column_name  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2961 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonreserved_keyword: /* nonreserved_keyword  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2967 "bison_parser.cpp"
        break;

    case YYSYMBOL_literal: /* literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2973 "bison_parser.cpp"
        break;

    case YYSYMBOL_string_literal: /* string_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2979 "bison_parser.cpp"
        break;

    case YYSYMBOL_bool_literal: /* bool_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2985 "bison_parser.cpp"
        break;

    case YYSYMBOL_num_literal: /* num_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2991 "bison_parser.cpp"
        break;

    case YYSYMBOL_int_literal: /* int_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2997 "bison_parser.cpp"
        break;

    case YYSYMBOL_null_literal: /* null_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3003 "bison_parser.cpp"
        break;

    case YYSYMBOL_date_literal: /* date_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3009 "bison_parser.cpp"
        break;

    case YYSYMBOL_interval_literal: /* interval_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3015 "bison_parser.cpp"
        break;

    case YYSYMBOL_param_expr: /* param_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3021 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref: /* table_ref  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3027 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_atomic: /* table_ref_atomic  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3033 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonjoin_table_ref_atomic: /* nonjoin_table_ref_atomic  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3039 "bison_parser.cpp"
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
#line 3052 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name: /* table_ref_name  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3058 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name_no_alias: /* table_ref_name_no_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3064 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_name: /* table_name  */
#line 197 "bison_parser.y"
            {
  free(((*yyvaluep).table_name).name);
  free(((*yyvaluep).table_name).schema);
}
#line 3073 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_index_name: /* opt_index_name  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3079 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_alias: /* table_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3085 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_table_alias: /* opt_table_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3091 "bison_parser.cpp"
        break;

    case YYSYMBOL_alias: /* alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3097 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_alias: /* opt_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3103 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause: /* opt_locking_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3109 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause_list: /* opt_locking_clause_list  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3115 "bison_parser.cpp"
        break;

    case YYSYMBOL_locking_clause: /* locking_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_t)); }
#line 3121 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_lock_mode: /* row_lock_mode  */
#line 196 "bison_parser.y"
            { }
#line 3127 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_row_lock_policy: /* opt_row_lock_policy  */
#line 196 "bison_parser.y"
            { }
#line 3133 "bison_parser.cpp"
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
#line 3146 "bison_parser.cpp"
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
#line 3159 "bison_parser.cpp"
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
#line 3172 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description: /* with_description  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).with_description_t)); }
#line 3178 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_clause: /* join_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3184 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_join_type: /* opt_join_type  */
#line 196 "bison_parser.y"
            { }
#line 3190 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_condition: /* join_condition  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3196 "bison_parser.cpp"
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
#line 3209 "bison_parser.cpp"
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

#line 3317 "bison_parser.cpp"

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
#line 378 "bison_parser.y"
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
#line 3547 "bison_parser.cpp"
    break;

  case 3: /* statement_list: statement  */
#line 398 "bison_parser.y"
                           {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyval.stmt_vec) = new std::vector<SQLStatement*>();
  (yyval.stmt_vec)->push_back((yyvsp[0].statement));
}
#line 3558 "bison_parser.cpp"
    break;

  case 4: /* statement_list: statement_list ';' statement  */
#line 404 "bison_parser.y"
                               {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyvsp[-2].stmt_vec)->push_back((yyvsp[0].statement));
  (yyval.stmt_vec) = (yyvsp[-2].stmt_vec);
}
#line 3569 "bison_parser.cpp"
    break;

  case 5: /* statement: prepare_statement opt_hints  */
#line 411 "bison_parser.y"
                                        {
  (yyval.statement) = (yyvsp[-1].prep_stmt);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3578 "bison_parser.cpp"
    break;

  case 6: /* statement: preparable_statement opt_hints  */
#line 415 "bison_parser.y"
                                 {
  (yyval.statement) = (yyvsp[-1].statement);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3587 "bison_parser.cpp"
    break;

  case 7: /* statement: show_statement  */
#line 419 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].show_stmt); }
#line 3593 "bison_parser.cpp"
    break;

  case 8: /* statement: import_statement  */
#line 420 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].import_stmt); }
#line 3599 "bison_parser.cpp"
    break;

  case 9: /* statement: export_statement  */
#line 421 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].export_stmt); }
#line 3605 "bison_parser.cpp"
    break;

  case 10: /* preparable_statement: select_statement  */
#line 423 "bison_parser.y"
                                        { (yyval.statement) = (yyvsp[0].select_stmt); }
#line 3611 "bison_parser.cpp"
    break;

  case 11: /* preparable_statement: create_statement  */
#line 424 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].create_stmt); }
#line 3617 "bison_parser.cpp"
    break;

  case 12: /* preparable_statement: insert_statement  */
#line 425 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].insert_stmt); }
#line 3623 "bison_parser.cpp"
    break;

  case 13: /* preparable_statement: delete_statement  */
#line 426 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3629 "bison_parser.cpp"
    break;

  case 14: /* preparable_statement: truncate_statement  */
#line 427 "bison_parser.y"
                     { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3635 "bison_parser.cpp"
    break;

  case 15: /* preparable_statement: update_statement  */
#line 428 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].update_stmt); }
#line 3641 "bison_parser.cpp"
    break;

  case 16: /* preparable_statement: drop_statement  */
#line 429 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].drop_stmt); }
#line 3647 "bison_parser.cpp"
    break;

  case 17: /* preparable_statement: alter_statement  */
#line 430 "bison_parser.y"
                  { (yyval.statement) = (yyvsp[0].alter_stmt); }
#line 3653 "bison_parser.cpp"
    break;

  case 18: /* preparable_statement: execute_statement  */
#line 431 "bison_parser.y"
                    { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 3659 "bison_parser.cpp"
    break;

  case 19: /* preparable_statement: transaction_statement  */
#line 432 "bison_parser.y"
                        { (yyval.statement) = (yyvsp[0].transaction_stmt); }
#line 3665 "bison_parser.cpp"
    break;

  case 20: /* opt_hints: WITH HINT '(' hint_list ')'  */
#line 438 "bison_parser.y"
                                        { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 3671 "bison_parser.cpp"
    break;

  case 21: /* opt_hints: %empty  */
#line 439 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 3677 "bison_parser.cpp"
    break;

  case 22: /* hint_list: hint  */
#line 441 "bison_parser.y"
                 {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 3686 "bison_parser.cpp"
    break;

  case 23: /* hint_list: hint_list ',' hint  */
#line 445 "bison_parser.y"
                     {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 3695 "bison_parser.cpp"
    break;

  case 24: /* hint: IDENTIFIER  */
#line 450 "bison_parser.y"
                  {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[0].sval);
}
#line 3704 "bison_parser.cpp"
    break;

  case 25: /* hint: IDENTIFIER '(' extended_literal_list ')'  */
#line 454 "bison_parser.y"
                                           {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[-3].sval);
  (yyval.expr)->exprList = (yyvsp[-1].expr_vec);
}
#line 3714 "bison_parser.cpp"
    break;

  case 26: /* transaction_statement: BEGIN opt_transaction_keyword  */
#line 464 "bison_parser.y"
                                                      { (yyval.transaction_stmt) = new TransactionStatement(kBeginTransaction); }
#line 3720 "bison_parser.cpp"
    break;

  case 27: /* transaction_statement: ROLLBACK opt_transaction_keyword  */
#line 465 "bison_parser.y"
                                   { (yyval.transaction_stmt) = new TransactionStatement(kRollbackTransaction); }
#line 3726 "bison_parser.cpp"
    break;

  case 28: /* transaction_statement: COMMIT opt_transaction_keyword  */
#line 466 "bison_parser.y"
                                 { (yyval.transaction_stmt) = new TransactionStatement(kCommitTransaction); }
#line 3732 "bison_parser.cpp"
    break;

  case 31: /* prepare_statement: PREPARE IDENTIFIER FROM prepare_target_query  */
#line 474 "bison_parser.y"
                                                                 {
  (yyval.prep_stmt) = new PrepareStatement();
  (yyval.prep_stmt)->name = (yyvsp[-2].sval);
  (yyval.prep_stmt)->query = (yyvsp[0].sval);
}
#line 3742 "bison_parser.cpp"
    break;

  case 33: /* execute_statement: EXECUTE IDENTIFIER  */
#line 482 "bison_parser.y"
                                       {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[0].sval);
}
#line 3751 "bison_parser.cpp"
    break;

  case 34: /* execute_statement: EXECUTE IDENTIFIER '(' opt_extended_literal_list ')'  */
#line 486 "bison_parser.y"
                                                       {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[-3].sval);
  (yyval.exec_stmt)->parameters = (yyvsp[-1].expr_vec);
}
#line 3761 "bison_parser.cpp"
    break;

  case 35: /* import_statement: IMPORT FROM file_type FILE file_path INTO table_name  */
#line 498 "bison_parser.y"
                                                                        {
  (yyval.import_stmt) = new ImportStatement((yyvsp[-4].import_type_t));
  (yyval.import_stmt)->filePath = (yyvsp[-2].sval);
  (yyval.import_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.import_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 3772 "bison_parser.cpp"
    break;

  case 36: /* import_statement: COPY table_name FROM file_path opt_import_export_options opt_where  */
#line 504 "bison_parser.y"
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
#line 3793 "bison_parser.cpp"
    break;

  case 37: /* file_type: IDENTIFIER  */
#line 521 "bison_parser.y"
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
#line 3812 "bison_parser.cpp"
    break;

  case 38: /* file_path: STRING  */
#line 536 "bison_parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 3818 "bison_parser.cpp"
    break;

  case 39: /* opt_import_export_options: WITH '(' import_export_options ')'  */
#line 538 "bison_parser.y"
                                                               { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 3824 "bison_parser.cpp"
    break;

  case 40: /* opt_import_export_options: '(' import_export_options ')'  */
#line 539 "bison_parser.y"
                                { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 3830 "bison_parser.cpp"
    break;

  case 41: /* opt_import_export_options: %empty  */
#line 540 "bison_parser.y"
              { (yyval.import_export_option_t) = new ImportExportOptions{}; }
#line 3836 "bison_parser.cpp"
    break;

  case 42: /* import_export_options: import_export_options ',' FORMAT file_type  */
#line 542 "bison_parser.y"
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
#line 3855 "bison_parser.cpp"
    break;

  case 43: /* import_export_options: FORMAT file_type  */
#line 556 "bison_parser.y"
                   {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->format = (yyvsp[0].import_type_t);
}
#line 3864 "bison_parser.cpp"
    break;

  case 44: /* import_export_options: import_export_options ',' ENCODING STRING  */
#line 560 "bison_parser.y"
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
#line 3879 "bison_parser.cpp"
    break;

  case 45: /* import_export_options: ENCODING STRING  */
#line 570 "bison_parser.y"
                  {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->encoding = (yyvsp[0].sval);
}
#line 3888 "bison_parser.cpp"
    break;

  case 46: /* import_export_options: import_export_options ',' csv_option  */
#line 574 "bison_parser.y"
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
#line 3917 "bison_parser.cpp"
    break;

  case 47: /* import_export_options: csv_option  */
#line 598 "bison_parser.y"
             {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->csv_options = new CsvOptions{};
  (yyval.import_export_option_t)->csv_options->accept_csv_option((yyvsp[0].csv_option_t));

  delete (yyvsp[0].csv_option_t);
}
#line 3929 "bison_parser.cpp"
    break;

  case 48: /* csv_option: IDENTIFIER STRING  */
#line 606 "bison_parser.y"
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
#line 3947 "bison_parser.cpp"
    break;

  case 49: /* csv_option: NULL STRING  */
#line 619 "bison_parser.y"
              { (yyval.csv_option_t) = new std::pair<CsvOptionType, char*>(CsvOptionType::Null, (yyvsp[0].sval)); }
#line 3953 "bison_parser.cpp"
    break;

  case 50: /* export_statement: COPY table_name TO file_path opt_import_export_options  */
#line 626 "bison_parser.y"
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
#line 3973 "bison_parser.cpp"
    break;

  case 51: /* export_statement: COPY select_with_paren TO file_path opt_import_export_options  */
#line 641 "bison_parser.y"
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
#line 3992 "bison_parser.cpp"
    break;

  case 52: /* show_statement: SHOW TABLES  */
#line 661 "bison_parser.y"
                             { (yyval.show_stmt) = new ShowStatement(kShowTables); }
#line 3998 "bison_parser.cpp"
    break;

  case 53: /* show_statement: SHOW COLUMNS table_name  */
#line 662 "bison_parser.y"
                          {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4008 "bison_parser.cpp"
    break;

  case 54: /* show_statement: DESCRIBE table_name  */
#line 667 "bison_parser.y"
                      {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4018 "bison_parser.cpp"
    break;

  case 55: /* create_statement: CREATE TABLE opt_not_exists table_name FROM IDENTIFIER FILE file_path  */
#line 678 "bison_parser.y"
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
#line 4036 "bison_parser.cpp"
    break;

  case 56: /* create_statement: CREATE TABLE opt_not_exists table_name '(' table_elem_commalist ')'  */
#line 691 "bison_parser.y"
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
#line 4053 "bison_parser.cpp"
    break;

  case 57: /* create_statement: CREATE TABLE opt_not_exists table_name AS select_statement  */
#line 703 "bison_parser.y"
                                                             {
  (yyval.create_stmt) = new CreateStatement(kCreateTable);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-3].bval);
  (yyval.create_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4065 "bison_parser.cpp"
    break;

  case 58: /* create_statement: CREATE INDEX opt_not_exists opt_index_name ON table_name '(' ident_commalist ')'  */
#line 710 "bison_parser.y"
                                                                                   {
  (yyval.create_stmt) = new CreateStatement(kCreateIndex);
  (yyval.create_stmt)->indexName = (yyvsp[-5].sval);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-6].bval);
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->indexColumns = (yyvsp[-1].str_vec);
}
#line 4077 "bison_parser.cpp"
    break;

  case 59: /* create_statement: CREATE VIEW opt_not_exists table_name opt_column_list AS select_statement  */
#line 717 "bison_parser.y"
                                                                            {
  (yyval.create_stmt) = new CreateStatement(kCreateView);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-4].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->viewColumns = (yyvsp[-2].str_vec);
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4090 "bison_parser.cpp"
    break;

  case 60: /* opt_not_exists: IF NOT EXISTS  */
#line 726 "bison_parser.y"
                               { (yyval.bval) = true; }
#line 4096 "bison_parser.cpp"
    break;

  case 61: /* opt_not_exists: %empty  */
#line 727 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4102 "bison_parser.cpp"
    break;

  case 62: /* table_elem_commalist: table_elem  */
#line 729 "bison_parser.y"
                                  {
  (yyval.table_element_vec) = new std::vector<TableElement*>();
  (yyval.table_element_vec)->push_back((yyvsp[0].table_element_t));
}
#line 4111 "bison_parser.cpp"
    break;

  case 63: /* table_elem_commalist: table_elem_commalist ',' table_elem  */
#line 733 "bison_parser.y"
                                      {
  (yyvsp[-2].table_element_vec)->push_back((yyvsp[0].table_element_t));
  (yyval.table_element_vec) = (yyvsp[-2].table_element_vec);
}
#line 4120 "bison_parser.cpp"
    break;

  case 64: /* table_elem: column_def  */
#line 738 "bison_parser.y"
                        { (yyval.table_element_t) = (yyvsp[0].column_t); }
#line 4126 "bison_parser.cpp"
    break;

  case 65: /* table_elem: table_constraint  */
#line 739 "bison_parser.y"
                   { (yyval.table_element_t) = (yyvsp[0].table_constraint_t); }
#line 4132 "bison_parser.cpp"
    break;

  case 66: /* column_def: IDENTIFIER column_type opt_column_constraints  */
#line 741 "bison_parser.y"
                                                           {
  (yyval.column_t) = new ColumnDefinition((yyvsp[-2].sval), (yyvsp[-1].column_type_t), (yyvsp[0].column_constraints_t)->constraints, (yyvsp[0].column_constraints_t)->references);
  if (!(yyval.column_t)->trySetNullableExplicit()) {
    yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{(yyvsp[-2].sval)}).c_str());
  }
  delete (yyvsp[0].column_constraints_t);
}
#line 4144 "bison_parser.cpp"
    break;

  case 67: /* column_type: BIGINT  */
#line 749 "bison_parser.y"
                     { (yyval.column_type_t) = ColumnType{DataType::BIGINT}; }
#line 4150 "bison_parser.cpp"
    break;

  case 68: /* column_type: BOOLEAN  */
#line 750 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::BOOLEAN}; }
#line 4156 "bison_parser.cpp"
    break;

  case 69: /* column_type: CHAR '(' INTVAL ')'  */
#line 751 "bison_parser.y"
                      { (yyval.column_type_t) = ColumnType{DataType::CHAR, (yyvsp[-1].ival)}; }
#line 4162 "bison_parser.cpp"
    break;

  case 70: /* column_type: CHARACTER_VARYING '(' INTVAL ')'  */
#line 752 "bison_parser.y"
                                   { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4168 "bison_parser.cpp"
    break;

  case 71: /* column_type: DATE  */
#line 753 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::DATE}; }
#line 4174 "bison_parser.cpp"
    break;

  case 72: /* column_type: DATETIME  */
#line 754 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4180 "bison_parser.cpp"
    break;

  case 73: /* column_type: DECIMAL opt_decimal_specification  */
#line 755 "bison_parser.y"
                                    {
  (yyval.column_type_t) = ColumnType{DataType::DECIMAL, 0, (yyvsp[0].ival_pair)->first, (yyvsp[0].ival_pair)->second};
  delete (yyvsp[0].ival_pair);
}
#line 4189 "bison_parser.cpp"
    break;

  case 74: /* column_type: DOUBLE  */
#line 759 "bison_parser.y"
         { (yyval.column_type_t) = ColumnType{DataType::DOUBLE}; }
#line 4195 "bison_parser.cpp"
    break;

  case 75: /* column_type: FLOAT  */
#line 760 "bison_parser.y"
        { (yyval.column_type_t) = ColumnType{DataType::FLOAT}; }
#line 4201 "bison_parser.cpp"
    break;

  case 76: /* column_type: INT  */
#line 761 "bison_parser.y"
      { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4207 "bison_parser.cpp"
    break;

  case 77: /* column_type: INTEGER  */
#line 762 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4213 "bison_parser.cpp"
    break;

  case 78: /* column_type: LONG  */
#line 763 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::LONG}; }
#line 4219 "bison_parser.cpp"
    break;

  case 79: /* column_type: REAL  */
#line 764 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::REAL}; }
#line 4225 "bison_parser.cpp"
    break;

  case 80: /* column_type: SMALLINT  */
#line 765 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::SMALLINT}; }
#line 4231 "bison_parser.cpp"
    break;

  case 81: /* column_type: TEXT  */
#line 766 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::TEXT}; }
#line 4237 "bison_parser.cpp"
    break;

  case 82: /* column_type: TIME opt_time_precision  */
#line 767 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::TIME, 0, (yyvsp[0].ival)}; }
#line 4243 "bison_parser.cpp"
    break;

  case 83: /* column_type: TIMESTAMP  */
#line 768 "bison_parser.y"
            { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4249 "bison_parser.cpp"
    break;

  case 84: /* column_type: VARCHAR '(' INTVAL ')'  */
#line 769 "bison_parser.y"
                         { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4255 "bison_parser.cpp"
    break;

  case 85: /* opt_time_precision: '(' INTVAL ')'  */
#line 771 "bison_parser.y"
                                    { (yyval.ival) = (yyvsp[-1].ival); }
#line 4261 "bison_parser.cpp"
    break;

  case 86: /* opt_time_precision: %empty  */
#line 772 "bison_parser.y"
              { (yyval.ival) = 0; }
#line 4267 "bison_parser.cpp"
    break;

  case 87: /* opt_decimal_specification: '(' INTVAL ',' INTVAL ')'  */
#line 774 "bison_parser.y"
                                                      { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-3].ival), (yyvsp[-1].ival)}; }
#line 4273 "bison_parser.cpp"
    break;

  case 88: /* opt_decimal_specification: '(' INTVAL ')'  */
#line 775 "bison_parser.y"
                 { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-1].ival), 0}; }
#line 4279 "bison_parser.cpp"
    break;

  case 89: /* opt_decimal_specification: %empty  */
#line 776 "bison_parser.y"
              { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{0, 0}; }
#line 4285 "bison_parser.cpp"
    break;

  case 90: /* opt_column_constraints: column_constraints  */
#line 778 "bison_parser.y"
                                            { (yyval.column_constraints_t) = (yyvsp[0].column_constraints_t); }
#line 4291 "bison_parser.cpp"
    break;

  case 91: /* opt_column_constraints: %empty  */
#line 779 "bison_parser.y"
              { (yyval.column_constraints_t) = new ColumnConstraints(); }
#line 4297 "bison_parser.cpp"
    break;

  case 92: /* column_constraints: column_constraint  */
#line 781 "bison_parser.y"
                                       {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
}
#line 4306 "bison_parser.cpp"
    break;

  case 93: /* column_constraints: column_constraints column_constraint  */
#line 785 "bison_parser.y"
                                       {
  (yyvsp[-1].column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4315 "bison_parser.cpp"
    break;

  case 94: /* column_constraints: references_spec  */
#line 789 "bison_parser.y"
                  {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyval.column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
}
#line 4325 "bison_parser.cpp"
    break;

  case 95: /* column_constraints: column_constraints references_spec  */
#line 794 "bison_parser.y"
                                     {
  // Multiple foreign keys for the same column could be possible, so we do not raise an error in that case.
  // Think of foreign keys referenced on multiple levels (returned item references sold item references items).
  (yyvsp[-1].column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyvsp[-1].column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4337 "bison_parser.cpp"
    break;

  case 96: /* column_constraint: PRIMARY KEY  */
#line 802 "bison_parser.y"
                                { (yyval.column_constraint_t) = ConstraintType::PrimaryKey; }
#line 4343 "bison_parser.cpp"
    break;

  case 97: /* column_constraint: UNIQUE  */
#line 803 "bison_parser.y"
         { (yyval.column_constraint_t) = ConstraintType::Unique; }
#line 4349 "bison_parser.cpp"
    break;

  case 98: /* column_constraint: NULL  */
#line 804 "bison_parser.y"
       { (yyval.column_constraint_t) = ConstraintType::Null; }
#line 4355 "bison_parser.cpp"
    break;

  case 99: /* column_constraint: NOT NULL  */
#line 805 "bison_parser.y"
           { (yyval.column_constraint_t) = ConstraintType::NotNull; }
#line 4361 "bison_parser.cpp"
    break;

  case 100: /* table_constraint: PRIMARY KEY '(' ident_commalist ')'  */
#line 807 "bison_parser.y"
                                                       { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::PrimaryKey, (yyvsp[-1].str_vec)); }
#line 4367 "bison_parser.cpp"
    break;

  case 101: /* table_constraint: UNIQUE '(' ident_commalist ')'  */
#line 808 "bison_parser.y"
                                 { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::Unique, (yyvsp[-1].str_vec)); }
#line 4373 "bison_parser.cpp"
    break;

  case 102: /* table_constraint: FOREIGN KEY '(' ident_commalist ')' references_spec  */
#line 809 "bison_parser.y"
                                                      { (yyval.table_constraint_t) = new ForeignKeyConstraint((yyvsp[-2].str_vec), (yyvsp[0].references_spec_t)); }
#line 4379 "bison_parser.cpp"
    break;

  case 103: /* references_spec: REFERENCES table_name opt_column_list  */
#line 811 "bison_parser.y"
                                                        { (yyval.references_spec_t) = new ReferencesSpecification((yyvsp[-1].table_name).schema, (yyvsp[-1].table_name).name, (yyvsp[0].str_vec)); }
#line 4385 "bison_parser.cpp"
    break;

  case 104: /* drop_statement: DROP TABLE opt_exists table_name  */
#line 819 "bison_parser.y"
                                                  {
  (yyval.drop_stmt) = new DropStatement(kDropTable);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4396 "bison_parser.cpp"
    break;

  case 105: /* drop_statement: DROP VIEW opt_exists table_name  */
#line 825 "bison_parser.y"
                                  {
  (yyval.drop_stmt) = new DropStatement(kDropView);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4407 "bison_parser.cpp"
    break;

  case 106: /* drop_statement: DEALLOCATE PREPARE IDENTIFIER  */
#line 831 "bison_parser.y"
                                {
  (yyval.drop_stmt) = new DropStatement(kDropPreparedStatement);
  (yyval.drop_stmt)->ifExists = false;
  (yyval.drop_stmt)->name = (yyvsp[0].sval);
}
#line 4417 "bison_parser.cpp"
    break;

  case 107: /* drop_statement: DROP INDEX opt_exists IDENTIFIER  */
#line 837 "bison_parser.y"
                                   {
  (yyval.drop_stmt) = new DropStatement(kDropIndex);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->indexName = (yyvsp[0].sval);
}
#line 4427 "bison_parser.cpp"
    break;

  case 108: /* opt_exists: IF EXISTS  */
#line 843 "bison_parser.y"
                       { (yyval.bval) = true; }
#line 4433 "bison_parser.cpp"
    break;

  case 109: /* opt_exists: %empty  */
#line 844 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4439 "bison_parser.cpp"
    break;

  case 110: /* alter_statement: ALTER TABLE opt_exists table_name alter_action  */
#line 851 "bison_parser.y"
                                                                 {
  (yyval.alter_stmt) = new AlterStatement((yyvsp[-1].table_name).name, (yyvsp[0].alter_action_t));
  (yyval.alter_stmt)->ifTableExists = (yyvsp[-2].bval);
  (yyval.alter_stmt)->schema = (yyvsp[-1].table_name).schema;
}
#line 4449 "bison_parser.cpp"
    break;

  case 111: /* alter_action: drop_action  */
#line 857 "bison_parser.y"
                           { (yyval.alter_action_t) = (yyvsp[0].drop_action_t); }
#line 4455 "bison_parser.cpp"
    break;

  case 112: /* drop_action: DROP COLUMN opt_exists IDENTIFIER  */
#line 859 "bison_parser.y"
                                                {
  (yyval.drop_action_t) = new DropColumnAction((yyvsp[0].sval));
  (yyval.drop_action_t)->ifExists = (yyvsp[-1].bval);
}
#line 4464 "bison_parser.cpp"
    break;

  case 113: /* delete_statement: DELETE FROM table_name opt_where  */
#line 869 "bison_parser.y"
                                                    {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[-1].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[-1].table_name).name;
  (yyval.delete_stmt)->expr = (yyvsp[0].expr);
}
#line 4475 "bison_parser.cpp"
    break;

  case 114: /* truncate_statement: TRUNCATE table_name  */
#line 876 "bison_parser.y"
                                         {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 4485 "bison_parser.cpp"
    break;

  case 115: /* insert_statement: INSERT INTO table_name opt_column_list VALUES '(' extended_literal_list ')'  */
#line 887 "bison_parser.y"
                                                                                               {
  (yyval.insert_stmt) = new InsertStatement(kInsertValues);
  (yyval.insert_stmt)->schema = (yyvsp[-5].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-5].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-4].str_vec);
  (yyval.insert_stmt)->values = (yyvsp[-1].expr_vec);
}
#line 4497 "bison_parser.cpp"
    break;

  case 116: /* insert_statement: INSERT INTO table_name opt_column_list select_no_paren  */
#line 894 "bison_parser.y"
                                                         {
  (yyval.insert_stmt) = new InsertStatement(kInsertSelect);
  (yyval.insert_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-1].str_vec);
  (yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4509 "bison_parser.cpp"
    break;

  case 117: /* opt_column_list: '(' ident_commalist ')'  */
#line 902 "bison_parser.y"
                                          { (yyval.str_vec) = (yyvsp[-1].str_vec); }
#line 4515 "bison_parser.cpp"
    break;

  case 118: /* opt_column_list: %empty  */
#line 903 "bison_parser.y"
              { (yyval.str_vec) = nullptr; }
#line 4521 "bison_parser.cpp"
    break;

  case 119: /* update_statement: UPDATE table_ref_name_no_alias SET update_clause_commalist opt_where  */
#line 910 "bison_parser.y"
                                                                                        {
  (yyval.update_stmt) = new UpdateStatement();
  (yyval.update_stmt)->table = (yyvsp[-3].table);
  (yyval.update_stmt)->updates = (yyvsp[-1].update_vec);
  (yyval.update_stmt)->where = (yyvsp[0].expr);
}
#line 4532 "bison_parser.cpp"
    break;

  case 120: /* update_clause_commalist: update_clause  */
#line 917 "bison_parser.y"
                                        {
  (yyval.update_vec) = new std::vector<UpdateClause*>();
  (yyval.update_vec)->push_back((yyvsp[0].update_t));
}
#line 4541 "bison_parser.cpp"
    break;

  case 121: /* update_clause_commalist: update_clause_commalist ',' update_clause  */
#line 921 "bison_parser.y"
                                            {
  (yyvsp[-2].update_vec)->push_back((yyvsp[0].update_t));
  (yyval.update_vec) = (yyvsp[-2].update_vec);
}
#line 4550 "bison_parser.cpp"
    break;

  case 122: /* update_clause: IDENTIFIER '=' expr  */
#line 926 "bison_parser.y"
                                    {
  (yyval.update_t) = new UpdateClause();
  (yyval.update_t)->column = (yyvsp[-2].sval);
  (yyval.update_t)->value = (yyvsp[0].expr);
}
#line 4560 "bison_parser.cpp"
    break;

  case 123: /* select_statement: opt_with_clause select_with_paren  */
#line 936 "bison_parser.y"
                                                     {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4569 "bison_parser.cpp"
    break;

  case 124: /* select_statement: opt_with_clause select_no_paren  */
#line 940 "bison_parser.y"
                                  {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4578 "bison_parser.cpp"
    break;

  case 125: /* select_statement: opt_with_clause select_with_paren set_operator select_within_set_operation opt_order opt_limit  */
#line 944 "bison_parser.y"
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
#line 4594 "bison_parser.cpp"
    break;

  case 128: /* select_within_set_operation_no_parentheses: select_clause  */
#line 958 "bison_parser.y"
                                                           { (yyval.select_stmt) = (yyvsp[0].select_stmt); }
#line 4600 "bison_parser.cpp"
    break;

  case 129: /* select_within_set_operation_no_parentheses: select_clause set_operator select_within_set_operation  */
#line 959 "bison_parser.y"
                                                         {
  (yyval.select_stmt) = (yyvsp[-2].select_stmt);
  if ((yyval.select_stmt)->setOperations == nullptr) {
    (yyval.select_stmt)->setOperations = new std::vector<SetOperation*>();
  }
  (yyval.select_stmt)->setOperations->push_back((yyvsp[-1].set_operator_t));
  (yyval.select_stmt)->setOperations->back()->nestedSelectStatement = (yyvsp[0].select_stmt);
}
#line 4613 "bison_parser.cpp"
    break;

  case 130: /* select_with_paren: '(' select_no_paren ')'  */
#line 968 "bison_parser.y"
                                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4619 "bison_parser.cpp"
    break;

  case 131: /* select_with_paren: '(' select_with_paren ')'  */
#line 969 "bison_parser.y"
                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4625 "bison_parser.cpp"
    break;

  case 132: /* select_no_paren: select_clause opt_order opt_limit opt_locking_clause  */
#line 971 "bison_parser.y"
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
#line 4644 "bison_parser.cpp"
    break;

  case 133: /* select_no_paren: select_clause set_operator select_within_set_operation opt_order opt_limit opt_locking_clause  */
#line 985 "bison_parser.y"
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
#line 4660 "bison_parser.cpp"
    break;

  case 134: /* set_operator: set_type opt_all  */
#line 997 "bison_parser.y"
                                {
  (yyval.set_operator_t) = (yyvsp[-1].set_operator_t);
  (yyval.set_operator_t)->isAll = (yyvsp[0].bval);
}
#line 4669 "bison_parser.cpp"
    break;

  case 135: /* set_type: UNION  */
#line 1002 "bison_parser.y"
                 {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetUnion;
}
#line 4678 "bison_parser.cpp"
    break;

  case 136: /* set_type: INTERSECT  */
#line 1006 "bison_parser.y"
            {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetIntersect;
}
#line 4687 "bison_parser.cpp"
    break;

  case 137: /* set_type: EXCEPT  */
#line 1010 "bison_parser.y"
         {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetExcept;
}
#line 4696 "bison_parser.cpp"
    break;

  case 138: /* opt_all: ALL  */
#line 1015 "bison_parser.y"
              { (yyval.bval) = true; }
#line 4702 "bison_parser.cpp"
    break;

  case 139: /* opt_all: %empty  */
#line 1016 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4708 "bison_parser.cpp"
    break;

  case 140: /* select_clause: SELECT opt_top opt_distinct select_list opt_from_clause opt_where opt_group  */
#line 1018 "bison_parser.y"
                                                                                            {
  (yyval.select_stmt) = new SelectStatement();
  (yyval.select_stmt)->limit = (yyvsp[-5].limit);
  (yyval.select_stmt)->selectDistinct = (yyvsp[-4].bval);
  (yyval.select_stmt)->selectList = (yyvsp[-3].expr_vec);
  (yyval.select_stmt)->fromTable = (yyvsp[-2].table);
  (yyval.select_stmt)->whereClause = (yyvsp[-1].expr);
  (yyval.select_stmt)->groupBy = (yyvsp[0].group_t);
}
#line 4722 "bison_parser.cpp"
    break;

  case 141: /* opt_distinct: DISTINCT  */
#line 1028 "bison_parser.y"
                        { (yyval.bval) = true; }
#line 4728 "bison_parser.cpp"
    break;

  case 142: /* opt_distinct: %empty  */
#line 1029 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4734 "bison_parser.cpp"
    break;

  case 144: /* opt_from_clause: from_clause  */
#line 1033 "bison_parser.y"
                              { (yyval.table) = (yyvsp[0].table); }
#line 4740 "bison_parser.cpp"
    break;

  case 145: /* opt_from_clause: %empty  */
#line 1034 "bison_parser.y"
              { (yyval.table) = nullptr; }
#line 4746 "bison_parser.cpp"
    break;

  case 146: /* from_clause: FROM table_ref  */
#line 1036 "bison_parser.y"
                             { (yyval.table) = (yyvsp[0].table); }
#line 4752 "bison_parser.cpp"
    break;

  case 147: /* opt_where: WHERE expr  */
#line 1038 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 4758 "bison_parser.cpp"
    break;

  case 148: /* opt_where: %empty  */
#line 1039 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4764 "bison_parser.cpp"
    break;

  case 149: /* opt_group: GROUP BY expr_list opt_having  */
#line 1041 "bison_parser.y"
                                          {
  (yyval.group_t) = new GroupByDescription();
  (yyval.group_t)->columns = (yyvsp[-1].expr_vec);
  (yyval.group_t)->having = (yyvsp[0].expr);
}
#line 4774 "bison_parser.cpp"
    break;

  case 150: /* opt_group: %empty  */
#line 1046 "bison_parser.y"
              { (yyval.group_t) = nullptr; }
#line 4780 "bison_parser.cpp"
    break;

  case 151: /* opt_having: HAVING expr  */
#line 1048 "bison_parser.y"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 4786 "bison_parser.cpp"
    break;

  case 152: /* opt_having: %empty  */
#line 1049 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4792 "bison_parser.cpp"
    break;

  case 153: /* opt_order: ORDER BY order_list  */
#line 1051 "bison_parser.y"
                                { (yyval.order_vec) = (yyvsp[0].order_vec); }
#line 4798 "bison_parser.cpp"
    break;

  case 154: /* opt_order: %empty  */
#line 1052 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 4804 "bison_parser.cpp"
    break;

  case 155: /* order_list: order_desc  */
#line 1054 "bison_parser.y"
                        {
  (yyval.order_vec) = new std::vector<OrderDescription*>();
  (yyval.order_vec)->push_back((yyvsp[0].order));
}
#line 4813 "bison_parser.cpp"
    break;

  case 156: /* order_list: order_list ',' order_desc  */
#line 1058 "bison_parser.y"
                            {
  (yyvsp[-2].order_vec)->push_back((yyvsp[0].order));
  (yyval.order_vec) = (yyvsp[-2].order_vec);
}
#line 4822 "bison_parser.cpp"
    break;

  case 157: /* order_desc: expr opt_order_type opt_null_ordering  */
#line 1063 "bison_parser.y"
                                                   { (yyval.order) = new OrderDescription((yyvsp[-1].order_type), (yyvsp[-2].expr), (yyvsp[0].null_ordering_t)); }
#line 4828 "bison_parser.cpp"
    break;

  case 158: /* opt_order_type: ASC  */
#line 1065 "bison_parser.y"
                     { (yyval.order_type) = kOrderAsc; }
#line 4834 "bison_parser.cpp"
    break;

  case 159: /* opt_order_type: DESC  */
#line 1066 "bison_parser.y"
       { (yyval.order_type) = kOrderDesc; }
#line 4840 "bison_parser.cpp"
    break;

  case 160: /* opt_order_type: %empty  */
#line 1067 "bison_parser.y"
              { (yyval.order_type) = kOrderAsc; }
#line 4846 "bison_parser.cpp"
    break;

  case 161: /* opt_null_ordering: %empty  */
#line 1069 "bison_parser.y"
                                { (yyval.null_ordering_t) = NullOrdering::Undefined; }
#line 4852 "bison_parser.cpp"
    break;

  case 162: /* opt_null_ordering: IDENTIFIER IDENTIFIER  */
#line 1070 "bison_parser.y"
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
#line 4876 "bison_parser.cpp"
    break;

  case 163: /* opt_top: TOP int_literal  */
#line 1092 "bison_parser.y"
                          { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 4882 "bison_parser.cpp"
    break;

  case 164: /* opt_top: TOP '(' int_literal ')'  */
#line 1093 "bison_parser.y"
                          { (yyval.limit) = new LimitDescription((yyvsp[-1].expr), nullptr); }
#line 4888 "bison_parser.cpp"
    break;

  case 165: /* opt_top: %empty  */
#line 1094 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 4894 "bison_parser.cpp"
    break;

  case 166: /* opt_limit: LIMIT expr  */
#line 1096 "bison_parser.y"
                       { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 4900 "bison_parser.cpp"
    break;

  case 167: /* opt_limit: OFFSET expr  */
#line 1097 "bison_parser.y"
              { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 4906 "bison_parser.cpp"
    break;

  case 168: /* opt_limit: LIMIT expr OFFSET expr  */
#line 1098 "bison_parser.y"
                         { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 4912 "bison_parser.cpp"
    break;

  case 169: /* opt_limit: LIMIT ALL  */
#line 1099 "bison_parser.y"
            { (yyval.limit) = new LimitDescription(nullptr, nullptr); }
#line 4918 "bison_parser.cpp"
    break;

  case 170: /* opt_limit: LIMIT ALL OFFSET expr  */
#line 1100 "bison_parser.y"
                        { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 4924 "bison_parser.cpp"
    break;

  case 171: /* opt_limit: OFFSET expr ROWS FETCH NEXT expr ROWS ONLY  */
#line 1101 "bison_parser.y"
                                             { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[-6].expr)); }
#line 4930 "bison_parser.cpp"
    break;

  case 172: /* opt_limit: %empty  */
#line 1102 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 4936 "bison_parser.cpp"
    break;

  case 173: /* expr_list: expr_alias  */
#line 1107 "bison_parser.y"
                       {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 4945 "bison_parser.cpp"
    break;

  case 174: /* expr_list: expr_list ',' expr_alias  */
#line 1111 "bison_parser.y"
                           {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 4954 "bison_parser.cpp"
    break;

  case 175: /* opt_extended_literal_list: extended_literal_list  */
#line 1117 "bison_parser.y"
                                                  { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 4960 "bison_parser.cpp"
    break;

  case 176: /* opt_extended_literal_list: %empty  */
#line 1118 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 4966 "bison_parser.cpp"
    break;

  case 177: /* extended_literal_list: casted_extended_literal  */
#line 1120 "bison_parser.y"
                                                {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 4975 "bison_parser.cpp"
    break;

  case 178: /* extended_literal_list: extended_literal_list ',' casted_extended_literal  */
#line 1124 "bison_parser.y"
                                                    {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 4984 "bison_parser.cpp"
    break;

  case 180: /* casted_extended_literal: CAST '(' extended_literal AS column_type ')'  */
#line 1129 "bison_parser.y"
                                                                                          {
  (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t));
}
#line 4992 "bison_parser.cpp"
    break;

  case 181: /* extended_literal: literal  */
#line 1133 "bison_parser.y"
                           { (yyval.expr) = (yyvsp[0].expr); }
#line 4998 "bison_parser.cpp"
    break;

  case 182: /* extended_literal: '-' num_literal  */
#line 1134 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5004 "bison_parser.cpp"
    break;

  case 183: /* extended_literal: '-' interval_literal  */
#line 1135 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5010 "bison_parser.cpp"
    break;

  case 184: /* expr_alias: expr opt_alias  */
#line 1137 "bison_parser.y"
                            {
  (yyval.expr) = (yyvsp[-1].expr);
  if ((yyvsp[0].alias_t)) {
    (yyval.expr)->alias = (yyvsp[0].alias_t)->name;
    (yyvsp[0].alias_t)->name = nullptr;
    delete (yyvsp[0].alias_t);
  }
}
#line 5023 "bison_parser.cpp"
    break;

  case 190: /* operand: '(' expr ')'  */
#line 1148 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[-1].expr); }
#line 5029 "bison_parser.cpp"
    break;

  case 200: /* operand: '(' select_no_paren ')'  */
#line 1150 "bison_parser.y"
                                         {
  (yyval.expr) = Expr::makeSelect((yyvsp[-1].select_stmt));
}
#line 5037 "bison_parser.cpp"
    break;

  case 203: /* unary_expr: '-' operand  */
#line 1156 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5043 "bison_parser.cpp"
    break;

  case 204: /* unary_expr: NOT operand  */
#line 1157 "bison_parser.y"
              { (yyval.expr) = Expr::makeOpUnary(kOpNot, (yyvsp[0].expr)); }
#line 5049 "bison_parser.cpp"
    break;

  case 205: /* unary_expr: operand ISNULL  */
#line 1158 "bison_parser.y"
                 { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-1].expr)); }
#line 5055 "bison_parser.cpp"
    break;

  case 206: /* unary_expr: operand IS NULL  */
#line 1159 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-2].expr)); }
#line 5061 "bison_parser.cpp"
    break;

  case 207: /* unary_expr: operand IS NOT NULL  */
#line 1160 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeOpUnary(kOpIsNull, (yyvsp[-3].expr))); }
#line 5067 "bison_parser.cpp"
    break;

  case 209: /* binary_expr: operand '-' operand  */
#line 1162 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpMinus, (yyvsp[0].expr)); }
#line 5073 "bison_parser.cpp"
    break;

  case 210: /* binary_expr: operand '+' operand  */
#line 1163 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPlus, (yyvsp[0].expr)); }
#line 5079 "bison_parser.cpp"
    break;

  case 211: /* binary_expr: operand '/' operand  */
#line 1164 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpSlash, (yyvsp[0].expr)); }
#line 5085 "bison_parser.cpp"
    break;

  case 212: /* binary_expr: operand '*' operand  */
#line 1165 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAsterisk, (yyvsp[0].expr)); }
#line 5091 "bison_parser.cpp"
    break;

  case 213: /* binary_expr: operand '%' operand  */
#line 1166 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPercentage, (yyvsp[0].expr)); }
#line 5097 "bison_parser.cpp"
    break;

  case 214: /* binary_expr: operand '^' operand  */
#line 1167 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpCaret, (yyvsp[0].expr)); }
#line 5103 "bison_parser.cpp"
    break;

  case 215: /* binary_expr: operand LIKE operand  */
#line 1168 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLike, (yyvsp[0].expr)); }
#line 5109 "bison_parser.cpp"
    break;

  case 216: /* binary_expr: operand NOT LIKE operand  */
#line 1169 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-3].expr), kOpNotLike, (yyvsp[0].expr)); }
#line 5115 "bison_parser.cpp"
    break;

  case 217: /* binary_expr: operand ILIKE operand  */
#line 1170 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpILike, (yyvsp[0].expr)); }
#line 5121 "bison_parser.cpp"
    break;

  case 218: /* binary_expr: operand CONCAT operand  */
#line 1171 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpConcat, (yyvsp[0].expr)); }
#line 5127 "bison_parser.cpp"
    break;

  case 219: /* logic_expr: expr AND expr  */
#line 1173 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAnd, (yyvsp[0].expr)); }
#line 5133 "bison_parser.cpp"
    break;

  case 220: /* logic_expr: expr OR expr  */
#line 1174 "bison_parser.y"
               { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpOr, (yyvsp[0].expr)); }
#line 5139 "bison_parser.cpp"
    break;

  case 221: /* in_expr: operand IN '(' expr_list ')'  */
#line 1176 "bison_parser.y"
                                       { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].expr_vec)); }
#line 5145 "bison_parser.cpp"
    break;

  case 222: /* in_expr: operand NOT IN '(' expr_list ')'  */
#line 1177 "bison_parser.y"
                                   { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].expr_vec))); }
#line 5151 "bison_parser.cpp"
    break;

  case 223: /* in_expr: operand IN '(' select_no_paren ')'  */
#line 1178 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].select_stmt)); }
#line 5157 "bison_parser.cpp"
    break;

  case 224: /* in_expr: operand NOT IN '(' select_no_paren ')'  */
#line 1179 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].select_stmt))); }
#line 5163 "bison_parser.cpp"
    break;

  case 225: /* case_expr: CASE expr case_list END  */
#line 1183 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-2].expr), (yyvsp[-1].expr), nullptr); }
#line 5169 "bison_parser.cpp"
    break;

  case 226: /* case_expr: CASE expr case_list ELSE expr END  */
#line 1184 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-4].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5175 "bison_parser.cpp"
    break;

  case 227: /* case_expr: CASE case_list END  */
#line 1185 "bison_parser.y"
                     { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-1].expr), nullptr); }
#line 5181 "bison_parser.cpp"
    break;

  case 228: /* case_expr: CASE case_list ELSE expr END  */
#line 1186 "bison_parser.y"
                               { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5187 "bison_parser.cpp"
    break;

  case 229: /* case_list: WHEN expr THEN expr  */
#line 1188 "bison_parser.y"
                                { (yyval.expr) = Expr::makeCaseList(Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5193 "bison_parser.cpp"
    break;

  case 230: /* case_list: case_list WHEN expr THEN expr  */
#line 1189 "bison_parser.y"
                                { (yyval.expr) = Expr::caseListAppend((yyvsp[-4].expr), Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5199 "bison_parser.cpp"
    break;

  case 231: /* exists_expr: EXISTS '(' select_no_paren ')'  */
#line 1191 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeExists((yyvsp[-1].select_stmt)); }
#line 5205 "bison_parser.cpp"
    break;

  case 232: /* exists_expr: NOT EXISTS '(' select_no_paren ')'  */
#line 1192 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeExists((yyvsp[-1].select_stmt))); }
#line 5211 "bison_parser.cpp"
    break;

  case 233: /* comp_expr: operand '=' operand  */
#line 1194 "bison_parser.y"
                                { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5217 "bison_parser.cpp"
    break;

  case 234: /* comp_expr: operand EQUALS operand  */
#line 1195 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5223 "bison_parser.cpp"
    break;

  case 235: /* comp_expr: operand NOTEQUALS operand  */
#line 1196 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpNotEquals, (yyvsp[0].expr)); }
#line 5229 "bison_parser.cpp"
    break;

  case 236: /* comp_expr: operand '<' operand  */
#line 1197 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLess, (yyvsp[0].expr)); }
#line 5235 "bison_parser.cpp"
    break;

  case 237: /* comp_expr: operand '>' operand  */
#line 1198 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreater, (yyvsp[0].expr)); }
#line 5241 "bison_parser.cpp"
    break;

  case 238: /* comp_expr: operand LESSEQ operand  */
#line 1199 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLessEq, (yyvsp[0].expr)); }
#line 5247 "bison_parser.cpp"
    break;

  case 239: /* comp_expr: operand GREATEREQ operand  */
#line 1200 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreaterEq, (yyvsp[0].expr)); }
#line 5253 "bison_parser.cpp"
    break;

  case 240: /* function_expr: IDENTIFIER '(' ')' opt_window  */
#line 1204 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 5259 "bison_parser.cpp"
    break;

  case 241: /* function_expr: IDENTIFIER '(' opt_distinct expr_list ')' opt_window  */
#line 1205 "bison_parser.y"
                                                       { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description)); }
#line 5265 "bison_parser.cpp"
    break;

  case 242: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' ')' opt_window  */
#line 1206 "bison_parser.y"
                                               {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), (yyvsp[-5].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description));
}
#line 5273 "bison_parser.cpp"
    break;

  case 243: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' opt_distinct expr_list ')' opt_window  */
#line 1209 "bison_parser.y"
                                                                      {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-7].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description));
}
#line 5281 "bison_parser.cpp"
    break;

  case 244: /* function_expr: nonreserved_keyword '(' ')' opt_window  */
#line 1214 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 5287 "bison_parser.cpp"
    break;

  case 245: /* function_expr: nonreserved_keyword '(' opt_distinct expr_list ')' opt_window  */
#line 1215 "bison_parser.y"
                                                                { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description)); }
#line 5293 "bison_parser.cpp"
    break;

  case 246: /* opt_window: OVER '(' opt_partition opt_order opt_frame_clause ')'  */
#line 1219 "bison_parser.y"
                                                                   { (yyval.window_description) = new WindowDescription((yyvsp[-3].expr_vec), (yyvsp[-2].order_vec), (yyvsp[-1].frame_description)); }
#line 5299 "bison_parser.cpp"
    break;

  case 247: /* opt_window: %empty  */
#line 1220 "bison_parser.y"
              { (yyval.window_description) = nullptr; }
#line 5305 "bison_parser.cpp"
    break;

  case 248: /* opt_partition: PARTITION BY expr_list  */
#line 1222 "bison_parser.y"
                                       { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5311 "bison_parser.cpp"
    break;

  case 249: /* opt_partition: %empty  */
#line 1223 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5317 "bison_parser.cpp"
    break;

  case 250: /* opt_frame_clause: frame_type frame_bound  */
#line 1228 "bison_parser.y"
                                          { (yyval.frame_description) = new FrameDescription{(yyvsp[-1].frame_type), (yyvsp[0].frame_bound), new FrameBound{0, kCurrentRow, false}}; }
#line 5323 "bison_parser.cpp"
    break;

  case 251: /* opt_frame_clause: frame_type BETWEEN frame_bound AND frame_bound  */
#line 1229 "bison_parser.y"
                                                 { (yyval.frame_description) = new FrameDescription{(yyvsp[-4].frame_type), (yyvsp[-2].frame_bound), (yyvsp[0].frame_bound)}; }
#line 5329 "bison_parser.cpp"
    break;

  case 252: /* opt_frame_clause: %empty  */
#line 1230 "bison_parser.y"
              {
  (yyval.frame_description) = new FrameDescription{kRange, new FrameBound{0, kPreceding, true}, new FrameBound{0, kCurrentRow, false}};
}
#line 5337 "bison_parser.cpp"
    break;

  case 253: /* frame_type: RANGE  */
#line 1234 "bison_parser.y"
                   { (yyval.frame_type) = kRange; }
#line 5343 "bison_parser.cpp"
    break;

  case 254: /* frame_type: ROWS  */
#line 1235 "bison_parser.y"
       { (yyval.frame_type) = kRows; }
#line 5349 "bison_parser.cpp"
    break;

  case 255: /* frame_type: GROUPS  */
#line 1236 "bison_parser.y"
         { (yyval.frame_type) = kGroups; }
#line 5355 "bison_parser.cpp"
    break;

  case 256: /* frame_bound: UNBOUNDED PRECEDING  */
#line 1238 "bison_parser.y"
                                  { (yyval.frame_bound) = new FrameBound{0, kPreceding, true}; }
#line 5361 "bison_parser.cpp"
    break;

  case 257: /* frame_bound: INTVAL PRECEDING  */
#line 1239 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kPreceding, false}; }
#line 5367 "bison_parser.cpp"
    break;

  case 258: /* frame_bound: UNBOUNDED FOLLOWING  */
#line 1240 "bison_parser.y"
                      { (yyval.frame_bound) = new FrameBound{0, kFollowing, true}; }
#line 5373 "bison_parser.cpp"
    break;

  case 259: /* frame_bound: INTVAL FOLLOWING  */
#line 1241 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kFollowing, false}; }
#line 5379 "bison_parser.cpp"
    break;

  case 260: /* frame_bound: CURRENT_ROW  */
#line 1242 "bison_parser.y"
              { (yyval.frame_bound) = new FrameBound{0, kCurrentRow, false}; }
#line 5385 "bison_parser.cpp"
    break;

  case 261: /* extract_expr: EXTRACT '(' datetime_field FROM expr ')'  */
#line 1244 "bison_parser.y"
                                                        { (yyval.expr) = Expr::makeExtract((yyvsp[-3].datetime_field), (yyvsp[-1].expr)); }
#line 5391 "bison_parser.cpp"
    break;

  case 262: /* cast_expr: CAST '(' expr AS column_type ')'  */
#line 1246 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t)); }
#line 5397 "bison_parser.cpp"
    break;

  case 263: /* datetime_field: SECOND  */
#line 1248 "bison_parser.y"
                        { (yyval.datetime_field) = kDatetimeSecond; }
#line 5403 "bison_parser.cpp"
    break;

  case 264: /* datetime_field: MINUTE  */
#line 1249 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMinute; }
#line 5409 "bison_parser.cpp"
    break;

  case 265: /* datetime_field: HOUR  */
#line 1250 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeHour; }
#line 5415 "bison_parser.cpp"
    break;

  case 266: /* datetime_field: DAY  */
#line 1251 "bison_parser.y"
      { (yyval.datetime_field) = kDatetimeDay; }
#line 5421 "bison_parser.cpp"
    break;

  case 267: /* datetime_field: MONTH  */
#line 1252 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeMonth; }
#line 5427 "bison_parser.cpp"
    break;

  case 268: /* datetime_field: YEAR  */
#line 1253 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeYear; }
#line 5433 "bison_parser.cpp"
    break;

  case 269: /* datetime_field_plural: SECONDS  */
#line 1255 "bison_parser.y"
                                { (yyval.datetime_field) = kDatetimeSecond; }
#line 5439 "bison_parser.cpp"
    break;

  case 270: /* datetime_field_plural: MINUTES  */
#line 1256 "bison_parser.y"
          { (yyval.datetime_field) = kDatetimeMinute; }
#line 5445 "bison_parser.cpp"
    break;

  case 271: /* datetime_field_plural: HOURS  */
#line 1257 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeHour; }
#line 5451 "bison_parser.cpp"
    break;

  case 272: /* datetime_field_plural: DAYS  */
#line 1258 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeDay; }
#line 5457 "bison_parser.cpp"
    break;

  case 273: /* datetime_field_plural: MONTHS  */
#line 1259 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMonth; }
#line 5463 "bison_parser.cpp"
    break;

  case 274: /* datetime_field_plural: YEARS  */
#line 1260 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeYear; }
#line 5469 "bison_parser.cpp"
    break;

  case 277: /* array_expr: ARRAY '[' expr_list ']'  */
#line 1264 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeArray((yyvsp[-1].expr_vec)); }
#line 5475 "bison_parser.cpp"
    break;

  case 278: /* array_index: operand '[' int_literal ']'  */
#line 1270 "bison_parser.y"
                                          {
  (yyval.expr) = Expr::makeArrayIndex((yyvsp[-3].expr), (yyvsp[-1].expr)->ival);
  delete (yyvsp[-1].expr);
}
#line 5484 "bison_parser.cpp"
    break;

  case 279: /* between_expr: operand BETWEEN operand AND operand  */
#line 1275 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeBetween((yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 5490 "bison_parser.cpp"
    break;

  case 280: /* column_name: IDENTIFIER  */
#line 1277 "bison_parser.y"
                         { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 5496 "bison_parser.cpp"
    break;

  case 281: /* column_name: IDENTIFIER '.' IDENTIFIER  */
#line 1278 "bison_parser.y"
                            { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 5502 "bison_parser.cpp"
    break;

  case 282: /* column_name: '*'  */
#line 1279 "bison_parser.y"
      { (yyval.expr) = Expr::makeStar(); }
#line 5508 "bison_parser.cpp"
    break;

  case 283: /* column_name: IDENTIFIER '.' '*'  */
#line 1280 "bison_parser.y"
                     { (yyval.expr) = Expr::makeStar((yyvsp[-2].sval)); }
#line 5514 "bison_parser.cpp"
    break;

  case 284: /* column_name: nonreserved_keyword  */
#line 1281 "bison_parser.y"
                      { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 5520 "bison_parser.cpp"
    break;

  case 285: /* nonreserved_keyword: SECOND  */
#line 1289 "bison_parser.y"
                             { (yyval.sval) = strdup("SECOND"); }
#line 5526 "bison_parser.cpp"
    break;

  case 286: /* nonreserved_keyword: MINUTE  */
#line 1290 "bison_parser.y"
         { (yyval.sval) = strdup("MINUTE"); }
#line 5532 "bison_parser.cpp"
    break;

  case 287: /* nonreserved_keyword: HOUR  */
#line 1291 "bison_parser.y"
       { (yyval.sval) = strdup("HOUR"); }
#line 5538 "bison_parser.cpp"
    break;

  case 288: /* nonreserved_keyword: DAY  */
#line 1292 "bison_parser.y"
      { (yyval.sval) = strdup("DAY"); }
#line 5544 "bison_parser.cpp"
    break;

  case 289: /* nonreserved_keyword: MONTH  */
#line 1293 "bison_parser.y"
        { (yyval.sval) = strdup("MONTH"); }
#line 5550 "bison_parser.cpp"
    break;

  case 290: /* nonreserved_keyword: YEAR  */
#line 1294 "bison_parser.y"
       { (yyval.sval) = strdup("YEAR"); }
#line 5556 "bison_parser.cpp"
    break;

  case 291: /* nonreserved_keyword: SECONDS  */
#line 1295 "bison_parser.y"
          { (yyval.sval) = strdup("SECONDS"); }
#line 5562 "bison_parser.cpp"
    break;

  case 292: /* nonreserved_keyword: MINUTES  */
#line 1296 "bison_parser.y"
          { (yyval.sval) = strdup("MINUTES"); }
#line 5568 "bison_parser.cpp"
    break;

  case 293: /* nonreserved_keyword: HOURS  */
#line 1297 "bison_parser.y"
        { (yyval.sval) = strdup("HOURS"); }
#line 5574 "bison_parser.cpp"
    break;

  case 294: /* nonreserved_keyword: DAYS  */
#line 1298 "bison_parser.y"
       { (yyval.sval) = strdup("DAYS"); }
#line 5580 "bison_parser.cpp"
    break;

  case 295: /* nonreserved_keyword: MONTHS  */
#line 1299 "bison_parser.y"
         { (yyval.sval) = strdup("MONTHS"); }
#line 5586 "bison_parser.cpp"
    break;

  case 296: /* nonreserved_keyword: YEARS  */
#line 1300 "bison_parser.y"
        { (yyval.sval) = strdup("YEARS"); }
#line 5592 "bison_parser.cpp"
    break;

  case 297: /* nonreserved_keyword: ISNULL  */
#line 1301 "bison_parser.y"
         { (yyval.sval) = strdup("ISNULL"); }
#line 5598 "bison_parser.cpp"
    break;

  case 298: /* nonreserved_keyword: FORMAT  */
#line 1302 "bison_parser.y"
         { (yyval.sval) = strdup("FORMAT"); }
#line 5604 "bison_parser.cpp"
    break;

  case 299: /* nonreserved_keyword: CHAR  */
#line 1303 "bison_parser.y"
       { (yyval.sval) = strdup("CHAR"); }
#line 5610 "bison_parser.cpp"
    break;

  case 300: /* nonreserved_keyword: VARCHAR  */
#line 1304 "bison_parser.y"
          { (yyval.sval) = strdup("VARCHAR"); }
#line 5616 "bison_parser.cpp"
    break;

  case 301: /* nonreserved_keyword: INT  */
#line 1305 "bison_parser.y"
      { (yyval.sval) = strdup("INT"); }
#line 5622 "bison_parser.cpp"
    break;

  case 302: /* nonreserved_keyword: INTEGER  */
#line 1306 "bison_parser.y"
          { (yyval.sval) = strdup("INTEGER"); }
#line 5628 "bison_parser.cpp"
    break;

  case 303: /* nonreserved_keyword: DATETIME  */
#line 1307 "bison_parser.y"
           { (yyval.sval) = strdup("DATETIME"); }
#line 5634 "bison_parser.cpp"
    break;

  case 304: /* nonreserved_keyword: TIMESTAMP  */
#line 1308 "bison_parser.y"
            { (yyval.sval) = strdup("TIMESTAMP"); }
#line 5640 "bison_parser.cpp"
    break;

  case 312: /* string_literal: STRING  */
#line 1312 "bison_parser.y"
                        { (yyval.expr) = Expr::makeLiteral((yyvsp[0].sval)); }
#line 5646 "bison_parser.cpp"
    break;

  case 313: /* bool_literal: TRUE  */
#line 1314 "bison_parser.y"
                    { (yyval.expr) = Expr::makeLiteral(true); }
#line 5652 "bison_parser.cpp"
    break;

  case 314: /* bool_literal: FALSE  */
#line 1315 "bison_parser.y"
        { (yyval.expr) = Expr::makeLiteral(false); }
#line 5658 "bison_parser.cpp"
    break;

  case 315: /* num_literal: FLOATVAL  */
#line 1317 "bison_parser.y"
                       { (yyval.expr) = Expr::makeLiteral((yyvsp[0].fval)); }
#line 5664 "bison_parser.cpp"
    break;

  case 317: /* int_literal: INTVAL  */
#line 1320 "bison_parser.y"
                     { (yyval.expr) = Expr::makeLiteral((yyvsp[0].ival)); }
#line 5670 "bison_parser.cpp"
    break;

  case 318: /* null_literal: NULL  */
#line 1322 "bison_parser.y"
                    { (yyval.expr) = Expr::makeNullLiteral(); }
#line 5676 "bison_parser.cpp"
    break;

  case 319: /* date_literal: DATE STRING  */
#line 1324 "bison_parser.y"
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
#line 5691 "bison_parser.cpp"
    break;

  case 320: /* interval_literal: INTVAL duration_field  */
#line 1335 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeIntervalLiteral((yyvsp[-1].ival), (yyvsp[0].datetime_field)); }
#line 5697 "bison_parser.cpp"
    break;

  case 321: /* interval_literal: INTERVAL STRING datetime_field  */
#line 1336 "bison_parser.y"
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
#line 5713 "bison_parser.cpp"
    break;

  case 322: /* interval_literal: INTERVAL STRING  */
#line 1347 "bison_parser.y"
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
#line 5749 "bison_parser.cpp"
    break;

  case 323: /* param_expr: '?'  */
#line 1379 "bison_parser.y"
                 {
  (yyval.expr) = Expr::makeParameter(yylloc.total_column);
  (yyval.expr)->ival2 = yylloc.total_column - 1;  // source column (0-based) of the '?' token
  yyloc.param_list.push_back((yyval.expr));
}
#line 5759 "bison_parser.cpp"
    break;

  case 324: /* param_expr: DOLLAR_PARAM  */
#line 1384 "bison_parser.y"
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
#line 5776 "bison_parser.cpp"
    break;

  case 325: /* param_expr: NAMED_PARAM  */
#line 1396 "bison_parser.y"
              {
  (yyval.expr) = Expr::makeNamedParameter((yyvsp[0].sval));
  (yyval.expr)->ival2 = yylloc.total_column - 1 - (int64_t)strlen((yyvsp[0].sval));
  yyloc.param_list.push_back((yyval.expr));
}
#line 5786 "bison_parser.cpp"
    break;

  case 327: /* table_ref: table_ref_commalist ',' table_ref_atomic  */
#line 1405 "bison_parser.y"
                                                                        {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  auto tbl = new TableRef(kTableCrossProduct);
  tbl->list = (yyvsp[-2].table_vec);
  (yyval.table) = tbl;
}
#line 5797 "bison_parser.cpp"
    break;

  case 331: /* nonjoin_table_ref_atomic: '(' select_statement ')' opt_table_alias  */
#line 1414 "bison_parser.y"
                                                                                     {
  auto tbl = new TableRef(kTableSelect);
  tbl->select = (yyvsp[-2].select_stmt);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 5808 "bison_parser.cpp"
    break;

  case 332: /* table_ref_commalist: table_ref_atomic  */
#line 1421 "bison_parser.y"
                                       {
  (yyval.table_vec) = new std::vector<TableRef*>();
  (yyval.table_vec)->push_back((yyvsp[0].table));
}
#line 5817 "bison_parser.cpp"
    break;

  case 333: /* table_ref_commalist: table_ref_commalist ',' table_ref_atomic  */
#line 1425 "bison_parser.y"
                                           {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  (yyval.table_vec) = (yyvsp[-2].table_vec);
}
#line 5826 "bison_parser.cpp"
    break;

  case 334: /* table_ref_name: table_name opt_table_alias  */
#line 1430 "bison_parser.y"
                                            {
  auto tbl = new TableRef(kTableName);
  tbl->schema = (yyvsp[-1].table_name).schema;
  tbl->name = (yyvsp[-1].table_name).name;
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 5838 "bison_parser.cpp"
    break;

  case 335: /* table_ref_name_no_alias: table_name  */
#line 1438 "bison_parser.y"
                                     {
  (yyval.table) = new TableRef(kTableName);
  (yyval.table)->schema = (yyvsp[0].table_name).schema;
  (yyval.table)->name = (yyvsp[0].table_name).name;
}
#line 5848 "bison_parser.cpp"
    break;

  case 336: /* table_name: IDENTIFIER  */
#line 1444 "bison_parser.y"
                        {
  (yyval.table_name).schema = nullptr;
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 5857 "bison_parser.cpp"
    break;

  case 337: /* table_name: IDENTIFIER '.' IDENTIFIER  */
#line 1448 "bison_parser.y"
                            {
  (yyval.table_name).schema = (yyvsp[-2].sval);
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 5866 "bison_parser.cpp"
    break;

  case 338: /* table_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1452 "bison_parser.y"
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
#line 5881 "bison_parser.cpp"
    break;

  case 339: /* opt_index_name: IDENTIFIER  */
#line 1463 "bison_parser.y"
                            { (yyval.sval) = (yyvsp[0].sval); }
#line 5887 "bison_parser.cpp"
    break;

  case 340: /* opt_index_name: %empty  */
#line 1464 "bison_parser.y"
              { (yyval.sval) = nullptr; }
#line 5893 "bison_parser.cpp"
    break;

  case 342: /* table_alias: AS IDENTIFIER '(' ident_commalist ')'  */
#line 1466 "bison_parser.y"
                                                            { (yyval.alias_t) = new Alias((yyvsp[-3].sval), (yyvsp[-1].str_vec)); }
#line 5899 "bison_parser.cpp"
    break;

  case 344: /* opt_table_alias: %empty  */
#line 1468 "bison_parser.y"
                                            { (yyval.alias_t) = nullptr; }
#line 5905 "bison_parser.cpp"
    break;

  case 345: /* alias: AS IDENTIFIER  */
#line 1470 "bison_parser.y"
                      { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 5911 "bison_parser.cpp"
    break;

  case 346: /* alias: AS STRING  */
#line 1474 "bison_parser.y"
            { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 5917 "bison_parser.cpp"
    break;

  case 347: /* alias: IDENTIFIER  */
#line 1475 "bison_parser.y"
             { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 5923 "bison_parser.cpp"
    break;

  case 349: /* opt_alias: %empty  */
#line 1477 "bison_parser.y"
                                { (yyval.alias_t) = nullptr; }
#line 5929 "bison_parser.cpp"
    break;

  case 350: /* opt_locking_clause: opt_locking_clause_list  */
#line 1483 "bison_parser.y"
                                             { (yyval.locking_clause_vec) = (yyvsp[0].locking_clause_vec); }
#line 5935 "bison_parser.cpp"
    break;

  case 351: /* opt_locking_clause: %empty  */
#line 1484 "bison_parser.y"
              { (yyval.locking_clause_vec) = nullptr; }
#line 5941 "bison_parser.cpp"
    break;

  case 352: /* opt_locking_clause_list: locking_clause  */
#line 1486 "bison_parser.y"
                                         {
  (yyval.locking_clause_vec) = new std::vector<LockingClause*>();
  (yyval.locking_clause_vec)->push_back((yyvsp[0].locking_t));
}
#line 5950 "bison_parser.cpp"
    break;

  case 353: /* opt_locking_clause_list: opt_locking_clause_list locking_clause  */
#line 1490 "bison_parser.y"
                                         {
  (yyvsp[-1].locking_clause_vec)->push_back((yyvsp[0].locking_t));
  (yyval.locking_clause_vec) = (yyvsp[-1].locking_clause_vec);
}
#line 5959 "bison_parser.cpp"
    break;

  case 354: /* locking_clause: FOR row_lock_mode opt_row_lock_policy  */
#line 1495 "bison_parser.y"
                                                       {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-1].lock_mode_t);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
  (yyval.locking_t)->tables = nullptr;
}
#line 5970 "bison_parser.cpp"
    break;

  case 355: /* locking_clause: FOR row_lock_mode OF ident_commalist opt_row_lock_policy  */
#line 1501 "bison_parser.y"
                                                           {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-3].lock_mode_t);
  (yyval.locking_t)->tables = (yyvsp[-1].str_vec);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
}
#line 5981 "bison_parser.cpp"
    break;

  case 356: /* row_lock_mode: UPDATE  */
#line 1508 "bison_parser.y"
                       { (yyval.lock_mode_t) = RowLockMode::ForUpdate; }
#line 5987 "bison_parser.cpp"
    break;

  case 357: /* row_lock_mode: NO KEY UPDATE  */
#line 1509 "bison_parser.y"
                { (yyval.lock_mode_t) = RowLockMode::ForNoKeyUpdate; }
#line 5993 "bison_parser.cpp"
    break;

  case 358: /* row_lock_mode: SHARE  */
#line 1510 "bison_parser.y"
        { (yyval.lock_mode_t) = RowLockMode::ForShare; }
#line 5999 "bison_parser.cpp"
    break;

  case 359: /* row_lock_mode: KEY SHARE  */
#line 1511 "bison_parser.y"
            { (yyval.lock_mode_t) = RowLockMode::ForKeyShare; }
#line 6005 "bison_parser.cpp"
    break;

  case 360: /* opt_row_lock_policy: SKIP LOCKED  */
#line 1513 "bison_parser.y"
                                  { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::SkipLocked; }
#line 6011 "bison_parser.cpp"
    break;

  case 361: /* opt_row_lock_policy: NOWAIT  */
#line 1514 "bison_parser.y"
         { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::NoWait; }
#line 6017 "bison_parser.cpp"
    break;

  case 362: /* opt_row_lock_policy: %empty  */
#line 1515 "bison_parser.y"
              { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::None; }
#line 6023 "bison_parser.cpp"
    break;

  case 364: /* opt_with_clause: %empty  */
#line 1521 "bison_parser.y"
                                            { (yyval.with_description_vec) = nullptr; }
#line 6029 "bison_parser.cpp"
    break;

  case 365: /* with_clause: WITH with_description_list  */
#line 1523 "bison_parser.y"
                                         { (yyval.with_description_vec) = (yyvsp[0].with_description_vec); }
#line 6035 "bison_parser.cpp"
    break;

  case 366: /* with_description_list: with_description  */
#line 1525 "bison_parser.y"
                                         {
  (yyval.with_description_vec) = new std::vector<WithDescription*>();
  (yyval.with_description_vec)->push_back((yyvsp[0].with_description_t));
}
#line 6044 "bison_parser.cpp"
    break;

  case 367: /* with_description_list: with_description_list ',' with_description  */
#line 1529 "bison_parser.y"
                                             {
  (yyvsp[-2].with_description_vec)->push_back((yyvsp[0].with_description_t));
  (yyval.with_description_vec) = (yyvsp[-2].with_description_vec);
}
#line 6053 "bison_parser.cpp"
    break;

  case 368: /* with_description: IDENTIFIER AS select_with_paren  */
#line 1534 "bison_parser.y"
                                                   {
  (yyval.with_description_t) = new WithDescription();
  (yyval.with_description_t)->alias = (yyvsp[-2].sval);
  (yyval.with_description_t)->select = (yyvsp[0].select_stmt);
}
#line 6063 "bison_parser.cpp"
    break;

  case 369: /* join_clause: table_ref_atomic NATURAL JOIN nonjoin_table_ref_atomic  */
#line 1544 "bison_parser.y"
                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = kJoinNatural;
  (yyval.table)->join->left = (yyvsp[-3].table);
  (yyval.table)->join->right = (yyvsp[0].table);
}
#line 6075 "bison_parser.cpp"
    break;

  case 370: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic ON join_condition  */
#line 1551 "bison_parser.y"
                                                                         {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (JoinType)(yyvsp[-4].join_type);
  (yyval.table)->join->left = (yyvsp[-5].table);
  (yyval.table)->join->right = (yyvsp[-2].table);
  (yyval.table)->join->condition = (yyvsp[0].expr);
}
#line 6088 "bison_parser.cpp"
    break;

  case 371: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic USING '(' ident_commalist ')'  */
#line 1559 "bison_parser.y"
                                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (yyvsp[-6].join_type);
  (yyval.table)->join->left = (yyvsp[-7].table);
  (yyval.table)->join->right = (yyvsp[-4].table);
  (yyval.table)->join->namedColumns = (yyvsp[-1].str_vec);
}
#line 6101 "bison_parser.cpp"
    break;

  case 372: /* opt_join_type: INNER  */
#line 1568 "bison_parser.y"
                      { (yyval.join_type) = kJoinInner; }
#line 6107 "bison_parser.cpp"
    break;

  case 373: /* opt_join_type: LEFT OUTER  */
#line 1569 "bison_parser.y"
             { (yyval.join_type) = kJoinLeft; }
#line 6113 "bison_parser.cpp"
    break;

  case 374: /* opt_join_type: LEFT  */
#line 1570 "bison_parser.y"
       { (yyval.join_type) = kJoinLeft; }
#line 6119 "bison_parser.cpp"
    break;

  case 375: /* opt_join_type: RIGHT OUTER  */
#line 1571 "bison_parser.y"
              { (yyval.join_type) = kJoinRight; }
#line 6125 "bison_parser.cpp"
    break;

  case 376: /* opt_join_type: RIGHT  */
#line 1572 "bison_parser.y"
        { (yyval.join_type) = kJoinRight; }
#line 6131 "bison_parser.cpp"
    break;

  case 377: /* opt_join_type: FULL OUTER  */
#line 1573 "bison_parser.y"
             { (yyval.join_type) = kJoinFull; }
#line 6137 "bison_parser.cpp"
    break;

  case 378: /* opt_join_type: OUTER  */
#line 1574 "bison_parser.y"
        { (yyval.join_type) = kJoinFull; }
#line 6143 "bison_parser.cpp"
    break;

  case 379: /* opt_join_type: FULL  */
#line 1575 "bison_parser.y"
       { (yyval.join_type) = kJoinFull; }
#line 6149 "bison_parser.cpp"
    break;

  case 380: /* opt_join_type: CROSS  */
#line 1576 "bison_parser.y"
        { (yyval.join_type) = kJoinCross; }
#line 6155 "bison_parser.cpp"
    break;

  case 381: /* opt_join_type: %empty  */
#line 1577 "bison_parser.y"
                       { (yyval.join_type) = kJoinInner; }
#line 6161 "bison_parser.cpp"
    break;

  case 385: /* ident_commalist: IDENTIFIER  */
#line 1588 "bison_parser.y"
                             {
  (yyval.str_vec) = new std::vector<char*>();
  (yyval.str_vec)->push_back((yyvsp[0].sval));
}
#line 6170 "bison_parser.cpp"
    break;

  case 386: /* ident_commalist: ident_commalist ',' IDENTIFIER  */
#line 1592 "bison_parser.y"
                                 {
  (yyvsp[-2].str_vec)->push_back((yyvsp[0].sval));
  (yyval.str_vec) = (yyvsp[-2].str_vec);
}
#line 6179 "bison_parser.cpp"
    break;


#line 6183 "bison_parser.cpp"

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

#line 1598 "bison_parser.y"


/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */

    // clang-format on
