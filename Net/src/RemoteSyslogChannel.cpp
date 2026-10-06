//
// RemoteSyslogChannel.cpp
//
// Library: Net
// Package: Logging
// Module:  RemoteSyslogChannel
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/RemoteSyslogChannel.h"
#include "Poco/Message.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DNS.h"
#include "Poco/LoggingFactory.h"
#include "Poco/Instantiator.h"
#include "Poco/String.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>


namespace Poco::Net {


const std::string RemoteSyslogChannel::BSD_TIMEFORMAT("%b %f %H:%M:%S");
const std::string RemoteSyslogChannel::SYSLOG_TIMEFORMAT("%Y-%m-%dT%H:%M:%S.%i%z");
const std::string RemoteSyslogChannel::PROP_NAME("name");
const std::string RemoteSyslogChannel::PROP_FACILITY("facility");
const std::string RemoteSyslogChannel::PROP_FORMAT("format");
const std::string RemoteSyslogChannel::PROP_LOGHOST("loghost");
const std::string RemoteSyslogChannel::PROP_HOST("host");
const std::string RemoteSyslogChannel::PROP_BUFFER("buffer");
const std::string RemoteSyslogChannel::PROP_TRANSPORT("transport");
const std::string RemoteSyslogChannel::PROP_FRAMING("framing");
const std::string RemoteSyslogChannel::PROP_TIMEOUT("timeout");
const std::string RemoteSyslogChannel::PROP_RETRY_INTERVAL("retryInterval");
const std::string RemoteSyslogChannel::STRUCTURED_DATA("structured-data");


namespace
{
	const Poco::Timespan DEFAULT_TIMEOUT(2, 0);
	const Poco::Timespan DEFAULT_RETRY_INTERVAL(5, 0);
}


struct RemoteSyslogChannel::HostNameLookup
	/// What was told about the name of the local host. The question is
	/// asked on a thread of its own: it takes the time that the name
	/// service takes, and it can neither be given a time nor be taken
	/// back. So whoever waits for the answer says for how long, and the
	/// thread, which keeps nothing but this, ends when the answer is there.
{
	std::mutex              mutex;
	std::condition_variable answered;
	bool                    done = false;
	std::string             name;
		/// Empty if the name could not be told.
};


RemoteSyslogChannel::RemoteSyslogChannel():
	_logHost("localhost"),
	_name("-"),
	_facility(SYSLOG_USER),
	_bsdFormat(false),
	_buffer(0),
	_transport(TRANSPORT_UDP),
	_framing(FRAMING_NEWLINE),
	_timeout(DEFAULT_TIMEOUT),
	_retryInterval(DEFAULT_RETRY_INTERVAL),
	_open(false),
	_connected(false),
	_failed(false)
{
}


RemoteSyslogChannel::RemoteSyslogChannel(const std::string& address, const std::string& name, int facility, bool bsdFormat):
	_logHost(address),
	_name(name),
	_facility(facility),
	_bsdFormat(bsdFormat),
	_buffer(0),
	_transport(TRANSPORT_UDP),
	_framing(FRAMING_NEWLINE),
	_timeout(DEFAULT_TIMEOUT),
	_retryInterval(DEFAULT_RETRY_INTERVAL),
	_open(false),
	_connected(false),
	_failed(false)
{
	if (_name.empty()) _name = "-";
}


RemoteSyslogChannel::~RemoteSyslogChannel()
{
	try
	{
		close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void RemoteSyslogChannel::open()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	openChannel();
}


void RemoteSyslogChannel::openChannel()
{
	if (_open) return;

	if (_transport == TRANSPORT_UDP)
	{
		_socketAddress = logHostAddress();
		// reset socket for the case that it has been previously closed
		_socket = DatagramSocket(_socketAddress.family());
		if (_buffer) _socket.setSendBufferSize(_buffer);
	}

	if (_host.empty()) lookUpHostName();

	_open = true;
}


void RemoteSyslogChannel::lookUpHostName()
{
	if (!_pHostNameLookup)
	{
		auto pLookup = std::make_shared<HostNameLookup>();
		HostNameSource source = hostNameSource();
		try
		{
			std::thread([pLookup, source]()
			{
				std::string name;
				try
				{
					name = source();
				}
				catch (...)
				{
				}
				{
					std::lock_guard<std::mutex> lock(pLookup->mutex);
					pLookup->name = std::move(name);
					pLookup->done = true;
				}
				pLookup->answered.notify_all();
			}).detach();
			_pHostNameLookup = pLookup;
		}
		catch (std::exception&)
		{
			// no thread to ask on
		}
	}

	bool answered = false;
	if (_pHostNameLookup)
	{
		std::unique_lock<std::mutex> lock(_pHostNameLookup->mutex);
		const HostNameLookup& lookup = *_pHostNameLookup;
		answered = _pHostNameLookup->answered.wait_for(lock, std::chrono::microseconds(_timeout.totalMicroseconds()), [&lookup] { return lookup.done; });
		if (answered) _host = lookup.name;
	}
	if (answered)
	{
		_pHostNameLookup.reset();
	}
	else
	{
		// Until the answer is there, the name that the system has for the host.
		try
		{
			_host = DNS::hostName();
		}
		catch (Poco::Exception&)
		{
		}
	}
	if (_host.empty())
	{
		if (_transport == TRANSPORT_UDP)
			_host = _socket.address().host().toString();
		else
			_host = "-"; // the NILVALUE of RFC 5424: not known
	}
}


void RemoteSyslogChannel::takeHostName()
{
	std::string name;
	{
		std::lock_guard<std::mutex> lock(_pHostNameLookup->mutex);
		if (!_pHostNameLookup->done) return;
		name = _pHostNameLookup->name;
	}
	_pHostNameLookup.reset();
	if (!name.empty()) _host = name;
}


void RemoteSyslogChannel::close()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	closeSockets();
}


void RemoteSyslogChannel::closeSockets()
{
	if (_open)
	{
		_socket.close();
		if (_connected)
		{
			try
			{
				_streamSocket.close();
			}
			catch (Poco::Exception&)
			{
			}
			_streamSocket = StreamSocket();
			_connected = false;
		}
		_failed = false;
		_open = false;
	}
}


void RemoteSyslogChannel::log(const Message& msg)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	openChannel();
	if (_pHostNameLookup) takeHostName();

