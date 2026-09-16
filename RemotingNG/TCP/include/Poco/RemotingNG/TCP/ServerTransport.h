//
// ServerTransport.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  ServerTransport
//
// Definition of the ServerTransport class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_TCP_ServerTransport_INCLUDED
#define RemotingNG_TCP_ServerTransport_INCLUDED


#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/RemotingNG/TCP/ChannelStream.h"
#include "Poco/RemotingNG/TCP/CredentialsStore.h"
#include "Poco/RemotingNG/ServerTransport.h"
#include "Poco/RemotingNG/BinarySerializer.h"
#include "Poco/RemotingNG/BinaryDeserializer.h"
#include "Poco/InflatingStream.h"
#include "Poco/DeflatingStream.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/SharedPtr.h"
#include "Poco/Event.h"
#include "Poco/Logger.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


class Listener;


class RemotingNGTCP_API ServerTransport: public Poco::RemotingNG::ServerTransport, public Poco::Runnable, public Poco::RefCountedObject
	/// The ServerTransport implementation for the RemotingNG TCP transport.
{
public:
	using Ptr = Poco::AutoPtr<ServerTransport>;

	ServerTransport(Listener& _listener, CredentialsStore::Ptr pCredentialsStore, const Poco::SharedPtr<ChannelInputStream>& pRequestStream, const Poco::SharedPtr<ChannelOutputStream>& pReplyStream, bool compressed, bool authenticated);
		/// Creates a ServerTransport.

	~ServerTransport();
		/// Destroys the ServerTransport().

	void waitReady();
		/// Waits until the server thread is ready.

	// ServerTransport
	bool authenticate(const std::string& method);
	bool authorize(const std::string& method, const std::string& permission);
	Deserializer& beginRequest();
	Serializer& sendReply(SerializerBase::MessageType messageType);
	void endRequest();

	// Runnable
	void run();

private:
	Listener& _listener;
	CredentialsStore::Ptr _pCredentialsStore;
	Poco::SharedPtr<ChannelInputStream> _pRequestStream;
	Poco::SharedPtr<ChannelOutputStream> _pReplyStream;
	bool _authenticated;
	Poco::InflatingInputStream* _pInflater;
	Poco::DeflatingOutputStream* _pDeflater;
	Poco::RemotingNG::BinarySerializer _serializer;
	Poco::RemotingNG::BinaryDeserializer _deserializer;
	Poco::Event _ready;
	Poco::Logger& _logger;
};


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_ServerTransport_INCLUDED
