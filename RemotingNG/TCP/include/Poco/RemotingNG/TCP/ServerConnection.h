//
// ServerConnection.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  ServerConnection
//
// Definition of the ServerConnection class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_TCP_ServerConnection_INCLUDED
#define RemotingNG_TCP_ServerConnection_INCLUDED


#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/RemotingNG/TCP/Listener.h"
#include "Poco/RemotingNG/TCP/CredentialsStore.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Logger.h"
#include <map>


namespace Poco {
namespace RemotingNG {
namespace TCP {


class RemotingNGTCP_API ServerConnection: public Poco::Net::TCPServerConnection
	/// The TCPServerConnection for handling incoming RemotingNG TCP Transport connections.
{
public:
	explicit ServerConnection(Listener::Ptr pListener, const Poco::Net::StreamSocket& socket);
		/// Creates a ServerConnectionFactory using the given Listener instance.

	~ServerConnection();
		/// Destroys the ServerConnectionFactory.

	// TCPServerConnection
	void run();

private:
	Listener::Ptr _pListener;
	CredentialsStore::Ptr _pCredentialsStore;
	Poco::Logger& _logger;
};


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_ServerConnection_INCLUDED
