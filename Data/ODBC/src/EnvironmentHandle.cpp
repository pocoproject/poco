//
// EnvironmentHandle.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  EnvironmentHandle
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/ODBC/EnvironmentHandle.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Error.h"
#include "Poco/Debugger.h"


namespace Poco {
namespace Data {
namespace ODBC {


EnvironmentHandle::EnvironmentHandle(): _henv(SQL_NULL_HENV)
{
	SQLRETURN rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_henv);
	if (Utility::isError(rc))
		throw ODBCException("EnvironmentHandle: Could not initialize ODBC environment.");

	rc = SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);
	if (Utility::isError(rc))
	{
		EnvironmentError err(_henv);
		throw ODBCException(err.toString());
	}
}


EnvironmentHandle::~EnvironmentHandle()
{
	try
	{
#if defined(_DEBUG)
		SQLRETURN rc = SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		if (Utility::isError(rc))
		{
			EnvironmentError err(_henv);
			Debugger::enter(err.toString(), __FILE__, __LINE__);
		}
#else
		SQLFreeHandle(SQL_HANDLE_ENV, _henv);
#endif
	}
	catch (...)
	{
		poco_unexpected();
	}
}


} } } // namespace Poco::Data::ODBC
