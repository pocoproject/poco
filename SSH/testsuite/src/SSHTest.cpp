//
// SSHTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "SSHTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/SSH/SSHHostKeyManager.h"
#include "Poco/SSH/SSHServer.h"
#include "Poco/SSH/SSHSession.h"
#include "Poco/SSH/SSHClient.h"
#include "Poco/SSH/SSHChannelStream.h"
#include "Poco/SSH/SSHException.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/Exception.h"
#include "Poco/Channel.h"
#include "Poco/Logger.h"
#include "Poco/Message.h"
#include "Poco/String.h"
#include "Poco/Types.h"
#include <libssh/libssh.h>
#include <libssh/callbacks.h>
#include <libssh/server.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

#if defined(POCO_OS_FAMILY_WINDOWS)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#endif


using namespace Poco::SSH;


namespace
{
	/// Simple echo session for testing: reads lines and echoes them back.
	class EchoSession: public SSHSession
	{
	public:
		EchoSession(ssh_session s, const SSHServerConfig& cfg, SSHServer& srv):
			SSHSession(s, cfg, srv)
		{
		}

		void handleSession(ssh_channel channel, const std::string& /*username*/) override
		{
			const std::function<bool()> stopCheck = [this]() { return stopRequested(); };
			SSHChannelStream out(channel, stopCheck);
			out << "HELLO\r\n" << std::flush;

			std::string line;
			while (sshReadLine(channel, line, DEFAULT_READ_LINE_MAX_LENGTH, DEFAULT_READ_LINE_TIMEOUT, stopCheck))
			{
				if (ssh_channel_is_eof(channel) || ssh_channel_is_closed(channel))
					break;
				if (line == "quit")
				{
					out << "BYE\r\n" << std::flush;
					break;
				}
				out << "ECHO:" << line << "\r\n" << std::flush;
			}
		}
	};

	/// Limits and stop check for the sshReadLine() calls of a ReadLineSession, and their results.
	struct ReadLineState
	{
		enum class Outcome
		{
			NONE,
			END_OF_INPUT,
			TIMEOUT,
			LINE_TOO_LONG
		};

		std::size_t maxLength = DEFAULT_READ_LINE_MAX_LENGTH;
		std::chrono::milliseconds timeout = DEFAULT_READ_LINE_TIMEOUT;
		bool stopCheck = false;
		std::vector<std::string> lines;
		Outcome outcome = Outcome::NONE;
		Poco::Event done;
	};

	/// Reads lines until sshReadLine() returns false or throws, and records them.
	class ReadLineSession: public SSHSession
	{
	public:
		ReadLineSession(ssh_session s, const SSHServerConfig& cfg, SSHServer& srv, ReadLineState& state):
			SSHSession(s, cfg, srv),
			_state(state)
		{
		}

		void handleSession(ssh_channel channel, const std::string& /*username*/) override
		{
			try
			{
				std::function<bool()> stopCheck;
				if (_state.stopCheck)
					stopCheck = [this]() { return stopRequested(); };
				std::string line;
				while (sshReadLine(channel, line, _state.maxLength, _state.timeout, stopCheck))
					_state.lines.push_back(line);
				_state.outcome = ReadLineState::Outcome::END_OF_INPUT;
			}
			catch (Poco::TimeoutException&)
			{
				_state.outcome = ReadLineState::Outcome::TIMEOUT;
			}
			catch (SSHChannelException&)
			{
				_state.outcome = ReadLineState::Outcome::LINE_TOO_LONG;
			}
			_state.done.set();
		}

	private:
		ReadLineState& _state;
	};

	/// Signals that its handler runs, then blocks like a handler that does not return,
	/// until the test releases it. It waits in steps and signals when the server stops it.
	class BlockingSession: public SSHSession
	{
	public:
		BlockingSession(
			ssh_session s,
			const SSHServerConfig& cfg,
			SSHServer& srv,
			Poco::Event& entered,
			Poco::Event& stopSeen,
			Poco::Event& release):
			SSHSession(s, cfg, srv),
			_entered(entered),
			_stopSeen(stopSeen),
			_release(release)
		{
		}

		void handleSession(ssh_channel /*channel*/, const std::string& /*username*/) override
		{
			_entered.set();
			// Bounded, so that a failing test does not leave the server waiting forever;
			// longer than the two rounds of 10 seconds that ~SSHServer() takes in a test
			const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(60);
			while (!_release.tryWait(static_cast<long>(STOP_CHECK_INTERVAL.count())) &&
				std::chrono::steady_clock::now() < deadline)
			{
				if (stopRequested())
					_stopSeen.set();
			}
		}

	private:
		Poco::Event& _entered;
		Poco::Event& _stopSeen;
		Poco::Event& _release;
	};

	/// Results of a WritingSession or a WindowWritingSession.
	struct WriteState
	{
		Poco::Event writing;
		std::size_t size = 0;
		bool bad = false;
		Poco::Event done;
	};

	/// More than the window that libssh gives a new channel
	constexpr std::size_t MORE_THAN_WINDOW = std::size_t{4} * 1024 * 1024;

	/// A client that reads PACE_SIZE per PACE_INTERVAL makes libssh grow its window after each PACE_SIZE, and takes
	/// longer than the time limit of stop() in the stop tests for PACED_WRITE_SIZE
	constexpr std::size_t PACE_SIZE = std::size_t{1} * 1024 * 1024;
	constexpr std::chrono::milliseconds PACE_INTERVAL = std::chrono::milliseconds(100);
	constexpr std::size_t PACED_WRITE_SIZE = std::size_t{64} * 1024 * 1024;

	/// Writes the given number of bytes in one write through an SSHChannelStream with a stop check, and records
	/// whether the stream is bad afterwards. With closeChannel, it then closes the channel, like a handler that
	/// ends its channel before it returns.
	class WritingSession: public SSHSession
	{
	public:
		WritingSession(ssh_session s, const SSHServerConfig& cfg, SSHServer& srv, WriteState& state,
			std::size_t writeSize, bool closeChannel = false):
			SSHSession(s, cfg, srv),
			_state(state),
			_writeSize(writeSize),
			_closeChannel(closeChannel)
		{
		}

		void handleSession(ssh_channel channel, const std::string& /*username*/) override
		{
			SSHChannelStream out(channel, [this]() { return stopRequested(); });
			_state.writing.set();
			out << std::string(_writeSize, 'x') << std::flush;
			_state.bad = out.bad();
			if (_closeChannel)
				ssh_channel_close(channel);
			_state.done.set();
		}

	private:
		WriteState& _state;
		std::size_t _writeSize;
		bool _closeChannel;
	};

	/// Waits until the client is ready, then writes as many bytes as the window of the client takes through an
	/// SSHChannelStream with a stop check and returns, which ends the session.
	class WindowWritingSession: public SSHSession
	{
	public:
		WindowWritingSession(ssh_session s, const SSHServerConfig& cfg, SSHServer& srv, WriteState& state):
			SSHSession(s, cfg, srv),
			_state(state)
		{
		}

		void handleSession(ssh_channel channel, const std::string& /*username*/) override
		{
			char ready = 0;
			if (ssh_channel_read_timeout(channel, &ready, 1, 0, 10000) == 1)
			{
				_state.size = ssh_channel_window_size(channel);
				SSHChannelStream out(channel, [this]() { return stopRequested(); });
				out << std::string(_state.size, 'x') << std::flush;
				_state.bad = out.bad();
			}
			_state.done.set();
		}

	private:
		WriteState& _state;
	};

	/// Results of a FullWindowSession.
	struct FullWindowState
	{
		std::atomic<std::size_t> fillSize{0};
		Poco::Event lineRead;
	};

	/// Uses up the window of the client to its last byte, then reads lines with a stop check until the client
	/// ends its input, so that the echo of the first line end waits for the client to grow the window.
	class FullWindowSession: public SSHSession
	{
	public:
		FullWindowSession(ssh_session s, const SSHServerConfig& cfg, SSHServer& srv, FullWindowState& state):
			SSHSession(s, cfg, srv),
			_state(state)
		{
		}

		void handleSession(ssh_channel channel, const std::string& /*username*/) override
		{
			const std::function<bool()> stopCheck = [this]() { return stopRequested(); };
			SSHChannelStream out(channel, stopCheck);
			// The client sends its input after it has read this byte, so that a window adjustment that the first
			// data causes (libssh 0.10 sends one without a read) arrives before the input
			out << 'x' << std::flush;
			if (ssh_channel_poll_timeout(channel, 10000, 0) <= 0)
				return;
			std::size_t fillSize = 0;
			while (out.good() && ssh_channel_window_size(channel) > 1)
			{
				const std::string data(ssh_channel_window_size(channel) - 1, 'x');
				out << data << std::flush;
				fillSize += data.size();
			}
			if (!out.good())
				return;
			_state.fillSize = fillSize;

			std::string line;
			while (sshReadLine(channel, line, DEFAULT_READ_LINE_MAX_LENGTH, DEFAULT_READ_LINE_TIMEOUT, stopCheck))
				_state.lineRead.set();
		}

	private:
		FullWindowState& _state;
	};

	/// The number of calls of the stop check up to which a StopCheckSession tries a stop
	constexpr int MAX_STOP_CALL = 10;

