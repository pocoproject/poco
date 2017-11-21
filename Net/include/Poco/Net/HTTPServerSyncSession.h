//
// HTTPServerSyncSession.h
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerSyncSession
//
// Definition of the HTTPServerSyncSession class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPServerSyncSession_INCLUDED
#define Net_HTTPServerSyncSession_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/HTTPServerSession.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Timespan.h"


namespace Poco {
namespace Net {


class Net_API HTTPServerSyncSession: public HTTPServerSession
	/// This class handles the server side of a
	/// HTTP session. It is used internally by
	/// HTTPServer.
{
public:
	HTTPServerSyncSession(const StreamSocket& socket, HTTPServerParams::Ptr pParams);
		/// Creates the HTTPServerSyncSession.

	HTTPServerSyncSession(HTTPServerParams::Ptr pParams/*, bool extBuf = false*/);
	/// Creates the HTTPServerSyncSession.

	virtual ~HTTPServerSyncSession();
		/// Destroys the HTTPServerSyncSession.
				
	bool hasMoreRequests();
		/// Returns true if there are requests available.
	
	bool canKeepAlive() const;
		/// Returns true if the session can be kept alive.

private:
	bool           _firstRequest;
	Poco::Timespan _keepAliveTimeout;
	int            _maxKeepAliveRequests;
	//SocketAddress  _clientAddress;
	//SocketAddress  _serverAddress;
};


//
// inlines
//
inline bool HTTPServerSyncSession::canKeepAlive() const
{
	return _maxKeepAliveRequests != 0;
}


} } // namespace Poco::Net


#endif // Net_HTTPServerSyncSession_INCLUDED
