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
#line 143 "bison_parser.y"

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

  // Adds a set operation to the select whose result it combines with its
  // nested select.
  void addSetOperation(SelectStatement* stmt, SetOperation* op) {
    if (!stmt->setOperations) stmt->setOperations = new std::vector<SetOperation*>();
    stmt->setOperations->push_back(op);
  }

  // Attaches the clauses around a query expression body to its statement. The
  // WITH clause belongs to the outermost statement. ORDER BY and LIMIT after a
  // set operation describe the combined result and belong to the last set
  // operation of the leading select; otherwise they belong to the select
  // itself. Locking clauses belong to the leading select. A parenthesized
  // query expression takes a clause it does not have yet as its own, the way
  // PostgreSQL merges them: (SELECT a FROM t LIMIT 1) ORDER BY a reads as
  // SELECT a FROM t ORDER BY a LIMIT 1. Returns the message to report when a
  // clause cannot be attached - the statement then owns none of the clauses -
  // and nullptr when all of them are in place.
  const char* attachQueryClauses(const QueryBody& body, std::vector<WithDescription*>* with,
                                 std::vector<OrderDescription*>* order, LimitDescription* limit,
                                 std::vector<LockingClause*>* lockings) {
    SelectStatement* stmt = body.head;
    if (with && stmt->withDescriptions) return "The query expression already has a WITH clause.";
    if (lockings && stmt->lockings) return "The query expression already has a locking clause.";
    if (stmt->setOperations) {
      SetOperation* last = stmt->setOperations->back();
      if (order && last->resultOrder) return "The query expression already has an ORDER BY clause.";
      if (limit && last->resultLimit) return "The query expression already has a LIMIT clause.";
      if (order) last->resultOrder = order;
      if (limit) last->resultLimit = limit;
    } else {
      if (order && stmt->order) return "The query expression already has an ORDER BY clause.";
      if (limit && stmt->limit) {
        // A parenthesized query keeps the limit it has; otherwise the limit
        // comes from TOP. A parenthesized TOP may hold a placeholder, which is
        // also referenced from the parameter list the input rule walks;
        // deleting it here would leave that reference dangling. TOP with a
        // literal has no such reference, so "top 20 ... limit 10" keeps
        // working.
        if (body.parenthesized) return "The query expression already has a LIMIT clause.";
        if (stmt->limit->limit->type != kExprLiteralInt) return "TOP with an expression cannot be combined with LIMIT.";
        // OFFSET alone and LIMIT ALL give no row count, so the one of TOP stays.
        if (!limit->limit) {
          limit->limit = stmt->limit->limit;
          stmt->limit->limit = nullptr;
        }
        delete stmt->limit;
      }
      if (order) stmt->order = order;
      if (limit) stmt->limit = limit;
    }
    if (with) stmt->withDescriptions = with;
    if (lockings) stmt->lockings = lockings;
    return nullptr;
  }

  // Releases the clauses of a query expression its statement did not take.
  void deleteQueryClauses(std::vector<WithDescription*>* with, std::vector<OrderDescription*>* order,
                          LimitDescription* limit, std::vector<LockingClause*>* lockings) {
    if (with) {
      for (auto ptr : *with) delete ptr;
      delete with;
    }
    if (order) {
      for (auto ptr : *order) delete ptr;
      delete order;
    }
    delete limit;
    if (lockings) {
      for (auto ptr : *lockings) delete ptr;
      delete lockings;
    }
  }

  }  // namespace
  // clang-format off

#line 212 "bison_parser.cpp"

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
  YYSYMBOL_CURRENT_DATE = 171,             /* CURRENT_DATE  */
  YYSYMBOL_CURRENT_TIME = 172,             /* CURRENT_TIME  */
  YYSYMBOL_CURRENT_TIMESTAMP = 173,        /* CURRENT_TIMESTAMP  */
  YYSYMBOL_FETCH = 174,                    /* FETCH  */
  YYSYMBOL_NEXT = 175,                     /* NEXT  */
  YYSYMBOL_ONLY = 176,                     /* ONLY  */
  YYSYMBOL_UNIQUE = 177,                   /* UNIQUE  */
  YYSYMBOL_PRIMARY = 178,                  /* PRIMARY  */
  YYSYMBOL_FOREIGN = 179,                  /* FOREIGN  */
  YYSYMBOL_KEY = 180,                      /* KEY  */
  YYSYMBOL_REFERENCES = 181,               /* REFERENCES  */
  YYSYMBOL_OUTERJOIN = 182,                /* OUTERJOIN  */
  YYSYMBOL_PRIOR = 183,                    /* PRIOR  */
  YYSYMBOL_ODBC_OJ = 184,                  /* ODBC_OJ  */
  YYSYMBOL_NEXT_VALUE_FOR = 185,           /* NEXT_VALUE_FOR  */
  YYSYMBOL_START_WITH = 186,               /* START_WITH  */
  YYSYMBOL_CONNECT_BY = 187,               /* CONNECT_BY  */
  YYSYMBOL_CONNECT_BY_NOCYCLE = 188,       /* CONNECT_BY_NOCYCLE  */
  YYSYMBOL_WITHIN_GROUP = 189,             /* WITHIN_GROUP  */
  YYSYMBOL_190_ = 190,                     /* '='  */
  YYSYMBOL_EQUALS = 191,                   /* EQUALS  */
  YYSYMBOL_NOTEQUALS = 192,                /* NOTEQUALS  */
  YYSYMBOL_193_ = 193,                     /* '<'  */
  YYSYMBOL_194_ = 194,                     /* '>'  */
  YYSYMBOL_LESS = 195,                     /* LESS  */
  YYSYMBOL_GREATER = 196,                  /* GREATER  */
  YYSYMBOL_LESSEQ = 197,                   /* LESSEQ  */
  YYSYMBOL_GREATEREQ = 198,                /* GREATEREQ  */
  YYSYMBOL_NOTNULL = 199,                  /* NOTNULL  */
  YYSYMBOL_200_ = 200,                     /* '+'  */
  YYSYMBOL_201_ = 201,                     /* '-'  */
  YYSYMBOL_202_ = 202,                     /* '*'  */
  YYSYMBOL_203_ = 203,                     /* '/'  */
  YYSYMBOL_204_ = 204,                     /* '%'  */
  YYSYMBOL_205_ = 205,                     /* '^'  */
  YYSYMBOL_UMINUS = 206,                   /* UMINUS  */
  YYSYMBOL_207_ = 207,                     /* '['  */
  YYSYMBOL_208_ = 208,                     /* ']'  */
  YYSYMBOL_SUBQUERY_AS_EXPR = 209,         /* SUBQUERY_AS_EXPR  */
  YYSYMBOL_210_ = 210,                     /* '('  */
  YYSYMBOL_211_ = 211,                     /* ')'  */
  YYSYMBOL_212_ = 212,                     /* '.'  */
  YYSYMBOL_213_ = 213,                     /* ';'  */
  YYSYMBOL_214_ = 214,                     /* ','  */
  YYSYMBOL_215_ = 215,                     /* '{'  */
  YYSYMBOL_216_ = 216,                     /* '}'  */
  YYSYMBOL_217_ = 217,                     /* '?'  */
  YYSYMBOL_YYACCEPT = 218,                 /* $accept  */
  YYSYMBOL_input = 219,                    /* input  */
  YYSYMBOL_statement_list = 220,           /* statement_list  */
  YYSYMBOL_statement = 221,                /* statement  */
  YYSYMBOL_preparable_statement = 222,     /* preparable_statement  */
  YYSYMBOL_opt_hints = 223,                /* opt_hints  */
  YYSYMBOL_hint_list = 224,                /* hint_list  */
  YYSYMBOL_hint = 225,                     /* hint  */
  YYSYMBOL_transaction_statement = 226,    /* transaction_statement  */
  YYSYMBOL_opt_transaction_keyword = 227,  /* opt_transaction_keyword  */
  YYSYMBOL_prepare_statement = 228,        /* prepare_statement  */
  YYSYMBOL_prepare_target_query = 229,     /* prepare_target_query  */
  YYSYMBOL_odbc_call_statement = 230,      /* odbc_call_statement  */
  YYSYMBOL_opt_odbc_return = 231,          /* opt_odbc_return  */
  YYSYMBOL_opt_call_args = 232,            /* opt_call_args  */
  YYSYMBOL_execute_statement = 233,        /* execute_statement  */
  YYSYMBOL_import_statement = 234,         /* import_statement  */
  YYSYMBOL_file_type = 235,                /* file_type  */
  YYSYMBOL_file_path = 236,                /* file_path  */
  YYSYMBOL_opt_import_export_options = 237, /* opt_import_export_options  */
  YYSYMBOL_import_export_options = 238,    /* import_export_options  */
  YYSYMBOL_csv_option = 239,               /* csv_option  */
  YYSYMBOL_export_statement = 240,         /* export_statement  */
  YYSYMBOL_show_statement = 241,           /* show_statement  */
  YYSYMBOL_create_statement = 242,         /* create_statement  */
  YYSYMBOL_opt_not_exists = 243,           /* opt_not_exists  */
  YYSYMBOL_table_elem_commalist = 244,     /* table_elem_commalist  */
  YYSYMBOL_table_elem = 245,               /* table_elem  */
  YYSYMBOL_column_def = 246,               /* column_def  */
  YYSYMBOL_column_type = 247,              /* column_type  */
  YYSYMBOL_opt_time_precision = 248,       /* opt_time_precision  */
  YYSYMBOL_opt_decimal_specification = 249, /* opt_decimal_specification  */
  YYSYMBOL_opt_column_constraints = 250,   /* opt_column_constraints  */
  YYSYMBOL_column_constraints = 251,       /* column_constraints  */
  YYSYMBOL_column_constraint = 252,        /* column_constraint  */
  YYSYMBOL_table_constraint = 253,         /* table_constraint  */
  YYSYMBOL_references_spec = 254,          /* references_spec  */
  YYSYMBOL_drop_statement = 255,           /* drop_statement  */
  YYSYMBOL_opt_exists = 256,               /* opt_exists  */
  YYSYMBOL_alter_statement = 257,          /* alter_statement  */
  YYSYMBOL_alter_action = 258,             /* alter_action  */
  YYSYMBOL_drop_action = 259,              /* drop_action  */
  YYSYMBOL_delete_statement = 260,         /* delete_statement  */
  YYSYMBOL_truncate_statement = 261,       /* truncate_statement  */
  YYSYMBOL_insert_statement = 262,         /* insert_statement  */
  YYSYMBOL_opt_column_list = 263,          /* opt_column_list  */
  YYSYMBOL_update_statement = 264,         /* update_statement  */
  YYSYMBOL_update_clause_commalist = 265,  /* update_clause_commalist  */
  YYSYMBOL_update_clause = 266,            /* update_clause  */
  YYSYMBOL_query_expression = 267,         /* query_expression  */
  YYSYMBOL_query_expression_body = 268,    /* query_expression_body  */
  YYSYMBOL_query_term = 269,               /* query_term  */
  YYSYMBOL_query_member = 270,             /* query_member  */
  YYSYMBOL_query_expression_parens = 271,  /* query_expression_parens  */
  YYSYMBOL_subquery = 272,                 /* subquery  */
  YYSYMBOL_union_operator = 273,           /* union_operator  */
  YYSYMBOL_intersect_operator = 274,       /* intersect_operator  */
  YYSYMBOL_opt_all = 275,                  /* opt_all  */
  YYSYMBOL_opt_hierarchical_clause = 276,  /* opt_hierarchical_clause  */
  YYSYMBOL_connect_by = 277,               /* connect_by  */
  YYSYMBOL_query_primary = 278,            /* query_primary  */
  YYSYMBOL_opt_distinct = 279,             /* opt_distinct  */
  YYSYMBOL_select_list = 280,              /* select_list  */
  YYSYMBOL_opt_from_clause = 281,          /* opt_from_clause  */
  YYSYMBOL_from_clause = 282,              /* from_clause  */
  YYSYMBOL_opt_where = 283,                /* opt_where  */
  YYSYMBOL_opt_group = 284,                /* opt_group  */
  YYSYMBOL_opt_having = 285,               /* opt_having  */
  YYSYMBOL_opt_within_group = 286,         /* opt_within_group  */
  YYSYMBOL_opt_order = 287,                /* opt_order  */
  YYSYMBOL_order_list = 288,               /* order_list  */
  YYSYMBOL_order_desc = 289,               /* order_desc  */
  YYSYMBOL_opt_order_type = 290,           /* opt_order_type  */
  YYSYMBOL_opt_null_ordering = 291,        /* opt_null_ordering  */
  YYSYMBOL_opt_top = 292,                  /* opt_top  */
  YYSYMBOL_opt_limit = 293,                /* opt_limit  */
  YYSYMBOL_expr_list = 294,                /* expr_list  */
  YYSYMBOL_opt_extended_literal_list = 295, /* opt_extended_literal_list  */
  YYSYMBOL_extended_literal_list = 296,    /* extended_literal_list  */
  YYSYMBOL_casted_extended_literal = 297,  /* casted_extended_literal  */
  YYSYMBOL_extended_literal = 298,         /* extended_literal  */
  YYSYMBOL_expr_alias = 299,               /* expr_alias  */
  YYSYMBOL_expr = 300,                     /* expr  */
  YYSYMBOL_operand = 301,                  /* operand  */
  YYSYMBOL_scalar_expr = 302,              /* scalar_expr  */
  YYSYMBOL_unary_expr = 303,               /* unary_expr  */
  YYSYMBOL_binary_expr = 304,              /* binary_expr  */
  YYSYMBOL_logic_expr = 305,               /* logic_expr  */
  YYSYMBOL_in_expr = 306,                  /* in_expr  */
  YYSYMBOL_in_row_list = 307,              /* in_row_list  */
  YYSYMBOL_row_expr_list = 308,            /* row_expr_list  */
  YYSYMBOL_case_expr = 309,                /* case_expr  */
  YYSYMBOL_case_list = 310,                /* case_list  */
  YYSYMBOL_exists_expr = 311,              /* exists_expr  */
  YYSYMBOL_comp_expr = 312,                /* comp_expr  */
  YYSYMBOL_table_function_expr = 313,      /* table_function_expr  */
  YYSYMBOL_function_expr = 314,            /* function_expr  */
  YYSYMBOL_opt_window = 315,               /* opt_window  */
  YYSYMBOL_opt_partition = 316,            /* opt_partition  */
  YYSYMBOL_opt_frame_clause = 317,         /* opt_frame_clause  */
  YYSYMBOL_frame_type = 318,               /* frame_type  */
  YYSYMBOL_frame_bound = 319,              /* frame_bound  */
  YYSYMBOL_extract_expr = 320,             /* extract_expr  */
  YYSYMBOL_next_value_expr = 321,          /* next_value_expr  */
  YYSYMBOL_cast_expr = 322,                /* cast_expr  */
  YYSYMBOL_datetime_field = 323,           /* datetime_field  */
  YYSYMBOL_datetime_field_plural = 324,    /* datetime_field_plural  */
  YYSYMBOL_duration_field = 325,           /* duration_field  */
  YYSYMBOL_interval_range = 326,           /* interval_range  */
  YYSYMBOL_array_expr = 327,               /* array_expr  */
  YYSYMBOL_array_index = 328,              /* array_index  */
  YYSYMBOL_between_expr = 329,             /* between_expr  */
  YYSYMBOL_column_name = 330,              /* column_name  */
  YYSYMBOL_nonreserved_keyword = 331,      /* nonreserved_keyword  */
  YYSYMBOL_name_after_qualifier = 332,     /* name_after_qualifier  */
  YYSYMBOL_qualified_name_or_keyword = 333, /* qualified_name_or_keyword  */
  YYSYMBOL_name_or_keyword = 334,          /* name_or_keyword  */
  YYSYMBOL_literal = 335,                  /* literal  */
  YYSYMBOL_string_literal = 336,           /* string_literal  */
  YYSYMBOL_bool_literal = 337,             /* bool_literal  */
  YYSYMBOL_num_literal = 338,              /* num_literal  */
  YYSYMBOL_int_literal = 339,              /* int_literal  */
  YYSYMBOL_null_literal = 340,             /* null_literal  */
  YYSYMBOL_date_literal = 341,             /* date_literal  */
  YYSYMBOL_time_literal = 342,             /* time_literal  */
  YYSYMBOL_timestamp_literal = 343,        /* timestamp_literal  */
  YYSYMBOL_interval_literal = 344,         /* interval_literal  */
  YYSYMBOL_param_expr = 345,               /* param_expr  */
  YYSYMBOL_table_ref = 346,                /* table_ref  */
  YYSYMBOL_table_ref_atomic = 347,         /* table_ref_atomic  */
  YYSYMBOL_nonjoin_table_ref_atomic = 348, /* nonjoin_table_ref_atomic  */
  YYSYMBOL_table_value_row_list = 349,     /* table_value_row_list  */
  YYSYMBOL_table_ref_commalist = 350,      /* table_ref_commalist  */
  YYSYMBOL_table_ref_name = 351,           /* table_ref_name  */
  YYSYMBOL_table_ref_name_no_alias = 352,  /* table_ref_name_no_alias  */
  YYSYMBOL_table_name = 353,               /* table_name  */
  YYSYMBOL_opt_index_name = 354,           /* opt_index_name  */
  YYSYMBOL_table_alias = 355,              /* table_alias  */
  YYSYMBOL_opt_table_alias = 356,          /* opt_table_alias  */
  YYSYMBOL_alias = 357,                    /* alias  */
  YYSYMBOL_opt_alias = 358,                /* opt_alias  */
  YYSYMBOL_opt_locking_clause = 359,       /* opt_locking_clause  */
  YYSYMBOL_opt_locking_clause_list = 360,  /* opt_locking_clause_list  */
  YYSYMBOL_locking_clause = 361,           /* locking_clause  */
  YYSYMBOL_row_lock_mode = 362,            /* row_lock_mode  */
  YYSYMBOL_opt_row_lock_policy = 363,      /* opt_row_lock_policy  */
  YYSYMBOL_with_clause = 364,              /* with_clause  */
  YYSYMBOL_with_description_list = 365,    /* with_description_list  */
  YYSYMBOL_with_description = 366,         /* with_description  */
  YYSYMBOL_join_clause = 367,              /* join_clause  */
  YYSYMBOL_opt_join_type = 368,            /* opt_join_type  */
  YYSYMBOL_join_condition = 369,           /* join_condition  */
  YYSYMBOL_opt_semicolon = 370,            /* opt_semicolon  */
  YYSYMBOL_ident_commalist = 371           /* ident_commalist  */
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
#define YYFINAL  111
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2731

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  218
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  154
/* YYNRULES -- Number of rules.  */
#define YYNRULES  452
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  823

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   453


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
       2,     2,     2,     2,     2,     2,     2,   204,     2,     2,
     210,   211,   202,   200,   214,   201,   212,   203,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   213,
     193,   190,   194,   217,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   207,     2,   208,   205,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   215,     2,   216,     2,     2,     2,     2,
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
     185,   186,   187,   188,   189,   191,   192,   195,   196,   197,
     198,   199,   206,   209
};

