//
// PostgreSQLTest.h
//
// Definition of the PostgreSQLTest class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PostgreSQLTest_INCLUDED
#define PostgreSQLTest_INCLUDED


#include "Poco/SQL/PostgreSQL/PostgreSQL.h"
#include "Poco/SQL/Session.h"
#include "Poco/SharedPtr.h"
#include "Poco/CppUnit/TestCase.h"
#include "SQLExecutor.h"


class PostgreSQLTest: public CppUnit::TestCase
	/// PostgreSQL test class
	/// Tested:
	///
	/// Driver			|	DB						| OS
	/// ----------------+---------------------------+------------------------------------------
	/// 03.51.12.00		| PostgreSQL 9.3.1.0(18)   	| Mac OSX  10.9.1
	///

{
public:
	PostgreSQLTest(const std::string& name);
	~PostgreSQLTest();

	void testConnectNoDB();
	void testPostgreSQLOIDs();
	void testBarebonePostgreSQL();

	void testSimpleAccess();
	void testComplexType();
	void testSimpleAccessVector();
	void testComplexTypeVector();
	void testInsertVector();
	void testInsertEmptyVector();

	void testInsertSingleBulk();
	void testInsertSingleBulkVec();

	void testLimit();
	void testLimitOnce();
	void testLimitPrepare();
	void testLimitZero();
	void testPrepare();

	void testSetSimple();
	void testSetComplex();
	void testSetComplexUnique();
	void testMultiSetSimple();
	void testMultiSetComplex();
	void testMapComplex();
	void testMapComplexUnique();
	void testMultiMapComplex();
	void testSelectIntoSingle();
	void testSelectIntoSingleStep();
	void testSelectIntoSingleFail();
	void testLowerLimitOk();
	void testLowerLimitFail();
	void testCombinedLimits();
	void testCombinedIllegalLimits();
	void testRange();
	void testIllegalRange();
	void testSingleSelect();
	void testEmptyDB();
	void testDateTime();
	void testBLOB();
	void testBLOBStmt();
	void testCLOBStmt();

	void testUnsignedInts();
	void testFloat();
	void testDouble();

	void testTuple();
	void testTupleVector();

	void testInternalExtraction();

	void testNull();
	void testNullVector();

	void testNullableInt();
	void testNullableString();
	void testTupleWithNullable();
	
	void testSessionTransaction();
	void testTransaction();

	void testReconnect();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:

	void dropTable(const std::string& tableName);
	void recreatePersonTable();
	void recreatePersonBLOBTable();
	void recreatePersonCLOBTable();
	void recreatePersonDateTimeTable();
	void recreatePersonDateTable();
	void recreatePersonTimeTable();
	void recreateStringsTable();
	void recreateIntsTable();
	void recreateUnsignedIntsTable();
	void recreateFloatsTable();
	void recreateTuplesTable();
	void recreateVectorsTable();
	void recreateNullableIntTable();
	void recreateNullableStringTable();

	static void dbInfo(Poco::SQL::Session& session);

	static std::string getHost();
	static std::string getPort();
	static std::string getBase();
	static std::string getUser();
	static std::string getPass();

	static std::string _dbConnString;
	static Poco::SharedPtr<Poco::SQL::Session> _pSession;
	static Poco::SharedPtr<SQLExecutor> _pExecutor;
	static const bool bindValues[8];
};


#endif // PostgreSQLTest_INCLUDED
