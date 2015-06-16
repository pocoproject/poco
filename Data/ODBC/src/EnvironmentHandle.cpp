//
// EnvironmentHandle.cpp
//
// $Id: //poco/Main/Data/ODBC/src/EnvironmentHandle.cpp#2 $
//
// Library: ODBC
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


namespace Poco {
namespace Data {
namespace ODBC {


EnvironmentHandle::EnvironmentHandle(): _henv(SQL_NULL_HENV),
	_isOwner(false)
{
	init();
}

EnvironmentHandle::EnvironmentHandle(const SQLHENV* henv) : _henv(SQL_NULL_HENV),
	_isOwner(false)
{
	if (!henv || *henv == SQL_NULL_HENV)
		init();
	else
		_henv = *henv;
}

void EnvironmentHandle::init()
{
	if (Utility::isError(SQLAllocHandle(SQL_HANDLE_ENV,
		SQL_NULL_HANDLE,
		&_henv)) ||
		Utility::isError(SQLSetEnvAttr(_henv,
		SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3,
		0)))
	{
		throw ODBCException("Could not initialize environment.");
	}
	_isOwner = true;
}

EnvironmentHandle::~EnvironmentHandle()
{
	try
	{
		if (_isOwner && _henv != SQL_NULL_HENV)
		{
			SQLRETURN rc = SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			_henv = SQL_NULL_HENV;
			poco_assert(!Utility::isError(rc));
		}
	}
	catch (...)
	{
		poco_unexpected();
	}
}


} } } // namespace Poco::Data::ODBC
