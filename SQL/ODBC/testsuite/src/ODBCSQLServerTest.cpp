//
// ODBCSQLServerTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ODBCSQLServerTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Any.h"
#include "Poco/DynamicAny.h"
#include "Poco/Tuple.h"
#include "Poco/UTFString.h"
#include "Poco/DateTime.h"
#include "Poco/SQL/RecordSet.h"
#include "Poco/SQL/ODBC/Diagnostics.h"
#include "Poco/SQL/ODBC/ODBCException.h"
#include "Poco/SQL/ODBC/ODBCStatementImpl.h"
#include <iostream>


using namespace Poco::SQL::Keywords;
using Poco::SQL::SQLException;
using Poco::SQL::Statement;
using Poco::SQL::RecordSet;
using Poco::SQL::CLOB;
using Poco::SQL::ODBC::Utility;
using Poco::SQL::ODBC::ConnectionException;
using Poco::SQL::ODBC::StatementException;
using Poco::SQL::ODBC::StatementDiagnostics;
using Poco::format;
using Poco::Tuple;
using Poco::Any;
using Poco::AnyCast;
using Poco::DynamicAny;
using Poco::DateTime;
using Poco::UTF16String;


// uncomment to force FreeTDS on Windows
//#define FORCE_FREE_TDS

// uncomment to use native SQL driver
//#define POCO_ODBC_USE_SQL_NATIVE

// FreeTDS version selection guide (from http://www.freetds.org/userguide/choosingtdsprotocol.htm)
// (see #define FREE_TDS_VERSION below)
// Product												TDS Version	Comment
// ---------------------------------------------------+------------+------------------------------------------------------------
// Sybase before System 10, Microsoft SQL Server 6.x	4.2			Still works with all products, subject to its limitations.
// Sybase System 10 and above							5.0			Still the most current protocol used by Sybase.
// Sybase System SQL Anywhere							5.0 only 	Originally Watcom SQL Server, a completely separate codebase.
// 																	Our best information is that SQL Anywhere first supported TDS
// 																	in version 5.5.03 using the OpenServer Gateway (OSG), and native
// 																	TDS 5.0 support arrived with version 6.0.
// Microsoft SQL Server 7.0								7.0			Includes support for the extended datatypes in SQL Server 7.0
// 																	(such as char/varchar fields of more than 255 characters), and
// 																	support for Unicode.
// Microsoft SQL Server 2000							8.0			Include support for bigint (64 bit integers), variant and collation
// 																	on all fields. variant is not supported; collation is not widely used.

#if defined(POCO_OS_FAMILY_WINDOWS) && !defined(FORCE_FREE_TDS)
	#ifdef POCO_ODBC_USE_SQL_NATIVE
		#define MS_SQL_SERVER_ODBC_DRIVER "SQL Server Native Client 10.0"
	#else
		#define MS_SQL_SERVER_ODBC_DRIVER "SQL Server"
	#endif
	#pragma message ("Using " MS_SQL_SERVER_ODBC_DRIVER " driver")
#else
	#define MS_SQL_SERVER_ODBC_DRIVER "FreeTDS"
	#define FREE_TDS_VERSION "8.0"
	#if defined(POCO_OS_FAMILY_WINDOWS)
		#pragma message ("Using " MS_SQL_SERVER_ODBC_DRIVER " driver, version " FREE_TDS_VERSION)
	#endif
#endif

#define MS_SQL_SERVER_DSN "PocoDataSQLServerTest"
#define MS_SQL_SERVER_SERVER POCO_ODBC_TEST_DATABASE_SERVER "\\SQLEXPRESS"
#define MS_SQL_SERVER_PORT "1433"
#define MS_SQL_SERVER_DB "poco"
#define MS_SQL_SERVER_UID "poco"
#define MS_SQL_SERVER_PWD "poco"


ODBCTest::SessionPtr ODBCSQLServerTest::_pSession;
ODBCTest::ExecPtr    ODBCSQLServerTest::_pExecutor;
std::string          ODBCSQLServerTest::_driver = MS_SQL_SERVER_ODBC_DRIVER;
std::string          ODBCSQLServerTest::_dsn = MS_SQL_SERVER_DSN;
std::string          ODBCSQLServerTest::_uid = MS_SQL_SERVER_UID;
std::string          ODBCSQLServerTest::_pwd = MS_SQL_SERVER_PWD;
std::string          ODBCSQLServerTest::_db  = MS_SQL_SERVER_DB;

