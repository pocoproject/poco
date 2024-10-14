//
// ConnectionHandle.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  ConnectionHandle
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/ODBC/ConnectionHandle.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Error.h"
#include "Poco/Debugger.h"


namespace Poco {
namespace Data {
namespace ODBC {


const std::string ConnectionHandle::UNSUPPORTED_SQLSTATE = "HYC00";
const std::string ConnectionHandle::GEN_ERR_SQLSTATE = "HY000";
const std::string ConnectionHandle::CANT_SET_ATTR_SQLSTATE = "HY011";


ConnectionHandle::ConnectionHandle(const std::string& connectString, SQLULEN loginTimeout, SQLULEN timeout):
	_pEnvironment(nullptr),
	_hdbc(SQL_NULL_HDBC),
	_connectString(connectString)
{
	alloc();
	setTimeouts(loginTimeout, timeout);
}


ConnectionHandle::~ConnectionHandle()
{
	try
	{
		disconnect();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void ConnectionHandle::alloc()
{
	if (_pEnvironment || _hdbc) free();
	_pEnvironment = new EnvironmentHandle;
	if (Utility::isError(SQLAllocHandle(SQL_HANDLE_DBC, _pEnvironment->handle(), &_hdbc)))
	{
		delete _pEnvironment;
		_pEnvironment = nullptr;
		_hdbc = SQL_NULL_HDBC;
		throw ODBCException("ODBC: Could not allocate connection handle.");
	}
}


void ConnectionHandle::free()
{
	if (_hdbc != SQL_NULL_HDBC)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
		_hdbc = SQL_NULL_HDBC;
	}

	if (_pEnvironment)
	{
		delete _pEnvironment;
		_pEnvironment = 0;
	}
}


void ConnectionHandle::setTimeouts(SQLULEN loginTimeout, SQLULEN timeout)
{
	if (loginTimeout)
	{
		try
		{
			setLoginTimeout(loginTimeout);
		}
		catch(const NotSupportedException&) {}
	}

	if (timeout)
	{
		try
		{
			setTimeout(static_cast<int>(timeout));
		}
		catch(const NotSupportedException&) {}
	}
}


bool ConnectionHandle::connect(const std::string& connectString, SQLULEN loginTimeout, SQLULEN timeout)
{
	if (isConnected())
		throw Poco::InvalidAccessException("ODBC: connection already established.");

	if (connectString.empty())
		throw Poco::InvalidArgumentException("ODBC: connection string is empty.");

	if (connectString != _connectString)
		_connectString = connectString;

	SQLCHAR connectOutput[512] = {0};
	SQLSMALLINT result;

	if (!_pEnvironment) alloc();

	setTimeouts(loginTimeout, timeout);

	if (Utility::isError(Poco::Data::ODBC::SQLDriverConnect(_hdbc
		, NULL
		,(SQLCHAR*) _connectString.c_str()
		,(SQLSMALLINT) SQL_NTS
		, connectOutput
		, sizeof(connectOutput)
		, &result
		, SQL_DRIVER_NOPROMPT)))
	{
		ConnectionError err(_hdbc);
		disconnect();
		throw ConnectionFailedException(err.toString());
	}

	try
	{
		// Setting timeouts before connection is valid by standard (as it makes sense for login timeout),
		// but not all drivers comply, so we have to check and try again after succesful connection.
		// Furthermore, it appears that Oracle has only one timeout, so it is not possible to have
		// different login and connection timeouts. Last but not least, some ODBC drivers (eg. DataDirect
		// for Oracle) flat out refuse to set login timeout and return error - that's why these calls
		// are wrapped in try/catch and silently ignore errors.
		if (getTimeout() != timeout)
			setTimeout(static_cast<int>(timeout));
		if (getLoginTimeout() != loginTimeout)
			setLoginTimeout(loginTimeout);
	}
	catch(NotSupportedException&){}
	catch(InvalidAccessException&){}

	return _hdbc != SQL_NULL_HDBC;
}


bool ConnectionHandle::disconnect()
{
	SQLRETURN rc = 0;
	if (isConnected())
		rc = SQLDisconnect(_hdbc);

	free();
	return !Utility::isError(rc);
}


void ConnectionHandle::setTimeoutImpl(SQLULEN timeout, SQLINTEGER attribute)
{
	if (attribute != SQL_ATTR_LOGIN_TIMEOUT && attribute != SQL_ATTR_CONNECTION_TIMEOUT)
		throw InvalidArgumentException(Poco::format("ODBC::ConnectionHandle::setTimeoutImpl(%d)", attribute));

	if (Utility::isError(SQLSetConnectAttr(_hdbc, attribute, (SQLPOINTER) timeout, 0)))
	{
		ConnectionError e(_hdbc);
		std::string name;
		switch (attribute)
		{
			case SQL_ATTR_LOGIN_TIMEOUT:
				name = "LOGIN"s;
				break;
			case SQL_ATTR_CONNECTION_TIMEOUT:
				name = "CONNECTION"s;
				break;
			default:
				break;
		}

		if (isUnsupported(e))
			throw NotSupportedException(Poco::format("ConnectionHandle::setTimeoutImpl(%s)", name));
		else if (isGenError(e) || cantSetAttr(e))
			throw InvalidAccessException(Poco::format("ConnectionHandle::setTimeoutImpl(%s)", name));

		throw ConnectionFailedException(e.toString());
	}
}


int ConnectionHandle::getTimeoutImpl(SQLINTEGER attribute) const
{
	SQLUINTEGER timeout = 0;
	if (Utility::isError(SQLGetConnectAttr(_hdbc, attribute, &timeout, sizeof(timeout), 0)))
	{
		ConnectionError e(_hdbc);
		if (isUnsupported(e))
			throw NotSupportedException("ConnectionHandle::getTimeoutImpl(%s)");

		throw ConnectionFailedException(e.toString());
	}
	return static_cast<int>(timeout);
}


bool ConnectionHandle::isUnsupported(const ConnectionError& e) const
{
	const ConnectionDiagnostics& cd = e.diagnostics();
	int diagRecs = cd.count();
	for (int i = 0; i < diagRecs; ++i)
	{
		if (cd.sqlState(i) == UNSUPPORTED_SQLSTATE)
		{
			return true;
		}
	}
	return false;
}


bool ConnectionHandle::isGenError(const ConnectionError& e) const
{
	const ConnectionDiagnostics& cd = e.diagnostics();
	int diagRecs = cd.count();
	for (int i = 0; i < diagRecs; ++i)
	{
		if (cd.sqlState(i) == GEN_ERR_SQLSTATE)
		{
			return true;
		}
	}
	return false;
}


bool ConnectionHandle::cantSetAttr(const ConnectionError& e) const
{
	const ConnectionDiagnostics& cd = e.diagnostics();
	int diagRecs = cd.count();
	for (int i = 0; i < diagRecs; ++i)
	{
		if (cd.sqlState(i) == CANT_SET_ATTR_SQLSTATE)
		{
			return true;
		}
	}
	return false;
}


bool ConnectionHandle::isConnected() const
{
	if (!*this) return false;

	SQLINTEGER value = -1;

	if (Utility::isError(Poco::Data::ODBC::SQLGetConnectAttr(_hdbc,
		SQL_ATTR_CONNECTION_DEAD,
		&value,
		sizeof(value),
		0))) return false;

	return (SQL_CD_FALSE == value);
}


} } } // namespace Poco::Data::ODBC
