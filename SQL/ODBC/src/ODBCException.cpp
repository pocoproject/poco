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


#include "Poco/SQL/ODBC/ODBCException.h"
#include <typeinfo>


namespace Poco {
namespace SQL {
namespace ODBC {


POCO_IMPLEMENT_EXCEPTION(ODBCException, Poco::SQL::SQLException, "Generic ODBC error")
POCO_IMPLEMENT_EXCEPTION(InsufficientStorageException, ODBCException, "Insufficient storage error")
POCO_IMPLEMENT_EXCEPTION(UnknownDataLengthException, ODBCException, "Unknown length of remaining data")
POCO_IMPLEMENT_EXCEPTION(DataTruncatedException, ODBCException, "Variable length character or binary data truncated")


} } } // namespace Poco::SQL::ODBC
