//
// DataSession.cpp
//
// Fuzzer for the Poco::Data session and statement surface over SQLite.
//
// Threat model:
// - Untrusted input: database FILE bytes (what an attacker-supplied or
//   corrupted .db/.sqlite file delivers to an application opening it) and
//   SQL statement text (attacker-controlled queries through any injection
//   or query-building path).
// - Trust boundary: SQLite's file/page parser reached through
//   Poco::Data::Session("sqlite", file), and the statement compile/execute
//   path reached through Poco::Data::Statement and RecordSet.
// - The Data and Data.SQLite modules (8.9k + SQLite engine source lines)
//   are linked by no fuzz target as of the 2026-11-10 report, so the
//   session/statement/binder/extractor layer is dark. This pair adds the
//   first fuzz target linking Poco::DataSQLite (with -DENABLE_DATA_SQLITE=ON
//   explicit; sqlite is bundled in dependencies/sqlite3) and drives
//   open-file -> execute -> extract with attacker-controlled inputs.
// - Bounds: every session is in-memory or a bounded temp file; SQL text is
//   bounded by libFuzzer's input cap; no network, no filesystem writes
//   beyond one wiped scratch directory.
// - What counts as a bug: memory-safety violations in Poco's
//   SessionImpl/StatementImpl/Binder/Extractor layer while handling
//   malformed databases, hostile SQL or odd type coercions (ASan/UBSan
//   findings). SQLite-engine-internal crashes are findings too, but the
//   primary target is the Poco wrapper layer.
//

#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Statement.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/File.h"
#include "Poco/Path.h"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

using namespace Poco::Data;


namespace
{

template <typename F>
void ignore(const F& func)
{
	try
	{
		func();
	}
	catch (const std::exception&)
	{
	}
}


const char* scratchRoot()
{
	static std::string root = std::string(Poco::Path::temp()) + "poco-data-fuzz";
	return root.c_str();
}


std::string scratchFile(std::size_t slot)
{
	const std::string root = scratchRoot();
	std::error_code ec;
	std::filesystem::create_directories(root, ec);
	const std::string path = root + "/fuzz" + std::to_string(slot % 4) + ".db";
	std::filesystem::remove(path, ec);
	return path;
}


void fuzzOpenFile(const std::string& bytes, std::size_t slot)
{
	// Attacker bytes become the database FILE: SQLite's page/header parser
	// runs on them through Poco::Data::Session open/close.
	const std::string path = scratchFile(slot);
	{
		std::ofstream out(path, std::ios::binary | std::ios::trunc);
		out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
	}
	ignore([&]
	{
		Session session("sqlite", path);
		[[maybe_unused]] bool connected = session.isConnected();
	});
	std::error_code ec;
	std::filesystem::remove(path, ec);
}


void fuzzExecute(const std::string& sql)
{
	ignore([&]
	{
		Session session("sqlite", ":memory:");
		session << sql, Poco::Data::Keywords::now;
	});
}


void fuzzQueryExtract(const std::string& sql)
{
	ignore([&]
	{
		Session session("sqlite", ":memory:");
		// Fixed schema so SELECT statements have something to extract.
		session << "CREATE TABLE IF NOT EXISTS fuzz "
		           "(id INTEGER PRIMARY KEY, name TEXT, val REAL, flag INTEGER)",
		    Poco::Data::Keywords::now;
		session << "INSERT INTO fuzz VALUES (1, 'alpha', 2.5, 1)",
		    Poco::Data::Keywords::now;
		session << "INSERT INTO fuzz VALUES (2, 'beta', -0.5, 0)",
		    Poco::Data::Keywords::now;

		RecordSet rs(session, sql);
		[[maybe_unused]] std::size_t rows = rs.rowCount();
		[[maybe_unused]] std::size_t cols = rs.columnCount();
		if (rows > 0 && cols > 0)
		{
			// Extraction through the Var-based type handler.
			Poco::Dynamic::Var v = rs.value(0, 0);
			[[maybe_unused]] const std::string& s = v.convert<std::string>();
		}
	});
}


void fuzzBinding(const std::string& bytes)
{
	ignore([&]
	{
		Session session("sqlite", ":memory:");
		session << "CREATE TABLE IF NOT EXISTS bind "
		           "(a TEXT, b INTEGER, c REAL)",
		    Poco::Data::Keywords::now;

		// Attacker bytes as bound parameters (string + numeric coercions).
		std::string text = bytes.substr(0, std::min<std::size_t>(64, bytes.size()));
		Poco::Int64 number = 0;
		for (std::size_t i = 0; i < bytes.size() && i < 8; ++i)
			number = (number << 8) | bytes[i];
		double real = static_cast<double>(number) / 1024.0;

		Statement insert = (session << "INSERT INTO bind VALUES (?, ?, ?)",
		    Poco::Data::Keywords::use(text),
		    Poco::Data::Keywords::use(number),
		    Poco::Data::Keywords::use(real));
		insert.execute();

		Statement select = (session << "SELECT a, b, c FROM bind",
		    Poco::Data::Keywords::now);
		RecordSet rs(select);
		[[maybe_unused]] std::size_t rows = rs.rowCount();
	});
}


} // namespace


extern "C" int LLVMFuzzerInitialize(int* argc, char*** argv)
{
	Poco::Data::SQLite::Connector::registerConnector();
	return 0;
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	// Input layout: [op:1][payload] - payload is a db file, SQL text or
	// bound-parameter bytes depending on the op.
	if (size < 1) return 0;

	static std::size_t slotCounter = 0;
	const std::size_t slot = slotCounter++ % 4;

	const std::uint8_t op = data[0];
	std::string payload(reinterpret_cast<const char*>(data) + 1, size - 1);

	const bool runAll = op == 0;

	if (runAll || op & 0x01)
		fuzzOpenFile(payload, slot);

	if (runAll || op & 0x02)
		fuzzExecute(payload);

	if (runAll || op & 0x04)
		fuzzQueryExtract(payload);

	if (runAll || op & 0x08)
		fuzzBinding(payload);

	return 0;
}
