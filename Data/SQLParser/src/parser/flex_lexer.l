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

  /*
   * Whitespace, carriage return included. Every rule that has to skip or span
   * whitespace uses this, so a CRLF input cannot be handled by one rule and
   * missed by the next.
   */
WS  [ \t\r\n]

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
%option noinput nounput
%option warn
%option case-insensitive
%option prefix="hsql_"
%option bison-locations
/* %option nodefault */


  /*
   * Exclusive: an inclusive COMMENT would also run the whitespace rule below,
   * which matches longer than the newline ending the comment and so never
   * returns to INITIAL - silently swallowing the line after the comment.
   */
%x COMMENT
%x afterarray

/***************************
 ** Section 3: Rules
 ***************************/
%%

--              BEGIN(COMMENT);
<COMMENT>[^\n]* /* skipping comment content until a end of line is read */;
<COMMENT>\n     BEGIN(INITIAL);

{WS}+           /* skip whitespace; CR is included so CRLF input never reaches the catch-all rule below, which ends the token stream */;

ADD         TOKEN(ADD)
AFTER       TOKEN(AFTER)
ALL         TOKEN(ALL)
ALTER       TOKEN(ALTER)
ANALYZE     TOKEN(ANALYZE)
AND         TOKEN(AND)
ARRAY       { BEGIN(afterarray); return SQL_ARRAY; }
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
ENCODING    TOKEN(ENCODING)
END         TOKEN(END)
ESCAPE      TOKEN(ESCAPE)
EXCEPT      TOKEN(EXCEPT)
EXECUTE     TOKEN(EXECUTE)
EXISTS      TOKEN(EXISTS)
EXPLAIN     TOKEN(EXPLAIN)
EXTRACT     TOKEN(EXTRACT)
FALSE       TOKEN(FALSE)
FETCH       TOKEN(FETCH)
FILE        TOKEN(FILE)
FLOAT       TOKEN(FLOAT)
FOLLOWING   TOKEN(FOLLOWING)
FOR         TOKEN(FOR)
FOREIGN     TOKEN(FOREIGN)
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
NEXT        TOKEN(NEXT)
NO          TOKEN(NO)
NOT         TOKEN(NOT)
NOWAIT      TOKEN(NOWAIT)
NULL        TOKEN(NULL)
NVARCHAR    TOKEN(NVARCHAR)
OF          TOKEN(OF)
OFF         TOKEN(OFF)
OFFSET      TOKEN(OFFSET)
ON          TOKEN(ON)
ONLY        TOKEN(ONLY)
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
PRIOR       TOKEN(PRIOR)
RANGE       TOKEN(RANGE)
REAL        TOKEN(REAL)
REFERENCES  TOKEN(REFERENCES)
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

  /*
   * Phrases rather than single reserved words: START, CONNECT and WITHIN are
   * ordinary names on their own, and reserving them broke statements such as
   * "SELECT * FROM start". The trailing context keeps "CONNECT BY nocycleflag"
   * from lexing as CONNECT_BY_NOCYCLE followed by "flag".
   */
START{WS}+WITH/[^A-Za-z0-9_]              TOKEN(START_WITH)
CONNECT{WS}+BY{WS}+NOCYCLE/[^A-Za-z0-9_]  TOKEN(CONNECT_BY_NOCYCLE)
CONNECT{WS}+BY/[^A-Za-z0-9_]              TOKEN(CONNECT_BY)
WITHIN{WS}+GROUP/[^A-Za-z0-9_]            TOKEN(WITHIN_GROUP)

CURRENT{WS}+ROW              TOKEN(CURRENT_ROW)
NEXT{WS}+VALUE{WS}+FOR       TOKEN(NEXT_VALUE_FOR)
CHARACTER{WS}+VARYING        TOKEN(CHARACTER_VARYING)

            /* Allow =/== see https://sqlite.org/lang_expr.html#collateop */
"=="        TOKEN(EQUALS)
"!="        TOKEN(NOTEQUALS)
"<>"        TOKEN(NOTEQUALS)
"<="        TOKEN(LESSEQ)
">="        TOKEN(GREATEREQ)
"||"        TOKEN(CONCAT)

  /*
   * ODBC outer join escape prefix. Lexed as one token so "oj" keeps working as
   * an ordinary identifier everywhere else. The trailing context stops the rule
   * from taking the "{oj" out of a name such as "{ojtest".
   */
"{"{WS}*oj/[^A-Za-z0-9_]    TOKEN(ODBC_OJ)

  /*
   * Oracle's legacy outer join marker. Lexed as one token so it cannot be
   * confused with a parenthesized expression - "(" followed by a lone "+" is
   * not valid SQL in any other position, since the grammar has no unary plus.
   */
"("{WS}*"+"{WS}*")"    TOKEN(OUTERJOIN)

\$[0-9]+    {
  yylval->ival = atoll(yytext + 1);
  return SQL_DOLLAR_PARAM;
}

:[A-Za-z][A-Za-z0-9_]*  {
  yylval->sval = strdup(yytext + 1);
  return SQL_NAMED_PARAM;
}

[-+*/(){},.;<>=^%:?[\]|]    { return yytext[0]; }

  /*
   * Scientific notation, e.g. 1.5e3, 1.5E+3, .5e-3, 2e10. Has to come before the
   * plain float and integer rules, which would otherwise take the mantissa and
   * leave the exponent to be scanned as an identifier.
   */
[0-9]+"."[0-9]*e[-+]?[0-9]+ |
"."[0-9]+e[-+]?[0-9]+ |
[0-9]+e[-+]?[0-9]+ {
  yylval->fval = atof(yytext);
  return SQL_FLOATVAL;
}

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

<afterarray>{WS}+ ; /* stay in afterarray across whitespace before the '[', e.g. "ARRAY [1,2,3]" */

<afterarray>\[ {
  // The '[' immediately after ARRAY belongs to the array_expr grammar
  // (ARRAY '[' expr_list ']'), not bracket-quoting - even for a single
  // bare-identifier element like ARRAY[foo]. Let it fall through as plain
  // punctuation instead of being swallowed by the bracket-identifier rule below.
  BEGIN(INITIAL);
  return yytext[0];
}

<afterarray>. {
  // Whatever follows ARRAY wasn't '[' (a syntax error downstream either
  // way) - drop back to normal lexing and reprocess this character there.
  BEGIN(INITIAL);
  yyless(0);
}

\[[A-Za-z_][A-Za-z0-9_ ]*\] {
  // T-SQL bracket-quoted identifier, e.g. [dbo] or [My Table]. Restricted to
  // content starting with a letter/underscore so it doesn't shadow the
  // existing array_index ('[' int_literal ']') grammar, whose bracket
  // contents start with a digit. The ARRAY[...] case (including a single
  // bare-identifier element) is handled above via the afterarray state.
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
