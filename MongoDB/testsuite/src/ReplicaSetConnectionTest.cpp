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
// and SOCKET_TIMEOUT bound every test that waits for a reply that never comes.
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
#include <functional>
#include <memory>
#include <sstream>
#include <string>
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
		/// reports all of them in its hello reply.
	{
	public:
		using Setup = std::function<void(const Servers&)>;

		explicit Fixture(std::size_t serverCount = 1,
			ReadPreference::Mode mode = ReadPreference::Primary,
			const Setup& setup = Setup()):
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
			_connection = std::make_unique<ReplicaSetConnection>(*_replicaSet,
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
	assertEqual(MAX_ATTEMPTS, fx.server().commandCount("find"s));
	assertEqual(MAX_ATTEMPTS, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testFindResentAfterTimeout()
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


void ReplicaSetConnectionTest::testInsertNotResentAfterTimeout()
{
	Fixture fx;
	scriptAction(fx.server(), "insert"s, Action::NoReply);

	InsertRequest request;
	expectThrows<Poco::IOException>([&]() { fx.send(request); },
		"an insert must not be sent again after a timeout"s);

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


void ReplicaSetConnectionTest::testGetMoreNotResentAfterTimeout()
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
		"getMore must not be sent again after a timeout"s);

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


void ReplicaSetConnectionTest::testCountResentAfterTimeout()
{
	Fixture fx;
	scriptAction(fx.server(), "count"s, Action::NoReply);

	Request request(OpMsgMessage::CMD_COUNT);
	assertTrue(fx.send(request).responseOk());
	assertEqual(2, fx.server().commandCount("count"s));
	assertEqual(2, fx.server().commandCount("hello"s));
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
	}

	assertEqual(10107, fx.response().body().get<Poco::Int32>("code"s, 0));
	assertEqual(1, fx.server().commandCount("find"s));
	assertEqual(2, fx.server().commandCount("hello"s));
}


void ReplicaSetConnectionTest::testNoSuitableServerAfterTimeout()
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
		assertTrue(e.nested()->displayText().find("Failed to read from socket"s) != std::string::npos);
	}

	assertEqual(1, fx.server().commandCount("find"s));
	assertEqual(2, fx.server().commandCount("hello"s));
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
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testFindResentAfterTimeout);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testInsertNotResentAfterTimeout);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testInsertNotResentAfterConnectionClosed);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testInsertResentAfterNotWritablePrimary);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testInsertNotResentAfterStateChange);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testWriteErrorsReplyReturned);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testWriteConcernErrorReturned);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testGetMoreNotResentAfterTimeout);
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
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testCountResentAfterTimeout);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testNoSuitableServerAfterErrorReply);
	CppUnit_addTest(pSuite, ReplicaSetConnectionTest, testNoSuitableServerAfterTimeout);

	return pSuite;
}
