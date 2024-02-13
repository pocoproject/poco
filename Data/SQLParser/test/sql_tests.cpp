/*
 * sql_tests.cpp
 */

#include "thirdparty/microtest/microtest.h"

#include "SQLParser.h"
#include "util/sqlhelper.h"

#include "sql_asserts.h"

using namespace hsql;

TEST(DeleteStatementTest) {
  SQLParserResult result;
  SQLParser::parse("DELETE FROM students WHERE grade > 2.0;", &result);

  ASSERT(result.isValid());
  ASSERT_EQ(result.size(), 1);
  ASSERT(result.getStatement(0)->type() == kStmtDelete);

  const DeleteStatement* stmt = (const DeleteStatement*)result.getStatement(0);
  ASSERT_STREQ(stmt->tableName, "students");
  ASSERT_NOTNULL(stmt->expr);
  ASSERT(stmt->expr->isType(kExprOperator));
  ASSERT_STREQ(stmt->expr->expr->name, "grade");
  ASSERT_EQ(stmt->expr->expr2->fval, 2.0);
}

TEST(CreateStatementTest) {
  SQLParserResult result;
  SQLParser::parse(
      "CREATE TABLE dummy_table ("
      "  c_bigint BIGINT, "
      "  c_boolean BOOLEAN, "
      "  c_char CHAR(42), "
      "  c_date DATE, "
      "  c_datetime DATETIME, "
      "  c_decimal DECIMAL, "
      "  c_decimal_precision DECIMAL(13), "
      "  c_decimal_precision_scale DECIMAL(13,37), "
      "  c_double_not_null DOUBLE NOT NULL, "
      "  c_float FLOAT, "
      "  c_int INT, "
      "  PRIMARY KEY(c_char, c_int), "
      "  c_integer_null INTEGER NULL, "
      "  c_long LONG, "
      "  c_real REAL, "
      "  c_smallint SMALLINT, "
      "  c_text TEXT UNIQUE PRIMARY KEY NOT NULL, "
      "  c_time TIME, "
      "  c_time_precision TIME(17), "
      "  c_timestamp TIMESTAMP, "
      "  c_varchar VARCHAR(50), "
      "  c_char_varying CHARACTER VARYING(60)"
      ")",
      &result);
  ASSERT(result.isValid());
  ASSERT_EQ(result.size(), 1);
  ASSERT_EQ(result.getStatement(0)->type(), kStmtCreate);

  const CreateStatement* stmt = (const CreateStatement*)result.getStatement(0);
  ASSERT_EQ(stmt->type, kCreateTable);
  ASSERT_STREQ(stmt->tableName, "dummy_table");
  ASSERT_NOTNULL(stmt->columns);
  ASSERT_EQ(stmt->columns->size(), 21);
  // c_bigint BIGINT
  ASSERT_STREQ(stmt->columns->at(0)->name, "c_bigint");
  ASSERT_EQ(stmt->columns->at(0)->type, (ColumnType{DataType::BIGINT}));
  ASSERT_EQ(stmt->columns->at(0)->nullable, true);
  // c_boolean BOOLEAN
  ASSERT_STREQ(stmt->columns->at(1)->name, "c_boolean");
  ASSERT_EQ(stmt->columns->at(1)->type, (ColumnType{DataType::BOOLEAN}));
  ASSERT_EQ(stmt->columns->at(1)->nullable, true);
  // c_char CHAR(42)
  ASSERT_STREQ(stmt->columns->at(2)->name, "c_char");
  ASSERT_EQ(stmt->columns->at(2)->type, (ColumnType{DataType::CHAR, 42}));
  ASSERT_NEQ(stmt->columns->at(2)->type, (ColumnType{DataType::CHAR, 43}));
  ASSERT_EQ(stmt->columns->at(2)->nullable, true);
  // c_date DATE
  ASSERT_STREQ(stmt->columns->at(3)->name, "c_date");
  ASSERT_EQ(stmt->columns->at(3)->type, (ColumnType{DataType::DATE}));
  ASSERT_EQ(stmt->columns->at(3)->nullable, true);
  // c_datetime DATETIME
  ASSERT_STREQ(stmt->columns->at(4)->name, "c_datetime");
  ASSERT_EQ(stmt->columns->at(4)->type, (ColumnType{DataType::DATETIME}));
  ASSERT_EQ(stmt->columns->at(4)->nullable, true);
  // c_decimal DECIMAL
  ASSERT_STREQ(stmt->columns->at(5)->name, "c_decimal");
  ASSERT_EQ(stmt->columns->at(5)->type, (ColumnType{DataType::DECIMAL}));
  ASSERT_EQ(stmt->columns->at(5)->nullable, true);
  // c_decimal_precision DECIMAL(13)
  ASSERT_STREQ(stmt->columns->at(6)->name, "c_decimal_precision");
  ASSERT_EQ(stmt->columns->at(6)->type, (ColumnType{DataType::DECIMAL, 0, 13}));
  ASSERT_NEQ(stmt->columns->at(6)->type, (ColumnType{DataType::DECIMAL, 0, 14}));
  ASSERT_NEQ(stmt->columns->at(6)->type, (ColumnType{DataType::DECIMAL, 1, 13}));
  ASSERT_EQ(stmt->columns->at(6)->nullable, true);
  // c_decimal_precision_scale DECIMAL(13,37)
  ASSERT_STREQ(stmt->columns->at(7)->name, "c_decimal_precision_scale");
  ASSERT_EQ(stmt->columns->at(7)->type, (ColumnType{DataType::DECIMAL, 0, 13, 37}));
  ASSERT_NEQ(stmt->columns->at(7)->type, (ColumnType{DataType::DECIMAL, 0, 14, 37}));
  ASSERT_NEQ(stmt->columns->at(7)->type, (ColumnType{DataType::DECIMAL, 0, 13, 38}));
  ASSERT_NEQ(stmt->columns->at(7)->type, (ColumnType{DataType::DECIMAL, 1, 13, 37}));
  ASSERT_EQ(stmt->columns->at(7)->nullable, true);
  // c_double_not_null DOUBLE NOT NULL
  ASSERT_STREQ(stmt->columns->at(8)->name, "c_double_not_null");
  ASSERT_EQ(stmt->columns->at(8)->type, (ColumnType{DataType::DOUBLE}));
  ASSERT_EQ(stmt->columns->at(8)->nullable, false);
  ASSERT_EQ(stmt->columns->at(8)->column_constraints->size(), 1);
  ASSERT(stmt->columns->at(8)->column_constraints->count(ConstraintType::NotNull));
  // c_float FLOAT
  ASSERT_STREQ(stmt->columns->at(9)->name, "c_float");
  ASSERT_EQ(stmt->columns->at(9)->type, (ColumnType{DataType::FLOAT}));
  ASSERT_EQ(stmt->columns->at(9)->nullable, true);
  // c_int INT
  ASSERT_STREQ(stmt->columns->at(10)->name, "c_int");
  ASSERT_EQ(stmt->columns->at(10)->type, (ColumnType{DataType::INT}));
  ASSERT_EQ(stmt->columns->at(10)->nullable, true);
  // c_integer INTEGER NULL
  ASSERT_STREQ(stmt->columns->at(11)->name, "c_integer_null");
  ASSERT_EQ(stmt->columns->at(11)->type, (ColumnType{DataType::INT}));
  ASSERT_EQ(stmt->columns->at(11)->nullable, true);
  ASSERT_EQ(stmt->columns->at(11)->column_constraints->size(), 1);
  ASSERT(stmt->columns->at(11)->column_constraints->count(ConstraintType::Null));
  // c_long LONG
  ASSERT_STREQ(stmt->columns->at(12)->name, "c_long");
  ASSERT_EQ(stmt->columns->at(12)->type, (ColumnType{DataType::LONG}));
  ASSERT_EQ(stmt->columns->at(12)->nullable, true);
  // c_real REAL
  ASSERT_STREQ(stmt->columns->at(13)->name, "c_real");
  ASSERT_EQ(stmt->columns->at(13)->type, (ColumnType{DataType::REAL}));
  ASSERT_EQ(stmt->columns->at(13)->nullable, true);
  // c_smallint SMALLINT
  ASSERT_STREQ(stmt->columns->at(14)->name, "c_smallint");
  ASSERT_EQ(stmt->columns->at(14)->type, (ColumnType{DataType::SMALLINT}));
  ASSERT_EQ(stmt->columns->at(14)->nullable, true);
  // c_text TEXT UNIQUE PRIMARY KEY NOT NULL
  ASSERT_STREQ(stmt->columns->at(15)->name, "c_text");
  ASSERT_EQ(stmt->columns->at(15)->type, (ColumnType{DataType::TEXT}));
  ASSERT_EQ(stmt->columns->at(15)->nullable, false);
  // Expecting two elements in column_constraints since information about NULL constraints is separately stored in
  // ColumnDefinition::nullable
  ASSERT_EQ(stmt->columns->at(15)->column_constraints->size(), 3);
  ASSERT(stmt->columns->at(15)->column_constraints->count(ConstraintType::Unique));
  ASSERT(stmt->columns->at(15)->column_constraints->count(ConstraintType::PrimaryKey));
  ASSERT(stmt->columns->at(15)->column_constraints->count(ConstraintType::NotNull));
  // c_time TIME
  ASSERT_STREQ(stmt->columns->at(16)->name, "c_time");
  ASSERT_EQ(stmt->columns->at(16)->type, (ColumnType{DataType::TIME}));
  ASSERT_EQ(stmt->columns->at(16)->nullable, true);
  // c_time_precision TIME(17)
  ASSERT_STREQ(stmt->columns->at(17)->name, "c_time_precision");
  ASSERT_EQ(stmt->columns->at(17)->type, (ColumnType{DataType::TIME, 0, 17}));
  ASSERT_NEQ(stmt->columns->at(17)->type, (ColumnType{DataType::TIME, 0, 18}));
  ASSERT_NEQ(stmt->columns->at(17)->type, (ColumnType{DataType::TIME, 1, 17}));
  ASSERT_EQ(stmt->columns->at(17)->nullable, true);
  // c_timestamp TIMESTAMP
  ASSERT_STREQ(stmt->columns->at(18)->name, "c_timestamp");
  ASSERT_EQ(stmt->columns->at(18)->type, (ColumnType{DataType::DATETIME}));
  ASSERT_EQ(stmt->columns->at(18)->nullable, true);
  // c_varchar VARCHAR(50)
  ASSERT_STREQ(stmt->columns->at(19)->name, "c_varchar");
  ASSERT_EQ(stmt->columns->at(19)->type, (ColumnType{DataType::VARCHAR, 50}));
  ASSERT_NEQ(stmt->columns->at(19)->type, (ColumnType{DataType::VARCHAR, 51}));
  ASSERT_EQ(stmt->columns->at(19)->nullable, true);
  // c_char_varying CHARACTER VARYING(60)
  ASSERT_STREQ(stmt->columns->at(20)->name, "c_char_varying");
  ASSERT_EQ(stmt->columns->at(20)->type, (ColumnType{DataType::VARCHAR, 60}));
  ASSERT_NEQ(stmt->columns->at(20)->type, (ColumnType{DataType::VARCHAR, 61}));
  // Table constraints are identified and separated during the parsing of the SQL string
  // Table constraints:
  // - PRIMARY KEY(c_char, c_int)
  ASSERT_EQ(stmt->tableConstraints->size(), 1);
  ASSERT(stmt->tableConstraints->at(0)->type == ConstraintType::PrimaryKey);
  ASSERT_STREQ(stmt->tableConstraints->at(0)->columnNames->at(0), "c_char");
  ASSERT_STREQ(stmt->tableConstraints->at(0)->columnNames->at(1), "c_int");
}

