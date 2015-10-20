//
// Connection.cpp
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Connection
//
// Implementation of the Connection class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Redis/Client.h"


namespace Poco {
namespace Redis {


Connection::Connection() : _address(), _socket()
{
}


Connection::Connection(const std::string& hostAndPort) : _address(hostAndPort), _socket()
{
	connect();
}


Connection::Connection(const std::string& host, int port) : _address(host, port), _socket()
{
	connect();
}


Connection::Connection(const Net::SocketAddress& addrs) : _address(addrs), _socket()
{
	connect();
}


Connection::~Connection()
{
}


void Connection::connect()
{
	_socket.connect(_address);
}


void Connection::connect(const std::string& hostAndPort)
{
	_address = Net::SocketAddress(hostAndPort);
	connect();
}


void Connection::connect(const std::string& host, int port)
{
	_address = Net::SocketAddress(host, port);
	connect();
}


void Connection::connect(const Net::SocketAddress& addrs)
{
	_address = addrs;
	connect();
}


void Connection::disconnect()
{
	_socket.close();
}

void Connection::sendCommand(const Array& command)
{
	std::string commandStr = command.toString();
	_socket.write(commandStr.c_str(), commandStr.length());
}


} } // Poco::Redis
