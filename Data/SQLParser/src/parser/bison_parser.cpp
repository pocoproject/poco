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
  YYSYMBOL_query_expression_parens = 269,  /* query_expression_parens  */
  YYSYMBOL_subquery = 270,                 /* subquery  */
  YYSYMBOL_set_operator = 271,             /* set_operator  */
  YYSYMBOL_set_type = 272,                 /* set_type  */
  YYSYMBOL_opt_all = 273,                  /* opt_all  */
  YYSYMBOL_opt_hierarchical_clause = 274,  /* opt_hierarchical_clause  */
  YYSYMBOL_connect_by = 275,               /* connect_by  */
  YYSYMBOL_query_primary = 276,            /* query_primary  */
  YYSYMBOL_opt_distinct = 277,             /* opt_distinct  */
  YYSYMBOL_select_list = 278,              /* select_list  */
  YYSYMBOL_opt_from_clause = 279,          /* opt_from_clause  */
  YYSYMBOL_from_clause = 280,              /* from_clause  */
  YYSYMBOL_opt_where = 281,                /* opt_where  */
  YYSYMBOL_opt_group = 282,                /* opt_group  */
  YYSYMBOL_opt_having = 283,               /* opt_having  */
  YYSYMBOL_opt_within_group = 284,         /* opt_within_group  */
  YYSYMBOL_opt_order = 285,                /* opt_order  */
  YYSYMBOL_order_list = 286,               /* order_list  */
  YYSYMBOL_order_desc = 287,               /* order_desc  */
  YYSYMBOL_opt_order_type = 288,           /* opt_order_type  */
  YYSYMBOL_opt_null_ordering = 289,        /* opt_null_ordering  */
  YYSYMBOL_opt_top = 290,                  /* opt_top  */
  YYSYMBOL_opt_limit = 291,                /* opt_limit  */
  YYSYMBOL_expr_list = 292,                /* expr_list  */
  YYSYMBOL_opt_extended_literal_list = 293, /* opt_extended_literal_list  */
  YYSYMBOL_extended_literal_list = 294,    /* extended_literal_list  */
  YYSYMBOL_casted_extended_literal = 295,  /* casted_extended_literal  */
  YYSYMBOL_extended_literal = 296,         /* extended_literal  */
  YYSYMBOL_expr_alias = 297,               /* expr_alias  */
  YYSYMBOL_expr = 298,                     /* expr  */
  YYSYMBOL_operand = 299,                  /* operand  */
  YYSYMBOL_scalar_expr = 300,              /* scalar_expr  */
  YYSYMBOL_unary_expr = 301,               /* unary_expr  */
  YYSYMBOL_binary_expr = 302,              /* binary_expr  */
  YYSYMBOL_logic_expr = 303,               /* logic_expr  */
  YYSYMBOL_in_expr = 304,                  /* in_expr  */
  YYSYMBOL_in_row_list = 305,              /* in_row_list  */
  YYSYMBOL_row_expr_list = 306,            /* row_expr_list  */
  YYSYMBOL_case_expr = 307,                /* case_expr  */
  YYSYMBOL_case_list = 308,                /* case_list  */
  YYSYMBOL_exists_expr = 309,              /* exists_expr  */
  YYSYMBOL_comp_expr = 310,                /* comp_expr  */
  YYSYMBOL_table_function_expr = 311,      /* table_function_expr  */
  YYSYMBOL_function_expr = 312,            /* function_expr  */
  YYSYMBOL_opt_window = 313,               /* opt_window  */
  YYSYMBOL_opt_partition = 314,            /* opt_partition  */
  YYSYMBOL_opt_frame_clause = 315,         /* opt_frame_clause  */
  YYSYMBOL_frame_type = 316,               /* frame_type  */
  YYSYMBOL_frame_bound = 317,              /* frame_bound  */
  YYSYMBOL_extract_expr = 318,             /* extract_expr  */
  YYSYMBOL_next_value_expr = 319,          /* next_value_expr  */
  YYSYMBOL_cast_expr = 320,                /* cast_expr  */
  YYSYMBOL_datetime_field = 321,           /* datetime_field  */
  YYSYMBOL_datetime_field_plural = 322,    /* datetime_field_plural  */
  YYSYMBOL_duration_field = 323,           /* duration_field  */
  YYSYMBOL_interval_range = 324,           /* interval_range  */
  YYSYMBOL_array_expr = 325,               /* array_expr  */
  YYSYMBOL_array_index = 326,              /* array_index  */
  YYSYMBOL_between_expr = 327,             /* between_expr  */
  YYSYMBOL_column_name = 328,              /* column_name  */
  YYSYMBOL_nonreserved_keyword = 329,      /* nonreserved_keyword  */
  YYSYMBOL_name_after_qualifier = 330,     /* name_after_qualifier  */
  YYSYMBOL_qualified_name_or_keyword = 331, /* qualified_name_or_keyword  */
  YYSYMBOL_name_or_keyword = 332,          /* name_or_keyword  */
  YYSYMBOL_literal = 333,                  /* literal  */
  YYSYMBOL_string_literal = 334,           /* string_literal  */
  YYSYMBOL_bool_literal = 335,             /* bool_literal  */
  YYSYMBOL_num_literal = 336,              /* num_literal  */
  YYSYMBOL_int_literal = 337,              /* int_literal  */
  YYSYMBOL_null_literal = 338,             /* null_literal  */
  YYSYMBOL_date_literal = 339,             /* date_literal  */
  YYSYMBOL_time_literal = 340,             /* time_literal  */
  YYSYMBOL_timestamp_literal = 341,        /* timestamp_literal  */
  YYSYMBOL_interval_literal = 342,         /* interval_literal  */
  YYSYMBOL_param_expr = 343,               /* param_expr  */
  YYSYMBOL_table_ref = 344,                /* table_ref  */
  YYSYMBOL_table_ref_atomic = 345,         /* table_ref_atomic  */
  YYSYMBOL_nonjoin_table_ref_atomic = 346, /* nonjoin_table_ref_atomic  */
  YYSYMBOL_table_value_row_list = 347,     /* table_value_row_list  */
  YYSYMBOL_table_ref_commalist = 348,      /* table_ref_commalist  */
  YYSYMBOL_table_ref_name = 349,           /* table_ref_name  */
  YYSYMBOL_table_ref_name_no_alias = 350,  /* table_ref_name_no_alias  */
  YYSYMBOL_table_name = 351,               /* table_name  */
  YYSYMBOL_opt_index_name = 352,           /* opt_index_name  */
  YYSYMBOL_table_alias = 353,              /* table_alias  */
  YYSYMBOL_opt_table_alias = 354,          /* opt_table_alias  */
  YYSYMBOL_alias = 355,                    /* alias  */
  YYSYMBOL_opt_alias = 356,                /* opt_alias  */
  YYSYMBOL_opt_locking_clause = 357,       /* opt_locking_clause  */
  YYSYMBOL_opt_locking_clause_list = 358,  /* opt_locking_clause_list  */
  YYSYMBOL_locking_clause = 359,           /* locking_clause  */
  YYSYMBOL_row_lock_mode = 360,            /* row_lock_mode  */
  YYSYMBOL_opt_row_lock_policy = 361,      /* opt_row_lock_policy  */
  YYSYMBOL_with_clause = 362,              /* with_clause  */
  YYSYMBOL_with_description_list = 363,    /* with_description_list  */
  YYSYMBOL_with_description = 364,         /* with_description  */
  YYSYMBOL_join_clause = 365,              /* join_clause  */
  YYSYMBOL_opt_join_type = 366,            /* opt_join_type  */
  YYSYMBOL_join_condition = 367,           /* join_condition  */
  YYSYMBOL_opt_semicolon = 368,            /* opt_semicolon  */
  YYSYMBOL_ident_commalist = 369           /* ident_commalist  */
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
#define YYFINAL  109
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2721

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  218
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  152
/* YYNRULES -- Number of rules.  */
#define YYNRULES  451
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  819

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
       0,   510,   510,   530,   536,   543,   547,   551,   552,   553,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   571,   572,   574,   578,   583,   587,   597,   598,   599,
     601,   601,   607,   613,   619,   628,   629,   631,   632,   634,
     646,   652,   669,   684,   686,   687,   688,   690,   704,   708,
     718,   722,   746,   754,   767,   774,   789,   809,   810,   815,
     826,   841,   853,   860,   868,   877,   878,   880,   884,   889,
     890,   892,   900,   901,   902,   903,   904,   905,   906,   910,
     911,   912,   913,   914,   915,   916,   917,   918,   919,   920,
     921,   922,   923,   925,   926,   928,   929,   930,   932,   933,
     935,   939,   943,   948,   956,   957,   958,   959,   961,   962,
     963,   965,   973,   979,   985,   991,   997,   998,  1005,  1011,
    1013,  1023,  1030,  1041,  1051,  1057,  1065,  1066,  1073,  1080,
    1084,  1089,  1104,  1113,  1133,  1137,  1138,  1150,  1163,  1164,
    1170,  1172,  1177,  1181,  1185,  1190,  1191,  1195,  1196,  1197,
    1198,  1199,  1203,  1203,  1205,  1217,  1218,  1220,  1222,  1223,
    1225,  1227,  1228,  1230,  1235,  1237,  1238,  1243,  1244,  1246,
    1247,  1249,  1253,  1258,  1260,  1261,  1262,  1264,  1265,  1289,
    1290,  1291,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1304,
    1308,  1314,  1315,  1317,  1321,  1326,  1326,  1330,  1331,  1332,
    1334,  1343,  1343,  1343,  1343,  1343,  1345,  1346,  1346,  1346,
    1346,  1346,  1346,  1346,  1346,  1346,  1347,  1347,  1351,  1351,
    1355,  1357,  1359,  1360,  1361,  1362,  1363,  1365,  1365,  1366,
    1367,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1376,  1377,
    1379,  1380,  1381,  1382,  1386,  1387,  1389,  1390,  1394,  1398,
    1405,  1410,  1417,  1418,  1419,  1420,  1422,  1423,  1425,  1426,
    1428,  1429,  1430,  1431,  1432,  1433,  1434,  1438,  1441,  1444,
    1447,  1452,  1455,  1462,  1470,  1471,  1473,  1474,  1479,  1480,
    1481,  1485,  1486,  1487,  1489,  1490,  1491,  1492,  1493,  1495,
    1500,  1506,  1508,  1509,  1510,  1511,  1512,  1513,  1515,  1516,
    1517,  1518,  1519,  1520,  1522,  1522,  1528,  1551,  1557,  1562,
    1564,  1565,  1566,  1567,  1568,  1577,  1578,  1581,  1585,  1591,
    1592,  1593,  1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,
    1609,  1610,  1611,  1612,  1613,  1614,  1615,  1616,  1617,  1618,
    1619,  1620,  1621,  1628,  1629,  1630,  1631,  1634,  1634,  1639,
    1639,  1641,  1641,  1641,  1641,  1641,  1641,  1641,  1642,  1642,
    1644,  1646,  1647,  1649,  1650,  1652,  1654,  1656,  1668,  1669,
    1675,  1676,  1677,  1678,  1687,  1688,  1689,  1690,  1699,  1700,
    1713,  1714,  1746,  1751,  1763,  1772,  1772,  1779,  1779,  1783,
    1785,  1785,  1793,  1803,  1812,  1816,  1821,  1825,  1830,  1838,
    1848,  1852,  1856,  1860,  1864,  1872,  1873,  1875,  1875,  1877,
    1877,  1879,  1883,  1887,  1888,  1890,  1890,  1896,  1897,  1899,
    1903,  1908,  1914,  1921,  1922,  1923,  1924,  1926,  1927,  1928,
    1934,  1936,  1940,  1945,  1963,  1970,  1978,  1987,  1988,  1989,
    1990,  1991,  1992,  1993,  1994,  1995,  1996,  1998,  2004,  2004,
    2007,  2011
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
  "query_expression", "query_expression_body", "query_expression_parens",
  "subquery", "set_operator", "set_type", "opt_all",
  "opt_hierarchical_clause", "connect_by", "query_primary", "opt_distinct",
  "select_list", "opt_from_clause", "from_clause", "opt_where",
  "opt_group", "opt_having", "opt_within_group", "opt_order", "order_list",
  "order_desc", "opt_order_type", "opt_null_ordering", "opt_top",
  "opt_limit", "expr_list", "opt_extended_literal_list",
  "extended_literal_list", "casted_extended_literal", "extended_literal",
  "expr_alias", "expr", "operand", "scalar_expr", "unary_expr",
  "binary_expr", "logic_expr", "in_expr", "in_row_list", "row_expr_list",
  "case_expr", "case_list", "exists_expr", "comp_expr",
  "table_function_expr", "function_expr", "opt_window", "opt_partition",
  "opt_frame_clause", "frame_type", "frame_bound", "extract_expr",
  "next_value_expr", "cast_expr", "datetime_field",
  "datetime_field_plural", "duration_field", "interval_range",
  "array_expr", "array_index", "between_expr", "column_name",
  "nonreserved_keyword", "name_after_qualifier",
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

