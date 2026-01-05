//
// ReplicaSetConnection.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReplicaSetConnection
//
// Definition of the ReplicaSetConnection class.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_ReplicaSetConnection_INCLUDED
#define MongoDB_ReplicaSetConnection_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/ReadPreference.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/SharedPtr.h"
#include <functional>


namespace Poco {
namespace MongoDB {


class OpMsgMessage;
class ReplicaSet;


class MongoDB_API ReplicaSetConnection
	/// Wrapper around Connection that provides automatic retry and failover
	/// for MongoDB replica set operations.
	///
	/// This class wraps a Connection and automatically retries failed operations
	/// on different replica set members. It detects retriable errors (network
	/// failures, "not master" errors, etc.) and seamlessly fails over to another
	/// suitable server.
	///
	/// Usage example:
	///   ReplicaSet rs(config);
	///   ReplicaSetConnection::Ptr conn = new ReplicaSetConnection(rs, ReadPreference::Primary);
	///
	///   OpMsgMessage request("mydb", "mycollection");
	///   request.setCommandName(OpMsgMessage::CMD_FIND);
	///   request.body().add("filter", filterDoc);
	///
	///   OpMsgMessage response;
	///   conn->sendRequest(request, response);  // Automatic retry on failure
	///
	/// THREAD SAFETY:
	/// This class is NOT thread-safe, just like Connection. Each thread must
	/// have its own ReplicaSetConnection instance, or use connection pooling
	/// with external synchronization.
	///
	/// For multi-threaded applications, use ReplicaSetPoolableConnectionFactory
	/// with Poco::ObjectPool.
{
public:
	using Ptr = Poco::SharedPtr<ReplicaSetConnection>;

	ReplicaSetConnection(ReplicaSet& replicaSet, const ReadPreference& readPref);
		/// Creates a ReplicaSetConnection for the given replica set and read preference.
		/// The connection is established lazily on first use.

	~ReplicaSetConnection();
		/// Destroys the ReplicaSetConnection.

	void sendRequest(OpMsgMessage& request, OpMsgMessage& response);
		/// Sends a request and reads the response.
		/// Automatically retries on retriable errors with failover.
		///
		/// Throws Poco::IOException if all retry attempts fail.

	void sendRequest(OpMsgMessage& request);
		/// Sends a one-way request (fire-and-forget).
		/// Sets MSG_MORE_TO_COME flag and acknowledged=false.
		///
		/// Note: One-way requests are not retried on failure.

	void readResponse(OpMsgMessage& response);
		/// Reads a response for a previously sent request.

	[[nodiscard]] Net::SocketAddress address() const;
		/// Returns the address of the currently connected server.
		/// Throws Poco::NullPointerException if not connected.

	[[nodiscard]] Connection& connection();
		/// Returns a reference to the underlying Connection.
		/// Throws Poco::NullPointerException if not connected.

	void reconnect();
		/// Forces reconnection by selecting a new server from the replica set.
		/// Useful if you detect an error and want to explicitly retry.

	[[nodiscard]] bool isConnected() const noexcept;
		/// Returns true if currently connected to a server.

	[[nodiscard]] bool matchesReadPreference() const noexcept;
		/// Returns true if the currently connected server still matches the read preference.
		/// Returns false if not connected or if the server no longer satisfies the read preference.
		/// This is useful for connection pool validation to detect when a server role has changed
		/// (e.g., primary became secondary).

private:
	void ensureConnection();
		/// Ensures we have an active connection, creating one if needed.

	void executeWithRetry(std::function<void()> operation);
		/// Executes an operation with automatic retry on retriable errors.

	bool isRetriableError(const std::exception& e);
		/// Returns true if the exception represents a retriable error.

	bool isRetriableMongoDBError(const OpMsgMessage& response);
		/// Returns true if the MongoDB response contains a retriable error code.

	void markServerFailed();
		/// Marks the current server as failed in the topology.

	ReplicaSet& _replicaSet;
	ReadPreference _readPreference;
	Connection::Ptr _connection;
};


} } // namespace Poco::MongoDB


#endif // MongoDB_ReplicaSetConnection_INCLUDED
