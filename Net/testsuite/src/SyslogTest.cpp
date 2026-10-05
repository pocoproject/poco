//
// SyslogTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SyslogTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/RemoteSyslogChannel.h"
#include "Poco/Net/RemoteSyslogListener.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/DNS.h"
#include "Poco/Thread.h"
#include "Poco/Message.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/Timespan.h"
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <list>
#include <mutex>
#include <set>
#include <string>
#include <vector>


using namespace Poco::Net;


class CachingChannel: public Poco::Channel
	/// Caches the last n Messages in memory
{
public:
	typedef std::list<Poco::Message> Messages;

	CachingChannel(std::size_t n = 100);
		/// Creates the CachingChannel. Caches n messages in memory

	~CachingChannel();
		/// Destroys the CachingChannel.

	void log(const Poco::Message& msg);
		/// Writes the log message to the cache

	void getMessages(std::vector<Poco::Message>& msg, int offset, int numEntries) const;
		/// Retrieves numEntries Messages starting with position offset. Most recent messages are first.

	std::size_t getMaxSize() const;

	std::size_t getCurrentSize() const;

private:
	CachingChannel(const CachingChannel&);

	Messages   _cache;
	std::size_t _size;
	std::size_t _maxSize;
	mutable Poco::FastMutex _mutex;
};


std::size_t CachingChannel::getMaxSize() const
{
	return _maxSize;
}


std::size_t CachingChannel::getCurrentSize() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _size;
}


CachingChannel::CachingChannel(std::size_t n):
	_cache(),
	_size(0),
	_maxSize(n),
	_mutex()
{
}


CachingChannel::~CachingChannel()
{
}


void CachingChannel::log(const Poco::Message& msg)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_cache.push_front(msg);
	if (_size == _maxSize)
	{
		_cache.pop_back();
	}
	else
		++_size;
}


void CachingChannel::getMessages(std::vector<Poco::Message>& msg, int offset, int numEntries) const
{
	msg.clear();
	Messages::const_iterator it = _cache.begin();

	while (offset > 0 && it != _cache.end())
		++it;

	while (numEntries > 0 && it != _cache.end())
	{
		msg.push_back(*it);
		++it;
	}
}


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

		bool waitForText(const std::string& text, std::size_t count, int milliseconds = 10000)
			/// Waits until count messages with the given text have arrived.
		{
			std::unique_lock<std::mutex> lock(_mutex);
			return _arrived.wait_for(lock, std::chrono::milliseconds(milliseconds), [this, &text, count]
			{
				return static_cast<std::size_t>(std::count_if(_messages.begin(), _messages.end(),
					[&text](const Poco::Message& msg) { return msg.getText() == text; })) >= count;
			});
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


	struct TCPListener
		/// A listener that takes messages from connections to a port
		/// that the system chooses for it.
	{
		explicit TCPListener(int maxMessageSize = 0):
			socket(SocketAddress("127.0.0.1", 0)),
			pListener(new RemoteSyslogListener(0)),
			pChannel(new CollectingChannel)
		{
			if (maxMessageSize > 0) pListener->setProperty("maxMessageSize", std::to_string(maxMessageSize));
			pListener->addServerSocket(socket);
			pListener->open();
			pListener->addChannel(pChannel);
		}

		~TCPListener()
		{
			pListener->close();
		}

		SocketAddress address() const
		{
			return socket.address();
		}

		ServerSocket socket;
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
		socket.setReceiveTimeout(Poco::Timespan(Poco::Timespan::TimeDiff(milliseconds)*1000));
		char buffer[256];
		try
		{
			return socket.receiveBytes(buffer, sizeof(buffer)) == 0;
		}
		catch (const Poco::TimeoutException&)
		{
			return false;
		}
		catch (const NetException&)
		{
			return true;
		}
	}
}


SyslogTest::SyslogTest(const std::string& name): CppUnit::TestCase(name)
{
}


SyslogTest::~SyslogTest()
{
}


