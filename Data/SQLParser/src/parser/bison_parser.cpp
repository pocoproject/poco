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
#line 70 "bison_parser.y"

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

  namespace {

  // Joins two name parts with a dot, taking ownership of both. Multi-part names
  // are folded into one string wherever the node has a single slot for them.
  char* dotJoin(char* first, char* second) {
    std::string combined(std::string(first) + "." + second);
    free(first);
    free(second);
    return strdup(combined.c_str());
  }

  // Flattens a possibly qualified name back into one string, taking ownership
  // of both parts. Used where the statement has a single name field, e.g. the
  // procedure of an ODBC call escape.
  char* qualifiedName(TableName name) {
    if (!name.schema) return name.name;
    return dotJoin(name.schema, name.name);
  }

  }  // namespace
  // clang-format off

#line 139 "bison_parser.cpp"

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
  YYSYMBOL_PRIOR = 180,                    /* PRIOR  */
  YYSYMBOL_ODBC_OJ = 181,                  /* ODBC_OJ  */
  YYSYMBOL_NEXT_VALUE_FOR = 182,           /* NEXT_VALUE_FOR  */
  YYSYMBOL_START_WITH = 183,               /* START_WITH  */
  YYSYMBOL_CONNECT_BY = 184,               /* CONNECT_BY  */
  YYSYMBOL_CONNECT_BY_NOCYCLE = 185,       /* CONNECT_BY_NOCYCLE  */
  YYSYMBOL_WITHIN_GROUP = 186,             /* WITHIN_GROUP  */
  YYSYMBOL_187_ = 187,                     /* '='  */
  YYSYMBOL_EQUALS = 188,                   /* EQUALS  */
  YYSYMBOL_NOTEQUALS = 189,                /* NOTEQUALS  */
  YYSYMBOL_190_ = 190,                     /* '<'  */
  YYSYMBOL_191_ = 191,                     /* '>'  */
  YYSYMBOL_LESS = 192,                     /* LESS  */
  YYSYMBOL_GREATER = 193,                  /* GREATER  */
  YYSYMBOL_LESSEQ = 194,                   /* LESSEQ  */
  YYSYMBOL_GREATEREQ = 195,                /* GREATEREQ  */
  YYSYMBOL_NOTNULL = 196,                  /* NOTNULL  */
  YYSYMBOL_197_ = 197,                     /* '+'  */
  YYSYMBOL_198_ = 198,                     /* '-'  */
  YYSYMBOL_199_ = 199,                     /* '*'  */
  YYSYMBOL_200_ = 200,                     /* '/'  */
  YYSYMBOL_201_ = 201,                     /* '%'  */
  YYSYMBOL_202_ = 202,                     /* '^'  */
  YYSYMBOL_UMINUS = 203,                   /* UMINUS  */
  YYSYMBOL_204_ = 204,                     /* '['  */
  YYSYMBOL_205_ = 205,                     /* ']'  */
  YYSYMBOL_206_ = 206,                     /* '('  */
  YYSYMBOL_207_ = 207,                     /* ')'  */
  YYSYMBOL_208_ = 208,                     /* '.'  */
  YYSYMBOL_209_ = 209,                     /* ';'  */
  YYSYMBOL_210_ = 210,                     /* ','  */
  YYSYMBOL_211_ = 211,                     /* '{'  */
  YYSYMBOL_212_ = 212,                     /* '}'  */
  YYSYMBOL_213_ = 213,                     /* '?'  */
  YYSYMBOL_YYACCEPT = 214,                 /* $accept  */
  YYSYMBOL_input = 215,                    /* input  */
  YYSYMBOL_statement_list = 216,           /* statement_list  */
  YYSYMBOL_statement = 217,                /* statement  */
  YYSYMBOL_preparable_statement = 218,     /* preparable_statement  */
  YYSYMBOL_opt_hints = 219,                /* opt_hints  */
  YYSYMBOL_hint_list = 220,                /* hint_list  */
  YYSYMBOL_hint = 221,                     /* hint  */
  YYSYMBOL_transaction_statement = 222,    /* transaction_statement  */
  YYSYMBOL_opt_transaction_keyword = 223,  /* opt_transaction_keyword  */
  YYSYMBOL_prepare_statement = 224,        /* prepare_statement  */
  YYSYMBOL_prepare_target_query = 225,     /* prepare_target_query  */
  YYSYMBOL_odbc_call_statement = 226,      /* odbc_call_statement  */
  YYSYMBOL_opt_odbc_return = 227,          /* opt_odbc_return  */
  YYSYMBOL_opt_call_args = 228,            /* opt_call_args  */
  YYSYMBOL_execute_statement = 229,        /* execute_statement  */
  YYSYMBOL_import_statement = 230,         /* import_statement  */
  YYSYMBOL_file_type = 231,                /* file_type  */
  YYSYMBOL_file_path = 232,                /* file_path  */
  YYSYMBOL_opt_import_export_options = 233, /* opt_import_export_options  */
  YYSYMBOL_import_export_options = 234,    /* import_export_options  */
  YYSYMBOL_csv_option = 235,               /* csv_option  */
  YYSYMBOL_export_statement = 236,         /* export_statement  */
  YYSYMBOL_show_statement = 237,           /* show_statement  */
  YYSYMBOL_create_statement = 238,         /* create_statement  */
  YYSYMBOL_opt_not_exists = 239,           /* opt_not_exists  */
  YYSYMBOL_table_elem_commalist = 240,     /* table_elem_commalist  */
  YYSYMBOL_table_elem = 241,               /* table_elem  */
  YYSYMBOL_column_def = 242,               /* column_def  */
  YYSYMBOL_column_type = 243,              /* column_type  */
  YYSYMBOL_opt_time_precision = 244,       /* opt_time_precision  */
  YYSYMBOL_opt_decimal_specification = 245, /* opt_decimal_specification  */
  YYSYMBOL_opt_column_constraints = 246,   /* opt_column_constraints  */
  YYSYMBOL_column_constraints = 247,       /* column_constraints  */
  YYSYMBOL_column_constraint = 248,        /* column_constraint  */
  YYSYMBOL_table_constraint = 249,         /* table_constraint  */
  YYSYMBOL_references_spec = 250,          /* references_spec  */
  YYSYMBOL_drop_statement = 251,           /* drop_statement  */
  YYSYMBOL_opt_exists = 252,               /* opt_exists  */
  YYSYMBOL_alter_statement = 253,          /* alter_statement  */
  YYSYMBOL_alter_action = 254,             /* alter_action  */
  YYSYMBOL_drop_action = 255,              /* drop_action  */
  YYSYMBOL_delete_statement = 256,         /* delete_statement  */
  YYSYMBOL_truncate_statement = 257,       /* truncate_statement  */
  YYSYMBOL_insert_statement = 258,         /* insert_statement  */
  YYSYMBOL_opt_column_list = 259,          /* opt_column_list  */
  YYSYMBOL_update_statement = 260,         /* update_statement  */
  YYSYMBOL_update_clause_commalist = 261,  /* update_clause_commalist  */
  YYSYMBOL_update_clause = 262,            /* update_clause  */
  YYSYMBOL_select_statement = 263,         /* select_statement  */
  YYSYMBOL_select_within_set_operation = 264, /* select_within_set_operation  */
  YYSYMBOL_select_within_set_operation_no_parentheses = 265, /* select_within_set_operation_no_parentheses  */
  YYSYMBOL_select_with_paren = 266,        /* select_with_paren  */
  YYSYMBOL_select_no_paren = 267,          /* select_no_paren  */
  YYSYMBOL_set_operator = 268,             /* set_operator  */
  YYSYMBOL_set_type = 269,                 /* set_type  */
  YYSYMBOL_opt_all = 270,                  /* opt_all  */
  YYSYMBOL_opt_hierarchical_clause = 271,  /* opt_hierarchical_clause  */
  YYSYMBOL_connect_by = 272,               /* connect_by  */
  YYSYMBOL_select_clause = 273,            /* select_clause  */
  YYSYMBOL_opt_distinct = 274,             /* opt_distinct  */
  YYSYMBOL_select_list = 275,              /* select_list  */
  YYSYMBOL_opt_from_clause = 276,          /* opt_from_clause  */
  YYSYMBOL_from_clause = 277,              /* from_clause  */
  YYSYMBOL_opt_where = 278,                /* opt_where  */
  YYSYMBOL_opt_group = 279,                /* opt_group  */
  YYSYMBOL_opt_having = 280,               /* opt_having  */
  YYSYMBOL_opt_within_group = 281,         /* opt_within_group  */
  YYSYMBOL_opt_order = 282,                /* opt_order  */
  YYSYMBOL_order_list = 283,               /* order_list  */
  YYSYMBOL_order_desc = 284,               /* order_desc  */
  YYSYMBOL_opt_order_type = 285,           /* opt_order_type  */
  YYSYMBOL_opt_null_ordering = 286,        /* opt_null_ordering  */
  YYSYMBOL_opt_top = 287,                  /* opt_top  */
  YYSYMBOL_opt_limit = 288,                /* opt_limit  */
  YYSYMBOL_expr_list = 289,                /* expr_list  */
  YYSYMBOL_opt_extended_literal_list = 290, /* opt_extended_literal_list  */
  YYSYMBOL_extended_literal_list = 291,    /* extended_literal_list  */
  YYSYMBOL_casted_extended_literal = 292,  /* casted_extended_literal  */
  YYSYMBOL_extended_literal = 293,         /* extended_literal  */
  YYSYMBOL_expr_alias = 294,               /* expr_alias  */
  YYSYMBOL_expr = 295,                     /* expr  */
  YYSYMBOL_operand = 296,                  /* operand  */
  YYSYMBOL_scalar_expr = 297,              /* scalar_expr  */
  YYSYMBOL_unary_expr = 298,               /* unary_expr  */
  YYSYMBOL_binary_expr = 299,              /* binary_expr  */
  YYSYMBOL_logic_expr = 300,               /* logic_expr  */
  YYSYMBOL_in_expr = 301,                  /* in_expr  */
  YYSYMBOL_row_expr_list = 302,            /* row_expr_list  */
  YYSYMBOL_case_expr = 303,                /* case_expr  */
  YYSYMBOL_case_list = 304,                /* case_list  */
  YYSYMBOL_exists_expr = 305,              /* exists_expr  */
  YYSYMBOL_comp_expr = 306,                /* comp_expr  */
  YYSYMBOL_function_expr = 307,            /* function_expr  */
  YYSYMBOL_table_function_expr = 308,      /* table_function_expr  */
  YYSYMBOL_opt_window = 309,               /* opt_window  */
  YYSYMBOL_opt_partition = 310,            /* opt_partition  */
  YYSYMBOL_opt_frame_clause = 311,         /* opt_frame_clause  */
  YYSYMBOL_frame_type = 312,               /* frame_type  */
  YYSYMBOL_frame_bound = 313,              /* frame_bound  */
  YYSYMBOL_extract_expr = 314,             /* extract_expr  */
  YYSYMBOL_next_value_expr = 315,          /* next_value_expr  */
  YYSYMBOL_cast_expr = 316,                /* cast_expr  */
  YYSYMBOL_datetime_field = 317,           /* datetime_field  */
  YYSYMBOL_datetime_field_plural = 318,    /* datetime_field_plural  */
  YYSYMBOL_duration_field = 319,           /* duration_field  */
  YYSYMBOL_array_expr = 320,               /* array_expr  */
  YYSYMBOL_array_index = 321,              /* array_index  */
  YYSYMBOL_between_expr = 322,             /* between_expr  */
  YYSYMBOL_column_name = 323,              /* column_name  */
  YYSYMBOL_nonreserved_keyword = 324,      /* nonreserved_keyword  */
  YYSYMBOL_name_or_keyword = 325,          /* name_or_keyword  */
  YYSYMBOL_literal = 326,                  /* literal  */
  YYSYMBOL_string_literal = 327,           /* string_literal  */
  YYSYMBOL_bool_literal = 328,             /* bool_literal  */
  YYSYMBOL_num_literal = 329,              /* num_literal  */
  YYSYMBOL_int_literal = 330,              /* int_literal  */
  YYSYMBOL_null_literal = 331,             /* null_literal  */
  YYSYMBOL_date_literal = 332,             /* date_literal  */
  YYSYMBOL_interval_literal = 333,         /* interval_literal  */
  YYSYMBOL_param_expr = 334,               /* param_expr  */
  YYSYMBOL_table_ref = 335,                /* table_ref  */
  YYSYMBOL_table_ref_atomic = 336,         /* table_ref_atomic  */
  YYSYMBOL_nonjoin_table_ref_atomic = 337, /* nonjoin_table_ref_atomic  */
  YYSYMBOL_table_value_row_list = 338,     /* table_value_row_list  */
  YYSYMBOL_table_ref_commalist = 339,      /* table_ref_commalist  */
  YYSYMBOL_table_ref_name = 340,           /* table_ref_name  */
  YYSYMBOL_table_ref_name_no_alias = 341,  /* table_ref_name_no_alias  */
  YYSYMBOL_table_name = 342,               /* table_name  */
  YYSYMBOL_opt_index_name = 343,           /* opt_index_name  */
  YYSYMBOL_table_alias = 344,              /* table_alias  */
  YYSYMBOL_opt_table_alias = 345,          /* opt_table_alias  */
  YYSYMBOL_alias = 346,                    /* alias  */
  YYSYMBOL_opt_alias = 347,                /* opt_alias  */
  YYSYMBOL_opt_locking_clause = 348,       /* opt_locking_clause  */
  YYSYMBOL_opt_locking_clause_list = 349,  /* opt_locking_clause_list  */
  YYSYMBOL_locking_clause = 350,           /* locking_clause  */
  YYSYMBOL_row_lock_mode = 351,            /* row_lock_mode  */
  YYSYMBOL_opt_row_lock_policy = 352,      /* opt_row_lock_policy  */
  YYSYMBOL_opt_with_clause = 353,          /* opt_with_clause  */
  YYSYMBOL_with_clause = 354,              /* with_clause  */
  YYSYMBOL_with_description_list = 355,    /* with_description_list  */
  YYSYMBOL_with_description = 356,         /* with_description  */
  YYSYMBOL_join_clause = 357,              /* join_clause  */
  YYSYMBOL_opt_join_type = 358,            /* opt_join_type  */
  YYSYMBOL_join_condition = 359,           /* join_condition  */
  YYSYMBOL_opt_semicolon = 360,            /* opt_semicolon  */
  YYSYMBOL_ident_commalist = 361           /* ident_commalist  */
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
#define YYFINAL  100
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2295

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  214
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  148
/* YYNRULES -- Number of rules.  */
#define YYNRULES  428
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  796

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   449


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
       2,     2,     2,     2,     2,     2,     2,   201,     2,     2,
     206,   207,   199,   197,   210,   198,   208,   200,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   209,
     190,   187,   191,   213,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   204,     2,   205,   202,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   211,     2,   212,     2,     2,     2,     2,
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
     185,   186,   188,   189,   192,   193,   194,   195,   196,   203
};

#if HSQL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   420,   420,   440,   446,   453,   457,   461,   462,   463,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   481,   482,   484,   488,   493,   497,   507,   508,   509,
     511,   511,   517,   523,   529,   538,   539,   541,   542,   544,
     556,   562,   579,   594,   596,   597,   598,   600,   614,   618,
     628,   632,   656,   664,   677,   684,   699,   719,   720,   725,
     736,   749,   761,   768,   775,   784,   785,   787,   791,   796,
     797,   799,   807,   808,   809,   810,   811,   812,   813,   817,
     818,   819,   820,   821,   822,   823,   824,   825,   826,   827,
     829,   830,   832,   833,   834,   836,   837,   839,   843,   847,
     852,   860,   861,   862,   863,   865,   866,   867,   869,   877,
     883,   889,   895,   901,   902,   909,   915,   917,   927,   934,
     945,   952,   960,   961,   968,   975,   979,   984,   994,   998,
    1002,  1014,  1014,  1016,  1017,  1026,  1027,  1029,  1060,  1072,
    1077,  1081,  1085,  1090,  1091,  1095,  1096,  1097,  1098,  1099,
    1103,  1103,  1105,  1117,  1118,  1120,  1122,  1123,  1125,  1127,
    1128,  1130,  1135,  1137,  1138,  1143,  1144,  1146,  1147,  1149,
    1153,  1158,  1160,  1161,  1162,  1164,  1165,  1191,  1192,  1193,
    1195,  1196,  1197,  1198,  1199,  1200,  1201,  1206,  1210,  1216,
    1217,  1219,  1223,  1228,  1228,  1232,  1233,  1234,  1236,  1245,
    1245,  1245,  1245,  1245,  1247,  1248,  1248,  1248,  1248,  1248,
    1248,  1248,  1248,  1248,  1249,  1249,  1253,  1253,  1257,  1259,
    1261,  1262,  1263,  1264,  1265,  1267,  1267,  1268,  1269,  1270,
    1271,  1272,  1273,  1274,  1275,  1276,  1278,  1279,  1281,  1282,
    1283,  1284,  1288,  1289,  1293,  1298,  1305,  1306,  1307,  1308,
    1310,  1311,  1313,  1314,  1316,  1317,  1318,  1319,  1320,  1321,
    1322,  1326,  1327,  1331,  1334,  1339,  1340,  1345,  1348,  1351,
    1354,  1360,  1361,  1363,  1364,  1369,  1370,  1371,  1375,  1376,
    1377,  1379,  1380,  1381,  1382,  1383,  1385,  1390,  1396,  1398,
    1399,  1400,  1401,  1402,  1403,  1405,  1406,  1407,  1408,  1409,
    1410,  1412,  1412,  1414,  1420,  1425,  1427,  1428,  1429,  1430,
    1431,  1437,  1438,  1441,  1442,  1445,  1446,  1447,  1457,  1458,
    1459,  1460,  1461,  1462,  1463,  1464,  1465,  1466,  1467,  1468,
    1469,  1470,  1471,  1472,  1473,  1474,  1475,  1476,  1477,  1482,
    1482,  1484,  1484,  1484,  1484,  1484,  1484,  1484,  1486,  1488,
    1489,  1491,  1492,  1494,  1496,  1498,  1509,  1510,  1521,  1553,
    1558,  1570,  1579,  1579,  1586,  1586,  1590,  1592,  1592,  1600,
    1609,  1618,  1622,  1627,  1631,  1636,  1644,  1653,  1657,  1661,
    1669,  1670,  1672,  1672,  1674,  1674,  1676,  1680,  1684,  1685,
    1687,  1687,  1693,  1694,  1696,  1700,  1705,  1711,  1718,  1719,
    1720,  1721,  1723,  1724,  1725,  1731,  1731,  1733,  1735,  1739,
    1744,  1754,  1761,  1769,  1778,  1779,  1780,  1781,  1782,  1783,
    1784,  1785,  1786,  1787,  1789,  1795,  1795,  1798,  1802
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
  "OUTERJOIN", "PRIOR", "ODBC_OJ", "NEXT_VALUE_FOR", "START_WITH",
  "CONNECT_BY", "CONNECT_BY_NOCYCLE", "WITHIN_GROUP", "'='", "EQUALS",
  "NOTEQUALS", "'<'", "'>'", "LESS", "GREATER", "LESSEQ", "GREATEREQ",
  "NOTNULL", "'+'", "'-'", "'*'", "'/'", "'%'", "'^'", "UMINUS", "'['",
  "']'", "'('", "')'", "'.'", "';'", "','", "'{'", "'}'", "'?'", "$accept",
  "input", "statement_list", "statement", "preparable_statement",
  "opt_hints", "hint_list", "hint", "transaction_statement",
  "opt_transaction_keyword", "prepare_statement", "prepare_target_query",
  "odbc_call_statement", "opt_odbc_return", "opt_call_args",
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
  "opt_hierarchical_clause", "connect_by", "select_clause", "opt_distinct",
  "select_list", "opt_from_clause", "from_clause", "opt_where",
  "opt_group", "opt_having", "opt_within_group", "opt_order", "order_list",
  "order_desc", "opt_order_type", "opt_null_ordering", "opt_top",
  "opt_limit", "expr_list", "opt_extended_literal_list",
  "extended_literal_list", "casted_extended_literal", "extended_literal",
  "expr_alias", "expr", "operand", "scalar_expr", "unary_expr",
  "binary_expr", "logic_expr", "in_expr", "row_expr_list", "case_expr",
  "case_list", "exists_expr", "comp_expr", "function_expr",
  "table_function_expr", "opt_window", "opt_partition", "opt_frame_clause",
  "frame_type", "frame_bound", "extract_expr", "next_value_expr",
  "cast_expr", "datetime_field", "datetime_field_plural", "duration_field",
  "array_expr", "array_index", "between_expr", "column_name",
  "nonreserved_keyword", "name_or_keyword", "literal", "string_literal",
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