std::string ODBCSQLServerTest::_connectString = "DRIVER=" MS_SQL_SERVER_ODBC_DRIVER ";"
	"UID=" MS_SQL_SERVER_UID ";"
	"PWD=" MS_SQL_SERVER_PWD ";"
	"DATABASE=" MS_SQL_SERVER_DB ";"
	"SERVER=" MS_SQL_SERVER_SERVER ";"
	"PORT=" MS_SQL_SERVER_PORT ";"
#ifdef FREE_TDS_VERSION
	"TDS_Version=" FREE_TDS_VERSION ";"
#endif
	;


ODBCSQLServerTest::ODBCSQLServerTest(const std::string& name):
	ODBCTest(name, _pSession, _pExecutor, _dsn, _uid, _pwd, _connectString)
{
}


ODBCSQLServerTest::~ODBCSQLServerTest()
{
}


void ODBCSQLServerTest::testBareboneODBC()
{
	std::string tableCreateString = "CREATE TABLE " + ExecUtil::test_tbl() +
		"(First VARCHAR(30),"
		"Second VARCHAR(30),"
		"Third VARBINARY(30),"
		"Fourth INTEGER,"
		"Fifth FLOAT,"
		"Sixth DATETIME)";

	executor().bareboneODBCTest(dbConnString(), tableCreateString,
		SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL, true, "CONVERT(VARBINARY(30),?)");
	executor().bareboneODBCTest(dbConnString(), tableCreateString,
		SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND, true, "CONVERT(VARBINARY(30),?)");
	executor().bareboneODBCTest(dbConnString(), tableCreateString,
		SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL, true, "CONVERT(VARBINARY(30),?)");
	executor().bareboneODBCTest(dbConnString(), tableCreateString,
		SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND, true, "CONVERT(VARBINARY(30),?)");

	tableCreateString = "CREATE TABLE " + ExecUtil::test_tbl() +
		"(First VARCHAR(30),"
		"Second INTEGER,"
		"Third FLOAT)";

	executor().bareboneODBCMultiResultTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCMultiResultTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND);
	executor().bareboneODBCMultiResultTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCMultiResultTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND);
}


void ODBCSQLServerTest::testBLOB()
{
	const std::size_t maxFldSize = 250000;
	session().setProperty("maxFieldSize", Poco::Any(maxFldSize-1));
	recreatePersonBLOBTable();

	try
	{
		executor().blob(maxFldSize, "CONVERT(VARBINARY(MAX),?)");
		fail ("must fail");
	}
	catch (SQLException&)
	{
		session().setProperty("maxFieldSize", Poco::Any(maxFldSize));
	}

	for (int i = 0; i < 8;)
	{
		recreatePersonBLOBTable();
		session().setFeature("autoBind", bindValue(i));
		session().setFeature("autoExtract", bindValue(i+1));
		executor().blob(maxFldSize, "CONVERT(VARBINARY(MAX),?)");
		i += 2;
	}

	recreatePersonBLOBTable();
	try
	{
		executor().blob(maxFldSize+1, "CONVERT(VARBINARY(MAX),?)");
		fail ("must fail");
	}
	catch (SQLException&) { }
}


void ODBCSQLServerTest::testNull()
{
	// test for NOT NULL violation exception
	for (int i = 0; i < 8;)
	{
		recreateNullsTable("NOT NULL");
		session().setFeature("autoBind", bindValue(i));
		session().setFeature("autoExtract", bindValue(i+1));
		executor().notNulls("23000");
		i += 2;
	}

	// test for null insertion
	for (int i = 0; i < 8;)
	{
		recreateNullsTable();
		session().setFeature("autoBind", bindValue(i));
		session().setFeature("autoExtract", bindValue(i+1));
		executor().nulls();
		i += 2;
	}
}


void ODBCSQLServerTest::testBulk()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);

	recreateMiscTable();
	_pExecutor->doBulkWithBool<std::vector<int>,
		std::vector<std::string>,
		std::vector<CLOB>,
		std::vector<double>,
		std::vector<DateTime>,
		std::vector<bool> >(100, "CONVERT(VARBINARY(30),?)");

	recreateMiscTable();
	_pExecutor->doBulkWithBool<std::deque<int>,
		std::deque<std::string>,
		std::deque<CLOB>,
		std::deque<double>,
		std::deque<DateTime>,
		std::deque<bool> >(100, "CONVERT(VARBINARY(30),?)");

	recreateMiscTable();
	_pExecutor->doBulkWithBool<std::list<int>,
		std::list<std::string>,
		std::list<CLOB>,
		std::list<double>,
		std::list<DateTime>,
		std::list<bool> >(100, "CONVERT(VARBINARY(30),?)");
}


