//
// SessionImpl.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  SessionImpl
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/ODBC/SessionImpl.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/ODBCStatementImpl.h"
#include "Poco/Data/ODBC/Error.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/Session.h"
#include "Poco/String.h"
#include <sqlext.h>


namespace Poco {
namespace Data {
namespace ODBC {


SessionImpl::SessionImpl(const std::string& connect,
	std::size_t loginTimeout,
	std::size_t maxFieldSize,
	bool autoBind,
	bool autoExtract):
	Poco::Data::AbstractSessionImpl<SessionImpl>(connect, loginTimeout),
		_connector(Connector::KEY),
		_maxFieldSize(maxFieldSize),
		_autoBind(autoBind),
		_autoExtract(autoExtract),
		_canTransact(ODBC_TXN_CAPABILITY_UNKNOWN),
		_inTransaction(false),
		_queryTimeout(-1),
		_dbEncoding("UTF-8")
{
	addFeatures();
	setFeature("bulk", true);
	setFeature("multiActiveResultset", true);

	// this option is obsolete; here only to support older drivers, should be changed to ODBC_CURSOR_USE_NEVER
	// https://github.com/MicrosoftDocs/sql-docs/blob/live/docs/odbc/reference/appendixes/using-the-odbc-cursor-library.md
	setCursorUse("", ODBC_CURSOR_USE_IF_NEEDED);

	open();
}


SessionImpl::SessionImpl(const std::string& connect,
	Poco::Any maxFieldSize,
	bool enforceCapability,
	bool autoBind,
	bool autoExtract): Poco::Data::AbstractSessionImpl<SessionImpl>(connect),
		_connector(Connector::KEY),
		_maxFieldSize(maxFieldSize),
		_autoBind(autoBind),
		_autoExtract(autoExtract),
		_canTransact(ODBC_TXN_CAPABILITY_UNKNOWN),
		_inTransaction(false),
		_queryTimeout(-1),
		_dbEncoding("UTF-8")
{
	addFeatures();
	setFeature("bulk", true);
	setFeature("multiActiveResultset", true);

	// this option is obsolete; here only to support older drivers, should be changed to ODBC_CURSOR_USE_NEVER
	// https://github.com/MicrosoftDocs/sql-docs/blob/live/docs/odbc/reference/appendixes/using-the-odbc-cursor-library.md
	setCursorUse("", ODBC_CURSOR_USE_IF_NEEDED);

	open();
}


SessionImpl::~SessionImpl()
{
	try
	{
		if (isTransaction() && !getFeature("autoCommit"))
		{
			try { rollback(); }
			catch (...) { }
		}

		close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


Poco::Data::StatementImpl::Ptr SessionImpl::createStatementImpl()
{
	return new ODBCStatementImpl(*this);
}


void SessionImpl::addFeatures()
{
	addFeature("autoCommit",
		&SessionImpl::autoCommit,
		&SessionImpl::isAutoCommit);

	addFeature("autoBind",
		&SessionImpl::autoBind,
		&SessionImpl::isAutoBind);

	addFeature("autoExtract",
		&SessionImpl::autoExtract,
		&SessionImpl::isAutoExtract);

	addProperty("maxFieldSize",
		&SessionImpl::setMaxFieldSize,
		&SessionImpl::getMaxFieldSize);

	addProperty("loginTimeout",
		&SessionImpl::setLoginTimeout,
		&SessionImpl::getLoginTimeout);

	addProperty("queryTimeout",
		&SessionImpl::setQueryTimeout,
		&SessionImpl::getQueryTimeout);

	addProperty("dbEncoding",
		&SessionImpl::setDBEncoding,
		&SessionImpl::getDBEncoding);

	// SQL Server supports multiple active resultsets
	// currently has no effect on other back ends
	addFeature("multiActiveResultset",
		&SessionImpl::setMultiActiveResultset,
		&SessionImpl::getMultiActiveResultset);
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

	if (connectionString().empty())
		throw InvalidArgumentException("SessionImpl::open(): Connection string empty");

	if (_db.connect(connectionString()))
	{
		setProperty("handle", _db.handle());

		_dataTypes.fillTypeInfo(_db.pHandle());
			addProperty("dataTypeInfo",
			&SessionImpl::setDataTypeInfo,
			&SessionImpl::dataTypeInfo);

		Poco::Data::ODBC::SQLSetConnectAttr(_db, SQL_ATTR_QUIET_MODE, 0, 0);

		if (!canTransact()) autoCommit("", true);
	}
	else
		throw ConnectionException(SQL_NULL_HDBC,
			Poco::format("Connection to '%s' failed.", connectionString()));
}


void SessionImpl::setDBEncoding(const std::string&, const Poco::Any& value)
{
	const std::string& enc = Poco::RefAnyCast<std::string>(value);
	Poco::TextEncoding::byName(enc); // throws if not found
	_dbEncoding = enc;
}


bool SessionImpl::isConnected() const
{
	return _db.isConnected();
}


inline void SessionImpl::setCursorUse(const std::string&, const Poco::Any& value)
{
#if POCO_OS == POCO_OS_WINDOWS_NT
#pragma warning (disable : 4995) // ignore marked as deprecated
#endif
	int cursorUse = static_cast<int>(Poco::AnyCast<CursorUse>(value));
	int rc = 0;
	switch (cursorUse)
	{
	case ODBC_CURSOR_USE_ALWAYS:
		rc = Poco::Data::ODBC::SQLSetConnectAttr(_db, SQL_ATTR_ODBC_CURSORS, (SQLPOINTER)SQL_CUR_USE_ODBC, SQL_IS_INTEGER);
		break;
	case ODBC_CURSOR_USE_IF_NEEDED:
		rc = Poco::Data::ODBC::SQLSetConnectAttr(_db, SQL_ATTR_ODBC_CURSORS, (SQLPOINTER)SQL_CUR_USE_IF_NEEDED, SQL_IS_INTEGER);
		break;
	case ODBC_CURSOR_USE_NEVER:
		rc = Poco::Data::ODBC::SQLSetConnectAttr(_db, SQL_ATTR_ODBC_CURSORS, (SQLPOINTER)SQL_CUR_USE_DRIVER, SQL_IS_INTEGER);
		break;
	default:
		throw Poco::InvalidArgumentException(Poco::format("SessionImpl::setCursorUse(%d)", cursorUse));
	}
#if POCO_OS == POCO_OS_WINDOWS_NT
#pragma warning (default : 4995)
#endif
	if (Utility::isError(rc))
	{
		throw Poco::Data::ODBC::HandleException<SQLHDBC, SQL_HANDLE_DBC>(_db, Poco::format("SessionImpl::setCursorUse(%d)", cursorUse));
	}
}


inline Poco::Any SessionImpl::getCursorUse(const std::string&) const
{
#if POCO_OS == POCO_OS_WINDOWS_NT
#pragma warning (disable : 4995) // ignore marked as deprecated
#endif
	SQLUINTEGER curUse = 0;
	Poco::Data::ODBC::SQLGetConnectAttr(_db, SQL_ATTR_ODBC_CURSORS, &curUse, SQL_IS_UINTEGER, 0);
	switch (curUse)
	{
	case SQL_CUR_USE_ODBC:
		return ODBC_CURSOR_USE_ALWAYS;
	case SQL_CUR_USE_IF_NEEDED:
		return ODBC_CURSOR_USE_IF_NEEDED;
	case SQL_CUR_USE_DRIVER:
		return ODBC_CURSOR_USE_NEVER;
	default: break;
	}
	throw Poco::InvalidArgumentException(Poco::format("SessionImpl::getCursorUse(%u)", curUse));
#if POCO_OS == POCO_OS_WINDOWS_NT
#pragma warning (default : 4995)
#endif
}


void SessionImpl::setConnectionTimeout(std::size_t timeout)
{
	SQLULEN value = static_cast<SQLUINTEGER>(timeout);
	_db.setTimeout(static_cast<int>(value));
}


std::size_t SessionImpl::getConnectionTimeout() const
{
	return _db.getTimeout();
}


void SessionImpl::setLoginTimeout(const std::string&, const Poco::Any& value)
{
	int timeout = 0;
	try
	{
		timeout = Poco::AnyCast<int>(value);
	}
	catch(const Poco::BadCastException&)
	{
		timeout = Poco::AnyCast<unsigned int>(value);
	}

	_db.setLoginTimeout(timeout);
}


bool SessionImpl::canTransact() const
{
	if (ODBC_TXN_CAPABILITY_UNKNOWN == _canTransact)
	{
		SQLUSMALLINT ret;
		checkError(Poco::Data::ODBC::SQLGetInfo(_db, SQL_TXN_CAPABLE, &ret, 0, 0),
			"Failed to obtain transaction capability info.");

		_canTransact = (SQL_TC_NONE != ret) ?
			ODBC_TXN_CAPABILITY_TRUE :
			ODBC_TXN_CAPABILITY_FALSE;
	}

	return ODBC_TXN_CAPABILITY_TRUE == _canTransact;
}


void SessionImpl::setTransactionIsolation(Poco::UInt32 ti)
{
	setTransactionIsolationImpl(ti);
}


void SessionImpl::setTransactionIsolationImpl(Poco::UInt32 ti) const
{
#if POCO_PTR_IS_64_BIT
	Poco::UInt64 isolation = 0;
#else
	Poco::UInt32 isolation = 0;
#endif

	if (ti & Session::TRANSACTION_READ_UNCOMMITTED)
		isolation |= SQL_TXN_READ_UNCOMMITTED;

	if (ti & Session::TRANSACTION_READ_COMMITTED)
		isolation |= SQL_TXN_READ_COMMITTED;

	if (ti & Session::TRANSACTION_REPEATABLE_READ)
		isolation |= SQL_TXN_REPEATABLE_READ;

	if (ti & Session::TRANSACTION_SERIALIZABLE)
		isolation |= SQL_TXN_SERIALIZABLE;

	checkError(SQLSetConnectAttr(_db, SQL_ATTR_TXN_ISOLATION, (SQLPOINTER) isolation, 0));
}


void SessionImpl::setMultiActiveResultset(const std::string&, bool val)
{
#ifdef POCO_DATA_ODBC_HAVE_SQL_SERVER_EXT
	int enabled = val ? SQL_MARS_ENABLED_YES : SQL_MARS_ENABLED_NO;
	checkError(Poco::Data::ODBC::SQLSetConnectAttr(_db, SQL_COPT_SS_MARS_ENABLED, &enabled, SQL_IS_INTEGER));
#endif
}


bool SessionImpl::getMultiActiveResultset(const std::string&) const
{
#ifdef POCO_DATA_ODBC_HAVE_SQL_SERVER_EXT
	SQLINTEGER mars;
	Poco::Data::ODBC::SQLGetConnectAttr(_db, SQL_COPT_SS_MARS_ENABLED, &mars, SQL_IS_INTEGER, 0);
	return mars == SQL_MARS_ENABLED_YES;
#else
	return false;
#endif
}


Poco::UInt32 SessionImpl::getTransactionIsolation() const
{
	SQLULEN isolation = 0;
	checkError(SQLGetConnectAttr(_db, SQL_ATTR_TXN_ISOLATION,
		&isolation,
		0,
		0));

	return transactionIsolation(isolation);
}


bool SessionImpl::hasTransactionIsolation(Poco::UInt32 ti) const
{
	if (isTransaction()) throw InvalidAccessException();

	Poco::UInt32 old = getTransactionIsolation();
	if (old == ti) return true;

	bool retval = true;
	try { setTransactionIsolationImpl(ti); }
	catch (Poco::Exception&) { retval = false; }
	setTransactionIsolationImpl(old);
	return retval;
}


Poco::UInt32 SessionImpl::getDefaultTransactionIsolation() const
{
	SQLUINTEGER isolation = 0;
	checkError(SQLGetInfo(_db, SQL_DEFAULT_TXN_ISOLATION,
		&isolation,
		0,
		0));

	return transactionIsolation(isolation);
}


Poco::UInt32 SessionImpl::transactionIsolation(SQLULEN isolation)
{
	if (0 == isolation)
		throw InvalidArgumentException("transactionIsolation(0): invalid isolation 0");

	Poco::UInt32 ret = 0;

	if (isolation & SQL_TXN_READ_UNCOMMITTED)
		ret |= Session::TRANSACTION_READ_UNCOMMITTED;

	if (isolation & SQL_TXN_READ_COMMITTED)
		ret |= Session::TRANSACTION_READ_COMMITTED;

	if (isolation & SQL_TXN_REPEATABLE_READ)
		ret |= Session::TRANSACTION_REPEATABLE_READ;

	if (isolation & SQL_TXN_SERIALIZABLE)
		ret |= Session::TRANSACTION_SERIALIZABLE;

	if (0 == ret)
		throw InvalidArgumentException(Poco::format("transactionIsolation(%u)", isolation));

	return ret;
}


void SessionImpl::autoCommit(const std::string&, bool val)
{
	if (val == isAutoCommit()) return;
	if (val && isTransaction())
	{
		throw InvalidAccessException("autoCommit not "
			"allowed for session in transaction");
	}

	checkError(Poco::Data::ODBC::SQLSetConnectAttr(_db,
		SQL_ATTR_AUTOCOMMIT,
		val ? (SQLPOINTER) SQL_AUTOCOMMIT_ON :
			(SQLPOINTER) SQL_AUTOCOMMIT_OFF,
		SQL_IS_UINTEGER), "Failed to set autocommit.");
}


bool SessionImpl::isAutoCommit(const std::string&) const
{
	if (!_db) return false;

	SQLULEN value = 0;

	checkError(Poco::Data::ODBC::SQLGetConnectAttr(_db,
		SQL_ATTR_AUTOCOMMIT,
		&value,
		0,
		0));

	return (0 != value);
}


bool SessionImpl::isTransaction() const
{
	if (!_db || !canTransact()) return false;

	SQLULEN value = 0;
	checkError(Poco::Data::ODBC::SQLGetConnectAttr(_db,
		SQL_ATTR_AUTOCOMMIT,
		&value,
		0,
		0));

	if (0 == value) return _inTransaction;
	else return false;
}


void SessionImpl::begin()
{
	if (isAutoCommit())
		throw InvalidAccessException("Session in auto commit mode.");

	{
		Poco::FastMutex::ScopedLock l(_mutex);

		if (_inTransaction)
			throw InvalidAccessException("Transaction in progress.");

		_inTransaction = true;
	}
}


void SessionImpl::commit()
{
	if (!isAutoCommit())
		checkError(SQLEndTran(SQL_HANDLE_DBC, _db, SQL_COMMIT));

	_inTransaction = false;
}


void SessionImpl::rollback()
{
	if (!isAutoCommit())
		checkError(SQLEndTran(SQL_HANDLE_DBC, _db, SQL_ROLLBACK));

	_inTransaction = false;
}


void SessionImpl::reset()
{

}


void SessionImpl::close()
{
	if (!isConnected()) return;

	try
	{
		commit();
	}
	catch (ConnectionException&)
	{
	}

	_db.disconnect();
	setProperty("handle", SQL_NULL_HDBC);
}


int SessionImpl::maxStatementLength() const
{
	SQLUINTEGER info;
	SQLRETURN rc = 0;
	if (Utility::isError(rc = Poco::Data::ODBC::SQLGetInfo(_db,
		SQL_MAXIMUM_STATEMENT_LENGTH,
		(SQLPOINTER) &info,
		0,
		0)))
	{
		throw ConnectionException(_db,
			"SQLGetInfo(SQL_MAXIMUM_STATEMENT_LENGTH)");
	}

	return info;
}


} } } // namespace Poco::Data::ODBC
