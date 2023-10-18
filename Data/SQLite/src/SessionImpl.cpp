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
#if defined(POCO_UNBUNDLED)
#include <sqlite3.h>
#else
#include "sqlite3.h"
#endif
#include <cstdlib>


#ifndef SQLITE_OPEN_URI
#define SQLITE_OPEN_URI 0
#endif


namespace Poco {
namespace Data {
namespace SQLite {


const std::string SessionImpl::DEFERRED_BEGIN_TRANSACTION("BEGIN DEFERRED");
const std::string SessionImpl::EXCLUSIVE_BEGIN_TRANSACTION("BEGIN EXCLUSIVE");
const std::string SessionImpl::IMMEDIATE_BEGIN_TRANSACTION("BEGIN IMMEDIATE");
const std::string SessionImpl::COMMIT_TRANSACTION("COMMIT");
const std::string SessionImpl::ABORT_TRANSACTION("ROLLBACK");


SessionImpl::SessionImpl(const std::string& fileName, std::size_t loginTimeout):
	Poco::Data::AbstractSessionImpl<SessionImpl>(fileName, loginTimeout),
	_connector(Connector::KEY),
	_pDB(0),
	_connected(false),
	_isTransaction(false),
	_transactionType(TransactionType::DEFERRED)
{
	open();
	setConnectionTimeout(loginTimeout);
	setProperty("handle", _pDB);
	addFeature("autoCommit",
		&SessionImpl::autoCommit,
		&SessionImpl::isAutoCommit);
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
	if (ti != Session::TRANSACTION_READ_COMMITTED)
		throw Poco::InvalidArgumentException("setTransactionIsolation()");
}


Poco::UInt32 SessionImpl::getTransactionIsolation() const
{
	return Session::TRANSACTION_READ_COMMITTED;
}


bool SessionImpl::hasTransactionIsolation(Poco::UInt32 ti) const
{
	if (ti == Session::TRANSACTION_READ_COMMITTED) return true;
	return false;
}


bool SessionImpl::isTransactionIsolation(Poco::UInt32 ti) const
{
	if (ti == Session::TRANSACTION_READ_COMMITTED) return true;
	return false;
}


void SessionImpl::open(const std::string& connect)
{
	if (connect != connectionString())
	{
		if (isConnected())
			throw InvalidAccessException("Session already connected");

		if (!connect.empty())
			setConnectionString(connect);
	}

	poco_assert_dbg (!connectionString().empty());

	try
	{
		int rc = 0;
		size_t tout = getLoginTimeout();
		Stopwatch sw; sw.start();
		while (true)
		{
			rc = sqlite3_open_v2(connectionString().c_str(), &_pDB,
				SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, NULL);
			if (rc == SQLITE_OK) break;
			if (!_pDB)
				throw ConnectionFailedException(std::string(sqlite3_errstr(rc)));
			if (sw.elapsedSeconds() >= tout)
			{
				Utility::throwException(_pDB, rc);
			}
			Thread::sleep(10);
			close();
		}
	}
	catch (SQLiteException& ex)
	{
		close();
		throw ConnectionFailedException(ex.displayText());
	}

	_connected = true;
}


void SessionImpl::close()
{
	if (_pDB)
	{
		sqlite3_close_v2(_pDB);
		_pDB = 0;
	}

	_connected = false;
}


void SessionImpl::reset()
{

}


bool SessionImpl::isConnected() const
{
	return _connected;
}


void SessionImpl::setConnectionTimeout(std::size_t timeout)
{
	int tout = static_cast<int>(1000 * timeout);
	int rc = sqlite3_busy_timeout(_pDB, tout);
	if (rc != 0) Utility::throwException(_pDB, rc);
	_timeout = tout;
}


void SessionImpl::setConnectionTimeout(const std::string& prop, const Poco::Any& value)
{
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
	Poco::Mutex::ScopedLock l(_mutex);
	return (0 != sqlite3_get_autocommit(_pDB));
}


// NOTE: Utility::dbHandle() has been moved here from Utility.cpp
// as a workaround for a failing AnyCast with Clang.
// See <https://github.com/pocoproject/poco/issues/578>
// for a discussion.
sqlite3* Utility::dbHandle(const Session& session)
{
	return AnyCast<sqlite3*>(session.getProperty("handle"));
}


} } } // namespace Poco::Data::SQLite
