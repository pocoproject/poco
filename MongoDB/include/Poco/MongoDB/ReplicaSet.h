//
// ReplicaSet.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReplicaSet
//
// Definition of the ReplicaSet class.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_ReplicaSet_INCLUDED
#define MongoDB_ReplicaSet_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/ReadPreference.h"
#include "Poco/MongoDB/TopologyDescription.h"
#include "Poco/Net/SocketAddress.h"
#include <atomic>
#include <chrono>
#include <cstddef>
#include <mutex>
#include <string>
#include <thread>
#include <vector>


namespace Poco {
namespace MongoDB {


class ReplicaSetURI;


class MongoDB_API ReplicaSet
	/// Class for working with a MongoDB replica set.
	///
	/// This class provides comprehensive replica set support including:
	/// - Automatic topology discovery from seed servers
	/// - Primary election detection
	/// - Connection failover on errors
	/// - Read preference routing (primary, secondary, nearest, etc.)
	/// - Background topology monitoring
	/// - Server health checking
	///
	/// Usage example:
	///   ReplicaSet::Config config;
	///   config.seeds = {
	///       Net::SocketAddress("mongo1:27017"),
	///       Net::SocketAddress("mongo2:27017"),
	///       Net::SocketAddress("mongo3:27017")
	///   };
	///   config.setName = "rs0";
	///   config.readPreference = ReadPreference::primaryPreferred();
	///
	///   ReplicaSet rs(config);
	///   Connection::Ptr conn = rs.getPrimaryConnection();
	///   // Use connection...
	///
	/// REQUIREMENTS:
	/// Requires MongoDB 5.1 or later. Earlier versions using the legacy
	/// isMaster command are not supported.
	///
	/// THREAD SAFETY:
	/// The ReplicaSet class is thread-safe. Multiple threads can call
	/// getConnection() and other methods concurrently. However, the
	/// returned Connection objects are NOT thread-safe and must be used
	/// by only one thread at a time, or protected by external synchronization.
	///
	/// For multi-threaded applications, use ReplicaSetConnection with
	/// connection pooling (PoolableObjectFactory pattern).
{
public:
	struct Config
		/// Replica set configuration
	{
		std::vector<Net::SocketAddress> seeds;
			/// Seed servers for initial topology discovery.
			/// At least one seed must be reachable.

		std::string setName;
			/// Expected replica set name.
			/// If empty, will be discovered from servers.

		ReadPreference readPreference{ReadPreference::Primary};
			/// Default read preference for this replica set.

		unsigned int connectTimeoutSeconds{10};
			/// Connection timeout in seconds (default: 10)
			///
			/// NOTE: This value is currently unused by ReplicaSet itself. It is intended
			/// for use by custom SocketFactory implementations. Custom factories can
			/// access this value via ReplicaSet::configuration() and use it when creating
			/// sockets. Use ReplicaSet::setSocketFactory() to set a custom factory that
			/// utilizes this timeout value.

		unsigned int socketTimeoutSeconds{30};
			/// Socket send/receive timeout in seconds (default: 30)
			///
			/// NOTE: This value is currently unused by ReplicaSet itself. It is intended
			/// for use by custom SocketFactory implementations. Custom factories can
			/// access this value via ReplicaSet::configuration() and use it when creating
			/// sockets. Use ReplicaSet::setSocketFactory() to set a custom factory that
			/// utilizes this timeout value.

		unsigned int heartbeatFrequencySeconds{10};
			/// Topology monitoring interval in seconds (default: 10)

		std::size_t serverReconnectRetries{10};
			/// Number of connection retries to a server/replica set if no server is available temporarily

		unsigned int serverReconnectDelaySeconds{1};
			/// Delay in seconds between re-connects to a server/replica set if no server is available temporarily

		bool enableMonitoring{true};
			/// Enable background topology monitoring (default: true)

		Connection::SocketFactory* socketFactory{nullptr};
			/// Optional socket factory for SSL/TLS connections.
			/// Can be set via config or later using setSocketFactory().
			/// Custom factories can access timeout config via ReplicaSet::configuration().
	};

	explicit ReplicaSet(const Config& config);
		/// Creates a ReplicaSet with the given configuration.
		/// Performs initial topology discovery.
		/// Throws Poco::IOException if initial discovery fails.

	explicit ReplicaSet(const std::vector<Net::SocketAddress>& seeds);
		/// Creates a ReplicaSet with default configuration and the given seed addresses.
		/// Performs initial topology discovery.
		/// Throws Poco::IOException if initial discovery fails.

	explicit ReplicaSet(const std::string& uri);
		/// Creates a ReplicaSet from a MongoDB URI string.
		/// Format: mongodb://host1:port1,host2:port2,...?options
		///
		/// Supported URI options:
		///   - replicaSet=name          - Replica set name
		///   - readPreference=mode      - primary|primaryPreferred|secondary|secondaryPreferred|nearest
		///   - connectTimeoutMS=ms      - Connection timeout in milliseconds
		///   - socketTimeoutMS=ms       - Socket timeout in milliseconds
		///   - heartbeatFrequencyMS=ms  - Heartbeat frequency in milliseconds (default: 10000)
		///   - reconnectRetries=n       - Number of reconnection retries (default: 10)
		///   - reconnectDelay=seconds   - Delay between reconnection attempts in seconds (default: 1)
		///
		/// Example: mongodb://mongo1:27017,mongo2:27017,mongo3:27017/?replicaSet=rs0&readPreference=primaryPreferred
		///
		/// Throws Poco::SyntaxException if URI is invalid.
		/// Throws Poco::UnknownURISchemeException if scheme is not "mongodb".

	explicit ReplicaSet(const ReplicaSetURI& uri);
		/// Creates a ReplicaSet from a ReplicaSetURI object.
		/// This allows for programmatic URI construction and modification before
		/// creating the replica set connection.
		///
		/// The ReplicaSetURI stores servers as strings without DNS resolution.
		/// This constructor resolves the server strings to SocketAddress objects.
		/// Servers that cannot be resolved are skipped and will be marked as
		/// unavailable during topology discovery.
		///
		/// Example:
		///   ReplicaSetURI uri;
		///   uri.addServer("host1:27017");
		///   uri.addServer("host2:27017");
		///   uri.setReplicaSet("rs0");
		///   uri.setReadPreference("primaryPreferred");
		///   ReplicaSet rs(uri);
		///
		/// Throws Poco::InvalidArgumentException if the URI contains no servers
		///        or if no servers can be resolved.
		/// Throws Poco::IOException if initial discovery fails.

	virtual ~ReplicaSet();
		/// Destroys the ReplicaSet and stops background monitoring.

	Connection::Ptr getConnection(const ReadPreference& readPref);
		/// Returns a connection to a server matching the read preference.
		/// Returns null if no suitable server is available.

	Connection::Ptr waitForServerAvailability(const ReadPreference& readPref);
		/// Waits for a server to become available for the given read preference.
		/// This method coordinates waiting between multiple threads - only one thread
		/// performs the actual sleep and topology refresh, while others benefit from
		/// the refresh done by the first thread.
		/// Returns a connection if a server becomes available, or null if still unavailable.
		/// Thread-safe: uses internal synchronization to prevent redundant refresh attempts.

	Connection::Ptr getPrimaryConnection();
		/// Returns a connection to the primary server.
		/// Returns null if no primary is available.

	Connection::Ptr getSecondaryConnection();
		/// Returns a connection to a secondary server.
		/// Returns null if no secondary is available.

	[[nodiscard]] Config configuration() const;
		// Returns a copy of replica set configuration.

	[[nodiscard]] TopologyDescription topology() const;
		/// Returns a copy of the current topology description.

	void refreshTopology();
		/// Forces an immediate topology refresh by querying all known servers.

	void startMonitoring();
		/// Starts the background monitoring thread if not already running.

	void stopMonitoring();
		/// Stops the background monitoring thread.

	void setSocketFactory(Connection::SocketFactory* factory);
		/// Sets the socket factory for creating connections.
		/// The factory can access timeout configuration via configuration().connectTimeoutSeconds
		/// and configuration().socketTimeoutSeconds.
		///
		/// Example:
		///   rs.setSocketFactory(&myCustomFactory);

	void setReadPreference(const ReadPreference& pref);
		/// Sets the default read preference.

	[[nodiscard]] ReadPreference readPreference() const;
		/// Returns the default read preference.

	[[nodiscard]] std::string setName() const;
		/// Returns the replica set name, or empty if not discovered.

	[[nodiscard]] bool hasPrimary() const;
		/// Returns true if a primary server is known.

private:
	void monitor() noexcept;
		/// Background monitoring thread function.

	Connection::Ptr selectServer(const ReadPreference& readPref);
		/// Selects a server based on read preference and creates a connection.

	Connection::Ptr createConnection(const Net::SocketAddress& address);
		/// Creates a new connection to the specified address.

	void updateTopologyFromHello(const Net::SocketAddress& address) noexcept;
		/// Queries a server with 'hello' command and updates topology.

	void updateTopologyFromAllServers() noexcept;
		/// Queries all known servers and updates topology.

	void parseURI(const std::string& uri);
		/// Parses a MongoDB URI into configuration.
		/// Extracts hosts and query parameters into _config.

	mutable std::mutex _mutex;

	Config _config;
	TopologyDescription _topology;

	std::thread _monitorThread;
	std::atomic<bool> _stopMonitoring{false};
	std::atomic<bool> _monitoringActive{false};

	std::mutex _serverAvailabilityRetryMutex;
	std::chrono::steady_clock::time_point _topologyRefreshTime;
};


} } // namespace Poco::MongoDB


#endif // MongoDB_ReplicaSet_INCLUDED