#if HSQL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   509,   509,   529,   535,   542,   546,   550,   551,   552,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   570,   571,   573,   577,   582,   586,   596,   597,   598,
     600,   600,   606,   612,   618,   627,   628,   630,   631,   633,
     645,   651,   668,   683,   685,   686,   687,   689,   703,   707,
     717,   721,   745,   753,   766,   773,   788,   808,   809,   814,
     825,   840,   852,   859,   867,   876,   877,   879,   883,   888,
     889,   891,   899,   900,   901,   902,   903,   904,   905,   909,
     910,   911,   912,   913,   914,   915,   916,   917,   918,   919,
     920,   921,   922,   924,   925,   927,   928,   929,   931,   932,
     934,   938,   942,   947,   955,   956,   957,   958,   960,   961,
     962,   964,   972,   978,   984,   990,   996,   997,  1004,  1010,
    1012,  1022,  1029,  1040,  1050,  1056,  1064,  1065,  1072,  1079,
    1083,  1088,  1103,  1112,  1131,  1132,  1145,  1146,  1159,  1163,
    1165,  1166,  1172,  1174,  1179,  1185,  1191,  1192,  1196,  1197,
    1198,  1199,  1200,  1204,  1204,  1206,  1218,  1219,  1221,  1223,
    1224,  1226,  1228,  1229,  1231,  1236,  1238,  1239,  1244,  1245,
    1247,  1248,  1250,  1254,  1259,  1261,  1262,  1263,  1265,  1266,
    1290,  1291,  1292,  1294,  1295,  1296,  1297,  1298,  1299,  1300,
    1305,  1309,  1315,  1316,  1318,  1322,  1327,  1327,  1331,  1332,
    1333,  1335,  1344,  1344,  1344,  1344,  1344,  1346,  1347,  1347,
    1347,  1347,  1347,  1347,  1347,  1347,  1347,  1348,  1348,  1352,
    1352,  1356,  1358,  1360,  1361,  1362,  1363,  1364,  1366,  1366,
    1367,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,  1377,
    1378,  1380,  1381,  1382,  1383,  1387,  1388,  1390,  1391,  1395,
    1399,  1406,  1411,  1418,  1419,  1420,  1421,  1423,  1424,  1426,
    1427,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1439,  1442,
    1445,  1448,  1453,  1456,  1463,  1471,  1472,  1474,  1475,  1480,
    1481,  1482,  1486,  1487,  1488,  1490,  1491,  1492,  1493,  1494,
    1496,  1501,  1507,  1509,  1510,  1511,  1512,  1513,  1514,  1516,
    1517,  1518,  1519,  1520,  1521,  1523,  1523,  1529,  1552,  1558,
    1563,  1565,  1566,  1567,  1568,  1569,  1578,  1579,  1582,  1586,
    1592,  1593,  1594,  1602,  1603,  1604,  1605,  1606,  1607,  1608,
    1609,  1610,  1611,  1612,  1613,  1614,  1615,  1616,  1617,  1618,
    1619,  1620,  1621,  1622,  1629,  1630,  1631,  1632,  1635,  1635,
    1640,  1640,  1642,  1642,  1642,  1642,  1642,  1642,  1642,  1643,
    1643,  1645,  1647,  1648,  1650,  1651,  1653,  1655,  1657,  1669,
    1670,  1676,  1677,  1678,  1679,  1688,  1689,  1690,  1691,  1700,
    1701,  1714,  1715,  1747,  1752,  1764,  1773,  1773,  1780,  1780,
    1784,  1786,  1786,  1794,  1804,  1813,  1817,  1822,  1826,  1831,
    1839,  1849,  1853,  1857,  1861,  1865,  1873,  1874,  1876,  1876,
    1878,  1878,  1880,  1884,  1888,  1889,  1891,  1891,  1897,  1898,
    1900,  1904,  1909,  1915,  1922,  1923,  1924,  1925,  1927,  1928,
    1929,  1935,  1937,  1941,  1946,  1964,  1971,  1979,  1988,  1989,
    1990,  1991,  1992,  1993,  1994,  1995,  1996,  1997,  1999,  2005,
    2005,  2008,  2012
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
  "GROUPS", "UNBOUNDED", "FOLLOWING", "PRECEDING", "CURRENT_ROW",
  "CURRENT_DATE", "CURRENT_TIME", "CURRENT_TIMESTAMP", "FETCH", "NEXT",
  "ONLY", "UNIQUE", "PRIMARY", "FOREIGN", "KEY", "REFERENCES", "OUTERJOIN",
  "PRIOR", "ODBC_OJ", "NEXT_VALUE_FOR", "START_WITH", "CONNECT_BY",
  "CONNECT_BY_NOCYCLE", "WITHIN_GROUP", "'='", "EQUALS", "NOTEQUALS",
  "'<'", "'>'", "LESS", "GREATER", "LESSEQ", "GREATEREQ", "NOTNULL", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'^'", "UMINUS", "'['", "']'",
  "SUBQUERY_AS_EXPR", "'('", "')'", "'.'", "';'", "','", "'{'", "'}'",
  "'?'", "$accept", "input", "statement_list", "statement",
  "preparable_statement", "opt_hints", "hint_list", "hint",
  "transaction_statement", "opt_transaction_keyword", "prepare_statement",
  "prepare_target_query", "odbc_call_statement", "opt_odbc_return",
  "opt_call_args", "execute_statement", "import_statement", "file_type",
  "file_path", "opt_import_export_options", "import_export_options",
  "csv_option", "export_statement", "show_statement", "create_statement",
  "opt_not_exists", "table_elem_commalist", "table_elem", "column_def",
  "column_type", "opt_time_precision", "opt_decimal_specification",
  "opt_column_constraints", "column_constraints", "column_constraint",
  "table_constraint", "references_spec", "drop_statement", "opt_exists",
  "alter_statement", "alter_action", "drop_action", "delete_statement",
  "truncate_statement", "insert_statement", "opt_column_list",
  "update_statement", "update_clause_commalist", "update_clause",
  "query_expression", "query_expression_body", "query_term",
  "query_member", "query_expression_parens", "subquery", "union_operator",
  "intersect_operator", "opt_all", "opt_hierarchical_clause", "connect_by",
  "query_primary", "opt_distinct", "select_list", "opt_from_clause",
  "from_clause", "opt_where", "opt_group", "opt_having",
  "opt_within_group", "opt_order", "order_list", "order_desc",
  "opt_order_type", "opt_null_ordering", "opt_top", "opt_limit",
  "expr_list", "opt_extended_literal_list", "extended_literal_list",
  "casted_extended_literal", "extended_literal", "expr_alias", "expr",
  "operand", "scalar_expr", "unary_expr", "binary_expr", "logic_expr",
  "in_expr", "in_row_list", "row_expr_list", "case_expr", "case_list",
  "exists_expr", "comp_expr", "table_function_expr", "function_expr",
  "opt_window", "opt_partition", "opt_frame_clause", "frame_type",
  "frame_bound", "extract_expr", "next_value_expr", "cast_expr",
  "datetime_field", "datetime_field_plural", "duration_field",
  "interval_range", "array_expr", "array_index", "between_expr",
  "column_name", "nonreserved_keyword", "name_after_qualifier",
  "qualified_name_or_keyword", "name_or_keyword", "literal",
  "string_literal", "bool_literal", "num_literal", "int_literal",
  "null_literal", "date_literal", "time_literal", "timestamp_literal",
  "interval_literal", "param_expr", "table_ref", "table_ref_atomic",
  "nonjoin_table_ref_atomic", "table_value_row_list",
  "table_ref_commalist", "table_ref_name", "table_ref_name_no_alias",
  "table_name", "opt_index_name", "table_alias", "opt_table_alias",
  "alias", "opt_alias", "opt_locking_clause", "opt_locking_clause_list",
  "locking_clause", "row_lock_mode", "opt_row_lock_policy", "with_clause",
  "with_description_list", "with_description", "join_clause",
  "opt_join_type", "join_condition", "opt_semicolon", "ident_commalist", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-724)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-448)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1640,    84,  2298,   130,   148,  2298,    14,    82,    99,   167,
      95,  2298,   177,  1629,    32,   161,  2363,    88,    88,    88,
     -13,    16,   248,    74,  -724,   186,  -724,   186,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,   139,   295,  -724,  -724,  -724,   -20,   351,   228,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
     243,  -724,  -724,   192,   336,  -724,   333,   333,   333,  2298,
     480,  2298,    20,   484,   398,  -724,   374,  -724,   376,    11,
     374,   374,   374,  2298,  -724,  -724,  -724,   402,   320,  -724,
    -724,  -724,  -724,  -724,   327,   328,  -724,  -724,  -724,   454,
     354,  -724,  1640,  -724,   447,  -724,  -724,   433,   424,   433,
     -20,    44,   433,   -20,   139,  -724,  2125,  2125,   441,  -724,
     562,   449,  2363,  2298,  2298,   492,  -724,   486,   -12,  -724,
    1140,  -724,  -724,  1140,  2363,   531,  2298,   584,   584,   584,
    2363,  2298,  2298,  -724,   380,  2363,  -724,  -724,  2298,  -724,
    -724,   388,  -724,  -724,  1140,  -724,   295,  1140,   830,   481,
    -724,  -724,    44,  -724,  -724,  -724,  -724,  -724,   391,  -724,
    -724,   393,  -724,  -724,   414,   602,   397,   611,  -724,   612,
     614,  -724,  -724,   409,   410,   411,    56,   412,   408,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,   577,  -724,   493,   -29,   418,  1140,
    -724,   584,  1466,   564,  -724,    46,   602,   380,   423,   425,
     985,  1295,   427,  1450,  2298,  1450,  -724,   675,  -724,   -50,
    1851,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,   448,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,   456,   426,  -724,
     544,   432,  -724,   159,   -47,  -724,   457,  -724,   555,  -724,
     -56,   -56,   -56,  -724,  -724,  -724,  -724,  -724,   192,   646,
     436,  -724,   247,   134,   594,    42,     5,  -724,   481,  -724,
     481,  2125,  2125,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,   702,
    -724,  -724,   442,   444,    27,    28,  -724,  -724,  -724,   441,
    -724,  2298,   649,   -13,  2060,  2363,   526,   166,   556,  -724,
     121,   451,     8,  1793,  -724,   442,  1140,  1140,   227,   188,
     380,  1450,   675,  2357,  1140,   450,  -724,   190,    38,   -42,
     135,  1140,  1140,  -724,  1450,  -724,  1450,   -30,   452,   -17,
    1450,  1450,  1450,  1450,  1450,  1450,  1450,  1450,  1450,  1450,
    1450,  1450,  1450,  1450,  1450,   653,   453,   585,  -724,    17,
    1617,   492,  -724,  1140,  -724,  2137,  -724,  -724,  2363,  -724,
    1140,   624,  -724,  -724,   458,   163,  -724,   492,  -724,   455,
     460,   168,  -724,  1140,  -724,  -724,   663,   498,  1140,  1140,
    -724,   487,  -724,   510,   113,  -724,  -724,  -724,  -724,   547,
     541,  -724,  -724,   466,  -724,   476,  -724,  -724,   479,   596,
    -724,   483,   514,   517,   187,  -724,  -724,  -724,  2576,   219,
     -13,  2298,   -13,  2363,   441,  -724,  1140,  -724,  -724,   102,
     604,   269,   241,   256,  1140,  1140,  -724,  -724,   -46,   595,
    -121,  -724,  1140,    54,  1140,  -724,   583,  2265,  2394,  1450,
     494,   675,  -724,  -724,   599,   450,  2394,  2394,  2394,  2394,
     554,   554,   554,   554,   190,   190,   -96,   -96,   -96,   -28,
     503,   635,   504,  -724,  -724,  1140,   162,  1617,     0,    67,
      67,   197,  -724,   287,  -724,   505,  -724,    67,  -724,   292,
    -724,  -724,  -724,  -724,  -724,   166,   374,   163,   713,   714,
     480,   716,   220,  -724,  -724,  -724,   441,  -724,   646,  -724,
     718,  -724,   552,   166,   166,   658,  -724,  2363,  -724,   566,
    -724,  2576,   442,  -724,  -724,  2363,   584,  2363,   520,   523,
    -724,  2060,  -724,  -724,   524,  -724,   527,  -724,  -724,   528,
    -724,  -724,   529,  -724,  -724,  -724,  -724,   530,  -724,   442,
    -724,   140,  -724,  -724,  -724,  -724,  -724,   239,   249,    18,
    1858,  1140,  2576,  1140,  1140,  -724,   193,   246,  -724,   166,
     606,   532,   166,  1450,  2394,   675,  -724,   250,  -724,  -724,
     613,   636,   255,  2125,   174,   534,  2286,  -724,  -724,  -724,
    -724,   645,  -724,  -724,  -724,   669,   670,   673,   659,  1617,
    -724,  1140,  -724,  -724,   693,  1140,  2363,   260,  -724,  -724,
    -724,  -724,  -724,   210,   273,  -724,  -724,  1140,  -724,   -78,
    -724,   557,  -724,   274,  -724,   278,  2363,  2363,  -724,   761,
     763,  -724,   764,   766,   768,  -724,   541,  -724,  -724,   671,
    -724,   598,  2298,  -724,   140,  -724,  -724,  -724,  -724,  -724,
    1140,  -724,  -724,   563,   -40,   565,   166,   309,  -724,  1140,
     569,   -10,  -724,  2357,   279,  -724,  1140,   652,   704,  -724,
     226,  -724,  1140,   285,   573,   574,   575,  1778,  -724,  -724,
    -724,  1617,   440,   146,   660,  -724,   144,  -724,  -724,   782,
     480,  -724,  -724,   206,  -724,  -724,  -724,  -724,   286,   291,
     580,   293,   582,   588,   589,  -724,  -724,   418,  -724,  -724,
     298,  1959,  -724,  -724,  -724,   166,   -10,  -724,   675,   313,
    -724,   314,  1140,   352,   337,    67,   586,  2363,  -724,   223,
    1140,  1140,  1140,  -724,  -724,   618,  -724,   620,  -724,  -724,
     789,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,   356,
     -44,   360,  -724,   593,  -724,   432,  -724,  -724,  -724,   600,
      39,  -724,  -724,  1140,   362,   597,  1140,   166,    -9,   166,
    -724,  -724,   619,  -724,  -724,  1140,  -724,   126,    45,   231,
    -724,  -724,   378,  -724,  2363,   166,  -724,  1140,  -724,  -724,
     385,  -724,  -724,   686,  -724,  -724,  -724,   386,   166,  -724,
      45,  -724,  -724
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     182,     0,     0,     0,     0,     0,     0,    31,    31,    31,
       0,    36,     0,   450,     3,    22,    20,    22,    19,    18,
       8,     9,     7,    11,    16,    17,    13,    14,    12,    15,
      10,   171,   134,   136,   139,   138,     0,     0,   350,   342,
     340,   338,   336,   335,   337,   341,   339,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   343,
     351,   401,   122,    38,     0,    59,    66,    66,    66,     0,
       0,     0,     0,   157,     0,   400,   117,   142,     0,     0,
     117,   117,   117,     0,    57,   350,   351,     0,   431,   432,
      30,    27,    29,    28,     0,   139,   384,   385,   383,     0,
       0,     1,   449,     2,     0,     6,     5,   147,     0,   147,
       0,   189,   147,     0,   171,   114,     0,     0,   193,    39,
       0,     0,   407,     0,     0,   163,    42,     0,   127,   366,
       0,   180,   156,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,   140,   141,     0,    35,
       4,     0,   146,   144,     0,   143,   135,     0,     0,   419,
     145,   137,   189,   344,   345,   346,   347,   349,   348,   402,
     348,   403,   361,   364,   366,     0,     0,     0,   367,     0,
       0,   362,   363,   369,   372,   376,     0,     0,   192,   194,
     196,   198,   352,   353,   354,   365,   355,   356,   357,   358,
     359,   360,    33,    32,     0,   406,     0,     0,   127,     0,
     121,     0,     0,     0,   124,   311,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,   218,     0,
     202,   209,   210,   211,   204,   206,   212,   205,   228,   169,
     213,   214,   216,   215,   217,   208,   203,   219,   315,   220,
     160,   158,   190,   417,   163,   129,     0,   116,     0,    43,
      46,    46,    46,   115,   112,   113,   434,   433,    38,     0,
     170,   172,   177,   184,   186,   183,     0,   132,   418,   420,
     419,     0,     0,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   379,   375,     0,
     368,   371,   382,     0,     0,     0,   199,   200,    37,     0,
      65,     0,     0,     0,     0,     0,     0,   162,     0,   451,
       0,     0,   157,     0,   259,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,   223,   291,   222,   142,     0,
       0,     0,     0,   181,     0,   225,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   276,   221,   157,
       0,   163,   159,     0,   415,     0,   416,   201,     0,   128,
       0,     0,   118,   119,     0,     0,    56,   163,    55,     0,
      25,     0,    23,     0,   176,   175,   178,     0,     0,     0,
     424,     0,   426,     0,   430,   421,   133,   405,   404,     0,
     380,   381,   370,     0,   373,     0,   377,   195,     0,     0,
      62,     0,     0,     0,     0,    67,    69,    70,     0,     0,
       0,     0,   126,     0,     0,   268,     0,   314,   320,   312,
       0,     0,     0,     0,     0,     0,   255,   260,     0,     0,
       0,   207,     0,     0,     0,   239,   240,     0,   235,     0,
       0,     0,   243,   226,     0,   238,   237,   261,   262,   263,
     264,   265,   266,   267,   230,   229,   232,   231,   233,   234,
       0,     0,     0,   274,   272,     0,   350,     0,     0,   411,
     411,   351,   161,   386,   388,     0,   391,   411,   389,   152,
     191,   412,   414,   413,   130,   131,   117,     0,     0,     0,
       0,     0,     0,    52,    41,    34,     0,    21,     0,   173,
       0,   174,     0,   187,   185,     0,   427,     0,   429,     0,
     422,     0,     0,   374,   378,     0,     0,     0,     0,     0,
      61,     0,    91,    82,     0,    87,    97,    88,    72,     0,
      79,    80,     0,    76,    77,    86,    89,    94,    81,    83,
      73,    99,   126,    64,    40,   125,   452,     0,     0,   157,
       0,     0,     0,     0,     0,   253,     0,     0,   308,   251,
       0,     0,   252,     0,   236,     0,   244,     0,   227,   309,
       0,   278,     0,     0,   447,     0,     0,   410,   392,   408,
     393,     0,   446,   438,   444,   442,   445,   440,     0,     0,
     399,     0,   153,   154,   165,     0,     0,     0,    53,    50,
      48,    54,    45,     0,     0,    24,   179,     0,   425,   430,
     428,     0,   307,     0,    60,     0,     0,     0,    68,     0,
       0,    78,     0,     0,     0,    90,    84,    85,   106,     0,
     105,     0,     0,    71,    98,   100,   102,   123,   269,   270,
       0,   318,   321,   316,     0,     0,   257,     0,   256,     0,
       0,     0,   245,   310,     0,   241,     0,     0,   171,   273,
     348,   390,     0,     0,   412,   413,     0,     0,   441,   443,
     439,     0,   387,   150,     0,   155,   151,   120,    44,     0,
       0,    51,    26,     0,   423,   197,    63,   109,     0,     0,
       0,     0,     0,     0,     0,   107,   104,   127,   101,   103,
       0,     0,   290,   292,   254,   258,     0,   246,     0,     0,
     242,     0,     0,   281,     0,   411,     0,     0,   435,   447,
       0,     0,     0,    49,    47,     0,   108,     0,    75,    96,
       0,    92,    74,    93,   111,   271,   319,   322,   317,     0,
       0,     0,   247,     0,   168,   277,   282,   283,   284,     0,
       0,   395,   394,     0,     0,     0,     0,   148,   167,   149,
     188,   110,     0,   248,   249,     0,   275,     0,     0,     0,
     289,   279,     0,   409,     0,   448,   436,     0,   164,    95,
       0,   288,   286,     0,   287,   285,   396,     0,   166,   250,
       0,   437,   280
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -724,  -724,  -724,   694,  -724,   778,  -724,   281,  -724,   407,
    -724,  -724,  -724,  -724,   535,  -724,  -724,  -492,  -141,   180,
     315,   198,  -724,  -724,  -724,   379,  -724,   259,  -724,  -358,
    -724,  -724,  -724,  -724,   175,  -724,  -600,  -724,   -74,  -724,
    -724,  -724,  -724,  -724,  -724,  -212,  -724,  -724,   461,    25,
     794,   721,   719,    35,   -11,  -724,  -724,   131,  -724,   141,
    -724,  -313,  -724,  -724,  -724,  -201,  -724,  -724,  -724,  -119,
     165,   459,  -724,  -724,  -724,   680,  -307,  -724,  -403,   540,
     536,   470,  -111,    50,  -724,  -724,  -724,  -724,  -724,   125,
    -667,  -724,   525,  -724,  -724,  -359,  -724,  -724,  -724,  -724,
    -724,  -723,  -724,  -724,  -724,  -299,  -724,  -724,   296,  -724,
    -724,  -724,  -724,    -2,  -122,   -88,   -15,  -118,  -724,  -724,
     668,   -70,  -724,  -724,  -724,  -724,   672,   845,  -724,  -467,
     172,  -724,  -724,  -724,  -724,    41,  -724,  -724,  -480,   607,
    -724,   581,  -724,   601,  -724,   233,  -724,  -724,   727,  -724,
    -724,  -724,  -724,  -310
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    22,    23,    24,    25,   115,   401,   402,    26,   101,
      27,   213,    28,   109,   129,    29,    30,   137,   270,   396,
     522,   523,    31,    32,    33,   132,   434,   435,   436,   571,
     655,   651,   663,   664,   665,   437,   666,    34,   146,    35,
     392,   393,    36,    37,    38,   223,    39,   264,   265,   104,
      41,    42,    43,    87,   238,   120,   123,   163,   624,   625,
      45,   143,   260,   381,   382,   220,   705,   808,   377,   121,
     280,   281,   406,   531,    83,   169,   261,   197,   198,   199,
     200,   262,   263,   240,   241,   242,   243,   244,   245,   739,
     350,   246,   339,   247,   248,   249,   250,   493,   688,   779,
     780,   801,   251,   252,   253,   305,   306,   307,   421,   254,
     255,   256,   257,   258,   180,   179,    71,   259,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   502,   503,
     504,   693,   505,   506,    84,   507,   216,   607,   608,   609,
     387,   287,   288,   289,   414,   540,    46,    98,    99,   508,
     618,   806,   113,   330
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      70,    97,    88,    70,   178,   172,   326,   271,   272,    70,
     201,    70,   141,   420,    96,   439,   150,   151,   152,   446,
     610,   500,   142,   106,   107,    40,   139,   620,   630,   239,
     604,   142,   142,   423,   425,    44,   450,   460,   219,   181,
      10,   577,   360,    72,   807,   797,    75,    10,    10,  -139,
      10,   797,    85,   282,    89,   105,   283,   285,   798,   394,
      10,   183,   184,   389,   729,   605,   495,   322,   351,   410,
     384,   771,   351,   469,   351,   813,   351,    70,   351,    70,
     328,    44,   538,   539,   352,  -139,    76,   588,   352,   473,
     352,    70,   352,   383,   352,  -139,    77,   822,   323,   409,
     470,   167,    16,    16,    90,    16,   474,   148,   327,   374,
     360,   375,  -139,    47,    91,    16,  -139,   215,   168,   338,
     135,  -139,   138,   634,   177,   177,   349,    78,   810,   266,
      96,    70,    70,    73,   153,   273,   443,    40,   500,   578,
      97,   411,    96,   276,    70,    92,   149,    44,    96,    70,
      70,    74,   702,    96,   395,    44,    70,   791,    44,  -139,
     351,   353,   384,   224,   597,   461,   518,   388,   412,   461,
     462,   732,   462,    44,   217,   218,   352,   590,    79,   375,
     509,   324,    93,   641,   591,   413,   117,   268,   602,   519,
      20,   201,   274,   275,   606,    80,   524,    20,   222,   278,
     738,   201,   611,   417,   418,   383,   799,   329,   190,   800,
      20,   449,   799,   518,   520,   800,   334,   118,   754,   445,
      96,    82,   119,    94,   675,   451,   452,   639,   494,   669,
     140,   458,    70,   108,   749,   643,   709,   645,   424,   426,
     465,   466,   612,   642,   100,   537,   658,   613,   111,   157,
     165,   611,   351,   170,   614,   615,   332,   105,   333,    86,
     500,   710,   351,   659,   351,   782,   670,    81,   352,   521,
     656,   616,   348,   538,   539,   346,   617,   351,   352,   515,
     352,   343,   454,   345,   351,   347,   385,   112,   684,   177,
     177,   612,   282,   352,   811,   812,   613,   533,   534,   407,
     352,   114,   455,   614,   615,   490,   122,   785,   456,   438,
     329,   351,   579,   678,   580,   611,   521,   660,   661,    70,
     616,   662,    96,    96,   351,   617,   201,   352,   360,   457,
     752,   448,   442,   622,   623,   443,   718,   719,   500,   404,
     352,   337,   500,   586,   587,   351,   463,   472,   430,   464,
     584,   589,   583,   592,   125,   612,   786,   679,    44,   351,
     613,   352,   428,   730,   351,   351,   405,   614,   615,   499,
     455,   755,   332,   266,   603,   352,   585,   348,   501,   527,
     352,   352,   528,   513,   616,   744,    96,   351,  -447,   617,
     691,   343,   371,   372,   373,   374,   582,   375,   550,   814,
     815,   551,   128,   352,   467,   644,   468,   379,   201,   127,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   102,   103,   351,   576,   734,
     572,   632,   130,   443,   633,   775,   579,   784,   291,    70,
     126,    96,   626,   352,   788,   182,   183,   184,   106,   107,
     667,   397,   398,   319,   185,   127,   133,   134,   673,   596,
     668,   685,   131,   383,   383,   573,   689,   575,   611,   383,
     674,   708,   676,   677,   633,    44,   802,    44,   621,   622,
     623,   690,   574,   136,   712,   716,   499,   319,   443,   717,
     740,   186,   443,   383,   817,   501,   745,   756,   142,   746,
     443,  -397,   757,   145,   759,   443,   348,   760,   612,   765,
     703,   147,   383,   613,   706,   764,   776,   777,   778,   594,
     614,   615,   329,   144,   772,   774,   713,   773,   403,   154,
     329,   187,   329,   105,   155,    96,   438,   616,   156,   157,
     158,  -447,   617,    96,   159,    96,   161,   188,   781,    96,
     162,   383,   164,   189,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   212,   793,   735,   743,
     773,   794,   214,   803,   464,   282,   443,   219,   672,   267,
     682,   221,   293,   294,   295,   296,   297,   298,   269,   816,
      20,   696,   383,   190,   191,   192,   819,   821,   279,   464,
     443,   177,   286,   291,   695,   292,   308,   309,   499,   768,
     355,   707,   193,   194,   195,   310,   311,   501,   312,   313,
     314,   315,   319,   318,    96,   320,   321,   770,   325,   331,
     348,   329,   329,   335,   344,   336,   379,   376,   378,   787,
     380,   789,   196,   683,    96,    96,   383,   390,   391,   400,
     403,   408,   429,   440,  -398,   422,   441,   375,   108,   139,
      70,   444,   471,   491,   492,   516,   530,   535,   517,   737,
     526,   525,   532,   536,   541,   805,   542,   543,   225,   182,
     183,   184,   106,   107,   770,   359,   499,   544,   226,   545,
     499,   546,   360,   547,   548,   501,   818,   549,   469,   501,
     581,   351,    50,   727,   595,   598,   182,   183,   184,   106,
     107,   599,    51,   600,   601,   185,   105,   628,   629,   619,
     631,   636,   638,   227,   228,   229,    52,   637,   640,   767,
     646,    53,   329,   647,   649,    10,   680,   650,   652,   653,
     654,   686,   681,   687,   692,    96,   697,  -448,  -448,   698,
     699,  -448,  -448,   700,   369,   370,   371,   372,   373,   374,
     701,   375,   230,    54,   704,   187,    55,   720,   715,   721,
     722,   105,   723,   348,   724,   731,   733,   725,   726,   736,
     742,   188,   118,  -350,  -351,   747,   753,   189,   751,   329,
      16,   758,   187,   761,   790,   792,   783,    56,   231,   762,
     763,   662,    96,   795,   820,   116,   160,   804,   188,   635,
     648,   796,   232,   399,   189,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,   190,   191,   192,
     809,   711,   627,   225,   182,   183,   184,   106,   107,   728,
     124,   166,   171,   226,   750,   419,   193,   194,   195,   514,
      69,   741,   290,   510,   190,   191,   192,    50,   233,   427,
     234,   769,   529,   453,   316,   657,   110,    51,   317,   748,
     386,   416,   714,   193,   194,   195,   235,   236,   227,   228,
     229,    52,   277,     0,     0,   237,    53,     0,     0,   415,
       0,     0,   108,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   196,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,    54,   108,
     187,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   188,     0,     0,     0,
       0,     0,   189,     0,     0,     0,     0,   284,     0,     0,
       0,     0,    56,   231,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,   190,   191,   192,     0,     0,     0,   225,   182,
     183,   184,   106,   107,     0,     0,     0,     0,   226,     0,
       0,   193,   194,   195,     0,    69,     0,     0,     0,     0,
       0,     0,    50,   233,     0,   234,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,   235,   236,   227,   228,   229,    52,     0,     0,     0,
     237,    53,     0,     0,     0,     0,     0,   108,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,    54,     0,   187,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,     0,     0,     0,     0,     0,   189,     0,   337,
       0,     0,     0,     0,     0,     0,     0,    56,   231,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,   190,   191,   192,
       0,     0,     0,   225,   182,   183,   184,   106,   107,     0,
       0,     0,     0,   226,     0,     0,   193,   194,   195,     0,
      69,     0,     0,     0,     0,     0,     0,    50,   233,     0,
     234,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,   235,   236,   227,   228,
     229,    52,     0,     0,     0,   237,    53,     0,     0,     0,
       0,     0,   108,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,    54,     0,
     187,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   188,     0,     0,     0,
       0,     0,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,   231,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,   190,   191,   192,     0,     0,     0,   225,   182,
     183,   184,   106,   107,     0,     0,     0,     0,   226,     0,
       0,   193,   194,   195,     0,    69,     0,     0,     0,     0,
       0,     0,    50,   233,     0,   234,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,   235,   236,   340,   228,   229,    52,     0,     0,     0,
     237,    53,     0,     0,     0,     0,     0,   108,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,    54,     0,   187,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,     0,     0,     0,     0,     0,   189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,   190,   191,   192,
       0,     0,     0,   225,   182,   183,   184,   106,   107,     0,
       0,     0,     0,   226,     0,     0,   193,   194,   195,    95,
      69,     0,     0,     0,     0,     0,     0,    50,   233,    49,
     234,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,    50,     0,     0,   235,   236,     0,   228,
     229,    52,     0,    51,     0,   342,    53,     0,     0,     0,
       0,     0,   108,     0,     0,     0,     0,    52,     0,     0,
       0,     0,    53,     0,     0,     0,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,    54,     0,
     187,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,   188,    55,     0,     0,
       0,     0,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,   341,     0,     0,     0,     0,     0,     0,
       0,    16,     0,     0,     0,     0,     0,   232,    56,     0,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,   190,   191,   192,     0,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,     0,     0,
     496,   193,   194,   195,     0,    69,     0,     0,     0,     0,
      49,     0,    48,   233,     0,   234,     0,     0,     0,     0,
       0,    69,    49,     0,    50,     0,     0,     0,     0,     1,
       0,   235,   236,     0,    51,     0,    50,     2,     0,     0,
     342,     0,     0,     0,     3,     0,    51,   108,    52,     4,
       0,     0,     0,    53,     0,     0,    20,     0,     0,     5,
      52,     0,     6,     7,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     8,     9,     0,     0,     0,     0,
      10,     0,     0,     0,    11,    54,     0,    12,    55,     0,
       0,     0,     0,     0,     0,     0,     0,    54,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,    13,
       0,     0,     0,    14,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    15,
       0,    56,     0,     0,     0,    16,     0,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    69,     0,     0,     0,   173,    17,    18,    19,
       0,   497,     0,     0,    69,    50,    49,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,   498,     0,    52,
      51,     0,     0,     0,    53,     0,     0,     0,     0,    20,
       0,     0,     0,     0,    52,     0,     0,     0,     0,    53,
      20,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   173,     0,     0,     0,     0,    54,     0,     0,    55,
     354,    49,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,    55,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,   355,     0,    52,
       0,     0,     0,     0,    53,    56,     0,     0,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
       0,     0,     0,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    54,     0,     0,    55,
       0,     0,     0,    69,   356,     0,     0,     0,     0,     0,
       0,     0,   173,     0,   174,   175,   176,     0,    69,     0,
       0,     0,    49,     0,   357,     0,     0,     0,     0,     0,
      56,   358,   359,     0,     0,     0,    50,     0,   498,   360,
     361,     0,     0,     0,     0,   447,    51,     0,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   174,
     175,   176,     0,    69,     0,     0,     0,     0,     0,     0,
       0,   362,   363,   364,   365,   366,     0,    54,   367,   368,
      55,   369,   370,   371,   372,   373,   374,     0,   375,     0,
     671,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    52,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   173,     0,
     174,   175,   176,     0,    69,     0,     0,     0,    49,     0,
     511,   512,     0,     0,     0,     0,     0,     0,    54,     0,
      49,    55,    50,     0,     0,     0,     0,     0,     0,     0,
       0,   766,    51,     0,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,    52,     0,     0,     0,
       0,    53,    56,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,     0,    54,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,    55,     0,
       0,     0,     0,     0,     0,    69,     0,   431,   432,   433,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,   694,
     512,     0,     0,     0,     0,     0,   174,   175,   176,    49,
      69,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    69,    50,     0,     0,     0,     0,     0,     0,
       0,   355,     0,    51,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,    52,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,   356,     0,
       0,     0,     0,     0,    54,     0,    49,    55,     0,     0,
       0,     0,     0,   593,     0,     0,    54,     0,   459,    55,
      50,     0,     0,     0,     0,     0,   359,     0,     0,     0,
      51,     0,     0,   360,   361,     0,     0,     0,    56,     0,
       0,     0,     0,   355,    52,     0,     0,     0,     0,    53,
      56,     0,     0,     0,     0,     0,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
     355,    54,     0,     0,    55,   362,   363,   364,   365,   366,
     356,    69,   367,   368,     0,   369,   370,   371,   372,   373,
     374,     0,   375,    69,     0,     0,     0,     0,     0,     0,
     459,     0,     0,     0,     0,    56,     0,     0,   359,     0,
       0,     0,     0,     0,     0,   360,   361,  -448,     0,     0,
       0,     0,     0,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   359,     0,     0,     0,     0,
       0,     0,   360,  -448,     0,     0,     0,     0,    69,     0,
       0,     0,     0,     0,     0,     0,     0,   362,   363,   364,
     365,   366,     0,     0,   367,   368,     0,   369,   370,   371,
     372,   373,   374,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -448,  -448,  -448,   365,   366,   552,
       0,   367,   368,     0,   369,   370,   371,   372,   373,   374,
       0,   375,     0,   553,     0,     0,     0,   554,   555,   556,
     557,   558,     0,   559,     0,     0,     0,     0,     0,     0,
       0,   560,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   562,     0,   563,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,     0,     0,   566,     0,   567,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   568,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   569,     0,
       0,   570
};

