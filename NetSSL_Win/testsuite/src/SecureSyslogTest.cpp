//
// SecureSyslogTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SecureSyslogTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/SecureRemoteSyslogChannel.h"
#include "Poco/Net/SecureRemoteSyslogListener.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/LoggingFactory.h"
#include "Poco/Message.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/Stopwatch.h"
#include "Poco/Timespan.h"
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>


using namespace Poco::Net;


namespace
{
	class CollectingChannel: public Poco::Channel
		/// Keeps the messages logged to it in the order of their arrival
		/// and lets a test wait for them.
	{
	public:
		void log(const Poco::Message& msg) override
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_messages.push_back(msg);
			}
			_arrived.notify_all();
		}

		bool waitFor(std::size_t count, int milliseconds = 10000)
			/// Waits until count messages have arrived.
			/// Returns false if they have not within the given time.
		{
			std::unique_lock<std::mutex> lock(_mutex);
			return _arrived.wait_for(lock, std::chrono::milliseconds(milliseconds), [this, count] { return _messages.size() >= count; });
		}

		std::vector<Poco::Message> messages() const
		{
			std::lock_guard<std::mutex> lock(_mutex);
			return _messages;
		}

		std::vector<std::string> texts() const
			/// Returns the texts of the messages in the order of their arrival.
		{
			std::lock_guard<std::mutex> lock(_mutex);
			std::vector<std::string> result;
			for (const auto& msg: _messages) result.push_back(msg.getText());
			return result;
		}

	private:
		std::vector<Poco::Message> _messages;
		mutable std::mutex _mutex;
		std::condition_variable _arrived;
	};


	struct TLSListener
		/// A listener that takes messages from TLS connections to a port
		/// that the system chooses for it.
	{
		TLSListener():
			socket(SocketAddress("127.0.0.1", 0)),
			pListener(new RemoteSyslogListener(0)),
			pChannel(new CollectingChannel)
		{
			pListener->addServerSocket(socket);
			pListener->open();
			pListener->addChannel(pChannel);
		}

		~TLSListener()
		{
			pListener->close();
		}

		SocketAddress address() const
		{
			return socket.address();
		}

		SecureServerSocket socket;
		Poco::AutoPtr<RemoteSyslogListener> pListener;
		Poco::AutoPtr<CollectingChannel> pChannel;
	};


	const std::string MESSAGE_HEADER("<34>1 2026-01-02T03:04:05.000Z ahost anapp 77 asource - ");


	std::string message(const std::string& text)
		/// Returns a syslog message with the given text.
	{
		return MESSAGE_HEADER + text;
	}


	std::string counted(const std::string& text)
		/// Returns a syslog message with the given text,
		/// preceded by its length (octet counting).
	{
		const std::string msg = message(text);
		return std::to_string(msg.size()) + ' ' + msg;
	}


	std::string line(const std::string& text)
		/// Returns a syslog message with the given text,
		/// followed by a line feed.
	{
		return message(text) + '\n';
	}


	const Poco::Timespan CLIENT_TIMEOUT(10, 0);


	SecureStreamSocket connectTLS(const SocketAddress& address)
		/// Connects a TLS client. Its timeouts make a server that does
		/// not answer fail the test instead of holding it up.
	{
		StreamSocket socket;
		socket.connect(address, CLIENT_TIMEOUT);
		socket.setReceiveTimeout(CLIENT_TIMEOUT);
		socket.setSendTimeout(CLIENT_TIMEOUT);
		return SecureStreamSocket::attach(socket, address.host().toString());
	}


	void send(StreamSocket& socket, const std::string& data)
	{
		std::size_t sent = 0;
		while (sent < data.size())
		{
			int n = socket.sendBytes(data.data() + sent, static_cast<int>(data.size() - sent));
			if (n <= 0) throw Poco::IOException("connection closed while sending");
			sent += n;
		}
	}


	bool closedByPeer(StreamSocket& socket, int milliseconds = 10000)
		/// Returns true if the peer closes or resets the connection
		/// within the given time.
	{
		// No socket option is set for the waiting: a connection that
		// was reset may not take one any more.
		try
		{
			if (!socket.poll(Poco::Timespan(Poco::Timespan::TimeDiff(milliseconds)*1000), Poco::Net::Socket::SELECT_READ)) return false;
			char buffer[256];
			return socket.receiveBytes(buffer, sizeof(buffer)) == 0;
		}
		catch (const Poco::TimeoutException&)
		{
			return false;
		}
		catch (const Poco::IOException&)
		{
			return true;
		}
	}


	Poco::UInt16 freePort()
		/// Returns a port that was free a moment ago.
	{
		ServerSocket socket(SocketAddress("127.0.0.1", 0));
		return socket.address().port();
	}


	class TargetChannel: public SecureRemoteSyslogChannel
		/// A channel that connects to nothing and keeps where
		/// it was asked to connect to.
	{
	public:
		std::vector<std::string> targets;
			/// The address and the host name of every attempt.

	protected:
		StreamSocket createSocket(const SocketAddress& address, const std::string& hostName, const Poco::Timespan& timeout) override
		{
			targets.push_back(address.toString() + " " + hostName);
			throw Poco::Net::ConnectionRefusedException();
		}
	};
}


