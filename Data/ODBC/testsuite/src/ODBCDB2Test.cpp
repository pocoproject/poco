//
// ODBCDB2Test.cpp
//
// $Id: //poco/Main/Data/ODBC/testsuite/src/ODBCDB2Test.cpp#5 $
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ODBCDB2Test.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Any.h"
#include "Poco/DynamicAny.h"
#include "Poco/Tuple.h"
#include "Poco/Exception.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Diagnostics.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/ODBC/ODBCStatementImpl.h"
#include "Poco/Environment.h"
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
using Poco::DynamicAny;
using Poco::NotFoundException;

static std::string db2Driver()
{
	return Poco::Environment::get("POCO_TEST_DB2_DRIVER", 
#if defined(POCO_OS_FAMILY_WINDOWS) 
	"IBM DB2 ODBC DRIVER - DB2COPY1"
#else
	"libdb2o.so"
#endif
	);
}

static std::string db2Uid()
{
	return Poco::Environment::get("POCO_TEST_DB2_UID", "db2admin");
}

static std::string db2Db()
{
	return Poco::Environment::get("POCO_TEST_DB2_DB", "POCOTEST");
}

static std::string db2Pwd()
{
	return Poco::Environment::get("POCO_TEST_DB2_PWD", "db2admin");
}

static std::string db2Extra()
{
	std::string e = Poco::Environment::get("POCO_TEST_DB2_EXTRA", "");
	return (e.empty() ? "" : e + ";");
}

#define DB2_DSN "PocoDataDB2Test"
#define DB2_SERVER POCO_ODBC_TEST_DATABASE_SERVER
#define DB2_PORT "50000"


ODBCTest::SessionPtr ODBCDB2Test::_pSession;
ODBCTest::ExecPtr    ODBCDB2Test::_pExecutor;
std::string          ODBCDB2Test::_driver = db2Driver();
std::string          ODBCDB2Test::_dsn = DB2_DSN;
std::string          ODBCDB2Test::_uid = db2Uid();
std::string          ODBCDB2Test::_pwd = db2Pwd();
std::string          ODBCDB2Test::_connectString = "Driver=" + db2Driver() + ";"
	+ db2Extra() +
	"Database=" + db2Db() + ";"
	"Hostname=" DB2_SERVER ";"
	"Port=" DB2_PORT ";"
	"Protocol=TCPIP;"
	"Uid=" + db2Uid() + ";"
	"Pwd=" + db2Pwd() + ";"
	;


ODBCDB2Test::ODBCDB2Test(const std::string& name): 
	ODBCTest(name, _pSession, _pExecutor, _dsn, _uid, _pwd, _connectString)
{
}


ODBCDB2Test::~ODBCDB2Test()
{
}


void ODBCDB2Test::testBareboneODBC()
{
	if (! &session()) fail ("Test not available.");

	std::string tableCreateString = "CREATE TABLE " + ExecUtil::test_tbl() +
		"(First VARCHAR(30),"
		"Second VARCHAR(30),"
		"Third BLOB,"
		"Fourth INTEGER,"
		"Fifth FLOAT,"
		"Sixth TIMESTAMP)";

	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND);


	tableCreateString = "CREATE TABLE " + ExecUtil::test_tbl() +
		"(First VARCHAR(30),"
		"Second INTEGER,"
		"Third FLOAT)";

	executor().bareboneODBCMultiResultTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCMultiResultTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND);
	executor().bareboneODBCMultiResultTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCMultiResultTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND);
}


void ODBCDB2Test::testBLOB()
{
	if (! &session()) fail ("Test not available.");
	
	const std::size_t maxFldSize = 1000000;
	session().setProperty("maxFieldSize", Poco::Any(maxFldSize-1));
	recreatePersonBLOBTable();

	try
	{
		executor().blob(maxFldSize);
		fail ("must fail");
	}
	catch (DataException&) 
	{
		session().setProperty("maxFieldSize", Poco::Any(maxFldSize));
	}

	for (int i = 0; i < 8;)
	{
		recreatePersonBLOBTable();
		session().setFeature("autoBind", bindValue(i));
		session().setFeature("autoExtract", bindValue(i+1));
		executor().blob(maxFldSize);
		i += 2;
	}

	recreatePersonBLOBTable();
	try
	{
		executor().blob(maxFldSize+1);
		fail ("must fail");
	}
	catch (DataException&) { }
}