static const yytype_int16 yycheck[] =
{
       2,    16,    13,     5,   126,   124,   218,   148,   149,    11,
     128,    13,    82,   312,    16,   325,    90,    91,    92,   332,
     500,   380,    14,     7,     8,     0,     6,   507,   520,   140,
     497,    14,    14,     6,     6,     0,   335,   344,    85,   127,
      60,   444,   138,     2,    53,     6,     5,    60,    60,    11,
      60,     6,    11,   164,    13,    20,   167,   168,    19,   115,
      60,     5,     6,   264,   664,    65,   379,    96,   118,    64,
       3,   738,   118,   103,   118,   798,   118,    79,   118,    81,
     221,    46,   160,   161,   134,    47,    72,   208,   134,   106,
     134,    93,   134,   214,   134,    57,    82,   820,   127,    57,
     130,    57,   115,   115,    72,   115,   123,    96,   219,   205,
     138,   207,    74,    29,    82,   115,    78,   132,    74,   230,
      79,    83,    81,   526,   126,   127,   237,   113,   795,   144,
     132,   133,   134,     3,    93,   150,   214,   112,   497,   446,
     155,   136,   144,   154,   146,   113,   135,   112,   150,   151,
     152,     3,   619,   155,   210,   120,   158,   757,   123,   121,
     118,   211,     3,   138,   471,   211,     3,   214,   163,   211,
     214,   211,   214,   138,   133,   134,   134,   123,    96,   207,
     381,   210,    21,   541,   130,   180,    47,   146,   495,    26,
     210,   309,   151,   152,   127,    96,   397,   210,   210,   158,
     210,   319,    28,   291,   292,   214,   167,   222,   152,   170,
     210,   333,   167,     3,    51,   170,   227,    78,   710,   211,
     222,   126,    83,    62,   582,   336,   337,   537,   211,   211,
     210,   342,   234,   217,   701,   545,    26,   547,   211,   211,
     351,   352,    68,   542,   156,   132,   106,    73,     0,   211,
     119,    28,   118,   122,    80,    81,   210,   222,   212,    82,
     619,    51,   118,   123,   118,   745,   579,   100,   134,   106,
     569,    97,   237,   160,   161,   234,   102,   118,   134,   390,
     134,   231,    94,   233,   118,   235,   127,   213,   595,   291,
     292,    68,   403,   134,   168,   169,    73,   408,   409,   165,
     134,   115,   114,    80,    81,   375,    11,    84,   120,   324,
     325,   118,   210,   120,   212,    28,   106,   177,   178,   321,
      97,   181,   324,   325,   118,   102,   444,   134,   138,   340,
     186,   333,   211,   187,   188,   214,   646,   647,   697,    92,
     134,   114,   701,   454,   455,   118,   211,   358,   323,   214,
      94,   462,   111,   464,     3,    68,   133,   111,   323,   118,
      73,   134,   321,   670,   118,   118,   119,    80,    81,   380,
     114,   165,   210,   388,   212,   134,   120,   342,   380,   211,
     134,   134,   214,   385,    97,   692,   388,   118,   101,   102,
     216,   341,   202,   203,   204,   205,   127,   207,   211,   168,
     169,   214,   210,   134,   354,   546,   356,   210,   526,   212,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,    18,    19,   118,   443,   120,
     211,   211,    96,   214,   214,   742,   210,   747,   212,   441,
     212,   443,   516,   134,   751,     4,     5,     6,     7,     8,
     211,   271,   272,   214,    13,   212,    77,    78,   580,   470,
     211,   211,   129,   214,   214,   440,   211,   442,    28,   214,
     581,   211,   583,   584,   214,   440,   783,   442,   186,   187,
     188,   603,   441,     3,   211,   211,   497,   214,   214,   211,
     211,    50,   214,   214,   804,   497,   211,   211,    14,   214,
     214,   214,   211,   129,   211,   214,   471,   214,    68,   211,
     621,   135,   214,    73,   625,   727,   164,   165,   166,   469,
      80,    81,   537,   125,   211,   211,   637,   214,   214,   127,
     545,    90,   547,   498,   214,   537,   551,    97,   211,   211,
      86,   101,   102,   545,   190,   547,    99,   106,   211,   551,
     117,   214,   128,   112,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,     4,   211,   679,   688,
     214,   211,   123,   211,   214,   686,   214,    85,   580,    48,
     591,    95,   140,   141,   142,   143,   144,   145,     4,   211,
     210,   606,   214,   152,   153,   154,   211,   211,   210,   214,
     214,   603,   121,   212,   606,   212,     4,   210,   619,   731,
      56,   626,   171,   172,   173,     4,     4,   619,     4,   210,
     210,   210,   214,   211,   626,    48,   133,   738,   210,    65,
     595,   646,   647,   210,   207,   210,   210,   189,   182,   750,
      96,   752,   201,   593,   646,   647,   214,   190,    93,     3,
     214,    57,     3,   127,   214,   211,   100,   207,   217,     6,
     662,   210,   210,   210,    79,    41,     3,   180,   210,   680,
     210,   216,   174,   163,   127,   786,   135,   211,     3,     4,
       5,     6,     7,     8,   795,   131,   697,   211,    13,   210,
     701,    95,   138,   210,   180,   697,   807,   180,   103,   701,
      96,   118,    27,   662,   210,   106,     4,     5,     6,     7,
       8,   208,    37,    78,   210,    13,   681,     4,     4,   214,
       4,     3,    64,    48,    49,    50,    51,   175,   162,   731,
     210,    56,   747,   210,   210,    60,   130,   210,   210,   210,
     210,   128,   210,   107,   210,   747,   101,   193,   194,    80,
      80,   197,   198,    80,   200,   201,   202,   203,   204,   205,
     101,   207,    87,    88,    71,    90,    91,     6,   211,     6,
       6,   736,     6,   738,     6,   212,   211,   106,   180,   210,
     128,   106,    78,   210,   210,   210,     4,   112,   128,   804,
     115,   211,    90,   211,   176,     6,   210,   122,   123,   211,
     211,   181,   804,   210,   118,    27,   112,   210,   106,   528,
     551,   211,   137,   278,   112,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     211,   633,   517,     3,     4,     5,     6,     7,     8,   664,
      46,   120,   123,    13,   703,   309,   171,   172,   173,   388,
     175,   686,   172,   383,   152,   153,   154,    27,   183,   319,
     185,   736,   403,   338,   196,   569,    21,    37,   196,   697,
     263,   290,   639,   171,   172,   173,   201,   202,    48,    49,
      50,    51,   155,    -1,    -1,   210,    56,    -1,    -1,   288,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,   217,
      90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    13,    -1,
      -1,   171,   172,   173,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    27,   183,    -1,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    48,    49,    50,    51,    -1,    -1,    -1,
     210,    56,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    13,    -1,    -1,   171,   172,   173,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    27,   183,    -1,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    48,    49,
      50,    51,    -1,    -1,    -1,   210,    56,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    13,    -1,
      -1,   171,   172,   173,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    27,   183,    -1,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    48,    49,    50,    51,    -1,    -1,    -1,
     210,    56,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    13,    -1,    -1,   171,   172,   173,     3,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    27,   183,    13,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,   201,   202,    -1,    49,
      50,    51,    -1,    37,    -1,   210,    56,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,   106,    91,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,   137,   122,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,    -1,    -1,
       3,   171,   172,   173,    -1,   175,    -1,    -1,    -1,    -1,
      13,    -1,     3,   183,    -1,   185,    -1,    -1,    -1,    -1,
      -1,   175,    13,    -1,    27,    -1,    -1,    -1,    -1,     9,
      -1,   201,   202,    -1,    37,    -1,    27,    17,    -1,    -1,
     210,    -1,    -1,    -1,    24,    -1,    37,   217,    51,    29,
      -1,    -1,    -1,    56,    -1,    -1,   210,    -1,    -1,    39,
      51,    -1,    42,    43,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,
      60,    -1,    -1,    -1,    64,    88,    -1,    67,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,   122,    -1,    -1,    -1,   115,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,   175,    -1,    -1,    -1,     3,   157,   158,   159,
      -1,   184,    -1,    -1,   175,    27,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,    51,
      37,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,   210,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,
     210,    -1,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    88,    -1,    -1,    91,
      19,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    91,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    51,
      -1,    -1,    -1,    -1,    56,   122,    -1,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    88,    -1,    -1,    91,
      -1,    -1,    -1,   175,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    -1,   171,   172,   173,    -1,   175,    -1,
      -1,    -1,    13,    -1,   123,    -1,    -1,    -1,    -1,    -1,
     122,   130,   131,    -1,    -1,    -1,    27,    -1,   210,   138,
     139,    -1,    -1,    -1,    -1,   202,    37,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,
     172,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   191,   192,   193,   194,    -1,    88,   197,   198,
      91,   200,   201,   202,   203,   204,   205,    -1,   207,    -1,
     202,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
     171,   172,   173,    -1,   175,    -1,    -1,    -1,    13,    -1,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      13,    91,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,    37,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    51,    -1,    -1,    -1,
      -1,    56,   122,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,     3,
       4,    -1,    -1,    -1,    -1,    -1,   171,   172,   173,    13,
     175,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,   175,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    37,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    51,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    13,    91,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    88,    -1,   123,    91,
      27,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,
      37,    -1,    -1,   138,   139,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    56,    51,    -1,    -1,    -1,    -1,    56,
     122,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      56,    88,    -1,    -1,    91,   190,   191,   192,   193,   194,
     103,   175,   197,   198,    -1,   200,   201,   202,   203,   204,
     205,    -1,   207,   175,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,   122,    -1,    -1,   131,    -1,
      -1,    -1,    -1,    -1,    -1,   138,   139,   103,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,
     193,   194,    -1,    -1,   197,   198,    -1,   200,   201,   202,
     203,   204,   205,    -1,   207,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,   191,   192,   193,   194,    13,
      -1,   197,   198,    -1,   200,   201,   202,   203,   204,   205,
      -1,   207,    -1,    27,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    90,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,    -1,    -1,    -1,   110,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   155
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     9,    17,    24,    29,    39,    42,    43,    54,    55,
      60,    64,    67,    89,    93,   109,   115,   157,   158,   159,
     210,   215,   219,   220,   221,   222,   226,   228,   230,   233,
     234,   240,   241,   242,   255,   257,   260,   261,   262,   264,
     267,   268,   269,   270,   271,   278,   364,    29,     3,    13,
      27,    37,    51,    56,    88,    91,   122,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   175,
     331,   334,   353,     3,     3,   353,    72,    82,   113,    96,
      96,   100,   126,   292,   352,   353,    82,   271,   272,   353,
      72,    82,   113,    21,    62,     3,   331,   334,   365,   366,
     156,   227,   227,   227,   267,   271,     7,     8,   217,   231,
     345,     0,   213,   370,   115,   223,   223,    47,    78,    83,
     273,   287,    11,   274,   268,     3,   212,   212,   210,   232,
      96,   129,   243,   243,   243,   353,     3,   235,   353,     6,
     210,   339,    14,   279,   125,   129,   256,   135,    96,   135,
     256,   256,   256,   353,   127,   214,   211,   211,    86,   190,
     221,    99,   117,   275,   128,   275,   269,    57,    74,   293,
     275,   270,   287,     3,   171,   172,   173,   331,   332,   333,
     332,   333,     4,     5,     6,    13,    50,    90,   106,   112,
     152,   153,   154,   171,   172,   173,   201,   295,   296,   297,
     298,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,     4,   229,   123,   334,   354,   353,   353,    85,
     283,    95,   210,   263,   267,     3,    13,    48,    49,    50,
      87,   123,   137,   183,   185,   201,   202,   210,   272,   300,
     301,   302,   303,   304,   305,   306,   309,   311,   312,   313,
     314,   320,   321,   322,   327,   328,   329,   330,   331,   335,
     280,   294,   299,   300,   265,   266,   334,    48,   353,     4,
     236,   236,   236,   334,   353,   353,   272,   366,   353,   210,
     288,   289,   300,   300,   117,   300,   121,   359,   360,   361,
     293,   212,   212,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   323,   324,   325,     4,   210,
       4,     4,     4,   210,   210,   210,   338,   344,   211,   214,
      48,   133,    96,   127,   210,   210,   263,   300,   236,   334,
     371,    65,   210,   212,   272,   210,   210,   114,   300,   310,
      48,   123,   210,   301,   207,   301,   353,   301,   271,   300,
     308,   118,   134,   211,    19,    56,   103,   123,   130,   131,
     138,   139,   190,   191,   192,   193,   194,   197,   198,   200,
     201,   202,   203,   204,   205,   207,   189,   286,   182,   210,
      96,   281,   282,   214,     3,   127,   357,   358,   214,   283,
     190,    93,   258,   259,   115,   210,   237,   237,   237,   232,
       3,   224,   225,   214,    92,   119,   290,   165,    57,    57,
      64,   136,   163,   180,   362,   361,   359,   333,   333,   298,
     323,   326,   211,     6,   211,     6,   211,   297,   353,     3,
     267,   177,   178,   179,   244,   245,   246,   253,   334,   371,
     127,   100,   211,   214,   210,   211,   279,   202,   331,   332,
     323,   300,   300,   310,    94,   114,   120,   272,   300,   123,
     294,   211,   214,   211,   214,   300,   300,   301,   301,   103,
     130,   210,   272,   106,   123,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     339,   210,    79,   315,   211,   279,     3,   184,   210,   272,
     313,   331,   346,   347,   348,   350,   351,   353,   367,   283,
     299,     3,     4,   331,   266,   300,    41,   210,     3,    26,
      51,   106,   238,   239,   283,   216,   210,   211,   214,   289,
       3,   291,   174,   300,   300,   180,   163,   132,   160,   161,
     363,   127,   135,   211,   211,   210,    95,   210,   180,   180,
     211,   214,    13,    27,    31,    32,    33,    34,    35,    37,
      45,    70,    88,    90,    91,   105,   110,   112,   122,   152,
     155,   247,   211,   267,   353,   267,   334,   296,   294,   210,
     212,    96,   127,   111,    94,   120,   300,   300,   208,   300,
     123,   130,   300,   118,   301,   210,   272,   294,   106,   208,
      78,   210,   294,   212,   347,    65,   127,   355,   356,   357,
     356,    28,    68,    73,    80,    81,    97,   102,   368,   214,
     356,   186,   187,   188,   276,   277,   256,   238,     4,     4,
     235,     4,   211,   214,   296,   225,     3,   175,    64,   371,
     162,   247,   323,   371,   236,   371,   210,   210,   245,   210,
     210,   249,   210,   210,   210,   248,   323,   326,   106,   123,
     177,   178,   181,   250,   251,   252,   254,   211,   211,   211,
     279,   202,   331,   332,   300,   247,   300,   300,   120,   111,
     130,   210,   272,   301,   294,   211,   128,   107,   316,   211,
     332,   216,   210,   349,     3,   331,   334,   101,    80,    80,
      80,   101,   347,   300,    71,   284,   300,   334,   211,    26,
      51,   239,   211,   300,   363,   211,   211,   211,   371,   371,
       6,     6,     6,     6,     6,   106,   180,   353,   252,   254,
     294,   212,   211,   211,   120,   300,   210,   272,   210,   307,
     211,   288,   128,   287,   294,   211,   214,   210,   348,   347,
     277,   128,   186,     4,   235,   165,   211,   211,   211,   211,
     214,   211,   211,   211,   263,   211,   202,   331,   332,   307,
     300,   308,   211,   214,   211,   294,   164,   165,   166,   317,
     318,   211,   356,   210,   371,    84,   133,   300,   294,   300,
     176,   254,     6,   211,   211,   210,   211,     6,    19,   167,
     170,   319,   294,   211,   210,   300,   369,    53,   285,   211,
     308,   168,   169,   319,   168,   169,   211,   371,   300,   211,
     118,   211,   319
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   218,   219,   220,   220,   221,   221,   221,   221,   221,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   223,   223,   224,   224,   225,   225,   226,   226,   226,
     227,   227,   228,   229,   230,   231,   231,   232,   232,   233,
     234,   234,   235,   236,   237,   237,   237,   238,   238,   238,
     238,   238,   238,   239,   239,   240,   240,   241,   241,   241,
     242,   242,   242,   242,   242,   243,   243,   244,   244,   245,
     245,   246,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   248,   248,   249,   249,   249,   250,   250,
     251,   251,   251,   251,   252,   252,   252,   252,   253,   253,
     253,   254,   255,   255,   255,   255,   256,   256,   257,   258,
     259,   260,   261,   262,   262,   262,   263,   263,   264,   265,
     265,   266,   267,   267,   268,   268,   269,   269,   270,   270,
     271,   271,   272,   273,   273,   274,   275,   275,   276,   276,
     276,   276,   276,   277,   277,   278,   279,   279,   280,   281,
     281,   282,   283,   283,   284,   284,   285,   285,   286,   286,
     287,   287,   288,   288,   289,   290,   290,   290,   291,   291,
     292,   292,   292,   293,   293,   293,   293,   293,   293,   293,
     294,   294,   295,   295,   296,   296,   297,   297,   298,   298,
     298,   299,   300,   300,   300,   300,   300,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   302,
     302,   302,   303,   303,   303,   303,   303,   303,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   305,
     305,   306,   306,   306,   306,   306,   306,   306,   306,   307,
     307,   308,   308,   309,   309,   309,   309,   310,   310,   311,
     311,   312,   312,   312,   312,   312,   312,   312,   313,   313,
     313,   313,   313,   313,   314,   315,   315,   316,   316,   317,
     317,   317,   318,   318,   318,   319,   319,   319,   319,   319,
     320,   321,   322,   323,   323,   323,   323,   323,   323,   324,
     324,   324,   324,   324,   324,   325,   325,   326,   327,   328,
     329,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   332,   332,   332,   332,   333,   333,
     334,   334,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   336,   337,   337,   338,   338,   339,   340,   341,   341,
     341,   342,   342,   342,   342,   343,   343,   343,   343,   344,
     344,   344,   344,   345,   345,   345,   346,   346,   347,   347,
     347,   348,   348,   348,   348,   349,   349,   350,   350,   351,
     352,   353,   353,   353,   353,   353,   354,   354,   355,   355,
     356,   356,   357,   357,   357,   357,   358,   358,   359,   359,
     360,   360,   361,   361,   362,   362,   362,   362,   363,   363,
     363,   364,   365,   365,   366,   367,   367,   367,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   369,   370,
     370,   371,   371
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
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     1,     4,     3,     0,     5,     3,     0,     1,     0,
       1,     2,     1,     2,     2,     1,     1,     2,     5,     4,
       6,     3,     4,     4,     3,     4,     2,     0,     5,     1,
       4,     4,     2,     8,     4,     7,     3,     0,     5,     1,
       3,     3,     4,     5,     1,     3,     1,     3,     1,     1,
       3,     3,     1,     2,     2,     2,     1,     0,     4,     4,
       2,     2,     0,     1,     1,     8,     1,     0,     1,     1,
       0,     2,     2,     0,     4,     0,     2,     0,     6,     0,
       3,     0,     1,     3,     3,     1,     1,     0,     0,     2,
       2,     4,     0,     2,     2,     4,     2,     4,     8,     0,
       1,     3,     1,     0,     1,     3,     1,     6,     1,     2,
       2,     2,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     3,     4,     1,     3,
       3,     3,     3,     3,     3,     3,     4,     3,     3,     3,
       3,     5,     6,     3,     4,     5,     6,     7,     8,     3,
       5,     3,     3,     4,     6,     3,     5,     4,     5,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     5,
       5,     7,     3,     5,     3,     6,     0,     3,     0,     2,
       5,     0,     1,     1,     1,     2,     2,     2,     2,     1,
       6,     2,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     4,
       5,     1,     3,     1,     3,     1,     5,     7,     5,     7,
       3,     5,     7,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       3,     2,     1,     3,     4,     2,     1,     3,     4,     2,
       3,     3,     2,     1,     1,     1,     1,     3,     1,     1,
       3,     1,     2,     2,     5,     3,     5,     1,     3,     2,
       1,     1,     3,     3,     5,     5,     1,     0,     1,     5,
       1,     0,     2,     2,     2,     1,     1,     0,     1,     0,
       1,     2,     3,     5,     1,     3,     1,     2,     2,     1,
       0,     2,     1,     3,     3,     4,     6,     8,     1,     2,
       1,     2,     1,     2,     1,     1,     1,     0,     1,     1,
       0,     1,     3
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
#line 328 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2707 "bison_parser.cpp"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 328 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2713 "bison_parser.cpp"
        break;

    case YYSYMBOL_FLOATVAL: /* FLOATVAL  */
#line 310 "bison_parser.y"
            { }
#line 2719 "bison_parser.cpp"
        break;

    case YYSYMBOL_INTVAL: /* INTVAL  */
#line 310 "bison_parser.y"
            { }
#line 2725 "bison_parser.cpp"
        break;

    case YYSYMBOL_DOLLAR_PARAM: /* DOLLAR_PARAM  */
#line 310 "bison_parser.y"
            { }
#line 2731 "bison_parser.cpp"
        break;

    case YYSYMBOL_NAMED_PARAM: /* NAMED_PARAM  */
#line 328 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2737 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement_list: /* statement_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).stmt_vec)) {
    for (auto ptr : *(((*yyvaluep).stmt_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).stmt_vec));
}
#line 2750 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2756 "bison_parser.cpp"
        break;

    case YYSYMBOL_preparable_statement: /* preparable_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2762 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_hints: /* opt_hints  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2775 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint_list: /* hint_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2788 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint: /* hint  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2794 "bison_parser.cpp"
        break;

    case YYSYMBOL_transaction_statement: /* transaction_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).transaction_stmt)); }
#line 2800 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_statement: /* prepare_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).prep_stmt)); }
#line 2806 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_target_query: /* prepare_target_query  */
#line 328 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2812 "bison_parser.cpp"
        break;

    case YYSYMBOL_odbc_call_statement: /* odbc_call_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2818 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_odbc_return: /* opt_odbc_return  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2824 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_call_args: /* opt_call_args  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2837 "bison_parser.cpp"
        break;

    case YYSYMBOL_execute_statement: /* execute_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2843 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_statement: /* import_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).import_stmt)); }
#line 2849 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_type: /* file_type  */
#line 310 "bison_parser.y"
            { }