void ODBCSQLServerTest::testStoredProcedure()
{
	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		session().setFeature("autoExtract", bindValue(k+1));

		dropObject("PROCEDURE", "storedProcedure");

		session() << "CREATE PROCEDURE storedProcedure(@outParam int OUTPUT) AS "
			"BEGIN "
			"SET @outParam = -1; "
			"END;"
		, now;
		
		int i = 0;
		session() << "{call storedProcedure(?)}", out(i), now;
		assertTrue (-1 == i);
		dropObject("PROCEDURE", "storedProcedure");

		session() << "CREATE PROCEDURE storedProcedure(@inParam int, @outParam int OUTPUT) AS "
			"BEGIN "
			"SET @outParam = @inParam*@inParam; "
			"END;"
		, now;

		i = 2;
		int j = 0;
		session() << "{call storedProcedure(?, ?)}", in(i), out(j), now;
		assertTrue (4 == j);
		dropObject("PROCEDURE", "storedProcedure");

		session() << "CREATE PROCEDURE storedProcedure(@ioParam int OUTPUT) AS "
			"BEGIN "
			"SET @ioParam = @ioParam*@ioParam; "
			"END;"
		, now;

		i = 2;
		session() << "{call storedProcedure(?)}", io(i), now;
		assertTrue (4 == i);
		dropObject("PROCEDURE", "storedProcedure");

		session() << "CREATE PROCEDURE storedProcedure(@ioParam DATETIME OUTPUT) AS "
			"BEGIN "
			" SET @ioParam = @ioParam + 1; "
			"END;" , now;

		DateTime dt(1965, 6, 18, 5, 35, 1);
		session() << "{call storedProcedure(?)}", io(dt), now;
		assertTrue (19 == dt.day());
		dropObject("PROCEDURE", "storedProcedure");

		k += 2;
	}

	{
		session().setFeature("autoBind", true);
		session() << "CREATE PROCEDURE storedProcedure(@inParam VARCHAR(MAX), @outParam VARCHAR(MAX) OUTPUT) AS "
			"BEGIN "
			"SET @outParam = @inParam; "
			"END;"
			, now;

		std::string inParam = "123";
		std::string outParam;
		try {
			session() << "{call storedProcedure(?, ?)}", in(inParam), out(outParam), now;
		}
		catch(StatementException& ex) {
			std::cout << ex.toString();
		}
		assertTrue (outParam == inParam);
		dropObject("PROCEDURE", "storedProcedure");
	}

	{
		session().setFeature("autoBind", true);
		session() << "CREATE PROCEDURE storedProcedure(@inParam NVARCHAR(MAX), @outParam NVARCHAR(MAX) OUTPUT) AS "
			"BEGIN "
			"SET @outParam = @inParam; "
			"END;"
			, now;

		UTF16String::value_type cs[] = { L'1', L'2', L'3', L'\0' };
		UTF16String inParam(cs);
		UTF16String outParam;
		try {
			session() << "{call storedProcedure(?, ?)}", in(inParam), out(outParam), now;
		}
		catch (StatementException& ex) {
			std::cout << ex.toString();
		}
		assertTrue (outParam == inParam);
		dropObject("PROCEDURE", "storedProcedure");
	}
}


void ODBCSQLServerTest::testCursorStoredProcedure()
{
	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		session().setFeature("autoExtract", bindValue(k+1));

		recreatePersonTable();
		typedef Tuple<std::string, std::string, std::string, int> Person;
		std::vector<Person> people;
		people.push_back(Person("Simpson", "Homer", "Springfield", 42));
		people.push_back(Person("Simpson", "Bart", "Springfield", 12));
		people.push_back(Person("Simpson", "Lisa", "Springfield", 10));
		session() << "INSERT INTO " << ExecUtil::person() << " VALUES (?, ?, ?, ?)", use(people), now;

		dropObject("PROCEDURE", "storedCursorProcedure");
		session() << "CREATE PROCEDURE storedCursorProcedure(@ageLimit int) AS "
			"BEGIN "
			" SELECT * "
			" FROM " << ExecUtil::person() <<
			" WHERE Age < @ageLimit "
			" ORDER BY Age DESC; "
			"END;"
		, now;

		people.clear();
		int age = 13;
		
		session() << "{call storedCursorProcedure(?)}", in(age), into(people), now;
		
		assertTrue (2 == people.size());
		assertTrue (Person("Simpson", "Bart", "Springfield", 12) == people[0]);
		assertTrue (Person("Simpson", "Lisa", "Springfield", 10) == people[1]);

		Statement stmt = ((session() << "{call storedCursorProcedure(?)}", in(age), now));
		RecordSet rs(stmt);
		assertTrue (rs["LastName"] == "Simpson");
		assertTrue (rs["FirstName"] == "Bart");
		assertTrue (rs["Address"] == "Springfield");
		assertTrue (rs["Age"] == 12);

		dropObject("TABLE", ExecUtil::person());
		dropObject("PROCEDURE", "storedCursorProcedure");

		k += 2;
	}
}


