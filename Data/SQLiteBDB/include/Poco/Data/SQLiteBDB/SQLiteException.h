//
// SQLiteBDBException.h
//
// Library: Data/SQLiteBDB
// Package: SQLiteBDB
// Module:  SQLiteBDBException
//
// Definition of SQLiteBDBException.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLiteBDB_SQLiteBDBException_INCLUDED
#define SQLiteBDB_SQLiteBDBException_INCLUDED


#include "Poco/Data/SQLiteBDB/SQLite.h"
#include "Poco/Data/DataException.h"


namespace Poco {
namespace Data {
namespace SQLiteBDB {


POCO_DECLARE_EXCEPTION(SQLiteBDB_API, SQLiteBDBException, Poco::Data::DataException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, InvalidSQLStatementException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, InternalDBErrorException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, DBAccessDeniedException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, ExecutionAbortedException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, DBLockedException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, TableLockedException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, NoMemoryException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, ReadOnlyException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, InterruptException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, IOErrorException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, CorruptImageException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, TableNotFoundException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, DatabaseFullException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, CantOpenDBFileException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, LockProtocolException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, SchemaDiffersException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, RowTooBigException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, ConstraintViolationException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, DataTypeMismatchException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, ParameterCountMismatchException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, InvalidLibraryUseException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, OSFeaturesMissingException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, AuthorizationDeniedException, SQLiteBDBException)
POCO_DECLARE_EXCEPTION(SQLiteBDB_API, TransactionException, SQLiteBDBException)


} } } // namespace Poco::Data::SQLiteBDB


#endif
