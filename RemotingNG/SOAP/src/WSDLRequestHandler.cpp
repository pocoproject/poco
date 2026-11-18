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
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/Net.h"
#include <string>
#include <string_view>


namespace Poco::RemotingNG::SOAP {


namespace
{
	// The file name comes from the request URI, so a character that a file name cannot contain is
	// refused before a Poco::Path or a Poco::File is built from it: a control character, which
	// includes the NUL that would end the name at the file system call, a path separator, and the
	// characters that are not allowed in a file name on Windows. With a WSDL directory given, a
	// colon names an alternate data stream of a file in that directory; it names another directory
	// only when the WSDL path is empty.
	bool isValidFileName(std::string_view name)
	{
		static constexpr std::string_view INVALID_CHARS("\\/:*?\"<>|");
		static constexpr unsigned char FIRST_PRINTABLE_CHAR = 0x20;

		if (name.empty()) return false;
		for (const char c: name)
		{
			if (static_cast<unsigned char>(c) < FIRST_PRINTABLE_CHAR) return false;
			if (INVALID_CHARS.find(c) != std::string_view::npos) return false;
		}
		return true;
	}


	void sendNotFound(Poco::Net::HTTPServerResponse& response, const std::string& fileName)
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
		response.setContentType("text/html");
		std::string message(
			"<html><head><title>WSDL Document Not Found</title></head>"
			"<body><h1>WSDL Document Not Found</h1>"
			"<p>No such file");
		if (!fileName.empty())
		{
			message += ": ";
			message += Poco::Net::htmlize(fileName);
		}
		message += "</p></body></html>";
		response.setContentLength(static_cast<int>(message.length()));
		response.setChunkedTransferEncoding(false);
		response.send() << message;
	}
}


WSDLRequestHandler::WSDLRequestHandler(const std::string& wsdlPath):
	_wsdlPath(wsdlPath)
{
}


WSDLRequestHandler::~WSDLRequestHandler() = default;


void WSDLRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	Poco::URI uri(request.getURI());
	const std::string& requestPath = uri.getPath();
	const std::string::size_type lastSlash = requestPath.rfind('/');
	const std::string requestedName(lastSlash == std::string::npos ? requestPath : requestPath.substr(lastSlash + 1));
	if (!isValidFileName(requestedName))
	{
		sendNotFound(response, std::string());
		return;
	}

	Poco::Path path(requestedName, Poco::Path::PATH_UNIX);
	path.setExtension("wsdl");
	const std::string fileName(path.getFileName());

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
	if (file.exists() && file.isFile())
	{
		response.sendFile(filePath, "text/xml");
	}
	else
	{
		sendNotFound(response, fileName);
	}
}


} // namespace Poco::RemotingNG::SOAP
