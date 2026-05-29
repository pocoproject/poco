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
  YYSYMBOL_UNIQUE = 171,                   /* UNIQUE  */
  YYSYMBOL_PRIMARY = 172,                  /* PRIMARY  */
  YYSYMBOL_FOREIGN = 173,                  /* FOREIGN  */
  YYSYMBOL_KEY = 174,                      /* KEY  */
  YYSYMBOL_REFERENCES = 175,               /* REFERENCES  */
  YYSYMBOL_176_ = 176,                     /* '='  */
  YYSYMBOL_EQUALS = 177,                   /* EQUALS  */
  YYSYMBOL_NOTEQUALS = 178,                /* NOTEQUALS  */
  YYSYMBOL_179_ = 179,                     /* '<'  */
  YYSYMBOL_180_ = 180,                     /* '>'  */
  YYSYMBOL_LESS = 181,                     /* LESS  */
  YYSYMBOL_GREATER = 182,                  /* GREATER  */
  YYSYMBOL_LESSEQ = 183,                   /* LESSEQ  */
  YYSYMBOL_GREATEREQ = 184,                /* GREATEREQ  */
  YYSYMBOL_NOTNULL = 185,                  /* NOTNULL  */
  YYSYMBOL_186_ = 186,                     /* '+'  */
  YYSYMBOL_187_ = 187,                     /* '-'  */
  YYSYMBOL_188_ = 188,                     /* '*'  */
  YYSYMBOL_189_ = 189,                     /* '/'  */
  YYSYMBOL_190_ = 190,                     /* '%'  */
  YYSYMBOL_191_ = 191,                     /* '^'  */
  YYSYMBOL_UMINUS = 192,                   /* UMINUS  */
  YYSYMBOL_193_ = 193,                     /* '['  */
  YYSYMBOL_194_ = 194,                     /* ']'  */
  YYSYMBOL_195_ = 195,                     /* '('  */
  YYSYMBOL_196_ = 196,                     /* ')'  */
  YYSYMBOL_197_ = 197,                     /* '.'  */
  YYSYMBOL_198_ = 198,                     /* ';'  */
  YYSYMBOL_199_ = 199,                     /* ','  */
  YYSYMBOL_200_ = 200,                     /* '?'  */
  YYSYMBOL_YYACCEPT = 201,                 /* $accept  */
  YYSYMBOL_input = 202,                    /* input  */
  YYSYMBOL_statement_list = 203,           /* statement_list  */
  YYSYMBOL_statement = 204,                /* statement  */
  YYSYMBOL_preparable_statement = 205,     /* preparable_statement  */
  YYSYMBOL_opt_hints = 206,                /* opt_hints  */
  YYSYMBOL_hint_list = 207,                /* hint_list  */
  YYSYMBOL_hint = 208,                     /* hint  */
  YYSYMBOL_transaction_statement = 209,    /* transaction_statement  */
  YYSYMBOL_opt_transaction_keyword = 210,  /* opt_transaction_keyword  */
  YYSYMBOL_prepare_statement = 211,        /* prepare_statement  */
  YYSYMBOL_prepare_target_query = 212,     /* prepare_target_query  */
  YYSYMBOL_execute_statement = 213,        /* execute_statement  */
  YYSYMBOL_import_statement = 214,         /* import_statement  */
  YYSYMBOL_file_type = 215,                /* file_type  */
  YYSYMBOL_file_path = 216,                /* file_path  */
  YYSYMBOL_opt_import_export_options = 217, /* opt_import_export_options  */
  YYSYMBOL_import_export_options = 218,    /* import_export_options  */
  YYSYMBOL_csv_option = 219,               /* csv_option  */
  YYSYMBOL_export_statement = 220,         /* export_statement  */
  YYSYMBOL_show_statement = 221,           /* show_statement  */
  YYSYMBOL_create_statement = 222,         /* create_statement  */
  YYSYMBOL_opt_not_exists = 223,           /* opt_not_exists  */
  YYSYMBOL_table_elem_commalist = 224,     /* table_elem_commalist  */
  YYSYMBOL_table_elem = 225,               /* table_elem  */
  YYSYMBOL_column_def = 226,               /* column_def  */
  YYSYMBOL_column_type = 227,              /* column_type  */
  YYSYMBOL_opt_time_precision = 228,       /* opt_time_precision  */
  YYSYMBOL_opt_decimal_specification = 229, /* opt_decimal_specification  */
  YYSYMBOL_opt_column_constraints = 230,   /* opt_column_constraints  */
  YYSYMBOL_column_constraints = 231,       /* column_constraints  */
  YYSYMBOL_column_constraint = 232,        /* column_constraint  */
  YYSYMBOL_table_constraint = 233,         /* table_constraint  */
  YYSYMBOL_references_spec = 234,          /* references_spec  */
  YYSYMBOL_drop_statement = 235,           /* drop_statement  */
  YYSYMBOL_opt_exists = 236,               /* opt_exists  */
  YYSYMBOL_alter_statement = 237,          /* alter_statement  */
  YYSYMBOL_alter_action = 238,             /* alter_action  */
  YYSYMBOL_drop_action = 239,              /* drop_action  */
  YYSYMBOL_delete_statement = 240,         /* delete_statement  */
  YYSYMBOL_truncate_statement = 241,       /* truncate_statement  */
  YYSYMBOL_insert_statement = 242,         /* insert_statement  */
  YYSYMBOL_opt_column_list = 243,          /* opt_column_list  */
  YYSYMBOL_update_statement = 244,         /* update_statement  */
  YYSYMBOL_update_clause_commalist = 245,  /* update_clause_commalist  */
  YYSYMBOL_update_clause = 246,            /* update_clause  */
  YYSYMBOL_select_statement = 247,         /* select_statement  */
  YYSYMBOL_select_within_set_operation = 248, /* select_within_set_operation  */
  YYSYMBOL_select_within_set_operation_no_parentheses = 249, /* select_within_set_operation_no_parentheses  */
  YYSYMBOL_select_with_paren = 250,        /* select_with_paren  */
  YYSYMBOL_select_no_paren = 251,          /* select_no_paren  */
  YYSYMBOL_set_operator = 252,             /* set_operator  */
  YYSYMBOL_set_type = 253,                 /* set_type  */
  YYSYMBOL_opt_all = 254,                  /* opt_all  */
  YYSYMBOL_select_clause = 255,            /* select_clause  */
  YYSYMBOL_opt_distinct = 256,             /* opt_distinct  */
  YYSYMBOL_select_list = 257,              /* select_list  */
  YYSYMBOL_opt_from_clause = 258,          /* opt_from_clause  */
  YYSYMBOL_from_clause = 259,              /* from_clause  */
  YYSYMBOL_opt_where = 260,                /* opt_where  */
  YYSYMBOL_opt_group = 261,                /* opt_group  */
  YYSYMBOL_opt_having = 262,               /* opt_having  */
  YYSYMBOL_opt_order = 263,                /* opt_order  */
  YYSYMBOL_order_list = 264,               /* order_list  */
  YYSYMBOL_order_desc = 265,               /* order_desc  */
  YYSYMBOL_opt_order_type = 266,           /* opt_order_type  */
  YYSYMBOL_opt_null_ordering = 267,        /* opt_null_ordering  */
  YYSYMBOL_opt_top = 268,                  /* opt_top  */
  YYSYMBOL_opt_limit = 269,                /* opt_limit  */
  YYSYMBOL_expr_list = 270,                /* expr_list  */
  YYSYMBOL_opt_extended_literal_list = 271, /* opt_extended_literal_list  */
  YYSYMBOL_extended_literal_list = 272,    /* extended_literal_list  */
  YYSYMBOL_casted_extended_literal = 273,  /* casted_extended_literal  */
  YYSYMBOL_extended_literal = 274,         /* extended_literal  */
  YYSYMBOL_expr_alias = 275,               /* expr_alias  */
  YYSYMBOL_expr = 276,                     /* expr  */
  YYSYMBOL_operand = 277,                  /* operand  */
  YYSYMBOL_scalar_expr = 278,              /* scalar_expr  */
  YYSYMBOL_unary_expr = 279,               /* unary_expr  */
  YYSYMBOL_binary_expr = 280,              /* binary_expr  */
  YYSYMBOL_logic_expr = 281,               /* logic_expr  */
  YYSYMBOL_in_expr = 282,                  /* in_expr  */
  YYSYMBOL_case_expr = 283,                /* case_expr  */
  YYSYMBOL_case_list = 284,                /* case_list  */
  YYSYMBOL_exists_expr = 285,              /* exists_expr  */
  YYSYMBOL_comp_expr = 286,                /* comp_expr  */
  YYSYMBOL_function_expr = 287,            /* function_expr  */
  YYSYMBOL_opt_window = 288,               /* opt_window  */
  YYSYMBOL_opt_partition = 289,            /* opt_partition  */
  YYSYMBOL_opt_frame_clause = 290,         /* opt_frame_clause  */
  YYSYMBOL_frame_type = 291,               /* frame_type  */
  YYSYMBOL_frame_bound = 292,              /* frame_bound  */
  YYSYMBOL_extract_expr = 293,             /* extract_expr  */
  YYSYMBOL_cast_expr = 294,                /* cast_expr  */
  YYSYMBOL_datetime_field = 295,           /* datetime_field  */
  YYSYMBOL_datetime_field_plural = 296,    /* datetime_field_plural  */
  YYSYMBOL_duration_field = 297,           /* duration_field  */
  YYSYMBOL_array_expr = 298,               /* array_expr  */
  YYSYMBOL_array_index = 299,              /* array_index  */
  YYSYMBOL_between_expr = 300,             /* between_expr  */
  YYSYMBOL_column_name = 301,              /* column_name  */
  YYSYMBOL_literal = 302,                  /* literal  */
  YYSYMBOL_string_literal = 303,           /* string_literal  */
  YYSYMBOL_bool_literal = 304,             /* bool_literal  */
  YYSYMBOL_num_literal = 305,              /* num_literal  */
  YYSYMBOL_int_literal = 306,              /* int_literal  */
  YYSYMBOL_null_literal = 307,             /* null_literal  */
  YYSYMBOL_date_literal = 308,             /* date_literal  */
  YYSYMBOL_interval_literal = 309,         /* interval_literal  */
  YYSYMBOL_param_expr = 310,               /* param_expr  */
  YYSYMBOL_table_ref = 311,                /* table_ref  */
  YYSYMBOL_table_ref_atomic = 312,         /* table_ref_atomic  */
  YYSYMBOL_nonjoin_table_ref_atomic = 313, /* nonjoin_table_ref_atomic  */
  YYSYMBOL_table_ref_commalist = 314,      /* table_ref_commalist  */
  YYSYMBOL_table_ref_name = 315,           /* table_ref_name  */
  YYSYMBOL_table_ref_name_no_alias = 316,  /* table_ref_name_no_alias  */
  YYSYMBOL_table_name = 317,               /* table_name  */
  YYSYMBOL_opt_index_name = 318,           /* opt_index_name  */
  YYSYMBOL_table_alias = 319,              /* table_alias  */
  YYSYMBOL_opt_table_alias = 320,          /* opt_table_alias  */
  YYSYMBOL_alias = 321,                    /* alias  */
  YYSYMBOL_opt_alias = 322,                /* opt_alias  */
  YYSYMBOL_opt_locking_clause = 323,       /* opt_locking_clause  */
  YYSYMBOL_opt_locking_clause_list = 324,  /* opt_locking_clause_list  */
  YYSYMBOL_locking_clause = 325,           /* locking_clause  */
  YYSYMBOL_row_lock_mode = 326,            /* row_lock_mode  */
  YYSYMBOL_opt_row_lock_policy = 327,      /* opt_row_lock_policy  */
  YYSYMBOL_opt_with_clause = 328,          /* opt_with_clause  */
  YYSYMBOL_with_clause = 329,              /* with_clause  */
  YYSYMBOL_with_description_list = 330,    /* with_description_list  */
  YYSYMBOL_with_description = 331,         /* with_description  */
  YYSYMBOL_join_clause = 332,              /* join_clause  */
  YYSYMBOL_opt_join_type = 333,            /* opt_join_type  */
  YYSYMBOL_join_condition = 334,           /* join_condition  */
  YYSYMBOL_opt_semicolon = 335,            /* opt_semicolon  */
  YYSYMBOL_ident_commalist = 336           /* ident_commalist  */
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
#define YYLAST   929

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  201
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  136
/* YYNRULES -- Number of rules.  */
#define YYNRULES  359
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  658

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   438


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
       2,     2,     2,     2,     2,     2,     2,   190,     2,     2,
     195,   196,   188,   186,   199,   187,   197,   189,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   198,
     179,   176,   180,   200,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   193,     2,   194,   191,     2,     2,     2,     2,     2,
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
     175,   177,   178,   181,   182,   183,   184,   185,   192
};

#if HSQL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   377,   377,   397,   403,   410,   414,   418,   419,   420,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     437,   438,   440,   444,   449,   453,   463,   464,   465,   467,
     467,   473,   479,   481,   485,   497,   503,   520,   535,   537,
     538,   539,   541,   555,   559,   569,   573,   597,   605,   618,
     625,   640,   660,   661,   666,   677,   690,   702,   709,   716,
     725,   726,   728,   732,   737,   738,   740,   748,   749,   750,
     751,   752,   753,   754,   758,   759,   760,   761,   762,   763,
     764,   765,   766,   767,   768,   770,   771,   773,   774,   775,
     777,   778,   780,   784,   788,   793,   801,   802,   803,   804,
     806,   807,   808,   810,   818,   824,   830,   836,   842,   843,
     850,   856,   858,   868,   875,   886,   893,   901,   902,   909,
     916,   920,   925,   935,   939,   943,   955,   955,   957,   958,
     967,   968,   970,   984,   996,  1001,  1005,  1009,  1014,  1015,
    1017,  1027,  1028,  1030,  1032,  1033,  1035,  1037,  1038,  1040,
    1045,  1047,  1048,  1050,  1051,  1053,  1057,  1062,  1064,  1065,
    1066,  1068,  1069,  1091,  1092,  1094,  1095,  1096,  1097,  1098,
    1099,  1104,  1108,  1114,  1115,  1117,  1121,  1126,  1126,  1130,
    1131,  1132,  1134,  1143,  1143,  1143,  1143,  1143,  1145,  1146,
    1146,  1146,  1146,  1146,  1146,  1146,  1146,  1147,  1147,  1151,
    1151,  1153,  1154,  1155,  1156,  1157,  1159,  1159,  1160,  1161,
    1162,  1163,  1164,  1165,  1166,  1167,  1168,  1170,  1171,  1173,
    1174,  1175,  1176,  1180,  1181,  1182,  1183,  1185,  1186,  1188,
    1189,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1201,  1202,
    1203,  1206,  1212,  1213,  1215,  1216,  1221,  1222,  1223,  1227,
    1228,  1229,  1231,  1232,  1233,  1234,  1235,  1237,  1239,  1241,
    1242,  1243,  1244,  1245,  1246,  1248,  1249,  1250,  1251,  1252,
    1253,  1255,  1255,  1257,  1259,  1261,  1263,  1264,  1265,  1266,
    1268,  1268,  1268,  1268,  1268,  1268,  1268,  1270,  1272,  1273,
    1275,  1276,  1278,  1280,  1282,  1293,  1294,  1305,  1337,  1342,
    1354,  1363,  1363,  1370,  1370,  1372,  1372,  1379,  1383,  1388,
    1396,  1402,  1406,  1411,  1412,  1414,  1414,  1416,  1416,  1418,
    1419,  1421,  1421,  1427,  1428,  1430,  1434,  1439,  1445,  1452,
    1453,  1454,  1455,  1457,  1458,  1459,  1465,  1465,  1467,  1469,
    1473,  1478,  1488,  1495,  1503,  1512,  1513,  1514,  1515,  1516,
    1517,  1518,  1519,  1520,  1521,  1523,  1529,  1529,  1532,  1536
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
  "GROUPS", "UNBOUNDED", "FOLLOWING", "PRECEDING", "CURRENT_ROW", "UNIQUE",
  "PRIMARY", "FOREIGN", "KEY", "REFERENCES", "'='", "EQUALS", "NOTEQUALS",
  "'<'", "'>'", "LESS", "GREATER", "LESSEQ", "GREATEREQ", "NOTNULL", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'^'", "UMINUS", "'['", "']'", "'('", "')'",
  "'.'", "';'", "','", "'?'", "$accept", "input", "statement_list",
  "statement", "preparable_statement", "opt_hints", "hint_list", "hint",
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
  "column_name", "literal", "string_literal", "bool_literal",
  "num_literal", "int_literal", "null_literal", "date_literal",
  "interval_literal", "param_expr", "table_ref", "table_ref_atomic",
  "nonjoin_table_ref_atomic", "table_ref_commalist", "table_ref_name",
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