TEST(CreateAsSelectStatementTest) {
  SQLParserResult result;
  SQLParser::parse("CREATE TABLE students_2 AS SELECT student_number, grade FROM students", &result);

  ASSERT(result.isValid());
  ASSERT_EQ(result.size(), 1);
  ASSERT_EQ(result.getStatement(0)->type(), kStmtCreate);

  const CreateStatement* stmt = (const CreateStatement*)result.getStatement(0);
  ASSERT_EQ(stmt->type, kCreateTable);
  ASSERT_STREQ(stmt->tableName, "students_2");
  ASSERT_NULL(stmt->columns);
  ASSERT_NOTNULL(stmt->select);
  ASSERT(stmt->select->selectList->at(0)->isType(kExprColumnRef));
  ASSERT_STREQ(stmt->select->selectList->at(0)->getName(), "student_number");
  ASSERT(stmt->select->selectList->at(1)->isType(kExprColumnRef));
  ASSERT_STREQ(stmt->select->selectList->at(1)->getName(), "grade");
}

TEST(UpdateStatementTest) {
  SQLParserResult result;
  SQLParser::parse("UPDATE students SET grade = 5.0, name = 'test' WHERE name = 'Max O''Mustermann';", &result);

  ASSERT(result.isValid());
  ASSERT_EQ(result.size(), 1);
  ASSERT_EQ(result.getStatement(0)->type(), kStmtUpdate);

  const UpdateStatement* stmt = (const UpdateStatement*)result.getStatement(0);
  ASSERT_NOTNULL(stmt->table);
  ASSERT_STREQ(stmt->table->name, "students");

  ASSERT_NOTNULL(stmt->updates);
  ASSERT_EQ(stmt->updates->size(), 2);
  ASSERT_STREQ(stmt->updates->at(0)->column, "grade");
  ASSERT_STREQ(stmt->updates->at(1)->column, "name");
  ASSERT(stmt->updates->at(0)->value->isType(kExprLiteralFloat));
  ASSERT(stmt->updates->at(1)->value->isType(kExprLiteralString));
  ASSERT_EQ(stmt->updates->at(0)->value->fval, 5.0);
  ASSERT_STREQ(stmt->updates->at(1)->value->name, "test");

  ASSERT_NOTNULL(stmt->where);
  ASSERT(stmt->where->isType(kExprOperator));
  ASSERT_EQ(stmt->where->opType, kOpEquals);
  ASSERT_STREQ(stmt->where->expr->name, "name");
  ASSERT_STREQ(stmt->where->expr2->name, "Max O'Mustermann");
}

