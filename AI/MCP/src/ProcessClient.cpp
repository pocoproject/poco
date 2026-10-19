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
#include "Poco/PipeStream.h"
#include "Poco/ProcessOptions.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <deque>
#include <utility>
#if defined(POCO_OS_FAMILY_UNIX)
#include <signal.h>
#include <sys/types.h>
#endif


namespace Poco::AI::MCP {


struct ProcessClient::Reader
	/// What the reader thread shares with the client. Shared ownership: the
	/// thread is left to end by itself, and outlives the client, when a
	/// process the server started keeps its standard output open.
{
	Poco::Mutex mutex;
	Poco::Condition ready;
	Id outstanding;
		/// The id of the request in flight; empty when there is none.
	Poco::JSON::Object::Ptr pReply;
		/// Its reply, once it has come.
	bool tooLarge = false;
		/// A line over the size limit came while the request was in flight.
	bool ended = false;
		/// The server's standard output is closed.
};


struct ProcessClient::Writer
	/// What the writer thread shares with the client: the lines to write, in
	/// order. A write that blocks, because the server does not read its
	/// standard input, holds up the requests whose time runs out, but never
	/// close().
{
	static constexpr std::size_t MAX_QUEUED = 16;

	Poco::Mutex mutex;
	Poco::Condition ready;
	std::deque<std::pair<Poco::UInt64, std::string>> lines;
		/// What is to be written, each line with its ticket.
	Poco::UInt64 issued = 0;
		/// The ticket of the last line handed to the thread.
	Poco::UInt64 done = 0;
		/// The ticket of the last line written, or given up on.
	bool busy = false;
		/// A line is being written.
	bool failed = false;
		/// A write failed: the server is gone.
	bool closing = false;
		/// Close the server's standard input once the thread is idle.
	bool ended = false;
		/// The thread is done, and the server's standard input closed.
};


ProcessClient::ProcessClient(const std::string& command, const std::vector<std::string>& args):
	_pReader(std::make_shared<Reader>()),
	_pWriter(std::make_shared<Writer>()),
	_grace(5, 0),
	_closed(false),
	_exitCode(-1)
{
	_pHandle = std::make_unique<Poco::ProcessHandle>(Poco::Process::launch(command, args, &_inPipe, &_outPipe, nullptr, Poco::PROCESS_NEW_GROUP));
	start(DEFAULT_MAX_MESSAGE_SIZE);
}


ProcessClient::ProcessClient(const std::string& command, const std::vector<std::string>& args, const std::string& initialDirectory, const Poco::Process::Env& env, std::size_t maxMessageSize):
	_pReader(std::make_shared<Reader>()),
	_pWriter(std::make_shared<Writer>()),
	_grace(5, 0),
	_closed(false),
	_exitCode(-1)
{
	_pHandle = std::make_unique<Poco::ProcessHandle>(Poco::Process::launch(command, args, initialDirectory, &_inPipe, &_outPipe, nullptr, env, Poco::PROCESS_NEW_GROUP));
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
	// A thread object must not be destroyed while it can still be joined.
	if (_readerThread.joinable()) _readerThread.detach();
	if (_writerThread.joinable()) _writerThread.detach();
}


void ProcessClient::start(std::size_t maxMessageSize)
{
	try
	{
		_readerThread = std::thread(&ProcessClient::read, _pReader, _outPipe, maxMessageSize);
		_writerThread = std::thread(&ProcessClient::write, _pWriter, _inPipe);
	}
	catch (...)
	{
		// The server must not outlive a client that could not be made.
		kill();
		(void) _pHandle->wait();
		if (_readerThread.joinable()) _readerThread.detach();
		throw;
	}
}


void ProcessClient::read(const std::shared_ptr<Reader>& pReader, Poco::Pipe pipe, std::size_t maxMessageSize)
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
			catch (const MessageTooLargeException&)
			{
				// It may be the reply, which is then lost.
				Poco::Mutex::ScopedLock lock(pReader->mutex);
				if (!pReader->outstanding.isEmpty())
				{
					pReader->tooLarge = true;
					pReader->ready.broadcast();
				}
				continue;
			}
			catch (const MCPException&)
			{
				// Not a message: passed over.
				continue;
			}
			if (pMessage == nullptr) break;
			// Only the reply to the request in flight is kept; the client
			// delivers nothing else.
			Poco::Mutex::ScopedLock lock(pReader->mutex);
			if (!pReader->outstanding.isEmpty() && isReply(pMessage, pReader->outstanding))
			{
				pReader->pReply = pMessage;
				pReader->ready.broadcast();
			}
		}
	}
	catch (...)
	{
		// The pipe failed: the server is as good as gone.
	}
	Poco::Mutex::ScopedLock lock(pReader->mutex);
	pReader->ended = true;
	pReader->ready.broadcast();
}