	/// Results of a StopCheckSession, for the stream and for sshReadLine(): the first call number from which on the
	/// stop check returned true and for which the write or read did not fail, or failed without a stop (0 if none),
	/// and whether a write or read ended before the stop check returned true.
	struct StopCheckState
	{
		int wrongStreamResult = 0;
		bool streamEndReached = false;
		int wrongReadLineResult = 0;
		bool readLineEndReached = false;
		Poco::Event done;
	};

	/// Writes through an SSHChannelStream, then reads lines with sshReadLine(), each time with a stop check that
	/// returns true from a given call on, starting with the first call, until a write or read finishes before that
	/// call.
	class StopCheckSession: public SSHSession
	{
	public:
		StopCheckSession(ssh_session s, const SSHServerConfig& cfg, SSHServer& srv, StopCheckState& state):
			SSHSession(s, cfg, srv),
			_state(state)
		{
		}

		void handleSession(ssh_channel channel, const std::string& /*username*/) override
		{
			for (int stopCall = 1; stopCall <= MAX_STOP_CALL && !_state.streamEndReached; ++stopCall)
			{
				int calls = 0;
				SSHChannelStream out(channel, [&calls, stopCall]() { return ++calls >= stopCall; });
				// A character goes through overflow(), a string through xsputn()
				out << 'x' << std::string("yz") << std::flush;
				const bool stopped = calls >= stopCall;
				if (out.bad() != stopped && _state.wrongStreamResult == 0)
					_state.wrongStreamResult = stopCall;
				_state.streamEndReached = !stopped;
			}
			for (int stopCall = 1; stopCall <= MAX_STOP_CALL && !_state.readLineEndReached; ++stopCall)
			{
				int calls = 0;
				std::string line;
				const bool lineRead = sshReadLine(channel, line, DEFAULT_READ_LINE_MAX_LENGTH,
					DEFAULT_READ_LINE_TIMEOUT, [&calls, stopCall]() { return ++calls >= stopCall; });
				const bool stopped = calls >= stopCall;
				if (lineRead == stopped && _state.wrongReadLineResult == 0)
					_state.wrongReadLineResult = stopCall;
				_state.readLineEndReached = !stopped;
			}
			_state.done.set();
		}

	private:
		StopCheckState& _state;
	};

	/// Results of a RawReadSession.
	struct RawReadState
	{
		Poco::Event entered;
		int result = 0;
		Poco::Event done;
	};

	/// What a RawReadSession does with a stop check before its read.
	enum class BeforeRawRead
	{
		NOTHING,
		STREAM_WRITE, /// writes one byte through an SSHChannelStream
		READ_LINE     /// reads a line with sshReadLine()
	};

	/// Signals that its handler runs, then does what BeforeRawRead says and waits in ssh_channel_read() without a
	/// timeout, like a handler that does not check for a stop, and records the result of the read.
	class RawReadSession: public SSHSession
	{
	public:
		RawReadSession(ssh_session s, const SSHServerConfig& cfg, SSHServer& srv, RawReadState& state,
			BeforeRawRead before = BeforeRawRead::NOTHING):
			SSHSession(s, cfg, srv),
			_state(state),
			_before(before)
		{
		}

		void handleSession(ssh_channel channel, const std::string& /*username*/) override
		{
			_state.entered.set();
			const std::function<bool()> stopCheck = [this]() { return stopRequested(); };
			if (_before == BeforeRawRead::STREAM_WRITE)
			{
				SSHChannelStream out(channel, stopCheck);
				out << 'x' << std::flush;
			}
			else if (_before == BeforeRawRead::READ_LINE)
			{
				std::string line;
				static_cast<void>(sshReadLine(channel, line, DEFAULT_READ_LINE_MAX_LENGTH, DEFAULT_READ_LINE_TIMEOUT,
					stopCheck));
			}
			char ch = 0;
			_state.result = ssh_channel_read(channel, &ch, 1, 0);
			_state.done.set();
		}

	private:
		RawReadState& _state;
		BeforeRawRead _before;
	};

	/// Waits before SSHSession::run() like a session whose pool thread
	/// has not started yet, until the test releases it.
	class DelayedSession: public SSHSession
	{
	public:
		DelayedSession(
			ssh_session s,
			const SSHServerConfig& cfg,
			SSHServer& srv,
			Poco::Event& waiting,
			Poco::Event& release):
			SSHSession(s, cfg, srv),
			_waiting(waiting),
			_release(release)
		{
		}

		void run() override
		{
			_waiting.set();
			// Bounded, so that a failing test does not leave the server waiting forever
			_release.tryWait(10000);
			SSHSession::run();
		}

		void handleSession(ssh_channel /*channel*/, const std::string& /*username*/) override
		{
		}

	private:
		Poco::Event& _waiting;
		Poco::Event& _release;
	};

	/// Passes the messages logged to it to a callback.
	class CallbackChannel: public Poco::Channel
	{
	public:
		using Callback = std::function<void(const Poco::Message&)>;

		explicit CallbackChannel(Callback callback):
			_callback(std::move(callback))
		{
		}

		void log(const Poco::Message& msg) override
		{
			_callback(msg);
		}

	private:
		Callback _callback;
	};

	/// Sends all messages of a logger to a callback while it exists.
	class LogCapture
	{
	public:
		LogCapture(const std::string& name, CallbackChannel::Callback callback):
			_logger(Poco::Logger::get(name)),
			_pChannel(_logger.getChannel()),
			_level(_logger.getLevel())
		{
			_logger.setChannel(new CallbackChannel(std::move(callback)));
			_logger.setLevel(Poco::Message::PRIO_DEBUG);
		}

		~LogCapture()
		{
			_logger.setChannel(_pChannel);
			_logger.setLevel(_level);
		}

		LogCapture(const LogCapture&) = delete;
		LogCapture(LogCapture&&) = delete;
		LogCapture& operator = (const LogCapture&) = delete;
		LogCapture& operator = (LogCapture&&) = delete;

	private:
		Poco::Logger& _logger;
		Poco::Channel::Ptr _pChannel;
		int _level;
	};

	/// A TCP connection to the server that sends only what the test writes,
	/// like a client that does not log in.
	class RawConnection
	{
	public:
		explicit RawConnection(Poco::UInt16 port):
			_fd(::socket(AF_INET, SOCK_STREAM, 0))
		{
			if (_fd == SSH_INVALID_FD)
				throw Poco::IOException("Cannot create a socket");
			struct sockaddr_in addr{};
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
			if (::connect(_fd, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr)) != 0)
			{
				closeSocket();
				throw Poco::IOException("Cannot connect to the SSH server");
			}
		}

		~RawConnection()
		{
			closeSocket();
		}

		RawConnection(const RawConnection&) = delete;
		RawConnection(RawConnection&&) = delete;
		RawConnection& operator = (const RawConnection&) = delete;
		RawConnection& operator = (RawConnection&&) = delete;

		void send(const std::string& data)
		{
#if defined(POCO_OS_FAMILY_WINDOWS)
			const int length = static_cast<int>(data.size());
#else
			const std::size_t length = data.size();
#endif
			if (::send(_fd, data.data(), length, 0) != static_cast<int>(data.size()))
				throw Poco::IOException("Cannot send to the SSH server");
		}

		[[nodiscard]] bool receivedData(std::chrono::milliseconds timeout)
			/// Returns true when the server has sent data, or false if nothing arrived
			/// within the timeout.
		{
#if defined(POCO_OS_FAMILY_WINDOWS)
			WSAPOLLFD pfd{};
			pfd.fd = _fd;
			pfd.events = POLLIN;
			return WSAPoll(&pfd, 1, static_cast<int>(timeout.count())) > 0;
#else
			struct pollfd pfd{};
			pfd.fd = _fd;
			pfd.events = POLLIN;
			return ::poll(&pfd, 1, static_cast<int>(timeout.count())) > 0;
#endif
		}

		[[nodiscard]] bool closedByServer(std::chrono::milliseconds timeout)
			/// Reads and discards what the server sends until the server closes the
			/// connection. Returns false if the connection is still open after the timeout.
		{
			constexpr int BUFFER_SIZE = 256;
			std::array<char, BUFFER_SIZE> buffer{};
			const auto deadline = std::chrono::steady_clock::now() + timeout;
			while (true)
			{
				const auto remaining =
					std::chrono::ceil<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now());
				if (remaining.count() <= 0)
					return false;
#if defined(POCO_OS_FAMILY_WINDOWS)
				WSAPOLLFD pfd{};
				pfd.fd = _fd;
				pfd.events = POLLIN;
				const int rc = WSAPoll(&pfd, 1, static_cast<int>(remaining.count()));
#else
				struct pollfd pfd{};
				pfd.fd = _fd;
				pfd.events = POLLIN;
				const int rc = ::poll(&pfd, 1, static_cast<int>(remaining.count()));
#endif
				if (rc > 0 && ::recv(_fd, buffer.data(), BUFFER_SIZE, 0) <= 0)
					return true;
			}
		}

	private:
		void closeSocket()
		{
#if defined(POCO_OS_FAMILY_WINDOWS)
			::closesocket(_fd);
#else
			::close(_fd);
#endif
		}

		ssh_socket_t _fd;
	};

	/// Forwards one TCP connection to the server. It delays the first data of the client, and the first data
	/// that the client sends after delayNextData(), so that the server waits for it longer than
	/// SSHSession::STOP_CHECK_INTERVAL.
	class DelayingProxy
	{
	public:
		DelayingProxy(Poco::UInt16 serverPort, std::chrono::milliseconds delay):
			_serverPort(serverPort),
			_delay(delay),
			_listenFd(::socket(AF_INET, SOCK_STREAM, 0))
		{
			struct sockaddr_in addr{};
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
			socklen_t length = sizeof(addr);
			if (_listenFd == SSH_INVALID_FD ||
				::bind(_listenFd, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr)) != 0 ||
				::listen(_listenFd, 1) != 0 ||
				::getsockname(_listenFd, reinterpret_cast<struct sockaddr*>(&addr), &length) != 0)
			{
				closeSocket(_listenFd);
				throw Poco::IOException("Cannot create the proxy socket");
			}
			_port = ntohs(addr.sin_port);
			_thread.startFunc([this]() { forward(); });
		}

		~DelayingProxy()
		{
			_thread.join();
			closeSocket(_listenFd);
		}

		DelayingProxy(const DelayingProxy&) = delete;
		DelayingProxy(DelayingProxy&&) = delete;
		DelayingProxy& operator = (const DelayingProxy&) = delete;
		DelayingProxy& operator = (DelayingProxy&&) = delete;

		[[nodiscard]] Poco::UInt16 port() const
		{
			return _port;
		}

		void delayNextData()
		{
			_delayNext = true;
		}

	private:
