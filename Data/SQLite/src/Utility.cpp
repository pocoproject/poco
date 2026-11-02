//
// Utility.cpp
//
// Library: Data/SQLite
// Package: SQLite
// Module:  Utility
//
// Implementation of Utility
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/SQLite/Utility.h"
#include "Poco/Data/SQLite/SQLiteException.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"
#include "Poco/Any.h"
#include "Poco/Exception.h"
#include <sqlite3.h>
#include <new>


#ifndef SQLITE_OPEN_URI
#define SQLITE_OPEN_URI 0
#endif


namespace Poco::Data::SQLite {


const std::string Utility::TRANSACTION_TYPE_PROPERTY_KEY = "transactionType";

const int Utility::THREAD_MODE_SINGLE = SQLITE_CONFIG_SINGLETHREAD;
const int Utility::THREAD_MODE_MULTI = SQLITE_CONFIG_MULTITHREAD;
const int Utility::THREAD_MODE_SERIAL = SQLITE_CONFIG_SERIALIZED;

#if defined(POCO_SQLITE_UNBUNDLED)
	#pragma message("WARNING: Utility::_threadMode is set based on SQLITE_THREADSAFE, which may not reflect the actual threading mode used by unbundled SQLite")
#endif

int Utility::_threadMode =
#if (SQLITE_THREADSAFE == 0)
	SQLITE_CONFIG_SINGLETHREAD;
#elif (SQLITE_THREADSAFE == 1)
	SQLITE_CONFIG_SERIALIZED;
#elif (SQLITE_THREADSAFE == 2)
	SQLITE_CONFIG_MULTITHREAD;
#endif

const int Utility::OPERATION_INSERT = SQLITE_INSERT;
const int Utility::OPERATION_DELETE = SQLITE_DELETE;
const int Utility::OPERATION_UPDATE = SQLITE_UPDATE;

const std::string Utility::SQLITE_DATE_FORMAT = "%Y-%m-%d";
const std::string Utility::SQLITE_TIME_FORMAT = "%H:%M:%S";
const std::string Utility::SQLITE_DATETIME_FRAC_FORMAT = "%Y-%m-%d %H:%M:%s";
Utility::TypeMap Utility::_types;
Poco::Mutex Utility::_mutex;


Utility::SQLiteMutex::SQLiteMutex(sqlite3* pDB): _pMutex((pDB) ? sqlite3_db_mutex(pDB) : nullptr)
{
	if (_pMutex)
		sqlite3_mutex_enter(_pMutex);
}


Utility::SQLiteMutex::~SQLiteMutex()
{
	if (_pMutex)
		sqlite3_mutex_leave(_pMutex);
}


Utility::Utility()
{
	if (_types.empty())
	{
		_types.insert(TypeMap::value_type("", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("BOOL", MetaColumn::FDT_BOOL));
		_types.insert(TypeMap::value_type("BOOLEAN", MetaColumn::FDT_BOOL));
		_types.insert(TypeMap::value_type("BIT", MetaColumn::FDT_BOOL));
		_types.insert(TypeMap::value_type("UINT8", MetaColumn::FDT_UINT8));
		_types.insert(TypeMap::value_type("UTINY", MetaColumn::FDT_UINT8));
		_types.insert(TypeMap::value_type("UINTEGER8", MetaColumn::FDT_UINT8));
		_types.insert(TypeMap::value_type("INT8", MetaColumn::FDT_INT8));
		_types.insert(TypeMap::value_type("TINY", MetaColumn::FDT_INT8));
		_types.insert(TypeMap::value_type("INTEGER8", MetaColumn::FDT_INT8));
		_types.insert(TypeMap::value_type("UINT16", MetaColumn::FDT_UINT16));
		_types.insert(TypeMap::value_type("USHORT", MetaColumn::FDT_UINT16));
		_types.insert(TypeMap::value_type("UINTEGER16", MetaColumn::FDT_UINT16));
		_types.insert(TypeMap::value_type("INT16", MetaColumn::FDT_INT16));
		_types.insert(TypeMap::value_type("SHORT", MetaColumn::FDT_INT16));
		_types.insert(TypeMap::value_type("INTEGER16", MetaColumn::FDT_INT16));
		_types.insert(TypeMap::value_type("UINT", MetaColumn::FDT_UINT32));
		_types.insert(TypeMap::value_type("UINT32", MetaColumn::FDT_UINT32));
		_types.insert(TypeMap::value_type("UINTEGER", MetaColumn::FDT_UINT64));
		_types.insert(TypeMap::value_type("UINTEGER32", MetaColumn::FDT_UINT32));
		_types.insert(TypeMap::value_type("INT", MetaColumn::FDT_INT32));
		_types.insert(TypeMap::value_type("INT32", MetaColumn::FDT_INT32));
		_types.insert(TypeMap::value_type("INTEGER", MetaColumn::FDT_INT64));
		_types.insert(TypeMap::value_type("INTEGER32", MetaColumn::FDT_INT32));
		_types.insert(TypeMap::value_type("UINT64", MetaColumn::FDT_UINT64));
		_types.insert(TypeMap::value_type("ULONG", MetaColumn::FDT_INT64));
		_types.insert(TypeMap::value_type("UINTEGER64", MetaColumn::FDT_UINT64));
		_types.insert(TypeMap::value_type("INT64", MetaColumn::FDT_INT64));
		_types.insert(TypeMap::value_type("LONG", MetaColumn::FDT_INT64));
		_types.insert(TypeMap::value_type("INTEGER64", MetaColumn::FDT_INT64));
		_types.insert(TypeMap::value_type("TINYINT", MetaColumn::FDT_INT8));
		_types.insert(TypeMap::value_type("SMALLINT", MetaColumn::FDT_INT16));
		_types.insert(TypeMap::value_type("BIGINT", MetaColumn::FDT_INT64));
		_types.insert(TypeMap::value_type("LONGINT", MetaColumn::FDT_INT64));
		_types.insert(TypeMap::value_type("COUNTER", MetaColumn::FDT_UINT64));
		_types.insert(TypeMap::value_type("AUTOINCREMENT", MetaColumn::FDT_UINT64));
		_types.insert(TypeMap::value_type("REAL", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("FLOA", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("FLOAT", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("DOUB", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("DOUBLE", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("DECIMAL", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("NUMERIC", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("CHAR", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("CLOB", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("TEXT", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("VARCHAR", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("NCHAR", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("NCLOB", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("NTEXT", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("NVARCHAR", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("LONGVARCHAR", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("BLOB", MetaColumn::FDT_BLOB));
		_types.insert(TypeMap::value_type("DATE", MetaColumn::FDT_DATE));
		_types.insert(TypeMap::value_type("TIME", MetaColumn::FDT_TIME));
		_types.insert(TypeMap::value_type("DATETIME", MetaColumn::FDT_TIMESTAMP));
		_types.insert(TypeMap::value_type("TIMESTAMP", MetaColumn::FDT_TIMESTAMP));
		_types.insert(TypeMap::value_type("UUID", MetaColumn::FDT_UUID));
		_types.insert(TypeMap::value_type("GUID", MetaColumn::FDT_UUID));
		_types.insert(TypeMap::value_type("JSON", MetaColumn::FDT_JSON));
	}
}


std::string Utility::lastError(sqlite3* pDB)
{
	std::string errStr;
	SQLiteMutex m(pDB);
	const char* pErr = sqlite3_errmsg(pDB);
	if (pErr) errStr = pErr;
	return errStr;
}


int Utility::setAttachedLimit(Session& session, int newVal)
{
	// sqlite3_limit acquires db->mutex internally, so no SQLiteMutex wrap
	// needed here. Existence rationale lives in the docstring (Utility.h):
	// we want callers to be able to set this limit without pulling
	// <sqlite3.h> into their TU, and we want the symbol exported through
	// PocoDataSQLite's normal SQLite_API surface so static-lib link works
	// on Windows.
	sqlite3* pDB = dbHandle(session);
	poco_check_ptr (pDB);
	return sqlite3_limit(pDB, SQLITE_LIMIT_ATTACHED, newVal);
}


MetaColumn::ColumnDataType Utility::getColumnType(sqlite3_stmt* pStmt, std::size_t pos)
{
	poco_check_ptr (pStmt);

	// Ensure statics are initialized
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		static Utility u;
	}

	const char* pc = sqlite3_column_decltype(pStmt, (int) pos);
	std::string sqliteType = pc ? pc : "";
	Poco::toUpperInPlace(sqliteType);
	sqliteType = sqliteType.substr(0, sqliteType.find_first_of(" ("));

	TypeMap::const_iterator it = _types.find(Poco::trimInPlace(sqliteType));
	if (_types.end() == it)	throw Poco::NotFoundException();

	return it->second;
}


void Utility::throwException(sqlite3* pDB, int rc, const std::string& addErrMsg)
{
	std::string errMsg = addErrMsg.empty() ? lastError(pDB) : addErrMsg;
	switch (rc)
	{
	case SQLITE_OK:
		break;
	case SQLITE_ERROR:
		throw InvalidSQLStatementException(errMsg);
	case SQLITE_INTERNAL:
		throw InternalDBErrorException(errMsg);
	case SQLITE_PERM:
		throw DBAccessDeniedException(errMsg);
	case SQLITE_ABORT:
		throw ExecutionAbortedException(errMsg);
	case SQLITE_BUSY:
	case SQLITE_BUSY_RECOVERY:
#if defined(SQLITE_BUSY_SNAPSHOT)
	case SQLITE_BUSY_SNAPSHOT:
#endif
		throw DBLockedException(errMsg);
	case SQLITE_LOCKED:
		throw TableLockedException(errMsg);
	case SQLITE_NOMEM:
		throw NoMemoryException(errMsg);
	case SQLITE_READONLY:
		throw ReadOnlyException(errMsg);
	case SQLITE_INTERRUPT:
		throw InterruptException(errMsg);
	case SQLITE_IOERR:
		throw IOErrorException(errMsg);
	case SQLITE_CORRUPT:
		throw CorruptImageException(errMsg);
	case SQLITE_NOTFOUND:
		throw TableNotFoundException(errMsg);
	case SQLITE_FULL:
		throw DatabaseFullException(errMsg);
	case SQLITE_CANTOPEN:
		throw CantOpenDBFileException(errMsg);
	case SQLITE_PROTOCOL:
		throw LockProtocolException(errMsg);
	case SQLITE_EMPTY:
		throw InternalDBErrorException(errMsg);
	case SQLITE_SCHEMA:
		throw SchemaDiffersException(errMsg);
	case SQLITE_TOOBIG:
		throw RowTooBigException(errMsg);
	case SQLITE_CONSTRAINT:
		throw ConstraintViolationException(errMsg);
	case SQLITE_MISMATCH:
		throw DataTypeMismatchException(errMsg);
	case SQLITE_MISUSE:
		throw InvalidLibraryUseException(errMsg);
	case SQLITE_NOLFS:
		throw OSFeaturesMissingException(errMsg);
	case SQLITE_AUTH:
		throw AuthorizationDeniedException(errMsg);
	case SQLITE_FORMAT:
		throw CorruptImageException(errMsg);
	case SQLITE_NOTADB:
		throw CorruptImageException(errMsg);
	case SQLITE_RANGE:
		throw InvalidSQLStatementException(errMsg);
	case SQLITE_ROW:
		break; // sqlite_step() has another row ready
	case SQLITE_DONE:
		break; // sqlite_step() has finished executing
	default:
		throw SQLiteException(Poco::format("Unknown error code: %d", rc), errMsg);
	}
}


bool Utility::fileToMemory(sqlite3* pInMemory, const std::string& fileName)
{
	poco_check_ptr (pInMemory);

	int rc;
	sqlite3* pFile;
	sqlite3_backup* pBackup;

	// Note: SQLITE_OPEN_READWRITE is required to correctly handle an existing hot journal.
	// See #3135
	rc = sqlite3_open_v2(fileName.c_str(), &pFile, SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI, nullptr);
	if(rc == SQLITE_OK )
	{
		pBackup = sqlite3_backup_init(pInMemory, "main", pFile, "main");
		if( pBackup )
		{
			sqlite3_backup_step(pBackup, -1);
			sqlite3_backup_finish(pBackup);
		}
		// the backup reports its outcome on the destination connection
		rc = sqlite3_errcode(pInMemory);
	}

	sqlite3_close(pFile);
	return SQLITE_OK == rc;
}


bool Utility::memoryToFile(const std::string& fileName, sqlite3* pInMemory)
{
	poco_check_ptr (pInMemory);

	int rc;
	sqlite3* pFile;
	sqlite3_backup* pBackup;

	rc = sqlite3_open_v2(fileName.c_str(), &pFile, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, nullptr);
	if(rc == SQLITE_OK )
	{
		pBackup = sqlite3_backup_init(pFile, "main", pInMemory, "main");
		if( pBackup )
		{
			sqlite3_backup_step(pBackup, -1);
			sqlite3_backup_finish(pBackup);
		}
		rc = sqlite3_errcode(pFile);
	}

	sqlite3_close(pFile);
	return SQLITE_OK == rc;
}


bool Utility::isThreadSafe()
{
	return 0 != sqlite3_threadsafe();
}


int Utility::getThreadMode()
{
	return _threadMode;
}


bool Utility::setThreadMode(int mode)
{
#if (SQLITE_THREADSAFE != 0)
	if (SQLITE_OK == sqlite3_shutdown())
	{
		if (SQLITE_OK == sqlite3_config(mode))
		{
			_threadMode = mode;
			if (SQLITE_OK == sqlite3_initialize())
				return true;
		}
		sqlite3_initialize();
	}
#endif
	return false;
}


namespace
{
	constexpr int HOOK_UPDATE = 0;
	constexpr int HOOK_COMMIT = 1;
	constexpr int HOOK_ROLLBACK = 2;
	constexpr int HOOK_KIND_COUNT = 3;


	struct HookRegistry
		/// The registrations made through Utility::registerUpdateHandler(): the
		/// parameter each callback was registered with, by connection and hook
		/// kind. The mutex is held across a lookup and the sqlite3_*_hook() call
		/// that goes with it. It is taken after the mutex of the connection, never
		/// before: SQLite runs a callback with the connection mutex held, and the
		/// callback may register or remove callbacks, or close a session.
	{
		Poco::FastMutex mutex;
		std::map<std::pair<sqlite3*, int>, void*> params;
	};


	HookRegistry& hookRegistry()
	{
		// Never destroyed: a session closed during static destruction still uses it.
		alignas(HookRegistry) static unsigned char storage[sizeof(HookRegistry)];
		static HookRegistry* const pRegistry = ::new (static_cast<void*>(storage)) HookRegistry;
		return *pRegistry;
	}


	template <typename CBT, typename HookFn>
	[[nodiscard]] bool registerHook(sqlite3* pDB, int kind, CBT callbackFn, void* pParam, HookFn hookFn)
		/// Registers callbackFn with pParam or, if callbackFn is null, removes the
		/// registration made with pParam. Returns false if there is none to remove.
	{
		// nothing is registered with a null handle, so there is nothing to remove
		if (pDB == nullptr && callbackFn == nullptr) return false;
		poco_check_ptr (pDB);

		HookRegistry& registry = hookRegistry();
		const std::pair<sqlite3*, int> key(pDB, kind);
		if (callbackFn == nullptr)
		{
			// pDB may be the handle of a connection closed since: use it only if
			// the registration exists, as closing removes it first
			Poco::FastMutex::ScopedLock lock(registry.mutex);
			auto it = registry.params.find(key);
			if (it == registry.params.end() || it->second != pParam) return false;
		}

		// the connection mutex before the registry mutex (see HookRegistry)
		Utility::SQLiteMutex dbLock(pDB);
		Poco::FastMutex::ScopedLock lock(registry.mutex);
		if (callbackFn != nullptr)
		{
			registry.params[key] = pParam;
		}
		else
		{
			// it may have been replaced or removed before the connection mutex was taken
			auto it = registry.params.find(key);
			if (it == registry.params.end() || it->second != pParam) return false;
			registry.params.erase(it);
		}
		hookFn(pDB, callbackFn, callbackFn != nullptr ? pParam : nullptr);
		return true;
	}
} // namespace


bool Utility::eventHookRegister(sqlite3* pDB, UpdateCallbackType callbackFn, void* pParam)
{
	return registerHook(pDB, HOOK_UPDATE, callbackFn, pParam, &sqlite3_update_hook);
}


bool Utility::eventHookRegister(sqlite3* pDB, CommitCallbackType callbackFn, void* pParam)
{
	return registerHook(pDB, HOOK_COMMIT, callbackFn, pParam, &sqlite3_commit_hook);
}


bool Utility::eventHookRegister(sqlite3* pDB, RollbackCallbackType callbackFn, void* pParam)
{
	return registerHook(pDB, HOOK_ROLLBACK, callbackFn, pParam, &sqlite3_rollback_hook);
}


void Utility::unregisterUpdateHandlers(sqlite3* pDB)
{
	try
	{
		// the connection mutex before the registry mutex (see HookRegistry)
		SQLiteMutex dbLock(pDB);
		HookRegistry& registry = hookRegistry();
		Poco::FastMutex::ScopedLock lock(registry.mutex);
		sqlite3_update_hook(pDB, nullptr, nullptr);
		sqlite3_commit_hook(pDB, nullptr, nullptr);
		sqlite3_rollback_hook(pDB, nullptr, nullptr);
		for (int kind = HOOK_UPDATE; kind < HOOK_KIND_COUNT; ++kind)
			registry.params.erase(std::pair<sqlite3*, int>(pDB, kind));
	}
	catch (...)
	{
		poco_unexpected();
	}
}


int Utility::registeredHooks(sqlite3* pDB, const void* pParam)
{
	if (pDB == nullptr) return 0;

	HookRegistry& registry = hookRegistry();
	Poco::FastMutex::ScopedLock lock(registry.mutex);
	int hooks = 0;
	for (int kind = HOOK_UPDATE; kind < HOOK_KIND_COUNT; ++kind)
	{
		auto it = registry.params.find(std::pair<sqlite3*, int>(pDB, kind));
		if (it != registry.params.end() && it->second == pParam) hooks |= 1 << kind;
	}
	return hooks;
}


// NOTE: Utility::dbHandle() has been moved to SessionImpl.cpp,
// as a workaround for a failing AnyCast with Clang.
// See <https://github.com/pocoproject/poco/issues/578>
// for a discussion.


} // namespace Poco::Data::SQLite
