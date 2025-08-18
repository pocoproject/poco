module;

#include "Poco/Data/AbstractBinder.h"
#include "Poco/Data/AbstractBinding.h"
#include "Poco/Data/AbstractExtraction.h"
#include "Poco/Data/AbstractExtractor.h"
#include "Poco/Data/AbstractPreparation.h"
#include "Poco/Data/AbstractPreparator.h"
#include "Poco/Data/AbstractSessionImpl.h"
#include "Poco/Data/ArchiveStrategy.h"
#include "Poco/Data/AutoTransaction.h"
#include "Poco/Data/Binding.h"
#include "Poco/Data/BulkBinding.h"
#include "Poco/Data/BulkExtraction.h"
#include "Poco/Data/Bulk.h"
#include "Poco/Data/Column.h"
#include "Poco/Data/Connector.h"
#include "Poco/Data/Constants.h"
#include "Poco/Data/DataException.h"
#include "Poco/Data/Data.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/DynamicDateTime.h"
#include "Poco/Data/DynamicLOB.h"
#include "Poco/Data/Extraction.h"
#include "Poco/Data/JSONRowFormatter.h"
#include "Poco/Data/Limit.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/LOBStream.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/Data/PooledSessionHolder.h"
#include "Poco/Data/PooledSessionImpl.h"
#include "Poco/Data/Position.h"
#include "Poco/Data/Preparation.h"
#include "Poco/Data/Range.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/RowFilter.h"
#include "Poco/Data/RowFormatter.h"
#include "Poco/Data/Row.h"
#include "Poco/Data/RowIterator.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SessionImpl.h"
#include "Poco/Data/SessionPoolContainer.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SimpleRowFormatter.h"
#include "Poco/Data/SQLChannel.h"
#include "Poco/Data/StatementCreator.h"
#include "Poco/Data/Statement.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/Time.h"
#include "Poco/Data/Transaction.h"
#include "Poco/Data/Transcoder.h"
#include "Poco/Data/TypeHandler.h"
#include "Poco/Data/MySQL/Binder.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/MySQL/Extractor.h"
#include "Poco/Data/MySQL/MySQLException.h"
#include "Poco/Data/MySQL/MySQL.h"
#include "Poco/Data/MySQL/MySQLStatementImpl.h"
#include "Poco/Data/MySQL/ResultMetadata.h"
#include "Poco/Data/MySQL/SessionHandle.h"
#include "Poco/Data/MySQL/SessionImpl.h"
#include "Poco/Data/MySQL/StatementExecutor.h"
#include "Poco/Data/MySQL/Utility.h"
#include "Poco/Data/ODBC/Binder.h"
#include "Poco/Data/ODBC/ConnectionHandle.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/Diagnostics.h"
#include "Poco/Data/ODBC/EnvironmentHandle.h"
#include "Poco/Data/ODBC/Error.h"
#include "Poco/Data/ODBC/Extractor.h"
#include "Poco/Data/ODBC/Handle.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/ODBCMetaColumn.h"
#include "Poco/Data/ODBC/ODBCStatementImpl.h"
#include "Poco/Data/ODBC/Parameter.h"
#include "Poco/Data/ODBC/Preparator.h"
#include "Poco/Data/ODBC/SessionImpl.h"
#include "Poco/Data/ODBC/TypeInfo.h"
#include "Poco/Data/ODBC/Unicode.h"
#include "Poco/Data/ODBC/Unicode_UNIXODBC.h"
#include "Poco/Data/ODBC/Unicode_WIN32.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/PostgreSQL/BinaryExtractor.h"
#include "Poco/Data/PostgreSQL/Binder.h"
#include "Poco/Data/PostgreSQL/Connector.h"
#include "Poco/Data/PostgreSQL/Extractor.h"
#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/PostgreSQL/PostgreSQLException.h"
#include "Poco/Data/PostgreSQL/PostgreSQLStatement.h"
#include "Poco/Data/PostgreSQL/PostgreSQLTypes.h"
#include "Poco/Data/PostgreSQL/SessionHandle.h"
#include "Poco/Data/PostgreSQL/SessionImpl.h"
#include "Poco/Data/PostgreSQL/StatementExecutor.h"
#include "Poco/Data/PostgreSQL/Utility.h"
#include "Poco/Data/SQLite/Binder.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/SQLite/Extractor.h"
#include "Poco/Data/SQLite/Notifier.h"
#include "Poco/Data/SQLite/SessionImpl.h"
#include "Poco/Data/SQLite/SQLiteException.h"
#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/SQLite/SQLiteStatementImpl.h"
#include "Poco/Data/SQLite/Utility.h"

