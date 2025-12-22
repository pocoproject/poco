//
// ODBCMySQLTest.h
//
// Definition of the ODBCMySQLTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ODBCMySQLTest_INCLUDED
#define ODBCMySQLTest_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "ODBCTest.h"


class ODBCMySQLTest: public ODBCTest
	/// MySQL ODBC test class
	/// Tested:
	///
	/// Driver          | DB                        | OS                                        | Driver Manager
	/// ----------------+---------------------------+-------------------------------------------+---------------------
	/// 03.51.12.00     | MySQL 5.0.27-community-nt | MS Windows XP Professional x64 v.2003/SP1 | 3.526.3959.0
	///  3.51.11.-6     | MySQL 5.0.27-community-nt | Ubuntu 7.04 (2.6.20-15-generic #2 SMP)    | unixODBC 2.2.11.-13
	///

{
public:
	ODBCMySQLTest(const std::string& name);
	~ODBCMySQLTest() override;

	void testBareboneODBC() override;

	void testBLOB() override;

	void testStoredProcedure() override;
	void testStoredFunction() override;

	void testNull() override;

	void testMultipleResults() override;
	void testFilter() override;

	static CppUnit::Test* suite();

private:
	void dropObject(const std::string& type, const std::string& name) override;
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
	static std::string _db;
	static std::string _connectString;
};


#endif // ODBCMySQLTest_INCLUDED