	std::string m;
	m.reserve(1024);
	m += '<';
	Poco::NumberFormatter::append(m, getPrio(msg) + _facility);
	m += '>';
	if (_bsdFormat)
	{
		Poco::DateTimeFormatter::append(m, msg.getTime(), BSD_TIMEFORMAT);
		m += ' ';
		m += _host;
	}
	else
	{
		m += "1 "; // version
		Poco::DateTimeFormatter::append(m, msg.getTime(), SYSLOG_TIMEFORMAT);
		m += ' ';
		m += _host;
		m += ' ';
		m += _name;
		m += ' ';
		Poco::NumberFormatter::append(m, msg.getPid());
		m += ' ';
		m += msg.getSource();
		m += ' ';
		if (msg.has(STRUCTURED_DATA))
		{
			m += msg.get(STRUCTURED_DATA);
		}
		else
		{
			m += "-";
		}
	}
	m += ' ';
	m += msg.getText();

	if (_transport == TRANSPORT_UDP)
	{
		_socket.sendTo(m.data(), static_cast<int>(m.size()), _socketAddress);
	}
	else if (_framing == FRAMING_OCTET_COUNTING)
	{
		std::string frame(Poco::NumberFormatter::format(m.size()));
		frame += ' ';
		frame += m;
		sendOverConnection(frame);
	}
	else
	{
		// a line feed within the message would end it there
		std::replace(m.begin(), m.end(), '\n', ' ');
		m += '\n';
		sendOverConnection(m);
	}
}


