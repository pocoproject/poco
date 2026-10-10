//
// ODBCDB2Test.h
//
// Definition of the ODBCDB2Test class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ODBCDB2Test_INCLUDED
#define ODBCDB2Test_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "ODBCTest.h"


class ODBCDB2Test: public ODBCTest
	/// IBM DB2 UDB ODBC test class
	/// Tested:
	///
	/// Driver		|	DB				| OS
	/// ------------+-------------------+------------------------------------------
	///	9.01.00.356 | DB2 Express-C 9.1	| MS Windows XP Professional x64 v.2003/SP1
{
public:
	ODBCDB2Test(const std::string& name);
	~ODBCDB2Test() override;

	void testBareboneODBC() override;

	void testBLOB() override;
	void testFilter() override;

	void testStoredProcedure() override;
	void testStoredProcedureAny() override;
	void testStoredProcedureDynamicVar() override;
	void testStoredFunction() override;

	static CppUnit::Test* suite();

private:
	void dropObject(const std::string& type, const std::string& tableName) override;
	void recreateNullableTable() override;
	void recreatePersonTable() override;
	void recreatePersonBLOBTable() override;
	void recreatePersonDateTable() override;
	void recreatePersonTimeTable() override;
	void recreatePersonDateTimeTable() override;
	void recreateStringsTable() override;
	void recreateIntsTable() override;
	void recreateFloatsTable() override;
	void recreateTuplesTable() override;
	void recreateVectorsTable() override;
	void recreateAnysTable() override;
	void recreateNullsTable(const std::string& notNull = "") override;
	void recreateMiscTable() override;
	void recreateLogTable() override;

	static ODBCTest::SessionPtr  _pSession;
	static ODBCTest::ExecPtr     _pExecutor;
	static std::string _driver;
	static std::string _dsn;
	static std::string _uid;
	static std::string _pwd;
	static std::string _connectString;
};


#endif // ODBCDB2Test_INCLUDED
