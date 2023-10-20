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


ConnectionHandle::ConnectionHandle(const std::string& connectString, SQLULEN timeout): _pEnvironment(nullptr),
	_hdbc(SQL_NULL_HDBC),
	_connectString(connectString)
{
	alloc();
	setTimeout(timeout);
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


bool ConnectionHandle::connect(const std::string& connectString, SQLULEN timeout)
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
	if (Utility::isError(Poco::Data::ODBC::SQLDriverConnect(_hdbc
		, NULL
		,(SQLCHAR*) _connectString.c_str()
		,(SQLSMALLINT) SQL_NTS
		, connectOutput
		, sizeof(connectOutput)
		, &result
		, SQL_DRIVER_NOPROMPT)))
	{
		disconnect();
		ConnectionError err(_hdbc);
		throw ConnectionFailedException(err.toString());
	}

	return _hdbc != SQL_NULL_HDBC;;
}


bool ConnectionHandle::disconnect()
{
	SQLRETURN rc = 0;
	if (isConnected())
		rc = SQLDisconnect(_hdbc);

	free();
	return !Utility::isError(rc);
}


void ConnectionHandle::setTimeout(SQLULEN timeout)
{
	if (Utility::isError(SQLSetConnectAttr(_hdbc, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER) timeout, 0)))
	{
		ConnectionError e(_hdbc);
		throw ConnectionFailedException(e.toString());
	}
}


int ConnectionHandle::getTimeout() const
{
	SQLULEN timeout = 0;
	if (Utility::isError(SQLGetConnectAttr(_hdbc, SQL_ATTR_LOGIN_TIMEOUT, &timeout, 0, 0)))
	{
		ConnectionError e(_hdbc);
		throw ConnectionFailedException(e.toString());
	}
	return static_cast<int>(timeout);
}


bool ConnectionHandle::isConnected() const
{
	if (!*this) return false;

	SQLULEN value = 0;

	if (Utility::isError(Poco::Data::ODBC::SQLGetConnectAttr(_hdbc,
		SQL_ATTR_CONNECTION_DEAD,
		&value,
		0,
		0))) return false;

	return (SQL_CD_FALSE == value);
}


} } } // namespace Poco::Data::ODBC