#define YYPACT_NINF (-493)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-357)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     650,    26,    86,    90,   128,    86,   -16,    56,    80,    78,
      86,   150,    20,   229,    41,   241,    94,    94,    94,   275,
     107,  -493,   194,  -493,   194,  -493,  -493,  -493,  -493,  -493,
    -493,  -493,  -493,  -493,  -493,  -493,  -493,   -15,  -493,   324,
     140,  -493,   149,   262,  -493,   242,   242,   242,    86,   380,
      86,   260,  -493,   261,   -15,   264,    42,   261,   261,   261,
      86,  -493,   280,   231,  -493,  -493,  -493,  -493,  -493,  -493,
     627,  -493,   319,  -493,  -493,   302,   219,  -493,   201,  -493,
     429,   161,   454,   340,   465,    86,    86,   405,  -493,   387,
     301,   496,   466,    86,   316,   320,   511,   511,   511,   516,
      86,    86,  -493,   337,   241,  -493,   344,   527,   524,  -493,
    -493,  -493,   -15,   424,   414,   -15,    16,  -493,  -493,  -493,
     631,  -493,  -493,   351,   543,  -493,   548,  -493,  -493,    27,
    -493,   357,   356,  -493,  -493,  -493,  -493,  -493,  -493,  -493,
    -493,  -493,  -493,  -493,  -493,  -493,   508,  -493,   425,   -35,
     301,   397,  -493,   511,   554,   235,   388,   -41,  -493,  -493,
     470,  -493,  -493,  -493,   -62,   -62,   -62,  -493,  -493,  -493,
    -493,  -493,   563,  -493,  -493,  -493,   397,   489,  -493,  -493,
     219,  -493,  -493,   397,   489,   397,   187,   447,  -493,  -493,
    -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,
    -493,  -493,  -493,    34,  -493,   308,  -493,  -493,  -493,   161,
    -493,    86,   571,   460,    32,   449,   -23,   383,   384,   391,
     193,   407,   390,   417,  -493,   348,   144,   442,  -493,  -493,
    -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,
    -493,  -493,  -493,  -493,   487,  -493,   132,   393,  -493,   397,
     496,  -493,   550,  -493,  -493,   398,    75,  -493,   405,  -493,
     401,   139,  -493,   502,   400,  -493,   131,    16,   -15,   402,
    -493,   200,    16,   144,   546,    10,    -4,  -493,   447,  -493,
     473,  -493,  -493,   413,   514,  -493,   700,   418,   440,   441,
     171,  -493,  -493,  -493,   460,    12,    18,   556,   308,   397,
     397,   250,   192,   428,   417,   711,   397,    64,   438,   -59,
     397,   397,   417,  -493,   417,   -34,   443,   116,   417,   417,
     417,   417,   417,   417,   417,   417,   417,   417,   417,   417,
     417,   417,   417,   527,    86,  -493,   621,   161,   144,  -493,
     261,    75,   633,   635,   380,   636,   173,  -493,  -493,   161,
    -493,   563,    22,   405,  -493,   397,  -493,   638,  -493,  -493,
    -493,  -493,   397,  -493,  -493,   639,   447,   397,   397,  -493,
     469,  -493,   482,    99,  -493,   700,   554,   511,  -493,  -493,
     451,  -493,   452,  -493,  -493,   453,  -493,  -493,   455,  -493,
    -493,  -493,  -493,   457,  -493,  -493,   -36,   554,   458,   459,
    -493,    32,  -493,   570,   397,   462,  -493,   467,   559,   -39,
     215,   209,   397,   397,  -493,   556,   555,    95,  -493,  -493,
    -493,   542,   693,   736,   417,   476,   348,  -493,   558,   468,
     736,   736,   736,   736,   338,   338,   338,   338,    64,    64,
      47,    47,    47,   -66,   471,  -493,  -493,   190,   659,   196,
    -493,  -493,  -493,  -493,  -493,   120,   210,  -493,   460,  -493,
     363,  -493,   474,  -493,    46,  -493,   597,  -493,  -493,  -493,
     669,  -493,  -493,   144,   144,   611,  -493,   554,  -493,   515,
    -493,   480,   220,  -493,   672,   674,  -493,   677,   678,   679,
    -493,  -493,   580,  -493,   513,    86,  -493,   -36,  -493,  -493,
     230,   554,   554,  -493,   493,  -493,   238,    14,  -493,   397,
     700,   397,   397,  -493,    29,   232,   494,  -493,   417,   736,
     348,   499,   243,  -493,  -493,  -493,  -493,  -493,   692,   380,
    -493,  -493,   501,   598,  -493,  -493,  -493,   618,   620,   622,
     600,    22,   703,  -493,  -493,  -493,   575,  -493,  -493,  -493,
     121,  -493,  -493,  -493,   519,   263,   522,   523,   525,  -493,
    -493,   301,  -493,  -493,  -493,   276,   282,   602,   570,   570,
     397,   -53,   526,   144,   239,  -493,   397,  -493,   711,   528,
     284,  -493,  -493,  -493,  -493,    46,    22,  -493,  -493,  -493,
      22,   509,   512,   397,  -493,  -493,  -493,   704,  -493,  -493,
    -493,  -493,  -493,   536,   584,   489,  -493,  -493,   290,  -493,
    -493,  -493,   144,  -493,  -493,  -493,  -493,   188,   554,   -10,
     529,  -493,   397,   213,   570,   531,   397,   292,   397,  -493,
    -493,   400,  -493,  -493,  -493,   532,    58,  -493,   554,   144,
    -493,  -493,   144,  -493,   272,    57,   305,  -493,  -493,   296,
    -493,  -493,   605,  -493,  -493,  -493,    57,  -493
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     337,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    30,    30,    30,     0,
     357,     3,    21,    19,    21,    18,     8,     9,     7,    11,
      16,    17,    13,    14,    12,    15,    10,     0,   336,     0,
     311,   114,    33,     0,    54,    61,    61,    61,     0,     0,
       0,     0,   310,   109,     0,     0,     0,   109,   109,   109,
       0,    52,     0,   338,   339,    29,    26,    28,    27,     1,
     337,     2,     0,     6,     5,   164,   123,   124,   154,   106,
       0,   174,     0,     0,   314,     0,     0,   148,    37,     0,
     118,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     4,     0,     0,   142,   136,
     137,   135,     0,   139,     0,     0,   170,   312,   287,   290,
     292,   299,   300,     0,     0,   293,     0,   288,   289,     0,
     298,     0,   173,   175,   177,   179,   280,   281,   282,   291,
     283,   284,   285,   286,    32,    31,     0,   313,     0,     0,
     118,     0,   113,     0,     0,     0,     0,   148,   120,   108,
       0,   131,   130,    38,    41,    41,    41,   107,   104,   105,
     341,   340,     0,   292,   163,   141,     0,   154,   127,   126,
     128,   138,   134,     0,   154,     0,     0,   324,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   295,     0,   294,   297,   180,   181,    34,     0,
      60,     0,     0,   337,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,   278,     0,   147,   183,   190,   191,
     192,   185,   187,   193,   186,   206,   194,   195,   196,   197,
     189,   184,   199,   200,     0,   358,     0,     0,   116,     0,
       0,   119,     0,   110,   111,     0,     0,    51,   148,    50,
      24,     0,    22,   145,   143,   171,   322,   170,     0,   153,
     155,   160,   170,   166,   168,   165,     0,   132,   323,   325,
       0,   296,   176,     0,     0,    57,     0,     0,     0,     0,
       0,    62,    64,    65,   337,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   202,     0,   201,     0,     0,
       0,     0,     0,   203,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   117,     0,     0,   122,   121,
     109,     0,     0,     0,     0,     0,     0,    47,    36,     0,
      20,     0,     0,   148,   144,     0,   320,     0,   321,   182,
     125,   129,     0,   159,   158,   161,   324,     0,     0,   329,
       0,   331,     0,   335,   326,     0,     0,     0,    83,    77,
       0,    79,    89,    80,    67,     0,    74,    75,     0,    71,
      72,    78,    81,    86,    76,    68,    91,     0,     0,     0,
      56,     0,    59,   243,     0,   277,   279,     0,     0,     0,
       0,     0,     0,     0,   225,     0,     0,     0,   198,   188,
     217,   218,     0,   213,     0,     0,     0,   204,     0,   216,
     215,   231,   232,   233,   234,   235,   236,   237,   208,   207,
     210,   209,   211,   212,     0,    35,   359,     0,     0,     0,
      48,    45,    43,    49,    40,     0,     0,    23,   337,   146,
     301,   303,     0,   305,   318,   304,   150,   172,   319,   156,
       0,   157,   133,   169,   167,     0,   332,     0,   334,     0,
     327,     0,     0,    55,     0,     0,    73,     0,     0,     0,
      82,    98,     0,    97,     0,     0,    66,    90,    92,    94,
       0,     0,     0,    63,     0,   238,     0,   142,   229,     0,
       0,     0,     0,   223,     0,     0,     0,   273,     0,   214,
       0,     0,     0,   205,   274,   115,   112,    39,     0,     0,
      46,    25,     0,     0,   353,   345,   351,   349,   352,   347,
       0,     0,     0,   317,   309,   315,     0,   140,   162,   330,
     335,   333,   178,    58,     0,     0,     0,     0,     0,    99,
      96,   118,    93,    95,   101,     0,     0,   245,   243,   243,
       0,     0,     0,   227,     0,   226,     0,   230,   275,     0,
       0,   221,   219,    44,    42,   318,     0,   348,   350,   346,
       0,   302,   319,     0,   328,    70,    88,     0,    84,    69,
      85,   103,   100,     0,     0,   154,   239,   240,     0,   257,
     258,   224,   228,   222,   220,   306,   342,   354,     0,   152,
       0,   102,     0,   248,   243,     0,     0,     0,     0,   149,
      87,   244,   249,   250,   251,     0,     0,   241,     0,   355,
     343,   316,   151,   242,     0,     0,     0,   256,   246,     0,
     255,   253,     0,   254,   252,   344,     0,   247
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -493,  -493,  -493,   660,  -493,   705,  -493,   389,  -493,   488,
    -493,  -493,  -493,  -493,  -322,   -79,   343,   403,   283,  -493,
    -493,  -493,   464,  -493,   345,  -493,  -328,  -493,  -493,  -493,
    -493,   244,  -493,  -446,  -493,   -43,  -493,  -493,  -493,  -493,
    -493,  -493,  -144,  -493,  -493,   497,  -212,   -84,  -493,   233,
     -50,   -24,  -493,  -493,   -85,  -278,  -493,  -493,  -493,  -133,
    -493,  -493,  -172,  -493,   386,  -493,  -493,  -493,    50,  -297,
    -493,  -269,   541,   549,   396,  -149,  -210,  -493,  -493,  -493,
    -493,  -493,  -493,   456,  -493,  -493,  -493,  -483,  -493,  -493,
    -493,  -492,  -493,  -493,  -153,  -493,  -493,  -493,  -493,  -493,
    -493,   -61,  -493,  -493,   624,  -100,  -493,  -493,   625,  -493,
    -493,  -484,   169,  -493,  -493,  -493,    -2,  -493,  -493,   175,
     490,  -493,   392,  -493,   483,  -493,   212,  -493,  -493,  -493,
     662,  -493,  -493,  -493,  -493,  -347
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    21,    22,    73,   261,   262,    23,    66,
      24,   145,    25,    26,    89,   164,   257,   346,   347,    27,
      28,    29,    84,   290,   291,   292,   396,   490,   486,   496,
     497,   498,   293,   499,    30,    93,    31,   253,   254,    32,
      33,    34,   155,    35,   157,   158,    36,   177,   178,   179,
      77,   112,   113,   182,    78,   176,   263,   353,   354,   152,
     547,   629,   116,   269,   270,   365,   471,   108,   187,   264,
     131,   132,   133,   134,   265,   266,   227,   228,   229,   230,
     231,   232,   233,   302,   234,   235,   236,   505,   605,   635,
     636,   648,   237,   238,   200,   201,   202,   239,   240,   241,
     242,   243,   136,   137,   138,   139,   140,   141,   142,   143,
     459,   460,   461,   462,   463,    51,   464,   148,   543,   544,
     545,   359,   277,   278,   279,   373,   480,    37,    38,    63,
      64,   465,   540,   640,    71,   246
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      41,   285,   226,    44,    95,   267,   215,   174,    52,   417,
      56,   305,   272,   307,    99,   100,   101,   404,   165,   166,
     135,   405,   452,    40,   251,    40,   175,   180,   175,   482,
     180,   184,   119,   120,   271,   286,   273,   275,   118,   119,
     120,   121,   122,   628,   151,    75,    87,   481,    90,   356,
     500,   563,   281,   255,   115,    39,    45,   591,   102,   310,
     369,   212,    60,   644,   644,   310,    46,   368,   447,   424,
     491,   301,   318,   185,   244,   311,   309,   645,   342,   310,
     456,   311,   402,   149,   150,   606,   607,   492,   510,    40,
     186,   160,   213,    42,   305,   311,   425,    47,   168,   169,
     338,   343,   422,    61,   423,   248,   617,   506,   429,   430,
     431,   432,   433,   434,   435,   436,   437,   438,   439,   440,
     441,   442,   443,   342,   124,   348,   344,   333,   310,   522,
     550,    43,   370,   256,   356,   493,   494,   419,    97,   495,
     125,   637,   135,   609,   311,   408,   528,   310,   135,   575,
     409,   410,    48,   652,   565,   566,   268,   621,   250,   371,
     214,   420,   421,   311,   657,   118,   119,   120,   121,   122,
     372,   529,   295,   542,   296,   308,    49,    98,    50,   126,
      54,   345,   572,   180,   361,   318,   126,   127,   128,   355,
     216,   118,   119,   120,   121,   122,   216,   118,   119,   120,
     121,   122,   318,   287,   288,   289,   406,   584,   403,   283,
     569,   123,   109,   271,   519,    54,   533,   458,   473,   474,
     466,   129,   427,   580,   646,   646,   345,   647,   647,   570,
     109,   477,    53,   444,   130,   217,   218,   219,   332,   428,
     333,   217,   218,   219,    62,    55,   532,   407,   110,   310,
      65,   124,   329,   330,   331,   332,   534,   333,   357,   478,
     479,   535,   310,   514,   515,   311,   110,   125,   536,   537,
      76,   627,   625,   608,   220,    69,   135,   124,   311,   114,
     220,   478,   479,   124,   111,   538,   412,    94,   135,   517,
     539,   649,   363,   125,   355,    75,   619,   448,   483,   125,
     247,    57,   111,   512,   274,    70,   413,   300,   578,    72,
     221,    58,   414,   126,   127,   128,   221,   360,   310,   364,
     336,   626,   366,   413,   222,   631,   511,    79,   335,   513,
     222,   336,   445,   310,   311,   350,   170,    80,   351,   126,
     127,   128,    59,   576,    81,   126,   127,   128,   129,   311,
     310,   216,   118,   119,   120,   121,   122,   310,    82,   611,
     571,   130,   573,   574,   300,   516,   311,   400,   310,   454,
     401,    83,   455,   311,   223,   224,   521,   632,   633,   634,
     223,   224,   225,    88,   311,    91,   525,   130,   225,   209,
      92,   533,   527,   130,   313,   455,   217,   218,   219,    96,
     216,   118,   119,   120,   121,   122,   531,   103,    75,   209,
     216,   118,   119,   120,   121,   122,   553,   601,   106,   336,
     216,   118,   119,   120,   121,   122,   564,   612,   107,   336,
     104,   534,   117,   623,   568,   220,   535,   355,   124,   582,
     650,   651,   355,   536,   537,   217,   218,   219,   188,   189,
     190,   191,   192,   193,   125,   303,   218,   219,   144,   596,
     538,   312,   597,   146,  -354,   539,   218,   219,   147,   317,
     579,   221,   602,   653,   654,   336,   318,   639,   603,   642,
     614,   336,   153,   355,   220,   222,   624,   124,   641,   355,
     151,   336,   655,   561,   220,   336,   154,   124,   313,   156,
     126,   127,   128,   125,   220,    67,    68,   124,   258,   259,
      85,    86,   161,   125,   159,   163,   162,  -357,  -357,   167,
     221,  -357,  -357,   125,   327,   328,   329,   330,   331,   332,
     304,   333,    54,   173,   222,   223,   224,   533,   175,   172,
     304,   181,   183,   225,   222,   314,   203,   204,   130,   126,
     127,   128,   205,   208,   222,   209,   210,   245,   211,   126,
     127,   128,  -307,   252,   249,   315,   260,   114,   276,   126,
     127,   128,   316,   317,   284,    15,   294,   534,   297,   298,
     318,   319,   535,   306,   223,   224,   299,   334,   337,   536,
     537,   340,   225,   341,   223,   224,   349,   130,   352,   355,
     375,   362,   225,   367,   223,   224,   538,   130,   376,   377,
    -354,   539,   225,   397,   398,   399,    75,   130,   320,   321,
     322,   323,   324,   415,   446,   325,   326,  -356,   327,   328,
     329,   330,   331,   332,   418,   333,     1,   450,   426,   451,
     453,   468,   470,   475,     2,   476,   484,   485,   487,   504,
     488,     3,   489,   501,   502,   509,     4,   507,   424,     1,
     310,   333,   526,   508,   523,   524,     5,     2,   546,     6,
       7,   520,   548,   541,     3,   549,   552,   551,   554,     4,
     555,     8,     9,   556,   557,   558,   559,   560,   567,     5,
     577,    10,     6,     7,    11,   581,   583,   585,   587,   586,
     588,   590,   589,   593,     8,     9,   592,   618,  -308,   604,
     620,   495,   622,   378,    10,   595,    12,    11,   598,   599,
      13,   600,   610,   656,   613,   630,   638,   379,   643,    74,
     105,   380,   381,   382,   383,   384,    14,   385,   530,    12,
     457,   562,    15,    13,   449,   386,   503,   339,   469,   313,
     282,   467,   280,   206,   207,   616,   358,   411,   472,    14,
     615,   374,   594,     0,     0,    15,   171,   313,     0,     0,
     387,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,     0,    16,    17,    18,     0,   388,     0,
     389,   390,   313,     0,     0,     0,   314,     0,     0,     0,
       0,     0,     0,     0,     0,   391,     0,    16,    17,    18,
     392,   518,   393,     0,   314,     0,   416,     0,     0,     0,
       0,     0,   394,     0,   317,     0,     0,     0,     0,     0,
       0,   318,   319,     0,   416,     0,     0,     0,     0,  -357,
       0,     0,   317,     0,     0,     0,     0,     0,     0,   318,
     319,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   317,     0,   320,
     321,   322,   323,   324,   318,  -357,   325,   326,     0,   327,
     328,   329,   330,   331,   332,     0,   333,   320,   321,   322,
     323,   324,     0,     0,   325,   326,     0,   327,   328,   329,
     330,   331,   332,     0,   333,     0,     0,     0,     0,     0,
       0,     0,  -357,  -357,  -357,   323,   324,     0,     0,   325,
     326,     0,   327,   328,   329,   330,   331,   332,     0,   333
};