void RemoteSyslogChannel::sendOverConnection(const std::string& frame)
{
	if (_connected && closedByServer()) disconnect();

	for (int attempt = 0; attempt < 2; ++attempt)
	{
		const bool fresh = !_connected;
		if (fresh && !connect()) return;
		try
		{
			sendFrame(frame);
			return;
		}
		catch (Poco::Exception&)
		{
			disconnect();
			if (fresh)
			{
				// a server that takes the connection and not the message is away as well
				_failed = true;
				_failedAt.update();
				return;
			}
		}
	}
}


bool RemoteSyslogChannel::connect()
{
	if (_failed && !_failedAt.isElapsed(_retryInterval.totalMicroseconds())) return false;

	try
	{
		// the address of the server is asked for with every connection: it may have changed
		_streamSocket = createSocket(logHostAddress(), logHostName(), _timeout);
		// the receive timeout bounds what a TLS socket reads while it sends
		_streamSocket.setSendTimeout(_timeout);
		_streamSocket.setReceiveTimeout(_timeout);
		_connected = true;
		_failed = false;
	}
	catch (Poco::Exception&)
	{
		_streamSocket = StreamSocket();
		_failed = true;
		_failedAt.update();
	}
	return _connected;
}


void RemoteSyslogChannel::disconnect()
{
	try
	{
		// a non-blocking socket is closed without an answer of the server being waited for
		_streamSocket.setBlocking(false);
		_streamSocket.close();
	}
	catch (Poco::Exception&)
	{
	}
	_streamSocket = StreamSocket();
	_connected = false;
}


void RemoteSyslogChannel::sendFrame(const std::string& frame)
{
	const Poco::Clock::ClockDiff timeout = _timeout.totalMicroseconds();
	Poco::Clock start;
	std::size_t sent = 0;
	while (sent < frame.size())
	{
		const int n = _streamSocket.sendBytes(frame.data() + sent, static_cast<int>(frame.size() - sent));
		if (n > 0) sent += static_cast<std::size_t>(n);
		if (sent < frame.size())
		{
			const Poco::Clock::ClockDiff elapsed = start.elapsed();
			if (elapsed >= timeout) throw Poco::TimeoutException("syslog message not sent in time");
			if (n <= 0) (void) _streamSocket.poll(Poco::Timespan(timeout - elapsed), Socket::SELECT_WRITE);
		}
	}
}


bool RemoteSyslogChannel::closedByServer()
{
	// A syslog server sends nothing. If there is something to read, it is
	// the end of the connection, or what a TLS socket keeps to itself.
	try
	{
		if (!_streamSocket.poll(Poco::Timespan(), Socket::SELECT_READ)) return false;
		char buffer[256];
		_streamSocket.setBlocking(false);
		const int n = _streamSocket.receiveBytes(buffer, sizeof(buffer));
		_streamSocket.setBlocking(true);
		return n == 0;
	}
	catch (Poco::Exception&)
	{
		return true;
	}
}


void RemoteSyslogChannel::splitLogHost(std::string& host, std::string& port) const
{
	host = _logHost;
	port.clear();
	if (!_logHost.empty() && _logHost[0] == '[')
	{
		const std::string::size_type end = _logHost.find(']');
		if (end != std::string::npos)
		{
			host = _logHost.substr(1, end - 1);
			if (end + 1 < _logHost.size() && _logHost[end + 1] == ':') port = _logHost.substr(end + 2);
		}
	}
	else
	{
		// an IPv6 address that is not in brackets has more than the one colon, and no port
		const std::string::size_type colon = _logHost.find(':');
		if (colon != std::string::npos && _logHost.find(':', colon + 1) == std::string::npos)
		{
			host = _logHost.substr(0, colon);
			port = _logHost.substr(colon + 1);
		}
	}
}


std::string RemoteSyslogChannel::logHostName() const
{
	std::string host;
	std::string port;
	splitLogHost(host, port);
	return host;
}


SocketAddress RemoteSyslogChannel::logHostAddress() const
{
	std::string host;
	std::string port;
	splitLogHost(host, port);
	if (port.empty())
		return SocketAddress(host, defaultPort());
	else
		return SocketAddress(host, port);
}


