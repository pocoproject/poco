//
// ODBCInformixTest.cpp
//
// Copyright (c) 2006-2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ODBCInformixTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Any.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Tuple.h"
#include "Poco/Exception.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Diagnostics.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/ODBC/ODBCStatementImpl.h"
#include <sqltypes.h>
#include <iostream>


using namespace Poco::Data::Keywords;
using Poco::Data::DataException;
using Poco::Data::ODBC::Utility;
using Poco::Data::ODBC::ConnectionException;
using Poco::Data::ODBC::StatementException;
using Poco::Data::ODBC::StatementDiagnostics;
using Poco::format;
using Poco::Tuple;
using Poco::Any;
using Poco::AnyCast;
using Poco::Dynamic::Var;
using Poco::NotFoundException;


#define INFORMIX_ODBC_DRIVER "Informix ODBC Driver"
#define INFORMIX_DSN "PocoDataInformixTest"
#define INFORMIX_SERVER POCO_ODBC_TEST_DATABASE_SERVER
#define INFORMIX_PORT "9088"
#define INFORMIX_SRV_NAME "ol_informix"
#define INFORMIX_DB "poco_test"
#define INFORMIX_UID "informix"
#define INFORMIX_PWD "poco"


ODBCTest::SessionPtr ODBCInformixTest::_pSession;
ODBCTest::ExecPtr    ODBCInformixTest::_pExecutor;
std::string          ODBCInformixTest::_driver = INFORMIX_ODBC_DRIVER;
std::string          ODBCInformixTest::_dsn = INFORMIX_DSN;
std::string          ODBCInformixTest::_uid = INFORMIX_UID;
std::string          ODBCInformixTest::_pwd = INFORMIX_PWD;
std::string          ODBCInformixTest::_connectString = "DRIVER={" INFORMIX_ODBC_DRIVER "};"
	"SERVER=" INFORMIX_SRV_NAME ";"
	"HOST=" INFORMIX_SERVER ";"
	"SERVICE=" INFORMIX_PORT ";"
	"PROTOCOL=onsoctcp;"
	"DATABASE=" INFORMIX_DB ";"
	"UID=" INFORMIX_UID ";"
	"PWD=" INFORMIX_PWD ";";


ODBCInformixTest::ODBCInformixTest(const std::string& name):
	ODBCTest(name, _pSession, _pExecutor, _dsn, _uid, _pwd, _connectString)
{
}


ODBCInformixTest::~ODBCInformixTest()
{
}


void ODBCInformixTest::testBareboneODBC()
{
	if (!_pSession) fail ("Test not available.");

	std::string tableCreateString = "CREATE TABLE Test "
		"(First VARCHAR(30),"
		"Second VARCHAR(30),"
		"Third BYTE,"
		"Fourth INTEGER,"
		"Fifth FLOAT,"
		"Sixth DATETIME YEAR TO FRACTION(5))";

	_pExecutor->bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL);
	_pExecutor->bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND);
	_pExecutor->bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL);
	_pExecutor->bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND);
}


void ODBCInformixTest::dropObject(const std::string& type, const std::string& name)
{
	try
	{
		*_pSession << format("DROP %s %s", type, name), now;
	}
	catch (StatementException& ex)
	{
		bool ignoreError = false;
		const StatementDiagnostics::FieldVec& flds = ex.diagnostics().fields();
		StatementDiagnostics::Iterator it = flds.begin();
		for (; it != flds.end(); ++it)
		{
			// Informix error codes:
			// -206: table does not exist
			// -319: table/view not found
			if (-206 == it->_nativeError || -319 == it->_nativeError)
			{
				ignoreError = true;
				break;
			}
		}

		if (!ignoreError) throw;
	}
}


void ODBCInformixTest::recreateNullableTable()
{
	dropObject("TABLE", "NullableTest");
	try { *_pSession << "CREATE TABLE NullableTest (EmptyString VARCHAR(30), EmptyInteger INTEGER, EmptyFloat FLOAT, EmptyDateTime DATETIME YEAR TO FRACTION(5), EmptyDate DATE)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateNullableTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateNullableTable()"); }
}


void ODBCInformixTest::recreatePersonTable()
{
	dropObject("TABLE", "Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonTable()"); }
}


void ODBCInformixTest::recreatePersonBLOBTable()
{
	dropObject("TABLE", "Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Image BYTE)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
}


void ODBCInformixTest::recreatePersonDateTable()
{
	dropObject("TABLE", "Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), BornDate DATE)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonDateTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonDateTable()"); }
}


void ODBCInformixTest::recreatePersonTimeTable()
{
	dropObject("TABLE", "Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), BornTime DATETIME HOUR TO SECOND)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonTimeTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonTimeTable()"); }
}


void ODBCInformixTest::recreatePersonDateTimeTable()
{
	dropObject("TABLE", "Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Born DATETIME YEAR TO FRACTION(5))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonDateTimeTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonDateTimeTable()"); }
}