#if defined(POCO_OS_FAMILY_WINDOWS)
		using PollFd = WSAPOLLFD;
#else
		using PollFd = struct pollfd;
#endif

		[[nodiscard]] static int pollSockets(PollFd* pFds, int count, int timeout)
		{
#if defined(POCO_OS_FAMILY_WINDOWS)
			return WSAPoll(pFds, static_cast<ULONG>(count), timeout);
#else
			return ::poll(pFds, static_cast<nfds_t>(count), timeout);
#endif
		}

		static void closeSocket(ssh_socket_t fd)
		{
#if defined(POCO_OS_FAMILY_WINDOWS)
			::closesocket(fd);
#else
			::close(fd);
#endif
		}

		[[nodiscard]] static ssh_socket_t connectToServer(Poco::UInt16 port)
		{
			const ssh_socket_t fd = ::socket(AF_INET, SOCK_STREAM, 0);
			if (fd == SSH_INVALID_FD)
				return fd;
			struct sockaddr_in addr{};
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
			if (::connect(fd, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr)) != 0)
			{
				closeSocket(fd);
				return SSH_INVALID_FD;
			}
			return fd;
		}

		[[nodiscard]] static bool relay(ssh_socket_t from, ssh_socket_t to)
			/// Passes the data that has arrived on one socket to the other. Returns false at the end of the
			/// connection or on an error.
		{
#if defined(MSG_NOSIGNAL)
			constexpr int SEND_FLAGS = MSG_NOSIGNAL;
#else
			constexpr int SEND_FLAGS = 0;
#endif
			constexpr int BUFFER_SIZE = 16384;
			std::array<char, BUFFER_SIZE> buffer{};
			const auto received = ::recv(from, buffer.data(), BUFFER_SIZE, 0);
			if (received <= 0)
				return false;
			int sent = 0;
			while (sent < received)
			{
				const auto n = ::send(to, buffer.data() + sent, static_cast<int>(received) - sent, SEND_FLAGS);
				if (n <= 0)
					return false;
				sent += static_cast<int>(n);
			}
			return true;
		}

		void forward()
		{
			// Bounded, so that a test that fails before it connects does not wait forever
			constexpr int ACCEPT_TIMEOUT_MS = 10000;
			PollFd listenPfd{};
			listenPfd.fd = _listenFd;
			listenPfd.events = POLLIN;
			if (pollSockets(&listenPfd, 1, ACCEPT_TIMEOUT_MS) <= 0)
				return;
			const ssh_socket_t clientFd = ::accept(_listenFd, nullptr, nullptr);
			if (clientFd == SSH_INVALID_FD)
				return;
			const ssh_socket_t serverFd = connectToServer(_serverPort);
			if (serverFd != SSH_INVALID_FD)
			{
				std::array<PollFd, 2> pfds{};
				pfds[0].fd = clientFd;
				pfds[0].events = POLLIN;
				pfds[1].fd = serverFd;
				pfds[1].events = POLLIN;
				bool open = true;
				while (open && pollSockets(pfds.data(), 2, -1) > 0)
				{
					if (pfds[0].revents != 0)
					{
						if (_delayNext.exchange(false))
							Poco::Thread::sleep(static_cast<long>(_delay.count()));
						open = relay(clientFd, serverFd);
					}
					if (open && pfds[1].revents != 0)
						open = relay(serverFd, clientFd);
				}
				closeSocket(serverFd);
			}
			closeSocket(clientFd);
		}

		Poco::UInt16 _serverPort;
		std::chrono::milliseconds _delay;
		ssh_socket_t _listenFd;
		Poco::UInt16 _port = 0;
		std::atomic<bool> _delayNext{true};
		Poco::Thread _thread;
	};

	/// The time limit of stop() in the stop tests, and the time within which stop() must return there
	constexpr std::chrono::milliseconds STOP_TIMEOUT = std::chrono::seconds(5);
	constexpr std::chrono::milliseconds MAX_STOP_TIME = std::chrono::seconds(2);

	/// Gives the socket of a session a small send buffer, so that a client that does not read from the connection
	/// takes less data than the window of a channel, and libssh holds the rest. Without it, the system grows the
	/// buffer and can take the whole window.
	void limitSendBuffer(ssh_session session)
	{
		constexpr int SEND_BUFFER_SIZE = 16384;
#if defined(POCO_OS_FAMILY_WINDOWS)
		const char* pValue = reinterpret_cast<const char*>(&SEND_BUFFER_SIZE);
#else
		const int* pValue = &SEND_BUFFER_SIZE;
#endif
		if (::setsockopt(ssh_get_fd(session), SOL_SOCKET, SO_SNDBUF, pValue, sizeof(SEND_BUFFER_SIZE)) != 0)
			throw Poco::IOException("Cannot set the send buffer size of the SSH session");
	}

	/// Stops the server with STOP_TIMEOUT and returns how long stop() took.
	[[nodiscard]] std::chrono::steady_clock::duration stopTime(SSHServer& server)
	{
		const auto start = std::chrono::steady_clock::now();
		server.stop(STOP_TIMEOUT);
		return std::chrono::steady_clock::now() - start;
	}

	/// Lets libssh process what the server sends to a client that does not read its channels, until the condition
	/// holds. Returns false if it does not hold after the timeout.
	[[nodiscard]] bool processUntil(ssh_session session, const std::function<bool()>& condition,
		std::chrono::milliseconds timeout)
	{
		ssh_event event = ssh_event_new();
		ssh_event_add_session(event, session);
		const auto deadline = std::chrono::steady_clock::now() + timeout;
		while (!condition())
		{
			const auto remaining =
				std::chrono::ceil<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now());
			if (remaining.count() <= 0)
				break;
			ssh_event_dopoll(event, static_cast<int>(remaining.count()));
		}
		ssh_event_remove_session(event, session);
		ssh_event_free(event);
		return condition();
	}

	/// Lets libssh process what the server sends to a client that does not read its channels, until the server
	/// closes the connection. Returns false if the connection is still open after the timeout.
	[[nodiscard]] bool closedByServer(ssh_session session, std::chrono::milliseconds timeout)
	{
		return processUntil(session, [session]() { return ssh_is_connected(session) == 0; }, timeout);
	}

	// Restrict the client to classical key exchange algorithms. libssh 0.12+
	// advertises ML-KEM hybrid KEX (mlkem768x25519-sha256 etc.) by default,
	// and the in-process client+server pair picks it. ML-KEM through OpenSSL
	// 3.6.x has been observed to fail ("Failed to construct client init
	// buffer") on Windows vcpkg builds. Pinning classical KEX keeps the test
	// deterministic across libssh upgrades until upstream stabilizes.
	void setClassicalKex(ssh_session session)
	{
		const char* kex =
			"curve25519-sha256,curve25519-sha256@libssh.org,"
			"ecdh-sha2-nistp256,ecdh-sha2-nistp384,ecdh-sha2-nistp521,"
			"diffie-hellman-group16-sha512,diffie-hellman-group14-sha256";
		ssh_options_set(session, SSH_OPTIONS_KEY_EXCHANGE, kex);
	}

	SSHServerConfig makeTestConfig(const std::string& keyDir)
		/// The server listens on a port that the system chooses, see SSHServer::port().
	{
		SSHServerConfig cfg;
		cfg.bindAddress = "127.0.0.1";
		cfg.port = 0;
		cfg.hostKeyPath = SSHHostKeyManager::ensureHostKey(keyDir);
		cfg.maxConnections = 2;
		cfg.maxAuthAttempts = 3;
		cfg.enablePasswordAuth = true;
		cfg.enablePubkeyAuth = false;
		cfg.passwordAuthenticator = [](const std::string& user, const std::string& pass) -> bool
		{
			return user == "testuser" && pass == "testpass";
		};
		return cfg;
	}

	void authorizeKey(const std::string& privateKeyFile, const std::string& keyFile)
		/// Writes the public key of the private key file to an authorized keys file, whose name is UTF-8 also on
		/// Windows, where libssh opens a file name in the ANSI code page.
	{
		// The name of the private key file is ASCII
		const std::string publicKeyFile = privateKeyFile + ".pub";
		ssh_key key = nullptr;
		if (ssh_pki_import_privkey_file(privateKeyFile.c_str(), nullptr, nullptr, nullptr, &key) != SSH_OK)
			throw Poco::IOException("Cannot read the private key", privateKeyFile);
		const int rc = ssh_pki_export_pubkey_file(key, publicKeyFile.c_str());
		ssh_key_free(key);
		if (rc != SSH_OK)
			throw Poco::IOException("Cannot write the public key", publicKeyFile);
		Poco::File(publicKeyFile).copyTo(keyFile);
	}

	[[nodiscard]] bool loginWithPublicKey(Poco::UInt16 port, const std::string& user, const std::string& privateKeyFile)
		/// Connects a new client to the server and returns whether it logs in as the user with the key.
	{
		SSHClient client;
		setClassicalKex(client.session());
		client.connect("127.0.0.1", port);
		return client.authenticatePublicKey(user, privateKeyFile);
	}

	/// Sends input to a ReadLineSession, closes the input if closeInput is true,
	/// and returns whether the session finished reading within 10 seconds.
	[[nodiscard]] bool readLines(const std::string& keyDir, const std::string& input, bool closeInput,
		ReadLineState& state)
	{
		SSHServerConfig cfg = makeTestConfig(keyDir);
		SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
		{
			return new ReadLineSession(s, c, srv, state);
		});
		server.start();

		SSHClient client;
		setClassicalKex(client.session());
		client.connect("127.0.0.1", server.port());
		if (!client.authenticatePassword("testuser", "testpass"))
			throw SSHAuthenticationException("Test user rejected");
		ssh_channel channel = client.openShellChannel();
		if (channel == nullptr)
			throw SSHChannelException("No shell channel");

		ssh_channel_write(channel, input.data(), static_cast<uint32_t>(input.size()));
		if (closeInput)
			ssh_channel_send_eof(channel);
		const bool finished = state.done.tryWait(10000);

		ssh_channel_free(channel);
		return finished;
	}
}


