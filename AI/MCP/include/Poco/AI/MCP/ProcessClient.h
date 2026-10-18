//
// ProcessClient.h
//
// Library: AI/MCP
// Package: Core
// Module:  ProcessClient
//
// Definition of the ProcessClient class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_ProcessClient_INCLUDED
#define AIMCP_ProcessClient_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Client.h"
#include "Poco/Process.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/Timespan.h"
#include <memory>
#include <string>
#include <vector>


namespace Poco::AI::MCP {


class AIMCP_API ProcessClient: public Client
	/// The stdio transport: launches an MCP server process and talks to it
	/// over its standard input and output, one message per line. The server's
	/// standard error is the application's.
	///
	/// A thread reads what the server writes, so a request times out after
	/// the client's timeout; the client then sends notifications/cancelled
	/// for it and throws Poco::TimeoutException.
	///
	/// close(), which the destructor calls, ends the server as the stdio
	/// transport describes it: it closes the server's standard input, waits
	/// for the server to exit, requests its termination after the grace
	/// period, and kills it after another.
{
public:
	ProcessClient(const std::string& command, const std::vector<std::string>& args);
		/// Launches the server.
		///
		/// Throws Poco::SystemException if the process cannot be launched.

	ProcessClient(const std::string& command, const std::vector<std::string>& args, const std::string& initialDirectory, const Poco::Process::Env& env, std::size_t maxMessageSize = DEFAULT_MAX_MESSAGE_SIZE);
		/// Launches the server in initialDirectory (if not empty) with the
		/// given environment variables. A line longer than maxMessageSize
		/// bytes is passed over.
		///
		/// Throws Poco::SystemException if the process cannot be launched.

	~ProcessClient() override;
		/// Closes the client.

	void setGracePeriod(const Poco::Timespan& grace);
		/// Sets how long close() waits at each step, 5 seconds by default.

	[[nodiscard]] Poco::Timespan getGracePeriod() const;
		/// Returns the grace period.

	[[nodiscard]] Poco::Process::PID id() const;
		/// Returns the process id of the server.

	int close();
		/// Ends the server, as described above, and returns its exit code.
		/// Does nothing but return the exit code if the client is closed.

protected:
	Poco::JSON::Object::Ptr exchange(const Poco::JSON::Object::Ptr& request) override;
		/// Throws Poco::IOException when the server is gone or ends before
		/// the reply, and Poco::TimeoutException when the reply takes longer
		/// than the client's timeout.

private:
	struct Reader;

	void start(std::size_t maxMessageSize);
	void send(const Poco::JSON::Object::Ptr& message);
	int waitForExit(const Poco::Timespan& timeout);

	ProcessClient(const ProcessClient&) = delete;
	ProcessClient& operator = (const ProcessClient&) = delete;

	std::shared_ptr<Reader> _pReader;
	Poco::Pipe _inPipe;
	Poco::Pipe _outPipe;
	std::unique_ptr<Poco::ProcessHandle> _pHandle;
	std::unique_ptr<Poco::PipeOutputStream> _pOut;
	Poco::Thread _thread;
	mutable Poco::FastMutex _mutex;
	Poco::Timespan _grace;
	bool _closed;
	int _exitCode;
};


} // namespace Poco::AI::MCP


#endif // AIMCP_ProcessClient_INCLUDED
