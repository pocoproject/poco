//
// StatementPlaceholderProvider.h
//
// Library: ActiveRecord
// Package: ActiveRecord
// Module:  StatementPlaceholderProvider
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecord_StatementPlaceholderProvider_INCLUDED
#define ActiveRecord_StatementPlaceholderProvider_INCLUDED


#include "Poco/ActiveRecord/ActiveRecordLib.h"
#include <string>
#include <memory>


namespace Poco {
namespace ActiveRecord {


class ActiveRecordLib_API StatementPlaceholderProvider
{
public:
	using Ptr = std::unique_ptr<StatementPlaceholderProvider>;

	virtual void reset() = 0;
	virtual std::string next() = 0;

	virtual ~StatementPlaceholderProvider();
};


class ActiveRecordLib_API DefaultStatementPlaceholderProvider: public StatementPlaceholderProvider
{
public:
	void reset() override;
	std::string next() override;
};


class ActiveRecordLib_API PostgresStatementPlaceholderProvider: public StatementPlaceholderProvider
{
public:
	void reset() override;
	std::string next() override;

private:
	int _n = 1;
};


} } // namespace Poco::ActiveRecord


#endif // ActiveRecord_StatementPlaceholderProvider_INCLUDED