void ODBCDB2Test::testFilter()
{
	if (! &session()) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateVectorsTable();
		session().setFeature("autoBind", bindValue(i));
		session().setFeature("autoExtract", bindValue(i+1));
		executor().filter("SELECT * FROM " + ExecUtil::vectors() + " ORDER BY i0 ASC", "i0");
		i += 2;
	}
}


void ODBCDB2Test::testStoredProcedure()
{
	if (! &session()) fail ("Test not available.");

	const std::string nm = ExecUtil::stored_proc();

	dropObject("PROCEDURE", nm + "(INTEGER)");
	dropObject("PROCEDURE", nm + "(INTEGER, INTEGER)");
	dropObject("PROCEDURE", nm + "(VARCHAR(1000), VARCHAR(1000))");

	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		session().setFeature("autoExtract", bindValue(k+1));

		session() << "CREATE PROCEDURE " << nm << "(OUT outParam INTEGER) "
			"BEGIN "
			" SET outParam = -1; "
			"END" , now;

		int i = 0;
		session() << "{call " + db2Db() + "." << nm << "(?)}", out(i), now;
		dropObject("PROCEDURE", nm + "(INTEGER)");
		assert(-1 == i);

		session() << "CREATE PROCEDURE " << nm << "(inParam INTEGER, OUT outParam INTEGER) "
			"BEGIN "
			" SET outParam = inParam*inParam; "
			"END" , now;
		
		i = 2;
		int j = 0;
		session() << "{call " + db2Db() + "." << nm << "(?, ?)}", in(i), out(j), now;
		dropObject("PROCEDURE", nm + "(INTEGER, INTEGER)");
		assert(4 == j);
	
		session() << "CREATE PROCEDURE " << nm << "(INOUT ioParam INTEGER) "
			"BEGIN "
			" SET ioParam = ioParam*ioParam; "
			"END" , now;

		i = 2;
		session() << "{call " + db2Db() + "." << nm << "(?)}", io(i), now;
		dropObject("PROCEDURE", nm + "(INTEGER)");
		assert(4 == i);

		//TIMESTAMP is not supported as stored procedure parameter in DB2
		//(SQL0182N An expression with a datetime value or a labeled duration is not valid.)

		session() << "CREATE PROCEDURE " << nm << "(inParam VARCHAR(1000), OUT outParam VARCHAR(1000)) "
			"BEGIN "
			" SET outParam = inParam; "
			"END" , now;

		std::string inParam = 
			"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
			"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
			"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
			"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
			"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
			"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
			"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
			"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
			"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
		std::string outParam;
		session() << "{call " + db2Db() + "." << nm << "(?,?)}", in(inParam), out(outParam), now;
		dropObject("PROCEDURE", nm + "(VARCHAR(1000), VARCHAR(1000))");
		assert(inParam == outParam);

		k += 2;
	}
}


void ODBCDB2Test::testStoredProcedureAny()
{
	if (! &session()) fail ("Test not available.");

	const std::string nm = ExecUtil::stored_proc();

	dropObject("PROCEDURE", nm + "(INTEGER)");
	dropObject("PROCEDURE", nm + "(INTEGER, INTEGER)");
	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		session().setFeature("autoExtract", bindValue(k+1));

		Any i = 2;
		Any j = 0;

		session() << "CREATE PROCEDURE " << nm << "(inParam INTEGER, OUT outParam INTEGER) "
			"BEGIN "
			" SET outParam = inParam*inParam; "
			"END" , now;

		session() << "{call " + db2Db() + "." << nm << "(?, ?)}", in(i), out(j), now;
		dropObject("PROCEDURE", nm + "(INTEGER, INTEGER)");
		assert(4 == AnyCast<int>(j));

		session() << "CREATE PROCEDURE " << nm << "(INOUT ioParam INTEGER) "
			"BEGIN "
			" SET ioParam = ioParam*ioParam; "
			"END" , now;

		i = 2;
		session() << "{call " + db2Db() + "." << nm << "(?)}", io(i), now;
		dropObject("PROCEDURE", nm + "(INTEGER)");
		assert(4 == AnyCast<int>(i));
		
		k += 2;
	}
}


