// clang-format off
%{

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
%}
// clang-format on
/*********************************
 ** Section 2: Bison Parser Declarations
 *********************************/

// Specify code that is included in the generated .h and .c files
// clang-format off

// Bison 3.x emits `int yynerrs = 0;` in yyparse() and only ever
// increments it (`++yynerrs`), never reading the value. GCC's
// -Wunused-but-set-variable (enabled by -Wall on g++ 10+) considers
// increment-only as "set but not used" and fires on the declaration.
// Silence it locally so downstream consumers can build with -Wall
// without per-file -Wno-* flags. %code top injects at the very top
// of the generated .cpp (not the .h), so the suppression scope is
// exactly bison_parser.cpp.
%code top {
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
}

%code requires {
// %code requires block

#include "../SQLParserResult.h"
#include "../sql/statements.h"
#include "parser_typedef.h"

// Carries the two conditions of an Oracle hierarchical query out of one
// grammar rule, so START WITH and CONNECT BY can be given in either order.
// Trivially copyable, which a %union member has to be.
struct HierarchicalClause {
  hsql::Expr* startWith;
  hsql::Expr* connectBy;
};

// A query expression body while its set operations are collected: the select
// that heads the tree, the select whose set operations take the next member,
// and whether the body is one parenthesized query expression. Trivially
// copyable, which a %union member has to be.
struct QueryBody {
  hsql::SelectStatement* head;
  hsql::SelectStatement* open;
  bool parenthesized;
};

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
}

// Define the names of the created files (defined in Makefile)
// %output  "bison_parser.cpp"
// %defines "bison_parser.h"

// Raise error on shift/reduce conflict, i.e., when bison's one-token lookahead cannot decide on a single next state.
// Without this line, only a warning is printed. The line raises an error when the expected number of conflicts (0)
// does not occur.
%expect 0

// Tell bison to create a reentrant parser.
%define api.pure full

// Prefix the parser
%define api.prefix {hsql_}
%define api.token.prefix {SQL_}

%define parse.error verbose
%locations

%initial-action {
  // Initialize
  @$.first_column = 0;
  @$.last_column = 0;
  @$.first_line = 0;
  @$.last_line = 0;
  @$.total_column = 0;
  @$.string_length = 0;
};


// Define additional parameters for yylex (http://www.gnu.org/software/bison/manual/html_node/Pure-Calling.html)
%lex-param   { yyscan_t scanner }

// Define additional parameters for yyparse
%parse-param { hsql::SQLParserResult* result }
%parse-param { yyscan_t scanner }

/*********************************
 ** Define all data-types (http://www.gnu.org/software/bison/manual/html_node/Union-Decl.html)
 *********************************/
%union {
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
  HierarchicalClause hierarchical_t;
  QueryBody query_body_t;
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
}

/*********************************
 ** Destructor symbols
 *********************************/

%destructor { } <fval> <ival> <bval> <join_type> <order_type> <datetime_field> <column_type_t> <column_constraint_t> <import_type_t> <lock_mode_t> <lock_wait_policy_t> <frame_type> <null_ordering_t>
%destructor {
  free($$.name);
  free($$.schema);
} <table_name>
%destructor {
  delete $$.startWith;
  delete $$.connectBy;
} <hierarchical_t>
%destructor { delete $$.head; } <query_body_t>
%destructor {
  if ($$) {
    for (auto ptr : *($$)) {
      free(ptr);
    }
  }
  delete ($$);
} <str_vec>
%destructor { free($$); } <sval>
%destructor {
  if ($$) {
    for (auto ptr : *($$)) {
      delete ptr;
    }
  }
  delete ($$);
} <table_vec> <table_element_vec> <update_vec> <expr_vec> <order_vec> <stmt_vec> <with_description_vec> <locking_clause_vec>
%destructor {
  free($$->second);
  delete ($$);
} <csv_option_t>
%destructor {
  // ColumnConstraints owns heap-allocated members (constraints, references).
  // The happy path transfers those pointers into a ColumnDefinition and then
  // deletes the wrapper; bison only reaches this destructor on parse error,
  // when the inner pointers are still owned by the wrapper and must be
  // released here to avoid leaks.
  if ($$) {
    delete $$->constraints;
    if ($$->references) {
      for (auto ptr : *($$->references)) {
        delete ptr;
      }
    }
    delete $$->references;
  }
  delete ($$);
} <column_constraints_t>
%destructor { delete ($$); } <*>


/*********************************
 ** Token Definition
 *********************************/
%token <sval> IDENTIFIER STRING
%token <fval> FLOATVAL
%token <ival> INTVAL
%token <ival> DOLLAR_PARAM
%token <sval> NAMED_PARAM

/* SQL Keywords */
%token DEALLOCATE PARAMETERS INTERSECT TEMPORARY TIMESTAMP
%token DISTINCT NVARCHAR RESTRICT TRUNCATE ANALYZE BETWEEN
%token CASCADE COLUMNS CONTROL DEFAULT EXECUTE EXPLAIN ENCODING
%token INTEGER NATURAL PREPARE SCHEMAS CHARACTER_VARYING REAL DECIMAL SMALLINT BIGINT
%token SPATIAL VARCHAR VIRTUAL DESCRIBE BEFORE COLUMN CREATE DELETE DIRECT
%token DOUBLE ESCAPE EXCEPT EXISTS EXTRACT CAST FORMAT GLOBAL HAVING IMPORT
%token INSERT ISNULL OFFSET RENAME SCHEMA SELECT SORTED
%token TABLES UNLOAD UPDATE VALUES AFTER ALTER CROSS
%token DELTA FLOAT GROUP INDEX INNER LIMIT LOCAL MERGE MINUS ORDER OVER
%token OUTER RIGHT TABLE UNION USING WHERE CALL CASE CHAR COPY DATE DATETIME
%token DESC DROP ELSE FILE FROM FULL HASH HINT INTO JOIN
%token LEFT LIKE LOAD LONG NULL PARTITION PLAN SHOW TEXT THEN TIME
%token VIEW WHEN WITH ADD ALL AND ASC END FOR INT
%token NOT OFF SET TOP AS BY IF IN IS OF ON OR TO NO
%token ARRAY CONCAT ILIKE SECOND MINUTE HOUR DAY MONTH YEAR
%token SECONDS MINUTES HOURS DAYS MONTHS YEARS INTERVAL
%token TRUE FALSE BOOLEAN
%token TRANSACTION BEGIN COMMIT ROLLBACK
%token NOWAIT SKIP LOCKED SHARE
%token RANGE ROWS GROUPS UNBOUNDED FOLLOWING PRECEDING CURRENT_ROW
%token CURRENT_DATE CURRENT_TIME CURRENT_TIMESTAMP
%token FETCH NEXT ONLY
%token UNIQUE PRIMARY FOREIGN KEY REFERENCES
%token OUTERJOIN PRIOR ODBC_OJ NEXT_VALUE_FOR
// Lexed as whole phrases, so START, CONNECT and WITHIN stay usable as names.
%token START_WITH CONNECT_BY CONNECT_BY_NOCYCLE WITHIN_GROUP

/*********************************
 ** Non-Terminal types (http://www.gnu.org/software/bison/manual/html_node/Type-Decl.html)
 *********************************/
%type <stmt_vec>               statement_list
%type <statement>              statement preparable_statement
%type <exec_stmt>              execute_statement odbc_call_statement
%type <transaction_stmt>       transaction_statement
%type <prep_stmt>              prepare_statement
%type <select_stmt>            query_expression query_expression_parens query_primary subquery
%type <query_body_t>           query_expression_body
%type <import_stmt>            import_statement
%type <export_stmt>            export_statement
%type <create_stmt>            create_statement
%type <insert_stmt>            insert_statement
%type <delete_stmt>            delete_statement truncate_statement
%type <update_stmt>            update_statement
%type <drop_stmt>              drop_statement
%type <alter_stmt>             alter_statement
%type <show_stmt>              show_statement
%type <table_name>             table_name
%type <sval>                   opt_index_name
%type <sval>                   file_path prepare_target_query nonreserved_keyword name_or_keyword name_after_qualifier
%type <sval>                   qualified_name_or_keyword
%type <frame_description>      opt_frame_clause
%type <frame_bound>            frame_bound
%type <frame_type>             frame_type
%type <window_description>     opt_window
%type <bval>                   opt_not_exists opt_exists opt_distinct opt_all
%type <ival_pair>              opt_decimal_specification
%type <ival>                   opt_time_precision
%type <join_type>              opt_join_type
%type <table>                  opt_from_clause from_clause table_ref table_ref_atomic table_ref_name nonjoin_table_ref_atomic
%type <table>                  join_clause table_ref_name_no_alias
%type <expr>                   expr operand scalar_expr unary_expr binary_expr logic_expr exists_expr extract_expr cast_expr
%type <expr>                   function_expr between_expr expr_alias param_expr next_value_expr opt_odbc_return
%type <expr>                   table_function_expr
%type <expr>                   column_name literal int_literal num_literal string_literal bool_literal date_literal interval_literal
%type <expr>                   time_literal timestamp_literal
%type <expr>                   comp_expr opt_where join_condition opt_having case_expr case_list in_expr hint
%type <hierarchical_t>         opt_hierarchical_clause
%type <expr>                   array_expr array_index null_literal extended_literal casted_extended_literal
%type <limit>                  opt_limit opt_top
%type <order>                  order_desc
%type <order_type>             opt_order_type
%type <null_ordering_t>        opt_null_ordering
%type <datetime_field>         datetime_field datetime_field_plural duration_field interval_range
%type <column_t>               column_def
%type <table_element_t>        table_elem
%type <column_type_t>          column_type
%type <references_spec_t>      references_spec
%type <table_constraint_t>     table_constraint
%type <update_t>               update_clause
%type <locking_t>              locking_clause
%type <group_t>                opt_group
%type <alias_t>                opt_table_alias table_alias opt_alias alias
%type <with_description_t>     with_description
%type <set_operator_t>         set_operator set_type
%type <column_constraint_t>    column_constraint
%type <column_constraints_t>   opt_column_constraints column_constraints
%type <alter_action_t>         alter_action
%type <drop_action_t>          drop_action
%type <lock_wait_policy_t>     opt_row_lock_policy
%type <lock_mode_t>            row_lock_mode

