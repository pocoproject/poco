//
// StatementPlaceholderProvider.cpp
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


#include "Poco/ActiveRecord/StatementPlaceholderProvider.h"
#include "Poco/Format.h"


using namespace std::string_literals;


namespace Poco {
namespace ActiveRecord {


StatementPlaceholderProvider::~StatementPlaceholderProvider()
{
}


void DefaultStatementPlaceholderProvider::reset()
{
}


std::string DefaultStatementPlaceholderProvider::next()
{
	return "?"s;
}


void PostgresStatementPlaceholderProvider::reset()
{
	_n = 1;
}


std::string PostgresStatementPlaceholderProvider::next()
{
	return Poco::format("$%d"s, _n++);
}


} } // namespace Poco::ActiveRecord