#define YYPACT_NINF (-693)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-447)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1739,    56,  2369,   103,   128,  2369,   197,    72,    76,    39,
      89,  2369,   167,  1716,   264,    43,  2441,    64,    64,    64,
     -18,    28,   254,    78,  -693,   202,  -693,   202,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,   250,  -693,  -693,   -20,   363,   130,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,   158,  -693,
    -693,   101,   306,  -693,   260,   260,   260,  2369,   404,  2369,
      24,   426,   304,  -693,   315,  -693,   313,   -16,   315,   315,
     315,  2369,  -693,  -693,  -693,   333,   252,  -693,  -693,  -693,
    -693,  -693,   263,   268,  -693,  -693,  -693,   382,   295,  -693,
    1739,  -693,   398,  -693,  -693,  -693,  -693,   385,  -693,   -20,
     431,   140,   250,  -693,  2118,  2118,   233,  -693,   519,   435,
    2441,  2369,  2369,   486,  -693,   478,    -8,  -693,  1239,  -693,
    -693,  1239,  2441,   536,  2369,   585,   585,   585,  2441,  2369,
    2369,  -693,   386,  2441,  -693,  -693,  2369,  -693,  -693,   396,
    1239,  -693,  -693,  -693,  -693,  1239,   929,   493,   140,  -693,
    -693,  -693,  -693,  -693,   403,  -693,  -693,   405,  -693,  -693,
     707,   614,   409,   617,  -693,   618,   619,  -693,  -693,   414,
     416,   420,    54,   421,   417,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
     586,  -693,   503,   -24,   428,  1239,  -693,   585,   695,   574,
    -693,    97,   614,   386,   432,   433,  1084,  1394,   438,  1549,
    2369,  1549,  -693,   774,  -693,   -61,   157,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,   457,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,   465,   439,  -693,   552,   436,  -693,    60,
     -31,  -693,   462,  -693,   560,  -693,   -48,   -48,   -48,  -693,
    -693,  -693,  -693,  -693,   101,   651,   441,  -693,   275,   147,
     600,   138,   121,  -693,   493,  -693,   493,  2118,  2118,  -693,
    -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,   408,  -693,  -693,   256,   447,
      15,    18,  -693,  -693,  -693,   233,  -693,  2369,   656,   -18,
    2053,  2441,   533,   -30,   561,  -693,   142,   452,    13,  1877,
    -693,   256,  1239,  1239,   258,     0,   386,  1549,   774,  2200,
    1239,   456,  -693,   298,   110,    41,   208,  1239,  1239,  -693,
    1549,  -693,  1549,   120,   458,   113,  1549,  1549,  1549,  1549,
    1549,  1549,  1549,  1549,  1549,  1549,  1549,  1549,  1549,  1549,
    1549,   660,   459,   588,  -693,    14,  1565,   486,  -693,  1239,
    -693,  2220,  -693,  -693,  2441,  -693,  1239,   629,  -693,  -693,
     461,    67,  -693,   486,  -693,   464,   463,   227,  -693,  1239,
    -693,  -693,   669,   500,  1239,  1239,  -693,   495,  -693,   514,
     139,  -693,  -693,  -693,  -693,   554,   548,  -693,  -693,   473,
    -693,   474,  -693,  -693,   477,   593,  -693,   479,   510,   512,
     235,  -693,  -693,  -693,  2566,   242,   -18,  2369,   -18,  2441,
     233,  -693,  1239,  -693,  -693,   294,   597,   293,   257,   150,
    1239,  1239,  -693,  -693,   -53,   591,    68,  -693,  1239,     2,
    1239,  -693,   578,  1784,  2355,  1549,   487,   774,  -693,  -693,
     594,   456,  2355,  2355,  2355,  2355,   513,   513,   513,   513,
     298,   298,   115,   115,   115,   -27,   491,   623,   494,  -693,
    -693,  1239,   317,  1565,   -14,    52,    52,   332,  -693,   350,
    -693,   489,  -693,    52,  -693,   407,  -693,  -693,  -693,  -693,
    -693,   -30,   315,    67,   701,   705,   404,   715,   248,  -693,
    -693,  -693,   233,  -693,   651,  -693,   720,  -693,   549,   -30,
     -30,   662,  -693,  2441,  -693,   565,  -693,  2566,   256,  -693,
    -693,  2441,   585,  2441,   520,   521,  -693,  2053,  -693,  -693,
     524,  -693,   525,  -693,  -693,   526,  -693,  -693,   527,  -693,
    -693,  -693,  -693,   528,  -693,   256,  -693,   112,  -693,  -693,
    -693,  -693,  -693,   253,   261,    36,  1892,  1239,  2566,  1239,
    1239,  -693,   174,   299,  -693,   -30,   599,   529,   -30,  1549,
    2355,   774,  -693,   262,  -693,  -693,   612,   635,   273,  2118,
      11,   534,  2292,  -693,  -693,  -693,  -693,   644,  -693,  -693,
    -693,   667,   668,   670,   648,  1565,  -693,  1239,  -693,  -693,
     681,  1239,  2441,   279,  -693,  -693,  -693,  -693,  -693,    83,
     297,  -693,  -693,  1239,  -693,   -78,  -693,   542,  -693,   310,
    -693,   311,  2441,  2441,  -693,   748,   754,  -693,   755,   778,
     782,  -693,   548,  -693,  -693,   683,  -693,   610,  2369,  -693,
     112,  -693,  -693,  -693,  -693,  -693,  1239,  -693,  -693,   579,
     -47,   581,   -30,   308,  -693,  1239,   583,     1,  -693,  2200,
     322,  -693,  1239,   666,   717,  -693,   380,  -693,  1239,   323,
     587,   589,   590,  1728,  -693,  -693,  -693,  1565,   415,   196,
     674,  -693,   -60,  -693,  -693,   792,   404,  -693,  -693,   206,
    -693,  -693,  -693,  -693,   324,   329,   592,   335,   595,   596,
     598,  -693,  -693,   428,  -693,  -693,   339,  2041,  -693,  -693,
    -693,   -30,     1,  -693,   774,   341,  -693,   343,  1239,   437,
     345,    52,   602,  2441,  -693,   397,  1239,  1239,  1239,  -693,
    -693,   628,  -693,   624,  -693,  -693,   802,  -693,  -693,  -693,
    -693,  -693,  -693,  -693,  -693,   354,    99,   356,  -693,   603,
    -693,   436,  -693,  -693,  -693,   604,    37,  -693,  -693,  1239,
     361,   606,  1239,   -30,    -5,   -30,  -693,  -693,   607,  -693,
    -693,  1239,  -693,   105,    38,   134,  -693,  -693,   371,  -693,
    2441,   -30,  -693,  1239,  -693,  -693,   372,  -693,  -693,   696,
    -693,  -693,  -693,   377,   -30,  -693,    38,  -693,  -693
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     181,     0,     0,     0,     0,     0,     0,    31,    31,    31,
       0,    36,     0,   449,     3,    22,    20,    22,    19,    18,
       8,     9,     7,    11,    16,    17,    13,    14,    12,    15,
      10,   170,   135,   134,     0,     0,   349,   341,   339,   337,
     335,   334,   336,   340,   338,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   342,   350,   400,
     122,    38,     0,    59,    66,    66,    66,     0,     0,     0,
       0,   156,     0,   399,   117,   140,     0,     0,   117,   117,
     117,     0,    57,   349,   350,     0,   430,   431,    30,    27,
      29,    28,     0,   135,   383,   384,   382,     0,     0,     1,
     448,     2,     0,     6,     5,   143,   144,     0,   142,     0,
     146,   188,   170,   114,     0,     0,   192,    39,     0,     0,
     406,     0,     0,   162,    42,     0,   127,   365,     0,   179,
     155,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,   138,   139,     0,    35,     4,     0,
       0,   137,   136,   145,   141,     0,     0,   418,   188,   343,
     344,   345,   346,   348,   347,   401,   347,   402,   360,   363,
     365,     0,     0,     0,   366,     0,     0,   361,   362,   368,
     371,   375,     0,     0,   191,   193,   195,   197,   351,   352,
     353,   364,   354,   355,   356,   357,   358,   359,    33,    32,
       0,   405,     0,     0,   127,     0,   121,     0,     0,     0,
     124,   310,   341,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,     0,   217,     0,   201,   208,   209,   210,
     203,   205,   211,   204,   227,   168,   212,   213,   215,   214,
     216,   207,   202,   218,   314,   219,   159,   157,   189,   416,
     162,   129,     0,   116,     0,    43,    46,    46,    46,   115,
     112,   113,   433,   432,    38,     0,   169,   171,   176,   183,
     185,   182,     0,   132,   417,   419,   418,     0,     0,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   378,   374,     0,   367,   370,   381,     0,
       0,     0,   198,   199,    37,     0,    65,     0,     0,     0,
       0,     0,     0,   161,     0,   450,     0,     0,   156,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,   223,
       0,   222,   290,   221,   140,     0,     0,     0,     0,   180,
       0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   275,   220,   156,     0,   162,   158,     0,
     414,     0,   415,   200,     0,   128,     0,     0,   118,   119,
       0,     0,    56,   162,    55,     0,    25,     0,    23,     0,
     175,   174,   177,     0,     0,     0,   423,     0,   425,     0,
     429,   420,   133,   404,   403,     0,   379,   380,   369,     0,
     372,     0,   376,   194,     0,     0,    62,     0,     0,     0,
       0,    67,    69,    70,     0,     0,     0,     0,   126,     0,
       0,   267,     0,   313,   319,   311,     0,     0,     0,     0,
       0,     0,   254,   259,     0,     0,     0,   206,     0,     0,
       0,   238,   239,     0,   234,     0,     0,     0,   242,   225,
       0,   237,   236,   260,   261,   262,   263,   264,   265,   266,
     229,   228,   231,   230,   232,   233,     0,     0,     0,   273,
     271,     0,   349,     0,     0,   410,   410,   350,   160,   385,
     387,     0,   390,   410,   388,   151,   190,   411,   413,   412,
     130,   131,   117,     0,     0,     0,     0,     0,     0,    52,
      41,    34,     0,    21,     0,   172,     0,   173,     0,   186,
     184,     0,   426,     0,   428,     0,   421,     0,     0,   373,
     377,     0,     0,     0,     0,     0,    61,     0,    91,    82,
       0,    87,    97,    88,    72,     0,    79,    80,     0,    76,
      77,    86,    89,    94,    81,    83,    73,    99,   126,    64,
      40,   125,   451,     0,     0,   156,     0,     0,     0,     0,
       0,   252,     0,     0,   307,   250,     0,     0,   251,     0,
     235,     0,   243,     0,   226,   308,     0,   277,     0,     0,
     446,     0,     0,   409,   391,   407,   392,     0,   445,   437,
     443,   441,   444,   439,     0,     0,   398,     0,   152,   153,
     164,     0,     0,     0,    53,    50,    48,    54,    45,     0,
       0,    24,   178,     0,   424,   429,   427,     0,   306,     0,
      60,     0,     0,     0,    68,     0,     0,    78,     0,     0,
       0,    90,    84,    85,   106,     0,   105,     0,     0,    71,
      98,   100,   102,   123,   268,   269,     0,   317,   320,   315,
       0,     0,   256,     0,   255,     0,     0,     0,   244,   309,
       0,   240,     0,     0,   170,   272,   347,   389,     0,     0,
     411,   412,     0,     0,   440,   442,   438,     0,   386,   149,
       0,   154,   150,   120,    44,     0,     0,    51,    26,     0,
     422,   196,    63,   109,     0,     0,     0,     0,     0,     0,
       0,   107,   104,   127,   101,   103,     0,     0,   289,   291,
     253,   257,     0,   245,     0,     0,   241,     0,     0,   280,
       0,   410,     0,     0,   434,   446,     0,     0,     0,    49,
      47,     0,   108,     0,    75,    96,     0,    92,    74,    93,
     111,   270,   318,   321,   316,     0,     0,     0,   246,     0,
     167,   276,   281,   282,   283,     0,     0,   394,   393,     0,
       0,     0,     0,   147,   166,   148,   187,   110,     0,   247,
     248,     0,   274,     0,     0,     0,   288,   278,     0,   408,
       0,   447,   435,     0,   163,    95,     0,   287,   285,     0,
     286,   284,   395,     0,   165,   249,     0,   436,   279
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -693,  -693,  -693,   709,  -693,   793,  -693,   302,  -693,   559,
    -693,  -693,  -693,  -693,   547,  -693,  -693,  -485,  -139,   331,
     314,   199,  -693,  -693,  -693,   535,  -693,   282,  -693,  -461,
    -693,  -693,  -693,  -693,   171,  -693,  -598,  -693,   -73,  -693,
    -693,  -693,  -693,  -693,  -693,  -208,  -693,  -693,   448,    33,
     789,    25,   -11,  -693,  -693,  -693,  -693,   160,   741,  -309,
    -693,  -693,  -693,  -211,  -693,  -693,  -693,  -118,   181,   467,
    -693,  -693,  -693,   699,  -293,  -693,  -341,   553,   564,   496,
     -70,   406,  -693,  -693,  -693,  -693,  -693,   141,  -681,  -693,
     538,  -693,  -693,  -356,  -693,  -693,  -693,  -693,  -693,  -692,
    -693,  -693,  -693,  -290,  -693,  -693,   309,  -693,  -693,  -693,
    -693,    -2,  -119,   -88,   -15,  -114,  -693,  -693,   684,   -67,
    -693,  -693,  -693,  -693,   685,   857,  -693,  -455,   186,  -693,
    -693,  -693,  -693,    21,  -693,  -693,  -474,   622,  -693,   601,
    -693,   608,  -693,   247,  -693,  -693,   730,  -693,  -693,  -693,
    -693,  -311
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    22,    23,    24,    25,   113,   397,   398,    26,    99,
      27,   209,    28,   107,   127,    29,    30,   135,   266,   392,
     518,   519,    31,    32,    33,   130,   430,   431,   432,   567,
     651,   647,   659,   660,   661,   433,   662,    34,   144,    35,
     388,   389,    36,    37,    38,   219,    39,   260,   261,   102,
      41,    85,   234,   119,   120,   164,   620,   621,    43,   141,
     256,   377,   378,   216,   701,   804,   373,   121,   276,   277,
     402,   527,    81,   167,   257,   193,   194,   195,   196,   258,
     259,   236,   237,   238,   239,   240,   241,   735,   346,   242,
     335,   243,   244,   245,   246,   489,   684,   775,   776,   797,
     247,   248,   249,   301,   302,   303,   417,   250,   251,   252,
     253,   254,   176,   175,    69,   255,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   498,   499,   500,   689,
     501,   502,    82,   503,   212,   603,   604,   605,   383,   283,
     284,   285,   410,   536,    44,    96,    97,   504,   614,   802,
     111,   326
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      68,    95,    86,    68,   168,   174,   322,   267,   268,    68,
     435,    68,   197,   139,    94,   148,   149,   150,   416,   442,
     496,   419,   606,    70,   421,    42,    73,   140,   140,   616,
     137,   626,    83,    40,    87,   104,   105,   177,   600,   607,
      10,   446,    10,   793,   793,   103,    10,   456,   803,   385,
     140,   601,    10,   767,   215,   380,   794,   347,   347,   179,
     180,    10,   725,   380,    91,   347,   491,   390,   235,    42,
     514,   347,   318,   348,   348,    68,   637,    68,   324,   608,
     146,   348,   534,   535,   609,    45,   514,   348,   347,    68,
     278,   610,   611,   515,   450,   279,   281,    16,   133,   573,
     136,    16,   809,   319,   348,    92,    71,    16,   612,   705,
     806,   356,   151,   613,   451,   211,    16,   671,   516,   147,
     452,  -135,   173,   173,   818,   586,   748,   262,    94,    68,
      68,    72,   587,   269,   706,    42,   439,   496,    95,    79,
      94,   272,    68,    40,   161,   323,    94,    68,    68,   574,
     349,    94,   213,   214,    68,   787,   334,  -135,   457,   347,
     698,    42,   391,   345,   728,   264,   505,  -135,    77,   220,
     270,   271,    78,   517,   593,   348,   350,   274,   347,   602,
     371,   630,   520,   384,  -135,   406,   320,   381,  -135,   517,
      20,   197,    20,  -135,   348,   405,    20,   165,   598,   413,
     414,   197,   218,   325,   795,   795,   186,   796,   796,   379,
     445,   734,   330,   351,   166,    80,    94,   347,   654,   469,
      98,   750,   635,   465,   441,   490,   420,   687,    68,   422,
     639,  -135,   641,   348,   138,   655,   470,   178,   179,   180,
     104,   105,   745,   103,   580,   106,   181,   665,   638,    84,
     466,   342,   457,   356,   109,   458,   347,   407,   344,   496,
     352,   115,   447,   448,   451,   347,   666,   778,   454,    74,
     581,   533,   348,   807,   808,   652,   584,   461,   462,    75,
     353,   348,   379,   182,   408,   173,   173,   354,   355,   656,
     657,   110,   347,   658,   674,   356,   357,   116,   680,   534,
     535,   409,   810,   811,   486,   434,   325,   328,   348,   329,
      76,   126,   403,   458,   347,    68,   511,   112,    94,    94,
     370,   155,   371,   183,   347,   453,   197,   444,   117,   278,
     348,   714,   715,   118,   529,   530,    88,   496,   424,   184,
     348,   496,   124,   468,    42,   185,    89,   358,   359,   360,
     361,   362,   426,   438,   363,   364,   439,   365,   366,   367,
     368,   369,   370,   344,   371,   495,   123,   400,   579,   262,
     125,   751,   333,   726,   497,   347,   347,    90,   607,   509,
     582,   583,    94,   618,   619,   186,   187,   188,   585,   129,
     588,   348,   348,   347,   401,   740,   289,   290,   291,   292,
     293,   294,   128,   640,   189,   190,   191,   134,   197,   348,
     675,   347,   178,   179,   180,   104,   105,   347,   608,   459,
     578,   181,   460,   609,   572,   607,   347,   348,   730,   142,
     610,   611,   780,   348,   192,    68,   356,    94,   523,   622,
     140,   524,   348,   607,   143,   771,   546,   612,   145,   547,
     106,  -446,   613,   568,   784,   592,   439,   669,   570,   628,
     152,    42,   629,    42,   663,   608,   153,   315,   156,   569,
     609,   571,   664,   681,   154,   379,   379,   610,   611,   155,
     686,   781,   495,   608,   685,   157,   798,   379,   609,   813,
     704,   497,   344,   629,   612,   610,   611,   159,   183,   613,
     367,   368,   369,   370,   575,   371,   576,   670,   708,   672,
     673,   315,   612,   160,   184,   760,  -446,   613,   325,   103,
     185,   712,   713,   208,   439,   439,   325,   328,   325,   599,
     782,    94,   434,   736,   741,   752,   379,   742,   439,    94,
     753,    94,   375,   439,   125,    94,   755,   699,   163,   756,
     761,   702,   768,   379,   770,   769,   777,   399,   210,   379,
     186,   187,   188,   709,  -396,   789,   739,   790,   769,   351,
     460,   215,   799,   217,   668,   439,   678,   100,   101,   189,
     190,   191,   812,   815,   263,   379,   460,   692,   817,   265,
     575,   439,   287,   617,   618,   619,    20,   173,   393,   394,
     691,   772,   773,   774,   495,   731,   275,   703,   764,   192,
     131,   132,   278,   497,   282,   287,   344,   288,   304,   305,
      94,   306,   307,   308,   309,   106,   310,   325,   325,  -397,
     311,   315,   314,   339,   316,   341,   317,   343,   321,   327,
      94,    94,   331,   332,   355,   340,   372,   374,   376,   375,
     379,   356,   386,   387,   396,   399,    68,   404,   418,   425,
     436,   437,   440,   371,   766,   733,   137,   488,   467,   487,
     512,   513,   526,   522,   528,   531,   783,   532,   785,   723,
     521,   537,   495,   538,   539,   540,   495,   541,   542,   543,
     544,   497,   545,   577,   465,   497,   347,   591,    93,   595,
     594,   596,   103,   615,   597,   624,  -447,  -447,    47,   625,
    -447,  -447,   801,   365,   366,   367,   368,   369,   370,   627,
     371,   766,    48,   632,   633,   763,   634,   636,   325,   676,
     642,   643,    49,   814,   645,   646,   648,   649,   650,   677,
     682,    94,   683,   339,   688,   693,    50,   694,   695,   697,
     696,    51,   700,   711,   716,    10,   463,   103,   464,   344,
     717,   718,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   221,   178,   179,
     180,   104,   105,    52,   719,   325,    53,   222,   720,   721,
     722,   727,   729,   732,   738,   117,   749,  -349,    94,  -350,
     743,    48,   747,   754,   786,   658,   757,   758,   788,   759,
      16,    49,   779,   791,   816,   792,   800,    54,   805,   158,
     114,   395,   223,   224,   225,    50,   631,   623,   707,   644,
      51,   724,   510,   122,    10,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   746,
     162,   226,    52,   737,   183,    53,   525,   286,   423,   415,
      67,   590,   449,   765,   653,   506,   312,   313,   108,   744,
     184,   382,   710,   273,     0,     0,   185,   412,     0,    16,
       0,     0,   411,     0,     0,     0,    54,   227,     0,     0,
       0,     0,     0,     0,     0,    20,     0,     0,     0,     0,
       0,   228,     0,     0,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,   186,   187,   188,     0,
       0,     0,   221,   178,   179,   180,   104,   105,     0,     0,
       0,     0,   222,     0,     0,   189,   190,   191,     0,    67,
       0,     0,     0,     0,     0,     0,    48,   229,     0,   230,
       0,     0,     0,     0,     0,     0,    49,     0,     0,     0,
       0,     0,     0,     0,     0,   231,   232,   223,   224,   225,
      50,     0,     0,     0,   233,    51,     0,     0,     0,     0,
       0,   106,     0,     0,     0,   679,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   226,    52,     0,   183,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,   185,     0,     0,     0,     0,   280,     0,     0,     0,
       0,    54,   227,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   228,     0,     0,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,   186,   187,   188,     0,     0,     0,   221,   178,   179,
     180,   104,   105,     0,     0,     0,     0,   222,     0,     0,
     189,   190,   191,     0,    67,     0,     0,     0,     0,     0,
       0,    48,   229,     0,   230,     0,     0,     0,     0,     0,
       0,    49,     0,     0,     0,     0,     0,     0,     0,     0,
     231,   232,   223,   224,   225,    50,     0,     0,     0,   233,
      51,     0,     0,     0,     0,     0,   106,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   226,    52,     0,   183,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     184,     0,     0,     0,     0,     0,   185,     0,   333,     0,
       0,     0,     0,     0,     0,     0,    54,   227,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   228,     0,     0,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,   186,   187,   188,     0,
       0,     0,   221,   178,   179,   180,   104,   105,     0,     0,
       0,     0,   222,     0,     0,   189,   190,   191,     0,    67,
       0,     0,     0,     0,     0,     0,    48,   229,     0,   230,
       0,     0,     0,     0,     0,     0,    49,     0,     0,     0,
       0,     0,     0,     0,     0,   231,   232,   223,   224,   225,
      50,     0,     0,     0,   233,    51,     0,     0,     0,     0,
       0,   106,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   226,    52,     0,   183,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,   185,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,   227,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   228,     0,     0,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,   186,   187,   188,     0,     0,     0,   221,   178,   179,
     180,   104,   105,     0,     0,     0,     0,   222,     0,     0,
     189,   190,   191,     0,    67,     0,     0,     0,     0,     0,
       0,    48,   229,     0,   230,     0,     0,     0,     0,     0,
       0,    49,     0,     0,     0,     0,     0,     0,     0,     0,
     231,   232,   336,   224,   225,    50,     0,     0,     0,   233,
      51,     0,     0,     0,     0,     0,   106,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   226,    52,     0,   183,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     184,     0,     0,     0,     0,     0,   185,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,   337,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   228,     0,     0,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,   186,   187,   188,     0,
       0,     0,   221,   178,   179,   180,   104,   105,     0,     0,
       0,     0,   222,     0,     0,   189,   190,   191,   492,    67,
       0,     0,     0,     0,     0,     0,    48,   229,    47,   230,
       0,     0,     0,     0,     0,     0,    49,     0,     0,     0,
       0,     0,    48,     0,     0,   231,   232,     0,   224,   225,
      50,     0,    49,     0,   338,    51,     0,     0,     0,     0,
       0,   106,     0,     0,     0,     0,    50,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   226,    52,     0,   183,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,   184,    53,     0,     0,     0,
       0,   185,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,   337,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   228,    54,     0,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,   186,   187,   188,     0,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,     0,     0,    46,
     189,   190,   191,     0,    67,     0,     0,     0,     0,    47,
       0,   492,   229,     0,   230,     0,     0,     0,     0,     0,
      67,    47,     0,    48,     0,     0,     0,     0,     1,   493,
     231,   232,     0,    49,     0,    48,     2,     0,     0,   338,
       0,     0,     0,     3,     0,    49,   106,    50,     4,     0,
       0,     0,    51,     0,     0,   494,     0,     0,     5,    50,
       0,     6,     7,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     8,     9,     0,     0,     0,     0,    10,
       0,     0,     0,    11,    52,     0,    12,    53,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,    13,     0,
       0,     0,    14,     0,     0,     0,     0,     0,    54,     0,
     351,     0,     0,     0,     0,     0,     0,     0,    15,     0,
      54,     0,     0,     0,    16,     0,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
     169,     0,     0,     0,     0,     0,     0,   352,     0,     0,
      47,    67,     0,     0,     0,   169,    17,    18,    19,     0,
       0,     0,   589,    67,    48,    47,     0,   455,     0,     0,
       0,     0,     0,     0,    49,   355,     0,     0,     0,    48,
       0,     0,   356,   357,     0,     0,    20,     0,    50,    49,
       0,     0,     0,    51,     0,     0,     0,     0,   494,     0,
       0,     0,     0,    50,     0,     0,     0,     0,    51,    20,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,    53,     0,
       0,     0,     0,     0,   358,   359,   360,   361,   362,     0,
      52,   363,   364,    53,   365,   366,   367,   368,   369,   370,
       0,   371,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,     0,
       0,     0,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,   169,     0,     0,     0,   170,   171,
     172,     0,    67,     0,    47,     0,    93,     0,     0,     0,
       0,     0,     0,   170,   171,   172,    47,    67,    48,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,   443,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,     0,    50,     0,   667,     0,     0,    51,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   169,     0,     0,     0,     0,     0,     0,     0,    52,
       0,    47,    53,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,    53,    48,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,     0,     0,     0,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,    51,    54,     0,     0,     0,     0,
       0,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,     0,    52,     0,     0,    53,
       0,     0,   170,   171,   172,     0,    67,     0,     0,     0,
       0,     0,     0,   507,   508,     0,     0,     0,    67,     0,
     427,   428,   429,    47,     0,     0,     0,     0,     0,     0,
      54,     0,     0,   762,     0,     0,     0,    48,     0,     0,
       0,     0,     0,     0,     0,     0,   351,    49,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
       0,    50,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   170,
     171,   172,     0,    67,     0,   690,   508,     0,     0,     0,
       0,     0,     0,   352,     0,    47,     0,     0,    52,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,    48,
       0,     0,     0,   455,     0,     0,     0,     0,     0,    49,
       0,   355,     0,     0,     0,     0,     0,     0,   356,   357,
       0,     0,    54,    50,     0,     0,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    46,     0,     0,     0,     0,     0,     0,     0,
      52,     0,    47,    53,     0,     0,     0,     0,     0,     0,
     358,   359,   360,   361,   362,    67,    48,   363,   364,     0,
     365,   366,   367,   368,   369,   370,    49,   371,     0,     0,
       0,   351,     0,     0,    54,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,     0,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    52,  -447,     0,
      53,     0,     0,     0,     0,     0,     0,    67,    48,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,   355,     0,     0,     0,
       0,    54,    50,   356,  -447,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,  -447,  -447,  -447,   361,   362,
       0,     0,   363,   364,     0,   365,   366,   367,   368,   369,
     370,     0,   371,    54,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   548,
       0,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,   549,     0,     0,     0,   550,   551,   552,
     553,   554,     0,   555,     0,     0,     0,     0,     0,     0,
       0,   556,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   557,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   558,     0,   559,   560,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   561,     0,     0,     0,     0,   562,     0,   563,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   564,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   565,     0,
       0,   566
};

