/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         HSQL_STYPE
#define YYLTYPE         HSQL_LTYPE
/* Substitute the variable and function names.  */
#define yyparse         hsql_parse
#define yylex           hsql_lex
#define yyerror         hsql_error
#define yydebug         hsql_debug
#define yynerrs         hsql_nerrs


/* Copy the first part of user declarations.  */
#line 2 "bison_parser.y" /* yacc.c:339  */


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

#line 105 "bison_parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "bison_parser.h".  */
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
#line 39 "bison_parser.y" /* yacc.c:355  */

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

#line 166 "bison_parser.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef HSQL_TOKENTYPE
# define HSQL_TOKENTYPE
  enum hsql_tokentype
  {
    SQL_IDENTIFIER = 258,
    SQL_STRING = 259,
    SQL_FLOATVAL = 260,
    SQL_INTVAL = 261,
    SQL_DEALLOCATE = 262,
    SQL_PARAMETERS = 263,
    SQL_INTERSECT = 264,
    SQL_TEMPORARY = 265,
    SQL_TIMESTAMP = 266,
    SQL_DISTINCT = 267,
    SQL_NVARCHAR = 268,
    SQL_RESTRICT = 269,
    SQL_TRUNCATE = 270,
    SQL_ANALYZE = 271,
    SQL_BETWEEN = 272,
    SQL_CASCADE = 273,
    SQL_COLUMNS = 274,
    SQL_CONTROL = 275,
    SQL_DEFAULT = 276,
    SQL_EXECUTE = 277,
    SQL_EXPLAIN = 278,
    SQL_ENCODING = 279,
    SQL_INTEGER = 280,
    SQL_NATURAL = 281,
    SQL_PREPARE = 282,
    SQL_SCHEMAS = 283,
    SQL_CHARACTER_VARYING = 284,
    SQL_REAL = 285,
    SQL_DECIMAL = 286,
    SQL_SMALLINT = 287,
    SQL_BIGINT = 288,
    SQL_SPATIAL = 289,
    SQL_VARCHAR = 290,
    SQL_VIRTUAL = 291,
    SQL_DESCRIBE = 292,
    SQL_BEFORE = 293,
    SQL_COLUMN = 294,
    SQL_CREATE = 295,
    SQL_DELETE = 296,
    SQL_DIRECT = 297,
    SQL_DOUBLE = 298,
    SQL_ESCAPE = 299,
    SQL_EXCEPT = 300,
    SQL_EXISTS = 301,
    SQL_EXTRACT = 302,
    SQL_CAST = 303,
    SQL_FORMAT = 304,
    SQL_GLOBAL = 305,
    SQL_HAVING = 306,
    SQL_IMPORT = 307,
    SQL_INSERT = 308,
    SQL_ISNULL = 309,
    SQL_OFFSET = 310,
    SQL_RENAME = 311,
    SQL_SCHEMA = 312,
    SQL_SELECT = 313,
    SQL_SORTED = 314,
    SQL_TABLES = 315,
    SQL_UNLOAD = 316,
    SQL_UPDATE = 317,
    SQL_VALUES = 318,
    SQL_AFTER = 319,
    SQL_ALTER = 320,
    SQL_CROSS = 321,
    SQL_DELTA = 322,
    SQL_FLOAT = 323,
    SQL_GROUP = 324,
    SQL_INDEX = 325,
    SQL_INNER = 326,
    SQL_LIMIT = 327,
    SQL_LOCAL = 328,
    SQL_MERGE = 329,
    SQL_MINUS = 330,
    SQL_ORDER = 331,
    SQL_OVER = 332,
    SQL_OUTER = 333,
    SQL_RIGHT = 334,
    SQL_TABLE = 335,
    SQL_UNION = 336,
    SQL_USING = 337,
    SQL_WHERE = 338,
    SQL_CALL = 339,
    SQL_CASE = 340,
    SQL_CHAR = 341,
    SQL_COPY = 342,
    SQL_DATE = 343,
    SQL_DATETIME = 344,
    SQL_DESC = 345,
    SQL_DROP = 346,
    SQL_ELSE = 347,
    SQL_FILE = 348,
    SQL_FROM = 349,
    SQL_FULL = 350,
    SQL_HASH = 351,
    SQL_HINT = 352,
    SQL_INTO = 353,
    SQL_JOIN = 354,
    SQL_LEFT = 355,
    SQL_LIKE = 356,
    SQL_LOAD = 357,
    SQL_LONG = 358,
    SQL_NULL = 359,
    SQL_PARTITION = 360,
    SQL_PLAN = 361,
    SQL_SHOW = 362,
    SQL_TEXT = 363,
    SQL_THEN = 364,
    SQL_TIME = 365,
    SQL_VIEW = 366,
    SQL_WHEN = 367,
    SQL_WITH = 368,
    SQL_ADD = 369,
    SQL_ALL = 370,
    SQL_AND = 371,
    SQL_ASC = 372,
    SQL_END = 373,
    SQL_FOR = 374,
    SQL_INT = 375,
    SQL_NOT = 376,
    SQL_OFF = 377,
    SQL_SET = 378,
    SQL_TOP = 379,
    SQL_AS = 380,
    SQL_BY = 381,
    SQL_IF = 382,
    SQL_IN = 383,
    SQL_IS = 384,
    SQL_OF = 385,
    SQL_ON = 386,
    SQL_OR = 387,
    SQL_TO = 388,
    SQL_NO = 389,
    SQL_ARRAY = 390,
    SQL_CONCAT = 391,
    SQL_ILIKE = 392,
    SQL_SECOND = 393,
    SQL_MINUTE = 394,
    SQL_HOUR = 395,
    SQL_DAY = 396,
    SQL_MONTH = 397,
    SQL_YEAR = 398,
    SQL_SECONDS = 399,
    SQL_MINUTES = 400,
    SQL_HOURS = 401,
    SQL_DAYS = 402,
    SQL_MONTHS = 403,
    SQL_YEARS = 404,
    SQL_INTERVAL = 405,
    SQL_TRUE = 406,
    SQL_FALSE = 407,
    SQL_BOOLEAN = 408,
    SQL_TRANSACTION = 409,
    SQL_BEGIN = 410,
    SQL_COMMIT = 411,
    SQL_ROLLBACK = 412,
    SQL_NOWAIT = 413,
    SQL_SKIP = 414,
    SQL_LOCKED = 415,
    SQL_SHARE = 416,
    SQL_RANGE = 417,
    SQL_ROWS = 418,
    SQL_GROUPS = 419,
    SQL_UNBOUNDED = 420,
    SQL_FOLLOWING = 421,
    SQL_PRECEDING = 422,
    SQL_CURRENT_ROW = 423,
    SQL_UNIQUE = 424,
    SQL_PRIMARY = 425,
    SQL_FOREIGN = 426,
    SQL_KEY = 427,
    SQL_REFERENCES = 428,
    SQL_EQUALS = 429,
    SQL_NOTEQUALS = 430,
    SQL_LESS = 431,
    SQL_GREATER = 432,
    SQL_LESSEQ = 433,
    SQL_GREATEREQ = 434,
    SQL_NOTNULL = 435,
    SQL_UMINUS = 436
  };
#endif

/* Value type.  */
#if ! defined HSQL_STYPE && ! defined HSQL_STYPE_IS_DECLARED

union HSQL_STYPE
{
#line 102 "bison_parser.y" /* yacc.c:355  */

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
  hsql::ColumnConstraints* column_constraints_t;
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
  hsql::NullOrdering null_ordering_t;
  hsql::ReferencesSpecification* references_spec_t;
  hsql::SetOperation* set_operator_t;
  hsql::TableConstraint* table_constraint_t;
  hsql::TableElement* table_element_t;
  hsql::TableName table_name;
  hsql::TableRef* table;
  hsql::UpdateClause* update_t;
  hsql::WindowDescription* window_description;
  hsql::WithDescription* with_description_t;

  std::vector<char*>* str_vec;
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

  hsql::ImportExportOptions* import_export_option_t;
  std::pair<hsql::CsvOptionType, char*>* csv_option_t;

  // clang-format off

#line 435 "bison_parser.cpp" /* yacc.c:355  */
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

/* Copy the second part of user declarations.  */

#line 465 "bison_parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined HSQL_LTYPE_IS_TRIVIAL && HSQL_LTYPE_IS_TRIVIAL \
             && defined HSQL_STYPE_IS_TRIVIAL && HSQL_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
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
#define YYLAST   856

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  199
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  136
/* YYNRULES -- Number of rules.  */
#define YYNRULES  357
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  656

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   436

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   188,     2,     2,
     193,   194,   186,   184,   197,   185,   195,   187,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   196,
     177,   174,   178,   198,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   191,     2,   192,   189,     2,     2,     2,     2,     2,
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
     165,   166,   167,   168,   169,   170,   171,   172,   173,   175,
     176,   179,   180,   181,   182,   183,   190
};

#if HSQL_DEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   343,   343,   362,   368,   375,   379,   383,   384,   385,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     402,   403,   405,   409,   414,   418,   428,   429,   430,   432,
     432,   438,   444,   446,   450,   462,   468,   485,   500,   502,
     503,   504,   506,   520,   524,   534,   538,   562,   570,   583,
     590,   605,   625,   626,   631,   642,   655,   667,   674,   681,
     690,   691,   693,   697,   702,   703,   705,   713,   714,   715,
     716,   717,   718,   719,   723,   724,   725,   726,   727,   728,
     729,   730,   731,   732,   733,   735,   736,   738,   739,   740,
     742,   743,   745,   749,   753,   758,   766,   767,   768,   769,
     771,   772,   773,   775,   783,   789,   795,   801,   807,   808,
     815,   821,   823,   833,   840,   851,   858,   866,   867,   874,
     881,   885,   890,   900,   904,   908,   920,   920,   922,   923,
     932,   933,   935,   949,   961,   966,   970,   974,   979,   980,
     982,   992,   993,   995,   997,   998,  1000,  1002,  1003,  1005,
    1010,  1012,  1013,  1015,  1016,  1018,  1022,  1027,  1029,  1030,
    1031,  1033,  1034,  1056,  1057,  1059,  1060,  1061,  1062,  1063,
    1064,  1069,  1073,  1079,  1080,  1082,  1086,  1091,  1091,  1095,
    1103,  1104,  1106,  1115,  1115,  1115,  1115,  1115,  1117,  1118,
    1118,  1118,  1118,  1118,  1118,  1118,  1118,  1119,  1119,  1123,
    1123,  1125,  1126,  1127,  1128,  1129,  1131,  1131,  1132,  1133,
    1134,  1135,  1136,  1137,  1138,  1139,  1140,  1142,  1143,  1145,
    1146,  1147,  1148,  1152,  1153,  1154,  1155,  1157,  1158,  1160,
    1161,  1163,  1164,  1165,  1166,  1167,  1168,  1169,  1173,  1174,
    1175,  1176,  1180,  1181,  1183,  1184,  1189,  1190,  1191,  1195,
    1196,  1197,  1199,  1200,  1201,  1202,  1203,  1205,  1207,  1209,
    1210,  1211,  1212,  1213,  1214,  1216,  1217,  1218,  1219,  1220,
    1221,  1223,  1223,  1225,  1227,  1229,  1231,  1232,  1233,  1234,
    1236,  1236,  1236,  1236,  1236,  1236,  1236,  1238,  1240,  1241,
    1243,  1244,  1246,  1248,  1250,  1261,  1262,  1273,  1305,  1314,
    1314,  1321,  1321,  1323,  1323,  1330,  1334,  1339,  1347,  1353,
    1357,  1362,  1363,  1365,  1365,  1367,  1367,  1369,  1370,  1372,
    1372,  1378,  1379,  1381,  1385,  1390,  1396,  1403,  1404,  1405,
    1406,  1408,  1409,  1410,  1416,  1416,  1418,  1420,  1424,  1429,
    1439,  1446,  1454,  1463,  1464,  1465,  1466,  1467,  1468,  1469,
    1470,  1471,  1472,  1474,  1480,  1480,  1483,  1487
};
#endif

