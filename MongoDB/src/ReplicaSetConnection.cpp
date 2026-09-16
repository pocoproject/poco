//
// ReplicaSetConnection.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReplicaSetConnection
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ReplicaSetConnection.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/MongoDB/ReplicaSet.h"
#include "Poco/MongoDB/TopologyDescription.h"
#include "Poco/Bugcheck.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <exception>
#include <string>
#include <vector>

using namespace std::literals;


namespace Poco {
namespace MongoDB {


namespace {


enum class ErrorCode
{
	HostUnreachable = 6,
	HostNotFound = 7,
	NetworkTimeout = 89,
	ShutdownInProgress = 91,
	PrimarySteppedDown = 189,
	SocketException = 9001,
	LegacyNotPrimary = 10058,
	NotWritablePrimary = 10107,
	NotPrimaryNoSecondaryOk = 13435,
	NotPrimaryOrSecondary = 13436,
	InterruptedAtShutdown = 11600,
	InterruptedDueToReplStateChange = 11602
};


// Floor for small topologies. With serverReconnectRetries at 0, a connect failure
// can still end the call after the second attempt.
constexpr std::size_t lowExecuteRetryThreshold { 5 };


enum class ResendPolicy
{
	Never,         // a re-send could skip or lose cursor data
	IfNotExecuted, // a re-send could apply a write twice
	Always         // reads without side effects
};


enum class ServerError
{
	None,
	NotExecuted,
	MaybeExecuted
};


bool hasWriteStage(const Document& body)
{
	const Array::Ptr pipeline = body.get<Array::Ptr>("pipeline"s, nullptr);
	if (pipeline.isNull())
		return false;

	// $out and $merge are only valid as the last top-level stage.
	for (std::size_t i = 0; i < pipeline->size(); ++i)
	{
		const Document::Ptr stage = pipeline->get<Document::Ptr>(i, nullptr);
		if (!stage.isNull() && (stage->exists("$out"s) || stage->exists("$merge"s)))
			return true;
	}
	return false;
}


ResendPolicy resendPolicy(const OpMsgMessage& request)
{
	// The server executes the first body element. commandName() names it for a
	// request built with setCommandName() or setCursor(); it is empty for a
	// hand-built body, whose first element name is read instead.
	std::string command = request.commandName();
	if (command.empty())
	{
		std::vector<std::string> names;
		request.body().elementNames(names);
		if (names.empty())
			return ResendPolicy::IfNotExecuted;
		command = names.front();
	}

	// String literals rather than OpMsgMessage::CMD_*: not all of them are
	// public or present in every supported release.
	if (command == "getMore"s || command == "killCursors"s)
		return ResendPolicy::Never;

	static const std::vector<std::string> readCommands {
		"find"s, "count"s, "distinct"s, "listCollections"s, "listIndexes"s, "listDatabases"s,
		"explain"s, "hello"s, "isMaster"s, "ismaster"s, "ping"s, "buildInfo"s
	};
	if (std::find(readCommands.begin(), readCommands.end(), command) != readCommands.end())
		return ResendPolicy::Always;

	if (command == "aggregate"s)
		return hasWriteStage(request.body()) ? ResendPolicy::IfNotExecuted : ResendPolicy::Always;

	return ResendPolicy::IfNotExecuted;
}


ServerError serverError(const Document& body)
{
	if (!body.exists("code"s))
		return ServerError::None;

	Poco::Int64 code = 0;
	try
	{
		// The server sends code as Int32; getInteger() also accepts Int64 and Double.
		code = body.getInteger("code"s);
	}
	catch (const Poco::BadCastException&)
	{
		return ServerError::None;
	}

	switch (static_cast<ErrorCode>(code))
	{
	case ErrorCode::NotWritablePrimary:
	case ErrorCode::NotPrimaryNoSecondaryOk:
	case ErrorCode::NotPrimaryOrSecondary:
	case ErrorCode::LegacyNotPrimary:
		return ServerError::NotExecuted;
	case ErrorCode::InterruptedAtShutdown:
	case ErrorCode::InterruptedDueToReplStateChange:
	case ErrorCode::PrimarySteppedDown:
	case ErrorCode::ShutdownInProgress:
	case ErrorCode::HostNotFound:
	case ErrorCode::HostUnreachable:
	case ErrorCode::NetworkTimeout:
	case ErrorCode::SocketException:
		return ServerError::MaybeExecuted;
	default:
		return ServerError::None;
	}
}


bool isRetriableException(const std::exception& e)
{
	// IOException covers Net::NetException and ProtocolException.
	return dynamic_cast<const Poco::IOException*>(&e) != nullptr ||
		dynamic_cast<const Poco::TimeoutException*>(&e) != nullptr;
}


} // namespace


ReplicaSetConnection::ReplicaSetConnection(ReplicaSet& replicaSet, const ReadPreference& readPref):
	_replicaSet(replicaSet),
	_readPreference(readPref)
{
}


ReplicaSetConnection::ReplicaSetConnection(ReplicaSet& replicaSet, const ReadPreference& readPref,
	Poco::Timespan connectTimeout, Poco::Timespan socketTimeout):
	_replicaSet(replicaSet),
	_readPreference(readPref),
	_connectTimeout(connectTimeout),
	_socketTimeout(socketTimeout),
	_hasTimeouts(true)
{
	// 0 means "not specified" here: without a socket timeout a reply that never
	// arrives would block the caller forever and defeat the attempt bound.
	if (_socketTimeout == 0)
		_socketTimeout = Poco::Timespan(static_cast<long>(replicaSet.configuration().socketTimeoutSeconds), 0);
}


ReplicaSetConnection::~ReplicaSetConnection() = default;


void ReplicaSetConnection::sendRequest(OpMsgMessage& request, OpMsgMessage& response)
{
	executeWithRetry(request, response);
}


void ReplicaSetConnection::sendRequest(OpMsgMessage& request)
{
	// One-way requests are not retried
	ensureConnection();
	_connection->sendRequest(request);
}


void ReplicaSetConnection::readResponse(OpMsgMessage& response)
{
	// A continuation of a reply can only come from the connection that carries it,
	// so a new connection must not be opened here.
	if (!isConnected())
		throw Poco::IOException("No MongoDB connection to read the response from"s);
	_connection->readResponse(response);
}


Net::SocketAddress ReplicaSetConnection::address() const
{
	if (_connection.isNull())
	{
		throw Poco::NullPointerException("Not connected to any server: address not available."s);
	}
	return _connection->address();
}


Connection& ReplicaSetConnection::connection()
{
	if (_connection.isNull())
	{
		throw Poco::NullPointerException("Not connected to any server: connection not available."s);
	}
	return *_connection;
}


void ReplicaSetConnection::reconnect()
{
	_connection = nullptr;
	ensureConnection();
}


bool ReplicaSetConnection::isConnected() const noexcept
{
	return !_connection.isNull() && _connection->isConnected();
}


bool ReplicaSetConnection::matchesReadPreference() const noexcept
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


Connection::Ptr ReplicaSetConnection::selectConnection(bool waitForServer)
{
	Connection::Ptr connection = _hasTimeouts
		? _replicaSet.getConnection(_readPreference, _connectTimeout, _socketTimeout)
		: _replicaSet.getConnection(_readPreference);

	if (connection.isNull() && waitForServer)
	{
		// ReplicaSet coordinates the sleep and refresh cycle across threads.
		connection = _hasTimeouts
			? _replicaSet.waitForServerAvailability(_readPreference, _connectTimeout, _socketTimeout)
			: _replicaSet.waitForServerAvailability(_readPreference);
	}
	return connection;
}


void ReplicaSetConnection::ensureConnection()
{
	// Connection closes itself when a response cannot be read. The server is not
	// marked as failed for that: the response may only be one Poco cannot parse.
	if (!_connection.isNull() && !_connection->isConnected())
		_connection = nullptr;

	if (_connection.isNull())
	{
		_connection = selectConnection(false);
		if (_connection.isNull())
		{
			throw Poco::IOException("No suitable server found in replica set"s);
		}
	}
}


void ReplicaSetConnection::dropConnectionAndRefresh()
{
	_connection = nullptr;
	try
	{
		_replicaSet.refreshTopology();
	}
	catch (...)
	{
		// For example an exception from a topology change observer; the monitor thread ignores it too.
	}
}


void ReplicaSetConnection::executeWithRetry(OpMsgMessage& request, OpMsgMessage& response)
{
	// As in Connection::sendRequest(): a request that fails before it is sent leaves no previous reply.
	response.clear();

	const ResendPolicy policy = resendPolicy(request);
	const std::size_t maxAttempts = std::max(_replicaSet.topology().serverCount(), lowExecuteRetryThreshold);

	std::exception_ptr lastException;

	for (std::size_t attempt = 0; attempt < maxAttempts; ++attempt)
	{
		if (_connection.isNull() || !_connection->isConnected())
		{
			_connection = nullptr;
			try
			{
				_connection = selectConnection(true);
			}
			catch (const std::exception& e)
			{
				if (!isRetriableException(e))
					throw;
				// No refresh: ReplicaSet has already marked the server Unknown.
				lastException = std::current_exception();
				continue;
			}
			if (_connection.isNull())
			{
				if (lastException)
				{
					try
					{
						std::rethrow_exception(lastException);
					}
					catch (const Poco::Exception& e)
					{
						throw Poco::IOException("No suitable server found in replica set"s, e);
					}
				}
				throw Poco::IOException("No suitable server found in replica set"s);
			}
		}

		try
		{
			_connection->sendRequest(request, response);
		}
		catch (const std::exception& e)
		{
			// Connection closes itself on errors that leave the stream out of sync;
			// it stays open after NotImplementedException.
			if (!_connection->isConnected())
				_connection = nullptr;
			if (!isRetriableException(e))
				throw;

			lastException = std::current_exception();
			dropConnectionAndRefresh();
			// The request may have been executed before the reply was lost.
			if (policy == ResendPolicy::Always)
				continue;
			throw;
		}

		if (response.responseOk())
			return;

		const ServerError kind = serverError(response.body());
		if (kind == ServerError::None)
			return;

		// Drop the connection so that a re-send goes through server selection again.
		dropConnectionAndRefresh();
		lastException = std::make_exception_ptr(
			Poco::IOException("MongoDB server error: "s + response.body().toString()));

		if ((kind == ServerError::NotExecuted && policy != ResendPolicy::Never) ||
			(kind == ServerError::MaybeExecuted && policy == ResendPolicy::Always))
			continue;
		break;
	}

	if (lastException)
		std::rethrow_exception(lastException);
	// Every exit of the loop other than a return records a failure, so this is unreachable.
	poco_bugcheck_msg("executeWithRetry ended without a recorded failure");
}


} } // namespace Poco::MongoDB
