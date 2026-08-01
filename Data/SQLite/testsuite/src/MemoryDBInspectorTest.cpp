//
// MemoryDBInspectorTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MemoryDBInspectorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Data/SQLite/MemoryDB.h"
#include "Poco/Data/SQLite/MemoryDBInspector.h"
#include "Poco/Data/Session.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/NumberFormatter.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Timespan.h"
#include <algorithm>
#include <atomic>
#include <iostream>
#include <locale>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>


using namespace Poco::Data::Keywords;
using Poco::Data::SQLite::MemoryDB;
using Poco::Data::SQLite::MemoryDBInspector;


namespace
{
	bool wellFormedJSON(const std::string& s)
		/// A deliberately small structural validator, so the testsuite does not have
		/// to link PocoJSON (Data/SQLite must build with ENABLE_JSON off). It checks
		/// what the emitter can plausibly get wrong: unbalanced containers, a stray
		/// separator, an unterminated or unescaped string.
	{
		std::vector<char> stack;
		bool inString = false;
		bool escaped = false;
		bool sawTop = false;
		for (std::size_t i = 0; i < s.size(); ++i)
		{
			const char c = s[i];
			if (inString)
			{
				if (escaped) { escaped = false; continue; }
				if (c == '\\') { escaped = true; continue; }
				if (c == '"') { inString = false; continue; }
				// Raw control characters must have been escaped.
				if (static_cast<unsigned char>(c) < 0x20) return false;
				continue;
			}
			switch (c)
			{
			case '"': inString = true; break;
			case '{': case '[':
				if (stack.empty())
				{
					if (sawTop) return false;   // more than one top-level value
					sawTop = true;
				}
				stack.push_back(c);
				break;
			case '}':
				if (stack.empty() || stack.back() != '{') return false;
				stack.pop_back();
				break;
			case ']':
				if (stack.empty() || stack.back() != '[') return false;
				stack.pop_back();
				break;
			case ',':
				// A separator directly before a closer is the classic emitter bug.
				if (i + 1 < s.size() && (s[i + 1] == '}' || s[i + 1] == ']')) return false;
				break;
			default:
				break;
			}
		}
		return !inString && stack.empty() && sawTop;
	}


	bool contains(const std::string& haystack, const std::string& needle)
	{
		return haystack.find(needle) != std::string::npos;
	}


	void seedTable(MemoryDB& db, int rows)
	{
		db << "CREATE TABLE IF NOT EXISTS t(id INTEGER PRIMARY KEY, "
			"name TEXT NOT NULL, score REAL DEFAULT 1.5, raw BLOB)", now;
		for (int i = 0; i < rows; ++i)
		{
			std::string name = "n" + Poco::NumberFormatter::format(i);
			db << "INSERT INTO t(name) VALUES(:n)", use(name), now;
		}
	}
}


MemoryDBInspectorTest::MemoryDBInspectorTest(const std::string& name): CppUnit::TestCase(name)
{
}


MemoryDBInspectorTest::~MemoryDBInspectorTest()
{
}


void MemoryDBInspectorTest::setUp()
{
	_dir = Poco::TemporaryFile::tempName();
}


void MemoryDBInspectorTest::tearDown()
{
	try { Poco::File(_dir).remove(true); }
	catch (...) {}
}


//
// status
//


void MemoryDBInspectorTest::testStatusFresh()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);

	const MemoryDBInspector::Status s = insp.status();
	assertTrue (s.directory == db.directory());
	assertTrue (s.readOnly);
	assertTrue (s.shardCount == 1);
	assertTrue (s.shards.size() == 1);
	assertTrue (!s.shards[0].sealed);
	assertTrue (!s.shards[0].attached);
	assertTrue (s.shards[0].alias.empty());
	// Nothing has been flushed, so the active shard has no file yet.
	assertTrue (s.shards[0].bytes == 0);
}


