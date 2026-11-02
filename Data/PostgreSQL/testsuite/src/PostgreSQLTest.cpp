//
// PostgreSQLTest.cpp
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PostgreSQLTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Environment.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Tuple.h"
#include "Poco/NamedTuple.h"
#include "Poco/Exception.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/PostgreSQL/Connector.h"
#include "Poco/Data/PostgreSQL/Utility.h"
#include "Poco/Data/PostgreSQL/SessionHandle.h"
#include "Poco/Data/PostgreSQL/PostgreSQLException.h"
#include "Poco/Nullable.h"
#include "Poco/Data/DataException.h"
#if defined(POCO_OS_FAMILY_UNIX)
#include <sys/resource.h>
#endif
#include <iostream>
#include <memory>

#include "Poco/Data/Transaction.h"

using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using Poco::Data::PostgreSQL::ConnectionException;
using Poco::Data::PostgreSQL::Utility;
using Poco::Data::PostgreSQL::StatementException;
using Poco::format;
using Poco::NotFoundException;
using Poco::Int32;
using Poco::Nullable;
using Poco::Tuple;
using Poco::NamedTuple;
using Poco::Environment;


Poco::SharedPtr<Poco::Data::Session> PostgreSQLTest::_pSession = nullptr;
Poco::SharedPtr<SQLExecutor> PostgreSQLTest::_pExecutor = nullptr;


//
// Connection string
std::string PostgreSQLTest::_dbConnString;


//
// Parameters for barebone-test
//
std::string PostgreSQLTest::getHost()
{
	return "localhost";
}


std::string PostgreSQLTest::getPort()
{
	return "5432";
}


std::string PostgreSQLTest::getBase()
{
	return "postgres";
}


std::string PostgreSQLTest::getUser()
{
	return "postgres";
}


std::string PostgreSQLTest::getPass()
{
	if (Environment::has("APPVEYOR"))
	{
		return "Password12!";
	}
	else
	{
		return "postgres";
	}
}


PostgreSQLTest::PostgreSQLTest(const std::string& name):
	CppUnit::TestCase(name)
{
	PostgreSQL::Connector::registerConnector();
}


PostgreSQLTest::~PostgreSQLTest()
{
	PostgreSQL::Connector::unregisterConnector();
}


void PostgreSQLTest::dbInfo(Session& session)
{
	std::cout << "Server Info: "  << Utility::serverInfo(session)  << std::endl;
	std::cout << "Server Version: "   << Utility::serverVersion(session)   << std::endl;
	std::cout << "Host Info: "<< Utility::hostInfo(session)<< std::endl;
	std::cout << "Session Encoding: " << Utility::sessionEncoding(session) << std::endl;
}


void PostgreSQLTest::testConnectNoDB()
{
	std::string dbConnString;
	dbConnString +=  "host=" + getHost();
	dbConnString += " user=" + getUser();
	dbConnString +=	" password=" + getPass();
	dbConnString += " port=" + getPort();

	try
	{
		std::cout << "Attempting to Connect to [" << dbConnString << "] without database: " << std::endl;
		Session session(PostgreSQL::Connector::KEY, dbConnString);
		std::cout << "Connected to [" << dbConnString << "] without database." << std::endl;
		dbInfo(session);
		std::cout << "Disconnecting ..." << std::endl;
		session.close();
		std::cout << "Disconnected." << std::endl;
	}
	catch (ConnectionFailedException& ex)
	{
		std::cout << ex.displayText() << std::endl;
	}
	catch (ConnectionException& ex)
	{
		std::cout << ex.displayText() << std::endl;
	}
}


void PostgreSQLTest::testFailedConnectParams()
{
	std::string dbConnString;
	dbConnString +=  "host=" + getHost();
	dbConnString += " user='invalid user'";
	dbConnString +=	" password='invalid password'";
	dbConnString += " port=" + getPort();

	try
	{
		Session session(PostgreSQL::Connector::KEY, dbConnString);
		failmsg ("must fail");
	}
	catch (ConnectionFailedException& ex) {}
	catch (ConnectionException& ex) {}
}


void PostgreSQLTest::testFailedConnectURI()
{
	for (const std::string prefix : {"postgresql://", "postgres://"})
	{
		std::string dbConnString = prefix + "invalid%20user:invalid%20password@" + getHost() + ":" + getPort();

		try
		{
			Session session(PostgreSQL::Connector::KEY, dbConnString);
			failmsg ("must fail");
		}
		catch (ConnectionFailedException& ex) {}
		catch (ConnectionException& ex) {}
	}
}


