//
// Handle.h
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Handle
//
// Definition of Handle.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_ODBC_Handle_INCLUDED
#define Data_ODBC_Handle_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/ConnectionHandle.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/ODBC/Utility.h"
#ifdef POCO_OS_FAMILY_WINDOWS
#include "Poco/UnWindows.h"
#endif
#include <sqltypes.h>


namespace Poco::Data::ODBC {


template <typename H, SQLSMALLINT handleType>
class Handle
/// ODBC handle class template
{
public:
	Handle(const ConnectionHandle& rConnection):
		_rConnection(rConnection),
		_handle(nullptr),
		_pGeneration(rConnection.generation()),
		_generation(*_pGeneration)
			/// Creates the Handle.
	{
		if (Utility::isError(SQLAllocHandle(handleType,
			_rConnection,
			&_handle)))
		{
			throw ODBCException("Could not allocate statement handle.");
		}
	}

	~Handle()
		/// Destroys the Handle.
		/// Does not free the native handle if its connection
		/// has been closed, because SQLDisconnect has freed it.
	{
		if (isValid()) SQLFreeHandle(handleType, _handle);
	}

	[[nodiscard]] bool isValid() const
		/// Returns true if the connection on which the native handle
		/// was allocated is still open. Once it has been closed, the
		/// native handle is freed and must not be used.
	{
		return *_pGeneration == _generation;
	}

	[[nodiscard]] operator const H& () const
		/// Const conversion operator into reference to native type.
	{
		return handle();
	}

	[[nodiscard]] const H& handle() const
		/// Returns const reference to native type.
	{
		return _handle;
	}

	[[nodiscard]] const ConnectionHandle& connection() const
		/// Returns the connection handle.
	{
		return _rConnection;
	}

private:
	Handle(const Handle&) = delete;
	const Handle& operator=(const Handle&) = delete;

	[[nodiscard]] operator H& ()
		/// Conversion operator into reference to native type.
	{
		return handle();
	}

	[[nodiscard]] H& handle()
		/// Returns reference to native type.
	{
		return _handle;
	}

	const ConnectionHandle& _rConnection;
	H _handle;
	std::shared_ptr<const ConnectionHandle::Counter> _pGeneration;
	Poco::UInt32 _generation;

	friend class ODBCStatementImpl;
};


typedef Handle<SQLHSTMT, SQL_HANDLE_STMT> StatementHandle;
typedef Handle<SQLHDESC, SQL_HANDLE_DESC> DescriptorHandle;


} // namespace Poco::Data::ODBC


#endif
