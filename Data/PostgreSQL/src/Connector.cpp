//
// Connector.cpp
//
// $Id: //poco/1.4/Data/PostgreSQL/src/Connector.cpp#1 $
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


#include "Poco/Data/PostgreSQL/Connector.h"
#include "Poco/Data/PostgreSQL/SessionImpl.h"
#include "Poco/Data/SessionFactory.h"

const PostgreSQLConnectorRegistrator pocoPostgreSQLConnectorRegistrator;

namespace Poco {
namespace Data {
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

Poco::AutoPtr<Poco::Data::SessionImpl>
Connector::createSession(const std::string& aConnectionString,
						 std::size_t aTimeout)
{
	return Poco::AutoPtr<Poco::Data::SessionImpl>(new SessionImpl(aConnectionString, aTimeout));
}

void Connector::registerConnector()
{
	Poco::Data::SessionFactory::instance().add(new Connector());
}

void Connector::unregisterConnector()
{
	Poco::Data::SessionFactory::instance().remove(POCO_DATA_POSTGRESQL_CONNECTOR_NAME);
}


} } } // namespace Poco::Data::PostgreSQL

