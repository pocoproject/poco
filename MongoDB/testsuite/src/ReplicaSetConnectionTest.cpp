//
// ReplicaSetConnectionTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

//
// This suite verifies the re-send rules of ReplicaSetConnection and the cursor
// release of OpMsgCursor::kill(). It needs no real MongoDB server: every test
// runs against one or two instances of the in-process MongoDBTestServer, whose
// replies are scripted per command (an error reply, no reply at all, a raw byte
// sequence, or a closed connection).
//
// Counting conventions: constructing the ReplicaSet sends one hello per server
// and each topology refresh sends one more, so the expected hello count is the
// number of refreshes plus one. ReplicaSetConnection makes max(serverCount, 5)
// attempts, which is MAX_ATTEMPTS for the topologies used here. CONNECT_TIMEOUT
// and SOCKET_TIMEOUT bound every test that waits for a reply that never comes;
// such a test is named "...AfterNoReply", because the socket timeout surfaces as
// a Poco::IOException from the header read, not as a Poco::TimeoutException.
//


#include "ReplicaSetConnectionTest.h"
#include "MongoDBTestServer.h"
#include "CppUnit/CppUnitException.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/BinaryWriter.h"
#include "Poco/Exception.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/MessageHeader.h"
#include "Poco/MongoDB/OpMsgCursor.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/MongoDB/ReadPreference.h"
#include "Poco/MongoDB/ReplicaSet.h"
#include "Poco/MongoDB/ReplicaSetConnection.h"
#include "Poco/MongoDB/TopologyDescription.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Timespan.h"
#include <chrono>
#include <condition_variable>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>


using namespace Poco::MongoDB;
using namespace std::string_literals;


namespace
{
	const int MAX_ATTEMPTS = 5; // ReplicaSetConnection makes max(serverCount, 5) attempts; each refresh adds one hello per server
	const Poco::Timespan CONNECT_TIMEOUT(1, 0);
	const Poco::Timespan SOCKET_TIMEOUT(1, 0);
	const Poco::Int64 CURSOR_ID = 42;

	using Action = MongoDBTestServer::Action;
	using Servers = std::vector<MongoDBTestServer*>;

	// Constructing the ReplicaSet sends one hello per server; each topology refresh adds one more per server.
	ReplicaSet::Config makeConfig(const Servers& servers)
	{
		ReplicaSet::Config config;
		for (const auto* server: servers)
			config.seeds.push_back(server->address());
		config.setName = "rs0"s;
		config.enableMonitoring = false;
		config.connectTimeoutSeconds = 1;
		config.socketTimeoutSeconds = 1;
		config.serverReconnectRetries = 0;
		config.serverReconnectDelaySeconds = 0;
		return config;
	}

	class Fixture
		/// Owns the test servers, the ReplicaSet, the ReplicaSetConnection and the
		/// last reply.
		///
		/// The servers are listening before setup runs, and setup runs before the
		/// ReplicaSet discovers the topology, so a test can decide there what the
		/// initial hello replies say. With more than one server, every server
		/// reports all of them in its hello reply. With configTimeouts, the
		/// connection is built without explicit timeouts and uses the ones the
		/// ReplicaSet is configured with.
	{
	public:
		using Setup = std::function<void(const Servers&)>;

		explicit Fixture(std::size_t serverCount = 1,
			ReadPreference::Mode mode = ReadPreference::Primary,
			const Setup& setup = Setup(),
			bool configTimeouts = false):
			_servers(serverCount)
		{
			std::vector<std::string> hosts;
			for (auto& server: _servers)
			{
				_pointers.push_back(&server);
				hosts.push_back(server.hostAndPort());
			}
			if (serverCount > 1)
			{
				for (auto* server: _pointers)
					server->setHosts(hosts);
			}
			if (setup)
				setup(_pointers);

			_replicaSet = std::make_unique<ReplicaSet>(makeConfig(_pointers));
			_connection = configTimeouts
				? std::make_unique<ReplicaSetConnection>(*_replicaSet, ReadPreference(mode))
				: std::make_unique<ReplicaSetConnection>(*_replicaSet,
					ReadPreference(mode), CONNECT_TIMEOUT, SOCKET_TIMEOUT);
		}

		MongoDBTestServer& server(std::size_t index = 0) { return *_pointers[index]; }
		ReplicaSet& replicaSet() { return *_replicaSet; }
		ReplicaSetConnection& connection() { return *_connection; }
		OpMsgMessage& response() { return _response; }

		OpMsgMessage& send(OpMsgMessage& request)
			/// Sends request and returns the reply, which stays available after a failure.
		{
			_connection->sendRequest(request, _response);
			return _response;
		}

	private:
		std::vector<MongoDBTestServer> _servers;
		Servers _pointers;
		OpMsgMessage _response;
		std::unique_ptr<ReplicaSet> _replicaSet;
		std::unique_ptr<ReplicaSetConnection> _connection;
	};

	struct Request: public OpMsgMessage
		/// A request with the command name already set.
	{
		explicit Request(const std::string& command,
			const std::string& db = "db"s, const std::string& collection = "c"s):
			OpMsgMessage(db, collection)
		{
			setCommandName(command);
		}
	};

	struct InsertRequest: public Request
		/// An insert of a single document.
	{
		InsertRequest(): Request(OpMsgMessage::CMD_INSERT)
		{
			Document::Ptr doc = new Document();
			doc->add("_id"s, 1);
			documents().push_back(doc);
		}
	};

	struct AggregateRequest: public Request
		/// An aggregate over the given pipeline stages.
	{
		explicit AggregateRequest(const std::vector<Document::Ptr>& stages):
			Request(OpMsgMessage::CMD_AGGREGATE)
		{
			Array& pipeline = body().addNewArray("pipeline"s);
			for (const auto& stage: stages)
				pipeline.add(stage);
		}
	};

	Document::Ptr matchStage()
	{
		Document::Ptr stage = new Document();
		stage->addNewDocument("$match"s);
		return stage;
	}

