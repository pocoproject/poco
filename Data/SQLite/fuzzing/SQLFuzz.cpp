//
// SQLFuzz.cpp
//
// Feeds an SQL script to a Poco::Data SQLite session and walks the
// resulting RecordSet so Statement, Extractor and Var conversion run
// on attacker-controlled SQL.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/DataException.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/SQLite/Utility.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Exception.h"
#include "sqlite3.h"

#include <cstdint>
#include <string>


using namespace Poco::Data;
using Poco::Data::SQLite::Utility;


namespace
{
	constexpr std::size_t kMaxInputBytes = 4096;
	constexpr int kProgressOpsPerCallback = 10;
	constexpr unsigned kProgressStepBudget = 100000;
	constexpr int kLimitLength = 50000;
	constexpr int kLimitVdbeOp = 25000;
	constexpr int kLimitLikePattern = 250;
	constexpr sqlite3_int64 kHardHeapLimit = 20000000;


	int onProgress(void* pBudget)
		/// Aborts the statement when the remaining step budget reaches zero.
	{
		auto* left = static_cast<unsigned*>(pBudget);
		if (*left == 0)
			return 1;
		--(*left);
		return 0;
	}


	void boundSession(Session& session, unsigned& stepsLeft)
		/// Opens limits as SQLite's ossfuzz.c does, plus ATTACH disabled.
	{
		Utility::setAttachedLimit(session, 0);

		sqlite3* pDB = Utility::dbHandle(session);
		sqlite3_limit(pDB, SQLITE_LIMIT_LENGTH, kLimitLength);
		sqlite3_limit(pDB, SQLITE_LIMIT_VDBE_OP, kLimitVdbeOp);
		sqlite3_limit(pDB, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, kLimitLikePattern);
		sqlite3_hard_heap_limit64(kHardHeapLimit);

		stepsLeft = kProgressStepBudget;
		sqlite3_progress_handler(pDB, kProgressOpsPerCallback, onProgress, &stepsLeft);
	}


	void clearTempStoreDirectory(Session& session)
		/// Drops any temp_store_directory the script may have set.
	{
		try
		{
			Statement pragma(session);
			pragma << "PRAGMA temp_store_directory=''";
			pragma.execute();
		}
		catch (const DataException&)
		{
		}
		catch (const Poco::SyntaxException&)
		{
		}
		catch (const Poco::NotFoundException&)
		{
		}
	}


	void readRecordSet(Statement& stmt)
		/// Walks every row and column; non-NULL values go through Var to string.
	{
		RecordSet rs(stmt);
		const std::size_t rows = rs.rowCount();
		const std::size_t cols = rs.columnCount();
		for (std::size_t row = 0; row < rows; ++row)
		{
			for (std::size_t col = 0; col < cols; ++col)
			{
				if (rs.isNull(col, row))
					continue;
				const std::string value = rs.value(col, row).convert<std::string>();
				(void) value;
			}
		}
	}
}


extern "C" int LLVMFuzzerInitialize(int* /*argc*/, char*** /*argv*/)
{
	Poco::Data::SQLite::Connector::registerConnector();
	return 0;
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size == 0 || size > kMaxInputBytes)
		return 0;

	const std::string script(reinterpret_cast<const char*>(data), size);

	Session session("sqlite", ":memory:");
	unsigned stepsLeft = 0;
	boundSession(session, stepsLeft);

	try
	{
		Statement stmt(session);
		stmt << script;
		stmt.execute();
		readRecordSet(stmt);
	}
	catch (const DataException&)
	{
	}
	catch (const Poco::SyntaxException&)
	{
	}
	catch (const Poco::NotFoundException&)
	{
	}

	clearTempStoreDirectory(session);
	return 0;
}
