module;

#ifdef ENABLE_DATA_SQLITE
#include "Poco/Data/SQLite/Binder.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/SQLite/Extractor.h"
#include "Poco/Data/SQLite/Notifier.h"
#include "Poco/Data/SQLite/SessionImpl.h"
#include "Poco/Data/SQLite/SQLiteException.h"
#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/SQLite/SQLiteStatementImpl.h"
#include "Poco/Data/SQLite/Utility.h"
#endif

export module Poco.Data:SQLite;

export namespace Poco::Data::SQLite {
	#ifdef ENABLE_DATA_SQLITE
	using Poco::Data::SQLite::AuthorizationDeniedException;
	using Poco::Data::SQLite::Binder;
	using Poco::Data::SQLite::CantOpenDBFileException;
	using Poco::Data::SQLite::Connector;
	using Poco::Data::SQLite::ConstraintViolationException;
	using Poco::Data::SQLite::CorruptImageException;
	using Poco::Data::SQLite::DBAccessDeniedException;
	using Poco::Data::SQLite::DBLockedException;
	using Poco::Data::SQLite::DataTypeMismatchException;
	using Poco::Data::SQLite::DatabaseFullException;
	using Poco::Data::SQLite::ExecutionAbortedException;
	using Poco::Data::SQLite::Extractor;
	using Poco::Data::SQLite::IOErrorException;
	using Poco::Data::SQLite::InternalDBErrorException;
	using Poco::Data::SQLite::InterruptException;
	using Poco::Data::SQLite::InvalidLibraryUseException;
	using Poco::Data::SQLite::InvalidSQLStatementException;
	using Poco::Data::SQLite::LockProtocolException;
	using Poco::Data::SQLite::NoMemoryException;
	using Poco::Data::SQLite::Notifier;
	using Poco::Data::SQLite::OSFeaturesMissingException;
	using Poco::Data::SQLite::ParameterCountMismatchException;
	using Poco::Data::SQLite::ReadOnlyException;
	using Poco::Data::SQLite::RowTooBigException;
	using Poco::Data::SQLite::SQLiteException;
	using Poco::Data::SQLite::SQLiteStatementImpl;
	using Poco::Data::SQLite::SchemaDiffersException;
	using Poco::Data::SQLite::SessionImpl;
	using Poco::Data::SQLite::TableLockedException;
	using Poco::Data::SQLite::TableNotFoundException;
	using Poco::Data::SQLite::TransactionException;
	using Poco::Data::SQLite::Utility;

	using Poco::Data::SQLite::TransactionType;
	#endif
}