// ImportType is used for compatibility reasons
%type <import_type_t>          file_type
%type <import_export_option_t> opt_import_export_options import_export_options
%type <csv_option_t>           csv_option

%type <str_vec>                ident_commalist opt_column_list
%type <expr_vec>               expr_list select_list opt_extended_literal_list extended_literal_list hint_list opt_hints opt_partition
%type <expr_vec>               row_expr_list table_value_row_list opt_call_args in_row_list
%type <table_vec>              table_ref_commalist
%type <order_vec>              opt_order order_list opt_within_group
%type <with_description_vec>   with_clause with_description_list
%type <update_vec>             update_clause_commalist
%type <table_element_vec>      table_elem_commalist
%type <locking_clause_vec>     opt_locking_clause_list opt_locking_clause

/******************************
 ** Token Precedence and Associativity
 ** Precedence: lowest to highest
 ******************************/
%left     OR
%left     AND
%right    NOT
%nonassoc '=' EQUALS NOTEQUALS LIKE ILIKE
%nonassoc '<' '>' LESS GREATER LESSEQ GREATEREQ

%nonassoc NOTNULL
%nonassoc ISNULL
%nonassoc IS        /* sets precedence for IS NULL, etc */
%left     '+' '-'
%left     '*' '/' '%'
%left     '^'
%left     CONCAT

/* Unary Operators */
%right    UMINUS
%right    PRIOR
%left     '[' ']'
%precedence SUBQUERY_AS_EXPR
%left     '(' ')'
%left     '.'
%left     JOIN
%%
/*********************************
 ** Section 3: Grammar Definition
 *********************************/

