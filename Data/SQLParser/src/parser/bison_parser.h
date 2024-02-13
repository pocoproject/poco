/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_HSQL_BISON_PARSER_H_INCLUDED
# define YY_HSQL_BISON_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef HSQL_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define HSQL_DEBUG 1
#  else
#   define HSQL_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define HSQL_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined HSQL_DEBUG */
#if HSQL_DEBUG
extern int hsql_debug;
#endif
/* "%code requires" blocks.  */
#line 38 "bison_parser.y"

// %code requires block

#include "../SQLParserResult.h"
#include "../sql/statements.h"
#include "parser_typedef.h"

// Auto update column and line number
#define YY_USER_ACTION                        \
  yylloc->first_line = yylloc->last_line;     \
  yylloc->first_column = yylloc->last_column; \
  for (int i = 0; yytext[i] != '\0'; i++) {   \
    yylloc->total_column++;                   \
    yylloc->string_length++;                  \
    if (yytext[i] == '\n') {                  \
      yylloc->last_line++;                    \
      yylloc->last_column = 0;                \
    } else {                                  \
      yylloc->last_column++;                  \
    }                                         \
  }

#line 80 "bison_parser.h"

/* Token kinds.  */
#ifndef HSQL_TOKENTYPE
# define HSQL_TOKENTYPE
  enum hsql_tokentype
  {
    SQL_HSQL_EMPTY = -2,
    SQL_YYEOF = 0,                 /* "end of file"  */
    SQL_HSQL_error = 256,          /* error  */
    SQL_HSQL_UNDEF = 257,          /* "invalid token"  */
    SQL_IDENTIFIER = 258,          /* IDENTIFIER  */
    SQL_STRING = 259,              /* STRING  */
    SQL_FLOATVAL = 260,            /* FLOATVAL  */
    SQL_INTVAL = 261,              /* INTVAL  */
    SQL_DEALLOCATE = 262,          /* DEALLOCATE  */
    SQL_PARAMETERS = 263,          /* PARAMETERS  */
    SQL_INTERSECT = 264,           /* INTERSECT  */
    SQL_TEMPORARY = 265,           /* TEMPORARY  */
    SQL_TIMESTAMP = 266,           /* TIMESTAMP  */
    SQL_DISTINCT = 267,            /* DISTINCT  */
    SQL_NVARCHAR = 268,            /* NVARCHAR  */
    SQL_RESTRICT = 269,            /* RESTRICT  */
    SQL_TRUNCATE = 270,            /* TRUNCATE  */
    SQL_ANALYZE = 271,             /* ANALYZE  */
    SQL_BETWEEN = 272,             /* BETWEEN  */
    SQL_CASCADE = 273,             /* CASCADE  */
    SQL_COLUMNS = 274,             /* COLUMNS  */
    SQL_CONTROL = 275,             /* CONTROL  */
    SQL_DEFAULT = 276,             /* DEFAULT  */
    SQL_EXECUTE = 277,             /* EXECUTE  */
    SQL_EXPLAIN = 278,             /* EXPLAIN  */
    SQL_INTEGER = 279,             /* INTEGER  */
    SQL_NATURAL = 280,             /* NATURAL  */
    SQL_PREPARE = 281,             /* PREPARE  */
    SQL_PRIMARY = 282,             /* PRIMARY  */
    SQL_SCHEMAS = 283,             /* SCHEMAS  */
    SQL_CHARACTER_VARYING = 284,   /* CHARACTER_VARYING  */
    SQL_REAL = 285,                /* REAL  */
    SQL_DECIMAL = 286,             /* DECIMAL  */
    SQL_SMALLINT = 287,            /* SMALLINT  */
    SQL_BIGINT = 288,              /* BIGINT  */
    SQL_SPATIAL = 289,             /* SPATIAL  */
    SQL_VARCHAR = 290,             /* VARCHAR  */
    SQL_VIRTUAL = 291,             /* VIRTUAL  */
    SQL_DESCRIBE = 292,            /* DESCRIBE  */
    SQL_BEFORE = 293,              /* BEFORE  */
    SQL_COLUMN = 294,              /* COLUMN  */
    SQL_CREATE = 295,              /* CREATE  */
    SQL_DELETE = 296,              /* DELETE  */
    SQL_DIRECT = 297,              /* DIRECT  */
    SQL_DOUBLE = 298,              /* DOUBLE  */
    SQL_ESCAPE = 299,              /* ESCAPE  */
    SQL_EXCEPT = 300,              /* EXCEPT  */
    SQL_EXISTS = 301,              /* EXISTS  */
    SQL_EXTRACT = 302,             /* EXTRACT  */
    SQL_CAST = 303,                /* CAST  */
    SQL_FORMAT = 304,              /* FORMAT  */
    SQL_GLOBAL = 305,              /* GLOBAL  */
    SQL_HAVING = 306,              /* HAVING  */
    SQL_IMPORT = 307,              /* IMPORT  */
    SQL_INSERT = 308,              /* INSERT  */
    SQL_ISNULL = 309,              /* ISNULL  */
    SQL_OFFSET = 310,              /* OFFSET  */
    SQL_RENAME = 311,              /* RENAME  */
    SQL_SCHEMA = 312,              /* SCHEMA  */
    SQL_SELECT = 313,              /* SELECT  */
    SQL_SORTED = 314,              /* SORTED  */
    SQL_TABLES = 315,              /* TABLES  */
    SQL_UNIQUE = 316,              /* UNIQUE  */
    SQL_UNLOAD = 317,              /* UNLOAD  */
    SQL_UPDATE = 318,              /* UPDATE  */
    SQL_VALUES = 319,              /* VALUES  */
    SQL_AFTER = 320,               /* AFTER  */
    SQL_ALTER = 321,               /* ALTER  */
    SQL_CROSS = 322,               /* CROSS  */
    SQL_DELTA = 323,               /* DELTA  */
    SQL_FLOAT = 324,               /* FLOAT  */
    SQL_GROUP = 325,               /* GROUP  */
    SQL_INDEX = 326,               /* INDEX  */
    SQL_INNER = 327,               /* INNER  */
    SQL_LIMIT = 328,               /* LIMIT  */
    SQL_LOCAL = 329,               /* LOCAL  */
    SQL_MERGE = 330,               /* MERGE  */
    SQL_MINUS = 331,               /* MINUS  */
    SQL_ORDER = 332,               /* ORDER  */
    SQL_OVER = 333,                /* OVER  */
    SQL_OUTER = 334,               /* OUTER  */
    SQL_RIGHT = 335,               /* RIGHT  */
    SQL_TABLE = 336,               /* TABLE  */
    SQL_UNION = 337,               /* UNION  */
    SQL_USING = 338,               /* USING  */
    SQL_WHERE = 339,               /* WHERE  */
    SQL_CALL = 340,                /* CALL  */
    SQL_CASE = 341,                /* CASE  */
    SQL_CHAR = 342,                /* CHAR  */
    SQL_COPY = 343,                /* COPY  */
    SQL_DATE = 344,                /* DATE  */
    SQL_DATETIME = 345,            /* DATETIME  */
    SQL_DESC = 346,                /* DESC  */
    SQL_DROP = 347,                /* DROP  */
    SQL_ELSE = 348,                /* ELSE  */
    SQL_FILE = 349,                /* FILE  */
    SQL_FROM = 350,                /* FROM  */
    SQL_FULL = 351,                /* FULL  */
    SQL_HASH = 352,                /* HASH  */
    SQL_HINT = 353,                /* HINT  */
    SQL_INTO = 354,                /* INTO  */
    SQL_JOIN = 355,                /* JOIN  */
    SQL_LEFT = 356,                /* LEFT  */
    SQL_LIKE = 357,                /* LIKE  */
    SQL_LOAD = 358,                /* LOAD  */
    SQL_LONG = 359,                /* LONG  */
    SQL_NULL = 360,                /* NULL  */
    SQL_PARTITION = 361,           /* PARTITION  */
    SQL_PLAN = 362,                /* PLAN  */
    SQL_SHOW = 363,                /* SHOW  */
    SQL_TEXT = 364,                /* TEXT  */
    SQL_THEN = 365,                /* THEN  */
    SQL_TIME = 366,                /* TIME  */
    SQL_VIEW = 367,                /* VIEW  */
    SQL_WHEN = 368,                /* WHEN  */
    SQL_WITH = 369,                /* WITH  */
    SQL_ADD = 370,                 /* ADD  */
    SQL_ALL = 371,                 /* ALL  */
    SQL_AND = 372,                 /* AND  */
    SQL_ASC = 373,                 /* ASC  */
    SQL_END = 374,                 /* END  */
    SQL_FOR = 375,                 /* FOR  */
    SQL_INT = 376,                 /* INT  */
    SQL_KEY = 377,                 /* KEY  */
    SQL_NOT = 378,                 /* NOT  */
    SQL_OFF = 379,                 /* OFF  */
    SQL_SET = 380,                 /* SET  */
    SQL_TOP = 381,                 /* TOP  */
    SQL_AS = 382,                  /* AS  */
    SQL_BY = 383,                  /* BY  */
    SQL_IF = 384,                  /* IF  */
    SQL_IN = 385,                  /* IN  */
    SQL_IS = 386,                  /* IS  */
    SQL_OF = 387,                  /* OF  */
    SQL_ON = 388,                  /* ON  */
    SQL_OR = 389,                  /* OR  */
    SQL_TO = 390,                  /* TO  */
    SQL_NO = 391,                  /* NO  */
    SQL_ARRAY = 392,               /* ARRAY  */
    SQL_CONCAT = 393,              /* CONCAT  */
    SQL_ILIKE = 394,               /* ILIKE  */
    SQL_SECOND = 395,              /* SECOND  */
    SQL_MINUTE = 396,              /* MINUTE  */
    SQL_HOUR = 397,                /* HOUR  */
    SQL_DAY = 398,                 /* DAY  */
    SQL_MONTH = 399,               /* MONTH  */
    SQL_YEAR = 400,                /* YEAR  */
    SQL_SECONDS = 401,             /* SECONDS  */
    SQL_MINUTES = 402,             /* MINUTES  */
    SQL_HOURS = 403,               /* HOURS  */
    SQL_DAYS = 404,                /* DAYS  */
    SQL_MONTHS = 405,              /* MONTHS  */
    SQL_YEARS = 406,               /* YEARS  */
    SQL_INTERVAL = 407,            /* INTERVAL  */
    SQL_TRUE = 408,                /* TRUE  */
    SQL_FALSE = 409,               /* FALSE  */
    SQL_BOOLEAN = 410,             /* BOOLEAN  */
    SQL_TRANSACTION = 411,         /* TRANSACTION  */
    SQL_BEGIN = 412,               /* BEGIN  */
    SQL_COMMIT = 413,              /* COMMIT  */
    SQL_ROLLBACK = 414,            /* ROLLBACK  */
    SQL_NOWAIT = 415,              /* NOWAIT  */
    SQL_SKIP = 416,                /* SKIP  */
    SQL_LOCKED = 417,              /* LOCKED  */
    SQL_SHARE = 418,               /* SHARE  */
    SQL_RANGE = 419,               /* RANGE  */
    SQL_ROWS = 420,                /* ROWS  */
    SQL_GROUPS = 421,              /* GROUPS  */
    SQL_UNBOUNDED = 422,           /* UNBOUNDED  */
    SQL_FOLLOWING = 423,           /* FOLLOWING  */
    SQL_PRECEDING = 424,           /* PRECEDING  */
    SQL_CURRENT_ROW = 425,         /* CURRENT_ROW  */
    SQL_EQUALS = 426,              /* EQUALS  */
    SQL_NOTEQUALS = 427,           /* NOTEQUALS  */
    SQL_LESS = 428,                /* LESS  */
    SQL_GREATER = 429,             /* GREATER  */
    SQL_LESSEQ = 430,              /* LESSEQ  */
    SQL_GREATEREQ = 431,           /* GREATEREQ  */
    SQL_NOTNULL = 432,             /* NOTNULL  */
    SQL_UMINUS = 433               /* UMINUS  */
  };
  typedef enum hsql_tokentype hsql_token_kind_t;