#define YYPACT_NINF (-715)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-426)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1109,    63,  1816,    95,   110,  1816,   323,    90,   133,   141,
    1816,   167,   719,   342,    65,  1917,   108,   108,   108,    29,
     273,   105,  -715,   214,  -715,   214,  -715,  -715,  -715,  -715,
    -715,  -715,  -715,  -715,  -715,  -715,  -715,  -715,  -715,   -28,
    -715,   330,   129,  -715,  -715,  -715,  -715,  -715,  -715,  -715,
    -715,  -715,  -715,  -715,  -715,  -715,  -715,  -715,  -715,  -715,
    -715,  -715,  -715,  -715,  -715,  -715,  -715,   152,   258,  -715,
     280,   280,   280,  1816,   412,  1816,   246,  -715,   296,   -28,
     303,    12,   296,   296,   296,  1816,  -715,  -715,   354,   264,
    -715,  -715,  -715,  -715,  -715,  -715,  -715,  -715,   377,   315,
    -715,   467,  -715,   393,  -715,  -715,   381,   292,  -715,   279,
    -715,  1982,    61,  -715,   509,   396,  1917,  1816,  1816,   445,
    -715,   438,   353,  1917,   518,  1816,   361,   372,   582,   582,
     582,  1917,  1816,  1816,  -715,   399,  1917,  1816,  -715,  -715,
     407,    24,   586,  -715,  -715,  -715,   -28,   500,   499,   -28,
      36,   430,  -715,  -715,  -715,   554,   431,   648,  -715,   652,
    -715,  -715,    76,   450,   449,  -715,  -715,  -715,  -715,  -715,
    -715,  -715,  -715,  -715,  -715,  -715,  -715,  -715,   610,  -715,
     527,   -32,   353,  1093,  -715,   582,  1917,   320,   -44,  -715,
     476,  -715,   574,  -715,  -715,  -715,   -27,   -27,   -27,  -715,
    -715,  -715,  -715,  -715,   152,   673,  -715,  1093,  -715,  -715,
    1093,   599,  -715,  -715,   292,  -715,  -715,  1093,   599,  1093,
     637,   560,  1917,  -715,  -715,  -715,  -715,  -715,  -715,  -715,
    -715,  -715,  -715,  -715,  -715,  -715,  -715,  -715,   155,  -715,
     490,  -715,  -715,  -715,    61,  -715,  1816,   679,   568,  1650,
     562,   237,   484,   485,   486,   789,  1245,   502,  1397,  1816,
    1397,  -715,   941,   242,  1535,  -715,  -715,  -715,  -715,  -715,
    -715,  -715,  -715,  -715,  -715,  -715,  -715,  -715,  -715,  -715,
     529,   503,  -715,   611,  -715,   -39,   504,  -715,  1917,  -715,
    1093,   671,  -715,  -715,   507,   236,  -715,   445,  -715,   505,
     508,    92,  -715,   -58,   619,   506,  -715,   145,    36,   -28,
     510,  -715,   300,    36,   159,   661,   -14,    -8,  -715,   560,
    -715,  -715,   592,  -715,  -715,   515,   634,  -715,   524,   556,
     557,   136,  -715,  -715,  -715,   332,   568,    11,  1327,   675,
     490,  1093,  1093,   -34,   327,   525,  1397,   941,  2054,  1093,
     532,  -715,   269,   530,   528,    98,   239,  1093,  1093,  1397,
    -715,  1397,    -4,   533,    -1,  1397,  1397,  1397,  1397,  1397,
    1397,  1397,  1397,  1397,  1397,  1397,  1397,  1397,  1397,  1397,
     734,  -715,    20,  1816,  -715,  1917,    61,  -715,   242,   296,
     236,   737,   738,   412,   740,   276,  -715,  -715,  -715,    61,
    -715,   673,  -715,   448,   445,  -715,  1093,  -715,  1715,  -715,
    -715,  -715,  -715,  1093,  -715,  -715,   742,   560,   576,  1093,
    1093,  -715,   571,  -715,   587,   218,  -715,   332,  1917,   582,
    1917,   543,   545,  -715,  1650,  -715,  -715,   546,  -715,   547,
    -715,  -715,   549,  -715,  -715,   552,  -715,  -715,  -715,  -715,
     555,  -715,  -715,   147,  -715,   678,  1093,   295,  -715,  -715,
     558,   666,   142,   282,   336,  1093,  1093,  -715,   675,   148,
     660,   177,  -715,  1093,  -715,   161,  1093,  -715,   646,  1974,
    2091,  1397,   561,   941,  -715,   663,   532,  2091,  2091,  2091,
    2091,   471,   471,   471,   471,   269,   269,   -19,   -19,   -19,
     -68,   567,   678,  1093,  -715,  -715,   290,  1917,   304,  -715,
    -715,  -715,  -715,  -715,   268,   308,  -715,   443,   448,   -12,
      46,  -715,   247,  -715,   563,  -715,    46,  -715,   305,  -715,
    -715,  -715,  -715,  -715,   763,  -715,  -715,   604,   242,   242,
     735,  -715,  1917,  -715,   636,  -715,   593,   313,  -715,   319,
    1917,  1917,  -715,   795,   797,  -715,   798,   799,   800,  -715,
    -715,   702,  -715,   635,  1816,  -715,   147,  -715,  -715,   605,
    -715,   325,    26,  1477,  -715,  1093,   332,  1093,  1093,  -715,
     359,   348,   606,  -715,  -715,   684,   609,  -715,  1397,  2091,
     941,   613,   335,  -715,  -715,  -715,   337,  -715,  -715,  -715,
     814,   412,  -715,  -715,    31,  1994,   215,   615,   616,  1751,
    -715,  -715,  -715,   721,  -715,  -715,  -715,   744,   745,   747,
     727,   448,  -715,  1093,  -715,  -715,   758,  1093,  -715,  1093,
    -715,   180,  -715,  -715,  -715,  -715,   343,   347,   623,   351,
     624,   625,   626,  -715,  -715,   353,  -715,  -715,   739,   656,
     678,  1093,   639,  -715,  -715,   163,   627,   242,   360,  -715,
    1093,  -715,   638,   675,  2054,   641,   357,  -715,  -715,   678,
    -715,  -715,  -715,  1093,   447,  -715,  1093,   362,    46,   643,
     645,   647,   871,  -715,  -715,  -715,   448,   250,   255,   724,
    -715,   -43,   238,  -715,  -715,   676,  -715,  -715,   849,  -715,
    -715,  -715,  -715,   728,   599,   651,   678,  -715,   368,  1549,
    -715,  -715,  -715,   242,   675,   664,  -715,  -715,  -715,   373,
      33,   374,    46,   667,  -715,  1917,  -715,   444,  1093,  1093,
    1093,   685,  -715,   665,  1093,   482,   803,  -715,   678,  -715,
    -715,  -715,   668,  -715,  -715,  -715,  1093,  -715,  -715,  1093,
     394,   672,  1093,   242,   -38,   242,  -715,  -715,   506,  -715,
    -715,  -715,   680,    42,   754,  -715,  -715,   404,   405,  -715,
    1917,   242,  -715,  1093,  -715,  -715,   229,    52,   284,  -715,
    -715,  1093,  -715,  -715,   411,   242,  -715,  -715,   765,  -715,
    -715,   429,  -715,    52,  -715,  -715
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     406,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,    31,    31,    36,
       0,   426,     3,    22,    20,    22,    19,    18,     8,     9,
       7,    11,    16,    17,    13,    14,    12,    15,    10,     0,
     405,     0,   339,   337,   335,   333,   331,   330,   332,   336,
     334,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   338,   340,   377,   119,    38,     0,    59,
      66,    66,    66,     0,     0,     0,     0,   376,   114,     0,
       0,     0,   114,   114,   114,     0,    57,   339,     0,   407,
     408,    30,    27,    29,    28,   360,   361,   359,     0,     0,
       1,   406,     2,     0,     6,     5,   179,   128,   129,   168,
     111,     0,   190,    39,     0,     0,   381,     0,     0,   160,
      42,     0,   123,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,    35,     4,
       0,     0,   154,   141,   142,   140,     0,   144,     0,     0,
     186,   339,   378,   348,   351,   353,     0,     0,   354,     0,
     349,   350,     0,     0,   189,   191,   193,   195,   341,   342,
     343,   352,   344,   345,   346,   347,    33,    32,     0,   380,
       0,     0,   123,     0,   118,     0,     0,     0,   160,   125,
       0,   113,     0,   136,   135,    43,    46,    46,    46,   112,
     109,   110,   410,   409,    38,     0,   353,     0,   177,   153,
       0,   168,   132,   131,   133,   143,   139,     0,   168,     0,
       0,   393,     0,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   356,     0,   355,
     358,   196,   197,    37,     0,    65,     0,     0,   406,     0,
       0,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,     0,   159,   199,   206,   207,   208,   201,   203,
     209,   202,   225,   210,   211,   213,   212,   214,   205,   200,
     216,   310,   217,     0,   427,     0,     0,   121,     0,   124,
       0,     0,   115,   116,     0,     0,    56,   160,    55,     0,
      25,     0,    23,     0,   157,   155,   187,   391,   186,     0,
     167,   169,   174,   186,   181,   183,   180,     0,   137,   392,
     394,   379,     0,   357,   192,     0,     0,    62,     0,     0,
       0,     0,    67,    69,    70,     0,   406,   154,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     220,   287,   219,     0,     0,   391,     0,     0,     0,     0,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   218,   154,     0,   122,     0,     0,   126,   127,   114,
       0,     0,     0,     0,     0,     0,    52,    41,    34,     0,
      21,     0,   178,     0,   160,   156,     0,   389,     0,   390,
     198,   130,   134,     0,   173,   172,   175,   393,     0,     0,
       0,   398,     0,   400,     0,   404,   395,     0,     0,     0,
       0,     0,     0,    61,     0,    88,    82,     0,    84,    94,
      85,    72,     0,    79,    80,     0,    76,    77,    83,    86,
      91,    81,    73,    96,    64,   272,     0,   307,   309,   315,
       0,     0,     0,     0,     0,     0,     0,   248,     0,     0,
       0,     0,   215,     0,   204,     0,     0,   236,   237,     0,
     232,     0,     0,     0,   223,     0,   235,   234,   254,   255,
     256,   257,   258,   259,   260,   227,   226,   229,   228,   230,
     231,     0,   272,     0,    40,   428,     0,     0,     0,    53,
      50,    48,    54,    45,     0,     0,    24,   339,     0,   406,
     385,   158,   362,   364,     0,   367,   385,   365,   149,   188,
     386,   388,   387,   170,     0,   171,   138,     0,   184,   182,
       0,   401,     0,   403,     0,   396,     0,     0,    60,     0,
       0,     0,    68,     0,     0,    78,     0,     0,     0,    87,
     103,     0,   102,     0,     0,    71,    95,    97,    99,     0,
     261,     0,   154,     0,   252,     0,     0,     0,     0,   246,
       0,     0,     0,   303,   244,     0,     0,   245,     0,   233,
       0,     0,     0,   224,   304,   265,     0,   120,   117,    44,
       0,     0,    51,    26,   154,     0,   423,     0,     0,     0,
     384,   369,   382,     0,   422,   414,   420,   418,   421,   416,
       0,     0,   375,     0,   150,   151,   162,     0,   176,     0,
     399,   404,   402,   194,    63,   106,     0,     0,     0,     0,
       0,     0,     0,   104,   101,   123,    98,   100,   274,   166,
     272,     0,   311,   313,   316,     0,     0,   250,     0,   249,
       0,   253,     0,     0,   305,     0,     0,   240,   238,   272,
      49,    47,   267,     0,   339,   366,     0,     0,   385,   386,
     387,     0,     0,   417,   419,   415,     0,   363,   147,     0,
     152,   148,     0,   397,   105,     0,    75,    93,     0,    89,
      74,    90,   108,     0,   168,     0,   272,   263,     0,     0,
     286,   288,   247,   251,     0,     0,   241,   239,   266,     0,
     154,     0,   385,     0,   368,     0,   411,   423,     0,     0,
       0,     0,   107,     0,     0,   277,     0,   262,   272,   312,
     314,   317,     0,   242,   268,   269,     0,   371,   370,     0,
       0,     0,     0,   145,   164,   146,   185,    92,   273,   278,
     279,   280,     0,     0,     0,   264,   243,     0,     0,   383,
       0,   424,   412,     0,   161,   271,     0,     0,     0,   285,
     275,     0,   270,   372,     0,   163,   284,   282,     0,   283,
     281,     0,   413,     0,   165,   276
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -715,  -715,  -715,   784,  -715,   861,  -715,   487,  -715,   520,
    -715,  -715,  -715,  -715,   686,  -715,  -715,  -384,  -123,   376,
     511,   375,  -715,  -715,  -715,   551,  -715,   458,  -715,  -355,
    -715,  -715,  -715,  -715,   328,  -715,  -515,  -715,   -55,  -715,
    -715,  -715,  -715,  -715,  -715,  -180,  -715,  -715,   608,  -234,
     -99,  -715,   253,   -35,   -71,  -715,  -715,  -715,   205,   -72,
    -336,  -715,  -715,  -715,  -153,  -715,  -715,  -715,  -206,   116,
     489,  -715,  -715,  -715,    86,  -338,  -715,  -175,   655,   662,
    -242,  -165,  -171,  -715,  -715,  -715,  -715,  -715,  -715,  -715,
     564,  -715,  -715,  -715,  -715,  -480,  -715,  -715,  -715,  -714,
    -715,  -715,  -715,  -183,  -715,  -715,  -715,  -715,  -715,  -715,
      -2,     1,   -88,  -715,  -715,   743,  -124,  -715,  -715,   748,
     885,  -715,  -440,   224,  -715,  -715,  -715,  -715,    21,  -715,
    -715,  -487,  -248,  -715,   492,  -715,   594,  -715,   283,  -715,
    -715,  -715,   779,  -715,  -715,  -715,  -715,  -409
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    20,    21,    22,    23,   104,   301,   302,    24,    92,
      25,   177,    26,    98,   113,    27,    28,   121,   196,   296,
     395,   396,    29,    30,    31,   116,   331,   332,   333,   453,
     559,   555,   565,   566,   567,   334,   568,    32,   125,    33,
     292,   293,    34,    35,    36,   187,    37,   188,   189,    38,
     211,   212,   213,   353,   146,   147,   216,   626,   627,   109,
     210,   304,   404,   405,   184,   690,   774,   706,   150,   310,
     311,   416,   535,   142,   221,   305,   163,   164,   165,   166,
     306,   307,   264,   265,   266,   267,   268,   269,   356,   270,
     344,   271,   272,   273,   520,   570,   704,   762,   763,   780,
     274,   275,   276,   235,   236,   237,   277,   278,   279,   280,
     281,    65,   282,   168,   169,   170,   171,   172,   173,   174,
     175,   521,   522,   523,   677,   524,   525,    76,   526,   180,
     610,   611,   612,   410,   318,   319,   320,   425,   545,    39,
      40,    89,    90,   527,   620,   772,   102,   285
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      64,   456,   250,    64,   108,   308,   197,   198,    64,   511,
      64,   471,   313,    64,   327,   773,    88,   208,   263,   547,
     354,   549,   595,    66,   167,   209,    69,   131,   132,   133,
     206,    77,   106,    81,   209,   289,    95,    96,   149,   622,
     209,   183,   303,   420,   127,   209,   503,   209,   776,   407,
     218,   647,   312,   607,   314,   316,   421,   323,   776,   409,
     357,   777,   283,   788,   247,   153,   154,   155,    95,    96,
     365,    64,   546,    64,   214,   357,   358,   214,   606,   795,
     342,   154,   155,    64,   357,   348,    85,   350,   294,   352,
     343,   358,    41,   219,   119,   248,   122,   355,    67,   481,
     358,   407,   454,    15,   357,   484,   134,   409,   129,    64,
     220,   156,   152,    68,    64,    64,    64,   179,   571,   365,
     358,    64,   485,    64,   190,   388,   482,    86,   422,    64,
      64,    64,   199,   631,    64,    64,   380,    88,   181,   182,
     730,   636,   637,   309,   397,   592,   192,   130,   407,   402,
     167,   157,   287,   200,   201,   423,   167,   461,   204,   153,
     154,   155,    95,    96,   529,   596,   288,   158,   384,   424,
     707,   385,   406,   609,   249,   348,   462,   463,    79,   295,
     732,   687,   469,   379,    64,   380,    73,   284,   479,   718,
     480,   724,   477,   478,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   778,
     412,   506,   779,   159,   160,   161,   357,   671,   455,   778,
      64,   656,   779,   321,   515,   408,   737,   502,   159,    74,
     207,   584,   358,   650,   587,   748,   651,   214,   672,   391,
     745,    75,    97,   613,    64,   157,   727,    64,   312,    78,
     335,   528,   666,   560,   538,   539,   501,    64,   765,   162,
     357,   158,   392,   357,    91,    80,   357,   325,   673,   576,
     561,   391,   408,   100,    97,   613,   358,   357,   613,   358,
     351,   357,   358,   614,   585,   608,    64,   393,   615,   190,
     143,   586,   107,   358,   600,   616,   617,   358,   167,   400,
     580,   581,   401,   143,   460,   474,   548,   159,   160,   161,
     589,   167,   618,   708,   101,   614,   750,   619,   614,   601,
     615,   562,   563,   615,   418,   564,   144,   616,   617,   103,
     616,   617,   126,   110,   507,   719,   459,   111,   721,   144,
     543,   544,   394,   433,   618,   435,   434,   618,  -423,   619,
     542,  -423,   619,   162,   114,   474,   357,   148,   112,   436,
     357,   784,   145,   437,   438,   439,   440,   441,    97,   442,
     710,   123,   358,   357,   394,   145,   358,   443,   543,   544,
     106,    64,   583,    64,   746,   286,   505,   406,   202,   358,
     385,   754,   414,   577,   411,    70,   758,   786,   787,   417,
     357,    64,   444,   731,   504,    71,   532,   365,   767,   115,
     655,   768,   657,   658,    82,   120,   358,   664,   357,   415,
     445,   465,   446,   447,    83,   124,    64,   675,    64,   284,
     578,   284,    64,   582,   358,   335,    72,   448,   128,   624,
     625,   466,   449,   337,   450,   338,   475,   467,   591,   476,
     466,   517,   789,   790,   451,    84,   579,  -373,   688,   660,
    -374,    43,   691,   137,   692,   702,   357,  -425,   376,   377,
     378,   379,   613,   380,   136,    44,     1,   357,   357,   659,
     712,   135,   358,   513,     2,    45,   514,   452,   623,   624,
     625,     3,   140,   358,   358,   713,     4,   597,   735,    46,
     244,   572,   138,   573,    47,    64,     5,   141,   598,     6,
       7,   599,   614,   176,   514,   603,    64,   615,   244,   178,
     634,     8,     9,   385,   616,   617,   635,   360,   751,   385,
     183,    10,   649,   185,    11,   406,    48,    93,    94,    49,
      64,   618,   668,   284,   669,   406,   619,   406,    64,    64,
     694,   284,   284,   385,   695,   665,    12,   385,   697,   186,
      13,   698,    64,   753,   717,   755,   191,   406,   193,   722,
      50,   654,   723,   297,   298,   738,    14,   752,   406,   194,
     744,   747,    15,   406,   406,   645,   195,   771,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
     209,   769,   364,    64,   385,    79,   152,   680,   785,   365,
     681,   782,   783,   205,   406,   406,   312,   215,   792,    64,
      63,   385,   117,   118,    16,    17,    18,   217,   715,   518,
     223,   224,   225,   226,   227,   228,   794,   238,   222,   413,
     251,   153,   154,   155,    95,    96,   759,   760,   761,   604,
      43,   605,   239,   720,   519,   222,   240,   243,   245,   244,
     246,  -426,  -426,   290,    44,  -426,  -426,   291,   374,   375,
     376,   377,   378,   379,    45,   380,   300,   148,    19,   742,
      64,   317,   326,    15,    64,   252,   253,   254,    46,   336,
     339,   340,   341,    47,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   349,   741,   381,   382,
     386,   383,   389,   390,   399,   403,   406,   398,   419,   427,
     413,   428,    42,    64,   255,    48,   284,   157,    49,   429,
     430,   468,    43,   431,   432,   106,   380,   472,   473,   483,
     206,   509,   510,   158,   512,   534,    44,   537,   540,   550,
     541,   551,   553,   554,   315,   556,    45,   569,   557,    50,
     256,   558,   575,   481,   357,   574,   628,   590,    64,   593,
      46,   284,   594,   621,   257,    47,   629,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,   159,
     160,   161,   251,   153,   154,   155,    95,    96,   632,   630,
     633,   638,    43,   639,   640,   641,   642,    48,   643,    63,
      49,   648,   644,   661,   662,   663,    44,   258,   670,   259,
     667,   676,   682,   678,   683,   684,    45,   685,   686,   689,
     696,   699,   700,   701,   711,   260,   261,   252,   253,   254,
      46,    50,   705,   262,   714,    47,   703,   709,   716,  -339,
      97,  -340,   729,   725,   564,   733,   734,   736,   756,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,   743,   757,   749,   517,   766,   255,    48,   770,   157,
      49,   764,   781,   793,    43,   139,   105,   775,   516,   602,
     299,    63,   552,   728,   646,   158,   387,   791,    44,   324,
     322,   508,   533,   342,    99,   241,   726,   464,    45,   536,
     242,    50,   256,   426,   693,   203,     0,     0,     0,     0,
       0,     0,    46,     0,     0,    79,   257,    47,     0,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,   159,   160,   161,   251,   153,   154,   155,    95,    96,
       0,     0,     0,     0,    43,     0,     0,     0,     0,    48,
       0,    63,    49,     0,     0,     0,     0,     0,    44,   258,
       0,   259,     0,     0,     0,     0,     0,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,   260,   261,   252,
     253,   254,    46,    50,     0,   262,     0,    47,     0,     0,
       0,   106,    97,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,     0,     0,     0,     0,     0,   255,    48,
       0,   157,    49,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,   158,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,   256,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   519,   257,     0,
       0,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,   159,   160,   161,   251,   153,   154,   155,
      95,    96,     0,     0,     0,     0,    43,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     1,     0,
      44,   258,     0,   259,     0,     0,     2,     0,     0,     0,
      45,     0,     0,     3,     0,     0,     0,     0,     4,   260,
     261,   252,   253,   254,    46,     0,     0,   262,     5,    47,
       0,     6,     7,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     8,     9,     0,     0,     0,     0,     0,
       0,     0,     0,    10,     0,     0,    11,     0,     0,     0,
     255,    48,     0,   157,    49,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    12,   158,
       0,     0,    13,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,   256,     0,    14,     0,
       0,     0,     0,     0,    15,     0,     0,     0,     0,     0,
     257,     0,     0,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,   159,   160,   161,   251,   153,
     154,   155,    95,    96,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,     0,    63,    16,    17,    18,     0,
       0,     0,    44,   258,     0,   259,     0,     0,     0,     0,
       0,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,   260,   261,   345,   253,   254,    46,     0,     0,   262,
       0,    47,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     457,     0,   255,    48,     0,   157,    49,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   158,     0,     0,    44,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,    50,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    46,     0,
       0,     0,   257,    47,     0,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,   159,   160,   161,
     251,   153,   154,   155,    95,    96,     0,     0,     0,     0,
      43,     0,     0,     0,     0,    48,     0,    63,    49,     0,
       0,     0,     0,     0,    44,   258,     0,   259,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     0,
       0,     0,     0,   260,   261,     0,   253,   254,    46,    50,
       0,   347,     0,    47,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,     0,
     652,     0,     0,     0,   255,    48,     0,   157,    49,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,   158,    44,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,    50,
     346,     0,     0,     0,     0,     0,   458,     0,    46,     0,
       0,     0,     0,    47,   257,     0,     0,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,   159,
     160,   161,   739,     0,   359,     0,     0,     0,     0,     0,
       0,     0,    43,     0,     0,    48,     0,     0,    49,    63,
       0,     0,     0,     0,     0,     0,    44,   258,     0,   259,
       0,     0,     0,     0,     0,     0,    45,     0,     0,     0,
       0,   360,     0,     0,     0,   260,   261,     0,     0,    50,
      46,     0,     0,   347,     0,    47,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    48,   361,     0,
      49,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,    87,     0,     0,     0,     0,   362,     0,
       0,     0,     0,    43,     0,   363,   364,     0,     0,     0,
       0,    50,     0,   365,   366,     0,   653,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    45,     0,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    46,     0,     0,     0,     0,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   530,   531,
       0,    63,   367,   368,   369,   370,   371,     0,    43,   372,
     373,     0,   374,   375,   376,   377,   378,   379,    48,   380,
       0,    49,    44,     0,     0,     0,     0,     0,   740,     0,
       0,     0,    45,     0,   679,   531,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,    46,     0,     0,     0,
       0,    47,    50,     0,     0,     0,     0,     0,    44,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    45,     0,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    46,    48,     0,     0,    49,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    42,
       0,     0,    63,     0,   328,   329,   330,     0,     0,    43,
       0,     0,     0,     0,     0,     0,     0,    50,     0,    48,
       0,     0,    49,    44,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    45,     0,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    46,     0,     0,
       0,     0,    47,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,     0,
       0,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,     0,    48,     0,     0,    49,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,    63,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,    44,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    46,     0,
       0,     0,     0,    47,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   151,     0,     0,    63,     0,
       0,     0,     0,     0,     0,    43,     0,   674,     0,     0,
       0,     0,     0,     0,     0,    48,     0,    43,    49,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    45,
       0,    44,     0,     0,     0,     0,     0,     0,     0,     0,
     360,    45,     0,    46,     0,     0,     0,     0,    47,    50,
       0,     0,     0,     0,     0,    46,     0,     0,     0,     0,
      47,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,     0,
      48,     0,     0,    49,     0,     0,     0,   361,     0,     0,
       0,     0,    48,     0,     0,    49,     0,     0,     0,    63,
       0,     0,   588,     0,     0,     0,     0,   470,     0,     0,
       0,     0,     0,     0,    50,   364,     0,     0,     0,     0,
     360,     0,   365,   366,     0,     0,    50,     0,     0,     0,
       0,     0,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,     0,   360,     0,     0,
       0,     0,     0,     0,    63,     0,     0,   361,     0,     0,
       0,   367,   368,   369,   370,   371,    63,     0,   372,   373,
       0,   374,   375,   376,   377,   378,   379,   470,   380,     0,
       0,     0,     0,     0,     0,   364,     0,     0,     0,     0,
       0,     0,   365,   366,  -426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   364,     0,     0,     0,     0,     0,     0,   365,
    -426,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,   368,   369,   370,   371,     0,     0,   372,   373,
       0,   374,   375,   376,   377,   378,   379,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -426,  -426,
    -426,   370,   371,     0,     0,   372,   373,     0,   374,   375,
     376,   377,   378,   379,     0,   380
};

