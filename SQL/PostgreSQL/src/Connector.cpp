//
// Connector.cpp
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  Connector
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/PostgreSQL/Connector.h"
#include "Poco/SQL/PostgreSQL/SessionImpl.h"
#include "Poco/SQL/SessionFactory.h"


const PostgreSQLConnectorRegistrator pocoPostgreSQLConnectorRegistrator;


namespace Poco {
namespace SQL {
namespace PostgreSQL {

std::string Connector::KEY = POCO_DATA_POSTGRESQL_CONNECTOR_NAME;


Connector::Connector()
{
}


Connector::~Connector()
{
}

const std::string&
Connector::name() const
{
	static const std::string n(POCO_DATA_POSTGRESQL_CONNECTOR_NAME);
	return n;
}

SessionImpl::Ptr Connector::createSession(const std::string& aConnectionString, std::size_t aTimeout)
{
	return Poco::AutoPtr<Poco::SQL::SessionImpl>(new SessionImpl(aConnectionString, aTimeout));
}

void Connector::registerConnector()
{
	Poco::SQL::SessionFactory::instance().add(new Connector());
}

void Connector::unregisterConnector()
{
	Poco::SQL::SessionFactory::instance().remove(POCO_DATA_POSTGRESQL_CONNECTOR_NAME);
}


} } } // namespace Poco::SQL::PostgreSQL