#line 2855 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_path: /* file_path  */
#line 328 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2861 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_import_export_options: /* opt_import_export_options  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2867 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_export_options: /* import_export_options  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2873 "bison_parser.cpp"
        break;

    case YYSYMBOL_csv_option: /* csv_option  */
#line 337 "bison_parser.y"
            {
  free(((*yyvaluep).csv_option_t)->second);
  delete (((*yyvaluep).csv_option_t));
}
#line 2882 "bison_parser.cpp"
        break;

    case YYSYMBOL_export_statement: /* export_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).export_stmt)); }
#line 2888 "bison_parser.cpp"
        break;

    case YYSYMBOL_show_statement: /* show_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).show_stmt)); }
#line 2894 "bison_parser.cpp"
        break;

    case YYSYMBOL_create_statement: /* create_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).create_stmt)); }
#line 2900 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_not_exists: /* opt_not_exists  */
#line 310 "bison_parser.y"
            { }
#line 2906 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem_commalist: /* table_elem_commalist  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).table_element_vec)) {
    for (auto ptr : *(((*yyvaluep).table_element_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_element_vec));
}
#line 2919 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem: /* table_elem  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).table_element_t)); }
#line 2925 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_def: /* column_def  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).column_t)); }
#line 2931 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_type: /* column_type  */
#line 310 "bison_parser.y"
            { }
#line 2937 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_time_precision: /* opt_time_precision  */
#line 310 "bison_parser.y"
            { }
#line 2943 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_decimal_specification: /* opt_decimal_specification  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).ival_pair)); }
#line 2949 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_column_constraints: /* opt_column_constraints  */
#line 341 "bison_parser.y"
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
#line 2971 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraints: /* column_constraints  */
#line 341 "bison_parser.y"
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
#line 2993 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraint: /* column_constraint  */
#line 310 "bison_parser.y"
            { }
#line 2999 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_constraint: /* table_constraint  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).table_constraint_t)); }
#line 3005 "bison_parser.cpp"
        break;

    case YYSYMBOL_references_spec: /* references_spec  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).references_spec_t)); }
#line 3011 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_statement: /* drop_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).drop_stmt)); }
#line 3017 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_exists: /* opt_exists  */
#line 310 "bison_parser.y"
            { }
#line 3023 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_statement: /* alter_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).alter_stmt)); }
#line 3029 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_action: /* alter_action  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).alter_action_t)); }
#line 3035 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_action: /* drop_action  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).drop_action_t)); }
#line 3041 "bison_parser.cpp"
        break;

    case YYSYMBOL_delete_statement: /* delete_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 3047 "bison_parser.cpp"
        break;

    case YYSYMBOL_truncate_statement: /* truncate_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 3053 "bison_parser.cpp"
        break;

    case YYSYMBOL_insert_statement: /* insert_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).insert_stmt)); }
#line 3059 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_column_list: /* opt_column_list  */
#line 320 "bison_parser.y"
            {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 3072 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_statement: /* update_statement  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).update_stmt)); }
#line 3078 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause_commalist: /* update_clause_commalist  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).update_vec)) {
    for (auto ptr : *(((*yyvaluep).update_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).update_vec));
}
#line 3091 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause: /* update_clause  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).update_t)); }
#line 3097 "bison_parser.cpp"
        break;

    case YYSYMBOL_query_expression: /* query_expression  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 3103 "bison_parser.cpp"
        break;

    case YYSYMBOL_query_expression_body: /* query_expression_body  */
#line 319 "bison_parser.y"
            { delete ((*yyvaluep).query_body_t).head; }
#line 3109 "bison_parser.cpp"
        break;

    case YYSYMBOL_query_term: /* query_term  */
#line 319 "bison_parser.y"
            { delete ((*yyvaluep).query_body_t).head; }
#line 3115 "bison_parser.cpp"
        break;

    case YYSYMBOL_query_member: /* query_member  */
#line 319 "bison_parser.y"
            { delete ((*yyvaluep).query_body_t).head; }
#line 3121 "bison_parser.cpp"
        break;

    case YYSYMBOL_query_expression_parens: /* query_expression_parens  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 3127 "bison_parser.cpp"
        break;

    case YYSYMBOL_subquery: /* subquery  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 3133 "bison_parser.cpp"
        break;

    case YYSYMBOL_union_operator: /* union_operator  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 3139 "bison_parser.cpp"
        break;

    case YYSYMBOL_intersect_operator: /* intersect_operator  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 3145 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_all: /* opt_all  */
#line 310 "bison_parser.y"
            { }
#line 3151 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_hierarchical_clause: /* opt_hierarchical_clause  */
#line 315 "bison_parser.y"
            {
  delete ((*yyvaluep).hierarchical_t).startWith;
  delete ((*yyvaluep).hierarchical_t).connectBy;
}
#line 3160 "bison_parser.cpp"
        break;

    case YYSYMBOL_query_primary: /* query_primary  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 3166 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_distinct: /* opt_distinct  */
#line 310 "bison_parser.y"
            { }
#line 3172 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_list: /* select_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3185 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_from_clause: /* opt_from_clause  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3191 "bison_parser.cpp"
        break;

    case YYSYMBOL_from_clause: /* from_clause  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3197 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_where: /* opt_where  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3203 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_group: /* opt_group  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).group_t)); }
#line 3209 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_having: /* opt_having  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3215 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_within_group: /* opt_within_group  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 3228 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order: /* opt_order  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 3241 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_list: /* order_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 3254 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_desc: /* order_desc  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).order)); }
#line 3260 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order_type: /* opt_order_type  */
#line 310 "bison_parser.y"
            { }
#line 3266 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_null_ordering: /* opt_null_ordering  */
#line 310 "bison_parser.y"
            { }
#line 3272 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_top: /* opt_top  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 3278 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_limit: /* opt_limit  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 3284 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_list: /* expr_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3297 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_extended_literal_list: /* opt_extended_literal_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3310 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal_list: /* extended_literal_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3323 "bison_parser.cpp"
        break;

    case YYSYMBOL_casted_extended_literal: /* casted_extended_literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3329 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal: /* extended_literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3335 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_alias: /* expr_alias  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3341 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr: /* expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3347 "bison_parser.cpp"
        break;

    case YYSYMBOL_operand: /* operand  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3353 "bison_parser.cpp"
        break;

    case YYSYMBOL_scalar_expr: /* scalar_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3359 "bison_parser.cpp"
        break;

    case YYSYMBOL_unary_expr: /* unary_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3365 "bison_parser.cpp"
        break;

    case YYSYMBOL_binary_expr: /* binary_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3371 "bison_parser.cpp"
        break;

    case YYSYMBOL_logic_expr: /* logic_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3377 "bison_parser.cpp"
        break;

    case YYSYMBOL_in_expr: /* in_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3383 "bison_parser.cpp"
        break;

    case YYSYMBOL_in_row_list: /* in_row_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3396 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_expr_list: /* row_expr_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3409 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_expr: /* case_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3415 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_list: /* case_list  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3421 "bison_parser.cpp"
        break;

    case YYSYMBOL_exists_expr: /* exists_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3427 "bison_parser.cpp"
        break;

    case YYSYMBOL_comp_expr: /* comp_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3433 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_function_expr: /* table_function_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3439 "bison_parser.cpp"
        break;

    case YYSYMBOL_function_expr: /* function_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3445 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_window: /* opt_window  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).window_description)); }
#line 3451 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_partition: /* opt_partition  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3464 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_frame_clause: /* opt_frame_clause  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).frame_description)); }
#line 3470 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_type: /* frame_type  */
#line 310 "bison_parser.y"
            { }
#line 3476 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_bound: /* frame_bound  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).frame_bound)); }
#line 3482 "bison_parser.cpp"
        break;

    case YYSYMBOL_extract_expr: /* extract_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3488 "bison_parser.cpp"
        break;

    case YYSYMBOL_next_value_expr: /* next_value_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3494 "bison_parser.cpp"
        break;

    case YYSYMBOL_cast_expr: /* cast_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3500 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field: /* datetime_field  */
#line 310 "bison_parser.y"
            { }
#line 3506 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field_plural: /* datetime_field_plural  */
#line 310 "bison_parser.y"
            { }
#line 3512 "bison_parser.cpp"
        break;

    case YYSYMBOL_duration_field: /* duration_field  */
#line 310 "bison_parser.y"
            { }
#line 3518 "bison_parser.cpp"
        break;

    case YYSYMBOL_interval_range: /* interval_range  */
#line 310 "bison_parser.y"
            { }
#line 3524 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_expr: /* array_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3530 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_index: /* array_index  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3536 "bison_parser.cpp"
        break;

    case YYSYMBOL_between_expr: /* between_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3542 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_name: /* column_name  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3548 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonreserved_keyword: /* nonreserved_keyword  */
#line 328 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3554 "bison_parser.cpp"
        break;

    case YYSYMBOL_name_after_qualifier: /* name_after_qualifier  */
#line 328 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3560 "bison_parser.cpp"
        break;

    case YYSYMBOL_qualified_name_or_keyword: /* qualified_name_or_keyword  */
#line 328 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3566 "bison_parser.cpp"
        break;

    case YYSYMBOL_name_or_keyword: /* name_or_keyword  */
#line 328 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3572 "bison_parser.cpp"
        break;

    case YYSYMBOL_literal: /* literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3578 "bison_parser.cpp"
        break;

    case YYSYMBOL_string_literal: /* string_literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3584 "bison_parser.cpp"
        break;

    case YYSYMBOL_bool_literal: /* bool_literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3590 "bison_parser.cpp"
        break;

    case YYSYMBOL_num_literal: /* num_literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3596 "bison_parser.cpp"
        break;

    case YYSYMBOL_int_literal: /* int_literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3602 "bison_parser.cpp"
        break;

    case YYSYMBOL_null_literal: /* null_literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3608 "bison_parser.cpp"
        break;

    case YYSYMBOL_date_literal: /* date_literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3614 "bison_parser.cpp"
        break;

    case YYSYMBOL_time_literal: /* time_literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3620 "bison_parser.cpp"
        break;

    case YYSYMBOL_timestamp_literal: /* timestamp_literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3626 "bison_parser.cpp"
        break;

    case YYSYMBOL_interval_literal: /* interval_literal  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3632 "bison_parser.cpp"
        break;

    case YYSYMBOL_param_expr: /* param_expr  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3638 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref: /* table_ref  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3644 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_atomic: /* table_ref_atomic  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3650 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonjoin_table_ref_atomic: /* nonjoin_table_ref_atomic  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3656 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_value_row_list: /* table_value_row_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3669 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_commalist: /* table_ref_commalist  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).table_vec)) {
    for (auto ptr : *(((*yyvaluep).table_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_vec));
}
#line 3682 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name: /* table_ref_name  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3688 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name_no_alias: /* table_ref_name_no_alias  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3694 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_name: /* table_name  */
#line 311 "bison_parser.y"
            {
  free(((*yyvaluep).table_name).name);
  free(((*yyvaluep).table_name).schema);
}
#line 3703 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_index_name: /* opt_index_name  */
#line 328 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3709 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_alias: /* table_alias  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3715 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_table_alias: /* opt_table_alias  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3721 "bison_parser.cpp"
        break;

    case YYSYMBOL_alias: /* alias  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3727 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_alias: /* opt_alias  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3733 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause: /* opt_locking_clause  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).locking_clause_vec)) {
    for (auto ptr : *(((*yyvaluep).locking_clause_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).locking_clause_vec));
}
#line 3746 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause_list: /* opt_locking_clause_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).locking_clause_vec)) {
    for (auto ptr : *(((*yyvaluep).locking_clause_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).locking_clause_vec));
}
#line 3759 "bison_parser.cpp"
        break;

    case YYSYMBOL_locking_clause: /* locking_clause  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).locking_t)); }
#line 3765 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_lock_mode: /* row_lock_mode  */
#line 310 "bison_parser.y"
            { }
#line 3771 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_row_lock_policy: /* opt_row_lock_policy  */
#line 310 "bison_parser.y"
            { }
#line 3777 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_clause: /* with_clause  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3790 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description_list: /* with_description_list  */
#line 329 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3803 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description: /* with_description  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).with_description_t)); }
#line 3809 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_clause: /* join_clause  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3815 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_join_type: /* opt_join_type  */
#line 310 "bison_parser.y"
            { }
#line 3821 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_condition: /* join_condition  */
#line 358 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3827 "bison_parser.cpp"
        break;

    case YYSYMBOL_ident_commalist: /* ident_commalist  */
#line 320 "bison_parser.y"
            {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 3840 "bison_parser.cpp"
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
#line 208 "bison_parser.y"
{
  // Initialize
  yylloc.first_column = 0;
  yylloc.last_column = 0;
  yylloc.first_line = 0;
  yylloc.last_line = 0;
  yylloc.total_column = 0;
  yylloc.string_length = 0;
}

#line 3948 "bison_parser.cpp"

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
#line 509 "bison_parser.y"
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
#line 4178 "bison_parser.cpp"
    break;

  case 3: /* statement_list: statement  */
#line 529 "bison_parser.y"
                           {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyval.stmt_vec) = new std::vector<SQLStatement*>();
  (yyval.stmt_vec)->push_back((yyvsp[0].statement));
}
#line 4189 "bison_parser.cpp"
    break;

  case 4: /* statement_list: statement_list ';' statement  */
#line 535 "bison_parser.y"
                               {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyvsp[-2].stmt_vec)->push_back((yyvsp[0].statement));
  (yyval.stmt_vec) = (yyvsp[-2].stmt_vec);
}
#line 4200 "bison_parser.cpp"
    break;

  case 5: /* statement: prepare_statement opt_hints  */