void MemoryDBInspectorTest::testStatusShardsAndBytes()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);

	seedTable(db, 20);
	db.flush();
	db.sealActive();
	db.flush();
	seedTable(db, 10);
	db.flush();

	const MemoryDBInspector::Status s = insp.status();
	assertTrue (s.shardCount >= 2);
	assertTrue (s.shards.size() == s.shardCount);
	assertTrue (s.shards[0].sealed);
	assertTrue (!s.shards.back().sealed);
	assertTrue (s.shards[0].sealedAt.epochMicroseconds() > 0);
	assertTrue (s.shards.back().sealedAt.epochMicroseconds() == 0);

	Poco::UInt64 sum = 0;
	for (const auto& sh: s.shards) sum += sh.bytes;
	assertTrue (s.shardBytes == sum);

	// The directory also holds manifest.db, so it is never smaller than the shards.
	assertTrue (s.diskBytes >= s.shardBytes);
	bool sawManifest = false;
	for (const auto& f: s.files) if (f.name == "manifest.db") sawManifest = true;
	assertTrue (sawManifest);
}


void MemoryDBInspectorTest::testStatusAttachedFlag()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);

	seedTable(db, 5);
	db.flush();
	db.sealActive();
	db.flush();
	seedTable(db, 5);
	db.flush();

	const std::vector<Poco::UInt32> archived = db.archivedShardIds();
	assertTrue (!archived.empty());
	const Poco::UInt32 id = archived[0];

	db.attachArchived(id);
	MemoryDBInspector::Status s = insp.status();
	bool found = false;
	for (const auto& sh: s.shards)
	{
		if (sh.id != id) continue;
		found = true;
		assertTrue (sh.attached);
		assertTrue (sh.alias == "arc_" + Poco::NumberFormatter::format(id));
	}
	assertTrue (found);

	db.detachArchived(id);
	s = insp.status();
	for (const auto& sh: s.shards)
	{
		if (sh.id != id) continue;
		assertTrue (!sh.attached);
		assertTrue (sh.alias.empty());
	}
}


//
// schema
//


void MemoryDBInspectorTest::testSchemaTablesColumnsIndexes()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);

	seedTable(db, 3);
	db << "CREATE INDEX t_name_idx ON t(name)", now;
	db << "CREATE VIEW v AS SELECT id, name FROM t", now;

	const MemoryDBInspector::Schema s = insp.schema();

	const MemoryDBInspector::DatabaseInfo* main = nullptr;
	for (const auto& d: s.databases) if (d.name == "main") main = &d;
	assertTrue (main != nullptr);

	const MemoryDBInspector::TableInfo* table = nullptr;
	const MemoryDBInspector::TableInfo* view = nullptr;
	for (const auto& o: main->objects)
	{
		if (o.name == "t") table = &o;
		if (o.name == "v") view = &o;
	}
	assertTrue (table != nullptr);
	assertTrue (view != nullptr);
	assertTrue (table->type == "table");
	assertTrue (view->type == "view");
	assertTrue (contains(table->sql, "CREATE TABLE"));

	assertTrue (table->columns.size() == 4);
	assertTrue (table->columns[0].name == "id");
	assertTrue (table->columns[0].primaryKey);
	assertTrue (table->columns[1].name == "name");
	assertTrue (table->columns[1].notNull);
	assertTrue (!table->columns[1].primaryKey);
	assertTrue (table->columns[2].name == "score");
	assertTrue (table->columns[2].hasDefault);
	assertTrue (contains(table->columns[2].defaultValue, "1.5"));
	assertTrue (!table->columns[3].hasDefault);

	// The index belongs to t and must not be attributed to the view.
	assertTrue (table->indexes.size() == 1);
	assertTrue (table->indexes[0].name == "t_name_idx");
	assertTrue (view->indexes.empty());
}


void MemoryDBInspectorTest::testSchemaRowCountsOmittedByDefault()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 7);

	MemoryDBInspector::Schema s = insp.schema();
	for (const auto& d: s.databases)
		for (const auto& o: d.objects)
			assertTrue (!o.rowsKnown);

	s = insp.schema(MemoryDBInspector::RowCounts::EXACT);
	bool checked = false;
	for (const auto& d: s.databases)
	{
		if (d.name != "main") continue;
		for (const auto& o: d.objects)
		{
			if (o.name != "t") continue;
			assertTrue (o.rowsKnown);
			assertTrue (o.rows == 7);
			checked = true;
		}
	}
	assertTrue (checked);
}


