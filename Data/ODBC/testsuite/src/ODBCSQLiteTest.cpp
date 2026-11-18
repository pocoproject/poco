//
// ODBCSQLiteTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ODBCSQLiteTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include "Poco/Any.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/Data/BulkExtraction.h"
#include "Poco/Nullable.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Diagnostics.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/ODBC/ODBCStatementImpl.h"
#include <sqltypes.h>
#include <iostream>
#include <exception>
#include <deque>
#include <list>
#include <string>
#include <vector>


using namespace Poco::Data::Keywords;
using Poco::Data::ODBC::Utility;
using Poco::Data::ODBC::ConnectionException;
using Poco::Data::ODBC::StatementException;
using Poco::Data::ODBC::StatementDiagnostics;
using Poco::format;
using Poco::NotFoundException;


#define SQLITE_ODBC_DRIVER "SQLite3 ODBC Driver"
#define SQLITE_DSN "PocoDataSQLiteTest"
#define SQLITE_DB "dummy.db"


ODBCTest::SessionPtr ODBCSQLiteTest::_pSession;
ODBCTest::ExecPtr    ODBCSQLiteTest::_pExecutor;
std::string          ODBCSQLiteTest::_driver = SQLITE_ODBC_DRIVER;
std::string          ODBCSQLiteTest::_dsn = SQLITE_DSN;
std::string          ODBCSQLiteTest::_uid = "";
std::string          ODBCSQLiteTest::_pwd = "";
std::string          ODBCSQLiteTest::_connectString = "Driver=" SQLITE_ODBC_DRIVER
	";Database=" SQLITE_DB ";";


ODBCSQLiteTest::ODBCSQLiteTest(const std::string& name):
	ODBCTest(name, _pSession, _pExecutor, _dsn, _uid, _pwd, _connectString)
{
}


ODBCSQLiteTest::~ODBCSQLiteTest()
{
}


void ODBCSQLiteTest::testBareboneODBC()
{
	std::string tableCreateString = "CREATE TABLE Test "
		"(First VARCHAR(30),"
		"Second VARCHAR(30),"
		"Third BLOB,"
		"Fourth INTEGER,"
		"Fifth REAL,"
		"Sixth TIMESTAMP)";

	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND);

	tableCreateString = "CREATE TABLE Test "
		"(First VARCHAR(30),"
		"Second VARCHAR(30),"
		"Third BLOB,"
		"Fourth INTEGER,"
		"Fifth REAL,"
		"Sixth DATETIME)";

	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND);

	tableCreateString = "CREATE TABLE Test "
		"(First VARCHAR(30),"
		"Second VARCHAR(30),"
		"Third BLOB,"
		"Fourth INTEGER,"
		"Fifth REAL,"
		"Sixth DATE)";

	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL);
	executor().bareboneODBCTest(dbConnString(), tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND);
}


void ODBCSQLiteTest::testAffectedRows()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		// see SQLiteStatementImpl::affectedRows() documentation for explanation
		// why "WHERE 1" is necessary here
		_pExecutor->affectedRows("WHERE 1");
		i += 2;
	}
}


void ODBCSQLiteTest::testNull()
{
	if (!_pSession) fail ("Test not available.");

	// test for NOT NULL violation exception
	for (int i = 0; i < 8;)
	{
		recreateNullsTable("NOT NULL");
		session().setFeature("autoBind", bindValue(i));
		session().setFeature("autoExtract", bindValue(i+1));
		_pExecutor->notNulls({"HY000"});
		i += 2;
	}
}


void ODBCSQLiteTest::dropObject(const std::string& type, const std::string& name)
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
			if (1 == it->_nativeError)//(no such table)
			{
				ignoreError = true;
				break;
			}
		}

		if (!ignoreError)
		{
			std::cout << ex.toString() << std::endl;
			throw;
		}
	}
}


void ODBCSQLiteTest::recreateNullableTable()
{
	dropObject("TABLE", "NullableTest");
	try { *_pSession << "CREATE TABLE NullableTest (EmptyString VARCHAR(30) NULL, EmptyInteger INTEGER NULL, EmptyFloat REAL NULL, EmptyDateTime TIMESTAMP NULL, EmptyDate DATE NULL)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonTable()"); }
}


