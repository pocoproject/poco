//
// SQLException.h
//
// Library: Data
// Package: DataCore
// Module:  DataException
//
// Definition of the DataException class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_DataException_INCLUDED
#define SQL_DataException_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/Exception.h"


namespace Poco {
namespace SQL {


POCO_DECLARE_EXCEPTION(Poco_SQL_API, DataException, Poco::IOException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, RowDataMissingException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, UnknownDataBaseException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, UnknownTypeException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, ExecutionException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, BindingException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, ExtractException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, LimitException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, NotSupportedException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, SessionUnavailableException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, SessionPoolExhaustedException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, SessionPoolExistsException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, NoDataException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, LengthExceededException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, ConnectionFailedException, DataException)
POCO_DECLARE_EXCEPTION(Poco_SQL_API, NotConnectedException, DataException)


} } // namespace Poco::Data


#endif // Data_DataException_INCLUDED
