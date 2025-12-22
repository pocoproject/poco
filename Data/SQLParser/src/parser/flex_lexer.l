/**
 * lexer
 *
 *
 */


/***************************
 ** Section 1: Definitions
 ***************************/
%{

#include "../sql/Expr.h"
#include "bison_parser.h"
#include <climits>
#include <stdio.h>
#include <sstream>

#define TOKEN(name) { return SQL_##name; }

static thread_local std::stringstream strbuf;

%}
%x singlequotedstring

/***************************
 ** Section 2: Rules
 ***************************/

/* Define the output files */
%option header-file="flex_lexer.h"
%option outfile="flex_lexer.cpp"

/* Make reentrant */
%option reentrant
%option bison-bridge

/* performance tweeks */
%option never-interactive
%option batch

/* other flags */
%option noyywrap
%option nounput
%option warn
%option case-insensitive
%option prefix="hsql_"
%option bison-locations
/* %option nodefault */


%s COMMENT

/***************************
 ** Section 3: Rules
 ***************************/
%%

--              BEGIN(COMMENT);
<COMMENT>[^\n]* /* skipping comment content until a end of line is read */;
<COMMENT>\n     BEGIN(INITIAL);

[ \t\n]+        /* skip whitespace */;

ADD         TOKEN(ADD)
AFTER       TOKEN(AFTER)
ALL         TOKEN(ALL)
ALTER       TOKEN(ALTER)
ANALYZE     TOKEN(ANALYZE)
AND         TOKEN(AND)
ARRAY       TOKEN(ARRAY)
AS          TOKEN(AS)
ASC         TOKEN(ASC)
BEFORE      TOKEN(BEFORE)
BEGIN       TOKEN(BEGIN)
BETWEEN     TOKEN(BETWEEN)
BIGINT      TOKEN(BIGINT)
BOOLEAN     TOKEN(BOOLEAN)
BY          TOKEN(BY)
CALL        TOKEN(CALL)
CASCADE     TOKEN(CASCADE)
CASE        TOKEN(CASE)
CAST        TOKEN(CAST)
CHAR        TOKEN(CHAR)
COLUMN      TOKEN(COLUMN)
COLUMNS     TOKEN(COLUMNS)
COMMIT      TOKEN(COMMIT)
CONTROL     TOKEN(CONTROL)
COPY        TOKEN(COPY)
CREATE      TOKEN(CREATE)
CROSS       TOKEN(CROSS)
DATE        TOKEN(DATE)
DATETIME    TOKEN(DATETIME)
DAY         TOKEN(DAY)
DAYS        TOKEN(DAYS)
DEALLOCATE  TOKEN(DEALLOCATE)
DECIMAL     TOKEN(DECIMAL)
DEFAULT     TOKEN(DEFAULT)
DELETE      TOKEN(DELETE)
DELTA       TOKEN(DELTA)
DESC        TOKEN(DESC)
DESCRIBE    TOKEN(DESCRIBE)
DIRECT      TOKEN(DIRECT)
DISTINCT    TOKEN(DISTINCT)
DOUBLE      TOKEN(DOUBLE)
DROP        TOKEN(DROP)
ELSE        TOKEN(ELSE)
END         TOKEN(END)
ESCAPE      TOKEN(ESCAPE)
EXCEPT      TOKEN(EXCEPT)
EXECUTE     TOKEN(EXECUTE)
EXISTS      TOKEN(EXISTS)
EXPLAIN     TOKEN(EXPLAIN)
EXTRACT     TOKEN(EXTRACT)
FALSE       TOKEN(FALSE)
FILE        TOKEN(FILE)
FLOAT       TOKEN(FLOAT)
FOLLOWING   TOKEN(FOLLOWING)
FOR         TOKEN(FOR)
FORMAT      TOKEN(FORMAT)
FROM        TOKEN(FROM)
FULL        TOKEN(FULL)
GLOBAL      TOKEN(GLOBAL)
GROUP       TOKEN(GROUP)
GROUPS      TOKEN(GROUPS)
HASH        TOKEN(HASH)
HAVING      TOKEN(HAVING)
HINT        TOKEN(HINT)
HOUR        TOKEN(HOUR)
HOURS       TOKEN(HOURS)
IF          TOKEN(IF)
ILIKE       TOKEN(ILIKE)
IMPORT      TOKEN(IMPORT)
IN          TOKEN(IN)
INDEX       TOKEN(INDEX)
INNER       TOKEN(INNER)
INSERT      TOKEN(INSERT)
INT         TOKEN(INT)
INTEGER     TOKEN(INTEGER)
INTERSECT   TOKEN(INTERSECT)
INTERVAL    TOKEN(INTERVAL)
INTO        TOKEN(INTO)
IS          TOKEN(IS)
ISNULL      TOKEN(ISNULL)
JOIN        TOKEN(JOIN)
KEY         TOKEN(KEY)
LEFT        TOKEN(LEFT)
LIKE        TOKEN(LIKE)
LIMIT       TOKEN(LIMIT)
LOAD        TOKEN(LOAD)
LOCAL       TOKEN(LOCAL)
LOCKED      TOKEN(LOCKED)
LONG        TOKEN(LONG)
MERGE       TOKEN(MERGE)
MINUS       TOKEN(MINUS)
MINUTE      TOKEN(MINUTE)
MINUTES     TOKEN(MINUTES)
MONTH       TOKEN(MONTH)
MONTHS      TOKEN(MONTHS)
NATURAL     TOKEN(NATURAL)
NO          TOKEN(NO)
NOT         TOKEN(NOT)
NOWAIT      TOKEN(NOWAIT)
NULL        TOKEN(NULL)
NVARCHAR    TOKEN(NVARCHAR)
OF          TOKEN(OF)
OFF         TOKEN(OFF)
OFFSET      TOKEN(OFFSET)
ON          TOKEN(ON)
OR          TOKEN(OR)
ORDER       TOKEN(ORDER)
OUTER       TOKEN(OUTER)
OVER        TOKEN(OVER)
PARAMETERS  TOKEN(PARAMETERS)
PARTITION   TOKEN(PARTITION)
PLAN        TOKEN(PLAN)
PRECEDING   TOKEN(PRECEDING)
PREPARE     TOKEN(PREPARE)
PRIMARY     TOKEN(PRIMARY)
RANGE       TOKEN(RANGE)
REAL        TOKEN(REAL)
RENAME      TOKEN(RENAME)
RESTRICT    TOKEN(RESTRICT)
RIGHT       TOKEN(RIGHT)
ROLLBACK    TOKEN(ROLLBACK)
ROWS        TOKEN(ROWS)
SCHEMA      TOKEN(SCHEMA)
SCHEMAS     TOKEN(SCHEMAS)
SECOND      TOKEN(SECOND)
SECONDS     TOKEN(SECONDS)
SELECT      TOKEN(SELECT)
SET         TOKEN(SET)
SHARE       TOKEN(SHARE)
SHOW        TOKEN(SHOW)
SKIP        TOKEN(SKIP)
SMALLINT    TOKEN(SMALLINT)
SORTED      TOKEN(SORTED)
SPATIAL     TOKEN(SPATIAL)
TABLE       TOKEN(TABLE)
TABLES      TOKEN(TABLES)
TEMPORARY   TOKEN(TEMPORARY)
TEXT        TOKEN(TEXT)
THEN        TOKEN(THEN)
TIME        TOKEN(TIME)
TIMESTAMP   TOKEN(TIMESTAMP)
TO          TOKEN(TO)
TOP         TOKEN(TOP)
TRANSACTION TOKEN(TRANSACTION)
TRUE        TOKEN(TRUE)
TRUNCATE    TOKEN(TRUNCATE)
UNBOUNDED   TOKEN(UNBOUNDED)
UNION       TOKEN(UNION)
UNIQUE      TOKEN(UNIQUE)
UNLOAD      TOKEN(UNLOAD)
UPDATE      TOKEN(UPDATE)
USING       TOKEN(USING)
VALUES      TOKEN(VALUES)
VARCHAR     TOKEN(VARCHAR)
VIEW        TOKEN(VIEW)
VIRTUAL     TOKEN(VIRTUAL)
WHEN        TOKEN(WHEN)
WHERE       TOKEN(WHERE)
WITH        TOKEN(WITH)
YEAR        TOKEN(YEAR)
YEARS       TOKEN(YEARS)