StreamSocket RemoteSyslogChannel::createSocket(const SocketAddress& address, const std::string& hostName, const Poco::Timespan& timeout)
{
	StreamSocket socket;
	socket.connect(address, timeout);
	return socket;
}


Poco::UInt16 RemoteSyslogChannel::defaultPort() const
{
	return SYSLOG_PORT;
}


RemoteSyslogChannel::HostNameSource RemoteSyslogChannel::hostNameSource() const
{
	return []() { return DNS::thisHost().name(); };
}


void RemoteSyslogChannel::setProperty(const std::string& name, const std::string& value)
{
	if (name == PROP_NAME)
	{
		_name = value;
		if (_name.empty()) _name = "-";
	}
	else if (name == PROP_FACILITY)
	{
		std::string facility;
		if (Poco::icompare(value, 4, "LOG_") == 0)
			facility = Poco::toUpper(value.substr(4));
		else if (Poco::icompare(value, 7, "SYSLOG_") == 0)
			facility = Poco::toUpper(value.substr(7));
		else
			facility = Poco::toUpper(value);

		if (facility == "KERN")
			_facility = SYSLOG_KERN;
		else if (facility == "USER")
			_facility = SYSLOG_USER;
		else if (facility == "MAIL")
			_facility = SYSLOG_MAIL;
		else if (facility == "DAEMON")
			_facility = SYSLOG_DAEMON;
		else if (facility == "AUTH")
			_facility = SYSLOG_AUTH;
		else if (facility == "AUTHPRIV")
			_facility = SYSLOG_AUTHPRIV;
		else if (facility == "SYSLOG")
			_facility = SYSLOG_SYSLOG;
		else if (facility == "LPR")
			_facility = SYSLOG_LPR;
		else if (facility == "NEWS")
			_facility = SYSLOG_NEWS;
		else if (facility == "UUCP")
			_facility = SYSLOG_UUCP;
		else if (facility == "CRON")
			_facility = SYSLOG_CRON;
		else if (facility == "FTP")
			_facility = SYSLOG_FTP;
		else if (facility == "NTP")
			_facility = SYSLOG_NTP;
		else if (facility == "LOGAUDIT")
			_facility = SYSLOG_LOGAUDIT;
		else if (facility == "LOGALERT")
			_facility = SYSLOG_LOGALERT;
		else if (facility == "CLOCK")
			_facility = SYSLOG_CLOCK;
		else if (facility == "LOCAL0")
			_facility = SYSLOG_LOCAL0;
		else if (facility == "LOCAL1")
			_facility = SYSLOG_LOCAL1;
		else if (facility == "LOCAL2")
			_facility = SYSLOG_LOCAL2;
		else if (facility == "LOCAL3")
			_facility = SYSLOG_LOCAL3;
		else if (facility == "LOCAL4")
			_facility = SYSLOG_LOCAL4;
		else if (facility == "LOCAL5")
			_facility = SYSLOG_LOCAL5;
		else if (facility == "LOCAL6")
			_facility = SYSLOG_LOCAL6;
		else if (facility == "LOCAL7")
			_facility = SYSLOG_LOCAL7;
	}
	else if (name == PROP_LOGHOST)
	{
		_logHost = value;
	}
	else if (name == PROP_HOST)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_host = value;
		// a name that is given is not replaced by one that is told later
		_pHostNameLookup.reset();
	}
	else if (name == PROP_FORMAT)
	{
		_bsdFormat = (value == "bsd" || value == "rfc3164");
	}
	else if (name == PROP_BUFFER)
	{
		_buffer = Poco::NumberParser::parse(value);
	}
	else if (name == PROP_TRANSPORT)
	{
		Transport transport;
		if (Poco::icompare(value, "udp") == 0)
			transport = TRANSPORT_UDP;
		else if (Poco::icompare(value, "tcp") == 0)
			transport = TRANSPORT_TCP;
		else
			throw Poco::InvalidArgumentException("Not a valid transport", value);

		Poco::FastMutex::ScopedLock lock(_mutex);
		if (transport != _transport)
		{
			// the sockets are those of the transport: the next message opens the channel again
			closeSockets();
			_transport = transport;
		}
	}
	else if (name == PROP_FRAMING)
	{
		Framing framing;
		if (Poco::icompare(value, "newline") == 0)
			framing = FRAMING_NEWLINE;
		else if (Poco::icompare(value, "octet-counting") == 0)
			framing = FRAMING_OCTET_COUNTING;
		else
			throw Poco::InvalidArgumentException("Not a valid framing", value);

		Poco::FastMutex::ScopedLock lock(_mutex);
		_framing = framing;
	}
	else if (name == PROP_TIMEOUT)
	{
		int val = Poco::NumberParser::parse(value);
		if (val <= 0) throw Poco::InvalidArgumentException("Not a valid timeout", value);

		Poco::FastMutex::ScopedLock lock(_mutex);
		_timeout = Poco::Timespan(val*Poco::Timespan::MILLISECONDS);
	}
	else if (name == PROP_RETRY_INTERVAL)
	{
		int val = Poco::NumberParser::parse(value);
		if (val < 0) throw Poco::InvalidArgumentException("Not a valid retry interval", value);

		Poco::FastMutex::ScopedLock lock(_mutex);
		_retryInterval = Poco::Timespan(val*Poco::Timespan::MILLISECONDS);
	}
	else
	{
		Channel::setProperty(name, value);
	}
}