static const yytype_int16 yycheck[] =
{
       2,    16,    13,     5,   122,   124,   214,   146,   147,    11,
     321,    13,   126,    80,    16,    88,    89,    90,   308,   328,
     376,     6,   496,     2,     6,     0,     5,    14,    14,   503,
       6,   516,    11,     0,    13,     7,     8,   125,   493,    28,
      60,   331,    60,     6,     6,    20,    60,   340,    53,   260,
      14,    65,    60,   734,    85,     3,    19,   118,   118,     5,
       6,    60,   660,     3,    21,   118,   375,   115,   138,    44,
       3,   118,    96,   134,   134,    77,   537,    79,   217,    68,
      96,   134,   160,   161,    73,    29,     3,   134,   118,    91,
     160,    80,    81,    26,    94,   165,   166,   115,    77,   440,
      79,   115,   794,   127,   134,    62,     3,   115,    97,    26,
     791,   138,    91,   102,   114,   130,   115,   578,    51,   135,
     120,    11,   124,   125,   816,   123,   186,   142,   130,   131,
     132,     3,   130,   148,    51,   110,   214,   493,   153,   100,
     142,   152,   144,   110,   119,   215,   148,   149,   150,   442,
     211,   153,   131,   132,   156,   753,   226,    47,   211,   118,
     615,   136,   210,   233,   211,   144,   377,    57,    96,   136,
     149,   150,    96,   106,   467,   134,    19,   156,   118,   127,
     207,   522,   393,   214,    74,    64,   210,   127,    78,   106,
     210,   305,   210,    83,   134,    57,   210,    57,   491,   287,
     288,   315,   210,   218,   167,   167,   152,   170,   170,   214,
     329,   210,   223,    56,    74,   126,   218,   118,   106,   106,
     156,   706,   533,   103,   211,   211,   211,   216,   230,   211,
     541,   121,   543,   134,   210,   123,   123,     4,     5,     6,
       7,     8,   697,   218,    94,   217,    13,   211,   538,    82,
     130,   230,   211,   138,     0,   214,   118,   136,   233,   615,
     103,    11,   332,   333,   114,   118,   575,   741,   338,    72,
     120,   132,   134,   168,   169,   565,   208,   347,   348,    82,
     123,   134,   214,    50,   163,   287,   288,   130,   131,   177,
     178,   213,   118,   181,   120,   138,   139,    47,   591,   160,
     161,   180,   168,   169,   371,   320,   321,   210,   134,   212,
     113,   210,   165,   214,   118,   317,   386,   115,   320,   321,
     205,   211,   207,    90,   118,   336,   440,   329,    78,   399,
     134,   642,   643,    83,   404,   405,    72,   693,   317,   106,
     134,   697,   212,   354,   319,   112,    82,   190,   191,   192,
     193,   194,   319,   211,   197,   198,   214,   200,   201,   202,
     203,   204,   205,   338,   207,   376,     3,    92,   111,   384,
     212,   165,   114,   666,   376,   118,   118,   113,    28,   381,
     450,   451,   384,   187,   188,   152,   153,   154,   458,   129,
     460,   134,   134,   118,   119,   688,   140,   141,   142,   143,
     144,   145,    96,   542,   171,   172,   173,     3,   522,   134,
     111,   118,     4,     5,     6,     7,     8,   118,    68,   211,
     127,    13,   214,    73,   439,    28,   118,   134,   120,   125,
      80,    81,   743,   134,   201,   437,   138,   439,   211,   512,
      14,   214,   134,    28,   129,   738,   211,    97,   135,   214,
     217,   101,   102,   211,   747,   466,   214,   576,   437,   211,
     127,   436,   214,   438,   211,    68,   214,   214,    86,   436,
      73,   438,   211,   211,   211,   214,   214,    80,    81,   211,
     599,    84,   493,    68,   211,   190,   779,   214,    73,   800,
     211,   493,   467,   214,    97,    80,    81,    99,    90,   102,
     202,   203,   204,   205,   210,   207,   212,   577,   211,   579,
     580,   214,    97,   128,   106,   723,   101,   102,   533,   494,
     112,   211,   211,     4,   214,   214,   541,   210,   543,   212,
     133,   533,   547,   211,   211,   211,   214,   214,   214,   541,
     211,   543,   210,   214,   212,   547,   211,   617,   117,   214,
     211,   621,   211,   214,   211,   214,   211,   214,   123,   214,
     152,   153,   154,   633,   214,   211,   684,   211,   214,    56,
     214,    85,   211,    95,   576,   214,   587,    18,    19,   171,
     172,   173,   211,   211,    48,   214,   214,   602,   211,     4,
     210,   214,   212,   186,   187,   188,   210,   599,   267,   268,
     602,   164,   165,   166,   615,   675,   210,   622,   727,   201,
      75,    76,   682,   615,   121,   212,   591,   212,     4,   210,
     622,     4,     4,     4,   210,   217,   210,   642,   643,   214,
     210,   214,   211,   227,    48,   229,   133,   231,   210,    65,
     642,   643,   210,   210,   131,   207,   189,   182,    96,   210,
     214,   138,   190,    93,     3,   214,   658,    57,   211,     3,
     127,   100,   210,   207,   734,   676,     6,    79,   210,   210,
      41,   210,     3,   210,   174,   180,   746,   163,   748,   658,
     216,   127,   693,   135,   211,   211,   697,   210,    95,   210,
     180,   693,   180,    96,   103,   697,   118,   210,     3,   208,
     106,    78,   677,   214,   210,     4,   193,   194,    13,     4,
     197,   198,   782,   200,   201,   202,   203,   204,   205,     4,
     207,   791,    27,     3,   175,   727,    64,   162,   743,   130,
     210,   210,    37,   803,   210,   210,   210,   210,   210,   210,
     128,   743,   107,   337,   210,   101,    51,    80,    80,   101,
      80,    56,    71,   211,     6,    60,   350,   732,   352,   734,
       6,     6,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,     3,     4,     5,
       6,     7,     8,    88,     6,   800,    91,    13,     6,   106,
     180,   212,   211,   210,   128,    78,     4,   210,   800,   210,
     210,    27,   128,   211,   176,   181,   211,   211,     6,   211,
     115,    37,   210,   210,   118,   211,   210,   122,   211,   110,
      27,   274,    48,    49,    50,    51,   524,   513,   629,   547,
      56,   660,   384,    44,    60,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   699,
     119,    87,    88,   682,    90,    91,   399,   168,   315,   305,
     175,   465,   334,   732,   565,   379,   192,   192,    21,   693,
     106,   259,   635,   153,    -1,    -1,   112,   286,    -1,   115,
      -1,    -1,   284,    -1,    -1,    -1,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   210,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    13,    -1,    -1,   171,   172,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    27,   183,    -1,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    48,    49,    50,
      51,    -1,    -1,    -1,   210,    56,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,   589,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    13,    -1,    -1,
     171,   172,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    27,   183,    -1,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     201,   202,    48,    49,    50,    51,    -1,    -1,    -1,   210,
      56,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    90,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    13,    -1,    -1,   171,   172,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    27,   183,    -1,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    48,    49,    50,
      51,    -1,    -1,    -1,   210,    56,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    13,    -1,    -1,
     171,   172,   173,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    27,   183,    -1,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     201,   202,    48,    49,    50,    51,    -1,    -1,    -1,   210,
      56,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    90,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    13,    -1,    -1,   171,   172,   173,     3,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    27,   183,    13,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,   201,   202,    -1,    49,    50,
      51,    -1,    37,    -1,   210,    56,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,   106,    91,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   122,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,    -1,    -1,     3,
     171,   172,   173,    -1,   175,    -1,    -1,    -1,    -1,    13,
      -1,     3,   183,    -1,   185,    -1,    -1,    -1,    -1,    -1,
     175,    13,    -1,    27,    -1,    -1,    -1,    -1,     9,   184,
     201,   202,    -1,    37,    -1,    27,    17,    -1,    -1,   210,
      -1,    -1,    -1,    24,    -1,    37,   217,    51,    29,    -1,
      -1,    -1,    56,    -1,    -1,   210,    -1,    -1,    39,    51,
      -1,    42,    43,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    64,    88,    -1,    67,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
     122,    -1,    -1,    -1,   115,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
       3,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      13,   175,    -1,    -1,    -1,     3,   157,   158,   159,    -1,
      -1,    -1,   118,   175,    27,    13,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    37,   131,    -1,    -1,    -1,    27,
      -1,    -1,   138,   139,    -1,    -1,   210,    -1,    51,    37,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,   210,    -1,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,   210,
      -1,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,   190,   191,   192,   193,   194,    -1,
      88,   197,   198,    91,   200,   201,   202,   203,   204,   205,
      -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,     3,    -1,    -1,    -1,   171,   172,
     173,    -1,   175,    -1,    13,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,   171,   172,   173,    13,   175,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,   202,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    51,    -1,   202,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    13,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    91,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    56,   122,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    88,    -1,    -1,    91,
      -1,    -1,   171,   172,   173,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,    13,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,   202,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    37,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,
     172,   173,    -1,   175,    -1,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,    13,    -1,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    37,
      -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,   138,   139,
      -1,    -1,   122,    51,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    13,    91,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,   192,   193,   194,   175,    27,   197,   198,    -1,
     200,   201,   202,   203,   204,   205,    37,   207,    -1,    -1,
      -1,    56,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    -1,    -1,    88,   103,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,   175,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,
      -1,   122,    51,   138,   139,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   190,   191,   192,   193,   194,
      -1,    -1,   197,   198,    -1,   200,   201,   202,   203,   204,
     205,    -1,   207,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    27,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
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
     267,   268,   269,   276,   362,    29,     3,    13,    27,    37,
      51,    56,    88,    91,   122,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   175,   329,   332,
     351,     3,     3,   351,    72,    82,   113,    96,    96,   100,
     126,   290,   350,   351,    82,   269,   270,   351,    72,    82,
     113,    21,    62,     3,   329,   332,   363,   364,   156,   227,
     227,   227,   267,   269,     7,     8,   217,   231,   343,     0,
     213,   368,   115,   223,   223,    11,    47,    78,    83,   271,
     272,   285,   268,     3,   212,   212,   210,   232,    96,   129,
     243,   243,   243,   351,     3,   235,   351,     6,   210,   337,
      14,   277,   125,   129,   256,   135,    96,   135,   256,   256,
     256,   351,   127,   214,   211,   211,    86,   190,   221,    99,
     128,   269,   276,   117,   273,    57,    74,   291,   285,     3,
     171,   172,   173,   329,   330,   331,   330,   331,     4,     5,
       6,    13,    50,    90,   106,   112,   152,   153,   154,   171,
     172,   173,   201,   293,   294,   295,   296,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,     4,   229,
     123,   332,   352,   351,   351,    85,   281,    95,   210,   263,
     267,     3,    13,    48,    49,    50,    87,   123,   137,   183,
     185,   201,   202,   210,   270,   298,   299,   300,   301,   302,
     303,   304,   307,   309,   310,   311,   312,   318,   319,   320,
     325,   326,   327,   328,   329,   333,   278,   292,   297,   298,
     265,   266,   332,    48,   351,     4,   236,   236,   236,   332,
     351,   351,   270,   364,   351,   210,   286,   287,   298,   298,
     117,   298,   121,   357,   358,   359,   291,   212,   212,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   321,   322,   323,     4,   210,     4,     4,     4,   210,
     210,   210,   336,   342,   211,   214,    48,   133,    96,   127,
     210,   210,   263,   298,   236,   332,   369,    65,   210,   212,
     270,   210,   210,   114,   298,   308,    48,   123,   210,   299,
     207,   299,   351,   299,   269,   298,   306,   118,   134,   211,
      19,    56,   103,   123,   130,   131,   138,   139,   190,   191,
     192,   193,   194,   197,   198,   200,   201,   202,   203,   204,
     205,   207,   189,   284,   182,   210,    96,   279,   280,   214,
       3,   127,   355,   356,   214,   281,   190,    93,   258,   259,
     115,   210,   237,   237,   237,   232,     3,   224,   225,   214,
      92,   119,   288,   165,    57,    57,    64,   136,   163,   180,
     360,   359,   357,   331,   331,   296,   321,   324,   211,     6,
     211,     6,   211,   295,   351,     3,   267,   177,   178,   179,
     244,   245,   246,   253,   332,   369,   127,   100,   211,   214,
     210,   211,   277,   202,   329,   330,   321,   298,   298,   308,
      94,   114,   120,   270,   298,   123,   292,   211,   214,   211,
     214,   298,   298,   299,   299,   103,   130,   210,   270,   106,
     123,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   337,   210,    79,   313,
     211,   277,     3,   184,   210,   270,   311,   329,   344,   345,
     346,   348,   349,   351,   365,   281,   297,     3,     4,   329,
     266,   298,    41,   210,     3,    26,    51,   106,   238,   239,
     281,   216,   210,   211,   214,   287,     3,   289,   174,   298,
     298,   180,   163,   132,   160,   161,   361,   127,   135,   211,
     211,   210,    95,   210,   180,   180,   211,   214,    13,    27,
      31,    32,    33,    34,    35,    37,    45,    70,    88,    90,
      91,   105,   110,   112,   122,   152,   155,   247,   211,   267,
     351,   267,   332,   294,   292,   210,   212,    96,   127,   111,
      94,   120,   298,   298,   208,   298,   123,   130,   298,   118,
     299,   210,   270,   292,   106,   208,    78,   210,   292,   212,
     345,    65,   127,   353,   354,   355,   354,    28,    68,    73,
      80,    81,    97,   102,   366,   214,   354,   186,   187,   188,
     274,   275,   256,   238,     4,     4,   235,     4,   211,   214,
     294,   225,     3,   175,    64,   369,   162,   247,   321,   369,
     236,   369,   210,   210,   245,   210,   210,   249,   210,   210,
     210,   248,   321,   324,   106,   123,   177,   178,   181,   250,
     251,   252,   254,   211,   211,   211,   277,   202,   329,   330,
     298,   247,   298,   298,   120,   111,   130,   210,   270,   299,
     292,   211,   128,   107,   314,   211,   330,   216,   210,   347,
       3,   329,   332,   101,    80,    80,    80,   101,   345,   298,
      71,   282,   298,   332,   211,    26,    51,   239,   211,   298,
     361,   211,   211,   211,   369,   369,     6,     6,     6,     6,
       6,   106,   180,   351,   252,   254,   292,   212,   211,   211,
     120,   298,   210,   270,   210,   305,   211,   286,   128,   285,
     292,   211,   214,   210,   346,   345,   275,   128,   186,     4,
     235,   165,   211,   211,   211,   211,   214,   211,   211,   211,
     263,   211,   202,   329,   330,   305,   298,   306,   211,   214,
     211,   292,   164,   165,   166,   315,   316,   211,   354,   210,
     369,    84,   133,   298,   292,   298,   176,   254,     6,   211,
     211,   210,   211,     6,    19,   167,   170,   317,   292,   211,
     210,   298,   367,    53,   283,   211,   306,   168,   169,   317,
     168,   169,   211,   369,   298,   211,   118,   211,   317
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
     265,   266,   267,   267,   268,   268,   268,   268,   269,   269,
     270,   271,   272,   272,   272,   273,   273,   274,   274,   274,
     274,   274,   275,   275,   276,   277,   277,   278,   279,   279,
     280,   281,   281,   282,   282,   283,   283,   284,   284,   285,
     285,   286,   286,   287,   288,   288,   288,   289,   289,   290,
     290,   290,   291,   291,   291,   291,   291,   291,   291,   292,
     292,   293,   293,   294,   294,   295,   295,   296,   296,   296,
     297,   298,   298,   298,   298,   298,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   300,   300,
     300,   301,   301,   301,   301,   301,   301,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   303,   303,
     304,   304,   304,   304,   304,   304,   304,   304,   305,   305,
     306,   306,   307,   307,   307,   307,   308,   308,   309,   309,
     310,   310,   310,   310,   310,   310,   310,   311,   311,   311,
     311,   311,   311,   312,   313,   313,   314,   314,   315,   315,
     315,   316,   316,   316,   317,   317,   317,   317,   317,   318,
     319,   320,   321,   321,   321,   321,   321,   321,   322,   322,
     322,   322,   322,   322,   323,   323,   324,   325,   326,   327,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   330,   330,   330,   330,   331,   331,   332,
     332,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     334,   335,   335,   336,   336,   337,   338,   339,   339,   339,
     340,   340,   340,   340,   341,   341,   341,   341,   342,   342,
     342,   342,   343,   343,   343,   344,   344,   345,   345,   345,
     346,   346,   346,   346,   347,   347,   348,   348,   349,   350,
     351,   351,   351,   351,   351,   352,   352,   353,   353,   354,
     354,   355,   355,   355,   355,   356,   356,   357,   357,   358,
     358,   359,   359,   360,   360,   360,   360,   361,   361,   361,
     362,   363,   363,   364,   365,   365,   365,   366,   366,   366,
     366,   366,   366,   366,   366,   366,   366,   367,   368,   368,
     369,   369
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
       3,     3,     4,     5,     1,     1,     3,     3,     3,     3,
       1,     2,     1,     1,     1,     1,     0,     4,     4,     2,
       2,     0,     1,     1,     8,     1,     0,     1,     1,     0,
       2,     2,     0,     4,     0,     2,     0,     6,     0,     3,
       0,     1,     3,     3,     1,     1,     0,     0,     2,     2,
       4,     0,     2,     2,     4,     2,     4,     8,     0,     1,
       3,     1,     0,     1,     3,     1,     6,     1,     2,     2,
       2,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     2,     3,     4,     1,     3,     3,
       3,     3,     3,     3,     3,     4,     3,     3,     3,     3,
       5,     6,     3,     4,     5,     6,     7,     8,     3,     5,
       3,     3,     4,     6,     3,     5,     4,     5,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     5,     5,
       7,     3,     5,     3,     6,     0,     3,     0,     2,     5,
       0,     1,     1,     1,     2,     2,     2,     2,     1,     6,
       2,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     4,     5,
       1,     3,     1,     3,     1,     5,     7,     5,     7,     3,
       5,     7,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     3,
       2,     1,     3,     4,     2,     1,     3,     4,     2,     3,
       3,     2,     1,     1,     1,     1,     3,     1,     1,     3,
       1,     2,     2,     5,     3,     5,     1,     3,     2,     1,
       1,     3,     3,     5,     5,     1,     0,     1,     5,     1,
       0,     2,     2,     2,     1,     1,     0,     1,     0,     1,
       2,     3,     5,     1,     3,     1,     2,     2,     1,     0,
       2,     1,     3,     3,     4,     6,     8,     1,     2,     1,
       2,     1,     2,     1,     1,     1,     0,     1,     1,     0,
       1,     3
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
#line 329 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2700 "bison_parser.cpp"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 329 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2706 "bison_parser.cpp"
        break;

    case YYSYMBOL_FLOATVAL: /* FLOATVAL  */
#line 311 "bison_parser.y"
            { }
#line 2712 "bison_parser.cpp"
        break;

    case YYSYMBOL_INTVAL: /* INTVAL  */
#line 311 "bison_parser.y"
            { }
#line 2718 "bison_parser.cpp"
        break;

    case YYSYMBOL_DOLLAR_PARAM: /* DOLLAR_PARAM  */
#line 311 "bison_parser.y"
            { }
#line 2724 "bison_parser.cpp"
        break;

    case YYSYMBOL_NAMED_PARAM: /* NAMED_PARAM  */
#line 329 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2730 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement_list: /* statement_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).stmt_vec)) {
    for (auto ptr : *(((*yyvaluep).stmt_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).stmt_vec));
}
#line 2743 "bison_parser.cpp"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2749 "bison_parser.cpp"
        break;

    case YYSYMBOL_preparable_statement: /* preparable_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).statement)); }
#line 2755 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_hints: /* opt_hints  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2768 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint_list: /* hint_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2781 "bison_parser.cpp"
        break;

    case YYSYMBOL_hint: /* hint  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2787 "bison_parser.cpp"
        break;

    case YYSYMBOL_transaction_statement: /* transaction_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).transaction_stmt)); }
#line 2793 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_statement: /* prepare_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).prep_stmt)); }
#line 2799 "bison_parser.cpp"
        break;

    case YYSYMBOL_prepare_target_query: /* prepare_target_query  */
#line 329 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2805 "bison_parser.cpp"
        break;

    case YYSYMBOL_odbc_call_statement: /* odbc_call_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2811 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_odbc_return: /* opt_odbc_return  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 2817 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_call_args: /* opt_call_args  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2830 "bison_parser.cpp"
        break;

    case YYSYMBOL_execute_statement: /* execute_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).exec_stmt)); }
#line 2836 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_statement: /* import_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).import_stmt)); }
#line 2842 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_type: /* file_type  */
#line 311 "bison_parser.y"
            { }
#line 2848 "bison_parser.cpp"
        break;

    case YYSYMBOL_file_path: /* file_path  */
#line 329 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 2854 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_import_export_options: /* opt_import_export_options  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2860 "bison_parser.cpp"
        break;

    case YYSYMBOL_import_export_options: /* import_export_options  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).import_export_option_t)); }
#line 2866 "bison_parser.cpp"
        break;

    case YYSYMBOL_csv_option: /* csv_option  */
#line 338 "bison_parser.y"
            {
  free(((*yyvaluep).csv_option_t)->second);
  delete (((*yyvaluep).csv_option_t));
}
#line 2875 "bison_parser.cpp"
        break;

    case YYSYMBOL_export_statement: /* export_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).export_stmt)); }
#line 2881 "bison_parser.cpp"
        break;

    case YYSYMBOL_show_statement: /* show_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).show_stmt)); }
#line 2887 "bison_parser.cpp"
        break;

    case YYSYMBOL_create_statement: /* create_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).create_stmt)); }
#line 2893 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_not_exists: /* opt_not_exists  */
#line 311 "bison_parser.y"
            { }
#line 2899 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem_commalist: /* table_elem_commalist  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).table_element_vec)) {
    for (auto ptr : *(((*yyvaluep).table_element_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_element_vec));
}
#line 2912 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_elem: /* table_elem  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).table_element_t)); }
#line 2918 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_def: /* column_def  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).column_t)); }
#line 2924 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_type: /* column_type  */
#line 311 "bison_parser.y"
            { }
#line 2930 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_time_precision: /* opt_time_precision  */
#line 311 "bison_parser.y"
            { }
#line 2936 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_decimal_specification: /* opt_decimal_specification  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).ival_pair)); }
#line 2942 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_column_constraints: /* opt_column_constraints  */
#line 342 "bison_parser.y"
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
#line 2964 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraints: /* column_constraints  */
#line 342 "bison_parser.y"
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
#line 2986 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_constraint: /* column_constraint  */
#line 311 "bison_parser.y"
            { }
