//
// ODBCSybaseTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ODBCSybaseTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Any.h"
#include "Poco/DynamicAny.h"
#include "Poco/Tuple.h"
#include "Poco/Exception.h"
#include "Poco/SQL/LOB.h"
#include "Poco/SQL/RecordSet.h"
#include "Poco/SQL/StatementImpl.h"
#include "Poco/SQL/ODBC/Connector.h"
#include "Poco/SQL/ODBC/Utility.h"
#include "Poco/SQL/ODBC/Diagnostics.h"
#include "Poco/SQL/ODBC/ODBCException.h"
#include "Poco/SQL/ODBC/ODBCStatementImpl.h"
#include <sqltypes.h>
#include <iostream>
#include <queue>


using namespace Poco::SQL::Keywords;
using Poco::SQL::SQLException;
using Poco::SQL::ODBC::Utility;
using Poco::SQL::ODBC::ConnectionException;
using Poco::SQL::ODBC::StatementException;
using Poco::SQL::ODBC::StatementDiagnostics;
using Poco::format;
using Poco::Tuple;
using Poco::Any;
using Poco::AnyCast;
using Poco::DynamicAny;
using Poco::NotFoundException;

#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

#define SYBASE_DSN ""
#define SYBASE_UID ""
#define SYBASE_PWD ""
#define SYBASE_DB "mstk"


static std::string sybaseDriver()
{
	return Poco::Environment::get("POCO_TEST_SYBASE_DRIVER",
#if defined(POCO_OS_FAMILY_WINDOWS)
	"{Adaptive Server Enterprise}"
#else
	"libsybdrvodb-sqllen8.so"
#endif
	);
}


static std::string sybaseExtra()
{
	std::string e = Poco::Environment::get("POCO_TEST_SYBASE_EXTRA", "");
	return (e.empty() ? "" : e + ";");
}

static std::string sybaseDatabase()
{
	return Poco::Environment::get("POCO_TEST_SYBASE_DB", SYBASE_DB);
}

static std::string sybaseUid()
{
	return Poco::Environment::get("POCO_TEST_SYBASE_UID", SYBASE_UID);
}

static std::string sybasePwd()
{
	return Poco::Environment::get("POCO_TEST_SYBASE_PWD", SYBASE_PWD);
}

static std::string sybaseDynamicPrepare()
{
	return Poco::Environment::get("POCO_TEST_SYBASE_DYNAMIC_PREPARE", "1");
}

std::string SybaseODBC::_connectString =
	"driver=" + sybaseDriver() + ";" +
	sybaseExtra() +
	"db=" SYBASE_DB ";"
	"uid=" SYBASE_UID ";"
	"pwd=" SYBASE_PWD ";"
	"db=" + sybaseDatabase() + ";" +
	"uid=" + sybaseUid() + ";" +
	"pwd=" + sybasePwd() + ";" +
	"DynamicPrepare=" + sybaseDynamicPrepare() + ";"
#if !defined(POCO_OS_FAMILY_WINDOWS)
	"CS=iso_1;"
#endif
;



ODBCTest::SessionPtr SybaseODBC::_pSession;
ODBCTest::ExecPtr    SybaseODBC::_pExecutor;
std::string          SybaseODBC::_driver = sybaseDriver();
std::string          SybaseODBC::_dsn = SYBASE_DSN;
std::string          SybaseODBC::_uid = sybaseUid();
std::string          SybaseODBC::_pwd = sybasePwd();


SybaseODBC::SybaseODBC(const std::string& name) :
ODBCTest(name, _pSession, _pExecutor, _dsn, _uid, _pwd, _connectString)
{

}

void SybaseODBC::testBareboneODBC()
{
}


void SybaseODBC::dropObject(const std::string& type, const std::string& name)
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
			if ((-204 == it->_nativeError) || (3701 /* Sybase */ == it->_nativeError) || (-141 /* Sybase IQ*/ == it->_nativeError))//(table does not exist)
			{
				ignoreError = true;
				break;
			}
		}

		if (!ignoreError) throw;
	}
}


