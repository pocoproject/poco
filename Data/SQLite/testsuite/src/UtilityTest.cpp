//
// UtilityTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "UtilityTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Data/Utility.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/Time.h"
#include "Poco/DateTime.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Nullable.h"
#include <chrono>
#include <memory>
#include <optional>
#include <string>


using Poco::Data::Session;
using Poco::Data::Utility;
using Poco::Data::SQLite::Connector;
using Poco::InvalidArgumentException;


UtilityTest::UtilityTest(const std::string& name): CppUnit::TestCase(name)
{
	Connector::registerConnector();
}


UtilityTest::~UtilityTest()
{
	Connector::unregisterConnector();
}


void UtilityTest::setUp()
{
}


void UtilityTest::tearDown()
{
}


void UtilityTest::testRenderValue()
{
	assertTrue(Utility::renderValue(42) == "42");
	assertTrue(Utility::renderValue(-7) == "-7");
	assertTrue(Utility::renderValue(0u) == "0");
	assertTrue(Utility::renderValue(true) == "1");
	assertTrue(Utility::renderValue(false) == "0");
	assertTrue(Utility::renderValue(nullptr) == "NULL");
	assertTrue(Utility::renderValue(std::string("hello")) == "'hello'");
	assertTrue(Utility::renderValue("hello") == "'hello'");
	assertTrue(Utility::renderValue(std::string("O'Brien")) == "'O''Brien'");
	const std::string d = Utility::renderValue(3.5);
	assertTrue(d == "3.5" || d == "3.5000000000000000");

	// Poco::Nullable<T>: empty → NULL, populated → recurse on T.
	assertTrue(Utility::renderValue(Poco::Nullable<int>()) == "NULL");
	assertTrue(Utility::renderValue(Poco::Nullable<int>(42)) == "42");
	assertTrue(Utility::renderValue(Poco::Nullable<std::string>(std::string("hi"))) == "'hi'");

	// std::optional<T>: empty → NULL, populated → recurse on T.
	assertTrue(Utility::renderValue(std::optional<int>()) == "NULL");
	assertTrue(Utility::renderValue(std::optional<int>(7)) == "7");
	assertTrue(Utility::renderValue(std::optional<std::string>(std::string("ok"))) == "'ok'");

	// Date / Time — SQL literal form.
	assertTrue(Utility::renderValue(Poco::Data::Date(2026, 5, 27)) == "'2026-05-27'");
	assertTrue(Utility::renderValue(Poco::Data::Time(14, 30, 45)) == "'14:30:45'");

	// DateTime / LocalDateTime — sortable format.
	assertTrue(Utility::renderValue(Poco::DateTime(2026, 5, 27, 14, 30, 45))
		== "'2026-05-27 14:30:45'");
	assertTrue(Utility::renderValue(Poco::LocalDateTime(2026, 5, 27, 14, 30, 45))
		== "'2026-05-27 14:30:45'");

	// Poco::Dynamic::Var — inspect and dispatch.
	assertTrue(Utility::renderValue(Poco::Dynamic::Var()) == "NULL");
	assertTrue(Utility::renderValue(Poco::Dynamic::Var(true)) == "1");
	assertTrue(Utility::renderValue(Poco::Dynamic::Var(false)) == "0");
	assertTrue(Utility::renderValue(Poco::Dynamic::Var(42)) == "42");
	assertTrue(Utility::renderValue(Poco::Dynamic::Var(3.5)) == "3.5");
	assertTrue(Utility::renderValue(Poco::Dynamic::Var(std::string("hi"))) == "'hi'");
	assertTrue(Utility::renderValue(Poco::Dynamic::Var(Poco::DateTime(2026, 5, 27, 14, 30, 45)))
		== "'2026-05-27 14:30:45'");
	assertTrue(Utility::renderValue(Poco::Dynamic::Var(Poco::Data::Date(2026, 5, 27)))
		== "'2026-05-27'");

	// BLOB — uppercase hex literal, X'...' form.
	{
		const unsigned char bytes[] = { 0xDE, 0xAD, 0xBE, 0xEF };
		Poco::Data::BLOB blob(bytes, sizeof(bytes));
		assertTrue(Utility::renderValue(blob) == "X'DEADBEEF'");
	}
	assertTrue(Utility::renderValue(Poco::Data::BLOB()) == "X''");

	// CLOB — single-quoted text, internal ' doubled.
	{
		const std::string s = "O'Brien";
		Poco::Data::CLOB clob(s.data(), s.size());
		assertTrue(Utility::renderValue(clob) == "'O''Brien'");
	}

	// Unknown type → "?" fallback (no operator<<, no specialised handling).
	struct Unknown {};
	assertTrue(Utility::renderValue(Unknown{}) == "?");
}