void ODBCSQLServerTest::testStoredProcedureAny()
{
	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		session().setFeature("autoExtract", bindValue(k+1));

		Any i = 2;
		Any j = 0;

		session() << "CREATE PROCEDURE storedProcedure(@inParam int, @outParam int OUTPUT) AS "
			"BEGIN "
			"SET @outParam = @inParam*@inParam; "
			"END;"
		, now;

		session() << "{call storedProcedure(?, ?)}", in(i), out(j), now;
		assertTrue (4 == AnyCast<int>(j));
		session() << "DROP PROCEDURE storedProcedure;", now;

		session() << "CREATE PROCEDURE storedProcedure(@ioParam int OUTPUT) AS "
			"BEGIN "
			"SET @ioParam = @ioParam*@ioParam; "
			"END;"
		, now;

		i = 2;
		session() << "{call storedProcedure(?)}", io(i), now;
		assertTrue (4 == AnyCast<int>(i));
		dropObject("PROCEDURE", "storedProcedure");

		k += 2;
	}
}


void ODBCSQLServerTest::testStoredProcedureDynamicAny()
{
	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		
		DynamicAny i = 2;
		DynamicAny j = 0;

		session() << "CREATE PROCEDURE storedProcedure(@inParam int, @outParam int OUTPUT) AS "
			"BEGIN "
			"SET @outParam = @inParam*@inParam; "
			"END;"
		, now;

		session() << "{call storedProcedure(?, ?)}", in(i), out(j), now;
		assertTrue (4 == j);
		session() << "DROP PROCEDURE storedProcedure;", now;

		session() << "CREATE PROCEDURE storedProcedure(@ioParam int OUTPUT) AS "
			"BEGIN "
			"SET @ioParam = @ioParam*@ioParam; "
			"END;"
		, now;

		i = 2;
		session() << "{call storedProcedure(?)}", io(i), now;
		assertTrue (4 == i);
		dropObject("PROCEDURE", "storedProcedure");

		k += 2;
	}
}


void ODBCSQLServerTest::testStoredFunction()
{
	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		session().setFeature("autoExtract", bindValue(k+1));

		dropObject("PROCEDURE", "storedFunction");
		session() << "CREATE PROCEDURE storedFunction AS "
			"BEGIN "
			"DECLARE @retVal int;"
			"SET @retVal = -1;"
			"RETURN @retVal;"
			"END;"
		, now;

		int i = 0;
		session() << "{? = call storedFunction}", out(i), now;
		assertTrue (-1 == i);
		dropObject("PROCEDURE", "storedFunction");


		session() << "CREATE PROCEDURE storedFunction(@inParam int) AS "
			"BEGIN "
			"RETURN @inParam*@inParam;"
			"END;"
		, now;

		i = 2;
		int result = 0;
		session() << "{? = call storedFunction(?)}", out(result), in(i), now;
		assertTrue (4 == result);
		dropObject("PROCEDURE", "storedFunction");


		session() << "CREATE PROCEDURE storedFunction(@inParam int, @outParam int OUTPUT) AS "
			"BEGIN "
			"SET @outParam = @inParam*@inParam;"
			"RETURN @outParam;"
			"END"
		, now;

		i = 2;
		int j = 0;
		result = 0;
		session() << "{? = call storedFunction(?, ?)}", out(result), in(i), out(j), now;
		assertTrue (4 == j);
		assertTrue (j == result);
		dropObject("PROCEDURE", "storedFunction");


		session() << "CREATE PROCEDURE storedFunction(@param1 int OUTPUT,@param2 int OUTPUT) AS "
			"BEGIN "
			"DECLARE @temp int; "
			"SET @temp = @param1;"
			"SET @param1 = @param2;"
			"SET @param2 = @temp;"
			"RETURN @param1 + @param2; "
			"END"
		, now;

		i = 1;
		j = 2;
		result = 0;
		session() << "{? = call storedFunction(?, ?)}", out(result), io(i), io(j), now;
		assertTrue (1 == j);
		assertTrue (2 == i);
		assertTrue (3 == result);

		Tuple<int, int> params(1, 2);
		assertTrue (1 == params.get<0>());
		assertTrue (2 == params.get<1>());
		result = 0;
		session() << "{? = call storedFunction(?, ?)}", out(result), io(params), now;
		assertTrue (1 == params.get<1>());
		assertTrue (2 == params.get<0>());
		assertTrue (3 == result);

		dropObject("PROCEDURE", "storedFunction");

		k += 2;
	}
}