SecureSyslogTest::SecureSyslogTest(const std::string& name): CppUnit::TestCase(name)
{
}


SecureSyslogTest::~SecureSyslogTest()
{
}


void SecureSyslogTest::testRoundTrip()
{
	// Both set up by their properties, with the default contexts.
	const Poco::UInt16 port = freePort();
	Poco::AutoPtr<SecureRemoteSyslogListener> listener = new SecureRemoteSyslogListener;
	listener->setProperty("tlsPort", std::to_string(port));
	listener->open();
	auto pCL = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL);

	Poco::AutoPtr<SecureRemoteSyslogChannel> channel = new SecureRemoteSyslogChannel;
	channel->setProperty("loghost", "127.0.0.1:" + std::to_string(port));
	channel->setProperty("facility", "LOCAL3");
	channel->setProperty("host", "ahost");
	channel->setProperty("name", "anapp");
	Poco::Message msg1("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	msg1.set("structured-data", "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"]");
	channel->log(msg1);
	Poco::Message msg2("asource", "two\nlines", Poco::Message::PRIO_ERROR);
	channel->log(msg2);
	bool arrived = pCL->waitFor(2);
	channel->close();
	listener->close();
	assertTrue (arrived);

	std::vector<Poco::Message> msgs = pCL->messages();
	assertTrue (msgs.size() == 2);
	assertTrue (msgs[0].getSource() == "asource");
	assertTrue (msgs[0].getText() == "amessage");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[0].get("facility") == "LOCAL3");
	assertTrue (msgs[0].get("host") == "ahost");
	assertTrue (msgs[0].get("app") == "anapp");
	assertTrue (msgs[0].get("addr") == "127.0.0.1");
	assertTrue (msgs[0].get("structured-data") == "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"]");
	assertTrue (msgs[1].getSource() == "asource");
	assertTrue (msgs[1].getText() == "two\nlines");
	assertTrue (msgs[1].getPriority() == Poco::Message::PRIO_ERROR);
}


void SecureSyslogTest::testSuppliedSocket()
{
	// A SecureServerSocket handed to a RemoteSyslogListener.
	TLSListener listener;
	SecureStreamSocket client = connectTLS(listener.address());
	send(client, counted("one") + counted("two"));
	bool arrived = listener.pChannel->waitFor(2);
	listener.pListener->close();
	assertTrue (arrived);

	std::vector<Poco::Message> msgs = listener.pChannel->messages();
	assertTrue (msgs.size() == 2);
	assertTrue (msgs[0].getText() == "one");
	assertTrue (msgs[0].getSource() == "asource");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[0].get("facility") == "AUTH");
	assertTrue (msgs[0].get("host") == "ahost");
	assertTrue (msgs[0].get("app") == "anapp");
	assertTrue (msgs[0].get("addr") == "127.0.0.1");
	assertTrue (msgs[1].getText() == "two");
}


