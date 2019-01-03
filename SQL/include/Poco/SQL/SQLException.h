//
// SQLException.h
//
// Library: Data
// Package: DataCore
// Module:  SQLException
//
// Definition of the SQLException class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_SQLException_INCLUDED
#define SQL_SQLException_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/Exception.h"


namespace Poco {
namespace SQL {


POCO_DECLARE_EXCEPTION(Poco_SQL_API, SQLException, Poco::IOException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, RowDataMissingException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, UnknownDataBaseException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, UnknownTypeException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, ExecutionException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, BindingException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, ExtractException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, LimitException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, NotSupportedException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, SessionUnavailableException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, SessionPoolExhaustedException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, SessionPoolExistsException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, NoSQLException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, LengthExceededException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, ConnectionFailedException, SQLException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, NotConnectedException, SQLException)


} } // namespace Poco::SQL


#endif // Data_SQLException_INCLUDED
