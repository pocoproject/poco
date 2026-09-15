//
// JSONRPCRequestHandlerFactory.h
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  JSONRPCRequestHandlerFactory
//
// Definition of the JSONRPCRequestHandlerFactory class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_JSONRPC_JSONRPCRequestHandlerFactory_INCLUDED
#define RemotingNG_JSONRPC_JSONRPCRequestHandlerFactory_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class Listener;


class RemotingNGJSONRPC_API JSONRPCRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	/// A factory for JSONRPCRequestHandler objects.
{
public:
	JSONRPCRequestHandlerFactory(Listener& listener);
		/// Creates the JSONRPCRequestHandlerFactory.

	~JSONRPCRequestHandlerFactory();
		/// Destroys the JSONRPCRequestHandlerFactory.

	// Poco::Net::HTTPRequestHandlerFactory
	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

private:
	Listener& _listener;
};


} } } // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_JSONRPCRequestHandlerFactory_INCLUDED