void SecureSyslogTest::testNewlineFraming()
{
	TLSListener listener;
	SecureStreamSocket client = connectTLS(listener.address());
	send(client, line("one") + message("two") + "\r\n" + counted("three") + line("four"));
	bool arrived = listener.pChannel->waitFor(4);

	// and from a channel that is told to send that way
	Poco::AutoPtr<SecureRemoteSyslogChannel> channel = new SecureRemoteSyslogChannel;
	channel->setProperty("loghost", listener.address().toString());
	channel->setProperty("framing", "newline");
	channel->setProperty("host", "ahost");
	Poco::Message msg("asource", "five", Poco::Message::PRIO_CRITICAL);
	channel->log(msg);
	bool arrivedFromChannel = listener.pChannel->waitFor(5);
	listener.pListener->close();
	channel->close();
	assertTrue (arrived);
	assertTrue (arrivedFromChannel);

	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts.size() == 5);
	assertTrue (texts[0] == "one");
	assertTrue (texts[1] == "two");
	assertTrue (texts[2] == "three");
	assertTrue (texts[3] == "four");
	assertTrue (texts[4] == "five");
}


void SecureSyslogTest::testLargeAndCoalesced()
{
	TLSListener listener;
	SecureStreamSocket client = connectTLS(listener.address());

	// a message of more than one TLS record
	std::string large(20*1024, '\0');
	for (std::size_t i = 0; i < large.size(); ++i) large[i] = static_cast<char>('a' + i % 26);
	send(client, counted(large));
	bool arrived = listener.pChannel->waitFor(1);

	// many messages in both framings, sent as one, of more
	// in all than is read from a connection in one go
	const int count = 2000;
	std::string data;
	for (int i = 0; i < count; ++i)
	{
		const std::string text = "message " + std::to_string(i);
		data += (i % 2) ? counted(text) : line(text);
	}
	send(client, data);
	bool arrivedBurst = listener.pChannel->waitFor(1 + count);

	send(client, line(large));
	bool arrivedLine = listener.pChannel->waitFor(2 + count);
	listener.pListener->close();
	assertTrue (arrived);
	assertTrue (arrivedBurst);
	assertTrue (arrivedLine);

	// one parser thread: in the order they were sent
	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts.size() == 2 + count);
	assertTrue (texts[0] == large);
	for (int i = 0; i < count; ++i)
	{
		assertTrue (texts[1 + i] == "message " + std::to_string(i));
	}
	assertTrue (texts[1 + count] == large);
}


void SecureSyslogTest::testPlainClient()
{
	TLSListener listener;
	// A client that does not speak TLS is dropped ...
	StreamSocket plain(listener.address());
	send(plain, line("in the clear"));
	bool dropped = closedByPeer(plain);

	// ... and the listener goes on serving.
	SecureStreamSocket client = connectTLS(listener.address());
	send(client, counted("over TLS"));
	bool arrived = listener.pChannel->waitFor(1);
	listener.pListener->close();
	assertTrue (dropped);
	assertTrue (arrived);

	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts.size() == 1);
	assertTrue (texts[0] == "over TLS");
}


void SecureSyslogTest::testStalledHandshake()
{
	TLSListener listener;
	// A client that begins a TLS record and does not go on ...
	StreamSocket stalled(listener.address());
	const char begin[] = {0x16, 0x03, 0x01};
	send(stalled, std::string(begin, sizeof(begin)));

	// ... does not keep the next one waiting.
	Poco::AutoPtr<SecureRemoteSyslogChannel> channel = new SecureRemoteSyslogChannel;
	channel->setProperty("loghost", listener.address().toString());
	channel->setProperty("timeout", "10000");
	channel->setProperty("host", "ahost");
	Poco::Message msg("asource", "not delayed", Poco::Message::PRIO_CRITICAL);
	channel->log(msg);
	bool arrived = listener.pChannel->waitFor(1);
	bool stillOpen = !stalled.poll(Poco::Timespan(), Socket::SELECT_READ);
	listener.pListener->close();
	channel->close();
	assertTrue (arrived);
	assertTrue (stillOpen);
	assertTrue (listener.pChannel->texts()[0] == "not delayed");
}


