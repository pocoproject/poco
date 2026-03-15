//
// ODBCInformixTest.h
//
// Definition of the ODBCInformixTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ODBCInformixTest_INCLUDED
#define ODBCInformixTest_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "ODBCTest.h"


class ODBCInformixTest: public ODBCTest
	/// IBM Informix ODBC test class
	/// Tested:
	///
	/// Driver				|	DB								| OS
	/// --------------------+-----------------------------------+------
	///	Informix ODBC Driver| Informix Developer Edition 14.10	| Linux
{
public:
	ODBCInformixTest(const std::string& name);
	~ODBCInformixTest() override;

	void testBareboneODBC() override;

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


#endif // ODBCInformixTest_INCLUDED
