//
// MemoryDB.cpp
//
// This sample demonstrates Poco::Data::SQLite::MemoryDB: an in-memory SQLite
// database with sharded, robust persistence to disk. It is used like a regular
// Poco::Data::Session (operator<<, now, into, use) but persists its data to a
// directory of shard files plus a manifest catalog, so the data survives
// process restarts.
//
// The program walks through the headline capabilities in order:
//
//   1. Construct in a fresh directory and use it like a Session.
//   2. flush() to disk, let the object go out of scope, then reopen the same
//      directory and verify the data survived.
//   3. sealActive() to roll the active shard, write more, and observe that the
//      directory now contains multiple shard files.
//   4. Reopen with loadArchivedShards=false so only the active shard is in RAM,
//      attach the sealed shards read-only, and use historyView() to query
//      live+history through a SQLite-enforced read-only view.
//
// Each step prints a labelled block and a "[PASS]" line; the program exits 0
// only if every check holds, so the sample doubles as a smoke test.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0


#include "Poco/Data/SQLite/MemoryDB.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


using namespace Poco::Data::Keywords;
using Poco::Data::SQLite::MemoryDB;


namespace
{
	int failures = 0;

	void check(bool ok, const std::string& what)
	{
		if (ok)
		{
			std::cout << "  [PASS] " << what << '\n';
		}
		else
		{
			std::cout << "  [FAIL] " << what << '\n';
			++failures;
		}
	}

	std::vector<std::string> shardFiles(const std::string& dir)
	{
		std::vector<std::string> names;
		Poco::File(dir).list(names);
		std::vector<std::string> shards;
		for (const auto& n: names)
			if (n.rfind("shard-", 0) == 0) shards.push_back(n);
		std::sort(shards.begin(), shards.end());
		return shards;
	}

	void insertReadings(MemoryDB& db, const std::string& sensor, int count, Poco::Int64 baseTs)
	{
		for (int i = 0; i < count; ++i)
		{
			Poco::Int64 ts = baseTs + i;
			std::string s = sensor;
			double v = 20.0 + 0.1 * i;
			db << "INSERT INTO reading(ts, sensor, value) VALUES(:t, :s, :v)",
				use(ts), use(s), use(v), now;
		}
	}
}


int main(int /*argc*/, char** /*argv*/)
{
	const std::string dir = Poco::TemporaryFile::tempName();
	std::cout << "MemoryDB sample, persisting under: " << dir << "\n\n";

	try
	{
		// 1. construct and use like a Session
		std::cout << "1. Create database, define schema, insert live readings\n";
		{
			MemoryDB db(dir);
			db << "CREATE TABLE reading("
				"id INTEGER PRIMARY KEY, "
				"ts INTEGER, "
				"sensor TEXT, "
				"value REAL)", now;
			insertReadings(db, "temp", 10, 1000);
			check(db.dirty(), "db is dirty after writes");
			check(db.shardCount() == 1, "one active shard exists in the model");

			// 2. flush, close, reopen
			std::cout << "\n2. Flush to disk, close, reopen the same directory\n";
			db.flush();
			check(!db.dirty(), "db is clean after flush");
			check(Poco::File(Poco::Path(dir, "manifest.db").toString()).exists(),
				"manifest.db exists on disk");
			check(shardFiles(dir).size() == 1, "one shard file exists on disk");
		} // db goes out of scope

		{
			MemoryDB db(dir);
			int count = 0;
			db << "SELECT count(*) FROM reading", into(count), now;
			check(count == 10, "10 rows survived the restart");

			// 3. seal and write more -> multiple shards
			std::cout << "\n3. Seal the active shard, write more, observe sharding\n";
			db.sealActive();
			db.flush();
			insertReadings(db, "humid", 5, 2000);
			db.flush();

			std::vector<Poco::UInt32> archived = db.archivedShardIds();
			check(archived.size() == 1, "one sealed shard recorded in the manifest");
			check(db.shardCount() == 2, "two shards total (one sealed, one active)");

			std::vector<std::string> files = shardFiles(dir);
			std::cout << "   shard files on disk:";
			for (const auto& f: files) std::cout << ' ' << f;
			std::cout << '\n';
			check(files.size() == 2, "two shard files exist on disk");
		}

		// 4. read live + history without loading sealed data into RAM
		std::cout << "\n4. Reopen with loadArchivedShards=false; history via UNION view\n";
		{
			MemoryDB::Options off;
			MemoryDB db(dir, off);

			int liveCount = 0;
			db << "SELECT count(*) FROM reading", into(liveCount), now;
			check(liveCount == 5, "live view holds only the active shard's 5 rows");

			db.attachAllArchived();
			std::string view = db.historyView("reading");
			check(view == "reading_history", "historyView returned the expected view name");

			int allCount = 0;
			db << "SELECT count(*) FROM reading_history", into(allCount), now;
			check(allCount == 15, "history view sees live + sealed (10 + 5 = 15)");

			bool readOnly = false;
			try
			{
				db << "INSERT INTO reading_history(ts, sensor, value) VALUES(0, 'x', 0.0)", now;
			}
			catch (Poco::Exception&)
			{
				readOnly = true;
			}
			check(readOnly, "writes through reading_history are rejected by SQLite");

			check(!db.dirty(), "internal DDL for the view did not dirty the db");
		}

		std::cout << '\n';
		if (failures == 0)
			std::cout << "All checks OK.\n";
		else
			std::cout << failures << " check(s) failed.\n";
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << "Poco::Exception: " << exc.displayText() << '\n';
		failures = 1;
	}
	catch (std::exception& exc)
	{
		std::cerr << "std::exception: " << exc.what() << '\n';
		failures = 1;
	}

	// 5. cleanup
	try { Poco::File(dir).remove(true); }
	catch (...) {}

	return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