#if HSQL_DEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "STRING", "FLOATVAL",
  "INTVAL", "DEALLOCATE", "PARAMETERS", "INTERSECT", "TEMPORARY",
  "TIMESTAMP", "DISTINCT", "NVARCHAR", "RESTRICT", "TRUNCATE", "ANALYZE",
  "BETWEEN", "CASCADE", "COLUMNS", "CONTROL", "DEFAULT", "EXECUTE",
  "EXPLAIN", "ENCODING", "INTEGER", "NATURAL", "PREPARE", "SCHEMAS",
  "CHARACTER_VARYING", "REAL", "DECIMAL", "SMALLINT", "BIGINT", "SPATIAL",
  "VARCHAR", "VIRTUAL", "DESCRIBE", "BEFORE", "COLUMN", "CREATE", "DELETE",
  "DIRECT", "DOUBLE", "ESCAPE", "EXCEPT", "EXISTS", "EXTRACT", "CAST",
  "FORMAT", "GLOBAL", "HAVING", "IMPORT", "INSERT", "ISNULL", "OFFSET",
  "RENAME", "SCHEMA", "SELECT", "SORTED", "TABLES", "UNLOAD", "UPDATE",
  "VALUES", "AFTER", "ALTER", "CROSS", "DELTA", "FLOAT", "GROUP", "INDEX",
  "INNER", "LIMIT", "LOCAL", "MERGE", "MINUS", "ORDER", "OVER", "OUTER",
  "RIGHT", "TABLE", "UNION", "USING", "WHERE", "CALL", "CASE", "CHAR",
  "COPY", "DATE", "DATETIME", "DESC", "DROP", "ELSE", "FILE", "FROM",
  "FULL", "HASH", "HINT", "INTO", "JOIN", "LEFT", "LIKE", "LOAD", "LONG",
  "NULL", "PARTITION", "PLAN", "SHOW", "TEXT", "THEN", "TIME", "VIEW",
  "WHEN", "WITH", "ADD", "ALL", "AND", "ASC", "END", "FOR", "INT", "NOT",
  "OFF", "SET", "TOP", "AS", "BY", "IF", "IN", "IS", "OF", "ON", "OR",
  "TO", "NO", "ARRAY", "CONCAT", "ILIKE", "SECOND", "MINUTE", "HOUR",
  "DAY", "MONTH", "YEAR", "SECONDS", "MINUTES", "HOURS", "DAYS", "MONTHS",
  "YEARS", "INTERVAL", "TRUE", "FALSE", "BOOLEAN", "TRANSACTION", "BEGIN",
  "COMMIT", "ROLLBACK", "NOWAIT", "SKIP", "LOCKED", "SHARE", "RANGE",
  "ROWS", "GROUPS", "UNBOUNDED", "FOLLOWING", "PRECEDING", "CURRENT_ROW",
  "UNIQUE", "PRIMARY", "FOREIGN", "KEY", "REFERENCES", "'='", "EQUALS",
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
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,    61,   429,   430,    60,    62,   431,
     432,   433,   434,   435,    43,    45,    42,    47,    37,    94,
     436,    91,    93,    40,    41,    46,    59,    44,    63
};
# endif

#define YYPACT_NINF -529

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-529)))