#endif

/* Value type.  */
#if ! defined HSQL_STYPE && ! defined HSQL_STYPE_IS_DECLARED
union HSQL_STYPE
{
#line 96 "bison_parser.y"

  // clang-format on
  bool bval;
  char* sval;
  double fval;
  int64_t ival;
  uintmax_t uval;

  // statements
  hsql::AlterStatement* alter_stmt;
  hsql::CreateStatement* create_stmt;
  hsql::DeleteStatement* delete_stmt;
  hsql::DropStatement* drop_stmt;
  hsql::ExecuteStatement* exec_stmt;
  hsql::ExportStatement* export_stmt;
  hsql::ImportStatement* import_stmt;
  hsql::InsertStatement* insert_stmt;
  hsql::PrepareStatement* prep_stmt;
  hsql::SelectStatement* select_stmt;
  hsql::ShowStatement* show_stmt;
  hsql::SQLStatement* statement;
  hsql::TransactionStatement* transaction_stmt;
  hsql::UpdateStatement* update_stmt;

  hsql::Alias* alias_t;
  hsql::AlterAction* alter_action_t;
  hsql::ColumnDefinition* column_t;
  hsql::ColumnType column_type_t;
  hsql::ConstraintType column_constraint_t;
  hsql::DatetimeField datetime_field;
  hsql::DropColumnAction* drop_action_t;
  hsql::Expr* expr;
  hsql::FrameBound* frame_bound;
  hsql::FrameDescription* frame_description;
  hsql::FrameType frame_type;
  hsql::GroupByDescription* group_t;
  hsql::ImportType import_type_t;
  hsql::JoinType join_type;
  hsql::LimitDescription* limit;
  hsql::LockingClause* locking_t;
  hsql::OrderDescription* order;
  hsql::OrderType order_type;
  hsql::SetOperation* set_operator_t;
  hsql::TableConstraint* table_constraint_t;
  hsql::TableElement* table_element_t;
  hsql::TableName table_name;
  hsql::TableRef* table;
  hsql::UpdateClause* update_t;
  hsql::WindowDescription* window_description;
  hsql::WithDescription* with_description_t;