static const yytype_int16 yycheck[] =
{
       2,   337,   182,     5,    39,   211,   129,   130,    10,   393,
      12,   349,   218,    15,   248,    53,    15,   141,   183,   428,
     262,   430,   502,     2,   112,    14,     5,    82,    83,    84,
       6,    10,    60,    12,    14,   188,     7,     8,   109,   526,
      14,    85,   207,    57,    79,    14,   382,    14,     6,     3,
     149,   566,   217,    65,   219,   220,    64,   240,     6,   307,
     118,    19,   185,   777,    96,     4,     5,     6,     7,     8,
     138,    73,   427,    75,   146,   118,   134,   149,   518,   793,
     114,     5,     6,    85,   118,   256,    21,   258,   115,   260,
     255,   134,    29,    57,    73,   127,    75,   262,     3,   103,
     134,     3,   336,   115,   118,   106,    85,   355,    96,   111,
      74,    50,   111,     3,   116,   117,   118,   116,   456,   138,
     134,   123,   123,   125,   123,   290,   130,    62,   136,   131,
     132,   133,   131,   542,   136,   137,   204,   136,   117,   118,
     183,   550,   551,   214,   297,   483,   125,   135,     3,   207,
     238,    90,   187,   132,   133,   163,   244,   340,   137,     4,
       5,     6,     7,     8,   406,   503,   210,   106,   207,   177,
     650,   210,   210,   127,   206,   346,   341,   342,   206,   206,
     695,   621,   347,   202,   186,   204,    96,   186,   359,   669,
     361,   678,   357,   358,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   167,
     309,   386,   170,   152,   153,   154,   118,   601,   207,   167,
     222,   576,   170,   222,   399,   127,   706,   207,   152,    96,
     206,   473,   134,   207,   476,   722,   572,   309,   207,     3,
     207,   100,   213,    28,   246,    90,   686,   249,   413,    82,
     249,   404,   590,   106,   419,   420,   380,   259,   738,   198,
     118,   106,    26,   118,   156,    12,   118,   246,   604,   127,
     123,     3,   127,     0,   213,    28,   134,   118,    28,   134,
     259,   118,   134,    68,   123,   519,   288,    51,    73,   288,
      11,   130,    39,   134,    26,    80,    81,   134,   386,   207,
     465,   466,   210,    11,   339,   207,   429,   152,   153,   154,
     481,   399,    97,   651,   209,    68,   725,   102,    68,    51,
      73,   174,   175,    73,   165,   178,    47,    80,    81,   115,
      80,    81,    79,     3,   389,   673,   338,   208,   676,    47,
     160,   161,   106,   207,    97,    13,   210,    97,   101,   102,
     132,   101,   102,   198,    96,   207,   118,    78,   206,    27,
     118,   770,    83,    31,    32,    33,    34,    35,   213,    37,
     207,   125,   134,   118,   106,    83,   134,    45,   160,   161,
      60,   383,   205,   385,   720,    65,   385,   210,   135,   134,
     210,   729,    92,   111,   308,    72,   734,   168,   169,   313,
     118,   403,    70,   165,   383,    82,   408,   138,   746,   129,
     575,   749,   577,   578,    72,     3,   134,   588,   118,   119,
      88,    94,    90,    91,    82,   129,   428,   212,   430,   428,
      94,   430,   434,   468,   134,   434,   113,   105,   135,   184,
     185,   114,   110,   206,   112,   208,   207,   120,   483,   210,
     114,     3,   168,   169,   122,   113,   120,   210,   623,   111,
     210,    13,   627,    86,   629,   645,   118,     0,   199,   200,
     201,   202,    28,   204,   210,    27,     9,   118,   118,   120,
     120,   127,   134,   207,    17,    37,   210,   155,   183,   184,
     185,    24,    99,   134,   134,   660,    29,   207,   704,    51,
     210,   206,   187,   208,    56,   507,    39,   126,   507,    42,
      43,   207,    68,     4,   210,   207,   518,    73,   210,   123,
     207,    54,    55,   210,    80,    81,   207,    56,    84,   210,
      85,    64,   207,    95,    67,   210,    88,    17,    18,    91,
     542,    97,   207,   542,   207,   210,   102,   210,   550,   551,
     207,   550,   551,   210,   207,   590,    89,   210,   207,   206,
      93,   210,   564,   728,   207,   730,    48,   210,   207,   207,
     122,   573,   210,   197,   198,   207,   109,   133,   210,   207,
     207,   207,   115,   210,   210,   564,     4,   752,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      14,   207,   131,   605,   210,   206,   605,   609,   773,   138,
     609,   207,   207,   206,   210,   210,   781,   117,   207,   621,
     172,   210,    71,    72,   157,   158,   159,   128,   663,   181,
     140,   141,   142,   143,   144,   145,   207,   206,   208,   210,
       3,     4,     5,     6,     7,     8,   164,   165,   166,   206,
      13,   208,     4,   206,   206,   208,     4,   207,    48,   210,
     133,   190,   191,   187,    27,   194,   195,    93,   197,   198,
     199,   200,   201,   202,    37,   204,     3,    78,   211,   714,
     682,   121,     3,   115,   686,    48,    49,    50,    51,   127,
     206,   206,   206,    56,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   204,   709,   179,   206,
     206,   100,    41,   206,   206,    96,   210,   212,    57,   127,
     210,   206,     3,   725,    87,    88,   725,    90,    91,    95,
     206,   206,    13,   177,   177,    60,   204,   207,   210,   206,
       6,     4,     4,   106,     4,     3,    27,   171,   177,   206,
     163,   206,   206,   206,   117,   206,    37,    79,   206,   122,
     123,   206,    96,   103,   118,   207,     3,   206,   770,   106,
      51,   770,   205,   210,   137,    56,   172,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,     3,     4,     5,     6,     7,     8,   162,    64,
     207,     6,    13,     6,     6,     6,     6,    88,   106,   172,
      91,   206,   177,   207,   130,   206,    27,   180,     4,   182,
     207,   206,   101,   207,    80,    80,    37,    80,   101,    71,
     207,   207,   207,   207,   207,   198,   199,    48,    49,    50,
      51,   122,   186,   206,   206,    56,   107,   208,   207,   206,
     213,   206,   128,   206,   178,     6,   128,   206,   173,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   207,   207,   206,     3,   207,    87,    88,   206,    90,
      91,    78,   128,   118,    13,   101,    25,   207,   401,   514,
     204,   172,   434,   688,   566,   106,   288,   781,    27,   244,
     238,   390,   413,   114,    19,   162,   682,   343,    37,   417,
     162,   122,   123,   319,   631,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,   206,   137,    56,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    88,
      -1,   172,    91,    -1,    -1,    -1,    -1,    -1,    27,   180,
      -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,   199,    48,
      49,    50,    51,   122,    -1,   206,    -1,    56,    -1,    -1,
      -1,    60,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   137,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,     9,    -1,
      27,   180,    -1,   182,    -1,    -1,    17,    -1,    -1,    -1,
      37,    -1,    -1,    24,    -1,    -1,    -1,    -1,    29,   198,
     199,    48,    49,    50,    51,    -1,    -1,   206,    39,    56,
      -1,    42,    43,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    -1,    -1,    -1,
      87,    88,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,   106,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,    -1,   109,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,   172,   157,   158,   159,    -1,
      -1,    -1,    27,   180,    -1,   182,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198,   199,    48,    49,    50,    51,    -1,    -1,   206,
      -1,    56,    -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    87,    88,    -1,    90,    91,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    -1,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,   137,    56,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    88,    -1,   172,    91,    -1,
      -1,    -1,    -1,    -1,    27,   180,    -1,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,   199,    -1,    49,    50,    51,   122,
      -1,   206,    -1,    56,    -1,    -1,    -1,    -1,   213,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,    -1,
       3,    -1,    -1,    -1,    87,    88,    -1,    90,    91,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,   106,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,   122,
     123,    -1,    -1,    -1,    -1,    -1,   199,    -1,    51,    -1,
      -1,    -1,    -1,    56,   137,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,     3,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    13,    -1,    -1,    88,    -1,    -1,    91,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    27,   180,    -1,   182,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    -1,   198,   199,    -1,    -1,   122,
      51,    -1,    -1,   206,    -1,    56,    -1,    -1,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,   103,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    13,    -1,   130,   131,    -1,    -1,    -1,
      -1,   122,    -1,   138,   139,    -1,   199,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
      -1,   172,   187,   188,   189,   190,   191,    -1,    13,   194,
     195,    -1,   197,   198,   199,   200,   201,   202,    88,   204,
      -1,    91,    27,    -1,    -1,    -1,    -1,    -1,   199,    -1,
      -1,    -1,    37,    -1,     3,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    -1,    51,    -1,    -1,    -1,
      -1,    56,   122,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    51,    88,    -1,    -1,    91,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,   172,    -1,   174,   175,   176,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    88,
      -1,    -1,    91,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,    51,    -1,    -1,
      -1,    -1,    56,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,    51,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    13,    91,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    37,    -1,    51,    -1,    -1,    -1,    -1,    56,   122,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,   172,
      -1,    -1,   118,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   131,    -1,    -1,    -1,    -1,
      56,    -1,   138,   139,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,    -1,   103,    -1,    -1,
      -1,   187,   188,   189,   190,   191,   172,    -1,   194,   195,
      -1,   197,   198,   199,   200,   201,   202,   123,   204,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,   189,   190,   191,    -1,    -1,   194,   195,
      -1,   197,   198,   199,   200,   201,   202,    -1,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
     189,   190,   191,    -1,    -1,   194,   195,    -1,   197,   198,
     199,   200,   201,   202,    -1,   204
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     9,    17,    24,    29,    39,    42,    43,    54,    55,
      64,    67,    89,    93,   109,   115,   157,   158,   159,   211,
     215,   216,   217,   218,   222,   224,   226,   229,   230,   236,
     237,   238,   251,   253,   256,   257,   258,   260,   263,   353,
     354,    29,     3,    13,    27,    37,    51,    56,    88,    91,
     122,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   172,   324,   325,   342,     3,     3,   342,
      72,    82,   113,    96,    96,   100,   341,   342,    82,   206,
     266,   342,    72,    82,   113,    21,    62,     3,   325,   355,
     356,   156,   223,   223,   223,     7,     8,   213,   227,   334,
       0,   209,   360,   115,   219,   219,    60,   266,   267,   273,
       3,   208,   206,   228,    96,   129,   239,   239,   239,   342,
       3,   231,   342,   125,   129,   252,   266,   267,   135,    96,
     135,   252,   252,   252,   342,   127,   210,    86,   187,   217,
      99,   126,   287,    11,    47,    83,   268,   269,    78,   268,
     282,     3,   325,     4,     5,     6,    50,    90,   106,   152,
     153,   154,   198,   290,   291,   292,   293,   326,   327,   328,
     329,   330,   331,   332,   333,   334,     4,   225,   123,   325,
     343,   342,   342,    85,   278,    95,   206,   259,   261,   262,
     325,    48,   342,   207,   207,     4,   232,   232,   232,   325,
     342,   342,   266,   356,   342,   206,     6,   206,   330,    14,
     274,   264,   265,   266,   273,   117,   270,   128,   264,    57,
      74,   288,   208,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   317,   318,   319,   206,     4,
       4,   329,   333,   207,   210,    48,   133,    96,   127,   206,
     259,     3,    48,    49,    50,    87,   123,   137,   180,   182,
     198,   199,   206,   295,   296,   297,   298,   299,   300,   301,
     303,   305,   306,   307,   314,   315,   316,   320,   321,   322,
     323,   324,   326,   232,   325,   361,    65,   267,   210,   278,
     187,    93,   254,   255,   115,   206,   233,   233,   233,   228,
       3,   220,   221,   295,   275,   289,   294,   295,   282,   268,
     283,   284,   295,   282,   295,   117,   295,   121,   348,   349,
     350,   325,   293,   317,   292,   342,     3,   263,   174,   175,
     176,   240,   241,   242,   249,   325,   127,   206,   208,   206,
     206,   206,   114,   295,   304,    48,   123,   206,   296,   204,
     296,   342,   296,   267,   294,   295,   302,   118,   134,    19,
      56,   103,   123,   130,   131,   138,   139,   187,   188,   189,
     190,   191,   194,   195,   197,   198,   199,   200,   201,   202,
     204,   179,   206,   100,   207,   210,   206,   262,   295,    41,
     206,     3,    26,    51,   106,   234,   235,   278,   212,   206,
     207,   210,   207,    96,   276,   277,   210,     3,   127,   346,
     347,   288,   264,   210,    92,   119,   285,   288,   165,    57,
      57,    64,   136,   163,   177,   351,   350,   127,   206,    95,
     206,   177,   177,   207,   210,    13,    27,    31,    32,    33,
      34,    35,    37,    45,    70,    88,    90,    91,   105,   110,
     112,   122,   155,   243,   263,   207,   274,     3,   199,   324,
     267,   317,   295,   295,   304,    94,   114,   120,   206,   295,
     123,   289,   207,   210,   207,   207,   210,   295,   295,   296,
     296,   103,   130,   206,   106,   123,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   330,   207,   274,   342,   325,   291,   252,   234,     4,
       4,   231,     4,   207,   210,   291,   221,     3,   181,   206,
     308,   335,   336,   337,   339,   340,   342,   357,   278,   294,
       3,     4,   324,   284,     3,   286,   348,   171,   295,   295,
     177,   163,   132,   160,   161,   352,   243,   361,   232,   361,
     206,   206,   241,   206,   206,   245,   206,   206,   206,   244,
     106,   123,   174,   175,   178,   246,   247,   248,   250,    79,
     309,   289,   206,   208,   207,    96,   127,   111,    94,   120,
     295,   295,   267,   205,   294,   123,   130,   294,   118,   296,
     206,   267,   289,   106,   205,   309,   289,   207,   325,   207,
      26,    51,   235,   207,   206,   208,   336,    65,   263,   127,
     344,   345,   346,    28,    68,    73,    80,    81,    97,   102,
     358,   210,   345,   183,   184,   185,   271,   272,     3,   172,
      64,   361,   162,   207,   207,   207,   361,   361,     6,     6,
       6,     6,     6,   106,   177,   342,   248,   250,   206,   207,
     207,   274,     3,   199,   324,   295,   243,   295,   295,   120,
     111,   207,   130,   206,   296,   267,   289,   207,   207,   207,
       4,   231,   207,   274,     3,   212,   206,   338,   207,     3,
     324,   325,   101,    80,    80,    80,   101,   336,   295,    71,
     279,   295,   295,   352,   207,   207,   207,   207,   210,   207,
     207,   207,   259,   107,   310,   186,   281,   309,   289,   208,
     207,   207,   120,   295,   206,   267,   207,   207,   309,   289,
     206,   289,   207,   210,   345,   206,   337,   336,   272,   128,
     183,   165,   250,     6,   128,   282,   206,   309,   207,     3,
     199,   324,   267,   207,   207,   207,   274,   207,   345,   206,
     361,    84,   133,   295,   289,   295,   173,   207,   289,   164,
     165,   166,   311,   312,    78,   309,   207,   289,   289,   207,
     206,   295,   359,    53,   280,   207,     6,    19,   167,   170,
     313,   128,   207,   207,   361,   295,   168,   169,   313,   168,
     169,   283,   207,   118,   207,   313
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   214,   215,   216,   216,   217,   217,   217,   217,   217,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   219,   219,   220,   220,   221,   221,   222,   222,   222,
     223,   223,   224,   225,   226,   227,   227,   228,   228,   229,
     230,   230,   231,   232,   233,   233,   233,   234,   234,   234,
     234,   234,   234,   235,   235,   236,   236,   237,   237,   237,
     238,   238,   238,   238,   238,   239,   239,   240,   240,   241,
     241,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     244,   244,   245,   245,   245,   246,   246,   247,   247,   247,
     247,   248,   248,   248,   248,   249,   249,   249,   250,   251,
     251,   251,   251,   252,   252,   253,   254,   255,   256,   257,
     258,   258,   259,   259,   260,   261,   261,   262,   263,   263,
     263,   264,   264,   265,   265,   266,   266,   267,   267,   268,
     269,   269,   269,   270,   270,   271,   271,   271,   271,   271,
     272,   272,   273,   274,   274,   275,   276,   276,   277,   278,
     278,   279,   279,   280,   280,   281,   281,   282,   282,   283,
     283,   284,   285,   285,   285,   286,   286,   287,   287,   287,
     288,   288,   288,   288,   288,   288,   288,   289,   289,   290,
     290,   291,   291,   292,   292,   293,   293,   293,   294,   295,
     295,   295,   295,   295,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   297,   297,   297,   298,
     298,   298,   298,   298,   298,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   300,   300,   301,   301,
     301,   301,   301,   301,   302,   302,   303,   303,   303,   303,
     304,   304,   305,   305,   306,   306,   306,   306,   306,   306,
     306,   307,   307,   307,   307,   307,   307,   308,   308,   308,
     308,   309,   309,   310,   310,   311,   311,   311,   312,   312,
     312,   313,   313,   313,   313,   313,   314,   315,   316,   317,
     317,   317,   317,   317,   317,   318,   318,   318,   318,   318,
     318,   319,   319,   320,   321,   322,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   325,
     325,   326,   326,   326,   326,   326,   326,   326,   327,   328,
     328,   329,   329,   330,   331,   332,   333,   333,   333,   334,
     334,   334,   335,   335,   336,   336,   336,   337,   337,   337,
     337,   338,   338,   339,   339,   340,   341,   342,   342,   342,
     343,   343,   344,   344,   345,   345,   346,   346,   346,   346,
     347,   347,   348,   348,   349,   349,   350,   350,   351,   351,
     351,   351,   352,   352,   352,   353,   353,   354,   355,   355,
     356,   357,   357,   357,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   359,   360,   360,   361,   361
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     3,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     0,     1,     3,     1,     4,     2,     2,     2,
       1,     0,     4,     1,     6,     2,     0,     3,     0,     3,
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
       1,     1,     1,     1,     0,     4,     4,     2,     2,     0,
       1,     1,     8,     1,     0,     1,     1,     0,     2,     2,
       0,     4,     0,     2,     0,     6,     0,     3,     0,     1,
       3,     3,     1,     1,     0,     0,     2,     2,     4,     0,
       2,     2,     4,     2,     4,     8,     0,     1,     3,     1,
       0,     1,     3,     1,     6,     1,     2,     2,     2,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     2,     2,
       2,     2,     2,     3,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     4,     3,     3,     3,     3,     5,     6,
       5,     6,     7,     8,     3,     3,     4,     6,     3,     5,
       4,     5,     4,     5,     3,     3,     3,     3,     3,     3,
       3,     4,     7,     6,     8,     4,     6,     3,     5,     5,
       7,     6,     0,     3,     0,     2,     5,     0,     1,     1,
       1,     2,     2,     2,     2,     1,     6,     2,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     5,     1,     3,     1,     3,
       1,     5,     7,     5,     7,     3,     5,     7,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     3,     2,     1,
       1,     1,     1,     3,     1,     1,     3,     1,     4,     2,
       5,     3,     5,     1,     3,     2,     1,     1,     3,     5,
       1,     0,     1,     5,     1,     0,     2,     2,     2,     1,
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
#line 244 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2513 "bison_parser.cpp"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 244 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2519 "bison_parser.cpp"
        break;

    case YYSYMBOL_FLOATVAL: /* FLOATVAL  */
#line 227 "bison_parser.y"
            { }
#line 2525 "bison_parser.cpp"
        break;

    case YYSYMBOL_INTVAL: /* INTVAL  */
#line 227 "bison_parser.y"
            { }
#line 2531 "bison_parser.cpp"
        break;

    case YYSYMBOL_DOLLAR_PARAM: /* DOLLAR_PARAM  */
#line 227 "bison_parser.y"
            { }
#line 2537 "bison_parser.cpp"
        break;

    case YYSYMBOL_NAMED_PARAM: /* NAMED_PARAM  */
#line 244 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2543 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement_list: /* statement_list  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).stmt_vec)) {
    for (auto ptr : *(((*yyvaluep).stmt_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).stmt_vec));
}
#line 2556 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2562 "bison_parser.cpp"
        break;

    case YYSYMBOL_preparable_statement: /* preparable_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2568 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_hints: /* opt_hints  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2581 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint_list: /* hint_list  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2594 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint: /* hint  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2600 "bison_parser.cpp"
        break;

    case YYSYMBOL_transaction_statement: /* transaction_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).transaction_stmt)); }
#line 2606 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_statement: /* prepare_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).prep_stmt)); }
#line 2612 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_target_query: /* prepare_target_query  */
#line 244 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2618 "bison_parser.cpp"
        break;

    case YYSYMBOL_odbc_call_statement: /* odbc_call_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2624 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_odbc_return: /* opt_odbc_return  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2630 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_call_args: /* opt_call_args  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2643 "bison_parser.cpp"
        break;

    case YYSYMBOL_execute_statement: /* execute_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2649 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_statement: /* import_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).import_stmt)); }
#line 2655 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_type: /* file_type  */
#line 227 "bison_parser.y"
            { }