static const yytype_int16 yycheck[] =
{
       2,   213,   151,     5,    54,   177,   150,   107,    10,   306,
      12,   221,   184,   223,    57,    58,    59,   295,    97,    98,
      81,     3,   344,     3,   157,     3,    14,   112,    14,   376,
     115,   115,     5,     6,   183,     3,   185,   186,     4,     5,
       6,     7,     8,    53,    85,    60,    48,   375,    50,     3,
     397,   497,   205,   115,    78,    29,    72,   541,    60,   118,
      64,    96,    21,     6,     6,   118,    82,    57,   337,   103,
     106,   220,   138,    57,   153,   134,   225,    19,     3,   118,
     349,   134,   294,    85,    86,   568,   569,   123,   127,     3,
      74,    93,   127,     3,   304,   134,   130,   113,   100,   101,
     249,    26,   312,    62,   314,   155,   590,   404,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,     3,    90,   258,    51,   193,   118,   426,
     477,     3,   136,   195,     3,   171,   172,   196,    96,   175,
     106,   624,   203,   196,   134,   298,    26,   118,   209,   120,
     299,   300,    96,   645,   501,   502,   180,   603,   199,   163,
     195,   310,   311,   134,   656,     4,     5,     6,     7,     8,
     174,    51,   195,   127,   197,   225,    96,   135,   100,   152,
     195,   106,   510,   268,   268,   138,   152,   153,   154,   199,
       3,     4,     5,     6,     7,     8,     3,     4,     5,     6,
       7,     8,   138,   171,   172,   173,   188,   529,   196,   211,
     196,    50,    11,   362,   424,   195,    28,   195,   367,   368,
     353,   187,   106,   520,   167,   167,   106,   170,   170,   507,
      11,   132,    82,   333,   200,    48,    49,    50,   191,   123,
     193,    48,    49,    50,     3,    12,   458,   297,    47,   118,
     156,    90,   188,   189,   190,   191,    68,   193,   127,   160,
     161,    73,   118,   412,   413,   134,    47,   106,    80,    81,
      37,   618,    84,   570,    87,     0,   337,    90,   134,    78,
      87,   160,   161,    90,    83,    97,    94,    54,   349,   194,
     102,   638,    92,   106,   199,    60,   593,   340,   377,   106,
      65,    72,    83,    94,   117,   198,   114,   114,   518,   115,
     123,    82,   120,   152,   153,   154,   123,   267,   118,   119,
     199,   133,   272,   114,   137,   622,   111,     3,   196,   120,
     137,   199,   334,   118,   134,   196,   103,   197,   199,   152,
     153,   154,   113,   111,   195,   152,   153,   154,   187,   134,
     118,     3,     4,     5,     6,     7,     8,   118,    96,   120,
     509,   200,   511,   512,   114,   415,   134,   196,   118,   196,
     199,   129,   199,   134,   187,   188,   426,   164,   165,   166,
     187,   188,   195,     3,   134,   125,   196,   200,   195,   199,
     129,    28,   196,   200,    56,   199,    48,    49,    50,   135,
       3,     4,     5,     6,     7,     8,   196,   127,    60,   199,
       3,     4,     5,     6,     7,     8,   196,   561,    99,   199,
       3,     4,     5,     6,     7,     8,   196,   576,   126,   199,
     199,    68,     3,   605,   196,    87,    73,   199,    90,   196,
     168,   169,   199,    80,    81,    48,    49,    50,   140,   141,
     142,   143,   144,   145,   106,    48,    49,    50,     4,   196,
      97,    19,   199,   123,   101,   102,    49,    50,     3,   131,
     520,   123,   196,   168,   169,   199,   138,   626,   196,   628,
     196,   199,    95,   199,    87,   137,   196,    90,   196,   199,
      85,   199,   196,   495,    87,   199,   195,    90,    56,     3,
     152,   153,   154,   106,    87,    17,    18,    90,   165,   166,
      46,    47,   196,   106,    48,     4,   196,   179,   180,     3,
     123,   183,   184,   106,   186,   187,   188,   189,   190,   191,
     123,   193,   195,     6,   137,   187,   188,    28,    14,   195,
     123,   117,   128,   195,   137,   103,   195,     4,   200,   152,
     153,   154,     4,   196,   137,   199,    48,     3,   133,   152,
     153,   154,   199,    93,   176,   123,     3,    78,   121,   152,
     153,   154,   130,   131,     3,   115,   127,    68,   195,   195,
     138,   139,    73,   193,   187,   188,   195,   100,   195,    80,
      81,    41,   195,   195,   187,   188,   195,   200,    96,   199,
     127,   199,   195,    57,   187,   188,    97,   200,   195,    95,
     101,   102,   195,   195,   174,   174,    60,   200,   176,   177,
     178,   179,   180,   195,     3,   183,   184,     0,   186,   187,
     188,   189,   190,   191,   196,   193,     9,     4,   195,     4,
       4,     3,     3,   174,    17,   163,   195,   195,   195,    79,
     195,    24,   195,   195,   195,    96,    29,   195,   103,     9,
     118,   193,     3,   196,   106,   194,    39,    17,    71,    42,
      43,   195,     3,   199,    24,    64,   196,   162,     6,    29,
       6,    54,    55,     6,     6,     6,   106,   174,   195,    39,
     196,    64,    42,    43,    67,   196,     4,   196,    80,   101,
      80,   101,    80,   128,    54,    55,     3,   195,   199,   107,
       6,   175,   128,    13,    64,   196,    89,    67,   196,   196,
      93,   196,   196,   118,   196,   196,   195,    27,   196,    24,
      70,    31,    32,    33,    34,    35,   109,    37,   455,    89,
     351,   497,   115,    93,   341,    45,   401,   250,   362,    56,
     209,   355,   203,   129,   129,   586,   266,   301,   366,   109,
     585,   278,   550,    -1,    -1,   115,   104,    56,    -1,    -1,
      70,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,   157,   158,   159,    -1,    88,    -1,
      90,    91,    56,    -1,    -1,    -1,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,   157,   158,   159,
     110,   118,   112,    -1,   103,    -1,   123,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   131,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,    -1,   123,    -1,    -1,    -1,    -1,   103,
      -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,   176,
     177,   178,   179,   180,   138,   139,   183,   184,    -1,   186,
     187,   188,   189,   190,   191,    -1,   193,   176,   177,   178,
     179,   180,    -1,    -1,   183,   184,    -1,   186,   187,   188,
     189,   190,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,   178,   179,   180,    -1,    -1,   183,
     184,    -1,   186,   187,   188,   189,   190,   191,    -1,   193
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     9,    17,    24,    29,    39,    42,    43,    54,    55,
      64,    67,    89,    93,   109,   115,   157,   158,   159,   202,
     203,   204,   205,   209,   211,   213,   214,   220,   221,   222,
     235,   237,   240,   241,   242,   244,   247,   328,   329,    29,
       3,   317,     3,     3,   317,    72,    82,   113,    96,    96,
     100,   316,   317,    82,   195,   250,   317,    72,    82,   113,
      21,    62,     3,   330,   331,   156,   210,   210,   210,     0,
     198,   335,   115,   206,   206,    60,   250,   251,   255,     3,
     197,   195,    96,   129,   223,   223,   223,   317,     3,   215,
     317,   125,   129,   236,   250,   251,   135,    96,   135,   236,
     236,   236,   317,   127,   199,   204,    99,   126,   268,    11,
      47,    83,   252,   253,    78,   252,   263,     3,     4,     5,
       6,     7,     8,    50,    90,   106,   152,   153,   154,   187,
     200,   271,   272,   273,   274,   302,   303,   304,   305,   306,
     307,   308,   309,   310,     4,   212,   123,     3,   318,   317,
     317,    85,   260,    95,   195,   243,     3,   245,   246,    48,
     317,   196,   196,     4,   216,   216,   216,     3,   317,   317,
     250,   331,   195,     6,   306,    14,   256,   248,   249,   250,
     255,   117,   254,   128,   248,    57,    74,   269,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     295,   296,   297,   195,     4,     4,   305,   309,   196,   199,
      48,   133,    96,   127,   195,   243,     3,    48,    49,    50,
      87,   123,   137,   187,   188,   195,   276,   277,   278,   279,
     280,   281,   282,   283,   285,   286,   287,   293,   294,   298,
     299,   300,   301,   302,   216,     3,   336,    65,   251,   176,
     199,   260,    93,   238,   239,   115,   195,   217,   217,   217,
       3,   207,   208,   257,   270,   275,   276,   263,   252,   264,
     265,   276,   263,   276,   117,   276,   121,   323,   324,   325,
     274,   295,   273,   317,     3,   247,     3,   171,   172,   173,
     224,   225,   226,   233,   127,   195,   197,   195,   195,   195,
     114,   276,   284,    48,   123,   277,   193,   277,   251,   276,
     118,   134,    19,    56,   103,   123,   130,   131,   138,   139,
     176,   177,   178,   179,   180,   183,   184,   186,   187,   188,
     189,   190,   191,   193,   100,   196,   199,   195,   276,   246,
      41,   195,     3,    26,    51,   106,   218,   219,   260,   195,
     196,   199,    96,   258,   259,   199,     3,   127,   321,   322,
     269,   248,   199,    92,   119,   266,   269,    57,    57,    64,
     136,   163,   174,   326,   325,   127,   195,    95,    13,    27,
      31,    32,    33,    34,    35,    37,    45,    70,    88,    90,
      91,   105,   110,   112,   122,   155,   227,   195,   174,   174,
     196,   199,   247,   196,   256,     3,   188,   251,   295,   276,
     276,   284,    94,   114,   120,   195,   123,   270,   196,   196,
     276,   276,   277,   277,   103,   130,   195,   106,   123,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   306,   317,     3,   272,   236,   218,
       4,     4,   215,     4,   196,   199,   272,   208,   195,   311,
     312,   313,   314,   315,   317,   332,   260,   275,     3,   265,
       3,   267,   323,   276,   276,   174,   163,   132,   160,   161,
     327,   227,   336,   216,   195,   195,   229,   195,   195,   195,
     228,   106,   123,   171,   172,   175,   230,   231,   232,   234,
     336,   195,   195,   225,    79,   288,   270,   195,   196,    96,
     127,   111,    94,   120,   276,   276,   251,   194,   118,   277,
     195,   251,   270,   106,   194,   196,     3,   196,    26,    51,
     219,   196,   247,    28,    68,    73,    80,    81,    97,   102,
     333,   199,   127,   319,   320,   321,    71,   261,     3,    64,
     336,   162,   196,   196,     6,     6,     6,     6,     6,   106,
     174,   317,   232,   234,   196,   336,   336,   195,   196,   196,
     256,   276,   227,   276,   276,   120,   111,   196,   277,   251,
     270,   196,   196,     4,   215,   196,   101,    80,    80,    80,
     101,   312,     3,   128,   327,   196,   196,   199,   196,   196,
     196,   243,   196,   196,   107,   289,   288,   288,   270,   196,
     196,   120,   276,   196,   196,   320,   313,   312,   195,   270,
       6,   234,   128,   263,   196,    84,   133,   336,    53,   262,
     196,   270,   164,   165,   166,   290,   291,   288,   195,   276,
     334,   196,   276,   196,     6,    19,   167,   170,   292,   336,
     168,   169,   292,   168,   169,   196,   118,   292
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   201,   202,   203,   203,   204,   204,   204,   204,   204,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     206,   206,   207,   207,   208,   208,   209,   209,   209,   210,
     210,   211,   212,   213,   213,   214,   214,   215,   216,   217,
     217,   217,   218,   218,   218,   218,   218,   218,   219,   219,
     220,   220,   221,   221,   221,   222,   222,   222,   222,   222,
     223,   223,   224,   224,   225,   225,   226,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   228,   228,   229,   229,   229,
     230,   230,   231,   231,   231,   231,   232,   232,   232,   232,
     233,   233,   233,   234,   235,   235,   235,   235,   236,   236,
     237,   238,   239,   240,   241,   242,   242,   243,   243,   244,
     245,   245,   246,   247,   247,   247,   248,   248,   249,   249,
     250,   250,   251,   251,   252,   253,   253,   253,   254,   254,
     255,   256,   256,   257,   258,   258,   259,   260,   260,   261,
     261,   262,   262,   263,   263,   264,   264,   265,   266,   266,
     266,   267,   267,   268,   268,   269,   269,   269,   269,   269,
     269,   270,   270,   271,   271,   272,   272,   273,   273,   274,
     274,   274,   275,   276,   276,   276,   276,   276,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   278,
     278,   279,   279,   279,   279,   279,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   281,   281,   282,
     282,   282,   282,   283,   283,   283,   283,   284,   284,   285,
     285,   286,   286,   286,   286,   286,   286,   286,   287,   287,
     287,   287,   288,   288,   289,   289,   290,   290,   290,   291,
     291,   291,   292,   292,   292,   292,   292,   293,   294,   295,
     295,   295,   295,   295,   295,   296,   296,   296,   296,   296,
     296,   297,   297,   298,   299,   300,   301,   301,   301,   301,
     302,   302,   302,   302,   302,   302,   302,   303,   304,   304,
     305,   305,   306,   307,   308,   309,   309,   309,   310,   310,
     310,   311,   311,   312,   312,   313,   313,   314,   314,   315,
     316,   317,   317,   318,   318,   319,   319,   320,   320,   321,
     321,   322,   322,   323,   323,   324,   324,   325,   325,   326,
     326,   326,   326,   327,   327,   327,   328,   328,   329,   330,
     330,   331,   332,   332,   332,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   334,   335,   335,   336,   336
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
       0,     0,     2,     2,     0,     2,     2,     4,     2,     4,
       0,     1,     3,     1,     0,     1,     3,     1,     6,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     2,     2,     2,     3,     4,     1,     3,     3,     3,
       3,     3,     3,     3,     4,     3,     3,     3,     3,     5,
       6,     5,     6,     4,     6,     3,     5,     4,     5,     4,
       5,     3,     3,     3,     3,     3,     3,     3,     4,     6,
       6,     8,     6,     0,     3,     0,     2,     5,     0,     1,
       1,     1,     2,     2,     2,     2,     1,     6,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     5,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     3,     2,     1,     1,
       1,     1,     3,     1,     1,     1,     4,     1,     3,     2,
       1,     1,     3,     1,     0,     1,     5,     1,     0,     2,
       1,     1,     0,     1,     0,     1,     2,     3,     5,     1,
       3,     1,     2,     2,     1,     0,     1,     0,     2,     1,
       3,     3,     4,     6,     8,     1,     2,     1,     2,     1,
       2,     1,     1,     1,     0,     1,     1,     0,     1,     3
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
#line 2120 "bison_parser.cpp"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2126 "bison_parser.cpp"
        break;

    case YYSYMBOL_FLOATVAL: /* FLOATVAL  */
#line 196 "bison_parser.y"
            { }
#line 2132 "bison_parser.cpp"
        break;

    case YYSYMBOL_INTVAL: /* INTVAL  */
#line 196 "bison_parser.y"
            { }
#line 2138 "bison_parser.cpp"
        break;

    case YYSYMBOL_DOLLAR_PARAM: /* DOLLAR_PARAM  */
#line 196 "bison_parser.y"
            { }
#line 2144 "bison_parser.cpp"
        break;

    case YYSYMBOL_NAMED_PARAM: /* NAMED_PARAM  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2150 "bison_parser.cpp"
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
#line 2163 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2169 "bison_parser.cpp"
        break;

    case YYSYMBOL_preparable_statement: /* preparable_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2175 "bison_parser.cpp"
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
#line 2188 "bison_parser.cpp"
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
#line 2201 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint: /* hint  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2207 "bison_parser.cpp"
        break;

    case YYSYMBOL_transaction_statement: /* transaction_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).transaction_stmt)); }
#line 2213 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_statement: /* prepare_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).prep_stmt)); }
#line 2219 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_target_query: /* prepare_target_query  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2225 "bison_parser.cpp"
        break;

    case YYSYMBOL_execute_statement: /* execute_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2231 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_statement: /* import_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_stmt)); }
#line 2237 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_type: /* file_type  */
#line 196 "bison_parser.y"
            { }
#line 2243 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_path: /* file_path  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2249 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_import_export_options: /* opt_import_export_options  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2255 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_export_options: /* import_export_options  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2261 "bison_parser.cpp"
        break;

    case YYSYMBOL_csv_option: /* csv_option  */
#line 218 "bison_parser.y"
            {
  free(((*yyvaluep).csv_option_t)->second);
  delete (((*yyvaluep).csv_option_t));
}
#line 2270 "bison_parser.cpp"
        break;

    case YYSYMBOL_export_statement: /* export_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).export_stmt)); }
#line 2276 "bison_parser.cpp"
        break;

    case YYSYMBOL_show_statement: /* show_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).show_stmt)); }
#line 2282 "bison_parser.cpp"
        break;

    case YYSYMBOL_create_statement: /* create_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).create_stmt)); }
#line 2288 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_not_exists: /* opt_not_exists  */
#line 196 "bison_parser.y"
            { }
#line 2294 "bison_parser.cpp"
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
#line 2307 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem: /* table_elem  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table_element_t)); }
#line 2313 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_def: /* column_def  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).column_t)); }
#line 2319 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_type: /* column_type  */
#line 196 "bison_parser.y"
            { }
#line 2325 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_time_precision: /* opt_time_precision  */
#line 196 "bison_parser.y"
            { }
#line 2331 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_decimal_specification: /* opt_decimal_specification  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).ival_pair)); }
#line 2337 "bison_parser.cpp"
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
#line 2359 "bison_parser.cpp"
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
#line 2381 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraint: /* column_constraint  */
#line 196 "bison_parser.y"
            { }
#line 2387 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_constraint: /* table_constraint  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table_constraint_t)); }
#line 2393 "bison_parser.cpp"
        break;

    case YYSYMBOL_references_spec: /* references_spec  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).references_spec_t)); }
#line 2399 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_statement: /* drop_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).drop_stmt)); }
#line 2405 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_exists: /* opt_exists  */
#line 196 "bison_parser.y"
            { }
#line 2411 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_statement: /* alter_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alter_stmt)); }
#line 2417 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_action: /* alter_action  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alter_action_t)); }
#line 2423 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_action: /* drop_action  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).drop_action_t)); }
#line 2429 "bison_parser.cpp"
        break;

    case YYSYMBOL_delete_statement: /* delete_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2435 "bison_parser.cpp"
        break;

    case YYSYMBOL_truncate_statement: /* truncate_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2441 "bison_parser.cpp"
        break;

    case YYSYMBOL_insert_statement: /* insert_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).insert_stmt)); }
#line 2447 "bison_parser.cpp"
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
#line 2460 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_statement: /* update_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).update_stmt)); }
#line 2466 "bison_parser.cpp"
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
#line 2479 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause: /* update_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).update_t)); }
#line 2485 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_statement: /* select_statement  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2491 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation: /* select_within_set_operation  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2497 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation_no_parentheses: /* select_within_set_operation_no_parentheses  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2503 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_with_paren: /* select_with_paren  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2509 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_no_paren: /* select_no_paren  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2515 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_operator: /* set_operator  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2521 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_type: /* set_type  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2527 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_all: /* opt_all  */
#line 196 "bison_parser.y"
            { }
#line 2533 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_clause: /* select_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2539 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_distinct: /* opt_distinct  */
#line 196 "bison_parser.y"
            { }
#line 2545 "bison_parser.cpp"
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
#line 2558 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_from_clause: /* opt_from_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2564 "bison_parser.cpp"
        break;

    case YYSYMBOL_from_clause: /* from_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2570 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_where: /* opt_where  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2576 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_group: /* opt_group  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).group_t)); }
#line 2582 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_having: /* opt_having  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2588 "bison_parser.cpp"
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
#line 2601 "bison_parser.cpp"
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
#line 2614 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_desc: /* order_desc  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).order)); }
#line 2620 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order_type: /* opt_order_type  */
#line 196 "bison_parser.y"
            { }
#line 2626 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_null_ordering: /* opt_null_ordering  */
#line 196 "bison_parser.y"
            { }
#line 2632 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_top: /* opt_top  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 2638 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_limit: /* opt_limit  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 2644 "bison_parser.cpp"
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
#line 2657 "bison_parser.cpp"
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
#line 2670 "bison_parser.cpp"
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
#line 2683 "bison_parser.cpp"
        break;

    case YYSYMBOL_casted_extended_literal: /* casted_extended_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2689 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal: /* extended_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2695 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_alias: /* expr_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2701 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr: /* expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2707 "bison_parser.cpp"
        break;

    case YYSYMBOL_operand: /* operand  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2713 "bison_parser.cpp"
        break;

    case YYSYMBOL_scalar_expr: /* scalar_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2719 "bison_parser.cpp"
        break;

    case YYSYMBOL_unary_expr: /* unary_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2725 "bison_parser.cpp"
        break;

    case YYSYMBOL_binary_expr: /* binary_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2731 "bison_parser.cpp"
        break;

    case YYSYMBOL_logic_expr: /* logic_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2737 "bison_parser.cpp"
        break;

    case YYSYMBOL_in_expr: /* in_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2743 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_expr: /* case_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2749 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_list: /* case_list  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2755 "bison_parser.cpp"
        break;

    case YYSYMBOL_exists_expr: /* exists_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2761 "bison_parser.cpp"
        break;

    case YYSYMBOL_comp_expr: /* comp_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2767 "bison_parser.cpp"
        break;

    case YYSYMBOL_function_expr: /* function_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2773 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_window: /* opt_window  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).window_description)); }
#line 2779 "bison_parser.cpp"
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
#line 2792 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_frame_clause: /* opt_frame_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).frame_description)); }
#line 2798 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_type: /* frame_type  */
#line 196 "bison_parser.y"
            { }
#line 2804 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_bound: /* frame_bound  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).frame_bound)); }
#line 2810 "bison_parser.cpp"
        break;

    case YYSYMBOL_extract_expr: /* extract_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2816 "bison_parser.cpp"
        break;

    case YYSYMBOL_cast_expr: /* cast_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2822 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field: /* datetime_field  */
#line 196 "bison_parser.y"
            { }
#line 2828 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field_plural: /* datetime_field_plural  */
#line 196 "bison_parser.y"
            { }
#line 2834 "bison_parser.cpp"
        break;

    case YYSYMBOL_duration_field: /* duration_field  */
#line 196 "bison_parser.y"
            { }
#line 2840 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_expr: /* array_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2846 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_index: /* array_index  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2852 "bison_parser.cpp"
        break;

    case YYSYMBOL_between_expr: /* between_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2858 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_name: /* column_name  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2864 "bison_parser.cpp"
        break;

    case YYSYMBOL_literal: /* literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2870 "bison_parser.cpp"
        break;

    case YYSYMBOL_string_literal: /* string_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2876 "bison_parser.cpp"
        break;

    case YYSYMBOL_bool_literal: /* bool_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2882 "bison_parser.cpp"
        break;

    case YYSYMBOL_num_literal: /* num_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2888 "bison_parser.cpp"
        break;

    case YYSYMBOL_int_literal: /* int_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2894 "bison_parser.cpp"
        break;

    case YYSYMBOL_null_literal: /* null_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2900 "bison_parser.cpp"
        break;

    case YYSYMBOL_date_literal: /* date_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2906 "bison_parser.cpp"
        break;

    case YYSYMBOL_interval_literal: /* interval_literal  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2912 "bison_parser.cpp"
        break;

    case YYSYMBOL_param_expr: /* param_expr  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2918 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref: /* table_ref  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2924 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_atomic: /* table_ref_atomic  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2930 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonjoin_table_ref_atomic: /* nonjoin_table_ref_atomic  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2936 "bison_parser.cpp"
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
#line 2949 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name: /* table_ref_name  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2955 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name_no_alias: /* table_ref_name_no_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2961 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_name: /* table_name  */
#line 197 "bison_parser.y"
            {
  free(((*yyvaluep).table_name).name);
  free(((*yyvaluep).table_name).schema);
}
#line 2970 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_index_name: /* opt_index_name  */
#line 209 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2976 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_alias: /* table_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 2982 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_table_alias: /* opt_table_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 2988 "bison_parser.cpp"
        break;

    case YYSYMBOL_alias: /* alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 2994 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_alias: /* opt_alias  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3000 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause: /* opt_locking_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3006 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause_list: /* opt_locking_clause_list  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3012 "bison_parser.cpp"
        break;

    case YYSYMBOL_locking_clause: /* locking_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).locking_t)); }
#line 3018 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_lock_mode: /* row_lock_mode  */
#line 196 "bison_parser.y"
            { }
#line 3024 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_row_lock_policy: /* opt_row_lock_policy  */
#line 196 "bison_parser.y"
            { }
#line 3030 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_with_clause: /* opt_with_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).with_description_vec)); }
#line 3036 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_clause: /* with_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).with_description_vec)); }
#line 3042 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description_list: /* with_description_list  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).with_description_vec)); }
#line 3048 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description: /* with_description  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).with_description_t)); }
#line 3054 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_clause: /* join_clause  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3060 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_join_type: /* opt_join_type  */
#line 196 "bison_parser.y"
            { }
#line 3066 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_condition: /* join_condition  */
#line 239 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3072 "bison_parser.cpp"
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
#line 3085 "bison_parser.cpp"
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

#line 3193 "bison_parser.cpp"

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
#line 377 "bison_parser.y"
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
#line 3423 "bison_parser.cpp"
    break;

  case 3: /* statement_list: statement  */
#line 397 "bison_parser.y"
                           {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyval.stmt_vec) = new std::vector<SQLStatement*>();
  (yyval.stmt_vec)->push_back((yyvsp[0].statement));
}
#line 3434 "bison_parser.cpp"
    break;

  case 4: /* statement_list: statement_list ';' statement  */