#line 2992 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_constraint: /* table_constraint  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).table_constraint_t)); }
#line 2998 "bison_parser.cpp"
        break;

    case YYSYMBOL_references_spec: /* references_spec  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).references_spec_t)); }
#line 3004 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_statement: /* drop_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).drop_stmt)); }
#line 3010 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_exists: /* opt_exists  */
#line 311 "bison_parser.y"
            { }
#line 3016 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_statement: /* alter_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).alter_stmt)); }
#line 3022 "bison_parser.cpp"
        break;

    case YYSYMBOL_alter_action: /* alter_action  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).alter_action_t)); }
#line 3028 "bison_parser.cpp"
        break;

    case YYSYMBOL_drop_action: /* drop_action  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).drop_action_t)); }
#line 3034 "bison_parser.cpp"
        break;

    case YYSYMBOL_delete_statement: /* delete_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 3040 "bison_parser.cpp"
        break;

    case YYSYMBOL_truncate_statement: /* truncate_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).delete_stmt)); }
#line 3046 "bison_parser.cpp"
        break;

    case YYSYMBOL_insert_statement: /* insert_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).insert_stmt)); }
#line 3052 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_column_list: /* opt_column_list  */
#line 321 "bison_parser.y"
            {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 3065 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_statement: /* update_statement  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).update_stmt)); }
#line 3071 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause_commalist: /* update_clause_commalist  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).update_vec)) {
    for (auto ptr : *(((*yyvaluep).update_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).update_vec));
}
#line 3084 "bison_parser.cpp"
        break;

    case YYSYMBOL_update_clause: /* update_clause  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).update_t)); }
#line 3090 "bison_parser.cpp"
        break;

    case YYSYMBOL_query_expression: /* query_expression  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 3096 "bison_parser.cpp"
        break;

    case YYSYMBOL_query_expression_body: /* query_expression_body  */
#line 320 "bison_parser.y"
            { delete ((*yyvaluep).query_body_t).head; }
#line 3102 "bison_parser.cpp"
        break;

    case YYSYMBOL_query_expression_parens: /* query_expression_parens  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 3108 "bison_parser.cpp"
        break;

    case YYSYMBOL_subquery: /* subquery  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 3114 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_operator: /* set_operator  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 3120 "bison_parser.cpp"
        break;

    case YYSYMBOL_set_type: /* set_type  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).set_operator_t)); }
#line 3126 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_all: /* opt_all  */
#line 311 "bison_parser.y"
            { }
#line 3132 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_hierarchical_clause: /* opt_hierarchical_clause  */
#line 316 "bison_parser.y"
            {
  delete ((*yyvaluep).hierarchical_t).startWith;
  delete ((*yyvaluep).hierarchical_t).connectBy;
}
#line 3141 "bison_parser.cpp"
        break;

    case YYSYMBOL_query_primary: /* query_primary  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).select_stmt)); }
#line 3147 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_distinct: /* opt_distinct  */
#line 311 "bison_parser.y"
            { }
#line 3153 "bison_parser.cpp"
        break;

    case YYSYMBOL_select_list: /* select_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3166 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_from_clause: /* opt_from_clause  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3172 "bison_parser.cpp"
        break;

    case YYSYMBOL_from_clause: /* from_clause  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3178 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_where: /* opt_where  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3184 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_group: /* opt_group  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).group_t)); }
#line 3190 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_having: /* opt_having  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3196 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_within_group: /* opt_within_group  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 3209 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order: /* opt_order  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 3222 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_list: /* order_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 3235 "bison_parser.cpp"
        break;

    case YYSYMBOL_order_desc: /* order_desc  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).order)); }
#line 3241 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_order_type: /* opt_order_type  */
#line 311 "bison_parser.y"
            { }
#line 3247 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_null_ordering: /* opt_null_ordering  */
#line 311 "bison_parser.y"
            { }
#line 3253 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_top: /* opt_top  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 3259 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_limit: /* opt_limit  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).limit)); }
#line 3265 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_list: /* expr_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3278 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_extended_literal_list: /* opt_extended_literal_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3291 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal_list: /* extended_literal_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3304 "bison_parser.cpp"
        break;

    case YYSYMBOL_casted_extended_literal: /* casted_extended_literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3310 "bison_parser.cpp"
        break;

    case YYSYMBOL_extended_literal: /* extended_literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3316 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr_alias: /* expr_alias  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3322 "bison_parser.cpp"
        break;

    case YYSYMBOL_expr: /* expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3328 "bison_parser.cpp"
        break;

    case YYSYMBOL_operand: /* operand  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3334 "bison_parser.cpp"
        break;

    case YYSYMBOL_scalar_expr: /* scalar_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3340 "bison_parser.cpp"
        break;

    case YYSYMBOL_unary_expr: /* unary_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3346 "bison_parser.cpp"
        break;

    case YYSYMBOL_binary_expr: /* binary_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3352 "bison_parser.cpp"
        break;

    case YYSYMBOL_logic_expr: /* logic_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3358 "bison_parser.cpp"
        break;

    case YYSYMBOL_in_expr: /* in_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3364 "bison_parser.cpp"
        break;

    case YYSYMBOL_in_row_list: /* in_row_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3377 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_expr_list: /* row_expr_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3390 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_expr: /* case_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3396 "bison_parser.cpp"
        break;

    case YYSYMBOL_case_list: /* case_list  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3402 "bison_parser.cpp"
        break;

    case YYSYMBOL_exists_expr: /* exists_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3408 "bison_parser.cpp"
        break;

    case YYSYMBOL_comp_expr: /* comp_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3414 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_function_expr: /* table_function_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3420 "bison_parser.cpp"
        break;

    case YYSYMBOL_function_expr: /* function_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3426 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_window: /* opt_window  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).window_description)); }
#line 3432 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_partition: /* opt_partition  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3445 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_frame_clause: /* opt_frame_clause  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).frame_description)); }
#line 3451 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_type: /* frame_type  */
#line 311 "bison_parser.y"
            { }
#line 3457 "bison_parser.cpp"
        break;

    case YYSYMBOL_frame_bound: /* frame_bound  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).frame_bound)); }
#line 3463 "bison_parser.cpp"
        break;

    case YYSYMBOL_extract_expr: /* extract_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3469 "bison_parser.cpp"
        break;

    case YYSYMBOL_next_value_expr: /* next_value_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3475 "bison_parser.cpp"
        break;

    case YYSYMBOL_cast_expr: /* cast_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3481 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field: /* datetime_field  */
#line 311 "bison_parser.y"
            { }
#line 3487 "bison_parser.cpp"
        break;

    case YYSYMBOL_datetime_field_plural: /* datetime_field_plural  */
#line 311 "bison_parser.y"
            { }
#line 3493 "bison_parser.cpp"
        break;

    case YYSYMBOL_duration_field: /* duration_field  */
#line 311 "bison_parser.y"
            { }
#line 3499 "bison_parser.cpp"
        break;

    case YYSYMBOL_interval_range: /* interval_range  */
#line 311 "bison_parser.y"
            { }
#line 3505 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_expr: /* array_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3511 "bison_parser.cpp"
        break;

    case YYSYMBOL_array_index: /* array_index  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3517 "bison_parser.cpp"
        break;

    case YYSYMBOL_between_expr: /* between_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3523 "bison_parser.cpp"
        break;

    case YYSYMBOL_column_name: /* column_name  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3529 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonreserved_keyword: /* nonreserved_keyword  */
#line 329 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3535 "bison_parser.cpp"
        break;

    case YYSYMBOL_name_after_qualifier: /* name_after_qualifier  */
#line 329 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3541 "bison_parser.cpp"
        break;

    case YYSYMBOL_qualified_name_or_keyword: /* qualified_name_or_keyword  */
#line 329 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3547 "bison_parser.cpp"
        break;

    case YYSYMBOL_name_or_keyword: /* name_or_keyword  */
#line 329 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3553 "bison_parser.cpp"
        break;

    case YYSYMBOL_literal: /* literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3559 "bison_parser.cpp"
        break;

    case YYSYMBOL_string_literal: /* string_literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3565 "bison_parser.cpp"
        break;

    case YYSYMBOL_bool_literal: /* bool_literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3571 "bison_parser.cpp"
        break;

    case YYSYMBOL_num_literal: /* num_literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3577 "bison_parser.cpp"
        break;

    case YYSYMBOL_int_literal: /* int_literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3583 "bison_parser.cpp"
        break;

    case YYSYMBOL_null_literal: /* null_literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3589 "bison_parser.cpp"
        break;

    case YYSYMBOL_date_literal: /* date_literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3595 "bison_parser.cpp"
        break;

    case YYSYMBOL_time_literal: /* time_literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3601 "bison_parser.cpp"
        break;

    case YYSYMBOL_timestamp_literal: /* timestamp_literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3607 "bison_parser.cpp"
        break;

    case YYSYMBOL_interval_literal: /* interval_literal  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3613 "bison_parser.cpp"
        break;

    case YYSYMBOL_param_expr: /* param_expr  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3619 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref: /* table_ref  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3625 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_atomic: /* table_ref_atomic  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3631 "bison_parser.cpp"
        break;

    case YYSYMBOL_nonjoin_table_ref_atomic: /* nonjoin_table_ref_atomic  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3637 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_value_row_list: /* table_value_row_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 3650 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_commalist: /* table_ref_commalist  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).table_vec)) {
    for (auto ptr : *(((*yyvaluep).table_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_vec));
}
#line 3663 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name: /* table_ref_name  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3669 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_ref_name_no_alias: /* table_ref_name_no_alias  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3675 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_name: /* table_name  */
#line 312 "bison_parser.y"
            {
  free(((*yyvaluep).table_name).name);
  free(((*yyvaluep).table_name).schema);
}
#line 3684 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_index_name: /* opt_index_name  */
#line 329 "bison_parser.y"
            { free(((*yyvaluep).sval)); }
#line 3690 "bison_parser.cpp"
        break;

    case YYSYMBOL_table_alias: /* table_alias  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3696 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_table_alias: /* opt_table_alias  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3702 "bison_parser.cpp"
        break;

    case YYSYMBOL_alias: /* alias  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3708 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_alias: /* opt_alias  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).alias_t)); }
#line 3714 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause: /* opt_locking_clause  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).locking_clause_vec)) {
    for (auto ptr : *(((*yyvaluep).locking_clause_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).locking_clause_vec));
}
#line 3727 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_locking_clause_list: /* opt_locking_clause_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).locking_clause_vec)) {
    for (auto ptr : *(((*yyvaluep).locking_clause_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).locking_clause_vec));
}
#line 3740 "bison_parser.cpp"
        break;

    case YYSYMBOL_locking_clause: /* locking_clause  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).locking_t)); }
#line 3746 "bison_parser.cpp"
        break;

    case YYSYMBOL_row_lock_mode: /* row_lock_mode  */
#line 311 "bison_parser.y"
            { }
#line 3752 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_row_lock_policy: /* opt_row_lock_policy  */
#line 311 "bison_parser.y"
            { }
#line 3758 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_clause: /* with_clause  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3771 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description_list: /* with_description_list  */
#line 330 "bison_parser.y"
            {
  if (((*yyvaluep).with_description_vec)) {
    for (auto ptr : *(((*yyvaluep).with_description_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).with_description_vec));
}
#line 3784 "bison_parser.cpp"
        break;

    case YYSYMBOL_with_description: /* with_description  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).with_description_t)); }
#line 3790 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_clause: /* join_clause  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).table)); }
#line 3796 "bison_parser.cpp"
        break;

    case YYSYMBOL_opt_join_type: /* opt_join_type  */
#line 311 "bison_parser.y"
            { }
#line 3802 "bison_parser.cpp"
        break;

    case YYSYMBOL_join_condition: /* join_condition  */
#line 359 "bison_parser.y"
            { delete (((*yyvaluep).expr)); }
#line 3808 "bison_parser.cpp"
        break;

    case YYSYMBOL_ident_commalist: /* ident_commalist  */
#line 321 "bison_parser.y"
            {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 3821 "bison_parser.cpp"
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
#line 209 "bison_parser.y"
{
  // Initialize
  yylloc.first_column = 0;
  yylloc.last_column = 0;
  yylloc.first_line = 0;
  yylloc.last_line = 0;
  yylloc.total_column = 0;
  yylloc.string_length = 0;
}

#line 3929 "bison_parser.cpp"

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
#line 510 "bison_parser.y"
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
#line 4159 "bison_parser.cpp"
    break;

  case 3: /* statement_list: statement  */
#line 530 "bison_parser.y"
                           {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyval.stmt_vec) = new std::vector<SQLStatement*>();
  (yyval.stmt_vec)->push_back((yyvsp[0].statement));
}
#line 4170 "bison_parser.cpp"
    break;

  case 4: /* statement_list: statement_list ';' statement  */
#line 536 "bison_parser.y"
                               {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyvsp[-2].stmt_vec)->push_back((yyvsp[0].statement));
  (yyval.stmt_vec) = (yyvsp[-2].stmt_vec);
}
#line 4181 "bison_parser.cpp"
    break;

  case 5: /* statement: prepare_statement opt_hints  */
#line 543 "bison_parser.y"
                                        {
  (yyval.statement) = (yyvsp[-1].prep_stmt);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 4190 "bison_parser.cpp"
    break;

  case 6: /* statement: preparable_statement opt_hints  */
#line 547 "bison_parser.y"
                                 {
  (yyval.statement) = (yyvsp[-1].statement);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 4199 "bison_parser.cpp"
    break;

  case 7: /* statement: show_statement  */
#line 551 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].show_stmt); }
#line 4205 "bison_parser.cpp"
    break;

  case 8: /* statement: import_statement  */
#line 552 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].import_stmt); }
#line 4211 "bison_parser.cpp"
    break;

  case 9: /* statement: export_statement  */
#line 553 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].export_stmt); }
#line 4217 "bison_parser.cpp"
    break;

  case 10: /* preparable_statement: query_expression  */
#line 555 "bison_parser.y"
                                        { (yyval.statement) = (yyvsp[0].select_stmt); }
#line 4223 "bison_parser.cpp"
    break;

  case 11: /* preparable_statement: create_statement  */
#line 556 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].create_stmt); }
#line 4229 "bison_parser.cpp"
    break;

  case 12: /* preparable_statement: insert_statement  */
#line 557 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].insert_stmt); }
#line 4235 "bison_parser.cpp"
    break;

  case 13: /* preparable_statement: delete_statement  */
#line 558 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 4241 "bison_parser.cpp"
    break;

  case 14: /* preparable_statement: truncate_statement  */
#line 559 "bison_parser.y"
                     { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 4247 "bison_parser.cpp"
    break;

  case 15: /* preparable_statement: update_statement  */
#line 560 "bison_parser.y"
                   { (yyval.statement) = (yyvsp[0].update_stmt); }
#line 4253 "bison_parser.cpp"
    break;

  case 16: /* preparable_statement: drop_statement  */
#line 561 "bison_parser.y"
                 { (yyval.statement) = (yyvsp[0].drop_stmt); }
#line 4259 "bison_parser.cpp"
    break;

  case 17: /* preparable_statement: alter_statement  */
#line 562 "bison_parser.y"
                  { (yyval.statement) = (yyvsp[0].alter_stmt); }
#line 4265 "bison_parser.cpp"
    break;

  case 18: /* preparable_statement: execute_statement  */
#line 563 "bison_parser.y"
                    { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 4271 "bison_parser.cpp"
    break;

  case 19: /* preparable_statement: odbc_call_statement  */
#line 564 "bison_parser.y"
                      { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 4277 "bison_parser.cpp"
    break;

  case 20: /* preparable_statement: transaction_statement  */
#line 565 "bison_parser.y"
                        { (yyval.statement) = (yyvsp[0].transaction_stmt); }
#line 4283 "bison_parser.cpp"
    break;

  case 21: /* opt_hints: WITH HINT '(' hint_list ')'  */
#line 571 "bison_parser.y"
                                        { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 4289 "bison_parser.cpp"
    break;

  case 22: /* opt_hints: %empty  */
#line 572 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 4295 "bison_parser.cpp"
    break;

  case 23: /* hint_list: hint  */
#line 574 "bison_parser.y"
                 {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 4304 "bison_parser.cpp"
    break;

  case 24: /* hint_list: hint_list ',' hint  */
#line 578 "bison_parser.y"
                     {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 4313 "bison_parser.cpp"
    break;

  case 25: /* hint: IDENTIFIER  */
#line 583 "bison_parser.y"
                  {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[0].sval);
}
#line 4322 "bison_parser.cpp"
    break;

  case 26: /* hint: IDENTIFIER '(' extended_literal_list ')'  */
#line 587 "bison_parser.y"
                                           {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[-3].sval);
  (yyval.expr)->exprList = (yyvsp[-1].expr_vec);
}
#line 4332 "bison_parser.cpp"
    break;

  case 27: /* transaction_statement: BEGIN opt_transaction_keyword  */
#line 597 "bison_parser.y"
                                                      { (yyval.transaction_stmt) = new TransactionStatement(kBeginTransaction); }
#line 4338 "bison_parser.cpp"
    break;

  case 28: /* transaction_statement: ROLLBACK opt_transaction_keyword  */
#line 598 "bison_parser.y"
                                   { (yyval.transaction_stmt) = new TransactionStatement(kRollbackTransaction); }
#line 4344 "bison_parser.cpp"
    break;

  case 29: /* transaction_statement: COMMIT opt_transaction_keyword  */
#line 599 "bison_parser.y"
                                 { (yyval.transaction_stmt) = new TransactionStatement(kCommitTransaction); }
#line 4350 "bison_parser.cpp"
    break;

  case 32: /* prepare_statement: PREPARE IDENTIFIER FROM prepare_target_query  */
#line 607 "bison_parser.y"
                                                                 {
  (yyval.prep_stmt) = new PrepareStatement();
  (yyval.prep_stmt)->name = (yyvsp[-2].sval);
  (yyval.prep_stmt)->query = (yyvsp[0].sval);
}
#line 4360 "bison_parser.cpp"
    break;

  case 34: /* odbc_call_statement: '{' opt_odbc_return CALL table_name opt_call_args '}'  */
#line 619 "bison_parser.y"
                                                                            {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->returnValue = (yyvsp[-4].expr);
  (yyval.exec_stmt)->name = qualifiedName((yyvsp[-2].table_name));
  (yyval.exec_stmt)->parameters = (yyvsp[-1].expr_vec);
}
#line 4371 "bison_parser.cpp"
    break;

  case 35: /* opt_odbc_return: param_expr '='  */
#line 628 "bison_parser.y"
                                 { (yyval.expr) = (yyvsp[-1].expr); }
#line 4377 "bison_parser.cpp"
    break;

  case 36: /* opt_odbc_return: %empty  */
#line 629 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 4383 "bison_parser.cpp"
    break;

  case 37: /* opt_call_args: '(' opt_extended_literal_list ')'  */
#line 631 "bison_parser.y"
                                                  { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 4389 "bison_parser.cpp"
    break;

  case 38: /* opt_call_args: %empty  */
#line 632 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 4395 "bison_parser.cpp"
    break;

  case 39: /* execute_statement: EXECUTE IDENTIFIER opt_call_args  */
#line 634 "bison_parser.y"
                                                     {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[-1].sval);
  (yyval.exec_stmt)->parameters = (yyvsp[0].expr_vec);
}
#line 4405 "bison_parser.cpp"
    break;

  case 40: /* import_statement: IMPORT FROM file_type FILE file_path INTO table_name  */
#line 646 "bison_parser.y"
                                                                        {
  (yyval.import_stmt) = new ImportStatement((yyvsp[-4].import_type_t));
  (yyval.import_stmt)->filePath = (yyvsp[-2].sval);
  (yyval.import_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.import_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 4416 "bison_parser.cpp"
    break;

  case 41: /* import_statement: COPY table_name FROM file_path opt_import_export_options opt_where  */
#line 652 "bison_parser.y"
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
#line 4437 "bison_parser.cpp"
    break;

  case 42: /* file_type: IDENTIFIER  */
#line 669 "bison_parser.y"
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
#line 4456 "bison_parser.cpp"
    break;

  case 43: /* file_path: STRING  */
#line 684 "bison_parser.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 4462 "bison_parser.cpp"
    break;

  case 44: /* opt_import_export_options: WITH '(' import_export_options ')'  */
#line 686 "bison_parser.y"
                                                               { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 4468 "bison_parser.cpp"
    break;

  case 45: /* opt_import_export_options: '(' import_export_options ')'  */
#line 687 "bison_parser.y"
                                { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 4474 "bison_parser.cpp"
    break;

  case 46: /* opt_import_export_options: %empty  */
#line 688 "bison_parser.y"
              { (yyval.import_export_option_t) = new ImportExportOptions{}; }
#line 4480 "bison_parser.cpp"
    break;

  case 47: /* import_export_options: import_export_options ',' FORMAT file_type  */
#line 690 "bison_parser.y"
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
#line 4499 "bison_parser.cpp"
    break;

  case 48: /* import_export_options: FORMAT file_type  */
#line 704 "bison_parser.y"
                   {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->format = (yyvsp[0].import_type_t);
}
#line 4508 "bison_parser.cpp"
    break;

  case 49: /* import_export_options: import_export_options ',' ENCODING STRING  */
#line 708 "bison_parser.y"
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
#line 4523 "bison_parser.cpp"
    break;

  case 50: /* import_export_options: ENCODING STRING  */
#line 718 "bison_parser.y"
                  {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->encoding = (yyvsp[0].sval);
}
#line 4532 "bison_parser.cpp"
    break;

  case 51: /* import_export_options: import_export_options ',' csv_option  */
#line 722 "bison_parser.y"
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
#line 4561 "bison_parser.cpp"
    break;

  case 52: /* import_export_options: csv_option  */
#line 746 "bison_parser.y"
             {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->csv_options = new CsvOptions{};
  (yyval.import_export_option_t)->csv_options->accept_csv_option((yyvsp[0].csv_option_t));

  delete (yyvsp[0].csv_option_t);
}
#line 4573 "bison_parser.cpp"
    break;

  case 53: /* csv_option: IDENTIFIER STRING  */
#line 754 "bison_parser.y"
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
#line 4591 "bison_parser.cpp"
    break;

  case 54: /* csv_option: NULL STRING  */
#line 767 "bison_parser.y"
              { (yyval.csv_option_t) = new std::pair<CsvOptionType, char*>(CsvOptionType::Null, (yyvsp[0].sval)); }
#line 4597 "bison_parser.cpp"
    break;

  case 55: /* export_statement: COPY table_name TO file_path opt_import_export_options  */
#line 774 "bison_parser.y"
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
#line 4617 "bison_parser.cpp"
    break;

  case 56: /* export_statement: COPY subquery TO file_path opt_import_export_options  */
#line 789 "bison_parser.y"
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
#line 4636 "bison_parser.cpp"
    break;

  case 57: /* show_statement: SHOW TABLES  */
#line 809 "bison_parser.y"
                             { (yyval.show_stmt) = new ShowStatement(kShowTables); }
#line 4642 "bison_parser.cpp"
    break;

  case 58: /* show_statement: SHOW COLUMNS table_name  */
#line 810 "bison_parser.y"
                          {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4652 "bison_parser.cpp"
    break;

  case 59: /* show_statement: DESCRIBE table_name  */
#line 815 "bison_parser.y"
                      {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4662 "bison_parser.cpp"
    break;

  case 60: /* create_statement: CREATE TABLE opt_not_exists table_name FROM IDENTIFIER FILE file_path  */
#line 826 "bison_parser.y"
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
#line 4682 "bison_parser.cpp"
    break;

  case 61: /* create_statement: CREATE TABLE opt_not_exists table_name '(' table_elem_commalist ')'  */
#line 841 "bison_parser.y"
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
#line 4699 "bison_parser.cpp"
    break;

  case 62: /* create_statement: CREATE TABLE opt_not_exists table_name AS query_expression  */
#line 853 "bison_parser.y"
                                                             {
  (yyval.create_stmt) = new CreateStatement(kCreateTable);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-3].bval);
  (yyval.create_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4711 "bison_parser.cpp"
    break;

  case 63: /* create_statement: CREATE INDEX opt_not_exists opt_index_name ON table_name '(' ident_commalist ')'  */
#line 860 "bison_parser.y"
                                                                                   {
  (yyval.create_stmt) = new CreateStatement(kCreateIndex);
  (yyval.create_stmt)->indexName = (yyvsp[-5].sval);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-6].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->indexColumns = (yyvsp[-1].str_vec);
}
#line 4724 "bison_parser.cpp"
    break;

  case 64: /* create_statement: CREATE VIEW opt_not_exists table_name opt_column_list AS query_expression  */
#line 868 "bison_parser.y"
                                                                            {
  (yyval.create_stmt) = new CreateStatement(kCreateView);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-4].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->viewColumns = (yyvsp[-2].str_vec);
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4737 "bison_parser.cpp"
    break;

  case 65: /* opt_not_exists: IF NOT EXISTS  */
#line 877 "bison_parser.y"
                               { (yyval.bval) = true; }
#line 4743 "bison_parser.cpp"
    break;

  case 66: /* opt_not_exists: %empty  */
#line 878 "bison_parser.y"
              { (yyval.bval) = false; }
#line 4749 "bison_parser.cpp"
    break;

  case 67: /* table_elem_commalist: table_elem  */
#line 880 "bison_parser.y"
                                  {
  (yyval.table_element_vec) = new std::vector<TableElement*>();
  (yyval.table_element_vec)->push_back((yyvsp[0].table_element_t));
}
#line 4758 "bison_parser.cpp"
    break;

  case 68: /* table_elem_commalist: table_elem_commalist ',' table_elem  */
#line 884 "bison_parser.y"
                                      {
  (yyvsp[-2].table_element_vec)->push_back((yyvsp[0].table_element_t));
  (yyval.table_element_vec) = (yyvsp[-2].table_element_vec);
}
#line 4767 "bison_parser.cpp"
    break;

  case 69: /* table_elem: column_def  */
#line 889 "bison_parser.y"
                        { (yyval.table_element_t) = (yyvsp[0].column_t); }
#line 4773 "bison_parser.cpp"
    break;

  case 70: /* table_elem: table_constraint  */
#line 890 "bison_parser.y"
                   { (yyval.table_element_t) = (yyvsp[0].table_constraint_t); }
#line 4779 "bison_parser.cpp"
    break;

  case 71: /* column_def: name_or_keyword column_type opt_column_constraints  */
#line 892 "bison_parser.y"
                                                                {
  (yyval.column_t) = new ColumnDefinition((yyvsp[-2].sval), (yyvsp[-1].column_type_t), (yyvsp[0].column_constraints_t)->constraints, (yyvsp[0].column_constraints_t)->references);
  if (!(yyval.column_t)->trySetNullableExplicit()) {
    yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{(yyvsp[-2].sval)}).c_str());
  }
  delete (yyvsp[0].column_constraints_t);
}
#line 4791 "bison_parser.cpp"
    break;

  case 72: /* column_type: BIGINT  */
#line 900 "bison_parser.y"
                     { (yyval.column_type_t) = ColumnType{DataType::BIGINT}; }
#line 4797 "bison_parser.cpp"
    break;

  case 73: /* column_type: BOOLEAN  */
#line 901 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::BOOLEAN}; }
#line 4803 "bison_parser.cpp"
    break;

  case 74: /* column_type: CHAR '(' INTVAL ')'  */
#line 902 "bison_parser.y"
                      { (yyval.column_type_t) = ColumnType{DataType::CHAR, (yyvsp[-1].ival)}; }
#line 4809 "bison_parser.cpp"
    break;

  case 75: /* column_type: CHARACTER_VARYING '(' INTVAL ')'  */
#line 903 "bison_parser.y"
                                   { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4815 "bison_parser.cpp"
    break;

  case 76: /* column_type: DATE  */
#line 904 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::DATE}; }
#line 4821 "bison_parser.cpp"
    break;

  case 77: /* column_type: DATETIME  */
#line 905 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4827 "bison_parser.cpp"
    break;

  case 78: /* column_type: DECIMAL opt_decimal_specification  */
#line 906 "bison_parser.y"
                                    {
  (yyval.column_type_t) = ColumnType{DataType::DECIMAL, 0, (yyvsp[0].ival_pair)->first, (yyvsp[0].ival_pair)->second};
  delete (yyvsp[0].ival_pair);
}
#line 4836 "bison_parser.cpp"
    break;

  case 79: /* column_type: DOUBLE  */
#line 910 "bison_parser.y"
         { (yyval.column_type_t) = ColumnType{DataType::DOUBLE}; }
#line 4842 "bison_parser.cpp"
    break;

  case 80: /* column_type: FLOAT  */
#line 911 "bison_parser.y"
        { (yyval.column_type_t) = ColumnType{DataType::FLOAT}; }
#line 4848 "bison_parser.cpp"
    break;

  case 81: /* column_type: INT  */
#line 912 "bison_parser.y"
      { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4854 "bison_parser.cpp"
    break;

  case 82: /* column_type: INTEGER  */
#line 913 "bison_parser.y"
          { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 4860 "bison_parser.cpp"
    break;

  case 83: /* column_type: INTERVAL  */
#line 914 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::INTERVAL}; }
#line 4866 "bison_parser.cpp"
    break;

  case 84: /* column_type: INTERVAL datetime_field  */
#line 915 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::INTERVAL, 0, 0, 0, (yyvsp[0].datetime_field)}; }
#line 4872 "bison_parser.cpp"
    break;

  case 85: /* column_type: INTERVAL interval_range  */
#line 916 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::INTERVAL, 0, 0, 0, (yyvsp[0].datetime_field)}; }
#line 4878 "bison_parser.cpp"
    break;

  case 86: /* column_type: LONG  */
#line 917 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::LONG}; }
#line 4884 "bison_parser.cpp"
    break;

  case 87: /* column_type: REAL  */
#line 918 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::REAL}; }
#line 4890 "bison_parser.cpp"
    break;

  case 88: /* column_type: SMALLINT  */
#line 919 "bison_parser.y"
           { (yyval.column_type_t) = ColumnType{DataType::SMALLINT}; }
#line 4896 "bison_parser.cpp"
    break;

  case 89: /* column_type: TEXT  */
#line 920 "bison_parser.y"
       { (yyval.column_type_t) = ColumnType{DataType::TEXT}; }
#line 4902 "bison_parser.cpp"
    break;

  case 90: /* column_type: TIME opt_time_precision  */
#line 921 "bison_parser.y"
                          { (yyval.column_type_t) = ColumnType{DataType::TIME, 0, (yyvsp[0].ival)}; }
#line 4908 "bison_parser.cpp"
    break;

  case 91: /* column_type: TIMESTAMP  */
#line 922 "bison_parser.y"
            { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 4914 "bison_parser.cpp"
    break;

  case 92: /* column_type: VARCHAR '(' INTVAL ')'  */
#line 923 "bison_parser.y"
                         { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 4920 "bison_parser.cpp"
    break;

  case 93: /* opt_time_precision: '(' INTVAL ')'  */
#line 925 "bison_parser.y"
                                    { (yyval.ival) = (yyvsp[-1].ival); }
#line 4926 "bison_parser.cpp"
    break;

  case 94: /* opt_time_precision: %empty  */
#line 926 "bison_parser.y"
              { (yyval.ival) = 0; }
#line 4932 "bison_parser.cpp"
    break;

  case 95: /* opt_decimal_specification: '(' INTVAL ',' INTVAL ')'  */
#line 928 "bison_parser.y"
                                                      { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-3].ival), (yyvsp[-1].ival)}; }
#line 4938 "bison_parser.cpp"
    break;

  case 96: /* opt_decimal_specification: '(' INTVAL ')'  */
#line 929 "bison_parser.y"
                 { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-1].ival), 0}; }
#line 4944 "bison_parser.cpp"
    break;

  case 97: /* opt_decimal_specification: %empty  */
#line 930 "bison_parser.y"
              { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{0, 0}; }
#line 4950 "bison_parser.cpp"
    break;

  case 98: /* opt_column_constraints: column_constraints  */
#line 932 "bison_parser.y"
                                            { (yyval.column_constraints_t) = (yyvsp[0].column_constraints_t); }
#line 4956 "bison_parser.cpp"
    break;

  case 99: /* opt_column_constraints: %empty  */
#line 933 "bison_parser.y"
              { (yyval.column_constraints_t) = new ColumnConstraints(); }
#line 4962 "bison_parser.cpp"
    break;

  case 100: /* column_constraints: column_constraint  */
#line 935 "bison_parser.y"
                                       {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
}
#line 4971 "bison_parser.cpp"
    break;

  case 101: /* column_constraints: column_constraints column_constraint  */
#line 939 "bison_parser.y"
                                       {
  (yyvsp[-1].column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 4980 "bison_parser.cpp"
    break;

  case 102: /* column_constraints: references_spec  */
#line 943 "bison_parser.y"
                  {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyval.column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
}
#line 4990 "bison_parser.cpp"
    break;

  case 103: /* column_constraints: column_constraints references_spec  */
#line 948 "bison_parser.y"
                                     {
  // Multiple foreign keys for the same column could be possible, so we do not raise an error in that case.
  // Think of foreign keys referenced on multiple levels (returned item references sold item references items).
  (yyvsp[-1].column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyvsp[-1].column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 5002 "bison_parser.cpp"
    break;

  case 104: /* column_constraint: PRIMARY KEY  */
#line 956 "bison_parser.y"
                                { (yyval.column_constraint_t) = ConstraintType::PrimaryKey; }
#line 5008 "bison_parser.cpp"
    break;

  case 105: /* column_constraint: UNIQUE  */
#line 957 "bison_parser.y"
         { (yyval.column_constraint_t) = ConstraintType::Unique; }
#line 5014 "bison_parser.cpp"
    break;

  case 106: /* column_constraint: NULL  */
#line 958 "bison_parser.y"
       { (yyval.column_constraint_t) = ConstraintType::Null; }
#line 5020 "bison_parser.cpp"
    break;

  case 107: /* column_constraint: NOT NULL  */
#line 959 "bison_parser.y"
           { (yyval.column_constraint_t) = ConstraintType::NotNull; }
#line 5026 "bison_parser.cpp"
    break;

  case 108: /* table_constraint: PRIMARY KEY '(' ident_commalist ')'  */
#line 961 "bison_parser.y"
                                                       { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::PrimaryKey, (yyvsp[-1].str_vec)); }
#line 5032 "bison_parser.cpp"
    break;

  case 109: /* table_constraint: UNIQUE '(' ident_commalist ')'  */
#line 962 "bison_parser.y"
                                 { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::Unique, (yyvsp[-1].str_vec)); }
#line 5038 "bison_parser.cpp"
    break;

  case 110: /* table_constraint: FOREIGN KEY '(' ident_commalist ')' references_spec  */
#line 963 "bison_parser.y"
                                                      { (yyval.table_constraint_t) = new ForeignKeyConstraint((yyvsp[-2].str_vec), (yyvsp[0].references_spec_t)); }
#line 5044 "bison_parser.cpp"
    break;

  case 111: /* references_spec: REFERENCES table_name opt_column_list  */
#line 965 "bison_parser.y"
                                                        { (yyval.references_spec_t) = new ReferencesSpecification((yyvsp[-1].table_name).schema, (yyvsp[-1].table_name).name, (yyvsp[0].str_vec)); }
#line 5050 "bison_parser.cpp"
    break;

  case 112: /* drop_statement: DROP TABLE opt_exists table_name  */
#line 973 "bison_parser.y"
                                                  {
  (yyval.drop_stmt) = new DropStatement(kDropTable);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 5061 "bison_parser.cpp"
    break;

  case 113: /* drop_statement: DROP VIEW opt_exists table_name  */
#line 979 "bison_parser.y"
                                  {
  (yyval.drop_stmt) = new DropStatement(kDropView);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 5072 "bison_parser.cpp"
    break;

  case 114: /* drop_statement: DEALLOCATE PREPARE IDENTIFIER  */
#line 985 "bison_parser.y"
                                {
  (yyval.drop_stmt) = new DropStatement(kDropPreparedStatement);
  (yyval.drop_stmt)->ifExists = false;
  (yyval.drop_stmt)->name = (yyvsp[0].sval);
}
#line 5082 "bison_parser.cpp"
    break;

  case 115: /* drop_statement: DROP INDEX opt_exists name_or_keyword  */
#line 991 "bison_parser.y"
                                        {
  (yyval.drop_stmt) = new DropStatement(kDropIndex);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->indexName = (yyvsp[0].sval);
}
#line 5092 "bison_parser.cpp"
    break;

  case 116: /* opt_exists: IF EXISTS  */
#line 997 "bison_parser.y"
                       { (yyval.bval) = true; }
#line 5098 "bison_parser.cpp"
    break;

  case 117: /* opt_exists: %empty  */
#line 998 "bison_parser.y"
              { (yyval.bval) = false; }
#line 5104 "bison_parser.cpp"
    break;

  case 118: /* alter_statement: ALTER TABLE opt_exists table_name alter_action  */
#line 1005 "bison_parser.y"
                                                                 {
  (yyval.alter_stmt) = new AlterStatement((yyvsp[-1].table_name).name, (yyvsp[0].alter_action_t));
  (yyval.alter_stmt)->ifTableExists = (yyvsp[-2].bval);
  (yyval.alter_stmt)->schema = (yyvsp[-1].table_name).schema;
}
#line 5114 "bison_parser.cpp"
    break;

  case 119: /* alter_action: drop_action  */
#line 1011 "bison_parser.y"
                           { (yyval.alter_action_t) = (yyvsp[0].drop_action_t); }
#line 5120 "bison_parser.cpp"
    break;

  case 120: /* drop_action: DROP COLUMN opt_exists name_or_keyword  */
#line 1013 "bison_parser.y"
                                                     {
  (yyval.drop_action_t) = new DropColumnAction((yyvsp[0].sval));
  (yyval.drop_action_t)->ifExists = (yyvsp[-1].bval);
}
#line 5129 "bison_parser.cpp"
    break;

  case 121: /* delete_statement: DELETE FROM table_name opt_where  */
#line 1023 "bison_parser.y"
                                                    {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[-1].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[-1].table_name).name;
  (yyval.delete_stmt)->expr = (yyvsp[0].expr);
}
#line 5140 "bison_parser.cpp"
    break;

  case 122: /* truncate_statement: TRUNCATE table_name  */
#line 1030 "bison_parser.y"
                                         {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 5150 "bison_parser.cpp"
    break;

  case 123: /* insert_statement: INSERT INTO table_name opt_column_list VALUES '(' extended_literal_list ')'  */
#line 1041 "bison_parser.y"
                                                                                               {
  (yyval.insert_stmt) = new InsertStatement(kInsertValues);
  (yyval.insert_stmt)->schema = (yyvsp[-5].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-5].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-4].str_vec);
  (yyval.insert_stmt)->values = (yyvsp[-1].expr_vec);
}
#line 5162 "bison_parser.cpp"
    break;

  case 124: /* insert_statement: INSERT INTO table_name query_expression  */
#line 1051 "bison_parser.y"
                                          {
  (yyval.insert_stmt) = new InsertStatement(kInsertSelect);
  (yyval.insert_stmt)->schema = (yyvsp[-1].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-1].table_name).name;
  (yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
}
#line 5173 "bison_parser.cpp"
    break;

  case 125: /* insert_statement: INSERT INTO table_name '(' ident_commalist ')' query_expression  */
#line 1057 "bison_parser.y"
                                                                  {
  (yyval.insert_stmt) = new InsertStatement(kInsertSelect);
  (yyval.insert_stmt)->schema = (yyvsp[-4].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-4].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-2].str_vec);
  (yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
}
#line 5185 "bison_parser.cpp"
    break;

  case 126: /* opt_column_list: '(' ident_commalist ')'  */
#line 1065 "bison_parser.y"
                                          { (yyval.str_vec) = (yyvsp[-1].str_vec); }
#line 5191 "bison_parser.cpp"
    break;

  case 127: /* opt_column_list: %empty  */
#line 1066 "bison_parser.y"
              { (yyval.str_vec) = nullptr; }
#line 5197 "bison_parser.cpp"
    break;

  case 128: /* update_statement: UPDATE table_ref_name_no_alias SET update_clause_commalist opt_where  */
#line 1073 "bison_parser.y"
                                                                                        {
  (yyval.update_stmt) = new UpdateStatement();
  (yyval.update_stmt)->table = (yyvsp[-3].table);
  (yyval.update_stmt)->updates = (yyvsp[-1].update_vec);
  (yyval.update_stmt)->where = (yyvsp[0].expr);
}
#line 5208 "bison_parser.cpp"
    break;

  case 129: /* update_clause_commalist: update_clause  */
#line 1080 "bison_parser.y"
                                        {
  (yyval.update_vec) = new std::vector<UpdateClause*>();
  (yyval.update_vec)->push_back((yyvsp[0].update_t));
}
#line 5217 "bison_parser.cpp"
    break;

  case 130: /* update_clause_commalist: update_clause_commalist ',' update_clause  */
#line 1084 "bison_parser.y"
                                            {
  (yyvsp[-2].update_vec)->push_back((yyvsp[0].update_t));
  (yyval.update_vec) = (yyvsp[-2].update_vec);
}
#line 5226 "bison_parser.cpp"
    break;

  case 131: /* update_clause: name_or_keyword '=' expr  */
#line 1089 "bison_parser.y"
                                         {
  (yyval.update_t) = new UpdateClause();
  (yyval.update_t)->column = (yyvsp[-2].sval);
  (yyval.update_t)->value = (yyvsp[0].expr);
}
#line 5236 "bison_parser.cpp"
    break;

  case 132: /* query_expression: query_expression_body opt_order opt_limit opt_locking_clause  */
#line 1104 "bison_parser.y"
                                                                                {
  (yyval.select_stmt) = (yyvsp[-3].query_body_t).head;
  if (const char* error = attachQueryClauses((yyvsp[-3].query_body_t), nullptr, (yyvsp[-2].order_vec), (yyvsp[-1].limit), (yyvsp[0].locking_clause_vec))) {
    delete (yyval.select_stmt);
    deleteQueryClauses(nullptr, (yyvsp[-2].order_vec), (yyvsp[-1].limit), (yyvsp[0].locking_clause_vec));
    yyerror(&yyloc, result, scanner, error);
    YYERROR;
  }
}
#line 5250 "bison_parser.cpp"
    break;

  case 133: /* query_expression: with_clause query_expression_body opt_order opt_limit opt_locking_clause  */
#line 1113 "bison_parser.y"
                                                                           {
  (yyval.select_stmt) = (yyvsp[-3].query_body_t).head;
  if (const char* error = attachQueryClauses((yyvsp[-3].query_body_t), (yyvsp[-4].with_description_vec), (yyvsp[-2].order_vec), (yyvsp[-1].limit), (yyvsp[0].locking_clause_vec))) {
    delete (yyval.select_stmt);
    deleteQueryClauses((yyvsp[-4].with_description_vec), (yyvsp[-2].order_vec), (yyvsp[-1].limit), (yyvsp[0].locking_clause_vec));
    yyerror(&yyloc, result, scanner, error);
    YYERROR;
  }
}
#line 5264 "bison_parser.cpp"
    break;

  case 134: /* query_expression_body: query_primary  */
#line 1133 "bison_parser.y"
                                      { (yyval.query_body_t) = QueryBody{(yyvsp[0].select_stmt), (yyvsp[0].select_stmt), false}; }
#line 5270 "bison_parser.cpp"
    break;

  case 135: /* query_expression_body: query_expression_parens  */
#line 1137 "bison_parser.y"
                                                 { (yyval.query_body_t) = QueryBody{(yyvsp[0].select_stmt), (yyvsp[0].select_stmt), true}; }
#line 5276 "bison_parser.cpp"
    break;

  case 136: /* query_expression_body: query_expression_body set_operator query_primary  */
#line 1138 "bison_parser.y"
                                                   {
  if ((yyvsp[-2].query_body_t).head->withDescriptions) {
    delete (yyvsp[-2].query_body_t).head;
    delete (yyvsp[-1].set_operator_t);
    delete (yyvsp[0].select_stmt);
    yyerror(&yyloc, result, scanner, "A WITH clause in parentheses cannot be followed by a set operation.");
    YYERROR;
  }
  (yyvsp[-1].set_operator_t)->nestedSelectStatement = (yyvsp[0].select_stmt);
  addSetOperation((yyvsp[-2].query_body_t).open, (yyvsp[-1].set_operator_t));
  (yyval.query_body_t) = QueryBody{(yyvsp[-2].query_body_t).head, (yyvsp[0].select_stmt), false};
}
#line 5293 "bison_parser.cpp"
    break;

  case 137: /* query_expression_body: query_expression_body set_operator query_expression_parens  */
#line 1150 "bison_parser.y"
                                                             {
  if ((yyvsp[-2].query_body_t).head->withDescriptions) {
    delete (yyvsp[-2].query_body_t).head;
    delete (yyvsp[-1].set_operator_t);
    delete (yyvsp[0].select_stmt);
    yyerror(&yyloc, result, scanner, "A WITH clause in parentheses cannot be followed by a set operation.");
    YYERROR;
  }
  (yyvsp[-1].set_operator_t)->nestedSelectStatement = (yyvsp[0].select_stmt);
  addSetOperation((yyvsp[-2].query_body_t).open, (yyvsp[-1].set_operator_t));
  (yyval.query_body_t) = QueryBody{(yyvsp[-2].query_body_t).head, (yyvsp[-2].query_body_t).open, false};
}
#line 5310 "bison_parser.cpp"
    break;

  case 138: /* query_expression_parens: '(' query_expression ')'  */
#line 1163 "bison_parser.y"
                                                   { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 5316 "bison_parser.cpp"
    break;

  case 139: /* query_expression_parens: '(' query_expression_parens ')'  */
#line 1164 "bison_parser.y"
                                  { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 5322 "bison_parser.cpp"
    break;

  case 141: /* set_operator: set_type opt_all  */
#line 1172 "bison_parser.y"
                                {
  (yyval.set_operator_t) = (yyvsp[-1].set_operator_t);
  (yyval.set_operator_t)->isAll = (yyvsp[0].bval);
}
#line 5331 "bison_parser.cpp"
    break;

  case 142: /* set_type: UNION  */
#line 1177 "bison_parser.y"
                 {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetUnion;
}
#line 5340 "bison_parser.cpp"
    break;

  case 143: /* set_type: INTERSECT  */
#line 1181 "bison_parser.y"
            {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetIntersect;
}
#line 5349 "bison_parser.cpp"
    break;

  case 144: /* set_type: EXCEPT  */
#line 1185 "bison_parser.y"
         {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetExcept;
}
#line 5358 "bison_parser.cpp"
    break;

  case 145: /* opt_all: ALL  */
#line 1190 "bison_parser.y"
              { (yyval.bval) = true; }
#line 5364 "bison_parser.cpp"
    break;

  case 146: /* opt_all: %empty  */
#line 1191 "bison_parser.y"
              { (yyval.bval) = false; }
#line 5370 "bison_parser.cpp"
    break;

  case 147: /* opt_hierarchical_clause: START_WITH expr connect_by expr  */
#line 1195 "bison_parser.y"
                                                          { (yyval.hierarchical_t) = HierarchicalClause{(yyvsp[-2].expr), (yyvsp[0].expr)}; }
#line 5376 "bison_parser.cpp"
    break;

  case 148: /* opt_hierarchical_clause: connect_by expr START_WITH expr  */
#line 1196 "bison_parser.y"
                                  { (yyval.hierarchical_t) = HierarchicalClause{(yyvsp[0].expr), (yyvsp[-2].expr)}; }
#line 5382 "bison_parser.cpp"
    break;

  case 149: /* opt_hierarchical_clause: START_WITH expr  */
#line 1197 "bison_parser.y"
                  { (yyval.hierarchical_t) = HierarchicalClause{(yyvsp[0].expr), nullptr}; }
#line 5388 "bison_parser.cpp"
    break;

  case 150: /* opt_hierarchical_clause: connect_by expr  */
#line 1198 "bison_parser.y"
                  { (yyval.hierarchical_t) = HierarchicalClause{nullptr, (yyvsp[0].expr)}; }
#line 5394 "bison_parser.cpp"
    break;

  case 151: /* opt_hierarchical_clause: %empty  */
#line 1199 "bison_parser.y"
              { (yyval.hierarchical_t) = HierarchicalClause{nullptr, nullptr}; }
#line 5400 "bison_parser.cpp"
    break;

  case 154: /* query_primary: SELECT opt_top opt_distinct select_list opt_from_clause opt_where opt_hierarchical_clause opt_group  */
#line 1205 "bison_parser.y"
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
#line 5416 "bison_parser.cpp"
    break;

  case 155: /* opt_distinct: DISTINCT  */
#line 1217 "bison_parser.y"
                        { (yyval.bval) = true; }
#line 5422 "bison_parser.cpp"
    break;

  case 156: /* opt_distinct: %empty  */
#line 1218 "bison_parser.y"
              { (yyval.bval) = false; }
#line 5428 "bison_parser.cpp"
    break;

  case 158: /* opt_from_clause: from_clause  */
#line 1222 "bison_parser.y"
                              { (yyval.table) = (yyvsp[0].table); }
#line 5434 "bison_parser.cpp"
    break;

  case 159: /* opt_from_clause: %empty  */
#line 1223 "bison_parser.y"
              { (yyval.table) = nullptr; }
#line 5440 "bison_parser.cpp"
    break;

  case 160: /* from_clause: FROM table_ref  */
#line 1225 "bison_parser.y"
                             { (yyval.table) = (yyvsp[0].table); }
#line 5446 "bison_parser.cpp"
    break;

  case 161: /* opt_where: WHERE expr  */
#line 1227 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 5452 "bison_parser.cpp"
    break;

  case 162: /* opt_where: %empty  */
#line 1228 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 5458 "bison_parser.cpp"
    break;

  case 163: /* opt_group: GROUP BY expr_list opt_having  */
#line 1230 "bison_parser.y"
                                          {
  (yyval.group_t) = new GroupByDescription();
  (yyval.group_t)->columns = (yyvsp[-1].expr_vec);
  (yyval.group_t)->having = (yyvsp[0].expr);
}
#line 5468 "bison_parser.cpp"
    break;

  case 164: /* opt_group: %empty  */
#line 1235 "bison_parser.y"
              { (yyval.group_t) = nullptr; }
#line 5474 "bison_parser.cpp"
    break;

  case 165: /* opt_having: HAVING expr  */
#line 1237 "bison_parser.y"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 5480 "bison_parser.cpp"
    break;

  case 166: /* opt_having: %empty  */
#line 1238 "bison_parser.y"
              { (yyval.expr) = nullptr; }
#line 5486 "bison_parser.cpp"
    break;

  case 167: /* opt_within_group: WITHIN_GROUP '(' ORDER BY order_list ')'  */
#line 1243 "bison_parser.y"
                                                            { (yyval.order_vec) = (yyvsp[-1].order_vec); }
#line 5492 "bison_parser.cpp"
    break;

  case 168: /* opt_within_group: %empty  */
#line 1244 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 5498 "bison_parser.cpp"
    break;

  case 169: /* opt_order: ORDER BY order_list  */
#line 1246 "bison_parser.y"
                                { (yyval.order_vec) = (yyvsp[0].order_vec); }
#line 5504 "bison_parser.cpp"
    break;

  case 170: /* opt_order: %empty  */
#line 1247 "bison_parser.y"
              { (yyval.order_vec) = nullptr; }
#line 5510 "bison_parser.cpp"
    break;

  case 171: /* order_list: order_desc  */
#line 1249 "bison_parser.y"
                        {
  (yyval.order_vec) = new std::vector<OrderDescription*>();
  (yyval.order_vec)->push_back((yyvsp[0].order));
}
#line 5519 "bison_parser.cpp"
    break;

  case 172: /* order_list: order_list ',' order_desc  */
#line 1253 "bison_parser.y"
                            {
  (yyvsp[-2].order_vec)->push_back((yyvsp[0].order));
  (yyval.order_vec) = (yyvsp[-2].order_vec);
}
#line 5528 "bison_parser.cpp"
    break;

  case 173: /* order_desc: expr opt_order_type opt_null_ordering  */
#line 1258 "bison_parser.y"
                                                   { (yyval.order) = new OrderDescription((yyvsp[-1].order_type), (yyvsp[-2].expr), (yyvsp[0].null_ordering_t)); }
#line 5534 "bison_parser.cpp"
    break;

  case 174: /* opt_order_type: ASC  */
#line 1260 "bison_parser.y"
                     { (yyval.order_type) = kOrderAsc; }
#line 5540 "bison_parser.cpp"
    break;

  case 175: /* opt_order_type: DESC  */
#line 1261 "bison_parser.y"
       { (yyval.order_type) = kOrderDesc; }
#line 5546 "bison_parser.cpp"
    break;

  case 176: /* opt_order_type: %empty  */
#line 1262 "bison_parser.y"
              { (yyval.order_type) = kOrderAsc; }
#line 5552 "bison_parser.cpp"
    break;

  case 177: /* opt_null_ordering: %empty  */
#line 1264 "bison_parser.y"
                                { (yyval.null_ordering_t) = NullOrdering::Undefined; }
#line 5558 "bison_parser.cpp"
    break;

  case 178: /* opt_null_ordering: IDENTIFIER IDENTIFIER  */
#line 1265 "bison_parser.y"
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
#line 5582 "bison_parser.cpp"
    break;

  case 179: /* opt_top: TOP int_literal  */
#line 1289 "bison_parser.y"
                          { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 5588 "bison_parser.cpp"
    break;

  case 180: /* opt_top: TOP '(' expr ')'  */
#line 1290 "bison_parser.y"
                   { (yyval.limit) = new LimitDescription((yyvsp[-1].expr), nullptr); }
#line 5594 "bison_parser.cpp"
    break;

  case 181: /* opt_top: %empty  */
#line 1291 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 5600 "bison_parser.cpp"
    break;

  case 182: /* opt_limit: LIMIT expr  */
#line 1293 "bison_parser.y"
                       { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 5606 "bison_parser.cpp"
    break;

  case 183: /* opt_limit: OFFSET expr  */
#line 1294 "bison_parser.y"
              { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 5612 "bison_parser.cpp"
    break;

  case 184: /* opt_limit: LIMIT expr OFFSET expr  */
#line 1295 "bison_parser.y"
                         { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 5618 "bison_parser.cpp"
    break;

  case 185: /* opt_limit: LIMIT ALL  */
#line 1296 "bison_parser.y"
            { (yyval.limit) = new LimitDescription(nullptr, nullptr); }
#line 5624 "bison_parser.cpp"
    break;

  case 186: /* opt_limit: LIMIT ALL OFFSET expr  */
#line 1297 "bison_parser.y"
                        { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 5630 "bison_parser.cpp"
    break;

  case 187: /* opt_limit: OFFSET expr ROWS FETCH NEXT expr ROWS ONLY  */
#line 1298 "bison_parser.y"
                                             { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[-6].expr)); }
#line 5636 "bison_parser.cpp"
    break;

  case 188: /* opt_limit: %empty  */
#line 1299 "bison_parser.y"
              { (yyval.limit) = nullptr; }
#line 5642 "bison_parser.cpp"
    break;

  case 189: /* expr_list: expr_alias  */
#line 1304 "bison_parser.y"
                       {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5651 "bison_parser.cpp"
    break;

  case 190: /* expr_list: expr_list ',' expr_alias  */
#line 1308 "bison_parser.y"
                           {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5660 "bison_parser.cpp"
    break;

  case 191: /* opt_extended_literal_list: extended_literal_list  */
#line 1314 "bison_parser.y"
                                                  { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 5666 "bison_parser.cpp"
    break;

  case 192: /* opt_extended_literal_list: %empty  */
#line 1315 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 5672 "bison_parser.cpp"
    break;

  case 193: /* extended_literal_list: casted_extended_literal  */
#line 1317 "bison_parser.y"
                                                {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5681 "bison_parser.cpp"
    break;

  case 194: /* extended_literal_list: extended_literal_list ',' casted_extended_literal  */
#line 1321 "bison_parser.y"
                                                    {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5690 "bison_parser.cpp"
    break;

  case 196: /* casted_extended_literal: CAST '(' extended_literal AS column_type ')'  */
#line 1326 "bison_parser.y"
                                                                                          {
  (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t));
}
#line 5698 "bison_parser.cpp"
    break;

  case 197: /* extended_literal: literal  */
#line 1330 "bison_parser.y"
                           { (yyval.expr) = (yyvsp[0].expr); }
#line 5704 "bison_parser.cpp"
    break;

  case 198: /* extended_literal: '-' num_literal  */
#line 1331 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5710 "bison_parser.cpp"
    break;

  case 199: /* extended_literal: '-' interval_literal  */
#line 1332 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5716 "bison_parser.cpp"
    break;

  case 200: /* expr_alias: expr opt_alias  */
#line 1334 "bison_parser.y"
                            {
  (yyval.expr) = (yyvsp[-1].expr);
  if ((yyvsp[0].alias_t)) {
    (yyval.expr)->alias = (yyvsp[0].alias_t)->name;
    (yyvsp[0].alias_t)->name = nullptr;
    delete (yyvsp[0].alias_t);
  }
}
#line 5729 "bison_parser.cpp"
    break;

  case 206: /* operand: '(' expr ')'  */
#line 1345 "bison_parser.y"
                       { (yyval.expr) = (yyvsp[-1].expr); }
#line 5735 "bison_parser.cpp"
    break;

  case 217: /* operand: subquery  */
#line 1347 "bison_parser.y"
                          {
  (yyval.expr) = Expr::makeSelect((yyvsp[0].select_stmt));
}
#line 5743 "bison_parser.cpp"
    break;

  case 220: /* scalar_expr: column_name OUTERJOIN  */
#line 1355 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpUnary(kOpOuterJoin, (yyvsp[-1].expr)); }
#line 5749 "bison_parser.cpp"
    break;

  case 221: /* unary_expr: '-' operand  */
#line 1357 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 5755 "bison_parser.cpp"
    break;

  case 222: /* unary_expr: PRIOR operand  */
#line 1359 "bison_parser.y"
                { (yyval.expr) = Expr::makeOpUnary(kOpPrior, (yyvsp[0].expr)); }
#line 5761 "bison_parser.cpp"
    break;

  case 223: /* unary_expr: NOT operand  */
#line 1360 "bison_parser.y"
              { (yyval.expr) = Expr::makeOpUnary(kOpNot, (yyvsp[0].expr)); }
#line 5767 "bison_parser.cpp"
    break;

  case 224: /* unary_expr: operand ISNULL  */
#line 1361 "bison_parser.y"
                 { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-1].expr)); }
#line 5773 "bison_parser.cpp"
    break;

  case 225: /* unary_expr: operand IS NULL  */
#line 1362 "bison_parser.y"
                  { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-2].expr)); }
#line 5779 "bison_parser.cpp"
    break;

  case 226: /* unary_expr: operand IS NOT NULL  */
#line 1363 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeOpUnary(kOpIsNull, (yyvsp[-3].expr))); }
#line 5785 "bison_parser.cpp"
    break;

  case 228: /* binary_expr: operand '-' operand  */
#line 1365 "bison_parser.y"
                                              { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpMinus, (yyvsp[0].expr)); }
#line 5791 "bison_parser.cpp"
    break;

  case 229: /* binary_expr: operand '+' operand  */
#line 1366 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPlus, (yyvsp[0].expr)); }
#line 5797 "bison_parser.cpp"
    break;

  case 230: /* binary_expr: operand '/' operand  */
#line 1367 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpSlash, (yyvsp[0].expr)); }
#line 5803 "bison_parser.cpp"
    break;

  case 231: /* binary_expr: operand '*' operand  */
#line 1368 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAsterisk, (yyvsp[0].expr)); }
#line 5809 "bison_parser.cpp"
    break;

  case 232: /* binary_expr: operand '%' operand  */
#line 1369 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPercentage, (yyvsp[0].expr)); }
#line 5815 "bison_parser.cpp"
    break;

  case 233: /* binary_expr: operand '^' operand  */
#line 1370 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpCaret, (yyvsp[0].expr)); }
#line 5821 "bison_parser.cpp"
    break;

  case 234: /* binary_expr: operand LIKE operand  */
#line 1371 "bison_parser.y"
                       { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLike, (yyvsp[0].expr)); }
#line 5827 "bison_parser.cpp"
    break;

  case 235: /* binary_expr: operand NOT LIKE operand  */
#line 1372 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-3].expr), kOpNotLike, (yyvsp[0].expr)); }
#line 5833 "bison_parser.cpp"
    break;

  case 236: /* binary_expr: operand ILIKE operand  */
#line 1373 "bison_parser.y"
                        { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpILike, (yyvsp[0].expr)); }
#line 5839 "bison_parser.cpp"
    break;

  case 237: /* binary_expr: operand CONCAT operand  */
#line 1374 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpConcat, (yyvsp[0].expr)); }
#line 5845 "bison_parser.cpp"
    break;

  case 238: /* logic_expr: expr AND expr  */
#line 1376 "bison_parser.y"
                           { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAnd, (yyvsp[0].expr)); }
#line 5851 "bison_parser.cpp"
    break;

  case 239: /* logic_expr: expr OR expr  */
#line 1377 "bison_parser.y"
               { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpOr, (yyvsp[0].expr)); }
#line 5857 "bison_parser.cpp"
    break;

  case 240: /* in_expr: operand IN '(' expr_list ')'  */
#line 1379 "bison_parser.y"
                                       { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].expr_vec)); }
#line 5863 "bison_parser.cpp"
    break;

  case 241: /* in_expr: operand NOT IN '(' expr_list ')'  */
#line 1380 "bison_parser.y"
                                   { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].expr_vec))); }
#line 5869 "bison_parser.cpp"
    break;

  case 242: /* in_expr: operand IN subquery  */
#line 1381 "bison_parser.y"
                      { (yyval.expr) = Expr::makeInOperator((yyvsp[-2].expr), (yyvsp[0].select_stmt)); }
#line 5875 "bison_parser.cpp"
    break;

  case 243: /* in_expr: operand NOT IN subquery  */
#line 1382 "bison_parser.y"
                          { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-3].expr), (yyvsp[0].select_stmt))); }