void MemoryDBInspectorTest::testSchemaSpansAttachedShards()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);

	seedTable(db, 5);
	db.flush();
	db.sealActive();
	db.flush();
	seedTable(db, 5);
	db.flush();

	const std::vector<Poco::UInt32> archived = db.archivedShardIds();
	assertTrue (!archived.empty());
	const std::string alias = db.attachArchived(archived[0]);

	const MemoryDBInspector::Schema s = insp.schema();
	const MemoryDBInspector::DatabaseInfo* arc = nullptr;
	for (const auto& d: s.databases) if (d.name == alias) arc = &d;
	assertTrue (arc != nullptr);
	assertTrue (!arc->file.empty());

	bool sawTable = false;
	for (const auto& o: arc->objects) if (o.name == "t") sawTable = true;
	assertTrue (sawTable);
}


void MemoryDBInspectorTest::testRowCountOnDemand()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 12);

	const MemoryDBInspector::RowCount rc = insp.rowCount("main", "t");
	assertTrue (rc.known);
	assertTrue (rc.rows == 12);
	assertTrue (rc.database == "main");
	assertTrue (rc.table == "t");
}


void MemoryDBInspectorTest::testRowCountUnknown()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 1);

	// A missing table must not throw; it comes back as "not known".
	const MemoryDBInspector::RowCount rc = insp.rowCount("main", "no_such_table");
	assertTrue (!rc.known);
}


//
// query
//


void MemoryDBInspectorTest::testQuerySelect()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 3);

	const MemoryDBInspector::QueryResult r = insp.query("SELECT id, name FROM t ORDER BY id");
	assertTrue (r.ok);
	assertTrue (r.error.empty());
	assertTrue (r.columns.size() == 2);
	assertTrue (r.columns[0].name == "id");
	assertTrue (r.columns[0].type == std::string("int64"));
	assertTrue (r.columns[1].name == "name");
	assertTrue (r.columns[1].type == std::string("string"));
	assertTrue (r.rows.size() == 3);
	assertTrue (r.rowCount == 3);
	assertTrue (!r.truncated);
	assertTrue (r.rows[0][1].text == "n0");
	assertTrue (!r.rows[0][1].isNull);
}


void MemoryDBInspectorTest::testQueryNullAndBlob()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 1);
	db << "UPDATE t SET raw = x'DEADBEEF' WHERE id = 1", now;

	const MemoryDBInspector::QueryResult r = insp.query("SELECT raw, score FROM t WHERE id = 1");
	assertTrue (r.ok);
	assertTrue (r.rows.size() == 1);
	// BLOBs never travel as bytes; they become a size marker.
	assertTrue (!r.rows[0][0].isNull);
	assertTrue (r.rows[0][0].text == "<blob, 4 bytes>");

	const MemoryDBInspector::QueryResult n = insp.query("SELECT NULL AS empty");
	assertTrue (n.ok);
	assertTrue (n.rows.size() == 1);
	assertTrue (n.rows[0][0].isNull);
	assertTrue (n.rows[0][0].text.empty());
}


void MemoryDBInspectorTest::testQueryTruncation()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 20);

	const MemoryDBInspector::QueryResult r = insp.query("SELECT id FROM t ORDER BY id", 5);
	assertTrue (r.ok);
	assertTrue (r.rows.size() == 5);
	assertTrue (r.rowCount == 5);
	assertTrue (r.truncated);
	assertTrue (r.maxRows == 5);

	// Exactly at the cap is not truncation.
	const MemoryDBInspector::QueryResult exact = insp.query("SELECT id FROM t ORDER BY id LIMIT 5", 5);
	assertTrue (exact.ok);
	assertTrue (exact.rows.size() == 5);
	assertTrue (!exact.truncated);
}


void MemoryDBInspectorTest::testQueryErrorReported()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 1);

	// A failing statement is a normal outcome, not an exception.
	const MemoryDBInspector::QueryResult r = insp.query("SELECT * FROM nope");
	assertTrue (!r.ok);
	assertTrue (!r.error.empty());
	assertTrue (r.rows.empty());
}


//
// policy
//


