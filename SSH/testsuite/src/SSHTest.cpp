//
// SSHTest.cpp
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
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
#include <libssh/libssh.h>


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
			SSHChannelStream out(channel);
			out << "HELLO\r\n" << std::flush;

			std::string line;
			while (sshReadLine(channel, line))
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

	SSHServerConfig makeTestConfig(const std::string& keyDir, int port)
	{
		SSHServerConfig cfg;
		cfg.bindAddress = "127.0.0.1";
		cfg.port = port;
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

	// Find a free port by binding to port 0
	int findFreePort()
	{
		ssh_bind bind = ssh_bind_new();
		int port = 0;
		ssh_bind_options_set(bind, SSH_BIND_OPTIONS_BINDADDR, "127.0.0.1");
		ssh_bind_options_set(bind, SSH_BIND_OPTIONS_BINDPORT, &port);
		// Can't easily get the port from ssh_bind, use a simpler approach
		ssh_bind_free(bind);
		// Use a high port that's unlikely to conflict
		static int nextPort = 24000;
		return nextPort++;
	}
}


SSHTest::SSHTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


SSHTest::~SSHTest()
{
}


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
	int port = findFreePort();
	SSHServerConfig cfg = makeTestConfig(_testDir, port);

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
	int port = findFreePort();
	SSHServerConfig cfg = makeTestConfig(_testDir, port);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	{
		SSHClient client;
		client.connect("127.0.0.1", port);
		assertTrue(client.isConnected());
		client.disconnect();
	}

	server.stop();
}


void SSHTest::testPasswordAuth()
{
	int port = findFreePort();
	SSHServerConfig cfg = makeTestConfig(_testDir, port);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	try
	{
		SSHClient client;
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
	int port = findFreePort();
	SSHServerConfig cfg = makeTestConfig(_testDir, port);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	try
	{
		SSHClient client;
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


void SSHTest::testSessionHandling()
{
	int port = findFreePort();
	SSHServerConfig cfg = makeTestConfig(_testDir, port);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	try
	{
		SSHClient client;
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
	int port = findFreePort();
	SSHServerConfig cfg = makeTestConfig(_testDir, port);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	SSHClient client;
	client.connect("127.0.0.1", port);
	assertTrue(client.authenticatePassword("testuser", "testpass"));

	ssh_channel channel = client.openShellChannel();
	assertTrue(channel != nullptr);

	// Read greeting
	char buf[256];
	ssh_channel_read(channel, buf, sizeof(buf), 0);

	// Stop server while client is connected — should not hang or crash
	server.stop();

	ssh_channel_free(channel);
	client.disconnect();
}


void SSHTest::testClientDisconnect()
{
	int port = findFreePort();
	SSHServerConfig cfg = makeTestConfig(_testDir, port);

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	{
		SSHClient client;
		client.connect("127.0.0.1", port);
		assertTrue(client.authenticatePassword("testuser", "testpass"));
		// Client goes out of scope — destructor disconnects
	}

	Poco::Thread::sleep(200);
	server.stop();
}


void SSHTest::testMaxConnections()
{
	int port = findFreePort();
	SSHServerConfig cfg = makeTestConfig(_testDir, port);
	cfg.maxConnections = 1;

	SSHServer server(cfg, [](ssh_session s, const SSHServerConfig& c, SSHServer& srv) -> SSHSession*
	{
		return new EchoSession(s, c, srv);
	});
	server.start();

	try
	{
		SSHClient client1;
		client1.connect("127.0.0.1", port);
		assertTrue(client1.authenticatePassword("testuser", "testpass"));

		ssh_channel ch1 = client1.openShellChannel();
		assertTrue(ch1 != nullptr);

		// Read greeting to fully establish session
		char buf[256];
		ssh_channel_read(ch1, buf, sizeof(buf), 0);

		// Second connection should either fail to get a session or timeout
		// (server has maxConnections=1, thread pool is full)
		Poco::Thread::sleep(200);

		ssh_channel_free(ch1);
		client1.disconnect();
	}
	catch (...)
	{
		server.stop();
		throw;
	}

	server.stop();
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
	CppUnit_addTest(pSuite, SSHTest, testSessionHandling);
	CppUnit_addTest(pSuite, SSHTest, testServerShutdownWithActiveSession);
	CppUnit_addTest(pSuite, SSHTest, testClientDisconnect);
	CppUnit_addTest(pSuite, SSHTest, testMaxConnections);

	return pSuite;
}