void PostgreSQLTest::testPostgreSQLOIDs()
{
	if (!_pSession) fail ("Test not available.");

	std::string tableCreateString = "CREATE TABLE Test ("
		"charcol         char,"
		"bigintcol       bigint,"
		"int8col         int8,"
		"bigserialcol    bigserial,"
		"serial8col      serial8,"
		"bit1col         bit(1),"
		"bit4col         bit(4),"
		"bit6col         bit(6),"
		"bit8col         bit(8),"
		"bit9col         bit(9),"
		"bitv1col        bit varying(1),"
		"bitv4col        bit varying(4),"
		"bitv6col        bit varying(6),"
		"bitv8col        bit varying(8),"
		"bitv9col        bit varying(9),"
		"booleancol      boolean,"
		"boolcol         bool,"
		"boxcol          box,"
		"byteacol        bytea,"
		"char1col        char(1),"
		"char4col        char(4),"
		"char6col        char(6),"
		"char8col        char(8),"
		"char9col        char(9),"
		"char12col       char(12),"
		"char256col      char(256),"
		"charv1col       char varying(1),"
		"charv4col       char varying(4),"
		"charv6col       char varying(6),"
		"charv8col       char varying(8),"
		"charv9col       char varying(9),"
		"charv12col      char varying(12),"
		"charv256col     char varying(256),"
		"cidrcol         cidr,"
		"circlecol       circle,"
		"datecol         date,"
		"doubleprecol    double precision,"
		"float8col       float8,"
		"inetcol         inet,"
		"integercol      integer,"
		"intcol          int,"
		"int4col         int4,"
		"jsoncol         json,"
//		"jsonbcol        jsonb,"  // requires 9.4
		"linecol         line,"
		"lsegcol         lseg,"
		"macaddrcol      macaddr,"
		"moneycol        money,"
		"numericcol      numeric(12,3),"
		"decimalcol      decimal(12,4),"
		"pathcol         path,"
//		"pglencol        pg_lsn,"  // requires 9.4
		"pointcol        point,"
		"polygoncol      polygon,"
		"realcol         real,"
		"float4col       float4,"
		"smallintcol     smallint,"
		"int2col         int2,"
		"smallserialcol  smallserial,"
		"serial2col      serial2,"
		"serialcol       serial,"
		"serial4col      serial4,"
		"textcol         text,"
		"timewtzcol      time(6) without time zone,"
		"tsquerycol      tsquery,"
		"tsvectorcol     tsvector,"
		"txidsnapshotcol txid_snapshot,"
		"uuidcol         uuid,"
		"xmlcol          xml"
		")";

	Oid OIDArray[] = {
		1042,
		20,
		20,
		20,
		20,
		1560,
		1560,
		1560,
		1560,
		1560,
		1562,
		1562,
		1562,
		1562,
		1562,
		16,
		16,
		603,
		17,
		1042,
		1042,
		1042,
		1042,
		1042,
		1042,
		1042,
		1043,
		1043,
		1043,
		1043,
		1043,
		1043,
		1043,
		650,
		718,
		1082,
		701,
		701,
		869,
		23,
		23,
		23,
		114,
//		3802,   // requires 9.4
		628,
		601,
		829,
		790,
		1700,
		1700,
		602,
//		3220,
		600,
		604,
		700,
		700,
		21,
		21,
		21,
		21,
		23,
		23,
		25,
		1083,
		3615,
		3614,
		2970,
		2950,
		142
	};

	 _pExecutor->oidPostgreSQLTest(getHost(), getUser(), getPass(), getBase(), getPort(), tableCreateString.c_str(), OIDArray);

}


void PostgreSQLTest::testBarebonePostgreSQL()
{
	if (!_pSession) fail ("Test not available.");
/*
	std::string tableCreateString = "CREATE TABLE Test "
		"(First VARCHAR(30),"
		"Second VARCHAR(30),"
		"Third VARCHAR(30),"
		"Fourth INTEGER,"
		"Fifth FLOAT)";

	_pExecutor->barebonePostgreSQLTest(POSTGRESQL_HOST, POSTGRESQL_USER, POSTGRESQL_PWD, POSTGRESQL_DB, POSTGRESQL_PORT, tableCreateString.c_str());
*/
}



void PostgreSQLTest::testSimpleAccess()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->simpleAccess();
}


void PostgreSQLTest::testComplexType()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->complexType();
}


void PostgreSQLTest::testSimpleAccessVector()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->simpleAccessVector();
}


void PostgreSQLTest::testComplexTypeVector()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->complexTypeVector();
}


void PostgreSQLTest::testInsertVector()
{
	if (!_pSession) fail ("Test not available.");

	recreateStringsTable();
	_pExecutor->insertVector();
}


void PostgreSQLTest::testInsertEmptyVector()
{
	if (!_pSession) fail ("Test not available.");

	recreateStringsTable();
	_pExecutor->insertEmptyVector();
}


void PostgreSQLTest::testInsertSingleBulk()
{
	if (!_pSession) fail ("Test not available.");

	recreateIntsTable();
	_pExecutor->insertSingleBulk();
}


void PostgreSQLTest::testInsertSingleBulkVec()
{
	if (!_pSession) fail ("Test not available.");

	recreateIntsTable();
	_pExecutor->insertSingleBulkVec();
}


void PostgreSQLTest::testLimit()
{
	if (!_pSession) fail ("Test not available.");

	recreateIntsTable();
	_pExecutor->limits();
}


void PostgreSQLTest::testLimitZero()
{
	if (!_pSession) fail ("Test not available.");

	recreateIntsTable();
	_pExecutor->limitZero();
}


void PostgreSQLTest::testLimitOnce()
{
	if (!_pSession) fail ("Test not available.");

	recreateIntsTable();
	_pExecutor->limitOnce();

}


void PostgreSQLTest::testLimitPrepare()
{
	if (!_pSession) fail ("Test not available.");

	recreateIntsTable();
	_pExecutor->limitPrepare();
}



void PostgreSQLTest::testPrepare()
{
	if (!_pSession) fail ("Test not available.");

	recreateIntsTable();
	_pExecutor->prepare();
}


void PostgreSQLTest::testSetSimple()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->setSimple();
}


void PostgreSQLTest::testSetComplex()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->setComplex();
}


void PostgreSQLTest::testSetComplexUnique()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->setComplexUnique();
}

void PostgreSQLTest::testMultiSetSimple()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->multiSetSimple();
}


void PostgreSQLTest::testMultiSetComplex()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->multiSetComplex();
}


void PostgreSQLTest::testMapComplex()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->mapComplex();
}


void PostgreSQLTest::testMapComplexUnique()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->mapComplexUnique();
}


void PostgreSQLTest::testMultiMapComplex()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->multiMapComplex();
}


void PostgreSQLTest::testSelectIntoSingle()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->selectIntoSingle();
}


void PostgreSQLTest::testSelectIntoSingleStep()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->selectIntoSingleStep();
}


void PostgreSQLTest::testSelectIntoSingleFail()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->selectIntoSingleFail();
}


void PostgreSQLTest::testLowerLimitOk()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->lowerLimitOk();
}


void PostgreSQLTest::testSingleSelect()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->singleSelect();
}


void PostgreSQLTest::testLowerLimitFail()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->lowerLimitFail();
}


void PostgreSQLTest::testCombinedLimits()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->combinedLimits();
}



void PostgreSQLTest::testRange()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->ranges();
}


void PostgreSQLTest::testCombinedIllegalLimits()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->combinedIllegalLimits();
}



void PostgreSQLTest::testIllegalRange()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->illegalRange();
}


void PostgreSQLTest::testEmptyDB()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->emptyDB();
}


void PostgreSQLTest::testDateTime()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonDateTimeTable();
	_pExecutor->dateTime();
	recreatePersonDateTable();
	_pExecutor->date();
	recreatePersonTimeTable();
	_pExecutor->time();
}