TEST(InsertStatementTest) {
  TEST_PARSE_SINGLE_SQL("INSERT INTO students VALUES ('Max Mustermann', 12345, 'Musterhausen', 2.0)", kStmtInsert,
                        InsertStatement, result, stmt);

  ASSERT_EQ(stmt->values->size(), 4);
  // TODO
}

TEST(AlterStatementDropActionTest) {
  SQLParserResult result;
  SQLParser::parse("ALTER TABLE mytable DROP COLUMN IF EXISTS mycolumn", &result);

  ASSERT(result.isValid());
  ASSERT_EQ(result.size(), 1);

  const AlterStatement* stmt = (const AlterStatement*)result.getStatement(0);
  ASSERT_STREQ(stmt->name, "mytable");
  ASSERT_EQ(stmt->ifTableExists, false);

  auto dropAction = (const DropColumnAction*)stmt->action;

  ASSERT_EQ(dropAction->type, hsql::ActionType::DropColumn);
  ASSERT_STREQ(dropAction->columnName, "mycolumn");
}

TEST(CreateIndexStatementTest) {
  SQLParserResult result;
  SQLParser::parse("CREATE INDEX myindex ON myTable (col1);", &result);

  ASSERT(result.isValid());
  ASSERT_EQ(result.size(), 1);

  const CreateStatement* stmt = (const CreateStatement*)result.getStatement(0);
  ASSERT_STREQ(stmt->indexName, "myindex");
  ASSERT_STREQ(stmt->tableName, "myTable");
  ASSERT_EQ(stmt->type, kCreateIndex);
  ASSERT_EQ(stmt->ifNotExists, false);
  ASSERT_EQ(stmt->indexColumns->size(), 1);
}