void ODBCSQLiteTest::recreatePersonTable()
{
	dropObject("TABLE", "Person");
	try { session() << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonTable()"); }
}


void ODBCSQLiteTest::recreatePersonBLOBTable()
{
	dropObject("TABLE", "Person");
	// Bound LOB buffers are sized from the column size the driver reports (the declared
	// size for SQLite ODBC) and longer values are rejected; blob() round-trips 1024 bytes.
	try { session() << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Image BLOB(1024))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
}


void ODBCSQLiteTest::recreatePersonDateTimeTable()
{
	dropObject("TABLE", "Person");
	try { session() << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Born TIMESTAMP)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonDateTimeTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonDateTimeTable()"); }
}


void ODBCSQLiteTest::recreateIntsTable()
{
	dropObject("TABLE", "Strings");
	try { session() << "CREATE TABLE Strings (str INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateIntsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateIntsTable()"); }
}


void ODBCSQLiteTest::recreateStringsTable()
{
	dropObject("TABLE", "Strings");
	try { session() << "CREATE TABLE Strings (str VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateStringsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateStringsTable()"); }
}


void ODBCSQLiteTest::recreateFloatsTable()
{
	dropObject("TABLE", "Strings");
	try { session() << "CREATE TABLE Strings (str REAL)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateFloatsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateFloatsTable()"); }
}


void ODBCSQLiteTest::recreateTuplesTable()
{
	dropObject("TABLE", "Tuples");
	try { session() << "CREATE TABLE Tuples "
		"(int0 INTEGER, int1 INTEGER, int2 INTEGER, int3 INTEGER, int4 INTEGER, int5 INTEGER, int6 INTEGER, "
		"int7 INTEGER, int8 INTEGER, int9 INTEGER, int10 INTEGER, int11 INTEGER, int12 INTEGER, int13 INTEGER,"
		"int14 INTEGER, int15 INTEGER, int16 INTEGER, int17 INTEGER, int18 INTEGER, int19 INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateTuplesTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateTuplesTable()"); }
}


void ODBCSQLiteTest::recreateVectorsTable()
{
	dropObject("TABLE", "Vectors");
	try { session() << "CREATE TABLE Vectors (int0 INTEGER, flt0 REAL, str0 VARCHAR)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateVectorsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateVectorsTable()"); }
}


void ODBCSQLiteTest::recreateAnysTable()
{
	dropObject("TABLE", "Anys");
	try { session() << "CREATE TABLE Anys (int0 INTEGER, flt0 REAL, str0 VARCHAR)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateAnysTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateAnysTable()"); }
}


