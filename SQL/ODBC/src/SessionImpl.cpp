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


#include "Poco/SQL/ODBC/SessionImpl.h"
#include "Poco/SQL/ODBC/Utility.h"
#include "Poco/SQL/ODBC/ODBCStatementImpl.h"
#include "Poco/SQL/ODBC/Error.h"
#include "Poco/SQL/ODBC/ODBCException.h"
#include "Poco/SQL/Session.h"
#include "Poco/String.h"
#include <sqlext.h>


namespace Poco {
namespace SQL {
namespace ODBC {


SessionImpl::SessionImpl(const std::string& connect,
	std::size_t loginTimeout,
	std::size_t maxFieldSize,
	bool autoBind,
	bool autoExtract,
	bool makeExtractorsBeforeExecute):
	Poco::SQL::AbstractSessionImpl<SessionImpl>(connect, loginTimeout),
		_connector(Connector::KEY),
		_maxFieldSize(maxFieldSize),
		_autoBind(autoBind),
		_autoExtract(autoExtract),
		_makeExtractorsBeforeExecute(makeExtractorsBeforeExecute),
		_canTransact(ODBC_TXN_CAPABILITY_UNKNOWN),
		_inTransaction(false),
		_queryTimeout(-1)
{
	init();
}


SessionImpl::SessionImpl(const std::string& connect,
	Poco::Any maxFieldSize,
	bool /*enforceCapability*/,
	bool autoBind,
	bool autoExtract,
	bool makeExtractorsBeforeExecute): Poco::SQL::AbstractSessionImpl<SessionImpl>(connect),
		_connector(Connector::KEY),
		_maxFieldSize(maxFieldSize),
		_autoBind(autoBind),
		_autoExtract(autoExtract),
		_makeExtractorsBeforeExecute(makeExtractorsBeforeExecute),
		_canTransact(ODBC_TXN_CAPABILITY_UNKNOWN),
		_inTransaction(false),
		_queryTimeout(-1)
{
	init();
}


void SessionImpl::init()
{
	setFeature("bulk", true);
	open();
	setProperty("handle", _db.handle());
	setProperty("handle", _db.handle());
}


SessionImpl::~SessionImpl()
{
	try
	{
		if (static_cast<bool>(_db) && isTransaction() && !getFeature("autoCommit"))
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


Poco::SQL::StatementImpl::Ptr SessionImpl::createStatementImpl()
{
	return new ODBCStatementImpl(*this);
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

	SQLULEN tout = static_cast<SQLULEN>(getLoginTimeout());
	if (Utility::isError(Poco::SQL::ODBC::SQLSetConnectAttr(_db, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER)tout, 0)))
	{
		if (Utility::isError(Poco::SQL::ODBC::SQLGetConnectAttr(_db, SQL_ATTR_LOGIN_TIMEOUT, &tout, 0, 0)) ||
				getLoginTimeout() != tout)
		{
			ConnectionException e(_db);
			throw ConnectionFailedException(e.errorString(), e);
		}
	}

	SQLCHAR connectOutput[512] = {0};
	SQLSMALLINT result;

	if (Utility::isError(Poco::SQL::ODBC::SQLDriverConnect(_db
		, NULL
		,(SQLCHAR*) connectionString().c_str()
		,(SQLSMALLINT) SQL_NTS
		, connectOutput
		, sizeof(connectOutput)
		, &result
		, SQL_DRIVER_NOPROMPT)))
	{
		ConnectionException e(_db);
		close();
		throw ConnectionFailedException(e.errorString(), e);
	}

	_dataTypes.fillTypeInfo(_db);
	
	addProperty("dataTypeInfo",
		&SessionImpl::setDataTypeInfo,
		&SessionImpl::dataTypeInfo);

	addFeature("autoCommit",
		&SessionImpl::autoCommit,
		&SessionImpl::isAutoCommit);

	addFeature("autoBind",
		&SessionImpl::autoBind,
		&SessionImpl::isAutoBind);

	addFeature("autoExtract",
		&SessionImpl::autoExtract,
		&SessionImpl::isAutoExtract);

	addFeature("makeExtractorsBeforeExecute", 
		&SessionImpl::makeExtractorsBeforeExecute, 
		&SessionImpl::isMakeExtractorsBeforeExecute);

	addProperty("maxFieldSize",
		&SessionImpl::setMaxFieldSize,
		&SessionImpl::getMaxFieldSize);

	addProperty("queryTimeout",
		&SessionImpl::setQueryTimeout,
		&SessionImpl::getQueryTimeout);

	Poco::SQL::ODBC::SQLSetConnectAttr(_db, SQL_ATTR_QUIET_MODE, 0, 0);

	if (!canTransact()) autoCommit("", true);
}


bool SessionImpl::isConnected() const
{
	SQLULEN value = 0;

	if (!static_cast<bool>(_db) || Utility::isError(Poco::SQL::ODBC::SQLGetConnectAttr(_db,
		SQL_ATTR_CONNECTION_DEAD,
		&value,
		0,
		0))) return false;

	return (SQL_CD_FALSE == value);
}


void SessionImpl::setConnectionTimeout(std::size_t timeout)
{
	SQLUINTEGER value = static_cast<SQLUINTEGER>(timeout);

	checkError(Poco::SQL::ODBC::SQLSetConnectAttr(_db,
		SQL_ATTR_CONNECTION_TIMEOUT,
		&value,
		SQL_IS_UINTEGER), "Failed to set connection timeout.");
}


std::size_t SessionImpl::getConnectionTimeout() const
{
	SQLULEN value = 0;

	checkError(Poco::SQL::ODBC::SQLGetConnectAttr(_db,
		SQL_ATTR_CONNECTION_TIMEOUT,
		&value,
		0,
		0), "Failed to get connection timeout.");

	return value;
}


bool SessionImpl::canTransact() const
{
	if (ODBC_TXN_CAPABILITY_UNKNOWN == _canTransact)
	{
		SQLUSMALLINT ret;
		SQLRETURN res = Poco::SQL::ODBC::SQLGetInfo(_db, SQL_TXN_CAPABLE, &ret, 0, 0);
		if (!Utility::isError(res))
		{
			_canTransact = static_cast<char>((SQL_TC_NONE != ret) ? ODBC_TXN_CAPABILITY_TRUE : ODBC_TXN_CAPABILITY_FALSE);
		}
		else
		{
			Error<SQLHDBC, SQL_HANDLE_DBC> err(_db);
			_canTransact = ODBC_TXN_CAPABILITY_FALSE;
		}
	}

	return ODBC_TXN_CAPABILITY_TRUE == _canTransact;
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

	checkError(Poco::SQL::ODBC::SQLSetConnectAttr(_db, SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)isolation, 0));
}


Poco::UInt32 SessionImpl::getTransactionIsolation() const
{
	SQLULEN isolation = 0;
	checkError(Poco::SQL::ODBC::SQLGetConnectAttr(_db, SQL_ATTR_TXN_ISOLATION,
		&isolation,
		0,
		0));

	return transactionIsolation(isolation);
}


bool SessionImpl::hasTransactionIsolation(Poco::UInt32 ti) const
{
	if (isTransaction()) throw InvalidAccessException();

	bool retval = true;
	Poco::UInt32 old = getTransactionIsolation();
	try { setTransactionIsolationImpl(ti); }
	catch (Poco::Exception&) { retval = false; }
	setTransactionIsolationImpl(old);
	return retval;
}


Poco::UInt32 SessionImpl::getDefaultTransactionIsolation() const
{
	SQLUINTEGER isolation = 0;
	checkError(Poco::SQL::ODBC::SQLGetInfo(_db, SQL_DEFAULT_TXN_ISOLATION,
		&isolation,
		0,
		0));

	return transactionIsolation(isolation);
}


Poco::UInt32 SessionImpl::transactionIsolation(SQLULEN isolation) const
{
	if (0 == isolation)
		throw InvalidArgumentException("transactionIsolation(SQLUINTEGER)");

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
		throw InvalidArgumentException("transactionIsolation(SQLUINTEGER)");

	return ret;
}


void SessionImpl::autoCommit(const std::string&, bool val)
{
	checkError(Poco::SQL::ODBC::SQLSetConnectAttr(_db,
		SQL_ATTR_AUTOCOMMIT,
		val ? (SQLPOINTER) SQL_AUTOCOMMIT_ON :
			(SQLPOINTER) SQL_AUTOCOMMIT_OFF,
		SQL_IS_UINTEGER), "Failed to set automatic commit.");
}


bool SessionImpl::isAutoCommit(const std::string&) const
{
	SQLULEN value = 0;

	checkError(Poco::SQL::ODBC::SQLGetConnectAttr(_db,
		SQL_ATTR_AUTOCOMMIT,
		&value,
		0,
		0));

	return (0 != value);
}


bool SessionImpl::isTransaction() const
{
	if (!canTransact()) return false;

	SQLULEN value = 0;
	checkError(Poco::SQL::ODBC::SQLGetConnectAttr(_db,
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

	SQLCHAR sqlState[5+1];
	SQLCHAR sqlErrorMessage[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER nativeErrorCode;
	SQLSMALLINT sqlErrorMessageLength;
	unsigned int retryCount = 10;
	do
	{
		SQLRETURN rc = SQLDisconnect(_db);
		if (SQL_SUCCESS != rc && SQL_SUCCESS_WITH_INFO != rc)
		{
			SQLGetDiagRec(SQL_HANDLE_DBC, _db,
							1,
							sqlState,
							&nativeErrorCode,
							sqlErrorMessage, SQL_MAX_MESSAGE_LENGTH, &sqlErrorMessageLength);
			if (std::string(reinterpret_cast<const char *>(sqlState)) == "25000"
				|| std::string(reinterpret_cast<const char *>(sqlState)) == "25501")
			{
				--retryCount;
				Poco::Thread::sleep(100);
			}
			else
			{
				break;
			}
		}
		else
		{
			return;
		}
	}
	while(retryCount > 0);
		
	throw ODBCException
		(std::string(reinterpret_cast<const char *>(sqlErrorMessage)), nativeErrorCode);
}


int SessionImpl::maxStatementLength() const
{
	SQLUINTEGER info;
	SQLRETURN rc = 0;
	if (Utility::isError(rc = Poco::SQL::ODBC::SQLGetInfo(_db,
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


} } } // namespace Poco::SQL::ODBC