export module Poco.Data;

export namespace Poco::Data {
	using Poco::Data::AbstractBinder;
	using Poco::Data::AbstractBinding;
	using Poco::Data::AbstractExtraction;
	using Poco::Data::AbstractExtractor;
	using Poco::Data::AbstractPreparation;
	using Poco::Data::AbstractPreparator;
	using Poco::Data::AbstractTypeHandler;
	using Poco::Data::ArchiveByAgeStrategy;
	using Poco::Data::ArchiveStrategy;
	using Poco::Data::Binding;
	using Poco::Data::BindingException;
	using Poco::Data::Bulk;
	using Poco::Data::BulkBinding;
	using Poco::Data::BulkExtraction;
	using Poco::Data::Column;
	using Poco::Data::ConnectionFailedException;
	using Poco::Data::Connector;
	using Poco::Data::CopyBinding;
	using Poco::Data::DataException;
	using Poco::Data::Date;
	using Poco::Data::ExecutionException;
	using Poco::Data::ExtractException;
	using Poco::Data::Extraction;
	using Poco::Data::InternalBulkExtraction;
	using Poco::Data::InternalExtraction;
	using Poco::Data::JSONRowFormatter;
	using Poco::Data::LOB;
	using Poco::Data::LOBIOS;
	using Poco::Data::LOBInputStream;
	using Poco::Data::LOBOutputStream;
	using Poco::Data::LOBStreamBuf;
	using Poco::Data::LengthExceededException;
	using Poco::Data::Limit;
	using Poco::Data::LimitException;
	using Poco::Data::MetaColumn;
	using Poco::Data::NoDataException;
	using Poco::Data::NotConnectedException;
	using Poco::Data::NotSupportedException;
	using Poco::Data::PooledSessionHolder;
	using Poco::Data::Position;
	using Poco::Data::Preparation;
	using Poco::Data::Range;
	using Poco::Data::RecordSet;
	using Poco::Data::Row;
	using Poco::Data::RowDataMissingException;
	using Poco::Data::RowFilter;
	using Poco::Data::RowFormatter;
	using Poco::Data::RowIterator;
	using Poco::Data::SQLChannel;
	using Poco::Data::Session;
	using Poco::Data::SessionFactory;
	using Poco::Data::SessionPool;
	using Poco::Data::SessionPoolContainer;
	using Poco::Data::SessionPoolExhaustedException;
	using Poco::Data::SessionPoolExistsException;
	using Poco::Data::SessionUnavailableException;
	using Poco::Data::SimpleRowFormatter;
	using Poco::Data::Statement;
	using Poco::Data::StatementCreator;
	using Poco::Data::Time;
	using Poco::Data::Transaction;
	using Poco::Data::Transcoder;
	using Poco::Data::TypeHandler;
	using Poco::Data::UnknownDataBaseException;
	using Poco::Data::UnknownTypeException;

	#ifdef POCO_MODULES_EXPORT_IMPL
	using Poco::Data::PooledSessionImpl;
	using Poco::Data::SessionImpl;
	using Poco::Data::StatementImpl;
	#endif

	using Poco::Data::BulkFnType;
	using Poco::Data::AbstractBindingDeq;
	using Poco::Data::AbstractBindingLst;
	using Poco::Data::AbstractBindingVec;
	using Poco::Data::AbstractExtractionDeq;
	using Poco::Data::AbstractExtractionDeqVec;
	using Poco::Data::AbstractExtractionLst;
	using Poco::Data::AbstractExtractionLst;
	using Poco::Data::AbstractExtractionLstVec;
	using Poco::Data::AbstractExtractionVec;
	using Poco::Data::AbstractExtractionVecVec;
	using Poco::Data::AutoTransaction;
	using Poco::Data::BLOB;
	using Poco::Data::BLOBInputStream;
	using Poco::Data::BLOBOutputStream;
	using Poco::Data::CLOB;
	using Poco::Data::CLOBInputStream;
	using Poco::Data::CLOBOutputStream;
	using Poco::Data::JSON;
	using Poco::Data::NullData;

