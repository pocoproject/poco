#include "ODBCSybaseTest.h"
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
using Poco::DynamicAny;
using Poco::NotFoundException;

#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#define SYBASE_DSN ""
#define SYBASE_UID "mstkdev"
#define SYBASE_PWD "mstkdev"
#define SYBASE_DB "mstk"
# ifdef _MSC_VER
std::string SybaseODBC::_connectString = "driver={Adaptive Server Enterprise};"
"DSURL=file://\\\\ms\\dist\\syb\\PROJ\\config\\incr\\common\\dba\\files\\sql.ini?NYT_MSTK;"
"db=" SYBASE_DB ";"
#if 1
"uid=" SYBASE_UID ";"
"pwd=" SYBASE_PWD ";"
#else
"AuthenticationClient=mitkerberos;"
"ServerPrincipal=sybase/visadb532;"
"UID=ignored;"
#endif
"DynamicPrepare=1;"
;
# else
std::string SybaseODBC::_connectString = "Driver=/ms/dist/syb/PROJ/oc/15.7.0.04/DataAccess64/ODBC/lib/libsybdrvodb-sqllen8.so;"
"Server=visadb532;"
"Port=11670;"
"db=" SYBASE_DB ";"
#if 0
"uid=" SYBASE_UID ";"
"pwd=" SYBASE_PWD ";"
#else
"AuthenticationClient=mitkerberos;"
"ServerPrincipal=sybase/visadb532;"
"UID=ignored;"
#endif
"CS=iso_1;"
"DynamicPrepare=1;";
# endif

ODBCTest::SessionPtr SybaseODBC::_pSession;
ODBCTest::ExecPtr    SybaseODBC::_pExecutor;
std::string          SybaseODBC::_driver = "";
std::string          SybaseODBC::_dsn = SYBASE_DSN;
std::string          SybaseODBC::_uid = SYBASE_UID;
std::string          SybaseODBC::_pwd = SYBASE_PWD;


SybaseODBC::SybaseODBC(const std::string& name) :
ODBCTest(name, _pSession, _pExecutor, _dsn, _uid, _pwd, _connectString)
{

}

void SybaseODBC::testBareboneODBC()
{
    if (!&session()) fail("Test not available.");
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
            if ((-204 == it->_nativeError) || (3701 /* Sybase */ == it->_nativeError))//(table does not exist)
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
    try { session() << "CREATE TABLE " << ExecUtil::nullabletest() << " (EmptyString VARCHAR(30) NULL, EmptyInteger INTEGER NULL, EmptyFloat FLOAT NULL , EmptyDateTime DATETIME NULL)", now; }
    catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreatePersonTable()"); }
    catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreatePersonTable()"); }
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
	dropObject("TABLE", ExecUtil::strings());
    try { session() << "CREATE TABLE " << ExecUtil::strings() <<" (str INTEGER)", now; }
    catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateIntsTable()"); }
    catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateIntsTable()"); }
}


void SybaseODBC::recreateStringsTable()
{
	dropObject("TABLE", ExecUtil::strings());
    try { session() << "CREATE TABLE " << ExecUtil::strings() <<" (str VARCHAR(30))", now; }
    catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateStringsTable()"); }
    catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateStringsTable()"); }
}


void SybaseODBC::recreateFloatsTable()
{
    dropObject("TABLE", ExecUtil::strings());
    try { session() << "CREATE TABLE " << ExecUtil::strings() <<" (str FLOAT)", now; }
    catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail("recreateFloatsTable()"); }
    catch (StatementException& se){ std::cout << se.toString() << std::endl; fail("recreateFloatsTable()"); }
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
    dropObject("TABLE", ExecUtil::anys() );
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
        session() << "CREATE TABLE "<< ExecUtil::misctest() <<
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

