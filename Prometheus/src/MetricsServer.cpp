//
// MetricsServer.cpp
//
// Library: Prometheus
// Package: HTTP
// Module:  MetricsServer
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/MetricsServer.h"
#include "Poco/Prometheus/MetricsRequestHandler.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"


using namespace std::string_literals;


namespace Poco {
namespace Prometheus {


class NotFoundRequestHandler: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
		response.setChunkedTransferEncoding(true);
		response.setContentType("text/html"s);
		response.send()
			<< "<html>"
			<< "<head><title>404 - Not Found</title></head>"
			<< "<body><h1>Not Found</h1><p>The requested resource was not found.</p></body>"
			<< "</html>";
	}
};


class MetricsRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
public:
	MetricsRequestHandlerFactory(const Registry& registry, const std::string& path):
		_registry(registry),
		_path(path)
	{
	}

	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request)
	{
		if (request.getURI() == _path)
		{
			return new MetricsRequestHandler(_registry);
		}
		else
		{
			return new NotFoundRequestHandler;
		}
	}

private:
	const Registry& _registry;
	const std::string _path;
};


const Poco::UInt16 MetricsServer::DEFAULT_PORT{9100};
const std::string MetricsServer::DEFAULT_PATH{"/metrics"s};


MetricsServer::MetricsServer(Poco::UInt16 port, const std::string& path):
	_httpServer(new MetricsRequestHandlerFactory(Registry::defaultRegistry(), path), port, defaultParams())
{
}


MetricsServer::MetricsServer(const Registry& registry, Poco::UInt16 port, const std::string& path):
	_httpServer(new MetricsRequestHandlerFactory(registry, path), port, defaultParams())
{
}


MetricsServer::MetricsServer(const Registry& registry, Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pServerParams, const std::string& path):
	_httpServer(new MetricsRequestHandlerFactory(registry, path), socket, pServerParams)
{
}


void MetricsServer::start()
{
	_httpServer.start();
}


void MetricsServer::stop()
{
	_httpServer.stopAll();
}


Poco::Net::HTTPServerParams::Ptr MetricsServer::defaultParams()
{
	Poco::Net::HTTPServerParams::Ptr pParams = new Poco::Net::HTTPServerParams;
	pParams->setMaxQueued(8);
	pParams->setMaxThreads(2);
	pParams->setKeepAlive(false);
	return pParams;
}


} } // namespace Poco::Prometheus