SSHTest::SSHTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


SSHTest::~SSHTest() = default;


void SSHTest::setUp()
{
	_testDir = Poco::TemporaryFile::tempName();
	Poco::File(_testDir).createDirectories();
}


void SSHTest::tearDown()
{
	try
	{
		Poco::File(_testDir).remove(true);
	}
	catch (...)
	{
		// Best-effort cleanup; log nothing in tests to avoid spurious output.
	}
}


void SSHTest::testHostKeyGeneration()
{
	std::string keyPath = SSHHostKeyManager::ensureHostKey(_testDir);
	Poco::File keyFile(keyPath);
	assertTrue(keyFile.exists());
	assertTrue(keyFile.getSize() > 0);

	// Verify the key loads
	ssh_key key = nullptr;
	int rc = ssh_pki_import_privkey_file(keyPath.c_str(), nullptr, nullptr, nullptr, &key);
	assertEqual(SSH_OK, rc);
	assertTrue(key != nullptr);
	ssh_key_free(key);
}


void SSHTest::testHostKeyReuse()
{
	std::string keyPath1 = SSHHostKeyManager::ensureHostKey(_testDir);
	Poco::File keyFile(keyPath1);
	auto modTime1 = keyFile.getLastModified();

	Poco::Thread::sleep(100);

	std::string keyPath2 = SSHHostKeyManager::ensureHostKey(_testDir);
	Poco::File keyFile2(keyPath2);
	auto modTime2 = keyFile2.getLastModified();

	assertEqual(keyPath1, keyPath2);
	assertEqual(modTime1.epochMicroseconds(), modTime2.epochMicroseconds());
}


void SSHTest::testServerStartStop()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});

	server.start();
	Poco::Thread::sleep(100);
	server.stop();
	// No crash = pass
}


void SSHTest::testClientConnect()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();
	const Poco::UInt16 port = server.port();

	{
		SSHClient client;
		setClassicalKex(client.session());
		client.connect("127.0.0.1", port);
		assertTrue(client.isConnected());
		client.disconnect();
	}

	server.stop();
}


void SSHTest::testPasswordAuth()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();
	const Poco::UInt16 port = server.port();

	try
	{
		SSHClient client;
		setClassicalKex(client.session());
		client.connect("127.0.0.1", port);
		bool ok = client.authenticatePassword("testuser", "testpass");
		assertTrue(ok);
		client.disconnect();
	}
	catch (...)
	{
		server.stop();
		throw;
	}

	server.stop();
}


void SSHTest::testPasswordAuthFail()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();
	const Poco::UInt16 port = server.port();

	try
	{
		SSHClient client;
		setClassicalKex(client.session());
		client.connect("127.0.0.1", port);
		bool ok = client.authenticatePassword("testuser", "wrongpass");
		assertTrue(!ok);
		client.disconnect();
	}
	catch (...)
	{
		server.stop();
		throw;
	}

	server.stop();
}


void SSHTest::testPublicKeyAuthUserNameMustEqualKeyFileName()
{
	const std::string clientKey = Poco::Path(_testDir, "client_key").toString();
	SSHHostKeyManager::generateKey(clientKey);
	const std::string authorizedDir = Poco::Path(_testDir, "authorized").toString();
	const std::string elsewhereDir = Poco::Path(_testDir, "elsewhere").toString();
	Poco::File(authorizedDir).createDirectories();
	Poco::File(elsewhereDir).createDirectories();
	authorizeKey(clientKey, Poco::Path(authorizedDir, "testuser").toString());
	authorizeKey(clientKey, Poco::Path(elsewhereDir, "uploaded").toString());
#if !defined(_WIN32)
	// Windows file names cannot contain ":", other systems allow it
	authorizeKey(clientKey, Poco::Path(authorizedDir, "a:b").toString());
#endif

	SSHServerConfig cfg = makeTestConfig(_testDir);
	cfg.enablePubkeyAuth = true;
	cfg.authorizedKeysDir = authorizedDir;
	// One session for each login, so that a session that has not ended yet does not refuse the next login
	cfg.maxConnections = 8;

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	const auto loginWithKey = [&server, &clientKey](const std::string& user)
	{
		return loginWithPublicKey(server.port(), user, clientKey);
	};
	assertTrue(loginWithKey("testuser"));
	assertFalse(loginWithKey("../elsewhere/uploaded"));
	// A backslash separates directories on Windows
	assertFalse(loginWithKey("..\\elsewhere\\uploaded"));
	// A colon follows a drive letter or separates a stream name on Windows
	assertFalse(loginWithKey("C:x"));
	assertFalse(loginWithKey("a:b"));

	if (Poco::File(Poco::Path(authorizedDir, "TESTUSER")).exists())
		assertFalse(loginWithKey("TESTUSER"));
	else
		warnmsg("The file system distinguishes the case of file names, the login in another case is not tested");

#if defined(_WIN32)
	// Windows removes a trailing dot from a file name
	assertFalse(loginWithKey("testuser."));
#endif

	server.stop();
}


void SSHTest::testPublicKeyAuthKeepsSessionWhenKeyFileCannotBeRead()
{
	const std::string clientKey = Poco::Path(_testDir, "client_key").toString();
	SSHHostKeyManager::generateKey(clientKey);
	const std::string missingDir = Poco::Path(_testDir, "missing").toString();
	const std::string authorizedDir = Poco::Path(_testDir, "authorized").toString();
	// The directory is listed like a key file of the user; Windows does not open it, other systems do not read it
	Poco::File(Poco::Path(authorizedDir, "testuser")).createDirectories();
	std::atomic<int> warnings{0};
	std::atomic<int> expectedWarnings{0};
	std::atomic<const char*> expectedText{""};
	const LogCapture capture("Poco.SSH.SSHSession",
		[&warnings, &expectedWarnings, &expectedText](const Poco::Message& msg)
		{
			if (msg.getPriority() != Poco::Message::PRIO_WARNING)
				return;
			++warnings;
			if (msg.getText().find(expectedText.load()) != std::string::npos)
				++expectedWarnings;
		});

	const std::vector<std::pair<std::string, const char*>> cases{
		{missingDir, "cannot list the authorized keys directory"},
		{authorizedDir, "cannot read the key file"}};
	for (const auto& [keysDir, text]: cases)
	{
		warnings = 0;
		expectedWarnings = 0;
		expectedText = text;
		SSHServerConfig cfg = makeTestConfig(_testDir);
		cfg.enablePubkeyAuth = true;
		cfg.authorizedKeysDir = keysDir;

		SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
		{
			return new EchoSession(s, c, srv);
		});
		server.start();

		SSHClient client;
		setClassicalKex(client.session());
		// Without a timeout, a login on a connection whose session has ended waits forever
		constexpr long timeoutSeconds = 10;
		ssh_options_set(client.session(), SSH_OPTIONS_TIMEOUT, &timeoutSeconds);
		client.connect("127.0.0.1", server.port());
		assertFalse(client.authenticatePublicKey("testuser", clientKey));
		// The refused key leaves the session open for another authentication method
		assertTrue(client.authenticatePassword("testuser", "testpass"));
		assertEqual(1, warnings.load());
		assertEqual(1, expectedWarnings.load());
		client.disconnect();

		server.stop();
	}
}


