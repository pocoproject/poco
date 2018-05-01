//
// ODBCAccessTest.h
//
// Definition of the ODBCAccessTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ODBCAccessTest_INCLUDED
#define ODBCAccessTest_INCLUDED


#include "Poco/SQL/ODBC/ODBC.h"
#include "Poco/SQL/Session.h"
#include "Poco/SQL/ODBC/Utility.h"
#include "Poco/SharedPtr.h"
#include "Poco/CppUnit/TestCase.h"
#include "SQLExecutor.h"


class ODBCAccessTest: public CppUnit::TestCase
	/// MS Access ODBC test class
	/// Tested:
	///
	/// Driver		|	DB		| OS
	/// ------------+-----------+------------------------------------------
	///	4.00.6305.00| Jet 4.0	| MS Windows XP Professional x64 v.2003/SP1
{
public:
	ODBCAccessTest(const std::string& name);
	~ODBCAccessTest();

	void testSimpleAccess();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void dropTable(const std::string& tableName);
	void recreatePersonTable();

	static bool init(const std::string& driver, const std::string& dsn);
	static bool canConnect(const std::string& driver, const std::string& dsn);

	static Poco::SQL::ODBC::Utility::DriverMap _drivers;
	static std::string _dbConnString;
	static Poco::SQL::Session* _pSession;
	bool _owner;
};


#endif // ODBCAccessTest_INCLUDED
