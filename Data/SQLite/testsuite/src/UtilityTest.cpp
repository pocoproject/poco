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

	// A null character pointer (distinct from the nullptr literal: the type
	// is const char*, not nullptr_t) is convertible to std::string_view, and
	// the string_view(const char*) constructor is UB on null. renderValue
	// must catch the null pointer before that conversion happens and map to
	// SQL NULL, matching what the bind layer would emit.
	{
		const char* p = nullptr;
		assertTrue(Utility::renderValue(p) == "NULL");
		char* q = nullptr;
		assertTrue(Utility::renderValue(q) == "NULL");
	}
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


void UtilityTest::testBoundSQLNamed()
{
	// Pins the current pass-through behaviour of the kExprParameterNamed
	// branch in Utility.cpp:131-134: paramIndex is set to npos and srcLen
	// to 1 + strlen(name), so renderRow at Utility.cpp:162-163 copies the
	// original :name span verbatim and the Style stays None - the arity
	// checks at lines 144/146 never fire and the supplied args are
	// silently ignored.
	//
	// Net effect: :name placeholders are a no-op pass-through; the args
	// are dropped and the returned SQL equals the input SQL. If Utility
	// is ever taught to actually substitute named placeholders, this
	// test must be rewritten.
	const std::string sql = "SELECT * FROM t WHERE u = :user AND a = :age";
	const std::string s = Utility::boundSQL(sql, std::string("John"), 42);
	assertTrue (s == sql);
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
		"hash-abc",
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
		"",
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
		"",
		cb,
		7);

	assertTrue(rc == 0);
	assertTrue(!captured.ok);
#ifndef POCO_DATA_NO_SQL_PARSER
	// Parser-based boundSQL refuses to render unparseable SQL; executeSQL falls
	// back to the raw template string with placeholders left in place.
	assertTrue(captured.sql == "INSERT INVALID NONSENSE ?");
#else
	// Hand-rolled scanner does not validate SQL; it substitutes the '?' even
	// though the rest of the statement is nonsense.
	assertTrue(captured.sql == "INSERT INVALID NONSENSE 7");
#endif
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
		"",
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
		"",
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
		"",
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


namespace
{
	// Custom value type for testBoundSQLCustomTypeHandler.
	struct UPair
	{
		int id;
		std::string name;
	};
}


namespace Poco {
namespace Data {

	template <>
	class TypeHandler<UPair>: public AbstractTypeHandler
	{
	public:
		static std::size_t size() { return 2u; }

		static void bind(std::size_t pos, const UPair& v, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
		{
			pBinder->bind(pos,     Poco::Int32(v.id), dir);
			pBinder->bind(pos + 1, v.name,           dir);
		}

		static void extract(std::size_t /*pos*/, UPair& /*v*/, const UPair& /*defVal*/, AbstractExtractor::Ptr /*pExt*/) {}
		static void prepare(std::size_t /*pos*/, const UPair& /*v*/, AbstractPreparator::Ptr /*pPreparator*/) {}
	};

} } // namespace Poco::Data


void UtilityTest::testBoundSQLVectorScalar()
{
	const std::vector<int> rows{1, 2, 3};
	const std::string s = Utility::boundSQL("INSERT INTO t VALUES(?)", rows);
	assertTrue (s == "INSERT INTO t VALUES(1)\n-- (+2 more rows)");
}


void UtilityTest::testBoundSQLVectorMaxRows3()
{
	const std::vector<int> rows{1, 2, 3};
	const std::string s = Utility::boundSQLBulk("INSERT INTO t VALUES(?)", 3, rows);
	assertTrue (s == "INSERT INTO t VALUES(1);\nINSERT INTO t VALUES(2);\nINSERT INTO t VALUES(3)");
}


void UtilityTest::testBoundSQLCustomTypeHandler()
{
	const UPair p{42, "Alice"};
	const std::string s = Utility::boundSQL("INSERT INTO t VALUES(?, ?)", p);
	assertTrue (s == "INSERT INTO t VALUES(42, 'Alice')");
}


void UtilityTest::testBoundSQLFromStatementScalar()
{
	Session session(Connector::KEY, ":memory:");
	int id = 42;
	std::string customer = "Alice";
	double total = 99.5;
	Poco::Data::Statement stmt(session);
	stmt << "INSERT INTO t VALUES(?, ?, ?)",
		Poco::Data::Keywords::use(id),
		Poco::Data::Keywords::use(customer),
		Poco::Data::Keywords::use(total);

	const std::string rendered = Utility::boundSQL(stmt);
	assertTrue (rendered == "INSERT INTO t VALUES(42, 'Alice', 99.5)");
}


void UtilityTest::testBoundSQLFromStatementVector()
{
	Session session(Connector::KEY, ":memory:");
	std::vector<int> rows{1, 2, 3};
	Poco::Data::Statement stmt(session);
	stmt << "INSERT INTO t VALUES(?)",
		Poco::Data::Keywords::use(rows);

	const std::string rendered = Utility::boundSQL(stmt);
	assertTrue (rendered == "INSERT INTO t VALUES(1)\n-- (+2 more rows)");
}


void UtilityTest::testBoundSQLFromStatementVectorBulk()
{
	Session session(Connector::KEY, ":memory:");
	std::vector<int> rows{1, 2, 3};
	Poco::Data::Statement stmt(session);
	stmt << "INSERT INTO t VALUES(?)",
		Poco::Data::Keywords::use(rows);

	const std::string rendered = Utility::boundSQLBulk(stmt, 3);
	assertTrue (rendered == "INSERT INTO t VALUES(1);\nINSERT INTO t VALUES(2);\nINSERT INTO t VALUES(3)");
}


void UtilityTest::testBoundSQLFromStatementCustomTypeHandler()
{
	Session session(Connector::KEY, ":memory:");
	UPair p{42, "Alice"};
	Poco::Data::Statement stmt(session);
	stmt << "INSERT INTO t VALUES(?, ?)",
		Poco::Data::Keywords::use(p);

	const std::string rendered = Utility::boundSQL(stmt);
	assertTrue (rendered == "INSERT INTO t VALUES(42, 'Alice')");
}


void UtilityTest::testBoundSQLFromStatementPreservesExecute()
{
	// Render via the helper, then execute, then render again. Both renderings
	// must agree and execute must succeed (the helper must reset iterators).
	Session session(Connector::KEY, ":memory:");
	session << "CREATE TABLE u_psv(id INTEGER, customer TEXT)",
		Poco::Data::Keywords::now;

	int id = 7;
	std::string customer = "Bob";
	Poco::Data::Statement stmt(session);
	stmt << "INSERT INTO u_psv(id, customer) VALUES(?, ?)",
		Poco::Data::Keywords::use(id),
		Poco::Data::Keywords::use(customer);

	const std::string before = Utility::boundSQL(stmt);
	assertTrue (before == "INSERT INTO u_psv(id, customer) VALUES(7, 'Bob')");

	const std::size_t affected = stmt.execute();
	assertTrue (affected == 1);

	const std::string after = Utility::boundSQL(stmt);
	assertTrue (after == before);
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
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLNamed);
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
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLVectorScalar);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLVectorMaxRows3);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLCustomTypeHandler);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLFromStatementScalar);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLFromStatementVector);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLFromStatementVectorBulk);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLFromStatementCustomTypeHandler);
	CppUnit_addTest(pSuite, UtilityTest, testBoundSQLFromStatementPreservesExecute);

	return pSuite;
}
