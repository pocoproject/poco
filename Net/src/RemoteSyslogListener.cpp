//
// RemoteSyslogListener.cpp
//
// Library: Net
// Package: Logging
// Module:  RemoteSyslogListener
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/RemoteSyslogListener.h"
#include "Poco/Net/RemoteSyslogChannel.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/TCPReactorServer.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Clock.h"
#include "Poco/Runnable.h"
#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DateTimeParser.h"
#include "Poco/Message.h"
#include "Poco/LoggingFactory.h"
#include "Poco/Buffer.h"
#include "Poco/Ascii.h"
#include <algorithm>
#include <atomic>
#include <cstddef>
#include <map>
#include <memory>


namespace Poco::Net {


//
// MessageNotification
//


class MessageNotification: public Poco::Notification
{
public:
	MessageNotification(const char* buffer, std::size_t length, const Poco::Net::SocketAddress& sourceAddress):
		_message(buffer, length),
		_sourceAddress(sourceAddress)
	{
	}

	MessageNotification(const std::string& message, const Poco::Net::SocketAddress& sourceAddress):
		_message(message),
		_sourceAddress(sourceAddress)
	{
	}

	~MessageNotification()
	{
	}

	const std::string& message() const
	{
		return _message;
	}

	const Poco::Net::SocketAddress& sourceAddress() const
	{
		return _sourceAddress;
	}

private:
	std::string _message;
	Poco::Net::SocketAddress _sourceAddress;
};


//
// RemoteUDPListener
//


class RemoteUDPListener: public Poco::Runnable
{
public:
	enum
	{
		WAITTIME_MILLISEC = 1000,
		BUFFER_SIZE = 65536
	};

	RemoteUDPListener(Poco::NotificationQueue& queue, Poco::UInt16 port, bool reusePort, int buffer, int maxQueued, std::atomic<Poco::UInt64>& dropped);
	~RemoteUDPListener();