TEST(CreateIndexStatementIfNotExistsTest) {
  SQLParserResult result;
  SQLParser::parse("CREATE INDEX IF NOT EXISTS myindex ON myTable (col1, col2);", &result);

  ASSERT(result.isValid());
  ASSERT_EQ(result.size(), 1);

  const CreateStatement* stmt = (const CreateStatement*)result.getStatement(0);
  ASSERT_STREQ(stmt->indexName, "myindex");
  ASSERT_STREQ(stmt->tableName, "myTable");
  ASSERT_EQ(stmt->type, kCreateIndex);
  ASSERT_EQ(stmt->ifNotExists, true);
  ASSERT_EQ(stmt->indexColumns->size(), 2);
}

TEST(DropIndexTest) {
  SQLParserResult result;
  SQLParser::parse("DROP INDEX myindex", &result);

  ASSERT(result.isValid());
  ASSERT_EQ(result.size(), 1);

  const DropStatement* stmt = (const DropStatement*)result.getStatement(0);
  ASSERT_STREQ(stmt->indexName, "myindex");
  ASSERT_EQ(stmt->ifExists, false);
}

TEST(DropIndexIfExistsTest) {
  SQLParserResult result;
  SQLParser::parse("DROP INDEX IF EXISTS myindex", &result);

  ASSERT(result.isValid());
  ASSERT_EQ(result.size(), 1);

  const DropStatement* stmt = (const DropStatement*)result.getStatement(0);
  ASSERT_STREQ(stmt->indexName, "myindex");
  ASSERT_EQ(stmt->ifExists, true);
}

TEST(DropTableStatementTest) {
  TEST_PARSE_SINGLE_SQL("DROP TABLE students", kStmtDrop, DropStatement, result, stmt);

  ASSERT_FALSE(stmt->ifExists);
  ASSERT_EQ(stmt->type, kDropTable);
  ASSERT_NOTNULL(stmt->name);
  ASSERT_STREQ(stmt->name, "students");
}

TEST(DropTableIfExistsStatementTest) {
  TEST_PARSE_SINGLE_SQL("DROP TABLE IF EXISTS students", kStmtDrop, DropStatement, result, stmt);

  ASSERT_TRUE(stmt->ifExists);
  ASSERT_EQ(stmt->type, kDropTable);
  ASSERT_NOTNULL(stmt->name);
  ASSERT_STREQ(stmt->name, "students");
}

TEST(ReleaseStatementTest) {
  TEST_PARSE_SINGLE_SQL("SELECT * FROM students;", kStmtSelect, SelectStatement, result, stmt);

  ASSERT_EQ(1, result.size());
  ASSERT_NULL(stmt->whereClause);

  std::vector<SQLStatement*> statements = result.releaseStatements();

  ASSERT_EQ(0, result.size());

  for (SQLStatement* stmt : statements) {
    delete stmt;
  }
}