std::string RemoteSyslogChannel::getProperty(const std::string& name) const
{
	if (name == PROP_NAME)
	{
		if (_name != "-")
			return _name;
		else
			return "";
	}
	else if (name == PROP_FACILITY)
	{
		if (_facility == SYSLOG_KERN)
			return "KERN";
		else if (_facility == SYSLOG_USER)
			return "USER";
		else if (_facility == SYSLOG_MAIL)
			return "MAIL";
		else if (_facility == SYSLOG_DAEMON)
			return "DAEMON";
		else if (_facility == SYSLOG_AUTH)
			return "AUTH";
		else if (_facility == SYSLOG_AUTHPRIV)
			return "AUTHPRIV";
		else if (_facility == SYSLOG_SYSLOG)
			return "SYSLOG";
		else if (_facility == SYSLOG_LPR)
			return "LPR";
		else if (_facility == SYSLOG_NEWS)
			return "NEWS";
		else if (_facility == SYSLOG_UUCP)
			return "UUCP";
		else if (_facility == SYSLOG_CRON)
			return "CRON";
		else if (_facility == SYSLOG_FTP)
			return "FTP";
		else if (_facility == SYSLOG_NTP)
			return "NTP";
		else if (_facility == SYSLOG_LOGAUDIT)
			return "LOGAUDIT";
		else if (_facility == SYSLOG_LOGALERT)
			return "LOGALERT";
		else if (_facility == SYSLOG_CLOCK)
			return "CLOCK";
		else if (_facility == SYSLOG_LOCAL0)
			return "LOCAL0";
		else if (_facility == SYSLOG_LOCAL1)
			return "LOCAL1";
		else if (_facility == SYSLOG_LOCAL2)
			return "LOCAL2";
		else if (_facility == SYSLOG_LOCAL3)
			return "LOCAL3";
		else if (_facility == SYSLOG_LOCAL4)
			return "LOCAL4";
		else if (_facility == SYSLOG_LOCAL5)
			return "LOCAL5";
		else if (_facility == SYSLOG_LOCAL6)
			return "LOCAL6";
		else if (_facility == SYSLOG_LOCAL7)
			return "LOCAL7";
		else
			return "";
	}
	else if (name == PROP_LOGHOST)
	{
		return _logHost;
	}
	else if (name == PROP_HOST)
	{
		return _host;
	}
	else if (name == PROP_FORMAT)
	{
		return _bsdFormat ? "rfc3164" : "rfc5424";
	}
	else if (name == PROP_BUFFER)
	{
		return Poco::NumberFormatter::format(_buffer);
	}
	else if (name == PROP_TRANSPORT)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _transport == TRANSPORT_UDP ? "udp" : "tcp";
	}
	else if (name == PROP_FRAMING)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _framing == FRAMING_NEWLINE ? "newline" : "octet-counting";
	}
	else if (name == PROP_TIMEOUT)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return Poco::NumberFormatter::format(_timeout.totalMilliseconds());
	}
	else if (name == PROP_RETRY_INTERVAL)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return Poco::NumberFormatter::format(_retryInterval.totalMilliseconds());
	}
	else
	{
		return Channel::getProperty(name);
	}
}


