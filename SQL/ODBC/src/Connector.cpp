//
// Connector.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Connector
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/ODBC/Connector.h"
#include "Poco/SQL/ODBC/SessionImpl.h"
#include "Poco/SQL/SessionFactory.h"


const ODBCConnectorRegistrator pocoODBCConnectorRegistrator;


namespace Poco {
namespace SQL {
namespace ODBC {


const std::string Connector::KEY(POCO_DATA_ODBC_CONNECTOR_NAME);


Connector::Connector()
{
}


Connector::~Connector()
{
}


Poco::AutoPtr<Poco::SQL::SessionImpl> Connector::createSession(const std::string& connectionString,
	std::size_t timeout)
{
	return Poco::AutoPtr<Poco::SQL::SessionImpl>(new SessionImpl(connectionString, timeout));
}


void Connector::registerConnector()
{
	Poco::SQL::SessionFactory::instance().add(new Connector());
}


void Connector::unregisterConnector()
{
	Poco::SQL::SessionFactory::instance().remove(POCO_DATA_ODBC_CONNECTOR_NAME);
}


} } } // namespace Poco::SQL::ODBC