void ODBCSQLServerTest::dropObject(const std::string& type, const std::string& name)
{
	try
	{
		session() << format("DROP %s %s", type, name), now;
	}
	catch (StatementException& ex)
	{
		bool ignoreError = false;
		const StatementDiagnostics::FieldVec& flds = ex.diagnostics().fields();
		StatementDiagnostics::Iterator it = flds.begin();
		for (; it != flds.end(); ++it)
		{
			if (3701 == it->_nativeError)//(table does not exist)
			{
				ignoreError = true;
				break;
			}
		}

		if (!ignoreError) throw;
	}
}


void ODBCSQLServerTest::recreateNullableTable()
{
	dropObject("TABLE", ExecUtil::nullabletest());
	try { *_pSession << "CREATE TABLE " << ExecUtil::nullabletest() << " (EmptyString VARCHAR(30) NULL, EmptyInteger INTEGER NULL, EmptyFloat FLOAT NULL , EmptyDateTime DATETIME NULL)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonTable()"); }
}


void ODBCSQLServerTest::recreatePersonTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() << " (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonTable()"); }
}


void ODBCSQLServerTest::recreatePersonBLOBTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() << " (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Image VARBINARY(MAX))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
}


void ODBCSQLServerTest::recreatePersonDateTimeTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() << " (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Born DATETIME)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonDateTimeTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonDateTimeTable()"); }
}


void ODBCSQLServerTest::recreateIntsTable()
{
	dropObject("TABLE", ExecUtil::ints());
	try { session() << "CREATE TABLE " << ExecUtil::ints() <<" (str INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateIntsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateIntsTable()"); }
}


void ODBCSQLServerTest::recreateStringsTable()
{
	dropObject("TABLE", ExecUtil::strings());
	try { session() << "CREATE TABLE " << ExecUtil::strings() <<" (str VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateStringsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateStringsTable()"); }
}


void ODBCSQLServerTest::recreateFloatsTable()
{
	dropObject("TABLE", ExecUtil::strings());
	try { session() << "CREATE TABLE " << ExecUtil::strings() <<" (str FLOAT)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateFloatsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateFloatsTable()"); }
}


void ODBCSQLServerTest::recreateTuplesTable()
{
	dropObject("TABLE", ExecUtil::tuples());
	try { session() << "CREATE TABLE " <<  ExecUtil::tuples()  <<
		"(int0 INTEGER, int1 INTEGER, int2 INTEGER, int3 INTEGER, int4 INTEGER, int5 INTEGER, int6 INTEGER, "
		"int7 INTEGER, int8 INTEGER, int9 INTEGER, int10 INTEGER, int11 INTEGER, int12 INTEGER, int13 INTEGER,"
		"int14 INTEGER, int15 INTEGER, int16 INTEGER, int17 INTEGER, int18 INTEGER, int19 INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateTuplesTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateTuplesTable()"); }
}


void ODBCSQLServerTest::recreateVectorTable()
{
	dropObject("TABLE", "Vector");
	try { session() << "CREATE TABLE Vector (i0 INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateVectorTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateVectorTable()"); }
}


void ODBCSQLServerTest::recreateVectorsTable()
{
	dropObject("TABLE", ExecUtil::vectors());
	try { session() << "CREATE TABLE " << ExecUtil::vectors() << " (int0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateVectorsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateVectorsTable()"); }
}


void ODBCSQLServerTest::recreateAnysTable()
{
	dropObject("TABLE", ExecUtil::anys() );
	try { session() << "CREATE TABLE " << ExecUtil::anys() << " (int0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateAnysTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateAnysTable()"); }
}