void SSHTest::testPublicKeyAuthTakesTildeAsFileName()
{
	const std::string clientKey = Poco::Path(_testDir, "client_key").toString();
	SSHHostKeyManager::generateKey(clientKey);
	const std::string authorizedDir = Poco::Path(_testDir, "authorized").toString();
	Poco::File(authorizedDir).createDirectories();
	// A path that begins with "~" names the home directory, the file name "~" does not
	authorizeKey(clientKey, Poco::Path(authorizedDir, "~").toString());

	SSHServerConfig cfg = makeTestConfig(_testDir);
	cfg.enablePubkeyAuth = true;
	cfg.authorizedKeysDir = authorizedDir;

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	assertTrue(loginWithPublicKey(server.port(), "~", clientKey));

	server.stop();
}


void SSHTest::testPublicKeyAuthNonAsciiUserNameUsesItsOwnKeyFile()
{
	const std::string keyA = Poco::Path(_testDir, "key_a").toString();
	const std::string keyB = Poco::Path(_testDir, "key_b").toString();
	SSHHostKeyManager::generateKey(keyA);
	SSHHostKeyManager::generateKey(keyB);
	const std::string authorizedDir = Poco::Path(_testDir, "authorized").toString();
	Poco::File(authorizedDir).createDirectories();
	// A name converted to the ANSI code page on Windows turns U+FF54 into 't' and names the key file of "testuser"
	const std::string fullwidthUser = "\xEF\xBD\x94" "estuser";
	authorizeKey(keyA, Poco::Path(authorizedDir, "testuser").toString());
	authorizeKey(keyB, Poco::Path(authorizedDir, fullwidthUser).toString());

	SSHServerConfig cfg = makeTestConfig(_testDir);
	cfg.enablePubkeyAuth = true;
	cfg.authorizedKeysDir = authorizedDir;
	// One session for each login, so that a session that has not ended yet does not refuse the next login
	cfg.maxConnections = 8;

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	assertTrue(loginWithPublicKey(server.port(), fullwidthUser, keyB));
	assertFalse(loginWithPublicKey(server.port(), fullwidthUser, keyA));
	assertTrue(loginWithPublicKey(server.port(), "testuser", keyA));

	server.stop();
}


void SSHTest::testPublicKeyAuthLogsUserNameOnOneLine()
{
	const std::string clientKey = Poco::Path(_testDir, "client_key").toString();
	SSHHostKeyManager::generateKey(clientKey);
	std::atomic<int> refusals{0};
	std::atomic<int> linesWithBreak{0};
	const LogCapture capture("Poco.SSH.SSHSession", [&refusals, &linesWithBreak](const Poco::Message& msg)
	{
		if (msg.getText().find("Public key authentication refused") != std::string::npos)
			++refusals;
		if (msg.getText().find_first_of("\r\n") != std::string::npos)
			++linesWithBreak;
	});

	SSHServerConfig cfg = makeTestConfig(_testDir);
	cfg.enablePubkeyAuth = true;
	cfg.authorizedKeysDir = _testDir;

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	// The server logs the refusal before it replies to the client
	assertFalse(loginWithPublicKey(server.port(), "test\r\nuser", clientKey));
	assertEqual(1, refusals.load());
	assertEqual(0, linesWithBreak.load());

	server.stop();
}


void SSHTest::testSessionHandling()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();
	const Poco::UInt16 port = server.port();

	try
	{
		SSHClient client;
		setClassicalKex(client.session());
		client.connect("127.0.0.1", port);
		assertTrue(client.authenticatePassword("testuser", "testpass"));

		ssh_channel channel = client.openShellChannel();
		assertTrue(channel != nullptr);

		// Read greeting
		char buf[256];
		int n = ssh_channel_read(channel, buf, sizeof(buf), 0);
		assertTrue(n > 0);
		std::string greeting(buf, n);
		assertTrue(greeting.find("HELLO") != std::string::npos);

		// Send quit
		ssh_channel_write(channel, "quit\r\n", 6);
		Poco::Thread::sleep(200);

		ssh_channel_send_eof(channel);
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		client.disconnect();
	}
	catch (...)
	{
		server.stop();
		throw;
	}

	server.stop();
}


void SSHTest::testServerShutdownWithActiveSession()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();
	const Poco::UInt16 port = server.port();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", port);
	assertTrue(client.authenticatePassword("testuser", "testpass"));

	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);

	// Read greeting
	char buf[256];
	ssh_channel_read(channel, buf, sizeof(buf), 0);

	// Stop server while client is connected - should not hang or crash
	server.stop();

	ssh_channel_free(channel);
	client.disconnect();
}


void SSHTest::testClientDisconnect()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();
	const Poco::UInt16 port = server.port();

	{
		SSHClient client;
		setClassicalKex(client.session());
		client.connect("127.0.0.1", port);
		assertTrue(client.authenticatePassword("testuser", "testpass"));
		// Client goes out of scope -- destructor disconnects
	}

	Poco::Thread::sleep(200);
	server.stop();
}


void SSHTest::testMaxConnections()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);
	cfg.maxConnections = 1;

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();
	const Poco::UInt16 port = server.port();

	SSHClient client1;
	setClassicalKex(client1.session());
	client1.connect("127.0.0.1", port);
	assertTrue(client1.authenticatePassword("testuser", "testpass"));

	ssh_channel ch1 = client1.openShellChannel();
	assertTrue(ch1 != nullptr);

	// Read greeting to fully establish session
	char buf[256];
	ssh_channel_read(ch1, buf, sizeof(buf), 0);

	// Second connection: server has maxConnections=1, thread pool is full.
	// The TCP connection succeeds but no thread is available to handle
	// the SSH session, so key exchange will not complete.
	// Set a timeout so the client doesn't block forever.
	SSHClient client2;
	setClassicalKex(client2.session());
	long timeout2 = 3;
	ssh_options_set(client2.session(), SSH_OPTIONS_TIMEOUT, &timeout2);
	bool connected2 = false;
	try
	{
		client2.connect("127.0.0.1", port);
		connected2 = true;
		// Key exchange should timeout since server can't handle the session
		bool auth2 = client2.authenticatePassword("testuser", "testpass");
		assertTrue(!auth2);
	}
	catch (Poco::Exception&)
	{
		// Connection or auth timeout/failure is expected
	}
	if (connected2) client2.disconnect();

	ssh_channel_free(ch1);
	client1.disconnect();

	server.stop();
}


void SSHTest::testReadLineKeepsNonAsciiBytes()
{
	ReadLineState state;
	assertTrue(readLines(_testDir, "caf\xC3\xA9\r", true, state));

	assertTrue(state.outcome == ReadLineState::Outcome::END_OF_INPUT);
	assertEqual(1, static_cast<int>(state.lines.size()));
	assertEqual(std::string("caf\xC3\xA9"), state.lines[0]);
}


void SSHTest::testReadLineBackspaceErasesUtf8Sequence()
{
	ReadLineState state;
	assertTrue(readLines(_testDir, "ab\xC3\xA9\x7f" "c\r", true, state));

	assertTrue(state.outcome == ReadLineState::Outcome::END_OF_INPUT);
	assertEqual(1, static_cast<int>(state.lines.size()));
	assertEqual(std::string("abc"), state.lines[0]);
}


void SSHTest::testReadLineMaxLength()
{
	ReadLineState state;
	state.maxLength = 8;
	assertTrue(readLines(_testDir, "12345678\r123456789\r", true, state));

	assertTrue(state.outcome == ReadLineState::Outcome::LINE_TOO_LONG);
	assertEqual(1, static_cast<int>(state.lines.size()));
	assertEqual(std::string("12345678"), state.lines[0]);
}


void SSHTest::testReadLineTimeout()
{
	ReadLineState state;
	state.timeout = std::chrono::milliseconds(200);
	assertTrue(readLines(_testDir, "incomplete", false, state));

	assertTrue(state.outcome == ReadLineState::Outcome::TIMEOUT);
	assertTrue(state.lines.empty());
}


void SSHTest::testReadLineWithoutTimeLimit()
{
	ReadLineState state;
	state.timeout = std::chrono::milliseconds::max();
	assertTrue(readLines(_testDir, "line\r", true, state));

	assertTrue(state.outcome == ReadLineState::Outcome::END_OF_INPUT);
	assertEqual(1, static_cast<int>(state.lines.size()));
	assertEqual(std::string("line"), state.lines[0]);
}


void SSHTest::testReadLineNegativeTimeout()
{
	// Without its check in deadlineAfter(), the timeout overflows when it is converted to nanoseconds and added to
	// the current time. The wrapped deadline of min() lies in the past, so only UndefinedBehaviorSanitizer reports
	// the overflow; the wrapped deadline of -17446744073710 ms lies about 31 years ahead, so the line is read.
	for (const auto timeout: {std::chrono::milliseconds::min(), std::chrono::milliseconds(-17446744073710)})
	{
		ReadLineState state;
		state.timeout = timeout;
		assertTrue(readLines(_testDir, "line\r", true, state));

		assertTrue(state.outcome == ReadLineState::Outcome::TIMEOUT);
		assertTrue(state.lines.empty());
	}
}


void SSHTest::testServerReportsChosenPort()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();
	assertTrue(server.port() != 0);

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	client.disconnect();
}