void SyslogTest::testListener()
{
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", "127.0.0.1:51400");
	channel->open();
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(51400);
	listener->open();
	auto pCL = Poco::makeAuto<CachingChannel>();
	listener->addChannel(pCL);
	assertTrue (pCL->getCurrentSize() == 0);
	Poco::Message msg("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	channel->log(msg);
	Poco::Thread::sleep(1000);
	listener->close();
	channel->close();
	assertTrue (pCL->getCurrentSize() == 1);
	std::vector<Poco::Message> msgs;
	pCL->getMessages(msgs, 0, 10);
	assertTrue (msgs.size() == 1);
	assertTrue (msgs[0].getSource() == "asource");
	assertTrue (msgs[0].getText() == "amessage");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
}

void SyslogTest::testChannelFacility()
{
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", "127.0.0.1:51400");
	channel->setProperty("facility", "KERN");
	channel->open();
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(51400);
	listener->open();
	auto pCL = Poco::makeAuto<CachingChannel>();
	listener->addChannel(pCL);
	assertTrue (pCL->getCurrentSize() == 0);
	Poco::Message msg("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	channel->log(msg);
	channel->setProperty("facility", "USER");
	msg.setText("asecondmessage");
	channel->log(msg);
	assertFalse (msg.has("facility"));
	Poco::Thread::sleep(1000);
	listener->close();
	channel->close();
	assertTrue (pCL->getCurrentSize() == 2);
	std::vector<Poco::Message> msgs;
	pCL->getMessages(msgs, 0, 10);

	assertTrue (msgs.size() == 2);

	assertTrue (msgs[1].getSource() == "asource");
	assertTrue (msgs[1].getText() == "amessage");
	assertTrue (msgs[1].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[1].has("facility"));
	assertTrue (msgs[1].get("facility") == "KERN");

	assertTrue (msgs[0].getSource() == "asource");
	assertTrue (msgs[0].getText() == "asecondmessage");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[0].has("facility"));
	assertTrue (msgs[0].get("facility") == "USER");
}

void SyslogTest::testChannelOpenClose()
{
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", "127.0.0.1:51400");
	channel->open();
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(51400);
	listener->open();
	auto pCL = Poco::makeAuto<CachingChannel>();
	listener->addChannel(pCL);

	assertTrue (pCL->getCurrentSize() == 0);
	Poco::Message msg1("source1", "message1", Poco::Message::PRIO_CRITICAL);
	channel->log(msg1);
	Poco::Thread::sleep(1000);
	assertTrue (pCL->getCurrentSize() == 1);

	channel->close(); // close and re-open channel
	channel->open();

	Poco::Message msg2("source2", "message2", Poco::Message::PRIO_ERROR);
	channel->log(msg2);
	Poco::Thread::sleep(1000);
	assertTrue (pCL->getCurrentSize() == 2);

	listener->close();
	std::vector<Poco::Message> msgs;
	pCL->getMessages(msgs, 0, 10);
	assertTrue (msgs.size() == 2);

	assertTrue (msgs[1].getSource() == "source1");
	assertTrue (msgs[1].getText() == "message1");
	assertTrue (msgs[1].getPriority() == Poco::Message::PRIO_CRITICAL);

	assertTrue (msgs[0].getSource() == "source2");
	assertTrue (msgs[0].getText() == "message2");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_ERROR);
}


