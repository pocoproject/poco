# This file contains a list of strings that are valid SQL queries.
# Each line contains a single SQL query.
# SELECT statement
SELECT * FROM orders;
SELECT a FROM foo WHERE a > 12 OR b > 3 AND NOT c LIMIT 10
SELECT a FROM some_schema.foo WHERE a > 12 OR b > 3 AND NOT c LIMIT 10
SELECT col1 AS myname, col2, 'test' FROM "table", foo AS t WHERE age > 12 AND zipcode = 12345 GROUP BY col1;
SELECT * from "table" JOIN table2 ON a = b WHERE (b OR NOT a) AND a = 12.5
(SELECT a FROM foo WHERE a > 12 OR b > 3 AND c NOT LIKE 's%' LIMIT 10);
SELECT * FROM "table" LIMIT 10 OFFSET 10; SELECT * FROM another;
SELECT * FROM t1 UNION SELECT * FROM t2 ORDER BY col1;
SELECT * FROM (SELECT * FROM t1);
SELECT * FROM t1 UNION (SELECT * FROM t2 UNION SELECT * FROM t3) ORDER BY col1;
SELECT TOP 10 * FROM t1 ORDER BY col1, col2;
SELECT a, MAX(b), MAX(c, d), CUSTOM(q, UP(r)) AS f FROM t1;
SELECT * FROM t WHERE a BETWEEN 1 and c;
SELECT * FROM t WHERE a = ? AND b = ?;
SELECT City.name, Product.category, SUM(price) FROM fact INNER JOIN City ON fact.city_id = City.id INNER JOIN Product ON fact.product_id = Product.id GROUP BY City.name, Product.category;
SELECT SUBSTR(a, 3, 5) FROM t;
SELECT * FROM t WHERE a = DATE '1996-12-31';
SELECT * FROM t WHERE a = TIMESTAMP '1996-12-31 12:11:10';
SELECT * FROM t WHERE a = TIME '12:11:10';
SELECT * FROM t WHERE a = CURRENT_DATE;
SELECT * FROM t WHERE a = CURRENT_TIMESTAMP;
SELECT * FROM t WHERE a = CURRENT_TIME;
SELECT CURRENT_TIMESTAMP();
SELECT CURRENT_TIMESTAMP(3);
SELECT CURRENT_TIME(), CURRENT_TIME(6), CURRENT_DATE();
SELECT timestamp FROM t;
SELECT * FROM t WHERE a = CURRENT_DATE - INTERVAL '1' DAY;
SELECT CURRENT_TIMESTAMP(0), CURRENT_TIME(0);
CREATE TABLE t ("current_date" INT);
SELECT dbo.current_date.a, dbo.current_date.* FROM dbo.current_date JOIN db.current_time.t ON dbo.current_date.a = t.b;
SELECT dbo.current_timestamp(), dbo.current_time(1) FROM t;
INSERT INTO dbo.current_date VALUES (1);
UPDATE dbo.current_time SET a = 1;
SELECT NEXT VALUE FOR dbo.current_timestamp;
SELECT t.current_date, dbo.t.current_time, mydb.dbo.t.current_timestamp FROM t WHERE t.current_date < CURRENT_DATE;
# JOIN
SELECT t1.a, t1.b, t2.c FROM "table" AS t1 JOIN (SELECT * FROM foo JOIN bar ON foo.id = bar.id) t2 ON t1.a = t2.b WHERE (t1.b OR NOT t1.a) AND t2.c = 12.5
SELECT * FROM t1 JOIN t2 ON c1 = c2;
SELECT a, SUM(b) FROM t2 GROUP BY a HAVING SUM(b) > 100;
# CREATE statement
CREATE TABLE "table" FROM TBL FILE 'students.tbl'
CREATE TABLE IF NOT EXISTS "table" FROM TBL FILE 'students.tbl'
CREATE INDEX myindex ON myschema.mytable (col1);
CREATE TABLE students (name TEXT, student_number INTEGER, city TEXT, grade DOUBLE, credits BIGINT)
CREATE TABLE students (name TEXT, student_number INTEGER NOT NULL, city TEXT, grade DOUBLE PRIMARY KEY UNIQUE)
CREATE TABLE teachers (name VARCHAR(30), student_number LONG, city CHAR(10), grade FLOAT)
CREATE TABLE teachers (name VARCHAR(30), student_number LONG, PRIMARY KEY (name, student_number), city CHAR(10), grade FLOAT)
CREATE TABLE teachers (name CHARACTER VARYING(30));
CREATE TABLE students_2 AS SELECT * FROM students
CREATE TABLE students_3 AS SELECT city, grade FROM students WHERE grade > 3.0
CREATE TABLE students (date_of_birth DATE, matriculation_date DATETIME, graduation_date TIMESTAMP, graduated BOOLEAN);
CREATE TABLE foo (a int, b int REFERENCES bar REFERENCES baz);
CREATE TABLE foo (a int, b int REFERENCES bar (x) REFERENCES baz (y));
CREATE TABLE foo (a int, b int, FOREIGN KEY (b) REFERENCES bar, FOREIGN KEY (b) REFERENCES baz);
CREATE TABLE foo (a int, b int, FOREIGN KEY (b) REFERENCES bar (x), FOREIGN KEY (b) REFERENCES baz (y));
CREATE TABLE foo (i INTERVAL);
CREATE TABLE t (ts TIMESTAMP, i INTERVAL HOUR TO MINUTE);
CREATE TABLE t (a INTERVAL YEAR, b INTERVAL DAY TO SECOND, c INTERVAL MINUTE TO SECOND);
# Multiple statements
CREATE TABLE "table" FROM TBL FILE 'students.tbl'; SELECT * FROM "table";
# INSERT
INSERT INTO test_table VALUES (1, 2, 'test');
INSERT INTO test_table (id, value, name) VALUES (1, 2, 'test');
INSERT INTO test_table SELECT * FROM students;
INSERT INTO some_schema.test_table SELECT * FROM another_schema.students;
INSERT INTO logs VALUES (101, CURRENT_TIMESTAMP, CURRENT_DATE, CURRENT_TIME);
INSERT INTO t VALUES (INTERVAL '1' YEAR, INTERVAL '1-2' YEAR TO MONTH, INTERVAL '2 04:30' DAY TO MINUTE);
# DELETE
DELETE FROM students WHERE grade > 3.0
DELETE FROM students
TRUNCATE students
# UPDATE
UPDATE students SET grade = 1.3 WHERE name = 'Max Mustermann';
UPDATE students SET grade = 1.3, name='Felix Fürstenberg' WHERE name = 'Max Mustermann';
UPDATE students SET grade = 1.0;
UPDATE some_schema.students SET grade = 1.0;
UPDATE logs SET ts = CURRENT_TIMESTAMP, d = CURRENT_DATE, t = CURRENT_TIME WHERE id = 101;
# ALTER
ALTER TABLE mytable DROP COLUMN IF EXISTS mycolumn;
ALTER TABLE IF EXISTS mytable DROP COLUMN IF EXISTS mycolumn;
# DROP
DROP TABLE students;
DROP TABLE IF EXISTS students;
DROP VIEW IF EXISTS students;
DROP INDEX myindex;
DROP INDEX IF EXISTS myindex;
# PREPARE
PREPARE prep_inst FROM 'INSERT INTO test VALUES (?, ?, ?)';
PREPARE prep2 FROM 'INSERT INTO test VALUES (?, 0, 0); INSERT INTO test VALUES (0, ?, 0); INSERT INTO test VALUES (0, 0, ?);';
EXECUTE prep_another_inst(1, 2, 3, CAST(1 AS LONG), -2.5, INTERVAL '3 HOURS', -2 SECONDS, TRUE, NULL, DATE '2000-01-01');
EXECUTE prep;
DEALLOCATE PREPARE prep;
# COPY
COPY students FROM 'student.tbl';
COPY students FROM 'file_path' WITH (FORMAT TBL);
COPY students FROM 'file_path' WITH (FORMAT CSV);
COPY students FROM 'file_path' WITH (FORMAT BIN);
COPY students FROM 'file_path' WITH (FORMAT BINARY);
COPY students FROM 'file_path' WITH (FORMAT CSV, DELIMITER '|', NULL '', QUOTE '"');
COPY students FROM 'file_path' WITH (DELIMITER '|', NULL '', FORMAT CSV, QUOTE '"');
COPY students FROM 'file_path' WITH (DELIMITER '|', NULL '', QUOTE '"');
COPY students FROM 'file_path' WITH (DELIMITER '|', FORMAT CSV);
COPY students FROM 'file_path' (FORMAT TBL);
COPY good_students FROM 'file_path' WHERE grade > (SELECT AVG(grade) from alumni);
COPY students TO 'student.tbl';
COPY students TO 'file_path' WITH (ENCODING 'some_encoding', FORMAT TBL);
COPY students TO 'file_path' WITH (FORMAT CSV);
COPY students TO 'file_path' WITH (FORMAT BIN);
COPY students TO 'file_path' WITH (FORMAT BINARY);
COPY students TO 'file_path' (FORMAT BINARY, ENCODING 'FSST');
COPY students TO 'file_path' WITH (ENCODING 'Dictionary');
COPY (SELECT firstname, COUNT(*) FROM students GROUP BY firstname) TO 'student_names.csv';
# HINTS
SELECT * FROM test WITH HINT(NO_CACHE);
SELECT * FROM test WITH HINT(NO_CACHE, NO_SAMPLING);
SELECT * FROM test WITH HINT(NO_CACHE, SAMPLE_RATE(0.1), OMW(1.0, 'test'), START_DATE(CAST('2000-01-01' AS DATE)));
SELECT * FROM test WITH HINT(TIME_DIFFERENCE(-3 HOURS), ALLOW_RESULT_CACHE(FALSE), DEFAULT_VALUE(NULL), TIMETRAVEL_TO(DATE '2000-01-01'));
SHOW TABLES;
SHOW COLUMNS students;
DESCRIBE students;
SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '30 DAYS';
SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '10' DAY;
SELECT TOP 5 a FROM t ORDER BY a OFFSET 10;
SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '1-2' YEAR TO MONTH;
SELECT CAST('12' AS INTERVAL YEAR), CAST('1-2' AS INTERVAL YEAR TO MONTH), CAST('2 21:46:20.123' AS INTERVAL DAY TO SECOND);
SELECT * FROM t WHERE a BETWEEN '2000-01-01' AND DATE '2000-01-01' - 1 MONTH;
SELECT (CAST('2002-5-01' as DATE) + INTERVAL '60 days');
SELECT CAST(student.student_number as BIGINT) FROM student;
SELECT student.name AS character FROM student;
# ROW LOCKING
SELECT * FROM test WHERE id = 1 FOR UPDATE;
SELECT * FROM test WHERE id = 1 FOR SHARE;
SELECT * FROM test WHERE id = 1 FOR NO KEY UPDATE;
SELECT * FROM test WHERE id = 1 FOR KEY SHARE;
SELECT * FROM test WHERE id = 1 FOR UPDATE SKIP LOCKED;
SELECT * FROM test WHERE id = 1 FOR UPDATE NOWAIT;
SELECT * FROM test1, test2 WHERE test1.id = 10 FOR UPDATE OF test1;
SELECT * FROM test1, test2 WHERE test2.val = 2 FOR SHARE OF test1, test2;
SELECT * FROM test1, test2 WHERE test2.val = 2 FOR UPDATE OF test1 FOR SHARE OF test2;
# WINDOW EXPRESSIONS
SELECT test1, sum(sum(test2)) OVER (PARTITION BY test3 ORDER BY test4 ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW) an_alias FROM test;
SELECT sum(test2)/sum(sum(test2)) OVER (PARTITION BY test1) FROM test GROUP BY test3;
SELECT test1, sum(sum(test2)) OVER (PARTITION BY test3, test4 ORDER BY test5, test6 ROWS BETWEEN 1 PRECEDING AND 2 FOLLOWING) FROM test;
SELECT test1, rank() OVER (ORDER BY test2 DESC, test3 ASC) rnk FROM test;
SELECT rank() OVER () FROM test;
SELECT rank() OVER (PARTITION BY test1) FROM test;
SELECT rank() OVER (PARTITION BY test1 ORDER BY test2) FROM test;
# Placeholders: PostgreSQL $N and SQLite :name (Poco fork extension)
SELECT * FROM t WHERE a = $1 AND b = $2;
SELECT * FROM t WHERE a = :user AND b = :id;
SELECT * FROM t WHERE a = ? AND b = :id AND c = $3;
INSERT INTO foo VALUES (?);
EXECUTE statement_a(?);
# T-SQL dialect (Poco fork extension)
SELECT TOP (10) * FROM test;
SELECT TOP (10) a, b FROM test ORDER BY a;
SELECT * FROM test ORDER BY id OFFSET 0 ROWS FETCH NEXT 10 ROWS ONLY;
SELECT * FROM test ORDER BY id OFFSET 20 ROWS FETCH NEXT 5 ROWS ONLY;
SELECT * FROM [test];
SELECT [my col] FROM [some schema].[my table];
SELECT * FROM mydb.some_schema.test;
SELECT ARRAY[foo] FROM test;
# T-SQL sequence expression
SELECT NEXT VALUE FOR seq;
SELECT NEXT VALUE FOR mydb.dbo.seq AS next_id;
SELECT COALESCE(MAX(seq_value) + 1, 1) value FROM test;
# ODBC escape sequences
SELECT * FROM {oj test LEFT OUTER JOIN other ON test.id = other.id};
{? = call some_proc(?, 'a', 1)};
{call some_proc(?)};
{ ? = call [mydb].[dbo].[some_proc]('a') };
# TOP with an expression
SELECT TOP (?) * FROM test;
SELECT TOP (:limit) * FROM test;
SELECT TOP (n + 1) * FROM test;
# Oracle hierarchical query
SELECT LEVEL FROM dual CONNECT BY LEVEL <= 5;
SELECT id FROM test START WITH id = 1 CONNECT BY PRIOR id = parent_id;
SELECT id FROM test CONNECT BY PRIOR id = parent_id;
# Ordered-set aggregate (WITHIN GROUP)
SELECT LISTAGG(a, ', ') WITHIN GROUP (ORDER BY b) FROM test;
SELECT LISTAGG(a, ', ') WITHIN GROUP (ORDER BY b DESC, c ASC) AS lst FROM test;
# Oracle outer join marker
SELECT * FROM test, other WHERE test.id (+) = other.id;
SELECT * FROM test, other WHERE test.id = other.id (+);
SELECT * FROM test, other WHERE test.id(+)=other.id AND test.x > 5;
# Table value constructor in FROM
SELECT * FROM (VALUES (0, 'Any'), (1, 'One')) AS v(id, name);
SELECT * FROM (VALUES (1)) AS v(x);
# Table-valued function in FROM
SELECT value FROM STRING_SPLIT('a,b', ',');
SELECT * FROM STRING_SPLIT('a,b', ',') AS s;
SELECT * FROM dbo.fn_split('a', ',') f;
# Row constructor on the left of IN
SELECT * FROM test WHERE (a, b) IN (SELECT x, y FROM other);
SELECT * FROM test WHERE (a, b, c) NOT IN (SELECT x, y, z FROM other);
# Multi-part column references
SELECT dbo.test.a, mydb.dbo.test.b FROM mydb.dbo.test;
SELECT dbo.test.*, mydb.dbo.test.* FROM mydb.dbo.test;
SELECT * FROM [mydb].[dbo].test INNER JOIN [mydb].[dbo].other ON [mydb].[dbo].test.id = [mydb].[dbo].other.id;
# String literal used as an alias
SELECT a AS 'Coil Id', b AS 'X' FROM test;
SELECT * FROM test AS 'my table';
# Non-reserved keywords used as names
SELECT * FROM test WHERE created >= DATEADD(MINUTE, -10, GETDATE());
SELECT DATEDIFF(SECOND, a, b), DATEADD(year, -1, GETDATE()) FROM test;
SELECT ISNULL(a, 0), CHAR(10), FORMAT(a, '00') FROM test;
SELECT CONVERT(INT, a), CONVERT(DATETIME, b) FROM test;
SELECT YEAR, MONTH FROM test ORDER BY YEAR;
# Scientific notation numeric literals
SELECT 1.5e3 FROM test;
SELECT 1.5E+3, 5e-1, .5e1, 2e10 FROM test;
SELECT * FROM test WHERE a = 0.5e-2;
# Non-reserved words usable as ordinary names
SELECT * FROM start;
SELECT start.a FROM start;
UPDATE test SET start = 1;
INSERT INTO test (start, connect) VALUES (1, 2);
CREATE TABLE test (start INT);
SELECT a AS within FROM test;
SELECT * FROM test AS connect;
SELECT test.YEAR, dbo.test.MONTH FROM dbo.test;
SELECT a AS year FROM test AS next;
SELECT * FROM year.test;
# Row constructor against a literal list
SELECT * FROM test WHERE (a, b) IN ((1, 2), (3, 4));
SELECT * FROM test WHERE (a, b, c) NOT IN ((1, 2, 3), (4, 5, 6));
# Hierarchical clauses in either order, with and without NOCYCLE
SELECT id FROM test CONNECT BY PRIOR id = parent_id START WITH id = 1;
SELECT id FROM test CONNECT BY NOCYCLE PRIOR id = parent_id;
SELECT id FROM test CONNECT BY NOCYCLE PRIOR id = parent_id START WITH id = 1;
SELECT * FROM test CONNECT BY nocycleflag = 1;
# Ordered-set aggregates and window clauses on every call form
SELECT dbo.LISTAGG(a, ', ') WITHIN GROUP (ORDER BY b) FROM test;
SELECT FORMAT(a) WITHIN GROUP (ORDER BY b) FROM test;
SELECT RANK() OVER (PARTITION BY a) FROM test;
# Table-valued function keeps its schema
SELECT * FROM dbo.fn_split('a', ',') f;
# ODBC call escape with a return marker
{? = call some_proc(?, ?)};
# Parenthesized query expressions in set operations, at any nesting depth
SELECT 1 UNION SELECT 1;
(SELECT 1 UNION SELECT 1);
((SELECT 1 UNION SELECT 1));
(SELECT 1) UNION SELECT 1;
((SELECT 1)) UNION SELECT 1;
((SELECT 1) UNION SELECT 1);
(((SELECT 1) UNION SELECT 1));
(((SELECT 1)) UNION SELECT 1);
((((SELECT 1)) UNION SELECT 1));
SELECT 1 UNION (SELECT 1);
(SELECT 1 UNION ((SELECT 1)));
((SELECT 1 UNION ((SELECT 1))));
SELECT 1 UNION ((SELECT 1));
(SELECT 1) UNION (SELECT 1);
((SELECT 1) UNION (SELECT 1));
(((SELECT 1) UNION (SELECT 1)));
((SELECT 1)) UNION (SELECT 1);
(((SELECT 1)) UNION (SELECT 1));
((((SELECT 1)) UNION (SELECT 1)));
(SELECT 1) UNION ((SELECT 1));
((SELECT 1) UNION ((SELECT 1)));
(((SELECT 1) UNION ((SELECT 1))));
((SELECT 1)) UNION ((SELECT 1));
(((SELECT 1)) UNION ((SELECT 1)));
((((SELECT 1)) UNION ((SELECT 1))));
# Set operations inside a scalar subquery
SELECT (SELECT 1  UNION SELECT 1);
SELECT ((SELECT 1) UNION SELECT 1);
SELECT (((SELECT 1)) UNION SELECT 1);
SELECT (SELECT 1 UNION (SELECT 1));
SELECT (SELECT 1 UNION ((SELECT 1)));
SELECT ((SELECT 1 UNION ((SELECT 1))));
SELECT ((SELECT 1) UNION (SELECT 1));
SELECT (((SELECT 1) UNION (SELECT 1)));
SELECT ((((SELECT 1)) UNION ((SELECT 1))));
# Clauses after a parenthesized query expression, and every place that takes one
(SELECT 1 UNION SELECT 1) ORDER BY 1;
(SELECT a FROM t) UNION (SELECT b FROM u) ORDER BY 1 LIMIT 5;
(SELECT a FROM t ORDER BY a LIMIT 1) UNION (SELECT b FROM u ORDER BY b LIMIT 1);
SELECT * FROM ((SELECT 1) UNION (SELECT 2)) AS x;
SELECT * FROM (SELECT 1 UNION SELECT 2) AS x;
WITH c AS (SELECT 1) (SELECT * FROM c) UNION (SELECT 2);
WITH c AS (SELECT 1) SELECT * FROM c UNION SELECT 2;
INSERT INTO t SELECT 1 UNION SELECT 2;
INSERT INTO t (SELECT 1) UNION (SELECT 2);
SELECT * FROM t WHERE a IN ((SELECT x FROM u) UNION (SELECT y FROM v));
SELECT 1 INTERSECT (SELECT 1) EXCEPT (SELECT 2);
(SELECT 1) FOR UPDATE;
((SELECT 1 ORDER BY 1 LIMIT 1 FOR UPDATE));
(SELECT 1) UNION (SELECT 2) FOR UPDATE SKIP LOCKED;
SELECT * FROM (WITH c AS (SELECT 1) SELECT * FROM c) AS x;
# Parenthesized members with subqueries, joins, WITH, placeholders and TOP
(SELECT t.a, u.b FROM t JOIN u ON t.id = u.id WHERE t.c > ? ORDER BY t.a LIMIT 10) UNION ALL (SELECT a, b FROM v WHERE d = $1) ORDER BY 1 DESC LIMIT 5 OFFSET 2;
(SELECT TOP 5 a FROM t ORDER BY a) UNION (SELECT TOP (:n) b FROM u) EXCEPT (SELECT c FROM w WHERE c IN ((SELECT x FROM x1) INTERSECT (SELECT y FROM y1)));
WITH recent AS ((SELECT id FROM orders WHERE day > :since) UNION (SELECT id FROM returns)) SELECT * FROM recent WHERE EXISTS ((SELECT 1 FROM items WHERE items.id = recent.id) UNION (SELECT 1 FROM gifts WHERE gifts.id = recent.id));
SELECT * FROM t WHERE (a, b) IN ((SELECT x, y FROM u) UNION (SELECT x, y FROM v)) AND c NOT IN ((SELECT z FROM w) EXCEPT (SELECT z FROM w2));
SELECT * FROM (WITH c AS (SELECT 1 AS a) (SELECT a FROM c) UNION (SELECT 2)) AS x JOIN ((SELECT 3 AS b) UNION (SELECT 4)) AS y ON x.a = y.b;
INSERT INTO t (a, b) (SELECT x, y FROM u WHERE x = $1) UNION (SELECT x, y FROM v WHERE y = $2) ORDER BY 1 LIMIT 100;
CREATE TABLE t AS (SELECT 1) UNION (SELECT 2);
CREATE VIEW v AS (SELECT a FROM t) INTERSECT (SELECT a FROM u) ORDER BY 1;
COPY ((SELECT a FROM t) UNION (SELECT b FROM u)) TO 'file_path' WITH (FORMAT CSV);
# A WITH clause in a nested query expression, scoped to it
SELECT * FROM t WHERE a IN (WITH c AS (SELECT 1) SELECT * FROM c);
SELECT * FROM t WHERE EXISTS (WITH c AS (SELECT 1) SELECT * FROM c);
SELECT (WITH c AS (SELECT 1) SELECT * FROM c);
COPY (WITH c AS (SELECT 1) SELECT * FROM c) TO 'file_path';
INSERT INTO t WITH c AS (SELECT 1) SELECT * FROM c;
INSERT INTO t (a) (WITH c AS (SELECT 1) SELECT * FROM c);
SELECT 1 UNION (WITH c AS (SELECT 2) SELECT * FROM c);
SELECT (SELECT COUNT(*) FROM ((SELECT a FROM t) UNION ALL (SELECT b FROM u)) AS c) AS n FROM d;
SELECT 1 UNION (SELECT 2 UNION SELECT 3 ORDER BY 1 LIMIT 1) UNION SELECT 4 ORDER BY 1 FOR UPDATE;
