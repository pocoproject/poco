//
// SecureRemoteSyslogListener.cpp
//
// Library: NetSSL_OpenSSL
// Package: Logging
// Module:  SecureRemoteSyslogListener
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SecureRemoteSyslogListener.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/LoggingFactory.h"
#include "Poco/Instantiator.h"
#include "Poco/Exception.h"


namespace Poco::Net {


const std::string SecureRemoteSyslogListener::PROP_TLS_PORT("tlsPort");


SecureRemoteSyslogListener::SecureRemoteSyslogListener():
	RemoteSyslogListener(0),
	_tlsPort(SYSLOG_TLS_PORT)
{
}


SecureRemoteSyslogListener::SecureRemoteSyslogListener(Poco::UInt16 tlsPort):
	RemoteSyslogListener(0),
	_tlsPort(tlsPort)
{
}


SecureRemoteSyslogListener::SecureRemoteSyslogListener(Poco::UInt16 tlsPort, Context::Ptr pContext):
	RemoteSyslogListener(0),
	_pContext(pContext),
	_tlsPort(tlsPort)
{
}


SecureRemoteSyslogListener::~SecureRemoteSyslogListener()
{
}


void SecureRemoteSyslogListener::setProperty(const std::string& name, const std::string& value)
{
	if (name == PROP_TLS_PORT)
	{
		int val = Poco::NumberParser::parse(value);
		if (val >= 0 && val < 65536)
			_tlsPort = static_cast<Poco::UInt16>(val);
		else
			throw Poco::InvalidArgumentException("Not a valid port number", value);
	}
	else
	{
		RemoteSyslogListener::setProperty(name, value);
	}
}


std::string SecureRemoteSyslogListener::getProperty(const std::string& name) const
{
	if (name == PROP_TLS_PORT)
		return Poco::NumberFormatter::format(_tlsPort);
	else
		return RemoteSyslogListener::getProperty(name);
}


void SecureRemoteSyslogListener::createServerSockets(std::vector<ServerSocket>& sockets)
{
	RemoteSyslogListener::createServerSockets(sockets);
	if (_tlsPort > 0)
	{
		Context::Ptr pContext = _pContext ? _pContext : SSLManager::instance().defaultServerContext();
		SecureServerSocket socket(pContext);
		socket.bind(SocketAddress(IPAddress(), _tlsPort), true, reusePort());
		socket.listen();
		sockets.push_back(socket);
	}
}


void SecureRemoteSyslogListener::registerChannel()
{
	Poco::LoggingFactory::defaultFactory().registerChannelClass("SecureRemoteSyslogListener", new Poco::Instantiator<SecureRemoteSyslogListener, Poco::Channel>);
}


} // namespace Poco::Net
