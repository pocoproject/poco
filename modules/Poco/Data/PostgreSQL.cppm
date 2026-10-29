//
// PostgreSQL.cppm
//
// C++ module file
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_DATA_POSTGRESQL
#include "Poco/Data/PostgreSQL/BinaryExtractor.h"
#include "Poco/Data/PostgreSQL/Binder.h"
#include "Poco/Data/PostgreSQL/Connector.h"
#include "Poco/Data/PostgreSQL/Extractor.h"
#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/PostgreSQL/PostgreSQLException.h"
#include "Poco/Data/PostgreSQL/PostgreSQLStatementImpl.h"
#include "Poco/Data/PostgreSQL/PostgreSQLTypes.h"
#include "Poco/Data/PostgreSQL/SessionHandle.h"
#include "Poco/Data/PostgreSQL/SessionImpl.h"
#include "Poco/Data/PostgreSQL/StatementExecutor.h"
#include "Poco/Data/PostgreSQL/Utility.h"
#endif

export module Poco:Data.PostgreSQL;

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

	using Poco::Data::PostgreSQL::BITOID;
	using Poco::Data::PostgreSQL::BOOLOID;
	using Poco::Data::PostgreSQL::BPCHAROID;
	using Poco::Data::PostgreSQL::BYTEAOID;
	using Poco::Data::PostgreSQL::CASHOID;
	using Poco::Data::PostgreSQL::CHAROID;
	using Poco::Data::PostgreSQL::DATEOID;
	using Poco::Data::PostgreSQL::FLOAT4OID;
	using Poco::Data::PostgreSQL::FLOAT8OID;
	using Poco::Data::PostgreSQL::INT2OID;
	using Poco::Data::PostgreSQL::INT4OID;
	using Poco::Data::PostgreSQL::INT8OID;
	using Poco::Data::PostgreSQL::INVALIDOID;
	using Poco::Data::PostgreSQL::MACADDROID;
	using Poco::Data::PostgreSQL::NUMERICOID;
	using Poco::Data::PostgreSQL::TEXTOID;
	using Poco::Data::PostgreSQL::TIMEOID;
	using Poco::Data::PostgreSQL::TIMESTAMPOID;
	using Poco::Data::PostgreSQL::TIMESTAMPTZOID;
	using Poco::Data::PostgreSQL::TIMETZOID;
	using Poco::Data::PostgreSQL::UUIDOID;
	using Poco::Data::PostgreSQL::VARCHAROID;
	using Poco::Data::PostgreSQL::VARYBITOID;
	
	using Poco::Data::PostgreSQL::oidToColumnDataType;
	#endif
}
