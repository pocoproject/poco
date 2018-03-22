//
// ODBCAccessTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ODBCAccessTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include "Poco/SQL/LOB.h"
#include "Poco/SQL/StatementImpl.h"
#include "Poco/SQL/ODBC/Connector.h"
#include "Poco/SQL/ODBC/Utility.h"
#include "Poco/SQL/ODBC/Diagnostics.h"
#include "Poco/SQL/ODBC/ODBCException.h"
#include "Poco/SQL/ODBC/ODBCStatementImpl.h"
#include <sqltypes.h>
#include <iostream>


using namespace Poco::SQL::Keywords;
using Poco::SQL::Session;
using Poco::SQL::ODBC::Utility;
using Poco::SQL::ODBC::ConnectionException;
using Poco::SQL::ODBC::StatementException;
using Poco::SQL::ODBC::StatementDiagnostics;
using Poco::format;
using Poco::NotFoundException;


Session*    ODBCAccessTest::_pSession = 0;
std::string ODBCAccessTest::_dbConnString;
Poco::SQL::ODBC::Utility::DriverMap ODBCAccessTest::_drivers;


ODBCAccessTest::ODBCAccessTest(const std::string& name):
	CppUnit::TestCase(name)
{
	Poco::SQL::ODBC::Connector::registerConnector();
}


ODBCAccessTest::~ODBCAccessTest()
{
}


void ODBCAccessTest::testSimpleAccess()
{
	if (!_pSession) fail ("Test not available.");

	std::string lastName = "lastName";
	int age = 133132;
	int count = 0;
	std::string result;

	recreatePersonTable();

	count = 0;
	try { *_pSession << "INSERT INTO PERSON VALUES('lastName', 'firstName', 'Address', 133132)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("testSimpleAccess()"); }
	catch(StatementException& ex){ std::cout << ex.toString() << std::endl; fail ("testSimpleAccess()");}

	try { *_pSession << "SELECT COUNT(*) FROM PERSON", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("testSimpleAccess()"); }
	catch(StatementException& ex){ std::cout << ex.toString() << std::endl; fail ("testSimpleAccess()"); }
	assertTrue (count == 1);

	try { *_pSession << "SELECT LastName FROM PERSON", into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("testSimpleAccess()"); }
	catch(StatementException& ex){ std::cout << ex.toString() << std::endl; fail ("testSimpleAccess()"); }
	assertTrue (lastName == result);

	try { *_pSession << "SELECT Age FROM PERSON", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("testSimpleAccess()"); }
	catch(StatementException& ex){ std::cout << ex.toString() << std::endl; fail ("testSimpleAccess()"); }
	assertTrue (count == age);
}


void ODBCAccessTest::dropTable(const std::string& tableName)
{
	try
	{
		*_pSession << format("DROP TABLE %s", tableName), now;
	}
	catch (StatementException& ex)
	{
		bool ignoreError = false;
		const StatementDiagnostics::FieldVec& flds = ex.diagnostics().fields();
		StatementDiagnostics::Iterator it = flds.begin();
		for (; it != flds.end(); ++it)
		{
			if (-1305 == it->_nativeError)//MS Access -1305 (table does not exist)
			{
				ignoreError = true;
				break;
			}
		}

		if (!ignoreError) throw;
	}
}


void ODBCAccessTest::recreatePersonTable()
{
	dropTable(ExecUtil::person());
	*_pSession << "CREATE TABLE " << ExecUtil::person() << " (LastName TEXT(30), FirstName TEXT(30), Address TEXT(30), Age INTEGER)", now;
}


bool ODBCAccessTest::canConnect(const std::string& driver, const std::string& dsn)
{
	Utility::DriverMap::iterator itDrv = _drivers.begin();
	for (; itDrv != _drivers.end(); ++itDrv)
	{
		if (((itDrv->first).find(driver) != std::string::npos))
		{
			std::cout << "Driver found: " << itDrv->first
				<< " (" << itDrv->second << ')' << std::endl;
			break;
		}
	}

	if (_drivers.end() == itDrv)
	{
		std::cout << driver << " driver NOT found, tests not available." << std::endl;
		return false;
	}

	Utility::DSNMap dataSources;
	Utility::dataSources(dataSources);
	Utility::DSNMap::iterator itDSN = dataSources.begin();
	for (; itDSN != dataSources.end(); ++itDSN)
	{
		if (itDSN->first == dsn && itDSN->second == driver)
		{
			std::cout << "DSN found: " << itDSN->first
				<< " (" << itDSN->second << ')' << std::endl;
			format(_dbConnString, "DSN=%s", dsn);
			return true;
		}
	}

	// DSN not found, try connect without it
	format(_dbConnString, "DRIVER=%s;"
		"UID=admin;"
		"UserCommitSync=Yes;"
		"Threads=3;"
		"SafeTransactions=0;"
		"PageTimeout=5;"
		"MaxScanRows=8;"
		"MaxBufferSize=2048;"
		"FIL=MS Access;"
		"DriverId=25;"
		"DBQ=test.mdb;", driver);

	return true;
}


void ODBCAccessTest::setUp()
{
}


void ODBCAccessTest::tearDown()
{
	dropTable(ExecUtil::person());
}


bool ODBCAccessTest::init(const std::string& driver, const std::string& dsn)
{
	Utility::drivers(_drivers);
	if (!canConnect(driver, dsn)) return false;

	Poco::SQL::ODBC::Connector::registerConnector();
	try
	{
		_pSession = new Session(Poco::SQL::ODBC::Connector::KEY, _dbConnString);
	}catch (ConnectionException& ex)
	{
		std::cout << ex.toString() << std::endl;
		return false;
	}

	//N.B. Access driver does not support check for connection.
	std::cout << "*** Connected to [" << driver << "] test database." << std::endl;

	return true;
}


CppUnit::Test* ODBCAccessTest::suite()
{
	if (init("Microsoft Access Driver (*.mdb)", "PocoDataAccessTest"))
	{
		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ODBCAccessTest");

		CppUnit_addTest(pSuite, ODBCAccessTest, testSimpleAccess);

		return pSuite;
	}

	return 0;
}
