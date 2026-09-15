//
// Transport.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  Transport
//
// Definition of the Transport class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_TCP_Transport_INCLUDED
#define RemotingNG_TCP_Transport_INCLUDED


#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/RemotingNG/TCP/Connection.h"
#include "Poco/RemotingNG/TCP/ChannelStream.h"
#include "Poco/RemotingNG/TCP/ClientAuthenticator.h"
#include "Poco/RemotingNG/BinarySerializer.h"
#include "Poco/RemotingNG/BinaryDeserializer.h"
#include "Poco/RemotingNG/Transport.h"
#include "Poco/RemotingNG/Credentials.h"
#include "Poco/DeflatingStream.h"
#include "Poco/InflatingStream.h"
#include "Poco/Timespan.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include "Poco/Logger.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


class ConnectionManager;


class RemotingNGTCP_API Transport: public Poco::RemotingNG::Transport
	/// The Transport implementation for RemotingNG TCP.
{
public:
	static const std::string PROTOCOL;
	
	explicit Transport(ConnectionManager& connectionManager);
		/// Creates a Transport using the given ConnectionManager.

	~Transport();
		/// Destroys the Transport.

	Poco::Timespan getTimeout() const;
		/// Returns the response timeout.
		
	void setTimeout(const Poco::Timespan& timeout);
		/// Sets the response timeout.

	bool isCompressionEnabled() const;
		/// Returns true iff zlib deflate compression is enabled for requests.
		/// Default is disabled.
		
	void enableCompression(bool enable);
		/// Enables or disables zlib deflate compression for requests.

	void setAuthenticator(ClientAuthenticator::Ptr pAuthenticator);
		/// Sets the ClientAuthenticator to be used for authentication.
		
	ClientAuthenticator::Ptr getAuthenticator() const;
		/// Returns the ClientAuthenticator.

	void setCredentials(const Credentials& credentials);
		/// Sets the credentials for authentication.
		
	const Credentials& getCredentials() const;
		/// Returns the credentials for authentication.
	
	// Poco::RemotingNG::Transport
	const std::string& endPoint() const;
	void connect(const std::string& endPoint);
	void disconnect();
	bool connected() const;
	Poco::RemotingNG::Serializer& beginMessage(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType);
	void sendMessage(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType);
	Poco::RemotingNG::Serializer& beginRequest(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType);
	Poco::RemotingNG::Deserializer& sendRequest(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType);
	void endRequest();

protected:
	void setupSerializer(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, Poco::RemotingNG::SerializerBase::MessageType messageType, Poco::UInt16 frameFlags);
	void authenticate();
	static bool verifyCredentials(const Credentials& credentials);

private:
	Transport();

	enum
	{
		DEFAULT_TIMEOUT = 30 // Seconds
	};
	
	ConnectionManager& _connectionManager;
	BinarySerializer _serializer;
	BinaryDeserializer _deserializer;
	std::string _endPoint;
	Poco::URI _endPointURI;
	Poco::Timespan _timeout;
	bool _compression;
	ClientAuthenticator::Ptr _pClientAuthenticator;
	Credentials _credentials;
	Connection::Ptr _pConnection;
	Poco::UInt32 _channel;
	Poco::UInt64 _authToken;
	Poco::UInt32 _authConnectionId;
	Poco::SharedPtr<ChannelOutputStream> _pRequestStream;
	Poco::SharedPtr<ChannelInputStream> _pReplyStream;
	Poco::SharedPtr<Poco::DeflatingOutputStream> _pDeflatingStream;
	Poco::SharedPtr<Poco::InflatingInputStream> _pInflatingStream;
	Poco::Logger& _logger;
};


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_Transport_INCLUDED
