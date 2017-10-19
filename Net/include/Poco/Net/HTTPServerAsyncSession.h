//
// HTTPServerAsyncSession.h
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerAsyncSession
//
// Definition of the HTTPServerAsyncSession class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPServerAsyncSession_INCLUDED
#define Net_HTTPServerAsyncSession_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPServerSession.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/HTTPServerAsyncSession.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Timespan.h"


namespace Poco {
namespace Net {


class Net_API HTTPServerAsyncSession: public HTTPServerSession
	/// This class handles the server side of a
	/// HTTP session. It is used internally by
	/// HTTPServer.
{
public:
	HTTPServerAsyncSession(HTTPServerParams::Ptr pParams);
	/// Creates the HTTPServerAsyncSession.

	virtual ~HTTPServerAsyncSession();
		/// Destroys the HTTPServerAsyncSession.
				
	bool hasMoreRequests();
		/// Returns true if there are requests available.
	
	bool canKeepAlive() const;
		/// Returns true if the session can be kept alive.
	
	SocketAddress clientAddress();
		/// Returns the client's address.
		
	SocketAddress serverAddress();
		/// Returns the server's address.
	
	void setClientAddress(const SocketAddress& addr);
		/// Sets the client address.

	void setServerAddress(const SocketAddress& addr);
		/// Sets the server address.

private:
	bool           _firstRequest;
	Poco::Timespan _keepAliveTimeout;
	int            _maxKeepAliveRequests;
	SocketAddress  _clientAddress;
	SocketAddress  _serverAddress;
};


//
// inlines
//
inline bool HTTPServerAsyncSession::canKeepAlive() const
{
	return _maxKeepAliveRequests != 0;
}


inline void HTTPServerAsyncSession::setClientAddress(const SocketAddress& addr)
{
	_clientAddress = addr;
}

inline void HTTPServerAsyncSession::setServerAddress(const SocketAddress& addr)
{
	_serverAddress = addr;
}


inline SocketAddress HTTPServerAsyncSession::clientAddress()
{
	return _clientAddress;
}


inline SocketAddress HTTPServerAsyncSession::serverAddress()
{
	return _clientAddress;
}


} } // namespace Poco::Net


#endif // Net_HTTPServerAsyncSession_INCLUDED
