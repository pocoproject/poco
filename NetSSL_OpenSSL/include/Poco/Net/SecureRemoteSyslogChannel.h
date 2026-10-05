//
// SecureRemoteSyslogChannel.h
//
// Library: NetSSL_OpenSSL
// Package: Logging
// Module:  SecureRemoteSyslogChannel
//
// Definition of the SecureRemoteSyslogChannel class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_SecureRemoteSyslogChannel_INCLUDED
#define NetSSL_SecureRemoteSyslogChannel_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/RemoteSyslogChannel.h"
#include "Poco/Net/Context.h"


namespace Poco::Net {


class NetSSL_API SecureRemoteSyslogChannel: public RemoteSyslogChannel
	/// This Channel implements remote syslog logging over TLS, according
	/// to RFC 5425 "Transport Layer Security (TLS) Transport Mapping
	/// for Syslog".
	///
	/// Messages are sent over a TLS connection to the host of the
	/// "loghost" property, and to port 6514 if the property names
	/// no port. As RFC 5425 has it, every message is preceded by its
	/// length. For a server that expects a line feed after every
	/// message instead, set the "framing" property to "newline".
	///
	/// The TLS context is the one given to the constructor. Without one,
	/// the default client context of the SSLManager is taken when the
	/// connection is made, so that a channel created by the logging
	/// configuration is set up by the configuration of the SSLManager.
	/// The certificate of the server is verified as the context has it,
	/// against the host name of the "loghost" property.
	///
	/// The connection is handled as RemoteSyslogChannel handles a TCP
	/// connection: the time of the "timeout" property is allowed for
	/// connecting and again for the TLS handshake, and a server that is
	/// away, or one that the TLS connection cannot be made with, makes
	/// the channel drop messages until the "retryInterval" property
	/// allows the next attempt.
{
public:
	using Ptr = Poco::AutoPtr<SecureRemoteSyslogChannel>;

	enum
	{
		SYSLOG_TLS_PORT = 6514
	};

	SecureRemoteSyslogChannel();
		/// Creates a SecureRemoteSyslogChannel.

	explicit SecureRemoteSyslogChannel(Context::Ptr pContext);
		/// Creates a SecureRemoteSyslogChannel with the given TLS context,
		/// which must be one for client use.

	SecureRemoteSyslogChannel(const std::string& address, const std::string& name, int facility = SYSLOG_USER, bool bsdFormat = false);
		/// Creates a SecureRemoteSyslogChannel with the given target address, name, and facility.
		/// If bsdFormat is true, messages are formatted according to RFC 3164.

	SecureRemoteSyslogChannel(const std::string& address, const std::string& name, int facility, bool bsdFormat, Context::Ptr pContext);
		/// Creates a SecureRemoteSyslogChannel with the given target address, name, facility
		/// and TLS context, which must be one for client use.
		/// If bsdFormat is true, messages are formatted according to RFC 3164.

	void setProperty(const std::string& name, const std::string& value) override;
		/// Sets the property with the given value.
		///
		/// The properties are those of RemoteSyslogChannel, with these differences:
		///     * transport: Always "tls". Another value is not accepted.
		///     * framing:   Defaults to "octet-counting".
		///     * loghost:   The port defaults to 6514.

	[[nodiscard]] std::string getProperty(const std::string& name) const override;
		/// Returns the value of the property with the given name.

	static void registerChannel();
		/// Registers the channel with the global LoggingFactory.

protected:
	~SecureRemoteSyslogChannel() override;

	StreamSocket createSocket(const SocketAddress& address, const std::string& hostName, const Poco::Timespan& timeout) override;
		/// Connects to the given address, makes the TLS handshake and
		/// verifies the certificate of the server against hostName.

	[[nodiscard]] Poco::UInt16 defaultPort() const override;

private:
	Context::Ptr _pContext;
};


} // namespace Poco::Net


#endif // NetSSL_SecureRemoteSyslogChannel_INCLUDED