#line 403 "bison_parser.y"
                               {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyvsp[-2].stmt_vec)->push_back((yyvsp[0].statement));
  (yyval.stmt_vec) = (yyvsp[-2].stmt_vec);
}
#line 3445 "bison_parser.cpp"
    break;

  case 5: /* statement: prepare_statement opt_hints  */
#line 410 "bison_parser.y"
                                        {
  (yyval.statement) = (yyvsp[-1].prep_stmt);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3454 "bison_parser.cpp"
    break;

  case 6: /* statement: preparable_statement opt_hints  */
#line 414 "bison_parser.y"
                                 {
  (yyval.statement) = (yyvsp[-1].statement);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3463 "bison_parser.cpp"
    break;

  case 7: /* statement: show_statement  */
#line 418 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].show_stmt); }
#line 3469 "bison_parser.cpp"
    break;

  case 8: /* statement: import_statement  */
#line 419 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].import_stmt); }
#line 3475 "bison_parser.cpp"
    break;

  case 9: /* statement: export_statement  */
#line 420 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].export_stmt); }
#line 3481 "bison_parser.cpp"
    break;

  case 10: /* preparable_statement: select_statement  */
#line 422 "bison_parser.y"
                                        { (yyval.statement) = (yyvsp[0].select_stmt); }
#line 3487 "bison_parser.cpp"
    break;

  case 11: /* preparable_statement: create_statement  */
#line 423 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].create_stmt); }
#line 3493 "bison_parser.cpp"
    break;

  case 12: /* preparable_statement: insert_statement  */
#line 424 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].insert_stmt); }
#line 3499 "bison_parser.cpp"
    break;

  case 13: /* preparable_statement: delete_statement  */
#line 425 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3505 "bison_parser.cpp"
    break;

  case 14: /* preparable_statement: truncate_statement  */
#line 426 "bison_parser.y"
                     { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3511 "bison_parser.cpp"
    break;

  case 15: /* preparable_statement: update_statement  */
#line 427 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].update_stmt); }
#line 3517 "bison_parser.cpp"
    break;

  case 16: /* preparable_statement: drop_statement  */
#line 428 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].drop_stmt); }
#line 3523 "bison_parser.cpp"
    break;

  case 17: /* preparable_statement: alter_statement  */
#line 429 "bison_parser.y"
                  { (yyval.statement) = (yyvsp[0].alter_stmt); }
#line 3529 "bison_parser.cpp"
    break;

  case 18: /* preparable_statement: execute_statement  */
#line 430 "bison_parser.y"
                    { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 3535 "bison_parser.cpp"
    break;

  case 19: /* preparable_statement: transaction_statement  */
#line 431 "bison_parser.y"
                        { (yyval.statement) = (yyvsp[0].transaction_stmt); }
#line 3541 "bison_parser.cpp"
    break;

  case 20: /* opt_hints: WITH HINT '(' hint_list ')'  */
#line 437 "bison_parser.y"
                                        { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 3547 "bison_parser.cpp"
    break;

  case 21: /* opt_hints: %empty  */
#line 438 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 3553 "bison_parser.cpp"
    break;

  case 22: /* hint_list: hint  */
#line 440 "bison_parser.y"
                 {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 3562 "bison_parser.cpp"
    break;

  case 23: /* hint_list: hint_list ',' hint  */
#line 444 "bison_parser.y"
                     {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 3571 "bison_parser.cpp"
    break;

  case 24: /* hint: IDENTIFIER  */
#line 449 "bison_parser.y"
                  {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[0].sval);
}
#line 3580 "bison_parser.cpp"
    break;

  case 25: /* hint: IDENTIFIER '(' extended_literal_list ')'  */
#line 453 "bison_parser.y"
                                           {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[-3].sval);
  (yyval.expr)->exprList = (yyvsp[-1].expr_vec);
}
#line 3590 "bison_parser.cpp"
    break;

  case 26: /* transaction_statement: BEGIN opt_transaction_keyword  */
#line 463 "bison_parser.y"
                                                      { (yyval.transaction_stmt) = new TransactionStatement(kBeginTransaction); }
#line 3596 "bison_parser.cpp"
    break;

  case 27: /* transaction_statement: ROLLBACK opt_transaction_keyword  */
#line 464 "bison_parser.y"
                                   { (yyval.transaction_stmt) = new TransactionStatement(kRollbackTransaction); }
#line 3602 "bison_parser.cpp"
    break;

  case 28: /* transaction_statement: COMMIT opt_transaction_keyword  */
#line 465 "bison_parser.y"
                                 { (yyval.transaction_stmt) = new TransactionStatement(kCommitTransaction); }
#line 3608 "bison_parser.cpp"
    break;

  case 31: /* prepare_statement: PREPARE IDENTIFIER FROM prepare_target_query  */
#line 473 "bison_parser.y"
                                                                 {
  (yyval.prep_stmt) = new PrepareStatement();
  (yyval.prep_stmt)->name = (yyvsp[-2].sval);
  (yyval.prep_stmt)->query = (yyvsp[0].sval);
}
#line 3618 "bison_parser.cpp"
    break;

  case 33: /* execute_statement: EXECUTE IDENTIFIER  */
#line 481 "bison_parser.y"
                                       {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[0].sval);
}
#line 3627 "bison_parser.cpp"
    break;

  case 34: /* execute_statement: EXECUTE IDENTIFIER '(' opt_extended_literal_list ')'  */
#line 485 "bison_parser.y"
                                                       {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[-3].sval);
  (yyval.exec_stmt)->parameters = (yyvsp[-1].expr_vec);
}
#line 3637 "bison_parser.cpp"
    break;

  case 35: /* import_statement: IMPORT FROM file_type FILE file_path INTO table_name  */
#line 497 "bison_parser.y"
                                                                        {
  (yyval.import_stmt) = new ImportStatement((yyvsp[-4].import_type_t));
  (yyval.import_stmt)->filePath = (yyvsp[-2].sval);
  (yyval.import_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.import_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 3648 "bison_parser.cpp"
    break;

  case 36: /* import_statement: COPY table_name FROM file_path opt_import_export_options opt_where  */
#line 503 "bison_parser.y"
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
#line 3669 "bison_parser.cpp"
    break;

  case 37: /* file_type: IDENTIFIER  */
#line 520 "bison_parser.y"
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
#line 3688 "bison_parser.cpp"
    break;

  case 38: /* file_path: STRING  */
#line 535 "bison_parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 3694 "bison_parser.cpp"
    break;

  case 39: /* opt_import_export_options: WITH '(' import_export_options ')'  */
#line 537 "bison_parser.y"
                                                               { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 3700 "bison_parser.cpp"
    break;

  case 40: /* opt_import_export_options: '(' import_export_options ')'  */
#line 538 "bison_parser.y"
                                { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 3706 "bison_parser.cpp"
    break;

  case 41: /* opt_import_export_options: %empty  */
#line 539 "bison_parser.y"
              { (yyval.import_export_option_t) = new ImportExportOptions{}; }
#line 3712 "bison_parser.cpp"
    break;

  case 42: /* import_export_options: import_export_options ',' FORMAT file_type  */
#line 541 "bison_parser.y"
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
#line 3731 "bison_parser.cpp"
    break;

  case 43: /* import_export_options: FORMAT file_type  */
#line 555 "bison_parser.y"
                   {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->format = (yyvsp[0].import_type_t);
}
#line 3740 "bison_parser.cpp"
    break;

  case 44: /* import_export_options: import_export_options ',' ENCODING STRING  */
#line 559 "bison_parser.y"
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
#line 3755 "bison_parser.cpp"
    break;

  case 45: /* import_export_options: ENCODING STRING  */
#line 569 "bison_parser.y"
                  {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->encoding = (yyvsp[0].sval);
}
#line 3764 "bison_parser.cpp"
    break;

  case 46: /* import_export_options: import_export_options ',' csv_option  */
#line 573 "bison_parser.y"
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
#line 3793 "bison_parser.cpp"
    break;

  case 47: /* import_export_options: csv_option  */
#line 597 "bison_parser.y"
             {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->csv_options = new CsvOptions{};
  (yyval.import_export_option_t)->csv_options->accept_csv_option((yyvsp[0].csv_option_t));

  delete (yyvsp[0].csv_option_t);
}
#line 3805 "bison_parser.cpp"
    break;

  case 48: /* csv_option: IDENTIFIER STRING  */
#line 605 "bison_parser.y"
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
#line 3823 "bison_parser.cpp"
    break;

  case 49: /* csv_option: NULL STRING  */
#line 618 "bison_parser.y"
              { (yyval.csv_option_t) = new std::pair<CsvOptionType, char*>(CsvOptionType::Null, (yyvsp[0].sval)); }
#line 3829 "bison_parser.cpp"
    break;

  case 50: /* export_statement: COPY table_name TO file_path opt_import_export_options  */
#line 625 "bison_parser.y"
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
#line 3849 "bison_parser.cpp"
    break;

  case 51: /* export_statement: COPY select_with_paren TO file_path opt_import_export_options  */
#line 640 "bison_parser.y"
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
#line 3868 "bison_parser.cpp"
    break;

  case 52: /* show_statement: SHOW TABLES  */
#line 660 "bison_parser.y"
                             { (yyval.show_stmt) = new ShowStatement(kShowTables); }
#line 3874 "bison_parser.cpp"
    break;

  case 53: /* show_statement: SHOW COLUMNS table_name  */
#line 661 "bison_parser.y"
                          {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 3884 "bison_parser.cpp"
    break;

  case 54: /* show_statement: DESCRIBE table_name  */
#line 666 "bison_parser.y"
                      {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 3894 "bison_parser.cpp"
    break;

  case 55: /* create_statement: CREATE TABLE opt_not_exists table_name FROM IDENTIFIER FILE file_path  */
#line 677 "bison_parser.y"
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
#line 3912 "bison_parser.cpp"
    break;

  case 56: /* create_statement: CREATE TABLE opt_not_exists table_name '(' table_elem_commalist ')'  */
#line 690 "bison_parser.y"
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
#line 3929 "bison_parser.cpp"
    break;

  case 57: /* create_statement: CREATE TABLE opt_not_exists table_name AS select_statement  */
#line 702 "bison_parser.y"
                                                             {
  (yyval.create_stmt) = new CreateStatement(kCreateTable);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-3].bval);
  (yyval.create_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 3941 "bison_parser.cpp"
    break;

  case 58: /* create_statement: CREATE INDEX opt_not_exists opt_index_name ON table_name '(' ident_commalist ')'  */
#line 709 "bison_parser.y"
                                                                                   {
  (yyval.create_stmt) = new CreateStatement(kCreateIndex);
  (yyval.create_stmt)->indexName = (yyvsp[-5].sval);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-6].bval);
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->indexColumns = (yyvsp[-1].str_vec);
}
#line 3953 "bison_parser.cpp"
    break;

  case 59: /* create_statement: CREATE VIEW opt_not_exists table_name opt_column_list AS select_statement  */
#line 716 "bison_parser.y"
                                                                            {
  (yyval.create_stmt) = new CreateStatement(kCreateView);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-4].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->viewColumns = (yyvsp[-2].str_vec);
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 3966 "bison_parser.cpp"
    break;

  case 60: /* opt_not_exists: IF NOT EXISTS  */
#line 725 "bison_parser.y"
                               { (yyval.bval) = true; }
#line 3972 "bison_parser.cpp"
    break;

  case 61: /* opt_not_exists: %empty  */
#line 726 "bison_parser.y"
              { (yyval.bval) = false; }
#line 3978 "bison_parser.cpp"
    break;

  case 62: /* table_elem_commalist: table_elem  */
#line 728 "bison_parser.y"
                                  {
  (yyval.table_element_vec) = new std::vector<TableElement*>();
  (yyval.table_element_vec)->push_back((yyvsp[0].table_element_t));
}
#line 3987 "bison_parser.cpp"
    break;

  case 63: /* table_elem_commalist: table_elem_commalist ',' table_elem  */
#line 732 "bison_parser.y"
                                      {
  (yyvsp[-2].table_element_vec)->push_back((yyvsp[0].table_element_t));
  (yyval.table_element_vec) = (yyvsp[-2].table_element_vec);
}
#line 3996 "bison_parser.cpp"
    break;

  case 64: /* table_elem: column_def  */
#line 737 "bison_parser.y"
                        { (yyval.table_element_t) = (yyvsp[0].column_t); }
#line 4002 "bison_parser.cpp"
    break;

  case 65: /* table_elem: table_constraint  */
#line 738 "bison_parser.y"
                   { (yyval.table_element_t) = (yyvsp[0].table_constraint_t); }
#line 4008 "bison_parser.cpp"
    break;

  case 66: /* column_def: IDENTIFIER column_type opt_column_constraints  */
#line 740 "bison_parser.y"
                                                           {
  (yyval.column_t) = new ColumnDefinition((yyvsp[-2].sval), (yyvsp[-1].column_type_t), (yyvsp[0].column_constraints_t)->constraints, (yyvsp[0].column_constraints_t)->references);
  if (!(yyval.column_t)->trySetNullableExplicit()) {
    yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{(yyvsp[-2].sval)}).c_str());
  }
  delete (yyvsp[0].column_constraints_t);
}
#line 4020 "bison_parser.cpp"
    break;

  case 67: /* column_type: BIGINT  */
#line 748 "bison_parser.y"
                     { (yyval.column_type_t) = ColumnType{DataType::BIGINT}; }
#line 4026 "bison_parser.cpp"
    break;

  case 68: /* column_type: BOOLEAN  */
#line 749 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::BOOLEAN}; }
#line 4032 "bison_parser.cpp"
    break;

  case 69: /* column_type: CHAR '(' INTVAL ')'  */
#line 750 "bison_parser.y"
                      { (yyval.column_type_t) = ColumnType{DataType::CHAR, (yyvsp[-1].ival)}; }
#line 4038 "bison_parser.cpp"
    break;

  case 70: /* column_type: CHARACTER_VARYING '(' INTVAL ')'  */
#line 751 "bison_parser.y"
                                   { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4044 "bison_parser.cpp"
    break;

  case 71: /* column_type: DATE  */
#line 752 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::DATE}; }
#line 4050 "bison_parser.cpp"
    break;

  case 72: /* column_type: DATETIME  */
#line 753 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4056 "bison_parser.cpp"
    break;

  case 73: /* column_type: DECIMAL opt_decimal_specification  */
#line 754 "bison_parser.y"
                                    {
  (yyval.column_type_t) = ColumnType{DataType::DECIMAL, 0, (yyvsp[0].ival_pair)->first, (yyvsp[0].ival_pair)->second};
  delete (yyvsp[0].ival_pair);
}
#line 4065 "bison_parser.cpp"
    break;

  case 74: /* column_type: DOUBLE  */
#line 758 "bison_parser.y"
         { (yyval.column_type_t) = ColumnType{DataType::DOUBLE}; }
#line 4071 "bison_parser.cpp"
    break;

  case 75: /* column_type: FLOAT  */
#line 759 "bison_parser.y"
        { (yyval.column_type_t) = ColumnType{DataType::FLOAT}; }
#line 4077 "bison_parser.cpp"
    break;

  case 76: /* column_type: INT  */
#line 760 "bison_parser.y"
      { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4083 "bison_parser.cpp"
    break;

  case 77: /* column_type: INTEGER  */
#line 761 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4089 "bison_parser.cpp"
    break;

  case 78: /* column_type: LONG  */
#line 762 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::LONG}; }
#line 4095 "bison_parser.cpp"
    break;

  case 79: /* column_type: REAL  */
#line 763 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::REAL}; }
#line 4101 "bison_parser.cpp"
    break;

  case 80: /* column_type: SMALLINT  */
#line 764 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::SMALLINT}; }
#line 4107 "bison_parser.cpp"
    break;

  case 81: /* column_type: TEXT  */
#line 765 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::TEXT}; }
#line 4113 "bison_parser.cpp"
    break;

  case 82: /* column_type: TIME opt_time_precision  */
#line 766 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::TIME, 0, (yyvsp[0].ival)}; }
#line 4119 "bison_parser.cpp"
    break;

  case 83: /* column_type: TIMESTAMP  */
#line 767 "bison_parser.y"
            { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4125 "bison_parser.cpp"
    break;

  case 84: /* column_type: VARCHAR '(' INTVAL ')'  */
#line 768 "bison_parser.y"
                         { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4131 "bison_parser.cpp"
    break;

  case 85: /* opt_time_precision: '(' INTVAL ')'  */
#line 770 "bison_parser.y"
                                    { (yyval.ival) = (yyvsp[-1].ival); }
#line 4137 "bison_parser.cpp"
    break;

  case 86: /* opt_time_precision: %empty  */
#line 771 "bison_parser.y"
              { (yyval.ival) = 0; }
#line 4143 "bison_parser.cpp"
    break;

  case 87: /* opt_decimal_specification: '(' INTVAL ',' INTVAL ')'  */
#line 773 "bison_parser.y"
                                                      { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-3].ival), (yyvsp[-1].ival)}; }
#line 4149 "bison_parser.cpp"
    break;

  case 88: /* opt_decimal_specification: '(' INTVAL ')'  */
#line 774 "bison_parser.y"
                 { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-1].ival), 0}; }
#line 4155 "bison_parser.cpp"
    break;

  case 89: /* opt_decimal_specification: %empty  */
#line 775 "bison_parser.y"
              { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{0, 0}; }
#line 4161 "bison_parser.cpp"
    break;

  case 90: /* opt_column_constraints: column_constraints  */
#line 777 "bison_parser.y"
                                            { (yyval.column_constraints_t) = (yyvsp[0].column_constraints_t); }
#line 4167 "bison_parser.cpp"
    break;

  case 91: /* opt_column_constraints: %empty  */
#line 778 "bison_parser.y"
              { (yyval.column_constraints_t) = new ColumnConstraints(); }
#line 4173 "bison_parser.cpp"
    break;

  case 92: /* column_constraints: column_constraint  */
#line 780 "bison_parser.y"
                                       {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
}
#line 4182 "bison_parser.cpp"
    break;

  case 93: /* column_constraints: column_constraints column_constraint  */
#line 784 "bison_parser.y"
                                       {
  (yyvsp[-1].column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4191 "bison_parser.cpp"
    break;

  case 94: /* column_constraints: references_spec  */
#line 788 "bison_parser.y"
                  {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyval.column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
}
#line 4201 "bison_parser.cpp"
    break;

  case 95: /* column_constraints: column_constraints references_spec  */
#line 793 "bison_parser.y"
                                     {
  // Multiple foreign keys for the same column could be possible, so we do not raise an error in that case.
  // Think of foreign keys referenced on multiple levels (returned item references sold item references items).
  (yyvsp[-1].column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyvsp[-1].column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4213 "bison_parser.cpp"
    break;

  case 96: /* column_constraint: PRIMARY KEY  */
#line 801 "bison_parser.y"
                                { (yyval.column_constraint_t) = ConstraintType::PrimaryKey; }
#line 4219 "bison_parser.cpp"
    break;

  case 97: /* column_constraint: UNIQUE  */
#line 802 "bison_parser.y"
         { (yyval.column_constraint_t) = ConstraintType::Unique; }
#line 4225 "bison_parser.cpp"
    break;

  case 98: /* column_constraint: NULL  */
#line 803 "bison_parser.y"
       { (yyval.column_constraint_t) = ConstraintType::Null; }
#line 4231 "bison_parser.cpp"
    break;

  case 99: /* column_constraint: NOT NULL  */
#line 804 "bison_parser.y"
           { (yyval.column_constraint_t) = ConstraintType::NotNull; }
#line 4237 "bison_parser.cpp"
    break;

  case 100: /* table_constraint: PRIMARY KEY '(' ident_commalist ')'  */
#line 806 "bison_parser.y"
                                                       { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::PrimaryKey, (yyvsp[-1].str_vec)); }
#line 4243 "bison_parser.cpp"
    break;

  case 101: /* table_constraint: UNIQUE '(' ident_commalist ')'  */
#line 807 "bison_parser.y"
                                 { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::Unique, (yyvsp[-1].str_vec)); }
#line 4249 "bison_parser.cpp"
    break;

  case 102: /* table_constraint: FOREIGN KEY '(' ident_commalist ')' references_spec  */
#line 808 "bison_parser.y"
                                                      { (yyval.table_constraint_t) = new ForeignKeyConstraint((yyvsp[-2].str_vec), (yyvsp[0].references_spec_t)); }
#line 4255 "bison_parser.cpp"
    break;

  case 103: /* references_spec: REFERENCES table_name opt_column_list  */
#line 810 "bison_parser.y"
                                                        { (yyval.references_spec_t) = new ReferencesSpecification((yyvsp[-1].table_name).schema, (yyvsp[-1].table_name).name, (yyvsp[0].str_vec)); }
#line 4261 "bison_parser.cpp"
    break;

  case 104: /* drop_statement: DROP TABLE opt_exists table_name  */
#line 818 "bison_parser.y"
                                                  {
  (yyval.drop_stmt) = new DropStatement(kDropTable);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4272 "bison_parser.cpp"
    break;

  case 105: /* drop_statement: DROP VIEW opt_exists table_name  */
#line 824 "bison_parser.y"
                                  {
  (yyval.drop_stmt) = new DropStatement(kDropView);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4283 "bison_parser.cpp"
    break;

  case 106: /* drop_statement: DEALLOCATE PREPARE IDENTIFIER  */
#line 830 "bison_parser.y"
                                {
  (yyval.drop_stmt) = new DropStatement(kDropPreparedStatement);
  (yyval.drop_stmt)->ifExists = false;
  (yyval.drop_stmt)->name = (yyvsp[0].sval);
}
#line 4293 "bison_parser.cpp"
    break;

  case 107: /* drop_statement: DROP INDEX opt_exists IDENTIFIER  */
#line 836 "bison_parser.y"
                                   {
  (yyval.drop_stmt) = new DropStatement(kDropIndex);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->indexName = (yyvsp[0].sval);
}
#line 4303 "bison_parser.cpp"
    break;

  case 108: /* opt_exists: IF EXISTS  */
#line 842 "bison_parser.y"
                       { (yyval.bval) = true; }
#line 4309 "bison_parser.cpp"
    break;

  case 109: /* opt_exists: %empty  */
#line 843 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4315 "bison_parser.cpp"
    break;

  case 110: /* alter_statement: ALTER TABLE opt_exists table_name alter_action  */
#line 850 "bison_parser.y"
                                                                 {
  (yyval.alter_stmt) = new AlterStatement((yyvsp[-1].table_name).name, (yyvsp[0].alter_action_t));
  (yyval.alter_stmt)->ifTableExists = (yyvsp[-2].bval);
  (yyval.alter_stmt)->schema = (yyvsp[-1].table_name).schema;
}
#line 4325 "bison_parser.cpp"
    break;

  case 111: /* alter_action: drop_action  */
#line 856 "bison_parser.y"
                           { (yyval.alter_action_t) = (yyvsp[0].drop_action_t); }
#line 4331 "bison_parser.cpp"
    break;

  case 112: /* drop_action: DROP COLUMN opt_exists IDENTIFIER  */
#line 858 "bison_parser.y"
                                                {
  (yyval.drop_action_t) = new DropColumnAction((yyvsp[0].sval));
  (yyval.drop_action_t)->ifExists = (yyvsp[-1].bval);
}
#line 4340 "bison_parser.cpp"
    break;

  case 113: /* delete_statement: DELETE FROM table_name opt_where  */
#line 868 "bison_parser.y"
                                                    {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[-1].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[-1].table_name).name;
  (yyval.delete_stmt)->expr = (yyvsp[0].expr);
}
#line 4351 "bison_parser.cpp"
    break;

  case 114: /* truncate_statement: TRUNCATE table_name  */
#line 875 "bison_parser.y"
                                         {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 4361 "bison_parser.cpp"
    break;

  case 115: /* insert_statement: INSERT INTO table_name opt_column_list VALUES '(' extended_literal_list ')'  */
#line 886 "bison_parser.y"
                                                                                               {
  (yyval.insert_stmt) = new InsertStatement(kInsertValues);
  (yyval.insert_stmt)->schema = (yyvsp[-5].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-5].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-4].str_vec);
  (yyval.insert_stmt)->values = (yyvsp[-1].expr_vec);
}
#line 4373 "bison_parser.cpp"
    break;

  case 116: /* insert_statement: INSERT INTO table_name opt_column_list select_no_paren  */
#line 893 "bison_parser.y"
                                                         {
  (yyval.insert_stmt) = new InsertStatement(kInsertSelect);
  (yyval.insert_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-1].str_vec);
  (yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4385 "bison_parser.cpp"
    break;

  case 117: /* opt_column_list: '(' ident_commalist ')'  */
#line 901 "bison_parser.y"
                                          { (yyval.str_vec) = (yyvsp[-1].str_vec); }
#line 4391 "bison_parser.cpp"
    break;

  case 118: /* opt_column_list: %empty  */
#line 902 "bison_parser.y"
              { (yyval.str_vec) = nullptr; }
#line 4397 "bison_parser.cpp"
    break;

  case 119: /* update_statement: UPDATE table_ref_name_no_alias SET update_clause_commalist opt_where  */
#line 909 "bison_parser.y"
                                                                                        {
  (yyval.update_stmt) = new UpdateStatement();
  (yyval.update_stmt)->table = (yyvsp[-3].table);
  (yyval.update_stmt)->updates = (yyvsp[-1].update_vec);
  (yyval.update_stmt)->where = (yyvsp[0].expr);
}
#line 4408 "bison_parser.cpp"
    break;

  case 120: /* update_clause_commalist: update_clause  */
#line 916 "bison_parser.y"
                                        {
  (yyval.update_vec) = new std::vector<UpdateClause*>();
  (yyval.update_vec)->push_back((yyvsp[0].update_t));
}
#line 4417 "bison_parser.cpp"
    break;

  case 121: /* update_clause_commalist: update_clause_commalist ',' update_clause  */
#line 920 "bison_parser.y"
                                            {
  (yyvsp[-2].update_vec)->push_back((yyvsp[0].update_t));
  (yyval.update_vec) = (yyvsp[-2].update_vec);
}
#line 4426 "bison_parser.cpp"
    break;

  case 122: /* update_clause: IDENTIFIER '=' expr  */
#line 925 "bison_parser.y"
                                    {
  (yyval.update_t) = new UpdateClause();
  (yyval.update_t)->column = (yyvsp[-2].sval);
  (yyval.update_t)->value = (yyvsp[0].expr);
}
#line 4436 "bison_parser.cpp"
    break;

  case 123: /* select_statement: opt_with_clause select_with_paren  */
#line 935 "bison_parser.y"
                                                     {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4445 "bison_parser.cpp"
    break;

  case 124: /* select_statement: opt_with_clause select_no_paren  */
#line 939 "bison_parser.y"
                                  {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4454 "bison_parser.cpp"
    break;

  case 125: /* select_statement: opt_with_clause select_with_paren set_operator select_within_set_operation opt_order opt_limit  */
#line 943 "bison_parser.y"
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
#line 4470 "bison_parser.cpp"
    break;

  case 128: /* select_within_set_operation_no_parentheses: select_clause  */
#line 957 "bison_parser.y"
                                                           { (yyval.select_stmt) = (yyvsp[0].select_stmt); }
#line 4476 "bison_parser.cpp"
    break;

  case 129: /* select_within_set_operation_no_parentheses: select_clause set_operator select_within_set_operation  */
#line 958 "bison_parser.y"
                                                         {
  (yyval.select_stmt) = (yyvsp[-2].select_stmt);
  if ((yyval.select_stmt)->setOperations == nullptr) {
    (yyval.select_stmt)->setOperations = new std::vector<SetOperation*>();
  }
  (yyval.select_stmt)->setOperations->push_back((yyvsp[-1].set_operator_t));
  (yyval.select_stmt)->setOperations->back()->nestedSelectStatement = (yyvsp[0].select_stmt);
}
#line 4489 "bison_parser.cpp"
    break;

  case 130: /* select_with_paren: '(' select_no_paren ')'  */
#line 967 "bison_parser.y"
                                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4495 "bison_parser.cpp"
    break;

  case 131: /* select_with_paren: '(' select_with_paren ')'  */
#line 968 "bison_parser.y"
                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4501 "bison_parser.cpp"
    break;

  case 132: /* select_no_paren: select_clause opt_order opt_limit opt_locking_clause  */
#line 970 "bison_parser.y"
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
#line 4520 "bison_parser.cpp"
    break;

  case 133: /* select_no_paren: select_clause set_operator select_within_set_operation opt_order opt_limit opt_locking_clause  */
#line 984 "bison_parser.y"
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
#line 4536 "bison_parser.cpp"
    break;

  case 134: /* set_operator: set_type opt_all  */
#line 996 "bison_parser.y"
                                {
  (yyval.set_operator_t) = (yyvsp[-1].set_operator_t);
  (yyval.set_operator_t)->isAll = (yyvsp[0].bval);
}
#line 4545 "bison_parser.cpp"
    break;

  case 135: /* set_type: UNION  */
#line 1001 "bison_parser.y"
                 {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetUnion;
}
#line 4554 "bison_parser.cpp"
    break;

  case 136: /* set_type: INTERSECT  */
#line 1005 "bison_parser.y"
            {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetIntersect;
}
#line 4563 "bison_parser.cpp"
    break;

  case 137: /* set_type: EXCEPT  */
#line 1009 "bison_parser.y"
         {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetExcept;
}
#line 4572 "bison_parser.cpp"
    break;

  case 138: /* opt_all: ALL  */
#line 1014 "bison_parser.y"
              { (yyval.bval) = true; }
#line 4578 "bison_parser.cpp"
    break;

  case 139: /* opt_all: %empty  */
#line 1015 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4584 "bison_parser.cpp"
    break;

  case 140: /* select_clause: SELECT opt_top opt_distinct select_list opt_from_clause opt_where opt_group  */
#line 1017 "bison_parser.y"
                                                                                            {
  (yyval.select_stmt) = new SelectStatement();
  (yyval.select_stmt)->limit = (yyvsp[-5].limit);
  (yyval.select_stmt)->selectDistinct = (yyvsp[-4].bval);
  (yyval.select_stmt)->selectList = (yyvsp[-3].expr_vec);
  (yyval.select_stmt)->fromTable = (yyvsp[-2].table);
  (yyval.select_stmt)->whereClause = (yyvsp[-1].expr);
  (yyval.select_stmt)->groupBy = (yyvsp[0].group_t);
}
#line 4598 "bison_parser.cpp"
    break;

  case 141: /* opt_distinct: DISTINCT  */
#line 1027 "bison_parser.y"
                        { (yyval.bval) = true; }
#line 4604 "bison_parser.cpp"
    break;

  case 142: /* opt_distinct: %empty  */
#line 1028 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4610 "bison_parser.cpp"
    break;

  case 144: /* opt_from_clause: from_clause  */
#line 1032 "bison_parser.y"
                              { (yyval.table) = (yyvsp[0].table); }
#line 4616 "bison_parser.cpp"
    break;

  case 145: /* opt_from_clause: %empty  */
#line 1033 "bison_parser.y"
              { (yyval.table) = nullptr; }
#line 4622 "bison_parser.cpp"
    break;

  case 146: /* from_clause: FROM table_ref  */
#line 1035 "bison_parser.y"
                             { (yyval.table) = (yyvsp[0].table); }
#line 4628 "bison_parser.cpp"
    break;

  case 147: /* opt_where: WHERE expr  */
#line 1037 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 4634 "bison_parser.cpp"
    break;

  case 148: /* opt_where: %empty  */
#line 1038 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4640 "bison_parser.cpp"
    break;

  case 149: /* opt_group: GROUP BY expr_list opt_having  */
#line 1040 "bison_parser.y"
                                          {
  (yyval.group_t) = new GroupByDescription();
  (yyval.group_t)->columns = (yyvsp[-1].expr_vec);
  (yyval.group_t)->having = (yyvsp[0].expr);
}
#line 4650 "bison_parser.cpp"
    break;

  case 150: /* opt_group: %empty  */
#line 1045 "bison_parser.y"
              { (yyval.group_t) = nullptr; }
#line 4656 "bison_parser.cpp"
    break;

  case 151: /* opt_having: HAVING expr  */
#line 1047 "bison_parser.y"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 4662 "bison_parser.cpp"
    break;

  case 152: /* opt_having: %empty  */
#line 1048 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4668 "bison_parser.cpp"
    break;

  case 153: /* opt_order: ORDER BY order_list  */
#line 1050 "bison_parser.y"
                                { (yyval.order_vec) = (yyvsp[0].order_vec); }
#line 4674 "bison_parser.cpp"
    break;

  case 154: /* opt_order: %empty  */
#line 1051 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 4680 "bison_parser.cpp"
    break;

  case 155: /* order_list: order_desc  */
#line 1053 "bison_parser.y"
                        {
  (yyval.order_vec) = new std::vector<OrderDescription*>();
  (yyval.order_vec)->push_back((yyvsp[0].order));
}
#line 4689 "bison_parser.cpp"
    break;

  case 156: /* order_list: order_list ',' order_desc  */
#line 1057 "bison_parser.y"
                            {
  (yyvsp[-2].order_vec)->push_back((yyvsp[0].order));
  (yyval.order_vec) = (yyvsp[-2].order_vec);
}
#line 4698 "bison_parser.cpp"
    break;

  case 157: /* order_desc: expr opt_order_type opt_null_ordering  */
#line 1062 "bison_parser.y"
                                                   { (yyval.order) = new OrderDescription((yyvsp[-1].order_type), (yyvsp[-2].expr), (yyvsp[0].null_ordering_t)); }
#line 4704 "bison_parser.cpp"
    break;

  case 158: /* opt_order_type: ASC  */
#line 1064 "bison_parser.y"
                     { (yyval.order_type) = kOrderAsc; }
#line 4710 "bison_parser.cpp"
    break;

  case 159: /* opt_order_type: DESC  */
#line 1065 "bison_parser.y"
       { (yyval.order_type) = kOrderDesc; }
#line 4716 "bison_parser.cpp"
    break;

  case 160: /* opt_order_type: %empty  */
#line 1066 "bison_parser.y"
              { (yyval.order_type) = kOrderAsc; }
#line 4722 "bison_parser.cpp"
    break;

  case 161: /* opt_null_ordering: %empty  */
#line 1068 "bison_parser.y"
                                { (yyval.null_ordering_t) = NullOrdering::Undefined; }
#line 4728 "bison_parser.cpp"
    break;

  case 162: /* opt_null_ordering: IDENTIFIER IDENTIFIER  */
#line 1069 "bison_parser.y"
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
#line 4752 "bison_parser.cpp"
    break;

  case 163: /* opt_top: TOP int_literal  */
#line 1091 "bison_parser.y"
                          { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 4758 "bison_parser.cpp"
    break;

  case 164: /* opt_top: %empty  */
#line 1092 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 4764 "bison_parser.cpp"
    break;

  case 165: /* opt_limit: LIMIT expr  */
#line 1094 "bison_parser.y"
                       { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 4770 "bison_parser.cpp"
    break;

  case 166: /* opt_limit: OFFSET expr  */
#line 1095 "bison_parser.y"
              { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 4776 "bison_parser.cpp"
    break;

  case 167: /* opt_limit: LIMIT expr OFFSET expr  */
#line 1096 "bison_parser.y"
                         { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 4782 "bison_parser.cpp"
    break;

  case 168: /* opt_limit: LIMIT ALL  */
#line 1097 "bison_parser.y"
            { (yyval.limit) = new LimitDescription(nullptr, nullptr); }
#line 4788 "bison_parser.cpp"
    break;

  case 169: /* opt_limit: LIMIT ALL OFFSET expr  */
#line 1098 "bison_parser.y"
                        { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 4794 "bison_parser.cpp"
    break;

  case 170: /* opt_limit: %empty  */
#line 1099 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 4800 "bison_parser.cpp"
    break;

  case 171: /* expr_list: expr_alias  */
#line 1104 "bison_parser.y"
                       {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 4809 "bison_parser.cpp"
    break;

  case 172: /* expr_list: expr_list ',' expr_alias  */
#line 1108 "bison_parser.y"
                           {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 4818 "bison_parser.cpp"
    break;

  case 173: /* opt_extended_literal_list: extended_literal_list  */
#line 1114 "bison_parser.y"
                                                  { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 4824 "bison_parser.cpp"
    break;

  case 174: /* opt_extended_literal_list: %empty  */
#line 1115 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 4830 "bison_parser.cpp"
    break;

  case 175: /* extended_literal_list: casted_extended_literal  */
#line 1117 "bison_parser.y"
                                                {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 4839 "bison_parser.cpp"
    break;

  case 176: /* extended_literal_list: extended_literal_list ',' casted_extended_literal  */
#line 1121 "bison_parser.y"
                                                    {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 4848 "bison_parser.cpp"
    break;

  case 178: /* casted_extended_literal: CAST '(' extended_literal AS column_type ')'  */
#line 1126 "bison_parser.y"
                                                                                          {
  (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t));
}
#line 4856 "bison_parser.cpp"
    break;

  case 179: /* extended_literal: literal  */
#line 1130 "bison_parser.y"
                           { (yyval.expr) = (yyvsp[0].expr); }
#line 4862 "bison_parser.cpp"
    break;

  case 180: /* extended_literal: '-' num_literal  */
#line 1131 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 4868 "bison_parser.cpp"
    break;

  case 181: /* extended_literal: '-' interval_literal  */
#line 1132 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 4874 "bison_parser.cpp"
    break;

  case 182: /* expr_alias: expr opt_alias  */
#line 1134 "bison_parser.y"
                            {
  (yyval.expr) = (yyvsp[-1].expr);
  if ((yyvsp[0].alias_t)) {
    (yyval.expr)->alias = (yyvsp[0].alias_t)->name;
    (yyvsp[0].alias_t)->name = nullptr;
    delete (yyvsp[0].alias_t);
  }
}
#line 4887 "bison_parser.cpp"
    break;

  case 188: /* operand: '(' expr ')'  */
#line 1145 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[-1].expr); }
#line 4893 "bison_parser.cpp"
    break;

  case 198: /* operand: '(' select_no_paren ')'  */
#line 1147 "bison_parser.y"
                                         {
  (yyval.expr) = Expr::makeSelect((yyvsp[-1].select_stmt));
}
#line 4901 "bison_parser.cpp"
    break;

  case 201: /* unary_expr: '-' operand  */
#line 1153 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 4907 "bison_parser.cpp"
    break;

  case 202: /* unary_expr: NOT operand  */
#line 1154 "bison_parser.y"
              { (yyval.expr) = Expr::makeOpUnary(kOpNot, (yyvsp[0].expr)); }
#line 4913 "bison_parser.cpp"
    break;

  case 203: /* unary_expr: operand ISNULL  */
#line 1155 "bison_parser.y"
                 { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-1].expr)); }
#line 4919 "bison_parser.cpp"
    break;

  case 204: /* unary_expr: operand IS NULL  */
#line 1156 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-2].expr)); }
#line 4925 "bison_parser.cpp"
    break;

  case 205: /* unary_expr: operand IS NOT NULL  */
#line 1157 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeOpUnary(kOpIsNull, (yyvsp[-3].expr))); }
#line 4931 "bison_parser.cpp"
    break;

  case 207: /* binary_expr: operand '-' operand  */
#line 1159 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpMinus, (yyvsp[0].expr)); }
#line 4937 "bison_parser.cpp"
    break;

  case 208: /* binary_expr: operand '+' operand  */
#line 1160 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPlus, (yyvsp[0].expr)); }
#line 4943 "bison_parser.cpp"
    break;

  case 209: /* binary_expr: operand '/' operand  */
#line 1161 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpSlash, (yyvsp[0].expr)); }
#line 4949 "bison_parser.cpp"
    break;

  case 210: /* binary_expr: operand '*' operand  */
#line 1162 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAsterisk, (yyvsp[0].expr)); }
#line 4955 "bison_parser.cpp"
    break;

  case 211: /* binary_expr: operand '%' operand  */
#line 1163 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPercentage, (yyvsp[0].expr)); }
#line 4961 "bison_parser.cpp"
    break;

  case 212: /* binary_expr: operand '^' operand  */
#line 1164 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpCaret, (yyvsp[0].expr)); }
#line 4967 "bison_parser.cpp"
    break;

  case 213: /* binary_expr: operand LIKE operand  */
#line 1165 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLike, (yyvsp[0].expr)); }
#line 4973 "bison_parser.cpp"
    break;

  case 214: /* binary_expr: operand NOT LIKE operand  */
#line 1166 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-3].expr), kOpNotLike, (yyvsp[0].expr)); }
#line 4979 "bison_parser.cpp"
    break;

  case 215: /* binary_expr: operand ILIKE operand  */
#line 1167 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpILike, (yyvsp[0].expr)); }
#line 4985 "bison_parser.cpp"
    break;

  case 216: /* binary_expr: operand CONCAT operand  */
#line 1168 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpConcat, (yyvsp[0].expr)); }
#line 4991 "bison_parser.cpp"
    break;

  case 217: /* logic_expr: expr AND expr  */
#line 1170 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAnd, (yyvsp[0].expr)); }
#line 4997 "bison_parser.cpp"
    break;

  case 218: /* logic_expr: expr OR expr  */
#line 1171 "bison_parser.y"
               { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpOr, (yyvsp[0].expr)); }
#line 5003 "bison_parser.cpp"
    break;

  case 219: /* in_expr: operand IN '(' expr_list ')'  */
#line 1173 "bison_parser.y"
                                       { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].expr_vec)); }
#line 5009 "bison_parser.cpp"
    break;

  case 220: /* in_expr: operand NOT IN '(' expr_list ')'  */
#line 1174 "bison_parser.y"
                                   { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].expr_vec))); }
#line 5015 "bison_parser.cpp"
    break;

  case 221: /* in_expr: operand IN '(' select_no_paren ')'  */
#line 1175 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].select_stmt)); }
#line 5021 "bison_parser.cpp"
    break;

  case 222: /* in_expr: operand NOT IN '(' select_no_paren ')'  */
#line 1176 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].select_stmt))); }
#line 5027 "bison_parser.cpp"
    break;

  case 223: /* case_expr: CASE expr case_list END  */
#line 1180 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-2].expr), (yyvsp[-1].expr), nullptr); }
#line 5033 "bison_parser.cpp"
    break;

  case 224: /* case_expr: CASE expr case_list ELSE expr END  */
#line 1181 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-4].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5039 "bison_parser.cpp"
    break;

  case 225: /* case_expr: CASE case_list END  */
#line 1182 "bison_parser.y"
                     { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-1].expr), nullptr); }
#line 5045 "bison_parser.cpp"
    break;

  case 226: /* case_expr: CASE case_list ELSE expr END  */
#line 1183 "bison_parser.y"
                               { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5051 "bison_parser.cpp"
    break;

  case 227: /* case_list: WHEN expr THEN expr  */
#line 1185 "bison_parser.y"
                                { (yyval.expr) = Expr::makeCaseList(Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5057 "bison_parser.cpp"
    break;

  case 228: /* case_list: case_list WHEN expr THEN expr  */
#line 1186 "bison_parser.y"
                                { (yyval.expr) = Expr::caseListAppend((yyvsp[-4].expr), Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5063 "bison_parser.cpp"
    break;

  case 229: /* exists_expr: EXISTS '(' select_no_paren ')'  */
#line 1188 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeExists((yyvsp[-1].select_stmt)); }
#line 5069 "bison_parser.cpp"
    break;

  case 230: /* exists_expr: NOT EXISTS '(' select_no_paren ')'  */
#line 1189 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeExists((yyvsp[-1].select_stmt))); }
#line 5075 "bison_parser.cpp"
    break;

  case 231: /* comp_expr: operand '=' operand  */
#line 1191 "bison_parser.y"
                                { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5081 "bison_parser.cpp"
    break;

  case 232: /* comp_expr: operand EQUALS operand  */
#line 1192 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5087 "bison_parser.cpp"
    break;

  case 233: /* comp_expr: operand NOTEQUALS operand  */
#line 1193 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpNotEquals, (yyvsp[0].expr)); }
#line 5093 "bison_parser.cpp"
    break;

  case 234: /* comp_expr: operand '<' operand  */
#line 1194 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLess, (yyvsp[0].expr)); }
#line 5099 "bison_parser.cpp"
    break;

  case 235: /* comp_expr: operand '>' operand  */
#line 1195 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreater, (yyvsp[0].expr)); }
#line 5105 "bison_parser.cpp"
    break;

  case 236: /* comp_expr: operand LESSEQ operand  */
#line 1196 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLessEq, (yyvsp[0].expr)); }
#line 5111 "bison_parser.cpp"
    break;

  case 237: /* comp_expr: operand GREATEREQ operand  */
#line 1197 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreaterEq, (yyvsp[0].expr)); }
#line 5117 "bison_parser.cpp"
    break;

  case 238: /* function_expr: IDENTIFIER '(' ')' opt_window  */
#line 1201 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 5123 "bison_parser.cpp"
    break;

  case 239: /* function_expr: IDENTIFIER '(' opt_distinct expr_list ')' opt_window  */
#line 1202 "bison_parser.y"
                                                       { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description)); }
#line 5129 "bison_parser.cpp"
    break;

  case 240: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' ')' opt_window  */
#line 1203 "bison_parser.y"
                                               {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), (yyvsp[-5].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description));
}
#line 5137 "bison_parser.cpp"
    break;

  case 241: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' opt_distinct expr_list ')' opt_window  */
#line 1206 "bison_parser.y"
                                                                      {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-7].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description));
}
#line 5145 "bison_parser.cpp"
    break;

  case 242: /* opt_window: OVER '(' opt_partition opt_order opt_frame_clause ')'  */