TEST(ShowTableStatementTest) {
  TEST_PARSE_SINGLE_SQL("SHOW TABLES;", kStmtShow, ShowStatement, result, stmt);

  ASSERT_EQ(stmt->type, kShowTables);
  ASSERT_NULL(stmt->name);
}

TEST(ShowColumnsStatementTest) {
  TEST_PARSE_SINGLE_SQL("SHOW COLUMNS students;", kStmtShow, ShowStatement, result, stmt);

  ASSERT_EQ(stmt->type, kShowColumns);
  ASSERT_NOTNULL(stmt->name);
  ASSERT_STREQ(stmt->name, "students");
}

TEST(DescribeStatementTest) {
  TEST_PARSE_SINGLE_SQL("DESCRIBE students;", kStmtShow, ShowStatement, result, stmt);

  ASSERT_EQ(stmt->type, kShowColumns);
  ASSERT_NOTNULL(stmt->name);
  ASSERT_STREQ(stmt->name, "students");
}

TEST(ImportStatementTest) {
  TEST_PARSE_SINGLE_SQL("IMPORT FROM TBL FILE 'students_file' INTO students;", kStmtImport, ImportStatement, result,
                        stmt);

  ASSERT_EQ(stmt->type, kImportTbl);
  ASSERT_NOTNULL(stmt->tableName);
  ASSERT_STREQ(stmt->tableName, "students");
  ASSERT_STREQ(stmt->filePath, "students_file");
}

TEST(CopyStatementTest) {
  TEST_PARSE_SINGLE_SQL("COPY students FROM 'students_file' WITH FORMAT BINARY;", kStmtImport, ImportStatement,
                        import_result, import_stmt);

  ASSERT_EQ(import_stmt->type, kImportBinary);
  ASSERT_NOTNULL(import_stmt->tableName);
  ASSERT_STREQ(import_stmt->tableName, "students");
  ASSERT_NOTNULL(import_stmt->filePath);
  ASSERT_STREQ(import_stmt->filePath, "students_file");
  ASSERT_NULL(import_stmt->whereClause);

  TEST_PARSE_SINGLE_SQL("COPY students FROM 'students_file' WHERE lastname = 'Potter';", kStmtImport, ImportStatement,
                        import_filter_result, import_filter_stmt);

  ASSERT_EQ(import_filter_stmt->type, kImportAuto);
  ASSERT_NOTNULL(import_filter_stmt->tableName);
  ASSERT_STREQ(import_filter_stmt->tableName, "students");
  ASSERT_NOTNULL(import_filter_stmt->filePath);
  ASSERT_STREQ(import_filter_stmt->filePath, "students_file");
  ASSERT_NOTNULL(import_filter_stmt->whereClause);
  ASSERT_EQ(import_filter_stmt->whereClause->opType, kOpEquals);
  ASSERT_EQ(import_filter_stmt->whereClause->expr->type, kExprColumnRef);
  ASSERT_STREQ(import_filter_stmt->whereClause->expr->name, "lastname");
  ASSERT_EQ(import_filter_stmt->whereClause->expr2->type, kExprLiteralString);
  ASSERT_STREQ(import_filter_stmt->whereClause->expr2->name, "Potter");

  TEST_PARSE_SINGLE_SQL("COPY students TO 'students_file' WITH FORMAT CSV;", kStmtExport, ExportStatement,
                        export_table_result, export_table_stmt);

  ASSERT_EQ(export_table_stmt->type, kImportCSV);
  ASSERT_NOTNULL(export_table_stmt->tableName);
  ASSERT_STREQ(export_table_stmt->tableName, "students");
  ASSERT_NOTNULL(export_table_stmt->filePath);
  ASSERT_STREQ(export_table_stmt->filePath, "students_file");
  ASSERT_NULL(export_table_stmt->select);

  TEST_PARSE_SINGLE_SQL("COPY (SELECT firstname, lastname FROM students) TO 'students_file';", kStmtExport,
                        ExportStatement, export_select_result, export_select_stmt);

  ASSERT_EQ(export_select_stmt->type, kImportAuto);
  ASSERT_NULL(export_select_stmt->tableName);
  ASSERT_NOTNULL(export_select_stmt->filePath);
  ASSERT_STREQ(export_select_stmt->filePath, "students_file");

  ASSERT_NOTNULL(export_select_stmt->select);
  const auto& select_stmt = export_select_stmt->select;
  ASSERT_NULL(select_stmt->whereClause);
  ASSERT_NULL(select_stmt->groupBy);
  ASSERT_EQ(select_stmt->selectList->size(), 2);
  ASSERT(select_stmt->selectList->at(0)->isType(kExprColumnRef));
  ASSERT_STREQ(select_stmt->selectList->at(0)->getName(), "firstname");
  ASSERT(select_stmt->selectList->at(1)->isType(kExprColumnRef));
  ASSERT_STREQ(select_stmt->selectList->at(1)->getName(), "lastname");
  ASSERT_NOTNULL(select_stmt->fromTable);
  ASSERT_STREQ(select_stmt->fromTable->name, "students");
}