#line 2661 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_path: /* file_path  */
#line 244 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2667 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_import_export_options: /* opt_import_export_options  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2673 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_export_options: /* import_export_options  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2679 "bison_parser.cpp"
        break;

    case YYSYMBOL_csv_option: /* csv_option  */
#line 253 "bison_parser.y"
            {
  free(((*yyvaluep).csv_option_t)->second);
  delete (((*yyvaluep).csv_option_t));
}
#line 2688 "bison_parser.cpp"
        break;

    case YYSYMBOL_export_statement: /* export_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).export_stmt)); }
#line 2694 "bison_parser.cpp"
        break;

    case YYSYMBOL_show_statement: /* show_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).show_stmt)); }
#line 2700 "bison_parser.cpp"
        break;

    case YYSYMBOL_create_statement: /* create_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).create_stmt)); }
#line 2706 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_not_exists: /* opt_not_exists  */
#line 227 "bison_parser.y"
            { }
#line 2712 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem_commalist: /* table_elem_commalist  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).table_element_vec)) {
    for (auto ptr : *(((*yyvaluep).table_element_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_element_vec));
}
#line 2725 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem: /* table_elem  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).table_element_t)); }
#line 2731 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_def: /* column_def  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).column_t)); }
#line 2737 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_type: /* column_type  */
#line 227 "bison_parser.y"
            { }
#line 2743 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_time_precision: /* opt_time_precision  */
#line 227 "bison_parser.y"
            { }
#line 2749 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_decimal_specification: /* opt_decimal_specification  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).ival_pair)); }
#line 2755 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_column_constraints: /* opt_column_constraints  */
#line 257 "bison_parser.y"
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
#line 2777 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraints: /* column_constraints  */
#line 257 "bison_parser.y"
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
#line 2799 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraint: /* column_constraint  */
#line 227 "bison_parser.y"
            { }
#line 2805 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_constraint: /* table_constraint  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).table_constraint_t)); }
#line 2811 "bison_parser.cpp"
        break;

    case YYSYMBOL_references_spec: /* references_spec  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).references_spec_t)); }
#line 2817 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_statement: /* drop_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).drop_stmt)); }
#line 2823 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_exists: /* opt_exists  */
#line 227 "bison_parser.y"
            { }
#line 2829 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_statement: /* alter_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).alter_stmt)); }
#line 2835 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_action: /* alter_action  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).alter_action_t)); }
#line 2841 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_action: /* drop_action  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).drop_action_t)); }
#line 2847 "bison_parser.cpp"
        break;

    case YYSYMBOL_delete_statement: /* delete_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2853 "bison_parser.cpp"
        break;

    case YYSYMBOL_truncate_statement: /* truncate_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 2859 "bison_parser.cpp"
        break;

    case YYSYMBOL_insert_statement: /* insert_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).insert_stmt)); }
#line 2865 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_column_list: /* opt_column_list  */
#line 236 "bison_parser.y"
            {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 2878 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_statement: /* update_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).update_stmt)); }
#line 2884 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause_commalist: /* update_clause_commalist  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).update_vec)) {
    for (auto ptr : *(((*yyvaluep).update_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).update_vec));
}
#line 2897 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause: /* update_clause  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).update_t)); }
#line 2903 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_statement: /* select_statement  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2909 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation: /* select_within_set_operation  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2915 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_within_set_operation_no_parentheses: /* select_within_set_operation_no_parentheses  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2921 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_with_paren: /* select_with_paren  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2927 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_no_paren: /* select_no_paren  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2933 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_operator: /* set_operator  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2939 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_type: /* set_type  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 2945 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_all: /* opt_all  */
#line 227 "bison_parser.y"
            { }
#line 2951 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_hierarchical_clause: /* opt_hierarchical_clause  */
#line 232 "bison_parser.y"
            {
  delete ((*yyvaluep).hierarchical_t).startWith;
  delete ((*yyvaluep).hierarchical_t).connectBy;
}
#line 2960 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_clause: /* select_clause  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 2966 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_distinct: /* opt_distinct  */
#line 227 "bison_parser.y"
            { }
#line 2972 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_list: /* select_list  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2985 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_from_clause: /* opt_from_clause  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2991 "bison_parser.cpp"
        break;

    case YYSYMBOL_from_clause: /* from_clause  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 2997 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_where: /* opt_where  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3003 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_group: /* opt_group  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).group_t)); }
#line 3009 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_having: /* opt_having  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3015 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_within_group: /* opt_within_group  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 3028 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order: /* opt_order  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 3041 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_list: /* order_list  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 3054 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_desc: /* order_desc  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).order)); }
#line 3060 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order_type: /* opt_order_type  */
#line 227 "bison_parser.y"
            { }
#line 3066 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_null_ordering: /* opt_null_ordering  */
#line 227 "bison_parser.y"
            { }
#line 3072 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_top: /* opt_top  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 3078 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_limit: /* opt_limit  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 3084 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_list: /* expr_list  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3097 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_extended_literal_list: /* opt_extended_literal_list  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3110 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal_list: /* extended_literal_list  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3123 "bison_parser.cpp"
        break;

    case YYSYMBOL_casted_extended_literal: /* casted_extended_literal  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3129 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal: /* extended_literal  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3135 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_alias: /* expr_alias  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3141 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr: /* expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3147 "bison_parser.cpp"
        break;

    case YYSYMBOL_operand: /* operand  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3153 "bison_parser.cpp"
        break;

    case YYSYMBOL_scalar_expr: /* scalar_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3159 "bison_parser.cpp"
        break;

    case YYSYMBOL_unary_expr: /* unary_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3165 "bison_parser.cpp"
        break;

    case YYSYMBOL_binary_expr: /* binary_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3171 "bison_parser.cpp"
        break;

    case YYSYMBOL_logic_expr: /* logic_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3177 "bison_parser.cpp"
        break;

    case YYSYMBOL_in_expr: /* in_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3183 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_expr_list: /* row_expr_list  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3196 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_expr: /* case_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3202 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_list: /* case_list  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3208 "bison_parser.cpp"
        break;

    case YYSYMBOL_exists_expr: /* exists_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3214 "bison_parser.cpp"
        break;

    case YYSYMBOL_comp_expr: /* comp_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3220 "bison_parser.cpp"
        break;

    case YYSYMBOL_function_expr: /* function_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3226 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_function_expr: /* table_function_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3232 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_window: /* opt_window  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).window_description)); }
#line 3238 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_partition: /* opt_partition  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3251 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_frame_clause: /* opt_frame_clause  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).frame_description)); }
#line 3257 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_type: /* frame_type  */
#line 227 "bison_parser.y"
            { }
#line 3263 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_bound: /* frame_bound  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).frame_bound)); }
#line 3269 "bison_parser.cpp"
        break;

    case YYSYMBOL_extract_expr: /* extract_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3275 "bison_parser.cpp"
        break;

    case YYSYMBOL_next_value_expr: /* next_value_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3281 "bison_parser.cpp"
        break;

    case YYSYMBOL_cast_expr: /* cast_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3287 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field: /* datetime_field  */
#line 227 "bison_parser.y"
            { }
#line 3293 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field_plural: /* datetime_field_plural  */
#line 227 "bison_parser.y"
            { }
#line 3299 "bison_parser.cpp"
        break;

    case YYSYMBOL_duration_field: /* duration_field  */
#line 227 "bison_parser.y"
            { }
#line 3305 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_expr: /* array_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3311 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_index: /* array_index  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3317 "bison_parser.cpp"
        break;

    case YYSYMBOL_between_expr: /* between_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3323 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_name: /* column_name  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3329 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonreserved_keyword: /* nonreserved_keyword  */
#line 244 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3335 "bison_parser.cpp"
        break;

    case YYSYMBOL_name_or_keyword: /* name_or_keyword  */
#line 244 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3341 "bison_parser.cpp"
        break;

    case YYSYMBOL_literal: /* literal  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3347 "bison_parser.cpp"
        break;

    case YYSYMBOL_string_literal: /* string_literal  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3353 "bison_parser.cpp"
        break;

    case YYSYMBOL_bool_literal: /* bool_literal  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3359 "bison_parser.cpp"
        break;

    case YYSYMBOL_num_literal: /* num_literal  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3365 "bison_parser.cpp"
        break;

    case YYSYMBOL_int_literal: /* int_literal  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3371 "bison_parser.cpp"
        break;

    case YYSYMBOL_null_literal: /* null_literal  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3377 "bison_parser.cpp"
        break;

    case YYSYMBOL_date_literal: /* date_literal  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3383 "bison_parser.cpp"
        break;

    case YYSYMBOL_interval_literal: /* interval_literal  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3389 "bison_parser.cpp"
        break;

    case YYSYMBOL_param_expr: /* param_expr  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3395 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref: /* table_ref  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3401 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_atomic: /* table_ref_atomic  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3407 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonjoin_table_ref_atomic: /* nonjoin_table_ref_atomic  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3413 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_value_row_list: /* table_value_row_list  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3426 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_commalist: /* table_ref_commalist  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).table_vec)) {
    for (auto ptr : *(((*yyvaluep).table_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_vec));
}
#line 3439 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name: /* table_ref_name  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3445 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name_no_alias: /* table_ref_name_no_alias  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3451 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_name: /* table_name  */
#line 228 "bison_parser.y"
            {
  free(((*yyvaluep).table_name).name);
  free(((*yyvaluep).table_name).schema);
}
#line 3460 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_index_name: /* opt_index_name  */
#line 244 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3466 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_alias: /* table_alias  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3472 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_table_alias: /* opt_table_alias  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3478 "bison_parser.cpp"
        break;

    case YYSYMBOL_alias: /* alias  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3484 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_alias: /* opt_alias  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3490 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause: /* opt_locking_clause  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3496 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause_list: /* opt_locking_clause_list  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).locking_clause_vec)); }
#line 3502 "bison_parser.cpp"
        break;

    case YYSYMBOL_locking_clause: /* locking_clause  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).locking_t)); }
#line 3508 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_lock_mode: /* row_lock_mode  */
#line 227 "bison_parser.y"
            { }
#line 3514 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_row_lock_policy: /* opt_row_lock_policy  */
#line 227 "bison_parser.y"
            { }
#line 3520 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_with_clause: /* opt_with_clause  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3533 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_clause: /* with_clause  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3546 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description_list: /* with_description_list  */
#line 245 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3559 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description: /* with_description  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).with_description_t)); }
#line 3565 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_clause: /* join_clause  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3571 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_join_type: /* opt_join_type  */
#line 227 "bison_parser.y"
            { }
#line 3577 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_condition: /* join_condition  */
#line 274 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3583 "bison_parser.cpp"
        break;

    case YYSYMBOL_ident_commalist: /* ident_commalist  */
#line 236 "bison_parser.y"
            {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 3596 "bison_parser.cpp"
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
#line 126 "bison_parser.y"
{
  // Initialize
  yylloc.first_column = 0;
  yylloc.last_column = 0;
  yylloc.first_line = 0;
  yylloc.last_line = 0;
  yylloc.total_column = 0;
  yylloc.string_length = 0;
}

#line 3704 "bison_parser.cpp"

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
#line 420 "bison_parser.y"
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
#line 3934 "bison_parser.cpp"
    break;

  case 3: /* statement_list: statement  */
#line 440 "bison_parser.y"
                           {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyval.stmt_vec) = new std::vector<SQLStatement*>();
  (yyval.stmt_vec)->push_back((yyvsp[0].statement));
}
#line 3945 "bison_parser.cpp"
    break;

  case 4: /* statement_list: statement_list ';' statement  */
#line 446 "bison_parser.y"
                               {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyvsp[-2].stmt_vec)->push_back((yyvsp[0].statement));
  (yyval.stmt_vec) = (yyvsp[-2].stmt_vec);
}
#line 3956 "bison_parser.cpp"
    break;

  case 5: /* statement: prepare_statement opt_hints  */
