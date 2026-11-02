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
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/DataException.h"


namespace Poco::Data::Test {


const std::string Connector::KEY("test");
const std::string Connector::UNREACHABLE("unreachable");
const std::string Connector::REJECTING("rejecting");


Connector::Connector()
{
}


Connector::~Connector()
{
}


Poco::AutoPtr<Poco::Data::SessionImpl> Connector::createSession(const std::string& connectionString,
		std::size_t timeout)
{
	if (connectionString == UNREACHABLE)
		throw Poco::Data::ConnectionFailedException(connectionString);

	Poco::AutoPtr<Poco::Data::SessionImpl> pImpl(new SessionImpl(connectionString, timeout));
	if (connectionString == REJECTING)
		pImpl->setFeature("throwOnHasNext", true);
	return pImpl;
}


void Connector::addToFactory()
{
	Poco::Data::SessionFactory::instance().add(new Connector());
}


void Connector::removeFromFactory()
{
	Poco::Data::SessionFactory::instance().remove(KEY);
}


} // namespace Poco::Data::Test