#line 542 "bison_parser.y"
                                        {
  (yyval.statement) = (yyvsp[-1].prep_stmt);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 4209 "bison_parser.cpp"
    break;

  case 6: /* statement: preparable_statement opt_hints  */
#line 546 "bison_parser.y"
                                 {
  (yyval.statement) = (yyvsp[-1].statement);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 4218 "bison_parser.cpp"
    break;

  case 7: /* statement: show_statement  */
#line 550 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].show_stmt); }
#line 4224 "bison_parser.cpp"
    break;

  case 8: /* statement: import_statement  */
#line 551 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].import_stmt); }
#line 4230 "bison_parser.cpp"
    break;

  case 9: /* statement: export_statement  */
#line 552 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].export_stmt); }
#line 4236 "bison_parser.cpp"
    break;

  case 10: /* preparable_statement: query_expression  */
#line 554 "bison_parser.y"
                                        { (yyval.statement) = (yyvsp[0].select_stmt); }
#line 4242 "bison_parser.cpp"
    break;

  case 11: /* preparable_statement: create_statement  */
#line 555 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].create_stmt); }
#line 4248 "bison_parser.cpp"
    break;

  case 12: /* preparable_statement: insert_statement  */
#line 556 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].insert_stmt); }
#line 4254 "bison_parser.cpp"
    break;

  case 13: /* preparable_statement: delete_statement  */
#line 557 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 4260 "bison_parser.cpp"
    break;

  case 14: /* preparable_statement: truncate_statement  */
#line 558 "bison_parser.y"
                     { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 4266 "bison_parser.cpp"
    break;

  case 15: /* preparable_statement: update_statement  */
#line 559 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].update_stmt); }
#line 4272 "bison_parser.cpp"
    break;

  case 16: /* preparable_statement: drop_statement  */
#line 560 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].drop_stmt); }
#line 4278 "bison_parser.cpp"
    break;

  case 17: /* preparable_statement: alter_statement  */
#line 561 "bison_parser.y"
                  { (yyval.statement) = (yyvsp[0].alter_stmt); }
#line 4284 "bison_parser.cpp"
    break;

  case 18: /* preparable_statement: execute_statement  */
#line 562 "bison_parser.y"
                    { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 4290 "bison_parser.cpp"
    break;

  case 19: /* preparable_statement: odbc_call_statement  */
#line 563 "bison_parser.y"
                      { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 4296 "bison_parser.cpp"
    break;

  case 20: /* preparable_statement: transaction_statement  */
#line 564 "bison_parser.y"
                        { (yyval.statement) = (yyvsp[0].transaction_stmt); }
#line 4302 "bison_parser.cpp"
    break;

  case 21: /* opt_hints: WITH HINT '(' hint_list ')'  */
#line 570 "bison_parser.y"
                                        { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 4308 "bison_parser.cpp"
    break;

  case 22: /* opt_hints: %empty  */
#line 571 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 4314 "bison_parser.cpp"
    break;

  case 23: /* hint_list: hint  */
#line 573 "bison_parser.y"
                 {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 4323 "bison_parser.cpp"
    break;

  case 24: /* hint_list: hint_list ',' hint  */
#line 577 "bison_parser.y"
                     {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 4332 "bison_parser.cpp"
    break;

  case 25: /* hint: IDENTIFIER  */
#line 582 "bison_parser.y"
                  {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[0].sval);
}
#line 4341 "bison_parser.cpp"
    break;

  case 26: /* hint: IDENTIFIER '(' extended_literal_list ')'  */
#line 586 "bison_parser.y"
                                           {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[-3].sval);
  (yyval.expr)->exprList = (yyvsp[-1].expr_vec);
}
#line 4351 "bison_parser.cpp"
    break;

  case 27: /* transaction_statement: BEGIN opt_transaction_keyword  */
#line 596 "bison_parser.y"
                                                      { (yyval.transaction_stmt) = new TransactionStatement(kBeginTransaction); }
#line 4357 "bison_parser.cpp"
    break;

  case 28: /* transaction_statement: ROLLBACK opt_transaction_keyword  */
#line 597 "bison_parser.y"
                                   { (yyval.transaction_stmt) = new TransactionStatement(kRollbackTransaction); }
#line 4363 "bison_parser.cpp"
    break;

  case 29: /* transaction_statement: COMMIT opt_transaction_keyword  */
#line 598 "bison_parser.y"
                                 { (yyval.transaction_stmt) = new TransactionStatement(kCommitTransaction); }
#line 4369 "bison_parser.cpp"
    break;

  case 32: /* prepare_statement: PREPARE IDENTIFIER FROM prepare_target_query  */
#line 606 "bison_parser.y"
                                                                 {
  (yyval.prep_stmt) = new PrepareStatement();
  (yyval.prep_stmt)->name = (yyvsp[-2].sval);
  (yyval.prep_stmt)->query = (yyvsp[0].sval);
}
#line 4379 "bison_parser.cpp"
    break;

  case 34: /* odbc_call_statement: '{' opt_odbc_return CALL table_name opt_call_args '}'  */
#line 618 "bison_parser.y"
                                                                            {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->returnValue = (yyvsp[-4].expr);
  (yyval.exec_stmt)->name = qualifiedName((yyvsp[-2].table_name));
  (yyval.exec_stmt)->parameters = (yyvsp[-1].expr_vec);
}
#line 4390 "bison_parser.cpp"
    break;

  case 35: /* opt_odbc_return: param_expr '='  */
#line 627 "bison_parser.y"
                                 { (yyval.expr) = (yyvsp[-1].expr); }
#line 4396 "bison_parser.cpp"
    break;

  case 36: /* opt_odbc_return: %empty  */
#line 628 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4402 "bison_parser.cpp"
    break;

  case 37: /* opt_call_args: '(' opt_extended_literal_list ')'  */
#line 630 "bison_parser.y"
                                                  { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 4408 "bison_parser.cpp"
    break;

  case 38: /* opt_call_args: %empty  */
#line 631 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 4414 "bison_parser.cpp"
    break;

  case 39: /* execute_statement: EXECUTE IDENTIFIER opt_call_args  */
#line 633 "bison_parser.y"
                                                     {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[-1].sval);
  (yyval.exec_stmt)->parameters = (yyvsp[0].expr_vec);
}
#line 4424 "bison_parser.cpp"
    break;

  case 40: /* import_statement: IMPORT FROM file_type FILE file_path INTO table_name  */
#line 645 "bison_parser.y"
                                                                        {
  (yyval.import_stmt) = new ImportStatement((yyvsp[-4].import_type_t));
  (yyval.import_stmt)->filePath = (yyvsp[-2].sval);
  (yyval.import_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.import_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 4435 "bison_parser.cpp"
    break;

  case 41: /* import_statement: COPY table_name FROM file_path opt_import_export_options opt_where  */
#line 651 "bison_parser.y"
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
#line 4456 "bison_parser.cpp"
    break;

  case 42: /* file_type: IDENTIFIER  */
#line 668 "bison_parser.y"
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
#line 4475 "bison_parser.cpp"
    break;

  case 43: /* file_path: STRING  */
#line 683 "bison_parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 4481 "bison_parser.cpp"
    break;

  case 44: /* opt_import_export_options: WITH '(' import_export_options ')'  */
#line 685 "bison_parser.y"
                                                               { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 4487 "bison_parser.cpp"
    break;

  case 45: /* opt_import_export_options: '(' import_export_options ')'  */
#line 686 "bison_parser.y"
                                { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 4493 "bison_parser.cpp"
    break;

  case 46: /* opt_import_export_options: %empty  */
#line 687 "bison_parser.y"
              { (yyval.import_export_option_t) = new ImportExportOptions{}; }
#line 4499 "bison_parser.cpp"
    break;

  case 47: /* import_export_options: import_export_options ',' FORMAT file_type  */
#line 689 "bison_parser.y"
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
#line 4518 "bison_parser.cpp"
    break;

  case 48: /* import_export_options: FORMAT file_type  */
#line 703 "bison_parser.y"
                   {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->format = (yyvsp[0].import_type_t);
}
#line 4527 "bison_parser.cpp"
    break;

  case 49: /* import_export_options: import_export_options ',' ENCODING STRING  */
#line 707 "bison_parser.y"
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
#line 4542 "bison_parser.cpp"
    break;

  case 50: /* import_export_options: ENCODING STRING  */
#line 717 "bison_parser.y"
                  {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->encoding = (yyvsp[0].sval);
}
#line 4551 "bison_parser.cpp"
    break;

  case 51: /* import_export_options: import_export_options ',' csv_option  */
#line 721 "bison_parser.y"
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
#line 4580 "bison_parser.cpp"
    break;

  case 52: /* import_export_options: csv_option  */
#line 745 "bison_parser.y"
             {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->csv_options = new CsvOptions{};
  (yyval.import_export_option_t)->csv_options->accept_csv_option((yyvsp[0].csv_option_t));

  delete (yyvsp[0].csv_option_t);
}
#line 4592 "bison_parser.cpp"
    break;

  case 53: /* csv_option: IDENTIFIER STRING  */
#line 753 "bison_parser.y"
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
#line 4610 "bison_parser.cpp"
    break;

  case 54: /* csv_option: NULL STRING  */
#line 766 "bison_parser.y"
              { (yyval.csv_option_t) = new std::pair<CsvOptionType, char*>(CsvOptionType::Null, (yyvsp[0].sval)); }
#line 4616 "bison_parser.cpp"
    break;

  case 55: /* export_statement: COPY table_name TO file_path opt_import_export_options  */
#line 773 "bison_parser.y"
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
#line 4636 "bison_parser.cpp"
    break;

  case 56: /* export_statement: COPY subquery TO file_path opt_import_export_options  */
#line 788 "bison_parser.y"
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
#line 4655 "bison_parser.cpp"
    break;

  case 57: /* show_statement: SHOW TABLES  */
#line 808 "bison_parser.y"
                             { (yyval.show_stmt) = new ShowStatement(kShowTables); }
#line 4661 "bison_parser.cpp"
    break;

  case 58: /* show_statement: SHOW COLUMNS table_name  */
#line 809 "bison_parser.y"
                          {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4671 "bison_parser.cpp"
    break;

  case 59: /* show_statement: DESCRIBE table_name  */
#line 814 "bison_parser.y"
                      {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4681 "bison_parser.cpp"
    break;

  case 60: /* create_statement: CREATE TABLE opt_not_exists table_name FROM IDENTIFIER FILE file_path  */
#line 825 "bison_parser.y"
                                                                                         {
  (yyval.create_stmt) = new CreateStatement(kCreateTableFromTbl);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-5].bval);
  (yyval.create_stmt)->schema = (yyvsp[-4].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-4].table_name).name;
  if (strcasecmp((yyvsp[-2].sval), "tbl") != 0) {
    free((yyvsp[-2].sval));
    free((yyvsp[0].sval));
    delete (yyval.create_stmt);
    yyerror(&yyloc, result, scanner, "File type is unknown.");
    YYERROR;
  }
  free((yyvsp[-2].sval));
  (yyval.create_stmt)->filePath = (yyvsp[0].sval);
}
#line 4701 "bison_parser.cpp"
    break;

  case 61: /* create_statement: CREATE TABLE opt_not_exists table_name '(' table_elem_commalist ')'  */
#line 840 "bison_parser.y"
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
#line 4718 "bison_parser.cpp"
    break;

  case 62: /* create_statement: CREATE TABLE opt_not_exists table_name AS query_expression  */
#line 852 "bison_parser.y"
                                                             {
  (yyval.create_stmt) = new CreateStatement(kCreateTable);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-3].bval);
  (yyval.create_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4730 "bison_parser.cpp"
    break;

  case 63: /* create_statement: CREATE INDEX opt_not_exists opt_index_name ON table_name '(' ident_commalist ')'  */
#line 859 "bison_parser.y"
                                                                                   {
  (yyval.create_stmt) = new CreateStatement(kCreateIndex);
  (yyval.create_stmt)->indexName = (yyvsp[-5].sval);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-6].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->indexColumns = (yyvsp[-1].str_vec);
}
#line 4743 "bison_parser.cpp"
    break;

  case 64: /* create_statement: CREATE VIEW opt_not_exists table_name opt_column_list AS query_expression  */
#line 867 "bison_parser.y"
                                                                            {
  (yyval.create_stmt) = new CreateStatement(kCreateView);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-4].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->viewColumns = (yyvsp[-2].str_vec);
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4756 "bison_parser.cpp"
    break;

  case 65: /* opt_not_exists: IF NOT EXISTS  */
#line 876 "bison_parser.y"
                               { (yyval.bval) = true; }
#line 4762 "bison_parser.cpp"
    break;

  case 66: /* opt_not_exists: %empty  */
#line 877 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4768 "bison_parser.cpp"
    break;

  case 67: /* table_elem_commalist: table_elem  */
#line 879 "bison_parser.y"
                                  {
  (yyval.table_element_vec) = new std::vector<TableElement*>();
  (yyval.table_element_vec)->push_back((yyvsp[0].table_element_t));
}
#line 4777 "bison_parser.cpp"
    break;

  case 68: /* table_elem_commalist: table_elem_commalist ',' table_elem  */
#line 883 "bison_parser.y"
                                      {
  (yyvsp[-2].table_element_vec)->push_back((yyvsp[0].table_element_t));
  (yyval.table_element_vec) = (yyvsp[-2].table_element_vec);
}
#line 4786 "bison_parser.cpp"
    break;

  case 69: /* table_elem: column_def  */
#line 888 "bison_parser.y"
                        { (yyval.table_element_t) = (yyvsp[0].column_t); }
#line 4792 "bison_parser.cpp"
    break;

  case 70: /* table_elem: table_constraint  */
#line 889 "bison_parser.y"
                   { (yyval.table_element_t) = (yyvsp[0].table_constraint_t); }
#line 4798 "bison_parser.cpp"
    break;

  case 71: /* column_def: name_or_keyword column_type opt_column_constraints  */
#line 891 "bison_parser.y"
                                                                {
  (yyval.column_t) = new ColumnDefinition((yyvsp[-2].sval), (yyvsp[-1].column_type_t), (yyvsp[0].column_constraints_t)->constraints, (yyvsp[0].column_constraints_t)->references);
  if (!(yyval.column_t)->trySetNullableExplicit()) {
    yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{(yyvsp[-2].sval)}).c_str());
  }
  delete (yyvsp[0].column_constraints_t);
}
#line 4810 "bison_parser.cpp"
    break;

  case 72: /* column_type: BIGINT  */
#line 899 "bison_parser.y"
                     { (yyval.column_type_t) = ColumnType{DataType::BIGINT}; }
#line 4816 "bison_parser.cpp"
    break;

  case 73: /* column_type: BOOLEAN  */
#line 900 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::BOOLEAN}; }
#line 4822 "bison_parser.cpp"
    break;

  case 74: /* column_type: CHAR '(' INTVAL ')'  */
#line 901 "bison_parser.y"
                      { (yyval.column_type_t) = ColumnType{DataType::CHAR, (yyvsp[-1].ival)}; }
#line 4828 "bison_parser.cpp"
    break;

  case 75: /* column_type: CHARACTER_VARYING '(' INTVAL ')'  */
#line 902 "bison_parser.y"
                                   { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4834 "bison_parser.cpp"
    break;

  case 76: /* column_type: DATE  */
#line 903 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::DATE}; }
#line 4840 "bison_parser.cpp"
    break;

  case 77: /* column_type: DATETIME  */
#line 904 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4846 "bison_parser.cpp"
    break;

  case 78: /* column_type: DECIMAL opt_decimal_specification  */
#line 905 "bison_parser.y"
                                    {
  (yyval.column_type_t) = ColumnType{DataType::DECIMAL, 0, (yyvsp[0].ival_pair)->first, (yyvsp[0].ival_pair)->second};
  delete (yyvsp[0].ival_pair);
}
#line 4855 "bison_parser.cpp"
    break;

  case 79: /* column_type: DOUBLE  */
#line 909 "bison_parser.y"
         { (yyval.column_type_t) = ColumnType{DataType::DOUBLE}; }
#line 4861 "bison_parser.cpp"
    break;

  case 80: /* column_type: FLOAT  */
#line 910 "bison_parser.y"
        { (yyval.column_type_t) = ColumnType{DataType::FLOAT}; }
#line 4867 "bison_parser.cpp"
    break;

  case 81: /* column_type: INT  */
#line 911 "bison_parser.y"
      { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4873 "bison_parser.cpp"
    break;

  case 82: /* column_type: INTEGER  */
#line 912 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4879 "bison_parser.cpp"
    break;

  case 83: /* column_type: INTERVAL  */
#line 913 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::INTERVAL}; }
#line 4885 "bison_parser.cpp"
    break;

  case 84: /* column_type: INTERVAL datetime_field  */
#line 914 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::INTERVAL, 0, 0, 0, (yyvsp[0].datetime_field)}; }
#line 4891 "bison_parser.cpp"
    break;

  case 85: /* column_type: INTERVAL interval_range  */
#line 915 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::INTERVAL, 0, 0, 0, (yyvsp[0].datetime_field)}; }
#line 4897 "bison_parser.cpp"
    break;

  case 86: /* column_type: LONG  */
#line 916 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::LONG}; }
#line 4903 "bison_parser.cpp"
    break;

  case 87: /* column_type: REAL  */
#line 917 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::REAL}; }
#line 4909 "bison_parser.cpp"
    break;

  case 88: /* column_type: SMALLINT  */
#line 918 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::SMALLINT}; }
#line 4915 "bison_parser.cpp"
    break;

  case 89: /* column_type: TEXT  */
#line 919 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::TEXT}; }
#line 4921 "bison_parser.cpp"
    break;

  case 90: /* column_type: TIME opt_time_precision  */
#line 920 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::TIME, 0, (yyvsp[0].ival)}; }
#line 4927 "bison_parser.cpp"
    break;

  case 91: /* column_type: TIMESTAMP  */
#line 921 "bison_parser.y"
            { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4933 "bison_parser.cpp"
    break;

  case 92: /* column_type: VARCHAR '(' INTVAL ')'  */
#line 922 "bison_parser.y"
                         { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4939 "bison_parser.cpp"
    break;

  case 93: /* opt_time_precision: '(' INTVAL ')'  */
#line 924 "bison_parser.y"
                                    { (yyval.ival) = (yyvsp[-1].ival); }
#line 4945 "bison_parser.cpp"
    break;

  case 94: /* opt_time_precision: %empty  */
#line 925 "bison_parser.y"
              { (yyval.ival) = 0; }
#line 4951 "bison_parser.cpp"
    break;

  case 95: /* opt_decimal_specification: '(' INTVAL ',' INTVAL ')'  */
#line 927 "bison_parser.y"
                                                      { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-3].ival), (yyvsp[-1].ival)}; }
#line 4957 "bison_parser.cpp"
    break;

  case 96: /* opt_decimal_specification: '(' INTVAL ')'  */
#line 928 "bison_parser.y"
                 { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-1].ival), 0}; }
#line 4963 "bison_parser.cpp"
    break;

  case 97: /* opt_decimal_specification: %empty  */
#line 929 "bison_parser.y"
              { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{0, 0}; }
#line 4969 "bison_parser.cpp"
    break;

  case 98: /* opt_column_constraints: column_constraints  */
#line 931 "bison_parser.y"
                                            { (yyval.column_constraints_t) = (yyvsp[0].column_constraints_t); }
#line 4975 "bison_parser.cpp"
    break;

  case 99: /* opt_column_constraints: %empty  */
#line 932 "bison_parser.y"
              { (yyval.column_constraints_t) = new ColumnConstraints(); }
#line 4981 "bison_parser.cpp"
    break;

  case 100: /* column_constraints: column_constraint  */
#line 934 "bison_parser.y"
                                       {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
}
#line 4990 "bison_parser.cpp"
    break;

  case 101: /* column_constraints: column_constraints column_constraint  */
#line 938 "bison_parser.y"
                                       {
  (yyvsp[-1].column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4999 "bison_parser.cpp"
    break;

  case 102: /* column_constraints: references_spec  */
#line 942 "bison_parser.y"
                  {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyval.column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
}
#line 5009 "bison_parser.cpp"
    break;

  case 103: /* column_constraints: column_constraints references_spec  */
#line 947 "bison_parser.y"
                                     {
  // Multiple foreign keys for the same column could be possible, so we do not raise an error in that case.
  // Think of foreign keys referenced on multiple levels (returned item references sold item references items).
  (yyvsp[-1].column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyvsp[-1].column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 5021 "bison_parser.cpp"
    break;

  case 104: /* column_constraint: PRIMARY KEY  */
#line 955 "bison_parser.y"
                                { (yyval.column_constraint_t) = ConstraintType::PrimaryKey; }
#line 5027 "bison_parser.cpp"
    break;

  case 105: /* column_constraint: UNIQUE  */
#line 956 "bison_parser.y"
         { (yyval.column_constraint_t) = ConstraintType::Unique; }
#line 5033 "bison_parser.cpp"
    break;

  case 106: /* column_constraint: NULL  */
#line 957 "bison_parser.y"
       { (yyval.column_constraint_t) = ConstraintType::Null; }
#line 5039 "bison_parser.cpp"
    break;

  case 107: /* column_constraint: NOT NULL  */
#line 958 "bison_parser.y"
           { (yyval.column_constraint_t) = ConstraintType::NotNull; }
#line 5045 "bison_parser.cpp"
    break;

  case 108: /* table_constraint: PRIMARY KEY '(' ident_commalist ')'  */
#line 960 "bison_parser.y"
                                                       { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::PrimaryKey, (yyvsp[-1].str_vec)); }
#line 5051 "bison_parser.cpp"
    break;

  case 109: /* table_constraint: UNIQUE '(' ident_commalist ')'  */
#line 961 "bison_parser.y"
                                 { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::Unique, (yyvsp[-1].str_vec)); }
#line 5057 "bison_parser.cpp"
    break;

  case 110: /* table_constraint: FOREIGN KEY '(' ident_commalist ')' references_spec  */
#line 962 "bison_parser.y"
                                                      { (yyval.table_constraint_t) = new ForeignKeyConstraint((yyvsp[-2].str_vec), (yyvsp[0].references_spec_t)); }
#line 5063 "bison_parser.cpp"
    break;

  case 111: /* references_spec: REFERENCES table_name opt_column_list  */
#line 964 "bison_parser.y"
                                                        { (yyval.references_spec_t) = new ReferencesSpecification((yyvsp[-1].table_name).schema, (yyvsp[-1].table_name).name, (yyvsp[0].str_vec)); }
#line 5069 "bison_parser.cpp"
    break;

  case 112: /* drop_statement: DROP TABLE opt_exists table_name  */
#line 972 "bison_parser.y"
                                                  {
  (yyval.drop_stmt) = new DropStatement(kDropTable);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 5080 "bison_parser.cpp"
    break;

  case 113: /* drop_statement: DROP VIEW opt_exists table_name  */
#line 978 "bison_parser.y"
                                  {
  (yyval.drop_stmt) = new DropStatement(kDropView);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 5091 "bison_parser.cpp"
    break;

  case 114: /* drop_statement: DEALLOCATE PREPARE IDENTIFIER  */
#line 984 "bison_parser.y"
                                {
  (yyval.drop_stmt) = new DropStatement(kDropPreparedStatement);
  (yyval.drop_stmt)->ifExists = false;
  (yyval.drop_stmt)->name = (yyvsp[0].sval);
}
#line 5101 "bison_parser.cpp"
    break;

  case 115: /* drop_statement: DROP INDEX opt_exists name_or_keyword  */
#line 990 "bison_parser.y"
                                        {
  (yyval.drop_stmt) = new DropStatement(kDropIndex);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->indexName = (yyvsp[0].sval);
}
#line 5111 "bison_parser.cpp"
    break;

  case 116: /* opt_exists: IF EXISTS  */
#line 996 "bison_parser.y"
                       { (yyval.bval) = true; }
#line 5117 "bison_parser.cpp"
    break;

  case 117: /* opt_exists: %empty  */
#line 997 "bison_parser.y"
              { (yyval.bval) = false; }
#line 5123 "bison_parser.cpp"
    break;

  case 118: /* alter_statement: ALTER TABLE opt_exists table_name alter_action  */
#line 1004 "bison_parser.y"
                                                                 {
  (yyval.alter_stmt) = new AlterStatement((yyvsp[-1].table_name).name, (yyvsp[0].alter_action_t));
  (yyval.alter_stmt)->ifTableExists = (yyvsp[-2].bval);
  (yyval.alter_stmt)->schema = (yyvsp[-1].table_name).schema;
}
#line 5133 "bison_parser.cpp"
    break;

  case 119: /* alter_action: drop_action  */
#line 1010 "bison_parser.y"
                           { (yyval.alter_action_t) = (yyvsp[0].drop_action_t); }
#line 5139 "bison_parser.cpp"
    break;

  case 120: /* drop_action: DROP COLUMN opt_exists name_or_keyword  */
#line 1012 "bison_parser.y"
                                                     {
  (yyval.drop_action_t) = new DropColumnAction((yyvsp[0].sval));
  (yyval.drop_action_t)->ifExists = (yyvsp[-1].bval);
}
#line 5148 "bison_parser.cpp"
    break;

  case 121: /* delete_statement: DELETE FROM table_name opt_where  */
#line 1022 "bison_parser.y"
                                                    {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[-1].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[-1].table_name).name;
  (yyval.delete_stmt)->expr = (yyvsp[0].expr);
}
#line 5159 "bison_parser.cpp"
    break;

  case 122: /* truncate_statement: TRUNCATE table_name  */
#line 1029 "bison_parser.y"
                                         {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 5169 "bison_parser.cpp"
    break;

  case 123: /* insert_statement: INSERT INTO table_name opt_column_list VALUES '(' extended_literal_list ')'  */
#line 1040 "bison_parser.y"
                                                                                               {
  (yyval.insert_stmt) = new InsertStatement(kInsertValues);
  (yyval.insert_stmt)->schema = (yyvsp[-5].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-5].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-4].str_vec);
  (yyval.insert_stmt)->values = (yyvsp[-1].expr_vec);
}
#line 5181 "bison_parser.cpp"
    break;

  case 124: /* insert_statement: INSERT INTO table_name query_expression  */
#line 1050 "bison_parser.y"
                                          {
  (yyval.insert_stmt) = new InsertStatement(kInsertSelect);
  (yyval.insert_stmt)->schema = (yyvsp[-1].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-1].table_name).name;
  (yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
}
#line 5192 "bison_parser.cpp"
    break;

  case 125: /* insert_statement: INSERT INTO table_name '(' ident_commalist ')' query_expression  */
#line 1056 "bison_parser.y"
                                                                  {
  (yyval.insert_stmt) = new InsertStatement(kInsertSelect);
  (yyval.insert_stmt)->schema = (yyvsp[-4].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-4].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-2].str_vec);
  (yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
}
#line 5204 "bison_parser.cpp"
    break;

  case 126: /* opt_column_list: '(' ident_commalist ')'  */
#line 1064 "bison_parser.y"
                                          { (yyval.str_vec) = (yyvsp[-1].str_vec); }
#line 5210 "bison_parser.cpp"
    break;

  case 127: /* opt_column_list: %empty  */
#line 1065 "bison_parser.y"
              { (yyval.str_vec) = nullptr; }
#line 5216 "bison_parser.cpp"
    break;

  case 128: /* update_statement: UPDATE table_ref_name_no_alias SET update_clause_commalist opt_where  */
#line 1072 "bison_parser.y"
                                                                                        {
  (yyval.update_stmt) = new UpdateStatement();
  (yyval.update_stmt)->table = (yyvsp[-3].table);
  (yyval.update_stmt)->updates = (yyvsp[-1].update_vec);
  (yyval.update_stmt)->where = (yyvsp[0].expr);
}
#line 5227 "bison_parser.cpp"
    break;

  case 129: /* update_clause_commalist: update_clause  */
#line 1079 "bison_parser.y"
                                        {
  (yyval.update_vec) = new std::vector<UpdateClause*>();
  (yyval.update_vec)->push_back((yyvsp[0].update_t));
}
#line 5236 "bison_parser.cpp"
    break;

  case 130: /* update_clause_commalist: update_clause_commalist ',' update_clause  */
#line 1083 "bison_parser.y"
                                            {
  (yyvsp[-2].update_vec)->push_back((yyvsp[0].update_t));
  (yyval.update_vec) = (yyvsp[-2].update_vec);
}
#line 5245 "bison_parser.cpp"
    break;

  case 131: /* update_clause: name_or_keyword '=' expr  */
#line 1088 "bison_parser.y"
                                         {
  (yyval.update_t) = new UpdateClause();
  (yyval.update_t)->column = (yyvsp[-2].sval);
  (yyval.update_t)->value = (yyvsp[0].expr);
}
#line 5255 "bison_parser.cpp"
    break;

  case 132: /* query_expression: query_expression_body opt_order opt_limit opt_locking_clause  */
#line 1103 "bison_parser.y"
                                                                                {
  (yyval.select_stmt) = (yyvsp[-3].query_body_t).head;
  if (const char* error = attachQueryClauses((yyvsp[-3].query_body_t), nullptr, (yyvsp[-2].order_vec), (yyvsp[-1].limit), (yyvsp[0].locking_clause_vec))) {
    delete (yyval.select_stmt);
    deleteQueryClauses(nullptr, (yyvsp[-2].order_vec), (yyvsp[-1].limit), (yyvsp[0].locking_clause_vec));
    yyerror(&yyloc, result, scanner, error);
    YYERROR;
  }
}
#line 5269 "bison_parser.cpp"
    break;

  case 133: /* query_expression: with_clause query_expression_body opt_order opt_limit opt_locking_clause  */
#line 1112 "bison_parser.y"
                                                                           {
  (yyval.select_stmt) = (yyvsp[-3].query_body_t).head;
  if (const char* error = attachQueryClauses((yyvsp[-3].query_body_t), (yyvsp[-4].with_description_vec), (yyvsp[-2].order_vec), (yyvsp[-1].limit), (yyvsp[0].locking_clause_vec))) {
    delete (yyval.select_stmt);
    deleteQueryClauses((yyvsp[-4].with_description_vec), (yyvsp[-2].order_vec), (yyvsp[-1].limit), (yyvsp[0].locking_clause_vec));
    yyerror(&yyloc, result, scanner, error);
    YYERROR;
  }
}
#line 5283 "bison_parser.cpp"
    break;

  case 135: /* query_expression_body: query_expression_body union_operator query_term  */
#line 1132 "bison_parser.y"
                                                  {
  if ((yyvsp[-2].query_body_t).head->withDescriptions) {
    delete (yyvsp[-2].query_body_t).head;
    delete (yyvsp[-1].set_operator_t);
    delete (yyvsp[0].query_body_t).head;
    yyerror(&yyloc, result, scanner, "A WITH clause in parentheses cannot be followed by a set operation.");
    YYERROR;
  }
  (yyvsp[-1].set_operator_t)->nestedSelectStatement = (yyvsp[0].query_body_t).head;
  addSetOperation((yyvsp[-2].query_body_t).head, (yyvsp[-1].set_operator_t));
  (yyval.query_body_t) = QueryBody{(yyvsp[-2].query_body_t).head, false};
}
#line 5300 "bison_parser.cpp"
    break;

  case 137: /* query_term: query_term intersect_operator query_member  */
#line 1146 "bison_parser.y"
                                             {
  if ((yyvsp[-2].query_body_t).head->withDescriptions) {
    delete (yyvsp[-2].query_body_t).head;
    delete (yyvsp[-1].set_operator_t);
    delete (yyvsp[0].query_body_t).head;
    yyerror(&yyloc, result, scanner, "A WITH clause in parentheses cannot be followed by a set operation.");
    YYERROR;
  }
  (yyvsp[-1].set_operator_t)->nestedSelectStatement = (yyvsp[0].query_body_t).head;
  addSetOperation((yyvsp[-2].query_body_t).head, (yyvsp[-1].set_operator_t));
  (yyval.query_body_t) = QueryBody{(yyvsp[-2].query_body_t).head, false};
}
#line 5317 "bison_parser.cpp"
    break;

  case 138: /* query_member: query_primary  */
#line 1159 "bison_parser.y"
                             { (yyval.query_body_t) = QueryBody{(yyvsp[0].select_stmt), false}; }
#line 5323 "bison_parser.cpp"
    break;

  case 139: /* query_member: query_expression_parens  */
#line 1163 "bison_parser.y"
                                                 { (yyval.query_body_t) = QueryBody{(yyvsp[0].select_stmt), true}; }
#line 5329 "bison_parser.cpp"
    break;

  case 140: /* query_expression_parens: '(' query_expression ')'  */
#line 1165 "bison_parser.y"
                                                   { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 5335 "bison_parser.cpp"
    break;

  case 141: /* query_expression_parens: '(' query_expression_parens ')'  */
#line 1166 "bison_parser.y"
                                  { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 5341 "bison_parser.cpp"
    break;

  case 143: /* union_operator: UNION opt_all  */
#line 1174 "bison_parser.y"
                               {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetUnion;
  (yyval.set_operator_t)->isAll = (yyvsp[0].bval);
}
#line 5351 "bison_parser.cpp"
    break;

  case 144: /* union_operator: EXCEPT opt_all  */
#line 1179 "bison_parser.y"
                 {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetExcept;
  (yyval.set_operator_t)->isAll = (yyvsp[0].bval);
}
#line 5361 "bison_parser.cpp"
    break;

  case 145: /* intersect_operator: INTERSECT opt_all  */
#line 1185 "bison_parser.y"
                                       {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetIntersect;
  (yyval.set_operator_t)->isAll = (yyvsp[0].bval);
}
#line 5371 "bison_parser.cpp"
    break;

  case 146: /* opt_all: ALL  */
#line 1191 "bison_parser.y"
              { (yyval.bval) = true; }
#line 5377 "bison_parser.cpp"
    break;

  case 147: /* opt_all: %empty  */
#line 1192 "bison_parser.y"
              { (yyval.bval) = false; }
#line 5383 "bison_parser.cpp"
    break;

  case 148: /* opt_hierarchical_clause: START_WITH expr connect_by expr  */
#line 1196 "bison_parser.y"
                                                          { (yyval.hierarchical_t) = HierarchicalClause{(yyvsp[-2].expr), (yyvsp[0].expr)}; }
#line 5389 "bison_parser.cpp"
    break;

  case 149: /* opt_hierarchical_clause: connect_by expr START_WITH expr  */
#line 1197 "bison_parser.y"
                                  { (yyval.hierarchical_t) = HierarchicalClause{(yyvsp[0].expr), (yyvsp[-2].expr)}; }
#line 5395 "bison_parser.cpp"
    break;

  case 150: /* opt_hierarchical_clause: START_WITH expr  */
#line 1198 "bison_parser.y"
                  { (yyval.hierarchical_t) = HierarchicalClause{(yyvsp[0].expr), nullptr}; }
#line 5401 "bison_parser.cpp"
    break;

  case 151: /* opt_hierarchical_clause: connect_by expr  */
#line 1199 "bison_parser.y"
                  { (yyval.hierarchical_t) = HierarchicalClause{nullptr, (yyvsp[0].expr)}; }
#line 5407 "bison_parser.cpp"
    break;

  case 152: /* opt_hierarchical_clause: %empty  */
#line 1200 "bison_parser.y"
              { (yyval.hierarchical_t) = HierarchicalClause{nullptr, nullptr}; }
#line 5413 "bison_parser.cpp"
    break;

  case 155: /* query_primary: SELECT opt_top opt_distinct select_list opt_from_clause opt_where opt_hierarchical_clause opt_group  */
#line 1206 "bison_parser.y"
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
#line 5429 "bison_parser.cpp"
    break;

  case 156: /* opt_distinct: DISTINCT  */
#line 1218 "bison_parser.y"
                        { (yyval.bval) = true; }
#line 5435 "bison_parser.cpp"
    break;

  case 157: /* opt_distinct: %empty  */
#line 1219 "bison_parser.y"
              { (yyval.bval) = false; }
#line 5441 "bison_parser.cpp"
    break;

  case 159: /* opt_from_clause: from_clause  */
#line 1223 "bison_parser.y"
                              { (yyval.table) = (yyvsp[0].table); }
#line 5447 "bison_parser.cpp"
    break;

  case 160: /* opt_from_clause: %empty  */
#line 1224 "bison_parser.y"
              { (yyval.table) = nullptr; }
#line 5453 "bison_parser.cpp"
    break;

  case 161: /* from_clause: FROM table_ref  */
#line 1226 "bison_parser.y"
                             { (yyval.table) = (yyvsp[0].table); }
#line 5459 "bison_parser.cpp"
    break;

  case 162: /* opt_where: WHERE expr  */
#line 1228 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 5465 "bison_parser.cpp"
    break;

  case 163: /* opt_where: %empty  */
#line 1229 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 5471 "bison_parser.cpp"
    break;

  case 164: /* opt_group: GROUP BY expr_list opt_having  */
#line 1231 "bison_parser.y"
                                          {
  (yyval.group_t) = new GroupByDescription();
  (yyval.group_t)->columns = (yyvsp[-1].expr_vec);
  (yyval.group_t)->having = (yyvsp[0].expr);
}
#line 5481 "bison_parser.cpp"
    break;

  case 165: /* opt_group: %empty  */
#line 1236 "bison_parser.y"
              { (yyval.group_t) = nullptr; }
#line 5487 "bison_parser.cpp"
    break;

  case 166: /* opt_having: HAVING expr  */
#line 1238 "bison_parser.y"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 5493 "bison_parser.cpp"
    break;

  case 167: /* opt_having: %empty  */
#line 1239 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 5499 "bison_parser.cpp"
    break;

  case 168: /* opt_within_group: WITHIN_GROUP '(' ORDER BY order_list ')'  */
#line 1244 "bison_parser.y"
                                                            { (yyval.order_vec) = (yyvsp[-1].order_vec); }
#line 5505 "bison_parser.cpp"
    break;

  case 169: /* opt_within_group: %empty  */
#line 1245 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 5511 "bison_parser.cpp"
    break;

  case 170: /* opt_order: ORDER BY order_list  */
#line 1247 "bison_parser.y"
                                { (yyval.order_vec) = (yyvsp[0].order_vec); }
#line 5517 "bison_parser.cpp"
    break;

  case 171: /* opt_order: %empty  */
#line 1248 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 5523 "bison_parser.cpp"
    break;

  case 172: /* order_list: order_desc  */
#line 1250 "bison_parser.y"
                        {
  (yyval.order_vec) = new std::vector<OrderDescription*>();
  (yyval.order_vec)->push_back((yyvsp[0].order));
}
#line 5532 "bison_parser.cpp"
    break;

  case 173: /* order_list: order_list ',' order_desc  */
#line 1254 "bison_parser.y"
                            {
  (yyvsp[-2].order_vec)->push_back((yyvsp[0].order));
  (yyval.order_vec) = (yyvsp[-2].order_vec);
}
#line 5541 "bison_parser.cpp"
    break;

  case 174: /* order_desc: expr opt_order_type opt_null_ordering  */
#line 1259 "bison_parser.y"
                                                   { (yyval.order) = new OrderDescription((yyvsp[-1].order_type), (yyvsp[-2].expr), (yyvsp[0].null_ordering_t)); }
#line 5547 "bison_parser.cpp"
    break;

  case 175: /* opt_order_type: ASC  */
#line 1261 "bison_parser.y"
                     { (yyval.order_type) = kOrderAsc; }
#line 5553 "bison_parser.cpp"
    break;

  case 176: /* opt_order_type: DESC  */
#line 1262 "bison_parser.y"
       { (yyval.order_type) = kOrderDesc; }
#line 5559 "bison_parser.cpp"
    break;

  case 177: /* opt_order_type: %empty  */
#line 1263 "bison_parser.y"
              { (yyval.order_type) = kOrderAsc; }
#line 5565 "bison_parser.cpp"
    break;

  case 178: /* opt_null_ordering: %empty  */
#line 1265 "bison_parser.y"
                                { (yyval.null_ordering_t) = NullOrdering::Undefined; }
#line 5571 "bison_parser.cpp"
    break;

  case 179: /* opt_null_ordering: IDENTIFIER IDENTIFIER  */
#line 1266 "bison_parser.y"
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
#line 5595 "bison_parser.cpp"
    break;

  case 180: /* opt_top: TOP int_literal  */
#line 1290 "bison_parser.y"
                          { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 5601 "bison_parser.cpp"
    break;

  case 181: /* opt_top: TOP '(' expr ')'  */
#line 1291 "bison_parser.y"
                   { (yyval.limit) = new LimitDescription((yyvsp[-1].expr), nullptr); }
#line 5607 "bison_parser.cpp"
    break;

  case 182: /* opt_top: %empty  */
#line 1292 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 5613 "bison_parser.cpp"
    break;

  case 183: /* opt_limit: LIMIT expr  */
#line 1294 "bison_parser.y"
                       { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 5619 "bison_parser.cpp"
    break;

  case 184: /* opt_limit: OFFSET expr  */
#line 1295 "bison_parser.y"
              { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 5625 "bison_parser.cpp"
    break;

  case 185: /* opt_limit: LIMIT expr OFFSET expr  */
#line 1296 "bison_parser.y"
                         { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 5631 "bison_parser.cpp"
    break;

  case 186: /* opt_limit: LIMIT ALL  */
#line 1297 "bison_parser.y"
            { (yyval.limit) = new LimitDescription(nullptr, nullptr); }
#line 5637 "bison_parser.cpp"
    break;

  case 187: /* opt_limit: LIMIT ALL OFFSET expr  */
#line 1298 "bison_parser.y"
                        { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 5643 "bison_parser.cpp"
    break;

  case 188: /* opt_limit: OFFSET expr ROWS FETCH NEXT expr ROWS ONLY  */
#line 1299 "bison_parser.y"
                                             { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[-6].expr)); }
#line 5649 "bison_parser.cpp"
    break;

  case 189: /* opt_limit: %empty  */
#line 1300 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 5655 "bison_parser.cpp"
    break;

  case 190: /* expr_list: expr_alias  */
#line 1305 "bison_parser.y"
                       {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5664 "bison_parser.cpp"
    break;

  case 191: /* expr_list: expr_list ',' expr_alias  */
#line 1309 "bison_parser.y"
                           {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5673 "bison_parser.cpp"
    break;

  case 192: /* opt_extended_literal_list: extended_literal_list  */
#line 1315 "bison_parser.y"
                                                  { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5679 "bison_parser.cpp"
    break;

  case 193: /* opt_extended_literal_list: %empty  */
#line 1316 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5685 "bison_parser.cpp"
    break;

  case 194: /* extended_literal_list: casted_extended_literal  */
#line 1318 "bison_parser.y"
                                                {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5694 "bison_parser.cpp"
    break;

  case 195: /* extended_literal_list: extended_literal_list ',' casted_extended_literal  */
#line 1322 "bison_parser.y"
                                                    {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5703 "bison_parser.cpp"
    break;

  case 197: /* casted_extended_literal: CAST '(' extended_literal AS column_type ')'  */
#line 1327 "bison_parser.y"
                                                                                          {
  (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t));
}
#line 5711 "bison_parser.cpp"
    break;

  case 198: /* extended_literal: literal  */
#line 1331 "bison_parser.y"
                           { (yyval.expr) = (yyvsp[0].expr); }
#line 5717 "bison_parser.cpp"
    break;

  case 199: /* extended_literal: '-' num_literal  */
#line 1332 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5723 "bison_parser.cpp"
    break;

  case 200: /* extended_literal: '-' interval_literal  */
#line 1333 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5729 "bison_parser.cpp"
    break;

  case 201: /* expr_alias: expr opt_alias  */
#line 1335 "bison_parser.y"
                            {
  (yyval.expr) = (yyvsp[-1].expr);
  if ((yyvsp[0].alias_t)) {
    (yyval.expr)->alias = (yyvsp[0].alias_t)->name;
    (yyvsp[0].alias_t)->name = nullptr;
    delete (yyvsp[0].alias_t);
  }
}
#line 5742 "bison_parser.cpp"
    break;

  case 207: /* operand: '(' expr ')'  */
#line 1346 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[-1].expr); }
#line 5748 "bison_parser.cpp"
    break;

  case 218: /* operand: subquery  */
#line 1348 "bison_parser.y"
                          {
  (yyval.expr) = Expr::makeSelect((yyvsp[0].select_stmt));
}
#line 5756 "bison_parser.cpp"
    break;

  case 221: /* scalar_expr: column_name OUTERJOIN  */
#line 1356 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpUnary(kOpOuterJoin, (yyvsp[-1].expr)); }
#line 5762 "bison_parser.cpp"
    break;

  case 222: /* unary_expr: '-' operand  */
#line 1358 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5768 "bison_parser.cpp"
    break;

  case 223: /* unary_expr: PRIOR operand  */
#line 1360 "bison_parser.y"
                { (yyval.expr) = Expr::makeOpUnary(kOpPrior, (yyvsp[0].expr)); }
#line 5774 "bison_parser.cpp"
    break;

  case 224: /* unary_expr: NOT operand  */
#line 1361 "bison_parser.y"
              { (yyval.expr) = Expr::makeOpUnary(kOpNot, (yyvsp[0].expr)); }
#line 5780 "bison_parser.cpp"
    break;

  case 225: /* unary_expr: operand ISNULL  */
#line 1362 "bison_parser.y"
                 { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-1].expr)); }
#line 5786 "bison_parser.cpp"
    break;

  case 226: /* unary_expr: operand IS NULL  */
#line 1363 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-2].expr)); }
#line 5792 "bison_parser.cpp"
    break;

  case 227: /* unary_expr: operand IS NOT NULL  */
#line 1364 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeOpUnary(kOpIsNull, (yyvsp[-3].expr))); }
#line 5798 "bison_parser.cpp"
    break;

  case 229: /* binary_expr: operand '-' operand  */
#line 1366 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpMinus, (yyvsp[0].expr)); }
#line 5804 "bison_parser.cpp"
    break;

  case 230: /* binary_expr: operand '+' operand  */
#line 1367 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPlus, (yyvsp[0].expr)); }
#line 5810 "bison_parser.cpp"
    break;

  case 231: /* binary_expr: operand '/' operand  */
#line 1368 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpSlash, (yyvsp[0].expr)); }
#line 5816 "bison_parser.cpp"
    break;

  case 232: /* binary_expr: operand '*' operand  */
#line 1369 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAsterisk, (yyvsp[0].expr)); }
#line 5822 "bison_parser.cpp"
    break;

  case 233: /* binary_expr: operand '%' operand  */
#line 1370 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPercentage, (yyvsp[0].expr)); }
#line 5828 "bison_parser.cpp"
    break;

  case 234: /* binary_expr: operand '^' operand  */
#line 1371 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpCaret, (yyvsp[0].expr)); }
#line 5834 "bison_parser.cpp"
    break;

  case 235: /* binary_expr: operand LIKE operand  */
#line 1372 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLike, (yyvsp[0].expr)); }
#line 5840 "bison_parser.cpp"
    break;

  case 236: /* binary_expr: operand NOT LIKE operand  */
#line 1373 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-3].expr), kOpNotLike, (yyvsp[0].expr)); }
#line 5846 "bison_parser.cpp"
    break;

  case 237: /* binary_expr: operand ILIKE operand  */
#line 1374 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpILike, (yyvsp[0].expr)); }
#line 5852 "bison_parser.cpp"
    break;

  case 238: /* binary_expr: operand CONCAT operand  */
#line 1375 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpConcat, (yyvsp[0].expr)); }
#line 5858 "bison_parser.cpp"
    break;

  case 239: /* logic_expr: expr AND expr  */
#line 1377 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAnd, (yyvsp[0].expr)); }
#line 5864 "bison_parser.cpp"
    break;

  case 240: /* logic_expr: expr OR expr  */
#line 1378 "bison_parser.y"
               { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpOr, (yyvsp[0].expr)); }
#line 5870 "bison_parser.cpp"
    break;

  case 241: /* in_expr: operand IN '(' expr_list ')'  */
#line 1380 "bison_parser.y"
                                       { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].expr_vec)); }
#line 5876 "bison_parser.cpp"
    break;

  case 242: /* in_expr: operand NOT IN '(' expr_list ')'  */
#line 1381 "bison_parser.y"
                                   { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].expr_vec))); }
#line 5882 "bison_parser.cpp"
    break;

  case 243: /* in_expr: operand IN subquery  */
#line 1382 "bison_parser.y"
                      { (yyval.expr) = Expr::makeInOperator((yyvsp[-2].expr), (yyvsp[0].select_stmt)); }
#line 5888 "bison_parser.cpp"
    break;

  case 244: /* in_expr: operand NOT IN subquery  */
#line 1383 "bison_parser.y"
                          { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-3].expr), (yyvsp[0].select_stmt))); }