void MemoryDBInspectorTest::testReadOnlyRejectsWrites()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 4);

	const char* writes[] = {
		"INSERT INTO t(name) VALUES('x')",
		"UPDATE t SET name = 'x'",
		"DELETE FROM t",
		"DROP TABLE t",
		"CREATE TABLE u(a INTEGER)",
		"ALTER TABLE t ADD COLUMN extra INTEGER",
		"ATTACH DATABASE ':memory:' AS other",
		"VACUUM"
	};
	for (const char* sql: writes)
	{
		const MemoryDBInspector::QueryResult r = insp.query(sql);
		assertTrue (!r.ok);
		assertTrue (!r.error.empty());
	}

	// Nothing got through.
	int n = 0;
	db << "SELECT count(*) FROM t", into(n), now;
	assertTrue (n == 4);
}


void MemoryDBInspectorTest::testReadOnlyCommentSmuggling()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 3);

	assertTrue (!insp.query("-- harmless\nDELETE FROM t").ok);
	assertTrue (!insp.query("/* harmless */ DELETE FROM t").ok);

	int n = 0;
	db << "SELECT count(*) FROM t", into(n), now;
	assertTrue (n == 3);
}


void MemoryDBInspectorTest::testReadOnlyCTEWrite()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 3);

	// The leading keyword is WITH, which a first-keyword check would wave through.
	// Either the parser classifies it as a DELETE or it fails to parse; both are
	// refusals, and the rows must survive either way.
	const MemoryDBInspector::QueryResult r =
		insp.query("WITH x AS (SELECT 1) DELETE FROM t");
	assertTrue (!r.ok);

	int n = 0;
	db << "SELECT count(*) FROM t", into(n), now;
	assertTrue (n == 3);
}


void MemoryDBInspectorTest::testReadOnlyMultiStatement()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 3);

	// SQLiteStatementImpl keeps a leftover and recompiles, so a trailing statement
	// really would execute. The parser reports both statements, so this is refused.
	const MemoryDBInspector::QueryResult r = insp.query("SELECT 1; DROP TABLE t");
	assertTrue (!r.ok);

	int n = 0;
	db << "SELECT count(*) FROM t", into(n), now;
	assertTrue (n == 3);
}


void MemoryDBInspectorTest::testReadOnlyPragmaAndExplain()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 2);

	// The parser has no statement type for PRAGMA or EXPLAIN, so they fail to parse
	// and the fail-closed rule refuses them. That is deliberate: it is what makes
	// the PRAGMA query_only(1) function form unreachable without an allow-list.
	const char* refused[] = {
		"PRAGMA table_info(t)",
		"PRAGMA database_list",
		"PRAGMA query_only = 1",
		"PRAGMA query_only(1)",
		"PRAGMA integrity_check",
		"EXPLAIN SELECT * FROM t"
	};
	for (const char* sql: refused)
	{
		const MemoryDBInspector::QueryResult r = insp.query(sql);
		assertTrue (!r.ok);
		// The reason must be the parse failure, so the fail-closed path is pinned
		// rather than incidentally passing for some other reason.
		assertTrue (contains(r.error, "cannot parse"));
	}
}


void MemoryDBInspectorTest::testPolicyIndependentOfSqlParseFeature()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 2);

	// The session feature "sqlParse" defaults to false. Classification must not
	// depend on it (Statement::isSelect() would report "unspecified" here), and the
	// inspector must not switch it on behind the owner's back: it is session-wide
	// state that also decides when the owner's transactions begin.
	assertTrue (!db.session().getFeature("sqlParse"));

	assertTrue (insp.query("SELECT count(*) FROM t").ok);
	assertTrue (!insp.query("DELETE FROM t").ok);

	assertTrue (!db.session().getFeature("sqlParse"));
}


void MemoryDBInspectorTest::testQueryOnlyNeverSet()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 2);

	// The inspector shares the owner's connection, so it must never issue
	// PRAGMA query_only - that is connection-scoped and sticky and would break the
	// owner's writes. Run a batch of read-only work, then prove the owner can still
	// write.
	for (int i = 0; i < 5; ++i)
	{
		assertTrue (insp.query("SELECT * FROM t").ok);
		assertTrue (!insp.query("DELETE FROM t").ok);
	}

	db << "INSERT INTO t(name) VALUES('still writable')", now;
	int n = 0;
	db << "SELECT count(*) FROM t", into(n), now;
	assertTrue (n == 3);
}