void ODBCSQLServerTest::recreateNullsTable(const std::string& notNull)
{
	dropObject("TABLE", ExecUtil::nulltest());
	try { session() << format("CREATE TABLE %s (i INTEGER %s, r FLOAT %s, v VARCHAR(30) %s)", ExecUtil::nulltest(),
		notNull,
		notNull,
		notNull), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateNullsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateNullsTable()"); }
}

void ODBCSQLServerTest::recreateBoolTable()
{
	dropObject("TABLE", "BoolTest");
	try { session() << "CREATE TABLE BoolTest (b BIT)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateBoolTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateBoolTable()"); }
}


void ODBCSQLServerTest::recreateMiscTable()
{
	dropObject("TABLE", ExecUtil::misctest());
	try
	{
		session() << "CREATE TABLE  "<< ExecUtil::misctest() <<
			"(First VARCHAR(30),"
			"Second VARBINARY(30),"
			"Third INTEGER,"
			"Fourth FLOAT,"
			"Fifth DATETIME,"
			"Sixth BIT)", now;
	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateMiscTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateMiscTable()"); }
}


void ODBCSQLServerTest::recreateLogTable()
{
	dropObject("TABLE", ExecUtil::pocolog());;
	dropObject("TABLE", ExecUtil::pocolog_a());;

	try
	{
		std::string sql = "CREATE TABLE %s "
			"(Source VARCHAR(max),"
			"Name VARCHAR(max),"
			"ProcessId INTEGER,"
			"Thread VARCHAR(max), "
			"ThreadId INTEGER,"
			"Priority INTEGER,"
			"Text VARCHAR(max),"
			"DateTime DATETIME)";

		session() << sql, ExecUtil::pocolog(), now;
		session() << sql, ExecUtil::pocolog_a(), now;

	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateLogTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateLogTable()"); }
}


void ODBCSQLServerTest::recreateUnicodeTable()
{
#if defined (POCO_ODBC_UNICODE)
	dropObject("TABLE", "UnicodeTable");
	try { session() << "CREATE TABLE UnicodeTable (str NVARCHAR(30))", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateUnicodeTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateUnicodeTable()"); }
#endif
}


CppUnit::Test* ODBCSQLServerTest::suite()
{
	if ((_pSession = init(_driver, _dsn, _uid, _pwd, _connectString, _db)))
	{
		std::cout << "*** Connected to [" << _driver << "] test database." << std::endl;

		_pExecutor = new SQLExecutor(_driver + " SQL Executor", _pSession);

		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ODBCSQLServerTest");

		CppUnit_addTest(pSuite, ODBCSQLServerTest, testBareboneODBC);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testZeroRows);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSyntaxError);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSimpleAccess);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testComplexType);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSimpleAccessVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testComplexTypeVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSharedPtrComplexTypeVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testAutoPtrComplexTypeVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertEmptyVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testBigStringVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSimpleAccessList);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testComplexTypeList);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertList);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertEmptyList);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSimpleAccessDeque);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testComplexTypeDeque);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertDeque);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertEmptyDeque);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testAffectedRows);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertSingleBulk);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertSingleBulkVec);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLimit);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLimitOnce);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLimitPrepare);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLimitZero);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testPrepare);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testBulk);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testBulkPerformance);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSetSimple);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSetComplex);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSetComplexUnique);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testMultiSetSimple);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testMultiSetComplex);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testMapComplex);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testMapComplexUnique);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testMultiMapComplex);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSelectIntoSingle);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSelectIntoSingleStep);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSelectIntoSingleFail);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLowerLimitOk);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLowerLimitFail);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testCombinedLimits);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testCombinedIllegalLimits);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testRange);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testIllegalRange);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSingleSelect);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testEmptyDB);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testBLOB);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testBLOBContainer);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testBLOBStmt);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testDateTime);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testFloat);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testDouble);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testTuple);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testTupleVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testStoredProcedure);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testCursorStoredProcedure);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testStoredProcedureAny);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testStoredProcedureDynamicAny);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testStoredFunction);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInternalExtraction);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testFilter);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInternalBulkExtraction);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInternalStorageType);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testNull);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testRowIterator);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testStdVectorBool);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testAsync);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testAny);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testDynamicAny);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testMultipleResults);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSQLChannel);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSQLLogger);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSessionTransaction);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testTransaction);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testTransactor);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testNullable);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testUnicode);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testReconnect);

		return pSuite;
	}

	return 0;
}
