//
// ConnectionHandle.h
//
// Library: Data/ODBC
// Package: ODBC
// Module:  ConnectionHandle
//
// Definition of ConnectionHandle.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_ODBC_ConnectionHandle_INCLUDED
#define SQL_ODBC_ConnectionHandle_INCLUDED

#include "Poco/SQL/ODBC/ODBC.h"
#include "Poco/SQL/ODBC/EnvironmentHandle.h"
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif
#include <sqltypes.h>


namespace Poco {
namespace SQL {
namespace ODBC {


class ODBC_API ConnectionHandle
/// ODBC connection handle class
{
public:
	ConnectionHandle(EnvironmentHandle* pEnvironment = 0);
		/// Creates the ConnectionHandle.

	~ConnectionHandle();
		/// Creates the ConnectionHandle.

	operator const SQLHDBC& () const;
		/// Const conversion operator into reference to native type.

	const SQLHDBC& handle() const;
		/// Returns const reference to handle;

	operator bool() const;
		/// Returns true if the handle is valid

private:
	operator SQLHDBC& ();
		/// Conversion operator into reference to native type.

	SQLHDBC& handle();
		/// Returns reference to handle;

	ConnectionHandle(const ConnectionHandle&);
	const ConnectionHandle& operator=(const ConnectionHandle&);

	const EnvironmentHandle _environment;
	SQLHDBC                  _hdbc;
};


//
// inlines
//
inline ConnectionHandle::operator const SQLHDBC& () const
{
	return handle();
}


inline const SQLHDBC& ConnectionHandle::handle() const
{
	return _hdbc;
}


inline ConnectionHandle::operator SQLHDBC& ()
{
	return handle();
}


inline SQLHDBC& ConnectionHandle::handle()
{
	return _hdbc;
}


inline ConnectionHandle::operator bool () const
{
	return _hdbc != SQL_NULL_HDBC;
}

} } } // namespace Poco::SQL::ODBC


#endif
