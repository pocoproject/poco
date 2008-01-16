//
// ApacheServerResponse.cpp
//
// $Id: //poco/Main/ApacheConnector/src/ApacheServerResponse.cpp#8 $
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "ApacheServerResponse.h"
#include "ApacheServerRequest.h"
#include "ApacheStream.h"
#include "ApacheConnector.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/File.h"
#include "Poco/Exception.h"
#include <fstream>
#include <vector>


using Poco::File;
using Poco::OpenFileException;
using Poco::Net::HTTPCookie;


ApacheServerResponse::ApacheServerResponse(ApacheServerRequest* pRequest):
	_pStream(0),
	_pApacheRequest(pRequest->_pApacheRequest)
{
	setVersion(pRequest->getVersion());
	setKeepAlive(pRequest->getKeepAlive());

	pRequest->setResponse(this);
}


ApacheServerResponse::~ApacheServerResponse()
{
	delete _pStream;
}


void ApacheServerResponse::initApacheOutputStream()
{
	poco_assert (!_pStream);

	_pApacheRequest->setContentType(getContentType());

	std::vector<HTTPCookie> cookies;
	getCookies(cookies);
	
	std::size_t cnt = cookies.size();
	for (int c = 0; c < cnt; c++)
	{
		_pApacheRequest->addHeader("Set-Cookie", cookies[c].toString());
	}

	_pStream = new ApacheOutputStream(_pApacheRequest);
}


void ApacheServerResponse::sendContinue()
{
	// should be handled by Apache
}


std::ostream& ApacheServerResponse::send()
{
	poco_assert (!_pStream);
		
	initApacheOutputStream();

	return *_pStream;
}


void ApacheServerResponse::sendFile(const std::string& path, const std::string& mediaType)
{
	poco_assert (!_pStream);

	initApacheOutputStream();

	File f(path);
	if (_pApacheRequest->sendFile(path, static_cast<unsigned int>(f.getSize()), mediaType) != 0)
		throw OpenFileException(path);
}


void ApacheServerResponse::sendBuffer(const void* pBuffer, std::size_t length)
{
	poco_assert (!_pStream);

	initApacheOutputStream();

	_pStream->write(static_cast<const char*>(pBuffer), static_cast<std::streamsize>(length));
}


void ApacheServerResponse::redirect(const std::string& uri)
{
	poco_assert (!_pStream);

	initApacheOutputStream();

	try
	{
		_pApacheRequest->redirect(uri);
	}
	catch (Poco::Exception&)
	{
		ApacheConnector::log(__FILE__, __LINE__, 7 , 0, "caught exception in ApacheServerResponse::redirect - ignoring\n");
	}
}


void ApacheServerResponse::sendErrorResponse(int status)
{		
	initApacheOutputStream();

	_pApacheRequest->sendErrorResponse(status);
}


void ApacheServerResponse::requireAuthentication(const std::string& realm)
{
	// should be handled by Apache
}