#line 5894 "bison_parser.cpp"
    break;

  case 245: /* in_expr: '(' row_expr_list ')' IN subquery  */
#line 1387 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeInOperator(Expr::makeArray((yyvsp[-3].expr_vec)), (yyvsp[0].select_stmt)); }
#line 5900 "bison_parser.cpp"
    break;

  case 246: /* in_expr: '(' row_expr_list ')' NOT IN subquery  */
#line 1388 "bison_parser.y"
                                        { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator(Expr::makeArray((yyvsp[-4].expr_vec)), (yyvsp[0].select_stmt))); }
#line 5906 "bison_parser.cpp"
    break;

  case 247: /* in_expr: '(' row_expr_list ')' IN '(' in_row_list ')'  */
#line 1390 "bison_parser.y"
                                               { (yyval.expr) = Expr::makeInOperator(Expr::makeArray((yyvsp[-5].expr_vec)), (yyvsp[-1].expr_vec)); }
#line 5912 "bison_parser.cpp"
    break;

  case 248: /* in_expr: '(' row_expr_list ')' NOT IN '(' in_row_list ')'  */
#line 1391 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator(Expr::makeArray((yyvsp[-6].expr_vec)), (yyvsp[-1].expr_vec))); }
#line 5918 "bison_parser.cpp"
    break;

  case 249: /* in_row_list: '(' row_expr_list ')'  */