CURRENT[ \t\n]+ROW       TOKEN(CURRENT_ROW)
CHARACTER[ \t\n]+VARYING TOKEN(CHARACTER_VARYING)

            /* Allow =/== see https://sqlite.org/lang_expr.html#collateop */
"=="        TOKEN(EQUALS)
"!="        TOKEN(NOTEQUALS)
"<>"        TOKEN(NOTEQUALS)
"<="        TOKEN(LESSEQ)
">="        TOKEN(GREATEREQ)
"||"        TOKEN(CONCAT)

[-+*/(){},.;<>=^%:?[\]|]    { return yytext[0]; }

[0-9]+"."[0-9]* |
"."[0-9]*  {
  yylval->fval = atof(yytext);
  return SQL_FLOATVAL;
}

  /*
   * Regularly, negative literals are treated as <unary minus> <positive literal>. This does not work for LLONG_MIN, as it has no
   * positive equivalent. We thus match for LLONG_MIN specifically. This is not an issue for floats, where
   *   numeric_limits<double>::lowest() == -numeric_limits<double>::max();
   */
-9223372036854775808 {
  yylval->ival = LLONG_MIN;
  return SQL_INTVAL;
}

[0-9]+ {
  errno = 0;
  yylval->ival = strtoll(yytext, nullptr, 0);
  if (errno) {
    return fprintf(stderr, "[SQL-Lexer-Error] Integer cannot be parsed - is it out of range?");
    return 0;
  }
  return SQL_INTVAL;
}

\"[^\"\n]+\" {
  // Crop the leading and trailing quote char
  yylval->sval = hsql::substr(yytext, 1, strlen(yytext)-1);
  return SQL_IDENTIFIER;
}

[A-Za-z][A-Za-z0-9_]* {
  yylval->sval = strdup(yytext);
  return SQL_IDENTIFIER;
}

\'                          { BEGIN singlequotedstring; strbuf.clear(); strbuf.str(""); }  // Clear strbuf manually, see #170
<singlequotedstring>\'\'    { strbuf << '\''; }
<singlequotedstring>[^']*   { strbuf << yytext; }
<singlequotedstring>\'      { BEGIN 0; yylval->sval = strdup(strbuf.str().c_str()); return SQL_STRING; }
<singlequotedstring><<EOF>> { fprintf(stderr, "[SQL-Lexer-Error] Unterminated string\n"); return 0; }

. { fprintf(stderr, "[SQL-Lexer-Error] Unknown Character: %c\n", yytext[0]); return 0; }

%%
/***************************
 ** Section 3: User code
 ***************************/

int yyerror(const char *msg) {
    fprintf(stderr, "[SQL-Lexer-Error] %s\n",msg); return 0;
}