void MemoryDBInspectorTest::testCheckSQLAllowedThrows()
{
	MemoryDB db(_dir);
	seedTable(db, 1);

	MemoryDBInspector ro(db);
	try
	{
		ro.checkSQLAllowed("DELETE FROM t");
		fail("checkSQLAllowed must throw for a write under READ_ONLY");
	}
	catch (const Poco::InvalidAccessException&)
	{
	}
	ro.checkSQLAllowed("SELECT * FROM t");   // must not throw

	MemoryDBInspector rw(db, MemoryDBInspector::SQLPolicy::ALLOW_WRITES);
	rw.checkSQLAllowed("DELETE FROM t");     // must not throw
}


void MemoryDBInspectorTest::testAllowWritesPolicy()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db, MemoryDBInspector::SQLPolicy::ALLOW_WRITES);
	seedTable(db, 2);

	const MemoryDBInspector::QueryResult r = insp.query("INSERT INTO t(name) VALUES('via inspector')");
	assertTrue (r.ok);
	assertTrue (r.affected == 1);
	assertTrue (r.columns.empty());

	int n = 0;
	db << "SELECT count(*) FROM t", into(n), now;
	assertTrue (n == 3);

	// Statements go through MemoryDB::operator <<, not session(), so MemoryDB's own
	// WITHOUT ROWID rejection still applies even under ALLOW_WRITES.
	const MemoryDBInspector::QueryResult wr =
		insp.query("CREATE TABLE wr(a TEXT PRIMARY KEY) WITHOUT ROWID");
	assertTrue (!wr.ok);
	assertTrue (contains(wr.error, "WITHOUT ROWID"));
}


//
// JSON
//


void MemoryDBInspectorTest::testJSONWellFormed()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);

	seedTable(db, 4);
	db << "CREATE INDEX t_name_idx ON t(name)", now;
	db.flush();
	db.sealActive();
	db.flush();
	seedTable(db, 2);
	db.flush();
	const std::vector<Poco::UInt32> archived = db.archivedShardIds();
	assertTrue (!archived.empty());
	db.attachArchived(archived[0]);

	std::ostringstream status;
	insp.writeStatusJSON(status);
	assertTrue (wellFormedJSON(status.str()));
	assertTrue (contains(status.str(), "\"shardCount\":"));
	assertTrue (contains(status.str(), "\"attached\":true"));
	assertTrue (contains(status.str(), "\"alias\":\"arc_" +
		Poco::NumberFormatter::format(archived[0]) + "\""));

	std::ostringstream schema;
	insp.writeSchemaJSON(schema);
	assertTrue (wellFormedJSON(schema.str()));
	// RowCounts::OMIT must render as JSON null, which the front end shows as "?".
	assertTrue (contains(schema.str(), "\"rows\":null"));

	std::ostringstream query;
	insp.writeQueryJSON(query, "SELECT id, name FROM t ORDER BY id");
	assertTrue (wellFormedJSON(query.str()));
	assertTrue (contains(query.str(), "\"ok\":true"));

	std::ostringstream rows;
	insp.writeRowCountJSON(rows, "main", "t");
	assertTrue (wellFormedJSON(rows.str()));

	std::ostringstream action;
	insp.writeActionJSON(action, "detachAll");
	assertTrue (wellFormedJSON(action.str()));

	// An empty result set and a failed query must still produce valid documents.
	std::ostringstream empty;
	insp.writeQueryJSON(empty, "SELECT id FROM t WHERE id < 0");
	assertTrue (wellFormedJSON(empty.str()));

	std::ostringstream bad;
	insp.writeQueryJSON(bad, "SELECT * FROM nope");
	assertTrue (wellFormedJSON(bad.str()));
	assertTrue (contains(bad.str(), "\"ok\":false"));
}