	Document::Ptr outStage()
	{
		Document::Ptr stage = new Document();
		stage->add("$out"s, "x"s);
		return stage;
	}

	Document::Ptr mergeStage()
	{
		Document::Ptr stage = new Document();
		stage->addNewDocument("$merge"s).add("into"s, "x"s);
		return stage;
	}

	void scriptAction(MongoDBTestServer& server, const std::string& command, Action action, int times = 1)
	{
		MongoDBTestServer::Behaviour behaviour;
		behaviour.action = action;
		server.addBehaviour(command, behaviour, times);
	}

	void scriptReply(MongoDBTestServer& server, const std::string& command,
		const Document::Ptr& reply, int times = 1)
	{
		MongoDBTestServer::Behaviour behaviour;
		behaviour.reply = reply;
		server.addBehaviour(command, behaviour, times);
	}

	void scriptError(MongoDBTestServer& server, const std::string& command, Poco::Int32 code,
		const std::string& codeName, const std::string& errmsg = std::string(), int times = 1)
	{
		scriptReply(server, command, MongoDBTestServer::errorReply(code, codeName, errmsg), times);
	}

	void scriptRaw(MongoDBTestServer& server, const std::string& command, const std::string& raw)
	{
		MongoDBTestServer::Behaviour behaviour;
		behaviour.action = Action::RawReply;
		behaviour.raw = raw;
		server.addBehaviour(command, behaviour);
	}

	void scriptCursor(MongoDBTestServer& server)
		/// find opens cursor CURSOR_ID with one document; killCursors releases it by default.
	{
		Document::Ptr doc = new Document();
		doc->add("_id"s, 1);
		scriptReply(server, "find"s, MongoDBTestServer::cursorReply("db.c"s, CURSOR_ID, { doc }));

		MongoDBTestServer::Behaviour killed;
		killed.reply = new Document();
		killed.reply->addNewArray("cursorsKilled"s).add(CURSOR_ID);
		killed.reply->add("ok"s, 1.0);
		server.setDefaultBehaviour("killCursors"s, killed);
	}

	void writeHeader(Poco::BinaryWriter& writer, Poco::Int32 messageLength)
	{
		writer << messageLength << Poco::Int32(0) << Poco::Int32(0) << static_cast<Poco::Int32>(Poco::MongoDB::MessageHeader::OP_MSG);
	}

	template <class E, class F>
	void expectThrows(F&& function, const std::string& message)
		/// Runs function and requires it to throw E. Any other exception propagates.
	{
		try
		{
			function();
		}
		catch (const E&)
		{
			return;
		}
		throw CppUnit::CppUnitException("fail: "s + message);
	}
}


ReplicaSetConnectionTest::ReplicaSetConnectionTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


ReplicaSetConnectionTest::~ReplicaSetConnectionTest()
{
}


void ReplicaSetConnectionTest::setUp()
{
}


void ReplicaSetConnectionTest::tearDown()
{
}


void ReplicaSetConnectionTest::testServerHelloAndPing()
{
	Fixture fx;
	assertTrue(fx.replicaSet().topology().hasPrimary());

	Request request(OpMsgMessage::CMD_PING, "admin"s, ""s);
	assertTrue(fx.send(request).responseOk());
	assertEqual(1, fx.server().commandCount("ping"s));
	assertTrue(fx.server().commandCount("hello"s) >= 1);
	// Nothing threw on the server thread, so the counts above describe the whole exchange.
	assertEqual(0, fx.server().errorCount());
	assertTrue(fx.server().lastError().empty());
}


void ReplicaSetConnectionTest::testScriptedErrorReply()
{
	MongoDBTestServer server;
	scriptError(server, "find"s, 2, "BadValue"s, "bad"s);

	Connection connection;
	connection.connect(server.address(), CONNECT_TIMEOUT, SOCKET_TIMEOUT);
	Request request(OpMsgMessage::CMD_FIND, "test"s, "coll"s);
	OpMsgMessage response;
	connection.sendRequest(request, response);

	assertFalse(response.responseOk());
	assertEqual(2, response.body().get<Poco::Int32>("code"s, 0));
	assertEqual(1, server.commandCount("find"s));
}


void ReplicaSetConnectionTest::testNoReplyTimesOut()
{
	MongoDBTestServer server;
	scriptAction(server, "find"s, Action::NoReply);

	Connection connection;
	connection.connect(server.address(), CONNECT_TIMEOUT, Poco::Timespan(0, 300000));
	Request request(OpMsgMessage::CMD_FIND, "test"s, "coll"s);
	OpMsgMessage response;
	expectThrows<Poco::IOException>([&]() { connection.sendRequest(request, response); },
		"sendRequest must time out"s);

	assertEqual(1, server.commandCount("find"s));
	assertFalse(connection.isConnected());
}


void ReplicaSetConnectionTest::testCloseConnectionAndStopListening()
{
	MongoDBTestServer server;
	scriptAction(server, "find"s, Action::CloseConnection);

	Connection connection;
	connection.connect(server.address(), CONNECT_TIMEOUT, SOCKET_TIMEOUT);
	Request request(OpMsgMessage::CMD_FIND, "test"s, "coll"s);
	OpMsgMessage response;
	expectThrows<Poco::IOException>([&]() { connection.sendRequest(request, response); },
		"sendRequest must fail on a closed connection"s);

	server.stopListening();
	Poco::Net::StreamSocket socket;
	try
	{
		socket.connect(server.address(), CONNECT_TIMEOUT);
		fail("connect must be refused"s);
	}
	catch (const Poco::Net::NetException&)
	{
	}
	catch (const Poco::TimeoutException&)
	{
		// On Windows, a refused loopback connect keeps retrying until the timeout instead of failing at once.
	}
}