void PostgreSQLTest::testDateTimeVariants()
{
	if (!_pSession) fail ("Test not available.");

	_pExecutor->dateTimeVariants();
}


void PostgreSQLTest::testBLOB()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonBLOBTable();
	_pExecutor->blob();

	const std::size_t maxFldSize = 65534;
	_pSession->setProperty("maxFieldSize", Poco::Any(maxFldSize-1));
	recreatePersonBLOBTable();

	try
	{
		_pExecutor->blob(maxFldSize);
		fail ("must fail");
	}
	catch (DataException&)
	{
		_pSession->setProperty("maxFieldSize", Poco::Any(maxFldSize));
	}

	recreatePersonBLOBTable();
	_pExecutor->blob(maxFldSize);

	recreatePersonBLOBTable();

	try
	{
		_pExecutor->blob(maxFldSize+1);
		fail ("must fail");
	}
	catch (DataException&) { }
}


void PostgreSQLTest::testCLOBStmt()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonCLOBTable();
	_pExecutor->clobStmt();
}


void PostgreSQLTest::testBLOBStmt()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonBLOBTable();
	_pExecutor->blobStmt();
}


void PostgreSQLTest::testUnsignedInts()
{
	if (!_pSession) fail ("Test not available.");

	recreateUnsignedIntsTable();
	_pExecutor->unsignedInts();
}


void PostgreSQLTest::testFloat()
{
	if (!_pSession) fail ("Test not available.");

	recreateFloatsTable();
	_pExecutor->floats();
}


void PostgreSQLTest::testDouble()
{
	if (!_pSession) fail ("Test not available.");

	recreateFloatsTable();
	_pExecutor->doubles();
}


void PostgreSQLTest::testUUID()
{
	if (!_pSession) fail ("Test not available.");

	recreateUUIDsTable();
	_pExecutor->uuids();
}


void PostgreSQLTest::testTuple()
{
	if (!_pSession) fail ("Test not available.");

	recreateTuplesTable();
	_pExecutor->tuples();
}


void PostgreSQLTest::testTupleVector()
{
	if (!_pSession) fail ("Test not available.");

	recreateTuplesTable();
	_pExecutor->tupleVector();
}


void PostgreSQLTest::testInternalExtraction()
{
	if (!_pSession) fail ("Test not available.");

	recreateVectorsTable();
	_pExecutor->internalExtraction();
}


void PostgreSQLTest::testNull()
{
	if (!_pSession) fail ("Test not available.");

	recreateVectorsTable();
	_pExecutor->doNull();
}


void PostgreSQLTest::testSessionTransaction()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->sessionTransaction(_dbConnString);
}


void PostgreSQLTest::testSessionTransactionNoAutoCommit()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->sessionTransactionNoAutoCommit(_dbConnString);
}


void PostgreSQLTest::testTransaction()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->transaction(_dbConnString);
}


void PostgreSQLTest::testReconnect()
{
	if (!_pSession) fail ("Test not available.");

	recreatePersonTable();
	_pExecutor->reconnect();
}


void PostgreSQLTest::testClosedSession()
{
	if (_pSession == nullptr) fail ("Test not available.");

	recreatePersonTable();

	Session session(PostgreSQL::Connector::KEY, _dbConnString);
	Statement stmt = (session << "DELETE FROM Person");

	// opening a connected session fails and leaves its transaction alone
	session.begin();
	try { session.open(_dbConnString); fail ("must fail"); }
	catch (ConnectionFailedException&) { }
	assertTrue (session.isTransaction());
	session.rollback();

	session.setFeature("autoCommit", false);
	session.close();
	assertTrue (!session.isConnected());

	try { stmt.execute(); fail ("must fail"); }
	catch (NotConnectedException&) { }

	try { (void) Utility::hostInfo(session); fail ("must fail"); }
	catch (NotConnectedException&) { }

	// a new connection starts in autocommit mode
	session.open();
	assertTrue (session.getFeature("autoCommit"));

	// the asynchronousCommit feature switches synchronous_commit, not autocommit
	session.setFeature("autoCommit", false);
	session.setFeature("asynchronousCommit", true);
	assertTrue (session.getFeature("asynchronousCommit"));
	assertTrue (!session.getFeature("autoCommit"));
	std::string synchronousCommit;
	session << "SELECT current_setting('synchronous_commit')", into(synchronousCommit), now;
	assertTrue (synchronousCommit == "off");
}


void PostgreSQLTest::testLostConnection()
{
	if (_pSession == nullptr) fail ("Test not available.");

	Session session(PostgreSQL::Connector::KEY, _dbConnString);
	int pid = 0;
	session << "SELECT pg_backend_pid()", into(pid), now;
	session.begin();
	assertTrue (session.isTransaction());
	assertTrue (!session.getFeature("autoCommit"));

	{
		Session killer(PostgreSQL::Connector::KEY, _dbConnString);
		bool terminated = false;
		killer << "SELECT pg_terminate_backend($1)", use(pid), into(terminated), now;
		assertTrue (terminated);
	}

	try { session << "SELECT 1", now; fail ("must fail"); }
	catch (StatementException&) { }
	assertTrue (!session.isConnected());

	// the server ended the transaction with the connection
	assertTrue (!session.isTransaction());

	// close() frees the dead connection, and a new connection starts in autocommit mode
	session.close();
	PGconn* pConnection = *Utility::handle(session);
	assertTrue (nullptr == pConnection);
	session.open();
	assertTrue (session.isConnected());
	assertTrue (session.getFeature("autoCommit"));
	assertTrue (!session.isTransaction());

	// open() without close() drops a dead connection too, and keeps nothing of it
	session.setFeature("asynchronousCommit", true);
	session << "SELECT pg_backend_pid()", into(pid), now;
	{
		Session killer(PostgreSQL::Connector::KEY, _dbConnString);
		bool terminated = false;
		killer << "SELECT pg_terminate_backend($1)", use(pid), into(terminated), now;
		assertTrue (terminated);
	}

	try { session << "SELECT 1", now; fail ("must fail"); }
	catch (StatementException&) { }
	assertTrue (!session.isConnected());

	session.open();
	assertTrue (session.isConnected());
	assertTrue (!session.getFeature("asynchronousCommit"));
}