void SyslogTest::testOldBSD()
{
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", "127.0.0.1:51400");
	channel->setProperty("format", "bsd");
	channel->open();
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(51400);
	listener->open();
	auto pCL = Poco::makeAuto<CachingChannel>();
	listener->addChannel(pCL);
	assertTrue (pCL->getCurrentSize() == 0);
	Poco::Message msg("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	channel->log(msg);
	Poco::Thread::sleep(1000);
	listener->close();
	channel->close();
	assertTrue (pCL->getCurrentSize() == 1);
	std::vector<Poco::Message> msgs;
	pCL->getMessages(msgs, 0, 10);
	assertTrue (msgs.size() == 1);
	// the source is lost with old BSD messages: we only send the local host name!
	assertTrue (msgs[0].getSource() == Poco::Net::DNS::thisHost().name());
	assertTrue (msgs[0].getText() == "amessage");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
}


void SyslogTest::testStructuredData()
{
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", "127.0.0.1:51400");
	channel->open();
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(51400);
	listener->open();
	auto pCL = Poco::makeAuto<CachingChannel>();
	listener->addChannel(pCL);
	assertTrue (pCL->getCurrentSize() == 0);
	Poco::Message msg1("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	msg1.set("structured-data", "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"]");
	channel->log(msg1);
	Poco::Message msg2("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	msg2.set("structured-data", "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"][examplePriority@32473 class=\"high\"]");
	channel->log(msg2);
	Poco::Thread::sleep(1000);
	listener->close();
	channel->close();
	assertTrue (pCL->getCurrentSize() == 2);
	std::vector<Poco::Message> msgs;
	pCL->getMessages(msgs, 0, 10);
	assertTrue (msgs.size() == 2);

	assertTrue (msgs[0].getSource() == "asource");
	assertTrue (msgs[0].getText() == "amessage");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[0].get("structured-data") == "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"][examplePriority@32473 class=\"high\"]");

	assertTrue (msgs[1].getSource() == "asource");
	assertTrue (msgs[1].getText() == "amessage");
	assertTrue (msgs[1].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[1].get("structured-data") == "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"]");
}


void SyslogTest::testBSDWithoutTimestamp()
{
	// A BSD message may come without a timestamp, the host name first.
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(0);
	listener->open();
	auto pCL = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL);
	listener->enqueueMessage("<34>myhost app: text", SocketAddress("127.0.0.1", 514));
	bool arrived = pCL->waitFor(1);
	listener->close();
	assertTrue (arrived);
	std::vector<Poco::Message> msgs = pCL->messages();
	assertTrue (msgs.size() == 1);
	assertTrue (msgs[0].getSource() == "myhost");
	assertTrue (msgs[0].getText() == "app: text");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[0].get("facility") == "AUTH");
	assertTrue (msgs[0].get("addr") == "127.0.0.1");
}


void SyslogTest::testTCPOctetCounting()
{
	TCPListener listener;
	StreamSocket client(listener.address());
	send(client, counted("one") + counted("two"));
	assertTrue (listener.pChannel->waitFor(2));

	std::vector<Poco::Message> msgs = listener.pChannel->messages();
	assertTrue (msgs.size() == 2);
	assertTrue (msgs[0].getText() == "one");
	assertTrue (msgs[0].getSource() == "asource");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[0].getPid() == 77);
	assertTrue (msgs[0].get("facility") == "AUTH");
	assertTrue (msgs[0].get("host") == "ahost");
	assertTrue (msgs[0].get("app") == "anapp");
	assertTrue (msgs[0].get("addr") == "127.0.0.1");
	assertTrue (msgs[1].getText() == "two");

	// a line end after a counted message is no part of the next one
	send(client, counted("three") + "\n" + counted("four") + "\r\n" + counted("five"));
	assertTrue (listener.pChannel->waitFor(5));
	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts.size() == 5);
	assertTrue (texts[2] == "three");
	assertTrue (texts[3] == "four");
	assertTrue (texts[4] == "five");
}


void SyslogTest::testTCPNewline()
{
	TCPListener listener;
	StreamSocket client(listener.address());
	send(client, line("one") + message("two") + "\r\n" + line("three"));
	assertTrue (listener.pChannel->waitFor(3));

	std::vector<Poco::Message> msgs = listener.pChannel->messages();
	assertTrue (msgs.size() == 3);
	assertTrue (msgs[0].getText() == "one");
	assertTrue (msgs[0].getSource() == "asource");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[0].get("addr") == "127.0.0.1");
	assertTrue (msgs[1].getText() == "two");
	assertTrue (msgs[2].getText() == "three");
}


void SyslogTest::testTCPMixedFraming()
{
	TCPListener listener;
	StreamSocket client(listener.address());
	// A counted message may hold line feeds, and a line may hold digits.
	send(client, line("one") + counted("two\nlines") + line("3 three") + counted("four") + counted("five") + line("six"));
	assertTrue (listener.pChannel->waitFor(6));

	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts.size() == 6);
	assertTrue (texts[0] == "one");
	assertTrue (texts[1] == "two\nlines");
	assertTrue (texts[2] == "3 three");
	assertTrue (texts[3] == "four");
	assertTrue (texts[4] == "five");
	assertTrue (texts[5] == "six");
}


