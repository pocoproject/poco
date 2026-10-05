//
// RemoteSyslogListener.h
//
// Library: Net
// Package: Logging
// Module:  RemoteSyslogListener
//
// Definition of the RemoteSyslogListener class.
//
// Copyright (c) 2007-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_RemoteSyslogListener_INCLUDED
#define Net_RemoteSyslogListener_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/ThreadPool.h"
#include "Poco/SplitterChannel.h"
#include "Poco/NotificationQueue.h"
#include <cstddef>
#include <vector>


namespace Poco::Net {


class RemoteUDPListener;
class RemoteTCPListener;
class SyslogParser;


class Net_API RemoteSyslogListener: public Poco::SplitterChannel
	/// RemoteSyslogListener implements listening for syslog messages
	/// sent over UDP, according to RFC 5424 "The Syslog Protocol"
	/// and RFC 5426 "Transmission of syslog messages over UDP",
	/// and over TCP, according to RFC 6587 "Transmission of Syslog
	/// Messages over TCP".
	///
	/// In addition, RemoteSyslogListener also supports the "old" BSD syslog
	/// protocol, as described in RFC 3164.
	///
	/// TCP is off unless the "tcpPort" property is set or a listening
	/// socket is given with addServerSocket(). A SecureServerSocket given
	/// that way makes the listener take syslog over TLS (RFC 5425).
	///
	/// On a connection both framings of RFC 6587 are taken and told
	/// apart message by message: a message that begins with a digit is
	/// preceded by its length in octets (octet counting), a message that
	/// begins with '<' ends at the next line feed, where a carriage return
	/// before the line feed is dropped. A last message that no line feed
	/// follows is taken when the sender closes the connection.
	/// A message longer than the "maxMessageSize" property is cut off
	/// at that size and the rest of it is skipped. What is not a message
	/// is skipped up to the next line feed. Neither closes the connection.
	///
	/// The RemoteSyslogListener is a subclass of Poco::SplitterChannel.
	/// Every received log message is sent to the channels registered
	/// with addChannel() or the "channel" property.
	///
	/// Poco::Message objects created by RemoteSyslogListener will have
	/// the following named parameters:
	///   - addr: IP address of the host/interface sending the message.
	///   - host: host name; only for "new" syslog messages.
	///   - app:  application name; only for "new" syslog messages.
	///   - structured-data: RFC 5424 structured data, or empty if not present.
{
public:
	RemoteSyslogListener();
		/// Creates the RemoteSyslogListener.

	RemoteSyslogListener(Poco::UInt16 port);
		/// Creates the RemoteSyslogListener, listening on the given port number.

	RemoteSyslogListener(Poco::UInt16 port, int threads);
		/// Creates the RemoteSyslogListener, listening on the given port number
		/// and using the number of threads for message processing.

	RemoteSyslogListener(Poco::UInt16 port, bool reusePort, int threads);
		/// Creates the RemoteSyslogListener, listening on the given port number
		/// and using the number of threads for message processing.
		///
		/// If reusePort is true, the underlying UDP socket will bind
		/// with the reusePort flag set.

	void setProperty(const std::string& name, const std::string& value);
		/// Sets the property with the given value.
		///
		/// The following properties are supported:
		///     * port: The UDP port number where to listen for UDP packets
		///       containing syslog messages. If 0 is specified, does not
		///       listen for UDP messages.
		///     * tcpPort: The TCP port number where to listen for
		///       connections of senders. If 0 is specified, which is
		///       the default, does not listen for TCP connections.
		///     * reusePort: If set to true, allows multiple instances
		///       binding to the same port number.
		///     * threads: The number of parser threads processing
		///       received syslog messages. Defaults to 1. A maximum
		///       of 15 threads is supported.
		///     * buffer: The UDP socket receive buffer size in bytes. If not
		///       specified, the system default is used.
		///     * maxMessageSize: The largest size, in octets, of a message
		///       taken from a connection. Defaults to 65536, which is
		///       also the most that is taken from a UDP packet.
		///
		/// These properties are read by open(): a change takes effect
		/// when the listener is opened the next time.

	[[nodiscard]] std::string getProperty(const std::string& name) const;
		/// Returns the value of the property with the given name.

	void addServerSocket(const ServerSocket& socket);
		/// Adds a listening socket for connections of senders, in addition
		/// to the one of the "tcpPort" property. The socket must be bound
		/// and listening. With a SecureServerSocket the listener takes
		/// syslog over TLS (RFC 5425).
		///
		/// Must be called before open(). close() lets go of the socket:
		/// to listen on it after another open(), add it again.

	void open();
		/// Starts the listener. Does nothing if it is open already.

	void close();
		/// Stops the listener and closes the connections of its senders.

	void processMessage(const std::string& messageText);
		/// Parses a single line of text containing a syslog message
		/// and sends it down the filter chain.

	void enqueueMessage(const std::string& messageText, const Poco::Net::SocketAddress& senderAddress);
		/// Enqueues a single line of text containing a syslog message
		/// for asynchronous processing by a parser thread.

	static void registerChannel();
		/// Registers the channel with the global LoggingFactory.

	static const std::string PROP_PORT;
	static const std::string PROP_TCP_PORT;
	static const std::string PROP_REUSE_PORT;
	static const std::string PROP_THREADS;
	static const std::string PROP_BUFFER;
	static const std::string PROP_MAX_MESSAGE_SIZE;

	static const std::string LOG_PROP_FACILITY;
	static const std::string LOG_PROP_APP;
	static const std::string LOG_PROP_HOST;
	static const std::string LOG_PROP_STRUCTURED_DATA;

protected:
	~RemoteSyslogListener();
		/// Destroys the RemoteSyslogListener, after stopping it.

	virtual void createServerSockets(std::vector<ServerSocket>& sockets);
		/// Called by open() to create the listening sockets that the
		/// properties ask for and to add them to sockets. Creates the
		/// socket of the "tcpPort" property.
		///
		/// A subclass that listens on a port of its own overrides this
		/// method, calls the one of its base class and adds its socket.

private:
	void stop();
		/// Stops and joins all threads.

	RemoteUDPListener*        _pListener;
	RemoteTCPListener*        _pTCPListener;
	SyslogParser*             _pParser;
	Poco::ThreadPool          _threadPool;
	Poco::NotificationQueue   _queue;
	std::vector<ServerSocket> _serverSockets;
	Poco::UInt16              _port;
	Poco::UInt16              _tcpPort;
	bool                      _reusePort;
	int                       _threads;
	int                       _buffer;
	std::size_t               _maxMessageSize;
};


} // namespace Poco::Net


#endif // Net_RemoteSyslogListener_INCLUDED