	constexpr std::size_t DATA_INVALID_ROW = POCO_DATA_INVALID_ROW;

	using Poco::Data::operator<<;
	using Poco::Data::swap;
	using Poco::Data::tupleBind;
	using Poco::Data::tupleExtract;
	using Poco::Data::tuplePrepare;

	namespace Keywords {
		using Poco::Data::Keywords::async;
		using Poco::Data::Keywords::bind;
		using Poco::Data::Keywords::bulk;
		using Poco::Data::Keywords::deque;
		using Poco::Data::Keywords::format;
		using Poco::Data::Keywords::from;
		using Poco::Data::Keywords::in;
		using Poco::Data::Keywords::into;
		using Poco::Data::Keywords::io;
		using Poco::Data::Keywords::limit;
		using Poco::Data::Keywords::list;
		using Poco::Data::Keywords::lowerLimit;
		using Poco::Data::Keywords::now;
		using Poco::Data::Keywords::out;
		using Poco::Data::Keywords::range;
		using Poco::Data::Keywords::reset;
		using Poco::Data::Keywords::sync;
		using Poco::Data::Keywords::upperLimit;
		using Poco::Data::Keywords::use;
		using Poco::Data::Keywords::useRef;
		using Poco::Data::Keywords::vector;
	}

	namespace MySQL {
		using Poco::Data::MySQL::Binder;
		using Poco::Data::MySQL::ConnectionException;
		using Poco::Data::MySQL::Connector;
		using Poco::Data::MySQL::Extractor;
		using Poco::Data::MySQL::MySQLException;
		using Poco::Data::MySQL::ResultMetadata;
		using Poco::Data::MySQL::SessionHandle;
		using Poco::Data::MySQL::StatementException;
		using Poco::Data::MySQL::StatementExecutor;
		using Poco::Data::MySQL::TransactionException;
		using Poco::Data::MySQL::Utility;
		
		#ifdef POCO_MODULES_EXPORT_IMPL
		using Poco::Data::MySQL::MySQLStatementImpl;
		using Poco::Data::MySQL::SessionImpl;
		#endif
	}

	namespace ODBC {
		using Poco::Data::ODBC::Binder;
		using Poco::Data::ODBC::ConnectionHandle;
		using Poco::Data::ODBC::Connector;
		using Poco::Data::ODBC::DataTruncatedException;
		using Poco::Data::ODBC::Diagnostics;
		using Poco::Data::ODBC::EnvironmentHandle;
		using Poco::Data::ODBC::Error;
		using Poco::Data::ODBC::Extractor;
		using Poco::Data::ODBC::Handle;
		using Poco::Data::ODBC::HandleException;
		using Poco::Data::ODBC::InsufficientStorageException;
		using Poco::Data::ODBC::ODBCException;
		using Poco::Data::ODBC::ODBCMetaColumn;
		using Poco::Data::ODBC::Parameter;
		using Poco::Data::ODBC::Preparator;
		using Poco::Data::ODBC::TypeInfo;
		using Poco::Data::ODBC::UnknownDataLengthException;
		using Poco::Data::ODBC::Utility;

		#ifdef POCO_MODULES_EXPORT_IMPL
		using Poco::Data::ODBC::ODBCStatementImpl;
		using Poco::Data::ODBC::SessionImpl;
		#endif

		using Poco::Data::ODBC::Connection;
		using Poco::Data::ODBC::ConnectionDiagnostics;
		using Poco::Data::ODBC::ConnectionError;
		using Poco::Data::ODBC::ConnectionException;
		using Poco::Data::ODBC::DescriptorDiagnostics;
		using Poco::Data::ODBC::DescriptorError;
		using Poco::Data::ODBC::DescriptorException;
		using Poco::Data::ODBC::EnvironmentDiagnostics;
		using Poco::Data::ODBC::EnvironmentError;
		using Poco::Data::ODBC::EnvironmentException;
		using Poco::Data::ODBC::StatementDiagnostics;
		using Poco::Data::ODBC::StatementError;
		using Poco::Data::ODBC::StatementException;