void SybaseODBC::recreateNullableTable()
{
	dropObject("TABLE", ExecUtil::nullabletest());
	try { session() << "CREATE TABLE " << ExecUtil::nullabletest() << " (EmptyUniString UNIVARCHAR(30) NULL, EmptyString VARCHAR(30) NULL, EmptyInteger INTEGER NULL, EmptyFloat FLOAT NULL, EmptyLob VARBINARY(100) NULL, EmptyDateTime DATETIME NULL, EmptyDate DATE NULL, EmptyTime TIME NULL)", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateNullableTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateNullableTable()"); }
}


void SybaseODBC::recreatePersonTable()
{
	doPersonTable();
}

void SybaseODBC::doPersonTable(const std::string& lnAttr)
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() << " (LastName VARCHAR(30)" << lnAttr << ", FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreatePersonTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreatePersonTable()"); }
}


void SybaseODBC::recreatePersonBLOBTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() << " (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Image VARBINARY(10240))", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreatePersonBLOBTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreatePersonBLOBTable()"); }
}


void SybaseODBC::recreatePersonDateTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() << " (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), BornDate DATE)", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreatePersonDateTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreatePersonDateTable()"); }
}


void SybaseODBC::recreatePersonTimeTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() << " (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), BornTime TIME)", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreatePersonTimeTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreatePersonTimeTable()"); }
}


void SybaseODBC::recreatePersonDateTimeTable()
{
	dropObject("TABLE", ExecUtil::person());
	try { session() << "CREATE TABLE " << ExecUtil::person() << " (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Born DATETIME)", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreatePersonDateTimeTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreatePersonDateTimeTable()"); }
}


void SybaseODBC::recreateIntsTable()
{
	dropObject("TABLE", ExecUtil::ints());
	try { session() << "CREATE TABLE " << ExecUtil::ints() << " (str INTEGER)", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateIntsTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateIntsTable()"); }
}


void SybaseODBC::recreateStringsTable()
{
	dropObject("TABLE", ExecUtil::strings());
	try { session() << "CREATE TABLE " << ExecUtil::strings() << " (str VARCHAR(30))", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateStringsTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateStringsTable()"); }
}


void SybaseODBC::recreateFloatsTable()
{
	dropObject("TABLE", ExecUtil::floats());
	try { session() << "CREATE TABLE " << ExecUtil::floats() << " (str FLOAT)", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateFloatsTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateFloatsTable()"); }
}


void SybaseODBC::recreateDoublesTable()
{
	dropObject("TABLE", ExecUtil::doubles());
	try { session() << "CREATE TABLE " << ExecUtil::doubles() <<" (str FLOAT)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateDoublesTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateDoublesTable()"); }
}


void SybaseODBC::recreateTuplesTable()
{
	dropObject("TABLE", ExecUtil::tuples());
	try {
		session() << "CREATE TABLE " << ExecUtil::tuples() <<
			"(int0 INTEGER, int1 INTEGER, int2 INTEGER, int3 INTEGER, int4 INTEGER, int5 INTEGER, int6 INTEGER, "
			"int7 INTEGER, int8 INTEGER, int9 INTEGER, int10 INTEGER, int11 INTEGER, int12 INTEGER, int13 INTEGER,"
			"int14 INTEGER, int15 INTEGER, int16 INTEGER, int17 INTEGER, int18 INTEGER, int19 INTEGER)", now;
	}
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateTuplesTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateTuplesTable()"); }
}


void SybaseODBC::recreateVectorsTable()
{
	dropObject("TABLE", ExecUtil::vectors());
	try { session() << "CREATE TABLE " << ExecUtil::vectors() << " (i0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateVectorsTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateVectorsTable()"); }
}


void SybaseODBC::recreateAnysTable()
{
	dropObject("TABLE", ExecUtil::anys());
	try { session() << "CREATE TABLE " << ExecUtil::anys() << " (i0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateAnysTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateAnysTable()"); }
}


void SybaseODBC::recreateNullsTable(const std::string& notNull)
{
	dropObject("TABLE", ExecUtil::nulltest());
	std::string nl = (notNull.empty() ? " NULL " : notNull);
	try {
		session() << format("CREATE TABLE %s (i INTEGER %s, r FLOAT %s, v VARCHAR(30) %s)", ExecUtil::nulltest(),
			nl,
			nl,
			nl), now;
	}
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateNullsTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateNullsTable()"); }
}

void SybaseODBC::doMiscTable(bool haveSecCol)
{
	dropObject("TABLE", ExecUtil::misctest());
	try
	{
		session() << "CREATE TABLE " << ExecUtil::misctest() <<
			"(First VARCHAR(30),"
			<< (haveSecCol ? "Second VARBINARY(10240)," : "") <<
			"Third INTEGER,"
			"Fourth FLOAT,"
			"Fifth DATETIME)", now;
	}
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateMiscTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateMiscTable()"); }
}

void SybaseODBC::testBulkPerformance()
{
	session().setFeature("autoBind", true);
	session().setFeature("autoExtract", true);

	doMiscTable(false);
	executor().doBulkPerformance(1000);
}


void SybaseODBC::recreateMiscTable()
{
	doMiscTable(true);
}

void SybaseODBC::recreateLogTable()
{
	dropObject("TABLE", ExecUtil::pocolog());
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
			"DateTime DATETIME)";

		session() << sql, ExecUtil::pocolog(), now;
		session() << sql, ExecUtil::pocolog_a(), now;

	}
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateLogTable()"); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateLogTable()"); }
}