#line 5881 "bison_parser.cpp"
    break;

  case 244: /* in_expr: '(' row_expr_list ')' IN subquery  */
#line 1386 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeInOperator(Expr::makeArray((yyvsp[-3].expr_vec)), (yyvsp[0].select_stmt)); }
#line 5887 "bison_parser.cpp"
    break;

  case 245: /* in_expr: '(' row_expr_list ')' NOT IN subquery  */
#line 1387 "bison_parser.y"
                                        { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator(Expr::makeArray((yyvsp[-4].expr_vec)), (yyvsp[0].select_stmt))); }
#line 5893 "bison_parser.cpp"
    break;

  case 246: /* in_expr: '(' row_expr_list ')' IN '(' in_row_list ')'  */
#line 1389 "bison_parser.y"
                                               { (yyval.expr) = Expr::makeInOperator(Expr::makeArray((yyvsp[-5].expr_vec)), (yyvsp[-1].expr_vec)); }
#line 5899 "bison_parser.cpp"
    break;

  case 247: /* in_expr: '(' row_expr_list ')' NOT IN '(' in_row_list ')'  */
#line 1390 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator(Expr::makeArray((yyvsp[-6].expr_vec)), (yyvsp[-1].expr_vec))); }
#line 5905 "bison_parser.cpp"
    break;

  case 248: /* in_row_list: '(' row_expr_list ')'  */
