//
// MongoDBTestServer.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MongoDBTestServer.h"
#include "Poco/ByteOrder.h"
#include "Poco/MemoryStream.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/MessageHeader.h"
#include "Poco/MongoDB/MongoDB.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Timespan.h"
#include <cstring>


using Poco::MongoDB::Array;
using Poco::MongoDB::Document;
using Poco::MongoDB::OpMsgMessage;
using Poco::Net::PollSet;
using Poco::Net::Socket;
using Poco::Net::SocketAddress;
using Poco::Net::SocketOutputStream;
using Poco::Net::StreamSocket;

using namespace std::string_literals;


namespace
{
	const Poco::Timespan POLL_TIMEOUT(0, 20000);
	const Poco::Timespan CLIENT_SOCKET_TIMEOUT(2, 0);
	const std::chrono::seconds PARTIAL_MESSAGE_DEADLINE(2);
	const Poco::Int32 MIN_MESSAGE_LENGTH = Poco::MongoDB::MessageHeader::MSG_HEADER_SIZE;
	const Poco::Int32 MAX_MESSAGE_LENGTH = Poco::MongoDB::MAX_MESSAGE_SIZE_BYTES;
}


MongoDBTestServer::MongoDBTestServer():
	_listener(SocketAddress("127.0.0.1"s, 0)),
	_port(_listener.address().port()),
	_setName("rs0"s)
{
	_hosts.push_back(hostAndPort());
	_me = hostAndPort();
	_pollSet.add(_listener, PollSet::POLL_READ);
	_thread = std::thread([this]() { run(); });
}


MongoDBTestServer::~MongoDBTestServer()
{
	_stop = true;
	_thread.join();
}


SocketAddress MongoDBTestServer::address() const
{
	return SocketAddress("127.0.0.1"s, _port);
}


std::string MongoDBTestServer::hostAndPort() const
{
	return "127.0.0.1:"s + std::to_string(_port);
}


void MongoDBTestServer::setPrimary(bool primary)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_primary = primary;
}


void MongoDBTestServer::setSetName(const std::string& setName)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_setName = setName;
}


void MongoDBTestServer::setHosts(const std::vector<std::string>& hosts)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_hosts = hosts;
}


void MongoDBTestServer::setMe(const std::string& me)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_me = me;
}


void MongoDBTestServer::addBehaviour(const std::string& command, const Behaviour& behaviour, int times)
{
	std::lock_guard<std::mutex> lock(_mutex);
	auto& queue = _scripts[command];
	for (int i = 0; i < times; ++i)
		queue.push_back(behaviour);
}


void MongoDBTestServer::setDefaultBehaviour(const std::string& command, const Behaviour& behaviour)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_defaults[command] = behaviour;
}


int MongoDBTestServer::commandCount(const std::string& command) const
{
	std::lock_guard<std::mutex> lock(_mutex);
	auto it = _commandCounts.find(command);
	return it != _commandCounts.end() ? it->second : 0;
}


int MongoDBTestServer::totalConnections() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _totalConnections;
}


void MongoDBTestServer::stopListening()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_stopListeningRequested = true;
	_controlDone.wait(lock, [this]() { return !_stopListeningRequested; });
}


void MongoDBTestServer::closeConnections()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_closeConnectionsRequested = true;
	_controlDone.wait(lock, [this]() { return !_closeConnectionsRequested; });
}


Document::Ptr MongoDBTestServer::errorReply(Poco::Int32 code, const std::string& codeName, const std::string& errmsg)
{
	Document::Ptr doc = new Document();
	doc->add("ok"s, 0.0);
	doc->add("errmsg"s, errmsg);
	doc->add("code"s, code);
	doc->add("codeName"s, codeName);
	return doc;
}


