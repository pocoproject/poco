//
// Listener.cpp
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  Listener
//
// Copyright (c) 2006-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/JSONRPC/Listener.h"
#include "Poco/RemotingNG/JSONRPC/Transport.h"
#include "Poco/RemotingNG/JSONRPC/JSONRPCRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


Listener::Listener(const std::string& endPoint, Protocol proto):
	Poco::RemotingNG::Listener(endPoint),
	_secure(proto == PROTO_HTTPS),
	_enableChunkedTransferEncoding(true),
	_enableCompression(true),
	_enableCORS(false)
{
}


Listener::Listener(const std::string& endPoint, Poco::Net::HTTPServerParams::Ptr pParams):
	Poco::RemotingNG::Listener(endPoint),
	_secure(false),
	_enableChunkedTransferEncoding(true),
	_enableCompression(true),
	_enableCORS(false)
{
	Poco::Net::SocketAddress addr(endPoint);
	Poco::Net::ServerSocket socket(addr);
	_pHTTPServer = new Poco::Net::HTTPServer(new JSONRPCRequestHandlerFactory(*this), socket, pParams);
}


Listener::Listener(const std::string& endPoint, const Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pParams):
	Poco::RemotingNG::Listener(endPoint),
	_secure(socket.secure()),
	_enableChunkedTransferEncoding(true),
	_enableCompression(true),
	_enableCORS(false)
{
	_pHTTPServer = new Poco::Net::HTTPServer(new JSONRPCRequestHandlerFactory(*this), socket, pParams);
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


} } } // namespace Poco::RemotingNG::JSONRPC
