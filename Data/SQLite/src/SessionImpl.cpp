//
// SessionImpl.cpp
//
// Library: Data/SQLite
// Package: SQLite
// Module:  SessionImpl
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/SQLite/SessionImpl.h"
#include "Poco/Data/SQLite/Utility.h"
#include "Poco/Data/SQLite/SQLiteStatementImpl.h"
#include "Poco/Data/SQLite/SQLiteException.h"
#include "Poco/Data/Session.h"
#include "Poco/Stopwatch.h"
#include "Poco/String.h"
#include "Poco/Mutex.h"
#include "Poco/Data/DataException.h"
#include <sqlite3.h>
#include <cstdlib>
#ifdef POCO_ENABLE_SQLITE_VEC
// SQLITE_CORE makes sqlite-vec.h use the already-included sqlite3.h instead
// of sqlite3ext.h (not shipped with the bundled amalgamation).
#ifndef SQLITE_CORE
#define SQLITE_CORE 1
#endif
#include "sqlite-vec.h"
#endif


#ifndef SQLITE_OPEN_URI
#define SQLITE_OPEN_URI 0
#endif


#ifdef POCO_ENABLE_SQLITE_VEC
// PRAGMA table_list (3.37) is required by MemoryDB's table classification;
// below 3.44 sqlite-vec itself builds with reduced functionality.
#if SQLITE_VERSION_NUMBER < 3037000
#error "POCO_ENABLE_SQLITE_VEC requires SQLite >= 3.37.0"
#elif SQLITE_VERSION_NUMBER < 3044000
#pragma message("SQLite older than 3.44.0: sqlite-vec builds with reduced functionality")
#endif
#endif