void PostgreSQLTest::testFailedExecute()
{
	if (_pSession == nullptr) fail ("Test not available.");

	_pSession->setFeature("autoCommit", true);
	dropTable("UniqueInts");
	*_pSession << "CREATE TABLE UniqueInts (i INTEGER PRIMARY KEY)", now;

	// every row is executed on its own; the third one violates the key after the
	// first two have produced results, and each result must be freed exactly once
	std::vector<int> ints = {1, 2, 2};
	{
		Statement insert(*_pSession);
		insert << "INSERT INTO UniqueInts VALUES ($1)", use(ints);
		try { insert.execute(); fail ("must fail"); }
		catch (StatementException&) { }

		try { insert.execute(); fail ("must fail"); }
		catch (StatementException&) { }
	}

	int count = 0;
	*_pSession << "SELECT COUNT(*) FROM UniqueInts", into(count), now;
	assertTrue (2 == count);
	dropTable("UniqueInts");
}


void PostgreSQLTest::testStatementAfterReopen()
{
	if (_pSession == nullptr) fail ("Test not available.");

	Session session(PostgreSQL::Connector::KEY, _dbConnString);
	{
		Statement stmt = (session << "SELECT 1", now);
		session.close();
		session.open();
		session.begin();

		// the server has freed the statement with the connection that prepared it;
		// executing it fails before it reaches the server, so the transaction goes on
		try { stmt.execute(); fail ("must fail"); }
		catch (NotConnectedException&) { }
		int one = 0;
		session << "SELECT 1", into(one), now;
		assertTrue (1 == one);
	}

	// the transaction ends normally, although a statement of the closed connection was destroyed in it
	session.commit();
	assertTrue (session.getFeature("autoCommit"));
	session.begin();
	session.rollback();
}


void PostgreSQLTest::testPausedStatementAfterReopen()
{
	if (_pSession == nullptr) fail ("Test not available.");

	Session session(PostgreSQL::Connector::KEY, _dbConnString);
	int value = 0;
	Statement stmt = (session << "SELECT * FROM generate_series(1, 3)", into(value), limit(1));
	int laterValue = 0;
	Statement laterStmt = (session << "SELECT * FROM generate_series(1, 3)", into(laterValue), limit(1));
	assertTrue (1 == stmt.execute());
	assertTrue (1 == value);
	assertTrue (stmt.paused());

	session.close();
	session.open();

	// resuming does not prepare or execute the statement again; it would go on with
	// the result of the closed connection, whose next row has already been fetched
	try { stmt.execute(); fail ("must fail"); }
	catch (NotConnectedException&) { }
	assertTrue (1 == value);
	assertTrue (stmt.done());

	// a statement created before close() and first executed after open() pauses and resumes
	assertTrue (1 == laterStmt.execute());
	assertTrue (1 == laterValue);
	assertTrue (1 == laterStmt.execute());
	assertTrue (2 == laterValue);
	assertTrue (laterStmt.paused());

	// SessionHandle::reset() replaces the connection as close() and open() do
	int resetValue = 0;
	Statement resetStmt = (session << "SELECT * FROM generate_series(1, 3)", into(resetValue), limit(1));
	assertTrue (1 == resetStmt.execute());
	assertTrue (resetStmt.paused());
	assertTrue (Utility::handle(session)->reset());
	try { resetStmt.execute(); fail ("must fail"); }
	catch (NotConnectedException&) { }
	assertTrue (1 == resetValue);
	assertTrue (resetStmt.done());
}


void PostgreSQLTest::testStaleStatementInSQLTransaction()
{
	if (_pSession == nullptr) fail ("Test not available.");

	Session session(PostgreSQL::Connector::KEY, _dbConnString);
	{
		Statement stmt = (session << "SELECT 1", now);
		session.close();
		session.open();
		session << "BEGIN", now;
	}

	// a DEALLOCATE of the statement of the closed connection would fail on the server
	// and abort the transaction begun with SQL, which the session does not track
	int one = 0;
	session << "SELECT 1", into(one), now;
	assertTrue (1 == one);
	session << "ROLLBACK", now;
}


void PostgreSQLTest::testStatementOutlivesSession()
{
	if (_pSession == nullptr) fail ("Test not available.");

	std::unique_ptr<Statement> pStmt;
	{
		Session session(PostgreSQL::Connector::KEY, _dbConnString);
		pStmt = std::make_unique<Statement>((session << "SELECT 1", now));
	}

	// destroying the statement does not touch the destroyed session
	pStmt.reset();
}


namespace
{
	void countNotice(void* pCount, const PGresult*)
	{
		++*static_cast<int*>(pCount);
	}
} // namespace


void PostgreSQLTest::testRollbackWithoutTransaction()
{
	if (_pSession == nullptr) fail ("Test not available.");

	int notices = 0;
	Session session(PostgreSQL::Connector::KEY, _dbConnString);
	PGconn* pConnection = *Utility::handle(session);
	PQsetNoticeReceiver(pConnection, countNotice, &notices);

	// with no transaction block open, nothing is sent to the server,
	// which would answer a ROLLBACK with a warning
	session.rollback();
	assertTrue (0 == notices);
	assertTrue (!session.isTransaction());

	// a transaction begun with SQL, which the session does not track, is rolled back
	session << "BEGIN", now;
	session << "CREATE TEMPORARY TABLE poco_rollback_probe (i INTEGER)", now;
	session.rollback();
	assertTrue (0 == notices);
	int count = -1;
	session << "SELECT COUNT(*) FROM pg_class WHERE relname = 'poco_rollback_probe'", into(count), now;
	assertTrue (0 == count);
}