#define YYTABLE_NINF -355

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-355)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     595,    27,    65,    71,    99,    65,   113,    83,   137,   140,
      65,   190,    21,   195,    37,   292,   120,   120,   120,   287,
     103,  -529,   221,  -529,   221,  -529,  -529,  -529,  -529,  -529,
    -529,  -529,  -529,  -529,  -529,  -529,  -529,   -26,  -529,   342,
     166,  -529,   183,   313,  -529,   251,   251,   251,    65,   415,
      65,   298,  -529,   297,   -26,   293,   -37,   297,   297,   297,
      65,  -529,   309,   267,  -529,  -529,  -529,  -529,  -529,  -529,
     578,  -529,   369,  -529,  -529,   350,   224,  -529,    35,  -529,
     479,   353,   486,   374,   494,    65,    65,   418,  -529,   413,
     314,   505,   464,    65,   315,   317,   508,   508,   508,   519,
      65,    65,  -529,   332,   292,  -529,   333,   521,   518,  -529,
    -529,  -529,   -26,   420,   410,   -26,    15,  -529,  -529,  -529,
     707,   344,   535,  -529,   536,  -529,  -529,    45,  -529,   349,
     347,  -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,
    -529,  -529,  -529,  -529,   500,  -529,   416,   -15,   314,   363,
    -529,   508,   547,   198,   380,   -38,  -529,  -529,   466,  -529,
    -529,  -529,   -75,   -75,   -75,  -529,  -529,  -529,  -529,  -529,
     552,  -529,  -529,  -529,   363,   483,  -529,  -529,   224,  -529,
    -529,   363,   483,   363,   151,   443,  -529,  -529,  -529,  -529,
    -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,
    -529,   200,  -529,   248,  -529,  -529,  -529,   353,  -529,    65,
     561,   454,    19,   441,   129,   375,   381,   382,   244,   367,
     396,    59,  -529,   335,    69,   395,  -529,  -529,  -529,  -529,
    -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,
    -529,  -529,   490,  -529,    38,   397,  -529,   363,   505,  -529,
     550,  -529,  -529,   398,   193,  -529,   418,  -529,   402,   180,
    -529,   502,   400,  -529,    44,    15,   -26,   401,  -529,   -31,
      15,    69,   544,    68,   106,  -529,   443,  -529,   476,  -529,
    -529,   411,   510,  -529,   668,   414,   434,   436,   205,  -529,
    -529,  -529,   454,     9,    36,   551,   248,   363,   363,   268,
     -43,   419,    59,   609,   363,   217,   417,   -23,   363,   363,
      59,  -529,    59,    24,   421,   -27,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,   521,    65,  -529,   610,   353,    69,  -529,   297,   193,
     612,   616,   415,   617,   222,  -529,  -529,   353,  -529,   552,
      22,   418,  -529,   363,  -529,   620,  -529,  -529,  -529,  -529,
     363,  -529,  -529,   621,   443,   363,   363,  -529,   455,  -529,
     465,   -70,  -529,   668,   547,   508,  -529,  -529,   432,  -529,
     435,  -529,  -529,   440,  -529,  -529,   444,  -529,  -529,  -529,
    -529,   445,  -529,  -529,   139,   547,   448,   449,  -529,    19,
    -529,   557,   363,   451,  -529,   452,   555,   -13,   201,   167,
     363,   363,  -529,   551,   528,   119,  -529,  -529,  -529,   529,
     538,   638,    59,   457,   335,  -529,   548,   460,   638,   638,
     638,   638,   653,   653,   653,   653,   217,   217,    92,    92,
      92,   -96,   461,  -529,  -529,   228,   652,   234,  -529,  -529,
    -529,  -529,  -529,   227,   239,  -529,   454,  -529,    87,  -529,
     459,  -529,    39,  -529,   589,  -529,  -529,  -529,   658,  -529,
    -529,    69,    69,   600,  -529,   547,  -529,   504,  -529,   472,
     241,  -529,   662,   664,  -529,   665,   666,   667,  -529,  -529,
     572,  -529,   506,    65,  -529,   139,  -529,  -529,   246,   547,
     547,  -529,   484,  -529,   253,    14,  -529,   363,   668,   363,
     363,  -529,   182,   211,   487,  -529,    59,   638,   335,   489,
     264,  -529,  -529,  -529,  -529,  -529,   676,   415,  -529,  -529,
     493,   585,  -529,  -529,  -529,   611,   618,   626,   591,    22,
     685,  -529,  -529,  -529,   568,  -529,  -529,  -529,   -77,  -529,
    -529,  -529,   501,   265,   511,   512,   515,  -529,  -529,   314,
    -529,  -529,  -529,   266,   271,   613,   557,   557,   363,   121,
     523,    69,   203,  -529,   363,  -529,   609,   527,   278,  -529,
    -529,  -529,  -529,    39,    22,  -529,  -529,  -529,    22,   247,
     539,   363,  -529,  -529,  -529,   722,  -529,  -529,  -529,  -529,
    -529,   558,   614,   483,  -529,  -529,   279,  -529,  -529,  -529,
      69,  -529,  -529,  -529,  -529,   463,   547,   -22,   543,  -529,
     363,   330,   557,   549,   363,   284,   363,  -529,  -529,   400,
    -529,  -529,  -529,   553,    61,  -529,   547,    69,  -529,  -529,
      69,  -529,    25,    55,   188,  -529,  -529,   286,  -529,  -529,
     625,  -529,  -529,  -529,    55,  -529
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     335,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    30,    30,    30,     0,
     355,     3,    21,    19,    21,    18,     8,     9,     7,    11,
      16,    17,    13,    14,    12,    15,    10,     0,   334,     0,
     309,   114,    33,     0,    54,    61,    61,    61,     0,     0,
       0,     0,   308,   109,     0,     0,     0,   109,   109,   109,
       0,    52,     0,   336,   337,    29,    26,    28,    27,     1,
     335,     2,     0,     6,     5,   164,   123,   124,   154,   106,
       0,   174,     0,     0,   312,     0,     0,   148,    37,     0,
     118,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     4,     0,     0,   142,   136,
     137,   135,     0,   139,     0,     0,   170,   310,   287,   290,
     292,     0,     0,   293,     0,   288,   289,     0,   298,     0,
     173,   175,   177,   179,   280,   281,   282,   291,   283,   284,
     285,   286,    32,    31,     0,   311,     0,     0,   118,     0,
     113,     0,     0,     0,     0,   148,   120,   108,     0,   131,
     130,    38,    41,    41,    41,   107,   104,   105,   339,   338,
       0,   292,   163,   141,     0,   154,   127,   126,   128,   138,
     134,     0,   154,     0,     0,   322,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     295,     0,   294,   297,   180,   181,    34,     0,    60,     0,
       0,   335,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,   278,     0,   147,   183,   190,   191,   192,   185,
     187,   193,   186,   206,   194,   195,   196,   197,   189,   184,
     199,   200,     0,   356,     0,     0,   116,     0,     0,   119,
       0,   110,   111,     0,     0,    51,   148,    50,    24,     0,
      22,   145,   143,   171,   320,   170,     0,   153,   155,   160,
     170,   166,   168,   165,     0,   132,   321,   323,     0,   296,
     176,     0,     0,    57,     0,     0,     0,     0,     0,    62,
      64,    65,   335,   142,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   202,     0,   201,     0,     0,     0,     0,
       0,   203,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   117,     0,     0,   122,   121,   109,     0,
       0,     0,     0,     0,     0,    47,    36,     0,    20,     0,
       0,   148,   144,     0,   318,     0,   319,   182,   125,   129,
       0,   159,   158,   161,   322,     0,     0,   327,     0,   329,
       0,   333,   324,     0,     0,     0,    83,    77,     0,    79,
      89,    80,    67,     0,    74,    75,     0,    71,    72,    78,
      81,    86,    76,    68,    91,     0,     0,     0,    56,     0,
      59,   243,     0,   277,   279,     0,     0,     0,     0,     0,
       0,     0,   225,     0,     0,     0,   198,   188,   217,   218,
       0,   213,     0,     0,     0,   204,     0,   216,   215,   231,
     232,   233,   234,   235,   236,   237,   208,   207,   210,   209,
     211,   212,     0,    35,   357,     0,     0,     0,    48,    45,
      43,    49,    40,     0,     0,    23,   335,   146,   299,   301,
       0,   303,   316,   302,   150,   172,   317,   156,     0,   157,
     133,   169,   167,     0,   330,     0,   332,     0,   325,     0,
       0,    55,     0,     0,    73,     0,     0,     0,    82,    98,
       0,    97,     0,     0,    66,    90,    92,    94,     0,     0,
       0,    63,     0,   238,     0,   142,   229,     0,     0,     0,
       0,   223,     0,     0,     0,   273,     0,   214,     0,     0,
       0,   205,   274,   115,   112,    39,     0,     0,    46,    25,
       0,     0,   351,   343,   349,   347,   350,   345,     0,     0,
       0,   315,   307,   313,     0,   140,   162,   328,   333,   331,
     178,    58,     0,     0,     0,     0,     0,    99,    96,   118,
      93,    95,   101,     0,     0,   245,   243,   243,     0,     0,
       0,   227,     0,   226,     0,   230,   275,     0,     0,   221,
     219,    44,    42,   316,     0,   346,   348,   344,     0,   300,
     317,     0,   326,    70,    88,     0,    84,    69,    85,   103,
     100,     0,     0,   154,   239,   240,     0,   257,   258,   224,
     228,   222,   220,   304,   340,   352,     0,   152,     0,   102,
       0,   248,   243,     0,     0,     0,     0,   149,    87,   244,
     249,   250,   251,     0,     0,   241,     0,   353,   341,   314,
     151,   242,     0,     0,     0,   256,   246,     0,   255,   253,
       0,   254,   252,   342,     0,   247
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -529,  -529,  -529,   673,  -529,   720,  -529,   399,  -529,   428,
    -529,  -529,  -529,  -529,  -325,   -79,   290,   422,   296,  -529,
    -529,  -529,   453,  -529,   354,  -529,  -338,  -529,  -529,  -529,
    -529,   260,  -529,  -429,  -529,   -45,  -529,  -529,  -529,  -529,
    -529,  -529,  -142,  -529,  -529,   514,  -202,   -87,  -529,   204,
     -49,    -5,  -529,  -529,   -85,  -278,  -529,  -529,  -529,  -139,
    -529,  -529,  -171,  -529,   403,  -529,  -529,  -529,   127,  -297,
    -529,   -67,   559,   563,   405,  -147,  -188,  -529,  -529,  -529,
    -529,  -529,  -529,   469,  -529,  -529,  -529,  -514,  -529,  -529,
    -529,  -528,  -529,  -529,  -148,  -529,  -529,  -529,  -529,  -529,
    -529,   -58,  -529,  -529,   632,  -106,  -529,  -529,   633,  -529,
    -529,  -496,   181,  -529,  -529,  -529,    -2,  -529,  -529,   186,
     509,  -529,   406,  -529,   496,  -529,   229,  -529,  -529,  -529,
     675,  -529,  -529,  -529,  -529,  -354
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    19,    20,    21,    22,    73,   259,   260,    23,    66,
      24,   143,    25,    26,    89,   162,   255,   344,   345,    27,
      28,    29,    84,   288,   289,   290,   394,   488,   484,   494,
     495,   496,   291,   497,    30,    93,    31,   251,   252,    32,
      33,    34,   153,    35,   155,   156,    36,   175,   176,   177,
      77,   112,   113,   180,    78,   174,   261,   351,   352,   150,
     545,   627,   116,   267,   268,   363,   469,   108,   185,   262,
     129,   130,   131,   132,   263,   264,   225,   226,   227,   228,
     229,   230,   231,   300,   232,   233,   234,   503,   603,   633,
     634,   646,   235,   236,   198,   199,   200,   237,   238,   239,
     240,   241,   134,   135,   136,   137,   138,   139,   140,   141,
     457,   458,   459,   460,   461,    51,   462,   146,   541,   542,
     543,   357,   275,   276,   277,   371,   478,    37,    38,    63,
      64,   463,   538,   638,    71,   244
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      41,   172,   224,    44,   265,    95,   213,   415,    52,   283,
      56,   270,    99,   100,   101,   402,   249,   450,   163,   164,
     480,   173,   284,   133,    40,    40,   173,   178,   182,   626,
     178,   303,    75,   305,   269,   479,   271,   273,   253,   403,
     316,   498,   354,   589,   109,   149,    87,   354,    90,   410,
     119,   120,   604,   605,    39,   279,    60,    97,   102,   361,
     475,   642,   214,   118,   119,   120,   561,   642,    40,   411,
     183,   299,   242,   115,    42,   412,   307,   425,   643,   210,
     110,   476,   477,   147,   148,   308,   362,   184,   476,   477,
     400,   158,   615,   308,   426,   331,    98,    61,   166,   167,
     336,   309,    43,   308,   246,   504,   216,   217,   635,   309,
     211,   114,   508,   531,   303,   650,   111,   346,   254,   309,
     334,   548,   420,   366,   421,   422,   655,   520,   427,   428,
     429,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   133,   218,   563,   564,   122,   406,   133,
     407,   408,   423,   532,   214,   118,   119,   120,   533,   248,
     308,   418,   419,   123,   540,   534,   535,    54,   367,   355,
     570,   417,   619,   266,   306,   353,   309,    48,   212,   359,
     302,   178,   536,    45,   308,   308,  -352,   537,   285,   286,
     287,   648,   649,    46,   220,   124,   340,   215,   216,   217,
     309,   309,   582,   401,   118,   119,   120,   281,   567,   124,
     125,   126,   464,   269,    54,   456,    55,   341,   471,   472,
     644,   578,   404,   645,    47,   442,   644,   568,   316,   645,
     340,    49,   333,   109,   517,   334,   218,   308,    50,   122,
     368,    76,   342,   489,   221,   222,   405,   214,   118,   119,
     120,   526,   223,   309,   530,   123,    75,   128,    94,   510,
     490,   245,   625,   512,   513,    57,   272,   369,   445,   110,
      53,   606,   219,   531,    65,    58,   527,   133,   370,   411,
     454,   330,   647,   331,  -305,   511,   220,    69,   122,   133,
     215,   216,   217,   446,   617,    62,   481,   343,   308,    70,
     573,   124,   125,   126,   123,   111,    59,   168,   491,   492,
     509,   515,   493,   532,   309,   607,   353,   308,   533,   308,
     574,   609,   293,   629,   294,   534,   535,   308,   576,   218,
     443,   343,   122,   309,    72,   309,   221,   222,   214,   118,
     119,   120,   536,   309,   223,    79,  -352,   537,   123,   128,
     124,   125,   126,   316,   651,   652,   298,   118,   119,   120,
     569,    80,   571,   572,   514,   219,   214,   118,   119,   120,
     214,   118,   119,   120,   348,   519,    81,   349,    83,   220,
     298,   215,   216,   217,   308,   127,   186,   187,   188,   189,
     190,   191,   358,    75,   124,   125,   126,   364,   128,   398,
     309,   121,   399,   327,   328,   329,   330,    82,   331,   215,
     216,   217,   310,   301,   216,   217,   452,   599,    88,   453,
     218,    91,   523,   122,    92,   207,    96,   610,   525,   221,
     222,   453,   621,   529,   103,   551,   207,   223,   334,   123,
     562,   122,   128,   334,  -306,    67,    68,   566,   218,   311,
     353,   122,   218,   256,   257,   122,   219,   123,   580,   594,
     600,   353,   595,   334,   104,   601,   106,   123,   334,   577,
     220,   123,   612,   622,   107,   353,   353,   637,   639,   640,
     653,   334,   117,   334,   219,   124,   125,   126,   302,   531,
     142,   559,   630,   631,   632,   144,   312,   145,   220,    85,
      86,   149,   220,   124,   125,   126,   151,   152,   154,   159,
     157,   160,   161,   124,   125,   126,   313,   124,   125,   126,
     221,   222,   165,   314,   315,    54,   170,   171,   223,   532,
     173,   316,   317,   128,   533,   179,   181,   201,   127,   202,
     203,   534,   535,   206,   207,   623,   208,   209,   221,   222,
     243,   128,   221,   222,   247,   258,   223,   250,   536,   114,
     223,   128,   274,   537,   282,   128,   292,    15,   295,   318,
     319,   320,   321,   322,   296,   297,   323,   324,  -354,   325,
     326,   327,   328,   329,   330,     1,   331,   304,   332,   338,
     335,   339,   311,     2,   624,   347,   350,   353,   360,   365,
       3,   373,     1,   375,   374,     4,   396,   395,   397,    75,
       2,   416,   413,   444,   424,     5,   448,     3,     6,     7,
     449,   451,     4,   466,   468,   482,   474,   473,   483,   422,
       8,     9,     5,   485,   502,     6,     7,   486,   487,   312,
      10,   499,   500,    11,   505,   308,   506,     8,     9,   507,
     518,   331,   521,   522,   516,   524,   539,    10,   544,   414,
      11,   546,   547,   311,   549,    12,   550,   315,   552,    13,
     553,   554,   555,   556,   316,   317,   557,   565,   558,   376,
     581,   575,    12,   579,   584,    14,    13,   583,   590,   585,
     588,    15,   311,   377,   591,   593,   586,   378,   379,   380,
     381,   382,    14,   383,   587,   596,   597,   311,    15,   598,
     312,   384,   318,   319,   320,   321,   322,   608,   602,   323,
     324,   611,   325,   326,   327,   328,   329,   330,   618,   331,
     414,   493,   616,    16,    17,    18,   385,   628,   315,  -355,
     620,   654,   636,   105,    74,   316,   317,   641,   455,   528,
      16,    17,    18,   501,   386,   560,   387,   388,   465,   204,
     205,   447,   337,   467,   278,   614,   280,   315,   409,   613,
     470,   389,   372,   356,   316,  -355,   390,   592,   391,   169,
       0,     0,   315,   318,   319,   320,   321,   322,   392,   316,
     323,   324,     0,   325,   326,   327,   328,   329,   330,     0,
     331,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -355,  -355,  -355,   321,   322,     0,     0,   323,
     324,   393,   325,   326,   327,   328,   329,   330,     0,   331,
    -355,  -355,     0,     0,  -355,  -355,     0,   325,   326,   327,
     328,   329,   330,     0,   331,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197
};

