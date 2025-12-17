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


#include "Poco/Redis/Client.h"
#include "Poco/Redis/Exception.h"
#include "Poco/Redis/RedisNotifications.h"


namespace Poco {
namespace Redis {


Client::Client()
{
}


Client::Client(const std::string& hostAndPort):
	_address(hostAndPort)
{
	connect();
}


Client::Client(const std::string& host, int port):
	_address(host, port)
{
	connect();
}


Client::Client(const Net::SocketAddress& addrs):
	_address(addrs)
{
	connect();
}


Client::Client(const Net::StreamSocket& socket)
{
	connect(socket);
}


Client::~Client()
{
#if !POCO_REDIS_HAVE_ATOMIC_SHARED_PTR
	delete _pNC.load();
#endif
}


void Client::connect()
{
	poco_assert(!_pInput);
	poco_assert(!_pOutput);

	try
	{
		_socket = Net::StreamSocket(_address);
		_pInput = std::make_unique<RedisInputStream>(_socket);
		_pOutput = std::make_unique<RedisOutputStream>(_socket);

		// Post connect notification
#if POCO_REDIS_HAVE_ATOMIC_SHARED_PTR
		auto pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisConnectNotification);
#else
		AsyncNotificationCenter* pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisConnectNotification);
#endif
	}
	catch (const Exception& ex)
	{
		// Clean up on failure - smart pointers handle this automatically
		_pInput.reset();
		_pOutput.reset();

		// Post error notification
#if POCO_REDIS_HAVE_ATOMIC_SHARED_PTR
		auto pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisErrorNotification(ex));
#else
		AsyncNotificationCenter* pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisErrorNotification(ex));
#endif
		throw;
	}
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
	poco_assert(!_pInput);
	poco_assert(!_pOutput);

	try
	{
		_socket = Net::StreamSocket();
		_socket.connect(_address, timeout);
		_pInput = std::make_unique<RedisInputStream>(_socket);
		_pOutput = std::make_unique<RedisOutputStream>(_socket);

		// Post connect notification
#if POCO_REDIS_HAVE_ATOMIC_SHARED_PTR
		auto pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisConnectNotification);
#else
		AsyncNotificationCenter* pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisConnectNotification);
#endif
	}
	catch (const Exception& ex)
	{
		_pInput.reset();
		_pOutput.reset();

#if POCO_REDIS_HAVE_ATOMIC_SHARED_PTR
		auto pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisErrorNotification(ex));
#else
		AsyncNotificationCenter* pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisErrorNotification(ex));
#endif
		throw;
	}
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
	poco_assert(!_pInput);
	poco_assert(!_pOutput);

	try
	{
		_address = socket.peerAddress();
		_socket = socket;
		_pInput = std::make_unique<RedisInputStream>(_socket);
		_pOutput = std::make_unique<RedisOutputStream>(_socket);

#if POCO_REDIS_HAVE_ATOMIC_SHARED_PTR
		auto pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisConnectNotification);
#else
		AsyncNotificationCenter* pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisConnectNotification);
#endif
	}
	catch (const Exception& ex)
	{
		_pInput.reset();
		_pOutput.reset();

#if POCO_REDIS_HAVE_ATOMIC_SHARED_PTR
		auto pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisErrorNotification(ex));
#else
		AsyncNotificationCenter* pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisErrorNotification(ex));
#endif
		throw;
	}
}


void Client::disconnect()
{
	if (_pInput || _pOutput)
	{
		_pInput.reset();
		_pOutput.reset();

		_socket.close();

#if POCO_REDIS_HAVE_ATOMIC_SHARED_PTR
		auto pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisDisconnectNotification);
#else
		AsyncNotificationCenter* pNC = _pNC.load();
		if (pNC) pNC->postNotification(new RedisDisconnectNotification);
#endif
	}
}


bool Client::isConnected() const
{
	return _pInput != nullptr;
}


void Client::writeCommand(const Array& command, bool doFlush)
{
	poco_assert(_pOutput);

	std::string commandStr = command.toString();

	_pOutput->write(commandStr.c_str(), commandStr.length());
	if (doFlush) _pOutput->flush();
}


RedisType::Ptr Client::readReply()
{
	poco_assert(_pInput);

	int c = _pInput->get();
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

	result->read(*_pInput);

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

	for (std::vector<Array>::const_iterator it = commands.begin(); it != commands.end(); ++it)
	{
		writeCommand(*it, false);
	}
	_pOutput->flush();

	for (std::size_t i = 0; i < commands.size(); ++i)
	{
		results.addRedisType(readReply());
	}

	return results;
}


Client::NotificationCenterPtr Client::notificationCenter()
{
#if POCO_REDIS_HAVE_ATOMIC_SHARED_PTR
	std::call_once(_ncInitFlag, [this]()
	{
		_pNC.store(std::make_shared<AsyncNotificationCenter>());
	});
	return _pNC.load();
#else
	std::call_once(_ncInitFlag, [this]()
	{
		_pNC.store(new AsyncNotificationCenter);
	});
	return NotificationCenterPtr(_pNC.load(), [](AsyncNotificationCenter*) {});
		// Return shared_ptr with no-op deleter since Client owns the pointer
#endif
}


} } // namespace Poco::Redis
