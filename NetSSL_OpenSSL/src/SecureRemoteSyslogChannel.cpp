//
// SecureRemoteSyslogChannel.cpp
//
// Library: NetSSL_OpenSSL
// Package: Logging
// Module:  SecureRemoteSyslogChannel
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SecureRemoteSyslogChannel.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/LoggingFactory.h"
#include "Poco/Instantiator.h"
#include "Poco/String.h"
#include "Poco/Exception.h"


namespace Poco::Net {


SecureRemoteSyslogChannel::SecureRemoteSyslogChannel():
	SecureRemoteSyslogChannel(Context::Ptr())
{
}


SecureRemoteSyslogChannel::SecureRemoteSyslogChannel(Context::Ptr pContext):
	_pContext(pContext)
{
	RemoteSyslogChannel::setProperty(PROP_TRANSPORT, "tcp");
	RemoteSyslogChannel::setProperty(PROP_FRAMING, "octet-counting");
}


SecureRemoteSyslogChannel::SecureRemoteSyslogChannel(const std::string& address, const std::string& name, int facility, bool bsdFormat):
	SecureRemoteSyslogChannel(address, name, facility, bsdFormat, Context::Ptr())
{
}


SecureRemoteSyslogChannel::SecureRemoteSyslogChannel(const std::string& address, const std::string& name, int facility, bool bsdFormat, Context::Ptr pContext):
	RemoteSyslogChannel(address, name, facility, bsdFormat),
	_pContext(pContext)
{
	RemoteSyslogChannel::setProperty(PROP_TRANSPORT, "tcp");
	RemoteSyslogChannel::setProperty(PROP_FRAMING, "octet-counting");
}


SecureRemoteSyslogChannel::~SecureRemoteSyslogChannel()
{
}


void SecureRemoteSyslogChannel::setProperty(const std::string& name, const std::string& value)
{
	if (name == PROP_TRANSPORT)
	{
		if (Poco::icompare(value, "tls") != 0)
			throw Poco::InvalidArgumentException("Not a valid transport", value);
	}
	else
	{
		RemoteSyslogChannel::setProperty(name, value);
	}
}


std::string SecureRemoteSyslogChannel::getProperty(const std::string& name) const
{
	if (name == PROP_TRANSPORT)
		return "tls";
	else
		return RemoteSyslogChannel::getProperty(name);
}


StreamSocket SecureRemoteSyslogChannel::createSocket(const SocketAddress& address, const std::string& hostName, const Poco::Timespan& timeout)
{
	Context::Ptr pContext = _pContext ? _pContext : SSLManager::instance().defaultClientContext();

	// The timeouts are set before TLS is put on the connection,
	// so that they hold for the handshake as well.
	StreamSocket plainSocket;
	plainSocket.connect(address, timeout);
	plainSocket.setReceiveTimeout(timeout);
	plainSocket.setSendTimeout(timeout);
	SecureStreamSocket socket(SecureStreamSocket::attach(plainSocket, hostName, pContext));

	// No message is handed to the connection before the handshake is
	// complete and the certificate of the server is verified.
	socket.setBlocking(false);
	const int rc = socket.completeHandshake();
	socket.setBlocking(true);
	if (rc < 0) throw Poco::TimeoutException("TLS handshake not completed in time", address.toString());
	socket.verifyPeerCertificate();
	return socket;
}


Poco::UInt16 SecureRemoteSyslogChannel::defaultPort() const
{
	return SYSLOG_TLS_PORT;
}


void SecureRemoteSyslogChannel::registerChannel()
{
	Poco::LoggingFactory::defaultFactory().registerChannelClass("SecureRemoteSyslogChannel", new Poco::Instantiator<SecureRemoteSyslogChannel, Poco::Channel>);
}


} // namespace Poco::Net
