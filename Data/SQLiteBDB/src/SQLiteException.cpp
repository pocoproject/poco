//
// SQLiteBDBException.cpp
//
// Library: Data/SQLiteBDB
// Package: SQLiteBDB
// Module:  SQLiteBDBException
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/SQLiteBDB/SQLiteException.h"
#include <typeinfo>


namespace Poco {
namespace Data {
namespace SQLiteBDB {


POCO_IMPLEMENT_EXCEPTION(SQLiteBDBException, Poco::Data::DataException, "SQLiteBDB exception")
POCO_IMPLEMENT_EXCEPTION(InvalidSQLStatementException, SQLiteBDBException, "Invalid SQL statement")
POCO_IMPLEMENT_EXCEPTION(InternalDBErrorException, SQLiteBDBException, "Internal DB error")
POCO_IMPLEMENT_EXCEPTION(DBAccessDeniedException, SQLiteBDBException, "DB access denied")
POCO_IMPLEMENT_EXCEPTION(ExecutionAbortedException, SQLiteBDBException, "Execution aborted")
POCO_IMPLEMENT_EXCEPTION(DBLockedException, SQLiteBDBException, "DB locked")
POCO_IMPLEMENT_EXCEPTION(TableLockedException, SQLiteBDBException, "Table locked")
POCO_IMPLEMENT_EXCEPTION(NoMemoryException, SQLiteBDBException, "Out of Memory")
POCO_IMPLEMENT_EXCEPTION(ReadOnlyException, SQLiteBDBException, "Read only")
POCO_IMPLEMENT_EXCEPTION(InterruptException, SQLiteBDBException, "Interrupt")
POCO_IMPLEMENT_EXCEPTION(IOErrorException, SQLiteBDBException, "I/O error")
POCO_IMPLEMENT_EXCEPTION(CorruptImageException, SQLiteBDBException, "Corrupt image")
POCO_IMPLEMENT_EXCEPTION(TableNotFoundException, SQLiteBDBException, "Table not found")
POCO_IMPLEMENT_EXCEPTION(DatabaseFullException, SQLiteBDBException, "Database full")
POCO_IMPLEMENT_EXCEPTION(CantOpenDBFileException, SQLiteBDBException, "Can't open DB file")
POCO_IMPLEMENT_EXCEPTION(LockProtocolException, SQLiteBDBException, "Lock protocol")
POCO_IMPLEMENT_EXCEPTION(SchemaDiffersException, SQLiteBDBException, "Schema differs")
POCO_IMPLEMENT_EXCEPTION(RowTooBigException, SQLiteBDBException, "Row too big")
POCO_IMPLEMENT_EXCEPTION(ConstraintViolationException, SQLiteBDBException, "Constraint violation")
POCO_IMPLEMENT_EXCEPTION(DataTypeMismatchException, SQLiteBDBException, "Data type mismatch")
POCO_IMPLEMENT_EXCEPTION(ParameterCountMismatchException, SQLiteBDBException, "Parameter count mismatch")
POCO_IMPLEMENT_EXCEPTION(InvalidLibraryUseException, SQLiteBDBException, "Invalid library use")
POCO_IMPLEMENT_EXCEPTION(OSFeaturesMissingException, SQLiteBDBException, "OS features missing")
POCO_IMPLEMENT_EXCEPTION(AuthorizationDeniedException, SQLiteBDBException, "Authorization denied")
POCO_IMPLEMENT_EXCEPTION(TransactionException, SQLiteBDBException, "Transaction exception")


} } } // namespace Poco::Data::SQLiteBDB
