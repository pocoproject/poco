module;

#define ENABLE_DATA

#ifdef ENABLE_DATA
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
#endif

export module Poco.Data;

export import :MySQL;
export import :ODBC;
export import :PostgreSQL;
export import :SQLite;

export namespace Poco::Data {
	#ifdef ENABLE_DATA
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

	namespace Test {
		// using Poco::Data::Test::SQLExecutor;
	}
	#endif
}
