//
// Connector.cpp
//
// Library: Data/SQLite
// Package: SQLite
// Module:  Connector
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/SQLite/Connector.h"
#include "Poco/SQL/SQLite/SessionImpl.h"
#include "Poco/SQL/SessionFactory.h"
#if defined(POCO_UNBUNDLED)
#include <sqlite3.h>
#else
#include "sqlite3.h"
#endif


const SQLiteConnectorRegistrator pocoSQLiteConnectorRegistrator;


namespace Poco {
namespace SQL {
namespace SQLite {


const std::string Connector::KEY(POCO_DATA_SQLITE_CONNECTOR_NAME);


Connector::Connector()
{
}


Connector::~Connector()
{
}


Poco::AutoPtr<Poco::SQL::SessionImpl> Connector::createSession(const std::string& connectionString,
	std::size_t timeout)
{
	return Poco::AutoPtr<Poco::SQL::SessionImpl>(new Poco::SQL::SQLite::SessionImpl(connectionString, timeout));
}


void Connector::registerConnector()
{
	Poco::SQL::SessionFactory::instance().add(new Connector());
}


void Connector::unregisterConnector()
{
	Poco::SQL::SessionFactory::instance().remove(POCO_DATA_SQLITE_CONNECTOR_NAME);
}


void Connector::enableSharedCache(bool flag)
{
	sqlite3_enable_shared_cache(flag ? 1 : 0);
}


void Connector::enableSoftHeapLimit(int limit)
{
	sqlite3_soft_heap_limit(limit);
}


} } } // namespace Poco::SQL::SQLite