#line 1394 "bison_parser.y"
                                    {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
}
#line 5914 "bison_parser.cpp"
    break;

  case 249: /* in_row_list: in_row_list ',' '(' row_expr_list ')'  */
#line 1398 "bison_parser.y"
                                        {
  (yyvsp[-4].expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
  (yyval.expr_vec) = (yyvsp[-4].expr_vec);
}
#line 5923 "bison_parser.cpp"
    break;

  case 250: /* row_expr_list: expr ',' expr  */
#line 1405 "bison_parser.y"
                              {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[-2].expr));
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 5933 "bison_parser.cpp"
    break;

  case 251: /* row_expr_list: row_expr_list ',' expr  */
#line 1410 "bison_parser.y"
                         {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 5942 "bison_parser.cpp"
    break;

  case 252: /* case_expr: CASE expr case_list END  */
#line 1417 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-2].expr), (yyvsp[-1].expr), nullptr); }
#line 5948 "bison_parser.cpp"
    break;

  case 253: /* case_expr: CASE expr case_list ELSE expr END  */
#line 1418 "bison_parser.y"
                                    { (yyval.expr) = Expr::makeCase((yyvsp[-4].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5954 "bison_parser.cpp"
    break;

  case 254: /* case_expr: CASE case_list END  */
#line 1419 "bison_parser.y"
                     { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-1].expr), nullptr); }
#line 5960 "bison_parser.cpp"
    break;

  case 255: /* case_expr: CASE case_list ELSE expr END  */
#line 1420 "bison_parser.y"
                               { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 5966 "bison_parser.cpp"
    break;

  case 256: /* case_list: WHEN expr THEN expr  */
#line 1422 "bison_parser.y"
                                { (yyval.expr) = Expr::makeCaseList(Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5972 "bison_parser.cpp"
    break;

  case 257: /* case_list: case_list WHEN expr THEN expr  */
#line 1423 "bison_parser.y"
                                { (yyval.expr) = Expr::caseListAppend((yyvsp[-4].expr), Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 5978 "bison_parser.cpp"
    break;

  case 258: /* exists_expr: EXISTS subquery  */
#line 1425 "bison_parser.y"
                              { (yyval.expr) = Expr::makeExists((yyvsp[0].select_stmt)); }
#line 5984 "bison_parser.cpp"
    break;

  case 259: /* exists_expr: NOT EXISTS subquery  */
#line 1426 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeExists((yyvsp[0].select_stmt))); }
#line 5990 "bison_parser.cpp"
    break;

  case 260: /* comp_expr: operand '=' operand  */
#line 1428 "bison_parser.y"
                                { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 5996 "bison_parser.cpp"
    break;

  case 261: /* comp_expr: operand EQUALS operand  */
#line 1429 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 6002 "bison_parser.cpp"
    break;

  case 262: /* comp_expr: operand NOTEQUALS operand  */
#line 1430 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpNotEquals, (yyvsp[0].expr)); }
#line 6008 "bison_parser.cpp"
    break;

  case 263: /* comp_expr: operand '<' operand  */
#line 1431 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLess, (yyvsp[0].expr)); }
#line 6014 "bison_parser.cpp"
    break;

  case 264: /* comp_expr: operand '>' operand  */
#line 1432 "bison_parser.y"
                      { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreater, (yyvsp[0].expr)); }
#line 6020 "bison_parser.cpp"
    break;

  case 265: /* comp_expr: operand LESSEQ operand  */
#line 1433 "bison_parser.y"
                         { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLessEq, (yyvsp[0].expr)); }
#line 6026 "bison_parser.cpp"
    break;

  case 266: /* comp_expr: operand GREATEREQ operand  */
#line 1434 "bison_parser.y"
                            { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreaterEq, (yyvsp[0].expr)); }
#line 6032 "bison_parser.cpp"
    break;

  case 267: /* table_function_expr: IDENTIFIER '(' ')'  */
#line 1438 "bison_parser.y"
                                         {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-2].sval), new std::vector<Expr*>(), false, nullptr);
}
#line 6040 "bison_parser.cpp"
    break;

  case 268: /* table_function_expr: IDENTIFIER '(' opt_distinct expr_list ')'  */
#line 1441 "bison_parser.y"
                                            {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-4].sval), (yyvsp[-1].expr_vec), (yyvsp[-2].bval), nullptr);
}
#line 6048 "bison_parser.cpp"
    break;

  case 269: /* table_function_expr: IDENTIFIER '.' name_after_qualifier '(' ')'  */
