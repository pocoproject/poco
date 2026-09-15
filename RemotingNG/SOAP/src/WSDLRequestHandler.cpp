//
// WSDLRequestHandler.cpp
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  WSDLRequestHandler
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/SOAP/WSDLRequestHandler.h"
#include "Poco/URI.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"


namespace Poco {
namespace RemotingNG {
namespace SOAP {


WSDLRequestHandler::WSDLRequestHandler(const std::string& wsdlPath):
	_wsdlPath(wsdlPath)
{
}


WSDLRequestHandler::~WSDLRequestHandler()
{
}


void WSDLRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	Poco::URI uri(request.getURI());
	Poco::Path path(uri.getPath(), Poco::Path::PATH_UNIX);
	path.setExtension("wsdl");
	std::string fileName(path.getFileName());
		
	std::string filePath;
	if (_wsdlPath.empty())
	{
		filePath = fileName;
	}
	else
	{
		Poco::Path p(_wsdlPath);
		p.makeDirectory();
		p.setFileName(fileName);
		filePath = p.toString();
	}

	Poco::File file(filePath);	
	if (file.exists())
	{
		response.sendFile(filePath, "text/xml");
	}
	else
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
		std::string message(
			"<html><head><title>WSDL Document Not Found</title></head>"
			"<body><h1>WSDL Document Not Found</h1>"
			"<p>No such file: ");
		message += filePath;
		message += "</p></body></html>";
		response.setContentLength(static_cast<int>(message.length()));
		response.setChunkedTransferEncoding(false);
		response.send() << message;
	}
}


} } } // namespace Poco::RemotingNG::SOAP
