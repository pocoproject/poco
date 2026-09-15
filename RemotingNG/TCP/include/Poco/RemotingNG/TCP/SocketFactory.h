//
// SocketFactory.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  SocketFactory
//
// Definition of the SocketFactory class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_TCP_SocketFactory_INCLUDED
#define RemotingNG_TCP_SocketFactory_INCLUDED


#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/URI.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


class RemotingNGTCP_API SocketFactory: public Poco::RefCountedObject
	/// SocketFactory is used by the ConnectionManager to
	/// create a socket for the connection to the
	/// listener on the server.
	///
	/// The default implementation simply creates a Poco::Net::StreamSocket.
	/// If a secure connection should be created, createSocket() should
	/// be overridden in a subclass, creating a Poco::Net::SecureStreamSocket
	/// instead.
	///
	/// The Remoting NG TCP transport uses the "remoting.tcp" URI scheme
	/// for unencrypted socket connections and the "remoting.tcps" URI scheme
	/// for SSL/TLS connections. Subclasses supporting SSL/TLS should check
	/// the URI scheme and create an appropriate socket.
{
public:
	using Ptr = Poco::AutoPtr<SocketFactory>;

	SocketFactory();
		/// Creates the SocketFactory.

	~SocketFactory();
		/// Destroys the SocketFactory.

	virtual Poco::Net::StreamSocket createSocket(const Poco::URI& uri);
		/// Create and return a Poco::Net::StreamSocket (or subclass of it,
		/// such as a Poco::Net::SecureStreamSocket), connected
		/// to the address given by the authority part of the URI.
		///
		/// The default implementation simply creates a Poco::Net::StreamSocket.
};


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_SocketFactory_INCLUDED
