//
// JSONRPCRequestHandler.h
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  JSONRPCRequestHandler
//
// Definition of the JSONRPCRequestHandler class.
//
// Copyright (c) 2006-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_JSONRPC_JSONRPCRequestHandler_INCLUDED
#define RemotingNG_JSONRPC_JSONRPCRequestHandler_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "Poco/RemotingNG/JSONRPC/Listener.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Logger.h"


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class RemotingNGJSONRPC_API JSONRPCRequestHandler: public Poco::Net::HTTPRequestHandler
	/// The HTTPRequestHandler implementation for RemotingNG JSONRPC.
{
public:
	JSONRPCRequestHandler(Listener& listener);
		/// Creates a JSONRPCRequestHandler, using the given Listener.

	~JSONRPCRequestHandler();
		/// Destroys the JSONRPCRequestHandler.

	// Poco::Net::HTTPRequestHandler
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

protected:
	bool handleCORS(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
	
private:
	JSONRPCRequestHandler();

	Listener& _listener;
	Poco::Logger& _logger;
};


} } } // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_JSONRPCRequestHandler_INCLUDED
