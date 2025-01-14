//
// Error.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Error
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/ODBC/Error.h"


namespace Poco {
namespace Data {
namespace ODBC {

// explicit instantiation definition
#ifndef POCO_DOC

#if defined(POCO_OS_FAMILY_WINDOWS)
template class ODBC_API Error<SQLHENV, SQL_HANDLE_ENV>;
template class ODBC_API Error<SQLHDBC, SQL_HANDLE_DBC>;
template class ODBC_API Error<SQLHSTMT, SQL_HANDLE_STMT>;
template class ODBC_API Error<SQLHDESC, SQL_HANDLE_DESC>;
#else
template class Error<SQLHENV, SQL_HANDLE_ENV>;
template class Error<SQLHDBC, SQL_HANDLE_DBC>;
template class Error<SQLHSTMT, SQL_HANDLE_STMT>;
template class Error<SQLHDESC, SQL_HANDLE_DESC>;
#endif

#endif


} } } // namespace Poco::Data::ODBC