SQLParserResult parse_and_move(std::string query) {
  hsql::SQLParserResult result;
  hsql::SQLParser::parse(query, &result);
  // Moves on return.
  return result;
}

SQLParserResult move_in_and_back(SQLParserResult res) {
  // Moves on return.
  return res;
}

TEST(MoveSQLResultTest) {
  SQLParserResult res = parse_and_move("SELECT * FROM test;");
  ASSERT(res.isValid());
  ASSERT_EQ(1, res.size());

  // Moved around.
  SQLParserResult new_res = move_in_and_back(std::move(res));

  // Original object should be invalid.
  ASSERT_FALSE(res.isValid());
  ASSERT_EQ(0, res.size());

  ASSERT(new_res.isValid());
  ASSERT_EQ(1, new_res.size());
}

TEST(HintTest) {
  TEST_PARSE_SINGLE_SQL("SELECT * FROM students WITH HINT(NO_CACHE, SAMPLE_RATE(10));", kStmtSelect, SelectStatement,
                        result, stmt);

  ASSERT_NOTNULL(stmt->hints);
  ASSERT_EQ(2, stmt->hints->size());
  ASSERT_STREQ("NO_CACHE", stmt->hints->at(0)->name);
  ASSERT_STREQ("SAMPLE_RATE", stmt->hints->at(1)->name);
  ASSERT_EQ(1, stmt->hints->at(1)->exprList->size());
  ASSERT_EQ(10, stmt->hints->at(1)->exprList->at(0)->ival);
}

TEST(StringLengthTest) {
  TEST_PARSE_SQL_QUERY("SELECT * FROM bar; INSERT INTO foo VALUES (4);\t\n SELECT * FROM foo;", result, 3);

  ASSERT_EQ(result.getStatement(0)->stringLength, 18);
  ASSERT_EQ(result.getStatement(1)->stringLength, 28);
  ASSERT_EQ(result.getStatement(2)->stringLength, 21);
}

TEST(ExceptOperatorTest) {
  TEST_PARSE_SINGLE_SQL("SELECT * FROM students EXCEPT SELECT * FROM students_2;", kStmtSelect, SelectStatement, result,
                        stmt);

  ASSERT_STREQ(stmt->setOperations->back()->nestedSelectStatement->fromTable->name, "students_2");
  ASSERT_STREQ(stmt->fromTable->name, "students");
  ASSERT_EQ(stmt->setOperations->back()->setType, kSetExcept);
}

TEST(IntersectOperatorTest) {
  TEST_PARSE_SINGLE_SQL("SELECT * FROM students INTERSECT SELECT * FROM students_2;", kStmtSelect, SelectStatement,
                        result, stmt);

  ASSERT_STREQ(stmt->setOperations->back()->nestedSelectStatement->fromTable->name, "students_2");
  ASSERT_STREQ(stmt->fromTable->name, "students");
  ASSERT_EQ(stmt->setOperations->back()->setType, kSetIntersect);
}

TEST(UnionOperatorTest) {
  TEST_PARSE_SINGLE_SQL("SELECT * FROM students UNION SELECT * FROM students_2;", kStmtSelect, SelectStatement, result,
                        stmt);

  ASSERT_STREQ(stmt->setOperations->back()->nestedSelectStatement->fromTable->name, "students_2");
  ASSERT_STREQ(stmt->fromTable->name, "students");
  ASSERT_EQ(stmt->setOperations->back()->setType, kSetUnion);
  ASSERT_FALSE(stmt->setOperations->back()->isAll);
}

TEST(UnionAllOperatorTest) {
  TEST_PARSE_SINGLE_SQL("SELECT * FROM students UNION ALL SELECT * FROM students_2;", kStmtSelect, SelectStatement,
                        result, stmt);

  ASSERT_STREQ(stmt->setOperations->back()->nestedSelectStatement->fromTable->name, "students_2");
  ASSERT_STREQ(stmt->fromTable->name, "students");
  ASSERT_TRUE(stmt->setOperations->back()->isAll);
}