void SecureSyslogTest::testCloseWithClients()
{
	SocketAddress address;
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(0);
	{
		// the listener is the only one to hold the socket
		SecureServerSocket socket(SocketAddress("127.0.0.1", 0));
		address = socket.address();
		listener->addServerSocket(socket);
	}
	listener->open();
	auto pCL = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL);

	std::vector<SecureStreamSocket> clients;
	for (int i = 0; i < 3; ++i)
	{
		clients.push_back(connectTLS(address));
		send(clients.back(), counted("before"));
	}
	// and one that has not even begun the handshake
	StreamSocket idle(address);
	assertTrue (pCL->waitFor(3));

	listener->close();
	for (auto& client: clients)
	{
		assertTrue (closedByPeer(client));
	}
	assertTrue (closedByPeer(idle));

	// The port is free again: it can be bound by a socket that does not
	// share it, and the listener can be opened on it once more.
	SecureServerSocket socket;
	socket.bind(address, true, false);
	socket.listen();
	listener->addServerSocket(socket);
	listener->open();
	auto pCL2 = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL2);
	SecureStreamSocket client = connectTLS(address);
	send(client, counted("after"));
	bool arrived = pCL2->waitFor(1);
	listener->close();
	assertTrue (arrived);
	assertTrue (pCL2->texts()[0] == "after");
}


void SecureSyslogTest::testChannelReconnect()
{
	SocketAddress address;
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(0);
	{
		SecureServerSocket socket(SocketAddress("127.0.0.1", 0));
		address = socket.address();
		listener->addServerSocket(socket);
	}
	listener->open();
	auto pCL = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL);

	Poco::AutoPtr<SecureRemoteSyslogChannel> channel = new SecureRemoteSyslogChannel;
	channel->setProperty("loghost", address.toString());
	channel->setProperty("retryInterval", "10");
	channel->setProperty("host", "ahost");
	Poco::Message before("asource", "before", Poco::Message::PRIO_CRITICAL);
	channel->log(before);
	assertTrue (pCL->waitFor(1));

	// the server goes away and comes back on the same port
	listener->close();
	SecureServerSocket socket;
	socket.bind(address, true, false);
	socket.listen();
	listener->addServerSocket(socket);
	listener->open();
	auto pCL2 = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL2);

	// The channel finds that its connection is closed and makes a new one.
	// A message handed to the old connection before that was known is
	// lost, so the message is logged until it has arrived.
	Poco::Message after("asource", "after", Poco::Message::PRIO_CRITICAL);
	bool arrived = false;
	for (int i = 0; i < 100 && !arrived; ++i)
	{
		channel->log(after);
		arrived = pCL2->waitFor(1, 100);
	}
	listener->close();
	channel->close();
	assertTrue (arrived);
	assertTrue (pCL2->texts()[0] == "after");
}


void SecureSyslogTest::testChannelHandshakeTimeout()
{
	// A server that takes the connection and never answers.
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	Poco::AutoPtr<SecureRemoteSyslogChannel> channel = new SecureRemoteSyslogChannel;
	channel->setProperty("loghost", server.address().toString());
	channel->setProperty("timeout", "300");
	channel->setProperty("retryInterval", "3600000");
	// with its name given, the channel does not look up the local host
	// when it is opened, which may take longer than any handshake
	channel->setProperty("host", "ahost");
	Poco::Message msg("asource", "dropped", Poco::Message::PRIO_CRITICAL);

	// neither held up for long nor made to fail
	Poco::Stopwatch sw;
	sw.start();
	channel->log(msg);
	sw.stop();
	assertTrue (sw.elapsed() < 5*Poco::Stopwatch::resolution());

	// the connection is given up, and no other is tried for now
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	StreamSocket connection = server.acceptConnection();
	sw.restart();
	channel->log(msg);
	sw.stop();
	assertTrue (sw.elapsed() < 5*Poco::Stopwatch::resolution());
	assertTrue (!server.poll(Poco::Timespan(), Socket::SELECT_READ));

	// Nothing of a message was sent: what arrived is of the handshake,
	// and the connection is closed.
	std::string received;
	bool closed = false;
	char buffer[4096];
	try
	{
		while (!closed && connection.poll(Poco::Timespan(10, 0), Socket::SELECT_READ))
		{
			int n = connection.receiveBytes(buffer, sizeof(buffer));
			if (n > 0)
				received.append(buffer, n);
			else
				closed = true;
		}
	}
	catch (const Poco::IOException&)
	{
		closed = true;
	}
	assertTrue (closed);
	assertTrue (received.find("dropped") == std::string::npos);
	channel->close();
}