#line 1212 "bison_parser.y"
                                                                   { (yyval.window_description) = new WindowDescription((yyvsp[-3].expr_vec), (yyvsp[-2].order_vec), (yyvsp[-1].frame_description)); }
#line 5151 "bison_parser.cpp"
    break;

  case 243: /* opt_window: %empty  */
#line 1213 "bison_parser.y"
              { (yyval.window_description) = nullptr; }
#line 5157 "bison_parser.cpp"
    break;

  case 244: /* opt_partition: PARTITION BY expr_list  */
#line 1215 "bison_parser.y"
                                       { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5163 "bison_parser.cpp"
    break;

  case 245: /* opt_partition: %empty  */
#line 1216 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5169 "bison_parser.cpp"
    break;

  case 246: /* opt_frame_clause: frame_type frame_bound  */
#line 1221 "bison_parser.y"
                                          { (yyval.frame_description) = new FrameDescription{(yyvsp[-1].frame_type), (yyvsp[0].frame_bound), new FrameBound{0, kCurrentRow, false}}; }
#line 5175 "bison_parser.cpp"
    break;

  case 247: /* opt_frame_clause: frame_type BETWEEN frame_bound AND frame_bound  */
#line 1222 "bison_parser.y"
                                                 { (yyval.frame_description) = new FrameDescription{(yyvsp[-4].frame_type), (yyvsp[-2].frame_bound), (yyvsp[0].frame_bound)}; }
#line 5181 "bison_parser.cpp"
    break;

  case 248: /* opt_frame_clause: %empty  */
#line 1223 "bison_parser.y"
              {
  (yyval.frame_description) = new FrameDescription{kRange, new FrameBound{0, kPreceding, true}, new FrameBound{0, kCurrentRow, false}};
}
#line 5189 "bison_parser.cpp"
    break;

  case 249: /* frame_type: RANGE  */
#line 1227 "bison_parser.y"
                   { (yyval.frame_type) = kRange; }
#line 5195 "bison_parser.cpp"
    break;

  case 250: /* frame_type: ROWS  */
#line 1228 "bison_parser.y"
       { (yyval.frame_type) = kRows; }
#line 5201 "bison_parser.cpp"
    break;

  case 251: /* frame_type: GROUPS  */
#line 1229 "bison_parser.y"
         { (yyval.frame_type) = kGroups; }
#line 5207 "bison_parser.cpp"
    break;

  case 252: /* frame_bound: UNBOUNDED PRECEDING  */
#line 1231 "bison_parser.y"
                                  { (yyval.frame_bound) = new FrameBound{0, kPreceding, true}; }
#line 5213 "bison_parser.cpp"
    break;

  case 253: /* frame_bound: INTVAL PRECEDING  */
#line 1232 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kPreceding, false}; }
#line 5219 "bison_parser.cpp"
    break;

  case 254: /* frame_bound: UNBOUNDED FOLLOWING  */
#line 1233 "bison_parser.y"
                      { (yyval.frame_bound) = new FrameBound{0, kFollowing, true}; }
#line 5225 "bison_parser.cpp"
    break;

  case 255: /* frame_bound: INTVAL FOLLOWING  */
#line 1234 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kFollowing, false}; }
#line 5231 "bison_parser.cpp"
    break;

  case 256: /* frame_bound: CURRENT_ROW  */
#line 1235 "bison_parser.y"
              { (yyval.frame_bound) = new FrameBound{0, kCurrentRow, false}; }
#line 5237 "bison_parser.cpp"
    break;

  case 257: /* extract_expr: EXTRACT '(' datetime_field FROM expr ')'  */
#line 1237 "bison_parser.y"
                                                        { (yyval.expr) = Expr::makeExtract((yyvsp[-3].datetime_field), (yyvsp[-1].expr)); }
#line 5243 "bison_parser.cpp"
    break;

  case 258: /* cast_expr: CAST '(' expr AS column_type ')'  */
#line 1239 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t)); }
#line 5249 "bison_parser.cpp"
    break;

  case 259: /* datetime_field: SECOND  */
#line 1241 "bison_parser.y"
                        { (yyval.datetime_field) = kDatetimeSecond; }
#line 5255 "bison_parser.cpp"
    break;

  case 260: /* datetime_field: MINUTE  */
#line 1242 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMinute; }
#line 5261 "bison_parser.cpp"
    break;

  case 261: /* datetime_field: HOUR  */
#line 1243 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeHour; }
#line 5267 "bison_parser.cpp"
    break;

  case 262: /* datetime_field: DAY  */
#line 1244 "bison_parser.y"
      { (yyval.datetime_field) = kDatetimeDay; }
#line 5273 "bison_parser.cpp"
    break;

  case 263: /* datetime_field: MONTH  */
#line 1245 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeMonth; }
#line 5279 "bison_parser.cpp"
    break;

  case 264: /* datetime_field: YEAR  */
#line 1246 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeYear; }
#line 5285 "bison_parser.cpp"
    break;

  case 265: /* datetime_field_plural: SECONDS  */
#line 1248 "bison_parser.y"
                                { (yyval.datetime_field) = kDatetimeSecond; }
#line 5291 "bison_parser.cpp"
    break;

  case 266: /* datetime_field_plural: MINUTES  */
#line 1249 "bison_parser.y"
          { (yyval.datetime_field) = kDatetimeMinute; }
#line 5297 "bison_parser.cpp"
    break;

  case 267: /* datetime_field_plural: HOURS  */
#line 1250 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeHour; }
#line 5303 "bison_parser.cpp"
    break;

  case 268: /* datetime_field_plural: DAYS  */
#line 1251 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeDay; }
#line 5309 "bison_parser.cpp"
    break;

  case 269: /* datetime_field_plural: MONTHS  */
#line 1252 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMonth; }
#line 5315 "bison_parser.cpp"
    break;

  case 270: /* datetime_field_plural: YEARS  */
#line 1253 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeYear; }
#line 5321 "bison_parser.cpp"
    break;

  case 273: /* array_expr: ARRAY '[' expr_list ']'  */
#line 1257 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeArray((yyvsp[-1].expr_vec)); }
#line 5327 "bison_parser.cpp"
    break;

  case 274: /* array_index: operand '[' int_literal ']'  */
#line 1259 "bison_parser.y"
                                          { (yyval.expr) = Expr::makeArrayIndex((yyvsp[-3].expr), (yyvsp[-1].expr)->ival); }
#line 5333 "bison_parser.cpp"
    break;

  case 275: /* between_expr: operand BETWEEN operand AND operand  */
#line 1261 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeBetween((yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 5339 "bison_parser.cpp"
    break;

  case 276: /* column_name: IDENTIFIER  */
#line 1263 "bison_parser.y"
                         { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 5345 "bison_parser.cpp"
    break;

  case 277: /* column_name: IDENTIFIER '.' IDENTIFIER  */
#line 1264 "bison_parser.y"
                            { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 5351 "bison_parser.cpp"
    break;

  case 278: /* column_name: '*'  */
#line 1265 "bison_parser.y"
      { (yyval.expr) = Expr::makeStar(); }
#line 5357 "bison_parser.cpp"
    break;

  case 279: /* column_name: IDENTIFIER '.' '*'  */
#line 1266 "bison_parser.y"
                     { (yyval.expr) = Expr::makeStar((yyvsp[-2].sval)); }
#line 5363 "bison_parser.cpp"
    break;

  case 287: /* string_literal: STRING  */
#line 1270 "bison_parser.y"
                        { (yyval.expr) = Expr::makeLiteral((yyvsp[0].sval)); }
#line 5369 "bison_parser.cpp"
    break;

  case 288: /* bool_literal: TRUE  */
#line 1272 "bison_parser.y"
                    { (yyval.expr) = Expr::makeLiteral(true); }
#line 5375 "bison_parser.cpp"
    break;

  case 289: /* bool_literal: FALSE  */
#line 1273 "bison_parser.y"
        { (yyval.expr) = Expr::makeLiteral(false); }
#line 5381 "bison_parser.cpp"
    break;

  case 290: /* num_literal: FLOATVAL  */
#line 1275 "bison_parser.y"
                       { (yyval.expr) = Expr::makeLiteral((yyvsp[0].fval)); }
#line 5387 "bison_parser.cpp"
    break;

  case 292: /* int_literal: INTVAL  */
#line 1278 "bison_parser.y"
                     { (yyval.expr) = Expr::makeLiteral((yyvsp[0].ival)); }
#line 5393 "bison_parser.cpp"
    break;

  case 293: /* null_literal: NULL  */
#line 1280 "bison_parser.y"
                    { (yyval.expr) = Expr::makeNullLiteral(); }
#line 5399 "bison_parser.cpp"
    break;

  case 294: /* date_literal: DATE STRING  */
#line 1282 "bison_parser.y"
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
#line 5414 "bison_parser.cpp"
    break;

  case 295: /* interval_literal: INTVAL duration_field  */
#line 1293 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeIntervalLiteral((yyvsp[-1].ival), (yyvsp[0].datetime_field)); }
#line 5420 "bison_parser.cpp"
    break;

  case 296: /* interval_literal: INTERVAL STRING datetime_field  */
#line 1294 "bison_parser.y"
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
#line 5436 "bison_parser.cpp"
    break;

  case 297: /* interval_literal: INTERVAL STRING  */
#line 1305 "bison_parser.y"
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
#line 5472 "bison_parser.cpp"
    break;

  case 298: /* param_expr: '?'  */
#line 1337 "bison_parser.y"
                 {
  (yyval.expr) = Expr::makeParameter(yylloc.total_column);
  (yyval.expr)->ival2 = yylloc.total_column - 1;  // source column (0-based) of the '?' token
  yyloc.param_list.push_back((yyval.expr));
}
#line 5482 "bison_parser.cpp"
    break;

  case 299: /* param_expr: DOLLAR_PARAM  */
#line 1342 "bison_parser.y"
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
#line 5499 "bison_parser.cpp"
    break;

  case 300: /* param_expr: NAMED_PARAM  */
#line 1354 "bison_parser.y"
              {
  (yyval.expr) = Expr::makeNamedParameter((yyvsp[0].sval));
  (yyval.expr)->ival2 = yylloc.total_column - 1 - (int64_t)strlen((yyvsp[0].sval));
  yyloc.param_list.push_back((yyval.expr));
}
#line 5509 "bison_parser.cpp"
    break;

  case 302: /* table_ref: table_ref_commalist ',' table_ref_atomic  */
#line 1363 "bison_parser.y"
                                                                        {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  auto tbl = new TableRef(kTableCrossProduct);
  tbl->list = (yyvsp[-2].table_vec);
  (yyval.table) = tbl;
}
#line 5520 "bison_parser.cpp"
    break;

  case 306: /* nonjoin_table_ref_atomic: '(' select_statement ')' opt_table_alias  */
#line 1372 "bison_parser.y"
                                                                                     {
  auto tbl = new TableRef(kTableSelect);
  tbl->select = (yyvsp[-2].select_stmt);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 5531 "bison_parser.cpp"
    break;

  case 307: /* table_ref_commalist: table_ref_atomic  */
#line 1379 "bison_parser.y"
                                       {
  (yyval.table_vec) = new std::vector<TableRef*>();
  (yyval.table_vec)->push_back((yyvsp[0].table));
}
#line 5540 "bison_parser.cpp"
    break;

  case 308: /* table_ref_commalist: table_ref_commalist ',' table_ref_atomic  */
#line 1383 "bison_parser.y"
                                           {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  (yyval.table_vec) = (yyvsp[-2].table_vec);
}
#line 5549 "bison_parser.cpp"
    break;

  case 309: /* table_ref_name: table_name opt_table_alias  */
#line 1388 "bison_parser.y"
                                            {
  auto tbl = new TableRef(kTableName);
  tbl->schema = (yyvsp[-1].table_name).schema;
  tbl->name = (yyvsp[-1].table_name).name;
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 5561 "bison_parser.cpp"
    break;

  case 310: /* table_ref_name_no_alias: table_name  */
#line 1396 "bison_parser.y"
                                     {
  (yyval.table) = new TableRef(kTableName);
  (yyval.table)->schema = (yyvsp[0].table_name).schema;
  (yyval.table)->name = (yyvsp[0].table_name).name;
}
#line 5571 "bison_parser.cpp"
    break;

  case 311: /* table_name: IDENTIFIER  */
#line 1402 "bison_parser.y"
                        {
  (yyval.table_name).schema = nullptr;
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 5580 "bison_parser.cpp"
    break;

  case 312: /* table_name: IDENTIFIER '.' IDENTIFIER  */
#line 1406 "bison_parser.y"
                            {
  (yyval.table_name).schema = (yyvsp[-2].sval);
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 5589 "bison_parser.cpp"
    break;

  case 313: /* opt_index_name: IDENTIFIER  */
#line 1411 "bison_parser.y"
                            { (yyval.sval) = (yyvsp[0].sval); }
#line 5595 "bison_parser.cpp"
    break;

  case 314: /* opt_index_name: %empty  */
#line 1412 "bison_parser.y"
              { (yyval.sval) = nullptr; }
#line 5601 "bison_parser.cpp"
    break;

  case 316: /* table_alias: AS IDENTIFIER '(' ident_commalist ')'  */
#line 1414 "bison_parser.y"
                                                            { (yyval.alias_t) = new Alias((yyvsp[-3].sval), (yyvsp[-1].str_vec)); }
#line 5607 "bison_parser.cpp"
    break;

  case 318: /* opt_table_alias: %empty  */
#line 1416 "bison_parser.y"
                                            { (yyval.alias_t) = nullptr; }
#line 5613 "bison_parser.cpp"
    break;

  case 319: /* alias: AS IDENTIFIER  */
#line 1418 "bison_parser.y"
                      { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 5619 "bison_parser.cpp"
    break;

  case 320: /* alias: IDENTIFIER  */
#line 1419 "bison_parser.y"
             { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 5625 "bison_parser.cpp"
    break;

  case 322: /* opt_alias: %empty  */
#line 1421 "bison_parser.y"
                                { (yyval.alias_t) = nullptr; }
#line 5631 "bison_parser.cpp"
    break;

  case 323: /* opt_locking_clause: opt_locking_clause_list  */
#line 1427 "bison_parser.y"
                                             { (yyval.locking_clause_vec) = (yyvsp[0].locking_clause_vec); }
#line 5637 "bison_parser.cpp"
    break;

  case 324: /* opt_locking_clause: %empty  */
#line 1428 "bison_parser.y"
              { (yyval.locking_clause_vec) = nullptr; }
#line 5643 "bison_parser.cpp"
    break;

  case 325: /* opt_locking_clause_list: locking_clause  */
#line 1430 "bison_parser.y"
                                         {
  (yyval.locking_clause_vec) = new std::vector<LockingClause*>();
  (yyval.locking_clause_vec)->push_back((yyvsp[0].locking_t));
}
#line 5652 "bison_parser.cpp"
    break;

  case 326: /* opt_locking_clause_list: opt_locking_clause_list locking_clause  */
#line 1434 "bison_parser.y"
                                         {
  (yyvsp[-1].locking_clause_vec)->push_back((yyvsp[0].locking_t));
  (yyval.locking_clause_vec) = (yyvsp[-1].locking_clause_vec);
}
#line 5661 "bison_parser.cpp"
    break;

  case 327: /* locking_clause: FOR row_lock_mode opt_row_lock_policy  */
#line 1439 "bison_parser.y"
                                                       {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-1].lock_mode_t);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
  (yyval.locking_t)->tables = nullptr;
}
#line 5672 "bison_parser.cpp"
    break;

  case 328: /* locking_clause: FOR row_lock_mode OF ident_commalist opt_row_lock_policy  */
#line 1445 "bison_parser.y"
                                                           {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-3].lock_mode_t);
  (yyval.locking_t)->tables = (yyvsp[-1].str_vec);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
}
#line 5683 "bison_parser.cpp"
    break;

  case 329: /* row_lock_mode: UPDATE  */
#line 1452 "bison_parser.y"
                       { (yyval.lock_mode_t) = RowLockMode::ForUpdate; }
#line 5689 "bison_parser.cpp"
    break;

  case 330: /* row_lock_mode: NO KEY UPDATE  */
#line 1453 "bison_parser.y"
                { (yyval.lock_mode_t) = RowLockMode::ForNoKeyUpdate; }
#line 5695 "bison_parser.cpp"
    break;

  case 331: /* row_lock_mode: SHARE  */
#line 1454 "bison_parser.y"
        { (yyval.lock_mode_t) = RowLockMode::ForShare; }
#line 5701 "bison_parser.cpp"
    break;

  case 332: /* row_lock_mode: KEY SHARE  */
#line 1455 "bison_parser.y"
            { (yyval.lock_mode_t) = RowLockMode::ForKeyShare; }
#line 5707 "bison_parser.cpp"
    break;

  case 333: /* opt_row_lock_policy: SKIP LOCKED  */
#line 1457 "bison_parser.y"
                                  { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::SkipLocked; }
#line 5713 "bison_parser.cpp"
    break;

  case 334: /* opt_row_lock_policy: NOWAIT  */
#line 1458 "bison_parser.y"
         { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::NoWait; }
#line 5719 "bison_parser.cpp"
    break;

  case 335: /* opt_row_lock_policy: %empty  */
#line 1459 "bison_parser.y"
              { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::None; }
#line 5725 "bison_parser.cpp"
    break;

  case 337: /* opt_with_clause: %empty  */
#line 1465 "bison_parser.y"
                                            { (yyval.with_description_vec) = nullptr; }
#line 5731 "bison_parser.cpp"
    break;

  case 338: /* with_clause: WITH with_description_list  */
#line 1467 "bison_parser.y"
                                         { (yyval.with_description_vec) = (yyvsp[0].with_description_vec); }
#line 5737 "bison_parser.cpp"
    break;

  case 339: /* with_description_list: with_description  */
#line 1469 "bison_parser.y"
                                         {
  (yyval.with_description_vec) = new std::vector<WithDescription*>();
  (yyval.with_description_vec)->push_back((yyvsp[0].with_description_t));
}
#line 5746 "bison_parser.cpp"
    break;

  case 340: /* with_description_list: with_description_list ',' with_description  */
#line 1473 "bison_parser.y"
                                             {
  (yyvsp[-2].with_description_vec)->push_back((yyvsp[0].with_description_t));
  (yyval.with_description_vec) = (yyvsp[-2].with_description_vec);
}
#line 5755 "bison_parser.cpp"
    break;

  case 341: /* with_description: IDENTIFIER AS select_with_paren  */
#line 1478 "bison_parser.y"
                                                   {
  (yyval.with_description_t) = new WithDescription();
  (yyval.with_description_t)->alias = (yyvsp[-2].sval);
  (yyval.with_description_t)->select = (yyvsp[0].select_stmt);
}
#line 5765 "bison_parser.cpp"
    break;

  case 342: /* join_clause: table_ref_atomic NATURAL JOIN nonjoin_table_ref_atomic  */
#line 1488 "bison_parser.y"
                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = kJoinNatural;
  (yyval.table)->join->left = (yyvsp[-3].table);
  (yyval.table)->join->right = (yyvsp[0].table);
}
#line 5777 "bison_parser.cpp"
    break;

  case 343: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic ON join_condition  */
#line 1495 "bison_parser.y"
                                                                         {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (JoinType)(yyvsp[-4].join_type);
  (yyval.table)->join->left = (yyvsp[-5].table);
  (yyval.table)->join->right = (yyvsp[-2].table);
  (yyval.table)->join->condition = (yyvsp[0].expr);
}
#line 5790 "bison_parser.cpp"
    break;

  case 344: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic USING '(' ident_commalist ')'  */
#line 1503 "bison_parser.y"
                                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (yyvsp[-6].join_type);
  (yyval.table)->join->left = (yyvsp[-7].table);
  (yyval.table)->join->right = (yyvsp[-4].table);
  (yyval.table)->join->namedColumns = (yyvsp[-1].str_vec);
}
#line 5803 "bison_parser.cpp"
    break;

  case 345: /* opt_join_type: INNER  */
#line 1512 "bison_parser.y"
                      { (yyval.join_type) = kJoinInner; }
#line 5809 "bison_parser.cpp"
    break;

  case 346: /* opt_join_type: LEFT OUTER  */
#line 1513 "bison_parser.y"
             { (yyval.join_type) = kJoinLeft; }
#line 5815 "bison_parser.cpp"
    break;

  case 347: /* opt_join_type: LEFT  */
#line 1514 "bison_parser.y"
       { (yyval.join_type) = kJoinLeft; }
#line 5821 "bison_parser.cpp"
    break;

  case 348: /* opt_join_type: RIGHT OUTER  */
#line 1515 "bison_parser.y"
              { (yyval.join_type) = kJoinRight; }
#line 5827 "bison_parser.cpp"
    break;

  case 349: /* opt_join_type: RIGHT  */
#line 1516 "bison_parser.y"
        { (yyval.join_type) = kJoinRight; }
#line 5833 "bison_parser.cpp"
    break;

  case 350: /* opt_join_type: FULL OUTER  */
#line 1517 "bison_parser.y"
             { (yyval.join_type) = kJoinFull; }
#line 5839 "bison_parser.cpp"
    break;

  case 351: /* opt_join_type: OUTER  */
#line 1518 "bison_parser.y"
        { (yyval.join_type) = kJoinFull; }
#line 5845 "bison_parser.cpp"
    break;

  case 352: /* opt_join_type: FULL  */
#line 1519 "bison_parser.y"
       { (yyval.join_type) = kJoinFull; }
#line 5851 "bison_parser.cpp"
    break;

  case 353: /* opt_join_type: CROSS  */
#line 1520 "bison_parser.y"
        { (yyval.join_type) = kJoinCross; }
#line 5857 "bison_parser.cpp"
    break;

  case 354: /* opt_join_type: %empty  */
#line 1521 "bison_parser.y"
                       { (yyval.join_type) = kJoinInner; }
#line 5863 "bison_parser.cpp"
    break;

  case 358: /* ident_commalist: IDENTIFIER  */
#line 1532 "bison_parser.y"
                             {
  (yyval.str_vec) = new std::vector<char*>();
  (yyval.str_vec)->push_back((yyvsp[0].sval));
}
#line 5872 "bison_parser.cpp"
    break;

  case 359: /* ident_commalist: ident_commalist ',' IDENTIFIER  */
#line 1536 "bison_parser.y"
                                 {
  (yyvsp[-2].str_vec)->push_back((yyvsp[0].sval));
  (yyval.str_vec) = (yyvsp[-2].str_vec);
}
#line 5881 "bison_parser.cpp"
    break;


#line 5885 "bison_parser.cpp"

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

#line 1542 "bison_parser.y"


/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */

    // clang-format on