	void run();
	void safeStop();

private:
	Poco::NotificationQueue& _queue;
	DatagramSocket           _socket;
	int                      _maxQueued;
	std::atomic<Poco::UInt64>& _dropped;
	std::atomic<bool>        _stopped;
};


RemoteUDPListener::RemoteUDPListener(Poco::NotificationQueue& queue, Poco::UInt16 port, bool reusePort, int buffer, int maxQueued, std::atomic<Poco::UInt64>& dropped):
	_queue(queue),
	_socket(Poco::Net::SocketAddress(Poco::Net::IPAddress(), port), false, reusePort),
	_maxQueued(maxQueued),
	_dropped(dropped),
	_stopped(false)
{
	if (buffer > 0)
	{
		_socket.setReceiveBufferSize(buffer);
	}
}


RemoteUDPListener::~RemoteUDPListener()
{
}


void RemoteUDPListener::run()
{
	Poco::Buffer<char> buffer(BUFFER_SIZE);
	Poco::Timespan waitTime(WAITTIME_MILLISEC* 1000);
	while (!_stopped)
	{
		try
		{
			if (_socket.poll(waitTime, Socket::SELECT_READ))
			{
				Poco::Net::SocketAddress sourceAddress;
				int n = _socket.receiveFrom(buffer.begin(), BUFFER_SIZE, sourceAddress);
				// a datagram that finds the queue full is dropped: a sender
				// cannot be slowed down over UDP
				if (n > 0)
				{
					if (_maxQueued == 0 || _queue.size() < _maxQueued)
						_queue.enqueueNotification(new MessageNotification(buffer.begin(), n, sourceAddress));
					else
						++_dropped;
				}
			}
		}
		catch (...)
		{
			// lazy exception catching
		}
	}
}


void RemoteUDPListener::safeStop()
{
	_stopped = true;
}


//
// RemoteTCPListener
//


class RemoteTCPListener
	/// Takes syslog messages from stream connections. Every listening
	/// socket has a TCPReactorServer and with it one thread, which
	/// accepts the connections of that socket and reads from them.
{
public:
	RemoteTCPListener(RemoteSyslogListener& listener, std::size_t maxMessageSize, int maxConnections, const Poco::Timespan& idleTimeout,
		std::atomic<Poco::UInt64>& refused, std::atomic<Poco::UInt64>& closedIdle);
	~RemoteTCPListener();

	void addServerSocket(const ServerSocket& socket);
	void start();
	void stop();

private:
	static constexpr std::size_t MAX_LENGTH_DIGITS = 10;
		/// More digits than these are not taken for the length of a message.

	static constexpr Poco::Clock::ClockDiff HELD_LONG = 10000;
		/// Taking the messages of a connection for longer than this, in
		/// microseconds, held the other connections of the server back.

	struct Connection
		/// Where the framing of a connection stands.
	{
		SocketAddress peerAddress;
		Poco::Clock   lastData;
			/// When the connection was taken or last sent something.
		Poco::UInt64  skip = 0;
			/// Octets still to be skipped of a message that was cut off.
		bool          skipLine = false;
			/// Skipping up to the next line feed.
		std::size_t   scanned = 0;
			/// Octets at the start of what is pending that hold no line feed.
	};

	struct Server
	{
		std::unique_ptr<TCPReactorServer> pServer;
		std::map<std::weak_ptr<TCPReactorServerConnection>, Connection, std::owner_less<>> connections;
			/// Used by the thread of the server only.
		Poco::Clock lastSweep;
			/// When the connections were last looked through for idle ones.
	};

	bool onAccept(Server& server, const TcpReactorConnectionPtr& pConnection);
		/// Takes the connection, unless as many as allowed are there.

	void onData(Server& server, const TcpReactorConnectionPtr& pConnection);
	void onClose(Server& server, const TcpReactorConnectionPtr& pConnection);
	void onTimeout(Server& server);

	Connection& admit(Server& server, const TcpReactorConnectionPtr& pConnection);
		/// Adds the connection to those of the server. The caller counts it.

	void sweep(Server& server);
		/// Closes the connections that have sent nothing for the idle
		/// timeout, at most every quarter of that time.

	std::size_t takeMessages(Connection& connection, const std::string& data);
		/// Enqueues the messages that data holds in full and returns
		/// the number of octets that are done with.

	void enqueue(const Connection& connection, const std::string& data, std::size_t pos, std::size_t length);

	RemoteSyslogListener&                _listener;
	std::size_t                          _maxMessageSize;
	int                                  _maxConnections;
	Poco::Timespan                       _idleTimeout;
	std::vector<std::unique_ptr<Server>> _servers;
	std::atomic<int>                     _connections;
	std::atomic<Poco::UInt64>&           _refused;
	std::atomic<Poco::UInt64>&           _closedIdle;
	std::atomic<bool>                    _stopping;
};


RemoteTCPListener::RemoteTCPListener(RemoteSyslogListener& listener, std::size_t maxMessageSize, int maxConnections, const Poco::Timespan& idleTimeout,
	std::atomic<Poco::UInt64>& refused, std::atomic<Poco::UInt64>& closedIdle):
	_listener(listener),
	_maxMessageSize(maxMessageSize),
	_maxConnections(maxConnections),
	_idleTimeout(idleTimeout),
	_connections(0),
	_refused(refused),
	_closedIdle(closedIdle),
	_stopping(false)
{
}


RemoteTCPListener::~RemoteTCPListener()
{
	try
	{
		stop();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void RemoteTCPListener::addServerSocket(const ServerSocket& socket)
{
	TCPServerParams::Ptr pParams = new TCPServerParams;
	pParams->setReactorMode(true);
	// the thread that accepts the connections reads from them as well
	pParams->setUseSelfReactor(true);
	pParams->setNonBlocking(true);
	// takeMessages() keeps the buffer of a connection within the size of a message
	pParams->setMaxPendingRequestSize(0);

	auto pServer = std::make_unique<Server>();
	Server& server = *pServer;
	pServer->pServer = std::make_unique<TCPReactorServer>(socket, pParams);
	pServer->pServer->setRecvMessageCallback(
		[this, &server](const TcpReactorConnectionPtr& pConnection)
		{
			onData(server, pConnection);
		});
	pServer->pServer->setCloseCallback(
		[this, &server](const TcpReactorConnectionPtr& pConnection)
		{
			onClose(server, pConnection);
		});
	pServer->pServer->setAcceptCallback(
		[this, &server](const TcpReactorConnectionPtr& pConnection)
		{
			return onAccept(server, pConnection);
		});
	// idle connections are also looked for when there is no traffic
	pServer->pServer->setTimeoutCallback([this, &server] { onTimeout(server); });
	_servers.push_back(std::move(pServer));
}


void RemoteTCPListener::start()
{
	for (auto& pServer: _servers)
	{
		pServer->pServer->start();
	}
}


void RemoteTCPListener::stop()
{
	_stopping = true;
	for (auto& pServer: _servers)
	{
		pServer->pServer->stop();
	}
	_servers.clear();
	_connections = 0;
}


bool RemoteTCPListener::onAccept(Server& server, const TcpReactorConnectionPtr& pConnection)
{
	sweep(server);
	// counted first: the servers of a listener take connections on threads of their own
	if (_maxConnections > 0 && ++_connections > _maxConnections)
	{
		--_connections;
		++_refused;
		return false;
	}
	if (_maxConnections == 0) ++_connections;
	admit(server, pConnection);
	return true;
}


RemoteTCPListener::Connection& RemoteTCPListener::admit(Server& server, const TcpReactorConnectionPtr& pConnection)
{
	Connection connection;
	try
	{
		connection.peerAddress = pConnection->socket().peerAddress();
	}
	catch (Poco::Exception&)
	{
		// gone already: what it sent is taken without its address
	}
	return server.connections.emplace(pConnection, connection).first->second;
}


void RemoteTCPListener::onTimeout(Server& server)
{
	sweep(server);
}


void RemoteTCPListener::sweep(Server& server)
{
	if (_idleTimeout == 0) return;
	const Poco::Clock::ClockDiff timeout = _idleTimeout.totalMicroseconds();
	if (!server.lastSweep.isElapsed(timeout/4)) return;
	server.lastSweep.update();

	// An idle connection is taken out of the map before it is closed:
	// what it had begun is not a message, and onClose() does nothing for
	// a connection it does not know.
	std::vector<TcpReactorConnectionPtr> idle;
	for (auto it = server.connections.begin(); it != server.connections.end();)
	{
		if (it->second.lastData.isElapsed(timeout))
		{
			if (TcpReactorConnectionPtr pConnection = it->first.lock()) idle.push_back(pConnection);
			it = server.connections.erase(it);
			--_connections;
			++_closedIdle;
		}
		else ++it;
	}
	for (auto& pConnection: idle)
	{
		pConnection->handleClose();
	}
}


void RemoteTCPListener::onData(Server& server, const TcpReactorConnectionPtr& pConnection)
{
	auto it = server.connections.find(pConnection);
	Connection* pConn = nullptr;
	if (it != server.connections.end())
	{
		pConn = &it->second;
	}
	else
	{
		++_connections;
		pConn = &admit(server, pConnection);
	}
	std::string& data = pConnection->buffer();
	const Poco::Clock before;
	data.erase(0, takeMessages(*pConn, data));
	// While the messages were taken, with a wait for room in the queue
	// maybe, no other connection of the server was read: none of them was
	// idle for that time, whatever waits in its socket.
	const Poco::Clock::ClockDiff held = before.elapsed();
	if (held >= HELD_LONG)
	{
		for (auto& p: server.connections) p.second.lastData += held;
	}
	pConn->lastData.update();
	sweep(server);
}


void RemoteTCPListener::onClose(Server& server, const TcpReactorConnectionPtr& pConnection)
{
	auto it = server.connections.find(pConnection);
	if (it == server.connections.end()) return;

	// Only a sender that closes the connection ends its last message with
	// that. A message that was counted and has not arrived in full is not
	// taken: its length says that something is missing.
	if (!_stopping)
	{
		std::string& data = pConnection->buffer();
		data.erase(0, takeMessages(it->second, data));
		if (!data.empty() && data[0] == '<')
		{
			std::size_t length = data.size();
			if (data[length - 1] == '\r') --length;
			enqueue(it->second, data, 0, std::min(length, _maxMessageSize));
		}
	}
	server.connections.erase(it);
	--_connections;
}


std::size_t RemoteTCPListener::takeMessages(Connection& connection, const std::string& data)
{
	const std::size_t size = data.size();
	std::size_t pos = 0;
	while (pos < size)
	{
		if (connection.skip > 0)
		{
			const std::size_t n = static_cast<std::size_t>(std::min<Poco::UInt64>(connection.skip, size - pos));
			connection.skip -= n;
			pos += n;
		}
		else if (connection.skipLine)
		{
			const std::size_t lf = data.find('\n', pos + connection.scanned);
			connection.scanned = 0;
			if (lf == std::string::npos)
			{
				pos = size;
			}
			else
			{
				connection.skipLine = false;
				pos = lf + 1;
			}
		}
		else if (data[pos] == '<')
		{
			// non-transparent framing: SYSLOG-MSG LF
			const std::size_t lf = data.find('\n', pos + connection.scanned);
			connection.scanned = 0;
			if (lf == std::string::npos)
			{
				// A message of just the largest size may still end with the
				// next octet. What is pending holds no line feed: the next
				// search begins after it.
				if (size - pos <= _maxMessageSize)
				{
					connection.scanned = size - pos;
					break;
				}
				enqueue(connection, data, pos, _maxMessageSize);
				connection.skipLine = true;
				pos += _maxMessageSize;
				connection.scanned = size - pos;
			}
			else
			{
				std::size_t length = lf - pos;
				if (data[lf - 1] == '\r') --length;
				enqueue(connection, data, pos, std::min(length, _maxMessageSize));
				pos = lf + 1;
			}
		}
		else if (Poco::Ascii::isDigit(data[pos]))
		{
			// octet counting: MSG-LEN SP SYSLOG-MSG
			Poco::UInt64 length = 0;
			std::size_t end = pos;
			while (end < size && end - pos < MAX_LENGTH_DIGITS && Poco::Ascii::isDigit(data[end]))
			{
				length = length*10 + static_cast<Poco::UInt64>(data[end] - '0');
				++end;
			}
			if (end == size) break;
			if (data[end] != ' ' || length == 0)
			{
				connection.skipLine = true;
				continue;
			}
			const std::size_t start = end + 1;
			const std::size_t taken = static_cast<std::size_t>(std::min<Poco::UInt64>(length, _maxMessageSize));
			if (size - start < taken) break;
			enqueue(connection, data, start, taken);
			connection.skip = length - taken;
			pos = start + taken;
		}
		else if (data[pos] == '\n' || data[pos] == '\r')
		{
			// a line end that a sender puts after a counted message
			++pos;
		}
		else
		{
			connection.skipLine = true;
		}
	}
	return pos;
}


void RemoteTCPListener::enqueue(const Connection& connection, const std::string& data, std::size_t pos, std::size_t length)
{
	_listener.enqueueMessage(data.data() + pos, length, connection.peerAddress);
}


//
// SyslogParser
//


class SyslogParser: public Poco::Runnable
{
public:
	static const std::string NILVALUE;

	enum
	{
		WAITTIME_MILLISEC = 1000
	};

	SyslogParser(Poco::NotificationQueue& queue, RemoteSyslogListener& listener, Poco::Event& room);
	~SyslogParser();

	void parse(const std::string& line, Poco::Message& message);
	void run();
	void safeStop();

	static Poco::Message::Priority convert(RemoteSyslogChannel::Severity severity);

private:
	void parsePrio(const std::string& line, std::size_t& pos, RemoteSyslogChannel::Severity& severity, RemoteSyslogChannel::Facility& fac);
	void parseNew(const std::string& line, RemoteSyslogChannel::Severity severity, RemoteSyslogChannel::Facility fac, std::size_t& pos, Poco::Message& message);
	void parseBSD(const std::string& line, RemoteSyslogChannel::Severity severity, RemoteSyslogChannel::Facility fac, std::size_t& pos, Poco::Message& message);

	static bool parseTimestamp(const std::string& timestamp, Poco::Timestamp& time);
		/// Parses the TIMESTAMP of an RFC 5424 message: a date and a time
		/// with an optional fraction of a second of one to six digits, followed
		/// by Z or by the offset of the time zone the time is given in.
		///
		/// Returns true and the instant the timestamp stands for, or false,
		/// leaving time as it is, if timestamp is not one. The NILVALUE is
		/// not one.

	static std::string parseUntilSpace(const std::string& line, std::size_t& pos);
		/// Parses until it encounters the next space char, returns the string from pos, excluding space
		/// pos will point past the space char

	static std::string parseStructuredData(const std::string& line, std::size_t& pos);
		/// Parses the structured data field.

	static std::string parseStructuredDataToken(const std::string& line, std::size_t& pos);
	/// Parses a token from the structured data field.

private:
	Poco::NotificationQueue& _queue;
	std::atomic<bool>        _stopped;
	RemoteSyslogListener&    _listener;
	Poco::Event&             _room;
		/// Set whenever a message has been taken from the queue.
};


const std::string SyslogParser::NILVALUE("-");


SyslogParser::SyslogParser(Poco::NotificationQueue& queue, RemoteSyslogListener& listener, Poco::Event& room):
	_queue(queue),
	_stopped(false),
	_listener(listener),
	_room(room)
{
}


SyslogParser::~SyslogParser()
{
}


void SyslogParser::run()
{
	while (!_stopped)
	{
		try
		{
			Poco::AutoPtr<Poco::Notification> pNf(_queue.waitDequeueNotification(WAITTIME_MILLISEC));
			if (pNf)
			{
				_room.set();
				Poco::AutoPtr<MessageNotification> pMsgNf = pNf.cast<MessageNotification>();
				// anything else was enqueued to end the wait
				if (!pMsgNf) continue;
				Poco::Message message;
				parse(pMsgNf->message(), message);
				message["addr"] =pMsgNf->sourceAddress().host().toString();
				_listener.log(message);
			}
		}
		catch (Poco::Exception&)
		{
			// parsing exception, what should we do?
		}
		catch (...)
		{
		}
	}
}


void SyslogParser::safeStop()
{
	_stopped = true;
}


void SyslogParser::parse(const std::string& line, Poco::Message& message)
{
	// <int> -> int: lower 3 bits severity, upper bits: facility
	std::size_t pos = 0;
	RemoteSyslogChannel::Severity severity;
	RemoteSyslogChannel::Facility fac;
	parsePrio(line, pos, severity, fac);

	// the next field decide if we parse an old BSD message or a new syslog message
	// BSD: expects a month value in string form: Jan, Feb...
	// SYSLOG expects a version number: 1

	if (Poco::Ascii::isDigit(line[pos]))
	{
		parseNew(line, severity, fac, pos, message);
	}
	else
	{
		parseBSD(line, severity, fac, pos, message);
	}
	poco_assert (pos == line.size());
}


void SyslogParser::parsePrio(const std::string& line, std::size_t& pos, RemoteSyslogChannel::Severity& severity, RemoteSyslogChannel::Facility& fac)
{
	poco_assert (pos < line.size());
	poco_assert (line[pos] == '<');
	++pos;
	std::size_t start = pos;

	while (pos < line.size() && Poco::Ascii::isDigit(line[pos]))
		++pos;

	poco_assert (line[pos] == '>');
	poco_assert (pos - start > 0);
	std::string valStr = line.substr(start, pos - start);
	++pos; // skip the >

	int val = Poco::NumberParser::parse(valStr);
	poco_assert (val >= 0 && val <= (static_cast<int>(RemoteSyslogChannel::SYSLOG_LOCAL7) + static_cast<int>(RemoteSyslogChannel::SYSLOG_DEBUG)));

	Poco::UInt16 pri = static_cast<Poco::UInt16>(val);
	// now get the lowest 3 bits
	severity = static_cast<RemoteSyslogChannel::Severity>(pri & 0x0007u);
	fac = static_cast<RemoteSyslogChannel::Facility>(pri & 0xfff8u);
}


void SyslogParser::parseNew(const std::string& line, RemoteSyslogChannel::Severity severity, RemoteSyslogChannel::Facility fac, std::size_t& pos, Poco::Message& message)
{
	Poco::Message::Priority prio = convert(severity);
	// rest of the unparsed header is:
	// VERSION SP TIMESTAMP SP HOSTNAME SP APP-NAME SP PROCID SP MSGID
	std::string versionStr(parseUntilSpace(line, pos));
	std::string timeStr(parseUntilSpace(line, pos)); // can be the nilvalue!
	std::string hostName(parseUntilSpace(line, pos));
	std::string appName(parseUntilSpace(line, pos));
	std::string procId(parseUntilSpace(line, pos));
	std::string msgId(parseUntilSpace(line, pos));
	std::string sd(parseStructuredData(line, pos));
	std::string messageText(line.substr(pos));
	pos = line.size();
	Poco::Message logEntry(msgId, messageText, prio);
	logEntry[RemoteSyslogListener::LOG_PROP_FACILITY] = RemoteSyslogChannel::facilityToString(fac);
	logEntry[RemoteSyslogListener::LOG_PROP_HOST] = hostName;
	logEntry[RemoteSyslogListener::LOG_PROP_APP] = appName;
	logEntry[RemoteSyslogListener::LOG_PROP_STRUCTURED_DATA] = sd;

	// Without a timestamp the message keeps the time of its arrival.
	Poco::Timestamp time;
	if (parseTimestamp(timeStr, time))
		logEntry.setTime(time);
	int lval(0);
	(void) Poco::NumberParser::tryParse(procId, lval);
	logEntry.setPid(lval);
	message.swap(logEntry);
}


void SyslogParser::parseBSD(const std::string& line, RemoteSyslogChannel::Severity severity, RemoteSyslogChannel::Facility fac, std::size_t& pos, Poco::Message& message)
{
	Poco::Message::Priority prio = convert(severity);
	// rest of the unparsed header is:
	// "%b %f %H:%M:%S" SP hostname|ipaddress
	// detect three spaces
	int spaceCnt = 0;
	std::size_t start = pos;
	while (spaceCnt < 3 && pos < line.size())
	{
		if (line[pos] == ' ')
		{
			spaceCnt++;
			if (spaceCnt == 1)
			{
				// size must be 3 chars for month
				if (pos - start != 3)
				{
					// probably a shortened time value, or the hostname
					// assume hostName
					Poco::Message logEntry(line.substr(start, pos-start), line.substr(pos+1), prio);
					logEntry[RemoteSyslogListener::LOG_PROP_FACILITY] = RemoteSyslogChannel::facilityToString(fac);
					pos = line.size();
					message.swap(logEntry);
					return;
				}
			}
			else if (spaceCnt == 2)
			{
				// a day value!
				if (!(Poco::Ascii::isDigit(line[pos-1]) && (Poco::Ascii::isDigit(line[pos-2]) || Poco::Ascii::isSpace(line[pos-2]))))
				{
					// assume the next field is a hostname
					spaceCnt = 3;
				}
			}
			if (pos + 1 < line.size() && line[pos+1] == ' ')
			{
				// we have two spaces when the day value is smaller than 10!
				++pos; // skip one
			}
		}
		++pos;
	}
	std::string timeStr(line.substr(start, pos-start-1));
	int tzd(0);
	Poco::DateTime date;
	int year = date.year(); // year is not included, use the current one
	bool hasDate = Poco::DateTimeParser::tryParse(RemoteSyslogChannel::BSD_TIMEFORMAT, timeStr, date, tzd);
	if (hasDate)
	{
		int m = date.month();
		int d = date.day();
		int h = date.hour();
		int min = date.minute();
		int sec = date.second();
		date = Poco::DateTime(year, m, d, h, min, sec);
	}
	// next entry is host SP
	std::string hostName(parseUntilSpace(line, pos));

	// TAG: at most 32 alphanumeric chars, ANY non alphannumeric indicates start of message content
	// ignore: treat everything as content
	std::string messageText(line.substr(pos));
	pos = line.size();
	Poco::Message logEntry(hostName, messageText, prio);
	logEntry.setTime(date.timestamp());
	logEntry[RemoteSyslogListener::LOG_PROP_FACILITY] = RemoteSyslogChannel::facilityToString(fac);
	message.swap(logEntry);
}


bool SyslogParser::parseTimestamp(const std::string& timestamp, Poco::Timestamp& time)
{
	// TIMESTAMP   = NILVALUE / FULL-DATE "T" FULL-TIME
	// FULL-DATE   = 4DIGIT "-" 2DIGIT "-" 2DIGIT
	// FULL-TIME   = 2DIGIT ":" 2DIGIT ":" 2DIGIT ["." 1*6DIGIT] TIME-OFFSET
	// TIME-OFFSET = "Z" / ("+" / "-") 2DIGIT ":" 2DIGIT
	const std::size_t size = timestamp.size();

	const auto number = [&timestamp, size](std::size_t pos, std::size_t digits, int& value)
	{
		if (pos + digits > size) return false;
		value = 0;
		for (std::size_t i = pos; i < pos + digits; ++i)
		{
			if (!Poco::Ascii::isDigit(timestamp[i])) return false;
			value = value*10 + (timestamp[i] - '0');
		}
		return true;
	};
	const auto is = [&timestamp, size](std::size_t pos, char c)
	{
		return pos < size && timestamp[pos] == c;
	};

	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
	if (!(number(0, 4, year) && is(4, '-') && number(5, 2, month) && is(7, '-') && number(8, 2, day) && is(10, 'T')
		&& number(11, 2, hour) && is(13, ':') && number(14, 2, minute) && is(16, ':') && number(17, 2, second)))
	{
		return false;
	}

	std::size_t pos = 19;
	int microseconds = 0;
	if (is(pos, '.'))
	{
		++pos;
		int digits = 0;
		for (; pos < size && Poco::Ascii::isDigit(timestamp[pos]); ++pos, ++digits)
		{
			if (digits == 6) return false;
			microseconds = microseconds*10 + (timestamp[pos] - '0');
		}
		if (digits == 0) return false;
		for (; digits < 6; ++digits) microseconds *= 10;
	}

	int offset = 0; // seconds east of UTC
	if (is(pos, 'Z'))
	{
		++pos;
	}
	else if (is(pos, '+') || is(pos, '-'))
	{
		int hours = 0;
		int minutes = 0;
		if (!(number(pos + 1, 2, hours) && is(pos + 3, ':') && number(pos + 4, 2, minutes)) || hours > 23 || minutes > 59)
		{
			return false;
		}
		offset = (timestamp[pos] == '+' ? 1 : -1)*(hours*3600 + minutes*60);
		pos += 6;
	}
	else return false;

	// A leap second is valid for a DateTime; RFC 5424 does not allow one.
	if (pos != size || second > 59 || !Poco::DateTime::isValid(year, month, day, hour, minute, second, microseconds/1000, microseconds%1000))
	{
		return false;
	}

	time = Poco::DateTime(year, month, day, hour, minute, second, microseconds/1000, microseconds%1000).timestamp();
	time -= Poco::Timespan(offset, 0);
	return true;
}


std::string SyslogParser::parseUntilSpace(const std::string& line, std::size_t& pos)
{
	std::size_t start = pos;
	while (pos < line.size() && !Poco::Ascii::isSpace(line[pos]))
		++pos;
	// skip space
	++pos;
	return line.substr(start, pos-start-1);
}


std::string SyslogParser::parseStructuredData(const std::string& line, std::size_t& pos)
{
	std::string sd;
	if (pos < line.size())
	{
		if (line[pos] == '-')
		{
			++pos;
		}
		else if (line[pos] == '[')
		{
			std::string tok = parseStructuredDataToken(line, pos);
			while (tok == "[")
			{
				sd += tok;
				tok = parseStructuredDataToken(line, pos);
				while (tok != "]" && !tok.empty())
				{
					sd += tok;
					tok = parseStructuredDataToken(line, pos);
				}
				sd += tok;
				if (pos < line.size() && line[pos] == '[') tok = parseStructuredDataToken(line, pos);
			}
		}
		if (pos < line.size() && Poco::Ascii::isSpace(line[pos])) ++pos;
	}
	return sd;
}


std::string SyslogParser::parseStructuredDataToken(const std::string& line, std::size_t& pos)
{
	std::string tok;
	if (pos < line.size())
	{
		if (Poco::Ascii::isSpace(line[pos]) || line[pos] == '=' || line[pos] == '[' || line[pos] == ']')
		{
			tok += line[pos++];
		}
		else if (line[pos] == '"')
		{
			tok += line[pos++];
			while (pos < line.size() && line[pos] != '"')
			{
				tok += line[pos++];
			}
			tok += '"';
			if (pos < line.size()) pos++;
		}
		else
		{
			while (pos < line.size() && !Poco::Ascii::isSpace(line[pos]) && line[pos] != '=')
			{
				tok += line[pos++];
			}
		}
	}
	return tok;
}

Poco::Message::Priority SyslogParser::convert(RemoteSyslogChannel::Severity severity)
{
	switch (severity)
	{
	case RemoteSyslogChannel::SYSLOG_EMERGENCY:
		return Poco::Message::PRIO_FATAL;
	case RemoteSyslogChannel::SYSLOG_ALERT:
		return Poco::Message::PRIO_FATAL;
	case RemoteSyslogChannel::SYSLOG_CRITICAL:
		return Poco::Message::PRIO_CRITICAL;
	case RemoteSyslogChannel::SYSLOG_ERROR:
		return Poco::Message::PRIO_ERROR;
	case RemoteSyslogChannel::SYSLOG_WARNING:
		return Poco::Message::PRIO_WARNING;
	case RemoteSyslogChannel::SYSLOG_NOTICE:
		return Poco::Message::PRIO_NOTICE;
	case RemoteSyslogChannel::SYSLOG_INFORMATIONAL:
		return Poco::Message::PRIO_INFORMATION;
	case RemoteSyslogChannel::SYSLOG_DEBUG:
		return Poco::Message::PRIO_DEBUG;
	}
	throw Poco::LogicException("Illegal severity value in message");
}


//
// RemoteSyslogListener
//


const std::string RemoteSyslogListener::PROP_PORT("port");
const std::string RemoteSyslogListener::PROP_TCP_PORT("tcpPort");
const std::string RemoteSyslogListener::PROP_REUSE_PORT("reusePort");
const std::string RemoteSyslogListener::PROP_THREADS("threads");
const std::string RemoteSyslogListener::PROP_BUFFER("buffer");
const std::string RemoteSyslogListener::PROP_MAX_MESSAGE_SIZE("maxMessageSize");
const std::string RemoteSyslogListener::PROP_MAX_CONNECTIONS("maxConnections");
const std::string RemoteSyslogListener::PROP_IDLE_TIMEOUT("idleTimeout");
const std::string RemoteSyslogListener::PROP_MAX_QUEUED("maxQueued");

const std::string RemoteSyslogListener::LOG_PROP_FACILITY("facility");
const std::string RemoteSyslogListener::LOG_PROP_APP("app");
const std::string RemoteSyslogListener::LOG_PROP_HOST("host");
const std::string RemoteSyslogListener::LOG_PROP_STRUCTURED_DATA("structured-data");


RemoteSyslogListener::RemoteSyslogListener():
	_port(RemoteSyslogChannel::SYSLOG_PORT),
	_tcpPort(0),
	_reusePort(false),
	_threads(1),
	_buffer(0),
	_maxMessageSize(RemoteUDPListener::BUFFER_SIZE)
{
}


RemoteSyslogListener::RemoteSyslogListener(Poco::UInt16 port):
	_port(port),
	_tcpPort(0),
	_reusePort(false),
	_threads(1),
	_buffer(0),
	_maxMessageSize(RemoteUDPListener::BUFFER_SIZE)
{
}


RemoteSyslogListener::RemoteSyslogListener(Poco::UInt16 port, int threads):
	_port(port),
	_tcpPort(0),
	_reusePort(false),
	_threads(threads),
	_buffer(0),
	_maxMessageSize(RemoteUDPListener::BUFFER_SIZE)
{
}


RemoteSyslogListener::RemoteSyslogListener(Poco::UInt16 port, bool reusePort, int threads):
	_port(port),
	_tcpPort(0),
	_reusePort(reusePort),
	_threads(threads),
	_buffer(0),
	_maxMessageSize(RemoteUDPListener::BUFFER_SIZE)
{
}


RemoteSyslogListener::~RemoteSyslogListener()
{
	try
	{
		stop();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void RemoteSyslogListener::processMessage(const std::string& messageText)
{
	Poco::Message message;
	_pParser->parse(messageText, message);
	log(message);
}


void RemoteSyslogListener::enqueueMessage(const std::string& messageText, const Poco::Net::SocketAddress& senderAddress)
{
	waitForQueueRoom();
	_queue.enqueueNotification(new MessageNotification(messageText, senderAddress));
}


void RemoteSyslogListener::enqueueMessage(const char* messageText, std::size_t length, const Poco::Net::SocketAddress& senderAddress)
{
	waitForQueueRoom();
	_queue.enqueueNotification(new MessageNotification(messageText, length, senderAddress));
}


void RemoteSyslogListener::waitForQueueRoom()
{
	// The thread that reads a connection waits here, so that TCP slows
	// the sender down, until a parser thread has taken a message.
	while (!_closing && _queueBound > 0 && _queue.size() >= _queueBound)
	{
		_queueRoom.tryWait(100);
	}
}


bool RemoteSyslogListener::reusePort() const
{
	return _reusePort;
}


Poco::UInt64 RemoteSyslogListener::connectionsRefused() const
{
	return _connectionsRefused;
}


Poco::UInt64 RemoteSyslogListener::connectionsClosedIdle() const
{
	return _connectionsClosedIdle;
}


Poco::UInt64 RemoteSyslogListener::messagesDropped() const
{
	return _messagesDropped;
}


void RemoteSyslogListener::setProperty(const std::string& name, const std::string& value)
{
	if (name == PROP_PORT)
	{
		int val = Poco::NumberParser::parse(value);
		if (val >= 0 && val < 65536)
			_port = static_cast<Poco::UInt16>(val);
		else
			throw Poco::InvalidArgumentException("Not a valid port number", value);
	}
	else if (name == PROP_TCP_PORT)
	{
		int val = Poco::NumberParser::parse(value);
		if (val >= 0 && val < 65536)
			_tcpPort = static_cast<Poco::UInt16>(val);
		else
			throw Poco::InvalidArgumentException("Not a valid port number", value);
	}
	else if (name == PROP_REUSE_PORT)
	{
		_reusePort = Poco::NumberParser::parseBool(value);
	}
	else if (name == PROP_THREADS)
	{
		int val = Poco::NumberParser::parse(value);
		if (val > 0 && val < 16)
			_threads = val;
		else
			throw Poco::InvalidArgumentException("Invalid number of threads", value);
	}
	else if (name == PROP_BUFFER)
	{
		_buffer = Poco::NumberParser::parse(value);
	}
	else if (name == PROP_MAX_MESSAGE_SIZE)
	{
		int val = Poco::NumberParser::parse(value);
		if (val > 0)
			_maxMessageSize = static_cast<std::size_t>(val);
		else
			throw Poco::InvalidArgumentException("Not a valid message size", value);
	}
	else if (name == PROP_MAX_CONNECTIONS)
	{
		int val = Poco::NumberParser::parse(value);
		if (val >= 0)
			_maxConnections = val;
		else
			throw Poco::InvalidArgumentException("Not a valid number of connections", value);
	}
	else if (name == PROP_IDLE_TIMEOUT)
	{
		int val = Poco::NumberParser::parse(value);
		if (val >= 0)
			_idleTimeout = Poco::Timespan(val*Poco::Timespan::MILLISECONDS);
		else
			throw Poco::InvalidArgumentException("Not a valid idle timeout", value);
	}
	else if (name == PROP_MAX_QUEUED)
	{
		int val = Poco::NumberParser::parse(value);
		if (val >= 0)
			_maxQueued = val;
		else
			throw Poco::InvalidArgumentException("Not a valid number of queued messages", value);
	}
	else
	{
		SplitterChannel::setProperty(name, value);
	}
}


std::string RemoteSyslogListener::getProperty(const std::string& name) const
{
	if (name == PROP_PORT)
		return Poco::NumberFormatter::format(_port);
	else if (name == PROP_TCP_PORT)
		return Poco::NumberFormatter::format(_tcpPort);
	else if (name == PROP_REUSE_PORT)
		return Poco::NumberFormatter::format(_reusePort);
	else if (name == PROP_THREADS)
		return Poco::NumberFormatter::format(_threads);
	else if (name == PROP_BUFFER)
		return Poco::NumberFormatter::format(_buffer);
	else if (name == PROP_MAX_MESSAGE_SIZE)
		return Poco::NumberFormatter::format(_maxMessageSize);
	else if (name == PROP_MAX_CONNECTIONS)
		return Poco::NumberFormatter::format(_maxConnections);
	else if (name == PROP_IDLE_TIMEOUT)
		return Poco::NumberFormatter::format(_idleTimeout.totalMilliseconds());
	else if (name == PROP_MAX_QUEUED)
		return Poco::NumberFormatter::format(_maxQueued);
	else
		return SplitterChannel::getProperty(name);
}


void RemoteSyslogListener::addServerSocket(const ServerSocket& socket)
{
	_serverSockets.push_back(socket);
}


void RemoteSyslogListener::createServerSockets(std::vector<ServerSocket>& sockets)
{
	if (_tcpPort > 0)
	{
		ServerSocket socket;
		socket.bind(Poco::Net::SocketAddress(Poco::Net::IPAddress(), _tcpPort), true, _reusePort);
		socket.listen();
		sockets.push_back(socket);
	}
}


void RemoteSyslogListener::open()
{
	if (_pParser) return;

	SplitterChannel::open();
	_closing = false;
	_queueBound = _maxQueued;
	try
	{
		_pParser = std::make_unique<SyslogParser>(_queue, *this, _queueRoom);
		if (_port > 0)
		{
			_pListener = std::make_unique<RemoteUDPListener>(_queue, _port, _reusePort, _buffer, _maxQueued, _messagesDropped);
		}
		std::vector<ServerSocket> sockets(_serverSockets);
		createServerSockets(sockets);
		if (!sockets.empty())
		{
			_pTCPListener = std::make_unique<RemoteTCPListener>(*this, _maxMessageSize, _maxConnections, _idleTimeout, _connectionsRefused, _connectionsClosedIdle);
			for (const auto& socket: sockets)
			{
				_pTCPListener->addServerSocket(socket);
			}
		}
		for (int i = 0; i < _threads; i++)
		{
			_threadPool.start(*_pParser);
		}
		if (_pListener)
		{
			_threadPool.start(*_pListener);
		}
		if (_pTCPListener)
		{
			_pTCPListener->start();
		}
	}
	catch (...)
	{
		// what was started must not go on without the rest
		stop();
		throw;
	}
}


void RemoteSyslogListener::close()
{
	stop();
	_serverSockets.clear();
	SplitterChannel::close();
}


void RemoteSyslogListener::stop()
{
	// a thread that waits for room in the queue is let go first
	_closing = true;
	_queueRoom.set();
	if (_pTCPListener)
	{
		_pTCPListener->stop();
	}
	if (_pListener)
	{
		_pListener->safeStop();
	}
	if (_pParser)
	{
		_pParser->safeStop();
	}
	_queue.clear();
	// one for every parser thread, so that none of them waits out its timeout
	for (int i = 0; _pParser && i < _threads; i++)
	{
		_queue.enqueueNotification(new Poco::Notification);
	}
	_threadPool.joinAll();
	_queue.clear();
	_pTCPListener.reset();
	_pListener.reset();
	_pParser.reset();
}


void RemoteSyslogListener::registerChannel()
{
	Poco::LoggingFactory::defaultFactory().registerChannelClass("RemoteSyslogListener", new Poco::Instantiator<RemoteSyslogListener, Poco::Channel>);
}


} // namespace Poco::Net