static const yytype_int16 yycheck[] =
{
       2,   107,   149,     5,   175,    54,   148,   304,    10,   211,
      12,   182,    57,    58,    59,   293,   155,   342,    97,    98,
     374,    12,     3,    81,     3,     3,    12,   112,   115,    51,
     115,   219,    58,   221,   181,   373,   183,   184,   113,     3,
     136,   395,     3,   539,     9,    83,    48,     3,    50,    92,
       5,     6,   566,   567,    27,   203,    19,    94,    60,    90,
     130,     6,     3,     4,     5,     6,   495,     6,     3,   112,
      55,   218,   151,    78,     3,   118,   223,   104,    17,    94,
      45,   158,   159,    85,    86,   116,   117,    72,   158,   159,
     292,    93,   588,   116,   121,   191,   133,    60,   100,   101,
     247,   132,     3,   116,   153,   402,    47,    48,   622,   132,
     125,    76,   125,    26,   302,   643,    81,   256,   193,   132,
     197,   475,   310,    55,   312,   101,   654,   424,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   201,    85,   499,   500,    88,   296,   207,
     297,   298,   128,    66,     3,     4,     5,     6,    71,   197,
     116,   308,   309,   104,   125,    78,    79,   193,    62,   125,
     508,   194,   601,   178,   223,   197,   132,    94,   193,   266,
     121,   266,    95,    70,   116,   116,    99,   100,   169,   170,
     171,   166,   167,    80,   135,   150,     3,    46,    47,    48,
     132,   132,   527,   194,     4,     5,     6,   209,   194,   150,
     151,   152,   351,   360,   193,   193,    12,    24,   365,   366,
     165,   518,   186,   168,   111,   331,   165,   505,   136,   168,
       3,    94,   194,     9,   422,   197,    85,   116,    98,    88,
     134,    37,    49,   104,   185,   186,   295,     3,     4,     5,
       6,    24,   193,   132,   456,   104,    58,   198,    54,    92,
     121,    63,   616,   410,   411,    70,   115,   161,   335,    45,
      80,   568,   121,    26,   154,    80,    49,   335,   172,   112,
     347,   189,   636,   191,   197,   118,   135,     0,    88,   347,
      46,    47,    48,   338,   591,     3,   375,   104,   116,   196,
     118,   150,   151,   152,   104,    81,   111,   103,   169,   170,
     109,   192,   173,    66,   132,   194,   197,   116,    71,   116,
     109,   118,   193,   620,   195,    78,    79,   116,   516,    85,
     332,   104,    88,   132,   113,   132,   185,   186,     3,     4,
       5,     6,    95,   132,   193,     3,    99,   100,   104,   198,
     150,   151,   152,   136,   166,   167,   112,     4,     5,     6,
     507,   195,   509,   510,   413,   121,     3,     4,     5,     6,
       3,     4,     5,     6,   194,   424,   193,   197,   127,   135,
     112,    46,    47,    48,   116,   185,   138,   139,   140,   141,
     142,   143,   265,    58,   150,   151,   152,   270,   198,   194,
     132,    48,   197,   186,   187,   188,   189,    94,   191,    46,
      47,    48,    17,    46,    47,    48,   194,   559,     3,   197,
      85,   123,   194,    88,   127,   197,   133,   574,   194,   185,
     186,   197,   603,   194,   125,   194,   197,   193,   197,   104,
     194,    88,   198,   197,   197,    17,    18,   194,    85,    54,
     197,    88,    85,   163,   164,    88,   121,   104,   194,   194,
     194,   197,   197,   197,   197,   194,    97,   104,   197,   518,
     135,   104,   194,   194,   124,   197,   197,   624,   194,   626,
     194,   197,     3,   197,   121,   150,   151,   152,   121,    26,
       4,   493,   162,   163,   164,   121,   101,     3,   135,    46,
      47,    83,   135,   150,   151,   152,    93,   193,     3,   194,
      46,   194,     4,   150,   151,   152,   121,   150,   151,   152,
     185,   186,     3,   128,   129,   193,   193,     6,   193,    66,
      12,   136,   137,   198,    71,   115,   126,   193,   185,     4,
       4,    78,    79,   194,   197,    82,    46,   131,   185,   186,
       3,   198,   185,   186,   174,     3,   193,    91,    95,    76,
     193,   198,   119,   100,     3,   198,   125,   113,   193,   174,
     175,   176,   177,   178,   193,   193,   181,   182,     0,   184,
     185,   186,   187,   188,   189,     7,   191,   191,    98,    39,
     193,   193,    54,    15,   131,   193,    94,   197,   197,    55,
      22,   125,     7,    93,   193,    27,   172,   193,   172,    58,
      15,   194,   193,     3,   193,    37,     4,    22,    40,    41,
       4,     4,    27,     3,     3,   193,   161,   172,   193,   101,
      52,    53,    37,   193,    77,    40,    41,   193,   193,   101,
      62,   193,   193,    65,   193,   116,   194,    52,    53,    94,
     193,   191,   104,   192,   116,     3,   197,    62,    69,   121,
      65,     3,    62,    54,   160,    87,   194,   129,     6,    91,
       6,     6,     6,     6,   136,   137,   104,   193,   172,    11,
       4,   194,    87,   194,    99,   107,    91,   194,     3,    78,
      99,   113,    54,    25,   126,   194,    78,    29,    30,    31,
      32,    33,   107,    35,    78,   194,   194,    54,   113,   194,
     101,    43,   174,   175,   176,   177,   178,   194,   105,   181,
     182,   194,   184,   185,   186,   187,   188,   189,     6,   191,
     121,   173,   193,   155,   156,   157,    68,   194,   129,   101,
     126,   116,   193,    70,    24,   136,   137,   194,   349,   453,
     155,   156,   157,   399,    86,   495,    88,    89,   353,   127,
     127,   339,   248,   360,   201,   584,   207,   129,   299,   583,
     364,   103,   276,   264,   136,   137,   108,   548,   110,   104,
      -1,    -1,   129,   174,   175,   176,   177,   178,   120,   136,
     181,   182,    -1,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,    -1,    -1,   181,
     182,   153,   184,   185,   186,   187,   188,   189,    -1,   191,
     177,   178,    -1,    -1,   181,   182,    -1,   184,   185,   186,
     187,   188,   189,    -1,   191,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     7,    15,    22,    27,    37,    40,    41,    52,    53,
      62,    65,    87,    91,   107,   113,   155,   156,   157,   200,
     201,   202,   203,   207,   209,   211,   212,   218,   219,   220,
     233,   235,   238,   239,   240,   242,   245,   326,   327,    27,
       3,   315,     3,     3,   315,    70,    80,   111,    94,    94,
      98,   314,   315,    80,   193,   248,   315,    70,    80,   111,
      19,    60,     3,   328,   329,   154,   208,   208,   208,     0,
     196,   333,   113,   204,   204,    58,   248,   249,   253,     3,
     195,   193,    94,   127,   221,   221,   221,   315,     3,   213,
     315,   123,   127,   234,   248,   249,   133,    94,   133,   234,
     234,   234,   315,   125,   197,   202,    97,   124,   266,     9,
      45,    81,   250,   251,    76,   250,   261,     3,     4,     5,
       6,    48,    88,   104,   150,   151,   152,   185,   198,   269,
     270,   271,   272,   300,   301,   302,   303,   304,   305,   306,
     307,   308,     4,   210,   121,     3,   316,   315,   315,    83,
     258,    93,   193,   241,     3,   243,   244,    46,   315,   194,
     194,     4,   214,   214,   214,     3,   315,   315,   248,   329,
     193,     6,   304,    12,   254,   246,   247,   248,   253,   115,
     252,   126,   246,    55,    72,   267,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   293,   294,
     295,   193,     4,     4,   303,   307,   194,   197,    46,   131,
      94,   125,   193,   241,     3,    46,    47,    48,    85,   121,
     135,   185,   186,   193,   274,   275,   276,   277,   278,   279,
     280,   281,   283,   284,   285,   291,   292,   296,   297,   298,
     299,   300,   214,     3,   334,    63,   249,   174,   197,   258,
      91,   236,   237,   113,   193,   215,   215,   215,     3,   205,
     206,   255,   268,   273,   274,   261,   250,   262,   263,   274,
     261,   274,   115,   274,   119,   321,   322,   323,   272,   293,
     271,   315,     3,   245,     3,   169,   170,   171,   222,   223,
     224,   231,   125,   193,   195,   193,   193,   193,   112,   274,
     282,    46,   121,   275,   191,   275,   249,   274,   116,   132,
      17,    54,   101,   121,   128,   129,   136,   137,   174,   175,
     176,   177,   178,   181,   182,   184,   185,   186,   187,   188,
     189,   191,    98,   194,   197,   193,   274,   244,    39,   193,
       3,    24,    49,   104,   216,   217,   258,   193,   194,   197,
      94,   256,   257,   197,     3,   125,   319,   320,   267,   246,
     197,    90,   117,   264,   267,    55,    55,    62,   134,   161,
     172,   324,   323,   125,   193,    93,    11,    25,    29,    30,
      31,    32,    33,    35,    43,    68,    86,    88,    89,   103,
     108,   110,   120,   153,   225,   193,   172,   172,   194,   197,
     245,   194,   254,     3,   186,   249,   293,   274,   274,   282,
      92,   112,   118,   193,   121,   268,   194,   194,   274,   274,
     275,   275,   101,   128,   193,   104,   121,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   304,   315,     3,   270,   234,   216,     4,     4,
     213,     4,   194,   197,   270,   206,   193,   309,   310,   311,
     312,   313,   315,   330,   258,   273,     3,   263,     3,   265,
     321,   274,   274,   172,   161,   130,   158,   159,   325,   225,
     334,   214,   193,   193,   227,   193,   193,   193,   226,   104,
     121,   169,   170,   173,   228,   229,   230,   232,   334,   193,
     193,   223,    77,   286,   268,   193,   194,    94,   125,   109,
      92,   118,   274,   274,   249,   192,   116,   275,   193,   249,
     268,   104,   192,   194,     3,   194,    24,    49,   217,   194,
     245,    26,    66,    71,    78,    79,    95,   100,   331,   197,
     125,   317,   318,   319,    69,   259,     3,    62,   334,   160,
     194,   194,     6,     6,     6,     6,     6,   104,   172,   315,
     230,   232,   194,   334,   334,   193,   194,   194,   254,   274,
     225,   274,   274,   118,   109,   194,   275,   249,   268,   194,
     194,     4,   213,   194,    99,    78,    78,    78,    99,   310,
       3,   126,   325,   194,   194,   197,   194,   194,   194,   241,
     194,   194,   105,   287,   286,   286,   268,   194,   194,   118,
     274,   194,   194,   318,   311,   310,   193,   268,     6,   232,
     126,   261,   194,    82,   131,   334,    51,   260,   194,   268,
     162,   163,   164,   288,   289,   286,   193,   274,   332,   194,
     274,   194,     6,    17,   165,   168,   290,   334,   166,   167,
     290,   166,   167,   194,   116,   290
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   199,   200,   201,   201,   202,   202,   202,   202,   202,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     204,   204,   205,   205,   206,   206,   207,   207,   207,   208,
     208,   209,   210,   211,   211,   212,   212,   213,   214,   215,
     215,   215,   216,   216,   216,   216,   216,   216,   217,   217,
     218,   218,   219,   219,   219,   220,   220,   220,   220,   220,
     221,   221,   222,   222,   223,   223,   224,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   226,   226,   227,   227,   227,
     228,   228,   229,   229,   229,   229,   230,   230,   230,   230,
     231,   231,   231,   232,   233,   233,   233,   233,   234,   234,
     235,   236,   237,   238,   239,   240,   240,   241,   241,   242,
     243,   243,   244,   245,   245,   245,   246,   246,   247,   247,
     248,   248,   249,   249,   250,   251,   251,   251,   252,   252,
     253,   254,   254,   255,   256,   256,   257,   258,   258,   259,
     259,   260,   260,   261,   261,   262,   262,   263,   264,   264,
     264,   265,   265,   266,   266,   267,   267,   267,   267,   267,
     267,   268,   268,   269,   269,   270,   270,   271,   271,   272,
     272,   272,   273,   274,   274,   274,   274,   274,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   276,
     276,   277,   277,   277,   277,   277,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   279,   279,   280,
     280,   280,   280,   281,   281,   281,   281,   282,   282,   283,
     283,   284,   284,   284,   284,   284,   284,   284,   285,   285,
     285,   285,   286,   286,   287,   287,   288,   288,   288,   289,
     289,   289,   290,   290,   290,   290,   290,   291,   292,   293,
     293,   293,   293,   293,   293,   294,   294,   294,   294,   294,
     294,   295,   295,   296,   297,   298,   299,   299,   299,   299,
     300,   300,   300,   300,   300,   300,   300,   301,   302,   302,
     303,   303,   304,   305,   306,   307,   307,   307,   308,   309,
     309,   310,   310,   311,   311,   312,   312,   313,   314,   315,
     315,   316,   316,   317,   317,   318,   318,   319,   319,   320,
     320,   321,   321,   322,   322,   323,   323,   324,   324,   324,
     324,   325,   325,   325,   326,   326,   327,   328,   328,   329,
     330,   330,   330,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   332,   333,   333,   334,   334
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
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
       3,     1,     1,     1,     4,     1,     3,     2,     1,     1,
       3,     1,     0,     1,     5,     1,     0,     2,     1,     1,
       0,     1,     0,     1,     2,     3,     5,     1,     3,     1,
       2,     2,     1,     0,     1,     0,     2,     1,     3,     3,
       4,     6,     8,     1,     2,     1,     2,     1,     2,     1,
       1,     1,     0,     1,     1,     0,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined HSQL_LTYPE_IS_TRIVIAL && HSQL_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
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

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, hsql::SQLParserResult* result, yyscan_t scanner)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (result);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, hsql::SQLParserResult* result, yyscan_t scanner)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, result, scanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, hsql::SQLParserResult* result, yyscan_t scanner)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , result, scanner);
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
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
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
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
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
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
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
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
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, hsql::SQLParserResult* result, yyscan_t scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (result);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 3: /* IDENTIFIER  */
#line 194 "bison_parser.y" /* yacc.c:1257  */
      { free(((*yyvaluep).sval)); }
