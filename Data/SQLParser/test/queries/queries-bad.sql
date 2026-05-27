# This file contains a list of strings that are NOT valid SQL queries.
# Each line contains a single SQL query.
# Each line starts with a '!' char to indicate that parsing should fail.
!
!1
!gibberish;
!CREATE TABLE "table" FROM TBL FILE 'students.tbl';gibberish
!CREATE TABLE "table" FROM TBL FILE 'students.tbl';1
!CREATE TABLE foo (a int, b int bar);
!CREATE TABLE foo (a int, b REFERENCES bar);
!CREATE TABLE foo (a int, b int, FOREIGN (b) REFERENCES bar);
!CREATE TABLE foo (a int, b int, KEY (b) REFERENCES bar);
!CREATE TABLE foo (a int, b int, FOREIGN KEY (b) bar);
!CREATE TABLE foo (a int, b int, FOREIGN KEY REFERENCES bar);
!CREATE TABLE foo (a int, b int, FOREIGN KEY b REFERENCES bar);
!CREATE TABLE foo (a int, b int, FOREIGN KEY (b) REFERENCES bar x);
!INSERT INTO test_table VALUESd (1, 2, 'test');
!SELECT * FROM t WHERE a = ? AND b = ?;gibberish;
!SHOW COLUMNS;
!DESCRIBE;
!COPY;
!COPY students;
!COPY students FROM 'students_file' WITH (FORMAT XYZ);
!COPY students TO 'students_file' WITH (FORMAT XYZ);
!COPY students FROM 'students_file' WITH ();
!COPY students TO 'students_file' WITH ();
!COPY students TO 'students_file' WITH (FORMAT CSV ENCODING 'Dictionary');
!COPY students TO 'students_file' WITH FORMAT CSV;
!COPY students TO 'students_file' WITH (FORMAT CSV, FORMAT BINARY);
!COPY students TO 'students_file' WITH (ENCODING 'Dictionary', ENCODING 'FSST');
!COPY students FROM 'students_file' WITH (ENCODING Dictionary);
!select a + 2 as b(spam, eggs) from B;
!WITH a AS SELECT 1 SELECT 1;
!WITH a AS (SELECT ) SELECT 1;
!WITH a AS (WITH b AS (SELECT 1) SELECT 1) SELECT 1; # We do not support nested WITH clauses
!WITH a AS (SELECT ) b AS (SELECT ) SELECT 1; # Missing comma between WITH descriptions
!BEGIN TRANSACTION transName; # Transaction naming is currently not supported
!SELECT -9223372036854775809; # Out of int64_t range
!SELECT 9223372036854775808; # Out of int64_t range
!SELECT * FROM t WHERE a = DATE 'anystring';
!SELECT * FROM t WHERE a = DATE '1996-12-310';
!SELECT * FROM t WHERE a = DATE '1996-120-31';
!SELECT * FROM t WHERE a = DATE '19960-12-31';
!SELECT * FROM t WHERE a = DATE 'asdf-gh-jkl';
!SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL 30;
!SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL 30 DAYS;
!SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL 30 'DAYS';
!SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL 'DAYS';
!SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '1' ANYTHING;
!SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '1 DAY' DAY;
!SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '30 ANYTHING';
!SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '30' DAYS;
!SELECT * FROM t WHERE a = DATE '2000-01-01' + x DAYS;
!SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL 'x' DAY;
!SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '3.3 DAYS';
# ON is not supported by postgres. We follow postgres here since the sql-92 standard does not specify index
# implementation details.
!DROP INDEX myindex ON mytable;
!SELECT * FROM test WHERE val = 2 FOR KEY UPDATE;
!SELECT * FROM test WHERE val = 2 FOR SHARE test1;
!SELECT * FROM test WHERE val = 2 FOR NO KEY SHARE;
!SELECT * FROM test WHERE val = 2 NOWAIT FOR UPDATE;
!CREATE TABLE a_table (a_column INT PRIMARY KEY NULL);
!CREATE TABLE a_table (a_column INT NULL PRIMARY KEY);
!CREATE TABLE a_table (a_column INT NOT NULL NULL);
!CREATE TABLE a_table (a_column INT NULL NOT NULL);
# WINDOW EXPRESSIONS
!SELECT test1, sum(sum(test2)) OVER (PARTITION BY test3 ORDER BY test4 ROWS BETWEEN UNBOUNDED AND CURRENT ROW) FROM test;
!SELECT test1, sum(sum(test2)) OVER (PARTITION BY test3 ORDER BY test4 ROWS BETWEEN -1 PRECEDING AND CURRENT ROW) FROM test;
!SELECT test1, rank() OVER (INVALID UNBOUNDED PRECEDING) FROM test;
!SELECT rank() OVER (INVALID) FROM test;
!SELECT rank OVER () FROM test;
!SELECT a = 1 OVER () FROM test;
!SELECT rank() OVER (ROWS UNBOUNDEDD PRECEDING) FROM test;
!SELECT rank() OVER (ROWS UNBOUNDED PRECEDINGG) FROM test;
!SELECT test1, rank() OVER (ROWS -1 PRECEDING) FROM test;
!SELECT test1, rank() OVER (ROWS BETWEEN UNBOUNDED PRECEDING AND -1 FOLLOWING) FROM test;
# Join USING
!SELECT * FROM foo INNER JOIN bar USING ();
!SELECT * FROM foo INNER JOIN bar USING (*);
# Both the SQL standard and Postgres allow column names only (no column references).
!SELECT * FROM foo INNER JOIN bar USING (foo.a);
!SELECT * FROM foo INNER JOIN bar USING (a b);
!SELECT * FROM foo INNER JOIN bar USING (a AS b);
!SELECT * FROM foo INNER JOIN bar USING (1);
# INSERT, EXECUTE, and HINTS only allow specific expressions.
!INSERT INTO foo VALUES (?);
!INSERT INTO foo VALUES (CAST(column_a AS INT));
!INSERT INTO foo VALUES (AVG(another_column));
!EXECUTE statement_a(?);
!EXECUTE statement_a(CAST(column_a AS INT));
!EXECUTE statement_a(AVG(another_column));
!SELECT * FROM foo WITH HINT (?);
!SELECT * FROM foo WITH HINT (CAST(column_a AS INT));
!SELECT * FROM foo WITH HINT (AVG(another_column));
# ORDER BY with NULL ordering.
!SELECT * FROM students ORDER BY name ASC NULL FIRST;
!SELECT * FROM students ORDER BY name ASC gibberish LAST;
!SELECT * FROM students ORDER BY name NULLS FIRS;
!SELECT * FROM students ORDER BY name NULLS;
!SELECT * FROM students ORDER BY name ASC NULLS;
!SELECT * FROM students ORDER BY name FIRST;
!SELECT * FROM students ORDER BY name ASC LAST;
!SELECT * FROM students ORDER BY name DESC NULLS gibberish;
# CSV options
!COPY students FROM 'file_path' WITH (FORMAT TBL, DELIMITER '|', NULL '', QUOTE '"');
!COPY students FROM 'file_path' WITH (DELIMITER '|', NULL '', QUOTE '"', FORMAT TBL);
!COPY students FROM 'file_path' WITH (DELIMITER '|', NULL '', FORMAT TBL, QUOTE '"');
!COPY students FROM 'file_path' WITH (DELIMITER '|', NULL '', QUOTE '"', NULL 'a');
!COPY students FROM 'file_path' WITH (NULL '', QUOTE '"', DELIMITER '|', DELIMITER '/');
!COPY students FROM 'file_path' WITH (QUOTE '"', NULL '', DELIMITER '/', QUOTE '_',);
!COPY students FROM 'file_path' WITH (FORMAT CSV, QUOTE '"', DELIMINIMITER '|');