#line 1444 "bison_parser.y"
                                              {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-2].sval), (yyvsp[-4].sval), new std::vector<Expr*>(), false, nullptr);
}
#line 6056 "bison_parser.cpp"
    break;

  case 270: /* table_function_expr: IDENTIFIER '.' name_after_qualifier '(' opt_distinct expr_list ')'  */
#line 1447 "bison_parser.y"
                                                                     {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-4].sval), (yyvsp[-6].sval), (yyvsp[-1].expr_vec), (yyvsp[-2].bval), nullptr);
}
#line 6064 "bison_parser.cpp"
    break;

  case 271: /* table_function_expr: nonreserved_keyword '(' ')'  */
#line 1452 "bison_parser.y"
                              {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-2].sval), new std::vector<Expr*>(), false, nullptr);
}
#line 6072 "bison_parser.cpp"
    break;

  case 272: /* table_function_expr: nonreserved_keyword '(' opt_distinct expr_list ')'  */
#line 1455 "bison_parser.y"
                                                     {
  (yyval.expr) = Expr::makeFunctionRef((yyvsp[-4].sval), (yyvsp[-1].expr_vec), (yyvsp[-2].bval), nullptr);
}
#line 6080 "bison_parser.cpp"
    break;

  case 273: /* function_expr: table_function_expr opt_within_group opt_window  */
#line 1462 "bison_parser.y"
                                                                {
  (yyval.expr) = (yyvsp[-2].expr);
  (yyval.expr)->withinGroupOrder = (yyvsp[-1].order_vec);
  (yyval.expr)->windowDescription = (yyvsp[0].window_description);
}
#line 6090 "bison_parser.cpp"
    break;

  case 274: /* opt_window: OVER '(' opt_partition opt_order opt_frame_clause ')'  */
#line 1470 "bison_parser.y"
                                                                   { (yyval.window_description) = new WindowDescription((yyvsp[-3].expr_vec), (yyvsp[-2].order_vec), (yyvsp[-1].frame_description)); }
#line 6096 "bison_parser.cpp"
    break;

  case 275: /* opt_window: %empty  */
#line 1471 "bison_parser.y"
              { (yyval.window_description) = nullptr; }
#line 6102 "bison_parser.cpp"
    break;

  case 276: /* opt_partition: PARTITION BY expr_list  */
#line 1473 "bison_parser.y"
                                       { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 6108 "bison_parser.cpp"
    break;

  case 277: /* opt_partition: %empty  */
#line 1474 "bison_parser.y"
              { (yyval.expr_vec) = nullptr; }
#line 6114 "bison_parser.cpp"
    break;

  case 278: /* opt_frame_clause: frame_type frame_bound  */
#line 1479 "bison_parser.y"
                                          { (yyval.frame_description) = new FrameDescription{(yyvsp[-1].frame_type), (yyvsp[0].frame_bound), new FrameBound{0, kCurrentRow, false}}; }
#line 6120 "bison_parser.cpp"
    break;

  case 279: /* opt_frame_clause: frame_type BETWEEN frame_bound AND frame_bound  */
#line 1480 "bison_parser.y"
                                                 { (yyval.frame_description) = new FrameDescription{(yyvsp[-4].frame_type), (yyvsp[-2].frame_bound), (yyvsp[0].frame_bound)}; }
#line 6126 "bison_parser.cpp"
    break;

  case 280: /* opt_frame_clause: %empty  */
#line 1481 "bison_parser.y"
              {
  (yyval.frame_description) = new FrameDescription{kRange, new FrameBound{0, kPreceding, true}, new FrameBound{0, kCurrentRow, false}};
}
#line 6134 "bison_parser.cpp"
    break;

  case 281: /* frame_type: RANGE  */
#line 1485 "bison_parser.y"
                   { (yyval.frame_type) = kRange; }
#line 6140 "bison_parser.cpp"
    break;

  case 282: /* frame_type: ROWS  */
#line 1486 "bison_parser.y"
       { (yyval.frame_type) = kRows; }
#line 6146 "bison_parser.cpp"
    break;

  case 283: /* frame_type: GROUPS  */
#line 1487 "bison_parser.y"
         { (yyval.frame_type) = kGroups; }
#line 6152 "bison_parser.cpp"
    break;

  case 284: /* frame_bound: UNBOUNDED PRECEDING  */
#line 1489 "bison_parser.y"
                                  { (yyval.frame_bound) = new FrameBound{0, kPreceding, true}; }
#line 6158 "bison_parser.cpp"
    break;

  case 285: /* frame_bound: INTVAL PRECEDING  */
#line 1490 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kPreceding, false}; }
#line 6164 "bison_parser.cpp"
    break;

  case 286: /* frame_bound: UNBOUNDED FOLLOWING  */
#line 1491 "bison_parser.y"
                      { (yyval.frame_bound) = new FrameBound{0, kFollowing, true}; }
#line 6170 "bison_parser.cpp"
    break;

  case 287: /* frame_bound: INTVAL FOLLOWING  */
#line 1492 "bison_parser.y"
                   { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kFollowing, false}; }
#line 6176 "bison_parser.cpp"
    break;

  case 288: /* frame_bound: CURRENT_ROW  */
#line 1493 "bison_parser.y"
              { (yyval.frame_bound) = new FrameBound{0, kCurrentRow, false}; }
#line 6182 "bison_parser.cpp"
    break;

  case 289: /* extract_expr: EXTRACT '(' datetime_field FROM expr ')'  */
#line 1495 "bison_parser.y"
                                                        { (yyval.expr) = Expr::makeExtract((yyvsp[-3].datetime_field), (yyvsp[-1].expr)); }
#line 6188 "bison_parser.cpp"
    break;

  case 290: /* next_value_expr: NEXT_VALUE_FOR table_name  */
#line 1500 "bison_parser.y"
                                            {
  auto args = new std::vector<Expr*>();
  args->push_back(Expr::makeColumnRef(qualifiedName((yyvsp[0].table_name))));
  (yyval.expr) = Expr::makeFunctionRef(strdup("NEXT VALUE FOR"), args, false, nullptr);
}
#line 6198 "bison_parser.cpp"
    break;

  case 291: /* cast_expr: CAST '(' expr AS column_type ')'  */
#line 1506 "bison_parser.y"
                                             { (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t)); }
#line 6204 "bison_parser.cpp"
    break;

  case 292: /* datetime_field: SECOND  */
#line 1508 "bison_parser.y"
                        { (yyval.datetime_field) = kDatetimeSecond; }
#line 6210 "bison_parser.cpp"
    break;

  case 293: /* datetime_field: MINUTE  */
#line 1509 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMinute; }
#line 6216 "bison_parser.cpp"
    break;

  case 294: /* datetime_field: HOUR  */
#line 1510 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeHour; }
#line 6222 "bison_parser.cpp"
    break;

  case 295: /* datetime_field: DAY  */
#line 1511 "bison_parser.y"
      { (yyval.datetime_field) = kDatetimeDay; }
#line 6228 "bison_parser.cpp"
    break;

  case 296: /* datetime_field: MONTH  */
#line 1512 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeMonth; }
#line 6234 "bison_parser.cpp"
    break;

  case 297: /* datetime_field: YEAR  */
#line 1513 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeYear; }
#line 6240 "bison_parser.cpp"
    break;

  case 298: /* datetime_field_plural: SECONDS  */
#line 1515 "bison_parser.y"
                                { (yyval.datetime_field) = kDatetimeSecond; }
#line 6246 "bison_parser.cpp"
    break;

  case 299: /* datetime_field_plural: MINUTES  */
#line 1516 "bison_parser.y"
          { (yyval.datetime_field) = kDatetimeMinute; }
#line 6252 "bison_parser.cpp"
    break;

  case 300: /* datetime_field_plural: HOURS  */
#line 1517 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeHour; }
#line 6258 "bison_parser.cpp"
    break;

  case 301: /* datetime_field_plural: DAYS  */
#line 1518 "bison_parser.y"
       { (yyval.datetime_field) = kDatetimeDay; }
#line 6264 "bison_parser.cpp"
    break;

  case 302: /* datetime_field_plural: MONTHS  */
#line 1519 "bison_parser.y"
         { (yyval.datetime_field) = kDatetimeMonth; }
#line 6270 "bison_parser.cpp"
    break;

  case 303: /* datetime_field_plural: YEARS  */
#line 1520 "bison_parser.y"
        { (yyval.datetime_field) = kDatetimeYear; }
#line 6276 "bison_parser.cpp"
    break;

  case 306: /* interval_range: datetime_field TO datetime_field  */
#line 1528 "bison_parser.y"
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
#line 6303 "bison_parser.cpp"
    break;

  case 307: /* array_expr: ARRAY '[' expr_list ']'  */
#line 1551 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeArray((yyvsp[-1].expr_vec)); }
#line 6309 "bison_parser.cpp"
    break;

  case 308: /* array_index: operand '[' int_literal ']'  */
#line 1557 "bison_parser.y"
                                          {
  (yyval.expr) = Expr::makeArrayIndex((yyvsp[-3].expr), (yyvsp[-1].expr)->ival);
  delete (yyvsp[-1].expr);
}
#line 6318 "bison_parser.cpp"
    break;

  case 309: /* between_expr: operand BETWEEN operand AND operand  */
