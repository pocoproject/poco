//
// HTTPServerRequestImpl.cpp
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerRequestImpl
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPServerResponseImpl.h"
#include "Poco/Net/HTTPServerSession.h"
#include "Poco/Net/HTTPHeaderStream.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/HTTPStream.h"
#include "Poco/Net/HTTPFixedLengthStream.h"
#include "Poco/Net/HTTPChunkedStream.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/String.h"


using Poco::icompare;


namespace Poco {
namespace Net {


HTTPServerRequestImpl::HTTPServerRequestImpl(HTTPServerResponseImpl& response, HTTPSession& session, HTTPServerParams* pParams):
	_response(response),
	_session(session),
	_pStream(nullptr),
	_pParams(pParams, true)
{
	response.attachRequest(this);

	HTTPHeaderInputStream hs(session);
	setAutoDecode(_pParams->getAutoDecodeHeaders());
	read(hs);

	// Now that we know socket is still connected, obtain addresses
	_clientAddress = session.clientAddress();
	_serverAddress = session.serverAddress();

	if (getChunkedTransferEncoding())
		_pStream = new HTTPChunkedInputStream(session, &session.requestTrailer());
	else if (hasContentLength())
#if defined(POCO_HAVE_INT64)
		_pStream = new HTTPFixedLengthInputStream(session, getContentLength64());
#else
		_pStream = new HTTPFixedLengthInputStream(session, getContentLength());
#endif
	else 
		_pStream = new HTTPFixedLengthInputStream(session, 0);
}


HTTPServerRequestImpl::~HTTPServerRequestImpl()
{
	delete _pStream;
}


bool HTTPServerRequestImpl::secure() const
{
	return _session.socket().secure();
}


StreamSocket& HTTPServerRequestImpl::socket()
{
	return _session.socket();
}


StreamSocket HTTPServerRequestImpl::detachSocket()
{
	return _session.detachSocket();
}


} } // namespace Poco::Net
