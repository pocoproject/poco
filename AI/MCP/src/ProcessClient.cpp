//
// ProcessClient.cpp
//
// Library: AI/MCP
// Package: Core
// Module:  ProcessClient
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/ProcessClient.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/Condition.h"
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"
#include <deque>


namespace Poco::AI::MCP {


struct ProcessClient::Reader
	/// What the reader thread shares with the client. Shared ownership: the
	/// thread may outlive the client when a process the server started keeps
	/// its standard output open.
{
	static constexpr std::size_t MAX_QUEUED = 1024;

	Poco::Mutex mutex;
	Poco::Condition ready;
	std::deque<Poco::JSON::Object::Ptr> messages;
	bool ended = false;
};


ProcessClient::ProcessClient(const std::string& command, const std::vector<std::string>& args):
	_pReader(std::make_shared<Reader>()),
	_grace(5, 0),
	_closed(false),
	_exitCode(-1)
{
	_pHandle = std::make_unique<Poco::ProcessHandle>(Poco::Process::launch(command, args, &_inPipe, &_outPipe, nullptr));
	start(DEFAULT_MAX_MESSAGE_SIZE);
}


ProcessClient::ProcessClient(const std::string& command, const std::vector<std::string>& args, const std::string& initialDirectory, const Poco::Process::Env& env, std::size_t maxMessageSize):
	_pReader(std::make_shared<Reader>()),
	_grace(5, 0),
	_closed(false),
	_exitCode(-1)
{
	_pHandle = std::make_unique<Poco::ProcessHandle>(Poco::Process::launch(command, args, initialDirectory, &_inPipe, &_outPipe, nullptr, env));
	start(maxMessageSize);
}


ProcessClient::~ProcessClient()
{
	try
	{
		close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void ProcessClient::start(std::size_t maxMessageSize)
{
	_pOut = std::make_unique<Poco::PipeOutputStream>(_inPipe);
	_thread.startFunc([pReader = _pReader, pipe = _outPipe, maxMessageSize]()
	{
		try
		{
			Poco::PipeInputStream in(pipe);
			for (;;)
			{
				Poco::JSON::Object::Ptr pMessage;
				try
				{
					pMessage = readMessage(in, maxMessageSize);
				}
				catch (const MCPException&)
				{
					// Not a message: passed over.
					continue;
				}
				if (pMessage == nullptr) break;
				Poco::Mutex::ScopedLock lock(pReader->mutex);
				if (pReader->messages.size() >= Reader::MAX_QUEUED)
				{
					pReader->messages.pop_front();
				}
				pReader->messages.push_back(pMessage);
				pReader->ready.broadcast();
			}
		}
		catch (...)
		{
			// The pipe failed: the server is as good as gone.
		}
		Poco::Mutex::ScopedLock lock(pReader->mutex);
		pReader->ended = true;
		pReader->ready.broadcast();
	});
}


void ProcessClient::setGracePeriod(const Poco::Timespan& grace)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_grace = grace;
}


Poco::Timespan ProcessClient::getGracePeriod() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _grace;
}


Poco::Process::PID ProcessClient::id() const
{
	return _pHandle->id();
}


void ProcessClient::send(const Poco::JSON::Object::Ptr& message)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	if (_closed || _pOut == nullptr)
	{
		throw Poco::IOException("The MCP server process is closed");
	}
	try
	{
		writeMessage(*_pOut, message);
	}
	catch (const Poco::Exception& exc)
	{
		throw Poco::IOException("Cannot write to the MCP server process", exc);
	}
}


Poco::JSON::Object::Ptr ProcessClient::exchange(const Poco::JSON::Object::Ptr& request)
{
	const Id id = request->get("id");
	const std::string method = request->getValue<std::string>("method");
	{
		// What is left is stale: replies to requests that timed out, and notifications.
		Poco::Mutex::ScopedLock lock(_pReader->mutex);
		_pReader->messages.clear();
	}
	send(request);

	const Poco::Timespan timeout = getTimeout();
	const Poco::Timestamp start;
	{
		Poco::Mutex::ScopedLock lock(_pReader->mutex);
		for (;;)
		{
			while (!_pReader->messages.empty())
			{
				const Poco::JSON::Object::Ptr pMessage = _pReader->messages.front();
				_pReader->messages.pop_front();
				if (isReply(pMessage, id)) return pMessage;
			}
			if (_pReader->ended)
			{
				throw Poco::IOException("The MCP server process ended before the reply to " + method);
			}
			const Poco::Timespan left = timeout - Poco::Timespan(start.elapsed());
			if (left.totalMilliseconds() <= 0) break;
			(void) _pReader->ready.tryWait(_pReader->mutex, static_cast<long>(left.totalMilliseconds()));
		}
	}

	Poco::JSON::Object::Ptr pParams = new Poco::JSON::Object;
	pParams->set("requestId", id);
	pParams->set("reason", "timeout");
	try
	{
		send(Request::makeNotification("notifications/cancelled", pParams));
	}
	catch (const Poco::Exception&)
	{
	}
	throw Poco::TimeoutException("No reply from the MCP server process to " + method);
}


int ProcessClient::waitForExit(const Poco::Timespan& timeout)
{
	const Poco::Timestamp start;
	for (;;)
	{
		const int rc = _pHandle->tryWait();
		if (rc != -1) return rc;
		if (start.isElapsed(timeout.totalMicroseconds())) return -1;
		Poco::Thread::sleep(10);
	}
}


int ProcessClient::close()
{
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		if (_closed) return _exitCode;
		_closed = true;
		_pOut.reset();
		_inPipe.close(Poco::Pipe::CLOSE_WRITE);
	}
	const Poco::Timespan grace = getGracePeriod();
	int rc = waitForExit(grace);
	if (rc == -1)
	{
		try
		{
			Poco::Process::requestTermination(_pHandle->id());
		}
		catch (const Poco::Exception&)
		{
		}
		rc = waitForExit(grace);
	}
	if (rc == -1)
	{
		try
		{
			Poco::Process::kill(*_pHandle);
		}
		catch (const Poco::Exception&)
		{
		}
		rc = _pHandle->wait();
	}
	// The reader ends when the server's standard output closes; a process the
	// server started may keep it open, and the reader is left to end by itself.
	(void) _thread.tryJoin(static_cast<long>(grace.totalMilliseconds()));
	Poco::FastMutex::ScopedLock lock(_mutex);
	_exitCode = rc;
	return rc;
}


} // namespace Poco::AI::MCP