namespace Poco::Data::SQLite {


const std::string SessionImpl::DEFERRED_BEGIN_TRANSACTION("BEGIN DEFERRED");
const std::string SessionImpl::EXCLUSIVE_BEGIN_TRANSACTION("BEGIN EXCLUSIVE");
const std::string SessionImpl::IMMEDIATE_BEGIN_TRANSACTION("BEGIN IMMEDIATE");
const std::string SessionImpl::COMMIT_TRANSACTION("COMMIT");
const std::string SessionImpl::ABORT_TRANSACTION("ROLLBACK");
const std::string SessionImpl::SQLITE_READ_UNCOMMITTED = "PRAGMA read_uncommitted = true";
const std::string SessionImpl::SQLITE_READ_COMMITTED  = "PRAGMA read_uncommitted = false";



SessionImpl::SessionImpl(const std::string& fileName, std::size_t loginTimeout):
	Poco::Data::AbstractSessionImpl<SessionImpl>(fileName, loginTimeout),
	_connector(Connector::KEY),
	_pDB(nullptr),
	_connected(false),
	_isTransaction(false),
	_transactionType(TransactionType::DEFERRED),
	_timeout(0),
	_transactionIsolationLevel(Session::TRANSACTION_READ_COMMITTED)
{
	open();
	setConnectionTimeout(loginTimeout);
	addFeature("autoCommit",
		&SessionImpl::autoCommit,
		&SessionImpl::isAutoCommit);
	addProperty("handle", &SessionImpl::setHandle, &SessionImpl::getHandle);
	addProperty("connectionTimeout", &SessionImpl::setConnectionTimeout, &SessionImpl::getConnectionTimeout);
	addProperty(Utility::TRANSACTION_TYPE_PROPERTY_KEY, &SessionImpl::setTransactionType, &SessionImpl::getTransactionType);
}


SessionImpl::~SessionImpl()
{
	try
	{
		close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void SessionImpl::setName()
{
	setDBMSName("SQLite"s);
}


Poco::Data::StatementImpl::Ptr SessionImpl::createStatementImpl()
{
	poco_check_ptr (_pDB);
	return new SQLiteStatementImpl(*this, _pDB);
}


void SessionImpl::begin()
{
	Poco::Mutex::ScopedLock l(_mutex);
	SQLiteStatementImpl tmp(*this, _pDB);
	switch (_transactionType)
	{
	case TransactionType::DEFERRED:
		tmp.add(DEFERRED_BEGIN_TRANSACTION);
		break;
	case TransactionType::EXCLUSIVE:
		tmp.add(EXCLUSIVE_BEGIN_TRANSACTION);
		break;
	case TransactionType::IMMEDIATE:
		tmp.add(IMMEDIATE_BEGIN_TRANSACTION);
		break;
	}
	tmp.execute();
	_isTransaction = true;
}


void SessionImpl::commit()
{
	Poco::Mutex::ScopedLock l(_mutex);
	SQLiteStatementImpl tmp(*this, _pDB);
	tmp.add(COMMIT_TRANSACTION);
	tmp.execute();
	_isTransaction = false;
}


void SessionImpl::rollback()
{
	Poco::Mutex::ScopedLock l(_mutex);
	SQLiteStatementImpl tmp(*this, _pDB);
	tmp.add(ABORT_TRANSACTION);
	tmp.execute();
	_isTransaction = false;
}


void SessionImpl::setTransactionIsolation(Poco::UInt32 ti)
{
	Poco::Mutex::ScopedLock l(_mutex);
	SQLiteStatementImpl tmp(*this, _pDB);
	switch (ti)
	{
	case Session::TRANSACTION_READ_COMMITTED:
		tmp.add(SQLITE_READ_COMMITTED);
		_transactionIsolationLevel = Session::TRANSACTION_READ_COMMITTED;
		break;
	case Session::TRANSACTION_READ_UNCOMMITTED:
		tmp.add(SQLITE_READ_UNCOMMITTED);
		_transactionIsolationLevel = Session::TRANSACTION_READ_UNCOMMITTED;
		break;
	case Session::TRANSACTION_REPEATABLE_READ:
		throw Poco::InvalidArgumentException("setTransactionIsolation(TRANSACTION_REPEATABLE_READ) - unsupported");
	case Session::TRANSACTION_SERIALIZABLE:
		throw Poco::InvalidArgumentException("setTransactionIsolation(TRANSACTION_SERIALIZABLE) - unsupported [SQLite transactions are serializable by design]");
	default:
		throw Poco::InvalidArgumentException(Poco::format("setTransactionIsolation(%u) - unsupported", ti));
		break;
	}
	tmp.execute();
}


Poco::UInt32 SessionImpl::getTransactionIsolation() const
{
	return _transactionIsolationLevel;
}


bool SessionImpl::hasTransactionIsolation(Poco::UInt32 ti) const
{
	switch (ti)
	{
	case Session::TRANSACTION_READ_COMMITTED:
		return true;
	case Session::TRANSACTION_READ_UNCOMMITTED:
		return true;
	case Session::TRANSACTION_REPEATABLE_READ:
		return false;
	case Session::TRANSACTION_SERIALIZABLE:
		return false;
	}

	return false;
}


bool SessionImpl::isTransactionIsolation(Poco::UInt32 ti) const
{
	return getTransactionIsolation() == ti;
}


void SessionImpl::open(const std::string& connect)
{
	Poco::Mutex::ScopedLock l(_mutex);

	if (isConnected())
		throw InvalidAccessException("Session already connected");

	if (connect != connectionString() && !connect.empty())
		setConnectionString(connect);

	poco_assert_dbg (!connectionString().empty());

#ifdef POCO_ENABLE_SQLITE_VEC
	// Registered before every open, not once: Utility::setThreadMode() calls
	// sqlite3_shutdown(), which clears the auto-extension list. The call is
	// idempotent and serialized inside SQLite. A failure here (SQLITE_NOMEM)
	// must fail the open: silently returning a session without vec would
	// violate the enabled-build contract.
	int rcVec = sqlite3_auto_extension(reinterpret_cast<void(*)(void)>(sqlite3_vec_init));
	if (rcVec != SQLITE_OK)
		throw ConnectionFailedException(std::string("cannot register sqlite-vec extension: ")
			+ sqlite3_errstr(rcVec));
#endif

	// isAutoCommit() uses the handle without a lock once it is published, so it is
	// published only once the connection is open
	sqlite3* pDB = nullptr;
	try
	{
		int rc = 0;
		std::size_t tout = getLoginTimeout();
		Stopwatch sw; sw.start();
		while (true)
		{
			rc = sqlite3_open_v2(connectionString().c_str(), &pDB,
				SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, nullptr);
			if (rc == SQLITE_OK) break;
			if (pDB == nullptr)
				throw ConnectionFailedException(std::string(sqlite3_errstr(rc)));
			if (static_cast<std::size_t>(sw.elapsedSeconds()) >= tout)
			{
				Utility::throwException(pDB, rc);
			}
			Thread::sleep(10);
			sqlite3_close_v2(pDB);
			pDB = nullptr;
		}
		// a connection opened again keeps the busy timeout the session was given
		rc = sqlite3_busy_timeout(pDB, _timeout);
		if (rc != 0) Utility::throwException(pDB, rc);
	}
	catch (SQLiteException& ex)
	{
		sqlite3_close_v2(pDB);
		throw ConnectionFailedException(ex.displayText());
	}

	Poco::FastMutex::ScopedLock h(_handleMutex);
	_pDB = pDB;
	_connected = true;
	_isTransaction = false;
	setProperty("handle", _pDB);
}


void SessionImpl::close()
{
	Poco::Mutex::ScopedLock l(_mutex);

	sqlite3* pDB = nullptr;
	{
		Poco::FastMutex::ScopedLock h(_handleMutex);
		pDB = _pDB;
		_pDB = nullptr;
		_connected = false;
		_isTransaction = false;
		setProperty("handle", _pDB);
		while (_handleUsers > 0) _handleReleased.wait(_handleMutex);
	}

	if (pDB != nullptr)
	{
		Utility::unregisterUpdateHandlers(pDB);
		// sqlite3_close_v2() rolls back only once the last statement compiled on the
		// connection is finalized, and the transaction keeps its locks until then
		if (sqlite3_get_autocommit(pDB) == 0)
			sqlite3_exec(pDB, ABORT_TRANSACTION.c_str(), nullptr, nullptr, nullptr);
		sqlite3_close_v2(pDB);
	}
}


void SessionImpl::reset()
{
	Poco::Mutex::ScopedLock l(_mutex);

	if (_pDB != nullptr) Utility::unregisterUpdateHandlers(_pDB);
}


bool SessionImpl::isConnected() const
{
	return _connected;
}


void SessionImpl::setConnectionTimeout(std::size_t timeout)
{
	Poco::Mutex::ScopedLock l(_mutex);
	poco_check_ptr (_pDB);
	int tout = static_cast<int>(1000 * timeout);
	int rc = sqlite3_busy_timeout(_pDB, tout);
	if (rc != 0) Utility::throwException(_pDB, rc);
	_timeout = tout;
}


void SessionImpl::setConnectionTimeout(const std::string& prop, const Poco::Any& value)
{
	// an int is what the getter returns, and what a SessionPool sets back
	if (value.type() == typeid(int))
		setConnectionTimeout(static_cast<std::size_t>(Poco::AnyCast<int>(value)));
	else
		setConnectionTimeout(Poco::RefAnyCast<std::size_t>(value));
}


Poco::Any SessionImpl::getConnectionTimeout(const std::string& prop) const
{
	return Poco::Any(_timeout/1000);
}

void SessionImpl::setTransactionType(TransactionType transactionType)
{
	_transactionType = transactionType;
}

void SessionImpl::setTransactionType(const std::string &prop, const Poco::Any& value)
{
	setTransactionType(Poco::RefAnyCast<TransactionType>(value));
}

Poco::Any SessionImpl::getTransactionType(const std::string& prop) const
{
	return Poco::Any(_transactionType);
}

void SessionImpl::autoCommit(const std::string&, bool)
{
}


bool SessionImpl::isAutoCommit(const std::string&) const
{
	sqlite3* pDB = nullptr;
	{
		Poco::FastMutex::ScopedLock h(_handleMutex);
		if (_pDB == nullptr) return true;
		pDB = _pDB;
		++_handleUsers;
	}
	// sqlite3_get_autocommit() locks the connection, which SQLite holds while it runs
	// a callback that may call this function: no session lock may be held meanwhile
	const bool autoCommit = (0 != sqlite3_get_autocommit(pDB));
	Poco::FastMutex::ScopedLock h(_handleMutex);
	if (--_handleUsers == 0) _handleReleased.broadcast();
	return autoCommit;
}


Poco::Any SessionImpl::getHandle(const std::string& name) const
{
	Poco::FastMutex::ScopedLock h(_handleMutex);
	return Poco::Data::AbstractSessionImpl<SessionImpl>::getHandle(name);
}


sqlite3* SessionImpl::handle() const
{
	Poco::FastMutex::ScopedLock h(_handleMutex);
	return _pDB;
}


// NOTE: Utility::dbHandle() has been moved here from Utility.cpp
// as a workaround for a failing AnyCast with Clang.
// See <https://github.com/pocoproject/poco/issues/578>
// for a discussion.
sqlite3* Utility::dbHandle(const Session& session)
{
	return AnyCast<sqlite3*>(session.getProperty("handle"));
}


} // namespace Poco::Data::SQLite
