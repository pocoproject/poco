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


ConnectionHandle::ConnectionHandle(EnvironmentHandle* pEnvironment):
	_pEnvironment(pEnvironment ? pEnvironment : new EnvironmentHandle),
	_hdbc(SQL_NULL_HDBC),
	_ownsEnvironment(pEnvironment ? false : true)
{
	if (Utility::isError(SQLAllocHandle(SQL_HANDLE_DBC,
		_pEnvironment->handle(),
		&_hdbc)))
	{
		throw ODBCException("Could not allocate connection handle.");
	}
}


ConnectionHandle::~ConnectionHandle()
{
	try
	{
		SQLDisconnect(_hdbc);
		SQLRETURN rc = SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
		if (_ownsEnvironment) delete _pEnvironment;
#if defined(_DEBUG)
		if (Utility::isError(rc))
			Debugger::enter(Poco::Error::getMessage(Poco::Error::last()), __FILE__, __LINE__);
#endif
	}
	catch (...)
	{
		poco_unexpected();
	}
}


} } } // namespace Poco::Data::ODBC