void MemoryDBInspectorTest::testJSONEscaping()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);

	db << "CREATE TABLE e(v TEXT)", now;
	std::string nasty = "a\"b\\c\nd\te\x01" "f\xC3\xA9";
	db << "INSERT INTO e(v) VALUES(:v)", use(nasty), now;

	// A quoted alias exercises escaping on the column-name side as well as the
	// value side. Plain double-quoted identifiers parse fine.
	std::ostringstream os;
	insp.writeQueryJSON(os, "SELECT v AS \"quoted alias\" FROM e");
	const std::string json = os.str();

	assertTrue (wellFormedJSON(json));
	assertTrue (contains(json, "\\\""));      // escaped quote
	assertTrue (contains(json, "\\\\"));      // escaped backslash
	assertTrue (contains(json, "\\n"));
	assertTrue (contains(json, "\\t"));
	assertTrue (contains(json, "\\u0001"));   // control character
	assertTrue (contains(json, "quoted alias"));
	// No raw control byte may survive into the document.
	for (const char c: json)
		assertTrue (static_cast<unsigned char>(c) >= 0x20 || c == '\n');

	// The documented edge of the fail-closed policy, pinned so it is a known
	// boundary rather than a surprise: ordinary quoted identifiers parse, but the
	// doubled-quote escape inside one does not, so it is refused as unparseable.
	// The escape hatch is SQLPolicy::ALLOW_WRITES, which does not parse at all.
	assertTrue (insp.query("SELECT v AS \"alias\" FROM e").ok);
	const MemoryDBInspector::QueryResult doubled = insp.query("SELECT v AS \"he\"\"llo\" FROM e");
	assertTrue (!doubled.ok);
	assertTrue (contains(doubled.error, "cannot parse"));

	MemoryDBInspector rw(db, MemoryDBInspector::SQLPolicy::ALLOW_WRITES);
	assertTrue (rw.query("SELECT v AS \"he\"\"llo\" FROM e").ok);
}


void MemoryDBInspectorTest::testJSONNumbers()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);
	seedTable(db, 1);

	// A stream imbued with a comma-decimal locale must not turn elapsedMs into
	// "0,42" - that would be silently invalid JSON. NumberFormatter is locale
	// independent; operator << would not be.
	std::ostringstream os;
	try { os.imbue(std::locale("de_DE.UTF-8")); }
	catch (const std::exception&)
	{
		try { os.imbue(std::locale("German_Germany.1252")); }
		catch (const std::exception&) { /* no such locale here; test the default */ }
	}

	MemoryDBInspector::QueryResult r = insp.query("SELECT 1");
	r.elapsedMs = 0.4225;
	// Values above 2^53 must survive exactly; they are why cells are strings.
	r.affected = 9007199254740993ULL;
	MemoryDBInspector::writeJSON(os, r);

	const std::string json = os.str();
	assertTrue (wellFormedJSON(json));
	assertTrue (!contains(json, "0,42"));
	assertTrue (contains(json, "0.42"));
	assertTrue (contains(json, "9007199254740993"));
}


//
// management
//


void MemoryDBInspectorTest::testActionVerbs()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db, MemoryDBInspector::SQLPolicy::ALLOW_WRITES);

	seedTable(db, 5);
	db.flush();
	assertTrue (insp.perform("seal").ok);
	seedTable(db, 5);
	db.flush();

	const std::vector<Poco::UInt32> archived = db.archivedShardIds();
	assertTrue (!archived.empty());
	const Poco::UInt32 id = archived[0];

	MemoryDBInspector::ActionResult a = insp.perform("attach", id);
	assertTrue (a.ok);
	assertTrue (a.detail == "arc_" + Poco::NumberFormatter::format(id));

	assertTrue (insp.perform("detach", id).ok);
	assertTrue (insp.perform("attachAll").ok);

	a = insp.perform("historyView", 0, "t");
	assertTrue (a.ok);
	assertTrue (a.detail == "t_history");

	assertTrue (insp.perform("detachAll").ok);
	assertTrue (insp.perform("flush").ok);

	a = insp.perform("no_such_verb");
	assertTrue (!a.ok);
	assertTrue (contains(a.error, "no_such_verb"));
}


