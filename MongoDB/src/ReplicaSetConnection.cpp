//
// ReplicaSetConnection.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReplicaSetConnection
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ReplicaSetConnection.h"
#include "Poco/MongoDB/TopologyDescription.h"
#include "Poco/Net/NetException.h"
#include "Poco/Exception.h"
#include <set>

using namespace std::string_literals;


namespace Poco {
namespace MongoDB {


// MongoDB error codes that indicate retriable errors
enum class ErrorCode
{
	NotMaster = 10107,
	NotMasterNoSlaveOk = 13435,
	NotMasterOrSecondary = 13436,
	InterruptedAtShutdown = 11600,
	InterruptedDueToReplStateChange = 11602,
	PrimarySteppedDown = 189,
	ShutdownInProgress = 91,
	HostNotFound = 7,
	HostUnreachable = 6,
	NetworkTimeout = 89,
	SocketException = 9001
};


ReplicaSetConnection::ReplicaSetConnection(ReplicaSet& replicaSet, const ReadPreference& readPref):
	_replicaSet(replicaSet),
	_readPreference(readPref)
{
}


ReplicaSetConnection::~ReplicaSetConnection() = default;


void ReplicaSetConnection::sendRequest(OpMsgMessage& request, OpMsgMessage& response)
{
	executeWithRetry([&]() {
		ensureConnection();
		_connection->sendRequest(request, response);

		// Check if response contains a retriable error
		if (!response.responseOk() && isRetriableMongoDBError(response))
		{
			markServerFailed();
			throw Poco::IOException("MongoDB retriable error: "s + response.body().toString());
		}
	});
}


void ReplicaSetConnection::sendRequest(OpMsgMessage& request)
{
	// One-way requests are not retried
	ensureConnection();
	_connection->sendRequest(request);
}


void ReplicaSetConnection::readResponse(OpMsgMessage& response)
{
	ensureConnection();
	_connection->readResponse(response);
}


Net::SocketAddress ReplicaSetConnection::address() const
{
	if (_connection.isNull())
	{
		throw Poco::NullPointerException("Not connected to any server: address not available.");
	}
	return _connection->address();
}


Connection& ReplicaSetConnection::connection()
{
	if (_connection.isNull())
	{
		throw Poco::NullPointerException("Not connected to any server: connection not available.");
	}
	return *_connection;
}


void ReplicaSetConnection::reconnect()
{
	_connection = nullptr;
	ensureConnection();
}


bool ReplicaSetConnection::isConnected() const
{
	return !_connection.isNull();
}


bool ReplicaSetConnection::matchesReadPreference() const
{
	if (!isConnected())
	{
		return false;
	}

	// Get the current topology
	TopologyDescription topology = _replicaSet.topology();

	// Get the server description for the currently connected server
	ServerDescription server = topology.getServer(_connection->address());

	// Check if the server is Unknown or has an error
	if (server.type() == ServerDescription::Unknown || server.hasError())
	{
		return false;
	}

	// Use ReadPreference::selectServers to check if our current server
	// would be selected with the current read preference
	std::vector<ServerDescription> eligibleServers = _readPreference.selectServers(topology);

	// Check if our current server is in the list of eligible servers
	for (const auto& eligible : eligibleServers)
	{
		if (eligible.address() == _connection->address())
		{
			return true;
		}
	}

	return false;
}


void ReplicaSetConnection::ensureConnection()
{
	if (_connection.isNull())
	{
		_connection = _replicaSet.getConnection(_readPreference);
		if (_connection.isNull())
		{
			throw Poco::IOException("No suitable server found in replica set");
		}
	}
}


void ReplicaSetConnection::executeWithRetry(std::function<void()> operation)
{
	std::exception_ptr lastException;
	std::set<Net::SocketAddress> triedServers;

	// Retry with different servers until we've tried all available servers
	TopologyDescription topology = _replicaSet.topology();
	const std::size_t maxAttempts = topology.serverCount();
	std::size_t attempt = 0;

	while (attempt < maxAttempts)
	{
		try
		{
			ensureConnection();
			triedServers.insert(_connection->address());
			operation();
			return;  // Success
		}
		catch (const Poco::Net::NetException& e)
		{
			if (!isRetriableError(e))
			{
				throw;  // Non-retriable network error
			}
			lastException = std::current_exception();
		}
		catch (const Poco::TimeoutException& e)
		{
			if (!isRetriableError(e))
			{
				throw;  // Non-retriable timeout
			}
			lastException = std::current_exception();
		}
		catch (const Poco::IOException& e)
		{
			if (!isRetriableError(e))
			{
				throw;  // Non-retriable I/O error
			}
			lastException = std::current_exception();
		}
		catch (...)
		{
			throw;  // Non-retriable error
		}

		// Mark current server as failed
		markServerFailed();

		// Try to get a new connection to a different server
		_connection = nullptr;

		// Get new connection, avoiding servers we've already tried
		bool foundNewServer = false;
		for (std::size_t i = 0; i < 10 && !foundNewServer; ++i)  // Try up to 10 times
		{
			Connection::Ptr newConn = _replicaSet.getConnection(_readPreference);
			if (newConn.isNull())
			{
				break;  // No servers available
			}

			Net::SocketAddress addr = newConn->address();
			if (triedServers.find(addr) == triedServers.end())
			{
				_connection = newConn;
				foundNewServer = true;
			}
		}

		if (!foundNewServer)
		{
			// No new servers to try
			break;
		}

		++attempt;
	}

	// All retries failed, rethrow the last exception
	if (lastException)
	{
		std::rethrow_exception(lastException);
	}
	else
	{
		throw Poco::IOException("Failed to execute operation on any replica set member");
	}
}


bool ReplicaSetConnection::isRetriableError(const std::exception& e)
{
	// Network exceptions are generally retriable
	if (dynamic_cast<const Poco::Net::NetException*>(&e))
	{
		return true;
	}

	// Timeout exceptions are retriable
	if (dynamic_cast<const Poco::TimeoutException*>(&e))
	{
		return true;
	}

	// I/O exceptions might be retriable
	const Poco::IOException* ioEx = dynamic_cast<const Poco::IOException*>(&e);
	if (ioEx)
	{
		const auto& msg = ioEx->message();
		// Check for specific retriable error messages
		if (msg.find("not master"s) != std::string::npos ||
			msg.find("NotMaster"s) != std::string::npos ||
			msg.find("Connection"s) != std::string::npos ||
			msg.find("connection"s) != std::string::npos)
		{
			return true;
		}
	}

	return false;
}


bool ReplicaSetConnection::isRetriableMongoDBError(const OpMsgMessage& response)
{
	if (response.responseOk())
	{
		return false;
	}

	const Document& body = response.body();

	// Check for error code
	if (body.exists("code"s))
	{
		ErrorCode code = static_cast<ErrorCode>(body.get<int>("code"s));

		switch (code)
		{
		case ErrorCode::NotMaster:
		case ErrorCode::NotMasterNoSlaveOk:
		case ErrorCode::NotMasterOrSecondary:
		case ErrorCode::InterruptedAtShutdown:
		case ErrorCode::InterruptedDueToReplStateChange:
		case ErrorCode::PrimarySteppedDown:
		case ErrorCode::ShutdownInProgress:
		case ErrorCode::HostNotFound:
		case ErrorCode::HostUnreachable:
		case ErrorCode::NetworkTimeout:
		case ErrorCode::SocketException:
			return true;
		default:
			return false;
		}
	}

	// Check for error message patterns
	if (body.exists("errmsg"s))
	{
		const auto& errmsg = body.get<std::string>("errmsg"s);
		if (errmsg.find("not master"s) != std::string::npos ||
			errmsg.find("NotMaster"s) != std::string::npos)
		{
			return true;
		}
	}

	return false;
}


void ReplicaSetConnection::markServerFailed()
{
	if (!_connection.isNull())
	{
		// Refresh topology to detect changes
		_replicaSet.refreshTopology();
	}
}


} } // namespace Poco::MongoDB