void SyslogTest::testTCPSplitFrames()
{
	TCPListener listener;
	StreamSocket client(listener.address());
	client.setNoDelay(true);
	StreamSocket other(listener.address());
	std::size_t marks = 0;

	// Sends a piece of a message and then, on another connection, a whole
	// message, and waits for that one: the listener has then been through
	// its connections and has seen the piece without what follows it.
	auto piece = [&](const std::string& data)
	{
		send(client, data);
		send(other, line("mark"));
		assertTrue (listener.pChannel->waitForText("mark", ++marks));
	};

	const std::string first = counted("counted");
	const std::size_t body = first.find(' ') + 1;
	assertTrue (body > 2);
	piece(first.substr(0, 1));          // within the length
	piece(first.substr(1, body - 2));   // the rest of the length, without the space that ends it
	piece(first.substr(body - 1, 11));  // the space and the beginning of the message
	piece(first.substr(body + 10));
	assertTrue (listener.pChannel->waitForText("counted", 1));

	const std::string second = line("terminated");
	piece(second.substr(0, 1));
	piece(second.substr(1, 20));
	piece(second.substr(21, second.size() - 22));   // up to the line feed
	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (std::find(texts.begin(), texts.end(), "terminated") == texts.end());
	piece(second.substr(second.size() - 1));
	assertTrue (listener.pChannel->waitForText("terminated", 1));

	texts = listener.pChannel->texts();
	assertTrue (texts.size() == marks + 2);
}


void SyslogTest::testTCPCoalescedFrames()
{
	TCPListener listener;
	StreamSocket client(listener.address());
	// both framings by turns, sent as one, and the connection closed at once
	const int count = 1000;
	std::string data;
	for (int i = 0; i < count; ++i)
	{
		const std::string text = "message " + std::to_string(i);
		data += (i % 2) ? counted(text) : line(text);
	}
	send(client, data);
	client.close();
	assertTrue (listener.pChannel->waitFor(count));

	// one parser thread: in the order they were sent
	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts.size() == count);
	for (int i = 0; i < count; ++i)
	{
		assertTrue (texts[i] == "message " + std::to_string(i));
	}
}


void SyslogTest::testTCPOversize()
{
	const int maxMessageSize = 100;
	TCPListener listener(maxMessageSize);
	assertTrue (listener.pListener->getProperty("maxMessageSize") == "100");
	StreamSocket client(listener.address());
	// A message that is too long is cut off, in either framing, and what
	// follows it is taken as it is.
	const std::string big(300, 'x');
	const std::string cut(maxMessageSize - MESSAGE_HEADER.size(), 'x');
	send(client, counted(big) + counted("after the counted one") + line(big) + line("after the line"));
	assertTrue (listener.pChannel->waitFor(4));

	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts.size() == 4);
	assertTrue (texts[0] == cut);
	assertTrue (texts[1] == "after the counted one");
	assertTrue (texts[2] == cut);
	assertTrue (texts[3] == "after the line");

	// a line that is too long before its line feed has arrived
	send(client, message(big));
	assertTrue (listener.pChannel->waitFor(5));
	send(client, big + "\n" + line("after the long line"));
	assertTrue (listener.pChannel->waitFor(6));
	texts = listener.pChannel->texts();
	assertTrue (texts.size() == 6);
	assertTrue (texts[4] == cut);
	assertTrue (texts[5] == "after the long line");

	// a message of exactly the size is whole
	const std::string fits(maxMessageSize - MESSAGE_HEADER.size(), 'y');
	send(client, counted(fits) + line(fits) + line("last"));
	assertTrue (listener.pChannel->waitFor(9));
	texts = listener.pChannel->texts();
	assertTrue (texts.size() == 9);
	assertTrue (texts[6] == fits);
	assertTrue (texts[7] == fits);
	assertTrue (texts[8] == "last");
}


