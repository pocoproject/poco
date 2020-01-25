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


#include "Poco/Data/PostgreSQL/Connector.h"
#include "Poco/Data/PostgreSQL/SessionImpl.h"
#include "Poco/Data/SessionFactory.h"


namespace Poco {
namespace Data {
namespace PostgreSQL {


const std::string Connector::KEY("postgresql");


Connector::Connector()
{
}


Connector::~Connector()
{
}


const std::string& Connector::name() const
{
	return KEY;
}


SessionImpl::Ptr Connector::createSession(const std::string& aConnectionString, std::size_t aTimeout)
{
	return Poco::AutoPtr<Poco::Data::SessionImpl>(new SessionImpl(aConnectionString, aTimeout));
}


void Connector::registerConnector()
{
	Poco::Data::SessionFactory::instance().add(new Connector());
}


void Connector::unregisterConnector()
{
	Poco::Data::SessionFactory::instance().remove(KEY);
}


} } } // namespace Poco::Data::PostgreSQL