#line 1947 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 4: /* STRING  */
#line 194 "bison_parser.y" /* yacc.c:1257  */
      { free(((*yyvaluep).sval)); }
#line 1953 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 5: /* FLOATVAL  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 1959 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 6: /* INTVAL  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 1965 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 201: /* statement_list  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).stmt_vec)) {
    for (auto ptr : *(((*yyvaluep).stmt_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).stmt_vec));
}
#line 1978 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 202: /* statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).statement)); }
#line 1984 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 203: /* preparable_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).statement)); }
#line 1990 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 204: /* opt_hints  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2003 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 205: /* hint_list  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2016 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 206: /* hint  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2022 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 207: /* transaction_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).transaction_stmt)); }
#line 2028 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 209: /* prepare_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).prep_stmt)); }
#line 2034 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 210: /* prepare_target_query  */
#line 194 "bison_parser.y" /* yacc.c:1257  */
      { free(((*yyvaluep).sval)); }
#line 2040 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 211: /* execute_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).exec_stmt)); }
#line 2046 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 212: /* import_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).import_stmt)); }
#line 2052 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 213: /* file_type  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2058 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 214: /* file_path  */
#line 194 "bison_parser.y" /* yacc.c:1257  */
      { free(((*yyvaluep).sval)); }
#line 2064 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 215: /* opt_import_export_options  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).import_export_option_t)); }
#line 2070 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 216: /* import_export_options  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).import_export_option_t)); }
#line 2076 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 217: /* csv_option  */
#line 203 "bison_parser.y" /* yacc.c:1257  */
      {
  free(((*yyvaluep).csv_option_t)->second);
  delete (((*yyvaluep).csv_option_t));
}
#line 2085 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 218: /* export_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).export_stmt)); }
#line 2091 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 219: /* show_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).show_stmt)); }
#line 2097 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 220: /* create_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).create_stmt)); }
#line 2103 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 221: /* opt_not_exists  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2109 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 222: /* table_elem_commalist  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_element_vec)) {
    for (auto ptr : *(((*yyvaluep).table_element_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_element_vec));
}
#line 2122 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 223: /* table_elem  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).table_element_t)); }
#line 2128 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 224: /* column_def  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).column_t)); }
#line 2134 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 225: /* column_type  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2140 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 226: /* opt_time_precision  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2146 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 227: /* opt_decimal_specification  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).ival_pair)); }
#line 2152 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 228: /* opt_column_constraints  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).column_constraints_t)); }
#line 2158 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 229: /* column_constraints  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).column_constraints_t)); }
#line 2164 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 230: /* column_constraint  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2170 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 231: /* table_constraint  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).table_constraint_t)); }
#line 2176 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 232: /* references_spec  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).references_spec_t)); }
#line 2182 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 233: /* drop_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).drop_stmt)); }
#line 2188 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 234: /* opt_exists  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2194 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 235: /* alter_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).alter_stmt)); }
#line 2200 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 236: /* alter_action  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).alter_action_t)); }
#line 2206 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 237: /* drop_action  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).drop_action_t)); }
#line 2212 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 238: /* delete_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).delete_stmt)); }
#line 2218 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 239: /* truncate_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).delete_stmt)); }
#line 2224 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 240: /* insert_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).insert_stmt)); }
#line 2230 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 241: /* opt_column_list  */
#line 186 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 2243 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 242: /* update_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).update_stmt)); }
#line 2249 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 243: /* update_clause_commalist  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_vec)) {
    for (auto ptr : *(((*yyvaluep).update_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).update_vec));
}
#line 2262 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 244: /* update_clause  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).update_t)); }
#line 2268 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 245: /* select_statement  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).select_stmt)); }
#line 2274 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 246: /* select_within_set_operation  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).select_stmt)); }
#line 2280 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 247: /* select_within_set_operation_no_parentheses  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).select_stmt)); }
#line 2286 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 248: /* select_with_paren  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).select_stmt)); }
#line 2292 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 249: /* select_no_paren  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).select_stmt)); }
#line 2298 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 250: /* set_operator  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).set_operator_t)); }
#line 2304 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 251: /* set_type  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).set_operator_t)); }
#line 2310 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 252: /* opt_all  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2316 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 253: /* select_clause  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).select_stmt)); }
#line 2322 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 254: /* opt_distinct  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2328 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 255: /* select_list  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2341 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 256: /* opt_from_clause  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).table)); }
#line 2347 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 257: /* from_clause  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).table)); }
#line 2353 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 258: /* opt_where  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2359 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 259: /* opt_group  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).group_t)); }
#line 2365 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 260: /* opt_having  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2371 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 261: /* opt_order  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 2384 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 262: /* order_list  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_vec)) {
    for (auto ptr : *(((*yyvaluep).order_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).order_vec));
}
#line 2397 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 263: /* order_desc  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).order)); }
#line 2403 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 264: /* opt_order_type  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2409 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 265: /* opt_null_ordering  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2415 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 266: /* opt_top  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).limit)); }
#line 2421 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 267: /* opt_limit  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).limit)); }
#line 2427 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 268: /* expr_list  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2440 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 269: /* opt_extended_literal_list  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2453 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 270: /* extended_literal_list  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2466 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 271: /* casted_extended_literal  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2472 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 272: /* extended_literal  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2478 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 273: /* expr_alias  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2484 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 274: /* expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2490 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 275: /* operand  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2496 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 276: /* scalar_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2502 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 277: /* unary_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2508 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 278: /* binary_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2514 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 279: /* logic_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2520 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 280: /* in_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2526 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 281: /* case_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2532 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 282: /* case_list  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2538 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 283: /* exists_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2544 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 284: /* comp_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2550 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 285: /* function_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2556 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 286: /* opt_window  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).window_description)); }
#line 2562 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 287: /* opt_partition  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expr_vec)) {
    for (auto ptr : *(((*yyvaluep).expr_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).expr_vec));
}
#line 2575 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 288: /* opt_frame_clause  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).frame_description)); }
#line 2581 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 289: /* frame_type  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2587 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 290: /* frame_bound  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).frame_bound)); }
#line 2593 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 291: /* extract_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2599 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 292: /* cast_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2605 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 293: /* datetime_field  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2611 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 294: /* datetime_field_plural  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2617 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 295: /* duration_field  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2623 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 296: /* array_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2629 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 297: /* array_index  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2635 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 298: /* between_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2641 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 299: /* column_name  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2647 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 300: /* literal  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2653 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 301: /* string_literal  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2659 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 302: /* bool_literal  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2665 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 303: /* num_literal  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2671 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 304: /* int_literal  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2677 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 305: /* null_literal  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2683 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 306: /* date_literal  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2689 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 307: /* interval_literal  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2695 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 308: /* param_expr  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2701 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 309: /* table_ref  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).table)); }
#line 2707 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 310: /* table_ref_atomic  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).table)); }
#line 2713 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 311: /* nonjoin_table_ref_atomic  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).table)); }
#line 2719 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 312: /* table_ref_commalist  */
#line 195 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_vec)) {
    for (auto ptr : *(((*yyvaluep).table_vec))) {
      delete ptr;
    }
  }
  delete (((*yyvaluep).table_vec));
}
#line 2732 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 313: /* table_ref_name  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).table)); }
#line 2738 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 314: /* table_ref_name_no_alias  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).table)); }
#line 2744 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 315: /* table_name  */
#line 182 "bison_parser.y" /* yacc.c:1257  */
      {
  free(((*yyvaluep).table_name).name);
  free(((*yyvaluep).table_name).schema);
}
#line 2753 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 316: /* opt_index_name  */
#line 194 "bison_parser.y" /* yacc.c:1257  */
      { free(((*yyvaluep).sval)); }
#line 2759 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 317: /* table_alias  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).alias_t)); }
#line 2765 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 318: /* opt_table_alias  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).alias_t)); }
#line 2771 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 319: /* alias  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).alias_t)); }
#line 2777 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 320: /* opt_alias  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).alias_t)); }
#line 2783 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 321: /* opt_locking_clause  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).locking_clause_vec)); }
#line 2789 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 322: /* opt_locking_clause_list  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).locking_clause_vec)); }
#line 2795 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 323: /* locking_clause  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).locking_t)); }
#line 2801 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 324: /* row_lock_mode  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2807 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 325: /* opt_row_lock_policy  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2813 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 326: /* opt_with_clause  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).with_description_vec)); }
#line 2819 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 327: /* with_clause  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).with_description_vec)); }
#line 2825 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 328: /* with_description_list  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).with_description_vec)); }
#line 2831 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 329: /* with_description  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).with_description_t)); }
#line 2837 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 330: /* join_clause  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).table)); }
#line 2843 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 331: /* opt_join_type  */
#line 181 "bison_parser.y" /* yacc.c:1257  */
      { }
#line 2849 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 332: /* join_condition  */
#line 207 "bison_parser.y" /* yacc.c:1257  */
      { delete (((*yyvaluep).expr)); }
#line 2855 "bison_parser.cpp" /* yacc.c:1257  */
        break;

    case 334: /* ident_commalist  */
#line 186 "bison_parser.y" /* yacc.c:1257  */
      {
  if (((*yyvaluep).str_vec)) {
    for (auto ptr : *(((*yyvaluep).str_vec))) {
      free(ptr);
    }
  }
  delete (((*yyvaluep).str_vec));
}
#line 2868 "bison_parser.cpp" /* yacc.c:1257  */
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
/* The lookahead symbol.  */
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
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

