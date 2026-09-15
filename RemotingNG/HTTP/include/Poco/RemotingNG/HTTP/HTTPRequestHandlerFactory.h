//
// HTTPRequestHandlerFactory.h
//
// Library: RemotingNG/HTTP
// Package: HTTP
// Module:  HTTPRequestHandlerFactory
//
// Definition of the HTTPRequestHandlerFactory class.
//
// Copyright (c) 2010-2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_HTTP_HTTPRequestHandlerFactory_INCLUDED
#define RemotingNG_HTTP_HTTPRequestHandlerFactory_INCLUDED


#include "Poco/RemotingNG/HTTP/HTTP.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"


namespace Poco {
namespace RemotingNG {
namespace HTTP {


class Listener;


class RemotingNGHTTP_API HTTPRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	/// A factory for HTTPRequestHandler objects.
{
public:
	HTTPRequestHandlerFactory(Listener& listener);
		/// Creates the HTTPRequestHandlerFactory.

	~HTTPRequestHandlerFactory();
		/// Destroys the HTTPRequestHandlerFactory.

	// Poco::Net::HTTPRequestHandlerFactory
	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

private:
	Listener& _listener;
};


} } } // namespace Poco::RemotingNG::HTTP


#endif // RemotingNG_HTTP_HTTPRequestHandlerFactory_INCLUDED
