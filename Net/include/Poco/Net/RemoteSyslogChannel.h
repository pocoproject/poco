//
// RemoteSyslogChannel.h
//
// Library: Net
// Package: Logging
// Module:  RemoteSyslogChannel
//
// Definition of the RemoteSyslogChannel class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_RemoteSyslogChannel_INCLUDED
#define Net_RemoteSyslogChannel_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Channel.h"
#include "Poco/Mutex.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/AutoPtr.h"
#include "Poco/Clock.h"
#include "Poco/Timespan.h"
#include <functional>
#include <memory>


namespace Poco::Net {


class Net_API RemoteSyslogChannel: public Poco::Channel
	/// This Channel implements remote syslog logging over UDP according
	/// to RFC 5424 "The Syslog Protocol"
	/// and RFC 5426 "Transmission of syslog messages over UDP",
	/// and over TCP according to RFC 6587 "Transmission of Syslog
	/// Messages over TCP".
	///
	/// In addition, RemoteSyslogChannel also supports the "old" BSD syslog
	/// protocol, as described in RFC 3164.
	///
	/// RFC 5424 structured data can be passed via the "structured-data"
	/// property of the log Message. The content of the "structured-data"
	/// property must be correct according to RFC 5424.
	///
	/// Example:
	///     msg.set("structured-data", "[exampleSDID@32473 iut=\"3\" eventSource=\"Application\" eventID=\"1011\"]");
	///
	/// With the "transport" property set to "tcp", the messages are sent
	/// over a TCP connection that is made with the first message and
	/// kept. A server that is away does not hold up the application:
	/// an attempt to connect is given up after the time of the "timeout"
	/// property, and the message is dropped, as are the messages that
	/// follow, until the time of the "retryInterval" property has passed
	/// and the next message makes another attempt. log() does not throw
	/// because of that, nor because of a server whose address cannot be
	/// found. A server that is given by name is looked up with every
	/// attempt to connect, and what the name service takes for that is
	/// not within the "timeout" property: where that matters, give the
	/// address of the server.
	///
	/// A connection that the server has closed is made anew with the next
	/// message, and a message that could not be sent over a connection
	/// is sent once more over a new one. Syslog has no acknowledgement:
	/// what was handed to a connection that then broke may be lost.
	///
	/// An attempt to connect that fails is reported through
	/// Poco::ErrorHandler, once, until a connection has been made again:
	/// a server that cannot be reached, or whose certificate does not
	/// verify, is not silent.
{
public:
	using Ptr = Poco::AutoPtr<RemoteSyslogChannel>;

	static const std::string BSD_TIMEFORMAT;
	static const std::string SYSLOG_TIMEFORMAT;

	enum Severity
	{
		SYSLOG_EMERGENCY     = 0, /// Emergency: system is unusable
		SYSLOG_ALERT         = 1, /// Alert: action must be taken immediately
		SYSLOG_CRITICAL      = 2, /// Critical: critical conditions
		SYSLOG_ERROR         = 3, /// Error: error conditions
		SYSLOG_WARNING       = 4, /// Warning: warning conditions
		SYSLOG_NOTICE        = 5, /// Notice: normal but significant condition
		SYSLOG_INFORMATIONAL = 6, /// Informational: informational messages
		SYSLOG_DEBUG         = 7  /// Debug: debug-level messages
	};

	enum Facility
	{
		SYSLOG_KERN     = ( 0<<3), /// kernel messages
		SYSLOG_USER     = ( 1<<3), /// random user-level messages
		SYSLOG_MAIL     = ( 2<<3), /// mail system
		SYSLOG_DAEMON   = ( 3<<3), /// system daemons
		SYSLOG_AUTH     = ( 4<<3), /// security/authorization messages
		SYSLOG_SYSLOG   = ( 5<<3), /// messages generated internally by syslogd
		SYSLOG_LPR      = ( 6<<3), /// line printer subsystem
		SYSLOG_NEWS     = ( 7<<3), /// network news subsystem
		SYSLOG_UUCP     = ( 8<<3), /// UUCP subsystem
		SYSLOG_CRON     = ( 9<<3), /// clock daemon
		SYSLOG_AUTHPRIV = (10<<3), /// security/authorization messages (private)
		SYSLOG_FTP      = (11<<3), /// ftp daemon
		SYSLOG_NTP      = (12<<3), /// ntp subsystem
		SYSLOG_LOGAUDIT = (13<<3), /// log audit
		SYSLOG_LOGALERT = (14<<3), /// log alert
		SYSLOG_CLOCK    = (15<<3), /// clock daemon
		SYSLOG_LOCAL0   = (16<<3), /// reserved for local use
		SYSLOG_LOCAL1   = (17<<3), /// reserved for local use
		SYSLOG_LOCAL2   = (18<<3), /// reserved for local use
		SYSLOG_LOCAL3   = (19<<3), /// reserved for local use
		SYSLOG_LOCAL4   = (20<<3), /// reserved for local use
		SYSLOG_LOCAL5   = (21<<3), /// reserved for local use
		SYSLOG_LOCAL6   = (22<<3), /// reserved for local use
		SYSLOG_LOCAL7   = (23<<3)  /// reserved for local use
	};

	enum
	{
		SYSLOG_PORT = 514
	};

	RemoteSyslogChannel();
		/// Creates a RemoteSyslogChannel.

	RemoteSyslogChannel(const std::string& address, const std::string& name, int facility = SYSLOG_USER, bool bsdFormat = false);
		/// Creates a RemoteSyslogChannel with the given target address, name, and facility.
		/// If bsdFormat is true, messages are formatted according to RFC 3164.

	void open();
		/// Opens the RemoteSyslogChannel. A channel that is not open is
		/// opened by the first message.

	void close();
		/// Closes the RemoteSyslogChannel.

	void log(const Message& msg);
		/// Sends the message's text to the syslog service.

	void setProperty(const std::string& name, const std::string& value);
		/// Sets the property with the given value.
		///
		/// The following properties are supported:
		///     * name:      The name used to identify the source of log messages.
		///     * facility:  The facility added to each log message. See the Facility enumeration for a list of supported values.
		///                  The LOG_ prefix can be omitted and values are case insensitive (e.g. a facility value "mail" is recognized as SYSLOG_MAIL)
		///     * format:    "bsd"/"rfc3164" (RFC 3164 format) or "new"/"rfc5424" (default)
		///     * loghost:   The target IP address or host name where log messages are sent. Optionally, a port number (separated
		///                  by a colon) can also be specified. An IPv6 address that a port number follows is put in
		///                  square brackets.
		///     * host:      (optional) Host name included in syslog messages. If not specified, the host's real domain name or
		///                  IP address will be used. The name service is asked for it when the channel is opened, with the
		///                  first message at the latest, and is waited for no longer than the time of the "timeout" property.
		///                  Until its answer is there, and when it has no name to tell, messages carry the name that the
		///                  system has for the host: the messages of one connection may thus name the host differently
		///                  before and after the answer.
		///     * buffer:    UDP socket send buffer size in bytes. If not specified, the system default is used.
		///     * transport: "udp" (default) or "tcp". A change closes the channel, and the next message opens it again.
		///     * framing:   How messages are told apart on a TCP connection (RFC 6587): "newline" (default), with a line feed
		///                  after every message and the line feeds within a message replaced by spaces, or "octet-counting",
		///                  with every message preceded by its length.
		///     * timeout:   The time in milliseconds allowed for connecting to the server and for sending a message over TCP,
		///                  and the longest that opening the channel waits for the name of the local host (see host).
		///                  Defaults to 2000.
		///     * retryInterval: The time in milliseconds after a failed attempt to connect during which no other is made
		///                  and messages are dropped. Defaults to 5000.

	[[nodiscard]] std::string getProperty(const std::string& name) const;
		/// Returns the value of the property with the given name.

	static void registerChannel();
		/// Registers the channel with the global LoggingFactory.

	[[nodiscard]] static const char* facilityToString(Facility facility);
		/// Returns the string describing the SyslogFacility

	static const std::string PROP_NAME;
	static const std::string PROP_FACILITY;
	static const std::string PROP_FORMAT;
	static const std::string PROP_LOGHOST;
	static const std::string PROP_HOST;
	static const std::string PROP_BUFFER;
	static const std::string PROP_TRANSPORT;
	static const std::string PROP_FRAMING;
	static const std::string PROP_TIMEOUT;
	static const std::string PROP_RETRY_INTERVAL;
	static const std::string STRUCTURED_DATA;

protected:
	~RemoteSyslogChannel();
	[[nodiscard]] static int getPrio(const Message& msg);

	virtual StreamSocket createSocket(const SocketAddress& address, const std::string& hostName, const Poco::Timespan& timeout);
		/// Creates the socket for the "tcp" transport and connects it to
		/// the given address within the given time. hostName is the host
		/// that the "loghost" property names, without a port.
		///
		/// A subclass can override this method to send over another kind
		/// of stream socket. A Poco::Exception or a std::exception thrown
		/// here counts as a server that is away.

	[[nodiscard]] virtual Poco::UInt16 defaultPort() const;
		/// Returns the port that messages are sent to if the "loghost"
		/// property names none.

	using HostNameSource = std::function<std::string()>;

	[[nodiscard]] virtual HostNameSource hostNameSource() const;
		/// Returns the function that tells the name of the local host for
		/// the messages of a channel whose "host" property names none.
		/// The default asks the name service (DNS::thisHost()).
		///
		/// The function is called on a thread of its own, since it takes
		/// the time that the name service takes, and it may still run
		/// when the channel is gone: it must not use the channel. An
		/// exception or an empty name means that the name cannot be told.

private:
	enum Transport
	{
		TRANSPORT_UDP,
		TRANSPORT_TCP
	};

	enum Framing
	{
		FRAMING_NEWLINE,
		FRAMING_OCTET_COUNTING
	};

	struct HostNameLookup;

	void openChannel();
		/// Opens the channel if it is not open. The caller holds the mutex.

	void lookUpHostName();
		/// Has the name of the local host asked for and waits for it for
		/// the time of the "timeout" property at the most. The caller
		/// holds the mutex.

	void takeHostName();
		/// Takes the name of the local host if it has been told since.
		/// The caller holds the mutex.

	void closeSockets();
		/// Closes the channel. The caller holds the mutex.

	void sendOverConnection(const std::string& frame);
		/// Sends the frame over the connection, which is made first if
		/// there is none. Drops the frame if the server is away.

	bool connect();
		/// Makes the connection, unless an attempt failed a short while
		/// ago. Returns true if the connection is there.

	void serverAway(const Poco::Exception& exc);
	void serverAway(const std::exception& exc);
		/// Notes that the server is away, which the exception says why.
		/// It is reported through the ErrorHandler if the server was not
		/// away before.

	void disconnect();
		/// Closes the connection without waiting for the server.

	void sendFrame(const std::string& frame);
		/// Sends all of the frame within the time allowed, or throws.

	[[nodiscard]] bool closedByServer();
		/// Returns true if the server has closed or reset the connection.

	void splitLogHost(std::string& host, std::string& port) const;
		/// Gives the host that the "loghost" property names and the port, which
		/// is empty if the property names none.

	[[nodiscard]] std::string logHostName() const;
		/// Returns the host that the "loghost" property names, without the port.

	[[nodiscard]] SocketAddress logHostAddress() const;
		/// Returns the address that messages are sent to. A host name is
		/// resolved, which takes as long as the name service takes.

	std::string _logHost;
	std::string _name;
	std::string _host;
	int  _facility;
	bool _bsdFormat;
	int _buffer;
	Transport _transport;
	Framing _framing;
	Poco::Timespan _timeout;
	Poco::Timespan _retryInterval;
	DatagramSocket _socket;
	StreamSocket _streamSocket;
	SocketAddress _socketAddress;
	bool _open;
	bool _connected;
	bool _failed;
	Poco::Clock _failedAt;
	std::shared_ptr<HostNameLookup> _pHostNameLookup;
		/// There for as long as the name of the local host is being asked for.
	mutable Poco::FastMutex _mutex;
};


} // namespace Poco::Net


#endif // Net_RemoteSyslogChannel_INCLUDED