void PostgreSQLTest::testIsolationAfterLostConnection()
{
	if (_pSession == nullptr) fail ("Test not available.");

	Session session(PostgreSQL::Connector::KEY, _dbConnString);
	std::string isolation;
	session << "SELECT current_setting('default_transaction_isolation')", into(isolation), now;
	assertTrue (isolation == "read committed");

	session.setTransactionIsolation(Session::TRANSACTION_SERIALIZABLE);
	assertTrue (session.getTransactionIsolation() == Session::TRANSACTION_SERIALIZABLE);

	int pid = 0;
	session << "SELECT pg_backend_pid()", into(pid), now;
	{
		Session killer(PostgreSQL::Connector::KEY, _dbConnString);
		bool terminated = false;
		killer << "SELECT pg_terminate_backend($1)", use(pid), into(terminated), now;
		assertTrue (terminated);
	}

	try { session << "SELECT 1", now; fail ("must fail"); }
	catch (StatementException&) { }
	assertTrue (!session.isConnected());

	// open() without close() drops the dead connection; the new one starts at the default level
	session.open();
	assertTrue (session.isConnected());
	assertTrue (session.getTransactionIsolation() == Session::TRANSACTION_READ_COMMITTED);
	session << "SELECT current_setting('default_transaction_isolation')", into(isolation), now;
	assertTrue (isolation == "read committed");

	// so setting the level again reaches the server
	session.setTransactionIsolation(Session::TRANSACTION_SERIALIZABLE);
	session << "SELECT current_setting('default_transaction_isolation')", into(isolation), now;
	assertTrue (isolation == "serializable");
}


void PostgreSQLTest::testCancelFailure()
{
	if (_pSession == nullptr) fail ("Test not available.");

#if defined(POCO_OS_FAMILY_UNIX)
	Session session(PostgreSQL::Connector::KEY, _dbConnString);
	PostgreSQL::SessionHandle* pHandle = Utility::handle(session);

	// with no file descriptor to spare, the cancel request cannot open its connection to the server
	struct rlimit limit{};
	assertTrue (0 == ::getrlimit(RLIMIT_NOFILE, &limit));
	struct rlimit noFiles = limit;
	noFiles.rlim_cur = 0;
	assertTrue (0 == ::setrlimit(RLIMIT_NOFILE, &noFiles));
	bool failed = false;
	try
	{
		pHandle->cancel();
	}
	catch (StatementException&)
	{
		failed = true;
	}
	catch (...)
	{
		(void) ::setrlimit(RLIMIT_NOFILE, &limit);
		throw;
	}
	assertTrue (0 == ::setrlimit(RLIMIT_NOFILE, &limit));
	assertTrue (failed);

	// with descriptors available again, the request is sent
	pHandle->cancel();
	assertTrue (session.isConnected());
#endif
}


void PostgreSQLTest::testFailedDeallocate()
{
	if (_pSession == nullptr) fail ("Test not available.");

	Session session(PostgreSQL::Connector::KEY, _dbConnString);
	session.begin();

	// destroyed in the transaction, both statements are deallocated when it ends;
	// by then DEALLOCATE ALL has removed them, which must not make that or a
	// later commit() or rollback() fail
	int one = 0;
	session << "SELECT 1", into(one), now;
	assertTrue (1 == one);
	session << "DEALLOCATE ALL", now;

	session.commit();
	assertTrue (!session.isTransaction());
	assertTrue (session.getFeature("autoCommit"));
	session.begin();
	session.rollback();
}


void PostgreSQLTest::testTransactionWithReconnect()
{
    if (!_pSession) fail ("Test not available.");

    try
    {
        _pSession->begin();
        *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now;
        _pSession->reconnect();
        _pSession->commit();
    }
    catch (Poco::Exception& e)
    {
        _pSession->rollback();
        std::cout << e.displayText() << std::endl;
    }

    try
    {
        _pSession->begin();
        *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now;
        _pSession->commit();
    }
    catch (Poco::Exception& e)
    {
        _pSession->rollback();
        std::cout << e.displayText() << std::endl;
    }
}


void PostgreSQLTest::testSqlState()
{
	if (!_pSession) fail ("Test not available.");

	try
	{
		*_pSession << "syntax error", now;
	}
	catch (const Poco::Data::PostgreSQL::PostgreSQLException & exception)
	{
		assertTrue(exception.sqlState() == std::string("42601"));
	}
}

void PostgreSQLTest::testNullableInt()
{
	if (!_pSession) fail ("Test not available.");

	recreateNullableIntTable();

	Nullable<Int32> i1(1);
	Nullable<Int32> i2;

	int id = 1;
	*_pSession << "INSERT INTO NullableIntTest VALUES($1, $2)", use(id), use(i1), now;
	id = 2;
	*_pSession << "INSERT INTO NullableIntTest VALUES($1, $2)", use(id), use(i2), now;
	id = 3;
	i2 = 3;
	*_pSession << "INSERT INTO NullableIntTest VALUES($1, $2)", use(id), use(i2), now;

	int count = 0;
	*_pSession << "SELECT COUNT(*) FROM NullableIntTest", into(count), now;
	assertTrue (count == 3);

	Nullable<Int32> ci1;
	Nullable<Int32> ci2;
	Nullable<Int32> ci3;
	id = 1;
	*_pSession << "SELECT Value FROM NullableIntTest WHERE Id = $1", into(ci1), use(id), now;
	assertTrue (ci1 == i1);
	id = 2;
	*_pSession << "SELECT Value FROM NullableIntTest WHERE Id = $1", into(ci2), use(id), now;
	assertTrue (ci2.isNull());
	assertTrue (!(0 == ci2));
	assertTrue (0 != ci2);
	assertTrue (!(ci2 == 0));
	assertTrue (ci2 != 0);
	ci2 = 10;
	assertTrue (10 == ci2);
	assertTrue (ci2 == 10);
	assertTrue (!ci2.isNull());
	id = 3;
	*_pSession << "SELECT Value FROM NullableIntTest WHERE Id = $1", into(ci3), use(id), now;
	assertTrue (!ci3.isNull());
	assertTrue (ci3 == 3);
	assertTrue (3 == ci3);
}