#line 453 "bison_parser.y"
                                        {
  (yyval.statement) = (yyvsp[-1].prep_stmt);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3965 "bison_parser.cpp"
    break;

  case 6: /* statement: preparable_statement opt_hints  */
#line 457 "bison_parser.y"
                                 {
  (yyval.statement) = (yyvsp[-1].statement);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3974 "bison_parser.cpp"
    break;

  case 7: /* statement: show_statement  */
#line 461 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].show_stmt); }
#line 3980 "bison_parser.cpp"
    break;

  case 8: /* statement: import_statement  */
#line 462 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].import_stmt); }
#line 3986 "bison_parser.cpp"
    break;

  case 9: /* statement: export_statement  */
#line 463 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].export_stmt); }
#line 3992 "bison_parser.cpp"
    break;

  case 10: /* preparable_statement: select_statement  */
#line 465 "bison_parser.y"
                                        { (yyval.statement) = (yyvsp[0].select_stmt); }
#line 3998 "bison_parser.cpp"
    break;

  case 11: /* preparable_statement: create_statement  */
#line 466 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].create_stmt); }
#line 4004 "bison_parser.cpp"
    break;

  case 12: /* preparable_statement: insert_statement  */
#line 467 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].insert_stmt); }
#line 4010 "bison_parser.cpp"
    break;

  case 13: /* preparable_statement: delete_statement  */
#line 468 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 4016 "bison_parser.cpp"
    break;

  case 14: /* preparable_statement: truncate_statement  */
#line 469 "bison_parser.y"
                     { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 4022 "bison_parser.cpp"
    break;

  case 15: /* preparable_statement: update_statement  */
#line 470 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].update_stmt); }
#line 4028 "bison_parser.cpp"
    break;

  case 16: /* preparable_statement: drop_statement  */
#line 471 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].drop_stmt); }
#line 4034 "bison_parser.cpp"
    break;

  case 17: /* preparable_statement: alter_statement  */
#line 472 "bison_parser.y"
                  { (yyval.statement) = (yyvsp[0].alter_stmt); }
#line 4040 "bison_parser.cpp"
    break;

  case 18: /* preparable_statement: execute_statement  */
#line 473 "bison_parser.y"
                    { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 4046 "bison_parser.cpp"
    break;

  case 19: /* preparable_statement: odbc_call_statement  */
#line 474 "bison_parser.y"
                      { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 4052 "bison_parser.cpp"
    break;

  case 20: /* preparable_statement: transaction_statement  */
#line 475 "bison_parser.y"
                        { (yyval.statement) = (yyvsp[0].transaction_stmt); }
#line 4058 "bison_parser.cpp"
    break;

  case 21: /* opt_hints: WITH HINT '(' hint_list ')'  */
#line 481 "bison_parser.y"
                                        { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 4064 "bison_parser.cpp"
    break;

  case 22: /* opt_hints: %empty  */
#line 482 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 4070 "bison_parser.cpp"
    break;

  case 23: /* hint_list: hint  */
#line 484 "bison_parser.y"
                 {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 4079 "bison_parser.cpp"
    break;

  case 24: /* hint_list: hint_list ',' hint  */
#line 488 "bison_parser.y"
                     {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 4088 "bison_parser.cpp"
    break;

  case 25: /* hint: IDENTIFIER  */
#line 493 "bison_parser.y"
                  {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[0].sval);
}
#line 4097 "bison_parser.cpp"
    break;

  case 26: /* hint: IDENTIFIER '(' extended_literal_list ')'  */
#line 497 "bison_parser.y"
                                           {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[-3].sval);
  (yyval.expr)->exprList = (yyvsp[-1].expr_vec);
}
#line 4107 "bison_parser.cpp"
    break;

  case 27: /* transaction_statement: BEGIN opt_transaction_keyword  */
#line 507 "bison_parser.y"
                                                      { (yyval.transaction_stmt) = new TransactionStatement(kBeginTransaction); }
#line 4113 "bison_parser.cpp"
    break;

  case 28: /* transaction_statement: ROLLBACK opt_transaction_keyword  */
#line 508 "bison_parser.y"
                                   { (yyval.transaction_stmt) = new TransactionStatement(kRollbackTransaction); }
#line 4119 "bison_parser.cpp"
    break;

  case 29: /* transaction_statement: COMMIT opt_transaction_keyword  */
#line 509 "bison_parser.y"
                                 { (yyval.transaction_stmt) = new TransactionStatement(kCommitTransaction); }
#line 4125 "bison_parser.cpp"
    break;

  case 32: /* prepare_statement: PREPARE IDENTIFIER FROM prepare_target_query  */
#line 517 "bison_parser.y"
                                                                 {
  (yyval.prep_stmt) = new PrepareStatement();
  (yyval.prep_stmt)->name = (yyvsp[-2].sval);
  (yyval.prep_stmt)->query = (yyvsp[0].sval);
}
#line 4135 "bison_parser.cpp"
    break;

  case 34: /* odbc_call_statement: '{' opt_odbc_return CALL table_name opt_call_args '}'  */
#line 529 "bison_parser.y"
                                                                            {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->returnValue = (yyvsp[-4].expr);
  (yyval.exec_stmt)->name = qualifiedName((yyvsp[-2].table_name));
  (yyval.exec_stmt)->parameters = (yyvsp[-1].expr_vec);
}
#line 4146 "bison_parser.cpp"
    break;

  case 35: /* opt_odbc_return: param_expr '='  */
#line 538 "bison_parser.y"
                                 { (yyval.expr) = (yyvsp[-1].expr); }
#line 4152 "bison_parser.cpp"
    break;

  case 36: /* opt_odbc_return: %empty  */
#line 539 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4158 "bison_parser.cpp"
    break;

  case 37: /* opt_call_args: '(' opt_extended_literal_list ')'  */
#line 541 "bison_parser.y"
                                                  { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 4164 "bison_parser.cpp"
    break;

  case 38: /* opt_call_args: %empty  */
#line 542 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 4170 "bison_parser.cpp"
    break;

  case 39: /* execute_statement: EXECUTE IDENTIFIER opt_call_args  */
#line 544 "bison_parser.y"
                                                     {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[-1].sval);
  (yyval.exec_stmt)->parameters = (yyvsp[0].expr_vec);
}
#line 4180 "bison_parser.cpp"
    break;

  case 40: /* import_statement: IMPORT FROM file_type FILE file_path INTO table_name  */
#line 556 "bison_parser.y"
                                                                        {
  (yyval.import_stmt) = new ImportStatement((yyvsp[-4].import_type_t));
  (yyval.import_stmt)->filePath = (yyvsp[-2].sval);
  (yyval.import_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.import_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 4191 "bison_parser.cpp"
    break;

  case 41: /* import_statement: COPY table_name FROM file_path opt_import_export_options opt_where  */
#line 562 "bison_parser.y"
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
#line 4212 "bison_parser.cpp"
    break;

  case 42: /* file_type: IDENTIFIER  */
#line 579 "bison_parser.y"
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
#line 4231 "bison_parser.cpp"
    break;

  case 43: /* file_path: STRING  */
#line 594 "bison_parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 4237 "bison_parser.cpp"
    break;

  case 44: /* opt_import_export_options: WITH '(' import_export_options ')'  */
#line 596 "bison_parser.y"
                                                               { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 4243 "bison_parser.cpp"
    break;

  case 45: /* opt_import_export_options: '(' import_export_options ')'  */
#line 597 "bison_parser.y"
                                { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 4249 "bison_parser.cpp"
    break;

  case 46: /* opt_import_export_options: %empty  */
#line 598 "bison_parser.y"
              { (yyval.import_export_option_t) = new ImportExportOptions{}; }
#line 4255 "bison_parser.cpp"
    break;

  case 47: /* import_export_options: import_export_options ',' FORMAT file_type  */
#line 600 "bison_parser.y"
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
#line 4274 "bison_parser.cpp"
    break;

  case 48: /* import_export_options: FORMAT file_type  */
#line 614 "bison_parser.y"
                   {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->format = (yyvsp[0].import_type_t);
}
#line 4283 "bison_parser.cpp"
    break;

  case 49: /* import_export_options: import_export_options ',' ENCODING STRING  */
#line 618 "bison_parser.y"
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
#line 4298 "bison_parser.cpp"
    break;

  case 50: /* import_export_options: ENCODING STRING  */
#line 628 "bison_parser.y"
                  {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->encoding = (yyvsp[0].sval);
}
#line 4307 "bison_parser.cpp"
    break;

  case 51: /* import_export_options: import_export_options ',' csv_option  */
#line 632 "bison_parser.y"
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
#line 4336 "bison_parser.cpp"
    break;

  case 52: /* import_export_options: csv_option  */
#line 656 "bison_parser.y"
             {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->csv_options = new CsvOptions{};
  (yyval.import_export_option_t)->csv_options->accept_csv_option((yyvsp[0].csv_option_t));

  delete (yyvsp[0].csv_option_t);
}
#line 4348 "bison_parser.cpp"
    break;

  case 53: /* csv_option: IDENTIFIER STRING  */
#line 664 "bison_parser.y"
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
#line 4366 "bison_parser.cpp"
    break;

  case 54: /* csv_option: NULL STRING  */
#line 677 "bison_parser.y"
              { (yyval.csv_option_t) = new std::pair<CsvOptionType, char*>(CsvOptionType::Null, (yyvsp[0].sval)); }
#line 4372 "bison_parser.cpp"
    break;

  case 55: /* export_statement: COPY table_name TO file_path opt_import_export_options  */
#line 684 "bison_parser.y"
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
#line 4392 "bison_parser.cpp"
    break;

  case 56: /* export_statement: COPY select_with_paren TO file_path opt_import_export_options  */
#line 699 "bison_parser.y"
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
#line 4411 "bison_parser.cpp"
    break;

  case 57: /* show_statement: SHOW TABLES  */
#line 719 "bison_parser.y"
                             { (yyval.show_stmt) = new ShowStatement(kShowTables); }
#line 4417 "bison_parser.cpp"
    break;

  case 58: /* show_statement: SHOW COLUMNS table_name  */
#line 720 "bison_parser.y"
                          {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4427 "bison_parser.cpp"
    break;

  case 59: /* show_statement: DESCRIBE table_name  */
#line 725 "bison_parser.y"
                      {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4437 "bison_parser.cpp"
    break;

  case 60: /* create_statement: CREATE TABLE opt_not_exists table_name FROM IDENTIFIER FILE file_path  */
#line 736 "bison_parser.y"
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
#line 4455 "bison_parser.cpp"
    break;

  case 61: /* create_statement: CREATE TABLE opt_not_exists table_name '(' table_elem_commalist ')'  */
#line 749 "bison_parser.y"
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
#line 4472 "bison_parser.cpp"
    break;

  case 62: /* create_statement: CREATE TABLE opt_not_exists table_name AS select_statement  */
#line 761 "bison_parser.y"
                                                             {
  (yyval.create_stmt) = new CreateStatement(kCreateTable);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-3].bval);
  (yyval.create_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4484 "bison_parser.cpp"
    break;

  case 63: /* create_statement: CREATE INDEX opt_not_exists opt_index_name ON table_name '(' ident_commalist ')'  */
#line 768 "bison_parser.y"
                                                                                   {
  (yyval.create_stmt) = new CreateStatement(kCreateIndex);
  (yyval.create_stmt)->indexName = (yyvsp[-5].sval);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-6].bval);
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->indexColumns = (yyvsp[-1].str_vec);
}
#line 4496 "bison_parser.cpp"
    break;

  case 64: /* create_statement: CREATE VIEW opt_not_exists table_name opt_column_list AS select_statement  */
#line 775 "bison_parser.y"
                                                                            {
  (yyval.create_stmt) = new CreateStatement(kCreateView);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-4].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->viewColumns = (yyvsp[-2].str_vec);
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4509 "bison_parser.cpp"
    break;

  case 65: /* opt_not_exists: IF NOT EXISTS  */
#line 784 "bison_parser.y"
                               { (yyval.bval) = true; }
#line 4515 "bison_parser.cpp"
    break;

  case 66: /* opt_not_exists: %empty  */
#line 785 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4521 "bison_parser.cpp"
    break;

  case 67: /* table_elem_commalist: table_elem  */
#line 787 "bison_parser.y"
                                  {
  (yyval.table_element_vec) = new std::vector<TableElement*>();
  (yyval.table_element_vec)->push_back((yyvsp[0].table_element_t));
}
#line 4530 "bison_parser.cpp"
    break;

  case 68: /* table_elem_commalist: table_elem_commalist ',' table_elem  */
#line 791 "bison_parser.y"
                                      {
  (yyvsp[-2].table_element_vec)->push_back((yyvsp[0].table_element_t));
  (yyval.table_element_vec) = (yyvsp[-2].table_element_vec);
}
#line 4539 "bison_parser.cpp"
    break;

  case 69: /* table_elem: column_def  */
#line 796 "bison_parser.y"
                        { (yyval.table_element_t) = (yyvsp[0].column_t); }
#line 4545 "bison_parser.cpp"
    break;

  case 70: /* table_elem: table_constraint  */
#line 797 "bison_parser.y"
                   { (yyval.table_element_t) = (yyvsp[0].table_constraint_t); }
#line 4551 "bison_parser.cpp"
    break;

  case 71: /* column_def: name_or_keyword column_type opt_column_constraints  */
#line 799 "bison_parser.y"
                                                                {
  (yyval.column_t) = new ColumnDefinition((yyvsp[-2].sval), (yyvsp[-1].column_type_t), (yyvsp[0].column_constraints_t)->constraints, (yyvsp[0].column_constraints_t)->references);
  if (!(yyval.column_t)->trySetNullableExplicit()) {
    yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{(yyvsp[-2].sval)}).c_str());
  }
  delete (yyvsp[0].column_constraints_t);
}
#line 4563 "bison_parser.cpp"
    break;

  case 72: /* column_type: BIGINT  */
#line 807 "bison_parser.y"
                     { (yyval.column_type_t) = ColumnType{DataType::BIGINT}; }
#line 4569 "bison_parser.cpp"
    break;

  case 73: /* column_type: BOOLEAN  */
#line 808 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::BOOLEAN}; }
#line 4575 "bison_parser.cpp"
    break;

  case 74: /* column_type: CHAR '(' INTVAL ')'  */
#line 809 "bison_parser.y"
                      { (yyval.column_type_t) = ColumnType{DataType::CHAR, (yyvsp[-1].ival)}; }
#line 4581 "bison_parser.cpp"
    break;

  case 75: /* column_type: CHARACTER_VARYING '(' INTVAL ')'  */
#line 810 "bison_parser.y"
                                   { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4587 "bison_parser.cpp"
    break;

  case 76: /* column_type: DATE  */
#line 811 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::DATE}; }
#line 4593 "bison_parser.cpp"
    break;

  case 77: /* column_type: DATETIME  */
#line 812 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4599 "bison_parser.cpp"
    break;

  case 78: /* column_type: DECIMAL opt_decimal_specification  */
#line 813 "bison_parser.y"
                                    {
  (yyval.column_type_t) = ColumnType{DataType::DECIMAL, 0, (yyvsp[0].ival_pair)->first, (yyvsp[0].ival_pair)->second};
  delete (yyvsp[0].ival_pair);
}
#line 4608 "bison_parser.cpp"
    break;

  case 79: /* column_type: DOUBLE  */
#line 817 "bison_parser.y"
         { (yyval.column_type_t) = ColumnType{DataType::DOUBLE}; }
#line 4614 "bison_parser.cpp"
    break;

  case 80: /* column_type: FLOAT  */
#line 818 "bison_parser.y"
        { (yyval.column_type_t) = ColumnType{DataType::FLOAT}; }
#line 4620 "bison_parser.cpp"
    break;

  case 81: /* column_type: INT  */
#line 819 "bison_parser.y"
      { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4626 "bison_parser.cpp"
    break;

  case 82: /* column_type: INTEGER  */
#line 820 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4632 "bison_parser.cpp"
    break;

  case 83: /* column_type: LONG  */
#line 821 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::LONG}; }
#line 4638 "bison_parser.cpp"
    break;

  case 84: /* column_type: REAL  */
#line 822 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::REAL}; }
#line 4644 "bison_parser.cpp"
    break;

  case 85: /* column_type: SMALLINT  */
#line 823 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::SMALLINT}; }
#line 4650 "bison_parser.cpp"
    break;

  case 86: /* column_type: TEXT  */
#line 824 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::TEXT}; }
#line 4656 "bison_parser.cpp"
    break;

  case 87: /* column_type: TIME opt_time_precision  */
#line 825 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::TIME, 0, (yyvsp[0].ival)}; }
#line 4662 "bison_parser.cpp"
    break;

  case 88: /* column_type: TIMESTAMP  */
#line 826 "bison_parser.y"
            { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4668 "bison_parser.cpp"
    break;

  case 89: /* column_type: VARCHAR '(' INTVAL ')'  */
#line 827 "bison_parser.y"
                         { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4674 "bison_parser.cpp"
    break;

  case 90: /* opt_time_precision: '(' INTVAL ')'  */
#line 829 "bison_parser.y"
                                    { (yyval.ival) = (yyvsp[-1].ival); }
#line 4680 "bison_parser.cpp"
    break;

  case 91: /* opt_time_precision: %empty  */
#line 830 "bison_parser.y"
              { (yyval.ival) = 0; }
#line 4686 "bison_parser.cpp"
    break;

  case 92: /* opt_decimal_specification: '(' INTVAL ',' INTVAL ')'  */
#line 832 "bison_parser.y"
                                                      { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-3].ival), (yyvsp[-1].ival)}; }
#line 4692 "bison_parser.cpp"
    break;

  case 93: /* opt_decimal_specification: '(' INTVAL ')'  */
#line 833 "bison_parser.y"
                 { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-1].ival), 0}; }
#line 4698 "bison_parser.cpp"
    break;

  case 94: /* opt_decimal_specification: %empty  */
#line 834 "bison_parser.y"
              { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{0, 0}; }
#line 4704 "bison_parser.cpp"
    break;

  case 95: /* opt_column_constraints: column_constraints  */
#line 836 "bison_parser.y"
                                            { (yyval.column_constraints_t) = (yyvsp[0].column_constraints_t); }
#line 4710 "bison_parser.cpp"
    break;

  case 96: /* opt_column_constraints: %empty  */
#line 837 "bison_parser.y"
              { (yyval.column_constraints_t) = new ColumnConstraints(); }
#line 4716 "bison_parser.cpp"
    break;

  case 97: /* column_constraints: column_constraint  */
#line 839 "bison_parser.y"
                                       {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
}
#line 4725 "bison_parser.cpp"
    break;

  case 98: /* column_constraints: column_constraints column_constraint  */
#line 843 "bison_parser.y"
                                       {
  (yyvsp[-1].column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4734 "bison_parser.cpp"
    break;

  case 99: /* column_constraints: references_spec  */
#line 847 "bison_parser.y"
                  {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyval.column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
}
#line 4744 "bison_parser.cpp"
    break;

  case 100: /* column_constraints: column_constraints references_spec  */
#line 852 "bison_parser.y"
                                     {
  // Multiple foreign keys for the same column could be possible, so we do not raise an error in that case.
  // Think of foreign keys referenced on multiple levels (returned item references sold item references items).
  (yyvsp[-1].column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyvsp[-1].column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4756 "bison_parser.cpp"
    break;

  case 101: /* column_constraint: PRIMARY KEY  */
#line 860 "bison_parser.y"
                                { (yyval.column_constraint_t) = ConstraintType::PrimaryKey; }
#line 4762 "bison_parser.cpp"
    break;

  case 102: /* column_constraint: UNIQUE  */
#line 861 "bison_parser.y"
         { (yyval.column_constraint_t) = ConstraintType::Unique; }
#line 4768 "bison_parser.cpp"
    break;

  case 103: /* column_constraint: NULL  */
#line 862 "bison_parser.y"
       { (yyval.column_constraint_t) = ConstraintType::Null; }
#line 4774 "bison_parser.cpp"
    break;

  case 104: /* column_constraint: NOT NULL  */
#line 863 "bison_parser.y"
           { (yyval.column_constraint_t) = ConstraintType::NotNull; }
#line 4780 "bison_parser.cpp"
    break;

  case 105: /* table_constraint: PRIMARY KEY '(' ident_commalist ')'  */
#line 865 "bison_parser.y"
                                                       { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::PrimaryKey, (yyvsp[-1].str_vec)); }
#line 4786 "bison_parser.cpp"
    break;

  case 106: /* table_constraint: UNIQUE '(' ident_commalist ')'  */
#line 866 "bison_parser.y"
                                 { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::Unique, (yyvsp[-1].str_vec)); }
#line 4792 "bison_parser.cpp"
    break;

  case 107: /* table_constraint: FOREIGN KEY '(' ident_commalist ')' references_spec  */
#line 867 "bison_parser.y"
                                                      { (yyval.table_constraint_t) = new ForeignKeyConstraint((yyvsp[-2].str_vec), (yyvsp[0].references_spec_t)); }
#line 4798 "bison_parser.cpp"
    break;

  case 108: /* references_spec: REFERENCES table_name opt_column_list  */
#line 869 "bison_parser.y"
                                                        { (yyval.references_spec_t) = new ReferencesSpecification((yyvsp[-1].table_name).schema, (yyvsp[-1].table_name).name, (yyvsp[0].str_vec)); }
#line 4804 "bison_parser.cpp"
    break;

  case 109: /* drop_statement: DROP TABLE opt_exists table_name  */
#line 877 "bison_parser.y"
                                                  {
  (yyval.drop_stmt) = new DropStatement(kDropTable);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4815 "bison_parser.cpp"
    break;

  case 110: /* drop_statement: DROP VIEW opt_exists table_name  */
#line 883 "bison_parser.y"
                                  {
  (yyval.drop_stmt) = new DropStatement(kDropView);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4826 "bison_parser.cpp"
    break;

  case 111: /* drop_statement: DEALLOCATE PREPARE IDENTIFIER  */
#line 889 "bison_parser.y"
                                {
  (yyval.drop_stmt) = new DropStatement(kDropPreparedStatement);
  (yyval.drop_stmt)->ifExists = false;
  (yyval.drop_stmt)->name = (yyvsp[0].sval);
}
#line 4836 "bison_parser.cpp"
    break;

  case 112: /* drop_statement: DROP INDEX opt_exists name_or_keyword  */
#line 895 "bison_parser.y"
                                        {
  (yyval.drop_stmt) = new DropStatement(kDropIndex);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->indexName = (yyvsp[0].sval);
}
#line 4846 "bison_parser.cpp"
    break;

  case 113: /* opt_exists: IF EXISTS  */
#line 901 "bison_parser.y"
                       { (yyval.bval) = true; }
#line 4852 "bison_parser.cpp"
    break;

  case 114: /* opt_exists: %empty  */
#line 902 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4858 "bison_parser.cpp"
    break;

  case 115: /* alter_statement: ALTER TABLE opt_exists table_name alter_action  */
#line 909 "bison_parser.y"
                                                                 {
  (yyval.alter_stmt) = new AlterStatement((yyvsp[-1].table_name).name, (yyvsp[0].alter_action_t));
  (yyval.alter_stmt)->ifTableExists = (yyvsp[-2].bval);
  (yyval.alter_stmt)->schema = (yyvsp[-1].table_name).schema;
}
#line 4868 "bison_parser.cpp"
    break;

  case 116: /* alter_action: drop_action  */
#line 915 "bison_parser.y"
                           { (yyval.alter_action_t) = (yyvsp[0].drop_action_t); }
#line 4874 "bison_parser.cpp"
    break;

  case 117: /* drop_action: DROP COLUMN opt_exists name_or_keyword  */
#line 917 "bison_parser.y"
                                                     {
  (yyval.drop_action_t) = new DropColumnAction((yyvsp[0].sval));
  (yyval.drop_action_t)->ifExists = (yyvsp[-1].bval);
}
#line 4883 "bison_parser.cpp"
    break;

  case 118: /* delete_statement: DELETE FROM table_name opt_where  */
#line 927 "bison_parser.y"
                                                    {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[-1].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[-1].table_name).name;
  (yyval.delete_stmt)->expr = (yyvsp[0].expr);
}
#line 4894 "bison_parser.cpp"
    break;

  case 119: /* truncate_statement: TRUNCATE table_name  */
#line 934 "bison_parser.y"
                                         {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 4904 "bison_parser.cpp"
    break;

  case 120: /* insert_statement: INSERT INTO table_name opt_column_list VALUES '(' extended_literal_list ')'  */
#line 945 "bison_parser.y"
                                                                                               {
  (yyval.insert_stmt) = new InsertStatement(kInsertValues);
  (yyval.insert_stmt)->schema = (yyvsp[-5].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-5].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-4].str_vec);
  (yyval.insert_stmt)->values = (yyvsp[-1].expr_vec);
}
#line 4916 "bison_parser.cpp"
    break;

  case 121: /* insert_statement: INSERT INTO table_name opt_column_list select_no_paren  */
#line 952 "bison_parser.y"
                                                         {
  (yyval.insert_stmt) = new InsertStatement(kInsertSelect);
  (yyval.insert_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-1].str_vec);
  (yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4928 "bison_parser.cpp"
    break;

  case 122: /* opt_column_list: '(' ident_commalist ')'  */
#line 960 "bison_parser.y"
                                          { (yyval.str_vec) = (yyvsp[-1].str_vec); }
#line 4934 "bison_parser.cpp"
    break;

  case 123: /* opt_column_list: %empty  */
#line 961 "bison_parser.y"
              { (yyval.str_vec) = nullptr; }
#line 4940 "bison_parser.cpp"
    break;

  case 124: /* update_statement: UPDATE table_ref_name_no_alias SET update_clause_commalist opt_where  */
#line 968 "bison_parser.y"
                                                                                        {
  (yyval.update_stmt) = new UpdateStatement();
  (yyval.update_stmt)->table = (yyvsp[-3].table);
  (yyval.update_stmt)->updates = (yyvsp[-1].update_vec);
  (yyval.update_stmt)->where = (yyvsp[0].expr);
}
#line 4951 "bison_parser.cpp"
    break;

  case 125: /* update_clause_commalist: update_clause  */
#line 975 "bison_parser.y"
                                        {
  (yyval.update_vec) = new std::vector<UpdateClause*>();
  (yyval.update_vec)->push_back((yyvsp[0].update_t));
}
#line 4960 "bison_parser.cpp"
    break;

  case 126: /* update_clause_commalist: update_clause_commalist ',' update_clause  */
#line 979 "bison_parser.y"
                                            {
  (yyvsp[-2].update_vec)->push_back((yyvsp[0].update_t));
  (yyval.update_vec) = (yyvsp[-2].update_vec);
}
#line 4969 "bison_parser.cpp"
    break;

  case 127: /* update_clause: name_or_keyword '=' expr  */
#line 984 "bison_parser.y"
                                         {
  (yyval.update_t) = new UpdateClause();
  (yyval.update_t)->column = (yyvsp[-2].sval);
  (yyval.update_t)->value = (yyvsp[0].expr);
}
#line 4979 "bison_parser.cpp"
    break;

  case 128: /* select_statement: opt_with_clause select_with_paren  */
#line 994 "bison_parser.y"
                                                     {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4988 "bison_parser.cpp"
    break;

  case 129: /* select_statement: opt_with_clause select_no_paren  */
#line 998 "bison_parser.y"
                                  {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4997 "bison_parser.cpp"
    break;

  case 130: /* select_statement: opt_with_clause select_with_paren set_operator select_within_set_operation opt_order opt_limit  */
#line 1002 "bison_parser.y"
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
#line 5013 "bison_parser.cpp"
    break;

  case 133: /* select_within_set_operation_no_parentheses: select_clause  */
#line 1016 "bison_parser.y"
                                                           { (yyval.select_stmt) = (yyvsp[0].select_stmt); }
#line 5019 "bison_parser.cpp"
    break;

  case 134: /* select_within_set_operation_no_parentheses: select_clause set_operator select_within_set_operation  */
#line 1017 "bison_parser.y"
                                                         {
  (yyval.select_stmt) = (yyvsp[-2].select_stmt);
  if ((yyval.select_stmt)->setOperations == nullptr) {
    (yyval.select_stmt)->setOperations = new std::vector<SetOperation*>();
  }
  (yyval.select_stmt)->setOperations->push_back((yyvsp[-1].set_operator_t));
  (yyval.select_stmt)->setOperations->back()->nestedSelectStatement = (yyvsp[0].select_stmt);
}
#line 5032 "bison_parser.cpp"
    break;

  case 135: /* select_with_paren: '(' select_no_paren ')'  */
#line 1026 "bison_parser.y"
                                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 5038 "bison_parser.cpp"
    break;

  case 136: /* select_with_paren: '(' select_with_paren ')'  */
#line 1027 "bison_parser.y"
                            { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 5044 "bison_parser.cpp"
    break;

  case 137: /* select_no_paren: select_clause opt_order opt_limit opt_locking_clause  */
#line 1029 "bison_parser.y"
                                                                       {
  (yyval.select_stmt) = (yyvsp[-3].select_stmt);
  (yyval.select_stmt)->order = (yyvsp[-2].order_vec);

  // Limit could have been set by TOP. A parenthesized TOP may hold a
  // placeholder, which is also referenced from the parameter list the input
  // rule walks; deleting it here would leave that reference dangling. TOP with
  // a literal has no such reference, so "top 20 ... limit 10" keeps working.
  if ((yyvsp[-1].limit)) {
    if ((yyval.select_stmt)->limit && (yyval.select_stmt)->limit->limit->type != kExprLiteralInt) {
      delete (yyvsp[-3].select_stmt);
      if ((yyvsp[-2].order_vec)) {
        for (auto ptr : *(yyvsp[-2].order_vec)) delete ptr;
        delete (yyvsp[-2].order_vec);
      }
      delete (yyvsp[-1].limit);
      if ((yyvsp[0].locking_clause_vec)) {
        for (auto ptr : *(yyvsp[0].locking_clause_vec)) delete ptr;
        delete (yyvsp[0].locking_clause_vec);
      }
      yyerror(&yyloc, result, scanner, "TOP with an expression cannot be combined with LIMIT.");
      YYERROR;
    }
    delete (yyval.select_stmt)->limit;
    (yyval.select_stmt)->limit = (yyvsp[-1].limit);
  }

  if ((yyvsp[0].locking_clause_vec)) {
    (yyval.select_stmt)->lockings = (yyvsp[0].locking_clause_vec);
  }
}
#line 5080 "bison_parser.cpp"
    break;

  case 138: /* select_no_paren: select_clause set_operator select_within_set_operation opt_order opt_limit opt_locking_clause  */
#line 1060 "bison_parser.y"
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
#line 5096 "bison_parser.cpp"
    break;

  case 139: /* set_operator: set_type opt_all  */
#line 1072 "bison_parser.y"
                                {
  (yyval.set_operator_t) = (yyvsp[-1].set_operator_t);
  (yyval.set_operator_t)->isAll = (yyvsp[0].bval);
}
#line 5105 "bison_parser.cpp"
    break;

  case 140: /* set_type: UNION  */
#line 1077 "bison_parser.y"
                 {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetUnion;
}
#line 5114 "bison_parser.cpp"
    break;

  case 141: /* set_type: INTERSECT  */
#line 1081 "bison_parser.y"
            {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetIntersect;
}
#line 5123 "bison_parser.cpp"
    break;

  case 142: /* set_type: EXCEPT  */
#line 1085 "bison_parser.y"
         {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetExcept;
}
#line 5132 "bison_parser.cpp"
    break;

  case 143: /* opt_all: ALL  */
#line 1090 "bison_parser.y"
              { (yyval.bval) = true; }
#line 5138 "bison_parser.cpp"
    break;

  case 144: /* opt_all: %empty  */
#line 1091 "bison_parser.y"
              { (yyval.bval) = false; }
#line 5144 "bison_parser.cpp"
    break;

  case 145: /* opt_hierarchical_clause: START_WITH expr connect_by expr  */
#line 1095 "bison_parser.y"
                                                          { (yyval.hierarchical_t) = HierarchicalClause{(yyvsp[-2].expr), (yyvsp[0].expr)}; }
#line 5150 "bison_parser.cpp"
    break;

  case 146: /* opt_hierarchical_clause: connect_by expr START_WITH expr  */
#line 1096 "bison_parser.y"
                                  { (yyval.hierarchical_t) = HierarchicalClause{(yyvsp[0].expr), (yyvsp[-2].expr)}; }
#line 5156 "bison_parser.cpp"
    break;

  case 147: /* opt_hierarchical_clause: START_WITH expr  */
#line 1097 "bison_parser.y"
                  { (yyval.hierarchical_t) = HierarchicalClause{(yyvsp[0].expr), nullptr}; }
#line 5162 "bison_parser.cpp"
    break;

  case 148: /* opt_hierarchical_clause: connect_by expr  */
#line 1098 "bison_parser.y"
                  { (yyval.hierarchical_t) = HierarchicalClause{nullptr, (yyvsp[0].expr)}; }
#line 5168 "bison_parser.cpp"
    break;

  case 149: /* opt_hierarchical_clause: %empty  */
#line 1099 "bison_parser.y"
              { (yyval.hierarchical_t) = HierarchicalClause{nullptr, nullptr}; }
#line 5174 "bison_parser.cpp"
    break;

  case 152: /* select_clause: SELECT opt_top opt_distinct select_list opt_from_clause opt_where opt_hierarchical_clause opt_group  */
#line 1105 "bison_parser.y"
                                                                                                                    {
  (yyval.select_stmt) = new SelectStatement();
  (yyval.select_stmt)->limit = (yyvsp[-6].limit);
  (yyval.select_stmt)->selectDistinct = (yyvsp[-5].bval);
  (yyval.select_stmt)->selectList = (yyvsp[-4].expr_vec);
  (yyval.select_stmt)->fromTable = (yyvsp[-3].table);
  (yyval.select_stmt)->whereClause = (yyvsp[-2].expr);
  (yyval.select_stmt)->startWith = (yyvsp[-1].hierarchical_t).startWith;
  (yyval.select_stmt)->connectBy = (yyvsp[-1].hierarchical_t).connectBy;
  (yyval.select_stmt)->groupBy = (yyvsp[0].group_t);
}
#line 5190 "bison_parser.cpp"
    break;

  case 153: /* opt_distinct: DISTINCT  */
#line 1117 "bison_parser.y"
                        { (yyval.bval) = true; }
#line 5196 "bison_parser.cpp"
    break;

  case 154: /* opt_distinct: %empty  */
#line 1118 "bison_parser.y"
              { (yyval.bval) = false; }
#line 5202 "bison_parser.cpp"
    break;

  case 156: /* opt_from_clause: from_clause  */
#line 1122 "bison_parser.y"
                              { (yyval.table) = (yyvsp[0].table); }
#line 5208 "bison_parser.cpp"
    break;

  case 157: /* opt_from_clause: %empty  */
#line 1123 "bison_parser.y"
              { (yyval.table) = nullptr; }
#line 5214 "bison_parser.cpp"
    break;

  case 158: /* from_clause: FROM table_ref  */
#line 1125 "bison_parser.y"
                             { (yyval.table) = (yyvsp[0].table); }
#line 5220 "bison_parser.cpp"
    break;

  case 159: /* opt_where: WHERE expr  */
#line 1127 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 5226 "bison_parser.cpp"
    break;

  case 160: /* opt_where: %empty  */
#line 1128 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 5232 "bison_parser.cpp"
    break;

  case 161: /* opt_group: GROUP BY expr_list opt_having  */
#line 1130 "bison_parser.y"
                                          {
  (yyval.group_t) = new GroupByDescription();
  (yyval.group_t)->columns = (yyvsp[-1].expr_vec);
  (yyval.group_t)->having = (yyvsp[0].expr);
}
#line 5242 "bison_parser.cpp"
    break;

  case 162: /* opt_group: %empty  */
#line 1135 "bison_parser.y"
              { (yyval.group_t) = nullptr; }
#line 5248 "bison_parser.cpp"
    break;

  case 163: /* opt_having: HAVING expr  */
#line 1137 "bison_parser.y"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 5254 "bison_parser.cpp"
    break;

  case 164: /* opt_having: %empty  */
#line 1138 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 5260 "bison_parser.cpp"
    break;

  case 165: /* opt_within_group: WITHIN_GROUP '(' ORDER BY order_list ')'  */
#line 1143 "bison_parser.y"
                                                            { (yyval.order_vec) = (yyvsp[-1].order_vec); }
#line 5266 "bison_parser.cpp"
    break;

  case 166: /* opt_within_group: %empty  */
#line 1144 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 5272 "bison_parser.cpp"
    break;

  case 167: /* opt_order: ORDER BY order_list  */
#line 1146 "bison_parser.y"
                                { (yyval.order_vec) = (yyvsp[0].order_vec); }
#line 5278 "bison_parser.cpp"
    break;

  case 168: /* opt_order: %empty  */
#line 1147 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 5284 "bison_parser.cpp"
    break;

  case 169: /* order_list: order_desc  */
#line 1149 "bison_parser.y"
                        {
  (yyval.order_vec) = new std::vector<OrderDescription*>();
  (yyval.order_vec)->push_back((yyvsp[0].order));
}
#line 5293 "bison_parser.cpp"
    break;

  case 170: /* order_list: order_list ',' order_desc  */
#line 1153 "bison_parser.y"
                            {
  (yyvsp[-2].order_vec)->push_back((yyvsp[0].order));
  (yyval.order_vec) = (yyvsp[-2].order_vec);
}
#line 5302 "bison_parser.cpp"
    break;

  case 171: /* order_desc: expr opt_order_type opt_null_ordering  */
#line 1158 "bison_parser.y"
                                                   { (yyval.order) = new OrderDescription((yyvsp[-1].order_type), (yyvsp[-2].expr), (yyvsp[0].null_ordering_t)); }
#line 5308 "bison_parser.cpp"
    break;

  case 172: /* opt_order_type: ASC  */
#line 1160 "bison_parser.y"
                     { (yyval.order_type) = kOrderAsc; }
#line 5314 "bison_parser.cpp"
    break;

  case 173: /* opt_order_type: DESC  */
#line 1161 "bison_parser.y"
       { (yyval.order_type) = kOrderDesc; }
#line 5320 "bison_parser.cpp"
    break;

  case 174: /* opt_order_type: %empty  */
#line 1162 "bison_parser.y"
              { (yyval.order_type) = kOrderAsc; }
#line 5326 "bison_parser.cpp"
    break;

  case 175: /* opt_null_ordering: %empty  */
#line 1164 "bison_parser.y"
                                { (yyval.null_ordering_t) = NullOrdering::Undefined; }
#line 5332 "bison_parser.cpp"
    break;

  case 176: /* opt_null_ordering: IDENTIFIER IDENTIFIER  */
#line 1165 "bison_parser.y"
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
#line 5356 "bison_parser.cpp"
    break;

  case 177: /* opt_top: TOP int_literal  */
#line 1191 "bison_parser.y"
                          { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 5362 "bison_parser.cpp"
    break;

  case 178: /* opt_top: TOP '(' expr ')'  */
#line 1192 "bison_parser.y"
                   { (yyval.limit) = new LimitDescription((yyvsp[-1].expr), nullptr); }
#line 5368 "bison_parser.cpp"
    break;

  case 179: /* opt_top: %empty  */
#line 1193 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 5374 "bison_parser.cpp"
    break;

  case 180: /* opt_limit: LIMIT expr  */
#line 1195 "bison_parser.y"
                       { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 5380 "bison_parser.cpp"
    break;

  case 181: /* opt_limit: OFFSET expr  */
#line 1196 "bison_parser.y"
              { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 5386 "bison_parser.cpp"
    break;

  case 182: /* opt_limit: LIMIT expr OFFSET expr  */
#line 1197 "bison_parser.y"
                         { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 5392 "bison_parser.cpp"
    break;

  case 183: /* opt_limit: LIMIT ALL  */
#line 1198 "bison_parser.y"
            { (yyval.limit) = new LimitDescription(nullptr, nullptr); }
#line 5398 "bison_parser.cpp"
    break;

  case 184: /* opt_limit: LIMIT ALL OFFSET expr  */
#line 1199 "bison_parser.y"
                        { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 5404 "bison_parser.cpp"
    break;

  case 185: /* opt_limit: OFFSET expr ROWS FETCH NEXT expr ROWS ONLY  */
#line 1200 "bison_parser.y"
                                             { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[-6].expr)); }
#line 5410 "bison_parser.cpp"
    break;

  case 186: /* opt_limit: %empty  */
#line 1201 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 5416 "bison_parser.cpp"
    break;

  case 187: /* expr_list: expr_alias  */
#line 1206 "bison_parser.y"
                       {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5425 "bison_parser.cpp"
    break;

  case 188: /* expr_list: expr_list ',' expr_alias  */
#line 1210 "bison_parser.y"
                           {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5434 "bison_parser.cpp"
    break;

  case 189: /* opt_extended_literal_list: extended_literal_list  */
#line 1216 "bison_parser.y"
                                                  { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5440 "bison_parser.cpp"
    break;

  case 190: /* opt_extended_literal_list: %empty  */
#line 1217 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5446 "bison_parser.cpp"
    break;

  case 191: /* extended_literal_list: casted_extended_literal  */
#line 1219 "bison_parser.y"
                                                {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5455 "bison_parser.cpp"
    break;

  case 192: /* extended_literal_list: extended_literal_list ',' casted_extended_literal  */
#line 1223 "bison_parser.y"
                                                    {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5464 "bison_parser.cpp"
    break;

  case 194: /* casted_extended_literal: CAST '(' extended_literal AS column_type ')'  */
#line 1228 "bison_parser.y"
                                                                                          {
  (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t));
}
#line 5472 "bison_parser.cpp"
    break;

  case 195: /* extended_literal: literal  */
#line 1232 "bison_parser.y"
                           { (yyval.expr) = (yyvsp[0].expr); }
#line 5478 "bison_parser.cpp"
    break;

  case 196: /* extended_literal: '-' num_literal  */
#line 1233 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5484 "bison_parser.cpp"
    break;

  case 197: /* extended_literal: '-' interval_literal  */
#line 1234 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5490 "bison_parser.cpp"
    break;

  case 198: /* expr_alias: expr opt_alias  */
#line 1236 "bison_parser.y"
                            {
  (yyval.expr) = (yyvsp[-1].expr);
  if ((yyvsp[0].alias_t)) {
    (yyval.expr)->alias = (yyvsp[0].alias_t)->name;
    (yyvsp[0].alias_t)->name = nullptr;
    delete (yyvsp[0].alias_t);
  }
}
#line 5503 "bison_parser.cpp"
    break;

  case 204: /* operand: '(' expr ')'  */
#line 1247 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[-1].expr); }
#line 5509 "bison_parser.cpp"
    break;

  case 215: /* operand: '(' select_no_paren ')'  */
#line 1249 "bison_parser.y"
                                         {
  (yyval.expr) = Expr::makeSelect((yyvsp[-1].select_stmt));
}
#line 5517 "bison_parser.cpp"
    break;

  case 218: /* scalar_expr: column_name OUTERJOIN  */
#line 1257 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpUnary(kOpOuterJoin, (yyvsp[-1].expr)); }
#line 5523 "bison_parser.cpp"
    break;

  case 219: /* unary_expr: '-' operand  */
#line 1259 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5529 "bison_parser.cpp"
    break;

  case 220: /* unary_expr: PRIOR operand  */
#line 1261 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpUnary(kOpPrior, (yyvsp[0].expr)); }
#line 5535 "bison_parser.cpp"
    break;

  case 221: /* unary_expr: NOT operand  */
#line 1262 "bison_parser.y"
              { (yyval.expr) = Expr::makeOpUnary(kOpNot, (yyvsp[0].expr)); }
#line 5541 "bison_parser.cpp"
    break;

  case 222: /* unary_expr: operand ISNULL  */
#line 1263 "bison_parser.y"
                 { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-1].expr)); }
#line 5547 "bison_parser.cpp"
    break;

  case 223: /* unary_expr: operand IS NULL  */
#line 1264 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-2].expr)); }
#line 5553 "bison_parser.cpp"
    break;

  case 224: /* unary_expr: operand IS NOT NULL  */
#line 1265 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeOpUnary(kOpIsNull, (yyvsp[-3].expr))); }
#line 5559 "bison_parser.cpp"
    break;

  case 226: /* binary_expr: operand '-' operand  */
#line 1267 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpMinus, (yyvsp[0].expr)); }
#line 5565 "bison_parser.cpp"
    break;

  case 227: /* binary_expr: operand '+' operand  */
#line 1268 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPlus, (yyvsp[0].expr)); }
#line 5571 "bison_parser.cpp"
    break;

  case 228: /* binary_expr: operand '/' operand  */
#line 1269 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpSlash, (yyvsp[0].expr)); }
#line 5577 "bison_parser.cpp"
    break;

  case 229: /* binary_expr: operand '*' operand  */
#line 1270 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAsterisk, (yyvsp[0].expr)); }
#line 5583 "bison_parser.cpp"
    break;

  case 230: /* binary_expr: operand '%' operand  */
#line 1271 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPercentage, (yyvsp[0].expr)); }
#line 5589 "bison_parser.cpp"
    break;

  case 231: /* binary_expr: operand '^' operand  */
#line 1272 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpCaret, (yyvsp[0].expr)); }
#line 5595 "bison_parser.cpp"
    break;

  case 232: /* binary_expr: operand LIKE operand  */
#line 1273 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLike, (yyvsp[0].expr)); }
#line 5601 "bison_parser.cpp"
    break;

  case 233: /* binary_expr: operand NOT LIKE operand  */
#line 1274 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-3].expr), kOpNotLike, (yyvsp[0].expr)); }
#line 5607 "bison_parser.cpp"
    break;

  case 234: /* binary_expr: operand ILIKE operand  */
#line 1275 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpILike, (yyvsp[0].expr)); }
#line 5613 "bison_parser.cpp"
    break;

  case 235: /* binary_expr: operand CONCAT operand  */
#line 1276 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpConcat, (yyvsp[0].expr)); }
#line 5619 "bison_parser.cpp"
    break;

  case 236: /* logic_expr: expr AND expr  */
#line 1278 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAnd, (yyvsp[0].expr)); }
#line 5625 "bison_parser.cpp"
    break;

  case 237: /* logic_expr: expr OR expr  */
#line 1279 "bison_parser.y"
               { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpOr, (yyvsp[0].expr)); }
#line 5631 "bison_parser.cpp"
    break;

  case 238: /* in_expr: operand IN '(' expr_list ')'  */
#line 1281 "bison_parser.y"
                                       { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].expr_vec)); }
#line 5637 "bison_parser.cpp"
    break;

  case 239: /* in_expr: operand NOT IN '(' expr_list ')'  */
#line 1282 "bison_parser.y"
                                   { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].expr_vec))); }
#line 5643 "bison_parser.cpp"
    break;

  case 240: /* in_expr: operand IN '(' select_no_paren ')'  */
#line 1283 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].select_stmt)); }
#line 5649 "bison_parser.cpp"
    break;

  case 241: /* in_expr: operand NOT IN '(' select_no_paren ')'  */
#line 1284 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].select_stmt))); }
#line 5655 "bison_parser.cpp"
    break;

  case 242: /* in_expr: '(' row_expr_list ')' IN '(' select_no_paren ')'  */
#line 1288 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeInOperator(Expr::makeArray((yyvsp[-5].expr_vec)), (yyvsp[-1].select_stmt)); }
#line 5661 "bison_parser.cpp"
    break;

  case 243: /* in_expr: '(' row_expr_list ')' NOT IN '(' select_no_paren ')'  */
#line 1289 "bison_parser.y"
                                                       { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator(Expr::makeArray((yyvsp[-6].expr_vec)), (yyvsp[-1].select_stmt))); }
#line 5667 "bison_parser.cpp"
    break;

  case 244: /* row_expr_list: expr_alias ',' expr_alias  */
#line 1293 "bison_parser.y"
                                          {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[-2].expr));
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5677 "bison_parser.cpp"
    break;

  case 245: /* row_expr_list: row_expr_list ',' expr_alias  */
#line 1298 "bison_parser.y"
                               {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5686 "bison_parser.cpp"
    break;

  case 246: /* case_expr: CASE expr case_list END  */
#line 1305 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-2].expr), (yyvsp[-1].expr), nullptr); }
#line 5692 "bison_parser.cpp"
    break;

  case 247: /* case_expr: CASE expr case_list ELSE expr END  */
#line 1306 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-4].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5698 "bison_parser.cpp"
    break;

  case 248: /* case_expr: CASE case_list END  */
#line 1307 "bison_parser.y"
                     { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-1].expr), nullptr); }
#line 5704 "bison_parser.cpp"
    break;

  case 249: /* case_expr: CASE case_list ELSE expr END  */
#line 1308 "bison_parser.y"
                               { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5710 "bison_parser.cpp"
    break;

  case 250: /* case_list: WHEN expr THEN expr  */
#line 1310 "bison_parser.y"
                                { (yyval.expr) = Expr::makeCaseList(Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5716 "bison_parser.cpp"
    break;

  case 251: /* case_list: case_list WHEN expr THEN expr  */
#line 1311 "bison_parser.y"
                                { (yyval.expr) = Expr::caseListAppend((yyvsp[-4].expr), Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5722 "bison_parser.cpp"
    break;

  case 252: /* exists_expr: EXISTS '(' select_no_paren ')'  */
#line 1313 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeExists((yyvsp[-1].select_stmt)); }
#line 5728 "bison_parser.cpp"
    break;

  case 253: /* exists_expr: NOT EXISTS '(' select_no_paren ')'  */
#line 1314 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeExists((yyvsp[-1].select_stmt))); }
#line 5734 "bison_parser.cpp"
    break;

  case 254: /* comp_expr: operand '=' operand  */
#line 1316 "bison_parser.y"
                                { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5740 "bison_parser.cpp"
    break;

  case 255: /* comp_expr: operand EQUALS operand  */
#line 1317 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5746 "bison_parser.cpp"
    break;

  case 256: /* comp_expr: operand NOTEQUALS operand  */
#line 1318 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpNotEquals, (yyvsp[0].expr)); }
#line 5752 "bison_parser.cpp"
    break;

  case 257: /* comp_expr: operand '<' operand  */
#line 1319 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLess, (yyvsp[0].expr)); }
#line 5758 "bison_parser.cpp"
    break;

  case 258: /* comp_expr: operand '>' operand  */
#line 1320 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreater, (yyvsp[0].expr)); }
#line 5764 "bison_parser.cpp"
    break;

  case 259: /* comp_expr: operand LESSEQ operand  */
#line 1321 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLessEq, (yyvsp[0].expr)); }
#line 5770 "bison_parser.cpp"
    break;

  case 260: /* comp_expr: operand GREATEREQ operand  */
#line 1322 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreaterEq, (yyvsp[0].expr)); }
#line 5776 "bison_parser.cpp"
    break;

  case 261: /* function_expr: IDENTIFIER '(' ')' opt_window  */
#line 1326 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 5782 "bison_parser.cpp"
    break;

  case 262: /* function_expr: IDENTIFIER '(' opt_distinct expr_list ')' opt_within_group opt_window  */
#line 1327 "bison_parser.y"
                                                                        {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-6].sval), (yyvsp[-3].expr_vec), (yyvsp[-4].bval), (yyvsp[0].window_description));
  (yyval.expr)->withinGroupOrder = (yyvsp[-1].order_vec);
}
#line 5791 "bison_parser.cpp"
    break;

  case 263: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' ')' opt_window  */
#line 1331 "bison_parser.y"
                                               {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), (yyvsp[-5].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description));
}
#line 5799 "bison_parser.cpp"
    break;

  case 264: /* function_expr: IDENTIFIER '.' IDENTIFIER '(' opt_distinct expr_list ')' opt_window  */
#line 1334 "bison_parser.y"
                                                                      {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-7].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description));
}
#line 5807 "bison_parser.cpp"
    break;

  case 265: /* function_expr: nonreserved_keyword '(' ')' opt_window  */
#line 1339 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 5813 "bison_parser.cpp"
    break;

  case 266: /* function_expr: nonreserved_keyword '(' opt_distinct expr_list ')' opt_window  */
#line 1340 "bison_parser.y"
                                                                { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description)); }
