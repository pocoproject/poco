module;

#define ENABLE_ACTIVERECORD

#ifdef ENABLE_ACTIVERECORD
#include "Poco/ActiveRecord/ActiveRecord.h"
#include "Poco/ActiveRecord/ActiveRecordLib.h"
#include "Poco/ActiveRecord/Context.h"
#include "Poco/ActiveRecord/IDTraits.h"
#include "Poco/ActiveRecord/Query.h"
#include "Poco/ActiveRecord/StatementPlaceholderProvider.h"
#endif

export module Poco.ActiveRecord;

export namespace Poco::ActiveRecord {
	#ifdef ENABLE_ACTIVERECORD
	using Poco::ActiveRecord::ActiveRecord;
	using Poco::ActiveRecord::ActiveRecordBase;
	using Poco::ActiveRecord::Context;
	using Poco::ActiveRecord::DefaultStatementPlaceholderProvider;
	using Poco::ActiveRecord::IDTraits;
	using Poco::ActiveRecord::KeylessActiveRecord;
	using Poco::ActiveRecord::PostgresStatementPlaceholderProvider;
	using Poco::ActiveRecord::Query;
	using Poco::ActiveRecord::StatementPlaceholderProvider;
	#endif
}
