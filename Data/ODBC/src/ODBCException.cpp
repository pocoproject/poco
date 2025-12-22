//
// ODBCException.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  ODBCException
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/ODBC/ODBCException.h"
#include <typeinfo>


namespace Poco {
namespace Data {
namespace ODBC {

#if defined(POCO_OS_FAMILY_WINDOWS)
template class ODBC_API HandleException<SQLHENV, SQL_HANDLE_ENV>;
template class ODBC_API HandleException<SQLHDBC, SQL_HANDLE_DBC>;
template class ODBC_API HandleException<SQLHSTMT, SQL_HANDLE_STMT>;
template class ODBC_API HandleException<SQLHDESC, SQL_HANDLE_DESC>;
#else
template class HandleException<SQLHENV, SQL_HANDLE_ENV>;
template class HandleException<SQLHDBC, SQL_HANDLE_DBC>;
template class HandleException<SQLHSTMT, SQL_HANDLE_STMT>;
template class HandleException<SQLHDESC, SQL_HANDLE_DESC>;
#endif

POCO_IMPLEMENT_EXCEPTION(ODBCException, Poco::Data::DataException, "Generic ODBC error")
POCO_IMPLEMENT_EXCEPTION(InsufficientStorageException, ODBCException, "Insufficient storage error")
POCO_IMPLEMENT_EXCEPTION(UnknownDataLengthException, ODBCException, "Unknown length of remaining data")
POCO_IMPLEMENT_EXCEPTION(DataTruncatedException, ODBCException, "Variable length character or binary data truncated")


} } } // namespace Poco::Data::ODBC
