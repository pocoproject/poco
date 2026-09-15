//
// Listener.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  Listener
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/Listener.h"
#include "Poco/RemotingNG/REST/Transport.h"
#include "Poco/RemotingNG/REST/RESTRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Exception.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


const std::string Listener::CORS_ALLOWED_HEADERS("Accept, Content-Type");


Listener::Listener(const std::string& endPoint, Protocol proto):
	Poco::RemotingNG::Listener(endPoint),
	_secure(proto == PROTO_HTTPS),
	_logServiceExceptions(false),
	_enableChunkedTransferEncoding(false),
	_enableCompression(true),
	_enableCORS(false),
	_corsAllowedHeaders(CORS_ALLOWED_HEADERS)
{
}


Listener::Listener(const std::string& endPoint, Poco::Net::HTTPServerParams::Ptr pParams):
	Poco::RemotingNG::Listener(endPoint),
	_secure(false),
	_logServiceExceptions(false),
	_enableChunkedTransferEncoding(false),
	_enableCompression(true),
	_enableCORS(false),
	_corsAllowedHeaders(CORS_ALLOWED_HEADERS)
{
	Poco::Net::SocketAddress addr(endPoint);
	Poco::Net::ServerSocket socket(addr);
	_pHTTPServer = new Poco::Net::HTTPServer(new RESTRequestHandlerFactory(*this), socket, pParams);
}


Listener::Listener(const std::string& endPoint, const Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pParams):
	Poco::RemotingNG::Listener(endPoint),
	_secure(socket.secure()),
	_logServiceExceptions(false),
	_enableChunkedTransferEncoding(false),
	_enableCompression(true),
	_enableCORS(false),
	_corsAllowedHeaders(CORS_ALLOWED_HEADERS)
{
	_pHTTPServer = new Poco::Net::HTTPServer(new RESTRequestHandlerFactory(*this), socket, pParams);
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


void Listener::enableCORS(bool enable)
{
	_enableCORS = enable;
}


void Listener::setCORSAllowedOrigin(const std::string& origin)
{
	_corsAllowedOrigin = origin;
}


void Listener::setCORSAllowedHeaders(const std::string& headers)
{
	if (headers.find("Content-Type") == std::string::npos)
		throw Poco::InvalidArgumentException("CORS allowed headers must include Content-Type");

	_corsAllowedHeaders = headers;
}


void Listener::logServiceExceptions(bool enable)
{
	_logServiceExceptions = enable;
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


} } } // namespace Poco::RemotingNG::REST