void SSHTest::testServerStopTimeout()
{
	Poco::Event entered;
	Poco::Event stopSeen;
	Poco::Event release;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg,
		[&entered, &stopSeen, &release](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
		{
			return new BlockingSession(s, c, srv, entered, stopSeen, release);
		});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	assertTrue(entered.tryWait(10000));

	try
	{
		server.stop(std::chrono::milliseconds(200));
		failmsg("stop() must throw while a session handler does not return");
	}
	catch (Poco::TimeoutException&)
	{
	}
	assertTrue(stopSeen.tryWait(10000));

	release.set();
	server.stop();

	ssh_channel_free(channel);
	client.disconnect();
}


void SSHTest::testServerStopWaitsForSessionNotStarted()
{
	Poco::Event waiting;
	Poco::Event release;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&waiting, &release](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new DelayedSession(s, c, srv, waiting, release);
	});
	server.start();

	// The client waits for the key exchange, which starts when the session runs
	const Poco::UInt16 port = server.port();
	Poco::Thread clientThread;
	clientThread.startFunc([port]()
	{
		try
		{
			SSHClient client;
			setClassicalKex(client.session());
			client.connect("127.0.0.1", port);
		}
		catch (Poco::Exception&)
		{
		}
	});
	assertTrue(waiting.tryWait(10000));

	bool timedOut = false;
	try
	{
		server.stop(std::chrono::milliseconds(200));
	}
	catch (Poco::TimeoutException&)
	{
		timedOut = true;
	}
	release.set();
	clientThread.join();
	assertTrue(timedOut);

	server.stop();
}


void SSHTest::testServerStopRequestsSessionsOnce()
{
	std::atomic<int> stopMessages{0};
	const LogCapture capture("Poco.SSH.SSHServer", [&stopMessages](const Poco::Message& msg)
	{
		if (Poco::startsWith(msg.getText(), std::string("Stopping")))
			++stopMessages;
	});

	Poco::Event entered;
	Poco::Event stopSeen;
	Poco::Event release;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg,
		[&entered, &stopSeen, &release](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
		{
			return new BlockingSession(s, c, srv, entered, stopSeen, release);
		});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	assertTrue(entered.tryWait(10000));

	for (int i = 0; i < 2; ++i)
	{
		try
		{
			server.stop(std::chrono::milliseconds(100));
			failmsg("stop() must throw while a session handler does not return");
		}
		catch (Poco::TimeoutException&)
		{
		}
	}
	release.set();
	server.stop();

	assertEqual(1, stopMessages.load());

	ssh_channel_free(channel);
	client.disconnect();
}


void SSHTest::testServerDestructorWarnsOnce()
{
	Poco::Event entered;
	Poco::Event stopSeen;
	Poco::Event release;
	std::atomic<int> warnings{0};
	std::atomic<int> timeouts{0};
	const LogCapture capture("Poco.SSH.SSHServer", [&warnings, &timeouts, &release](const Poco::Message& msg)
	{
		if (msg.getPriority() == Poco::Message::PRIO_WARNING)
			++warnings;
		// The handler returns after the second round of the destructor has timed out
		if (msg.getText().find("still running") != std::string::npos && ++timeouts == 2)
			release.set();
	});

	SSHServerConfig cfg = makeTestConfig(_testDir);
	SSHClient client;
	ssh_channel channel = nullptr;
	{
		auto factory =
			[&entered, &stopSeen, &release](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
			{
				return new BlockingSession(s, c, srv, entered, stopSeen, release);
			};
		SSHServer server(cfg, factory);
		server.start();

		setClassicalKex(client.session());
		client.connect("127.0.0.1", server.port());
		assertTrue(client.authenticatePassword("testuser", "testpass"));
		channel = client.openShellChannel();
		assertTrue(channel != nullptr);
		assertTrue(entered.tryWait(10000));
	}

	assertEqual(2, timeouts.load());
	assertEqual(1, warnings.load());

	ssh_channel_free(channel);
	client.disconnect();
}


void SSHTest::testLoginGraceTime()
{
	std::atomic<int> closeMessages{0};
	const LogCapture capture("Poco.SSH.SSHSession", [&closeMessages](const Poco::Message& msg)
	{
		if (msg.getText().find("from 127.0.0.1 port ") != std::string::npos)
			++closeMessages;
	});
	SSHServerConfig cfg = makeTestConfig(_testDir);
	cfg.loginGraceTime = std::chrono::milliseconds(500);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	// Neither a client that sends nothing nor one that sends only its banner completes the key exchange
	for (const std::string& input: {std::string(), std::string("SSH-2.0-PocoTest\r\n")})
	{
		const auto start = std::chrono::steady_clock::now();
		RawConnection connection(server.port());
		if (!input.empty())
			connection.send(input);
		assertTrue(connection.closedByServer(std::chrono::seconds(10)));
		assertTrue(std::chrono::steady_clock::now() - start >= cfg.loginGraceTime);
	}
	assertEqual(2, closeMessages.load());
}


void SSHTest::testLoginGraceTimeLimitsAuthentication()
{
	std::atomic<int> closeMessages{0};
	const LogCapture capture("Poco.SSH.SSHSession", [&closeMessages](const Poco::Message& msg)
	{
		if (msg.getText().find("from 127.0.0.1 port ") != std::string::npos)
			++closeMessages;
	});
	SSHServerConfig cfg = makeTestConfig(_testDir);
	cfg.loginGraceTime = std::chrono::seconds(1);
	// The number of requests does not end the authentication before the grace time
	cfg.maxAuthAttempts = 1000;

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	// The grace time of the server starts before the key exchange
	const auto start = std::chrono::steady_clock::now();
	client.connect("127.0.0.1", server.port());

	// Each request arrives well within the grace time of the previous one, but the requests together take longer
	// A request on a closed connection waits without a time limit, so each one is sent only while it is connected.
	while (ssh_is_connected(client.session()) != 0 &&
		std::chrono::steady_clock::now() - start < std::chrono::seconds(10))
	{
		ssh_userauth_none(client.session(), nullptr);
		Poco::Thread::sleep(100);
	}
	assertTrue(ssh_is_connected(client.session()) == 0);
	assertTrue(std::chrono::steady_clock::now() - start >= cfg.loginGraceTime);
	assertEqual(1, closeMessages.load());
}


void SSHTest::testLoginGraceTimeLimitsChannelOpening()
{
	std::atomic<int> closeMessages{0};
	const LogCapture capture("Poco.SSH.SSHSession", [&closeMessages](const Poco::Message& msg)
	{
		if (msg.getText().find("from 127.0.0.1 port ") != std::string::npos)
			++closeMessages;
	});
	SSHServerConfig cfg = makeTestConfig(_testDir);
	cfg.loginGraceTime = std::chrono::seconds(1);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	// The grace time of the server starts before the key exchange
	const auto start = std::chrono::steady_clock::now();
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));

	// The client opens no channel
	assertTrue(closedByServer(client.session(), std::chrono::seconds(10)));
	assertTrue(std::chrono::steady_clock::now() - start >= cfg.loginGraceTime);
	assertEqual(1, closeMessages.load());
}


void SSHTest::testLoginGraceTimeZeroSetsNoLimit()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);
	cfg.loginGraceTime = std::chrono::milliseconds::zero();

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	RawConnection connection(server.port());
	assertFalse(connection.closedByServer(std::chrono::milliseconds(1500)));
}


void SSHTest::testLoginGraceTimeEndsWithChannelOpening()
{
	ReadLineState state;
	// Longer than the grace time
	state.timeout = std::chrono::seconds(3);
	SSHServerConfig cfg = makeTestConfig(_testDir);
	cfg.loginGraceTime = std::chrono::seconds(1);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new ReadLineSession(s, c, srv, state);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);

	// The read times out instead of ending, so the connection stayed open after the grace time
	assertTrue(state.done.tryWait(10000));
	assertTrue(state.outcome == ReadLineState::Outcome::TIMEOUT);

	ssh_channel_free(channel);
}


void SSHTest::testServerStopDuringPasswordCheck()
{
	Poco::Event checkEntered;
	Poco::Event clientDone;
	SSHServerConfig cfg = makeTestConfig(_testDir);
	cfg.passwordAuthenticator = [&checkEntered, &clientDone](const std::string&, const std::string&) -> bool
	{
		checkEntered.set();
		// The check ends after stop() has disconnected the client; bounded, so that a failing test ends
		clientDone.tryWait(10000);
		return true;
	};

	auto factory = [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	};
	auto pServer = std::make_unique<SSHServer>(cfg, factory);
	pServer->start();

	const Poco::UInt16 port = pServer->port();
	Poco::Thread clientThread;
	clientThread.startFunc([port, &clientDone]()
	{
		try
		{
			SSHClient client;
			setClassicalKex(client.session());
			client.connect("127.0.0.1", port);
			static_cast<void>(client.authenticatePassword("testuser", "testpass"));
		}
		catch (Poco::Exception&)
		{
		}
		clientDone.set();
	});
	assertTrue(checkEntered.tryWait(10000));

	try
	{
		pServer->stop(std::chrono::seconds(5));
	}
	catch (Poco::TimeoutException&)
	{
		clientDone.set();
		clientThread.join();
		// The session does not end, so ~SSHServer() would wait for it forever
		static_cast<void>(pServer.release());
		failmsg("stop() must end a session whose password check returns after the stop");
	}
	clientThread.join();
}