TEST(NestedSetOperationTest) {
  TEST_PARSE_SINGLE_SQL("SELECT * FROM students INTERSECT SELECT grade FROM students_2 UNION SELECT * FROM employees;",
                        kStmtSelect, SelectStatement, result, stmt);

  ASSERT_STREQ(
      stmt->setOperations->back()->nestedSelectStatement->setOperations->back()->nestedSelectStatement->fromTable->name,
      "employees");
  ASSERT_STREQ(stmt->setOperations->back()->nestedSelectStatement->fromTable->name, "students_2");
  ASSERT_STREQ(stmt->fromTable->name, "students");
  ASSERT_EQ(stmt->setOperations->back()->setType, kSetIntersect);
  ASSERT_EQ(stmt->setOperations->back()->nestedSelectStatement->setOperations->back()->setType, kSetUnion);
  ASSERT_FALSE(stmt->setOperations->back()->isAll);
}

TEST(OrderByFullStatementTest) {
  TEST_PARSE_SINGLE_SQL(
      "SELECT * FROM students INTERSECT SELECT grade FROM students_2 UNION SELECT * FROM employees ORDER BY grade ASC;",
      kStmtSelect, SelectStatement, result, stmt);

  ASSERT_EQ(stmt->setOperations->back()->resultOrder->at(0)->type, kOrderAsc);
  ASSERT_STREQ(stmt->setOperations->back()->resultOrder->at(0)->expr->name, "grade");
  ASSERT_FALSE(stmt->setOperations->back()->isAll);
}

TEST(SetOperationSubQueryOrder) {
  TEST_PARSE_SINGLE_SQL(
      "(SELECT * FROM students ORDER BY name DESC) INTERSECT SELECT grade FROM students_2 UNION SELECT * FROM "
      "employees ORDER BY grade ASC;",
      kStmtSelect, SelectStatement, result, stmt);

  ASSERT_EQ(stmt->order->at(0)->type, kOrderDesc);
  ASSERT_STREQ(stmt->order->at(0)->expr->name, "name");

  ASSERT_EQ(stmt->setOperations->back()->resultOrder->at(0)->type, kOrderAsc);
  ASSERT_STREQ(stmt->setOperations->back()->resultOrder->at(0)->expr->name, "grade");
  ASSERT_FALSE(stmt->setOperations->back()->isAll);
}

TEST(SetOperationLastSubQueryOrder) {
  TEST_PARSE_SINGLE_SQL(
      "SELECT * FROM students INTERSECT SELECT grade FROM students_2 UNION (SELECT * FROM employees ORDER BY name "
      "DESC) ORDER BY grade ASC;",
      kStmtSelect, SelectStatement, result, stmt);

  ASSERT_EQ(stmt->setOperations->back()
                ->nestedSelectStatement->setOperations->back()
                ->nestedSelectStatement->order->at(0)
                ->type,
            kOrderDesc);
  ASSERT_STREQ(stmt->setOperations->back()
                   ->nestedSelectStatement->setOperations->back()
                   ->nestedSelectStatement->order->at(0)
                   ->expr->name,
               "name");

  ASSERT_EQ(stmt->setOperations->back()->resultOrder->at(0)->type, kOrderAsc);
  ASSERT_STREQ(stmt->setOperations->back()->resultOrder->at(0)->expr->name, "grade");
  ASSERT_FALSE(stmt->setOperations->back()->isAll);
}

TEST(NestedDifferentSetOperationsWithWithClause) {
  TEST_PARSE_SINGLE_SQL(
      "WITH UNION_FIRST AS (SELECT * FROM A UNION SELECT * FROM B) SELECT * FROM UNION_FIRST EXCEPT SELECT * FROM C",
      kStmtSelect, SelectStatement, result, stmt);

  ASSERT_STREQ(stmt->withDescriptions->back()->alias, "UNION_FIRST");
  ASSERT_EQ(stmt->withDescriptions->back()->select->setOperations->back()->setType, kSetUnion);
  ASSERT_STREQ(stmt->withDescriptions->back()->select->fromTable->name, "A");
  ASSERT_STREQ(stmt->withDescriptions->back()->select->setOperations->back()->nestedSelectStatement->fromTable->name,
               "B");

  ASSERT_EQ(stmt->setOperations->back()->setType, kSetExcept);
  ASSERT_STREQ(stmt->fromTable->name, "UNION_FIRST");
  ASSERT_STREQ(stmt->setOperations->back()->nestedSelectStatement->fromTable->name, "C");
}

