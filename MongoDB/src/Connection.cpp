//
// Connection.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  Connection
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/Exception.h"
#include "Poco/Format.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/NumberParser.h"
#include "Poco/URI.h"

using namespace std::string_literals;


namespace Poco {
namespace MongoDB {


Connection::SocketFactory::SocketFactory() = default;


Connection::SocketFactory::~SocketFactory() = default;


Poco::Net::StreamSocket Connection::SocketFactory::createSocket(const std::string& host, int port, Poco::Timespan connectTimeout, bool secure)
{
	if (!secure)
	{
		Poco::Net::SocketAddress addr(host, port);
		Poco::Net::StreamSocket socket;
		if (connectTimeout > 0)
			socket.connect(addr, connectTimeout);
		else
			socket.connect(addr);
		return socket;
	}
	else throw Poco::NotImplementedException("Default SocketFactory implementation does not support SecureStreamSocket");
}


Connection::Connection() = default;


Connection::Connection(const std::string& hostAndPort):
	_address(hostAndPort),
	_socket()
{
	connect();
}


Connection::Connection(const std::string& uri, SocketFactory& socketFactory):
	_address(),
	_socket()
{
	connect(uri, socketFactory);
}


Connection::Connection(const std::string& host, int port):
	_address(host, port),
	_socket()
{
	connect();
}


Connection::Connection(const Poco::Net::SocketAddress& addrs):
	_address(addrs),
	_socket()
{
	connect();
}


Connection::Connection(const Poco::Net::StreamSocket& socket):
	_address(socket.peerAddress()),
	_socket(socket)
{
}


Connection::~Connection()
{
	try
	{
		disconnect();
	}
	catch (...)
	{
	}
}


void Connection::connect()
{
	_socket.connect(_address);
}


void Connection::connect(const std::string& hostAndPort)
{
	_address = Poco::Net::SocketAddress(hostAndPort);
	connect();
}


void Connection::connect(const std::string& host, int port)
{
	_address = Poco::Net::SocketAddress(host, port);
	connect();
}


void Connection::connect(const Poco::Net::SocketAddress& addrs)
{
	_address = addrs;
	connect();
}


void Connection::connect(const Poco::Net::StreamSocket& socket)
{
	_address = socket.peerAddress();
	_socket = socket;
}


void Connection::connect(const std::string& uri, SocketFactory& socketFactory)
{
	Poco::URI theURI(uri);
	if (theURI.getScheme() != "mongodb") throw Poco::UnknownURISchemeException(uri);

	const std::string& userInfo = theURI.getUserInfo();
	const std::string& host = theURI.getHost();
	Poco::UInt16 port = theURI.getPort();
	if (port == 0) port = 27017;

	std::string databaseName = theURI.getPath();
	if (!databaseName.empty() && databaseName[0] == '/') databaseName.erase(0, 1);
	if (databaseName.empty()) databaseName = "admin";

	bool ssl = false;
	Poco::Timespan connectTimeout;
	Poco::Timespan socketTimeout;
	std::string authMechanism = Database::AUTH_SCRAM_SHA1;

	Poco::URI::QueryParameters params = theURI.getQueryParameters();
	for (Poco::URI::QueryParameters::const_iterator it = params.begin(); it != params.end(); ++it)
	{
		if (it->first == "ssl"s)
		{
			ssl = (it->second == "true"s);
		}
		else if (it->first == "connectTimeoutMS"s)
		{
			connectTimeout = static_cast<Poco::Timespan::TimeDiff>(1000)*Poco::NumberParser::parse(it->second);
		}
		else if (it->first == "socketTimeoutMS"s)
		{
			socketTimeout = static_cast<Poco::Timespan::TimeDiff>(1000)*Poco::NumberParser::parse(it->second);
		}
		else if (it->first == "authMechanism"s)
		{
			authMechanism = it->second;
		}
	}

	connect(socketFactory.createSocket(host, port, connectTimeout, ssl));

	if (socketTimeout > 0)
	{
		_socket.setSendTimeout(socketTimeout);
		_socket.setReceiveTimeout(socketTimeout);
	}

	if (!userInfo.empty())
	{
		std::string username;
		std::string password;
		std::string::size_type pos = userInfo.find(':');
		if (pos != std::string::npos)
		{
			username.assign(userInfo, 0, pos++);
			password.assign(userInfo, pos, userInfo.size() - pos);
		}
		else username = userInfo;

		Database database(databaseName);
		if (!database.authenticate(*this, username, password, authMechanism))
			throw Poco::NoPermissionException(Poco::format("Access to MongoDB database %s denied for user %s", databaseName, username));
	}
}


void Connection::disconnect()
{
	_socket.close();
}


void Connection::sendRequest(OpMsgMessage& request, OpMsgMessage& response)
{
	Poco::Net::SocketOutputStream sos(_socket);
	request.send(sos);

	response.clear();
	readResponse(response);
}


void Connection::sendRequest(OpMsgMessage& request)
{
	request.setAcknowledgedRequest(false);
	Poco::Net::SocketOutputStream sos(_socket);
	request.send(sos);
}


void Connection::readResponse(OpMsgMessage& response)
{
	Poco::Net::SocketInputStream sis(_socket);
	response.read(sis);
}



} } // Poco::MongoDB
