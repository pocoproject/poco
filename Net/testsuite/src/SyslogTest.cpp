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
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/StreamSocketImpl.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/DNS.h"
#include "Poco/Message.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Event.h"
#include "Poco/Stopwatch.h"
#include "Poco/Timespan.h"
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <utility>
#include <initializer_list>
#include <functional>
#include <thread>
#include <atomic>
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

		explicit TCPListener(std::initializer_list<std::pair<const char*, const char*>> properties):
			socket(SocketAddress("127.0.0.1", 0)),
			pListener(new RemoteSyslogListener(0)),
			pChannel(new CollectingChannel)
		{
			for (const auto& property: properties) pListener->setProperty(property.first, property.second);
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


	struct UDPListener
		/// A listener that takes messages from datagrams sent to a port
		/// that was free when the listener was opened.
	{
		UDPListener():
			port(0),
			pChannel(new CollectingChannel)
		{
			// The listener binds its port itself and cannot be asked which
			// one it has. So a free port is looked for and then tried:
			// somebody else may have taken it in between.
			for (int i = 0; i < 100 && !pListener; ++i)
			{
				{
					DatagramSocket probe(SocketAddress("127.0.0.1", 0), false);
					port = probe.address().port();
				}
				Poco::AutoPtr<RemoteSyslogListener> pCandidate = new RemoteSyslogListener(port);
				try
				{
					pCandidate->open();
					pListener = pCandidate;
				}
				catch (Poco::Exception&)
				{
				}
			}
			if (!pListener) throw Poco::IOException("no UDP port to listen on");
			pListener->addChannel(pChannel);
		}

		~UDPListener()
		{
			pListener->close();
		}

		std::string address() const
		{
			return "127.0.0.1:" + std::to_string(port);
		}

		Poco::UInt16 port;
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


	bool endsWith(const std::string& data, const std::string& end)
	{
		return data.size() >= end.size() && data.compare(data.size() - end.size(), end.size(), end) == 0;
	}


	std::string receiveUntil(StreamSocket& socket, const std::string& end, int milliseconds = 10000)
		/// Reads from the socket until what was read ends with the given
		/// text. Gives up when the peer closes the connection or sends
		/// nothing for the given time, and returns what was read.
	{
		const Poco::Timespan timeout(Poco::Timespan::TimeDiff(milliseconds)*1000);
		std::string data;
		char buffer[1024];
		while (!endsWith(data, end) && socket.poll(timeout, Socket::SELECT_READ))
		{
			int n = socket.receiveBytes(buffer, sizeof(buffer));
			if (n <= 0) break;
			data.append(buffer, n);
		}
		return data;
	}


	class ObservedChannel: public RemoteSyslogChannel
		/// A channel that lets a test see its connections.
	{
	public:
		void breakConnection()
			/// Breaks the connection in a way that shows only
			/// when the next message is sent.
		{
			_last.shutdownSend();
		}

		bool waitForServerClose(int milliseconds = 10000)
			/// Waits until the close of the connection by the server
			/// has arrived.
		{
			return _last.poll(Poco::Timespan(Poco::Timespan::TimeDiff(milliseconds)*1000), Socket::SELECT_READ);
		}

		int connections() const
		{
			return _connections;
		}

		const std::string& hostName() const
		{
			return _hostName;
		}

	protected:
		StreamSocket createSocket(const SocketAddress& address, const std::string& hostName, const Poco::Timespan& timeout) override
		{
			_last = RemoteSyslogChannel::createSocket(address, hostName, timeout);
			_hostName = hostName;
			++_connections;
			return _last;
		}

	private:
		StreamSocket _last;
		std::string _hostName;
		int _connections = 0;
	};


	class NamedChannel: public RemoteSyslogChannel
		/// A channel whose name service is the test: it tells the name
		/// of the local host when the test lets it, or that there is none.
	{
	public:
		NamedChannel(const std::string& name = "canonical.example"):
			_pService(std::make_shared<Service>())
		{
			_pService->name = name;
		}

		void answer()
			/// Lets the name service answer.
		{
			{
				std::lock_guard<std::mutex> lock(_pService->mutex);
				_pService->open = true;
			}
			_pService->opened.notify_all();
		}

	protected:
		~NamedChannel()
		{
			// nothing is left waiting, whatever became of the test
			answer();
		}

		HostNameSource hostNameSource() const override
		{
			std::shared_ptr<Service> pService = _pService;
			return [pService]()
			{
				std::unique_lock<std::mutex> lock(pService->mutex);
				pService->opened.wait(lock, [&pService] { return pService->open; });
				if (pService->name.empty()) throw Poco::Net::HostNotFoundException();
				return pService->name;
			};
		}

	private:
		struct Service
		{
			std::mutex mutex;
			std::condition_variable opened;
			bool open = false;
			std::string name;
		};

		std::shared_ptr<Service> _pService;
	};


	class TargetChannel: public RemoteSyslogChannel
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


namespace
{
	class HoldingChannel: public Poco::Channel
		/// A channel that holds the thread that logs to it until it is
		/// let go, which holds a parser thread of a listener.
	{
	public:
		void log(const Poco::Message&) override
		{
			_entered.set();
			(void) _letGo.tryWait(10000);
		}

		bool waitUntilEntered(long milliseconds = 10000)
		{
			return _entered.tryWait(milliseconds);
		}

		void letGo()
		{
			_letGo.set();
		}

	private:
		Poco::Event _entered;
		Poco::Event _letGo{Poco::Event::EVENT_MANUALRESET};
	};


	class CountingErrorHandler: public Poco::ErrorHandler
		/// Counts the exceptions reported to it, and logs through a channel
		/// while it is at it, as the handler of an application may.
	{
	public:
		explicit CountingErrorHandler(Poco::Channel::Ptr pChannel):
			_pChannel(pChannel)
		{
		}

		void exception(const Poco::Exception&) override
		{
			reported();
		}

		void exception(const std::exception&) override
		{
			reported();
		}

		void exception() override
		{
			reported();
		}

		int reports() const
		{
			return _reports;
		}

	private:
		void reported()
		{
			++_reports;
			_pChannel->log(Poco::Message("handler", "reported", Poco::Message::PRIO_ERROR));
		}

		Poco::Channel::Ptr _pChannel;
		std::atomic<int>   _reports{0};
	};


	class ErrorHandlerGuard
		/// Installs an ErrorHandler for the time of a test.
	{
	public:
		explicit ErrorHandlerGuard(Poco::ErrorHandler& handler):
			_pPrevious(Poco::ErrorHandler::set(&handler))
		{
		}

		~ErrorHandlerGuard()
		{
			Poco::ErrorHandler::set(_pPrevious);
		}

	private:
		Poco::ErrorHandler* _pPrevious;
	};


	class Joiner
		/// Joins a thread of a test when the test is left, also by a
		/// failed assertion, after it has made the thread stop.
	{
	public:
		Joiner(std::thread& thread, std::function<void()> stop):
			_thread(thread),
			_stop(std::move(stop))
		{
		}

		~Joiner()
		{
			_stop();
			if (_thread.joinable()) _thread.join();
		}

	private:
		std::thread& _thread;
		std::function<void()> _stop;
	};


	class StallingSocketImpl: public StreamSocketImpl
		/// A connected socket that takes nothing: what a server that never
		/// reads leaves a sender with once the buffers are full, without
		/// a dependence on how much a system buffers.
	{
	public:
		int sendBytes(const void*, int, int) override
		{
			// nothing taken, as a non-blocking socket says
			return -1;
		}

		bool poll(const Poco::Timespan& timeout, int) override
		{
			// nothing arrives on the connection either: the time runs out
			return StreamSocketImpl::poll(timeout, SELECT_READ);
		}
	};


	class StallingSocket: public StreamSocket
	{
	public:
		StallingSocket(): StreamSocket(new StallingSocketImpl)
		{
		}
	};


	class StallingChannel: public RemoteSyslogChannel
		/// A channel whose connections take nothing.
	{
	protected:
		StreamSocket createSocket(const SocketAddress& address, const std::string&, const Poco::Timespan& timeout) override
		{
			StallingSocket socket;
			socket.connect(address, timeout);
			return socket;
		}
	};
}


SyslogTest::SyslogTest(const std::string& name): CppUnit::TestCase(name)
{
}


SyslogTest::~SyslogTest()
{
}


void SyslogTest::testListener()
{
	UDPListener listener;
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", listener.address());
	channel->setProperty("host", "ahost");
	channel->open();
	Poco::Message msg("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	channel->log(msg);
	assertTrue (listener.pChannel->waitFor(1));
	channel->close();
	std::vector<Poco::Message> msgs = listener.pChannel->messages();
	assertTrue (msgs.size() == 1);
	assertTrue (msgs[0].getSource() == "asource");
	assertTrue (msgs[0].getText() == "amessage");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
}


void SyslogTest::testChannelFacility()
{
	UDPListener listener;
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", listener.address());
	channel->setProperty("host", "ahost");
	channel->setProperty("facility", "KERN");
	channel->open();
	Poco::Message msg("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	channel->log(msg);
	channel->setProperty("facility", "USER");
	msg.setText("asecondmessage");
	channel->log(msg);
	assertFalse (msg.has("facility"));
	assertTrue (listener.pChannel->waitFor(2));
	channel->close();
	std::vector<Poco::Message> msgs = listener.pChannel->messages();
	assertTrue (msgs.size() == 2);

	assertTrue (msgs[0].getSource() == "asource");
	assertTrue (msgs[0].getText() == "amessage");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[0].has("facility"));
	assertTrue (msgs[0].get("facility") == "KERN");

	assertTrue (msgs[1].getSource() == "asource");
	assertTrue (msgs[1].getText() == "asecondmessage");
	assertTrue (msgs[1].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[1].has("facility"));
	assertTrue (msgs[1].get("facility") == "USER");
}


void SyslogTest::testChannelOpenClose()
{
	UDPListener listener;
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", listener.address());
	channel->setProperty("host", "ahost");
	channel->open();

	Poco::Message msg1("source1", "message1", Poco::Message::PRIO_CRITICAL);
	channel->log(msg1);
	assertTrue (listener.pChannel->waitFor(1));

	channel->close(); // close and re-open channel
	channel->open();

	Poco::Message msg2("source2", "message2", Poco::Message::PRIO_ERROR);
	channel->log(msg2);
	assertTrue (listener.pChannel->waitFor(2));
	channel->close();

	std::vector<Poco::Message> msgs = listener.pChannel->messages();
	assertTrue (msgs.size() == 2);

	assertTrue (msgs[0].getSource() == "source1");
	assertTrue (msgs[0].getText() == "message1");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);

	assertTrue (msgs[1].getSource() == "source2");
	assertTrue (msgs[1].getText() == "message2");
	assertTrue (msgs[1].getPriority() == Poco::Message::PRIO_ERROR);
}


void SyslogTest::testOldBSD()
{
	UDPListener listener;
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", listener.address());
	channel->setProperty("host", "ahost");
	channel->setProperty("format", "bsd");
	channel->open();
	Poco::Message msg("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	channel->log(msg);
	assertTrue (listener.pChannel->waitFor(1));
	channel->close();
	std::vector<Poco::Message> msgs = listener.pChannel->messages();
	assertTrue (msgs.size() == 1);
	// the source is lost with old BSD messages: we only send the local host name!
	assertTrue (msgs[0].getSource() == "ahost");
	assertTrue (msgs[0].getText() == "amessage");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
}


void SyslogTest::testStructuredData()
{
	UDPListener listener;
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", listener.address());
	channel->setProperty("host", "ahost");
	channel->open();
	Poco::Message msg1("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	msg1.set("structured-data", "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"]");
	channel->log(msg1);
	Poco::Message msg2("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	msg2.set("structured-data", "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"][examplePriority@32473 class=\"high\"]");
	channel->log(msg2);
	assertTrue (listener.pChannel->waitFor(2));
	channel->close();
	std::vector<Poco::Message> msgs = listener.pChannel->messages();
	assertTrue (msgs.size() == 2);

	assertTrue (msgs[0].getSource() == "asource");
	assertTrue (msgs[0].getText() == "amessage");
	assertTrue (msgs[0].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[0].get("structured-data") == "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"]");

	assertTrue (msgs[1].getSource() == "asource");
	assertTrue (msgs[1].getText() == "amessage");
	assertTrue (msgs[1].getPriority() == Poco::Message::PRIO_CRITICAL);
	assertTrue (msgs[1].get("structured-data") == "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"][examplePriority@32473 class=\"high\"]");
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


void SyslogTest::testTimestamp()
{
	// The TIMESTAMP of an RFC 5424 message: the fraction of a second is
	// optional and has one to six digits, and the time is that of the time
	// zone whose offset follows it. What is not such a timestamp leaves the
	// message the time of its arrival.
	struct Sample
	{
		const char* timestamp;
		const char* utc; // null: not a timestamp
	};
	const Sample samples[] =
	{
		// the examples of RFC 5424, 6.2.3.1
		{"1985-04-12T23:20:50.52Z", "1985-04-12T23:20:50.520000"},
		{"1985-04-12T19:20:50.52-04:00", "1985-04-12T23:20:50.520000"},
		{"2003-10-11T22:14:15.003Z", "2003-10-11T22:14:15.003000"},
		{"2003-08-24T05:14:15.000003-07:00", "2003-08-24T12:14:15.000003"},
		{"2003-08-24T05:14:15.000000003-07:00", nullptr},

		{"2003-10-11T22:14:15Z", "2003-10-11T22:14:15.000000"},
		{"2003-10-11T22:14:15+02:00", "2003-10-11T20:14:15.000000"},
		{"2003-10-11T22:14:15.5Z", "2003-10-11T22:14:15.500000"},
		{"2003-10-11T22:14:15.52Z", "2003-10-11T22:14:15.520000"},
		{"2003-10-11T22:14:15.123Z", "2003-10-11T22:14:15.123000"},
		{"2003-10-11T22:14:15.1234Z", "2003-10-11T22:14:15.123400"},
		{"2003-10-11T22:14:15.12345Z", "2003-10-11T22:14:15.123450"},
		{"2003-10-11T22:14:15.123456Z", "2003-10-11T22:14:15.123456"},
		{"2003-10-11T22:14:15.003+02:00", "2003-10-11T20:14:15.003000"},
		{"2003-12-31T23:30:00.25-05:30", "2004-01-01T05:00:00.250000"},
		{"2004-03-01T00:15:00+01:00", "2004-02-29T23:15:00.000000"},
		{"2003-10-11T22:14:15+00:00", "2003-10-11T22:14:15.000000"},

		{"-", nullptr},
		{"2003-10-11T22:14:15", nullptr},
		{"2003-10-11T22:14:15.Z", nullptr},
		{"2003-10-11T22:14:15.1234567Z", nullptr},
		{"2003-10-11T22:14:15+0200", nullptr},
		{"2003-10-11T22:14:15+02", nullptr},
		{"2003-10-11T22:14:15+24:00", nullptr},
		{"2003-10-11T22:14:15+02:60", nullptr},
		{"2003-10-11T22:14:15z", nullptr},
		{"2003-10-11t22:14:15Z", nullptr},
		{"2003-10-11T22:14:15ZZ", nullptr},
		{"2003-10-11T22:14Z", nullptr},
		{"2003-10-11", nullptr},
		{"20031011T221415Z", nullptr},
		{"2003-02-29T22:14:15Z", nullptr},
		{"2003-13-11T22:14:15Z", nullptr},
		{"2003-10-11T24:00:00Z", nullptr},
		{"2003-10-11T22:60:15Z", nullptr},
		{"2003-10-11T23:59:60Z", nullptr}
	};

	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(0);
	listener->open();
	auto pCL = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL);

	// One message at a time, so that each is known by its place and the
	// time of its arrival lies between two readings of the clock.
	std::vector<std::pair<Poco::Timestamp, Poco::Timestamp>> arrivals;
	bool arrived = true;
	std::size_t count = 0;
	for (const Sample& sample: samples)
	{
		const Poco::Timestamp before;
		listener->enqueueMessage(std::string("<34>1 ") + sample.timestamp + " ahost anapp 77 asource - text", SocketAddress("127.0.0.1", 514));
		arrived = pCL->waitFor(++count);
		if (!arrived) break;
		arrivals.emplace_back(before, Poco::Timestamp());
	}
	listener->close();
	assertTrue (arrived);

	std::vector<Poco::Message> msgs = pCL->messages();
	assertEqual (sizeof(samples)/sizeof(samples[0]), msgs.size());
	for (std::size_t i = 0; i < msgs.size(); ++i)
	{
		const Poco::Timestamp time = msgs[i].getTime();
		if (samples[i].utc)
		{
			assertEqual (std::string(samples[i].utc), Poco::DateTimeFormatter::format(time, "%Y-%m-%dT%H:%M:%S.%F"));
		}
		else if (time < arrivals[i].first || time > arrivals[i].second)
		{
			failmsg(std::string("not the time of arrival: ") + samples[i].timestamp);
		}
		// The rest of the message is read whatever its timestamp.
		assertEqual (std::string("text"), msgs[i].getText());
		assertEqual (std::string("asource"), msgs[i].getSource());
	}
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

	// A counted message that is too long and arrives in pieces: the cut
	// message is taken from the first piece, the rest is skipped as it
	// comes, and the message after it is whole.
	const std::string frame = counted(big);
	const std::size_t piece = frame.find(' ') + 1 + maxMessageSize + 7;
	send(client, frame.substr(0, piece));
	assertTrue (listener.pChannel->waitFor(7));
	send(client, frame.substr(piece) + counted("after the pieces"));
	assertTrue (listener.pChannel->waitFor(8));
	texts = listener.pChannel->texts();
	assertTrue (texts.size() == 8);
	assertTrue (texts[6] == cut);
	assertTrue (texts[7] == "after the pieces");

	// a message of exactly the size is whole
	const std::string fits(maxMessageSize - MESSAGE_HEADER.size(), 'y');
	send(client, counted(fits) + line(fits) + line("last"));
	assertTrue (listener.pChannel->waitFor(11));
	texts = listener.pChannel->texts();
	assertTrue (texts.size() == 11);
	assertTrue (texts[8] == fits);
	assertTrue (texts[9] == fits);
	assertTrue (texts[10] == "last");
}


void SyslogTest::testTCPExactSize()
{
	const int maxSize = 256;
	TCPListener listener(maxSize);
	StreamSocket socket(listener.address());

	// A message of just the largest size is whole, and is taken when its
	// end has arrived: the carriage return is of the end, not of the
	// message. The message before it tells when the listener has seen
	// all that was sent so far.
	const std::string text(maxSize - MESSAGE_HEADER.size() - 1, 'a');
	send(socket, line("before") + message(text) + '\r');
	assertTrue (listener.pChannel->waitFor(1));
	send(socket, '\n' + line("after"));
	assertTrue (listener.pChannel->waitFor(3));

	const std::string whole(maxSize - MESSAGE_HEADER.size(), 'b');
	send(socket, line(whole));
	assertTrue (listener.pChannel->waitFor(4));
	socket.close();

	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts.size() == 4);
	assertTrue (texts[0] == "before");
	assertTrue (texts[1] == text);
	assertTrue (texts[2] == "after");
	assertTrue (texts[3] == whole);
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
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(0);
	assertTrue (listener->getProperty("tcpPort") == "0");
	assertTrue (listener->getProperty("maxMessageSize") == "65536");
	// A port that was free a moment ago may have been taken in between:
	// another is tried then.
	Poco::UInt16 port = 0;
	for (int i = 0; i < 100 && port == 0; ++i)
	{
		Poco::UInt16 candidate = 0;
		{
			ServerSocket socket(SocketAddress("127.0.0.1", 0));
			candidate = socket.address().port();
		}
		listener->setProperty("tcpPort", std::to_string(candidate));
		assertTrue (listener->getProperty("tcpPort") == std::to_string(candidate));
		try
		{
			listener->open();
			port = candidate;
		}
		catch (Poco::Net::NetException&)
		{
		}
	}
	assertTrue (port != 0);
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


void SyslogTest::testTCPChannel()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", server.address().toString());
	channel->setProperty("transport", "tcp");
	channel->setProperty("host", "ahost");
	channel->setProperty("name", "anapp");
	Poco::Message msg("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	const std::string tail = " ahost anapp " + std::to_string(msg.getPid()) + " asource - ";

	// the connection is made with the first message
	assertTrue (!server.poll(Poco::Timespan(), Socket::SELECT_READ));
	channel->log(msg);
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	StreamSocket connection = server.acceptConnection();

	// a line feed after the message
	std::string data = receiveUntil(connection, "amessage\n");
	assertTrue (data.compare(0, 6, "<10>1 ") == 0);
	assertTrue (endsWith(data, tail + "amessage\n"));
	assertTrue (data.find('\n') == data.size() - 1);

	// and none within it
	msg.setText("two\nlines");
	channel->log(msg);
	data = receiveUntil(connection, "lines\n");
	assertTrue (data.compare(0, 6, "<10>1 ") == 0);
	assertTrue (endsWith(data, tail + "two lines\n"));
	assertTrue (data.find('\n') == data.size() - 1);

	// the length before the message, which is as it was given
	channel->setProperty("framing", "octet-counting");
	channel->log(msg);
	data = receiveUntil(connection, "lines");
	std::string::size_type space = data.find(' ');
	assertTrue (space != std::string::npos);
	assertTrue (data.substr(0, space) == std::to_string(data.size() - space - 1));
	assertTrue (data.compare(space + 1, 6, "<10>1 ") == 0);
	assertTrue (endsWith(data, tail + "two\nlines"));

	// over the same connection
	assertTrue (!server.poll(Poco::Timespan(), Socket::SELECT_READ));
	channel->close();
	assertTrue (closedByPeer(connection));
}


void SyslogTest::testTCPChannelToListener()
{
	TCPListener listener;
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", listener.address().toString());
	channel->setProperty("transport", "tcp");
	channel->setProperty("facility", "LOCAL3");
	channel->setProperty("host", "ahost");
	channel->setProperty("name", "anapp");

	Poco::Message msg1("asource", "amessage", Poco::Message::PRIO_CRITICAL);
	msg1.set("structured-data", "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"]");
	channel->log(msg1);
	channel->setProperty("framing", "octet-counting");
	Poco::Message msg2("asource", "two\nlines", Poco::Message::PRIO_ERROR);
	channel->log(msg2);
	channel->setProperty("format", "bsd");
	Poco::Message msg3("asource", "an old message", Poco::Message::PRIO_WARNING);
	channel->log(msg3);
	channel->setProperty("framing", "newline");
	channel->log(msg3);
	assertTrue (listener.pChannel->waitFor(4));
	channel->close();

	std::vector<Poco::Message> msgs = listener.pChannel->messages();
	assertTrue (msgs.size() == 4);
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
	// the source is lost with old BSD messages, the host takes its place
	assertTrue (msgs[2].getSource() == "ahost");
	assertTrue (msgs[2].getText() == "an old message");
	assertTrue (msgs[2].getPriority() == Poco::Message::PRIO_WARNING);
	assertTrue (msgs[3].getSource() == "ahost");
	assertTrue (msgs[3].getText() == "an old message");
}


void SyslogTest::testTCPChannelReconnect()
{
	SocketAddress address;
	Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(0);
	{
		ServerSocket socket(SocketAddress("127.0.0.1", 0));
		address = socket.address();
		listener->addServerSocket(socket);
	}
	listener->open();
	auto pCL = Poco::makeAuto<CollectingChannel>();
	listener->addChannel(pCL);

	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", address.toString());
	channel->setProperty("transport", "tcp");
	channel->setProperty("retryInterval", "10");
	channel->setProperty("host", "ahost");
	Poco::Message before("asource", "before", Poco::Message::PRIO_CRITICAL);
	channel->log(before);
	assertTrue (pCL->waitFor(1));

	// the server goes away and comes back on the same port
	listener->close();
	ServerSocket socket;
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
	channel->close();
	listener->close();
	assertTrue (arrived);
	assertTrue (pCL2->texts()[0] == "after");
}


void SyslogTest::testTCPChannelServerClosed()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	Poco::AutoPtr<ObservedChannel> channel = new ObservedChannel;
	channel->setProperty("loghost", server.address().toString());
	channel->setProperty("transport", "tcp");
	channel->setProperty("host", "ahost");
	Poco::Message msg("asource", "one", Poco::Message::PRIO_CRITICAL);
	channel->log(msg);
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	StreamSocket first = server.acceptConnection();
	assertTrue (endsWith(receiveUntil(first, "one\n"), " - one\n"));

	// A connection that the server has closed is not sent to: the next
	// message makes a new one and is not lost.
	first.close();
	assertTrue (channel->waitForServerClose());
	msg.setText("two");
	channel->log(msg);
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	StreamSocket second = server.acceptConnection();
	assertTrue (endsWith(receiveUntil(second, "two\n"), " - two\n"));
	assertTrue (channel->connections() == 2);
	channel->close();
}


void SyslogTest::testTCPChannelResend()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	Poco::AutoPtr<ObservedChannel> channel = new ObservedChannel;
	channel->setProperty("loghost", server.address().toString());
	channel->setProperty("transport", "tcp");
	channel->setProperty("host", "ahost");
	Poco::Message msg("asource", "one", Poco::Message::PRIO_CRITICAL);
	channel->log(msg);
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	StreamSocket first = server.acceptConnection();
	assertTrue (endsWith(receiveUntil(first, "one\n"), " - one\n"));
	assertTrue (channel->connections() == 1);
	assertTrue (channel->hostName() == "127.0.0.1");

	// The message that cannot be sent over the connection it has
	// is sent over a new one.
	channel->breakConnection();
	msg.setText("two");
	channel->log(msg);
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	StreamSocket second = server.acceptConnection();
	assertTrue (endsWith(receiveUntil(second, "two\n"), " - two\n"));
	assertTrue (channel->connections() == 2);

	msg.setText("three");
	channel->log(msg);
	assertTrue (endsWith(receiveUntil(second, "three\n"), " - three\n"));
	assertTrue (channel->connections() == 2);
	channel->close();
}


void SyslogTest::testTCPChannelServerAway()
{
	// a port that nobody listens on, and that nobody else can take meanwhile
	ServerSocket server;
	server.bind(SocketAddress("127.0.0.1", 0), true, false);
	const SocketAddress address = server.address();
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", address.toString());
	channel->setProperty("transport", "tcp");
	channel->setProperty("timeout", "500");
	channel->setProperty("retryInterval", "3600000");
	// with its name given, the channel does not look up the local host
	// when it is opened, which may take longer than any connecting
	channel->setProperty("host", "ahost");
	Poco::Message msg("asource", "dropped", Poco::Message::PRIO_CRITICAL);

	// neither held up nor made to fail
	Poco::Stopwatch sw;
	sw.start();
	channel->log(msg);
	sw.stop();
	assertTrue (sw.elapsed() < 5*Poco::Stopwatch::resolution());

	// The server is there now. No connection is tried before the retry
	// interval is over, and the messages are dropped.
	server.listen();
	sw.restart();
	channel->log(msg);
	channel->log(msg);
	sw.stop();
	assertTrue (sw.elapsed() < 5*Poco::Stopwatch::resolution());
	assertTrue (!server.poll(Poco::Timespan(), Socket::SELECT_READ));

	// With the interval over, the next message makes the connection.
	channel->setProperty("retryInterval", "0");
	msg.setText("sent");
	channel->log(msg);
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	StreamSocket connection = server.acceptConnection();
	std::string data = receiveUntil(connection, "sent\n");
	assertTrue (endsWith(data, " - sent\n"));
	assertTrue (data.find("dropped") == std::string::npos);
	channel->close();
}


void SyslogTest::testTCPChannelTarget()
{
	Poco::AutoPtr<TargetChannel> channel = new TargetChannel;
	channel->setProperty("transport", "tcp");
	channel->setProperty("retryInterval", "0");
	Poco::Message msg("asource", "amessage", Poco::Message::PRIO_CRITICAL);

	// what the "loghost" property may say, and where that is
	const char* targets[][2] =
	{
		{"127.0.0.1", "127.0.0.1:514 127.0.0.1"},
#if defined(POCO_HAVE_IPv6)
		{"[::1]", "[::1]:514 ::1"},
		{"[::1]:1514", "[::1]:1514 ::1"},
		{"::1", "[::1]:514 ::1"},
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

	// A server whose address cannot be found is a server that is away:
	// the message is dropped, and log() does not throw.
	const std::size_t attempts = channel->targets.size();
	channel->setProperty("loghost", "127.0.0.1:nosuchservice");
	channel->log(msg);
	assertTrue (channel->targets.size() == attempts);
	channel->close();
}


void SyslogTest::testTCPChannelHostName()
{
	TCPListener listener;
	Poco::Message msg("asource", "amessage", Poco::Message::PRIO_CRITICAL);

	// The name service has told the name by the time the first message goes.
	{
		Poco::AutoPtr<NamedChannel> channel = new NamedChannel;
		channel->setProperty("loghost", listener.address().toString());
		channel->setProperty("transport", "tcp");
		channel->answer();
		channel->log(msg);
		assertTrue (listener.pChannel->waitFor(1));
		assertTrue (listener.pChannel->messages().back().get("host") == "canonical.example");
		channel->close();
	}

	// The name service does not answer. The first message is not held up
	// for longer than the channel is told to wait, and says the name that
	// the system has for the host. Once the answer is there, the messages
	// say the name it told.
	{
		Poco::AutoPtr<NamedChannel> channel = new NamedChannel;
		channel->setProperty("loghost", listener.address().toString());
		channel->setProperty("transport", "tcp");
		channel->setProperty("timeout", "100");
		// the first message waits for the answer no longer than the timeout
		Poco::Stopwatch sw;
		sw.start();
		channel->log(msg);
		sw.stop();
		assertTrue (sw.elapsed() >= Poco::Stopwatch::resolution()/10);
		assertTrue (sw.elapsed() < 5*Poco::Stopwatch::resolution());
		assertTrue (listener.pChannel->waitFor(2));
		assertTrue (listener.pChannel->messages().back().get("host") == DNS::hostName());

		channel->answer();
		bool told = false;
		sw.restart();
		for (std::size_t i = 3; !told && sw.elapsed() < 10*Poco::Stopwatch::resolution(); ++i)
		{
			channel->log(msg);
			assertTrue (listener.pChannel->waitFor(i));
			told = listener.pChannel->messages().back().get("host") == "canonical.example";
		}
		assertTrue (told);
		channel->close();
	}

	// The name service has no name to tell: the name that the system has
	// for the host is sent.
	{
		const std::size_t before = listener.pChannel->messages().size();
		Poco::AutoPtr<NamedChannel> channel = new NamedChannel("");
		channel->setProperty("loghost", listener.address().toString());
		channel->setProperty("transport", "tcp");
		channel->answer();
		channel->log(msg);
		assertTrue (listener.pChannel->waitFor(before + 1));
		assertTrue (listener.pChannel->messages().back().get("host") == DNS::hostName());
		channel->close();
	}

	// A name that is given is taken, and nothing is asked.
	{
		const std::size_t before = listener.pChannel->messages().size();
		Poco::AutoPtr<NamedChannel> channel = new NamedChannel;
		channel->setProperty("loghost", listener.address().toString());
		channel->setProperty("transport", "tcp");
		channel->setProperty("host", "ahost");
		channel->log(msg);
		assertTrue (listener.pChannel->waitFor(before + 1));
		assertTrue (listener.pChannel->messages().back().get("host") == "ahost");
		channel->close();
	}
}


void SyslogTest::testTCPChannelSwitchTransport()
{
	// a port that is free for TCP and for UDP
	ServerSocket server;
	DatagramSocket datagrams;
	bool bound = false;
	for (int i = 0; i < 100 && !bound; ++i)
	{
		server = ServerSocket(SocketAddress("127.0.0.1", 0));
		try
		{
			datagrams = DatagramSocket(server.address(), false);
			bound = true;
		}
		catch (Poco::Exception&)
		{
		}
	}
	assertTrue (bound);

	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	channel->setProperty("loghost", server.address().toString());
	channel->setProperty("transport", "tcp");
	channel->setProperty("host", "ahost");
	Poco::Message msg("asource", "one", Poco::Message::PRIO_CRITICAL);
	channel->log(msg);
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	StreamSocket connection = server.acceptConnection();
	assertTrue (endsWith(receiveUntil(connection, "one\n"), " - one\n"));

	// the change takes effect with the next message, and the connection goes
	channel->setProperty("transport", "udp");
	assertTrue (closedByPeer(connection));
	msg.setText("two");
	channel->log(msg);
	assertTrue (datagrams.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	char buffer[1024];
	int n = datagrams.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (endsWith(std::string(buffer, n), " - two"));

	channel->setProperty("transport", "tcp");
	msg.setText("three");
	channel->log(msg);
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	connection = server.acceptConnection();
	assertTrue (endsWith(receiveUntil(connection, "three\n"), " - three\n"));
	assertTrue (!datagrams.poll(Poco::Timespan(), Socket::SELECT_READ));
	channel->close();
}


void SyslogTest::testTCPChannelProperties()
{
	Poco::AutoPtr<RemoteSyslogChannel> channel = new RemoteSyslogChannel();
	assertTrue (channel->getProperty("transport") == "udp");
	assertTrue (channel->getProperty("framing") == "newline");
	assertTrue (channel->getProperty("timeout") == "2000");
	assertTrue (channel->getProperty("retryInterval") == "5000");

	channel->setProperty("transport", "TCP");
	channel->setProperty("framing", "octet-counting");
	channel->setProperty("timeout", "150");
	channel->setProperty("retryInterval", "0");
	assertTrue (channel->getProperty("transport") == "tcp");
	assertTrue (channel->getProperty("framing") == "octet-counting");
	assertTrue (channel->getProperty("timeout") == "150");
	assertTrue (channel->getProperty("retryInterval") == "0");

	const char* invalid[][2] =
	{
		{"transport", "sctp"},
		{"framing", "none"},
		{"timeout", "0"},
		{"retryInterval", "-1"}
	};
	for (const auto& property: invalid)
	{
		try
		{
			channel->setProperty(property[0], property[1]);
			fail(std::string("not a valid ") + property[0] + " - must throw");
		}
		catch (Poco::InvalidArgumentException&)
		{
		}
	}
	assertTrue (channel->getProperty("transport") == "tcp");
	assertTrue (channel->getProperty("timeout") == "150");
}


void SyslogTest::testTCPOpenFailed()
{
	// An open() that fails because its TCP port is taken leaves nothing
	// behind: the UDP port is free again, and a later open() succeeds.
	// A port that was free a moment ago may have been taken by somebody
	// else in between: the whole is tried again then.
	bool done = false;
	for (int i = 0; i < 100 && !done; ++i)
	{
		Poco::UInt16 udpPort = 0;
		{
			DatagramSocket probe(SocketAddress("127.0.0.1", 0), false);
			udpPort = probe.address().port();
		}
		// Bound to every address and without SO_REUSEADDR, on which the
		// listener binds its own socket: no system lets it have the port.
		ServerSocket taken;
		taken.bind(SocketAddress(IPAddress(), 0), false, false);
		taken.listen();
		Poco::AutoPtr<RemoteSyslogListener> listener = new RemoteSyslogListener(udpPort);
		listener->setProperty("tcpPort", std::to_string(taken.address().port()));
		try
		{
			listener->open();
			fail("the TCP port is taken - open() must throw");
		}
		catch (Poco::IOException&)
		{
			// the address is in use, or Windows denies it
		}
		try
		{
			// nobody holds the UDP port: the socket of the failed open() is gone
			DatagramSocket probe;
			probe.bind(SocketAddress("127.0.0.1", udpPort), false);
		}
		catch (Poco::IOException&)
		{
			continue;
		}
		taken.close();
		listener->open();
		listener->close();
		done = true;
	}
	assertTrue (done);
}


void SyslogTest::testTCPMaxConnections()
{
	Poco::AutoPtr<RemoteSyslogListener> defaults = new RemoteSyslogListener(0);
	assertTrue (defaults->getProperty("maxConnections") == "0");
	assertTrue (defaults->getProperty("idleTimeout") == "0");
	assertTrue (defaults->getProperty("maxQueued") == "0");
	for (const char* name: {"maxConnections", "idleTimeout", "maxQueued"})
	{
		try
		{
			defaults->setProperty(name, "-1");
			fail("a negative limit - must throw");
		}
		catch (Poco::InvalidArgumentException&)
		{
		}
	}

	// With two connections allowed, a third is closed at once, and once
	// one of the two is gone, a new one is taken.
	TCPListener listener({{"maxConnections", "2"}});
	StreamSocket first(listener.address());
	StreamSocket second(listener.address());
	send(first, line("first"));
	send(second, line("second"));
	assertTrue (listener.pChannel->waitFor(2));

	StreamSocket third(listener.address());
	assertTrue (closedByPeer(third));
	assertTrue (listener.pListener->connectionsRefused() == 1);

	first.close();
	// The listener learns of the close on its own thread, so a new
	// connection may still find two there: it is tried again then.
	bool taken = false;
	for (int i = 0; i < 100 && !taken; ++i)
	{
		try
		{
			StreamSocket fourth(listener.address());
			send(fourth, line("fourth"));
			taken = listener.pChannel->waitForText("fourth", 1, 200);
		}
		catch (Poco::Exception&)
		{
			// closed before the message was out
		}
	}
	assertTrue (taken);
}


void SyslogTest::testTCPIdleTimeout()
{
	// Two connections allowed. One sends part of a message and then
	// nothing, the other keeps sending. The first is closed by the
	// listener once the timeout is over, and not before, and the part is
	// dropped; the second stays, and its messages arrive. The slot of the
	// closed one is free again.
	TCPListener listener({{"idleTimeout", "500"}, {"maxConnections", "2"}});
	StreamSocket idle(listener.address());
	StreamSocket busy(listener.address());
	Poco::Stopwatch sw;
	sw.start();
	send(idle, message("begun and never ended"));
	int sentByBusy = 0;
	// the busy one sends whenever the idle one has not been closed for another 50 ms
	while (!idle.poll(Poco::Timespan(50000), Socket::SELECT_READ) && sw.elapsed() < 30*Poco::Stopwatch::resolution())
	{
		send(busy, line("busy " + std::to_string(sentByBusy++)));
	}
	sw.stop();
	assertTrue (closedByPeer(idle));
	assertTrue (sw.elapsed() >= Poco::Stopwatch::resolution()/2);
	assertTrue (listener.pChannel->waitFor(sentByBusy));
	send(busy, line("still busy"));
	assertTrue (listener.pChannel->waitForText("still busy", 1));
	StreamSocket next(listener.address());
	send(next, line("next"));
	assertTrue (listener.pChannel->waitForText("next", 1));
	assertTrue (listener.pListener->connectionsClosedIdle() == 1);
	for (const auto& text: listener.pChannel->texts())
	{
		assertTrue (text != "begun and never ended");
	}

	// With no traffic at all, a listener finds an idle connection as well.
	TCPListener quiet({{"idleTimeout", "500"}});
	StreamSocket lonely(quiet.address());
	send(lonely, message("alone"));
	assertTrue (closedByPeer(lonely));
	assertTrue (quiet.pListener->connectionsClosedIdle() == 1);
}


void SyslogTest::testTCPIdleTimeoutQueueFull()
{
	// While the listener waits for room in the queue, it reads no
	// connection. A sender whose message waits in its socket meanwhile,
	// for longer than the idle timeout, is not idle: it is not closed,
	// and its message arrives.
	TCPListener listener({{"maxQueued", "1"}, {"idleTimeout", "300"}});
	auto pHolding = Poco::makeAuto<HoldingChannel>();
	listener.pListener->addChannel(pHolding);

	StreamSocket a(listener.address());
	StreamSocket b(listener.address());
	// the parser thread is held with the first message of b
	send(b, line("b1"));
	assertTrue (pHolding->waitUntilEntered());
	// One message of a waits in the queue, and the next one holds the
	// thread that reads the connections.
	send(a, line("a1") + line("a2"));
	assertTrue (!closedByPeer(a, 100));
	// this one waits in the socket of b for longer than the idle timeout
	send(b, line("b2"));
	assertTrue (!closedByPeer(b, 600));
	pHolding->letGo();
	assertTrue (listener.pChannel->waitFor(4));
	const std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (std::find(texts.begin(), texts.end(), "b2") != texts.end());
	assertTrue (listener.pListener->connectionsClosedIdle() == 0);
}


void SyslogTest::testTCPMaxQueued()
{
	// With one message allowed to wait, the one that is brought while
	// one waits is held, with the thread that brings it, until the parser
	// thread has taken the waiting one: that is how TCP slows a sender
	// down. Nothing is dropped.
	TCPListener listener({{"maxQueued", "1"}});
	auto pHolding = Poco::makeAuto<HoldingChannel>();
	listener.pListener->addChannel(pHolding);
	const SocketAddress sender("127.0.0.1", 4321);

	listener.pListener->enqueueMessage(message("held by the channel"), sender);
	assertTrue (pHolding->waitUntilEntered());
	// the parser thread is held with the first message; one may wait
	listener.pListener->enqueueMessage(message("waiting in the queue"), sender);

	Poco::Event through;
	std::thread bringer([&]
	{
		listener.pListener->enqueueMessage(message("held at the door"), sender);
		through.set();
	});
	Joiner joiner(bringer, [&] { pHolding->letGo(); });
	// held: not through while the queue is full
	assertTrue (!through.tryWait(100));
	pHolding->letGo();
	assertTrue (through.tryWait(10000));
	assertTrue (listener.pChannel->waitFor(3));
	std::vector<std::string> texts = listener.pChannel->texts();
	assertTrue (texts[0] == "held by the channel");
	assertTrue (texts[1] == "waiting in the queue");
	assertTrue (texts[2] == "held at the door");

	// a burst over a connection arrives whole and in order
	StreamSocket client(listener.address());
	std::string burst;
	for (int i = 0; i < 20; ++i) burst += line("burst " + std::to_string(i));
	send(client, burst);
	assertTrue (listener.pChannel->waitFor(23));
	texts = listener.pChannel->texts();
	assertTrue (texts.size() == 23);
	assertTrue (texts[3] == "burst 0");
	assertTrue (texts[22] == "burst 19");
}


void SyslogTest::testTCPChannelServerNotReading()
{
	// A server that takes the connection and does not read holds a message
	// up for the time allowed, not for good: log() returns without the
	// message having been sent, and the server counts as away.
	ServerSocket server(SocketAddress("127.0.0.1", 0));

	Poco::AutoPtr<StallingChannel> channel = new StallingChannel;
	channel->setProperty("loghost", server.address().toString());
	channel->setProperty("transport", "tcp");
	channel->setProperty("timeout", "300");
	channel->setProperty("host", "ahost");
	Poco::Message msg("asource", "never taken", Poco::Message::PRIO_CRITICAL);

	Poco::Stopwatch sw;
	sw.start();
	channel->log(msg);
	sw.stop();
	assertTrue (sw.elapsed() >= 3*Poco::Stopwatch::resolution()/10);
	assertTrue (sw.elapsed() < 10*Poco::Stopwatch::resolution());

	// the next message is dropped at once
	sw.restart();
	channel->log(msg);
	sw.stop();
	assertTrue (sw.elapsed() < 3*Poco::Stopwatch::resolution()/10);
	channel->close();
}


void SyslogTest::testTCPChannelServerAwayReported()
{
	// A server that is away is reported through the ErrorHandler once,
	// until a connection has been made again, and after log() has let
	// go of its lock: the handler logs through the channel while it is
	// at it.
	ServerSocket server;
	server.bind(SocketAddress("127.0.0.1", 0), true, false);
	Poco::AutoPtr<ObservedChannel> channel = new ObservedChannel;
	channel->setProperty("loghost", server.address().toString());
	channel->setProperty("transport", "tcp");
	channel->setProperty("timeout", "500");
	channel->setProperty("retryInterval", "0");
	channel->setProperty("host", "ahost");
	Poco::Message msg("asource", "sent", Poco::Message::PRIO_CRITICAL);

	CountingErrorHandler handler(channel);
	ErrorHandlerGuard guard(handler);
	channel->log(msg);
	channel->log(msg);
	assertTrue (handler.reports() == 1);

	// the server is there: the next message connects and is sent
	server.listen();
	channel->log(msg);
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	StreamSocket connection = server.acceptConnection();
	assertTrue (endsWith(receiveUntil(connection, "sent\n"), " - sent\n"));

	// gone again, which the channel sees with the next message once the
	// close has arrived: reported once more, and once only
	connection.close();
	server.close();
	assertTrue (channel->waitForServerClose());
	channel->log(msg);
	channel->log(msg);
	assertTrue (handler.reports() == 2);
	channel->close();
}


void SyslogTest::testTCPChannelServerCloses()
{
	// A server that takes every connection and closes it at once is away:
	// the second connection in a row that the channel finds closed is
	// reported, and no connection is made before the retry interval is
	// over.
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	std::atomic<bool> stop(false);
	std::thread closer([&]
	{
		while (!stop)
		{
			if (server.poll(Poco::Timespan(100000), Socket::SELECT_READ))
			{
				StreamSocket connection = server.acceptConnection();
				connection.close();
			}
		}
	});
	Joiner joiner(closer, [&] { stop = true; });

	Poco::AutoPtr<ObservedChannel> channel = new ObservedChannel;
	channel->setProperty("loghost", server.address().toString());
	channel->setProperty("transport", "tcp");
	channel->setProperty("timeout", "5000");
	channel->setProperty("retryInterval", "3600000");
	channel->setProperty("host", "ahost");
	Poco::Message msg("asource", "lost", Poco::Message::PRIO_CRITICAL);
	CountingErrorHandler handler(channel);
	ErrorHandlerGuard guard(handler);

	// Every message finds the connection of the one before it closed, or
	// cannot be sent over its own, which is up to the timing: after a few,
	// the server is away. The close of a connection has arrived before the
	// next message is logged.
	for (int i = 0; i < 5 && handler.reports() == 0; ++i)
	{
		channel->log(msg);
		if (handler.reports() == 0) assertTrue (channel->waitForServerClose());
	}
	assertTrue (handler.reports() == 1);

	// within the retry interval, a message makes no connection
	const int made = channel->connections();
	channel->log(msg);
	channel->log(msg);
	assertTrue (channel->connections() == made);
	assertTrue (handler.reports() == 1);
	channel->close();
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
	CppUnit_addTest(pSuite, SyslogTest, testTimestamp);
	CppUnit_addTest(pSuite, SyslogTest, testTCPOctetCounting);
	CppUnit_addTest(pSuite, SyslogTest, testTCPNewline);
	CppUnit_addTest(pSuite, SyslogTest, testTCPMixedFraming);
	CppUnit_addTest(pSuite, SyslogTest, testTCPSplitFrames);
	CppUnit_addTest(pSuite, SyslogTest, testTCPCoalescedFrames);
	CppUnit_addTest(pSuite, SyslogTest, testTCPOversize);
	CppUnit_addTest(pSuite, SyslogTest, testTCPExactSize);
	CppUnit_addTest(pSuite, SyslogTest, testTCPGarbage);
	CppUnit_addTest(pSuite, SyslogTest, testTCPUnterminated);
	CppUnit_addTest(pSuite, SyslogTest, testTCPManyConnections);
	CppUnit_addTest(pSuite, SyslogTest, testTCPOpenTwice);
	CppUnit_addTest(pSuite, SyslogTest, testTCPTwoSockets);
	CppUnit_addTest(pSuite, SyslogTest, testTCPCloseWithClients);
	CppUnit_addTest(pSuite, SyslogTest, testTCPPort);
	CppUnit_addTest(pSuite, SyslogTest, testTCPOpenFailed);
	CppUnit_addTest(pSuite, SyslogTest, testTCPMaxConnections);
	CppUnit_addTest(pSuite, SyslogTest, testTCPIdleTimeout);
	CppUnit_addTest(pSuite, SyslogTest, testTCPIdleTimeoutQueueFull);
	CppUnit_addTest(pSuite, SyslogTest, testTCPMaxQueued);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannel);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelToListener);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelReconnect);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelServerClosed);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelResend);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelServerAway);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelServerNotReading);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelServerAwayReported);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelServerCloses);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelTarget);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelHostName);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelSwitchTransport);
	CppUnit_addTest(pSuite, SyslogTest, testTCPChannelProperties);

	return pSuite;
}
