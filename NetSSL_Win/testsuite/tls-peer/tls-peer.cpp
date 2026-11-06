//
// tls-peer.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//
// A TLS peer for the tests of OpenSSLPeerTest. They need byte arrangements that
// Schannel does not produce: a key update or a renegotiation after application data,
// written together with it. The program runs on a Unix host with OpenSSL 3.
//
// Build:
//   c++ -std=c++17 -o tls-peer tls-peer.cpp -I<openssl include> -L<openssl lib> -lssl -lcrypto
// Certificate and key, from the certificate of the testsuite:
//   openssl pkcs12 -in any.pfx -passin pass:secret -nodes -out any.pem
//   (add -legacy if OpenSSL 3 refuses the PKCS #12 file)
// Run:
//   tls-peer <base port> any.pem
// and on the Windows side: set POCO_TLS_PEER=<host>:<base port>
//
// Each port serves one scenario, one connection at a time:
//   base + 0  server-keyupdate    TLS 1.3 server, writes [one][KeyUpdate][two] at once
//   base + 1  server-renegotiate  TLS 1.2 server, writes a HelloRequest, then the final
//                                 flight of the renegotiation and [two] at once
//   base + 2  client-keyupdate    TLS 1.3 client on the accepted connection, writes
//                                 [one][KeyUpdate][two] at once
//   base + 3  client-renegotiate  TLS 1.2 client on the accepted connection, writes
//                                 [one][ClientHello] at once, then the final flight of
//                                 the renegotiation and [two] at once
// After that, each scenario echoes what it receives until the connection ends.
//


#include <openssl/err.h>
#include <openssl/ssl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>


namespace
{
	enum class Role
	{
		SERVER,
		CLIENT
	};

	enum class Action
	{
		KEY_UPDATE,
		RENEGOTIATE
	};

	struct Scenario
	{
		const char* name;
		Role role;
		Action action;
	};

	const Scenario SCENARIOS[] =
	{
		{"server-keyupdate", Role::SERVER, Action::KEY_UPDATE},
		{"server-renegotiate", Role::SERVER, Action::RENEGOTIATE},
		{"client-keyupdate", Role::CLIENT, Action::KEY_UPDATE},
		{"client-renegotiate", Role::CLIENT, Action::RENEGOTIATE}
	};

	constexpr int READ_SIZE = 16384;

	std::mutex logMutex;

	void log(const Scenario& scenario, const std::string& text)
	{
		std::lock_guard<std::mutex> lock(logMutex);
		std::cerr << scenario.name << ": " << text << std::endl;
	}

	std::string sslErrors()
		/// Returns and clears the OpenSSL errors of the calling thread.
	{
		std::string text;
		unsigned long error = 0;
		while ((error = ERR_get_error()) != 0)
		{
			char buffer[256];
			ERR_error_string_n(error, buffer, sizeof(buffer));
			if (!text.empty()) text += "; ";
			text += buffer;
		}
		return text;
	}

	using ContextPtr = std::unique_ptr<SSL_CTX, decltype(&SSL_CTX_free)>;

	ContextPtr createContext(const Scenario& scenario, const std::string& pemPath)
	{
		ContextPtr pContext(SSL_CTX_new(scenario.role == Role::SERVER ? TLS_server_method() : TLS_client_method()), &SSL_CTX_free);
		if (pContext == nullptr)
			throw std::runtime_error("SSL_CTX_new: " + sslErrors());
		// the certificate of the testsuite is signed with SHA-1, which the default security level refuses
		SSL_CTX_set_security_level(pContext.get(), 0);
		if (scenario.action == Action::KEY_UPDATE)
			SSL_CTX_set_min_proto_version(pContext.get(), TLS1_3_VERSION);
		else
			SSL_CTX_set_max_proto_version(pContext.get(), TLS1_2_VERSION);
		if (scenario.role == Role::SERVER)
		{
			if (SSL_CTX_use_certificate_file(pContext.get(), pemPath.c_str(), SSL_FILETYPE_PEM) != 1
				|| SSL_CTX_use_PrivateKey_file(pContext.get(), pemPath.c_str(), SSL_FILETYPE_PEM) != 1)
				throw std::runtime_error("cannot load " + pemPath + ": " + sslErrors());
		}
		else
		{
			SSL_CTX_set_verify(pContext.get(), SSL_VERIFY_NONE, nullptr);
		}
		return pContext;
	}