// Defines our general input.
input : statement_list opt_semicolon {
  for (SQLStatement* stmt : *$1) {
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
    delete $1;
  };

// clang-format on
statement_list : statement {
  $1->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  $$ = new std::vector<SQLStatement*>();
  $$->push_back($1);
}
| statement_list ';' statement {
  $3->stringLength = yylloc.string_length;
  yylloc.string_length = 0;
  $1->push_back($3);
  $$ = $1;
};

statement : prepare_statement opt_hints {
  $$ = $1;
  $$->hints = $2;
}
| preparable_statement opt_hints {
  $$ = $1;
  $$->hints = $2;
}
| show_statement { $$ = $1; }
| import_statement { $$ = $1; }
| export_statement { $$ = $1; };

preparable_statement : query_expression { $$ = $1; }
| create_statement { $$ = $1; }
| insert_statement { $$ = $1; }
| delete_statement { $$ = $1; }
| truncate_statement { $$ = $1; }
| update_statement { $$ = $1; }
| drop_statement { $$ = $1; }
| alter_statement { $$ = $1; }
| execute_statement { $$ = $1; }
| odbc_call_statement { $$ = $1; }
| transaction_statement { $$ = $1; };

/******************************
 * Hints
 ******************************/

opt_hints : WITH HINT '(' hint_list ')' { $$ = $4; }
| /* empty */ { $$ = nullptr; };

hint_list : hint {
  $$ = new std::vector<Expr*>();
  $$->push_back($1);
}
| hint_list ',' hint {
  $1->push_back($3);
  $$ = $1;
};

hint : IDENTIFIER {
  $$ = Expr::make(kExprHint);
  $$->name = $1;
}
| IDENTIFIER '(' extended_literal_list ')' {
  $$ = Expr::make(kExprHint);
  $$->name = $1;
  $$->exprList = $3;
};

/******************************
 * Transaction Statement
 ******************************/

transaction_statement : BEGIN opt_transaction_keyword { $$ = new TransactionStatement(kBeginTransaction); }
| ROLLBACK opt_transaction_keyword { $$ = new TransactionStatement(kRollbackTransaction); }
| COMMIT opt_transaction_keyword { $$ = new TransactionStatement(kCommitTransaction); };

opt_transaction_keyword : TRANSACTION | /* empty */
    ;

/******************************
 * Prepared Statement
 ******************************/
prepare_statement : PREPARE IDENTIFIER FROM prepare_target_query {
  $$ = new PrepareStatement();
  $$->name = $2;
  $$->query = $4;
};

prepare_target_query : STRING;

// ODBC procedure call escape, e.g. {? = call some_proc(?, ?)} or
// {call some_proc(?)}. The optional "? =" is the driver's return value marker
// and carries nothing the statement itself needs, so both forms produce the
// same ExecuteStatement an EXECUTE would.
odbc_call_statement : '{' opt_odbc_return CALL table_name opt_call_args '}' {
  $$ = new ExecuteStatement();
  $$->returnValue = $2;
  $$->name = qualifiedName($4);
  $$->parameters = $5;
};

// The marker has to go through param_expr: consumed as a bare '?' it would not
// reach the parameter list, and ODBC counts it as the first bound parameter.
opt_odbc_return : param_expr '=' { $$ = $1; }
| /* empty */ { $$ = nullptr; };

opt_call_args : '(' opt_extended_literal_list ')' { $$ = $2; }
| /* empty */ { $$ = nullptr; };

execute_statement : EXECUTE IDENTIFIER opt_call_args {
  $$ = new ExecuteStatement();
  $$->name = $2;
  $$->parameters = $3;
};

/******************************
 * Import Statement
 * IMPORT FROM TBL FILE 'test/students.tbl' INTO students
 * COPY students FROM 'test/students.tbl'
 * COPY students FROM 'test/students.tbl' WITH (FORMAT TBL, ENCODING 'Dictionary')
 ******************************/
import_statement : IMPORT FROM file_type FILE file_path INTO table_name {
  $$ = new ImportStatement($3);
  $$->filePath = $5;
  $$->schema = $7.schema;
  $$->tableName = $7.name;
}
| COPY table_name FROM file_path opt_import_export_options opt_where {
  $$ = new ImportStatement($5->format);
  $$->filePath = $4;
  $$->schema = $2.schema;
  $$->tableName = $2.name;
  $$->whereClause = $6;
  if ($5->encoding) {
    $$->encoding = $5->encoding;
    $5->encoding = nullptr;
  }
  if ($5->csv_options) {
    $$->csv_options = $5->csv_options;
    $5->csv_options = nullptr;
  }
  delete $5;
};

file_type : IDENTIFIER {
  if (strcasecmp($1, "csv") == 0) {
    $$ = kImportCSV;
  } else if (strcasecmp($1, "tbl") == 0) {
    $$ = kImportTbl;
  } else if (strcasecmp($1, "binary") == 0 || strcasecmp($1, "bin") == 0) {
    $$ = kImportBinary;
  } else {
    free($1);
    yyerror(&yyloc, result, scanner, "File type is unknown.");
    YYERROR;
  }
  free($1);
};

file_path : STRING { $$ = $1; };

opt_import_export_options : WITH '(' import_export_options ')' { $$ = $3; }
| '(' import_export_options ')' { $$ = $2; }
| /* empty */ { $$ = new ImportExportOptions{}; };

import_export_options : import_export_options ',' FORMAT file_type {
  if ($1->format != kImportAuto) {
    delete $1;
    yyerror(&yyloc, result, scanner, "File type must only be provided once.");
    YYERROR;
  }
  if ($1->csv_options && $4 != kImportCSV && $4 != kImportAuto) {
    delete $1;
    yyerror(&yyloc, result, scanner, "CSV options (DELIMITER, NULL, QUOTE) are only allowed for CSV files.");
    YYERROR;
  }
  $1->format = $4;
  $$ = $1;
}
| FORMAT file_type {
  $$ = new ImportExportOptions{};
  $$->format = $2;
}
| import_export_options ',' ENCODING STRING {
  if ($1->encoding) {
    delete $1;
    free($4);
    yyerror(&yyloc, result, scanner, "Encoding type must only be provided once.");
    YYERROR;
  }
  $1->encoding = $4;
  $$ = $1;
}
| ENCODING STRING {
  $$ = new ImportExportOptions{};
  $$->encoding = $2;
}
| import_export_options ',' csv_option {
  if ($1->format != kImportAuto && $1->format != kImportCSV) {
    delete $1;
    free($3->second);
    delete $3;
    yyerror(&yyloc, result, scanner, "CSV options (DELIMITER, NULL, QUOTE) are only allowed for CSV files.");
    YYERROR;
  }

  if ($1->csv_options == nullptr) {
    $1->csv_options = new CsvOptions{};
  }

  if (!$1->csv_options->accept_csv_option($3)) {
    free($3->second);
    delete $3;
    delete $1;
    yyerror(&yyloc, result, scanner, "CSV options (DELIMITER, NULL, QUOTE) cannot be provided more than once.");
    YYERROR;
  }

  delete $3;
  $$ = $1;
}
| csv_option {
  $$ = new ImportExportOptions{};
  $$->csv_options = new CsvOptions{};
  $$->csv_options->accept_csv_option($1);

  delete $1;
}

csv_option : IDENTIFIER STRING {
  if (strcasecmp($1, "DELIMITER") == 0) {
    $$ = new std::pair<CsvOptionType, char*>(CsvOptionType::Delimiter, $2);
  } else if (strcasecmp($1, "QUOTE") == 0) {
    $$ = new std::pair<CsvOptionType, char*>(CsvOptionType::Quote, $2);
  } else {
    free($1);
    free($2);
    yyerror(&yyloc, result, scanner, "Unknown CSV option.");
    YYERROR;
  }
  free($1);
}
| NULL STRING { $$ = new std::pair<CsvOptionType, char*>(CsvOptionType::Null, $2); }

/******************************
 * Export Statement
 * COPY students TO 'test/students.tbl'
 * COPY students TO 'test/students.tbl' WITH (FORMAT BINARY, ENCODING 'Dictionary')
 ******************************/
export_statement : COPY table_name TO file_path opt_import_export_options {
  $$ = new ExportStatement($5->format);
  $$->filePath = $4;
  $$->schema = $2.schema;
  $$->tableName = $2.name;
  if ($5->encoding) {
    $$->encoding = $5->encoding;
    $5->encoding = nullptr;
  }
  if ($5->csv_options) {
    $$->csv_options = $5->csv_options;
    $5->csv_options = nullptr;
  }
  delete $5;
}
| COPY subquery TO file_path opt_import_export_options {
  $$ = new ExportStatement($5->format);
  $$->filePath = $4;
  $$->select = $2;
  if ($5->encoding) {
    $$->encoding = $5->encoding;
    $5->encoding = nullptr;
  }
  if ($5->csv_options) {
    $$->csv_options = $5->csv_options;
    $5->csv_options = nullptr;
  }
  delete $5;
};

/******************************
 * Show Statement
 * SHOW TABLES;
 ******************************/

show_statement : SHOW TABLES { $$ = new ShowStatement(kShowTables); }
| SHOW COLUMNS table_name {
  $$ = new ShowStatement(kShowColumns);
  $$->schema = $3.schema;
  $$->name = $3.name;
}
| DESCRIBE table_name {
  $$ = new ShowStatement(kShowColumns);
  $$->schema = $2.schema;
  $$->name = $2.name;
};

/******************************
 * Create Statement
 * CREATE TABLE students (name TEXT, student_number INTEGER, city TEXT, grade DOUBLE)
 * CREATE TABLE students FROM TBL FILE 'test/students.tbl'
 ******************************/
create_statement : CREATE TABLE opt_not_exists table_name FROM IDENTIFIER FILE file_path {
  $$ = new CreateStatement(kCreateTableFromTbl);
  $$->ifNotExists = $3;
  $$->schema = $4.schema;
  $$->tableName = $4.name;
  if (strcasecmp($6, "tbl") != 0) {
    free($6);
    free($8);
    delete $$;
    yyerror(&yyloc, result, scanner, "File type is unknown.");
    YYERROR;
  }
  free($6);
  $$->filePath = $8;
}
| CREATE TABLE opt_not_exists table_name '(' table_elem_commalist ')' {
  $$ = new CreateStatement(kCreateTable);
  $$->ifNotExists = $3;
  $$->schema = $4.schema;
  $$->tableName = $4.name;
  $$->setColumnDefsAndConstraints($6);
  delete $6;
  if (result->errorMsg()) {
    delete $$;
    YYERROR;
  }
}
| CREATE TABLE opt_not_exists table_name AS query_expression {
  $$ = new CreateStatement(kCreateTable);
  $$->ifNotExists = $3;
  $$->schema = $4.schema;
  $$->tableName = $4.name;
  $$->select = $6;
}
| CREATE INDEX opt_not_exists opt_index_name ON table_name '(' ident_commalist ')' {
  $$ = new CreateStatement(kCreateIndex);
  $$->indexName = $4;
  $$->ifNotExists = $3;
  $$->schema = $6.schema;
  $$->tableName = $6.name;
  $$->indexColumns = $8;
}
| CREATE VIEW opt_not_exists table_name opt_column_list AS query_expression {
  $$ = new CreateStatement(kCreateView);
  $$->ifNotExists = $3;
  $$->schema = $4.schema;
  $$->tableName = $4.name;
  $$->viewColumns = $5;
  $$->select = $7;
};

opt_not_exists : IF NOT EXISTS { $$ = true; }
| /* empty */ { $$ = false; };

table_elem_commalist : table_elem {
  $$ = new std::vector<TableElement*>();
  $$->push_back($1);
}
| table_elem_commalist ',' table_elem {
  $1->push_back($3);
  $$ = $1;
};

table_elem : column_def { $$ = $1; }
| table_constraint { $$ = $1; };

column_def : name_or_keyword column_type opt_column_constraints {
  $$ = new ColumnDefinition($1, $2, $3->constraints, $3->references);
  if (!$$->trySetNullableExplicit()) {
    yyerror(&yyloc, result, scanner, ("Conflicting nullability constraints for " + std::string{$1}).c_str());
  }
  delete $3;
};

column_type : BIGINT { $$ = ColumnType{DataType::BIGINT}; }
| BOOLEAN { $$ = ColumnType{DataType::BOOLEAN}; }
| CHAR '(' INTVAL ')' { $$ = ColumnType{DataType::CHAR, $3}; }
| CHARACTER_VARYING '(' INTVAL ')' { $$ = ColumnType{DataType::VARCHAR, $3}; }
| DATE { $$ = ColumnType{DataType::DATE}; }
| DATETIME { $$ = ColumnType{DataType::DATETIME}; }
| DECIMAL opt_decimal_specification {
  $$ = ColumnType{DataType::DECIMAL, 0, $2->first, $2->second};
  delete $2;
}
| DOUBLE { $$ = ColumnType{DataType::DOUBLE}; }
| FLOAT { $$ = ColumnType{DataType::FLOAT}; }
| INT { $$ = ColumnType{DataType::INT}; }
| INTEGER { $$ = ColumnType{DataType::INT}; }
| INTERVAL { $$ = ColumnType{DataType::INTERVAL}; }
| INTERVAL datetime_field { $$ = ColumnType{DataType::INTERVAL, 0, 0, 0, $2}; }
| INTERVAL interval_range { $$ = ColumnType{DataType::INTERVAL, 0, 0, 0, $2}; }
| LONG { $$ = ColumnType{DataType::LONG}; }
| REAL { $$ = ColumnType{DataType::REAL}; }
| SMALLINT { $$ = ColumnType{DataType::SMALLINT}; }
| TEXT { $$ = ColumnType{DataType::TEXT}; }
| TIME opt_time_precision { $$ = ColumnType{DataType::TIME, 0, $2}; }
| TIMESTAMP { $$ = ColumnType{DataType::DATETIME}; }
| VARCHAR '(' INTVAL ')' { $$ = ColumnType{DataType::VARCHAR, $3}; };

opt_time_precision : '(' INTVAL ')' { $$ = $2; }
| /* empty */ { $$ = 0; };

opt_decimal_specification : '(' INTVAL ',' INTVAL ')' { $$ = new std::pair<int64_t, int64_t>{$2, $4}; }
| '(' INTVAL ')' { $$ = new std::pair<int64_t, int64_t>{$2, 0}; }
| /* empty */ { $$ = new std::pair<int64_t, int64_t>{0, 0}; };

opt_column_constraints : column_constraints { $$ = $1; }
| /* empty */ { $$ = new ColumnConstraints(); };

column_constraints : column_constraint {
  $$ = new ColumnConstraints();
  $$->constraints->insert($1);
}
| column_constraints column_constraint {
  $1->constraints->insert($2);
  $$ = $1;
}
| references_spec {
  $$ = new ColumnConstraints();
  $$->constraints->insert(ConstraintType::ForeignKey);
  $$->references->emplace_back($1);
}
| column_constraints references_spec {
  // Multiple foreign keys for the same column could be possible, so we do not raise an error in that case.
  // Think of foreign keys referenced on multiple levels (returned item references sold item references items).
  $1->constraints->insert(ConstraintType::ForeignKey);
  $1->references->emplace_back($2);
  $$ = $1;
};

column_constraint : PRIMARY KEY { $$ = ConstraintType::PrimaryKey; }
| UNIQUE { $$ = ConstraintType::Unique; }
| NULL { $$ = ConstraintType::Null; }
| NOT NULL { $$ = ConstraintType::NotNull; };

table_constraint : PRIMARY KEY '(' ident_commalist ')' { $$ = new TableConstraint(ConstraintType::PrimaryKey, $4); }
| UNIQUE '(' ident_commalist ')' { $$ = new TableConstraint(ConstraintType::Unique, $3); }
| FOREIGN KEY '(' ident_commalist ')' references_spec { $$ = new ForeignKeyConstraint($4, $6); };

references_spec : REFERENCES table_name opt_column_list { $$ = new ReferencesSpecification($2.schema, $2.name, $3); };

/******************************
 * Drop Statement
 * DROP TABLE students;
 * DEALLOCATE PREPARE stmt;
 ******************************/

drop_statement : DROP TABLE opt_exists table_name {
  $$ = new DropStatement(kDropTable);
  $$->ifExists = $3;
  $$->schema = $4.schema;
  $$->name = $4.name;
}
| DROP VIEW opt_exists table_name {
  $$ = new DropStatement(kDropView);
  $$->ifExists = $3;
  $$->schema = $4.schema;
  $$->name = $4.name;
}
| DEALLOCATE PREPARE IDENTIFIER {
  $$ = new DropStatement(kDropPreparedStatement);
  $$->ifExists = false;
  $$->name = $3;
}

| DROP INDEX opt_exists name_or_keyword {
  $$ = new DropStatement(kDropIndex);
  $$->ifExists = $3;
  $$->indexName = $4;
};

opt_exists : IF EXISTS { $$ = true; }
| /* empty */ { $$ = false; };

/******************************
 * ALTER Statement
 * ALTER TABLE students DROP COLUMN name;
 ******************************/

alter_statement : ALTER TABLE opt_exists table_name alter_action {
  $$ = new AlterStatement($4.name, $5);
  $$->ifTableExists = $3;
  $$->schema = $4.schema;
};

alter_action : drop_action { $$ = $1; }

drop_action : DROP COLUMN opt_exists name_or_keyword {
  $$ = new DropColumnAction($4);
  $$->ifExists = $3;
};

/******************************
 * Delete Statement / Truncate statement
 * DELETE FROM students WHERE grade > 3.0
 * DELETE FROM students <=> TRUNCATE students
 ******************************/
delete_statement : DELETE FROM table_name opt_where {
  $$ = new DeleteStatement();
  $$->schema = $3.schema;
  $$->tableName = $3.name;
  $$->expr = $4;
};

truncate_statement : TRUNCATE table_name {
  $$ = new DeleteStatement();
  $$->schema = $2.schema;
  $$->tableName = $2.name;
};

/******************************
 * Insert Statement
 * INSERT INTO students VALUES ('Max', 1112233, 'Musterhausen', 2.3)
 * INSERT INTO employees SELECT * FROM stundents
 ******************************/
insert_statement : INSERT INTO table_name opt_column_list VALUES '(' extended_literal_list ')' {
  $$ = new InsertStatement(kInsertValues);
  $$->schema = $3.schema;
  $$->tableName = $3.name;
  $$->columns = $4;
  $$->values = $7;
}
// The column list is written out instead of being optional, because an empty
// rule in front of the query could not be told from the parenthesis opening a
// parenthesized query expression.
| INSERT INTO table_name query_expression {
  $$ = new InsertStatement(kInsertSelect);
  $$->schema = $3.schema;
  $$->tableName = $3.name;
  $$->select = $4;
}
| INSERT INTO table_name '(' ident_commalist ')' query_expression {
  $$ = new InsertStatement(kInsertSelect);
  $$->schema = $3.schema;
  $$->tableName = $3.name;
  $$->columns = $5;
  $$->select = $7;
};

opt_column_list : '(' ident_commalist ')' { $$ = $2; }
| /* empty */ { $$ = nullptr; };

/******************************
 * Update Statement
 * UPDATE students SET grade = 1.3, name='Felix Fürstenberg' WHERE name = 'Max Mustermann';
 ******************************/

update_statement : UPDATE table_ref_name_no_alias SET update_clause_commalist opt_where {
  $$ = new UpdateStatement();
  $$->table = $2;
  $$->updates = $4;
  $$->where = $5;
};

update_clause_commalist : update_clause {
  $$ = new std::vector<UpdateClause*>();
  $$->push_back($1);
}
| update_clause_commalist ',' update_clause {
  $1->push_back($3);
  $$ = $1;
};

update_clause : name_or_keyword '=' expr {
  $$ = new UpdateClause();
  $$->column = $1;
  $$->value = $3;
};

/******************************
 * Select Statement
 ******************************/

// A query expression as the SQL standard shapes it: an optional WITH clause,
// a body of set operations and the clauses that order, limit and lock the
// result. The WITH clause is written out instead of being optional, because
// an empty rule in front of the body could not be told from the parenthesis
// opening an expression.
query_expression : query_expression_body opt_order opt_limit opt_locking_clause {
  $$ = $1.head;
  if (const char* error = attachQueryClauses($1, nullptr, $2, $3, $4)) {
    delete $$;
    deleteQueryClauses(nullptr, $2, $3, $4);
    yyerror(&yyloc, result, scanner, error);
    YYERROR;
  }
}
| with_clause query_expression_body opt_order opt_limit opt_locking_clause {
  $$ = $2.head;
  if (const char* error = attachQueryClauses($2, $1, $3, $4, $5)) {
    delete $$;
    deleteQueryClauses($1, $3, $4, $5);
    yyerror(&yyloc, result, scanner, error);
    YYERROR;
  }
};

// Set operations are written left to right while the tree chains them to the
// right: the set operations of a select hold the member that follows it, the
// member holding the one after it. The body therefore carries the select that
// takes the next member. An unparenthesized member takes the members after
// it; a parenthesized member is a query expression of its own and takes none,
// so the next member goes beside it and a parenthesized group stays together
// with its ORDER BY and LIMIT. Only a parenthesized body can carry a WITH
// clause here, and the tree would hang the members that follow under it,
// while in SQL it covers the parentheses alone; such a set operation is
// rejected rather than read with the wider scope.
query_expression_body : query_primary { $$ = QueryBody{$1, $1, false}; }
// A parenthesized query expression followed by ')' is also the inner part of
// a nested parenthesization. Both read the same, so the lower precedence lets
// the parenthesis be shifted instead of reducing the body here.
| query_expression_parens %prec SUBQUERY_AS_EXPR { $$ = QueryBody{$1, $1, true}; }
| query_expression_body set_operator query_primary {
  if ($1.head->withDescriptions) {
    delete $1.head;
    delete $2;
    delete $3;
    yyerror(&yyloc, result, scanner, "A WITH clause in parentheses cannot be followed by a set operation.");
    YYERROR;
  }
  $2->nestedSelectStatement = $3;
  addSetOperation($1.open, $2);
  $$ = QueryBody{$1.head, $3, false};
}
| query_expression_body set_operator query_expression_parens {
  if ($1.head->withDescriptions) {
    delete $1.head;
    delete $2;
    delete $3;
    yyerror(&yyloc, result, scanner, "A WITH clause in parentheses cannot be followed by a set operation.");
    YYERROR;
  }
  $2->nestedSelectStatement = $3;
  addSetOperation($1.open, $2);
  $$ = QueryBody{$1.head, $1.open, false};
};

query_expression_parens : '(' query_expression ')' { $$ = $2; }
| '(' query_expression_parens ')' { $$ = $2; };

// A query expression in parentheses where an expression operand is possible,
// e.g. a scalar subquery or the right side of IN. The parentheses around a
// nested one, ((SELECT 1)), are either those of the subquery or those of an
// expression; both read the same, so the lower precedence lets ')' be shifted.
subquery : query_expression_parens %prec SUBQUERY_AS_EXPR;

set_operator : set_type opt_all {
  $$ = $1;
  $$->isAll = $2;
};

set_type : UNION {
  $$ = new SetOperation();
  $$->setType = SetType::kSetUnion;
}
| INTERSECT {
  $$ = new SetOperation();
  $$->setType = SetType::kSetIntersect;
}
| EXCEPT {
  $$ = new SetOperation();
  $$->setType = SetType::kSetExcept;
};

opt_all : ALL { $$ = true; }
| /* empty */ { $$ = false; };

// Oracle hierarchical query clauses. Oracle accepts START WITH and CONNECT BY
// in either order and either may be omitted, so both are produced by one rule.
opt_hierarchical_clause : START_WITH expr connect_by expr { $$ = HierarchicalClause{$2, $4}; }
| connect_by expr START_WITH expr { $$ = HierarchicalClause{$4, $2}; }
| START_WITH expr { $$ = HierarchicalClause{$2, nullptr}; }
| connect_by expr { $$ = HierarchicalClause{nullptr, $2}; }
| /* empty */ { $$ = HierarchicalClause{nullptr, nullptr}; };

// NOCYCLE only tells Oracle to tolerate loops in the data; it carries nothing
// the tree needs, so both spellings reduce to the same clause.
connect_by : CONNECT_BY | CONNECT_BY_NOCYCLE;

query_primary : SELECT opt_top opt_distinct select_list opt_from_clause opt_where opt_hierarchical_clause opt_group {
  $$ = new SelectStatement();
  $$->limit = $2;
  $$->selectDistinct = $3;
  $$->selectList = $4;
  $$->fromTable = $5;
  $$->whereClause = $6;
  $$->startWith = $7.startWith;
  $$->connectBy = $7.connectBy;
  $$->groupBy = $8;
};

opt_distinct : DISTINCT { $$ = true; }
| /* empty */ { $$ = false; };

select_list : expr_list;

opt_from_clause : from_clause { $$ = $1; }
| /* empty */ { $$ = nullptr; };

from_clause : FROM table_ref { $$ = $2; };

opt_where : WHERE expr { $$ = $2; }
| /* empty */ { $$ = nullptr; };

opt_group : GROUP BY expr_list opt_having {
  $$ = new GroupByDescription();
  $$->columns = $3;
  $$->having = $4;
}
| /* empty */ { $$ = nullptr; };

opt_having : HAVING expr { $$ = $2; }
| /* empty */ { $$ = nullptr; };

// Ordered-set aggregate, e.g. LISTAGG(x, ', ') WITHIN GROUP (ORDER BY y). The
// sort order belongs to the aggregate itself, so it is kept apart from the
// window description an OVER clause would produce.
opt_within_group : WITHIN_GROUP '(' ORDER BY order_list ')' { $$ = $5; }
| /* empty */ { $$ = nullptr; };

opt_order : ORDER BY order_list { $$ = $3; }
| /* empty */ { $$ = nullptr; };

order_list : order_desc {
  $$ = new std::vector<OrderDescription*>();
  $$->push_back($1);
}
| order_list ',' order_desc {
  $1->push_back($3);
  $$ = $1;
};

order_desc : expr opt_order_type opt_null_ordering { $$ = new OrderDescription($2, $1, $3); };

opt_order_type : ASC { $$ = kOrderAsc; }
| DESC { $$ = kOrderDesc; }
| /* empty */ { $$ = kOrderAsc; };

opt_null_ordering : /* empty */ { $$ = NullOrdering::Undefined; }
| IDENTIFIER IDENTIFIER {
  auto null_ordering = NullOrdering::Undefined;
  if (strcasecmp($1, "nulls") == 0) {
    if (strcasecmp($2, "first") == 0) {
      null_ordering = NullOrdering::First;
    } else if (strcasecmp($2, "last") == 0) {
      null_ordering = NullOrdering::Last;
    }
  }
  free($1);
  free($2);

  if (null_ordering == NullOrdering::Undefined) {
    yyerror(&yyloc, result, scanner, "Expected NULLS FIRST or NULLS LAST ordering.");
    YYERROR;
  }

  $$ = null_ordering;
};

// T-SQL requires the parentheses exactly when TOP takes an expression rather
// than a constant, e.g. TOP (?) or TOP (:limit). The bare form is literal-only:
// "TOP a" cannot be told apart from a select list starting with a column named
// a.
opt_top : TOP int_literal { $$ = new LimitDescription($2, nullptr); }
| TOP '(' expr ')' { $$ = new LimitDescription($3, nullptr); }
| /* empty */ { $$ = nullptr; };

opt_limit : LIMIT expr { $$ = new LimitDescription($2, nullptr); }
| OFFSET expr { $$ = new LimitDescription(nullptr, $2); }
| LIMIT expr OFFSET expr { $$ = new LimitDescription($2, $4); }
| LIMIT ALL { $$ = new LimitDescription(nullptr, nullptr); }
| LIMIT ALL OFFSET expr { $$ = new LimitDescription(nullptr, $4); }
| OFFSET expr ROWS FETCH NEXT expr ROWS ONLY { $$ = new LimitDescription($6, $2); }
| /* empty */ { $$ = nullptr; };

/******************************
 * Expressions
 ******************************/
expr_list : expr_alias {
  $$ = new std::vector<Expr*>();
  $$->push_back($1);
}
| expr_list ',' expr_alias {
  $1->push_back($3);
  $$ = $1;
};

// Literals, casted literals, and negative numbers/intervals are allowed for INSERT and EXECUTE statements or hints.
opt_extended_literal_list : extended_literal_list { $$ = $1; }
| /* empty */ { $$ = nullptr; };

extended_literal_list : casted_extended_literal {
  $$ = new std::vector<Expr*>();
  $$->push_back($1);
}
| extended_literal_list ',' casted_extended_literal {
  $1->push_back($3);
  $$ = $1;
};

casted_extended_literal : extended_literal | CAST '(' extended_literal AS column_type ')' {
  $$ = Expr::makeCast($3, $5);
};

extended_literal : literal { $$ = $1; }
| '-' num_literal { $$ = Expr::makeOpUnary(kOpUnaryMinus, $2); }
| '-' interval_literal { $$ = Expr::makeOpUnary(kOpUnaryMinus, $2); };

expr_alias : expr opt_alias {
  $$ = $1;
  if ($2) {
    $$->alias = $2->name;
    $2->name = nullptr;
    delete $2;
  }
};

expr : operand | between_expr | logic_expr | exists_expr | in_expr;

operand : '(' expr ')' { $$ = $2; }
| array_index | scalar_expr | unary_expr | binary_expr | case_expr | function_expr | extract_expr | cast_expr | next_value_expr |
    array_expr | subquery {
  $$ = Expr::makeSelect($1);
};

scalar_expr : column_name | literal
// Oracle outer join marker, e.g. WHERE a.id (+) = b.id. Only a column
// reference can carry it, which is where Oracle allows it; the marked side
// stays visible in the tree as a unary operator.
| column_name OUTERJOIN { $$ = Expr::makeOpUnary(kOpOuterJoin, $1); };

unary_expr : '-' operand { $$ = Expr::makeOpUnary(kOpUnaryMinus, $2); }
// Oracle's PRIOR, referring to the parent row inside CONNECT BY.
| PRIOR operand { $$ = Expr::makeOpUnary(kOpPrior, $2); }
| NOT operand { $$ = Expr::makeOpUnary(kOpNot, $2); }
| operand ISNULL { $$ = Expr::makeOpUnary(kOpIsNull, $1); }
| operand IS NULL { $$ = Expr::makeOpUnary(kOpIsNull, $1); }
| operand IS NOT NULL { $$ = Expr::makeOpUnary(kOpNot, Expr::makeOpUnary(kOpIsNull, $1)); };

binary_expr : comp_expr | operand '-' operand { $$ = Expr::makeOpBinary($1, kOpMinus, $3); }
| operand '+' operand { $$ = Expr::makeOpBinary($1, kOpPlus, $3); }
| operand '/' operand { $$ = Expr::makeOpBinary($1, kOpSlash, $3); }
| operand '*' operand { $$ = Expr::makeOpBinary($1, kOpAsterisk, $3); }
| operand '%' operand { $$ = Expr::makeOpBinary($1, kOpPercentage, $3); }
| operand '^' operand { $$ = Expr::makeOpBinary($1, kOpCaret, $3); }
| operand LIKE operand { $$ = Expr::makeOpBinary($1, kOpLike, $3); }
| operand NOT LIKE operand { $$ = Expr::makeOpBinary($1, kOpNotLike, $4); }
| operand ILIKE operand { $$ = Expr::makeOpBinary($1, kOpILike, $3); }
| operand CONCAT operand { $$ = Expr::makeOpBinary($1, kOpConcat, $3); };

logic_expr : expr AND expr { $$ = Expr::makeOpBinary($1, kOpAnd, $3); }
| expr OR expr { $$ = Expr::makeOpBinary($1, kOpOr, $3); };

in_expr : operand IN '(' expr_list ')' { $$ = Expr::makeInOperator($1, $4); }
| operand NOT IN '(' expr_list ')' { $$ = Expr::makeOpUnary(kOpNot, Expr::makeInOperator($1, $5)); }
| operand IN subquery { $$ = Expr::makeInOperator($1, $3); }
| operand NOT IN subquery { $$ = Expr::makeOpUnary(kOpNot, Expr::makeInOperator($1, $4)); }
// Row constructor on the left of IN, e.g. (a, b) IN (SELECT x, y FROM t). The
// tuple is represented as an array expression - Expr has no dedicated row type
// and this keeps every element in the tree instead of dropping any.
| '(' row_expr_list ')' IN subquery { $$ = Expr::makeInOperator(Expr::makeArray($2), $5); }
| '(' row_expr_list ')' NOT IN subquery { $$ = Expr::makeOpUnary(kOpNot, Expr::makeInOperator(Expr::makeArray($2), $6)); }
// The literal-list counterpart, e.g. (a, b) IN ((1, 2), (3, 4)).
| '(' row_expr_list ')' IN '(' in_row_list ')' { $$ = Expr::makeInOperator(Expr::makeArray($2), $6); }
| '(' row_expr_list ')' NOT IN '(' in_row_list ')' { $$ = Expr::makeOpUnary(kOpNot, Expr::makeInOperator(Expr::makeArray($2), $7)); };

// Each row of the list is an array expression, the same representation the row
// constructor on the left uses.
in_row_list : '(' row_expr_list ')' {
  $$ = new std::vector<Expr*>();
  $$->push_back(Expr::makeArray($2));
}
| in_row_list ',' '(' row_expr_list ')' {
  $1->push_back(Expr::makeArray($4));
  $$ = $1;
};

// Two or more elements: a single parenthesized expression stays a plain operand,
// so the grammar has no ambiguity to resolve between "(a)" and a one-element row.
row_expr_list : expr ',' expr {
  $$ = new std::vector<Expr*>();
  $$->push_back($1);
  $$->push_back($3);
}
| row_expr_list ',' expr {
  $1->push_back($3);
  $$ = $1;
};

// CASE grammar based on: flex & bison by John Levine
// https://www.safaribooksonline.com/library/view/flex-bison/9780596805418/ch04.html#id352665
case_expr : CASE expr case_list END { $$ = Expr::makeCase($2, $3, nullptr); }
| CASE expr case_list ELSE expr END { $$ = Expr::makeCase($2, $3, $5); }
| CASE case_list END { $$ = Expr::makeCase(nullptr, $2, nullptr); }
| CASE case_list ELSE expr END { $$ = Expr::makeCase(nullptr, $2, $4); };

case_list : WHEN expr THEN expr { $$ = Expr::makeCaseList(Expr::makeCaseListElement($2, $4)); }
| case_list WHEN expr THEN expr { $$ = Expr::caseListAppend($1, Expr::makeCaseListElement($3, $5)); };

exists_expr : EXISTS subquery { $$ = Expr::makeExists($2); }
| NOT EXISTS subquery { $$ = Expr::makeOpUnary(kOpNot, Expr::makeExists($3)); };

comp_expr : operand '=' operand { $$ = Expr::makeOpBinary($1, kOpEquals, $3); }
| operand EQUALS operand { $$ = Expr::makeOpBinary($1, kOpEquals, $3); }
| operand NOTEQUALS operand { $$ = Expr::makeOpBinary($1, kOpNotEquals, $3); }
| operand '<' operand { $$ = Expr::makeOpBinary($1, kOpLess, $3); }
| operand '>' operand { $$ = Expr::makeOpBinary($1, kOpGreater, $3); }
| operand LESSEQ operand { $$ = Expr::makeOpBinary($1, kOpLessEq, $3); }
| operand GREATEREQ operand { $$ = Expr::makeOpBinary($1, kOpGreaterEq, $3); };

// The bare call. Every call form is written once here, so OVER and WITHIN GROUP
// below apply to all of them, and a table reference can reuse the same shape.
table_function_expr : IDENTIFIER '(' ')' {
  $$ = Expr::makeFunctionRef($1, new std::vector<Expr*>(), false, nullptr);
}
| IDENTIFIER '(' opt_distinct expr_list ')' {
  $$ = Expr::makeFunctionRef($1, $4, $3, nullptr);
}
| IDENTIFIER '.' name_after_qualifier '(' ')' {
  $$ = Expr::makeFunctionRef($3, $1, new std::vector<Expr*>(), false, nullptr);
}
| IDENTIFIER '.' name_after_qualifier '(' opt_distinct expr_list ')' {
  $$ = Expr::makeFunctionRef($3, $1, $6, $5, nullptr);
}
// Dialect functions whose names collide with grammar keywords, e.g.
// ISNULL(a, 0), CHAR(10), FORMAT(x, '00'). See nonreserved_keyword.
| nonreserved_keyword '(' ')' {
  $$ = Expr::makeFunctionRef($1, new std::vector<Expr*>(), false, nullptr);
}
| nonreserved_keyword '(' opt_distinct expr_list ')' {
  $$ = Expr::makeFunctionRef($1, $4, $3, nullptr);
};

// A call in an expression, which may carry the ordered-set aggregate sort order
// and a window description. A table reference uses table_function_expr directly:
// a window function or an ordered-set aggregate is not a table.
function_expr : table_function_expr opt_within_group opt_window {
  $$ = $1;
  $$->withinGroupOrder = $2;
  $$->windowDescription = $3;
};

// Window function expressions, based on https://www.postgresql.org/docs/15/sql-expressions.html#SYNTAX-WINDOW-FUNCTIONS
// We do not support named windows, collations and exclusions (for simplicity) and filters (not part of the SQL standard).
opt_window : OVER '(' opt_partition opt_order opt_frame_clause ')' { $$ = new WindowDescription($3, $4, $5); }
| /* empty */ { $$ = nullptr; };

opt_partition : PARTITION BY expr_list { $$ = $3; }
| /* empty */ { $$ = nullptr; };

// We use the Postgres default if the frame end or the whole frame clause is omitted. "If `frame_end` is omitted, the
// end defaults to `CURRENT ROW`. [...] The default framing option is `RANGE UNBOUNDED PRECEDING`, which is the same as
// `RANGE BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW`."
opt_frame_clause : frame_type frame_bound { $$ = new FrameDescription{$1, $2, new FrameBound{0, kCurrentRow, false}}; }
| frame_type BETWEEN frame_bound AND frame_bound { $$ = new FrameDescription{$1, $3, $5}; }
| /* empty */ {
  $$ = new FrameDescription{kRange, new FrameBound{0, kPreceding, true}, new FrameBound{0, kCurrentRow, false}};
};

frame_type : RANGE { $$ = kRange; }
| ROWS { $$ = kRows; }
| GROUPS { $$ = kGroups; };

frame_bound : UNBOUNDED PRECEDING { $$ = new FrameBound{0, kPreceding, true}; }
| INTVAL PRECEDING { $$ = new FrameBound{$1, kPreceding, false}; }
| UNBOUNDED FOLLOWING { $$ = new FrameBound{0, kFollowing, true}; }
| INTVAL FOLLOWING { $$ = new FrameBound{$1, kFollowing, false}; }
| CURRENT_ROW { $$ = new FrameBound{0, kCurrentRow, false}; };

extract_expr : EXTRACT '(' datetime_field FROM expr ')' { $$ = Expr::makeExtract($3, $5); };

// T-SQL sequence expression, e.g. SELECT NEXT VALUE FOR mydb.dbo.seq. It yields
// a value like a function call does, so it is kept as one, with the sequence as
// its single argument - Expr has no dedicated sequence node.
next_value_expr : NEXT_VALUE_FOR table_name {
  auto args = new std::vector<Expr*>();
  args->push_back(Expr::makeColumnRef(qualifiedName($2)));
  $$ = Expr::makeFunctionRef(strdup("NEXT VALUE FOR"), args, false, nullptr);
};

cast_expr : CAST '(' expr AS column_type ')' { $$ = Expr::makeCast($3, $5); };

datetime_field : SECOND { $$ = kDatetimeSecond; }
| MINUTE { $$ = kDatetimeMinute; }
| HOUR { $$ = kDatetimeHour; }
| DAY { $$ = kDatetimeDay; }
| MONTH { $$ = kDatetimeMonth; }
| YEAR { $$ = kDatetimeYear; };

datetime_field_plural : SECONDS { $$ = kDatetimeSecond; }
| MINUTES { $$ = kDatetimeMinute; }
| HOURS { $$ = kDatetimeHour; }
| DAYS { $$ = kDatetimeDay; }
| MONTHS { $$ = kDatetimeMonth; }
| YEARS { $$ = kDatetimeYear; };

duration_field : datetime_field | datetime_field_plural;

// The range qualifier of an interval type or literal, from a leading to a
// trailing field such as DAY TO SECOND. Only the seven ranges of the SQL
// standard are accepted. EXTRACT keeps taking datetime_field, where a range has
// no meaning.
interval_range : datetime_field TO datetime_field {
  if ($1 == kDatetimeYear && $3 == kDatetimeMonth) {
    $$ = kIntervalYearToMonth;
  } else if ($1 == kDatetimeDay && $3 == kDatetimeHour) {
    $$ = kIntervalDayToHour;
  } else if ($1 == kDatetimeDay && $3 == kDatetimeMinute) {
    $$ = kIntervalDayToMinute;
  } else if ($1 == kDatetimeDay && $3 == kDatetimeSecond) {
    $$ = kIntervalDayToSecond;
  } else if ($1 == kDatetimeHour && $3 == kDatetimeMinute) {
    $$ = kIntervalHourToMinute;
  } else if ($1 == kDatetimeHour && $3 == kDatetimeSecond) {
    $$ = kIntervalHourToSecond;
  } else if ($1 == kDatetimeMinute && $3 == kDatetimeSecond) {
    $$ = kIntervalMinuteToSecond;
  } else {
    yyerror(&yyloc, result, scanner,
            "Found incorrect interval field range. Expected one of: YEAR TO MONTH, DAY TO HOUR, DAY TO MINUTE, "
            "DAY TO SECOND, HOUR TO MINUTE, HOUR TO SECOND, MINUTE TO SECOND");
    YYERROR;
  }
};

array_expr : ARRAY '[' expr_list ']' { $$ = Expr::makeArray($3); };

// makeArrayIndex takes the index by value, so the int_literal Expr is not
// adopted by the result and has to be released here - bison only runs the
// %destructor for symbols discarded during error recovery, not for ones a
// successful reduction consumed.
array_index : operand '[' int_literal ']' {
  $$ = Expr::makeArrayIndex($1, $3->ival);
  delete $3;
};

between_expr : operand BETWEEN operand AND operand { $$ = Expr::makeBetween($1, $3, $5); };

column_name : IDENTIFIER { $$ = Expr::makeColumnRef($1); }
| IDENTIFIER '.' name_after_qualifier { $$ = Expr::makeColumnRef($1, $3); }
| '*' { $$ = Expr::makeStar(); }
| IDENTIFIER '.' '*' { $$ = Expr::makeStar($1); }
| nonreserved_keyword { $$ = Expr::makeColumnRef($1); }
// Column qualified by a multi-part table name, e.g. dbo.Table.Column or
// mydb.dbo.Table.Column - the column-side counterpart of the multi-part
// table_name below. The part before the table fills Expr::schema. Expr has no
// database slot, so a four-part name folds database and schema into it as
// "database.schema", the same way table_name does. The qualifiers are spelled
// out part by part: a table_name here would have to be reduced from the first
// IDENTIFIER before the parser can see whether the dot starts a two-part
// column, a qualified star or a schema-qualified function call.
| IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier { $$ = Expr::makeColumnRef($1, $3, $5); }
| IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier '.' name_after_qualifier {
  $$ = Expr::makeColumnRef(dotJoin($1, $3), $5, $7);
}
| IDENTIFIER '.' name_after_qualifier '.' '*' {
  $$ = Expr::makeStar($3);
  $$->schema = $1;
}
| IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier '.' '*' {
  $$ = Expr::makeStar($5);
  $$->schema = dotJoin($1, $3);
}
// Qualified forms of the keywords below, e.g. t.YEAR, dbo.t.MONTH. Without
// these the keyword only works as an unqualified name.
| IDENTIFIER '.' nonreserved_keyword { $$ = Expr::makeColumnRef($1, $3); }
| IDENTIFIER '.' name_after_qualifier '.' nonreserved_keyword { $$ = Expr::makeColumnRef($1, $3, $5); }
| IDENTIFIER '.' name_after_qualifier '.' name_after_qualifier '.' nonreserved_keyword {
  $$ = Expr::makeColumnRef(dotJoin($1, $3), $5, $7);
};

// Keywords the grammar needs as tokens elsewhere, but which dialects also use
// as plain names - column names, and date part arguments such as
// DATEADD(MINUTE, -10, GETDATE()). The token's canonical spelling is used as
// the name; the original casing is not preserved.
nonreserved_keyword : SECOND { $$ = strdup("SECOND"); }
| MINUTE { $$ = strdup("MINUTE"); }
| HOUR { $$ = strdup("HOUR"); }
| DAY { $$ = strdup("DAY"); }
| MONTH { $$ = strdup("MONTH"); }
| YEAR { $$ = strdup("YEAR"); }
| SECONDS { $$ = strdup("SECONDS"); }
| MINUTES { $$ = strdup("MINUTES"); }
| HOURS { $$ = strdup("HOURS"); }
| DAYS { $$ = strdup("DAYS"); }
| MONTHS { $$ = strdup("MONTHS"); }
| YEARS { $$ = strdup("YEARS"); }
| ISNULL { $$ = strdup("ISNULL"); }
| FORMAT { $$ = strdup("FORMAT"); }
| CHAR { $$ = strdup("CHAR"); }
| VARCHAR { $$ = strdup("VARCHAR"); }
| INT { $$ = strdup("INT"); }
| INTEGER { $$ = strdup("INTEGER"); }
| DATETIME { $$ = strdup("DATETIME"); }
| TIMESTAMP { $$ = strdup("TIMESTAMP"); }
| NEXT { $$ = strdup("NEXT"); };

// A name after a qualifier: an identifier, or one of the datetime keywords. On
// their own CURRENT_DATE, CURRENT_TIME and CURRENT_TIMESTAMP stand for the
// current date, time or timestamp, so only a qualified one is a name, e.g.
// t.current_date, dbo.current_date.a or dbo.current_timestamp(). As for
// nonreserved_keyword, the canonical spelling is the name.
name_after_qualifier : IDENTIFIER
| CURRENT_DATE { $$ = strdup("CURRENT_DATE"); }
| CURRENT_TIME { $$ = strdup("CURRENT_TIME"); }
| CURRENT_TIMESTAMP { $$ = strdup("CURRENT_TIMESTAMP"); };

// The last part of a qualified name, which may also be a nonreserved_keyword.
qualified_name_or_keyword : name_after_qualifier | nonreserved_keyword;

// Any position that names a database object. Kept out of column_name, which
// would have to choose between reducing an IDENTIFIER here and shifting the
// '(' of a function call.
name_or_keyword : IDENTIFIER | nonreserved_keyword;

literal : string_literal | bool_literal | num_literal | null_literal | date_literal | time_literal | timestamp_literal
| interval_literal | param_expr;

string_literal : STRING { $$ = Expr::makeLiteral($1); };

bool_literal : TRUE { $$ = Expr::makeLiteral(true); }
| FALSE { $$ = Expr::makeLiteral(false); };

num_literal : FLOATVAL { $$ = Expr::makeLiteral($1); }
| int_literal;

int_literal : INTVAL { $$ = Expr::makeLiteral($1); };

null_literal : NULL { $$ = Expr::makeNullLiteral(); };

date_literal : DATE STRING {
  int day{0}, month{0}, year{0}, chars_parsed{0};
  // If the whole string is parsed, chars_parsed points to the terminating null byte after the last character
  if (sscanf($2, "%4d-%2d-%2d%n", &day, &month, &year, &chars_parsed) != 3 || $2[chars_parsed] != 0) {
    free($2);
    yyerror(&yyloc, result, scanner, "Found incorrect date format. Expected format: YYYY-MM-DD");
    YYERROR;
  }
  $$ = Expr::makeDateLiteral($2);
}
// The current date is the same kind of literal, without a value of its own.
// The MySQL spelling with parentheses is accepted as well.
| CURRENT_DATE { $$ = Expr::makeDateLiteral(nullptr); }
| CURRENT_DATE '(' ')' { $$ = Expr::makeDateLiteral(nullptr); };

// The string of a time or timestamp literal is kept as it is: fractional seconds
// and time zones vary too much for one format. CURRENT_TIME and CURRENT_TIMESTAMP
// may carry the fractional seconds precision in parentheses, which goes to ival;
// MySQL also writes them with empty parentheses.
time_literal : TIME STRING { $$ = Expr::makeTimeLiteral($2); }
| CURRENT_TIME { $$ = Expr::makeTimeLiteral(nullptr); }
| CURRENT_TIME '(' ')' { $$ = Expr::makeTimeLiteral(nullptr); }
| CURRENT_TIME '(' INTVAL ')' {
  if ($3 < 0) {
    yyerror(&yyloc, result, scanner, "The precision of CURRENT_TIME cannot be negative.");
    YYERROR;
  }
  $$ = Expr::makeTimeLiteral(nullptr);
  $$->ival = $3;
};

timestamp_literal : TIMESTAMP STRING { $$ = Expr::makeTimestampLiteral($2); }
| CURRENT_TIMESTAMP { $$ = Expr::makeTimestampLiteral(nullptr); }
| CURRENT_TIMESTAMP '(' ')' { $$ = Expr::makeTimestampLiteral(nullptr); }
| CURRENT_TIMESTAMP '(' INTVAL ')' {
  if ($3 < 0) {
    yyerror(&yyloc, result, scanner, "The precision of CURRENT_TIMESTAMP cannot be negative.");
    YYERROR;
  }
  $$ = Expr::makeTimestampLiteral(nullptr);
  $$->ival = $3;
};

interval_literal : INTVAL duration_field { $$ = Expr::makeIntervalLiteral($1, $2); }
| INTERVAL STRING datetime_field {
  int duration{0}, chars_parsed{0};
  // If the whole string is parsed, chars_parsed points to the terminating null byte after the last character
  if (sscanf($2, "%d%n", &duration, &chars_parsed) != 1 || $2[chars_parsed] != 0) {
    free($2);
    yyerror(&yyloc, result, scanner, "Found incorrect interval format. Expected format: INTEGER");
    YYERROR;
  }
  free($2);
  $$ = Expr::makeIntervalLiteral(duration, $3);
}
// A field range such as YEAR TO MONTH takes a formatted value, e.g. '1-2',
// which is kept as the string it is.
| INTERVAL STRING interval_range { $$ = Expr::makeIntervalLiteral($2, $3); }
| INTERVAL STRING {
  int duration{0}, chars_parsed{0};
  // 'seconds' and 'minutes' are the longest accepted interval qualifiers (7 chars) + null byte
  char unit_string[8];
  // If the whole string is parsed, chars_parsed points to the terminating null byte after the last character
  if (sscanf($2, "%d %7s%n", &duration, unit_string, &chars_parsed) != 2 || $2[chars_parsed] != 0) {
    free($2);
    yyerror(&yyloc, result, scanner, "Found incorrect interval format. Expected format: INTEGER INTERVAL_QUALIFIER");
    YYERROR;
  }
  free($2);

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
  $$ = Expr::makeIntervalLiteral(duration, unit);
};

param_expr : '?' {
  $$ = Expr::makeParameter(yylloc.total_column);
  $$->ival2 = yylloc.total_column - 1;  // source column (0-based) of the '?' token
  yyloc.param_list.push_back($$);
}
| DOLLAR_PARAM {
  if ($1 < 1) {
    yyerror(&yyloc, result, scanner, "$0 is not a valid positional parameter.");
    YYERROR;
  }
  $$ = Expr::makeDollarParameter($1);
  // length of $N token: 1 for '$' + digit count of N
  int64_t dollarLen = 1;
  for (int64_t v = $1; v > 0; v /= 10) ++dollarLen;
  $$->ival2 = yylloc.total_column - dollarLen;
  yyloc.param_list.push_back($$);
}
| NAMED_PARAM {
  $$ = Expr::makeNamedParameter($1);
  $$->ival2 = yylloc.total_column - 1 - (int64_t)strlen($1);
  yyloc.param_list.push_back($$);
};

/******************************
 * Table
 ******************************/
table_ref : table_ref_atomic | table_ref_commalist ',' table_ref_atomic {
  $1->push_back($3);
  auto tbl = new TableRef(kTableCrossProduct);
  tbl->list = $1;
  $$ = tbl;
};

table_ref_atomic : nonjoin_table_ref_atomic | join_clause
// ODBC outer join escape, e.g. FROM {oj T LEFT OUTER JOIN U ON T.id = U.id}.
// The escape only marks the join for the driver, so the join itself is what
// ends up in the tree.
| ODBC_OJ table_ref_atomic '}' { $$ = $2; };

nonjoin_table_ref_atomic : table_ref_name | subquery opt_table_alias {
  auto tbl = new TableRef(kTableSelect);
  tbl->select = $1;
  tbl->alias = $2;
  $$ = tbl;
}
// Table-valued function, e.g. FROM STRING_SPLIT(s, ','). name mirrors the
// function name so getName() behaves like it does for a plain table.
| table_function_expr opt_table_alias {
  auto tbl = new TableRef(kTableFunc);
  tbl->func = $1;
  tbl->name = strdup($1->name);
  if ($1->schema) tbl->schema = strdup($1->schema);
  tbl->alias = $2;
  $$ = tbl;
}
// Table value constructor, e.g. FROM (VALUES (1, 'a'), (2, 'b')) AS t(id, name).
// The column names come from the alias, which table_alias already parses.
| '(' VALUES table_value_row_list ')' opt_table_alias {
  auto tbl = new TableRef(kTableValues);
  tbl->values = $3;
  tbl->alias = $5;
  $$ = tbl;
};

// Each row is kept as an array expression, the same representation used for a
// row constructor in IN.
table_value_row_list : '(' expr_list ')' {
  $$ = new std::vector<Expr*>();
  $$->push_back(Expr::makeArray($2));
}
| table_value_row_list ',' '(' expr_list ')' {
  $1->push_back(Expr::makeArray($4));
  $$ = $1;
};

table_ref_commalist : table_ref_atomic {
  $$ = new std::vector<TableRef*>();
  $$->push_back($1);
}
| table_ref_commalist ',' table_ref_atomic {
  $1->push_back($3);
  $$ = $1;
};

table_ref_name : table_name opt_table_alias {
  auto tbl = new TableRef(kTableName);
  tbl->schema = $1.schema;
  tbl->name = $1.name;
  tbl->alias = $2;
  $$ = tbl;
};

table_ref_name_no_alias : table_name {
  $$ = new TableRef(kTableName);
  $$->schema = $1.schema;
  $$->name = $1.name;
};

// Only the last part may be a keyword. Allowing one in a leading part would
// put '.' in its follow set, which collides with the qualified function call
// in table_function_expr. The datetime keywords are names in every part after
// the first, see name_after_qualifier.
table_name : name_or_keyword {
  $$.schema = nullptr;
  $$.name = $1;
}
| IDENTIFIER '.' qualified_name_or_keyword {
  $$.schema = $1;
  $$.name = $3;
}
| nonreserved_keyword '.' qualified_name_or_keyword {
  $$.schema = $1;
  $$.name = $3;
}
| nonreserved_keyword '.' qualified_name_or_keyword '.' qualified_name_or_keyword {
  $$.schema = dotJoin($1, $3);
  $$.name = $5;
}
| IDENTIFIER '.' name_after_qualifier '.' qualified_name_or_keyword {
  // Three-part (database.schema.table) name. TableName has no separate
  // database slot, so fold database+schema into schema as "database.schema" -
  // callers here only need the statement to parse, not the individual parts.
  $$.schema = dotJoin($1, $3);
  $$.name = $5;
};

opt_index_name : name_or_keyword { $$ = $1; }
| /* empty */ { $$ = nullptr; };

table_alias : alias | AS name_or_keyword '(' ident_commalist ')' { $$ = new Alias($2, $4); };

opt_table_alias : table_alias | /* empty */ { $$ = nullptr; };

alias : AS IDENTIFIER { $$ = new Alias($2); }
// A keyword from nonreserved_keyword may also be the alias, e.g. SELECT a AS year.
// Only accepted after AS: a bare one would be ambiguous with an interval
// literal such as SELECT 1 YEAR.
| AS nonreserved_keyword { $$ = new Alias($2); }
// T-SQL also accepts a string literal as the alias, e.g. SELECT a AS 'My Col'.
// Only the AS form is accepted: without AS the string is indistinguishable from
// a string literal in the select list.
| AS STRING { $$ = new Alias($2); }
| IDENTIFIER { $$ = new Alias($1); };

opt_alias : alias | /* empty */ { $$ = nullptr; };

/******************************
 * Row Locking Descriptions
 ******************************/

opt_locking_clause : opt_locking_clause_list { $$ = $1; }
| /* empty */ { $$ = nullptr; };

opt_locking_clause_list : locking_clause {
  $$ = new std::vector<LockingClause*>();
  $$->push_back($1);
}
| opt_locking_clause_list locking_clause {
  $1->push_back($2);
  $$ = $1;
};

locking_clause : FOR row_lock_mode opt_row_lock_policy {
  $$ = new LockingClause();
  $$->rowLockMode = $2;
  $$->rowLockWaitPolicy = $3;
  $$->tables = nullptr;
}
| FOR row_lock_mode OF ident_commalist opt_row_lock_policy {
  $$ = new LockingClause();
  $$->rowLockMode = $2;
  $$->tables = $4;
  $$->rowLockWaitPolicy = $5;
};

row_lock_mode : UPDATE { $$ = RowLockMode::ForUpdate; }
| NO KEY UPDATE { $$ = RowLockMode::ForNoKeyUpdate; }
| SHARE { $$ = RowLockMode::ForShare; }
| KEY SHARE { $$ = RowLockMode::ForKeyShare; };

opt_row_lock_policy : SKIP LOCKED { $$ = RowLockWaitPolicy::SkipLocked; }
| NOWAIT { $$ = RowLockWaitPolicy::NoWait; }
| /* empty */ { $$ = RowLockWaitPolicy::None; };

/******************************
 * With Descriptions
 ******************************/

with_clause : WITH with_description_list { $$ = $2; };

with_description_list : with_description {
  $$ = new std::vector<WithDescription*>();
  $$->push_back($1);
}
| with_description_list ',' with_description {
  $1->push_back($3);
  $$ = $1;
};

with_description : name_or_keyword AS subquery {
  // The query expression of a WITH description cannot carry a WITH clause of
  // its own.
  if ($3->withDescriptions) {
    free($1);
    delete $3;
    yyerror(&yyloc, result, scanner, "Nested CTE is not allowed.");
    YYERROR;
  }
  $$ = new WithDescription();
  $$->alias = $1;
  $$->select = $3;
};

/******************************
 * Join Statements
 ******************************/

join_clause : table_ref_atomic NATURAL JOIN nonjoin_table_ref_atomic {
  $$ = new TableRef(kTableJoin);
  $$->join = new JoinDefinition();
  $$->join->type = kJoinNatural;
  $$->join->left = $1;
  $$->join->right = $4;
}
| table_ref_atomic opt_join_type JOIN table_ref_atomic ON join_condition {
  $$ = new TableRef(kTableJoin);
  $$->join = new JoinDefinition();
  $$->join->type = (JoinType)$2;
  $$->join->left = $1;
  $$->join->right = $4;
  $$->join->condition = $6;
}
| table_ref_atomic opt_join_type JOIN table_ref_atomic USING '(' ident_commalist ')' {
  $$ = new TableRef(kTableJoin);
  $$->join = new JoinDefinition();
  $$->join->type = $2;
  $$->join->left = $1;
  $$->join->right = $4;
  $$->join->namedColumns = $7;
};

opt_join_type : INNER { $$ = kJoinInner; }
| LEFT OUTER { $$ = kJoinLeft; }
| LEFT { $$ = kJoinLeft; }
| RIGHT OUTER { $$ = kJoinRight; }
| RIGHT { $$ = kJoinRight; }
| FULL OUTER { $$ = kJoinFull; }
| OUTER { $$ = kJoinFull; }
| FULL { $$ = kJoinFull; }
| CROSS { $$ = kJoinCross; }
| /* empty, default */ { $$ = kJoinInner; };

join_condition : expr;

/******************************
 * Misc
 ******************************/

opt_semicolon : ';' | /* empty */
    ;

ident_commalist : name_or_keyword {
  $$ = new std::vector<char*>();
  $$->push_back($1);
}
| ident_commalist ',' name_or_keyword {
  $1->push_back($3);
  $$ = $1;
};

// clang-format off
%%

/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */

    // clang-format on
