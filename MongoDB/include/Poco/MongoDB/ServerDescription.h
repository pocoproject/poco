//
// ServerDescription.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  ServerDescription
//
// Definition of the ServerDescription class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_ServerDescription_INCLUDED
#define MongoDB_ServerDescription_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Timestamp.h"
#include <vector>
#include <string>


namespace Poco {
namespace MongoDB {


class MongoDB_API ServerDescription
	/// Represents the state of a single MongoDB server in a replica set.
	///
	/// This class stores metadata about a MongoDB server obtained from
	/// the 'hello' or legacy 'isMaster' command response, including:
	/// - Server type (primary, secondary, arbiter, etc.)
	/// - Replica set membership information
	/// - Round-trip time for server selection
	/// - Server tags for tag-based read preferences
	///
	/// THREAD SAFETY:
	/// This class is NOT thread-safe. External synchronization is required
	/// if instances are accessed from multiple threads.
{
public:
	enum ServerType
		/// MongoDB server type enumeration
	{
		Unknown,       /// Server type not yet determined
		Standalone,    /// Standalone MongoDB instance (not in a replica set)
		RsPrimary,     /// Replica set primary (writable)
		RsSecondary,   /// Replica set secondary (read-only)
		RsArbiter,     /// Replica set arbiter (no data, votes only)
		RsOther,       /// Other replica set member type
		RsGhost,       /// Ghost member (removed or not yet initialized)
		Mongos         /// MongoDB sharding router
	};

	ServerDescription();
		/// Creates an Unknown server description.

	explicit ServerDescription(const Net::SocketAddress& address);
		/// Creates an Unknown server description for the given address.

	ServerDescription(const ServerDescription& other);
		/// Copy constructor.

	ServerDescription(ServerDescription&& other) noexcept;
		/// Move constructor.

	~ServerDescription();
		/// Destroys the ServerDescription.

	ServerDescription& operator=(const ServerDescription& other);
		/// Assignment operator.

	ServerDescription& operator=(ServerDescription&& other) noexcept;
		/// Move assignment operator.

	[[nodiscard]] ServerType type() const;
		/// Returns the server type.

	[[nodiscard]] const Net::SocketAddress& address() const;
		/// Returns the server address.

	[[nodiscard]] Timestamp lastUpdateTime() const;
		/// Returns the timestamp of the last successful update.

	[[nodiscard]] Poco::Int64 roundTripTime() const;
		/// Returns the round-trip time in microseconds.
		/// This is used for "nearest" read preference selection.

	[[nodiscard]] const std::string& setName() const;
		/// Returns the replica set name, or empty string if not in a replica set.


	[[nodiscard]] bool isWritable() const;
		/// Returns true if this server can accept write operations.
		/// Only primary servers are writable.

	[[nodiscard]] const Document& tags() const;
		/// Returns the server tags for tag-based read preferences.
		/// Returns an empty document if no tags are configured.

	[[nodiscard]] bool isPrimary() const;
		/// Returns true if this is a primary server (RsPrimary) or a standalone server.
		/// Standalone servers are treated as primaries for read preference purposes.

	[[nodiscard]] bool isSecondary() const;
		/// Returns true if this is a secondary server.

	[[nodiscard]] bool hasError() const;
		/// Returns true if the last update attempt resulted in an error.

	[[nodiscard]] const std::string& error() const;
		/// Returns the last error message, or empty string if no error.

	[[nodiscard]] std::vector<Net::SocketAddress> updateFromHelloResponse(const Document& helloResponse, Poco::Int64 rttMicros);
		/// Updates the server description from a 'hello' command response.
		/// The rttMicros parameter should contain the round-trip time
		/// of the hello command in microseconds.
		/// Returns a list of all replica set members (hosts, passives, arbiters)
		/// discovered in the hello response.

	void markError(const std::string& errorMessage);
		/// Marks this server as having an error.
		/// This sets the type to Unknown and stores the error message.

	void setAddress(const Net::SocketAddress& address);
		/// Sets the server address.

	void reset();
		/// Resets the server description to Unknown state.

private:
	void parseServerType(const Document& doc);
	std::vector<Net::SocketAddress> parseHosts(const Document& doc);
	void parseTags(const Document& doc);

	Net::SocketAddress _address;
	ServerType _type{Unknown};
	Timestamp _lastUpdateTime;
	Poco::Int64 _roundTripTime{0};
	std::string _setName;
	Document _tags;
	std::string _error;
	bool _hasError{false};
};


//
// inlines
//


inline ServerDescription::ServerType ServerDescription::type() const
{
	return _type;
}


inline const Net::SocketAddress& ServerDescription::address() const
{
	return _address;
}


inline Timestamp ServerDescription::lastUpdateTime() const
{
	return _lastUpdateTime;
}


inline Poco::Int64 ServerDescription::roundTripTime() const
{
	return _roundTripTime;
}


inline const std::string& ServerDescription::setName() const
{
	return _setName;
}


inline bool ServerDescription::isWritable() const
{
	return _type == RsPrimary || _type == Standalone;
}


inline const Document& ServerDescription::tags() const
{
	return _tags;
}


inline bool ServerDescription::isPrimary() const
{
	// Standalone servers should be treated as primary for read preference purposes
	return _type == RsPrimary || _type == Standalone;
}


inline bool ServerDescription::isSecondary() const
{
	return _type == RsSecondary;
}


inline bool ServerDescription::hasError() const
{
	return _hasError;
}


inline const std::string& ServerDescription::error() const
{
	return _error;
}


inline void ServerDescription::setAddress(const Net::SocketAddress& address)
{
	_address = address;
}


} } // namespace Poco::MongoDB


#endif // MongoDB_ServerDescription_INCLUDED