	class Connection
		/// A TLS connection on a socket. Its output collects in a memory BIO until
		/// flush() writes it to the socket at once; its input is read from the socket
		/// into a memory BIO when OpenSSL needs more.
	{
	public:
		Connection(SSL_CTX* pContext, int fd, Role role):
			_fd(fd),
			_pSSL(SSL_new(pContext), &SSL_free)
		{
			if (_pSSL == nullptr)
			{
				::close(fd);
				throw std::runtime_error("SSL_new: " + sslErrors());
			}
			_pInput = BIO_new(BIO_s_mem());
			_pOutput = BIO_new(BIO_s_mem());
			if (_pInput == nullptr || _pOutput == nullptr)
			{
				BIO_free(_pInput);
				BIO_free(_pOutput);
				::close(fd);
				throw std::runtime_error("BIO_new: " + sslErrors());
			}
			// the SSL owns the BIOs from here on
			SSL_set_bio(_pSSL.get(), _pInput, _pOutput);
			if (role == Role::SERVER)
				SSL_set_accept_state(_pSSL.get());
			else
				SSL_set_connect_state(_pSSL.get());
		}

		~Connection()
		{
			_pSSL.reset();
			::close(_fd);
		}

		Connection(const Connection&) = delete;
		Connection& operator = (const Connection&) = delete;

		SSL* ssl() const
		{
			return _pSSL.get();
		}

		void flush()
			/// Writes all pending output to the socket in one send().
		{
			char* pData = nullptr;
			const long length = BIO_get_mem_data(_pOutput, &pData);
			long sent = 0;
			while (sent < length)
			{
				const ssize_t n = ::send(_fd, pData + sent, static_cast<std::size_t>(length - sent), 0);
				if (n <= 0)
					throw std::runtime_error(std::string("send failed: ") + std::strerror(errno));
				sent += n;
			}
			static_cast<void>(BIO_reset(_pOutput));
		}

		template <typename Operation>
		int call(Operation operation)
			/// Repeats the SSL operation while it waits for input: the pending output
			/// is flushed and the socket is read. Returns the result of the operation,
			/// or 0 when the socket delivers no more data.
		{
			for (;;)
			{
				const int rc = operation();
				if (rc > 0 || SSL_get_error(_pSSL.get(), rc) != SSL_ERROR_WANT_READ)
					return rc;
				flush();
				if (!fill())
					return 0;
			}
		}

		bool fill()
			/// Reads from the socket into the input BIO. Returns false at the end of
			/// the stream, on a timeout and on an error.
		{
			char buffer[READ_SIZE];
			const ssize_t n = ::recv(_fd, buffer, sizeof(buffer), 0);
			if (n <= 0)
			{
				_endReason = n == 0 ? "connection closed by the partner" : std::string("recv failed: ") + std::strerror(errno);
				return false;
			}
			BIO_write(_pInput, buffer, static_cast<int>(n));
			return true;
		}

		std::string describeEnd(int rc)
			/// Describes why an SSL operation that call() returned rc for has ended.
		{
			if (!_endReason.empty())
				return _endReason;
			if (SSL_get_error(_pSSL.get(), rc) == SSL_ERROR_ZERO_RETURN)
				return "close_notify received";
			return "SSL error " + std::to_string(SSL_get_error(_pSSL.get(), rc)) + ": " + sslErrors();
		}

	private:
		int _fd;
		std::unique_ptr<SSL, decltype(&SSL_free)> _pSSL;
		BIO* _pInput = nullptr;
		BIO* _pOutput = nullptr;
		std::string _endReason;
	};

	void write(Connection& connection, const std::string& data)
		/// Encrypts the data into the output; nothing is sent.
	{
		if (SSL_write(connection.ssl(), data.data(), static_cast<int>(data.size())) != static_cast<int>(data.size()))
			throw std::runtime_error("SSL_write: " + sslErrors());
	}

	void finishRenegotiation(Connection& connection)
		/// Runs the renegotiation with the partner until it is complete. The final
		/// flight stays in the output, so that it is sent together with what follows.
	{
		SSL* pSSL = connection.ssl();
		while (SSL_renegotiate_pending(pSSL) == 1)
		{
			// the handshake messages of the partner are processed by a read
			char byte = 0;
			const int rc = SSL_peek(pSSL, &byte, 1);
			if (rc > 0)
				throw std::runtime_error("application data before the end of the renegotiation");
			if (SSL_get_error(pSSL, rc) != SSL_ERROR_WANT_READ)
				throw std::runtime_error("renegotiation failed: " + connection.describeEnd(rc));
			if (SSL_renegotiate_pending(pSSL) == 0)
				break;
			connection.flush();
			if (!connection.fill())
				throw std::runtime_error("renegotiation failed: " + connection.describeEnd(rc));
		}
	}