void PostgreSQLTest::testNullableString()
{
	if (!_pSession) fail ("Test not available.");

	recreateNullableStringTable();

	Int32 id = 0;
	Nullable<std::string> address("Address");
	Nullable<Int32> age = 10;
	*_pSession << "INSERT INTO NullableStringTest VALUES($1, $2, $3)", use(id), use(address), use(age), now;
	id++;
	address = null;
	age = null;
	*_pSession << "INSERT INTO NullableStringTest VALUES($1, $2, $3)", use(id), use(address), use(age), now;

	Nullable<std::string> resAddress;
	Nullable<Int32> resAge;
	*_pSession << "SELECT Address, Age FROM NullableStringTest WHERE Id = $1", into(resAddress), into(resAge), use(id), now;
	assertTrue (resAddress == address);
	assertTrue (resAge == age);
	assertTrue (resAddress.isNull());
	assertTrue (null == resAddress);
	assertTrue (resAddress == null);

	resAddress = std::string("Test");
	assertTrue (!resAddress.isNull());
	assertTrue (resAddress == std::string("Test"));
	assertTrue (std::string("Test") == resAddress);
	assertTrue (null != resAddress);
	assertTrue (resAddress != null);
}


void PostgreSQLTest::testOptionalString()
{
	if (!_pSession) fail ("Test not available.");

	recreateNullableStringTable();
	_pExecutor->stdOptional();
}

void PostgreSQLTest::testTupleWithNullable()
{
	if (!_pSession) fail ("Test not available.");

	recreateNullableStringTable();

	typedef Poco::Tuple<Int32, Nullable<std::string>, Nullable<Int32> > Info;

	Info info(0, std::string("Address"), 10);
	*_pSession << "INSERT INTO NullableStringTest VALUES($1, $2, $3)", use(info), now;

	info.set<0>(info.get<0>()++);
	info.set<1>(null);
	*_pSession << "INSERT INTO NullableStringTest VALUES($1, $2, $3)", use(info), now;

	info.set<0>(info.get<0>()++);
	info.set<1>(std::string("Address!"));
	info.set<2>(null);
	*_pSession << "INSERT INTO NullableStringTest VALUES($1, $2, $3)", use(info), now;

	std::vector<Info> infos;
	infos.push_back(Info(10, std::string("A"), 0));
	// GCC incorrectly warns about uninitialized access when copying Nullable<string>
	// containing null into a Tuple. This is a false positive: when Nullable is null,
	// the internal std::optional has no value and the string is never accessed.
	// GCC's static analysis gets confused by the deep template instantiation chain.
#if defined(__GNUC__) && !defined(__clang__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
	infos.push_back(Info(11, null, 12));
	infos.push_back(Info(12, std::string("B"), null));
#if defined(__GNUC__) && !defined(__clang__)
	#pragma GCC diagnostic pop
#endif

	*_pSession << "INSERT INTO NullableStringTest VALUES($1, $2, $3)", use(infos), now;

	std::vector<Info> result;

	*_pSession << "SELECT Id, Address, Age FROM NullableStringTest", into(result), now;

	assertTrue (result[0].get<1>() == std::string("Address"));
	assertTrue (result[0].get<2>() == 10);

	assertTrue (result[1].get<1>() == null);
	assertTrue (result[1].get<2>() == 10);

	assertTrue (result[2].get<1>() == std::string("Address!"));
	assertTrue (result[2].get<2>() == null);

	assertTrue (result[3].get<1>() == std::string("A"));
	assertTrue (result[3].get<2>() == 0);

	assertTrue (result[4].get<1>() == null);
	assertTrue (result[4].get<2>() == 12);

	assertTrue (result[5].get<1>() == std::string("B"));
	assertTrue (result[5].get<2>() == null);
}


void PostgreSQLTest::testStdTupleWithOptional()
{
	if (!_pSession) fail ("Test not available.");

	recreateNullableStringTable();
	_pExecutor->stdTupleWithOptional();
}


void PostgreSQLTest::testBinarySimpleAccess()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("binaryExtraction", true);

	recreatePersonTable();
	_pExecutor->simpleAccess();
}


void PostgreSQLTest::testBinaryComplexType()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("binaryExtraction", true);

	recreatePersonTable();
	_pExecutor->complexType();
}


void PostgreSQLTest::testBinarySimpleAccessVector()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("binaryExtraction", true);

	recreatePersonTable();
	_pExecutor->simpleAccessVector();
}


void PostgreSQLTest::testBinaryComplexTypeVector()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("binaryExtraction", true);

	recreatePersonTable();
	_pExecutor->complexTypeVector();
}


void PostgreSQLTest::testBinaryInts()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("binaryExtraction", true);

	recreateUnsignedIntsTable();
	_pExecutor->unsignedInts();
}


void PostgreSQLTest::testBinaryFloat()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("binaryExtraction", true);

	recreateFloatsTable();
	_pExecutor->floats();
}


void PostgreSQLTest::testBinaryDouble()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("binaryExtraction", true);

	recreateFloatsTable();
	_pExecutor->doubles();
}


void PostgreSQLTest::testBinaryUUID()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("binaryExtraction", true);

	recreateUUIDsTable();
	_pExecutor->uuids();
}


void PostgreSQLTest::testBinaryDateTime()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("binaryExtraction", true);

	recreatePersonDateTimeTable();
	_pExecutor->dateTime();
	recreatePersonDateTable();
	_pExecutor->date();
	recreatePersonTimeTable();
	_pExecutor->time();
}


void PostgreSQLTest::testBinaryCLOBStmt()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("binaryExtraction", true);

	recreatePersonCLOBTable();
	_pExecutor->clobStmt();
}


void PostgreSQLTest::testBinaryBLOBStmt()
{
	if (!_pSession) fail ("Test not available.");

	_pSession->setFeature("binaryExtraction", true);

	recreatePersonBLOBTable();
	_pExecutor->blobStmt();
}

void PostgreSQLTest::dropTable(const std::string& tableName)
{
	try
	{
		*_pSession << "SET client_min_messages=warning", now;
		*_pSession << format("DROP TABLE IF EXISTS %s", tableName), now;
		*_pSession << "RESET client_min_messages", now;
	}
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("dropTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("dropTable()"); }
}


void PostgreSQLTest::recreatePersonTable()
{
	dropTable("Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreatePersonTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreatePersonTable()"); }
}