  std::vector<char*>* str_vec;
  std::unordered_set<hsql::ConstraintType>* column_constraint_set;
  std::vector<hsql::Expr*>* expr_vec;
  std::vector<hsql::OrderDescription*>* order_vec;
  std::vector<hsql::SQLStatement*>* stmt_vec;
  std::vector<hsql::TableElement*>* table_element_vec;
  std::vector<hsql::TableRef*>* table_vec;
  std::vector<hsql::UpdateClause*>* update_vec;
  std::vector<hsql::WithDescription*>* with_description_vec;
  std::vector<hsql::LockingClause*>* locking_clause_vec;

  std::pair<int64_t, int64_t>* ival_pair;

  hsql::RowLockMode lock_mode_t;
  hsql::RowLockWaitPolicy lock_wait_policy_t;

#line 343 "bison_parser.h"

};
typedef union HSQL_STYPE HSQL_STYPE;
# define HSQL_STYPE_IS_TRIVIAL 1
# define HSQL_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined HSQL_LTYPE && ! defined HSQL_LTYPE_IS_DECLARED
typedef struct HSQL_LTYPE HSQL_LTYPE;
struct HSQL_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define HSQL_LTYPE_IS_DECLARED 1
# define HSQL_LTYPE_IS_TRIVIAL 1
#endif




int hsql_parse (hsql::SQLParserResult* result, yyscan_t scanner);


#endif /* !YY_HSQL_BISON_PARSER_H_INCLUDED  */
