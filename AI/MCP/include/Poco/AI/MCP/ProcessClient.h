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
#include "Poco/Mutex.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include <memory>
#include <string>
#include <thread>
#include <vector>


namespace Poco::AI::MCP {


class AIMCP_API ProcessClient: public Client
	/// The stdio transport: launches an MCP server process and talks to it
	/// over its standard input and output, one message per line. The server's
	/// standard error is the application's.
	///
	/// The server runs in a process group of its own (PROCESS_NEW_GROUP), so
	/// that what the terminal or the console sends to the group of the
	/// application, such as Ctrl+C, does not reach it, and so that close()
	/// can end it the way console programs understand.
	///
	/// Two threads talk to the server: one writes the requests, one reads
	/// what the server writes. A request times out after the client's
	/// timeout, counted from the moment it is handed to the writer: a server
	/// that stops reading its standard input times a request out too, and
	/// does not hold up close(). After a timeout the client sends
	/// notifications/cancelled for the request and throws
	/// Poco::TimeoutException. Only the reply to the request in flight is
	/// kept; notifications and late replies are passed over.
	///
	/// close(), which the destructor calls, ends the server as the stdio
	/// transport describes it: it closes the server's standard input, waits
	/// for the server to exit, requests its termination after the grace
	/// period, and kills it after another. On POSIX platforms the request
	/// (SIGINT) and the kill (SIGKILL) go to the server's whole process
	/// group, so that the processes it started (npx, uvx, a shell) end with
	/// it. On Windows the request is a CTRL_BREAK_EVENT to the group, which
	/// console programs take as a request to end, and the kill ends the
	/// server process only. A process that keeps the server's standard
	/// output open after that (one that left the group) does not hold up
	/// close(): the reader thread is left to end when that process does.
{
public:
	ProcessClient(const std::string& command, const std::vector<std::string>& args);
		/// Launches the server.
		///
		/// On Windows, throws Poco::SystemException if the process cannot be
		/// launched. On POSIX platforms a command that cannot be executed
		/// does not throw here: the process is created, ends at once with
		/// exit code PROCESS_EXIT_EXEC_FAILED (72), the first request throws
		/// Poco::IOException, and close() returns 72.

	ProcessClient(const std::string& command, const std::vector<std::string>& args, const std::string& initialDirectory, const Poco::Process::Env& env, std::size_t maxMessageSize = DEFAULT_MAX_MESSAGE_SIZE);
		/// Launches the server in initialDirectory (if not empty) with the
		/// given environment variables. A line longer than maxMessageSize
		/// bytes fails the request it arrives during, and is passed over
		/// otherwise.
		///
		/// What env does differs, as it does for Poco::Process::launch(): on
		/// POSIX platforms its variables are added to the environment of this
		/// process, which the server inherits, secrets included; on Windows
		/// a non-empty env is the whole environment of the server. A server
		/// that is not trusted with the environment of the application is
		/// started on POSIX platforms through a program that clears it, such
		/// as env -i, with the variables it needs (PATH among them).
		///
		/// Failures are reported as by the other constructor; on POSIX
		/// platforms an initialDirectory that does not exist is one too.

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
		/// the reply, Poco::TimeoutException when the request cannot be
		/// written or answered within the client's timeout, and
		/// Poco::ProtocolException for a line longer than the size limit.

private:
	struct Reader;
	struct Writer;

	static void read(const std::shared_ptr<Reader>& pReader, Poco::Pipe pipe, std::size_t maxMessageSize);
	static void write(const std::shared_ptr<Writer>& pWriter, Poco::Pipe pipe);
	void start(std::size_t maxMessageSize);
	void send(const std::string& line, const Poco::Timestamp& start, const Poco::Timespan& timeout, bool wait);
	void requestTermination();
	void kill();
	int waitForExit(const Poco::Timespan& timeout);
	void finish(std::thread& thread, bool ended);

	ProcessClient(const ProcessClient&) = delete;
	ProcessClient& operator = (const ProcessClient&) = delete;

	std::shared_ptr<Reader> _pReader;
	std::shared_ptr<Writer> _pWriter;
	Poco::Pipe _inPipe;
	Poco::Pipe _outPipe;
	std::unique_ptr<Poco::ProcessHandle> _pHandle;
	std::thread _readerThread;
	std::thread _writerThread;
	mutable Poco::FastMutex _mutex;
	Poco::Timespan _grace;
	bool _closed;
	int _exitCode;
};


} // namespace Poco::AI::MCP


#endif // AIMCP_ProcessClient_INCLUDED
