//
// SecureRemoteSyslogListener.h
//
// Library: NetSSL_OpenSSL
// Package: Logging
// Module:  SecureRemoteSyslogListener
//
// Definition of the SecureRemoteSyslogListener class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_SecureRemoteSyslogListener_INCLUDED
#define NetSSL_SecureRemoteSyslogListener_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/RemoteSyslogListener.h"
#include "Poco/Net/Context.h"


namespace Poco::Net {


class NetSSL_API SecureRemoteSyslogListener: public RemoteSyslogListener
	/// SecureRemoteSyslogListener implements listening for syslog messages
	/// sent over TLS, according to RFC 5425 "Transport Layer Security (TLS)
	/// Transport Mapping for Syslog".
	///
	/// The listener takes TLS connections on the port of the "tlsPort"
	/// property, which is 6514 unless set otherwise. Listening for UDP
	/// packets and for TCP connections, which RemoteSyslogListener does,
	/// is off unless the "port" and "tcpPort" properties are set.
	///
	/// The TLS context is the one given to the constructor. Without one,
	/// the default server context of the SSLManager is taken when the
	/// listener is opened, so that a listener created by the logging
	/// configuration is set up by the configuration of the SSLManager.
	///
	/// Messages are taken from a TLS connection as RemoteSyslogListener
	/// takes them from a TCP connection, in both framings of RFC 6587:
	/// the octet counting that RFC 5425 asks for, and messages that
	/// end with a line feed.
{
public:
	enum
	{
		SYSLOG_TLS_PORT = 6514
	};

	SecureRemoteSyslogListener();
		/// Creates the SecureRemoteSyslogListener.

	explicit SecureRemoteSyslogListener(Poco::UInt16 tlsPort);
		/// Creates the SecureRemoteSyslogListener, listening for
		/// TLS connections on the given port number.

	SecureRemoteSyslogListener(Poco::UInt16 tlsPort, Context::Ptr pContext);
		/// Creates the SecureRemoteSyslogListener, listening for
		/// TLS connections on the given port number, with the
		/// given TLS context, which must be one for server use.

	void setProperty(const std::string& name, const std::string& value) override;
		/// Sets the property with the given value.
		///
		/// In addition to the properties of RemoteSyslogListener,
		/// the following property is supported:
		///     * tlsPort: The TCP port number where to listen for TLS
		///       connections of senders. Defaults to 6514. If 0 is
		///       specified, does not listen for TLS connections.

	[[nodiscard]] std::string getProperty(const std::string& name) const override;
		/// Returns the value of the property with the given name.

	static void registerChannel();
		/// Registers the channel with the global LoggingFactory.

	static const std::string PROP_TLS_PORT;

protected:
	~SecureRemoteSyslogListener() override;
		/// Destroys the SecureRemoteSyslogListener.

	void createServerSockets(std::vector<ServerSocket>& sockets) override;
		/// Adds the socket for the TLS connections.

private:
	Context::Ptr _pContext;
	Poco::UInt16 _tlsPort;
};


} // namespace Poco::Net


#endif // NetSSL_SecureRemoteSyslogListener_INCLUDED
