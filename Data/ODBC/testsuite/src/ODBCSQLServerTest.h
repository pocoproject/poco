//
// ODBCSQLServerTest.h
//
// Definition of the ODBCSQLServerTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ODBCSQLServerTest_INCLUDED
#define ODBCSQLServerTest_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "ODBCTest.h"


// uncomment to use native SQL Server ODBC driver
// #define POCO_ODBC_USE_SQL_NATIVE


class ODBCSQLServerTest: public ODBCTest
	/// SQLServer ODBC test class
	/// Tested:
	///
	///  Driver name                        | Driver version    | DB version            | OS
	/// ------------------------------------+-------------------+-----------------------+------------------------------------------
	///  SQL Server Express 9.0.2047        | 2000.86.1830.00   |                       | MS Windows XP Professional x64 v.2003/SP1
	///  SQL Server Express 9.0.2047        | 2005.90.2047.00   |                       | MS Windows XP Professional x64 v.2003/SP1
	///  SQL Server Express 10.50.1600.1    | 2009.100.1600.01  |                       | MS Windows XP Professional x64 v.2003/SP1
	///  SQL Server                         | 10.00.22621.1992  | 16.0.1000.6 (64-bit)  | Windows 11
	///  ODBC Driver 17 for SQL Server      | 2017.1710.03.01   | 16.0.1000.6 (64-bit)  | Windows 11
	///  ODBC Driver 18 for SQL Server      | 2018.183.01.01    | 16.0.1000.6 (64-bit)  | Windows 11
	///
	/// Drivers download (x86, x64, ARM64):
	/// https://learn.microsoft.com/en-us/sql/connect/odbc/download-odbc-driver-for-sql-server?view=sql-server-ver16
{
public:
	ODBCSQLServerTest(const std::string& name);
	~ODBCSQLServerTest();

	void testBareboneODBC();

	void testTempTable();

	void testBLOB();
	void testNull();
	void testBulk();

	void testStoredProcedure();
	void testCursorStoredProcedure();
	void testStoredProcedureAny();
	void testStoredProcedureDynamicAny();

	void testStoredProcedureReturn();
	void testStoredFunction();

	static CppUnit::Test* suite();

private:
	void dropObject(const std::string& type, const std::string& name);
	void recreateNullableTable();
	void recreatePersonTable();
	void recreatePersonBLOBTable();
	void recreatePersonDateTimeTable();
	void recreatePersonDateTable() { /* no-op */ };
	void recreatePersonTimeTable() { /* no-op */ };
	void recreateStringsTable();
	void recreateIntsTable();
	void recreateFloatsTable();
	void recreateUUIDsTable();
	void recreateTuplesTable();
	void recreateVectorTable();
	void recreateVectorsTable();
	void recreateAnysTable();
	void recreateNullsTable(const std::string& notNull = "");
	void recreateBoolTable();
	void recreateMiscTable();
	void recreateLogTable();
	void recreateUnicodeTable();
	void recreateEncodingTables();

	static SessionPtr  _pSession;
	static SessionPtr  _pEncSession;
	static ExecPtr     _pExecutor;
	static std::string _driver;
	static std::string _dsn;
	static std::string _uid;
	static std::string _pwd;
	static std::string _db;
	static std::string _connectString;
};


#endif // ODBCSQLServerTest_INCLUDED