int RemoteSyslogChannel::getPrio(const Message& msg)
{
	switch (msg.getPriority())
	{
	case Message::PRIO_TRACE:
	case Message::PRIO_DEBUG:
		return SYSLOG_DEBUG;
	case Message::PRIO_INFORMATION:
		return SYSLOG_INFORMATIONAL;
	case Message::PRIO_NOTICE:
		return SYSLOG_NOTICE;
	case Message::PRIO_WARNING:
		return SYSLOG_WARNING;
	case Message::PRIO_ERROR:
		return SYSLOG_ERROR;
	case Message::PRIO_CRITICAL:
		return SYSLOG_CRITICAL;
	case Message::PRIO_FATAL:
		return SYSLOG_ALERT;
	default:
		return 0;
	}
}

const char* RemoteSyslogChannel::facilityToString(const Facility facility)
{
	switch(facility)
	{
	case RemoteSyslogChannel::SYSLOG_KERN:
		return "KERN";
	case RemoteSyslogChannel::SYSLOG_USER:
		return "USER";
	case RemoteSyslogChannel::SYSLOG_MAIL:
		return "MAIL";
	case RemoteSyslogChannel::SYSLOG_DAEMON:
		return "DAEMON";
	case RemoteSyslogChannel::SYSLOG_AUTH:
		return "AUTH";
	case RemoteSyslogChannel::SYSLOG_SYSLOG:
		return "SYSLOG";
	case RemoteSyslogChannel::SYSLOG_LPR:
		return "LPR";
	case RemoteSyslogChannel::SYSLOG_NEWS:
		return "NEWS";
	case RemoteSyslogChannel::SYSLOG_UUCP:
		return "UUCP";
	case RemoteSyslogChannel::SYSLOG_CRON:
		return "CRON";
	case RemoteSyslogChannel::SYSLOG_AUTHPRIV:
		return "AUTHPRIV";
	case RemoteSyslogChannel::SYSLOG_FTP:
		return "FTP";
	case RemoteSyslogChannel::SYSLOG_NTP:
		return "NTP";
	case RemoteSyslogChannel::SYSLOG_LOGAUDIT:
		return "LOGAUDIT";
	case RemoteSyslogChannel::SYSLOG_LOGALERT:
		return "LOGALERT";
	case RemoteSyslogChannel::SYSLOG_CLOCK:
		return "CLOCK";
	case RemoteSyslogChannel::SYSLOG_LOCAL0:
		return "LOCAL0";
	case RemoteSyslogChannel::SYSLOG_LOCAL1:
		return "LOCAL1";
	case RemoteSyslogChannel::SYSLOG_LOCAL2:
		return "LOCAL2";
	case RemoteSyslogChannel::SYSLOG_LOCAL3:
		return "LOCAL3";
	case RemoteSyslogChannel::SYSLOG_LOCAL4:
		return "LOCAL4";
	case RemoteSyslogChannel::SYSLOG_LOCAL5:
		return "LOCAL5";
	case RemoteSyslogChannel::SYSLOG_LOCAL6:
		return "LOCAL6";
	case RemoteSyslogChannel::SYSLOG_LOCAL7:
		return "LOCAL7";
	default:
		return "";
	}
}


void RemoteSyslogChannel::registerChannel()
{
	Poco::LoggingFactory::defaultFactory().registerChannelClass("RemoteSyslogChannel", new Poco::Instantiator<RemoteSyslogChannel, Poco::Channel>);
}


} // namespace Poco::Net
