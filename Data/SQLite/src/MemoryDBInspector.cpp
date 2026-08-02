//
// MemoryDBInspector.cpp
//
// Library: Data/SQLite
// Package: SQLite
// Module:  MemoryDBInspector
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/SQLite/MemoryDBInspector.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/Data/Range.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/JSONString.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Path.h"
#include "Poco/Stopwatch.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <ostream>

#include "SQLParser.h"
#include "sql/SQLStatement.h"


using namespace Poco::Data::Keywords;


namespace Poco::Data::SQLite {


namespace {


class JSONWriter
	/// Minimal streaming JSON emitter: just enough for the documents this module
	/// produces, so PocoDataSQLite does not have to link PocoJSON. String escaping
	/// is delegated to Poco::toJSON (Foundation), the same call
	/// Data/src/JSONRowFormatter.cpp uses.
	///
	/// Separator bookkeeping is a stack of "this container is still empty" flags,
	/// so nesting arrays inside objects inside arrays needs no discipline at the
	/// call site. Keys are always ASCII literals from this file - no
	/// database-derived string is ever used as a key - so key() emits them raw.
{
public:
	explicit JSONWriter(std::ostream& os): _os(os)
	{
	}

	JSONWriter& beginObject() { sep(); _os << '{'; _empty.push_back(true); return *this; }
	JSONWriter& endObject()   { _os << '}'; if (!_empty.empty()) _empty.pop_back(); return *this; }
	JSONWriter& beginArray()  { sep(); _os << '['; _empty.push_back(true); return *this; }
	JSONWriter& endArray()    { _os << ']'; if (!_empty.empty()) _empty.pop_back(); return *this; }

	JSONWriter& key(const char* k)
	{
		sep();
		_os << '"' << k << "\":";
		_pendingKey = true;
		return *this;
	}

	JSONWriter& value(const std::string& v)
	{
		sep();
		Poco::toJSON(v, _os, Poco::JSON_WRAP_STRINGS);
		return *this;
	}

	JSONWriter& value(const char* v)
		// Load-bearing: without it a string literal binds to value(bool).
	{
		return value(std::string(v));
	}

	JSONWriter& value(bool v) { sep(); _os << (v ? "true" : "false"); return *this; }

	// Numbers go through NumberFormatter, never operator <<: doubleToStr hard-codes
	// '.' and ignores the locale, while a stream imbued with a comma-decimal locale
	// would silently emit invalid JSON. The same reasoning covers integers, whose
	// rendering an imbued std::hex/showbase stream would otherwise change.
	JSONWriter& value(Poco::Int64 v)  { sep(); _os << Poco::NumberFormatter::format(v); return *this; }
	JSONWriter& value(Poco::UInt64 v) { sep(); _os << Poco::NumberFormatter::format(v); return *this; }

	JSONWriter& value(double v)
	{
		sep();
		// JSON has no NaN or Infinity, and NumberFormatter would emit "nan"/"inf",
		// which no parser accepts. null is the only correct rendering.
		if (!std::isfinite(v)) _os << "null";
		else _os << Poco::NumberFormatter::format(v);
		return *this;
	}

	JSONWriter& null() { sep(); _os << "null"; return *this; }

	// Explicit overloads rather than a template: an int argument would be ambiguous
	// between the Int64/UInt64/double/bool ones, and the error message is unreadable.
	JSONWriter& pair(const char* k, const std::string& v) { return key(k).value(v); }
	JSONWriter& pair(const char* k, const char* v)        { return key(k).value(v); }
	JSONWriter& pair(const char* k, bool v)               { return key(k).value(v); }
	JSONWriter& pair(const char* k, Poco::Int64 v)        { return key(k).value(v); }
	JSONWriter& pair(const char* k, Poco::UInt64 v)       { return key(k).value(v); }
	JSONWriter& pair(const char* k, double v)             { return key(k).value(v); }
	JSONWriter& pairNull(const char* k)                   { return key(k).null(); }

private:
	void sep()
	{
		if (_pendingKey) { _pendingKey = false; return; }
		if (_empty.empty()) return;
		if (_empty.back()) _empty.back() = false;
		else _os << ',';
	}

	std::ostream&     _os;
	std::vector<bool> _empty;
	bool              _pendingKey = false;
};


std::string quoteIdent(const std::string& name)
	/// SQLite identifier quoting: wrap in double quotes, double any embedded one.
	/// Schema and table names reach the schema queries from sqlite_master and
	/// PRAGMA database_list rather than from a request, but they still have to
	/// survive being spelled back into SQL.
{
	std::string out("\"");
	for (const char c: name)
	{
		if (c == '"') out += '"';
		out += c;
	}
	out += '"';
	return out;
}


const char* columnTypeName(MetaColumn::ColumnDataType type)
{
	switch (type)
	{
	case MetaColumn::FDT_BOOL:      return "bool";
	case MetaColumn::FDT_INT8:      return "int8";
	case MetaColumn::FDT_UINT8:     return "uint8";
	case MetaColumn::FDT_INT16:     return "int16";
	case MetaColumn::FDT_UINT16:    return "uint16";
	case MetaColumn::FDT_INT32:     return "int32";
	case MetaColumn::FDT_UINT32:    return "uint32";
	case MetaColumn::FDT_INT64:     return "int64";
	case MetaColumn::FDT_UINT64:    return "uint64";
	case MetaColumn::FDT_FLOAT:     return "float";
	case MetaColumn::FDT_DOUBLE:    return "double";
	case MetaColumn::FDT_STRING:    return "string";
	case MetaColumn::FDT_WSTRING:   return "wstring";
	case MetaColumn::FDT_BLOB:      return "blob";
	case MetaColumn::FDT_CLOB:      return "clob";
	case MetaColumn::FDT_DATE:      return "date";
	case MetaColumn::FDT_TIME:      return "time";
	case MetaColumn::FDT_TIMESTAMP: return "timestamp";
	case MetaColumn::FDT_UUID:      return "uuid";
	case MetaColumn::FDT_JSON:      return "json";
	default:                        return "unknown";
	}
}


void writeTimestamp(JSONWriter& w, const char* key, const Poco::Timestamp& ts)
	/// Timestamps travel as microseconds since the epoch, so a front end can format
	/// them in local time without a parse step. Epoch 0 means "never" and stays 0.
{
	w.pair(key, static_cast<Poco::Int64>(ts.epochMicroseconds()));
}


} // anonymous namespace


const std::size_t MemoryDBInspector::DEFAULT_MAX_ROWS;


MemoryDBInspector::MemoryDBInspector(MemoryDB& db, SQLPolicy policy):
	_db(db),
	_policy(policy),
	_maxRows(DEFAULT_MAX_ROWS)
{
}


MemoryDBInspector::~MemoryDBInspector()
{
}


//
// Group A
//


std::vector<MemoryDBInspector::FileInfo> MemoryDBInspector::listFiles(Poco::UInt64& totalBytes) const
{
	std::vector<FileInfo> files;
	totalBytes = 0;
	try
	{
		for (Poco::DirectoryIterator it(_db.directory()), end; it != end; ++it)
		{
			if (!it->isFile()) continue;
			FileInfo f;
			f.name     = Poco::Path(it->path()).getFileName();
			f.bytes    = it->getSize();
			f.modified = it->getLastModified();
			totalBytes += f.bytes;
			files.push_back(f);
		}
	}
	catch (const Poco::Exception&)
	{
		// A shard dropped by a concurrent retention sweep vanishes mid-walk;
		// report what was listed rather than failing the whole call.
	}
	std::sort(files.begin(), files.end(),
		[](const FileInfo& a, const FileInfo& b) { return a.name < b.name; });
	return files;
}


MemoryDBInspector::Status MemoryDBInspector::status()
{
	Status s;
	s.directory  = _db.directory();
	s.readOnly   = (_policy == SQLPolicy::READ_ONLY);
	s.dirty      = _db.dirty();
	s.lastFlush  = _db.lastFlushTime();
	s.shardCount = _db.shardCount();

	// Both of these are MemoryDB's own thread-safe accessors, so this whole method
	// runs no SQL and needs no external lock.
	const std::vector<Poco::UInt32> attached = _db.attachedShardIds();

	for (const auto& d: _db.shards())
	{
		ShardStatus st;
		st.id        = d.id;
		st.sealed    = d.sealed;
		st.createdAt = d.createdAt;
		st.sealedAt  = d.sealedAt;
		st.bytes     = d.bytes;
		st.filename  = d.filename;
		st.attached  = std::find(attached.begin(), attached.end(), d.id) != attached.end();
		if (st.attached) st.alias = "arc_" + Poco::NumberFormatter::format(d.id);
		s.shardBytes += d.bytes;
		s.shards.push_back(st);
	}

	s.files = listFiles(s.diskBytes);
	return s;
}


//
// Group B
//


std::vector<std::pair<std::string, std::string> > MemoryDBInspector::databaseNames()
	/// (schema alias, backing file) for every database on the connection. An
	/// in-memory schema reports an empty file.
{
	std::vector<std::pair<std::string, std::string> > out;
	try
	{
		Statement stmt = (_db.session() << "PRAGMA database_list");
		stmt.execute();
		RecordSet rs(stmt);
		for (bool more = rs.moveFirst(); more; more = rs.moveNext())
		{
			Poco::Dynamic::Var name = rs[1];
			Poco::Dynamic::Var file = rs[2];
			out.emplace_back(name.isEmpty() ? std::string() : name.convert<std::string>(),
				file.isEmpty() ? std::string() : file.convert<std::string>());
		}
	}
	catch (const Poco::Exception&)
	{
		out.emplace_back("main", std::string());
	}
	return out;
}


std::vector<MemoryDBInspector::ColumnInfo> MemoryDBInspector::tableColumns(
	const std::string& database, const std::string& table)
{
	std::vector<ColumnInfo> columns;
	try
	{
		Statement stmt = (_db.session() <<
			("PRAGMA " + quoteIdent(database) + ".table_info(" + quoteIdent(table) + ")"));
		stmt.execute();
		RecordSet rs(stmt);
		for (bool more = rs.moveFirst(); more; more = rs.moveNext())
		{
			ColumnInfo c;
			c.name       = rs[1].convert<std::string>();
			c.type       = rs[2].convert<std::string>();
			c.notNull    = rs[3].convert<std::string>() != "0";
			c.primaryKey = rs[5].convert<std::string>() != "0";
			Poco::Dynamic::Var dflt = rs[4];
			if (!dflt.isEmpty())
			{
				c.hasDefault   = true;
				c.defaultValue = dflt.convert<std::string>();
			}
			columns.push_back(c);
		}
	}
	catch (const Poco::Exception&)
	{
	}
	return columns;
}


MemoryDBInspector::RowCount MemoryDBInspector::rowCount(
	const std::string& database, const std::string& table)
{
	RowCount rc;
	rc.database = database;
	rc.table    = table;
	try
	{
		Poco::Int64 n = 0;
		_db.session() << ("SELECT count(*) FROM " + quoteIdent(database) + "." + quoteIdent(table)),
			into(n), now;
		rc.rows  = n;
		rc.known = true;
	}
	catch (const Poco::Exception&)
	{
		// A view whose UNION references a detached shard cannot be counted.
	}
	return rc;
}


std::vector<MemoryDBInspector::TableInfo> MemoryDBInspector::schemaObjects(
	const std::string& database, RowCounts rowCounts)
{
	std::vector<TableInfo> objects;

	std::vector<TableInfo> tables;
	std::vector<std::pair<std::string, IndexInfo> > indexes; // (owning table, index)
	try
	{
		Statement stmt = (_db.session() <<
			("SELECT type, name, tbl_name, ifnull(sql, '') FROM " + quoteIdent(database) +
			 ".sqlite_master ORDER BY type, name"));
		stmt.execute();
		RecordSet rs(stmt);
		for (bool more = rs.moveFirst(); more; more = rs.moveNext())
		{
			const std::string type = rs[0].convert<std::string>();
			if (type == "index")
			{
				IndexInfo idx;
				idx.name = rs[1].convert<std::string>();
				idx.sql  = rs[3].convert<std::string>();
				indexes.emplace_back(rs[2].convert<std::string>(), idx);
			}
			else if (type == "table" || type == "view")
			{
				TableInfo t;
				t.name = rs[1].convert<std::string>();
				t.type = type;
				t.sql  = rs[3].convert<std::string>();
				tables.push_back(t);
			}
		}
	}
	catch (const Poco::Exception&)
	{
		// The schema vanished (force-detached shard). Contribute nothing.
		return objects;
	}

	for (auto& t: tables)
	{
		t.columns = tableColumns(database, t.name);
		if (rowCounts == RowCounts::EXACT)
		{
			const RowCount rc = rowCount(database, t.name);
			t.rows      = rc.rows;
			t.rowsKnown = rc.known;
		}
		for (const auto& i: indexes)
			if (i.first == t.name) t.indexes.push_back(i.second);
		objects.push_back(t);
	}
	return objects;
}


MemoryDBInspector::Schema MemoryDBInspector::schema(RowCounts rowCounts)
{
	Schema s;
	for (const auto& db: databaseNames())
	{
		DatabaseInfo info;
		info.name    = db.first;
		info.file    = db.second;
		info.objects = schemaObjects(db.first, rowCounts);
		s.databases.push_back(info);
	}
	return s;
}


void MemoryDBInspector::checkSQLAllowed(const std::string& sql)
{
	if (_policy == SQLPolicy::ALLOW_WRITES) return;

	// Classification uses the same parser MemoryDB::onStatement uses to decide what
	// counts as a write, so the two agree by construction. Spelled through
	// Poco::Data::Parser rather than the underlying namespace.
	//
	// Deliberately NOT Statement::isSelect(): that is gated on
	// SessionImpl::shouldParse(), i.e. the session feature "sqlParse", which
	// defaults to false - so it would report "unspecified" here. Turning the
	// feature on is not ours to do: it is session-wide state the owner did not ask
	// for, and Statement::checkBeginTransaction uses it to decide whether to open a
	// transaction, so flipping it would change when the owner's transactions start.
	Parser::SQLParserResult result;
	if (!Parser::SQLParser::parse(sql, &result) || !result.isValid())
	{
		// Unparseable is treated as a write. The parser does not cover every
		// SQLite-ism, and on a live store the safe reading of "I do not know what
		// this is" is "assume it changes something".
		throw Poco::InvalidAccessException(std::string("read-only: cannot parse this "
			"statement, so it is treated as a write: ") +
			(result.errorMsg() ? result.errorMsg() : "parse error"));
	}

	if (result.size() == 0)
		throw Poco::InvalidAccessException("read-only: no statement to run");

	for (std::size_t i = 0; i < result.size(); ++i)
	{
		// Every statement must be a SELECT, which is what catches both a trailing
		// "; DROP TABLE t" (SQLiteStatementImpl executes leftovers) and a
		// "WITH ... DELETE" whose leading keyword looks harmless.
		if (result.getStatement(i)->type() != Parser::kStmtSelect)
		{
			throw Poco::InvalidAccessException("read-only: only SELECT is allowed; statement " +
				Poco::NumberFormatter::format(i + 1) + " is not a SELECT");
		}
	}

	// Note there is deliberately no PRAGMA query_only here. The connection is
	// shared with the owner and query_only is connection-scoped and sticky, so
	// setting it would break the owner's writes for as long as it stayed on, and
	// toggling it around each query would break them inside the window. Refusal at
	// this entry point is the only correct enforcement.
}


void MemoryDBInspector::fillQueryResult(QueryResult& result, Statement& stmt, std::size_t maxRows)
{
	RecordSet rs(stmt);
	const std::size_t cols = rs.columnCount();

	for (std::size_t c = 0; c < cols; ++c)
	{
		ResultColumn rc;
		rc.name = rs.columnName(c);
		rc.type = columnTypeName(rs.columnType(c));
		result.columns.push_back(rc);
	}

	if (cols == 0) return;

	std::size_t index = 0;
	for (bool more = rs.moveFirst(); more; more = rs.moveNext(), ++index)
	{
		// The statement carries a limit of maxRows + 1, so at most one extra row is
		// ever materialized; seeing it is how we know the result was truncated.
		if (result.rows.size() >= maxRows)
		{
			result.truncated = true;
			break;
		}

		std::vector<Cell> row;
		row.reserve(cols);
		for (std::size_t c = 0; c < cols; ++c)
		{
			Cell cell;
			try
			{
				const Poco::Dynamic::Var v = rs.value(c, index);
				if (!v.isEmpty())
				{
					cell.isNull = false;
					if (rs.columnType(c) == MetaColumn::FDT_BLOB)
					{
						// extract<BLOB>() returns a reference - no copy - but it matches
						// the holder by RTTI, and LOB<T>, being a template instantiation,
						// has no exported typeinfo: a hidden-visibility build (macOS)
						// cannot match it across the PocoData boundary where the Var was
						// created. Take the cheap path when it works and fall back to
						// convert(), whose virtual dispatch always works at the cost of
						// copying the bytes to count them.
						std::size_t blobSize = 0;
						try
						{
							blobSize = v.extract<Poco::Data::BLOB>().size();
						}
						catch (const Poco::BadCastException&)
						{
							blobSize = v.convert<std::string>().size();
						}
						cell.text = "<blob, " +
							Poco::NumberFormatter::format(blobSize) + " bytes>";
					}
					else cell.text = v.convert<std::string>();
				}
			}
			catch (const Poco::Exception&)
			{
				cell.isNull = false;
				cell.text   = "<unreadable>";
			}
			row.push_back(cell);
		}
		result.rows.push_back(row);
	}

	result.rowCount = result.rows.size();
	if (!result.truncated && !stmt.done()) result.truncated = true;
}


MemoryDBInspector::QueryResult MemoryDBInspector::query(const std::string& sql, std::size_t maxRows)
{
	// The statement's row limit is a Poco::UInt32 carrying maxRows + 1: clamp
	// up front so an oversized request can neither wrap the + 1 nor truncate in
	// the cast. The clamped value is also what the result reports as its cap.
	constexpr std::size_t maxRowsCap = std::numeric_limits<Poco::UInt32>::max() - 1;
	if (maxRows > maxRowsCap) maxRows = maxRowsCap;

	QueryResult result;
	result.maxRows = maxRows;

	Poco::Stopwatch sw;
	sw.start();
	try
	{
		checkSQLAllowed(sql);

		// Through MemoryDB::operator <<, never session(), so the WITHOUT ROWID
		// rejection and the poisoned-instance check apply as they do to the owner's
		// own writes.
		Statement stmt = (_db << sql);
		// Bound the extraction itself. Without this, RecordSet materializes every
		// row before maxRows is ever consulted, so a SELECT over a large table
		// would pull the whole table into memory first.
		stmt, limit(static_cast<Poco::UInt32>(maxRows + 1));

		result.affected = stmt.execute();
		// Extraction is part of the measured work: with the row limit bound,
		// rows materialize while the result set is walked, so stopping the
		// timer before fillQueryResult would report a large SELECT as
		// near-instantaneous.
		fillQueryResult(result, stmt, maxRows);
		sw.stop();
		result.ok = true;
	}
	catch (const Poco::Exception& exc)
	{
		sw.stop();
		result.ok    = false;
		result.error = exc.displayText();
	}
	result.elapsedMs = sw.elapsed() / 1000.0;
	return result;
}


//
// Group C
//


bool MemoryDBInspector::flush()
{
	// MemoryDB::flush() is skip-on-busy and returns void. Its own documentation
	// prescribes bracketing with lastFlushTime() rather than inferring from
	// dirty(), which a concurrent writer can re-dirty the instant a flush lands.
	const Poco::Timestamp before;
	_db.flush();
	return _db.lastFlushTime() >= before;
}


MemoryDBInspector::ActionResult MemoryDBInspector::perform(const std::string& action,
	Poco::UInt32 shardId, const std::string& table)
{
	ActionResult r;
	try
	{
		const bool writes = (action == "seal" || action == "drop" || action == "enforceRetention");
		if (writes && _policy == SQLPolicy::READ_ONLY)
			throw Poco::InvalidAccessException("read-only: '" + action + "' would modify the store");

		if (action == "flush")
		{
			r.message = flush() ? "flushed to disk" : "flush skipped: another flush is in progress";
		}
		else if (action == "seal")
		{
			_db.sealActive();
			flush();
			r.message = "active shard sealed";
		}
		else if (action == "attach")
		{
			r.detail  = _db.attachArchived(shardId);
			r.message = "attached as " + r.detail;
		}
		else if (action == "detach")
		{
			_db.detachArchived(shardId);
			r.message = "shard " + Poco::NumberFormatter::format(shardId) + " detached";
		}
		else if (action == "attachAll")
		{
			_db.attachAllArchived();
			r.message = "all sealed shards attached read-only";
		}
		else if (action == "detachAll")
		{
			_db.detachAllArchived();
			r.message = "all sealed shards detached";
		}
		else if (action == "drop")
		{
			_db.deleteShard(shardId);
			r.message = "shard " + Poco::NumberFormatter::format(shardId) + " deleted";
		}
		else if (action == "enforceRetention")
		{
			r.count   = _db.enforceRetention();
			r.message = Poco::NumberFormatter::format(r.count) + " shard(s) dropped";
		}
		else if (action == "historyView")
		{
			if (table.empty())
				throw Poco::InvalidArgumentException("historyView needs a table name");
			_db.attachAllArchived();
			r.detail  = _db.historyView(table);
			r.message = "view " + r.detail + " spans live plus attached shards";
		}
		else
		{
			throw Poco::InvalidArgumentException("unknown action: " + action);
		}
		r.ok = true;
	}
	catch (const Poco::Exception& exc)
	{
		r.ok    = false;
		r.error = exc.displayText();
	}
	return r;
}


//
// serializers
//


void MemoryDBInspector::writeJSON(std::ostream& os, const Status& status)
{
	JSONWriter w(os);
	w.beginObject();
	w.pair("ok", true);
	w.pair("directory", status.directory);
	w.pair("readOnly", status.readOnly);
	w.pair("dirty", status.dirty);
	writeTimestamp(w, "lastFlush", status.lastFlush);
	w.pair("shardCount", static_cast<Poco::UInt64>(status.shardCount));

	w.key("shards").beginArray();
	for (const auto& s: status.shards)
	{
		w.beginObject();
		w.pair("id", static_cast<Poco::UInt64>(s.id));
		w.pair("sealed", s.sealed);
		writeTimestamp(w, "createdAt", s.createdAt);
		writeTimestamp(w, "sealedAt", s.sealedAt);
		w.pair("bytes", s.bytes);
		w.pair("filename", s.filename);
		w.pair("attached", s.attached);
		w.pair("alias", s.alias);
		w.endObject();
	}
	w.endArray();
	w.pair("shardBytes", status.shardBytes);

	w.key("files").beginArray();
	for (const auto& f: status.files)
	{
		w.beginObject();
		w.pair("name", f.name);
		w.pair("bytes", f.bytes);
		writeTimestamp(w, "modified", f.modified);
		w.endObject();
	}
	w.endArray();
	w.pair("diskBytes", status.diskBytes);
	w.endObject();
}


void MemoryDBInspector::writeJSON(std::ostream& os, const Schema& schema)
{
	JSONWriter w(os);
	w.beginObject();
	w.pair("ok", true);
	w.key("databases").beginArray();
	for (const auto& db: schema.databases)
	{
		w.beginObject();
		w.pair("name", db.name);
		w.pair("file", db.file);
		w.key("objects").beginArray();
		for (const auto& t: db.objects)
		{
			w.beginObject();
			w.pair("name", t.name);
			w.pair("type", t.type);
			w.pair("sql", t.sql);
			w.key("columns").beginArray();
			for (const auto& c: t.columns)
			{
				w.beginObject();
				w.pair("name", c.name);
				w.pair("type", c.type);
				w.pair("notNull", c.notNull);
				if (c.hasDefault) w.pair("default", c.defaultValue);
				else w.pairNull("default");
				w.pair("pk", c.primaryKey);
				w.endObject();
			}
			w.endArray();
			if (t.rowsKnown) w.pair("rows", t.rows);
			else w.pairNull("rows");
			w.key("indexes").beginArray();
			for (const auto& i: t.indexes)
			{
				w.beginObject();
				w.pair("name", i.name);
				w.pair("sql", i.sql);
				w.endObject();
			}
			w.endArray();
			w.endObject();
		}
		w.endArray();
		w.endObject();
	}
	w.endArray();
	w.endObject();
}


void MemoryDBInspector::writeJSON(std::ostream& os, const RowCount& rowCount)
{
	JSONWriter w(os);
	w.beginObject();
	w.pair("ok", true);
	w.pair("database", rowCount.database);
	w.pair("table", rowCount.table);
	if (rowCount.known) w.pair("rows", rowCount.rows);
	else w.pairNull("rows");
	w.endObject();
}


void MemoryDBInspector::writeJSON(std::ostream& os, const QueryResult& result)
{
	JSONWriter w(os);
	w.beginObject();
	w.pair("ok", result.ok);
	w.pair("error", result.error);
	w.key("columns").beginArray();
	for (const auto& c: result.columns)
	{
		w.beginObject();
		w.pair("name", c.name);
		w.pair("type", c.type);
		w.endObject();
	}
	w.endArray();
	w.key("rows").beginArray();
	for (const auto& row: result.rows)
	{
		w.beginArray();
		for (const auto& cell: row)
		{
			if (cell.isNull) w.null();
			else w.value(cell.text);
		}
		w.endArray();
	}
	w.endArray();
	w.pair("rowCount", result.rowCount);
	w.pair("truncated", result.truncated);
	w.pair("maxRows", result.maxRows);
	w.pair("affected", result.affected);
	w.pair("elapsedMs", result.elapsedMs);
	w.endObject();
}


void MemoryDBInspector::writeJSON(std::ostream& os, const ActionResult& result)
{
	JSONWriter w(os);
	w.beginObject();
	w.pair("ok", result.ok);
	w.pair("message", result.message);
	w.pair("error", result.error);
	w.pair("detail", result.detail);
	w.pair("count", result.count);
	w.endObject();
}


void MemoryDBInspector::writeStatusJSON(std::ostream& os)
{
	const Status s = status();
	writeJSON(os, s);
}


void MemoryDBInspector::writeSchemaJSON(std::ostream& os, RowCounts rowCounts)
{
	const Schema s = schema(rowCounts);
	writeJSON(os, s);
}


void MemoryDBInspector::writeRowCountJSON(std::ostream& os,
	const std::string& database, const std::string& table)
{
	const RowCount rc = rowCount(database, table);
	writeJSON(os, rc);
}


void MemoryDBInspector::writeQueryJSON(std::ostream& os, const std::string& sql)
{
	writeQueryJSON(os, sql, _maxRows);
}


void MemoryDBInspector::writeQueryJSON(std::ostream& os, const std::string& sql, std::size_t maxRows)
{
	const QueryResult r = query(sql, maxRows);
	writeJSON(os, r);
}


void MemoryDBInspector::writeActionJSON(std::ostream& os, const std::string& action,
	Poco::UInt32 shardId, const std::string& table)
{
	const ActionResult r = perform(action, shardId, table);
	writeJSON(os, r);
}


} // namespace Poco::Data::SQLite