void UtilityTest::testBoundSQLQuestion()
{
	const std::string s = Utility::boundSQL(
		"INSERT INTO orders(id, customer, total) VALUES(?, ?, ?)",
		42, std::string("John"), 99.5);
	assertTrue(s == "INSERT INTO orders(id, customer, total) VALUES(42, 'John', 99.5)");
}


void UtilityTest::testBoundSQLQuoteDoubling()
{
	const std::string s = Utility::boundSQL("SELECT ? AS name", std::string("O'Brien"));
	assertTrue(s == "SELECT 'O''Brien' AS name");
}


void UtilityTest::testBoundSQLPlaceholderInLiteral()
{
	const std::string s = Utility::boundSQL(
		"SELECT * FROM t WHERE name = 'a?b' AND id = ?", 7);
	assertTrue(s == "SELECT * FROM t WHERE name = 'a?b' AND id = 7");
}


void UtilityTest::testBoundSQLQuestionTooFew()
{
	try
	{
		Utility::boundSQL("INSERT INTO t VALUES(?, ?, ?)", 1, 2);
		fail("must fail with too-few-args");
	}
	catch (const InvalidArgumentException&)
	{
	}
}


void UtilityTest::testBoundSQLQuestionTooMany()
{
	try
	{
		Utility::boundSQL("INSERT INTO t VALUES(?)", 1, 2);
		fail("must fail with too-many-args");
	}
	catch (const InvalidArgumentException&)
	{
	}
}


void UtilityTest::testBoundSQLDollar()
{
	const std::string s = Utility::boundSQL(
		"INSERT INTO t VALUES($1, $2)", 42, std::string("John"));
	assertTrue(s == "INSERT INTO t VALUES(42, 'John')");
}


void UtilityTest::testBoundSQLDollarRepetition()
{
	const std::string s = Utility::boundSQL("INSERT INTO t VALUES($1, $1)", 42);
	assertTrue(s == "INSERT INTO t VALUES(42, 42)");
}


void UtilityTest::testBoundSQLDollarGap()
{
	try
	{
		Utility::boundSQL("INSERT INTO t VALUES($1, $3)", 1, 2);
		fail("must fail with $N arity gap");
	}
	catch (const InvalidArgumentException&)
	{
	}
}


void UtilityTest::testBoundSQLDollarOutOfRange()
{
	try
	{
		Utility::boundSQL("INSERT INTO t VALUES($5)", 1, 2);
		fail("must fail with $N out of range");
	}
	catch (const InvalidArgumentException&)
	{
	}
}


void UtilityTest::testBoundSQLDollarZero()
{
	try
	{
		Utility::boundSQL("INSERT INTO t VALUES($0)", 1);
		fail("must fail with $0");
	}
	catch (const InvalidArgumentException&)
	{
	}
}


void UtilityTest::testBoundSQLMixedStyles()
{
	try
	{
		Utility::boundSQL("INSERT INTO t VALUES(?, $1)", 1, 2);
		fail("must fail with mixed styles");
	}
	catch (const InvalidArgumentException&)
	{
	}
}