void ODBCInformixTest::recreateIntsTable()
{
	dropObject("TABLE", "Strings");
	try { *_pSession << "CREATE TABLE Strings (str INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateIntsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateIntsTable()"); }
}


void ODBCInformixTest::recreateStringsTable()
{
	dropObject("TABLE", "Strings");
	try { *_pSession << "CREATE TABLE Strings (str VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateStringsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateStringsTable()"); }
}


void ODBCInformixTest::recreateFloatsTable()
{
	dropObject("TABLE", "Strings");
	try { *_pSession << "CREATE TABLE Strings (str FLOAT)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateFloatsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateFloatsTable()"); }
}


void ODBCInformixTest::recreateTuplesTable()
{
	dropObject("TABLE", "Tuples");
	try { *_pSession << "CREATE TABLE Tuples "
		"(int0 INTEGER, int1 INTEGER, int2 INTEGER, int3 INTEGER, int4 INTEGER, int5 INTEGER, int6 INTEGER, "
		"int7 INTEGER, int8 INTEGER, int9 INTEGER, int10 INTEGER, int11 INTEGER, int12 INTEGER, int13 INTEGER,"
		"int14 INTEGER, int15 INTEGER, int16 INTEGER, int17 INTEGER, int18 INTEGER, int19 INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateTuplesTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateTuplesTable()"); }
}


void ODBCInformixTest::recreateVectorsTable()
{
	dropObject("TABLE", "Vectors");
	try { *_pSession << "CREATE TABLE Vectors (i0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateVectorsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateVectorsTable()"); }
}


void ODBCInformixTest::recreateAnysTable()
{
	dropObject("TABLE", "Anys");
	try { *_pSession << "CREATE TABLE Anys (i0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateAnysTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateAnysTable()"); }
}


void ODBCInformixTest::recreateNullsTable(const std::string& notNull)
{
	dropObject("TABLE", "NullTest");
	try { *_pSession << format("CREATE TABLE NullTest (i INTEGER %s, r FLOAT %s, v VARCHAR(30) %s)",
		notNull,
		notNull,
		notNull), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateNullsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateNullsTable()"); }
}


void ODBCInformixTest::recreateMiscTable()
{
	dropObject("TABLE", "MiscTest");
	try
	{
		session() << "CREATE TABLE MiscTest "
			"(First VARCHAR(30),"
			"Second BYTE,"
			"Third INTEGER,"
			"Fourth FLOAT,"
			"Fifth DATETIME YEAR TO FRACTION(5))", now;
	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateMiscTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateMiscTable()"); }
}


void ODBCInformixTest::recreateLogTable()
{
	dropObject("TABLE", "T_POCO_LOG");
	dropObject("TABLE", "T_POCO_LOG_ARCHIVE");

	try
	{
		std::string sql = "CREATE TABLE %s "
			"(Source VARCHAR(100),"
			"Name VARCHAR(100),"
			"ProcessId INTEGER,"
			"Thread VARCHAR(100), "
			"ThreadId INTEGER,"
			"Priority INTEGER,"
			"Text VARCHAR(100),"
			"DateTime DATETIME YEAR TO FRACTION(5))";

		session() << sql, "T_POCO_LOG", now;
		session() << sql, "T_POCO_LOG_ARCHIVE", now;

	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateLogTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateLogTable()"); }
}


CppUnit::Test* ODBCInformixTest::suite()
{
	if ((_pSession = init(_driver, _dsn, _uid, _pwd, _connectString)))
	{
		std::cout << "*** Connected to [" << _driver << "] test database." << std::endl;

		_pExecutor = new SQLExecutor(_driver + " SQL Executor", _pSession);

		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ODBCInformixTest");

		CppUnit_addTest(pSuite, ODBCInformixTest, testBareboneODBC);
		CppUnit_addTest(pSuite, ODBCInformixTest, testConnection);
		CppUnit_addTest(pSuite, ODBCInformixTest, testSession);
		CppUnit_addTest(pSuite, ODBCInformixTest, testZeroRows);
		CppUnit_addTest(pSuite, ODBCInformixTest, testSimpleAccess);
		CppUnit_addTest(pSuite, ODBCInformixTest, testSimpleAccessVector);
		CppUnit_addTest(pSuite, ODBCInformixTest, testComplexType);
		CppUnit_addTest(pSuite, ODBCInformixTest, testInsertVector);
		CppUnit_addTest(pSuite, ODBCInformixTest, testInsertEmptyVector);
		CppUnit_addTest(pSuite, ODBCInformixTest, testAffectedRows);
		CppUnit_addTest(pSuite, ODBCInformixTest, testPrepare);
		CppUnit_addTest(pSuite, ODBCInformixTest, testNull);
		CppUnit_addTest(pSuite, ODBCInformixTest, testReconnect);

		return pSuite;
	}

	return nullptr;
}
