//
// MongoDBTestServer.h
//
// Definition of the MongoDBTestServer class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

//
// The server answers each command from a per-command queue of scripted
// behaviours, falling back to a per-command default and finally to a generated
// hello reply or { ok: 1.0 }. It counts the requests it received per command
// and the connections it accepted, and the test thread can close the listening
// socket or all accepted sockets to simulate a server going away.
//


#ifndef MongoDBTestServer_INCLUDED
#define MongoDBTestServer_INCLUDED


#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/Net/PollSet.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>


class MongoDBTestServer
	/// A fake MongoDB server speaking OP_MSG on 127.0.0.1, for tests only.
	///
	/// One server thread owns all sockets. Public methods must be called
	/// from the test thread, never from the server thread.
	///
	/// An exception caught on the server thread is counted and its message
	/// kept, so that a broken server thread is visible to the test instead of
	/// looking like a server that stopped answering.
{
public:
	enum class Action
	{
		Reply,
		RawReply,
		NoReply,
		CloseConnection
	};

	struct Behaviour
	{
		Action action = Action::Reply;
		Poco::MongoDB::Document::Ptr reply;
			/// Reply: the body to send; { ok: 1.0 } if null.
		std::string raw;
			/// RawReply: bytes sent as-is.
	};

	MongoDBTestServer();
		/// Binds to an ephemeral port and starts the server thread.

	~MongoDBTestServer();
		/// Stops the server thread and closes all sockets.

	MongoDBTestServer(const MongoDBTestServer&) = delete;
	MongoDBTestServer& operator=(const MongoDBTestServer&) = delete;

	[[nodiscard]] Poco::Net::SocketAddress address() const;
		/// Returns the listening address.

	[[nodiscard]] std::string hostAndPort() const;
		/// Returns "127.0.0.1:<port>".

	void setPrimary(bool primary);
		/// Sets isWritablePrimary in the generated hello reply (default true).

	void setSetName(const std::string& setName);
		/// Sets setName in the generated hello reply (default "rs0").

	void setHosts(const std::vector<std::string>& hosts);
		/// Sets hosts in the generated hello reply (default { hostAndPort() }).

	void setMe(const std::string& me);
		/// Sets me in the generated hello reply (default hostAndPort()).

	void addBehaviour(const std::string& command, const Behaviour& behaviour, int times = 1);
		/// Queues times copies of behaviour for the command.

	void setDefaultBehaviour(const std::string& command, const Behaviour& behaviour);
		/// Sets the behaviour used when the command's queue is empty.

	[[nodiscard]] int commandCount(const std::string& command) const;
		/// Returns the number of received requests for the command.

	[[nodiscard]] Poco::MongoDB::Document::Ptr lastRequest(const std::string& command) const;
		/// Returns the body of the last received request for the command, or null.

	[[nodiscard]] int totalConnections() const;
		/// Returns the number of accepted connections.

	[[nodiscard]] int errorCount() const;
		/// Returns the number of exceptions caught on the server thread.

	[[nodiscard]] std::string lastError() const;
		/// Returns the message of the last exception caught on the server thread.

	void stopListening();
		/// Closes the listening socket; blocks until done.

	void closeConnections();
		/// Closes all accepted client sockets; blocks until done.

	static Poco::MongoDB::Document::Ptr errorReply(Poco::Int32 code, const std::string& codeName, const std::string& errmsg = std::string());
		/// Returns { ok: 0.0, errmsg, code, codeName }.

	static Poco::MongoDB::Document::Ptr cursorReply(const std::string& ns, Poco::Int64 cursorId, const std::vector<Poco::MongoDB::Document::Ptr>& batch, bool firstBatch = true);
		/// Returns { cursor: { firstBatch|nextBatch: [batch], id, ns }, ok: 1.0 }.

private:
	struct Client
	{
		Poco::Net::StreamSocket socket;
		std::string buffer;
			/// Received bytes not yet parsed.
		std::optional<std::chrono::steady_clock::time_point> partialSince;
			/// When the buffer started to hold an incomplete message.
	};

	void run();
	void pollOnce();
	void acceptClient();
	void discardClient(const Poco::Net::StreamSocket& socket);
		/// Takes a socket that could not be registered as a client out of the poll set.
	bool handleClient(Client& client);
	bool dispatch(Client& client, const Poco::MongoDB::OpMsgMessage& request);
	void closeClient(const Poco::Net::Socket& socket);
	void closeStalledClients();
	void processControlRequests();
	Behaviour nextBehaviour(const std::string& command);
	Poco::MongoDB::Document::Ptr helloReply() const;
	void recordError(const std::string& where);
		/// Counts the error, keeps its message and prints it. Must not be called
		/// with _mutex held.

	Poco::Net::ServerSocket _listener;
	Poco::UInt16 _port;

	// Used by the server thread only.
	bool _listening = true;
	std::map<Poco::Net::Socket, Client> _clients;
	Poco::Net::PollSet _pollSet;

	mutable std::mutex _mutex;
	std::condition_variable _controlDone;
	bool _stopListeningRequested = false;
	bool _closeConnectionsRequested = false;

	bool _primary = true;
	std::string _setName;
	std::vector<std::string> _hosts;
	std::string _me;
	std::map<std::string, std::deque<Behaviour>> _scripts;
	std::map<std::string, Behaviour> _defaults;
	std::map<std::string, int> _commandCounts;
	std::map<std::string, Poco::MongoDB::Document::Ptr> _lastRequests;
	int _totalConnections = 0;
	int _errorCount = 0;
	std::string _lastError;

	std::atomic<bool> _stop{false};
	std::thread _thread;
};


#endif // MongoDBTestServer_INCLUDED
