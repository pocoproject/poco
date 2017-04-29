//
// Connection.cpp
//
// $Id$
//
// Library: MongoDB
// Package: MongoDB
// Module:  Connection
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketStream.h"
#include "Poco/MongoDB/Connection.h"


namespace Poco {
namespace MongoDB {


Connection::Connection():
	_address(),
	_socket()
{
}


Connection::Connection(const std::string& hostAndPort):
	_address(hostAndPort),
	_socket()
{
	connect();
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


void Connection::disconnect()
{
	_socket.close();
}


void Connection::sendRequest(RequestMessage& request)
{
	Poco::Net::SocketOutputStream sos(_socket);
	request.send(sos);
}


void Connection::sendRequest(RequestMessage& request, ResponseMessage& response)
{
	sendRequest(request);

	Poco::Net::SocketInputStream sis(_socket);
	response.read(sis);
}


} } // Poco::MongoDB
