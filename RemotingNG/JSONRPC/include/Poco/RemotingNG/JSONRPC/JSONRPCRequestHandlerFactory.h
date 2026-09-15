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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