	void run(const Scenario& scenario, Connection& connection)
	{
		SSL* pSSL = connection.ssl();
		int rc = connection.call([pSSL]() { return SSL_do_handshake(pSSL); });
		if (rc != 1)
			throw std::runtime_error("handshake failed: " + connection.describeEnd(rc));
		connection.flush();
		log(scenario, std::string("handshake done, ") + SSL_get_version(pSSL));

		if (scenario.action == Action::KEY_UPDATE)
		{
			write(connection, "one");
			if (SSL_key_update(pSSL, SSL_KEY_UPDATE_NOT_REQUESTED) != 1 || SSL_do_handshake(pSSL) != 1)
				throw std::runtime_error("key update failed: " + sslErrors());
			write(connection, "two");
			connection.flush();
			log(scenario, "sent [one][KeyUpdate][two]");
		}
		else
		{
			if (scenario.role == Role::CLIENT)
				write(connection, "one");
			if (SSL_renegotiate(pSSL) != 1)
				throw std::runtime_error("SSL_renegotiate: " + sslErrors());
			// writes the HelloRequest or the ClientHello; the result says only whether the partner has answered
			static_cast<void>(SSL_do_handshake(pSSL));
			connection.flush();
			log(scenario, scenario.role == Role::CLIENT ? "sent [one][ClientHello]" : "sent [HelloRequest]");
			finishRenegotiation(connection);
			write(connection, "two");
			connection.flush();
			log(scenario, "renegotiation done, sent [final flight][two]");
		}

		std::size_t echoed = 0;
		for (;;)
		{
			char buffer[READ_SIZE];
			rc = connection.call([pSSL, &buffer]() { return SSL_read(pSSL, buffer, sizeof(buffer)); });
			if (rc <= 0)
				break;
			write(connection, std::string(buffer, static_cast<std::size_t>(rc)));
			connection.flush();
			echoed += static_cast<std::size_t>(rc);
		}
		log(scenario, "echoed " + std::to_string(echoed) + " bytes, end: " + connection.describeEnd(rc));
	}

	void serve(const Scenario& scenario, int listenFd, SSL_CTX* pContext)
	{
		for (;;)
		{
			sockaddr_in address{};
			socklen_t addressLength = sizeof(address);
			const int fd = ::accept(listenFd, reinterpret_cast<sockaddr*>(&address), &addressLength);
			if (fd < 0)
			{
				log(scenario, std::string("accept failed: ") + std::strerror(errno));
				continue;
			}
			char host[INET_ADDRSTRLEN] = "";
			::inet_ntop(AF_INET, &address.sin_addr, host, sizeof(host));
			log(scenario, std::string("connection from ") + host + ":" + std::to_string(ntohs(address.sin_port)));
			timeval timeout{10, 0};
			::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
			try
			{
				Connection connection(pContext, fd, scenario.role);
				run(scenario, connection);
			}
			catch (std::exception& exc)
			{
				log(scenario, exc.what());
			}
			ERR_clear_error();
		}
	}

	int listenOn(int port)
	{
		const int fd = ::socket(AF_INET, SOCK_STREAM, 0);
		if (fd < 0)
			throw std::runtime_error(std::string("socket failed: ") + std::strerror(errno));
		const int on = 1;
		::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		sockaddr_in address{};
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(static_cast<std::uint16_t>(port));
		if (::bind(fd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0 || ::listen(fd, 4) != 0)
			throw std::runtime_error("cannot listen on port " + std::to_string(port) + ": " + std::strerror(errno));
		return fd;
	}
}


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "usage: " << argv[0] << " <base port> <certificate and key PEM file>" << std::endl;
		return 1;
	}
	// a partner that resets the connection must not end the program
	std::signal(SIGPIPE, SIG_IGN);
	try
	{
		const int basePort = std::stoi(argv[1]);
		std::vector<ContextPtr> contexts;
		std::vector<int> listenFds;
		for (std::size_t i = 0; i < std::size(SCENARIOS); ++i)
		{
			contexts.push_back(createContext(SCENARIOS[i], argv[2]));
			listenFds.push_back(listenOn(basePort + static_cast<int>(i)));
		}
		std::vector<std::thread> threads;
		for (std::size_t i = 0; i < std::size(SCENARIOS); ++i)
		{
			std::cerr << SCENARIOS[i].name << " on port " << basePort + static_cast<int>(i) << std::endl;
			threads.emplace_back(serve, std::cref(SCENARIOS[i]), listenFds[i], contexts[i].get());
		}
		for (auto& thread: threads)
			thread.join();
	}
	catch (std::exception& exc)
	{
		std::cerr << exc.what() << std::endl;
		return 1;
	}
	return 0;
}
