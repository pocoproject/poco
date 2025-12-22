//
// Diagnostics.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Diagnostics
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/ODBC/Diagnostics.h"


namespace Poco {
namespace Data {
namespace ODBC {

// explicit instantiation definition
#ifndef POCO_DOC

#if defined(POCO_OS_FAMILY_WINDOWS)
template class ODBC_API Diagnostics<SQLHENV, SQL_HANDLE_ENV>;
template class ODBC_API Diagnostics<SQLHDBC, SQL_HANDLE_DBC>;
template class ODBC_API Diagnostics<SQLHSTMT, SQL_HANDLE_STMT>;
template class ODBC_API Diagnostics<SQLHDESC, SQL_HANDLE_DESC>;
#else
template class Diagnostics<SQLHENV, SQL_HANDLE_ENV>;
template class Diagnostics<SQLHDBC, SQL_HANDLE_DBC>;
template class Diagnostics<SQLHSTMT, SQL_HANDLE_STMT>;
template class Diagnostics<SQLHDESC, SQL_HANDLE_DESC>;
#endif

#endif


} } } // namespace Poco::Data::ODBC
