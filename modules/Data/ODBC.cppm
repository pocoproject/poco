module;

#define ENABLE_DATA_ODBC

#ifdef ENABLE_DATA_ODBC
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
#endif

export module Poco.Data:ODBC;

export namespace Poco::Data::ODBC {
	#ifdef ENABLE_DATA_ODBC
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
	using Poco::Data::ODBC::ODBCStatementImpl;
	using Poco::Data::ODBC::Parameter;
	using Poco::Data::ODBC::Preparator;
	using Poco::Data::ODBC::SessionImpl;
	using Poco::Data::ODBC::TypeInfo;
	using Poco::Data::ODBC::UnknownDataLengthException;
	using Poco::Data::ODBC::Utility;

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
	#endif
}