void SybaseODBC::testStoredProcedureIQ()
{
	Poco::SQL::Statement select(session());

	if (sybaseDriver().find("libdbodbc11") == std::string::npos)
	{
		std::cout << "Skipping testStoredProcedureIQ for driver = " << sybaseDriver() << std::endl;
		return;
	}

	try
	{
		select << "DBA.TestProc11";
		select.execute();

		Poco::SQL::RecordSet rs(select);

		std::queue< std::string > expected({ "aaa", "bbb", "ccc", "ddd" });
		bool more = rs.moveFirst();
		while (more)
		{
			if (expected.empty())
				throw std::runtime_error("unexpected result from stored proc");
			if (rs[0].isEmpty())
				throw std::runtime_error("unexpected null value returned from stored proc");

			Poco::Nullable<std::string> name = rs[0].convert<std::string>();
			if (expected.front() != name.value())
				throw std::runtime_error("got: " + name.value() + "; expected: " + expected.front());

			expected.pop();

			more = rs.moveNext();
		}

		if (!expected.empty())
			throw std::runtime_error("missing results from the stored proc");
	}
	catch(const Poco::SQL::ODBC::ODBCException& ex_)
	{
		std::cout << "caught Poco::SQL::ODBC::ODBCException:" << ex_.what() << std::endl << ex_.message() << std::endl << ex_.displayText() << std::endl;
	}
	catch (const std::exception& ex_)
	{
		std::cout << "caught:" << ex_.what() << std::endl;
	}
}

void SybaseODBC::testStoredProcedure()
{
	const std::string nm(ExecUtil::stored_proc());
	dropObject("procedure", nm);

	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		session().setFeature("autoExtract", bindValue(k + 1));

		{
			session() << "create procedure " + nm + " "
				"as "
				"select -1 where 1 = 2 ", now;
			Poco::SQL::Statement stat(session());
			stat << "{ call " << nm << "() }", now;
			Poco::SQL::RecordSet rs(stat);
			dropObject("procedure", nm);
			assertTrue (0 == rs.rowCount());
		}
		{
			Poco::Nullable<std::string> ins;
			Poco::Nullable<std::string> os = Poco::Nullable<std::string>("ab");
			Poco::Nullable<int> oi(12);
			Poco::Nullable<Poco::SQL::Date> od = Poco::Nullable<Poco::SQL::Date>(Poco::SQL::Date());
			Poco::Nullable<Poco::DateTime> odtm = Poco::Nullable<Poco::DateTime>(Poco::DateTime());
			session() << "create procedure " + nm + " @ins varchar(40), @oi integer output, @os varchar(10) output, @od date output, @dtm datetime output "
				"as\n"
				"begin\n"
				"select @oi = null\n"
				"select @os = @ins\n"
				"select @od = null\n"
				"select @dtm = null\n"
				"end"
				, now;
			session() << "{ call " << nm << "(?, ?, ?, ?, ?) }", in(ins), out(oi), out(os), out(od), out(odtm), now;
			dropObject("procedure", nm);
			assertTrue (oi.isNull());
			assertTrue (os.isNull());
			assertTrue (od.isNull());
			assertTrue (odtm.isNull());
		}
		{
			session() << "create procedure " << nm << " @c char(8) AS select @c", now;
			Poco::Nullable<std::string> ns;
			Poco::SQL::Statement stat(session());
			stat << "{ call " << nm << "(?) }", use(ns), now;
			dropObject("procedure", nm);
			Poco::SQL::RecordSet rs(stat);
			assertTrue (1 == rs.rowCount());
			bool nl = rs.isNull(size_t(0), 0);
			assertTrue ( nl );
		}
		{
			Poco::SQL::Statement stat(session());
			stat << "{ exec  -- @exType='mdExch', @exList='TRAD' }", Poco::SQL::Keywords::limit(1);
			while (!stat.done())
			{
				stat.execute();
				Poco::SQL::RecordSet rs(stat);
				assertTrue (0 == rs.rowCount());
			}
		}

		session() << "create procedure " + nm + " "
			"@outParam int output "
			"as "
			"select @outParam = -1", now;

		int i = 0;
		session() << "{ call " << nm << "(?) }", out(i), now;
		dropObject("procedure", nm);
		assertTrue (-1 == i);
		session() << "create procedure " + nm + " "
			"@inParam int, @outParam int output "
			"as "
			"select @outParam = @inParam * @inParam"
			, now;

		i = 2;
		int j = 0;
		session() << "{ call " << nm << "(?, ?)} ", in(i), out(j), now;
		dropObject("procedure", nm);
		assertTrue (4 == j);
		session() << "create procedure " + nm + " "
			"@ioParam int output "
			"as "
			"select @ioParam = @ioParam * @ioParam"
			, now;
		i = 2;
		session() << "{ call " << nm << "(?) }", io(i), now;
		dropObject("procedure", nm);
		assertTrue (4 == i);
		session() << "create procedure " + nm + " "
			"@inParam varchar(1000), @outParam varchar(1000) output "
			"as "
			"select @outParam = @inParam"
			, now;
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
		session() << "{ call " << nm << "(?,?) }", in(inParam), out(outParam), now;
		dropObject("procedure", nm);
		assertTrue (inParam == outParam);
		k += 2;
	}
}

