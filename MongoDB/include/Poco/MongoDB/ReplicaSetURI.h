//
// ReplicaSetURI.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReplicaSetURI
//
// Definition of the ReplicaSetURI class.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_ReplicaSetURI_INCLUDED
#define MongoDB_ReplicaSetURI_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/ReadPreference.h"
#include "Poco/URI.h"
#include <vector>
#include <string>


namespace Poco {
namespace MongoDB {


class MongoDB_API ReplicaSetURI
	/// Class for parsing and generating MongoDB replica set URIs.
	///
	/// This class handles parsing of MongoDB connection strings in the format:
	///   mongodb://[username:password@]host1[:port1][,host2[:port2],...][/[database][?options]]
	///
	/// It also provides functionality to:
	/// - Access and modify the list of servers
	/// - Access and modify configuration options
	/// - Generate a URI string from the current state
	///
	/// Usage example:
	///   ReplicaSetURI uri("mongodb://host1:27017,host2:27017/?replicaSet=rs0");
	///
	///   // Access parsed data
	///   std::vector<Net::SocketAddress> servers = uri.servers();
	///   std::string setName = uri.replicaSet();
	///
	///   // Modify and regenerate
	///   uri.addServer(Net::SocketAddress("host3:27017"));
	///   uri.setReadPreference("secondaryPreferred");
	///   std::string newUri = uri.toString();
{
public:
	ReplicaSetURI();
		/// Creates an empty ReplicaSetURI.

	explicit ReplicaSetURI(const std::string& uri);
		/// Creates a ReplicaSetURI by parsing the given MongoDB connection string.
		///
		/// Throws Poco::SyntaxException if the URI format is invalid.
		/// Throws Poco::UnknownURISchemeException if the scheme is not "mongodb".

	~ReplicaSetURI();
		/// Destroys the ReplicaSetURI.

	// Server management
	[[nodiscard]] const std::vector<std::string>& servers() const;
		/// Returns the list of server addresses as strings (host:port format).
		/// Servers are NOT resolved - they remain as strings exactly as provided in the URI.

	void setServers(const std::vector<std::string>& servers);
		/// Sets the list of server addresses as strings (host:port format).

	void addServer(const std::string& server);
		/// Adds a server to the list as a string (host:port format).

	void clearServers();
		/// Clears the list of servers.

	// Configuration options
	[[nodiscard]] std::string replicaSet() const;
		/// Returns the replica set name, or empty string if not set.

	void setReplicaSet(const std::string& name);
		/// Sets the replica set name.

	[[nodiscard]] ReadPreference readPreference() const;
		/// Returns the read preference.

	void setReadPreference(const ReadPreference& pref);
		/// Sets the read preference.

	void setReadPreference(const std::string& mode);
		/// Sets the read preference from a string mode.
		/// Valid modes: primary, primaryPreferred, secondary, secondaryPreferred, nearest

	[[nodiscard]] unsigned int connectTimeoutMS() const;
		/// Returns the connection timeout in milliseconds.

	void setConnectTimeoutMS(unsigned int timeoutMS);
		/// Sets the connection timeout in milliseconds.

	[[nodiscard]] unsigned int socketTimeoutMS() const;
		/// Returns the socket timeout in milliseconds.

	void setSocketTimeoutMS(unsigned int timeoutMS);
		/// Sets the socket timeout in milliseconds.

	[[nodiscard]] unsigned int heartbeatFrequencyMS() const;
		/// Returns the heartbeat frequency in milliseconds.

	void setHeartbeatFrequencyMS(unsigned int milliseconds);
		/// Sets the heartbeat frequency in milliseconds.

	[[nodiscard]] unsigned int reconnectRetries() const;
		/// Returns the number of reconnection retries.

	void setReconnectRetries(unsigned int retries);
		/// Sets the number of reconnection retries.

	[[nodiscard]] unsigned int reconnectDelay() const;
		/// Returns the reconnection delay in seconds.

	void setReconnectDelay(unsigned int seconds);
		/// Sets the reconnection delay in seconds.

	[[nodiscard]] std::string database() const;
		/// Returns the database name from the URI path, or empty string if not set.

	void setDatabase(const std::string& database);
		/// Sets the database name.

	[[nodiscard]] std::string username() const;
		/// Returns the username, or empty string if not set.

	void setUsername(const std::string& username);
		/// Sets the username.

	[[nodiscard]] std::string password() const;
		/// Returns the password, or empty string if not set.

	void setPassword(const std::string& password);
		/// Sets the password.

	// URI generation
	[[nodiscard]] std::string toString() const;
		/// Generates a MongoDB connection string from the current configuration.
		/// Format: mongodb://[username:password@]host1:port1[,host2:port2,...][/database][?options]

	// Parsing
	void parse(const std::string& uri);
		/// Parses a MongoDB connection string and updates the configuration.
		///
		/// Throws Poco::SyntaxException if the URI format is invalid.
		/// Throws Poco::UnknownURISchemeException if the scheme is not "mongodb".

private:
	void parseOptions(const Poco::URI::QueryParameters& params);
		/// Parses query parameters from a QueryParameters collection.

	std::string buildQueryString() const;
		/// Builds the query string from current configuration options.

	std::vector<std::string> _servers;
		/// Server addresses stored as strings (host:port format).
		/// NOT resolved to avoid DNS errors for non-existent hosts.

	std::string _replicaSet;
	ReadPreference _readPreference{ReadPreference::Primary};
	unsigned int _connectTimeoutMS{10000};
	unsigned int _socketTimeoutMS{30000};
	unsigned int _heartbeatFrequencyMS{10000};
	unsigned int _reconnectRetries{10};
	unsigned int _reconnectDelay{1};
	std::string _database;
	std::string _username;
	std::string _password;
};


} } // namespace Poco::MongoDB


#endif // MongoDB_ReplicaSetURI_INCLUDED