void ReplicaSetConnectionTest::testPartialMessageDoesNotStall()
{
	MongoDBTestServer server;

	Request ping(OpMsgMessage::CMD_PING, "admin"s, ""s);
	std::ostringstream encoded;
	ping.send(encoded);
	const std::string header = encoded.str().substr(0, 10);

	Poco::Net::StreamSocket stalled;
	stalled.connect(server.address(), CONNECT_TIMEOUT);
	assertEqual(static_cast<int>(header.size()), stalled.sendBytes(header.data(), static_cast<int>(header.size())));

	Connection connection;
	connection.connect(server.address(), CONNECT_TIMEOUT, SOCKET_TIMEOUT);
	Request request(OpMsgMessage::CMD_PING, "admin"s, ""s);
	OpMsgMessage response;
	connection.sendRequest(request, response);
	assertTrue(response.responseOk());

	// The receive timeout is longer than the server's deadline for incomplete messages.
	stalled.setReceiveTimeout(Poco::Timespan(3, 0));
	char buffer[16];
	try
	{
		assertEqual(0, stalled.receiveBytes(buffer, sizeof(buffer)));
	}
	catch (const Poco::TimeoutException&)
	{
		fail("the server must close a client with an incomplete message"s);
	}
	catch (const Poco::Net::NetException&)
	{
	}
}


