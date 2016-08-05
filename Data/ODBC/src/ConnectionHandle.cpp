//
// ConnectionHandle.cpp
//
// $Id: //poco/Main/Data/ODBC/src/ConnectionHandle.cpp#2 $
//
// Library: ODBC
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


namespace Poco {
namespace Data {
namespace ODBC {


ConnectionHandle::ConnectionHandle(EnvironmentHandle* pEnvironment): 
	_environment(pEnvironment ? &pEnvironment->handle() : 0),
	_hdbc(SQL_NULL_HDBC)
{
	if (Utility::isError(SQLAllocHandle(SQL_HANDLE_DBC, 
		_environment.handle(), 
		&_hdbc))) 
	{
		throw ODBCException("Could not allocate connection handle.");
	}
}


ConnectionHandle::~ConnectionHandle()
{
	try
	{
		if (_hdbc != SQL_NULL_HDBC)
		{
			SQLDisconnect(_hdbc);
			SQLRETURN rc = SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
			_hdbc = SQL_NULL_HDBC;

			poco_assert(!Utility::isError(rc));
		}
	}
	catch (...)
	{
		poco_unexpected();
	}
}


} } } // namespace Poco::Data::ODBC
