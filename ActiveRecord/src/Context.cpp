//
// Context.cpp
//
// Library: ActiveRecord
// Package: ActiveRecord
// Module:  Context
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/ActiveRecord/Context.h"


using namespace std::string_literals;


namespace Poco {
namespace ActiveRecord {


Context::Context(const Poco::Data::Session& session):
	_session(session)
{
}


Context::Context(const std::string& connector, const std::string& connectionString):
	_session(connector, connectionString)
{
}


StatementPlaceholderProvider::Ptr Context::statementPlaceholderProvider() const
{
	if (Poco::icompare(_session.connector(), "postgresql"s) == 0)
		return std::make_unique<PostgresStatementPlaceholderProvider>();
	else
		return std::make_unique<DefaultStatementPlaceholderProvider>();
}


} } // namespace Poco::ActiveRecord