void MemoryDBInspectorTest::testActionPolicyGate()
{
	MemoryDB db(_dir);

	seedTable(db, 5);
	db.flush();
	db.sealActive();
	db.flush();
	seedTable(db, 5);
	db.flush();

	const std::vector<Poco::UInt32> archived = db.archivedShardIds();
	assertTrue (!archived.empty());
	const Poco::UInt32 id = archived[0];
	const std::size_t before = db.shardCount();

	MemoryDBInspector ro(db);
	assertTrue (!ro.perform("drop", id).ok);
	assertTrue (!ro.perform("seal").ok);
	assertTrue (!ro.perform("enforceRetention").ok);
	// The refusals must be real: the store is untouched.
	assertTrue (db.shardCount() == before);

	// Non-destructive verbs stay available even under READ_ONLY.
	assertTrue (ro.perform("attachAll").ok);
	assertTrue (ro.perform("detachAll").ok);
	assertTrue (ro.perform("flush").ok);

	MemoryDBInspector rw(db, MemoryDBInspector::SQLPolicy::ALLOW_WRITES);
	assertTrue (rw.perform("drop", id).ok);
	const std::vector<Poco::UInt32> after = db.archivedShardIds();
	assertTrue (std::find(after.begin(), after.end(), id) == after.end());
}


void MemoryDBInspectorTest::testFlushReportsCompletion()
{
	MemoryDB db(_dir);
	MemoryDBInspector insp(db);

	seedTable(db, 5);
	assertTrue (db.dirty());
	assertTrue (insp.flush());
	assertTrue (!db.dirty());

	// The skip-on-busy branch is not deterministically reachable single-threaded,
	// so it is exercised by testConcurrentOwnerWritesWhileInspectorReads instead.
}


//
// concurrency
//