#line 5819 "bison_parser.cpp"
    break;

  case 267: /* table_function_expr: IDENTIFIER '(' ')'  */
#line 1345 "bison_parser.y"
                                         {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-2].sval), new std::vector<Expr*>(), false, nullptr);
}
#line 5827 "bison_parser.cpp"
    break;

  case 268: /* table_function_expr: IDENTIFIER '(' opt_distinct expr_list ')'  */
#line 1348 "bison_parser.y"
                                            {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-4].sval), (yyvsp[-1].expr_vec), (yyvsp[-2].bval), nullptr);
}
#line 5835 "bison_parser.cpp"
    break;

  case 269: /* table_function_expr: IDENTIFIER '.' IDENTIFIER '(' ')'  */
#line 1351 "bison_parser.y"
                                    {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-2].sval), (yyvsp[-4].sval), new std::vector<Expr*>(), false, nullptr);
}
#line 5843 "bison_parser.cpp"
    break;

  case 270: /* table_function_expr: IDENTIFIER '.' IDENTIFIER '(' opt_distinct expr_list ')'  */
#line 1354 "bison_parser.y"
                                                           {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-4].sval), (yyvsp[-6].sval), (yyvsp[-1].expr_vec), (yyvsp[-2].bval), nullptr);
}
#line 5851 "bison_parser.cpp"
    break;

  case 271: /* opt_window: OVER '(' opt_partition opt_order opt_frame_clause ')'  */
