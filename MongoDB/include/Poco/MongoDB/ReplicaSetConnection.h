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
#include "Poco/Timespan.h"
#include <functional>


namespace Poco::MongoDB {


class OpMsgMessage;
class ReplicaSet;


class MongoDB_API ReplicaSetConnection
	/// Wrapper around Connection that selects a replica set server by read
	/// preference and re-sends a failed request when that is safe. The
	/// connection is opened on first use and reused while it stays open; a
	/// failed attempt drops it, so the next attempt selects a server again.
	/// Safe reads are re-sent after a network error, a timeout or a listed
	/// error reply; getMore and killCursors are never re-sent; every other
	/// command only after a not-primary reply, which means it was not
	/// executed. MongoDB/README-ReplicaSet.md lists the commands and replies.
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
	///   conn->sendRequest(request, response);  // find is re-sent after a network error
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
		/// Timeouts and the wait for an available server come from the ReplicaSet
		/// configuration. Connects on first use.

	ReplicaSetConnection(ReplicaSet& replicaSet, const ReadPreference& readPref,
		Poco::Timespan connectTimeout, Poco::Timespan socketTimeout = 0);
		/// Creates a ReplicaSetConnection with explicit timeouts for its own
		/// connections; topology refreshes keep the configured ones. socketTimeout
		/// 0 (the default) means the ReplicaSet's configured socket timeout; set
		/// Config::socketTimeoutSeconds to 0 for no timeout. Connects on first use.

	~ReplicaSetConnection();
		/// Destroys the ReplicaSetConnection.

	void sendRequest(OpMsgMessage& request, OpMsgMessage& response);
		/// Sends a request and reads the response, re-sending as described above.
		/// Makes at most max(number of servers in the topology, 5) attempts; a
		/// failed connect counts as one. An error reply not listed in
		/// MongoDB/README-ReplicaSet.md is returned in response.
		/// Throws Poco::IOException or Poco::TimeoutException when a network
		/// failure is not re-sent or the last attempt fails; IOException("MongoDB
		/// server error: ...") for a listed reply that is not re-sent or ends the
		/// last attempt, which response holds; IOException("No suitable server
		/// found in replica set"), with the previous failure, if any, nested;
		/// DataFormatException or NotImplementedException for a malformed reply
		/// or an unsupported BSON element type.

	void sendRequest(OpMsgMessage& request);
		/// Sends a one-way request (fire-and-forget).
		/// Sets MSG_MORE_TO_COME flag and acknowledged=false.
		///
		/// Note: One-way requests are not re-sent on failure.

	void readResponse(OpMsgMessage& response);
		/// Reads a response for a previously sent request.
		///
		/// Throws Poco::IOException if the connection that carries the reply is
		/// gone; no other connection can continue it.

	[[nodiscard]] Net::SocketAddress address() const;
		/// Returns the address of the currently connected server.
		/// Throws Poco::NullPointerException if not connected.

	[[nodiscard]] Connection& connection();
		/// Returns a reference to the underlying Connection.
		/// Throws Poco::NullPointerException if not connected.

	void reconnect();
		/// Forces reconnection by selecting a new server from the replica set.
		/// Useful if you detect an error and want to send the request again.

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

	Connection::Ptr selectConnection(bool waitForServer);
		/// Returns a new connection to a server that matches the read preference,
		/// or null if there is none. If waitForServer is true, waits for a server
		/// to become available before giving up. Connect errors propagate.

	void dropConnectionAndRefresh();
		/// Drops the connection and refreshes the topology. A refresh failure is ignored:
		/// it must not replace the failure being handled.

	void executeWithRetry(OpMsgMessage& request, OpMsgMessage& response);
		/// Sends the request and reads the response. Re-sends only when that cannot
		/// apply the command twice or skip data; a re-send may reach the same server.

	ReplicaSet& _replicaSet;
	ReadPreference _readPreference;
	Connection::Ptr _connection;
	Poco::Timespan _connectTimeout;
	Poco::Timespan _socketTimeout;
	bool _hasTimeouts = false;
};


} // namespace Poco::MongoDB


#endif // MongoDB_ReplicaSetConnection_INCLUDED