void SyslogTest::testTCPGarbage()
{
	TCPListener listener;
	StreamSocket client(listener.address());
	// What is no message is skipped up to the next line feed
	// and the connection stays.
	send(client, "this is no syslog message\n" + line("one"));
	assertTrue (listener.pChannel->waitFor(1));
	send(client, "12is no length\n" + counted("two"));
	assertTrue (listener.pChannel->waitFor(2));
	send(client, "0 " + message("counted as nothing") + "\n" + line("three"));
	assertTrue (listener.pChannel->waitFor(3));
	send(client, "123456789012 " + message("a length of too many digits") + "\n" + counted("four"));
	assertTrue (listener.pChannel->waitFor(4));
	send(client, std::string("\0\xff\n\n\r\n", 6) + line("five"));
	assertTrue (listener.pChannel->waitFor(5));

	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts.size() == 5);
	assertTrue (texts[0] == "one");
	assertTrue (texts[1] == "two");
	assertTrue (texts[2] == "three");
	assertTrue (texts[3] == "four");
	assertTrue (texts[4] == "five");
}


void SyslogTest::testTCPUnterminated()
{
	TCPListener listener;
	{
		// a last message without a line feed ends with the connection
		StreamSocket client(listener.address());
		send(client, line("first") + message("last"));
		client.close();
	}
	assertTrue (listener.pChannel->waitFor(2));
	{
		// a counted message that has not arrived in full is not a message
		StreamSocket client(listener.address());
		const std::string msg = message("cut short");
		send(client, std::to_string(msg.size() + 10) + ' ' + msg);
		client.close();
	}
	{
		// By the time the second of these has arrived, the listener has
		// been through its connections after that close.
		StreamSocket client(listener.address());
		send(client, line("after"));
		assertTrue (listener.pChannel->waitFor(3));
		send(client, line("again"));
		assertTrue (listener.pChannel->waitFor(4));
	}

	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts.size() == 4);
	assertTrue (texts[0] == "first");
	assertTrue (texts[1] == "last");
	assertTrue (texts[2] == "after");
	assertTrue (texts[3] == "again");
}


void SyslogTest::testTCPManyConnections()
{
	TCPListener listener;
	const int count = 64;
	std::vector<StreamSocket> clients;
	for (int i = 0; i < count; ++i)
	{
		clients.push_back(StreamSocket(listener.address()));
	}
	for (int i = 0; i < count; ++i)
	{
		const std::string text = "from " + std::to_string(i);
		send(clients[i], (i % 2) ? counted(text) : line(text));
	}
	assertTrue (listener.pChannel->waitFor(count));
	// and again, over the same connections
	for (int i = 0; i < count; ++i)
	{
		send(clients[i], line("again from " + std::to_string(i)));
	}
	assertTrue (listener.pChannel->waitFor(2*count));

	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts.size() == 2*count);
	std::set<std::string> distinct(texts.begin(), texts.end());
	assertTrue (distinct.size() == 2*count);
	for (int i = 0; i < count; ++i)
	{
		assertTrue (distinct.count("from " + std::to_string(i)) == 1);
		assertTrue (distinct.count("again from " + std::to_string(i)) == 1);
	}
}


void SyslogTest::testTCPOpenTwice()
{
	// opening an open listener and closing a closed one change nothing
	TCPListener listener;
	listener.pListener->open();
	StreamSocket client(listener.address());
	send(client, line("one"));
	assertTrue (listener.pChannel->waitFor(1));
	listener.pListener->close();
	listener.pListener->close();
	assertTrue (closedByPeer(client));
	assertTrue (listener.pChannel->texts().size() == 1);
}


void SyslogTest::testTCPTwoSockets()
{
	ServerSocket first(SocketAddress("127.0.0.1", 0));
	ServerSocket second(SocketAddress("127.0.0.1", 0));
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(0);
	listener->addServerSocket(first);
	listener->addServerSocket(second);
	listener->open();
	auto pCL = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL);

	StreamSocket toFirst(first.address());
	StreamSocket toSecond(second.address());
	send(toFirst, line("to the first"));
	send(toSecond, counted("to the second"));
	bool arrived = pCL->waitFor(2);
	listener->close();
	assertTrue (arrived);
	std::vector<std::string> texts = pCL->texts();
	assertTrue (std::find(texts.begin(), texts.end(), "to the first") != texts.end());
	assertTrue (std::find(texts.begin(), texts.end(), "to the second") != texts.end());
}


