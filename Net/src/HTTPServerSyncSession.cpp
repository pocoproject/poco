//
// HTTPServerSyncSession.cpp
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerSyncSession
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPServerSyncSession.h"


namespace Poco {
namespace Net {


HTTPServerSyncSession::HTTPServerSyncSession(const StreamSocket& socket, HTTPServerParams::Ptr pParams):
	HTTPServerSession(socket, pParams),
	_firstRequest(true),
	_keepAliveTimeout(pParams->getKeepAliveTimeout()),
	_maxKeepAliveRequests(pParams->getMaxKeepAliveRequests())
{
	setTimeout(pParams->getTimeout());
	this->socket().setReceiveTimeout(pParams->getTimeout());
}


HTTPServerSyncSession::HTTPServerSyncSession(HTTPServerParams::Ptr pParams) :
	_firstRequest(true),
	_keepAliveTimeout(pParams->getKeepAliveTimeout()),
	_maxKeepAliveRequests(pParams->getMaxKeepAliveRequests())
{
	setTimeout(pParams->getTimeout());
	this->socket().setReceiveTimeout(pParams->getTimeout());
}


HTTPServerSyncSession::~HTTPServerSyncSession()
{
}


bool HTTPServerSyncSession::hasMoreRequests()
{
	if (!socket().impl()->initialized()) return false;

	if (_firstRequest)
	{
		_firstRequest = false;
		--_maxKeepAliveRequests;
		return socket().poll(getTimeout(), Socket::SELECT_READ);
	}
	else if (_maxKeepAliveRequests != 0 && getKeepAlive())
	{
		if (_maxKeepAliveRequests > 0) 
			--_maxKeepAliveRequests;
		return buffered() > 0 || socket().poll(_keepAliveTimeout, Socket::SELECT_READ);
	}
	else return false;
}

/*
SocketAddress HTTPServerSyncSession::clientAddress()
{
	return socket().peerAddress();
}


SocketAddress HTTPServerSyncSession::serverAddress()
{
	return socket().address();
}
*/

} } // namespace Poco::Net
