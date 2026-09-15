//
// HTTPRequestHandler.h
//
// Library: RemotingNG/HTTP
// Package: HTTP
// Module:  HTTPRequestHandler
//
// Definition of the HTTPRequestHandler class.
//
// Copyright (c) 2006-2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_HTTP_HTTPRequestHandler_INCLUDED
#define RemotingNG_HTTP_HTTPRequestHandler_INCLUDED


#include "Poco/RemotingNG/HTTP/HTTP.h"
#include "Poco/RemotingNG/HTTP/Listener.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Logger.h"


namespace Poco {
namespace RemotingNG {
namespace HTTP {


class RemotingNGHTTP_API HTTPRequestHandler: public Poco::Net::HTTPRequestHandler
	/// The HTTPRequestHandler implementation for RemotingNG HTTP.
{
public:
	HTTPRequestHandler(Listener& listener);
		/// Creates a HTTPRequestHandler, using the given Listener.

	~HTTPRequestHandler();
		/// Destroys the HTTPRequestHandler.

	// Poco::Net::HTTPRequestHandler
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
	
private:
	HTTPRequestHandler();

	Listener& _listener;
	Poco::Logger& _logger;
};


} } } // namespace Poco::RemotingNG::HTTP


#endif // RemotingNG_HTTP_HTTPRequestHandler_INCLUDED
