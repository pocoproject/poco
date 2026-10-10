//
// SQLiteThreadSafetyTest.h
//
// Definition of the SQLiteThreadSafetyTest class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLiteThreadSafetyTest_INCLUDED
#define SQLiteThreadSafetyTest_INCLUDED


#include "Poco/Data/SQLite/SQLite.h"
#include "CppUnit/TestCase.h"
#include <string>


class SQLiteThreadSafetyTest: public CppUnit::TestCase
	/// Regression test for the lock-free bare reads of SQLite Vdbe / connection
	/// state in Poco::Data::SQLite::SQLiteStatementImpl that fire when one
	/// Session is shared across threads concurrently with operations that
	/// mutate the state under sqlite3_db_mutex. The canonical trigger is
	/// ATTACH / DETACH, which calls sqlite3ExpirePreparedStatements and writes
	/// Vdbe::expired in the same word as Vdbe::readOnly.
	///
	/// SQLiteStatementImpl::hasNext (line 249) and
	/// SQLiteStatementImpl::affectedRowCount (line 322) read connection /
	/// stmt state via sqlite3_stmt_readonly and sqlite3_changes. Both are
	/// wrapped in Utility::SQLiteMutex to make the read atomic with respect
	/// to writes by other threads. This test exercises the hasNext path
	/// (the affectedRowCount one is only reachable in the microsecond window
	/// between Statement compile and the first hasNext, which POCO's
	/// per-Statement mutex hides from external threads; the fix there is
	/// defensive, by symmetry with the hasNext one).
	///
	/// In release mode the test passes (the torn-byte reads return values
	/// that are still valid for the stmt's lifetime). Under TSan it passes
	/// while the SQLiteMutex wrap stays in place at SQLiteStatementImpl.cpp;
	/// removing it surfaces a data race in sqlite3ExpirePreparedStatements
	/// at sqlite3.c:93062 paired with the bare read in
	/// SQLiteStatementImpl::hasNext (verified during development by
	/// temporarily reverting the wrap and rerunning this test under TSan).
{
public:
	SQLiteThreadSafetyTest(const std::string& name);
	~SQLiteThreadSafetyTest();

	void testRaceHasNextVsAttach();
		/// Reader thread runs SELECT statements in a tight loop (each
		/// invocation goes through hasNext, which reads Vdbe / connection
		/// state via sqlite3_stmt_readonly and sqlite3_changes). Other
		/// thread runs ATTACH/DETACH on the same Session, which fires
		/// sqlite3ExpirePreparedStatements on every prepared statement on
		/// the connection. With the SQLiteMutex wrap at
		/// SQLiteStatementImpl.cpp:249 in place, TSan reports nothing;
		/// without it, TSan reports a race on the Vdbe bytes.

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	std::string _dbFile;
};


#endif // SQLiteThreadSafetyTest_INCLUDED
