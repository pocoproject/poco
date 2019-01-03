//
// Connector.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Connector.h"
#include "SessionImpl.h"
#include "Poco/SQL/SessionFactory.h"


namespace Poco {
namespace SQL {
namespace Test {


const std::string Connector::KEY("test");


Connector::Connector()
{
}


Connector::~Connector()
{
}


Poco::AutoPtr<Poco::SQL::SessionImpl> Connector::createSession(const std::string& connectionString,
		std::size_t timeout)
{
	return Poco::AutoPtr<Poco::SQL::SessionImpl>(new Poco::SQL::Test::SessionImpl(connectionString, timeout));
}


void Connector::addToFactory()
{
	Poco::SQL::SessionFactory::instance().add(new Connector());
}


void Connector::removeFromFactory()
{
	Poco::SQL::SessionFactory::instance().remove(KEY);
}


} } } // namespace Poco::SQL::Test
