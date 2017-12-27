//
// SQLException.cpp
//
// Library: Data
// Package: DataCore
// Module:  SQLException
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/SQLException.h"
#include <typeinfo>


namespace Poco {
namespace SQL {


POCO_IMPLEMENT_EXCEPTION(SQLException, Poco::IOException, "Database Exception")
POCO_IMPLEMENT_EXCEPTION(RowDataMissingException, SQLException, "Data for row missing")
POCO_IMPLEMENT_EXCEPTION(UnknownDataBaseException, SQLException, "Type of data base unknown")
POCO_IMPLEMENT_EXCEPTION(UnknownTypeException, SQLException, "Type of data unknown")
POCO_IMPLEMENT_EXCEPTION(ExecutionException, SQLException, "Execution error")
POCO_IMPLEMENT_EXCEPTION(BindingException, SQLException, "Binding error")
POCO_IMPLEMENT_EXCEPTION(ExtractException, SQLException, "Extraction error")
POCO_IMPLEMENT_EXCEPTION(LimitException, SQLException, "Limit error")
POCO_IMPLEMENT_EXCEPTION(NotSupportedException, SQLException, "Feature or property not supported")
POCO_IMPLEMENT_EXCEPTION(SessionUnavailableException, SQLException, "Session is unavailable")
POCO_IMPLEMENT_EXCEPTION(SessionPoolExhaustedException, SQLException, "No more sessions available from the session pool")
POCO_IMPLEMENT_EXCEPTION(SessionPoolExistsException, SQLException, "Session already exists in the pool")
POCO_IMPLEMENT_EXCEPTION(NoSQLException, SQLException, "No data found")
POCO_IMPLEMENT_EXCEPTION(LengthExceededException, SQLException, "Data too long")
POCO_IMPLEMENT_EXCEPTION(ConnectionFailedException, SQLException, "Connection attempt failed")
POCO_IMPLEMENT_EXCEPTION(NotConnectedException, SQLException, "Not connected to data source")


} } // namespace Poco::SQL
