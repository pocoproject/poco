//
// MetricsRequestHandler.cpp
//
// Library: Prometheus
// Package: HTTP
// Module:  MetricsRequestHandler
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/MetricsRequestHandler.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/TextExporter.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/DeflatingStream.h"


using namespace std::string_literals;


namespace Poco {
namespace Prometheus {


MetricsRequestHandler::MetricsRequestHandler():
	_registry(Registry::defaultRegistry())
{
}


MetricsRequestHandler::MetricsRequestHandler(const Registry& registry):
	_registry(registry)
{
}


void MetricsRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET || request.getMethod() == Poco::Net::HTTPRequest::HTTP_HEAD)
	{
		response.setChunkedTransferEncoding(true);
		response.setContentType("text/plain; version=0.0.4"s);
		bool compressResponse(request.hasToken("Accept-Encoding"s, "gzip"s));
		if (compressResponse) response.set("Content-Encoding"s, "gzip"s);
		response.set("Cache-Control"s, "no-cache, no-store"s);
		std::ostream& plainResponseStream = response.send();
		if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET)
		{
			Poco::DeflatingOutputStream gzipStream(plainResponseStream, Poco::DeflatingStreamBuf::STREAM_GZIP, 1);
			std::ostream& responseStream = compressResponse ? gzipStream : plainResponseStream;
			TextExporter exporter(responseStream);
			_registry.exportTo(exporter);
		}
	}
	else
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
		response.setContentLength(0);
		response.send();
	}
}


} } // namespace Poco::Prometheus
