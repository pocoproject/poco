module;

#define ENABLE_DATA_POSTGRESQL

#ifdef ENABLE_DATA_POSTGRESQL
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
#endif

export module Poco.Data:PostgreSQL;

export namespace Poco::Data::PostgreSQL {
	#ifdef ENABLE_DATA_POSTGRESQL
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
	using Poco::Data::PostgreSQL::PostgreSQLStatementImpl;
	using Poco::Data::PostgreSQL::SessionHandle;
	using Poco::Data::PostgreSQL::SessionImpl;
	using Poco::Data::PostgreSQL::SessionParameters;
	using Poco::Data::PostgreSQL::StatementException;
	using Poco::Data::PostgreSQL::StatementExecutor;
	using Poco::Data::PostgreSQL::TransactionException;
	using Poco::Data::PostgreSQL::Utility;

	using Poco::Data::PostgreSQL::InputParameterVector;
	using Poco::Data::PostgreSQL::OutputParameterVector;
	using Poco::Data::PostgreSQL::SessionParametersMap;
	
	using Poco::Data::PostgreSQL::oidToColumnDataType;
	#endif
}
