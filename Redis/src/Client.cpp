//
// Client.cpp
//
// Library: Redis
// Package: Redis
// Module:  Client
//
// Implementation of the Client class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <utility>

#include "Poco/Redis/Client.h"
#include "Poco/Redis/Exception.h"


namespace Poco {
namespace Redis {


Client::Client():
	_address(),
	_socket(),
	_input(nullptr),
	_output(nullptr)
{
}


Client::Client(const std::string& hostAndPort):
	_address(hostAndPort),
	_socket(),
	_input(nullptr),
	_output(nullptr)
{
	connect();
}


Client::Client(const std::string& host, int port):
	_address(host, port),
	_socket(),
	_input(nullptr),
	_output(nullptr)
{
	connect();
}


Client::Client(Net::SocketAddress  addrs):
	_address(std::move(addrs)),
	_socket(),
	_input(nullptr),
	_output(nullptr)
{
	connect();
}


Client::Client(const Net::StreamSocket& socket):
	_address(),
	_socket(),
	_input(nullptr),
	_output(nullptr)
{
	connect(socket);
}


Client::~Client()
{
	delete _input;
	delete _output;
}


void Client::connect()
{
	poco_assert(! _input);
	poco_assert(! _output);

	_socket = Net::StreamSocket(_address);
	_input = new RedisInputStream(_socket);
	_output = new RedisOutputStream(_socket);
}


void Client::connect(const std::string& hostAndPort)
{
	_address = Net::SocketAddress(hostAndPort);
	connect();
}


void Client::connect(const std::string& host, int port)
{
	_address = Net::SocketAddress(host, port);
	connect();
}


void Client::connect(const Net::SocketAddress& addrs)
{
	_address = addrs;
	connect();
}


void Client::connect(const Timespan& timeout)
{
	poco_assert(! _input);
	poco_assert(! _output);

	_socket = Net::StreamSocket();
	_socket.connect(_address, timeout);
	_input = new RedisInputStream(_socket);
	_output = new RedisOutputStream(_socket);
}


void Client::connect(const std::string& hostAndPort, const Timespan& timeout)
{
	_address = Net::SocketAddress(hostAndPort);
	connect(timeout);
}


void Client::connect(const std::string& host, int port, const Timespan& timeout)
{
	_address = Net::SocketAddress(host, port);
	connect(timeout);
}


void Client::connect(const Net::SocketAddress& addrs, const Timespan& timeout)
{
	_address = addrs;
	connect(timeout);
}


void Client::connect(const Poco::Net::StreamSocket& socket)
{
	poco_assert(! _input);
	poco_assert(! _output);

	_address = socket.peerAddress();
	_socket = socket;
	_input = new RedisInputStream(_socket);
	_output = new RedisOutputStream(_socket);
}


void Client::disconnect()
{
	delete _input;
	_input = nullptr;

	delete _output;
	_output = nullptr;

	_socket.close();
}


bool Client::isConnected() const
{
	return _input != nullptr;
}


void Client::writeCommand(const Array& command, bool doFlush)
{
	poco_assert(_output);

	std::string commandStr = command.toString();

	_output->write(commandStr.c_str(), commandStr.length());
	if (doFlush) _output->flush();
}


RedisType::Ptr Client::readReply()
{
	poco_assert(_input);

	int c = _input->get();
	if (c == -1)
	{
		disconnect();
		throw RedisException("Lost connection to Redis server");
	}
	RedisType::Ptr result = RedisType::createRedisType(c);
	if (result.isNull())
	{
		throw RedisException("Invalid Redis type returned");
	}

	result->read(*_input);

	return result;
}


RedisType::Ptr Client::sendCommand(const Array& command)
{
	writeCommand(command, true);
	return readReply();
}


Array Client::sendCommands(const std::vector<Array>& commands)
{
	Array results;

	for (const auto & command : commands)
	{
		writeCommand(command, false);
	}
	_output->flush();

	for (int i = 0; i < commands.size(); ++i)
	{
		results.addRedisType(readReply());
	}

	return results;
}


} } // namespace Poco::Redis
