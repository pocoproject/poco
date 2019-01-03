//
// MySQLException.cpp
//
// Library: Data/MySQL
// Package: MySQL
// Module:  Connector
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/MySQL/Connector.h"
#include "Poco/SQL/MySQL/SessionImpl.h"
#include "Poco/SQL/SessionFactory.h"
#include "Poco/Exception.h"
#include <mysql.h>


const MySQLConnectorRegistrator pocoMySQLConnectorRegistrator;


namespace Poco {
namespace SQL {
namespace MySQL {


std::string Connector::KEY(POCO_DATA_MYSQL_CONNECTOR_NAME);
Poco::FastMutex Connector::_mutex;


Connector::Connector()
{
}


Connector::~Connector()
{
}

const std::string& Connector::name() const
{
	static const std::string n(POCO_DATA_MYSQL_CONNECTOR_NAME);
	return n;
}

Poco::AutoPtr<Poco::SQL::SessionImpl> Connector::createSession(const std::string& connectionString,
	std::size_t timeout)
{
	static bool initDone = false;
	{
		Poco::FastMutex::ScopedLock l(_mutex);
		if (!initDone)
		{
			if (mysql_library_init(0, 0, 0) != 0)
			{
				throw Exception("mysql_library_init error");
			}
			initDone = true;
		}
	}

	return Poco::AutoPtr<Poco::SQL::SessionImpl>(new Poco::SQL::MySQL::SessionImpl(connectionString, timeout));
}


void Connector::registerConnector()
{
	Poco::SQL::SessionFactory::instance().add(new Connector());
}


void Connector::unregisterConnector()
{
	Poco::SQL::SessionFactory::instance().remove(POCO_DATA_MYSQL_CONNECTOR_NAME);
	mysql_library_end();
}


} } } // namespace Poco::SQL::MySQL

