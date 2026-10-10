//
// TimeServiceProxyFactory.cpp
//
// Package: Generated
// Module:  TimeServiceProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "TimeServiceProxyFactory.h"


namespace Services {


TimeServiceProxyFactory::TimeServiceProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


TimeServiceProxyFactory::~TimeServiceProxyFactory()
{
}


Poco::RemotingNG::Proxy* TimeServiceProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new Services::TimeServiceProxy(oid);
}


} // namespace Services

