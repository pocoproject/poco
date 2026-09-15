//
// JSONRPCRequestHandlerFactory.cpp
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  JSONRPCRequestHandlerFactory
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/JSONRPC/JSONRPCRequestHandlerFactory.h"
#include "Poco/RemotingNG/JSONRPC/JSONRPCRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


JSONRPCRequestHandlerFactory::JSONRPCRequestHandlerFactory(Listener& listener):
	_listener(listener)
{
}


JSONRPCRequestHandlerFactory::~JSONRPCRequestHandlerFactory()
{
}


Poco::Net::HTTPRequestHandler* JSONRPCRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
	return new JSONRPCRequestHandler(_listener);
}


} } } // namespace Poco::RemotingNG::JSONRPC