void MemoryDBInspectorTest::testConcurrentOwnerWritesWhileInspectorReads()
{
	// Models the shape a real owner has (HubMonitor's LocalHistoryStore): one
	// writer thread holding a lock around every session() access, inspector threads
	// taking the SAME lock for Group B calls and no lock for Group A, and an admin
	// thread running Group C verbs with no lock at all.
	//
	// What it is proving: no deadlock, no unexpected exception, every emitted
	// document well formed, and no lost writes.

	MemoryDB::Options o;
	// Tuning taken from MemoryDBTest::testConcurrentAccess: seal often enough to
	// exercise attach/detach, while staying under SQLITE_LIMIT_ATTACHED so the
	// auto-drop backstop does not dominate the run.
	o.shardMaxBytes    = 4 * 1024 * 1024;
	o.idleInterval     = Poco::Timespan(0, 50 * 1000);
	o.maxFlushInterval = Poco::Timespan(0, 200 * 1000);
	o.checkIntervalMs  = 25;

	std::atomic<long> writes{0};
	std::atomic<long> errors{0};
	std::atomic<long> badJSON{0};
	std::atomic<bool> stop{false};

	{
		MemoryDB db(_dir, o);
		std::mutex ownerLock;   // stands in for LocalHistoryStore::_dbMutex

		{
			std::lock_guard<std::mutex> g(ownerLock);
			db << "CREATE TABLE t(id INTEGER PRIMARY KEY, name TEXT, score REAL)", now;
		}

		MemoryDBInspector insp(db);

		auto note = [&errors](const char* who, const Poco::Exception& exc)
		{
			// Attach/detach on one thread can invalidate another thread's in-flight
			// statement. MemoryDBTest documents the exact surface these transients
			// take; they are expected and not counted.
			const std::string msg = exc.displayText();
			const bool transient =
				msg.find("no such table: arc_") != std::string::npos ||
				msg.find("unable to open database") != std::string::npos ||
				msg.find("not an error") != std::string::npos ||
				msg.find("another row available") != std::string::npos;
			if (!transient)
			{
				std::cerr << "[inspector-concur] " << who << ": " << msg << '\n';
				errors.fetch_add(1, std::memory_order_relaxed);
			}
		};

		std::thread writer([&]
		{
			long i = 0;
			while (!stop.load(std::memory_order_relaxed))
			{
				try
				{
					std::string name = "n" + Poco::NumberFormatter::format(i++);
					double score = 0.5 * static_cast<double>(i);
					std::lock_guard<std::mutex> g(ownerLock);
					db << "INSERT INTO t(name, score) VALUES(:n, :s)", use(name), use(score), now;
					writes.fetch_add(1, std::memory_order_relaxed);
				}
				catch (Poco::Exception& exc) { note("writer", exc); }
			}
		});

		std::vector<std::thread> readers;
		for (int r = 0; r < 2; ++r)
		{
			readers.emplace_back([&]
			{
				while (!stop.load(std::memory_order_relaxed))
				{
					try
					{
						// Group A: no lock.
						const MemoryDBInspector::Status st = insp.status();

						// Group B: gather under the owner's lock...
						MemoryDBInspector::Schema sc;
						MemoryDBInspector::QueryResult q;
						{
							std::lock_guard<std::mutex> g(ownerLock);
							sc = insp.schema();
							q = insp.query("SELECT id, name, score FROM t ORDER BY id DESC", 50);
						}

						// ...and serialize outside it. This is the whole point of the
						// struct-then-writeJSON split.
						std::ostringstream a, b, c;
						MemoryDBInspector::writeJSON(a, st);
						MemoryDBInspector::writeJSON(b, sc);
						MemoryDBInspector::writeJSON(c, q);
						if (!wellFormedJSON(a.str()) || !wellFormedJSON(b.str()) ||
							!wellFormedJSON(c.str()))
						{
							badJSON.fetch_add(1, std::memory_order_relaxed);
						}
					}
					catch (Poco::Exception& exc) { note("reader", exc); }
				}
			});
		}

		std::thread admin([&]
		{
			int i = 0;
			while (!stop.load(std::memory_order_relaxed))
			{
				try
				{
					// Group C: deliberately NOT under ownerLock. flush() holds
					// MemoryDB's _flushMutex across seconds of IO; running it under
					// the owner's write lock would stall the writer thread.
					switch (i++ % 3)
					{
					case 0: insp.perform("flush"); break;
					case 1: insp.perform("attachAll"); break;
					default: insp.perform("detachAll"); break;
					}
				}
				catch (Poco::Exception& exc) { note("admin", exc); }
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
		});

		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		stop.store(true, std::memory_order_relaxed);

		writer.join();
		for (auto& t: readers) t.join();
		admin.join();

		{
			std::lock_guard<std::mutex> g(ownerLock);
			int n = 0;
			db << "SELECT count(*) FROM t", into(n), now;
			assertTrue (n == writes.load());
		}
	}

	assertTrue (errors.load() == 0);
	assertTrue (badJSON.load() == 0);
	assertTrue (writes.load() > 0);

	// The data must also survive a clean reopen.
	{
		MemoryDB::Options reopen;
		reopen.loadArchivedShards = true;
		MemoryDB db(_dir, reopen);
		int n = 0;
		db << "SELECT count(*) FROM t", into(n), now;
		assertTrue (n == writes.load());
	}
}


CppUnit::Test* MemoryDBInspectorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MemoryDBInspectorTest");

	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testStatusFresh);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testStatusShardsAndBytes);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testStatusAttachedFlag);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testSchemaTablesColumnsIndexes);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testSchemaRowCountsOmittedByDefault);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testSchemaSpansAttachedShards);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testRowCountOnDemand);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testRowCountUnknown);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testQuerySelect);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testQueryNullAndBlob);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testQueryTruncation);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testQueryErrorReported);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testReadOnlyRejectsWrites);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testReadOnlyCommentSmuggling);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testReadOnlyCTEWrite);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testReadOnlyMultiStatement);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testReadOnlyPragmaAndExplain);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testPolicyIndependentOfSqlParseFeature);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testQueryOnlyNeverSet);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testCheckSQLAllowedThrows);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testAllowWritesPolicy);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testJSONWellFormed);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testJSONEscaping);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testJSONNumbers);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testActionVerbs);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testActionPolicyGate);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testFlushReportsCompletion);
	CppUnit_addTest(pSuite, MemoryDBInspectorTest, testConcurrentOwnerWritesWhileInspectorReads);

	return pSuite;
}
