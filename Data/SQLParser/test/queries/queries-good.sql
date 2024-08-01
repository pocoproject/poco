# This file contains a list of strings that are NOT valid SQL queries.
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
# JOIN
SELECT t1.a, t1.b, t2.c FROM "table" AS t1 JOIN (SELECT * FROM foo JOIN bar ON foo.id = bar.id) t2 ON t1.a = t2.b WHERE (t1.b OR NOT t1.a) AND t2.c = 12.5
SELECT * FROM t1 JOIN t2 ON c1 = c2;
SELECT a, SUM(b) FROM t2 GROUP BY a HAVING SUM(b) > 100;
# CREATE statement
CREATE TABLE "table" FROM TBL FILE 'students.tbl'
CREATE TABLE IF NOT EXISTS "table" FROM TBL FILE 'students.tbl'
CREATE TABLE students (name TEXT, student_number INTEGER, city TEXT, grade DOUBLE, credits BIGINT)
CREATE TABLE students (name TEXT, student_number INTEGER NOT NULL, city TEXT, grade DOUBLE PRIMARY KEY UNIQUE)
CREATE TABLE teachers (name VARCHAR(30), student_number LONG, city CHAR(10), grade FLOAT)
CREATE TABLE teachers (name VARCHAR(30), student_number LONG, PRIMARY KEY (name, student_number), city CHAR(10), grade FLOAT)
CREATE TABLE teachers (name CHARACTER VARYING(30));
CREATE TABLE students_2 AS SELECT * FROM students
CREATE TABLE students_3 AS SELECT city, grade FROM students WHERE grade > 3.0
CREATE TABLE students (date_of_birth DATE, matriculation_date DATETIME, graduation_date TIMESTAMP, graduated BOOLEAN);
# Multiple statements
CREATE TABLE "table" FROM TBL FILE 'students.tbl'; SELECT * FROM "table";
# INSERT
INSERT INTO test_table VALUES (1, 2, 'test');
INSERT INTO test_table (id, value, name) VALUES (1, 2, 'test');
INSERT INTO test_table SELECT * FROM students;
INSERT INTO some_schema.test_table SELECT * FROM another_schema.students;
# DELETE
DELETE FROM students WHERE grade > 3.0
DELETE FROM students
TRUNCATE students
# UPDATE
UPDATE students SET grade = 1.3 WHERE name = 'Max Mustermann';
UPDATE students SET grade = 1.3, name='Felix Fürstenberg' WHERE name = 'Max Mustermann';
UPDATE students SET grade = 1.0;
UPDATE some_schema.students SET grade = 1.0;
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
EXECUTE prep_inst(1, 2, 3);
EXECUTE prep;
DEALLOCATE PREPARE prep;
# COPY
COPY students FROM 'student.tbl';
COPY students FROM 'file_path' WITH FORMAT TBL;
COPY students FROM 'file_path' WITH FORMAT CSV;
COPY students FROM 'file_path' WITH FORMAT BIN;
COPY students FROM 'file_path' WITH FORMAT BINARY;
COPY good_students FROM 'file_path' WHERE grade > (SELECT AVG(grade) from alumni);
COPY students TO 'student.tbl';
COPY students TO 'file_path' WITH FORMAT TBL;
COPY students TO 'file_path' WITH FORMAT CSV;
COPY students TO 'file_path' WITH FORMAT BIN;
COPY students TO 'file_path' WITH FORMAT BINARY;
COPY (SELECT firstname, COUNT(*) FROM students GROUP BY firstname) TO 'student_names.csv';
# HINTS
SELECT * FROM test WITH HINT(NO_CACHE);
SELECT * FROM test WITH HINT(NO_CACHE, NO_SAMPLING);
SELECT * FROM test WITH HINT(NO_CACHE, SAMPLE_RATE(0.1), OMW(1.0, 'test'));
SHOW TABLES;
SHOW COLUMNS students;
DESCRIBE students;
SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '30 DAYS';
SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '10' DAY;
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