void PostgreSQLTest::recreatePersonBLOBTable()
{
	dropTable("Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Image BYTEA)", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreatePersonBLOBTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreatePersonBLOBTable()"); }
}

void PostgreSQLTest::recreatePersonCLOBTable()
{
	dropTable("Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Story Text)", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreatePersonCLOBTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreatePersonCLOBTable()"); }
}


void PostgreSQLTest::recreatePersonDateTimeTable()
{
	dropTable("Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Birthday TIMESTAMP)", now; } // DATETIME NOT SUPPORTED BY POSTGRES
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreatePersonDateTimeTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreatePersonDateTimeTable()"); }
}


void PostgreSQLTest::recreatePersonDateTable()
{
	dropTable("Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Birthday DATE)", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreatePersonDateTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreatePersonDateTable()"); }
}


void PostgreSQLTest::recreatePersonTimeTable()
{
	dropTable("Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Birthday TIME)", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreatePersonTimeTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreatePersonTimeTable()"); }
}


void PostgreSQLTest::recreateIntsTable()
{
	dropTable("Strings");
	try { *_pSession << "CREATE TABLE Strings (str INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreateIntsTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreateIntsTable()"); }
}


void PostgreSQLTest::recreateStringsTable()
{
	dropTable("Strings");
	try { *_pSession << "CREATE TABLE Strings (str VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreateStringsTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreateStringsTable()"); }
}


void PostgreSQLTest::recreateUnsignedIntsTable()
{
	dropTable("Strings");
	try { *_pSession << "CREATE TABLE Strings (str BIGINT)", now; } // UNSIGNED QULIFIER NOT SUPPORTED BY POSTGRES
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreateUnsignedIntegersTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreateUnsignedIntegersTable()"); }
}


void PostgreSQLTest::recreateFloatsTable()
{
	dropTable("Strings");
	try { *_pSession << "CREATE TABLE Strings (str FLOAT)", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreateFloatsTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreateFloatsTable()"); }
}


void PostgreSQLTest::recreateDoublesTable()
{
	dropTable("Strings");
	try { *_pSession << "CREATE TABLE Strings (str DOUBLE)", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreateDoublesTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreateDoublesTable()"); }
}

void PostgreSQLTest::recreateUUIDsTable()
{
	dropTable("Strings");
	try { *_pSession << "CREATE TABLE Strings (str UUID)", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreateUUIDsTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreateUUIDsTable()"); }
}


void PostgreSQLTest::recreateTuplesTable()
{
	dropTable("Tuples");
	try { *_pSession << "CREATE TABLE Tuples "
		"(i0 INTEGER, i1 INTEGER, i2 INTEGER, i3 INTEGER, i4 INTEGER, i5 INTEGER, i6 INTEGER, "
		"i7 INTEGER, i8 INTEGER, i9 INTEGER, i10 INTEGER, i11 INTEGER, i12 INTEGER, i13 INTEGER,"
		"i14 INTEGER, i15 INTEGER, i16 INTEGER, i17 INTEGER, i18 INTEGER, i19 INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreateTuplesTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreateTuplesTable()"); }
}


void PostgreSQLTest::recreateNullableIntTable()
{
	dropTable("NullableIntTest");
	try {
		*_pSession << "CREATE TABLE NullableIntTest (Id INTEGER, Value INTEGER)", now;
	}
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreateNullableIntTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreateNullableIntTable()"); }
}


void PostgreSQLTest::recreateNullableStringTable()
{
	dropTable("NullableStringTest");
	try {
		*_pSession << "CREATE TABLE NullableStringTest (Id INTEGER, Address VARCHAR(30), Age INTEGER)", now;
	}
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreateNullableStringTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreateNullableStringTable()"); }
}


void PostgreSQLTest::recreateVectorsTable()
{
	dropTable("Vectors");
	try { *_pSession << "CREATE TABLE Vectors (i0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail ("recreateVectorsTable()"); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail ("recreateVectorsTable()"); }
}


void PostgreSQLTest::setUp()
{
}


void PostgreSQLTest::tearDown()
{
	dropTable("Person");
	dropTable("Strings");
	_pSession->setFeature("binaryExtraction", false);
}


CppUnit::Test* PostgreSQLTest::suite()
{
	PostgreSQL::Connector::registerConnector();

	_dbConnString += "host=" + getHost();
	_dbConnString += " user=" + getUser();
	_dbConnString += " password=" + getPass();
	_dbConnString += " dbname=" + getBase();
	_dbConnString += " port=" + getPort();

	try
	{
		_pSession = new Session(PostgreSQL::Connector::KEY, _dbConnString);
	}
	catch (ConnectionFailedException& ex)
	{
		std::cout << ex.displayText() << std::endl;
		return nullptr;
	}

	std::cout << "*** Connected to [" << "PostgreSQL" << "] test database." << std::endl;
	dbInfo(*_pSession);

	_pExecutor = new SQLExecutor("PostgreSQL SQL Executor", _pSession);

	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PostgreSQLTest");

	CppUnit_addTest(pSuite, PostgreSQLTest, testConnectNoDB);
	CppUnit_addTest(pSuite, PostgreSQLTest, testFailedConnectParams);
	CppUnit_addTest(pSuite, PostgreSQLTest, testFailedConnectURI);
	CppUnit_addTest(pSuite, PostgreSQLTest, testPostgreSQLOIDs);
	//CppUnit_addTest(pSuite, PostgreSQLTest, testBarebonePostgreSQL);
	CppUnit_addTest(pSuite, PostgreSQLTest, testSimpleAccess);
	CppUnit_addTest(pSuite, PostgreSQLTest, testComplexType);
	CppUnit_addTest(pSuite, PostgreSQLTest, testSimpleAccessVector);
	CppUnit_addTest(pSuite, PostgreSQLTest, testComplexTypeVector);
	CppUnit_addTest(pSuite, PostgreSQLTest, testInsertVector);
	CppUnit_addTest(pSuite, PostgreSQLTest, testInsertEmptyVector);
	CppUnit_addTest(pSuite, PostgreSQLTest, testInsertSingleBulk);
	CppUnit_addTest(pSuite, PostgreSQLTest, testInsertSingleBulkVec);
	CppUnit_addTest(pSuite, PostgreSQLTest, testLimit);
	CppUnit_addTest(pSuite, PostgreSQLTest, testLimitOnce);
	CppUnit_addTest(pSuite, PostgreSQLTest, testLimitPrepare);
	CppUnit_addTest(pSuite, PostgreSQLTest, testLimitZero);
	CppUnit_addTest(pSuite, PostgreSQLTest, testPrepare);
	CppUnit_addTest(pSuite, PostgreSQLTest, testSetSimple);
	CppUnit_addTest(pSuite, PostgreSQLTest, testSetComplex);
	CppUnit_addTest(pSuite, PostgreSQLTest, testSetComplexUnique);
	CppUnit_addTest(pSuite, PostgreSQLTest, testMultiSetSimple);
	CppUnit_addTest(pSuite, PostgreSQLTest, testMultiSetComplex);
	CppUnit_addTest(pSuite, PostgreSQLTest, testMapComplex);
	CppUnit_addTest(pSuite, PostgreSQLTest, testMapComplexUnique);
	CppUnit_addTest(pSuite, PostgreSQLTest, testMultiMapComplex);
	CppUnit_addTest(pSuite, PostgreSQLTest, testSelectIntoSingle);
	CppUnit_addTest(pSuite, PostgreSQLTest, testSelectIntoSingleStep);
	CppUnit_addTest(pSuite, PostgreSQLTest, testSelectIntoSingleFail);
	CppUnit_addTest(pSuite, PostgreSQLTest, testLowerLimitOk);
	CppUnit_addTest(pSuite, PostgreSQLTest, testLowerLimitFail);
	CppUnit_addTest(pSuite, PostgreSQLTest, testCombinedLimits);
	CppUnit_addTest(pSuite, PostgreSQLTest, testCombinedIllegalLimits);
	CppUnit_addTest(pSuite, PostgreSQLTest, testRange);
	CppUnit_addTest(pSuite, PostgreSQLTest, testIllegalRange);
	CppUnit_addTest(pSuite, PostgreSQLTest, testSingleSelect);
	CppUnit_addTest(pSuite, PostgreSQLTest, testEmptyDB);
	CppUnit_addTest(pSuite, PostgreSQLTest, testDateTime);
	CppUnit_addTest(pSuite, PostgreSQLTest, testDateTimeVariants);
	//CppUnit_addTest(pSuite, PostgreSQLTest, testBLOB);
	CppUnit_addTest(pSuite, PostgreSQLTest, testCLOBStmt);
	CppUnit_addTest(pSuite, PostgreSQLTest, testBLOBStmt);
	CppUnit_addTest(pSuite, PostgreSQLTest, testUnsignedInts);
	CppUnit_addTest(pSuite, PostgreSQLTest, testFloat);
	CppUnit_addTest(pSuite, PostgreSQLTest, testDouble);
	CppUnit_addTest(pSuite, PostgreSQLTest, testUUID);
	CppUnit_addTest(pSuite, PostgreSQLTest, testTuple);
	CppUnit_addTest(pSuite, PostgreSQLTest, testTupleVector);
	CppUnit_addTest(pSuite, PostgreSQLTest, testInternalExtraction);
	CppUnit_addTest(pSuite, PostgreSQLTest, testNull);
	CppUnit_addTest(pSuite, PostgreSQLTest, testNullableInt);
	CppUnit_addTest(pSuite, PostgreSQLTest, testNullableString);
	CppUnit_addTest(pSuite, PostgreSQLTest, testOptionalString);
	CppUnit_addTest(pSuite, PostgreSQLTest, testTupleWithNullable);
	CppUnit_addTest(pSuite, PostgreSQLTest, testStdTupleWithOptional);
	CppUnit_addTest(pSuite, PostgreSQLTest, testSqlState);

	CppUnit_addTest(pSuite, PostgreSQLTest, testBinarySimpleAccess);
	CppUnit_addTest(pSuite, PostgreSQLTest, testBinaryComplexType);
	CppUnit_addTest(pSuite, PostgreSQLTest, testBinarySimpleAccessVector);
	CppUnit_addTest(pSuite, PostgreSQLTest, testBinaryComplexTypeVector);
	CppUnit_addTest(pSuite, PostgreSQLTest, testBinaryInts);
	CppUnit_addTest(pSuite, PostgreSQLTest, testBinaryFloat);
	CppUnit_addTest(pSuite, PostgreSQLTest, testBinaryDouble);
	CppUnit_addTest(pSuite, PostgreSQLTest, testBinaryUUID);
	CppUnit_addTest(pSuite, PostgreSQLTest, testBinaryDateTime);
	CppUnit_addTest(pSuite, PostgreSQLTest, testBinaryCLOBStmt);
	CppUnit_addTest(pSuite, PostgreSQLTest, testBinaryBLOBStmt);

	CppUnit_addTest(pSuite, PostgreSQLTest, testSessionTransaction);
	CppUnit_addTest(pSuite, PostgreSQLTest, testSessionTransactionNoAutoCommit);
	CppUnit_addTest(pSuite, PostgreSQLTest, testTransaction);
	CppUnit_addTest(pSuite, PostgreSQLTest, testReconnect);
	CppUnit_addTest(pSuite, PostgreSQLTest, testClosedSession);
	CppUnit_addTest(pSuite, PostgreSQLTest, testLostConnection);
	CppUnit_addTest(pSuite, PostgreSQLTest, testFailedExecute);
	CppUnit_addTest(pSuite, PostgreSQLTest, testStatementAfterReopen);
	CppUnit_addTest(pSuite, PostgreSQLTest, testPausedStatementAfterReopen);
	CppUnit_addTest(pSuite, PostgreSQLTest, testStaleStatementInSQLTransaction);
	CppUnit_addTest(pSuite, PostgreSQLTest, testStatementOutlivesSession);
	CppUnit_addTest(pSuite, PostgreSQLTest, testRollbackWithoutTransaction);
	CppUnit_addTest(pSuite, PostgreSQLTest, testIsolationAfterLostConnection);
	CppUnit_addTest(pSuite, PostgreSQLTest, testCancelFailure);
	CppUnit_addTest(pSuite, PostgreSQLTest, testFailedDeallocate);
    CppUnit_addTest(pSuite, PostgreSQLTest, testTransactionWithReconnect);

	return pSuite;
}