void SecureSyslogTest::testDefaultPort()
{
	Poco::AutoPtr<TargetChannel> channel = new TargetChannel;
	channel->setProperty("retryInterval", "0");
	Poco::Message msg("asource", "amessage", Poco::Message::PRIO_CRITICAL);

	// what the "loghost" property may say, and where that is
	const char* targets[][2] =
	{
		{"127.0.0.1", "127.0.0.1:6514 127.0.0.1"},
#if defined(POCO_HAVE_IPv6)
		{"[::1]", "[::1]:6514 ::1"},
		{"[::1]:1514", "[::1]:1514 ::1"},
		{"::1", "[::1]:6514 ::1"},
#endif
		{"127.0.0.1:1514", "127.0.0.1:1514 127.0.0.1"}
	};
	for (const auto& target: targets)
	{
		channel->setProperty("loghost", target[0]);
		channel->log(msg);
		assertTrue (!channel->targets.empty());
		assertEqual (std::string(target[1]), channel->targets.back());
	}
	channel->close();
}


void SecureSyslogTest::testProperties()
{
	Poco::AutoPtr<SecureRemoteSyslogChannel> channel = new SecureRemoteSyslogChannel;
	assertTrue (channel->getProperty("transport") == "tls");
	assertTrue (channel->getProperty("framing") == "octet-counting");
	assertTrue (channel->getProperty("timeout") == "2000");
	channel->setProperty("transport", "TLS");
	channel->setProperty("framing", "newline");
	assertTrue (channel->getProperty("transport") == "tls");
	assertTrue (channel->getProperty("framing") == "newline");
	const char* transports[] = {"tcp", "udp", ""};
	for (const char* transport: transports)
	{
		try
		{
			channel->setProperty("transport", transport);
			fail("a secure channel sends over TLS only - must throw");
		}
		catch (Poco::InvalidArgumentException&)
		{
		}
	}

	Poco::AutoPtr<SecureRemoteSyslogListener> listener = new SecureRemoteSyslogListener;
	assertTrue (listener->getProperty("tlsPort") == "6514");
	assertTrue (listener->getProperty("port") == "0");
	assertTrue (listener->getProperty("tcpPort") == "0");
	listener->setProperty("tlsPort", "16514");
	listener->setProperty("threads", "2");
	assertTrue (listener->getProperty("tlsPort") == "16514");
	assertTrue (listener->getProperty("threads") == "2");
	try
	{
		listener->setProperty("tlsPort", "65536");
		fail("not a port number - must throw");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}

	// both can be created by the logging configuration
	SecureRemoteSyslogChannel::registerChannel();
	SecureRemoteSyslogListener::registerChannel();
	Poco::Channel::Ptr pChannel = Poco::LoggingFactory::defaultFactory().createChannel("SecureRemoteSyslogChannel");
	assertTrue (!pChannel.cast<SecureRemoteSyslogChannel>().isNull());
	assertTrue (pChannel->getProperty("transport") == "tls");
	Poco::Channel::Ptr pListener = Poco::LoggingFactory::defaultFactory().createChannel("SecureRemoteSyslogListener");
	assertTrue (!pListener.cast<SecureRemoteSyslogListener>().isNull());
	assertTrue (pListener->getProperty("tlsPort") == "6514");
}


void SecureSyslogTest::setUp()
{
}


void SecureSyslogTest::tearDown()
{
}


CppUnit::Test* SecureSyslogTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SecureSyslogTest");

	CppUnit_addTest(pSuite, SecureSyslogTest, testRoundTrip);
	CppUnit_addTest(pSuite, SecureSyslogTest, testSuppliedSocket);
	CppUnit_addTest(pSuite, SecureSyslogTest, testNewlineFraming);
	CppUnit_addTest(pSuite, SecureSyslogTest, testLargeAndCoalesced);
	CppUnit_addTest(pSuite, SecureSyslogTest, testPlainClient);
	CppUnit_addTest(pSuite, SecureSyslogTest, testStalledHandshake);
	CppUnit_addTest(pSuite, SecureSyslogTest, testCloseWithClients);
	CppUnit_addTest(pSuite, SecureSyslogTest, testChannelReconnect);
	CppUnit_addTest(pSuite, SecureSyslogTest, testChannelHandshakeTimeout);
	CppUnit_addTest(pSuite, SecureSyslogTest, testDefaultPort);
	CppUnit_addTest(pSuite, SecureSyslogTest, testProperties);

	return pSuite;
}