void ODBCDB2Test::testStoredProcedureDynamicAny()
{
	if (! &session()) fail ("Test not available.");

	const std::string nm = ExecUtil::stored_proc();

	dropObject("PROCEDURE", nm + "(INTEGER)");
	dropObject("PROCEDURE", nm + "(INTEGER, INTEGER)");
	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		
		DynamicAny i = 2;
		DynamicAny j = 0;

		session() << "CREATE PROCEDURE " << nm << "(inParam INTEGER, OUT outParam INTEGER) "
			"BEGIN "
			" SET outParam = inParam*inParam; "
			"END" , now;

		session() << "{call " + db2Db() + "." << nm << "(?, ?)}", in(i), out(j), now;
		dropObject("PROCEDURE", nm + "(INTEGER, INTEGER)");
		assert(4 == j);

		session() << "CREATE PROCEDURE " << nm << "(INOUT ioParam INTEGER) "
			"BEGIN "
			" SET ioParam = ioParam*ioParam; "
			"END" , now;

		i = 2;
		session() << "{call " + db2Db() + "." << nm << "(?)}", io(i), now;
		dropObject("PROCEDURE", nm + "(INTEGER)");
		assert(4 == i);

		k += 2;
	}
}


void ODBCDB2Test::testStoredFunction()
{
	const std::string nm = ExecUtil::stored_func();
	if (! &session()) fail ("Test not available.");

	dropObject("PROCEDURE", nm + "()");
	dropObject("PROCEDURE", nm + "(INTEGER)");
	dropObject("PROCEDURE", nm + "(INTEGER, INTEGER)");
	dropObject("PROCEDURE", nm + "(VARCHAR(10), DATE, TIME, TIMESTAMP, INTEGER, SMALLINT, REAL, DOUBLE, VARCHAR(10), INTEGER)");
	dropObject("PROCEDURE", nm + "(VARCHAR(10), VARCHAR(10))");

	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		session().setFeature("autoExtract", bindValue(k + 1));

		{
			session() << "CREATE PROCEDURE " << nm << "() "
				"BEGIN "
				" DECLARE C1 CURSOR FOR select * from sysibm.sysdummy1 where 1=2;"
				" OPEN C1;"
				"  RETURN;"
				"END", now;

			Poco::Data::Statement stat(session());
			stat << "{ call " + db2Db() + "." << nm << "()}", now;
			Poco::Data::RecordSet rs(stat);

			assert(0 == rs.rowCount());
			dropObject("PROCEDURE", nm + "()");
		}
		{
			session() << "CREATE PROCEDURE " << nm << "(inp VARCHAR(10), out dt DATE, out tm TIME, out tms TIMESTAMP, out int32 INTEGER, "
				"out si SMALLINT, out fl REAL, out dbl DOUBLE, out s2 VARCHAR(10), out an INTEGER)"
				"BEGIN "
				"set dt =null; set tm =null; set tms =null; set int32 =null; set si =null; set fl =null; set dbl =null; set s2 = inp; set an = inp;"
				"END", now;

			Poco::Data::Statement stat(session());
			Poco::Nullable<std::string> ns;
			Poco::Nullable<Poco::Data::Date> nd = Poco::Nullable<Poco::Data::Date>(Poco::Data::Date());
			Poco::Nullable<int> n_i(1);
			Poco::Nullable<Poco::Data::Time> tm = Poco::Nullable<Poco::Data::Time>(Poco::Data::Time());
			Poco::Nullable<Poco::DateTime> tms = Poco::Nullable<Poco::DateTime>(Poco::DateTime());
			Poco::Nullable<Poco::Int16> i16(1);
			Poco::Nullable<float> flt(1);
			Poco::Nullable<double> dbl(1);
			Poco::Nullable<std::string> s2("ddd");
			Poco::Nullable<Any> an(Any(2));
			stat << "{call " + db2Db() + "." << nm << "(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}", useRef(ns), out(nd), out(tm), out(tms), out(n_i), out(i16), out(flt), out(dbl), out(s2), out(an), now;
			dropObject("PROCEDURE", nm + "(VARCHAR(10), DATE, TIME, TIMESTAMP, INTEGER, SMALLINT, REAL, DOUBLE, VARCHAR(10), INTEGER)");
			assert(nd.isNull());
			assert(n_i.isNull());
			assert(tm.isNull());
			assert(tms.isNull());
			assert(i16.isNull());
			assert(flt.isNull());
			assert(dbl.isNull());
			assert(s2.isNull());
			assert(an.isNull());
		}

		session() << "CREATE PROCEDURE " << nm << "() "
			"BEGIN "
			"  RETURN -1; "
			"END" , now;

		int i = 0;
		session() << "{? = call " + db2Db() + "." << nm << "()}", out(i), now;
		dropObject("PROCEDURE", nm + "()");
		assert(-1 == i);


		session() << "CREATE PROCEDURE " << nm << "(inParam INTEGER) "
			"BEGIN "
			" RETURN inParam*inParam; "
			"END" , now;
		
		i = 2;
		int result = 0;
		session() << "{? = call " + db2Db() + "." << nm << "(?)}", out(result), in(i), now;
		dropObject("PROCEDURE", nm + "(INTEGER)");
		assert(4 == result);

		session() << "CREATE PROCEDURE " << nm << "(inParam INTEGER, OUT outParam INTEGER) "
			"BEGIN "
			" SET outParam = inParam*inParam; "
			" RETURN outParam; "
			"END" , now;

		i = 2;
		int j = 0;
		result = 0;
		session() << "{? = call " + db2Db() + "." << nm << "(?, ?)}", out(result), in(i), out(j), now;
		dropObject("PROCEDURE", nm + "(INTEGER, INTEGER)");
		assert(4 == j);
		assert(j == result); 

		session() << "CREATE PROCEDURE " << nm << "(INOUT param1 INTEGER, INOUT param2 INTEGER) "
			"BEGIN "
			" DECLARE temp INTEGER;"
			" SET temp = param1; "
			" SET param1 = param2; "
			" SET param2 = temp; "
			" RETURN param1 + param2; "
			"END" , now;

		i = 1;
		j = 2;
		result = 0;
		session() << "{? = call " + db2Db() + "." << nm << "(?, ?)}", out(result), io(i), io(j), now;
		assert(1 == j);
		assert(2 == i);
		assert(3 == result); 

		Tuple<int, int> params(1, 2);
		assert(1 == params.get<0>());
		assert(2 == params.get<1>());
		result = 0;
		session() << "{? = call " + db2Db() + "." << nm << "(?, ?)}", out(result), io(params), now;
		dropObject("PROCEDURE", nm + "(INTEGER, INTEGER)");
		assert(1 == params.get<1>());
		assert(2 == params.get<0>());
		assert(3 == result); 

		session().setFeature("autoBind", true);

		session() << "CREATE PROCEDURE " << nm << "(inParam VARCHAR(10), OUT outParam VARCHAR(10)) "
			"BEGIN "
			" SET outParam = inParam; "
			" RETURN LENGTH(outParam);"//DB2 allows only integer as return type
			"END" , now;

		std::string inParam = "123456789";
		std::string outParam;
		int ret;
		session() << "{? = call " + db2Db() + "." << nm << "(?,?)}", out(ret), in(inParam), out(outParam), now;
		dropObject("PROCEDURE", nm + "(VARCHAR(10), VARCHAR(10))");
		assert(inParam == outParam);
		assert(ret == inParam.size());

		k += 2;
	}
}


