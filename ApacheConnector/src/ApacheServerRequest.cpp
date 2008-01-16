//
// ApacheServerRequest.cpp
//
// $Id: //poco/Main/ApacheConnector/src/ApacheServerRequest.cpp#9 $
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "ApacheServerRequest.h"
#include "ApacheServerResponse.h"
#include "ApacheRequestHandlerFactory.h"
#include "Poco/Exception.h"
#include <set>


ApacheServerRequest::ApacheServerRequest(
	ApacheRequestRec* pApacheRequest, 
	const char* serverName, 
	int serverPort, 
	const char* clientName, 
	int clientPort):
	_pApacheRequest(pApacheRequest),
    _pResponse(0),
    _pStream(new ApacheInputStream(_pApacheRequest)),
	_serverAddress(serverName, serverPort),
	_clientAddress(clientName, clientPort)
{
}


ApacheServerRequest::~ApacheServerRequest()
{
	delete _pStream;
}

	
const Poco::Net::HTTPServerParams& ApacheServerRequest::serverParams() const
{
	throw Poco::NotImplementedException("No HTTPServerParams available in Apache modules.");
}


Poco::Net::HTTPServerResponse& ApacheServerRequest::response() const
{
	return *_pResponse;
}


void ApacheServerRequest::setResponse(ApacheServerResponse* pResponse)
{
	_pResponse = pResponse;
}


bool ApacheServerRequest::expectContinue() const
{
	return false;
}
