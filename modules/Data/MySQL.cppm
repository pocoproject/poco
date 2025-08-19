module;

#define ENABLE_DATA_MYSQL

#ifdef ENABLE_DATA_MYSQL
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
#endif

export module Poco.Data:MySQL;

export namespace Poco::Data::MySQL {
	#ifdef ENABLE_DATA_MYSQL
	using Poco::Data::MySQL::Binder;
	using Poco::Data::MySQL::ConnectionException;
	using Poco::Data::MySQL::Connector;
	using Poco::Data::MySQL::Extractor;
	using Poco::Data::MySQL::MySQLException;
	using Poco::Data::MySQL::MySQLStatementImpl;
	using Poco::Data::MySQL::ResultMetadata;
	using Poco::Data::MySQL::SessionHandle;
	using Poco::Data::MySQL::SessionImpl;
	using Poco::Data::MySQL::StatementException;
	using Poco::Data::MySQL::StatementExecutor;
	using Poco::Data::MySQL::TransactionException;
	using Poco::Data::MySQL::Utility;
	#endif
}