#line 1562 "bison_parser.y"
                                                   { (yyval.expr) = Expr::makeBetween((yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 6324 "bison_parser.cpp"
    break;

  case 310: /* column_name: IDENTIFIER  */
#line 1564 "bison_parser.y"
                         { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 6330 "bison_parser.cpp"
    break;

  case 311: /* column_name: IDENTIFIER '.' name_after_qualifier  */
#line 1565 "bison_parser.y"
                                      { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 6336 "bison_parser.cpp"
    break;

  case 312: /* column_name: '*'  */
#line 1566 "bison_parser.y"
      { (yyval.expr) = Expr::makeStar(); }
#line 6342 "bison_parser.cpp"
    break;

  case 313: /* column_name: IDENTIFIER '.' '*'  */
#line 1567 "bison_parser.y"
                     { (yyval.expr) = Expr::makeStar((yyvsp[-2].sval)); }
#line 6348 "bison_parser.cpp"
    break;

  case 314: /* column_name: nonreserved_keyword  */
#line 1568 "bison_parser.y"
                      { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 6354 "bison_parser.cpp"
    break;

  case 315: /* column_name: IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier  */
#line 1577 "bison_parser.y"
                                                               { (yyval.expr) = Expr::makeColumnRef((yyvsp[-4].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 6360 "bison_parser.cpp"
    break;

  case 316: /* column_name: IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier '.' name_after_qualifier  */
#line 1578 "bison_parser.y"
                                                                                        {
  (yyval.expr) = Expr::makeColumnRef(dotJoin((yyvsp[-6].sval), (yyvsp[-4].sval)), (yyvsp[-2].sval), (yyvsp[0].sval));
}
#line 6368 "bison_parser.cpp"
    break;

  case 317: /* column_name: IDENTIFIER '.' name_after_qualifier '.' '*'  */
#line 1581 "bison_parser.y"
                                              {
  (yyval.expr) = Expr::makeStar((yyvsp[-2].sval));
  (yyval.expr)->schema = (yyvsp[-4].sval);
}
#line 6377 "bison_parser.cpp"
    break;

  case 318: /* column_name: IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier '.' '*'  */
#line 1585 "bison_parser.y"
                                                                       {
  (yyval.expr) = Expr::makeStar((yyvsp[-2].sval));
  (yyval.expr)->schema = dotJoin((yyvsp[-6].sval), (yyvsp[-4].sval));
}
#line 6386 "bison_parser.cpp"
    break;

  case 319: /* column_name: IDENTIFIER '.' nonreserved_keyword  */
#line 1591 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 6392 "bison_parser.cpp"
    break;

  case 320: /* column_name: IDENTIFIER '.' name_after_qualifier '.' nonreserved_keyword  */
#line 1592 "bison_parser.y"
                                                              { (yyval.expr) = Expr::makeColumnRef((yyvsp[-4].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 6398 "bison_parser.cpp"
    break;

  case 321: /* column_name: IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier '.' nonreserved_keyword  */
#line 1593 "bison_parser.y"
                                                                                       {
  (yyval.expr) = Expr::makeColumnRef(dotJoin((yyvsp[-6].sval), (yyvsp[-4].sval)), (yyvsp[-2].sval), (yyvsp[0].sval));
}
#line 6406 "bison_parser.cpp"
    break;

  case 322: /* nonreserved_keyword: SECOND  */
#line 1601 "bison_parser.y"
                             { (yyval.sval) = strdup("SECOND"); }
#line 6412 "bison_parser.cpp"
    break;

  case 323: /* nonreserved_keyword: MINUTE  */
#line 1602 "bison_parser.y"
         { (yyval.sval) = strdup("MINUTE"); }
#line 6418 "bison_parser.cpp"
    break;

  case 324: /* nonreserved_keyword: HOUR  */
#line 1603 "bison_parser.y"
       { (yyval.sval) = strdup("HOUR"); }
#line 6424 "bison_parser.cpp"
    break;

  case 325: /* nonreserved_keyword: DAY  */
#line 1604 "bison_parser.y"
      { (yyval.sval) = strdup("DAY"); }
#line 6430 "bison_parser.cpp"
    break;

  case 326: /* nonreserved_keyword: MONTH  */
#line 1605 "bison_parser.y"
        { (yyval.sval) = strdup("MONTH"); }
#line 6436 "bison_parser.cpp"
    break;

  case 327: /* nonreserved_keyword: YEAR  */
#line 1606 "bison_parser.y"
       { (yyval.sval) = strdup("YEAR"); }
#line 6442 "bison_parser.cpp"
    break;

  case 328: /* nonreserved_keyword: SECONDS  */
#line 1607 "bison_parser.y"
          { (yyval.sval) = strdup("SECONDS"); }
#line 6448 "bison_parser.cpp"
    break;

  case 329: /* nonreserved_keyword: MINUTES  */
#line 1608 "bison_parser.y"
          { (yyval.sval) = strdup("MINUTES"); }
#line 6454 "bison_parser.cpp"
    break;

  case 330: /* nonreserved_keyword: HOURS  */
#line 1609 "bison_parser.y"
        { (yyval.sval) = strdup("HOURS"); }
#line 6460 "bison_parser.cpp"
    break;

  case 331: /* nonreserved_keyword: DAYS  */
#line 1610 "bison_parser.y"
       { (yyval.sval) = strdup("DAYS"); }
#line 6466 "bison_parser.cpp"
    break;

  case 332: /* nonreserved_keyword: MONTHS  */
#line 1611 "bison_parser.y"
         { (yyval.sval) = strdup("MONTHS"); }
#line 6472 "bison_parser.cpp"
    break;

  case 333: /* nonreserved_keyword: YEARS  */
#line 1612 "bison_parser.y"
        { (yyval.sval) = strdup("YEARS"); }
#line 6478 "bison_parser.cpp"
    break;

  case 334: /* nonreserved_keyword: ISNULL  */
#line 1613 "bison_parser.y"
         { (yyval.sval) = strdup("ISNULL"); }
#line 6484 "bison_parser.cpp"
    break;

  case 335: /* nonreserved_keyword: FORMAT  */
#line 1614 "bison_parser.y"
         { (yyval.sval) = strdup("FORMAT"); }
#line 6490 "bison_parser.cpp"
    break;

  case 336: /* nonreserved_keyword: CHAR  */
#line 1615 "bison_parser.y"
       { (yyval.sval) = strdup("CHAR"); }
#line 6496 "bison_parser.cpp"
    break;

  case 337: /* nonreserved_keyword: VARCHAR  */
#line 1616 "bison_parser.y"
          { (yyval.sval) = strdup("VARCHAR"); }
#line 6502 "bison_parser.cpp"
    break;

  case 338: /* nonreserved_keyword: INT  */
#line 1617 "bison_parser.y"
      { (yyval.sval) = strdup("INT"); }
#line 6508 "bison_parser.cpp"
    break;

  case 339: /* nonreserved_keyword: INTEGER  */
#line 1618 "bison_parser.y"
          { (yyval.sval) = strdup("INTEGER"); }
#line 6514 "bison_parser.cpp"
    break;

  case 340: /* nonreserved_keyword: DATETIME  */
#line 1619 "bison_parser.y"
           { (yyval.sval) = strdup("DATETIME"); }
#line 6520 "bison_parser.cpp"
    break;

  case 341: /* nonreserved_keyword: TIMESTAMP  */
#line 1620 "bison_parser.y"
            { (yyval.sval) = strdup("TIMESTAMP"); }
#line 6526 "bison_parser.cpp"
    break;

  case 342: /* nonreserved_keyword: NEXT  */
#line 1621 "bison_parser.y"
       { (yyval.sval) = strdup("NEXT"); }
#line 6532 "bison_parser.cpp"
    break;

  case 344: /* name_after_qualifier: CURRENT_DATE  */
#line 1629 "bison_parser.y"
               { (yyval.sval) = strdup("CURRENT_DATE"); }
#line 6538 "bison_parser.cpp"
    break;

  case 345: /* name_after_qualifier: CURRENT_TIME  */
#line 1630 "bison_parser.y"
               { (yyval.sval) = strdup("CURRENT_TIME"); }
#line 6544 "bison_parser.cpp"
    break;

  case 346: /* name_after_qualifier: CURRENT_TIMESTAMP  */
#line 1631 "bison_parser.y"
                    { (yyval.sval) = strdup("CURRENT_TIMESTAMP"); }
#line 6550 "bison_parser.cpp"
    break;

  case 360: /* string_literal: STRING  */
#line 1644 "bison_parser.y"
                        { (yyval.expr) = Expr::makeLiteral((yyvsp[0].sval)); }
#line 6556 "bison_parser.cpp"
    break;

  case 361: /* bool_literal: TRUE  */
#line 1646 "bison_parser.y"
                    { (yyval.expr) = Expr::makeLiteral(true); }
#line 6562 "bison_parser.cpp"
    break;

  case 362: /* bool_literal: FALSE  */
#line 1647 "bison_parser.y"
        { (yyval.expr) = Expr::makeLiteral(false); }
#line 6568 "bison_parser.cpp"
    break;

  case 363: /* num_literal: FLOATVAL  */
#line 1649 "bison_parser.y"
                       { (yyval.expr) = Expr::makeLiteral((yyvsp[0].fval)); }
#line 6574 "bison_parser.cpp"
    break;

  case 365: /* int_literal: INTVAL  */
#line 1652 "bison_parser.y"
                     { (yyval.expr) = Expr::makeLiteral((yyvsp[0].ival)); }
#line 6580 "bison_parser.cpp"
    break;

  case 366: /* null_literal: NULL  */
#line 1654 "bison_parser.y"
                    { (yyval.expr) = Expr::makeNullLiteral(); }
#line 6586 "bison_parser.cpp"
    break;

  case 367: /* date_literal: DATE STRING  */
#line 1656 "bison_parser.y"
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
#line 6601 "bison_parser.cpp"
    break;

  case 368: /* date_literal: CURRENT_DATE  */
#line 1668 "bison_parser.y"
               { (yyval.expr) = Expr::makeDateLiteral(nullptr); }
#line 6607 "bison_parser.cpp"
    break;

  case 369: /* date_literal: CURRENT_DATE '(' ')'  */
#line 1669 "bison_parser.y"
                       { (yyval.expr) = Expr::makeDateLiteral(nullptr); }
#line 6613 "bison_parser.cpp"
    break;

  case 370: /* time_literal: TIME STRING  */
#line 1675 "bison_parser.y"
                           { (yyval.expr) = Expr::makeTimeLiteral((yyvsp[0].sval)); }
#line 6619 "bison_parser.cpp"
    break;

  case 371: /* time_literal: CURRENT_TIME  */
#line 1676 "bison_parser.y"
               { (yyval.expr) = Expr::makeTimeLiteral(nullptr); }
#line 6625 "bison_parser.cpp"
    break;

  case 372: /* time_literal: CURRENT_TIME '(' ')'  */
#line 1677 "bison_parser.y"
                       { (yyval.expr) = Expr::makeTimeLiteral(nullptr); }
#line 6631 "bison_parser.cpp"
    break;

  case 373: /* time_literal: CURRENT_TIME '(' INTVAL ')'  */
#line 1678 "bison_parser.y"
                              {
  if ((yyvsp[-1].ival) < 0) {
    yyerror(&yyloc, result, scanner, "The precision of CURRENT_TIME cannot be negative.");
    YYERROR;
  }
  (yyval.expr) = Expr::makeTimeLiteral(nullptr);
  (yyval.expr)->ival = (yyvsp[-1].ival);
}
#line 6644 "bison_parser.cpp"
    break;

  case 374: /* timestamp_literal: TIMESTAMP STRING  */
#line 1687 "bison_parser.y"
                                     { (yyval.expr) = Expr::makeTimestampLiteral((yyvsp[0].sval)); }
#line 6650 "bison_parser.cpp"
    break;

  case 375: /* timestamp_literal: CURRENT_TIMESTAMP  */
#line 1688 "bison_parser.y"
                    { (yyval.expr) = Expr::makeTimestampLiteral(nullptr); }
#line 6656 "bison_parser.cpp"
    break;

  case 376: /* timestamp_literal: CURRENT_TIMESTAMP '(' ')'  */
#line 1689 "bison_parser.y"
                            { (yyval.expr) = Expr::makeTimestampLiteral(nullptr); }
#line 6662 "bison_parser.cpp"
    break;

  case 377: /* timestamp_literal: CURRENT_TIMESTAMP '(' INTVAL ')'  */
#line 1690 "bison_parser.y"
                                   {
  if ((yyvsp[-1].ival) < 0) {
    yyerror(&yyloc, result, scanner, "The precision of CURRENT_TIMESTAMP cannot be negative.");
    YYERROR;
  }
  (yyval.expr) = Expr::makeTimestampLiteral(nullptr);
  (yyval.expr)->ival = (yyvsp[-1].ival);
}
#line 6675 "bison_parser.cpp"
    break;

  case 378: /* interval_literal: INTVAL duration_field  */
#line 1699 "bison_parser.y"
                                         { (yyval.expr) = Expr::makeIntervalLiteral((yyvsp[-1].ival), (yyvsp[0].datetime_field)); }
#line 6681 "bison_parser.cpp"
    break;

  case 379: /* interval_literal: INTERVAL STRING datetime_field  */
#line 1700 "bison_parser.y"
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
#line 6697 "bison_parser.cpp"
    break;

  case 380: /* interval_literal: INTERVAL STRING interval_range  */
#line 1713 "bison_parser.y"
                                 { (yyval.expr) = Expr::makeIntervalLiteral((yyvsp[-1].sval), (yyvsp[0].datetime_field)); }
#line 6703 "bison_parser.cpp"
    break;

  case 381: /* interval_literal: INTERVAL STRING  */
#line 1714 "bison_parser.y"
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
#line 6739 "bison_parser.cpp"
    break;

  case 382: /* param_expr: '?'  */
#line 1746 "bison_parser.y"
                 {
  (yyval.expr) = Expr::makeParameter(yylloc.total_column);
  (yyval.expr)->ival2 = yylloc.total_column - 1;  // source column (0-based) of the '?' token
  yyloc.param_list.push_back((yyval.expr));
}
#line 6749 "bison_parser.cpp"
    break;

  case 383: /* param_expr: DOLLAR_PARAM  */
#line 1751 "bison_parser.y"
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
#line 6766 "bison_parser.cpp"
    break;

  case 384: /* param_expr: NAMED_PARAM  */
#line 1763 "bison_parser.y"
              {
  (yyval.expr) = Expr::makeNamedParameter((yyvsp[0].sval));
  (yyval.expr)->ival2 = yylloc.total_column - 1 - (int64_t)strlen((yyvsp[0].sval));
  yyloc.param_list.push_back((yyval.expr));
}
#line 6776 "bison_parser.cpp"
    break;

  case 386: /* table_ref: table_ref_commalist ',' table_ref_atomic  */
#line 1772 "bison_parser.y"
                                                                        {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  auto tbl = new TableRef(kTableCrossProduct);
  tbl->list = (yyvsp[-2].table_vec);
  (yyval.table) = tbl;
}
#line 6787 "bison_parser.cpp"
    break;

  case 389: /* table_ref_atomic: ODBC_OJ table_ref_atomic '}'  */
#line 1783 "bison_parser.y"
                               { (yyval.table) = (yyvsp[-1].table); }
#line 6793 "bison_parser.cpp"
    break;

  case 391: /* nonjoin_table_ref_atomic: subquery opt_table_alias  */
#line 1785 "bison_parser.y"
                                                                     {
  auto tbl = new TableRef(kTableSelect);
  tbl->select = (yyvsp[-1].select_stmt);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6804 "bison_parser.cpp"
    break;

  case 392: /* nonjoin_table_ref_atomic: table_function_expr opt_table_alias  */
#line 1793 "bison_parser.y"
                                      {
  auto tbl = new TableRef(kTableFunc);
  tbl->func = (yyvsp[-1].expr);
  tbl->name = strdup((yyvsp[-1].expr)->name);
  if ((yyvsp[-1].expr)->schema) tbl->schema = strdup((yyvsp[-1].expr)->schema);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6817 "bison_parser.cpp"
    break;

  case 393: /* nonjoin_table_ref_atomic: '(' VALUES table_value_row_list ')' opt_table_alias  */
#line 1803 "bison_parser.y"
                                                      {
  auto tbl = new TableRef(kTableValues);
  tbl->values = (yyvsp[-2].expr_vec);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6828 "bison_parser.cpp"
    break;

  case 394: /* table_value_row_list: '(' expr_list ')'  */
#line 1812 "bison_parser.y"
                                         {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
}
#line 6837 "bison_parser.cpp"
    break;

  case 395: /* table_value_row_list: table_value_row_list ',' '(' expr_list ')'  */
#line 1816 "bison_parser.y"
                                             {
  (yyvsp[-4].expr_vec)->push_back(Expr::makeArray((yyvsp[-1].expr_vec)));
  (yyval.expr_vec) = (yyvsp[-4].expr_vec);
}
#line 6846 "bison_parser.cpp"
    break;

  case 396: /* table_ref_commalist: table_ref_atomic  */
#line 1821 "bison_parser.y"
                                       {
  (yyval.table_vec) = new std::vector<TableRef*>();
  (yyval.table_vec)->push_back((yyvsp[0].table));
}
#line 6855 "bison_parser.cpp"
    break;

  case 397: /* table_ref_commalist: table_ref_commalist ',' table_ref_atomic  */
#line 1825 "bison_parser.y"
                                           {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  (yyval.table_vec) = (yyvsp[-2].table_vec);
}
#line 6864 "bison_parser.cpp"
    break;

  case 398: /* table_ref_name: table_name opt_table_alias  */
#line 1830 "bison_parser.y"
                                            {
  auto tbl = new TableRef(kTableName);
  tbl->schema = (yyvsp[-1].table_name).schema;
  tbl->name = (yyvsp[-1].table_name).name;
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 6876 "bison_parser.cpp"
    break;

  case 399: /* table_ref_name_no_alias: table_name  */
#line 1838 "bison_parser.y"
                                     {
  (yyval.table) = new TableRef(kTableName);
  (yyval.table)->schema = (yyvsp[0].table_name).schema;
  (yyval.table)->name = (yyvsp[0].table_name).name;
}
#line 6886 "bison_parser.cpp"
    break;

  case 400: /* table_name: name_or_keyword  */
#line 1848 "bison_parser.y"
                             {
  (yyval.table_name).schema = nullptr;
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6895 "bison_parser.cpp"
    break;

  case 401: /* table_name: IDENTIFIER '.' qualified_name_or_keyword  */
#line 1852 "bison_parser.y"
                                           {
  (yyval.table_name).schema = (yyvsp[-2].sval);
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6904 "bison_parser.cpp"
    break;

  case 402: /* table_name: nonreserved_keyword '.' qualified_name_or_keyword  */
#line 1856 "bison_parser.y"
                                                    {
  (yyval.table_name).schema = (yyvsp[-2].sval);
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6913 "bison_parser.cpp"
    break;

  case 403: /* table_name: nonreserved_keyword '.' qualified_name_or_keyword '.' qualified_name_or_keyword  */
#line 1860 "bison_parser.y"
                                                                                  {
  (yyval.table_name).schema = dotJoin((yyvsp[-4].sval), (yyvsp[-2].sval));
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6922 "bison_parser.cpp"
    break;

  case 404: /* table_name: IDENTIFIER '.' name_after_qualifier '.' qualified_name_or_keyword  */
#line 1864 "bison_parser.y"
                                                                    {
  // Three-part (database.schema.table) name. TableName has no separate
  // database slot, so fold database+schema into schema as "database.schema" -
  // callers here only need the statement to parse, not the individual parts.
  (yyval.table_name).schema = dotJoin((yyvsp[-4].sval), (yyvsp[-2].sval));
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 6934 "bison_parser.cpp"
    break;

  case 405: /* opt_index_name: name_or_keyword  */
#line 1872 "bison_parser.y"
                                 { (yyval.sval) = (yyvsp[0].sval); }
#line 6940 "bison_parser.cpp"
    break;

  case 406: /* opt_index_name: %empty  */
#line 1873 "bison_parser.y"
              { (yyval.sval) = nullptr; }
#line 6946 "bison_parser.cpp"
    break;

  case 408: /* table_alias: AS name_or_keyword '(' ident_commalist ')'  */
#line 1875 "bison_parser.y"
                                                                 { (yyval.alias_t) = new Alias((yyvsp[-3].sval), (yyvsp[-1].str_vec)); }
#line 6952 "bison_parser.cpp"
    break;

  case 410: /* opt_table_alias: %empty  */
#line 1877 "bison_parser.y"
                                            { (yyval.alias_t) = nullptr; }
#line 6958 "bison_parser.cpp"
    break;

  case 411: /* alias: AS IDENTIFIER  */
#line 1879 "bison_parser.y"
                      { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6964 "bison_parser.cpp"
    break;

  case 412: /* alias: AS nonreserved_keyword  */
#line 1883 "bison_parser.y"
                         { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6970 "bison_parser.cpp"
    break;

  case 413: /* alias: AS STRING  */
#line 1887 "bison_parser.y"
            { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6976 "bison_parser.cpp"
    break;

  case 414: /* alias: IDENTIFIER  */
#line 1888 "bison_parser.y"
             { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 6982 "bison_parser.cpp"
    break;

  case 416: /* opt_alias: %empty  */
#line 1890 "bison_parser.y"
                                { (yyval.alias_t) = nullptr; }
#line 6988 "bison_parser.cpp"
    break;

  case 417: /* opt_locking_clause: opt_locking_clause_list  */
#line 1896 "bison_parser.y"
                                             { (yyval.locking_clause_vec) = (yyvsp[0].locking_clause_vec); }
#line 6994 "bison_parser.cpp"
    break;

  case 418: /* opt_locking_clause: %empty  */
#line 1897 "bison_parser.y"
              { (yyval.locking_clause_vec) = nullptr; }
#line 7000 "bison_parser.cpp"
    break;

  case 419: /* opt_locking_clause_list: locking_clause  */
#line 1899 "bison_parser.y"
                                         {
  (yyval.locking_clause_vec) = new std::vector<LockingClause*>();
  (yyval.locking_clause_vec)->push_back((yyvsp[0].locking_t));
}
#line 7009 "bison_parser.cpp"
    break;

  case 420: /* opt_locking_clause_list: opt_locking_clause_list locking_clause  */
#line 1903 "bison_parser.y"
                                         {
  (yyvsp[-1].locking_clause_vec)->push_back((yyvsp[0].locking_t));
  (yyval.locking_clause_vec) = (yyvsp[-1].locking_clause_vec);
}
#line 7018 "bison_parser.cpp"
    break;

  case 421: /* locking_clause: FOR row_lock_mode opt_row_lock_policy  */
#line 1908 "bison_parser.y"
                                                       {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-1].lock_mode_t);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
  (yyval.locking_t)->tables = nullptr;
}
#line 7029 "bison_parser.cpp"
    break;

  case 422: /* locking_clause: FOR row_lock_mode OF ident_commalist opt_row_lock_policy  */
#line 1914 "bison_parser.y"
                                                           {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-3].lock_mode_t);
  (yyval.locking_t)->tables = (yyvsp[-1].str_vec);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
}
#line 7040 "bison_parser.cpp"
    break;

  case 423: /* row_lock_mode: UPDATE  */
#line 1921 "bison_parser.y"
                       { (yyval.lock_mode_t) = RowLockMode::ForUpdate; }
#line 7046 "bison_parser.cpp"
    break;

  case 424: /* row_lock_mode: NO KEY UPDATE  */
#line 1922 "bison_parser.y"
                { (yyval.lock_mode_t) = RowLockMode::ForNoKeyUpdate; }
#line 7052 "bison_parser.cpp"
    break;

  case 425: /* row_lock_mode: SHARE  */
#line 1923 "bison_parser.y"
        { (yyval.lock_mode_t) = RowLockMode::ForShare; }
#line 7058 "bison_parser.cpp"
    break;

  case 426: /* row_lock_mode: KEY SHARE  */
#line 1924 "bison_parser.y"
            { (yyval.lock_mode_t) = RowLockMode::ForKeyShare; }
#line 7064 "bison_parser.cpp"
    break;

  case 427: /* opt_row_lock_policy: SKIP LOCKED  */
#line 1926 "bison_parser.y"
                                  { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::SkipLocked; }
#line 7070 "bison_parser.cpp"
    break;

  case 428: /* opt_row_lock_policy: NOWAIT  */
#line 1927 "bison_parser.y"
         { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::NoWait; }
#line 7076 "bison_parser.cpp"
    break;

  case 429: /* opt_row_lock_policy: %empty  */
#line 1928 "bison_parser.y"
              { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::None; }
#line 7082 "bison_parser.cpp"
    break;

  case 430: /* with_clause: WITH with_description_list  */
#line 1934 "bison_parser.y"
                                         { (yyval.with_description_vec) = (yyvsp[0].with_description_vec); }
#line 7088 "bison_parser.cpp"
    break;

  case 431: /* with_description_list: with_description  */
#line 1936 "bison_parser.y"
                                         {
  (yyval.with_description_vec) = new std::vector<WithDescription*>();
  (yyval.with_description_vec)->push_back((yyvsp[0].with_description_t));
}
#line 7097 "bison_parser.cpp"
    break;

  case 432: /* with_description_list: with_description_list ',' with_description  */
#line 1940 "bison_parser.y"
                                             {
  (yyvsp[-2].with_description_vec)->push_back((yyvsp[0].with_description_t));
  (yyval.with_description_vec) = (yyvsp[-2].with_description_vec);
}
#line 7106 "bison_parser.cpp"
    break;

  case 433: /* with_description: name_or_keyword AS subquery  */
#line 1945 "bison_parser.y"
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
#line 7124 "bison_parser.cpp"
    break;

  case 434: /* join_clause: table_ref_atomic NATURAL JOIN nonjoin_table_ref_atomic  */
#line 1963 "bison_parser.y"
                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = kJoinNatural;
  (yyval.table)->join->left = (yyvsp[-3].table);
  (yyval.table)->join->right = (yyvsp[0].table);
}
#line 7136 "bison_parser.cpp"
    break;

  case 435: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic ON join_condition  */
#line 1970 "bison_parser.y"
                                                                         {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (JoinType)(yyvsp[-4].join_type);
  (yyval.table)->join->left = (yyvsp[-5].table);
  (yyval.table)->join->right = (yyvsp[-2].table);
  (yyval.table)->join->condition = (yyvsp[0].expr);
}
#line 7149 "bison_parser.cpp"
    break;

  case 436: /* join_clause: table_ref_atomic opt_join_type JOIN table_ref_atomic USING '(' ident_commalist ')'  */
#line 1978 "bison_parser.y"
                                                                                     {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (yyvsp[-6].join_type);
  (yyval.table)->join->left = (yyvsp[-7].table);
  (yyval.table)->join->right = (yyvsp[-4].table);
  (yyval.table)->join->namedColumns = (yyvsp[-1].str_vec);
}
#line 7162 "bison_parser.cpp"
    break;

  case 437: /* opt_join_type: INNER  */
#line 1987 "bison_parser.y"
                      { (yyval.join_type) = kJoinInner; }
#line 7168 "bison_parser.cpp"
    break;

  case 438: /* opt_join_type: LEFT OUTER  */
#line 1988 "bison_parser.y"
             { (yyval.join_type) = kJoinLeft; }
#line 7174 "bison_parser.cpp"
    break;

  case 439: /* opt_join_type: LEFT  */
#line 1989 "bison_parser.y"
       { (yyval.join_type) = kJoinLeft; }
#line 7180 "bison_parser.cpp"
    break;

  case 440: /* opt_join_type: RIGHT OUTER  */
#line 1990 "bison_parser.y"
              { (yyval.join_type) = kJoinRight; }
#line 7186 "bison_parser.cpp"
    break;

  case 441: /* opt_join_type: RIGHT  */
#line 1991 "bison_parser.y"
        { (yyval.join_type) = kJoinRight; }
#line 7192 "bison_parser.cpp"
    break;

  case 442: /* opt_join_type: FULL OUTER  */
#line 1992 "bison_parser.y"
             { (yyval.join_type) = kJoinFull; }
#line 7198 "bison_parser.cpp"
    break;

  case 443: /* opt_join_type: OUTER  */
#line 1993 "bison_parser.y"
        { (yyval.join_type) = kJoinFull; }
#line 7204 "bison_parser.cpp"
    break;

  case 444: /* opt_join_type: FULL  */
#line 1994 "bison_parser.y"
       { (yyval.join_type) = kJoinFull; }
#line 7210 "bison_parser.cpp"
    break;

  case 445: /* opt_join_type: CROSS  */
#line 1995 "bison_parser.y"
        { (yyval.join_type) = kJoinCross; }
#line 7216 "bison_parser.cpp"
    break;

  case 446: /* opt_join_type: %empty  */
#line 1996 "bison_parser.y"
                       { (yyval.join_type) = kJoinInner; }
#line 7222 "bison_parser.cpp"
    break;

  case 450: /* ident_commalist: name_or_keyword  */
#line 2007 "bison_parser.y"
                                  {
  (yyval.str_vec) = new std::vector<char*>();
  (yyval.str_vec)->push_back((yyvsp[0].sval));
}
#line 7231 "bison_parser.cpp"
    break;

  case 451: /* ident_commalist: ident_commalist ',' name_or_keyword  */
#line 2011 "bison_parser.y"
                                      {
  (yyvsp[-2].str_vec)->push_back((yyvsp[0].sval));
  (yyval.str_vec) = (yyvsp[-2].str_vec);
}
#line 7240 "bison_parser.cpp"
    break;


#line 7244 "bison_parser.cpp"

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

#line 2017 "bison_parser.y"


/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */

    // clang-format on