void UtilityTest::testExecuteSQLSuccess()
{
	Session session(Connector::KEY, ":memory:");
	session << "CREATE TABLE u_test(id INTEGER PRIMARY KEY, customer TEXT, total REAL)",
		Poco::Data::Keywords::now;

	Utility::ExecResult captured;
	auto cb = [&captured](const Utility::ExecResult& r) { captured = r; };

	const auto start = std::chrono::duration_cast<std::chrono::microseconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();

	const auto rc = Utility::executeSQL(session,
		"INSERT INTO u_test(id, customer, total) VALUES(?, ?, ?)",
		std::make_unique<std::string>("hash-abc"),
		cb,
		1, std::string("John"), 99.5);

	assertTrue(rc == 1);
	assertTrue(captured.ok);
	assertTrue(captured.sql == "INSERT INTO u_test(id, customer, total) VALUES(1, 'John', 99.5)");
	assertTrue(captured.rowsAffected == 1);
	assertTrue(captured.context == "hash-abc");
	assertTrue(captured.error.empty());
	assertTrue(captured.tsMicros >= start);
}


void UtilityTest::testExecuteSQLConstraintFailure()
{
	Session session(Connector::KEY, ":memory:");
	session << "CREATE TABLE u_pk(id INTEGER PRIMARY KEY)", Poco::Data::Keywords::now;
	session << "INSERT INTO u_pk(id) VALUES(1)", Poco::Data::Keywords::now;

	Utility::ExecResult captured;
	auto cb = [&captured](const Utility::ExecResult& r) { captured = r; };

	const auto rc = Utility::executeSQL(session,
		"INSERT INTO u_pk(id) VALUES(?)",
		nullptr,
		cb,
		1);

	assertTrue(rc == 0);
	assertTrue(!captured.ok);
	assertTrue(captured.rowsAffected == 0);
	assertTrue(!captured.error.empty());
	assertTrue(captured.context.empty());
}


void UtilityTest::testExecuteSQLMalformed()
{
	Session session(Connector::KEY, ":memory:");

	Utility::ExecResult captured;
	auto cb = [&captured](const Utility::ExecResult& r) { captured = r; };

	const auto rc = Utility::executeSQL(session,
		"INSERT INVALID NONSENSE ?",
		nullptr,
		cb,
		7);

	assertTrue(rc == 0);
	assertTrue(!captured.ok);
	// Parser-based boundSQL refuses to render unparseable SQL; executeSQL falls
	// back to the raw template string with placeholders left in place.
	assertTrue(captured.sql == "INSERT INVALID NONSENSE ?");
	assertTrue(!captured.error.empty());
}


void UtilityTest::testExecuteSQLNullContext()
{
	Session session(Connector::KEY, ":memory:");
	session << "CREATE TABLE u_nc(id INTEGER)", Poco::Data::Keywords::now;

	Utility::ExecResult captured;
	auto cb = [&captured](const Utility::ExecResult& r) { captured = r; };

	Utility::executeSQL(session,
		"INSERT INTO u_nc(id) VALUES(?)",
		nullptr,
		cb,
		1);

	assertTrue(captured.context.empty());
}


void UtilityTest::testExecuteSQLNullCallback()
{
	Session session(Connector::KEY, ":memory:");
	session << "CREATE TABLE u_nck(id INTEGER)", Poco::Data::Keywords::now;

	const auto rc = Utility::executeSQL(session,
		"INSERT INTO u_nck(id) VALUES(?)",
		nullptr,
		Utility::Callback(),
		42);

	assertTrue(rc == 1);
}


void UtilityTest::testExecuteSQLNoBindings()
{
	Session session(Connector::KEY, ":memory:");

	Utility::ExecResult captured;
	auto cb = [&captured](const Utility::ExecResult& r) { captured = r; };

	Utility::executeSQL(session,
		"CREATE TABLE u_nob(id INTEGER)",
		nullptr,
		cb);

	assertTrue(captured.ok);
	assertTrue(captured.sql == "CREATE TABLE u_nob(id INTEGER)");
}