void ODBCDB2Test::testXMLColumn()
{
	const std::string tbl = ExecUtil::mangleTable("xmlColumn");
	dropObject("TABLE", tbl);
	try { 
		const std::string xmlStr = "<a> xml text </a>";
		Poco::UTF16String uStr;
		for (unsigned c = 0x400; c < 0x409; ++c) uStr.append(3, Poco::UTF16Char(c) );
		session() << "CREATE TABLE " << tbl << " (id integer, x XML, cl CLOB, dbcl DBCLOB)", now;
		session() << "INSERT INTO " << tbl << " (id , x, cl, dbcl)  VALUES(1, '" << xmlStr << "', ?, ?)", bind(xmlStr), bind(uStr), now;
		Poco::Data::Statement  stat(session());
		stat << "SELECT id, x,cl, dbcl FROM " << tbl, now;
		
		Poco::Data::RecordSet rs(stat);
		assert(1 == rs.rowCount());
		assert(4 == rs.columnCount());
		int id = rs.value<int>(0);
		assert(1 == id);

		Poco::Data::BLOB xml = rs.value<Poco::Data::BLOB>(1);
		std::string readStr(reinterpret_cast<const char*>(xml.rawContent()), xml.size());
		assert(readStr.find(xmlStr) < readStr.length());

		Poco::Data::CLOB cl = rs.value<Poco::Data::CLOB>(2);
		assert(xmlStr == std::string(cl.rawContent(), cl.size()));

		const Poco::UTF16String us = rs.value<Poco::UTF16String>(3);
		assert(uStr == us);
		// check nullables
		Poco::Nullable<Poco::Data::CLOB> ncl = Poco::Nullable<Poco::Data::CLOB>(Poco::Data::CLOB());
		assert(false == ncl.isNull());
		Poco::Nullable<Poco::Data::BLOB> nbl = Poco::Nullable<Poco::Data::BLOB>(Poco::Data::BLOB());
		assert(false == nbl.isNull());
		Poco::Nullable<Poco::UTF16String> usn(Poco::UTF16String(2, Poco::UTF16Char('a')));
		assert(false == usn.isNull());
		session() << "INSERT INTO " << tbl << " (id) VALUES (99) ", now;
		session() << "SELECT x,cl, dbcl FROM " << tbl << " WHERE id > 1", into(nbl), into(ncl), into(usn), now;

		assert(true == ncl.isNull());
		assert(true == nbl.isNull());
		assert(true == usn.isNull());
	}
	catch (const Poco::Exception& e)
	{
		dropObject("TABLE", tbl);

		std::cerr << e.message() << std::endl;
		throw;
	}
	dropObject("TABLE", tbl);
}


