//
// TimeServer.cpp
//
// $Id: //poco/Main/ApacheConnector/samples/TimeServer/src/TimeServer.cpp#1 $
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/ClassLibrary.h"


using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Timestamp;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;


class TimeRequestHandler: public HTTPRequestHandler
	/// Return a HTML document with the current date and time.
{
public:
	TimeRequestHandler() 
	{
	}
	
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		Timestamp now;
		std::string dt(DateTimeFormatter::format(now, DateTimeFormat::SORTABLE_FORMAT));

		response.setChunkedTransferEncoding(true);
		response.setContentType("text/html");

		std::ostream& ostr = response.send();
		ostr << "<html><head><title>TimeServer powered by POCO ApacheConnector</title>";
		ostr << "<meta http-equiv=\"refresh\" content=\"1\"></head>";
		ostr << "<body><p style=\"text-align: center; font-size: 48px;\">";
		ostr << dt;
		ostr << "</p></body></html>";
	}
};


class TimeRequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
	TimeRequestHandlerFactory()
	{
	}

	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		return new TimeRequestHandler;
	}
};


POCO_BEGIN_MANIFEST(HTTPRequestHandlerFactory)
	POCO_EXPORT_CLASS(TimeRequestHandlerFactory)
POCO_END_MANIFEST