TEST(NestedAllSetOperationsWithWithClause) {
  TEST_PARSE_SINGLE_SQL(
      "WITH UNION_FIRST AS (SELECT * FROM A UNION SELECT * FROM B) SELECT * FROM UNION_FIRST EXCEPT SELECT * FROM "
      "(SELECT * FROM C INTERSECT SELECT * FROM D)",
      kStmtSelect, SelectStatement, result, stmt);

  ASSERT_STREQ(stmt->withDescriptions->back()->alias, "UNION_FIRST");
  ASSERT_EQ(stmt->withDescriptions->back()->select->setOperations->back()->setType, kSetUnion);
  ASSERT_STREQ(stmt->withDescriptions->back()->select->fromTable->name, "A");
  ASSERT_STREQ(stmt->withDescriptions->back()->select->setOperations->back()->nestedSelectStatement->fromTable->name,
               "B");

  ASSERT_EQ(stmt->setOperations->back()->setType, kSetExcept);
  ASSERT_STREQ(stmt->fromTable->name, "UNION_FIRST");
  ASSERT_EQ(stmt->setOperations->back()->nestedSelectStatement->fromTable->select->setOperations->back()->setType,
            kSetIntersect);
  ASSERT_STREQ(stmt->setOperations->back()->nestedSelectStatement->fromTable->select->fromTable->name, "C");
  ASSERT_STREQ(stmt->setOperations->back()
                   ->nestedSelectStatement->fromTable->select->setOperations->back()
                   ->nestedSelectStatement->fromTable->name,
               "D");
}

TEST(NestedSetOperationsWithMultipleWithClauses) {
  TEST_PARSE_SINGLE_SQL(
      "WITH UNION_FIRST AS (SELECT * FROM A UNION SELECT * FROM B),INTERSECT_SECOND AS (SELECT * FROM UNION_FIRST "
      "INTERSECT SELECT * FROM C) SELECT * FROM UNION_FIRST EXCEPT SELECT * FROM INTERSECT_SECOND",
      kStmtSelect, SelectStatement, result, stmt);

  ASSERT_STREQ(stmt->withDescriptions->at(0)->alias, "UNION_FIRST");
  ASSERT_STREQ(stmt->withDescriptions->back()->alias, "INTERSECT_SECOND");

  ASSERT_EQ(stmt->withDescriptions->at(0)->select->setOperations->back()->setType, kSetUnion);
  ASSERT_STREQ(stmt->withDescriptions->at(0)->select->fromTable->name, "A");
  ASSERT_STREQ(stmt->withDescriptions->at(0)->select->setOperations->back()->nestedSelectStatement->fromTable->name,
               "B");

  ASSERT_EQ(stmt->withDescriptions->back()->select->setOperations->back()->setType, kSetIntersect);
  ASSERT_STREQ(stmt->withDescriptions->back()->select->fromTable->name, "UNION_FIRST");
  ASSERT_STREQ(stmt->withDescriptions->back()->select->setOperations->back()->nestedSelectStatement->fromTable->name,
               "C");

  ASSERT_EQ(stmt->setOperations->back()->setType, kSetExcept);
  ASSERT_STREQ(stmt->fromTable->name, "UNION_FIRST");
  ASSERT_STREQ(stmt->setOperations->back()->nestedSelectStatement->fromTable->name, "INTERSECT_SECOND");
}

TEST(WrongOrderByStatementTest) {
  SQLParserResult res = parse_and_move("SELECT * FROM students ORDER BY name INTERSECT SELECT grade FROM students_2;");
  ASSERT_FALSE(res.isValid());
}

TEST(BeginTransactionTest) {
  {
    TEST_PARSE_SINGLE_SQL("BEGIN TRANSACTION;", kStmtTransaction, TransactionStatement, transaction_result,
                          transaction_stmt);

    ASSERT_EQ(transaction_stmt->command, kBeginTransaction);
  }

  {
    TEST_PARSE_SINGLE_SQL("BEGIN;", kStmtTransaction, TransactionStatement, transaction_result, transaction_stmt);

    ASSERT_EQ(transaction_stmt->command, kBeginTransaction);
  }
}

TEST(RollbackTransactionTest) {
  TEST_PARSE_SINGLE_SQL("ROLLBACK TRANSACTION;", kStmtTransaction, TransactionStatement, transaction_result,
                        transaction_stmt);

  ASSERT_EQ(transaction_stmt->command, kRollbackTransaction);
}

TEST(CommitTransactionTest) {
  TEST_PARSE_SINGLE_SQL("COMMIT TRANSACTION;", kStmtTransaction, TransactionStatement, transaction_result,
                        transaction_stmt);

  ASSERT_EQ(transaction_stmt->command, kCommitTransaction);
}

TEST(CastAsType) {
  TEST_PARSE_SINGLE_SQL("SELECT CAST(ID AS VARCHAR(8)) FROM TEST", kStmtSelect, SelectStatement, result, stmt);

  ASSERT_TRUE(result.isValid());
  ASSERT_EQ(stmt->selectList->size(), 1);
  ASSERT_EQ(stmt->selectList->front()->columnType.data_type, DataType::VARCHAR);
  ASSERT_EQ(stmt->selectList->front()->columnType.length, 8);
}

TEST_MAIN();