void ODBCDB2Test::dropObject(const std::string& type, const std::string& name)
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
            		//(table does not exist)        // procedure not found
            		if (-204 == it->_nativeError || (-458 == it->_nativeError) )
			{
				ignoreError = true;
				break;
			}
		}

		if (!ignoreError) throw;
	}
}


void ODBCDB2Test::recreateNullableTable()
{
	dropObject("TABLE", ExecUtil::nullabletest());
	try { session() << "CREATE TABLE " << ExecUtil::nullabletest() << " (EmptyString VARCHAR(30), EmptyInteger INTEGER , EmptyFloat FLOAT , EmptyDateTime TIMESTAMP)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonTable()"); }
}

void ODBCDB2Test::recreateNumericTable()
{
	dropObject("TABLE", ExecUtil::numeric_tbl());
	try {
		session() << "CREATE TABLE " << ExecUtil::numeric_tbl() <<
			" (id integer, num8 NUMERIC(8), num16_3 NUMERIC(16,3), num18 NUMERIC(18), num18_8 NUMERIC(18,8), num22 NUMERIC(22))", now;
	}
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateNumericTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateNumericTable()"); }
}


void ODBCDB2Test::recreatePersonTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() <<" (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonTable()"); }
}


void ODBCDB2Test::recreatePersonBLOBTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() <<" (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Image BLOB)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
}


void ODBCDB2Test::recreatePersonDateTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() <<" (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), BornDate DATE)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonDateTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonDateTable()"); }
}


void ODBCDB2Test::recreatePersonTimeTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() <<" (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), BornTime TIME)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonTimeTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonTimeTable()"); }
}


void ODBCDB2Test::recreatePersonDateTimeTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() <<" (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Born TIMESTAMP)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonDateTimeTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonDateTimeTable()"); }
}


void ODBCDB2Test::recreateIntsTable()
{
	dropObject("TABLE", ExecUtil::strings());
	try { session() << "CREATE TABLE " << ExecUtil::strings() << " (str INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateIntsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateIntsTable()"); }
}


void ODBCDB2Test::recreateStringsTable()
{
	dropObject("TABLE", ExecUtil::strings());
	try { session() << "CREATE TABLE " << ExecUtil::strings() << " (str VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateStringsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateStringsTable()"); }
}


void ODBCDB2Test::recreateFloatsTable()
{
	dropObject("TABLE", ExecUtil::strings());
	try { session() << "CREATE TABLE " << ExecUtil::strings() << " (str FLOAT)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateFloatsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateFloatsTable()"); }
}