/* User initialization code.  */
#line 81 "bison_parser.y" /* yacc.c:1429  */
{
  // Initialize
  yylloc.first_column = 0;
  yylloc.last_column = 0;
  yylloc.first_line = 0;
  yylloc.last_line = 0;
  yylloc.total_column = 0;
  yylloc.string_length = 0;
}

#line 2987 "bison_parser.cpp" /* yacc.c:1429  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
| yyreduce -- Do a reduction.  |
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 343 "bison_parser.y" /* yacc.c:1646  */
    {
  for (SQLStatement* stmt : *(yyvsp[-1].stmt_vec)) {
    // Transfers ownership of the statement.
    result->addStatement(stmt);
  }

  unsigned param_id = 0;
  for (void* param : yyloc.param_list) {
    if (param) {
      Expr* expr = (Expr*)param;
      expr->ival = param_id;
      result->addParameter(expr);
      ++param_id;
    }
  }
    delete (yyvsp[-1].stmt_vec);
  }
#line 3192 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 362 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyval.stmt_vec) = new std::vector<SQLStatement*>();
  (yyval.stmt_vec)->push_back((yyvsp[0].statement));
}
#line 3203 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 368 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[0].statement)->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  (yyvsp[-2].stmt_vec)->push_back((yyvsp[0].statement));
  (yyval.stmt_vec) = (yyvsp[-2].stmt_vec);
}
#line 3214 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 375 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.statement) = (yyvsp[-1].prep_stmt);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3223 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 379 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.statement) = (yyvsp[-1].statement);
  (yyval.statement)->hints = (yyvsp[0].expr_vec);
}
#line 3232 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 383 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].show_stmt); }
#line 3238 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 384 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].import_stmt); }
#line 3244 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 385 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].export_stmt); }
#line 3250 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 387 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].select_stmt); }
#line 3256 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 388 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].create_stmt); }
#line 3262 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 389 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].insert_stmt); }
#line 3268 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 390 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3274 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 391 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 3280 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 392 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].update_stmt); }
#line 3286 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 393 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].drop_stmt); }
#line 3292 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 394 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].alter_stmt); }
#line 3298 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 395 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 3304 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 396 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].transaction_stmt); }
#line 3310 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 402 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr_vec) = (yyvsp[-1].expr_vec); }
#line 3316 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 403 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr_vec) = nullptr; }
#line 3322 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 405 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 3331 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 409 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 3340 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 414 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[0].sval);
}
#line 3349 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 418 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = Expr::make(kExprHint);
  (yyval.expr)->name = (yyvsp[-3].sval);
  (yyval.expr)->exprList = (yyvsp[-1].expr_vec);
}
#line 3359 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 428 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.transaction_stmt) = new TransactionStatement(kBeginTransaction); }
#line 3365 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 429 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.transaction_stmt) = new TransactionStatement(kRollbackTransaction); }
#line 3371 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 430 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.transaction_stmt) = new TransactionStatement(kCommitTransaction); }
#line 3377 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 438 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.prep_stmt) = new PrepareStatement();
  (yyval.prep_stmt)->name = (yyvsp[-2].sval);
  (yyval.prep_stmt)->query = (yyvsp[0].sval);
}
#line 3387 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 446 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[0].sval);
}
#line 3396 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 450 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.exec_stmt) = new ExecuteStatement();
  (yyval.exec_stmt)->name = (yyvsp[-3].sval);
  (yyval.exec_stmt)->parameters = (yyvsp[-1].expr_vec);
}
#line 3406 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 462 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.import_stmt) = new ImportStatement((yyvsp[-4].import_type_t));
  (yyval.import_stmt)->filePath = (yyvsp[-2].sval);
  (yyval.import_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.import_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 3417 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 468 "bison_parser.y" /* yacc.c:1646  */
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
#line 3438 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 485 "bison_parser.y" /* yacc.c:1646  */
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
#line 3457 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 500 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.sval) = (yyvsp[0].sval); }
#line 3463 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 502 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 3469 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 503 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.import_export_option_t) = (yyvsp[-1].import_export_option_t); }
#line 3475 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 504 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.import_export_option_t) = new ImportExportOptions{}; }
#line 3481 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 506 "bison_parser.y" /* yacc.c:1646  */
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
#line 3500 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 520 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->format = (yyvsp[0].import_type_t);
}
#line 3509 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 524 "bison_parser.y" /* yacc.c:1646  */
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
#line 3524 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 534 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->encoding = (yyvsp[0].sval);
}
#line 3533 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 538 "bison_parser.y" /* yacc.c:1646  */
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
#line 3562 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 562 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.import_export_option_t) = new ImportExportOptions{};
  (yyval.import_export_option_t)->csv_options = new CsvOptions{};
  (yyval.import_export_option_t)->csv_options->accept_csv_option((yyvsp[0].csv_option_t));

  delete (yyvsp[0].csv_option_t);
}
#line 3574 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 570 "bison_parser.y" /* yacc.c:1646  */
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
#line 3592 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 583 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.csv_option_t) = new std::pair<CsvOptionType, char*>(CsvOptionType::Null, (yyvsp[0].sval)); }
#line 3598 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 590 "bison_parser.y" /* yacc.c:1646  */
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
#line 3618 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 605 "bison_parser.y" /* yacc.c:1646  */
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
#line 3637 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 625 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.show_stmt) = new ShowStatement(kShowTables); }
#line 3643 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 626 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 3653 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 631 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.show_stmt) = new ShowStatement(kShowColumns);
  (yyval.show_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.show_stmt)->name = (yyvsp[0].table_name).name;
}
#line 3663 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 642 "bison_parser.y" /* yacc.c:1646  */
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
#line 3681 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 655 "bison_parser.y" /* yacc.c:1646  */
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
#line 3698 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 667 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.create_stmt) = new CreateStatement(kCreateTable);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-3].bval);
  (yyval.create_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 3710 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 674 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.create_stmt) = new CreateStatement(kCreateIndex);
  (yyval.create_stmt)->indexName = (yyvsp[-5].sval);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-6].bval);
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->indexColumns = (yyvsp[-1].str_vec);
}
#line 3722 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 681 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.create_stmt) = new CreateStatement(kCreateView);
  (yyval.create_stmt)->ifNotExists = (yyvsp[-4].bval);
  (yyval.create_stmt)->schema = (yyvsp[-3].table_name).schema;
  (yyval.create_stmt)->tableName = (yyvsp[-3].table_name).name;
  (yyval.create_stmt)->viewColumns = (yyvsp[-2].str_vec);
  (yyval.create_stmt)->select = (yyvsp[0].select_stmt);
}
#line 3735 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 690 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = true; }
#line 3741 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 691 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = false; }
#line 3747 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 693 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.table_element_vec) = new std::vector<TableElement*>();
  (yyval.table_element_vec)->push_back((yyvsp[0].table_element_t));
}
#line 3756 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 697 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].table_element_vec)->push_back((yyvsp[0].table_element_t));
  (yyval.table_element_vec) = (yyvsp[-2].table_element_vec);
}
#line 3765 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 702 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.table_element_t) = (yyvsp[0].column_t); }
#line 3771 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 703 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.table_element_t) = (yyvsp[0].table_constraint_t); }
#line 3777 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 705 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.column_t) = new ColumnDefinition((yyvsp[-2].sval), (yyvsp[-1].column_type_t), (yyvsp[0].column_constraints_t)->constraints, (yyvsp[0].column_constraints_t)->references);
  if (!(yyval.column_t)->trySetNullableExplicit()) {
    yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{(yyvsp[-2].sval)}).c_str());
  }
  delete (yyvsp[0].column_constraints_t);
}
#line 3789 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 713 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::BIGINT}; }
#line 3795 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 714 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::BOOLEAN}; }
#line 3801 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 715 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::CHAR, (yyvsp[-1].ival)}; }
#line 3807 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 716 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 3813 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 717 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::DATE}; }
#line 3819 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 718 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 3825 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 719 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.column_type_t) = ColumnType{DataType::DECIMAL, 0, (yyvsp[0].ival_pair)->first, (yyvsp[0].ival_pair)->second};
  delete (yyvsp[0].ival_pair);
}
#line 3834 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 723 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::DOUBLE}; }
#line 3840 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 724 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::FLOAT}; }
#line 3846 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 725 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 3852 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 726 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::INT}; }
#line 3858 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 727 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::LONG}; }
#line 3864 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 728 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::REAL}; }
#line 3870 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 729 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::SMALLINT}; }
#line 3876 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 730 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::TEXT}; }
#line 3882 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 731 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::TIME, 0, (yyvsp[0].ival)}; }
#line 3888 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 732 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::DATETIME}; }
#line 3894 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 733 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_type_t) = ColumnType{DataType::VARCHAR, (yyvsp[-1].ival)}; }
#line 3900 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 735 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[-1].ival); }
#line 3906 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 736 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 3912 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 738 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-3].ival), (yyvsp[-1].ival)}; }
#line 3918 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 739 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{(yyvsp[-1].ival), 0}; }
#line 3924 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 740 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.ival_pair) = new std::pair<int64_t, int64_t>{0, 0}; }
#line 3930 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 742 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_constraints_t) = (yyvsp[0].column_constraints_t); }
#line 3936 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 743 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_constraints_t) = new ColumnConstraints(); }
#line 3942 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 745 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
}
#line 3951 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 749 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-1].column_constraints_t)->constraints->insert((yyvsp[0].column_constraint_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 3960 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 753 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.column_constraints_t) = new ColumnConstraints();
  (yyval.column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyval.column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
}
#line 3970 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 758 "bison_parser.y" /* yacc.c:1646  */
    {
  // Multiple foreign keys for the same column could be possible, so we do not raise an error in that case.
  // Think of foreign keys referenced on multiple levels (returned item references sold item references items).
  (yyvsp[-1].column_constraints_t)->constraints->insert(ConstraintType::ForeignKey);
  (yyvsp[-1].column_constraints_t)->references->emplace_back((yyvsp[0].references_spec_t));
  (yyval.column_constraints_t) = (yyvsp[-1].column_constraints_t);
}
#line 3982 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 766 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_constraint_t) = ConstraintType::PrimaryKey; }
#line 3988 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 767 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_constraint_t) = ConstraintType::Unique; }
#line 3994 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 768 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_constraint_t) = ConstraintType::Null; }
#line 4000 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 769 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_constraint_t) = ConstraintType::NotNull; }
#line 4006 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 771 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::PrimaryKey, (yyvsp[-1].str_vec)); }
#line 4012 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 772 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.table_constraint_t) = new TableConstraint(ConstraintType::Unique, (yyvsp[-1].str_vec)); }
#line 4018 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 773 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.table_constraint_t) = new ForeignKeyConstraint((yyvsp[-2].str_vec), (yyvsp[0].references_spec_t)); }
#line 4024 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 775 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.references_spec_t) = new ReferencesSpecification((yyvsp[-1].table_name).schema, (yyvsp[-1].table_name).name, (yyvsp[0].str_vec)); }
#line 4030 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 783 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.drop_stmt) = new DropStatement(kDropTable);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4041 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 789 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.drop_stmt) = new DropStatement(kDropView);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.drop_stmt)->name = (yyvsp[0].table_name).name;
}
#line 4052 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 795 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.drop_stmt) = new DropStatement(kDropPreparedStatement);
  (yyval.drop_stmt)->ifExists = false;
  (yyval.drop_stmt)->name = (yyvsp[0].sval);
}
#line 4062 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 801 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.drop_stmt) = new DropStatement(kDropIndex);
  (yyval.drop_stmt)->ifExists = (yyvsp[-1].bval);
  (yyval.drop_stmt)->indexName = (yyvsp[0].sval);
}
#line 4072 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 807 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = true; }
#line 4078 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 808 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = false; }
#line 4084 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 815 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.alter_stmt) = new AlterStatement((yyvsp[-1].table_name).name, (yyvsp[0].alter_action_t));
  (yyval.alter_stmt)->ifTableExists = (yyvsp[-2].bval);
  (yyval.alter_stmt)->schema = (yyvsp[-1].table_name).schema;
}
#line 4094 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 821 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.alter_action_t) = (yyvsp[0].drop_action_t); }
#line 4100 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 823 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.drop_action_t) = new DropColumnAction((yyvsp[0].sval));
  (yyval.drop_action_t)->ifExists = (yyvsp[-1].bval);
}
#line 4109 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 833 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[-1].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[-1].table_name).name;
  (yyval.delete_stmt)->expr = (yyvsp[0].expr);
}
#line 4120 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 840 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.delete_stmt) = new DeleteStatement();
  (yyval.delete_stmt)->schema = (yyvsp[0].table_name).schema;
  (yyval.delete_stmt)->tableName = (yyvsp[0].table_name).name;
}
#line 4130 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 851 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.insert_stmt) = new InsertStatement(kInsertValues);
  (yyval.insert_stmt)->schema = (yyvsp[-5].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-5].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-4].str_vec);
  (yyval.insert_stmt)->values = (yyvsp[-1].expr_vec);
}
#line 4142 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 858 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.insert_stmt) = new InsertStatement(kInsertSelect);
  (yyval.insert_stmt)->schema = (yyvsp[-2].table_name).schema;
  (yyval.insert_stmt)->tableName = (yyvsp[-2].table_name).name;
  (yyval.insert_stmt)->columns = (yyvsp[-1].str_vec);
  (yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
}
#line 4154 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 866 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.str_vec) = (yyvsp[-1].str_vec); }
#line 4160 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 867 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.str_vec) = nullptr; }
#line 4166 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 874 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.update_stmt) = new UpdateStatement();
  (yyval.update_stmt)->table = (yyvsp[-3].table);
  (yyval.update_stmt)->updates = (yyvsp[-1].update_vec);
  (yyval.update_stmt)->where = (yyvsp[0].expr);
}
#line 4177 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 881 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.update_vec) = new std::vector<UpdateClause*>();
  (yyval.update_vec)->push_back((yyvsp[0].update_t));
}
#line 4186 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 885 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].update_vec)->push_back((yyvsp[0].update_t));
  (yyval.update_vec) = (yyvsp[-2].update_vec);
}
#line 4195 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 890 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.update_t) = new UpdateClause();
  (yyval.update_t)->column = (yyvsp[-2].sval);
  (yyval.update_t)->value = (yyvsp[0].expr);
}
#line 4205 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 900 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4214 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 904 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.select_stmt) = (yyvsp[0].select_stmt);
  (yyval.select_stmt)->withDescriptions = (yyvsp[-1].with_description_vec);
}
#line 4223 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 908 "bison_parser.y" /* yacc.c:1646  */
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
#line 4239 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 922 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.select_stmt) = (yyvsp[0].select_stmt); }
#line 4245 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 923 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.select_stmt) = (yyvsp[-2].select_stmt);
  if ((yyval.select_stmt)->setOperations == nullptr) {
    (yyval.select_stmt)->setOperations = new std::vector<SetOperation*>();
  }
  (yyval.select_stmt)->setOperations->push_back((yyvsp[-1].set_operator_t));
  (yyval.select_stmt)->setOperations->back()->nestedSelectStatement = (yyvsp[0].select_stmt);
}
#line 4258 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 932 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4264 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 933 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 4270 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 935 "bison_parser.y" /* yacc.c:1646  */
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
#line 4289 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 949 "bison_parser.y" /* yacc.c:1646  */
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
#line 4305 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 961 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.set_operator_t) = (yyvsp[-1].set_operator_t);
  (yyval.set_operator_t)->isAll = (yyvsp[0].bval);
}
#line 4314 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 966 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetUnion;
}
#line 4323 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 970 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetIntersect;
}
#line 4332 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 974 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.set_operator_t) = new SetOperation();
  (yyval.set_operator_t)->setType = SetType::kSetExcept;
}
#line 4341 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 979 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = true; }
#line 4347 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 980 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = false; }
#line 4353 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 982 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.select_stmt) = new SelectStatement();
  (yyval.select_stmt)->limit = (yyvsp[-5].limit);
  (yyval.select_stmt)->selectDistinct = (yyvsp[-4].bval);
  (yyval.select_stmt)->selectList = (yyvsp[-3].expr_vec);
  (yyval.select_stmt)->fromTable = (yyvsp[-2].table);
  (yyval.select_stmt)->whereClause = (yyvsp[-1].expr);
  (yyval.select_stmt)->groupBy = (yyvsp[0].group_t);
}
#line 4367 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 992 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = true; }
#line 4373 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 993 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = false; }
#line 4379 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 997 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.table) = (yyvsp[0].table); }
#line 4385 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 998 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.table) = nullptr; }
#line 4391 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1000 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.table) = (yyvsp[0].table); }
#line 4397 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1002 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 4403 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1003 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = nullptr; }
#line 4409 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1005 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.group_t) = new GroupByDescription();
  (yyval.group_t)->columns = (yyvsp[-1].expr_vec);
  (yyval.group_t)->having = (yyvsp[0].expr);
}
#line 4419 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1010 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.group_t) = nullptr; }
#line 4425 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1012 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 4431 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1013 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = nullptr; }
#line 4437 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1015 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.order_vec) = (yyvsp[0].order_vec); }
#line 4443 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1016 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.order_vec) = nullptr; }
#line 4449 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1018 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.order_vec) = new std::vector<OrderDescription*>();
  (yyval.order_vec)->push_back((yyvsp[0].order));
}
#line 4458 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1022 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].order_vec)->push_back((yyvsp[0].order));
  (yyval.order_vec) = (yyvsp[-2].order_vec);
}
#line 4467 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1027 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.order) = new OrderDescription((yyvsp[-1].order_type), (yyvsp[-2].expr), (yyvsp[0].null_ordering_t)); }
#line 4473 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 1029 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.order_type) = kOrderAsc; }
#line 4479 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1030 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.order_type) = kOrderDesc; }
#line 4485 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1031 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.order_type) = kOrderAsc; }
#line 4491 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1033 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.null_ordering_t) = NullOrdering::Undefined; }
#line 4497 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1034 "bison_parser.y" /* yacc.c:1646  */
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
#line 4521 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1056 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 4527 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 1057 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.limit) = nullptr; }
#line 4533 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1059 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.limit) = new LimitDescription((yyvsp[0].expr), nullptr); }
#line 4539 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1060 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 4545 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1061 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.limit) = new LimitDescription((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 4551 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1062 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.limit) = new LimitDescription(nullptr, nullptr); }
#line 4557 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 1063 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.limit) = new LimitDescription(nullptr, (yyvsp[0].expr)); }
#line 4563 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1064 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.limit) = nullptr; }
#line 4569 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 1069 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 4578 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 1073 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 4587 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 1079 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 4593 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 1080 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr_vec) = nullptr; }
#line 4599 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 1082 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr_vec) = new std::vector<Expr*>();
  (yyval.expr_vec)->push_back((yyvsp[0].expr));
}
#line 4608 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 1086 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr));
  (yyval.expr_vec) = (yyvsp[-2].expr_vec);
}
#line 4617 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 1091 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t));
}
#line 4625 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 1095 "bison_parser.y" /* yacc.c:1646  */
    {
  if ((yyvsp[0].expr)->type == ExprType::kExprParameter) {
    delete (yyvsp[0].expr);
    yyerror(&yyloc, result, scanner, "Parameter ? is not a valid literal.");
    YYERROR;
  }
  (yyval.expr) = (yyvsp[0].expr);
}
#line 4638 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 1103 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 4644 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1104 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 4650 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1106 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[-1].expr);
  if ((yyvsp[0].alias_t)) {
    (yyval.expr)->alias = (yyvsp[0].alias_t)->name;
    (yyvsp[0].alias_t)->name = nullptr;
    delete (yyvsp[0].alias_t);
  }
}
#line 4663 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1117 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[-1].expr); }
#line 4669 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1119 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = Expr::makeSelect((yyvsp[-1].select_stmt));
}
#line 4677 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1125 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(kOpUnaryMinus, (yyvsp[0].expr)); }
#line 4683 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1126 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(kOpNot, (yyvsp[0].expr)); }
#line 4689 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1127 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-1].expr)); }
#line 4695 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1128 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(kOpIsNull, (yyvsp[-2].expr)); }
#line 4701 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1129 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeOpUnary(kOpIsNull, (yyvsp[-3].expr))); }
#line 4707 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1131 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpMinus, (yyvsp[0].expr)); }
#line 4713 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1132 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPlus, (yyvsp[0].expr)); }
#line 4719 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1133 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpSlash, (yyvsp[0].expr)); }
#line 4725 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1134 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAsterisk, (yyvsp[0].expr)); }
#line 4731 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1135 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpPercentage, (yyvsp[0].expr)); }
#line 4737 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1136 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpCaret, (yyvsp[0].expr)); }
#line 4743 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1137 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLike, (yyvsp[0].expr)); }
#line 4749 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 1138 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-3].expr), kOpNotLike, (yyvsp[0].expr)); }
#line 4755 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1139 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpILike, (yyvsp[0].expr)); }
#line 4761 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1140 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpConcat, (yyvsp[0].expr)); }
#line 4767 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1142 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpAnd, (yyvsp[0].expr)); }
#line 4773 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1143 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpOr, (yyvsp[0].expr)); }
#line 4779 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1145 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].expr_vec)); }
#line 4785 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1146 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].expr_vec))); }
#line 4791 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1147 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeInOperator((yyvsp[-4].expr), (yyvsp[-1].select_stmt)); }
#line 4797 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1148 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeInOperator((yyvsp[-5].expr), (yyvsp[-1].select_stmt))); }
#line 4803 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1152 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeCase((yyvsp[-2].expr), (yyvsp[-1].expr), nullptr); }
#line 4809 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1153 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeCase((yyvsp[-4].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 4815 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1154 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-1].expr), nullptr); }
#line 4821 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1155 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeCase(nullptr, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 4827 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1157 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeCaseList(Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 4833 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1158 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::caseListAppend((yyvsp[-4].expr), Expr::makeCaseListElement((yyvsp[-2].expr), (yyvsp[0].expr))); }
#line 4839 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1160 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeExists((yyvsp[-1].select_stmt)); }
#line 4845 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1161 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(kOpNot, Expr::makeExists((yyvsp[-1].select_stmt))); }
#line 4851 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1163 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 4857 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1164 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpEquals, (yyvsp[0].expr)); }
#line 4863 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1165 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpNotEquals, (yyvsp[0].expr)); }
#line 4869 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1166 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLess, (yyvsp[0].expr)); }
#line 4875 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1167 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreater, (yyvsp[0].expr)); }
#line 4881 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1168 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpLessEq, (yyvsp[0].expr)); }
#line 4887 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1169 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), kOpGreaterEq, (yyvsp[0].expr)); }
#line 4893 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1173 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 4899 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 1174 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description)); }
#line 4905 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1175 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-3].sval), (yyvsp[-5].sval), new std::vector<Expr*>(), false, (yyvsp[0].window_description)); }
#line 4911 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1176 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-5].sval), (yyvsp[-7].sval), (yyvsp[-2].expr_vec), (yyvsp[-3].bval), (yyvsp[0].window_description)); }
#line 4917 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1180 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.window_description) = new WindowDescription((yyvsp[-3].expr_vec), (yyvsp[-2].order_vec), (yyvsp[-1].frame_description)); }
#line 4923 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1181 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.window_description) = nullptr; }
#line 4929 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1183 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 4935 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1184 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr_vec) = nullptr; }
#line 4941 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1189 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.frame_description) = new FrameDescription{(yyvsp[-1].frame_type), (yyvsp[0].frame_bound), new FrameBound{0, kCurrentRow, false}}; }
#line 4947 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 1190 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.frame_description) = new FrameDescription{(yyvsp[-4].frame_type), (yyvsp[-2].frame_bound), (yyvsp[0].frame_bound)}; }
#line 4953 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 1191 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.frame_description) = new FrameDescription{kRange, new FrameBound{0, kPreceding, true}, new FrameBound{0, kCurrentRow, false}};
}
#line 4961 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1195 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.frame_type) = kRange; }
#line 4967 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1196 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.frame_type) = kRows; }
#line 4973 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 1197 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.frame_type) = kGroups; }
#line 4979 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1199 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.frame_bound) = new FrameBound{0, kPreceding, true}; }
#line 4985 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 1200 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kPreceding, false}; }
#line 4991 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 1201 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.frame_bound) = new FrameBound{0, kFollowing, true}; }
#line 4997 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 1202 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.frame_bound) = new FrameBound{(yyvsp[-1].ival), kFollowing, false}; }
#line 5003 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 1203 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.frame_bound) = new FrameBound{0, kCurrentRow, false}; }
#line 5009 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 1205 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeExtract((yyvsp[-3].datetime_field), (yyvsp[-1].expr)); }
#line 5015 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 258:
#line 1207 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeCast((yyvsp[-3].expr), (yyvsp[-1].column_type_t)); }
#line 5021 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 1209 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeSecond; }
#line 5027 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 260:
#line 1210 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeMinute; }
#line 5033 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 1211 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeHour; }
#line 5039 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 1212 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeDay; }
#line 5045 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 1213 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeMonth; }
#line 5051 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 1214 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeYear; }
#line 5057 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 1216 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeSecond; }
#line 5063 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 1217 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeMinute; }
#line 5069 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 1218 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeHour; }
#line 5075 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 1219 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeDay; }
#line 5081 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 1220 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeMonth; }
#line 5087 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 1221 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.datetime_field) = kDatetimeYear; }
#line 5093 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 1225 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeArray((yyvsp[-1].expr_vec)); }
#line 5099 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 1227 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeArrayIndex((yyvsp[-3].expr), (yyvsp[-1].expr)->ival); }
#line 5105 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 1229 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeBetween((yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 5111 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 1231 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 5117 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 1232 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 5123 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 1233 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeStar(); }
#line 5129 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 1234 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeStar((yyvsp[-2].sval)); }
#line 5135 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 1238 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeLiteral((yyvsp[0].sval)); }
#line 5141 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 1240 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeLiteral(true); }
#line 5147 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 1241 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeLiteral(false); }
#line 5153 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 1243 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeLiteral((yyvsp[0].fval)); }
#line 5159 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 1246 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeLiteral((yyvsp[0].ival)); }
#line 5165 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 293:
#line 1248 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeNullLiteral(); }
#line 5171 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 294:
#line 1250 "bison_parser.y" /* yacc.c:1646  */
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
#line 5186 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 295:
#line 1261 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeIntervalLiteral((yyvsp[-1].ival), (yyvsp[0].datetime_field)); }
#line 5192 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 296:
#line 1262 "bison_parser.y" /* yacc.c:1646  */
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
#line 5208 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 297:
#line 1273 "bison_parser.y" /* yacc.c:1646  */
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
#line 5244 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 298:
#line 1305 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = Expr::makeParameter(yylloc.total_column);
  (yyval.expr)->ival2 = yyloc.param_list.size();
  yyloc.param_list.push_back((yyval.expr));
}
#line 5254 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 300:
#line 1314 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  auto tbl = new TableRef(kTableCrossProduct);
  tbl->list = (yyvsp[-2].table_vec);
  (yyval.table) = tbl;
}
#line 5265 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 304:
#line 1323 "bison_parser.y" /* yacc.c:1646  */
    {
  auto tbl = new TableRef(kTableSelect);
  tbl->select = (yyvsp[-2].select_stmt);
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 5276 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 305:
#line 1330 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.table_vec) = new std::vector<TableRef*>();
  (yyval.table_vec)->push_back((yyvsp[0].table));
}
#line 5285 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 306:
#line 1334 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].table_vec)->push_back((yyvsp[0].table));
  (yyval.table_vec) = (yyvsp[-2].table_vec);
}
#line 5294 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 307:
#line 1339 "bison_parser.y" /* yacc.c:1646  */
    {
  auto tbl = new TableRef(kTableName);
  tbl->schema = (yyvsp[-1].table_name).schema;
  tbl->name = (yyvsp[-1].table_name).name;
  tbl->alias = (yyvsp[0].alias_t);
  (yyval.table) = tbl;
}
#line 5306 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 308:
#line 1347 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.table) = new TableRef(kTableName);
  (yyval.table)->schema = (yyvsp[0].table_name).schema;
  (yyval.table)->name = (yyvsp[0].table_name).name;
}
#line 5316 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 309:
#line 1353 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.table_name).schema = nullptr;
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 5325 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 310:
#line 1357 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.table_name).schema = (yyvsp[-2].sval);
  (yyval.table_name).name = (yyvsp[0].sval);
}
#line 5334 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 311:
#line 1362 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.sval) = (yyvsp[0].sval); }
#line 5340 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 312:
#line 1363 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.sval) = nullptr; }
#line 5346 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 1365 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.alias_t) = new Alias((yyvsp[-3].sval), (yyvsp[-1].str_vec)); }
#line 5352 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 1367 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.alias_t) = nullptr; }
#line 5358 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 1369 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 5364 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 318:
#line 1370 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.alias_t) = new Alias((yyvsp[0].sval)); }
#line 5370 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 320:
#line 1372 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.alias_t) = nullptr; }
#line 5376 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 321:
#line 1378 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.locking_clause_vec) = (yyvsp[0].locking_clause_vec); }
#line 5382 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 322:
#line 1379 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.locking_clause_vec) = nullptr; }
#line 5388 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 323:
#line 1381 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.locking_clause_vec) = new std::vector<LockingClause*>();
  (yyval.locking_clause_vec)->push_back((yyvsp[0].locking_t));
}
#line 5397 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 324:
#line 1385 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-1].locking_clause_vec)->push_back((yyvsp[0].locking_t));
  (yyval.locking_clause_vec) = (yyvsp[-1].locking_clause_vec);
}
#line 5406 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 1390 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-1].lock_mode_t);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
  (yyval.locking_t)->tables = nullptr;
}
#line 5417 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 1396 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.locking_t) = new LockingClause();
  (yyval.locking_t)->rowLockMode = (yyvsp[-3].lock_mode_t);
  (yyval.locking_t)->tables = (yyvsp[-1].str_vec);
  (yyval.locking_t)->rowLockWaitPolicy = (yyvsp[0].lock_wait_policy_t);
}
#line 5428 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 327:
#line 1403 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.lock_mode_t) = RowLockMode::ForUpdate; }
#line 5434 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 328:
#line 1404 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.lock_mode_t) = RowLockMode::ForNoKeyUpdate; }
#line 5440 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 1405 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.lock_mode_t) = RowLockMode::ForShare; }
#line 5446 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 1406 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.lock_mode_t) = RowLockMode::ForKeyShare; }
#line 5452 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 1408 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::SkipLocked; }
#line 5458 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 1409 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::NoWait; }
#line 5464 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 333:
#line 1410 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.lock_wait_policy_t) = RowLockWaitPolicy::None; }
#line 5470 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 335:
#line 1416 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.with_description_vec) = nullptr; }
#line 5476 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 336:
#line 1418 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.with_description_vec) = (yyvsp[0].with_description_vec); }
#line 5482 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 337:
#line 1420 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.with_description_vec) = new std::vector<WithDescription*>();
  (yyval.with_description_vec)->push_back((yyvsp[0].with_description_t));
}
#line 5491 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 338:
#line 1424 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].with_description_vec)->push_back((yyvsp[0].with_description_t));
  (yyval.with_description_vec) = (yyvsp[-2].with_description_vec);
}
#line 5500 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 339:
#line 1429 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.with_description_t) = new WithDescription();
  (yyval.with_description_t)->alias = (yyvsp[-2].sval);
  (yyval.with_description_t)->select = (yyvsp[0].select_stmt);
}
#line 5510 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 340:
#line 1439 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = kJoinNatural;
  (yyval.table)->join->left = (yyvsp[-3].table);
  (yyval.table)->join->right = (yyvsp[0].table);
}
#line 5522 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 341:
#line 1446 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (JoinType)(yyvsp[-4].join_type);
  (yyval.table)->join->left = (yyvsp[-5].table);
  (yyval.table)->join->right = (yyvsp[-2].table);
  (yyval.table)->join->condition = (yyvsp[0].expr);
}
#line 5535 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 342:
#line 1454 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.table) = new TableRef(kTableJoin);
  (yyval.table)->join = new JoinDefinition();
  (yyval.table)->join->type = (yyvsp[-6].join_type);
  (yyval.table)->join->left = (yyvsp[-7].table);
  (yyval.table)->join->right = (yyvsp[-4].table);
  (yyval.table)->join->namedColumns = (yyvsp[-1].str_vec);
}
#line 5548 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 343:
#line 1463 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.join_type) = kJoinInner; }
#line 5554 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 344:
#line 1464 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.join_type) = kJoinLeft; }
#line 5560 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 345:
#line 1465 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.join_type) = kJoinLeft; }
#line 5566 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 346:
#line 1466 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.join_type) = kJoinRight; }
#line 5572 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 347:
#line 1467 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.join_type) = kJoinRight; }
#line 5578 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 348:
#line 1468 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.join_type) = kJoinFull; }
#line 5584 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 349:
#line 1469 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.join_type) = kJoinFull; }
#line 5590 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 350:
#line 1470 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.join_type) = kJoinFull; }
#line 5596 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 351:
#line 1471 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.join_type) = kJoinCross; }
#line 5602 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 352:
#line 1472 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.join_type) = kJoinInner; }
#line 5608 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 356:
#line 1483 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyval.str_vec) = new std::vector<char*>();
  (yyval.str_vec)->push_back((yyvsp[0].sval));
}
#line 5617 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 357:
#line 1487 "bison_parser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].str_vec)->push_back((yyvsp[0].sval));
  (yyval.str_vec) = (yyvsp[-2].str_vec);
}
#line 5626 "bison_parser.cpp" /* yacc.c:1646  */
    break;


#line 5630 "bison_parser.cpp" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, result, scanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, result, scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, result, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
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
                  yystos[*yyssp], yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1493 "bison_parser.y" /* yacc.c:1906  */


/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */

    // clang-format on