void SyslogTest::testTCPCloseWithClients()
{
	SocketAddress address;
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(0);
	{
		// the listener is the only one to hold the socket
		ServerSocket socket(SocketAddress("127.0.0.1", 0));
		address = socket.address();
		listener->addServerSocket(socket);
	}
	listener->open();
	auto pCL = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL);

	std::vector<StreamSocket> clients;
	for (int i = 0; i < 3; ++i)
	{
		clients.push_back(StreamSocket(address));
		send(clients.back(), line("before"));
	}
	// one of them in the middle of a message
	send(clients[0], message("not finished"));
	assertTrue (pCL->waitFor(3));

	listener->close();
	for (auto& client: clients)
	{
		assertTrue (closedByPeer(client));
	}
	assertTrue (pCL->messages().size() == 3);

	// The port is free again: it can be bound by a socket that does not
	// share it, and the listener can be opened on it once more.
	ServerSocket socket;
	socket.bind(address, true, false);
	socket.listen();
	listener->addServerSocket(socket);
	listener->open();
	auto pCL2 = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL2);
	StreamSocket client(address);
	send(client, line("after"));
	bool arrived = pCL2->waitFor(1);
	listener->close();
	assertTrue (arrived);
	assertTrue (pCL2->texts()[0] == "after");
}


void SyslogTest::testTCPPort()
{
	Poco::UInt16 port = 0;
	{
		// a port that was free a moment ago
		ServerSocket socket(SocketAddress("127.0.0.1", 0));
		port = socket.address().port();
	}
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(0);
	assertTrue (listener->getProperty("tcpPort") == "0");
	assertTrue (listener->getProperty("maxMessageSize") == "65536");
	listener->setProperty("tcpPort", std::to_string(port));
	assertTrue (listener->getProperty("tcpPort") == std::to_string(port));
	listener->open();
	auto pCL = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL);

	StreamSocket client(SocketAddress("127.0.0.1", port));
	send(client, line("to the port"));
	bool arrived = pCL->waitFor(1);
	listener->close();
	assertTrue (arrived);
	assertTrue (pCL->texts()[0] == "to the port");

	try
	{
		listener->setProperty("tcpPort", "65536");
		fail("not a port number - must throw");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}
	try
	{
		listener->setProperty("maxMessageSize", "0");
		fail("not a message size - must throw");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}
}


void SyslogTest::setUp()
{
}


void SyslogTest::tearDown()
{
}


CppUnit::Test* SyslogTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SyslogTest");

	CppUnit_addTest(pSuite, SyslogTest, testListener);
	CppUnit_addTest(pSuite, SyslogTest, testChannelFacility);
	CppUnit_addTest(pSuite, SyslogTest, testChannelOpenClose);
	CppUnit_addTest(pSuite, SyslogTest, testOldBSD);
	CppUnit_addTest(pSuite, SyslogTest, testStructuredData);
	CppUnit_addTest(pSuite, SyslogTest, testBSDWithoutTimestamp);
	CppUnit_addTest(pSuite, SyslogTest, testTCPOctetCounting);
	CppUnit_addTest(pSuite, SyslogTest, testTCPNewline);
	CppUnit_addTest(pSuite, SyslogTest, testTCPMixedFraming);
	CppUnit_addTest(pSuite, SyslogTest, testTCPSplitFrames);
	CppUnit_addTest(pSuite, SyslogTest, testTCPCoalescedFrames);
	CppUnit_addTest(pSuite, SyslogTest, testTCPOversize);
	CppUnit_addTest(pSuite, SyslogTest, testTCPGarbage);
	CppUnit_addTest(pSuite, SyslogTest, testTCPUnterminated);
	CppUnit_addTest(pSuite, SyslogTest, testTCPManyConnections);
	CppUnit_addTest(pSuite, SyslogTest, testTCPOpenTwice);
	CppUnit_addTest(pSuite, SyslogTest, testTCPTwoSockets);
	CppUnit_addTest(pSuite, SyslogTest, testTCPCloseWithClients);
	CppUnit_addTest(pSuite, SyslogTest, testTCPPort);

	return pSuite;
}
