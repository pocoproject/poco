//
// SOAPRequestHandler.h
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  SOAPRequestHandler
//
// Definition of the SOAPRequestHandler class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_SOAP_SOAPRequestHandler_INCLUDED
#define RemotingNG_SOAP_SOAPRequestHandler_INCLUDED


#include "Poco/RemotingNG/SOAP/SOAP.h"
#include "Poco/RemotingNG/SOAP/Listener.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Logger.h"


namespace Poco::RemotingNG::SOAP {


class RemotingNGSOAP_API SOAPRequestHandler: public Poco::Net::HTTPRequestHandler
	/// The SOAPRequestHandler implementation for RemotingNG SOAP.
{
public:
	SOAPRequestHandler(Listener& listener);
		/// Creates a SOAPRequestHandler, using the given Listener.

	~SOAPRequestHandler() override;
		/// Destroys the SOAPRequestHandler.

	SOAPRequestHandler() = delete;

	// Poco::Net::HTTPSOAPRequestHandler
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

protected:
	static const std::string SOAPACTION;
	static const std::string EMPTY;
	
private:
	Listener& _listener;
	Poco::Logger& _logger;
};


} // namespace Poco::RemotingNG::SOAP


#endif // RemotingNG_SOAP_SOAPRequestHandler_INCLUDED