void SybaseODBC::testStoredProcedureDynamicAny()
{
	const std::string nm(ExecUtil::stored_proc());
	dropObject("procedure", nm);

	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));

		DynamicAny i = 2;
		DynamicAny j = 0;

		session() << "create procedure " << nm << " "
			"@inParam int, @outParam int output "
			"as "
			"select @outParam = @inParam * @inParam"
			, now;

		session() << "{ call " << nm << "(?, ?) }", in(i), out(j), now;
		dropObject("procedure", nm);
		assertTrue (4 == j);

		session() << "create procedure " << nm << " @outParam int output "
			"as "
			"select @outParam = @outParam * @outParam"
			, now;

		i = 2;
		session() << "{ call " << nm << "(?) }", io(i), now;
		dropObject("procedure", nm);
		assertTrue (4 == i);

		k += 2;
	}
}

void SybaseODBC::testStoredProcedureAny()
{
	const std::string nm(ExecUtil::stored_proc());
	dropObject("procedure", nm);

	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		session().setFeature("autoExtract", bindValue(k + 1));

		Any i = 2;
		Any j = 0;

		session() << "create procedure " << nm << " "
			"@inParam int, @outParam int output "
			"as "
			"select @outParam = @inParam * @inParam"
			, now;

		session() << "{ call " << nm << "(?, ?) }", in(i), out(j), now;

		dropObject("procedure", nm);
		assertTrue (4 == AnyCast<int>(j));

		session() << "create procedure " << nm << " @outParam int output "
			"as "
			"select @outParam = @outParam * @outParam"
			, now;

		i = 2;
		session() << "{ call " << nm << "(?) }", io(i), now;
		dropObject("procedure", nm);
		assertTrue (4 == AnyCast<int>(i));

		session() << "create procedure " << nm << " "
			"@i int , @f float , @s1 varchar(10) , @d date , @t time , @dt datetime , @bin binary , @res int output"
			" as "
			"select @res = 11 where (@i is null) and (@f is null) and (@s1 is null) and (@d is null) and (@t is null) and (@dt is null) and (@bin is null)"
			, now;
		Poco::Dynamic::Var res(0);
		Poco::Dynamic::Var null;
		Poco::SQL::Statement stat(session());
		stat << "{ call " << nm << "(?,?,?,?,?,?,?  ?) }";
		stat.addBind(Poco::SQL::Keywords::bind(Poco::SQL::DATA_NULL_INTEGER));
		stat.addBind(Poco::SQL::Keywords::bind(Poco::SQL::DATA_NULL_FLOAT));
		stat.addBind(Poco::SQL::Keywords::bind(Poco::SQL::DATA_NULL_STRING));
		stat.addBind(Poco::SQL::Keywords::bind(Poco::SQL::DATA_NULL_DATE));
		stat.addBind(Poco::SQL::Keywords::bind(Poco::Dynamic::Var(Poco::SQL::DATA_NULL_TIME)));
		stat.addBind(Poco::SQL::Keywords::bind(Poco::SQL::DATA_NULL_DATETIME));
		stat.addBind(Poco::SQL::Keywords::bind(Poco::Dynamic::Var(Poco::SQL::DATA_NULL_BLOB)));
			
		stat.addBind(Poco::SQL::Keywords::out(res));
		stat.execute();
		dropObject("procedure", nm);
		assertTrue (11 == res.extract<int>());

		k += 2;
	}
}