#line 1360 "bison_parser.y"
                                                                   { (yyval.window_description) = new WindowDescription((yyvsp[-3].expr_vec), (yyvsp[-2].order_vec), (yyvsp[-1].frame_description)); }
#line 5857 "bison_parser.cpp"
    break;

  case 272: /* opt_window: %empty  */
#line 1361 "bison_parser.y"
              { (yyval.window_description) = nullptr; }
#line 5863 "bison_parser.cpp"
    break;

  case 273: /* opt_partition: PARTITION BY expr_list  */
#line 1363 "bison_parser.y"
                                       { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5869 "bison_parser.cpp"
    break;

  case 274: /* opt_partition: %empty  */
#line 1364 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5875 "bison_parser.cpp"
    break;

  case 275: /* opt_frame_clause: frame_type frame_bound  */
#line 1369 "bison_parser.y"
                                          { (yyval.frame_description) = new FrameDescription{(yyvsp[-1].frame_type), (yyvsp[0].frame_bound), new FrameBound{0, kCurrentRow, false}}; }
#line 5881 "bison_parser.cpp"
    break;

  case 276: /* opt_frame_clause: frame_type BETWEEN frame_bound AND frame_bound  */
#line 1370 "bison_parser.y"
                                                 { (yyval.frame_description) = new FrameDescription{(yyvsp[-4].frame_type), (yyvsp[-2].frame_bound), (yyvsp[0].frame_bound)}; }
#line 5887 "bison_parser.cpp"
    break;

  case 277: /* opt_frame_clause: %empty  */
#line 1371 "bison_parser.y"
              {
  (yyval.frame_description) = new FrameDescription{kRange, new FrameBound{0, kPreceding, true}, new FrameBound{0, kCurrentRow, false}};
}
#line 5895 "bison_parser.cpp"
    break;

  case 278: /* frame_type: RANGE  */
#line 1375 "bison_parser.y"
                   { (yyval.frame_type) = kRange; }
#line 5901 "bison_parser.cpp"
    break;

  case 279: /* frame_type: ROWS  */
#line 1376 "bison_parser.y"
       { (yyval.frame_type) = kRows; }
#line 5907 "bison_parser.cpp"
    break;

  case 280: /* frame_type: GROUPS  */
#line 1377 "bison_parser.y"
         { (yyval.frame_type) = kGroups; }
#line 5913 "bison_parser.cpp"
    break;

  case 281: /* frame_bound: UNBOUNDED PRECEDING  */
#line 1379 "bison_parser.y"
                                  { (yyval.frame_bound) = new FrameBound{0, kPreceding, true}; }
#line 5919 "bison_parser.cpp"
    break;

  case 282: /* frame_bound: INTVAL PRECEDING  */
#line 1380 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kPreceding, false}; }
#line 5925 "bison_parser.cpp"
    break;

  case 283: /* frame_bound: UNBOUNDED FOLLOWING  */
#line 1381 "bison_parser.y"
                      { (yyval.frame_bound) = new FrameBound{0, kFollowing, true}; }
#line 5931 "bison_parser.cpp"
    break;

  case 284: /* frame_bound: INTVAL FOLLOWING  */
#line 1382 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kFollowing, false}; }
#line 5937 "bison_parser.cpp"
    break;

  case 285: /* frame_bound: CURRENT_ROW  */
#line 1383 "bison_parser.y"
              { (yyval.frame_bound) = new FrameBound{0, kCurrentRow, false}; }
#line 5943 "bison_parser.cpp"
    break;

  case 286: /* extract_expr: EXTRACT '(' datetime_field FROM expr ')'  */
#line 1385 "bison_parser.y"
                                                        { (yyval.expr) = Expr::makeExtract((yyvsp[-3].datetime_field), (yyvsp[-1].expr)); }
#line 5949 "bison_parser.cpp"
    break;

  case 287: /* next_value_expr: NEXT_VALUE_FOR table_name  */
#line 1390 "bison_parser.y"
                                            {
  auto args = new std::vector<Expr*>();
  args->push_back(Expr::makeColumnRef(qualifiedName((yyvsp[0].table_name))));
  (yyval.expr) = Expr::makeFunctionRef(strdup("NEXT VALUE FOR"), args, false, nullptr);
}
#line 5959 "bison_parser.cpp"
    break;

  case 288: /* cast_expr: CAST '(' expr AS column_type ')'  */
#line 1396 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t)); }
#line 5965 "bison_parser.cpp"
    break;

  case 289: /* datetime_field: SECOND  */
#line 1398 "bison_parser.y"
                        { (yyval.datetime_field) = kDatetimeSecond; }
#line 5971 "bison_parser.cpp"
    break;

  case 290: /* datetime_field: MINUTE  */
#line 1399 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMinute; }
#line 5977 "bison_parser.cpp"
    break;

  case 291: /* datetime_field: HOUR  */
#line 1400 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeHour; }
#line 5983 "bison_parser.cpp"
    break;

  case 292: /* datetime_field: DAY  */
#line 1401 "bison_parser.y"
      { (yyval.datetime_field) = kDatetimeDay; }
#line 5989 "bison_parser.cpp"
    break;

  case 293: /* datetime_field: MONTH  */
#line 1402 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeMonth; }
#line 5995 "bison_parser.cpp"
    break;

  case 294: /* datetime_field: YEAR  */
#line 1403 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeYear; }
#line 6001 "bison_parser.cpp"
    break;

  case 295: /* datetime_field_plural: SECONDS  */
#line 1405 "bison_parser.y"
                                { (yyval.datetime_field) = kDatetimeSecond; }
#line 6007 "bison_parser.cpp"
    break;

  case 296: /* datetime_field_plural: MINUTES  */
#line 1406 "bison_parser.y"
          { (yyval.datetime_field) = kDatetimeMinute; }
#line 6013 "bison_parser.cpp"
    break;

  case 297: /* datetime_field_plural: HOURS  */
#line 1407 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeHour; }
#line 6019 "bison_parser.cpp"
    break;

  case 298: /* datetime_field_plural: DAYS  */
#line 1408 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeDay; }
#line 6025 "bison_parser.cpp"
    break;

  case 299: /* datetime_field_plural: MONTHS  */
#line 1409 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMonth; }
#line 6031 "bison_parser.cpp"
    break;

  case 300: /* datetime_field_plural: YEARS  */
#line 1410 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeYear; }
#line 6037 "bison_parser.cpp"
    break;

  case 303: /* array_expr: ARRAY '[' expr_list ']'  */
#line 1414 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeArray((yyvsp[-1].expr_vec)); }
#line 6043 "bison_parser.cpp"
    break;

  case 304: /* array_index: operand '[' int_literal ']'  */
#line 1420 "bison_parser.y"
                                          {
  (yyval.expr) = Expr::makeArrayIndex((yyvsp[-3].expr), (yyvsp[-1].expr)->ival);
  delete (yyvsp[-1].expr);
}
#line 6052 "bison_parser.cpp"
    break;

  case 305: /* between_expr: operand BETWEEN operand AND operand  */
#line 1425 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeBetween((yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 6058 "bison_parser.cpp"
    break;

  case 306: /* column_name: IDENTIFIER  */
#line 1427 "bison_parser.y"
                         { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 6064 "bison_parser.cpp"
    break;

  case 307: /* column_name: IDENTIFIER '.' IDENTIFIER  */
#line 1428 "bison_parser.y"
                            { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 6070 "bison_parser.cpp"
    break;

  case 308: /* column_name: '*'  */
#line 1429 "bison_parser.y"
      { (yyval.expr) = Expr::makeStar(); }
#line 6076 "bison_parser.cpp"
    break;

  case 309: /* column_name: IDENTIFIER '.' '*'  */
#line 1430 "bison_parser.y"
                     { (yyval.expr) = Expr::makeStar((yyvsp[-2].sval)); }
#line 6082 "bison_parser.cpp"
    break;

  case 310: /* column_name: nonreserved_keyword  */
#line 1431 "bison_parser.y"
                      { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 6088 "bison_parser.cpp"
    break;

  case 311: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1437 "bison_parser.y"
                                           { (yyval.expr) = Expr::makeColumnRef(dotJoin((yyvsp[-4].sval), (yyvsp[-2].sval)), (yyvsp[0].sval)); }
#line 6094 "bison_parser.cpp"
    break;

  case 312: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER  */
#line 1438 "bison_parser.y"
                                                          {
  (yyval.expr) = Expr::makeColumnRef(dotJoin(dotJoin((yyvsp[-6].sval), (yyvsp[-4].sval)), (yyvsp[-2].sval)), (yyvsp[0].sval));
}
#line 6102 "bison_parser.cpp"
    break;

  case 313: /* column_name: IDENTIFIER '.' IDENTIFIER '.' '*'  */
#line 1441 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeStar(dotJoin((yyvsp[-4].sval), (yyvsp[-2].sval))); }
#line 6108 "bison_parser.cpp"
    break;

  case 314: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER '.' '*'  */
#line 1442 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeStar(dotJoin(dotJoin((yyvsp[-6].sval), (yyvsp[-4].sval)), (yyvsp[-2].sval))); }
#line 6114 "bison_parser.cpp"
    break;

  case 315: /* column_name: IDENTIFIER '.' nonreserved_keyword  */
#line 1445 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 6120 "bison_parser.cpp"
    break;

  case 316: /* column_name: IDENTIFIER '.' IDENTIFIER '.' nonreserved_keyword  */
#line 1446 "bison_parser.y"
                                                    { (yyval.expr) = Expr::makeColumnRef(dotJoin((yyvsp[-4].sval), (yyvsp[-2].sval)), (yyvsp[0].sval)); }
#line 6126 "bison_parser.cpp"
    break;

  case 317: /* column_name: IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER '.' nonreserved_keyword  */
#line 1447 "bison_parser.y"
                                                                   {
  (yyval.expr) = Expr::makeColumnRef(dotJoin(dotJoin((yyvsp[-6].sval), (yyvsp[-4].sval)), (yyvsp[-2].sval)), (yyvsp[0].sval));
}
#line 6134 "bison_parser.cpp"
    break;

  case 318: /* nonreserved_keyword: SECOND  */
#line 1457 "bison_parser.y"
                             { (yyval.sval) = strdup("SECOND"); }
#line 6140 "bison_parser.cpp"
    break;

  case 319: /* nonreserved_keyword: MINUTE  */
#line 1458 "bison_parser.y"
         { (yyval.sval) = strdup("MINUTE"); }
#line 6146 "bison_parser.cpp"
    break;

  case 320: /* nonreserved_keyword: HOUR  */
#line 1459 "bison_parser.y"
       { (yyval.sval) = strdup("HOUR"); }
#line 6152 "bison_parser.cpp"
    break;

  case 321: /* nonreserved_keyword: DAY  */
#line 1460 "bison_parser.y"
      { (yyval.sval) = strdup("DAY"); }
#line 6158 "bison_parser.cpp"
    break;

  case 322: /* nonreserved_keyword: MONTH  */
#line 1461 "bison_parser.y"
        { (yyval.sval) = strdup("MONTH"); }
#line 6164 "bison_parser.cpp"
    break;

  case 323: /* nonreserved_keyword: YEAR  */
#line 1462 "bison_parser.y"
       { (yyval.sval) = strdup("YEAR"); }
#line 6170 "bison_parser.cpp"
    break;

  case 324: /* nonreserved_keyword: SECONDS  */
#line 1463 "bison_parser.y"
          { (yyval.sval) = strdup("SECONDS"); }
#line 6176 "bison_parser.cpp"
    break;

  case 325: /* nonreserved_keyword: MINUTES  */
#line 1464 "bison_parser.y"
          { (yyval.sval) = strdup("MINUTES"); }
#line 6182 "bison_parser.cpp"
    break;

  case 326: /* nonreserved_keyword: HOURS  */
#line 1465 "bison_parser.y"
        { (yyval.sval) = strdup("HOURS"); }
#line 6188 "bison_parser.cpp"
    break;

  case 327: /* nonreserved_keyword: DAYS  */
#line 1466 "bison_parser.y"
       { (yyval.sval) = strdup("DAYS"); }
#line 6194 "bison_parser.cpp"
    break;

  case 328: /* nonreserved_keyword: MONTHS  */
#line 1467 "bison_parser.y"
         { (yyval.sval) = strdup("MONTHS"); }
#line 6200 "bison_parser.cpp"
    break;

  case 329: /* nonreserved_keyword: YEARS  */
#line 1468 "bison_parser.y"
        { (yyval.sval) = strdup("YEARS"); }
#line 6206 "bison_parser.cpp"
    break;

  case 330: /* nonreserved_keyword: ISNULL  */
#line 1469 "bison_parser.y"
         { (yyval.sval) = strdup("ISNULL"); }
#line 6212 "bison_parser.cpp"
    break;

  case 331: /* nonreserved_keyword: FORMAT  */
#line 1470 "bison_parser.y"
         { (yyval.sval) = strdup("FORMAT"); }
#line 6218 "bison_parser.cpp"
    break;

  case 332: /* nonreserved_keyword: CHAR  */
#line 1471 "bison_parser.y"
       { (yyval.sval) = strdup("CHAR"); }
#line 6224 "bison_parser.cpp"
    break;

  case 333: /* nonreserved_keyword: VARCHAR  */
#line 1472 "bison_parser.y"
          { (yyval.sval) = strdup("VARCHAR"); }
#line 6230 "bison_parser.cpp"
    break;

  case 334: /* nonreserved_keyword: INT  */
#line 1473 "bison_parser.y"
      { (yyval.sval) = strdup("INT"); }
#line 6236 "bison_parser.cpp"
    break;

  case 335: /* nonreserved_keyword: INTEGER  */
#line 1474 "bison_parser.y"
          { (yyval.sval) = strdup("INTEGER"); }
#line 6242 "bison_parser.cpp"
    break;

  case 336: /* nonreserved_keyword: DATETIME  */
#line 1475 "bison_parser.y"
           { (yyval.sval) = strdup("DATETIME"); }
#line 6248 "bison_parser.cpp"
    break;

  case 337: /* nonreserved_keyword: TIMESTAMP  */
#line 1476 "bison_parser.y"
            { (yyval.sval) = strdup("TIMESTAMP"); }
#line 6254 "bison_parser.cpp"
    break;

  case 338: /* nonreserved_keyword: NEXT  */
#line 1477 "bison_parser.y"
       { (yyval.sval) = strdup("NEXT"); }
#line 6260 "bison_parser.cpp"
    break;

  case 348: /* string_literal: STRING  */
#line 1486 "bison_parser.y"
                        { (yyval.expr) = Expr::makeLiteral((yyvsp[0].sval)); }
#line 6266 "bison_parser.cpp"
    break;

  case 349: /* bool_literal: TRUE  */
#line 1488 "bison_parser.y"
                    { (yyval.expr) = Expr::makeLiteral(true); }
#line 6272 "bison_parser.cpp"
    break;

  case 350: /* bool_literal: FALSE  */
#line 1489 "bison_parser.y"
        { (yyval.expr) = Expr::makeLiteral(false); }
#line 6278 "bison_parser.cpp"
    break;

  case 351: /* num_literal: FLOATVAL  */
#line 1491 "bison_parser.y"
                       { (yyval.expr) = Expr::makeLiteral((yyvsp[0].fval)); }
#line 6284 "bison_parser.cpp"
    break;

  case 353: /* int_literal: INTVAL  */
#line 1494 "bison_parser.y"
                     { (yyval.expr) = Expr::makeLiteral((yyvsp[0].ival)); }
#line 6290 "bison_parser.cpp"
    break;

  case 354: /* null_literal: NULL  */
#line 1496 "bison_parser.y"
                    { (yyval.expr) = Expr::makeNullLiteral(); }
#line 6296 "bison_parser.cpp"
    break;

  case 355: /* date_literal: DATE STRING  */
#line 1498 "bison_parser.y"
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
#line 6311 "bison_parser.cpp"
    break;

  case 356: /* interval_literal: INTVAL duration_field  */
#line 1509 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeIntervalLiteral((yyvsp[-1].ival), (yyvsp[0].datetime_field)); }
#line 6317 "bison_parser.cpp"
    break;

  case 357: /* interval_literal: INTERVAL STRING datetime_field  */
#line 1510 "bison_parser.y"
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
#line 6333 "bison_parser.cpp"
    break;

  case 358: /* interval_literal: INTERVAL STRING  */
#line 1521 "bison_parser.y"
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
#line 6369 "bison_parser.cpp"
    break;

  case 359: /* param_expr: '?'  */
#line 1553 "bison_parser.y"
                 {
  (yyval.expr) = Expr::makeParameter(yylloc.total_column);
  (yyval.expr)->ival2 = yylloc.total_column - 1;  // source column (0-based) of the '?' token
  yyloc.param_list.push_back((yyval.expr));
}
#line 6379 "bison_parser.cpp"
    break;

  case 360: /* param_expr: DOLLAR_PARAM  */
#line 1558 "bison_parser.y"
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
#line 6396 "bison_parser.cpp"
    break;

  case 361: /* param_expr: NAMED_PARAM  */
#line 1570 "bison_parser.y"
              {
  (yyval.expr) = Expr::makeNamedParameter((yyvsp[0].sval));
  (yyval.expr)->ival2 = yylloc.total_column - 1 - (int64_t)strlen((yyvsp[0].sval));
  yyloc.param_list.push_back((yyval.expr));
}
#line 6406 "bison_parser.cpp"
    break;

  case 363: /* table_ref: table_ref_commalist ',' table_ref_atomic  */
#line 1579 "bison_parser.y"
                                                                        {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  auto tbl = new TableRef(kTableCrossProduct);
  tbl->list = (yyvsp[-2].table_vec);
  (yyval.table) = tbl;
}
#line 6417 "bison_parser.cpp"
    break;

  case 366: /* table_ref_atomic: ODBC_OJ table_ref_atomic '}'  */
#line 1590 "bison_parser.y"
                               { (yyval.table) = (yyvsp[-1].table); }
#line 6423 "bison_parser.cpp"
    break;

  case 368: /* nonjoin_table_ref_atomic: '(' select_statement ')' opt_table_alias  */
#line 1592 "bison_parser.y"
                                                                                     {
  auto tbl = new TableRef(kTableSelect);
  tbl->select = (yyvsp[-2].select_stmt);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6434 "bison_parser.cpp"
    break;

  case 369: /* nonjoin_table_ref_atomic: table_function_expr opt_table_alias  */
#line 1600 "bison_parser.y"
                                      {
  auto tbl = new TableRef(kTableFunc);
  tbl->func = (yyvsp[-1].expr);
  if ((yyvsp[-1].expr)->name) tbl->name = strdup((yyvsp[-1].expr)->name);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6446 "bison_parser.cpp"
    break;

  case 370: /* nonjoin_table_ref_atomic: '(' VALUES table_value_row_list ')' opt_table_alias  */
#line 1609 "bison_parser.y"
                                                      {
  auto tbl = new TableRef(kTableValues);
  tbl->values = (yyvsp[-2].expr_vec);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6457 "bison_parser.cpp"
    break;

  case 371: /* table_value_row_list: '(' expr_list ')'  */
#line 1618 "bison_parser.y"
                                         {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
}
#line 6466 "bison_parser.cpp"
    break;

  case 372: /* table_value_row_list: table_value_row_list ',' '(' expr_list ')'  */
#line 1622 "bison_parser.y"
                                             {
  (yyvsp[-4].expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
  (yyval.expr_vec) = (yyvsp[-4].expr_vec);
}
#line 6475 "bison_parser.cpp"
    break;

  case 373: /* table_ref_commalist: table_ref_atomic  */
#line 1627 "bison_parser.y"
                                       {
  (yyval.table_vec) = new std::vector<TableRef*>();
  (yyval.table_vec)->push_back((yyvsp[0].table));
}
#line 6484 "bison_parser.cpp"
    break;

  case 374: /* table_ref_commalist: table_ref_commalist ',' table_ref_atomic  */
#line 1631 "bison_parser.y"
                                           {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  (yyval.table_vec) = (yyvsp[-2].table_vec);
}
#line 6493 "bison_parser.cpp"
    break;

  case 375: /* table_ref_name: table_name opt_table_alias  */
#line 1636 "bison_parser.y"
                                            {
  auto tbl = new TableRef(kTableName);
  tbl->schema = (yyvsp[-1].table_name).schema;
  tbl->name = (yyvsp[-1].table_name).name;
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6505 "bison_parser.cpp"
    break;

  case 376: /* table_ref_name_no_alias: table_name  */
#line 1644 "bison_parser.y"
                                     {
  (yyval.table) = new TableRef(kTableName);
  (yyval.table)->schema = (yyvsp[0].table_name).schema;
  (yyval.table)->name = (yyvsp[0].table_name).name;
}
#line 6515 "bison_parser.cpp"
    break;

  case 377: /* table_name: name_or_keyword  */
#line 1653 "bison_parser.y"
                             {
  (yyval.table_name).schema = nullptr;
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6524 "bison_parser.cpp"
    break;

  case 378: /* table_name: IDENTIFIER '.' name_or_keyword  */
#line 1657 "bison_parser.y"
                                 {
  (yyval.table_name).schema = (yyvsp[-2].sval);
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6533 "bison_parser.cpp"
    break;

  case 379: /* table_name: IDENTIFIER '.' IDENTIFIER '.' name_or_keyword  */
#line 1661 "bison_parser.y"
                                                {
  // Three-part (database.schema.table) name. TableName has no separate
  // database slot, so fold database+schema into schema as "database.schema" -
  // callers here only need the statement to parse, not the individual parts.
  (yyval.table_name).schema = dotJoin((yyvsp[-4].sval), (yyvsp[-2].sval));
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6545 "bison_parser.cpp"
    break;

  case 380: /* opt_index_name: name_or_keyword  */
#line 1669 "bison_parser.y"
                                 { (yyval.sval) = (yyvsp[0].sval); }
#line 6551 "bison_parser.cpp"
    break;

  case 381: /* opt_index_name: %empty  */
#line 1670 "bison_parser.y"
              { (yyval.sval) = nullptr; }
#line 6557 "bison_parser.cpp"
    break;

  case 383: /* table_alias: AS name_or_keyword '(' ident_commalist ')'  */
#line 1672 "bison_parser.y"
                                                                 { (yyval.alias_t) = new Alias((yyvsp[-3].sval), (yyvsp[-1].str_vec)); }
#line 6563 "bison_parser.cpp"
    break;

  case 385: /* opt_table_alias: %empty  */
#line 1674 "bison_parser.y"
                                            { (yyval.alias_t) = nullptr; }
#line 6569 "bison_parser.cpp"
    break;

  case 386: /* alias: AS IDENTIFIER  */
#line 1676 "bison_parser.y"
                      { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6575 "bison_parser.cpp"
    break;

  case 387: /* alias: AS nonreserved_keyword  */
#line 1680 "bison_parser.y"
                         { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6581 "bison_parser.cpp"
    break;

  case 388: /* alias: AS STRING  */
#line 1684 "bison_parser.y"
            { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6587 "bison_parser.cpp"
    break;

  case 389: /* alias: IDENTIFIER  */
#line 1685 "bison_parser.y"
             { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6593 "bison_parser.cpp"
    break;

  case 391: /* opt_alias: %empty  */
#line 1687 "bison_parser.y"
                                { (yyval.alias_t) = nullptr; }
#line 6599 "bison_parser.cpp"
    break;

  case 392: /* opt_locking_clause: opt_locking_clause_list  */
#line 1693 "bison_parser.y"
                                             { (yyval.locking_clause_vec) = (yyvsp[0].locking_clause_vec); }
#line 6605 "bison_parser.cpp"
    break;

  case 393: /* opt_locking_clause: %empty  */
#line 1694 "bison_parser.y"
              { (yyval.locking_clause_vec) = nullptr; }
#line 6611 "bison_parser.cpp"
    break;

  case 394: /* opt_locking_clause_list: locking_clause  */
#line 1696 "bison_parser.y"
                                         {
  (yyval.locking_clause_vec) = new std::vector<LockingClause*>();
  (yyval.locking_clause_vec)->push_back((yyvsp[0].locking_t));
}
#line 6620 "bison_parser.cpp"
    break;

  case 395: /* opt_locking_clause_list: opt_locking_clause_list locking_clause  */
#line 1700 "bison_parser.y"
                                         {
  (yyvsp[-1].locking_clause_vec)->push_back((yyvsp[0].locking_t));
  (yyval.locking_clause_vec) = (yyvsp[-1].locking_clause_vec);
}
#line 6629 "bison_parser.cpp"
    break;

  case 396: /* locking_clause: FOR row_lock_mode opt_row_lock_policy  */
#line 1705 "bison_parser.y"
                                                       {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-1].lock_mode_t);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
  (yyval.locking_t)->tables = nullptr;
}
#line 6640 "bison_parser.cpp"
    break;

  case 397: /* locking_clause: FOR row_lock_mode OF ident_commalist opt_row_lock_policy  */
#line 1711 "bison_parser.y"
                                                           {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-3].lock_mode_t);
  (yyval.locking_t)->tables = (yyvsp[-1].str_vec);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
}
#line 6651 "bison_parser.cpp"
    break;

  case 398: /* row_lock_mode: UPDATE  */
#line 1718 "bison_parser.y"
                       { (yyval.lock_mode_t) = RowLockMode::ForUpdate; }
#line 6657 "bison_parser.cpp"
    break;

  case 399: /* row_lock_mode: NO KEY UPDATE  */
#line 1719 "bison_parser.y"
                { (yyval.lock_mode_t) = RowLockMode::ForNoKeyUpdate; }
#line 6663 "bison_parser.cpp"
    break;

  case 400: /* row_lock_mode: SHARE  */
#line 1720 "bison_parser.y"
        { (yyval.lock_mode_t) = RowLockMode::ForShare; }
#line 6669 "bison_parser.cpp"
    break;

  case 401: /* row_lock_mode: KEY SHARE  */
#line 1721 "bison_parser.y"
            { (yyval.lock_mode_t) = RowLockMode::ForKeyShare; }
#line 6675 "bison_parser.cpp"
    break;

  case 402: /* opt_row_lock_policy: SKIP LOCKED  */
#line 1723 "bison_parser.y"
                                  { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::SkipLocked; }
#line 6681 "bison_parser.cpp"
    break;

  case 403: /* opt_row_lock_policy: NOWAIT  */
#line 1724 "bison_parser.y"
         { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::NoWait; }
#line 6687 "bison_parser.cpp"
    break;

  case 404: /* opt_row_lock_policy: %empty  */
#line 1725 "bison_parser.y"
              { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::None; }
#line 6693 "bison_parser.cpp"
    break;

  case 406: /* opt_with_clause: %empty  */
#line 1731 "bison_parser.y"
                                            { (yyval.with_description_vec) = nullptr; }
#line 6699 "bison_parser.cpp"
    break;

  case 407: /* with_clause: WITH with_description_list  */
#line 1733 "bison_parser.y"
                                         { (yyval.with_description_vec) = (yyvsp[0].with_description_vec); }
#line 6705 "bison_parser.cpp"
    break;

  case 408: /* with_description_list: with_description  */
#line 1735 "bison_parser.y"
                                         {
  (yyval.with_description_vec) = new std::vector<WithDescription*>();
  (yyval.with_description_vec)->push_back((yyvsp[0].with_description_t));
}
#line 6714 "bison_parser.cpp"
    break;

  case 409: /* with_description_list: with_description_list ',' with_description  */
#line 1739 "bison_parser.y"
                                             {
  (yyvsp[-2].with_description_vec)->push_back((yyvsp[0].with_description_t));
  (yyval.with_description_vec) = (yyvsp[-2].with_description_vec);
}
#line 6723 "bison_parser.cpp"
    break;

  case 410: /* with_description: name_or_keyword AS select_with_paren  */
#line 1744 "bison_parser.y"
                                                        {
  (yyval.with_description_t) = new WithDescription();
  (yyval.with_description_t)->alias = (yyvsp[-2].sval);
  (yyval.with_description_t)->select = (yyvsp[0].select_stmt);
}
#line 6733 "bison_parser.cpp"
    break;

  case 411: /* join_clause: table_ref_atomic NATURAL JOIN nonjoin_table_ref_atomic  */
#line 1754 "bison_parser.y"
                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = kJoinNatural;
  (yyval.table)->join->left = (yyvsp[-3].table);
  (yyval.table)->join->right = (yyvsp[0].table);
}
#line 6745 "bison_parser.cpp"
    break;

  case 412: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic ON join_condition  */
#line 1761 "bison_parser.y"
                                                                         {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (JoinType)(yyvsp[-4].join_type);
  (yyval.table)->join->left = (yyvsp[-5].table);
  (yyval.table)->join->right = (yyvsp[-2].table);
  (yyval.table)->join->condition = (yyvsp[0].expr);
}
#line 6758 "bison_parser.cpp"
    break;

  case 413: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic USING '(' ident_commalist ')'  */
#line 1769 "bison_parser.y"
                                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (yyvsp[-6].join_type);
  (yyval.table)->join->left = (yyvsp[-7].table);
  (yyval.table)->join->right = (yyvsp[-4].table);
  (yyval.table)->join->namedColumns = (yyvsp[-1].str_vec);
}
#line 6771 "bison_parser.cpp"
    break;

  case 414: /* opt_join_type: INNER  */
#line 1778 "bison_parser.y"
                      { (yyval.join_type) = kJoinInner; }
#line 6777 "bison_parser.cpp"
    break;

  case 415: /* opt_join_type: LEFT OUTER  */
#line 1779 "bison_parser.y"
             { (yyval.join_type) = kJoinLeft; }
#line 6783 "bison_parser.cpp"
    break;

  case 416: /* opt_join_type: LEFT  */
#line 1780 "bison_parser.y"
       { (yyval.join_type) = kJoinLeft; }
#line 6789 "bison_parser.cpp"
    break;

  case 417: /* opt_join_type: RIGHT OUTER  */
#line 1781 "bison_parser.y"
              { (yyval.join_type) = kJoinRight; }
#line 6795 "bison_parser.cpp"
    break;

  case 418: /* opt_join_type: RIGHT  */
#line 1782 "bison_parser.y"
        { (yyval.join_type) = kJoinRight; }
#line 6801 "bison_parser.cpp"
    break;

  case 419: /* opt_join_type: FULL OUTER  */
#line 1783 "bison_parser.y"
             { (yyval.join_type) = kJoinFull; }
#line 6807 "bison_parser.cpp"
    break;

  case 420: /* opt_join_type: OUTER  */
#line 1784 "bison_parser.y"
        { (yyval.join_type) = kJoinFull; }
#line 6813 "bison_parser.cpp"
    break;

  case 421: /* opt_join_type: FULL  */
#line 1785 "bison_parser.y"
       { (yyval.join_type) = kJoinFull; }
#line 6819 "bison_parser.cpp"
    break;

  case 422: /* opt_join_type: CROSS  */
#line 1786 "bison_parser.y"
        { (yyval.join_type) = kJoinCross; }
#line 6825 "bison_parser.cpp"
    break;

  case 423: /* opt_join_type: %empty  */
#line 1787 "bison_parser.y"
                       { (yyval.join_type) = kJoinInner; }
#line 6831 "bison_parser.cpp"
    break;

  case 427: /* ident_commalist: name_or_keyword  */
#line 1798 "bison_parser.y"
                                  {
  (yyval.str_vec) = new std::vector<char*>();
  (yyval.str_vec)->push_back((yyvsp[0].sval));
}
#line 6840 "bison_parser.cpp"
    break;

  case 428: /* ident_commalist: ident_commalist ',' name_or_keyword  */
#line 1802 "bison_parser.y"
                                      {
  (yyvsp[-2].str_vec)->push_back((yyvsp[0].sval));
  (yyval.str_vec) = (yyvsp[-2].str_vec);
}
#line 6849 "bison_parser.cpp"
    break;


#line 6853 "bison_parser.cpp"

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

#line 1808 "bison_parser.y"


/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */

    // clang-format on
