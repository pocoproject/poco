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


#ifndef Data_ODBC_ConnectionHandle_INCLUDED
#define Data_ODBC_ConnectionHandle_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/EnvironmentHandle.h"
#include "Poco/Data/ODBC/Error.h"
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif
#include <sqltypes.h>


namespace Poco {
namespace Data {
namespace ODBC {

class SessionImpl;

class ODBC_API ConnectionHandle
	/// ODBC connection handle class
{
public:
	static const std::string UNSUPPORTED_SQLSTATE;
	static const std::string GEN_ERR_SQLSTATE;
	static const std::string CANT_SET_ATTR_SQLSTATE;

	ConnectionHandle(const std::string& connectString = "", SQLULEN loginTimeout = 0, SQLULEN timeout = 0);
		/// Creates the ConnectionHandle.
		///
		/// NOTE: some drivers/DBMS do not distinguish between login and connection timeouts,
		/// and setting one timeout overwrites the other.

	~ConnectionHandle();
		/// Creates the ConnectionHandle.

	bool connect(const std::string& connectString = "", SQLULEN loginTimeout = 0, SQLULEN timeout = 0);
		/// Connects the handle to the database.

	bool disconnect();
		/// Disconnects the handle from database.

	bool isConnected() const;
		/// Returns true if connected.

	void setTimeout(int timeout);
		/// Sets the connection timeout in seconds.

	int getTimeout() const;
		/// Returns the connection timeout in seconds.

	void setLoginTimeout(SQLULEN timeout);
		/// Sets the login timeout in seconds.
		/// Must be called before the connection attempt.

	int getLoginTimeout() const;
		/// Returns the login timeout in seconds.
		/// Must be called before the connection attempt.

	const SQLHDBC& handle() const;
		/// Returns const reference to handle;

	const SQLHDBC* pHandle() const;
		/// Returns const pointer to handle;

	operator const SQLHDBC& () const;
		/// Const conversion operator into reference to native type.

	operator bool();
		/// Returns true if handles are not null.
		/// True value is not a guarantee that the connection is valid.

private:
	operator SQLHDBC& ();
		/// Conversion operator into reference to native type.

	SQLHDBC& handle();
		/// Returns reference to handle;

	void alloc();
		/// Allocates the connection handle.

	void free();
		/// Frees the connection handle.

	void setTimeoutImpl(SQLULEN timeout, SQLINTEGER attribute);
		/// Sets the timeout for the attribute.

	int getTimeoutImpl(SQLINTEGER attribute) const;
		/// Returns the timeout for the attribute.

	void setTimeouts(SQLULEN loginTimeout, SQLULEN timeout);

	bool isUnsupported(const ConnectionError& e) const;
		/// Returns true if SQLSTATE is "HYC000"
		/// (Optional feature not implemented)

	bool isGenError(const ConnectionError& e) const;
		/// Returns true if SQLSTATE is "HY000"
		/// (General error)

	bool cantSetAttr(const ConnectionError& e) const;
		/// Returns true if SQLSTATE is "HY011"
		/// (Can't set attribute)

	ConnectionHandle(const ConnectionHandle&);
	const ConnectionHandle& operator=(const ConnectionHandle&);

	const EnvironmentHandle* _pEnvironment = nullptr;
	SQLHDBC                  _hdbc = SQL_NULL_HDBC;
	std::string              _connectString;

	friend class Poco::Data::ODBC::SessionImpl;
};


using Connection = ConnectionHandle;


//
// inlines
//
inline ConnectionHandle::operator const SQLHDBC& () const
{
	return handle();
}


inline ConnectionHandle::operator bool()
{
	return _pEnvironment != nullptr && _hdbc != SQL_NULL_HDBC;
}


inline const SQLHDBC& ConnectionHandle::handle() const
{
	return _hdbc;
}


inline const SQLHDBC* ConnectionHandle::pHandle() const
{
	return &_hdbc;
}


inline ConnectionHandle::operator SQLHDBC& ()
{
	return handle();
}


inline SQLHDBC& ConnectionHandle::handle()
{
	return _hdbc;
}


inline void ConnectionHandle::setTimeout(int timeout)
{
	setTimeoutImpl(static_cast<SQLULEN>(timeout), SQL_ATTR_CONNECTION_TIMEOUT);
}


inline int ConnectionHandle::getTimeout() const
{
	return getTimeoutImpl(SQL_ATTR_CONNECTION_TIMEOUT);
}


inline void ConnectionHandle::setLoginTimeout(SQLULEN timeout)
{
	setTimeoutImpl(timeout, SQL_ATTR_LOGIN_TIMEOUT);
}


inline int ConnectionHandle::getLoginTimeout() const
{
	return getTimeoutImpl(SQL_ATTR_LOGIN_TIMEOUT);
}


} } } // namespace Poco::Data::ODBC


#endif