void UtilityTest::testToJSONSuccess()
{
	Utility::ExecResult r;
	r.sql             = "INSERT INTO t VALUES(1, 'a')";
	r.ok              = true;
	r.rowsAffected    = 3;
	r.tsMicros        = 1716750000000000LL;
	r.executionMicros = 125;
	r.context         = "hash-abc";

	const std::string expected =
		"{\"sql\":\"INSERT INTO t VALUES(1, 'a')\""
		",\"ok\":true"
		",\"tsMicros\":1716750000000000"
		",\"executionMicros\":125"
		",\"rowsAffected\":3"
		",\"context\":\"hash-abc\"}";
	assertTrue(Utility::toJSON(r) == expected);
}


void UtilityTest::testToJSONFailure()
{
	Utility::ExecResult r;
	r.sql             = "INSERT INTO t VALUES(1)";
	r.ok              = false;
	r.error           = "UNIQUE constraint failed";
	r.tsMicros        = 1716750000000000LL;
	r.executionMicros = 7;
	r.context         = "hash-abc";

	const std::string expected =
		"{\"sql\":\"INSERT INTO t VALUES(1)\""
		",\"ok\":false"
		",\"tsMicros\":1716750000000000"
		",\"executionMicros\":7"
		",\"error\":\"UNIQUE constraint failed\""
		",\"context\":\"hash-abc\"}";
	assertTrue(Utility::toJSON(r) == expected);
}


void UtilityTest::testToJSONNoContext()
{
	Utility::ExecResult r;
	r.sql             = "SELECT 1";
	r.ok              = true;
	r.rowsAffected    = 0;
	r.tsMicros        = 42;
	r.executionMicros = 0;

	const std::string expected =
		"{\"sql\":\"SELECT 1\""
		",\"ok\":true"
		",\"tsMicros\":42"
		",\"executionMicros\":0"
		",\"rowsAffected\":0}";
	assertTrue(Utility::toJSON(r) == expected);
}


void UtilityTest::testToJSONEscapes()
{
	Utility::ExecResult r;
	r.sql             = "SELECT \"x\"\nFROM\tt\\u";
	r.ok              = false;
	// Concatenated literal so \x01 isn't greedily extended by 'end'.
	r.error           = "oops\x01" "end";
	r.tsMicros        = 0;
	r.executionMicros = 0;
	r.context         = "a\"b\\c";

	const std::string expected =
		"{\"sql\":\"SELECT \\\"x\\\"\\nFROM\\tt\\\\u\""
		",\"ok\":false"
		",\"tsMicros\":0"
		",\"executionMicros\":0"
		",\"error\":\"oops\\u0001end\""
		",\"context\":\"a\\\"b\\\\c\"}";
	assertTrue(Utility::toJSON(r) == expected);
}


CppUnit::Test* UtilityTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("UtilityTest");

	CppUnit_addTest(pSuite, UtilityTest, testRenderValue);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLQuestion);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLQuoteDoubling);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLPlaceholderInLiteral);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLQuestionTooFew);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLQuestionTooMany);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLDollar);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLDollarRepetition);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLDollarGap);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLDollarOutOfRange);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLDollarZero);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLMixedStyles);
	CppUnit_addTest(pSuite, UtilityTest, testExecuteSQLSuccess);
	CppUnit_addTest(pSuite, UtilityTest, testExecuteSQLConstraintFailure);
	CppUnit_addTest(pSuite, UtilityTest, testExecuteSQLMalformed);
	CppUnit_addTest(pSuite, UtilityTest, testExecuteSQLNullContext);
	CppUnit_addTest(pSuite, UtilityTest, testExecuteSQLNullCallback);
	CppUnit_addTest(pSuite, UtilityTest, testExecuteSQLNoBindings);
	CppUnit_addTest(pSuite, UtilityTest, testToJSONSuccess);
	CppUnit_addTest(pSuite, UtilityTest, testToJSONFailure);
	CppUnit_addTest(pSuite, UtilityTest, testToJSONNoContext);
	CppUnit_addTest(pSuite, UtilityTest, testToJSONEscapes);

	return pSuite;
}
