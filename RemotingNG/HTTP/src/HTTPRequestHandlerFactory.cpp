//
// HTTPRequestHandlerFactory.cpp
//
// Library: RemotingNG/HTTP
// Package: HTTP
// Module:  HTTPRequestHandlerFactory
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/HTTP/HTTPRequestHandlerFactory.h"
#include "Poco/RemotingNG/HTTP/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"


namespace Poco {
namespace RemotingNG {
namespace HTTP {


HTTPRequestHandlerFactory::HTTPRequestHandlerFactory(Listener& listener):
	_listener(listener)
{
}


HTTPRequestHandlerFactory::~HTTPRequestHandlerFactory()
{
}


Poco::Net::HTTPRequestHandler* HTTPRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
	return new HTTPRequestHandler(_listener);
}


} } } // namespace Poco::RemotingNG::HTTP
