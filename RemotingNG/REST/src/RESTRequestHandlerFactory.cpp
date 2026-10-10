//
// RESTRequestHandlerFactory.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  RESTRequestHandlerFactory
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/RESTRequestHandlerFactory.h"
#include "Poco/RemotingNG/REST/RESTRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


RESTRequestHandlerFactory::RESTRequestHandlerFactory(Listener& listener):
	_listener(listener)
{
}


RESTRequestHandlerFactory::~RESTRequestHandlerFactory()
{
}


Poco::Net::HTTPRequestHandler* RESTRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
	return new RESTRequestHandler(_listener);
}


} } } // namespace Poco::RemotingNG::REST
