//
// Utility.h
//
// Library: Data/SQLite
// Package: SQLite
// Module:  Utility
//
// Definition of Utility.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLite_Utility_INCLUDED
#define SQLite_Utility_INCLUDED


#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/Data/Session.h"
#include "Poco/Mutex.h"
#include "Poco/Types.h"
#include <map>


extern "C"
{
	typedef struct sqlite3 sqlite3;
	typedef struct sqlite3_stmt sqlite3_stmt;
	typedef struct sqlite3_mutex* _pMutex;
}


namespace Poco::Data::SQLite {


class SessionImpl;
class Notifier;


class SQLite_API Utility
	/// Various utility functions for SQLite.
{
public:

	// SQLite Transaction type
	static const std::string TRANSACTION_TYPE_PROPERTY_KEY;

	enum class TransactionType
	{
		DEFERRED,
		EXCLUSIVE,
		IMMEDIATE
	};

	static const std::string SQLITE_DATE_FORMAT;
	static const std::string SQLITE_TIME_FORMAT;
	static const std::string SQLITE_DATETIME_FRAC_FORMAT;
	typedef std::map<std::string, MetaColumn::ColumnDataType> TypeMap;

	static const int THREAD_MODE_SINGLE;
	static const int THREAD_MODE_MULTI;
	static const int THREAD_MODE_SERIAL;

	static const int OPERATION_INSERT;
	static const int OPERATION_DELETE;
	static const int OPERATION_UPDATE;

	[[nodiscard]] static sqlite3* dbHandle(const Session& session);
		/// Returns the native handle of the session's current connection, or nullptr
		/// if the session is not connected. The handle changes when the session is
		/// closed, opened or reconnected.
		/// Throws NullPointerException if the session was moved from.

	static int setAttachedLimit(Session& session, int newVal);
		/// Sets the per-connection limit on the number of databases that can
		/// be attached simultaneously (SQLite's SQLITE_LIMIT_ATTACHED). Thin
		/// wrapper over sqlite3_limit() so callers don't need the raw handle
		/// or to pull <sqlite3.h> into their include path (and so the
		/// underlying symbol doesn't need to be re-exported from
		/// PocoDataSQLite on platforms whose static-lib link only sees
		/// __declspec(dllexport) symbols).
		///
		/// Passing newVal < 0 leaves the limit unchanged and returns the
		/// current value. The runtime limit cannot exceed
		/// SQLITE_MAX_ATTACHED (compile-time constant, default 10, SQLite
		/// hard ceiling 125); requests above the compile-time cap are
		/// silently clamped to it by SQLite. Returns the previous value.
		/// Throws NullPointerException if the session is not connected.

	[[nodiscard]] static std::string lastError(sqlite3* pDB);
		/// Retreives the last error code from sqlite and converts it to a string.

	[[nodiscard]] static std::string lastError(const Session& session);
		/// Retreives the last error code from sqlite and converts it to a string.

	static void throwException(sqlite3* pDB, int rc, const std::string& addErrMsg = std::string());
		/// Throws for an error code the appropriate exception

	[[nodiscard]] static MetaColumn::ColumnDataType getColumnType(sqlite3_stmt* pStmt, std::size_t pos);
		/// Returns column data type.
		/// Throws NullPointerException if pStmt is null.

	static bool fileToMemory(sqlite3* pInMemory, const std::string& fileName);
		/// Loads the contents of a database file on disk into an opened
		/// database in memory.
		///
		/// Returns true if succesful.
		/// Throws NullPointerException if pInMemory is null.

	static bool fileToMemory(const Session& session, const std::string& fileName);
		/// Loads the contents of a database file on disk into an opened
		/// database in memory.
		///
		/// Returns true if succesful.
		/// Throws NullPointerException if the session is not connected.

	static bool memoryToFile(const std::string& fileName, sqlite3* pInMemory);
		/// Saves the contents of an opened database in memory to the
		/// database on disk.
		///
		/// Returns true if succesful.
		/// Throws NullPointerException if pInMemory is null.

	static bool memoryToFile(const std::string& fileName, const Session& session);
		/// Saves the contents of an opened database in memory to the
		/// database on disk.
		///
		/// Returns true if succesful.
		/// Throws NullPointerException if the session is not connected.

	[[nodiscard]] static bool isThreadSafe();
		/// Returns true if SQLite was compiled in multi-thread or serialized mode.
		/// See http://www.sqlite.org/c3ref/threadsafe.html for details.
		///
		/// Returns true if succesful

	static bool setThreadMode(int mode);
		/// Sets the threading mode to single, multi or serialized.
		/// See http://www.sqlite.org/threadsafe.html for details.
		///
		/// Returns true if succesful

	[[nodiscard]] static int getThreadMode();
		/// Returns the thread mode.

	typedef void(*UpdateCallbackType)(void*, int, const char*, const char*, long long);
		/// Update callback function type. The row id parameter is long long to match
		/// SQLite's sqlite3_int64 typedef exactly; calling the callback through a
		/// function pointer of any other type (e.g. with Poco::Int64, which is long
		/// on most 64-bit platforms) is undefined behavior even when the underlying
		/// width agrees.

	typedef int(*CommitCallbackType)(void*);
		/// Commit callback function type.

	typedef void(*RollbackCallbackType)(void*);
		/// Rollback callback function type.

	template <typename T, typename CBT>
	static bool registerUpdateHandler(sqlite3* pDB, CBT callbackFn, T* pParam)
		/// Registers the callback for (1)(insert, delete, update), (2)(commit) or
		/// or (3)(rollback) events. Only one function per group can be registered
		/// at a time; a registration replaces the one before it. Storage pointed to
		/// by pParam must remain valid as long as registration is active.
		/// Registering with callbackFn set to zero disables the notifications of
		/// the group if they were registered with pParam, and returns false if they
		/// were not; a registration made with another pParam stays in place.
		/// Disabling uses the connection only if they were, so pDB may be the handle
		/// of a connection closed before; the connection must not be closed while
		/// another thread registers or disables a callback of it.
		/// Closing the session, or returning it to its pool, disables all
		/// notifications of its connection.
		/// Throws NullPointerException if pDB is null and callbackFn is not zero.
		///
		/// See http://www.sqlite.org/c3ref/update_hook.html and
		/// http://www.sqlite.org/c3ref/commit_hook.html for details.
	{
		return eventHookRegister(pDB, callbackFn, pParam);
	}

	template <typename T, typename CBT>
	static bool registerUpdateHandler(const Session& session, CBT callbackFn, T* pParam)
		/// Registers the callback by calling registerUpdateHandler(sqlite3*, CBT, T*).
		/// Throws NullPointerException if the session was moved from, or if callbackFn
		/// is not zero and the session is not connected.
	{
		return registerUpdateHandler(dbHandle(session), callbackFn, pParam);
	}

	class SQLiteMutex
	{
	public:
		SQLiteMutex(sqlite3* pDB);
		~SQLiteMutex();

	private:
		SQLiteMutex();
		sqlite3_mutex* _pMutex;
	};

private:
	Utility();
		/// Maps SQLite column declared types to Poco::Data types through
		/// static TypeMap member.
		///
		/// Note: SQLite is type-agnostic and it is the end-user responsibility
		/// to ensure that column declared data type corresponds to the type of
		/// data actually held in the database.
		///
		/// Column types are case-insensitive.

	Utility(const Utility&) = delete;
	Utility& operator = (const Utility&) = delete;

	[[nodiscard]] static bool eventHookRegister(sqlite3* pDB, UpdateCallbackType callbackFn, void* pParam);
	[[nodiscard]] static bool eventHookRegister(sqlite3* pDB, CommitCallbackType callbackFn, void* pParam);
	[[nodiscard]] static bool eventHookRegister(sqlite3* pDB, RollbackCallbackType callbackFn, void* pParam);

	static void unregisterUpdateHandlers(sqlite3* pDB);
		/// Disables the update, commit and rollback callbacks of the connection
		/// and removes their registrations. Called by SessionImpl before it closes
		/// the connection or the session goes back to its pool. Does not throw.

	[[nodiscard]] static int registeredHooks(sqlite3* pDB, const void* pParam);
		/// Returns the callbacks of the connection registered with pParam: 1 for
		/// update, 2 for commit and 4 for rollback callbacks, as Notifier's
		/// SQLITE_NOTIFY_UPDATE, SQLITE_NOTIFY_COMMIT and SQLITE_NOTIFY_ROLLBACK.
		/// Returns 0 if pDB is null. Uses pDB only to look the registrations up,
		/// so it may be the handle of a connection closed before.

	static TypeMap     _types;
	static Poco::Mutex _mutex;
	static int         _threadMode;

	friend class Poco::Data::SQLite::SessionImpl;
	friend class Poco::Data::SQLite::Notifier;
};


using TransactionType = Utility::TransactionType;


//
// inlines
//
inline std::string Utility::lastError(const Session& session)
{
	poco_assert_dbg ((0 == icompare(session.connector(), 0, 6, "sqlite")));
	return lastError(dbHandle(session));
}


inline bool Utility::memoryToFile(const std::string& fileName, const Session& session)
{
	poco_assert_dbg ((0 == icompare(session.connector(), 0, 6, "sqlite")));
	return memoryToFile(fileName, dbHandle(session));
}


inline bool Utility::fileToMemory(const Session& session, const std::string& fileName)
{
	poco_assert_dbg ((0 == icompare(session.connector(), 0, 6, "sqlite")));
	return fileToMemory(dbHandle(session), fileName);
}


} // namespace Poco::Data::SQLite


#endif // SQLite_Utility_INCLUDED
