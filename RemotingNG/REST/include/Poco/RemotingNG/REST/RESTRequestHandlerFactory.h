//
// RESTRequestHandlerFactory.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  RESTRequestHandlerFactory
//
// Definition of the RESTRequestHandlerFactory class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_RESTRequestHandlerFactory_INCLUDED
#define RemotingNG_REST_RESTRequestHandlerFactory_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


class Listener;


class RemotingNGREST_API RESTRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	/// A factory for RESTRequestHandler objects.
{
public:
	RESTRequestHandlerFactory(Listener& listener);
		/// Creates the RESTRequestHandlerFactory.

	~RESTRequestHandlerFactory();
		/// Destroys the RESTRequestHandlerFactory.

	// Poco::Net::HTTPRequestHandlerFactory
	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

private:
	Listener& _listener;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_RESTRequestHandlerFactory_INCLUDED
