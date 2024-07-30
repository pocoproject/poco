//
// ODBCSQLiteTest.h
//
// Definition of the ODBCSQLiteTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ODBCSQLiteTest_INCLUDED
#define ODBCSQLiteTest_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "ODBCTest.h"


class ODBCSQLiteTest: public ODBCTest
	/// SQLite3 ODBC test class
	/// Tested:
	///
	/// Driver		|	DB			| OS
	/// ------------+---------------+------------------------------------------
	///	00.70.00.00	| SQLite 3.*	| MS Windows XP Professional x64 v.2003/SP1
{
public:
	ODBCSQLiteTest(const std::string& name);
	~ODBCSQLiteTest() override;

	void testBareboneODBC() override;
	void testAffectedRows() override;
	void testNull() override;

	static CppUnit::Test* suite();

private:
	void dropObject(const std::string& type, const std::string& name) override;
	void recreateNullableTable() override;
	void recreatePersonTable() override;
	void recreatePersonBLOBTable() override;
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

	static ODBCTest::SessionPtr _pSession;
	static ODBCTest::ExecPtr    _pExecutor;
	static std::string          _driver;
	static std::string          _dsn;
	static std::string          _uid;
	static std::string          _pwd;
	static std::string          _connectString;
};


#endif // ODBCSQLiteTest_INCLUDED