void SybaseODBC::testStoredProcedure()
{
	const std::string nm(ExecUtil::stored_proc());
	dropObject("procedure", nm);

	for (int k = 0; k < 8;)
	{
		session().setFeature("autoBind", bindValue(k));
		session().setFeature("autoExtract", bindValue(k + 1));

		session() << "create procedure " + nm + " "
			"@outParam int output "
			"as "
			"select @outParam = -1", now;

		int i = 0;
		session() << "{ call " << nm << "(?) }", out(i), now;
		dropObject("procedure", nm);
		assert(-1 == i);

		session() << "create procedure " + nm + " "
			"@inParam int, @outParam int output "
			"as "
			"select @outParam = @inParam * @inParam"
			, now;

		i = 2;
		int j = 0;
		session() << "{ call " << nm << "(?, ?)} ", in(i), out(j), now;
		dropObject("procedure", nm);
		assert(4 == j);

		session() << "create procedure " + nm + " "
			"@ioParam int output "
			"as "
			"select @ioParam = @ioParam * @ioParam"
			, now;

		i = 2;
		session() << "{ call " << nm << "(?) }", io(i), now;
		dropObject("procedure", nm);
		assert(4 == i);

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
		assert(inParam == outParam);

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
		assert(4 == j);

		session() << "create procedure " << nm << " @outParam int output "
			"as "
			"select @outParam = @outParam * @outParam"
			, now;

		i = 2;
		session() << "{ call " << nm << "(?) }", io(i), now;
		dropObject("procedure", nm);
		assert(4 == i);

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
		assert(4 == AnyCast<int>(j));

		session() << "create procedure " << nm << " @outParam int output "
			"as "
			"select @outParam = @outParam * @outParam"
			, now;

		i = 2;
		session() << "{ call " << nm << "(?) }", io(i), now;
		dropObject("procedure", nm);
		assert(4 == AnyCast<int>(i));

		k += 2;
	}
}


void SybaseODBC::testTransaction()
{
    if (!&session())fail("Test not available.");

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
        CppUnit_addTest(pSuite, SybaseODBC, testSimpleAccess);
        CppUnit_addTest(pSuite, SybaseODBC, testComplexType);
        CppUnit_addTest(pSuite, SybaseODBC, testSimpleAccessVector);
        CppUnit_addTest(pSuite, SybaseODBC, testComplexTypeVector);
        CppUnit_addTest(pSuite, SybaseODBC, testSharedPtrComplexTypeVector);
        CppUnit_addTest(pSuite, SybaseODBC, testAutoPtrComplexTypeVector);
        CppUnit_addTest(pSuite, SybaseODBC, testInsertVector);
        CppUnit_addTest(pSuite, SybaseODBC, testInsertEmptyVector);
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
        CppUnit_addTest(pSuite, SybaseODBC, testStoredProcedureAny);
        CppUnit_addTest(pSuite, SybaseODBC, testStoredProcedureDynamicAny);
        CppUnit_addTest(pSuite, SybaseODBC, testNull);
        CppUnit_addTest(pSuite, SybaseODBC, testRowIterator);
        CppUnit_addTest(pSuite, SybaseODBC, testAsync);
        CppUnit_addTest(pSuite, SybaseODBC, testAny);
        CppUnit_addTest(pSuite, SybaseODBC, testDynamicAny);
        CppUnit_addTest(pSuite, SybaseODBC, testMultipleResults);
        CppUnit_addTest(pSuite, SybaseODBC, testMultipleResultsNoProj);
        CppUnit_addTest(pSuite, SybaseODBC, testSQLChannel); // this test may suffer from race conditions
        CppUnit_addTest(pSuite, SybaseODBC, testSQLLogger);
        CppUnit_addTest(pSuite, SybaseODBC, testSessionTransaction);
        CppUnit_addTest(pSuite, SybaseODBC, testTransaction);
        CppUnit_addTest(pSuite, SybaseODBC, testTransactor);
        CppUnit_addTest(pSuite, SybaseODBC, testNullable);
        CppUnit_addTest(pSuite, SybaseODBC, testReconnect);

        _pExecutor = 0;
        _pSession = 0;

        return pSuite;
    }

    return 0;
}
