//
// Listener.cpp
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  Listener
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/SOAP/Listener.h"
#include "Poco/RemotingNG/SOAP/Transport.h"
#include "Poco/RemotingNG/SOAP/SOAPRequestHandler.h"
#include "Poco/RemotingNG/SOAP/WSDLRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"


namespace Poco {
namespace RemotingNG {
namespace SOAP {


class SOAPRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
public:
	SOAPRequestHandlerFactory(Listener& listener, const std::string& wsdlPath):
		_listener(listener),
		_wsdlPath(wsdlPath)
	{
	}
	
	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request)
	{
		if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
			return new SOAPRequestHandler(_listener);
		else
			return new WSDLRequestHandler(_wsdlPath);
	}
	
private:
	Listener& _listener;
	std::string _wsdlPath;
};


Listener::Listener(const std::string& endPoint, Protocol proto):
	Poco::RemotingNG::Listener(endPoint),
	_secure(proto == PROTO_HTTPS),
	_enableChunkedTransferEncoding(true),
	_enableCompression(true),
	_enableMTOM(false),
	_enableWSSE(false)
{
}


Listener::Listener(const std::string& endPoint, const std::string& wsdlPath, Poco::Net::HTTPServerParams::Ptr pParams):
	Poco::RemotingNG::Listener(endPoint),
	_secure(false),
	_enableChunkedTransferEncoding(true),
	_enableCompression(true),
	_enableMTOM(false),
	_enableWSSE(false)
{
	Poco::Net::SocketAddress addr(endPoint);
	Poco::Net::ServerSocket socket(addr);
	_pHTTPServer = new Poco::Net::HTTPServer(new SOAPRequestHandlerFactory(*this, wsdlPath), socket, pParams);
}


Listener::Listener(const std::string& endPoint, const std::string& wsdlPath, const Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pParams):
	Poco::RemotingNG::Listener(endPoint),
	_secure(socket.secure()),
	_enableChunkedTransferEncoding(true),
	_enableCompression(true),
	_enableMTOM(false),
	_enableWSSE(false)
{
	_pHTTPServer = new Poco::Net::HTTPServer(new SOAPRequestHandlerFactory(*this, wsdlPath), socket, pParams);
}


Listener::~Listener()
{
}


void Listener::enableChunkedTransferEncoding(bool enable)
{
	_enableChunkedTransferEncoding = enable;
}


void Listener::enableCompression(bool enable)
{
	_enableCompression = enable;
}


void Listener::enableMTOM(bool enable)
{
	_enableMTOM = enable;
}


void Listener::enableWSSE(bool enable)
{
	_enableWSSE = enable;
}


void Listener::addRequestHeaderInterceptor(RequestHeaderInterceptor::Ptr pInterceptor)
{
	_requestHeaderInterceptors.push_back(pInterceptor);
}


void Listener::removeRequestHeaderInterceptor(RequestHeaderInterceptor::Ptr pInterceptor)
{
	_requestHeaderInterceptors.erase(
		std::remove(_requestHeaderInterceptors.begin(), _requestHeaderInterceptors.end(), pInterceptor),
		_requestHeaderInterceptors.end()
	);
}


void Listener::start()
{
	if (_pHTTPServer) _pHTTPServer->start();
}


void Listener::stop()
{
	if (_pHTTPServer) _pHTTPServer->stop();
}


const std::string& Listener::protocol() const
{
	return Transport::PROTOCOL;
}


std::string Listener::createURI(const Poco::RemotingNG::Identifiable::TypeId& typeId, const Poco::RemotingNG::Identifiable::ObjectId& objectId)
{
	std::string uri(_secure ? "https://" : "http://");
	uri += endPoint();
	uri += '/';
	uri += protocol();
	uri += '/';
	uri += typeId;
	uri += '/';
	uri += objectId;
	return uri;
}


bool Listener::handlesURI(const std::string& /*uri*/)
{
	return false;
}


void Listener::registerObject(Poco::RemotingNG::RemoteObject::Ptr /*pRemoteObject*/, Poco::RemotingNG::Skeleton::Ptr /*pSkeleton*/)
{
}


void Listener::unregisterObject(Poco::RemotingNG::RemoteObject::Ptr /*pRemoteObject*/)
{
}


} } } // namespace Poco::RemotingNG::SOAP