void ODBCDB2Test::recreateTuplesTable()
{
	dropObject("TABLE", ExecUtil::tuples());
	try { session() << "CREATE TABLE " << ExecUtil::tuples() << 
		"(int0 INTEGER, int1 INTEGER, int2 INTEGER, int3 INTEGER, int4 INTEGER, int5 INTEGER, int6 INTEGER, "
		"int7 INTEGER, int8 INTEGER, int9 INTEGER, int10 INTEGER, int11 INTEGER, int12 INTEGER, int13 INTEGER,"
		"int14 INTEGER, int15 INTEGER, int16 INTEGER, int17 INTEGER, int18 INTEGER, int19 INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateTuplesTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateTuplesTable()"); }
}


void ODBCDB2Test::recreateVectorsTable()
{
	dropObject("TABLE", ExecUtil::vectors());
	try { session() << "CREATE TABLE " << ExecUtil::vectors() << " (i0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateVectorsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateVectorsTable()"); }
}


void ODBCDB2Test::recreateAnysTable()
{
	dropObject("TABLE", ExecUtil::anys() );
	try { session() << "CREATE TABLE " << ExecUtil::anys() << " (i0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateAnysTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateAnysTable()"); }
}


void ODBCDB2Test::recreateNullsTable(const std::string& notNull)
{
	dropObject("TABLE", ExecUtil::nulltest());
	try { session() << format("CREATE TABLE %s (i INTEGER %s, r FLOAT %s, v VARCHAR(30) %s)", ExecUtil::nulltest(),
		notNull,
		notNull,
		notNull), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateNullsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateNullsTable()"); }
}


void ODBCDB2Test::recreateMiscTable()
{
	dropObject("TABLE", ExecUtil::misctest());
	try 
	{ 
		session() << "CREATE TABLE " << ExecUtil::misctest() <<
			"(First VARCHAR(30),"
			"Second BLOB,"
			"Third INTEGER,"
			"Fourth FLOAT,"
			"Fifth TIMESTAMP)", now; 
	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateMiscTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateMiscTable()"); }
}


void ODBCDB2Test::recreateLogTable()
{
	dropObject("TABLE", ExecUtil::pocolog());;
	dropObject("TABLE", ExecUtil::pocolog_a());;

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
			"DateTime TIMESTAMP)"; 

		session() << sql, ExecUtil::pocolog(), now; 
		session() << sql, ExecUtil::pocolog_a(), now;

	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateLogTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateLogTable()"); }
}


CppUnit::Test* ODBCDB2Test::suite()
{
	if ((_pSession = init(_driver, _dsn, _uid, _pwd, _connectString)))
	{
		std::cout << "*** Connected to [" << _driver << "] test database." << std::endl;

		_pExecutor = new SQLExecutor(_driver + " SQL Executor", _pSession);

		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ODBCDB2Test");

		CppUnit_addTest(pSuite, ODBCDB2Test, testBareboneODBC);
		CppUnit_addTest(pSuite, ODBCDB2Test, testZeroRows);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSyntaxError);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSimpleAccess);
		CppUnit_addTest(pSuite, ODBCDB2Test, testComplexType);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSimpleAccessVector);
		CppUnit_addTest(pSuite, ODBCDB2Test, testComplexTypeVector);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSharedPtrComplexTypeVector);
		CppUnit_addTest(pSuite, ODBCDB2Test, testAutoPtrComplexTypeVector);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInsertVector);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInsertEmptyVector);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSimpleAccessList);
		CppUnit_addTest(pSuite, ODBCDB2Test, testComplexTypeList);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInsertList);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInsertEmptyList);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSimpleAccessDeque);
		CppUnit_addTest(pSuite, ODBCDB2Test, testComplexTypeDeque);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInsertDeque);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInsertEmptyDeque);
		CppUnit_addTest(pSuite, ODBCDB2Test, testAffectedRows);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInsertSingleBulk);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInsertSingleBulkVec);
		CppUnit_addTest(pSuite, ODBCDB2Test, testLimit);
		CppUnit_addTest(pSuite, ODBCDB2Test, testLimitOnce);
		CppUnit_addTest(pSuite, ODBCDB2Test, testLimitPrepare);
		CppUnit_addTest(pSuite, ODBCDB2Test, testLimitZero);
		CppUnit_addTest(pSuite, ODBCDB2Test, testPrepare);
		CppUnit_addTest(pSuite, ODBCDB2Test, testBulk);
		CppUnit_addTest(pSuite, ODBCDB2Test, testBulkPerformance);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSetSimple);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSetComplex);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSetComplexUnique);
		CppUnit_addTest(pSuite, ODBCDB2Test, testMultiSetSimple);
		CppUnit_addTest(pSuite, ODBCDB2Test, testMultiSetComplex);
		CppUnit_addTest(pSuite, ODBCDB2Test, testMapComplex);
		CppUnit_addTest(pSuite, ODBCDB2Test, testMapComplexUnique);
		CppUnit_addTest(pSuite, ODBCDB2Test, testMultiMapComplex);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSelectIntoSingle);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSelectIntoSingleStep);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSelectIntoSingleFail);
		CppUnit_addTest(pSuite, ODBCDB2Test, testLowerLimitOk);
		CppUnit_addTest(pSuite, ODBCDB2Test, testLowerLimitFail);
		CppUnit_addTest(pSuite, ODBCDB2Test, testCombinedLimits);
		CppUnit_addTest(pSuite, ODBCDB2Test, testCombinedIllegalLimits);
		CppUnit_addTest(pSuite, ODBCDB2Test, testRange);
		CppUnit_addTest(pSuite, ODBCDB2Test, testIllegalRange);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSingleSelect);
		CppUnit_addTest(pSuite, ODBCDB2Test, testEmptyDB);
		CppUnit_addTest(pSuite, ODBCDB2Test, testBLOB);
		CppUnit_addTest(pSuite, ODBCDB2Test, testBLOBContainer);
		CppUnit_addTest(pSuite, ODBCDB2Test, testBLOBStmt);
		CppUnit_addTest(pSuite, ODBCDB2Test, testDate);
		CppUnit_addTest(pSuite, ODBCDB2Test, testTime);
		CppUnit_addTest(pSuite, ODBCDB2Test, testDateTime);
		CppUnit_addTest(pSuite, ODBCDB2Test, testFloat);
		CppUnit_addTest(pSuite, ODBCDB2Test, testDouble);
		CppUnit_addTest(pSuite, ODBCDB2Test, testTuple);
		CppUnit_addTest(pSuite, ODBCDB2Test, testTupleVector);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInternalExtraction);
		CppUnit_addTest(pSuite, ODBCDB2Test, testFilter);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInternalBulkExtraction);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInternalStorageType);
		CppUnit_addTest(pSuite, ODBCDB2Test, testStoredProcedure);
		CppUnit_addTest(pSuite, ODBCDB2Test, testStoredProcedureAny);
		CppUnit_addTest(pSuite, ODBCDB2Test, testStoredProcedureDynamicAny);
		CppUnit_addTest(pSuite, ODBCDB2Test, testStoredFunction);
		CppUnit_addTest(pSuite, ODBCDB2Test, testNull);
		CppUnit_addTest(pSuite, ODBCDB2Test, testRowIterator);
		CppUnit_addTest(pSuite, ODBCDB2Test, testAsync);
		CppUnit_addTest(pSuite, ODBCDB2Test, testAny);
		CppUnit_addTest(pSuite, ODBCDB2Test, testDynamicAny);
		CppUnit_addTest(pSuite, ODBCDB2Test, testMultipleResults);
		CppUnit_addTest(pSuite, ODBCDB2Test, testMultipleResultsNoProj);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSQLChannel);
		CppUnit_addTest(pSuite, ODBCDB2Test, testSQLLogger);
		//CppUnit_addTest(pSuite, ODBCDB2Test, testSessionTransaction); // this test fails when connection is fast
		CppUnit_addTest(pSuite, ODBCDB2Test, testTransaction);
		CppUnit_addTest(pSuite, ODBCDB2Test, testTransactor);
		CppUnit_addTest(pSuite, ODBCDB2Test, testNullable);
		CppUnit_addTest(pSuite, ODBCDB2Test, testReconnect);
		CppUnit_addTest(pSuite, ODBCDB2Test, testNumeric);
		CppUnit_addTest(pSuite, ODBCDB2Test, testXMLColumn);
		CppUnit_addTest(pSuite, ODBCDB2Test, testInsertStatReuse);

		ODBCDB2Test::_pExecutor = 0;
		ODBCDB2Test::_pSession = 0;

		return pSuite;
	}

	return 0;
}