void ReplicaSetConnectionTest::testFindServerErrorBounded()
{
	Fixture fx;
	scriptError(fx.server(), "find"s, 6, "HostUnreachable"s, "host unreachable"s, MAX_ATTEMPTS * 4);

	Request request(OpMsgMessage::CMD_FIND);
	try
	{
		fx.send(request);
		fail("sendRequest must stop after the attempt limit"s);
	}
	catch (const Poco::IOException& e)
	{
		assertTrue(e.displayText().find("HostUnreachable"s) != std::string::npos);
	}

	assertEqual(6, fx.response().body().get<Poco::Int32>("code"s, 0));
	assertEqual(MAX_ATTEMPTS, fx.server().commandCount("find"s));
	assertEqual(MAX_ATTEMPTS + 1, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testFindNoReplyBounded()
{
	Fixture fx;
	scriptAction(fx.server(), "find"s, Action::NoReply, MAX_ATTEMPTS + 3);

	Request request(OpMsgMessage::CMD_FIND);
	expectThrows<Poco::IOException>([&]() { fx.send(request); },
		"sendRequest must stop after the attempt limit"s);

	assertEqual(MAX_ATTEMPTS, fx.server().commandCount("find"s));
	assertEqual(MAX_ATTEMPTS + 1, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testFindSucceedsAfterServerErrors()
{
	Fixture fx;
	scriptError(fx.server(), "find"s, 189, "PrimarySteppedDown"s, "primary stepped down"s, MAX_ATTEMPTS - 1);

	Request request(OpMsgMessage::CMD_FIND);
	assertTrue(fx.send(request).responseOk());
	// The successful reply replaces the error replies of the earlier attempts.
	assertFalse(fx.response().body().exists("code"s));
	assertFalse(fx.response().body().exists("errmsg"s));
	assertEqual(MAX_ATTEMPTS, fx.server().commandCount("find"s));
	assertEqual(MAX_ATTEMPTS, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testFindResentAfterNoReply()
{
	Fixture fx;
	scriptAction(fx.server(), "find"s, Action::NoReply);

	Request request(OpMsgMessage::CMD_FIND);
	const int before = fx.server().totalConnections();
	assertTrue(fx.send(request).responseOk());
	assertEqual(2, fx.server().commandCount("find"s));
	assertEqual(2, fx.server().commandCount("hello"s));
	// The first attempt, the refresh hello and the re-send each open a connection.
	assertEqual(before + 3, fx.server().totalConnections());
}


void ReplicaSetConnectionTest::testInsertNotResentAfterNoReply()
{
	Fixture fx;
	scriptAction(fx.server(), "insert"s, Action::NoReply);

	InsertRequest request;
	expectThrows<Poco::IOException>([&]() { fx.send(request); },
		"an insert must not be sent again when no reply arrives"s);

	assertEqual(1, fx.server().commandCount("insert"s));
	assertEqual(2, fx.server().commandCount("hello"s));
	assertFalse(fx.connection().isConnected());
}


void ReplicaSetConnectionTest::testInsertNotResentAfterConnectionClosed()
{
	Fixture fx;
	scriptAction(fx.server(), "insert"s, Action::CloseConnection);

	InsertRequest request;
	expectThrows<Poco::IOException>([&]() { fx.send(request); },
		"an insert must not be sent again after the connection is lost"s);

	assertEqual(1, fx.server().commandCount("insert"s));
	assertEqual(2, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testInsertResentAfterNotWritablePrimary()
{
	// Before discovery: only server 0 is the primary.
	Fixture fx(2, ReadPreference::Primary, [](const Servers& servers) { servers[1]->setPrimary(false); });

	// Monitoring is off, so the topology still has the first server as the primary.
	fx.server(0).setPrimary(false);
	fx.server(1).setPrimary(true);
	scriptError(fx.server(0), "insert"s, 10107, "NotWritablePrimary"s, "not primary"s);

	InsertRequest request;
	assertTrue(fx.send(request).responseOk());
	assertEqual(1, fx.server(0).commandCount("insert"s));
	assertEqual(1, fx.server(1).commandCount("insert"s));
	assertEqual(2, fx.server(0).commandCount("hello"s));
	assertEqual(2, fx.server(1).commandCount("hello"s));
}


void ReplicaSetConnectionTest::testInsertNotResentAfterStateChange()
{
	Fixture fx;
	scriptError(fx.server(), "insert"s, 11602, "InterruptedDueToReplStateChange"s, "state change"s);

	InsertRequest request;
	expectThrows<Poco::IOException>([&]() { fx.send(request); },
		"an insert that may have been executed must not be sent again"s);

	assertEqual(11602, fx.response().body().get<Poco::Int32>("code"s, 0));
	assertEqual(1, fx.server().commandCount("insert"s));
	assertEqual(2, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testWriteErrorsReplyReturned()
{
	Document::Ptr reply = new Document();
	reply->add("ok"s, 1.0);
	reply->add("n"s, 0);
	Document::Ptr writeError = new Document();
	writeError->add("index"s, 0);
	writeError->add("code"s, 11000);
	writeError->add("errmsg"s, "E11000 duplicate key"s);
	reply->addNewArray("writeErrors"s).add(writeError);

	Fixture fx;
	scriptReply(fx.server(), "insert"s, reply);

	InsertRequest request;
	assertTrue(fx.send(request).responseOk());
	assertTrue(fx.response().body().exists("writeErrors"s));
	assertEqual(1, fx.server().commandCount("insert"s));
	assertEqual(1, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testWriteConcernErrorReturned()
{
	Document::Ptr reply = new Document();
	reply->add("ok"s, 1.0);
	reply->add("n"s, 1);
	Document& wce = reply->addNewDocument("writeConcernError"s);
	wce.add("code"s, 64);
	wce.add("codeName"s, "WriteConcernFailed"s);
	wce.add("errmsg"s, "waiting for replication timed out"s);

	Fixture fx;
	scriptReply(fx.server(), "insert"s, reply);

	InsertRequest request;
	assertTrue(fx.send(request).responseOk());
	assertTrue(fx.response().body().exists("writeConcernError"s));
	assertEqual(1, fx.server().commandCount("insert"s));
	assertEqual(1, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testGetMoreNotResentAfterNoReply()
{
	Fixture fx;
	scriptCursor(fx.server());
	scriptAction(fx.server(), "getMore"s, Action::NoReply);

	OpMsgCursor cursor("db"s, "c"s);
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);

	OpMsgMessage& first = cursor.next(fx.connection());
	assertTrue(first.responseOk());
	assertEqual(1, static_cast<int>(first.documents().size()));
	expectThrows<Poco::IOException>([&]() { cursor.next(fx.connection()); },
		"getMore must not be sent again when no reply arrives"s);

	assertEqual(1, fx.server().commandCount("getMore"s));
	assertEqual(2, fx.server().commandCount("hello"s));

	cursor.kill(fx.connection());
	assertEqual(Poco::Int64(0), cursor.cursorID());
}


void ReplicaSetConnectionTest::testGetMoreNotResentAfterNotPrimary()
{
	Fixture fx;
	scriptCursor(fx.server());
	scriptError(fx.server(), "getMore"s, 13435, "NotPrimaryNoSecondaryOk"s, "not primary"s);

	OpMsgCursor cursor("db"s, "c"s);
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);

	assertTrue(cursor.next(fx.connection()).responseOk());
	expectThrows<Poco::IOException>([&]() { cursor.next(fx.connection()); },
		"getMore must not be sent again"s);

	assertEqual(1, fx.server().commandCount("getMore"s));
	assertEqual(2, fx.server().commandCount("hello"s));

	cursor.kill(fx.connection());
	assertEqual(Poco::Int64(0), cursor.cursorID());
}


void ReplicaSetConnectionTest::testKillCursorsNotResent()
{
	Fixture fx;
	scriptCursor(fx.server());
	scriptAction(fx.server(), "killCursors"s, Action::CloseConnection);

	OpMsgCursor cursor("db"s, "c"s);
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);

	assertTrue(cursor.next(fx.connection()).responseOk());
	expectThrows<Poco::IOException>([&]() { cursor.kill(fx.connection()); },
		"killCursors must report the connection loss"s);

	assertEqual(1, fx.server().commandCount("killCursors"s));
	assertEqual(2, fx.server().commandCount("hello"s));
	assertEqual(Poco::Int64(0), cursor.cursorID());

	// The cursor is already released, so a second kill() must be a no-op.
	cursor.kill(fx.connection());
	assertEqual(1, fx.server().commandCount("killCursors"s));
}


void ReplicaSetConnectionTest::testKillAcceptsCursorNotFound()
{
	MongoDBTestServer::Behaviour notFound;
	notFound.reply = new Document();
	notFound.reply->addNewArray("cursorsKilled"s);
	notFound.reply->addNewArray("cursorsNotFound"s).add(CURSOR_ID);
	notFound.reply->addNewArray("cursorsAlive"s);
	notFound.reply->addNewArray("cursorsUnknown"s);
	notFound.reply->add("ok"s, 1.0);

	Fixture fx;
	scriptCursor(fx.server());
	fx.server().setDefaultBehaviour("killCursors"s, notFound);

	OpMsgCursor cursor("db"s, "c"s);
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);

	assertTrue(cursor.next(fx.connection()).responseOk());
	cursor.kill(fx.connection());

	assertEqual(Poco::Int64(0), cursor.cursorID());
}


void ReplicaSetConnectionTest::testKillWithUnconfirmedReplyReleasesCursor()
{
	MongoDBTestServer::Behaviour unconfirmed;
	unconfirmed.reply = new Document();
	unconfirmed.reply->addNewArray("cursorsKilled"s);
	unconfirmed.reply->addNewArray("cursorsNotFound"s);
	unconfirmed.reply->add("ok"s, 1.0);

	Fixture fx;
	scriptCursor(fx.server());
	fx.server().setDefaultBehaviour("killCursors"s, unconfirmed);

	OpMsgCursor cursor("db"s, "c"s);
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);

	assertTrue(cursor.next(fx.connection()).responseOk());
	expectThrows<Poco::ProtocolException>([&]() { cursor.kill(fx.connection()); },
		"kill() must report an unconfirmed reply"s);

	assertEqual(1, fx.server().commandCount("killCursors"s));
	assertEqual(Poco::Int64(0), cursor.cursorID());
}


void ReplicaSetConnectionTest::testAggregateWithOutputStageNotResent()
{
	Fixture fx;
	scriptAction(fx.server(), "aggregate"s, Action::CloseConnection, 2);

	AggregateRequest outRequest({ matchStage(), outStage() });
	expectThrows<Poco::IOException>([&]() { fx.send(outRequest); },
		"aggregate with $out must not be sent again"s);

	AggregateRequest mergeRequest({ matchStage(), mergeStage() });
	expectThrows<Poco::IOException>([&]() { fx.send(mergeRequest); },
		"aggregate with $merge must not be sent again"s);

	assertEqual(2, fx.server().commandCount("aggregate"s));
	assertEqual(3, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testAggregateResentAfterConnectionLoss()
{
	Fixture fx;
	scriptAction(fx.server(), "aggregate"s, Action::CloseConnection);

	AggregateRequest request({ matchStage() });
	assertTrue(fx.send(request).responseOk());
	assertEqual(2, fx.server().commandCount("aggregate"s));
	assertEqual(2, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testConnectFailureRetriedOnSecondServer()
{
	Fixture fx(2, ReadPreference::PrimaryPreferred,
		[](const Servers& servers) { servers[1]->setPrimary(false); });
	fx.server(0).stopListening();

	Request request(OpMsgMessage::CMD_FIND);
	assertTrue(fx.send(request).responseOk());
	assertEqual(0, fx.server(0).commandCount("find"s));
	assertEqual(1, fx.server(1).commandCount("find"s));
	assertEqual(1, fx.server(0).commandCount("hello"s));
	assertEqual(1, fx.server(1).commandCount("hello"s));
}


void ReplicaSetConnectionTest::testStaleConnectionFindRecoversInsertFails()
{
	Fixture fx;

	Request find(OpMsgMessage::CMD_FIND);
	assertTrue(fx.send(find).responseOk());

	fx.server().closeConnections();
	int before = fx.server().totalConnections();
	assertTrue(fx.send(find).responseOk());
	// The request written to the closed connection does not reach the server.
	assertEqual(2, fx.server().commandCount("find"s));
	assertEqual(2, fx.server().commandCount("hello"s));
	// The refresh hello and the re-send; the stale connection is reused for the first attempt.
	assertEqual(before + 2, fx.server().totalConnections());

	fx.server().closeConnections();
	before = fx.server().totalConnections();
	InsertRequest insert;
	expectThrows<Poco::IOException>([&]() { fx.send(insert); },
		"an insert must not be sent again on a new connection"s);
	assertEqual(0, fx.server().commandCount("insert"s));
	assertEqual(3, fx.server().commandCount("hello"s));
	// Only the refresh hello; no re-send.
	assertEqual(before + 1, fx.server().totalConnections());
}


void ReplicaSetConnectionTest::testMalformedReplyNotResent()
{
	// messageLength 16 is below the OP_MSG minimum of 20.
	std::ostringstream raw;
	Poco::BinaryWriter writer(raw, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writeHeader(writer, 16);
	writer.flush();

	Fixture fx;
	scriptRaw(fx.server(), "find"s, raw.str());

	Request request(OpMsgMessage::CMD_FIND);
	expectThrows<Poco::DataFormatException>([&]() { fx.send(request); },
		"a malformed reply must be reported"s);

	assertEqual(1, fx.server().commandCount("find"s));
	assertEqual(1, fx.server().commandCount("hello"s));
	// A malformed message makes Connection close the socket.
	assertFalse(fx.connection().isConnected());
}


void ReplicaSetConnectionTest::testUnsupportedElementTypeNotResent()
{
	// Body { x: undefined }: BSON type 0x06 is not implemented by Document::readValue.
	const std::size_t documentSize = 8;
	std::ostringstream raw;
	Poco::BinaryWriter writer(raw, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writeHeader(writer, static_cast<Poco::Int32>(16 + 4 + 1 + documentSize));
	writer << Poco::UInt32(0) << '\0';
	writer << static_cast<Poco::Int32>(documentSize) << '\x06' << 'x' << '\0' << '\0';
	writer.flush();

	Fixture fx;
	scriptRaw(fx.server(), "find"s, raw.str());

	Request request(OpMsgMessage::CMD_FIND);
	expectThrows<Poco::NotImplementedException>([&]() { fx.send(request); },
		"an unsupported element type must be reported"s);

	assertEqual(1, fx.server().commandCount("find"s));
	assertEqual(1, fx.server().commandCount("hello"s));
	// NotImplementedException is thrown after the whole reply was read, so the socket stays in sync.
	assertTrue(fx.connection().isConnected());
}


void ReplicaSetConnectionTest::testKillCursorsNotResentAfterNotPrimary()
{
	Fixture fx;
	scriptCursor(fx.server());
	scriptError(fx.server(), "killCursors"s, 13435, "NotPrimaryNoSecondaryOk"s, "not primary"s);

	OpMsgCursor cursor("db"s, "c"s);
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);

	assertTrue(cursor.next(fx.connection()).responseOk());
	expectThrows<Poco::IOException>([&]() { cursor.kill(fx.connection()); },
		"killCursors must not be sent again"s);

	assertEqual(1, fx.server().commandCount("killCursors"s));
	assertEqual(2, fx.server().commandCount("hello"s));

	assertEqual(Poco::Int64(0), cursor.cursorID());
}


void ReplicaSetConnectionTest::testNoSuitableServer()
{
	Fixture fx(1, ReadPreference::Primary, [](const Servers& servers) { servers[0]->setPrimary(false); });

	Request request(OpMsgMessage::CMD_FIND);
	try
	{
		fx.send(request);
		fail("sendRequest must fail without a primary"s);
	}
	catch (const Poco::IOException& e)
	{
		assertTrue(e.displayText().find("No suitable server found in replica set"s) != std::string::npos);
	}

	assertEqual(0, fx.server().commandCount("find"s));
	assertEqual(1, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testFindResentAfterNotExecutedReply()
{
	Fixture fx;
	scriptError(fx.server(), "find"s, 13436, "NotPrimaryOrSecondary"s, "not primary or secondary"s);

	Request request(OpMsgMessage::CMD_FIND);
	assertTrue(fx.send(request).responseOk());
	assertEqual(2, fx.server().commandCount("find"s));
	assertEqual(2, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testInsertResentAfterLegacyNotPrimary()
{
	Fixture fx;
	scriptError(fx.server(), "insert"s, 10058, "LegacyNotPrimary"s, "legacy not primary"s);

	InsertRequest request;
	assertTrue(fx.send(request).responseOk());
	assertEqual(2, fx.server().commandCount("insert"s));
	assertEqual(2, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testCountResentAfterNoReply()
{
	Fixture fx;
	scriptAction(fx.server(), "count"s, Action::NoReply);

	Request request(OpMsgMessage::CMD_COUNT);
	const int before = fx.server().totalConnections();
	assertTrue(fx.send(request).responseOk());
	assertEqual(2, fx.server().commandCount("count"s));
	assertEqual(2, fx.server().commandCount("hello"s));
	// The first attempt, the refresh hello and the re-send each open a connection.
	assertEqual(before + 3, fx.server().totalConnections());
}


void ReplicaSetConnectionTest::testNoSuitableServerAfterErrorReply()
{
	Fixture fx;
	scriptError(fx.server(), "find"s, 10107, "NotWritablePrimary"s);
	fx.server().setPrimary(false);

	Request request(OpMsgMessage::CMD_FIND);
	try
	{
		fx.send(request);
		fail("sendRequest must fail when no primary is left"s);
	}
	catch (const Poco::IOException& e)
	{
		assertTrue(e.displayText().find("No suitable server found in replica set"s) != std::string::npos);
		// The error reply that ended the first attempt is nested in it.
		assertTrue(e.nested() != nullptr);
		assertTrue(e.nested()->displayText().find("MongoDB server error"s) != std::string::npos);
	}

	assertEqual(10107, fx.response().body().get<Poco::Int32>("code"s, 0));
	assertEqual(1, fx.server().commandCount("find"s));
	assertEqual(2, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testNoSuitableServerAfterNoReply()
{
	Fixture fx;
	scriptAction(fx.server(), "find"s, Action::NoReply);
	fx.server().setPrimary(false);

	Request request(OpMsgMessage::CMD_FIND);
	try
	{
		fx.send(request);
		fail("sendRequest must fail when no primary is left"s);
	}
	catch (const Poco::IOException& e)
	{
		assertTrue(e.displayText().find("No suitable server found in replica set"s) != std::string::npos);
		assertTrue(e.nested() != nullptr);
		// The lost reply is nested as the I/O failure it is, whatever the socket layer calls it.
		assertTrue(dynamic_cast<const Poco::IOException*>(e.nested()) != nullptr);
	}

	assertEqual(1, fx.server().commandCount("find"s));
	assertEqual(2, fx.server().commandCount("hello"s));
}



void ReplicaSetConnectionTest::testUnlistedErrorReplyReturned()
{
	Fixture fx;
	scriptError(fx.server(), "find"s, 2, "BadValue"s, "bad"s);

	// BadValue is not one of the listed errors, so the reply is returned, not thrown.
	Request request(OpMsgMessage::CMD_FIND);
	assertFalse(fx.send(request).responseOk());
	assertEqual(2, fx.response().body().get<Poco::Int32>("code"s, 0));
	assertEqual(1, fx.server().commandCount("find"s));
	// No re-send and no topology refresh.
	assertEqual(1, fx.server().commandCount("hello"s));
	assertTrue(fx.connection().isConnected());
}


void ReplicaSetConnectionTest::testCodelessErrorReplyReturned()
{
	Document::Ptr reply = new Document();
	reply->add("ok"s, 0.0);
	reply->add("errmsg"s, "no code"s);

	Fixture fx;
	scriptReply(fx.server(), "find"s, reply);

	// A failed reply without a code cannot be classified, so it is returned as it is.
	Request request(OpMsgMessage::CMD_FIND);
	assertFalse(fx.send(request).responseOk());
	assertFalse(fx.response().body().exists("code"s));
	assertEqual(1, fx.server().commandCount("find"s));
	// No re-send and no topology refresh.
	assertEqual(1, fx.server().commandCount("hello"s));
	assertTrue(fx.connection().isConnected());
}


void ReplicaSetConnectionTest::testCursorRequestsCarryCursorId()
{
	Fixture fx;
	scriptCursor(fx.server());
	scriptReply(fx.server(), "getMore"s, MongoDBTestServer::cursorReply("db.c"s, 0, {}, false));

	OpMsgCursor cursor("db"s, "c"s);
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);
	assertTrue(cursor.next(fx.connection()).responseOk());
	assertTrue(cursor.next(fx.connection()).responseOk());

	// getMore names the cursor the find reply opened.
	const Document::Ptr getMore = fx.server().lastRequest("getMore"s);
	assertTrue(!getMore.isNull());
	assertEqual(CURSOR_ID, getMore->get<Poco::Int64>("getMore"s, 0));

	// A second cursor, because the reply with id 0 released the first one.
	scriptCursor(fx.server());
	OpMsgCursor second("db"s, "c"s);
	second.query().setCommandName(OpMsgMessage::CMD_FIND);
	assertTrue(second.next(fx.connection()).responseOk());
	second.kill(fx.connection());

	// killCursors names that cursor and no other.
	const Document::Ptr killCursors = fx.server().lastRequest("killCursors"s);
	assertTrue(!killCursors.isNull());
	const Array::Ptr cursors = killCursors->get<Array::Ptr>("cursors"s, nullptr);
	assertTrue(!cursors.isNull());
	assertEqual(1, static_cast<int>(cursors->size()));
	assertEqual(CURSOR_ID, cursors->get<Poco::Int64>(0, -1));
}


void ReplicaSetConnectionTest::testAttemptBoundScalesWithServerCount()
{
	// Six servers allow six attempts; the floor of five alone would allow five.
	const std::size_t serverCount = 6;
	Fixture fx(serverCount, ReadPreference::Nearest,
		[](const Servers& servers)
		{
			for (auto* server: servers)
				server->setPrimary(false);
		});
	for (std::size_t i = 0; i < serverCount; ++i)
		scriptError(fx.server(i), "find"s, 6, "HostUnreachable"s, "host unreachable"s, 8);

	Request request(OpMsgMessage::CMD_FIND);
	expectThrows<Poco::IOException>([&]() { fx.send(request); },
		"sendRequest must stop after the attempt limit"s);

	// Every attempt reached one of the servers, wherever selection sent it.
	int attempts = 0;
	for (std::size_t i = 0; i < serverCount; ++i)
		attempts += fx.server(i).commandCount("find"s);
	assertEqual(static_cast<int>(serverCount), attempts);
}


void ReplicaSetConnectionTest::testSafeReadsResentAndOtherCommandsNot()
{
	Fixture fx;

	const std::vector<std::string> safeReads {
		"find"s, "count"s, "distinct"s, "listCollections"s, "listIndexes"s, "listDatabases"s,
		"explain"s, "hello"s, "isMaster"s, "ismaster"s, "ping"s, "buildInfo"s
	};
	for (const auto& command: safeReads)
	{
		scriptAction(fx.server(), command, Action::CloseConnection);
		const int before = fx.server().commandCount(command);
		Request request(command);
		// A read without side effects is sent again after the connection is lost.
		if (!fx.send(request).responseOk())
			fail("a safe read must succeed after the connection was closed: "s + command);
		// The lost attempt and the re-send; for hello the refresh in between sends one more.
		const int expected = before + (command == "hello"s ? 3 : 2);
		if (fx.server().commandCount(command) != expected)
			fail("a safe read must be sent again: "s + command);
	}

	const std::vector<std::string> otherCommands { "update"s, "delete"s, "findAndModify"s };
	for (const auto& command: otherCommands)
	{
		scriptAction(fx.server(), command, Action::CloseConnection);
		Request request(command);
		// A command that may already have been executed is reported instead.
		expectThrows<Poco::IOException>([&]() { fx.send(request); },
			"a command with side effects must not be sent again: "s + command);
		if (fx.server().commandCount(command) != 1)
			fail("a command with side effects must be sent once: "s + command);
	}
}


void ReplicaSetConnectionTest::testCursorFollowsResentFind()
{
	Fixture fx(2, ReadPreference::PrimaryPreferred,
		[](const Servers& servers) { servers[1]->setPrimary(false); });

	// The ping opens the connection to the primary, which then stops answering.
	Request ping(OpMsgMessage::CMD_PING, "admin"s, ""s);
	assertTrue(fx.send(ping).responseOk());
	fx.server(0).stopListening();
	scriptAction(fx.server(0), "find"s, Action::CloseConnection);
	scriptCursor(fx.server(1));
	scriptReply(fx.server(1), "getMore"s, MongoDBTestServer::cursorReply("db.c"s, CURSOR_ID, {}, false));

	OpMsgCursor cursor("db"s, "c"s);
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);
	// The find is re-sent to the secondary once the refresh marks the primary unknown.
	assertTrue(cursor.next(fx.connection()).responseOk());
	assertTrue(cursor.next(fx.connection()).responseOk());
	cursor.kill(fx.connection());

	// The cursor stays on the server that opened it.
	assertEqual(1, fx.server(0).commandCount("find"s));
	assertEqual(0, fx.server(0).commandCount("getMore"s));
	assertEqual(0, fx.server(0).commandCount("killCursors"s));
	assertEqual(1, fx.server(1).commandCount("find"s));
	assertEqual(1, fx.server(1).commandCount("getMore"s));
	assertEqual(1, fx.server(1).commandCount("killCursors"s));

	// Both follow-up requests name the cursor that server 1 opened.
	const Document::Ptr getMore = fx.server(1).lastRequest("getMore"s);
	assertTrue(!getMore.isNull());
	assertEqual(CURSOR_ID, getMore->get<Poco::Int64>("getMore"s, 0));
	const Document::Ptr killCursors = fx.server(1).lastRequest("killCursors"s);
	assertTrue(!killCursors.isNull());
	const Array::Ptr cursors = killCursors->get<Array::Ptr>("cursors"s, nullptr);
	assertTrue(!cursors.isNull());
	assertEqual(CURSOR_ID, cursors->get<Poco::Int64>(0, -1));
}


void ReplicaSetConnectionTest::testGetMoreCursorNotFoundReturned()
{
	Fixture fx;
	scriptCursor(fx.server());
	scriptError(fx.server(), "getMore"s, 43, "CursorNotFound"s, "cursor not found"s);

	OpMsgCursor cursor("db"s, "c"s);
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);
	assertTrue(cursor.next(fx.connection()).responseOk());

	// CursorNotFound is not a listed error, so next() returns the reply instead of throwing.
	assertFalse(cursor.next(fx.connection()).responseOk());
	assertFalse(cursor.isActive());
	assertEqual(Poco::Int64(0), cursor.cursorID());

	// The cursor is already released, so kill() sends nothing.
	cursor.kill(fx.connection());
	assertEqual(0, fx.server().commandCount("killCursors"s));
	// The connection was kept, so there was no refresh.
	assertEqual(1, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testOneWayRequestDelivered()
{
	Fixture fx;

	InsertRequest request;
	fx.connection().sendRequest(request);

	// A one-way request is not answered, so its arrival shows only in the server's count.
	for (int i = 0; i < 200 && fx.server().commandCount("insert"s) < 1; ++i)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	assertEqual(1, fx.server().commandCount("insert"s));
	assertEqual(1, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testReadResponseWithoutConnection()
{
	Fixture fx;

	// Nothing was sent yet, so no connection can carry a reply.
	expectThrows<Poco::IOException>([&]() { fx.connection().readResponse(fx.response()); },
		"readResponse must fail without a connection"s);
	assertEqual(0, fx.server().commandCount("ping"s));
	assertEqual(1, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testReconnectAfterConnectionClosed()
{
	Fixture fx;
	Request ping(OpMsgMessage::CMD_PING, "admin"s, ""s);
	assertTrue(fx.send(ping).responseOk());

	fx.server().closeConnections();
	const int before = fx.server().totalConnections();

	// reconnect() selects a server again and opens a connection to it.
	fx.connection().reconnect();
	assertTrue(fx.connection().isConnected());

	// The second ping travels over the new connection, which the server has accepted by then.
	assertTrue(fx.send(ping).responseOk());
	assertEqual(2, fx.server().commandCount("ping"s));
	assertEqual(before + 1, fx.server().totalConnections());
}


void ReplicaSetConnectionTest::testKillWithForeignIdReleasesCursor()
{
	MongoDBTestServer::Behaviour foreign;
	foreign.reply = new Document();
	foreign.reply->addNewArray("cursorsKilled"s).add(CURSOR_ID + 1);
	foreign.reply->addNewArray("cursorsNotFound"s);
	foreign.reply->add("ok"s, 1.0);

	Fixture fx;
	scriptCursor(fx.server());
	fx.server().setDefaultBehaviour("killCursors"s, foreign);

	OpMsgCursor cursor("db"s, "c"s);
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);
	assertTrue(cursor.next(fx.connection()).responseOk());

	// A reply that confirms another cursor does not confirm this one.
	expectThrows<Poco::ProtocolException>([&]() { cursor.kill(fx.connection()); },
		"kill() must report a reply that names another cursor"s);
	assertEqual(Poco::Int64(0), cursor.cursorID());
	assertEqual(1, fx.server().commandCount("killCursors"s));
}


void ReplicaSetConnectionTest::testFindResentAfterNoReplyWithConfigTimeouts()
{
	// No explicit timeouts: the configured socket timeout must end the first attempt.
	Fixture fx(1, ReadPreference::Primary, Fixture::Setup(), true);
	scriptAction(fx.server(), "find"s, Action::NoReply);

	Request request(OpMsgMessage::CMD_FIND);
	assertTrue(fx.send(request).responseOk());
	assertEqual(2, fx.server().commandCount("find"s));
	assertEqual(2, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testSocketTimeoutFallsBackToConfig()
{
	Fixture fx;
	// The three-argument constructor leaves the socket timeout at 0, which must
	// fall back to the configured one instead of leaving the attempt unbounded.
	ReplicaSetConnection connection(fx.replicaSet(), ReadPreference(ReadPreference::Primary), CONNECT_TIMEOUT);
	scriptAction(fx.server(), "find"s, Action::NoReply);

	std::mutex mutex;
	std::condition_variable finished;
	bool done = false;
	bool watchdogFired = false;
	// Without the fallback the first attempt never returns; closing the connection
	// turns that into an exception, so the test fails instead of hanging.
	std::thread watchdog([&]()
	{
		std::unique_lock<std::mutex> lock(mutex);
		if (!finished.wait_for(lock, std::chrono::seconds(4), [&done]() { return done; }))
		{
			watchdogFired = true;
			fx.server().closeConnections();
		}
	});

	Request request(OpMsgMessage::CMD_FIND);
	OpMsgMessage response;
	std::exception_ptr failure;
	try
	{
		connection.sendRequest(request, response);
	}
	catch (...)
	{
		failure = std::current_exception();
	}

	{
		std::lock_guard<std::mutex> lock(mutex);
		done = true;
	}
	finished.notify_one();
	watchdog.join();

	if (failure)
		std::rethrow_exception(failure);
	// The configured socket timeout ended the first attempt, not the watchdog.
	assertFalse(watchdogFired);
	assertTrue(response.responseOk());
	assertEqual(2, fx.server().commandCount("find"s));
}


void ReplicaSetConnectionTest::testHandBuiltBodyClassifiedByFirstElement()
{
	Fixture fx;
	scriptAction(fx.server(), "find"s, Action::CloseConnection);
	scriptAction(fx.server(), "update"s, Action::CloseConnection);

	// Without setCommandName() the first body element names the command.
	OpMsgMessage find("db"s, "c"s);
	find.body().add("find"s, "c"s);
	assertTrue(fx.send(find).responseOk());
	assertEqual(2, fx.server().commandCount("find"s));

	OpMsgMessage update("db"s, "c"s);
	update.body().add("update"s, "c"s);
	expectThrows<Poco::IOException>([&]() { fx.send(update); },
		"a hand-built update must not be sent again"s);
	assertEqual(1, fx.server().commandCount("update"s));
}


CppUnit::Test* ReplicaSetConnectionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ReplicaSetConnectionTest"s);

	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testServerHelloAndPing);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testScriptedErrorReply);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testNoReplyTimesOut);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testCloseConnectionAndStopListening);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testPartialMessageDoesNotStall);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testFindServerErrorBounded);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testFindNoReplyBounded);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testFindSucceedsAfterServerErrors);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testFindResentAfterNoReply);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testInsertNotResentAfterNoReply);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testInsertNotResentAfterConnectionClosed);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testInsertResentAfterNotWritablePrimary);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testInsertNotResentAfterStateChange);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testWriteErrorsReplyReturned);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testWriteConcernErrorReturned);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testGetMoreNotResentAfterNoReply);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testGetMoreNotResentAfterNotPrimary);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testKillCursorsNotResent);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testKillAcceptsCursorNotFound);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testKillWithUnconfirmedReplyReleasesCursor);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testAggregateWithOutputStageNotResent);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testAggregateResentAfterConnectionLoss);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testConnectFailureRetriedOnSecondServer);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testStaleConnectionFindRecoversInsertFails);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testMalformedReplyNotResent);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testUnsupportedElementTypeNotResent);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testKillCursorsNotResentAfterNotPrimary);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testNoSuitableServer);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testFindResentAfterNotExecutedReply);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testInsertResentAfterLegacyNotPrimary);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testCountResentAfterNoReply);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testNoSuitableServerAfterErrorReply);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testNoSuitableServerAfterNoReply);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testUnlistedErrorReplyReturned);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testCodelessErrorReplyReturned);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testCursorRequestsCarryCursorId);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testAttemptBoundScalesWithServerCount);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testSafeReadsResentAndOtherCommandsNot);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testCursorFollowsResentFind);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testGetMoreCursorNotFoundReturned);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testOneWayRequestDelivered);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testReadResponseWithoutConnection);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testReconnectAfterConnectionClosed);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testKillWithForeignIdReleasesCursor);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testFindResentAfterNoReplyWithConfigTimeouts);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testSocketTimeoutFallsBackToConfig);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testHandBuiltBodyClassifiedByFirstElement);

	return pSuite;
}