void SSHTest::testServerStopDuringKeyExchange()
{
	std::atomic<int> stopMessages{0};
	const LogCapture capture("Poco.SSH.SSHSession", [&stopMessages](const Poco::Message& msg)
	{
		if (Poco::startsWith(msg.getText(), std::string("SSH session from 127.0.0.1 port ")) &&
			Poco::endsWith(msg.getText(), std::string(" stopped by the server")))
		{
			++stopMessages;
		}
	});
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	RawConnection connection(server.port());
	connection.send("SSH-2.0-PocoTest\r\n");
	// The server sends its banner in the key exchange
	assertTrue(connection.receivedData(std::chrono::seconds(10)));

	assertTrue(stopTime(server) < MAX_STOP_TIME);
	assertEqual(1, stopMessages.load());
}


void SSHTest::testServerStopDuringAuthentication()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	// The client completes the key exchange and sends no authentication request
	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());

	assertTrue(stopTime(server) < MAX_STOP_TIME);
}


void SSHTest::testServerStopDuringChannelOpening()
{
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	// The client authenticates and opens no channel
	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));

	assertTrue(stopTime(server) < MAX_STOP_TIME);
}


void SSHTest::testServerStopDuringReadLine()
{
	ReadLineState state;
	state.timeout = std::chrono::seconds(60);
	state.stopCheck = true;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new ReadLineSession(s, c, srv, state);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	// The echo shows that the handler reads the line
	ssh_channel_write(channel, "a", 1);
	char echo = 0;
	assertEqual(1, ssh_channel_read_timeout(channel, &echo, 1, 0, 10000));

	assertTrue(stopTime(server) < MAX_STOP_TIME);
	assertTrue(state.done.tryWait(0));
	assertTrue(state.outcome == ReadLineState::Outcome::END_OF_INPUT);
	assertTrue(state.lines.empty());

	ssh_channel_free(channel);
}


void SSHTest::testServerStopDuringStreamWrite()
{
	WriteState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new WritingSession(s, c, srv, state, MORE_THAN_WINDOW);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	assertTrue(state.writing.tryWait(10000));

	// The client receives the data but does not read the channel, so the window of the channel is not grown and
	// the write of the handler waits for the window, not for the socket
	bool closed = false;
	Poco::Thread clientThread;
	clientThread.startFunc([&client, &closed]()
	{
		closed = closedByServer(client.session(), std::chrono::seconds(10));
	});
	std::chrono::steady_clock::duration elapsed = STOP_TIMEOUT;
	try
	{
		elapsed = stopTime(server);
	}
	catch (Poco::TimeoutException&)
	{
		// elapsed keeps STOP_TIMEOUT, so the assertion below reports the failure
	}
	// Joined before an assertion can end the test, because the thread uses the client
	clientThread.join();

	assertTrue(elapsed < MAX_STOP_TIME);
	assertTrue(closed);
	assertTrue(state.done.tryWait(0));
	assertTrue(state.bad);

	ssh_channel_free(channel);
}


void SSHTest::testServerStopDuringStreamWriteToFullConnection()
{
	WriteState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new WritingSession(s, c, srv, state, MORE_THAN_WINDOW);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	assertTrue(state.writing.tryWait(10000));

	// The client does not read from the connection, so the socket buffers fill before the window of the channel is
	// used up
	try
	{
		assertTrue(stopTime(server) < MAX_STOP_TIME);
	}
	catch (Poco::TimeoutException&)
	{
		// Closing the connection ends the write, so that the server can be destroyed
		client.disconnect();
		server.stop();
		failmsg("stop() must end a write to a client that does not read from the connection");
	}
	assertTrue(state.done.tryWait(0));
	assertTrue(state.bad);

	ssh_channel_free(channel);
}


void SSHTest::testServerStopDuringStreamWriteBeforeChannelClose()
{
	WriteState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		limitSendBuffer(s);
		return new WritingSession(s, c, srv, state, MORE_THAN_WINDOW, true);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	assertTrue(state.writing.tryWait(10000));

	// The client does not read from the connection, so libssh holds data when the write stops, and closing the
	// channel waits for libssh to send it
	try
	{
		assertTrue(stopTime(server) < MAX_STOP_TIME);
	}
	catch (Poco::TimeoutException&)
	{
		// Closing the connection ends the wait, so that the server can be destroyed
		client.disconnect();
		server.stop();
		failmsg("stop() must end a handler that closes its channel after a stopped write");
	}
	assertTrue(state.done.tryWait(0));
	assertTrue(state.bad);

	ssh_channel_free(channel);
}


void SSHTest::testLoginResumesAfterTimedOutWaits()
{
	ReadLineState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new ReadLineSession(s, c, srv, state);
	});
	server.start();

	// The proxy delays the banner of the client, so that the key exchange of the server times out several times
	DelayingProxy proxy(server.port(), 3 * SSHSession::STOP_CHECK_INTERVAL);
	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", proxy.port());
	// The authentication of the server times out several times as well
	proxy.delayNextData();
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);

	const std::string input("line\r");
	ssh_channel_write(channel, input.data(), static_cast<uint32_t>(input.size()));
	ssh_channel_send_eof(channel);
	assertTrue(state.done.tryWait(10000));
	assertTrue(state.outcome == ReadLineState::Outcome::END_OF_INPUT);
	assertEqual(1, static_cast<int>(state.lines.size()));
	assertEqual(std::string("line"), state.lines[0]);

	ssh_channel_free(channel);
}


void SSHTest::testServerStopDuringRawRead()
{
	RawReadState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new RawReadSession(s, c, srv, state);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	assertTrue(state.entered.tryWait(10000));

	assertTrue(stopTime(server) < MAX_STOP_TIME);
	assertTrue(state.done.tryWait(0));

	ssh_channel_free(channel);
}


void SSHTest::testHandlerReadWaitsWithoutTimeLimit()
{
	RawReadState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new RawReadSession(s, c, srv, state);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	assertTrue(state.entered.tryWait(10000));

	// The waits of the login limit the timeout of the session; the handler waits without it
	assertFalse(state.done.tryWait(static_cast<long>(3 * SSHSession::STOP_CHECK_INTERVAL.count())));
	ssh_channel_write(channel, "a", 1);
	assertTrue(state.done.tryWait(10000));
	assertEqual(1, state.result);

	ssh_channel_free(channel);
}


void SSHTest::testHandlerReadAfterStreamWriteWaitsWithoutTimeLimit()
{
	RawReadState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new RawReadSession(s, c, srv, state, BeforeRawRead::STREAM_WRITE);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	assertTrue(state.entered.tryWait(10000));

	// The steps of the write limit the timeout of the session; the read after the write waits without it
	assertFalse(state.done.tryWait(static_cast<long>(3 * SSHSession::STOP_CHECK_INTERVAL.count())));
	ssh_channel_write(channel, "a", 1);
	assertTrue(state.done.tryWait(10000));
	assertEqual(1, state.result);

	ssh_channel_free(channel);
}


void SSHTest::testServerStopDuringReadLineBeforeRawRead()
{
	RawReadState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new RawReadSession(s, c, srv, state, BeforeRawRead::READ_LINE);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	assertTrue(state.entered.tryWait(10000));

	// The client sends nothing, so the read after the stopped sshReadLine() waits for the session timeout
	try
	{
		assertTrue(stopTime(server) < MAX_STOP_TIME);
	}
	catch (Poco::TimeoutException&)
	{
		// Closing the connection ends the read, so that the server can be destroyed
		client.disconnect();
		server.stop();
		failmsg("stop() must end a handler that reads without a timeout after a stopped sshReadLine()");
	}
	assertTrue(state.done.tryWait(0));

	ssh_channel_free(channel);
}


void SSHTest::testReadLineEchoWaitsForWindow()
{
	FullWindowState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new FullWindowSession(s, c, srv, state);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	char first = 0;
	assertEqual(1, ssh_channel_read_timeout(channel, &first, 1, 0, 10000));
	ssh_channel_write(channel, "\r", 1);

	// The client takes the data from the connection but does not read the channel, so the window stays used up,
	// until the first byte of the echo has arrived
	assertTrue(processUntil(client.session(), [&state, channel]()
		{
			const std::size_t fillSize = state.fillSize;
			return fillSize > 0 && ssh_channel_poll(channel, 0) > static_cast<int>(fillSize);
		},
		std::chrono::seconds(10)));
	assertEqual(static_cast<int>(state.fillSize.load()) + 1, ssh_channel_poll(channel, 0));
	assertFalse(state.lineRead.tryWait(static_cast<long>(3 * SSHSession::STOP_CHECK_INTERVAL.count())));

	// Reading the channel grows the window, so the rest of the echo follows
	std::string received;
	constexpr std::size_t BUFFER_SIZE = 65536;
	std::array<char, BUFFER_SIZE> buffer{};
	const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(10);
	while (received.size() < state.fillSize + 2 && std::chrono::steady_clock::now() < deadline)
	{
		const std::size_t missing = state.fillSize + 2 - received.size();
		const int n = ssh_channel_read_timeout(channel, buffer.data(),
			static_cast<uint32_t>(std::min(missing, buffer.size())), 0, 10000);
		if (n > 0)
			received.append(buffer.data(), static_cast<std::size_t>(n));
	}
	assertTrue(state.lineRead.tryWait(10000));
	assertEqual(state.fillSize + 2, received.size());
	assertTrue(Poco::endsWith(received, std::string("\r\n")));

	ssh_channel_send_eof(channel);
	ssh_channel_free(channel);
}


