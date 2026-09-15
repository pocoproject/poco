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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