void ProcessClient::write(const std::shared_ptr<Writer>& pWriter, Poco::Pipe pipe)
{
	for (;;)
	{
		std::string line;
		Poco::UInt64 ticket = 0;
		{
			Poco::Mutex::ScopedLock lock(pWriter->mutex);
			while (pWriter->lines.empty() && !pWriter->closing)
			{
				pWriter->ready.wait(pWriter->mutex);
			}
			if (pWriter->lines.empty()) break;
			ticket = pWriter->lines.front().first;
			line.swap(pWriter->lines.front().second);
			pWriter->lines.pop_front();
			pWriter->busy = true;
		}
		bool written = true;
		try
		{
			std::size_t offset = 0;
			while (offset < line.size())
			{
				const int chunk = static_cast<int>(std::min<std::size_t>(line.size() - offset, 65536));
				const int n = pipe.writeBytes(line.data() + offset, chunk);
				if (n <= 0)
				{
					written = false;
					break;
				}
				offset += static_cast<std::size_t>(n);
			}
		}
		catch (...)
		{
			written = false;
		}
		Poco::Mutex::ScopedLock lock(pWriter->mutex);
		pWriter->busy = false;
		pWriter->done = ticket;
		if (!written)
		{
			pWriter->failed = true;
			pWriter->lines.clear();
		}
		pWriter->ready.broadcast();
		if (!written) break;
	}
	try
	{
		// End of file for the server, or the end of a pipe that failed.
		pipe.close(Poco::Pipe::CLOSE_WRITE);
	}
	catch (...)
	{
	}
	Poco::Mutex::ScopedLock lock(pWriter->mutex);
	pWriter->ended = true;
	pWriter->ready.broadcast();
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


void ProcessClient::send(const std::string& line, const Poco::Timestamp& start, const Poco::Timespan& timeout, bool wait)
{
	Poco::Mutex::ScopedLock lock(_pWriter->mutex);
	if (_pWriter->failed)
	{
		throw Poco::IOException("Cannot write to the MCP server process");
	}
	if (_pWriter->closing)
	{
		throw Poco::IOException("The MCP server process is closed");
	}
	if (_pWriter->lines.size() >= Writer::MAX_QUEUED)
	{
		throw Poco::TimeoutException("The MCP server process does not read its standard input");
	}
	const Poco::UInt64 ticket = ++_pWriter->issued;
	_pWriter->lines.emplace_back(ticket, line);
	_pWriter->ready.broadcast();
	if (!wait) return;

	while (_pWriter->done < ticket)
	{
		if (_pWriter->failed)
		{
			throw Poco::IOException("Cannot write to the MCP server process");
		}
		const Poco::Timespan left = timeout - Poco::Timespan(start.elapsed());
		if (left.totalMilliseconds() <= 0)
		{
			// A line that is not being written yet is taken back.
			const auto it = std::find_if(_pWriter->lines.begin(), _pWriter->lines.end(), [ticket](const std::pair<Poco::UInt64, std::string>& entry)
			{
				return entry.first == ticket;
			});
			if (it != _pWriter->lines.end()) _pWriter->lines.erase(it);
			throw Poco::TimeoutException("The MCP server process does not read its standard input");
		}
		(void) _pWriter->ready.tryWait(_pWriter->mutex, static_cast<long>(left.totalMilliseconds()));
	}
	if (_pWriter->failed && _pWriter->done == ticket)
	{
		throw Poco::IOException("Cannot write to the MCP server process");
	}
}


Poco::JSON::Object::Ptr ProcessClient::exchange(const Poco::JSON::Object::Ptr& request)
{
	const Id id = request->get("id");
	const std::string method = request->getValue<std::string>("method");
	const Poco::Timespan timeout = getTimeout();
	const Poco::Timestamp start;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		if (_closed)
		{
			throw Poco::IOException("The MCP server process is closed");
		}
	}
	{
		Poco::Mutex::ScopedLock lock(_pReader->mutex);
		_pReader->outstanding = id;
		_pReader->pReply = nullptr;
		_pReader->tooLarge = false;
	}
	try
	{
		send(serialize(request) + '\n', start, timeout, true);
	}
	catch (...)
	{
		Poco::Mutex::ScopedLock lock(_pReader->mutex);
		_pReader->outstanding = Id();
		throw;
	}

	{
		Poco::Mutex::ScopedLock lock(_pReader->mutex);
		for (;;)
		{
			if (_pReader->pReply != nullptr)
			{
				Poco::JSON::Object::Ptr pReply;
				std::swap(pReply, _pReader->pReply);
				_pReader->outstanding = Id();
				return pReply;
			}
			if (_pReader->tooLarge)
			{
				_pReader->outstanding = Id();
				throw Poco::ProtocolException("A message of the MCP server process exceeds the size limit, during " + method);
			}
			if (_pReader->ended)
			{
				_pReader->outstanding = Id();
				throw Poco::IOException("The MCP server process ended before the reply to " + method);
			}
			const Poco::Timespan left = timeout - Poco::Timespan(start.elapsed());
			if (left.totalMilliseconds() <= 0) break;
			(void) _pReader->ready.tryWait(_pReader->mutex, static_cast<long>(left.totalMilliseconds()));
		}
		_pReader->outstanding = Id();
	}

	Poco::JSON::Object::Ptr pParams = new Poco::JSON::Object;
	pParams->set("requestId", id);
	pParams->set("reason", "timeout");
	try
	{
		send(serialize(Request::makeNotification("notifications/cancelled", pParams)) + '\n', start, timeout, false);
	}
	catch (const Poco::Exception&)
	{
	}
	throw Poco::TimeoutException("No reply from the MCP server process to " + method);
}