void SybaseODBC::testTransaction()
{
	for (int i = 0; i < 8;)
	{
		doPersonTable(" UNIQUE ");
		session().setFeature("autoBind", bindValue(i));
		session().setFeature("autoExtract", bindValue(i + 1));
		executor().transaction(dbConnString());
		i += 2;
	}
}

/*static*/
CppUnit::Test* SybaseODBC::suite()
{
	if ((_pSession = init(_driver, _dsn, _uid, _pwd, _connectString)))
	{
		std::cout << "*** Connected to [" << _driver << "] test database." << std::endl;

		_pExecutor = new SQLExecutor(_driver + " SQL Executor", _pSession);

		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SybaseODBC");

		CppUnit_addTest(pSuite, SybaseODBC, testBareboneODBC);
		CppUnit_addTest(pSuite, SybaseODBC, testSyntaxError);
		CppUnit_addTest(pSuite, SybaseODBC, testSimpleAccess);
		CppUnit_addTest(pSuite, SybaseODBC, testComplexType);
		CppUnit_addTest(pSuite, SybaseODBC, testSimpleAccessVector);
		CppUnit_addTest(pSuite, SybaseODBC, testComplexTypeVector);
		CppUnit_addTest(pSuite, SybaseODBC, testSharedPtrComplexTypeVector);
		CppUnit_addTest(pSuite, SybaseODBC, testAutoPtrComplexTypeVector);
		CppUnit_addTest(pSuite, SybaseODBC, testInsertVector);
		CppUnit_addTest(pSuite, SybaseODBC, testInsertEmptyVector);
		//CppUnit_addTest(pSuite, SybaseODBC, testBigStringVector); // Sybase doesn't raise any error when a string is too big
		CppUnit_addTest(pSuite, SybaseODBC, testSimpleAccessList);
		CppUnit_addTest(pSuite, SybaseODBC, testComplexTypeList);
		CppUnit_addTest(pSuite, SybaseODBC, testInsertList);
		CppUnit_addTest(pSuite, SybaseODBC, testInsertEmptyList);
		CppUnit_addTest(pSuite, SybaseODBC, testSimpleAccessDeque);
		CppUnit_addTest(pSuite, SybaseODBC, testComplexTypeDeque);
		CppUnit_addTest(pSuite, SybaseODBC, testInsertDeque);
		CppUnit_addTest(pSuite, SybaseODBC, testInsertEmptyDeque);
		CppUnit_addTest(pSuite, SybaseODBC, testAffectedRows);
		CppUnit_addTest(pSuite, SybaseODBC, testInsertSingleBulk);
		CppUnit_addTest(pSuite, SybaseODBC, testInsertSingleBulkVec);
		CppUnit_addTest(pSuite, SybaseODBC, testInsertSingleBulkNullableVec);
		CppUnit_addTest(pSuite, SybaseODBC, testLimit);
		CppUnit_addTest(pSuite, SybaseODBC, testLimitOnce);
		CppUnit_addTest(pSuite, SybaseODBC, testLimitPrepare);
		CppUnit_addTest(pSuite, SybaseODBC, testLimitZero);
		CppUnit_addTest(pSuite, SybaseODBC, testPrepare);
		CppUnit_addTest(pSuite, SybaseODBC, testBulk);
		CppUnit_addTest(pSuite, SybaseODBC, testBulkPerformance);
		CppUnit_addTest(pSuite, SybaseODBC, testSetSimple);
		CppUnit_addTest(pSuite, SybaseODBC, testSetComplex);
		CppUnit_addTest(pSuite, SybaseODBC, testSetComplexUnique);
		CppUnit_addTest(pSuite, SybaseODBC, testMultiSetSimple);
		CppUnit_addTest(pSuite, SybaseODBC, testMultiSetComplex);
		CppUnit_addTest(pSuite, SybaseODBC, testMapComplex);
		CppUnit_addTest(pSuite, SybaseODBC, testMapComplexUnique);
		CppUnit_addTest(pSuite, SybaseODBC, testMultiMapComplex);
		CppUnit_addTest(pSuite, SybaseODBC, testSelectIntoSingle);
		CppUnit_addTest(pSuite, SybaseODBC, testSelectIntoSingleStep);
		CppUnit_addTest(pSuite, SybaseODBC, testSelectIntoSingleFail);
		CppUnit_addTest(pSuite, SybaseODBC, testLowerLimitOk);
		CppUnit_addTest(pSuite, SybaseODBC, testLowerLimitFail);
		CppUnit_addTest(pSuite, SybaseODBC, testCombinedLimits);
		CppUnit_addTest(pSuite, SybaseODBC, testCombinedIllegalLimits);
		CppUnit_addTest(pSuite, SybaseODBC, testRange);
		CppUnit_addTest(pSuite, SybaseODBC, testIllegalRange);
		CppUnit_addTest(pSuite, SybaseODBC, testSingleSelect);
		CppUnit_addTest(pSuite, SybaseODBC, testEmptyDB);
		CppUnit_addTest(pSuite, SybaseODBC, testBLOB);
		CppUnit_addTest(pSuite, SybaseODBC, testBLOBContainer);
		CppUnit_addTest(pSuite, SybaseODBC, testBLOBStmt);
		CppUnit_addTest(pSuite, SybaseODBC, testDate);
		CppUnit_addTest(pSuite, SybaseODBC, testTime);
		CppUnit_addTest(pSuite, SybaseODBC, testDateTime);
		CppUnit_addTest(pSuite, SybaseODBC, testFloat);
		CppUnit_addTest(pSuite, SybaseODBC, testDouble);
		CppUnit_addTest(pSuite, SybaseODBC, testTuple);
		CppUnit_addTest(pSuite, SybaseODBC, testTupleVector);
		CppUnit_addTest(pSuite, SybaseODBC, testInternalExtraction);
		CppUnit_addTest(pSuite, SybaseODBC, testFilter);
		CppUnit_addTest(pSuite, SybaseODBC, testInternalBulkExtraction);
		CppUnit_addTest(pSuite, SybaseODBC, testInternalStorageType);
		CppUnit_addTest(pSuite, SybaseODBC, testStoredProcedure);
		CppUnit_addTest(pSuite, SybaseODBC, testStoredProcedureIQ);
		CppUnit_addTest(pSuite, SybaseODBC, testStoredProcedureAny);
		CppUnit_addTest(pSuite, SybaseODBC, testStoredProcedureDynamicAny);
		CppUnit_addTest(pSuite, SybaseODBC, testNull);
		CppUnit_addTest(pSuite, SybaseODBC, testRowIterator);
		CppUnit_addTest(pSuite, SybaseODBC, testAsync);
		CppUnit_addTest(pSuite, SybaseODBC, testAny);
		CppUnit_addTest(pSuite, SybaseODBC, testDynamicAny);
		CppUnit_addTest(pSuite, SybaseODBC, testMultipleResults);
		//CppUnit_addTest(pSuite, SybaseODBC, testMultipleResultsNoProj); // the part with limit fails on Sybase
		CppUnit_addTest(pSuite, SybaseODBC, testSQLChannel); // this test may suffer from race conditions
		CppUnit_addTest(pSuite, SybaseODBC, testSQLLogger);
		//CppUnit_addTest(pSuite, SybaseODBC, testSessionTransaction); // this test fails when connection is fast
		CppUnit_addTest(pSuite, SybaseODBC, testTransaction);
		CppUnit_addTest(pSuite, SybaseODBC, testTransactor);
		CppUnit_addTest(pSuite, SybaseODBC, testNullable);
		CppUnit_addTest(pSuite, SybaseODBC, testReconnect);
		CppUnit_addTest(pSuite, SybaseODBC, testInsertStatReuse);

		_pExecutor = 0;
		_pSession = 0;

		return pSuite;
	}

	return 0;
}