		using Poco::Data::ODBC::SQLBrowseConnect;
		using Poco::Data::ODBC::SQLColAttribute;
		using Poco::Data::ODBC::SQLColAttributes;
		using Poco::Data::ODBC::SQLColumnPrivileges;
		using Poco::Data::ODBC::SQLColumns;
		using Poco::Data::ODBC::SQLConnect;
		using Poco::Data::ODBC::SQLDataSources;
		using Poco::Data::ODBC::SQLDescribeCol;
		using Poco::Data::ODBC::SQLDriverConnect;
		using Poco::Data::ODBC::SQLDrivers;
		using Poco::Data::ODBC::SQLError;
		using Poco::Data::ODBC::SQLExecDirect;
		using Poco::Data::ODBC::SQLForeignKeys;
		using Poco::Data::ODBC::SQLGetConnectAttr;
		using Poco::Data::ODBC::SQLGetConnectOption;
		using Poco::Data::ODBC::SQLGetCursorName;
		using Poco::Data::ODBC::SQLGetDescField;
		using Poco::Data::ODBC::SQLGetDescRec;
		using Poco::Data::ODBC::SQLGetDiagField;
		using Poco::Data::ODBC::SQLGetDiagRec;
		using Poco::Data::ODBC::SQLGetInfo;
		using Poco::Data::ODBC::SQLGetStmtAttr;
		using Poco::Data::ODBC::SQLGetTypeInfo;
		using Poco::Data::ODBC::SQLNativeSql;
		using Poco::Data::ODBC::SQLPrepare;
		using Poco::Data::ODBC::SQLPrimaryKeys;
		using Poco::Data::ODBC::SQLProcedureColumns;
		using Poco::Data::ODBC::SQLProcedures;
		using Poco::Data::ODBC::SQLSetConnectAttr;
		using Poco::Data::ODBC::SQLSetConnectOption;
		using Poco::Data::ODBC::SQLSetCursorName;
		using Poco::Data::ODBC::SQLSetDescField;
		using Poco::Data::ODBC::SQLSetStmtAttr;
		using Poco::Data::ODBC::SQLSpecialColumns;
		using Poco::Data::ODBC::SQLStatistics;
		using Poco::Data::ODBC::SQLTablePrivileges;
		using Poco::Data::ODBC::SQLTables;
		using Poco::Data::ODBC::isString;
		using Poco::Data::ODBC::stringLength;
	}

	namespace PostgreSQL {
		using Poco::Data::PostgreSQL::BinaryExtractor;
		using Poco::Data::PostgreSQL::Binder;
		using Poco::Data::PostgreSQL::ConnectionException;
		using Poco::Data::PostgreSQL::Connector;
		using Poco::Data::PostgreSQL::Extractor;
		using Poco::Data::PostgreSQL::InputParameter;
		using Poco::Data::PostgreSQL::OutputParameter;
		using Poco::Data::PostgreSQL::PGCancelFree;
		using Poco::Data::PostgreSQL::PQConnectionInfoOptionsFree;
		using Poco::Data::PostgreSQL::PQResultClear;
		using Poco::Data::PostgreSQL::PostgreSQLException;
		using Poco::Data::PostgreSQL::SessionHandle;
		using Poco::Data::PostgreSQL::SessionParameters;
		using Poco::Data::PostgreSQL::StatementException;
		using Poco::Data::PostgreSQL::StatementExecutor;
		using Poco::Data::PostgreSQL::TransactionException;
		using Poco::Data::PostgreSQL::Utility;

		#ifdef POCO_MODULES_EXPORT_IMPL
		using Poco::Data::PostgreSQL::PostgreSQLStatementImpl;
		using Poco::Data::PostgreSQL::SessionImpl;
		#endif

		using Poco::Data::PostgreSQL::InputParameterVector;
		using Poco::Data::PostgreSQL::OutputParameterVector;
		using Poco::Data::PostgreSQL::SessionParametersMap;
		
		using Poco::Data::PostgreSQL::oidToColumnDataType;
	}

	namespace SQLite {
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
		using Poco::Data::SQLite::SchemaDiffersException;
		using Poco::Data::SQLite::TableLockedException;
		using Poco::Data::SQLite::TableNotFoundException;
		using Poco::Data::SQLite::TransactionException;
		using Poco::Data::SQLite::Utility;

		#ifdef POCO_MODULES_EXPORT_IMPL
		using Poco::Data::SQLite::SQLiteStatementImpl;
		using Poco::Data::SQLite::SessionImpl;
		#endif

		using Poco::Data::SQLite::TransactionType;
	}

	namespace Test {
		using Poco::Data::Test::SQLExecutor;
	}
}