void ProcessClient::requestTermination()
{
#if defined(POCO_OS_FAMILY_UNIX)
	// The server leads a process group of its own: the request goes to every
	// process of it, as Ctrl+C in a terminal does. While the server has not
	// been waited for, its group exists, and its id names no other.
	::killpg(static_cast<pid_t>(_pHandle->id()), SIGINT);
#else
	try
	{
		Poco::Process::requestTermination(_pHandle->id(), Poco::PROCESS_NEW_GROUP);
	}
	catch (const Poco::Exception&)
	{
	}
#endif
}


void ProcessClient::kill()
{
#if defined(POCO_OS_FAMILY_UNIX)
	::killpg(static_cast<pid_t>(_pHandle->id()), SIGKILL);
#else
	try
	{
		// By its ID: killing through the handle closes it, and the handle is
		// still needed for the wait. While the handle is open, the ID cannot
		// name another process.
		Poco::Process::kill(_pHandle->id());
	}
	catch (const Poco::Exception&)
	{
	}
#endif
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


void ProcessClient::finish(std::thread& thread, bool ended)
{
	if (!thread.joinable()) return;
	// A thread that has ended its work returns at once; one that has not is
	// left to end by itself, with what it shares with the client.
	if (ended) thread.join();
	else thread.detach();
}


int ProcessClient::close()
{
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		if (_closed) return _exitCode;
		_closed = true;
	}
	const Poco::Timespan grace = getGracePeriod();

	// The writer closes the server's standard input as soon as it is idle;
	// one that is stuck in a write, because the server does not read, gets
	// no end of file to it, and the server is asked to end at once.
	bool inputClosed = false;
	{
		Poco::Mutex::ScopedLock lock(_pWriter->mutex);
		_pWriter->closing = true;
		_pWriter->lines.clear();
		_pWriter->ready.broadcast();
		const Poco::Timestamp start;
		while (!_pWriter->ended && !_pWriter->busy)
		{
			const Poco::Timespan left = grace - Poco::Timespan(start.elapsed());
			if (left.totalMilliseconds() <= 0) break;
			(void) _pWriter->ready.tryWait(_pWriter->mutex, static_cast<long>(left.totalMilliseconds()));
		}
		inputClosed = _pWriter->ended;
	}
	int rc = inputClosed ? waitForExit(grace) : -1;
	if (rc == -1)
	{
		requestTermination();
		rc = waitForExit(grace);
	}
	if (rc == -1)
	{
		kill();
		rc = _pHandle->wait();
	}

	// The reader ends when the server's standard output closes, and the
	// writer once its last write fails or completes; a process the server
	// started may keep either pipe open.
	const auto awaitEnd = [&grace](Poco::Mutex& mutex, Poco::Condition& ready, const bool& ended)
	{
		Poco::Mutex::ScopedLock lock(mutex);
		const Poco::Timestamp start;
		while (!ended)
		{
			const Poco::Timespan left = grace - Poco::Timespan(start.elapsed());
			if (left.totalMilliseconds() <= 0) break;
			(void) ready.tryWait(mutex, static_cast<long>(left.totalMilliseconds()));
		}
		return ended;
	};
	finish(_readerThread, awaitEnd(_pReader->mutex, _pReader->ready, _pReader->ended));
	finish(_writerThread, awaitEnd(_pWriter->mutex, _pWriter->ready, _pWriter->ended));

	Poco::FastMutex::ScopedLock lock(_mutex);
	_exitCode = rc;
	return rc;
}


} // namespace Poco::AI::MCP
