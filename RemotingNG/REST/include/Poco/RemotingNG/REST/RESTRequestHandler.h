//
// RESTRequestHandler.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  RESTRequestHandler
//
// Definition of the RESTRequestHandler class.
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_REST_RESTRequestHandler_INCLUDED
#define RemotingNG_REST_RESTRequestHandler_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/Listener.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Logger.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API RESTRequestHandler: public Poco::Net::HTTPRequestHandler
	/// The HTTPRequestHandler implementation for RemotingNG REST.
{
public:
	RESTRequestHandler(Listener& listener);
		/// Creates a RESTRequestHandler, using the given Listener.

	~RESTRequestHandler();
		/// Destroys the RESTRequestHandler.

	// Poco::Net::HTTPRequestHandler
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

protected:
	bool handleCORS(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
	
private:
	RESTRequestHandler();

	Listener& _listener;
	Poco::Logger& _logger;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_RESTRequestHandler_INCLUDED
