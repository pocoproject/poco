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
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/String.h"
#include "Poco/Ascii.h"
#include <algorithm>


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

	// A request whose length is ambiguous must be refused rather than framed on
	// one of the competing values: a proxy in front may well pick the other one.
	std::size_t contentLengths = 0;
	std::size_t transferEncodings = 0;
	for (const auto& [name, value]: *this)
	{
		if (icompare(name, CONTENT_LENGTH) == 0) ++contentLengths;
		else if (icompare(name, TRANSFER_ENCODING) == 0) ++transferEncodings;
	}
	if (contentLengths > 1 || transferEncodings > 1)
		throw MessageException("Duplicate Content-Length or Transfer-Encoding header");
	if (transferEncodings == 1 && contentLengths == 1)
		throw MessageException("Both Content-Length and Transfer-Encoding present");

	// NumberParser skips thousands separators without checking their position,
	// so "5,5" would be read as 55 while a front-end sees 5 or rejects it.
	if (contentLengths == 1)
	{
		const std::string& value = get(CONTENT_LENGTH);
		if (value.empty() || !std::all_of(value.begin(), value.end(),
				[](char c) { return Poco::Ascii::isDigit(c); }))
			throw MessageException("Malformed Content-Length header");
	}

	// Only "chunked" frames the body. Any other transfer coding would leave the
	// payload in the connection to be read as the next request.
	if (transferEncodings == 1 && !getChunkedTransferEncoding())
		throw MessageException("Unsupported Transfer-Encoding");

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