#line 1395 "bison_parser.y"
                                    {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
}
#line 5927 "bison_parser.cpp"
    break;

  case 250: /* in_row_list: in_row_list ',' '(' row_expr_list ')'  */
#line 1399 "bison_parser.y"
                                        {
  (yyvsp[-4].expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
  (yyval.expr_vec) = (yyvsp[-4].expr_vec);
}
#line 5936 "bison_parser.cpp"
    break;

  case 251: /* row_expr_list: expr ',' expr  */
#line 1406 "bison_parser.y"
                              {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[-2].expr));
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5946 "bison_parser.cpp"
    break;

  case 252: /* row_expr_list: row_expr_list ',' expr  */
#line 1411 "bison_parser.y"
                         {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5955 "bison_parser.cpp"
    break;

  case 253: /* case_expr: CASE expr case_list END  */
#line 1418 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-2].expr), (yyvsp[-1].expr), nullptr); }
#line 5961 "bison_parser.cpp"
    break;

  case 254: /* case_expr: CASE expr case_list ELSE expr END  */
#line 1419 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-4].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5967 "bison_parser.cpp"
    break;

  case 255: /* case_expr: CASE case_list END  */
#line 1420 "bison_parser.y"
                     { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-1].expr), nullptr); }
#line 5973 "bison_parser.cpp"
    break;

  case 256: /* case_expr: CASE case_list ELSE expr END  */
#line 1421 "bison_parser.y"
                               { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5979 "bison_parser.cpp"
    break;

  case 257: /* case_list: WHEN expr THEN expr  */
#line 1423 "bison_parser.y"
                                { (yyval.expr) = Expr::makeCaseList(Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5985 "bison_parser.cpp"
    break;

  case 258: /* case_list: case_list WHEN expr THEN expr  */
#line 1424 "bison_parser.y"
                                { (yyval.expr) = Expr::caseListAppend((yyvsp[-4].expr), Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5991 "bison_parser.cpp"
    break;

  case 259: /* exists_expr: EXISTS subquery  */
#line 1426 "bison_parser.y"
                              { (yyval.expr) = Expr::makeExists((yyvsp[0].select_stmt)); }
#line 5997 "bison_parser.cpp"
    break;

  case 260: /* exists_expr: NOT EXISTS subquery  */
#line 1427 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeExists((yyvsp[0].select_stmt))); }
#line 6003 "bison_parser.cpp"
    break;

  case 261: /* comp_expr: operand '=' operand  */
#line 1429 "bison_parser.y"
                                { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 6009 "bison_parser.cpp"
    break;

  case 262: /* comp_expr: operand EQUALS operand  */
#line 1430 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 6015 "bison_parser.cpp"
    break;

  case 263: /* comp_expr: operand NOTEQUALS operand  */
#line 1431 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpNotEquals, (yyvsp[0].expr)); }
#line 6021 "bison_parser.cpp"
    break;

  case 264: /* comp_expr: operand '<' operand  */
#line 1432 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLess, (yyvsp[0].expr)); }
#line 6027 "bison_parser.cpp"
    break;

  case 265: /* comp_expr: operand '>' operand  */
#line 1433 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreater, (yyvsp[0].expr)); }
#line 6033 "bison_parser.cpp"
    break;

  case 266: /* comp_expr: operand LESSEQ operand  */
#line 1434 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLessEq, (yyvsp[0].expr)); }
#line 6039 "bison_parser.cpp"
    break;

  case 267: /* comp_expr: operand GREATEREQ operand  */
#line 1435 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreaterEq, (yyvsp[0].expr)); }
#line 6045 "bison_parser.cpp"
    break;

  case 268: /* table_function_expr: IDENTIFIER '(' ')'  */
#line 1439 "bison_parser.y"
                                         {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-2].sval), new std::vector<Expr*>(), false, nullptr);
}
#line 6053 "bison_parser.cpp"
    break;

  case 269: /* table_function_expr: IDENTIFIER '(' opt_distinct expr_list ')'  */
#line 1442 "bison_parser.y"
                                            {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-4].sval), (yyvsp[-1].expr_vec), (yyvsp[-2].bval), nullptr);
}
#line 6061 "bison_parser.cpp"
    break;

  case 270: /* table_function_expr: IDENTIFIER '.' name_after_qualifier '(' ')'  */
#line 1445 "bison_parser.y"
                                              {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-2].sval), (yyvsp[-4].sval), new std::vector<Expr*>(), false, nullptr);
}
#line 6069 "bison_parser.cpp"
    break;

  case 271: /* table_function_expr: IDENTIFIER '.' name_after_qualifier '(' opt_distinct expr_list ')'  */
#line 1448 "bison_parser.y"
                                                                     {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-4].sval), (yyvsp[-6].sval), (yyvsp[-1].expr_vec), (yyvsp[-2].bval), nullptr);
}
#line 6077 "bison_parser.cpp"
    break;

  case 272: /* table_function_expr: nonreserved_keyword '(' ')'  */
#line 1453 "bison_parser.y"
                              {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-2].sval), new std::vector<Expr*>(), false, nullptr);
}
#line 6085 "bison_parser.cpp"
    break;

  case 273: /* table_function_expr: nonreserved_keyword '(' opt_distinct expr_list ')'  */
#line 1456 "bison_parser.y"
                                                     {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-4].sval), (yyvsp[-1].expr_vec), (yyvsp[-2].bval), nullptr);
}
#line 6093 "bison_parser.cpp"
    break;

  case 274: /* function_expr: table_function_expr opt_within_group opt_window  */
#line 1463 "bison_parser.y"
                                                                {
  (yyval.expr) = (yyvsp[-2].expr);
  (yyval.expr)->withinGroupOrder = (yyvsp[-1].order_vec);
  (yyval.expr)->windowDescription = (yyvsp[0].window_description);
}
#line 6103 "bison_parser.cpp"
    break;

  case 275: /* opt_window: OVER '(' opt_partition opt_order opt_frame_clause ')'  */
#line 1471 "bison_parser.y"
                                                                   { (yyval.window_description) = new WindowDescription((yyvsp[-3].expr_vec), (yyvsp[-2].order_vec), (yyvsp[-1].frame_description)); }
#line 6109 "bison_parser.cpp"
    break;

  case 276: /* opt_window: %empty  */
#line 1472 "bison_parser.y"
              { (yyval.window_description) = nullptr; }
#line 6115 "bison_parser.cpp"
    break;

  case 277: /* opt_partition: PARTITION BY expr_list  */