Document::Ptr MongoDBTestServer::cursorReply(const std::string& ns, Poco::Int64 cursorId, const std::vector<Document::Ptr>& batch, bool firstBatch)
{
	Document::Ptr doc = new Document();
	Document& cursor = doc->addNewDocument("cursor"s);
	Array& docs = cursor.addNewArray(firstBatch ? "firstBatch"s : "nextBatch"s);
	for (const auto& d: batch)
		docs.add(d);
	cursor.add("id"s, cursorId);
	cursor.add("ns"s, ns);
	doc->add("ok"s, 1.0);
	return doc;
}


void MongoDBTestServer::run()
{
	// An exception escaping a std::thread terminates the process, so each step is guarded.
	while (!_stop)
	{
		try
		{
			pollOnce();
		}
		catch (...)
		{
		}
		try
		{
			closeStalledClients();
		}
		catch (...)
		{
		}
		processControlRequests();
	}

	try
	{
		while (!_clients.empty())
			closeClient(_clients.begin()->first);
		if (_listening)
		{
			_listening = false;
			_pollSet.clear();
			_listener.close();
		}
	}
	catch (...)
	{
	}
}


void MongoDBTestServer::pollOnce()
{
	if (!_listening && _clients.empty())
	{
		// Some PollSet implementations return at once for an empty set.
		std::this_thread::sleep_for(std::chrono::microseconds(POLL_TIMEOUT.totalMicroseconds()));
		return;
	}

	const PollSet::SocketModeMap ready = _pollSet.poll(POLL_TIMEOUT);
	for (const auto& entry: ready)
	{
		if (_listening && entry.first == _listener)
		{
			acceptClient();
			continue;
		}
		auto it = _clients.find(entry.first);
		if (it == _clients.end())
			continue;
		bool keep = false;
		try
		{
			keep = handleClient(it->second);
		}
		catch (...)
		{
		}
		if (!keep)
			closeClient(entry.first);
	}
}


void MongoDBTestServer::acceptClient()
{
	try
	{
		StreamSocket socket = _listener.acceptConnection();
		// Safety nets only; stalled messages are bounded by PARTIAL_MESSAGE_DEADLINE.
		socket.setReceiveTimeout(CLIENT_SOCKET_TIMEOUT);
		socket.setSendTimeout(CLIENT_SOCKET_TIMEOUT);
		_clients.emplace(socket, Client{socket, std::string(), std::nullopt});
		_pollSet.add(socket, PollSet::POLL_READ);
		std::lock_guard<std::mutex> lock(_mutex);
		++_totalConnections;
	}
	catch (...)
	{
	}
}


bool MongoDBTestServer::handleClient(Client& client)
{
	// Reads only what is available, so a partial message never blocks the server thread.
	const int available = client.socket.available();
	if (available <= 0)
		return false;
	const std::size_t oldSize = client.buffer.size();
	client.buffer.resize(oldSize + static_cast<std::size_t>(available));
	const int received = client.socket.receiveBytes(&client.buffer[oldSize], available);
	if (received <= 0)
		return false;
	client.buffer.resize(oldSize + static_cast<std::size_t>(received));

	bool consumed = false;
	while (client.buffer.size() >= sizeof(Poco::Int32))
	{
		Poco::Int32 length = 0;
		std::memcpy(&length, client.buffer.data(), sizeof(length));
		length = Poco::ByteOrder::fromLittleEndian(length);
		if (length < MIN_MESSAGE_LENGTH || length > MAX_MESSAGE_LENGTH)
			return false;
		if (client.buffer.size() < static_cast<std::size_t>(length))
			break;

		OpMsgMessage request;
		{
			Poco::MemoryInputStream in(client.buffer.data(), length);
			request.read(in);
		}
		client.buffer.erase(0, static_cast<std::size_t>(length));
		consumed = true;
		if (!dispatch(client, request))
			return false;
	}

	if (client.buffer.empty())
		client.partialSince.reset();
	else if (consumed || !client.partialSince.has_value())
		client.partialSince = std::chrono::steady_clock::now();
	return true;
}


