//
// ODBCSybaseTest.h
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ODBCSybaseTest_INCLUDED
#define ODBCSybaseTest_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "ODBCTest.h"
#include "Poco/Format.h"

class SybaseODBC : public ODBCTest {
public:
	SybaseODBC(const std::string& name);
	static CppUnit::Test* suite();

	virtual void testBareboneODBC();
	virtual void testBulkPerformance();
private:
	void dropObject(const std::string& type, const std::string& tableName);
	void recreateNullableTable();
	void recreatePersonTable();
	void recreatePersonBLOBTable();
	void recreatePersonDateTable();
	void recreatePersonTimeTable();
	void recreatePersonDateTimeTable();
	void recreateStringsTable();
	void recreateIntsTable();
	void recreateFloatsTable();
	void recreateTuplesTable();
	void recreateVectorsTable();
	void recreateAnysTable();
	void recreateNullsTable(const std::string& notNull = "");
	void recreateMiscTable();
	void recreateLogTable();
	void testStoredProcedure();
	void testStoredProcedureDynamicAny();
	void testStoredProcedureAny();
	void testTransaction();

	virtual std::string str2NumExpr(const std::string& num, unsigned len, unsigned dec)
	{
		std::string res;
		Poco::format(res, " CONVERT(NUMERIC(%u, %u), '%s') ", len, dec, num);
		return res;
	}

	bool emptyStringIsSpace() { return true; }

	void doMiscTable(bool haveSecCol);
	void doPersonTable(const std::string& lnAttr = "");

	static ODBCTest::SessionPtr  _pSession;
	static ODBCTest::ExecPtr     _pExecutor;
	static std::string _driver;
	static std::string _dsn;
	static std::string _uid;
	static std::string _pwd;
	static std::string _connectString;
};


#endif
