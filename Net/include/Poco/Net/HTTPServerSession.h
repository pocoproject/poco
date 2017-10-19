//
// HTTPServerSession.h
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerSession
//
// Definition of the HTTPServerSession class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPServerSession_INCLUDED
#define Net_HTTPServerSession_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/HTTPServerSession.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Timespan.h"


namespace Poco {
namespace Net {


class Net_API HTTPServerSession: public HTTPSession
	/// This class handles the server side of a
	/// HTTP session. It is used internally by
	/// HTTPServer.
{
public:
	HTTPServerSession(const StreamSocket& socket, HTTPServerParams::Ptr pParams);
		/// Creates the HTTPServerSession.

	HTTPServerSession();
	/// Creates the HTTPServerSession.

	virtual ~HTTPServerSession();
		/// Destroys the HTTPServerSession.
				
	virtual bool hasMoreRequests() = 0;
		/// Returns true if there are requests available.
	
	virtual bool canKeepAlive() const = 0;
		/// Returns true if the session can be kept alive.

	virtual SocketAddress clientAddress();
	/// Returns the client's address.

	virtual SocketAddress serverAddress();
	/// Returns the server's address.
};

//
// inlines
//


inline SocketAddress HTTPServerSession::clientAddress()
{
	return socket().peerAddress();
}


inline SocketAddress HTTPServerSession::serverAddress()
{
	return socket().address();
}


} } // namespace Poco::Net


#endif // Net_HTTPServerSession_INCLUDED