bool MongoDBTestServer::dispatch(Client& client, const OpMsgMessage& request)
{
	std::vector<std::string> names;
	request.body().elementNames(names);
	const std::string command = names.empty() ? std::string() : names.front();

	const Behaviour behaviour = nextBehaviour(command);
	switch (behaviour.action)
	{
	case Action::Reply:
		{
			OpMsgMessage response;
			if (behaviour.reply.isNull())
				response.body().add("ok"s, 1.0);
			else
				response.body() = *behaviour.reply;
			SocketOutputStream out(client.socket);
			response.send(out);
			out.flush();
			// SocketOutputStream reports write failures through the stream state only.
			return out.good();
		}
	case Action::RawReply:
		{
			SocketOutputStream out(client.socket);
			out.write(behaviour.raw.data(), static_cast<std::streamsize>(behaviour.raw.size()));
			out.flush();
			return out.good();
		}
	case Action::NoReply:
		return true;
	case Action::CloseConnection:
		return false;
	}
	return true;
}


void MongoDBTestServer::closeClient(const Socket& socket)
{
	auto it = _clients.find(socket);
	if (it == _clients.end())
		return;
	try
	{
		_pollSet.remove(it->second.socket);
	}
	catch (...)
	{
	}
	try
	{
		it->second.socket.close();
	}
	catch (...)
	{
	}
	_clients.erase(it);
}


void MongoDBTestServer::closeStalledClients()
{
	const auto now = std::chrono::steady_clock::now();
	std::vector<Socket> stalled;
	for (const auto& entry: _clients)
	{
		const auto& since = entry.second.partialSince;
		if (since.has_value() && now - *since > PARTIAL_MESSAGE_DEADLINE)
			stalled.push_back(entry.first);
	}
	for (const auto& socket: stalled)
		closeClient(socket);
}


void MongoDBTestServer::processControlRequests()
{
	std::unique_lock<std::mutex> lock(_mutex);
	const bool stopListening = _stopListeningRequested;
	const bool closeConnections = _closeConnectionsRequested;
	if (!stopListening && !closeConnections)
		return;
	lock.unlock();

	if (stopListening && _listening)
	{
		_listening = false;
		try
		{
			_pollSet.remove(_listener);
		}
		catch (...)
		{
		}
		try
		{
			_listener.close();
		}
		catch (...)
		{
		}
	}
	if (closeConnections)
	{
		while (!_clients.empty())
			closeClient(_clients.begin()->first);
	}

	// Marked done even when an action failed, so that the caller never waits forever.
	lock.lock();
	if (stopListening)
		_stopListeningRequested = false;
	if (closeConnections)
		_closeConnectionsRequested = false;
	_controlDone.notify_all();
}


MongoDBTestServer::Behaviour MongoDBTestServer::nextBehaviour(const std::string& command)
{
	std::lock_guard<std::mutex> lock(_mutex);
	++_commandCounts[command];

	auto script = _scripts.find(command);
	if (script != _scripts.end() && !script->second.empty())
	{
		Behaviour behaviour = script->second.front();
		script->second.pop_front();
		return behaviour;
	}
	auto def = _defaults.find(command);
	if (def != _defaults.end())
		return def->second;

	Behaviour behaviour;
	if (command == "hello"s || command == "isMaster"s)
		behaviour.reply = helloReply();
	return behaviour;
}


Document::Ptr MongoDBTestServer::helloReply() const
{
	// Called with _mutex held.
	Document::Ptr doc = new Document();
	doc->add("ok"s, 1.0);
	doc->add("isWritablePrimary"s, _primary);
	doc->add("secondary"s, !_primary);
	doc->add("setName"s, _setName);
	Array& hosts = doc->addNewArray("hosts"s);
	for (const auto& host: _hosts)
		hosts.add(host);
	doc->add("me"s, _me);
	doc->add("minWireVersion"s, static_cast<Poco::Int32>(0));
	doc->add("maxWireVersion"s, static_cast<Poco::Int32>(21));
	return doc;
}
