//
// TopologyDescription.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  TopologyDescription
//
// Definition of the TopologyDescription class.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_TopologyDescription_INCLUDED
#define MongoDB_TopologyDescription_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/ServerDescription.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Types.h"
#include <cstddef>
#include <map>
#include <mutex>
#include <string>
#include <vector>


namespace Poco {
namespace MongoDB {


class Document;


class MongoDB_API TopologyDescription
	/// Maintains the complete MongoDB replica set topology state.
	///
	/// This class tracks all known servers in a replica set and their
	/// current state. It is updated based on 'hello' command responses
	/// from MongoDB servers.
	///
	/// The topology type automatically transitions based on server
	/// discovery:
	/// - Unknown: Initial state, no servers contacted
	/// - Single: Single standalone server
	/// - ReplicaSetNoPrimary: Replica set without a primary
	/// - ReplicaSetWithPrimary: Replica set with a primary
	/// - Sharded: Sharded cluster (mongos routers)
	///
	/// THREAD SAFETY:
	/// This class is thread-safe. All public methods use internal
	/// synchronization to protect the topology state.
{
public:
	enum TopologyType
		/// MongoDB topology type enumeration
	{
		Unknown,                  /// Topology not yet determined
		Single,                   /// Single server (standalone)
		ReplicaSetNoPrimary,      /// Replica set without primary
		ReplicaSetWithPrimary,    /// Replica set with primary
		Sharded                   /// Sharded cluster
	};

	TopologyDescription();
		/// Creates an empty topology description.

	explicit TopologyDescription(const std::string& setName);
		/// Creates a topology description for a replica set with the given name.

	TopologyDescription(const TopologyDescription& other);
		/// Copy constructor.

	TopologyDescription(TopologyDescription&& other) noexcept;
		/// Move constructor.

	~TopologyDescription();
		/// Destroys the TopologyDescription.

	TopologyDescription& operator=(const TopologyDescription& other);
		/// Assignment operator.

	TopologyDescription& operator=(TopologyDescription&& other) noexcept;
		/// Move assignment operator.

	bool operator==(const TopologyDescription& other) const;
		/// Equality comparison operator.
		/// Compares topology type, set name, and all servers.

	bool operator!=(const TopologyDescription& other) const;
		/// Inequality comparison operator.

	[[nodiscard]] TopologyType type() const;
		/// Returns the current topology type.

	[[nodiscard]] std::string setName() const;
		/// Returns the replica set name, or empty string if not a replica set.

	void setName(const std::string& name);
		/// Sets the replica set name.

	[[nodiscard]] std::vector<ServerDescription> servers() const;
		/// Returns a copy of all server descriptions.
		/// This is thread-safe but creates a copy.

	[[nodiscard]] ServerDescription findPrimary() const;
		/// Finds and returns the primary server.
		/// Returns an Unknown server description if no primary exists.

	[[nodiscard]] std::vector<ServerDescription> findSecondaries() const;
		/// Finds and returns all secondary servers.

	[[nodiscard]] bool hasPrimary() const;
		/// Returns true if a primary server exists in the topology.

	[[nodiscard]] bool hasServer(const Net::SocketAddress& address) const;
		/// Returns true if the server with the given address is in the topology.

	[[nodiscard]] ServerDescription getServer(const Net::SocketAddress& address) const;
		/// Returns the server description for the given address.
		/// Returns an Unknown server description if not found.

	const ServerDescription& updateServer(const Net::SocketAddress& address, const Document& helloResponse, Poco::Int64 rttMicros);
		/// Updates a server's description from a 'hello' command response.
		/// If the server doesn't exist in the topology, it is added.
		/// This may also trigger topology type transitions.
		///
		/// Returns a const reference to the updated server description.

	void markServerUnknown(const Net::SocketAddress& address, const std::string& error = "");
		/// Marks a server as Unknown (typically after an error).
		/// This may trigger topology type transitions.

	void addServer(const Net::SocketAddress& address);
		/// Adds a server to the topology in Unknown state.
		/// If the server already exists, this is a no-op.

	void removeServer(const Net::SocketAddress& address);
		/// Removes a server from the topology.
		/// This may trigger topology type transitions.

	void clear();
		/// Removes all servers and resets to Unknown topology type.

	[[nodiscard]] std::size_t serverCount() const;
		/// Returns the number of servers in the topology.

	[[nodiscard]] static std::string typeToString(TopologyType type);
		/// Converts a topology type enum to a human-readable string.
		/// Returns "Unknown", "Single Server", "Replica Set (with Primary)",
		/// "Replica Set (no Primary)", or "Sharded Cluster".

private:
	void updateTopologyType();
		/// Updates the topology type based on current server states.
		/// Must be called while holding the mutex.

	mutable std::mutex _mutex;
	TopologyType _type{Unknown};
	std::string _setName;
	std::map<Net::SocketAddress, ServerDescription> _servers;
};


} } // namespace Poco::MongoDB


#endif // MongoDB_TopologyDescription_INCLUDED