#line 1474 "bison_parser.y"
                                       { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 6121 "bison_parser.cpp"
    break;

  case 278: /* opt_partition: %empty  */
#line 1475 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 6127 "bison_parser.cpp"
    break;

  case 279: /* opt_frame_clause: frame_type frame_bound  */
#line 1480 "bison_parser.y"
                                          { (yyval.frame_description) = new FrameDescription{(yyvsp[-1].frame_type), (yyvsp[0].frame_bound), new FrameBound{0, kCurrentRow, false}}; }
#line 6133 "bison_parser.cpp"
    break;

  case 280: /* opt_frame_clause: frame_type BETWEEN frame_bound AND frame_bound  */
#line 1481 "bison_parser.y"
                                                 { (yyval.frame_description) = new FrameDescription{(yyvsp[-4].frame_type), (yyvsp[-2].frame_bound), (yyvsp[0].frame_bound)}; }
#line 6139 "bison_parser.cpp"
    break;

  case 281: /* opt_frame_clause: %empty  */
#line 1482 "bison_parser.y"
              {
  (yyval.frame_description) = new FrameDescription{kRange, new FrameBound{0, kPreceding, true}, new FrameBound{0, kCurrentRow, false}};
}
#line 6147 "bison_parser.cpp"
    break;

  case 282: /* frame_type: RANGE  */
#line 1486 "bison_parser.y"
                   { (yyval.frame_type) = kRange; }
#line 6153 "bison_parser.cpp"
    break;

  case 283: /* frame_type: ROWS  */
#line 1487 "bison_parser.y"
       { (yyval.frame_type) = kRows; }
#line 6159 "bison_parser.cpp"
    break;

  case 284: /* frame_type: GROUPS  */
#line 1488 "bison_parser.y"
         { (yyval.frame_type) = kGroups; }
#line 6165 "bison_parser.cpp"
    break;

  case 285: /* frame_bound: UNBOUNDED PRECEDING  */
#line 1490 "bison_parser.y"
                                  { (yyval.frame_bound) = new FrameBound{0, kPreceding, true}; }
#line 6171 "bison_parser.cpp"
    break;

  case 286: /* frame_bound: INTVAL PRECEDING  */
#line 1491 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kPreceding, false}; }
#line 6177 "bison_parser.cpp"
    break;

  case 287: /* frame_bound: UNBOUNDED FOLLOWING  */
#line 1492 "bison_parser.y"
                      { (yyval.frame_bound) = new FrameBound{0, kFollowing, true}; }
#line 6183 "bison_parser.cpp"
    break;

  case 288: /* frame_bound: INTVAL FOLLOWING  */
#line 1493 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kFollowing, false}; }
#line 6189 "bison_parser.cpp"
    break;

  case 289: /* frame_bound: CURRENT_ROW  */
#line 1494 "bison_parser.y"
              { (yyval.frame_bound) = new FrameBound{0, kCurrentRow, false}; }
#line 6195 "bison_parser.cpp"
    break;

  case 290: /* extract_expr: EXTRACT '(' datetime_field FROM expr ')'  */
#line 1496 "bison_parser.y"
                                                        { (yyval.expr) = Expr::makeExtract((yyvsp[-3].datetime_field), (yyvsp[-1].expr)); }
#line 6201 "bison_parser.cpp"
    break;

  case 291: /* next_value_expr: NEXT_VALUE_FOR table_name  */
#line 1501 "bison_parser.y"
                                            {
  auto args = new std::vector<Expr*>();
  args->push_back(Expr::makeColumnRef(qualifiedName((yyvsp[0].table_name))));
  (yyval.expr) = Expr::makeFunctionRef(strdup("NEXT VALUE FOR"), args, false, nullptr);
}
#line 6211 "bison_parser.cpp"
    break;

  case 292: /* cast_expr: CAST '(' expr AS column_type ')'  */
#line 1507 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t)); }
#line 6217 "bison_parser.cpp"
    break;

  case 293: /* datetime_field: SECOND  */
#line 1509 "bison_parser.y"
                        { (yyval.datetime_field) = kDatetimeSecond; }
#line 6223 "bison_parser.cpp"
    break;

  case 294: /* datetime_field: MINUTE  */
#line 1510 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMinute; }
#line 6229 "bison_parser.cpp"
    break;

  case 295: /* datetime_field: HOUR  */
#line 1511 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeHour; }
#line 6235 "bison_parser.cpp"
    break;

  case 296: /* datetime_field: DAY  */
#line 1512 "bison_parser.y"
      { (yyval.datetime_field) = kDatetimeDay; }
#line 6241 "bison_parser.cpp"
    break;

  case 297: /* datetime_field: MONTH  */
#line 1513 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeMonth; }
#line 6247 "bison_parser.cpp"
    break;

  case 298: /* datetime_field: YEAR  */
#line 1514 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeYear; }
#line 6253 "bison_parser.cpp"
    break;

  case 299: /* datetime_field_plural: SECONDS  */
#line 1516 "bison_parser.y"
                                { (yyval.datetime_field) = kDatetimeSecond; }
#line 6259 "bison_parser.cpp"
    break;

  case 300: /* datetime_field_plural: MINUTES  */
#line 1517 "bison_parser.y"
          { (yyval.datetime_field) = kDatetimeMinute; }
#line 6265 "bison_parser.cpp"
    break;

  case 301: /* datetime_field_plural: HOURS  */
#line 1518 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeHour; }
#line 6271 "bison_parser.cpp"
    break;

  case 302: /* datetime_field_plural: DAYS  */
#line 1519 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeDay; }
#line 6277 "bison_parser.cpp"
    break;

  case 303: /* datetime_field_plural: MONTHS  */
#line 1520 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMonth; }
#line 6283 "bison_parser.cpp"
    break;

  case 304: /* datetime_field_plural: YEARS  */
#line 1521 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeYear; }
#line 6289 "bison_parser.cpp"
    break;

  case 307: /* interval_range: datetime_field TO datetime_field  */
#line 1529 "bison_parser.y"
                                                  {
  if ((yyvsp[-2].datetime_field) == kDatetimeYear && (yyvsp[0].datetime_field) == kDatetimeMonth) {
    (yyval.datetime_field) = kIntervalYearToMonth;
  } else if ((yyvsp[-2].datetime_field) == kDatetimeDay && (yyvsp[0].datetime_field) == kDatetimeHour) {
    (yyval.datetime_field) = kIntervalDayToHour;
  } else if ((yyvsp[-2].datetime_field) == kDatetimeDay && (yyvsp[0].datetime_field) == kDatetimeMinute) {
    (yyval.datetime_field) = kIntervalDayToMinute;
  } else if ((yyvsp[-2].datetime_field) == kDatetimeDay && (yyvsp[0].datetime_field) == kDatetimeSecond) {
    (yyval.datetime_field) = kIntervalDayToSecond;
  } else if ((yyvsp[-2].datetime_field) == kDatetimeHour && (yyvsp[0].datetime_field) == kDatetimeMinute) {
    (yyval.datetime_field) = kIntervalHourToMinute;
  } else if ((yyvsp[-2].datetime_field) == kDatetimeHour && (yyvsp[0].datetime_field) == kDatetimeSecond) {
    (yyval.datetime_field) = kIntervalHourToSecond;
  } else if ((yyvsp[-2].datetime_field) == kDatetimeMinute && (yyvsp[0].datetime_field) == kDatetimeSecond) {
    (yyval.datetime_field) = kIntervalMinuteToSecond;
  } else {
    yyerror(&yyloc, result, scanner,
            "Found incorrect interval field range. Expected one of: YEAR TO MONTH, DAY TO HOUR, DAY TO MINUTE, "
            "DAY TO SECOND, HOUR TO MINUTE, HOUR TO SECOND, MINUTE TO SECOND");
    YYERROR;
  }
}
#line 6316 "bison_parser.cpp"
    break;

  case 308: /* array_expr: ARRAY '[' expr_list ']'  */
#line 1552 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeArray((yyvsp[-1].expr_vec)); }
#line 6322 "bison_parser.cpp"
    break;

  case 309: /* array_index: operand '[' int_literal ']'  */
#line 1558 "bison_parser.y"
                                          {
  (yyval.expr) = Expr::makeArrayIndex((yyvsp[-3].expr), (yyvsp[-1].expr)->ival);
  delete (yyvsp[-1].expr);
}
#line 6331 "bison_parser.cpp"
    break;

  case 310: /* between_expr: operand BETWEEN operand AND operand  */
#line 1563 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeBetween((yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 6337 "bison_parser.cpp"
    break;

  case 311: /* column_name: IDENTIFIER  */
#line 1565 "bison_parser.y"
                         { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 6343 "bison_parser.cpp"
    break;

  case 312: /* column_name: IDENTIFIER '.' name_after_qualifier  */
#line 1566 "bison_parser.y"
                                      { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 6349 "bison_parser.cpp"
    break;

  case 313: /* column_name: '*'  */
#line 1567 "bison_parser.y"
      { (yyval.expr) = Expr::makeStar(); }
#line 6355 "bison_parser.cpp"
    break;

  case 314: /* column_name: IDENTIFIER '.' '*'  */
#line 1568 "bison_parser.y"
                     { (yyval.expr) = Expr::makeStar((yyvsp[-2].sval)); }
#line 6361 "bison_parser.cpp"
    break;

  case 315: /* column_name: nonreserved_keyword  */
#line 1569 "bison_parser.y"
                      { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 6367 "bison_parser.cpp"
    break;

  case 316: /* column_name: IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier  */
#line 1578 "bison_parser.y"
                                                               { (yyval.expr) = Expr::makeColumnRef((yyvsp[-4].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 6373 "bison_parser.cpp"
    break;

  case 317: /* column_name: IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier '.' name_after_qualifier  */
#line 1579 "bison_parser.y"
                                                                                        {
  (yyval.expr) = Expr::makeColumnRef(dotJoin((yyvsp[-6].sval), (yyvsp[-4].sval)), (yyvsp[-2].sval), (yyvsp[0].sval));
}
#line 6381 "bison_parser.cpp"
    break;

  case 318: /* column_name: IDENTIFIER '.' name_after_qualifier '.' '*'  */
#line 1582 "bison_parser.y"
                                              {
  (yyval.expr) = Expr::makeStar((yyvsp[-2].sval));
  (yyval.expr)->schema = (yyvsp[-4].sval);
}
#line 6390 "bison_parser.cpp"
    break;

  case 319: /* column_name: IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier '.' '*'  */
#line 1586 "bison_parser.y"
                                                                       {
  (yyval.expr) = Expr::makeStar((yyvsp[-2].sval));
  (yyval.expr)->schema = dotJoin((yyvsp[-6].sval), (yyvsp[-4].sval));
}
#line 6399 "bison_parser.cpp"
    break;

  case 320: /* column_name: IDENTIFIER '.' nonreserved_keyword  */
#line 1592 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 6405 "bison_parser.cpp"
    break;

  case 321: /* column_name: IDENTIFIER '.' name_after_qualifier '.' nonreserved_keyword  */
#line 1593 "bison_parser.y"
                                                              { (yyval.expr) = Expr::makeColumnRef((yyvsp[-4].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 6411 "bison_parser.cpp"
    break;

  case 322: /* column_name: IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier '.' nonreserved_keyword  */
#line 1594 "bison_parser.y"
                                                                                       {
  (yyval.expr) = Expr::makeColumnRef(dotJoin((yyvsp[-6].sval), (yyvsp[-4].sval)), (yyvsp[-2].sval), (yyvsp[0].sval));
}
#line 6419 "bison_parser.cpp"
    break;

  case 323: /* nonreserved_keyword: SECOND  */
#line 1602 "bison_parser.y"
                             { (yyval.sval) = strdup("SECOND"); }
#line 6425 "bison_parser.cpp"
    break;

  case 324: /* nonreserved_keyword: MINUTE  */
#line 1603 "bison_parser.y"
         { (yyval.sval) = strdup("MINUTE"); }
#line 6431 "bison_parser.cpp"
    break;

  case 325: /* nonreserved_keyword: HOUR  */
#line 1604 "bison_parser.y"
       { (yyval.sval) = strdup("HOUR"); }
#line 6437 "bison_parser.cpp"
    break;

  case 326: /* nonreserved_keyword: DAY  */
#line 1605 "bison_parser.y"
      { (yyval.sval) = strdup("DAY"); }
#line 6443 "bison_parser.cpp"
    break;

  case 327: /* nonreserved_keyword: MONTH  */
#line 1606 "bison_parser.y"
        { (yyval.sval) = strdup("MONTH"); }
#line 6449 "bison_parser.cpp"
    break;

  case 328: /* nonreserved_keyword: YEAR  */
#line 1607 "bison_parser.y"
       { (yyval.sval) = strdup("YEAR"); }
#line 6455 "bison_parser.cpp"
    break;

  case 329: /* nonreserved_keyword: SECONDS  */
#line 1608 "bison_parser.y"
          { (yyval.sval) = strdup("SECONDS"); }
#line 6461 "bison_parser.cpp"
    break;

  case 330: /* nonreserved_keyword: MINUTES  */
#line 1609 "bison_parser.y"
          { (yyval.sval) = strdup("MINUTES"); }
#line 6467 "bison_parser.cpp"
    break;

  case 331: /* nonreserved_keyword: HOURS  */
#line 1610 "bison_parser.y"
        { (yyval.sval) = strdup("HOURS"); }
#line 6473 "bison_parser.cpp"
    break;

  case 332: /* nonreserved_keyword: DAYS  */
#line 1611 "bison_parser.y"
       { (yyval.sval) = strdup("DAYS"); }
#line 6479 "bison_parser.cpp"
    break;

  case 333: /* nonreserved_keyword: MONTHS  */
#line 1612 "bison_parser.y"
         { (yyval.sval) = strdup("MONTHS"); }
#line 6485 "bison_parser.cpp"
    break;

  case 334: /* nonreserved_keyword: YEARS  */
#line 1613 "bison_parser.y"
        { (yyval.sval) = strdup("YEARS"); }
#line 6491 "bison_parser.cpp"
    break;

  case 335: /* nonreserved_keyword: ISNULL  */
#line 1614 "bison_parser.y"
         { (yyval.sval) = strdup("ISNULL"); }
#line 6497 "bison_parser.cpp"
    break;

  case 336: /* nonreserved_keyword: FORMAT  */
#line 1615 "bison_parser.y"
         { (yyval.sval) = strdup("FORMAT"); }
#line 6503 "bison_parser.cpp"
    break;

  case 337: /* nonreserved_keyword: CHAR  */
#line 1616 "bison_parser.y"
       { (yyval.sval) = strdup("CHAR"); }
#line 6509 "bison_parser.cpp"
    break;

  case 338: /* nonreserved_keyword: VARCHAR  */
#line 1617 "bison_parser.y"
          { (yyval.sval) = strdup("VARCHAR"); }
#line 6515 "bison_parser.cpp"
    break;

  case 339: /* nonreserved_keyword: INT  */
#line 1618 "bison_parser.y"
      { (yyval.sval) = strdup("INT"); }
#line 6521 "bison_parser.cpp"
    break;

  case 340: /* nonreserved_keyword: INTEGER  */
#line 1619 "bison_parser.y"
          { (yyval.sval) = strdup("INTEGER"); }
#line 6527 "bison_parser.cpp"
    break;

  case 341: /* nonreserved_keyword: DATETIME  */
#line 1620 "bison_parser.y"
           { (yyval.sval) = strdup("DATETIME"); }
#line 6533 "bison_parser.cpp"
    break;

  case 342: /* nonreserved_keyword: TIMESTAMP  */
#line 1621 "bison_parser.y"
            { (yyval.sval) = strdup("TIMESTAMP"); }
#line 6539 "bison_parser.cpp"
    break;

  case 343: /* nonreserved_keyword: NEXT  */
#line 1622 "bison_parser.y"
       { (yyval.sval) = strdup("NEXT"); }
#line 6545 "bison_parser.cpp"
    break;

  case 345: /* name_after_qualifier: CURRENT_DATE  */
#line 1630 "bison_parser.y"
               { (yyval.sval) = strdup("CURRENT_DATE"); }
#line 6551 "bison_parser.cpp"
    break;

  case 346: /* name_after_qualifier: CURRENT_TIME  */
#line 1631 "bison_parser.y"
               { (yyval.sval) = strdup("CURRENT_TIME"); }
#line 6557 "bison_parser.cpp"
    break;

  case 347: /* name_after_qualifier: CURRENT_TIMESTAMP  */
#line 1632 "bison_parser.y"
                    { (yyval.sval) = strdup("CURRENT_TIMESTAMP"); }
#line 6563 "bison_parser.cpp"
    break;

  case 361: /* string_literal: STRING  */
#line 1645 "bison_parser.y"
                        { (yyval.expr) = Expr::makeLiteral((yyvsp[0].sval)); }
#line 6569 "bison_parser.cpp"
    break;

  case 362: /* bool_literal: TRUE  */
#line 1647 "bison_parser.y"
                    { (yyval.expr) = Expr::makeLiteral(true); }
#line 6575 "bison_parser.cpp"
    break;

  case 363: /* bool_literal: FALSE  */
#line 1648 "bison_parser.y"
        { (yyval.expr) = Expr::makeLiteral(false); }
#line 6581 "bison_parser.cpp"
    break;

  case 364: /* num_literal: FLOATVAL  */
#line 1650 "bison_parser.y"
                       { (yyval.expr) = Expr::makeLiteral((yyvsp[0].fval)); }
#line 6587 "bison_parser.cpp"
    break;

  case 366: /* int_literal: INTVAL  */
#line 1653 "bison_parser.y"
                     { (yyval.expr) = Expr::makeLiteral((yyvsp[0].ival)); }
#line 6593 "bison_parser.cpp"
    break;

  case 367: /* null_literal: NULL  */
#line 1655 "bison_parser.y"
                    { (yyval.expr) = Expr::makeNullLiteral(); }
#line 6599 "bison_parser.cpp"
    break;

  case 368: /* date_literal: DATE STRING  */
#line 1657 "bison_parser.y"
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
#line 6614 "bison_parser.cpp"
    break;

  case 369: /* date_literal: CURRENT_DATE  */
#line 1669 "bison_parser.y"
               { (yyval.expr) = Expr::makeDateLiteral(nullptr); }
#line 6620 "bison_parser.cpp"
    break;

  case 370: /* date_literal: CURRENT_DATE '(' ')'  */
#line 1670 "bison_parser.y"
                       { (yyval.expr) = Expr::makeDateLiteral(nullptr); }
#line 6626 "bison_parser.cpp"
    break;

  case 371: /* time_literal: TIME STRING  */
#line 1676 "bison_parser.y"
                           { (yyval.expr) = Expr::makeTimeLiteral((yyvsp[0].sval)); }
#line 6632 "bison_parser.cpp"
    break;

  case 372: /* time_literal: CURRENT_TIME  */
#line 1677 "bison_parser.y"
               { (yyval.expr) = Expr::makeTimeLiteral(nullptr); }
#line 6638 "bison_parser.cpp"
    break;

  case 373: /* time_literal: CURRENT_TIME '(' ')'  */
#line 1678 "bison_parser.y"
                       { (yyval.expr) = Expr::makeTimeLiteral(nullptr); }
#line 6644 "bison_parser.cpp"
    break;

  case 374: /* time_literal: CURRENT_TIME '(' INTVAL ')'  */
#line 1679 "bison_parser.y"
                              {
  if ((yyvsp[-1].ival) < 0) {
    yyerror(&yyloc, result, scanner, "The precision of CURRENT_TIME cannot be negative.");
    YYERROR;
  }
  (yyval.expr) = Expr::makeTimeLiteral(nullptr);
  (yyval.expr)->ival = (yyvsp[-1].ival);
}
#line 6657 "bison_parser.cpp"
    break;

  case 375: /* timestamp_literal: TIMESTAMP STRING  */
#line 1688 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeTimestampLiteral((yyvsp[0].sval)); }
#line 6663 "bison_parser.cpp"
    break;

  case 376: /* timestamp_literal: CURRENT_TIMESTAMP  */
#line 1689 "bison_parser.y"
                    { (yyval.expr) = Expr::makeTimestampLiteral(nullptr); }
#line 6669 "bison_parser.cpp"
    break;

  case 377: /* timestamp_literal: CURRENT_TIMESTAMP '(' ')'  */
#line 1690 "bison_parser.y"
                            { (yyval.expr) = Expr::makeTimestampLiteral(nullptr); }
#line 6675 "bison_parser.cpp"
    break;

  case 378: /* timestamp_literal: CURRENT_TIMESTAMP '(' INTVAL ')'  */
#line 1691 "bison_parser.y"
                                   {
  if ((yyvsp[-1].ival) < 0) {
    yyerror(&yyloc, result, scanner, "The precision of CURRENT_TIMESTAMP cannot be negative.");
    YYERROR;
  }
  (yyval.expr) = Expr::makeTimestampLiteral(nullptr);
  (yyval.expr)->ival = (yyvsp[-1].ival);
}
#line 6688 "bison_parser.cpp"
    break;

  case 379: /* interval_literal: INTVAL duration_field  */
#line 1700 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeIntervalLiteral((yyvsp[-1].ival), (yyvsp[0].datetime_field)); }
#line 6694 "bison_parser.cpp"
    break;

  case 380: /* interval_literal: INTERVAL STRING datetime_field  */
#line 1701 "bison_parser.y"
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
#line 6710 "bison_parser.cpp"
    break;

  case 381: /* interval_literal: INTERVAL STRING interval_range  */
#line 1714 "bison_parser.y"
                                 { (yyval.expr) = Expr::makeIntervalRangeLiteral((yyvsp[-1].sval), (yyvsp[0].datetime_field)); }
#line 6716 "bison_parser.cpp"
    break;

  case 382: /* interval_literal: INTERVAL STRING  */
#line 1715 "bison_parser.y"
                  {
  int duration{0}, chars_parsed{0};
  // 'seconds' and 'minutes' are the longest accepted interval qualifiers (7 chars) + null byte
  char unit_string[8];
  // If the whole string is parsed, chars_parsed points to the terminating null byte after the last character
  if (sscanf((yyvsp[0].sval), "%d %7s%n", &duration, unit_string, &chars_parsed) != 2 || (yyvsp[0].sval)[chars_parsed] != 0) {
    free((yyvsp[0].sval));
    yyerror(&yyloc, result, scanner, "Found incorrect interval format. Expected format: INTEGER INTERVAL_QUALIFIER");
    YYERROR;
  }
  free((yyvsp[0].sval));

  DatetimeField unit = kDatetimeNone;
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
#line 6752 "bison_parser.cpp"
    break;

  case 383: /* param_expr: '?'  */
#line 1747 "bison_parser.y"
                 {
  (yyval.expr) = Expr::makeParameter(yylloc.total_column);
  (yyval.expr)->ival2 = yylloc.total_column - 1;  // source column (0-based) of the '?' token
  yyloc.param_list.push_back((yyval.expr));
}
#line 6762 "bison_parser.cpp"
    break;

  case 384: /* param_expr: DOLLAR_PARAM  */
#line 1752 "bison_parser.y"
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
#line 6779 "bison_parser.cpp"
    break;

  case 385: /* param_expr: NAMED_PARAM  */
#line 1764 "bison_parser.y"
              {
  (yyval.expr) = Expr::makeNamedParameter((yyvsp[0].sval));
  (yyval.expr)->ival2 = yylloc.total_column - 1 - (int64_t)strlen((yyvsp[0].sval));
  yyloc.param_list.push_back((yyval.expr));
}
#line 6789 "bison_parser.cpp"
    break;

  case 387: /* table_ref: table_ref_commalist ',' table_ref_atomic  */
#line 1773 "bison_parser.y"
                                                                        {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  auto tbl = new TableRef(kTableCrossProduct);
  tbl->list = (yyvsp[-2].table_vec);
  (yyval.table) = tbl;
}
#line 6800 "bison_parser.cpp"
    break;

  case 390: /* table_ref_atomic: ODBC_OJ table_ref_atomic '}'  */
#line 1784 "bison_parser.y"
                               { (yyval.table) = (yyvsp[-1].table); }
#line 6806 "bison_parser.cpp"
    break;

  case 392: /* nonjoin_table_ref_atomic: subquery opt_table_alias  */
#line 1786 "bison_parser.y"
                                                                     {
  auto tbl = new TableRef(kTableSelect);
  tbl->select = (yyvsp[-1].select_stmt);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6817 "bison_parser.cpp"
    break;

  case 393: /* nonjoin_table_ref_atomic: table_function_expr opt_table_alias  */
#line 1794 "bison_parser.y"
                                      {
  auto tbl = new TableRef(kTableFunc);
  tbl->func = (yyvsp[-1].expr);
  tbl->name = strdup((yyvsp[-1].expr)->name);
  if ((yyvsp[-1].expr)->schema) tbl->schema = strdup((yyvsp[-1].expr)->schema);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6830 "bison_parser.cpp"
    break;

  case 394: /* nonjoin_table_ref_atomic: '(' VALUES table_value_row_list ')' opt_table_alias  */
#line 1804 "bison_parser.y"
                                                      {
  auto tbl = new TableRef(kTableValues);
  tbl->values = (yyvsp[-2].expr_vec);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6841 "bison_parser.cpp"
    break;

  case 395: /* table_value_row_list: '(' expr_list ')'  */
#line 1813 "bison_parser.y"
                                         {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
}
#line 6850 "bison_parser.cpp"
    break;

  case 396: /* table_value_row_list: table_value_row_list ',' '(' expr_list ')'  */
#line 1817 "bison_parser.y"
                                             {
  (yyvsp[-4].expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
  (yyval.expr_vec) = (yyvsp[-4].expr_vec);
}
#line 6859 "bison_parser.cpp"
    break;

  case 397: /* table_ref_commalist: table_ref_atomic  */
#line 1822 "bison_parser.y"
                                       {
  (yyval.table_vec) = new std::vector<TableRef*>();
  (yyval.table_vec)->push_back((yyvsp[0].table));
}
#line 6868 "bison_parser.cpp"
    break;

  case 398: /* table_ref_commalist: table_ref_commalist ',' table_ref_atomic  */
#line 1826 "bison_parser.y"
                                           {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  (yyval.table_vec) = (yyvsp[-2].table_vec);
}
#line 6877 "bison_parser.cpp"
    break;

  case 399: /* table_ref_name: table_name opt_table_alias  */
#line 1831 "bison_parser.y"
                                            {
  auto tbl = new TableRef(kTableName);
  tbl->schema = (yyvsp[-1].table_name).schema;
  tbl->name = (yyvsp[-1].table_name).name;
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6889 "bison_parser.cpp"
    break;

  case 400: /* table_ref_name_no_alias: table_name  */
#line 1839 "bison_parser.y"
                                     {
  (yyval.table) = new TableRef(kTableName);
  (yyval.table)->schema = (yyvsp[0].table_name).schema;
  (yyval.table)->name = (yyvsp[0].table_name).name;
}
#line 6899 "bison_parser.cpp"
    break;

  case 401: /* table_name: name_or_keyword  */
#line 1849 "bison_parser.y"
                             {
  (yyval.table_name).schema = nullptr;
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6908 "bison_parser.cpp"
    break;

  case 402: /* table_name: IDENTIFIER '.' qualified_name_or_keyword  */
#line 1853 "bison_parser.y"
                                           {
  (yyval.table_name).schema = (yyvsp[-2].sval);
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6917 "bison_parser.cpp"
    break;

  case 403: /* table_name: nonreserved_keyword '.' qualified_name_or_keyword  */
#line 1857 "bison_parser.y"
                                                    {
  (yyval.table_name).schema = (yyvsp[-2].sval);
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6926 "bison_parser.cpp"
    break;

  case 404: /* table_name: nonreserved_keyword '.' qualified_name_or_keyword '.' qualified_name_or_keyword  */
#line 1861 "bison_parser.y"
                                                                                  {
  (yyval.table_name).schema = dotJoin((yyvsp[-4].sval), (yyvsp[-2].sval));
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6935 "bison_parser.cpp"
    break;

  case 405: /* table_name: IDENTIFIER '.' name_after_qualifier '.' qualified_name_or_keyword  */
#line 1865 "bison_parser.y"
                                                                    {
  // Three-part (database.schema.table) name. TableName has no separate
  // database slot, so fold database+schema into schema as "database.schema" -
  // callers here only need the statement to parse, not the individual parts.
  (yyval.table_name).schema = dotJoin((yyvsp[-4].sval), (yyvsp[-2].sval));
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6947 "bison_parser.cpp"
    break;

  case 406: /* opt_index_name: name_or_keyword  */
#line 1873 "bison_parser.y"
                                 { (yyval.sval) = (yyvsp[0].sval); }
#line 6953 "bison_parser.cpp"
    break;

  case 407: /* opt_index_name: %empty  */
#line 1874 "bison_parser.y"
              { (yyval.sval) = nullptr; }
#line 6959 "bison_parser.cpp"
    break;

  case 409: /* table_alias: AS name_or_keyword '(' ident_commalist ')'  */
#line 1876 "bison_parser.y"
                                                                 { (yyval.alias_t) = new Alias((yyvsp[-3].sval), (yyvsp[-1].str_vec)); }
#line 6965 "bison_parser.cpp"
    break;

  case 411: /* opt_table_alias: %empty  */
#line 1878 "bison_parser.y"
                                            { (yyval.alias_t) = nullptr; }
#line 6971 "bison_parser.cpp"
    break;

  case 412: /* alias: AS IDENTIFIER  */
#line 1880 "bison_parser.y"
                      { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6977 "bison_parser.cpp"
    break;

  case 413: /* alias: AS nonreserved_keyword  */
#line 1884 "bison_parser.y"
                         { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6983 "bison_parser.cpp"
    break;

  case 414: /* alias: AS STRING  */
#line 1888 "bison_parser.y"
            { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6989 "bison_parser.cpp"
    break;

  case 415: /* alias: IDENTIFIER  */
#line 1889 "bison_parser.y"
             { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6995 "bison_parser.cpp"
    break;

  case 417: /* opt_alias: %empty  */
#line 1891 "bison_parser.y"
                                { (yyval.alias_t) = nullptr; }
#line 7001 "bison_parser.cpp"
    break;

  case 418: /* opt_locking_clause: opt_locking_clause_list  */
#line 1897 "bison_parser.y"
                                             { (yyval.locking_clause_vec) = (yyvsp[0].locking_clause_vec); }
#line 7007 "bison_parser.cpp"
    break;

  case 419: /* opt_locking_clause: %empty  */
#line 1898 "bison_parser.y"
              { (yyval.locking_clause_vec) = nullptr; }
#line 7013 "bison_parser.cpp"
    break;

  case 420: /* opt_locking_clause_list: locking_clause  */
#line 1900 "bison_parser.y"
                                         {
  (yyval.locking_clause_vec) = new std::vector<LockingClause*>();
  (yyval.locking_clause_vec)->push_back((yyvsp[0].locking_t));
}
#line 7022 "bison_parser.cpp"
    break;

  case 421: /* opt_locking_clause_list: opt_locking_clause_list locking_clause  */
#line 1904 "bison_parser.y"
                                         {
  (yyvsp[-1].locking_clause_vec)->push_back((yyvsp[0].locking_t));
  (yyval.locking_clause_vec) = (yyvsp[-1].locking_clause_vec);
}
#line 7031 "bison_parser.cpp"
    break;

  case 422: /* locking_clause: FOR row_lock_mode opt_row_lock_policy  */
#line 1909 "bison_parser.y"
                                                       {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-1].lock_mode_t);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
  (yyval.locking_t)->tables = nullptr;
}
#line 7042 "bison_parser.cpp"
    break;

  case 423: /* locking_clause: FOR row_lock_mode OF ident_commalist opt_row_lock_policy  */
#line 1915 "bison_parser.y"
                                                           {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-3].lock_mode_t);
  (yyval.locking_t)->tables = (yyvsp[-1].str_vec);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
}
#line 7053 "bison_parser.cpp"
    break;

  case 424: /* row_lock_mode: UPDATE  */
#line 1922 "bison_parser.y"
                       { (yyval.lock_mode_t) = RowLockMode::ForUpdate; }
#line 7059 "bison_parser.cpp"
    break;

  case 425: /* row_lock_mode: NO KEY UPDATE  */
#line 1923 "bison_parser.y"
                { (yyval.lock_mode_t) = RowLockMode::ForNoKeyUpdate; }
#line 7065 "bison_parser.cpp"
    break;

  case 426: /* row_lock_mode: SHARE  */
#line 1924 "bison_parser.y"
        { (yyval.lock_mode_t) = RowLockMode::ForShare; }
#line 7071 "bison_parser.cpp"
    break;

  case 427: /* row_lock_mode: KEY SHARE  */
#line 1925 "bison_parser.y"
            { (yyval.lock_mode_t) = RowLockMode::ForKeyShare; }
#line 7077 "bison_parser.cpp"
    break;

  case 428: /* opt_row_lock_policy: SKIP LOCKED  */
#line 1927 "bison_parser.y"
                                  { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::SkipLocked; }
#line 7083 "bison_parser.cpp"
    break;

  case 429: /* opt_row_lock_policy: NOWAIT  */
#line 1928 "bison_parser.y"
         { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::NoWait; }
#line 7089 "bison_parser.cpp"
    break;

  case 430: /* opt_row_lock_policy: %empty  */
#line 1929 "bison_parser.y"
              { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::None; }
#line 7095 "bison_parser.cpp"
    break;

  case 431: /* with_clause: WITH with_description_list  */
#line 1935 "bison_parser.y"
                                         { (yyval.with_description_vec) = (yyvsp[0].with_description_vec); }
#line 7101 "bison_parser.cpp"
    break;

  case 432: /* with_description_list: with_description  */
#line 1937 "bison_parser.y"
                                         {
  (yyval.with_description_vec) = new std::vector<WithDescription*>();
  (yyval.with_description_vec)->push_back((yyvsp[0].with_description_t));
}
#line 7110 "bison_parser.cpp"
    break;

  case 433: /* with_description_list: with_description_list ',' with_description  */
#line 1941 "bison_parser.y"
                                             {
  (yyvsp[-2].with_description_vec)->push_back((yyvsp[0].with_description_t));
  (yyval.with_description_vec) = (yyvsp[-2].with_description_vec);
}
#line 7119 "bison_parser.cpp"
    break;

  case 434: /* with_description: name_or_keyword AS subquery  */
#line 1946 "bison_parser.y"
                                               {
  // The query expression of a WITH description cannot carry a WITH clause of
  // its own.
  if ((yyvsp[0].select_stmt)->withDescriptions) {
    free((yyvsp[-2].sval));
    delete (yyvsp[0].select_stmt);
    yyerror(&yyloc, result, scanner, "Nested CTE is not allowed.");
    YYERROR;
  }
  (yyval.with_description_t) = new WithDescription();
  (yyval.with_description_t)->alias = (yyvsp[-2].sval);
  (yyval.with_description_t)->select = (yyvsp[0].select_stmt);
}
#line 7137 "bison_parser.cpp"
    break;

  case 435: /* join_clause: table_ref_atomic NATURAL JOIN nonjoin_table_ref_atomic  */
#line 1964 "bison_parser.y"
                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = kJoinNatural;
  (yyval.table)->join->left = (yyvsp[-3].table);
  (yyval.table)->join->right = (yyvsp[0].table);
}
#line 7149 "bison_parser.cpp"
    break;

  case 436: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic ON join_condition  */
#line 1971 "bison_parser.y"
                                                                         {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (JoinType)(yyvsp[-4].join_type);
  (yyval.table)->join->left = (yyvsp[-5].table);
  (yyval.table)->join->right = (yyvsp[-2].table);
  (yyval.table)->join->condition = (yyvsp[0].expr);
}
#line 7162 "bison_parser.cpp"
    break;

  case 437: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic USING '(' ident_commalist ')'  */
#line 1979 "bison_parser.y"
                                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (yyvsp[-6].join_type);
  (yyval.table)->join->left = (yyvsp[-7].table);
  (yyval.table)->join->right = (yyvsp[-4].table);
  (yyval.table)->join->namedColumns = (yyvsp[-1].str_vec);
}
#line 7175 "bison_parser.cpp"
    break;

  case 438: /* opt_join_type: INNER  */
#line 1988 "bison_parser.y"
                      { (yyval.join_type) = kJoinInner; }
#line 7181 "bison_parser.cpp"
    break;

  case 439: /* opt_join_type: LEFT OUTER  */
#line 1989 "bison_parser.y"
             { (yyval.join_type) = kJoinLeft; }
#line 7187 "bison_parser.cpp"
    break;

  case 440: /* opt_join_type: LEFT  */
#line 1990 "bison_parser.y"
       { (yyval.join_type) = kJoinLeft; }
#line 7193 "bison_parser.cpp"
    break;

  case 441: /* opt_join_type: RIGHT OUTER  */
#line 1991 "bison_parser.y"
              { (yyval.join_type) = kJoinRight; }
#line 7199 "bison_parser.cpp"
    break;

  case 442: /* opt_join_type: RIGHT  */
#line 1992 "bison_parser.y"
        { (yyval.join_type) = kJoinRight; }
#line 7205 "bison_parser.cpp"
    break;

  case 443: /* opt_join_type: FULL OUTER  */
#line 1993 "bison_parser.y"
             { (yyval.join_type) = kJoinFull; }
#line 7211 "bison_parser.cpp"
    break;

  case 444: /* opt_join_type: OUTER  */
#line 1994 "bison_parser.y"
        { (yyval.join_type) = kJoinFull; }
#line 7217 "bison_parser.cpp"
    break;

  case 445: /* opt_join_type: FULL  */
#line 1995 "bison_parser.y"
       { (yyval.join_type) = kJoinFull; }
#line 7223 "bison_parser.cpp"
    break;

  case 446: /* opt_join_type: CROSS  */
#line 1996 "bison_parser.y"
        { (yyval.join_type) = kJoinCross; }
#line 7229 "bison_parser.cpp"
    break;

  case 447: /* opt_join_type: %empty  */
#line 1997 "bison_parser.y"
                       { (yyval.join_type) = kJoinInner; }
#line 7235 "bison_parser.cpp"
    break;

  case 451: /* ident_commalist: name_or_keyword  */
#line 2008 "bison_parser.y"
                                  {
  (yyval.str_vec) = new std::vector<char*>();
  (yyval.str_vec)->push_back((yyvsp[0].sval));
}
#line 7244 "bison_parser.cpp"
    break;

  case 452: /* ident_commalist: ident_commalist ',' name_or_keyword  */
#line 2012 "bison_parser.y"
                                      {
  (yyvsp[-2].str_vec)->push_back((yyvsp[0].sval));
  (yyval.str_vec) = (yyvsp[-2].str_vec);
}
#line 7253 "bison_parser.cpp"
    break;


#line 7257 "bison_parser.cpp"

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
  while (true)
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

#line 2018 "bison_parser.y"


/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */

    // clang-format on