void ODBCSQLiteTest::recreateNullsTable(const std::string& notNull)
{
	dropObject("TABLE", "NullTest");
	try { session() << format("CREATE TABLE NullTest (i INTEGER %s, r REAL %s, v VARCHAR(30) %s)",
		notNull,
		notNull,
		notNull), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateNullsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateNullsTable()"); }
}


void ODBCSQLiteTest::recreateMiscTable()
{
	dropObject("TABLE", "MiscTest");
	try
	{
		// SQLite fails with BLOB bulk operations
		session() << "CREATE TABLE MiscTest "
			"(First VARCHAR(30),"
			//"Second BLOB,"
			"Third INTEGER,"
			"Fourth REAL,"
			"Fifth DATETIME)", now;
	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateMiscTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateMiscTable()"); }
}


void ODBCSQLiteTest::recreateLogTable()
{
	dropObject("TABLE", "T_POCO_LOG");
	dropObject("TABLE", "T_POCO_LOG_ARCHIVE");

	try
	{
		std::string sql = "CREATE TABLE %s "
			"(Source VARCHAR,"
			"Name VARCHAR,"
			"ProcessId INTEGER,"
			"Thread VARCHAR, "
			"ThreadId INTEGER,"
			"Priority INTEGER,"
			"Text VARCHAR,"
			"DateTime DATETIME)";

		session() << sql, "T_POCO_LOG", now;
		session() << sql, "T_POCO_LOG_ARCHIVE", now;

	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateLogTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateLogTable()"); }
}


namespace
{
	std::string boundSizeExceeded(std::size_t dataSize, std::size_t capacity)
		/// The part of Extractor::BOUND_SIZE_EXCEEDED_FMT that identifies the bound
		/// buffer check. Driver errors, checkDataSize() and other DataException
		/// subclasses carry different text.
	{
		return Poco::format("the value (%z bytes) does not fit into the bound buffer (%z bytes)", dataSize, capacity);
	}


	template <typename L>
	L makeLOB(std::size_t size, char fill)
	{
		L lob;
		lob.assignVal(size, static_cast<typename L::ValueType>(fill));
		return lob;
	}


	template <typename L>
	L toLOB(const Poco::Data::CLOB& clob)
	{
		return L(std::vector<typename L::ValueType>(clob.begin(), clob.end()));
	}
}


std::size_t ODBCSQLiteTest::probeColumnLength(const std::string& query, bool character)
{
	// Column metadata is only filled for statements without user extractions
	// (ODBCStatementImpl::makeInternalExtractors), hence no into(). The probe
	// extracts existing rows itself, so it must run on the empty table.
	Poco::Data::Statement probe(session());
	probe << query, now;
	Poco::Data::RecordSet rs(probe);
	const Poco::Data::MetaColumn::ColumnDataType type = rs.columnType(0);
	if (character)
		assertTrue(type == Poco::Data::MetaColumn::FDT_STRING || type == Poco::Data::MetaColumn::FDT_WSTRING);
	else
		assertTrue(type == Poco::Data::MetaColumn::FDT_BLOB);
	// Below maxFieldSize (default 1024) the bound buffer is the described size;
	// Preparator::maxDataSize() adds room for the terminator of character columns.
	const std::size_t length = rs.columnLength(0);
	assertTrue(length > 1 && length < 1024);
	return length;
}


template <typename L>
void ODBCSQLiteTest::checkBoundLOB(const std::string& query, const L& expected, std::size_t capacity)
{
	L output;
	bool rejected = false;
	try
	{
		session() << query, into(output), now;
	}
	catch (const Poco::Data::ODBC::UnknownDataLengthException& ex)
	{
		failmsg("the driver reported an unknown length (SQL_NO_TOTAL): " + ex.displayText());
	}
	catch (const Poco::Data::ODBC::ODBCException& ex)
	{
		// ODBC errors are DataExceptions too; they must not pass for the capacity check.
		failmsg("driver error instead of the capacity check: " + ex.displayText());
	}
	catch (const Poco::Data::DataException& ex)
	{
		assertTrue(ex.message().find(boundSizeExceeded(expected.size(), capacity)) != std::string::npos);
		rejected = true;
	}
	if (expected.size() > capacity) assertTrue(rejected);
	else
	{
		assertFalse(rejected);
		assertTrue(output == expected);
	}
}


template <typename C>
void ODBCSQLiteTest::checkBoundLOBBulk(const std::string& query, const std::vector<Poco::Data::CLOB>& expected)
{
	using L = typename C::value_type;
	const Poco::UInt32 rows = static_cast<Poco::UInt32>(expected.size());
	// BulkExtraction keeps a container of the right size, so every element must
	// be overwritten: NULL rows with an empty LOB, not left stale. The elements
	// share the sentinel's content, so emptying one in place would empty it too.
	const L sentinel = makeLOB<L>(5, 's');
	C output(rows, sentinel);
	Poco::Data::Statement stmt(session());
	stmt << query, into(output, bulk(rows));
	try
	{
		stmt.execute();
	}
	catch (const Poco::Exception& ex)
	{
		failmsg("bulk LOB extraction failed: " + ex.displayText());
	}
	catch (const std::exception& ex)
	{
		// for example std::length_error, if a NULL indicator were used as a length
		failmsg(std::string("bulk LOB extraction failed: ") + ex.what());
	}
	assertTrue(sentinel == makeLOB<L>(5, 's'));
	assertTrue(output.size() == rows);
	typename C::const_iterator it = output.begin();
	for (Poco::UInt32 row = 0; row < rows; ++row, ++it)
	{
		assertTrue(*it == toLOB<L>(expected[row]));
	}
}


template <typename C>
void ODBCSQLiteTest::checkBoundLOBBulkRejected(const std::string& query, Poco::UInt32 rows, std::size_t capacity)
{
	C output;
	bool rejected = false;
	try
	{
		session() << query, into(output, bulk(rows)), now;
	}
	catch (const Poco::Data::ODBC::UnknownDataLengthException& ex)
	{
		failmsg("the driver reported an unknown length (SQL_NO_TOTAL): " + ex.displayText());
	}
	catch (const Poco::Data::ODBC::ODBCException& ex)
	{
		failmsg("driver error instead of the capacity check: " + ex.displayText());
	}
	catch (const Poco::Data::DataException& ex)
	{
		assertTrue(ex.message().find(boundSizeExceeded(capacity + 1, capacity)) != std::string::npos);
		rejected = true;
	}
	assertTrue(rejected);
}


void ODBCSQLiteTest::testBoundLOBCapacity()
{
	session().setFeature("autoBind", true);
	session().setFeature("autoExtract", true);
	session() << "DROP TABLE IF EXISTS BoundLOBCapacity", now;
	session() << "CREATE TABLE BoundLOBCapacity (Payload BLOB(32))", now;
	const std::string query("SELECT Payload FROM BoundLOBCapacity");
	const std::size_t capacity = probeColumnLength(query, false);

	// NULL must stay NULL: Preparator::actualDataSize() returns SQL_NULL_DATA
	// as SIZE_MAX, so isNull() has to come before the size check.
	session() << "INSERT INTO BoundLOBCapacity VALUES (NULL)", now;
	try
	{
		Poco::Nullable<Poco::Data::CLOB> nullable(Poco::Data::CLOB("stale", 5));
		session() << query, into(nullable), now;
		assertTrue(nullable.isNull());
		Poco::Data::BLOB output = makeLOB<Poco::Data::BLOB>(5, 's');
		session() << query, into(output), now;
		assertTrue(output.size() == 0);
	}
	catch (const Poco::Data::DataException& ex)
	{
		failmsg("NULL LOB rejected: " + ex.displayText());
	}

	for (std::size_t size: {std::size_t(0), capacity - 1, capacity, capacity + 1, std::size_t(1024)})
	{
		session() << "DELETE FROM BoundLOBCapacity", now;
		Poco::Data::CLOB input = makeLOB<Poco::Data::CLOB>(size, 'x');
		session() << "INSERT INTO BoundLOBCapacity VALUES (?)", use(input), now;
		// scalar CLOB, then BLOB
		checkBoundLOB(query, input, capacity);
		checkBoundLOB(query, makeLOB<Poco::Data::BLOB>(size, 'x'), capacity);
	}
	session() << "DROP TABLE BoundLOBCapacity", now;
}


void ODBCSQLiteTest::testBoundLOBBulkCapacity()
{
	session().setFeature("autoBind", true);
	session().setFeature("autoExtract", true);
	session() << "DROP TABLE IF EXISTS BoundLOBBulkCapacity", now;
	session() << "CREATE TABLE BoundLOBBulkCapacity (Id INTEGER, Payload BLOB(32))", now;
	const std::string query("SELECT Payload FROM BoundLOBBulkCapacity ORDER BY Id");
	const std::size_t capacity = probeColumnLength(query, false);

	// One row per bulk slot: POCO sets no SQL_ATTR_ROWS_FETCHED_PTR and extracts
	// all bulk(rows) slots. Distinct fill bytes expose a stride mismatch; the last
	// row fills the last slot exactly. This also shows that the driver fills
	// bulk row arrays at all.
	const Poco::UInt32 rows = 4;
	std::vector<Poco::Data::CLOB> expected(rows);
	for (Poco::UInt32 row = 0; row < rows; ++row)
	{
		int id = static_cast<int>(row);
		expected[row] = makeLOB<Poco::Data::CLOB>((row % 2) ? capacity : capacity - 1, static_cast<char>('a' + row));
		session() << "INSERT INTO BoundLOBBulkCapacity VALUES (?, ?)", use(id), use(expected[row]), now;
	}
	checkBoundLOBBulk<std::vector<Poco::Data::CLOB>>(query, expected);
	session() << "DROP TABLE BoundLOBBulkCapacity", now;
}


void ODBCSQLiteTest::testBoundLOBBulkNull()
{
	session().setFeature("autoBind", true);
	session().setFeature("autoExtract", true);
	session() << "DROP TABLE IF EXISTS BoundLOBBulkNull", now;
	session() << "CREATE TABLE BoundLOBBulkNull (Id INTEGER, Payload BLOB(32))", now;
	const std::string query("SELECT Payload FROM BoundLOBBulkNull ORDER BY Id");
	const std::size_t capacity = probeColumnLength(query, false);

	// NULL in the first, a middle and the last slot. For NULL the driver sets the
	// indicator to SQL_NULL_DATA, which Preparator::actualDataSize() returns as
	// SIZE_MAX; the values between the NULLs must keep their rows.
	const Poco::UInt32 rows = 5;
	std::vector<Poco::Data::CLOB> expected(rows);
	for (Poco::UInt32 row = 0; row < rows; ++row)
	{
		int id = static_cast<int>(row);
		if (row % 2 == 0)
		{
			session() << "INSERT INTO BoundLOBBulkNull VALUES (?, NULL)", use(id), now;
		}
		else
		{
			expected[row] = makeLOB<Poco::Data::CLOB>((row == 1) ? capacity - 1 : capacity, static_cast<char>('a' + row));
			session() << "INSERT INTO BoundLOBBulkNull VALUES (?, ?)", use(id), use(expected[row]), now;
		}
	}
	checkBoundLOBBulk<std::vector<Poco::Data::CLOB>>(query, expected);
	session() << "DROP TABLE BoundLOBBulkNull", now;
}


void ODBCSQLiteTest::testBoundLOBBulkOverflow()
{
	session().setFeature("autoBind", true);
	session().setFeature("autoExtract", true);
	session() << "DROP TABLE IF EXISTS BoundLOBBulkOverflow", now;
	session() << "CREATE TABLE BoundLOBBulkOverflow (Id INTEGER, Payload BLOB(32))", now;
	const std::string query("SELECT Payload FROM BoundLOBBulkOverflow ORDER BY Id");
	const std::size_t capacity = probeColumnLength(query, false);

	// The bulk buffer holds rows * capacity bytes. The oversized value is first
	// the last row, whose slot ends the allocation, then a middle row, whose slot
	// is followed by the next row: both must be rejected.
	const Poco::UInt32 rows = 4;
	for (Poco::UInt32 oversized: {Poco::UInt32(rows - 1), Poco::UInt32(1)})
	{
		session() << "DELETE FROM BoundLOBBulkOverflow", now;
		for (Poco::UInt32 row = 0; row < rows; ++row)
		{
			int id = static_cast<int>(row);
			Poco::Data::CLOB input = makeLOB<Poco::Data::CLOB>((row == oversized) ? capacity + 1 : capacity, static_cast<char>('a' + row));
			session() << "INSERT INTO BoundLOBBulkOverflow VALUES (?, ?)", use(id), use(input), now;
		}
		checkBoundLOBBulkRejected<std::vector<Poco::Data::CLOB>>(query, rows, capacity);
	}
	session() << "DROP TABLE BoundLOBBulkOverflow", now;
}


void ODBCSQLiteTest::testBoundBLOBBulk()
{
	session().setFeature("autoBind", true);
	session().setFeature("autoExtract", true);
	session() << "DROP TABLE IF EXISTS BoundBLOBBulk", now;
	session() << "CREATE TABLE BoundBLOBBulk (Id INTEGER, Payload BLOB(32))", now;
	const std::string query("SELECT Payload FROM BoundBLOBBulk ORDER BY Id");
	const std::size_t capacity = probeColumnLength(query, false);

	// BLOB containers used to be bound as char arrays but read as unsigned char
	// arrays (null pointer dereference). NULLs and in-capacity values first ...
	const Poco::UInt32 rows = 4;
	std::vector<Poco::Data::CLOB> expected(rows);
	for (Poco::UInt32 row = 0; row < rows; ++row)
	{
		int id = static_cast<int>(row);
		if (row % 2 == 0)
		{
			session() << "INSERT INTO BoundBLOBBulk VALUES (?, NULL)", use(id), now;
		}
		else
		{
			expected[row] = makeLOB<Poco::Data::CLOB>((row == 1) ? capacity - 1 : capacity, static_cast<char>('a' + row));
			session() << "INSERT INTO BoundBLOBBulk VALUES (?, ?)", use(id), use(expected[row]), now;
		}
	}
	checkBoundLOBBulk<std::vector<Poco::Data::BLOB>>(query, expected);
	checkBoundLOBBulk<std::deque<Poco::Data::BLOB>>(query, expected);
	checkBoundLOBBulk<std::list<Poco::Data::BLOB>>(query, expected);

	// ... then an oversized last row.
	session() << "DELETE FROM BoundBLOBBulk", now;
	for (Poco::UInt32 row = 0; row < rows; ++row)
	{
		int id = static_cast<int>(row);
		Poco::Data::CLOB input = makeLOB<Poco::Data::CLOB>((row == rows - 1) ? capacity + 1 : capacity, static_cast<char>('a' + row));
		session() << "INSERT INTO BoundBLOBBulk VALUES (?, ?)", use(id), use(input), now;
	}
	checkBoundLOBBulkRejected<std::vector<Poco::Data::BLOB>>(query, rows, capacity);
	checkBoundLOBBulkRejected<std::deque<Poco::Data::BLOB>>(query, rows, capacity);
	checkBoundLOBBulkRejected<std::list<Poco::Data::BLOB>>(query, rows, capacity);
	session() << "DROP TABLE BoundBLOBBulk", now;
}


void ODBCSQLiteTest::testBoundStringBulk()
{
	session().setFeature("autoBind", true);
	session().setFeature("autoExtract", true);
	session().setFeature("emptyStringIsNull", false);
	session().setFeature("forceEmptyString", false);
	session() << "DROP TABLE IF EXISTS BoundStringBulk", now;
	session() << "CREATE TABLE BoundStringBulk (Id INTEGER, Text VARCHAR(32))", now;
	const std::string query("SELECT Text FROM BoundStringBulk ORDER BY Id");
	const std::size_t capacity = probeColumnLength(query, true);

	// NULL in the first, a middle and the last slot, values of different lengths
	// between them. A bulk row holds the column size plus the terminator, so a
	// value as long as the column comes back unchanged.
	// Literals rather than parameters keep the Binder's size checks out of the test.
	const Poco::UInt32 rows = 5;
	std::vector<std::string> expected(rows);
	for (Poco::UInt32 row = 0; row < rows; ++row)
	{
		if (row % 2 == 0)
		{
			session() << Poco::format("INSERT INTO BoundStringBulk VALUES (%u, NULL)", row), now;
		}
		else
		{
			expected[row].assign((row == 1) ? capacity : 1, static_cast<char>('a' + row));
			session() << Poco::format("INSERT INTO BoundStringBulk VALUES (%u, '%s')", row, expected[row]), now;
		}
	}

	{
		// Pre-filled container: NULL rows must come back empty, neither stale
		// nor by passing SQL_NULL_DATA (SIZE_MAX) to std::string::assign().
		std::vector<std::string> output(rows, std::string("stale"));
		Poco::Data::Statement stmt(session());
		stmt << query, into(output, bulk(rows));
		try
		{
			stmt.execute();
		}
		catch (const Poco::Exception& ex)
		{
			failmsg("bulk string extraction failed: " + ex.displayText());
		}
		catch (const std::exception& ex)
		{
			// for example std::length_error, if a NULL indicator were used as a length
			failmsg(std::string("bulk string extraction failed: ") + ex.what());
		}
		for (Poco::UInt32 row = 0; row < rows; ++row)
		{
			assertTrue(output[row] == expected[row]);
		}
	}
	session() << "DROP TABLE BoundStringBulk", now;
}


void ODBCSQLiteTest::testBoundStringBulkOverflow()
{
	session().setFeature("autoBind", true);
	session().setFeature("autoExtract", true);
	session() << "DROP TABLE IF EXISTS BoundStringBulkOverflow", now;
	session() << "CREATE TABLE BoundStringBulkOverflow (Id INTEGER, Text VARCHAR(32))", now;
	const std::string query("SELECT Text FROM BoundStringBulkOverflow ORDER BY Id");
	const std::size_t capacity = probeColumnLength(query, true);

	// The two cases of testBoundLOBBulkOverflow for std::string containers. A row
	// holds capacity characters plus the terminator; the driver truncates a longer
	// value and reports its full length.
	const Poco::UInt32 rows = 4;
	for (Poco::UInt32 oversized: {Poco::UInt32(rows - 1), Poco::UInt32(1)})
	{
		session() << "DELETE FROM BoundStringBulkOverflow", now;
		for (Poco::UInt32 row = 0; row < rows; ++row)
		{
			std::string text((row == oversized) ? capacity + 1 : capacity, static_cast<char>('a' + row));
			session() << Poco::format("INSERT INTO BoundStringBulkOverflow VALUES (%u, '%s')", row, text), now;
		}

		std::vector<std::string> output;
		bool rejected = false;
		try
		{
			session() << query, into(output, bulk(rows)), now;
		}
		catch (const Poco::Data::ODBC::UnknownDataLengthException& ex)
		{
			failmsg("the driver reported an unknown length (SQL_NO_TOTAL): " + ex.displayText());
		}
		catch (const Poco::Data::ODBC::ODBCException& ex)
		{
			// ODBC errors are DataExceptions too; they must not pass for the capacity check.
			failmsg("driver error instead of the capacity check: " + ex.displayText());
		}
		catch (const Poco::Data::DataException& ex)
		{
			assertTrue(ex.message().find(boundSizeExceeded(capacity + 1, capacity + 1)) != std::string::npos);
			rejected = true;
		}
		assertTrue(rejected);
	}
	session() << "DROP TABLE BoundStringBulkOverflow", now;
}


void ODBCSQLiteTest::testBulkThenScalarLOB()
{
	// A bulk extraction sets the rowset size and a non-bulk extraction is bound
	// to one row, so Statement must reject the mix, also when a scalar parameter
	// in between resets the bulk mode flags. The statements are built, never
	// executed, so a regression cannot write past a buffer.
	std::vector<int> ids;
	Poco::Data::CLOB payload;
	int minId = 0;
	{
		Poco::Data::Statement stmt(session());
		stmt << "SELECT Id, Payload FROM BulkThenScalarLOB", into(ids, bulk(100));
		try
		{
			stmt, into(payload);
			fail("non-bulk extraction accepted after bulk extraction");
		}
		catch (const Poco::InvalidAccessException&)
		{
		}
	}
	{
		Poco::Data::Statement stmt(session());
		stmt << "SELECT Id, Payload FROM BulkThenScalarLOB WHERE Id >= ?", into(ids, bulk(100)), use(minId);
		try
		{
			stmt, into(payload);
			fail("non-bulk extraction accepted after bulk extraction and a scalar parameter");
		}
		catch (const Poco::InvalidAccessException&)
		{
		}
	}
	{
		Poco::Data::Statement stmt(session());
		stmt << "SELECT Id FROM BulkThenScalarLOB WHERE Id >= ?", into(ids, bulk(4)), use(minId);
		try
		{
			stmt, limit(100);
			fail("limit different from the bulk size accepted after a scalar parameter");
		}
		catch (const Poco::InvalidArgumentException&)
		{
		}
	}
}


void ODBCSQLiteTest::testAnyUntypedColumn()
{
	// The driver can describe a result column without a declared type, such as
	// COUNT(*) or an expression, differently after the fetch than before the
	// execution. Bound extraction into Poco::Any and Poco::Dynamic::Var reads
	// the value as the type its buffer was bound for. SQLChannel extracts its
	// archive row count this way.
	session().setFeature("autoBind", true);
	session().setFeature("autoExtract", true);
	session() << "DROP TABLE IF EXISTS AnyUntypedColumn", now;
	session() << "CREATE TABLE AnyUntypedColumn (Id INTEGER)", now;
	session() << "INSERT INTO AnyUntypedColumn VALUES (1)", now;
	session() << "INSERT INTO AnyUntypedColumn VALUES (2)", now;

	Poco::Dynamic::Var count;
	session() << "SELECT COUNT(*) FROM AnyUntypedColumn", into(count), now;
	assertTrue (count == 2);
	Poco::Any anyCount;
	session() << "SELECT COUNT(*) FROM AnyUntypedColumn", into(anyCount), now;
	assertFalse (anyCount.empty());
	assertTrue (anyCount.type() == count.type());

	Poco::Dynamic::Var sum;
	session() << "SELECT Id + 40 FROM AnyUntypedColumn WHERE Id = 2", into(sum), now;
	assertTrue (sum == 42);
	Poco::Any anySum;
	session() << "SELECT Id + 40 FROM AnyUntypedColumn WHERE Id = 2", into(anySum), now;
	assertFalse (anySum.empty());
	assertTrue (anySum.type() == sum.type());
	session() << "DROP TABLE AnyUntypedColumn", now;
}


CppUnit::Test* ODBCSQLiteTest::suite()
{
	if ((_pSession = init(_driver, _dsn, _uid, _pwd, _connectString)))
	{
		std::cout << "*** Connected to [" << _driver << "] test database." << std::endl;

		_pExecutor = new SQLExecutor(_driver + " SQL Executor", _pSession);

		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ODBCSQLiteTest");
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBoundLOBCapacity);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBoundLOBBulkCapacity);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBoundLOBBulkNull);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBoundLOBBulkOverflow);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBoundBLOBBulk);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBoundStringBulk);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBoundStringBulkOverflow);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBulkThenScalarLOB);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBulkRowsetBackstop);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testParameterArraySize);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBulkFullWidthStrings);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testManualBLOB);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testManualUUIDLength);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testTranscodedBindingOwnership);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBindingOutlivesStatement);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBoundLongValue);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBulkLongStrings);

		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBareboneODBC);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testConnection);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSession);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSessionPool);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testZeroRows);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSimpleAccess);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testComplexType);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSimpleAccessVector);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testComplexTypeVector);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSharedPtrComplexTypeVector);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testAutoPtrComplexTypeVector);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testInsertVector);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testInsertEmptyVector);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSimpleAccessList);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testComplexTypeList);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testInsertList);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testInsertEmptyList);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSimpleAccessDeque);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testComplexTypeDeque);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testInsertDeque);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testInsertEmptyDeque);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testAffectedRows);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testInsertSingleBulk);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testInsertSingleBulkVec);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testLimit);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testLimitOnce);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testLimitPrepare);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testLimitZero);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testPrepare);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSetSimple);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSetComplex);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSetComplexUnique);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testMultiSetSimple);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testMultiSetComplex);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testMapComplex);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testMapComplexUnique);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testMultiMapComplex);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSelectIntoSingle);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSelectIntoSingleStep);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSelectIntoSingleFail);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testLowerLimitOk);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testLowerLimitFail);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testCombinedLimits);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testCombinedIllegalLimits);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testRange);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testIllegalRange);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSingleSelect);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testEmptyDB);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBLOB);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBLOBContainer);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testBLOBStmt);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testDateTime);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testFloat);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testDouble);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testTuple);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testTupleVector);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testInternalExtraction);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testFilter);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testInternalStorageType);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testNull);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testRowIterator);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testAsync);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testAny);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testDynamicAny);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testAnyDateTime);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testAnyBulk);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testAnyBoundType);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testAnyUntypedColumn);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSQLChannel);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSQLLogger);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testAutoCommit);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testTransactionIsolation);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSessionTransaction);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testTransaction);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testTransactor);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testReconnect);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testClosedSession);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testExceptionCopy);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testSQLGetInfo);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testNonASCIIText);
		CppUnit_addTest(pSuite, ODBCSQLiteTest, testRecordSetBeforeExecute);

		return pSuite;
	}

	return nullptr;
}