void SSHTest::testStreamWriteSendsAllDataBeforeSessionEnds()
{
	WriteState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new WindowWritingSession(s, c, srv, state);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);

	// Records how much data has arrived, also when the end of the session arrives with it. The data stays in the
	// channel, so the client does not grow the window and sends nothing that the server leaves unread when it
	// closes the connection, which would reset it.
	std::size_t received = 0;
	ssh_channel_callbacks_struct callbacks{};
	callbacks.userdata = &received;
	callbacks.channel_data_function =
		[](ssh_session, ssh_channel, void*, uint32_t length, int, void* pUserdata) -> int
		{
			*static_cast<std::size_t*>(pUserdata) = length;
			return 0;
		};
	ssh_callbacks_init(&callbacks);
	assertEqual(SSH_OK, ssh_set_channel_callbacks(channel, &callbacks));
	ssh_channel_write(channel, "r", 1);

	// The client takes nothing from the connection for longer than a wait of the server, then everything until the
	// server ends the session
	Poco::Thread::sleep(static_cast<long>(3 * SSHSession::STOP_CHECK_INTERVAL.count()));
	assertTrue(closedByServer(client.session(), std::chrono::seconds(10)));
	assertTrue(state.done.tryWait(0));
	assertFalse(state.bad);
	assertTrue(state.size > 0);
	assertEqual(state.size, received);

	ssh_channel_free(channel);
}


void SSHTest::testServerStopDuringStreamWriteToReadingClient()
{
	WriteState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new WritingSession(s, c, srv, state, PACED_WRITE_SIZE);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);
	assertTrue(state.writing.tryWait(10000));

	// The client reads the channel steadily, so that it grows the window within each wait of the server for it,
	// until the server ends the session
	Poco::Event receiving;
	Poco::Thread clientThread;
	clientThread.startFunc([channel, &receiving]()
	{
		constexpr std::size_t BUFFER_SIZE = 65536;
		std::array<char, BUFFER_SIZE> buffer{};
		while (true)
		{
			std::size_t paceReceived = 0;
			while (paceReceived < PACE_SIZE)
			{
				const int n = ssh_channel_read_timeout(channel, buffer.data(), static_cast<uint32_t>(buffer.size()),
					0, 10000);
				if (n <= 0)
					return;
				paceReceived += static_cast<std::size_t>(n);
			}
			receiving.set();
			// The pause is the pace under test, not a synchronization
			Poco::Thread::sleep(static_cast<long>(PACE_INTERVAL.count()));
		}
	});
	const bool wasReceiving = receiving.tryWait(10000);
	std::chrono::steady_clock::duration elapsed = STOP_TIMEOUT;
	try
	{
		elapsed = stopTime(server);
	}
	catch (Poco::TimeoutException&)
	{
		// elapsed keeps STOP_TIMEOUT, so the assertion below reports the failure
	}
	// Joined before an assertion can end the test, because the thread uses the client
	clientThread.join();

	assertTrue(wasReceiving);
	assertTrue(elapsed < MAX_STOP_TIME);
	assertTrue(state.done.tryWait(0));
	assertTrue(state.bad);

	ssh_channel_free(channel);
}


void SSHTest::testWriteFailsWhenStopCheckReturnsTrue()
{
	StopCheckState state;
	SSHServerConfig cfg = makeTestConfig(_testDir);

	SSHServer server(cfg, [&state](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new StopCheckSession(s, c, srv, state);
	});
	server.start();

	SSHClient client;
	setClassicalKex(client.session());
	client.connect("127.0.0.1", server.port());
	assertTrue(client.authenticatePassword("testuser", "testpass"));
	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);

	// One line end for each sshReadLine() call of the session
	const std::string input(static_cast<std::size_t>(MAX_STOP_CALL), '\r');
	assertEqual(static_cast<int>(input.size()),
		ssh_channel_write(channel, input.data(), static_cast<uint32_t>(input.size())));
	assertTrue(state.done.tryWait(10000));
	assertEqual(0, state.wrongStreamResult);
	assertTrue(state.streamEndReached);
	assertEqual(0, state.wrongReadLineResult);
	assertTrue(state.readLineEndReached);

	ssh_channel_free(channel);
}


CppUnit::Test* SSHTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SSHTest");

	CppUnit_addTest(pSuite, SSHTest, testHostKeyGeneration);
	CppUnit_addTest(pSuite, SSHTest, testHostKeyReuse);
	CppUnit_addTest(pSuite, SSHTest, testServerStartStop);
	CppUnit_addTest(pSuite, SSHTest, testClientConnect);
	CppUnit_addTest(pSuite, SSHTest, testPasswordAuth);
	CppUnit_addTest(pSuite, SSHTest, testPasswordAuthFail);
	CppUnit_addTest(pSuite, SSHTest, testPublicKeyAuthUserNameMustEqualKeyFileName);
	CppUnit_addTest(pSuite, SSHTest, testPublicKeyAuthKeepsSessionWhenKeyFileCannotBeRead);
	CppUnit_addTest(pSuite, SSHTest, testPublicKeyAuthTakesTildeAsFileName);
	CppUnit_addTest(pSuite, SSHTest, testPublicKeyAuthNonAsciiUserNameUsesItsOwnKeyFile);
	CppUnit_addTest(pSuite, SSHTest, testPublicKeyAuthLogsUserNameOnOneLine);
	CppUnit_addTest(pSuite, SSHTest, testSessionHandling);
	CppUnit_addTest(pSuite, SSHTest, testServerShutdownWithActiveSession);
	CppUnit_addTest(pSuite, SSHTest, testClientDisconnect);
	CppUnit_addTest(pSuite, SSHTest, testMaxConnections);
	CppUnit_addTest(pSuite, SSHTest, testReadLineKeepsNonAsciiBytes);
	CppUnit_addTest(pSuite, SSHTest, testReadLineBackspaceErasesUtf8Sequence);
	CppUnit_addTest(pSuite, SSHTest, testReadLineMaxLength);
	CppUnit_addTest(pSuite, SSHTest, testReadLineTimeout);
	CppUnit_addTest(pSuite, SSHTest, testReadLineWithoutTimeLimit);
	CppUnit_addTest(pSuite, SSHTest, testReadLineNegativeTimeout);
	CppUnit_addTest(pSuite, SSHTest, testServerReportsChosenPort);
	CppUnit_addTest(pSuite, SSHTest, testServerStopTimeout);
	CppUnit_addTest(pSuite, SSHTest, testServerStopWaitsForSessionNotStarted);
	CppUnit_addTest(pSuite, SSHTest, testServerStopRequestsSessionsOnce);
	CppUnit_addLongTest(pSuite, SSHTest, testServerDestructorWarnsOnce);
	CppUnit_addTest(pSuite, SSHTest, testServerStopDuringKeyExchange);
	CppUnit_addTest(pSuite, SSHTest, testServerStopDuringAuthentication);
	CppUnit_addTest(pSuite, SSHTest, testServerStopDuringChannelOpening);
	CppUnit_addTest(pSuite, SSHTest, testServerStopDuringReadLine);
	CppUnit_addTest(pSuite, SSHTest, testServerStopDuringStreamWrite);
	CppUnit_addTest(pSuite, SSHTest, testServerStopDuringStreamWriteToFullConnection);
	CppUnit_addTest(pSuite, SSHTest, testServerStopDuringStreamWriteBeforeChannelClose);
	CppUnit_addTest(pSuite, SSHTest, testLoginResumesAfterTimedOutWaits);
	CppUnit_addTest(pSuite, SSHTest, testHandlerReadWaitsWithoutTimeLimit);
	CppUnit_addTest(pSuite, SSHTest, testHandlerReadAfterStreamWriteWaitsWithoutTimeLimit);
	CppUnit_addTest(pSuite, SSHTest, testServerStopDuringReadLineBeforeRawRead);
	CppUnit_addTest(pSuite, SSHTest, testReadLineEchoWaitsForWindow);
	CppUnit_addTest(pSuite, SSHTest, testStreamWriteSendsAllDataBeforeSessionEnds);
	CppUnit_addTest(pSuite, SSHTest, testServerStopDuringStreamWriteToReadingClient);
	CppUnit_addTest(pSuite, SSHTest, testWriteFailsWhenStopCheckReturnsTrue);
#if !defined(POCO_OS_FAMILY_WINDOWS)
	// Only the wake descriptor of POSIX systems ends a session whose handler or callback waits without a stop check
	CppUnit_addTest(pSuite, SSHTest, testServerStopDuringPasswordCheck);
	CppUnit_addTest(pSuite, SSHTest, testServerStopDuringRawRead);
#endif
	CppUnit_addTest(pSuite, SSHTest, testLoginGraceTime);
	CppUnit_addTest(pSuite, SSHTest, testLoginGraceTimeLimitsAuthentication);
	CppUnit_addTest(pSuite, SSHTest, testLoginGraceTimeLimitsChannelOpening);
	CppUnit_addTest(pSuite, SSHTest, testLoginGraceTimeZeroSetsNoLimit);
	CppUnit_addTest(pSuite, SSHTest, testLoginGraceTimeEndsWithChannelOpening);

	return pSuite;
}
