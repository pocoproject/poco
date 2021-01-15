//
// Connector.cpp
//
// Library: Data/SQLiteBDB
// Package: SQLiteBDB
// Module:  Connector
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/SQLiteBDB/Connector.h"
#include "Poco/Data/SQLiteBDB/SessionImpl.h"
#include "Poco/Data/SessionFactory.h"
#include "dbsql.h"


namespace Poco {
namespace Data {
namespace SQLiteBDB {


const std::string Connector::KEY("sqlitebdb");


Connector::Connector()
{
}


Connector::~Connector()
{
}


Poco::AutoPtr<Poco::Data::SessionImpl> Connector::createSession(const std::string& connectionString,
	std::size_t timeout)
{
	return Poco::AutoPtr<Poco::Data::SessionImpl>(new SessionImpl(connectionString, timeout));
}


void Connector::registerConnector()
{
	Poco::Data::SessionFactory::instance().add(new Connector());
}


void Connector::unregisterConnector()
{
	Poco::Data::SessionFactory::instance().remove(KEY);
}


void Connector::enableSharedCache(bool flag)
{
	sqlite3_enable_shared_cache(flag ? 1 : 0);
}


void Connector::enableSoftHeapLimit(int limit)
{
	sqlite3_soft_heap_limit(limit);
}


} } } // namespace Poco::Data::SQLiteBDB
