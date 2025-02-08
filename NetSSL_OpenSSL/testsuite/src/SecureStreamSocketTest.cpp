//
// SecureStreamSocketTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SecureStreamSocketTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/RejectCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Thread.h"
#include "Poco/File.h"
#include "Poco/TemporaryFile.h"
#include "Poco/FileStream.h"
#include <iostream>


using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::StreamSocket;
using Poco::Net::SecureStreamSocket;
using Poco::Net::SecureServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::Context;
using Poco::Net::Session;
using Poco::Net::SSLManager;
using Poco::Thread;
using Poco::Util::Application;


namespace
{
	class EchoConnection: public TCPServerConnection
	{
	public:
		EchoConnection(const StreamSocket& s): TCPServerConnection(s)
		{
		}

		void run()
		{
			StreamSocket& ss = socket();
			try
			{
				char buffer[256];
				int n = ss.receiveBytes(buffer, sizeof(buffer));
				while (n > 0)
				{
					ss.sendBytes(buffer, n);
					n = ss.receiveBytes(buffer, sizeof(buffer));
				}
			}
			catch (Poco::Exception& exc)
			{
				std::cerr << "EchoConnection: " << exc.displayText() << std::endl;
			}
		}
	};

	class CopyToStringConnection: public TCPServerConnection
	{
	public:
		CopyToStringConnection(const StreamSocket& s): 
			TCPServerConnection(s)
		{
		}

		void run()
		{
			{
				Poco::FastMutex::ScopedLock lock(_mutex);
				_data.clear();
			}
			StreamSocket& ss = socket();
			try
			{
				char buffer[256];
				int n = ss.receiveBytes(buffer, sizeof(buffer));
				while (n > 0)
				{
					{
						Poco::FastMutex::ScopedLock lock(_mutex);
						_data.append(buffer, n);
					}
					n = ss.receiveBytes(buffer, sizeof(buffer));
				}
			}
			catch (Poco::Exception& exc)
			{
				std::cerr << "CopyToStringConnection: " << exc.displayText() << std::endl;
			}
		}

		static const std::string data()
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _data;
		}

	private:
		static Poco::FastMutex _mutex;
		static std::string _data;
	};

	Poco::FastMutex CopyToStringConnection::_mutex;
	std::string CopyToStringConnection::_data;
}


SecureStreamSocketTest::SecureStreamSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


SecureStreamSocketTest::~SecureStreamSocketTest()
{
}


void SecureStreamSocketTest::testSendReceive()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa);
	std::string data("hello, world");
	ss1.sendBytes(data.data(), static_cast<int>(data.size()));
	char buffer[8192];
	int rc = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (rc > 0);
	assertTrue (std::string(buffer, rc) == data);

	const std::vector<std::size_t> sizes = {67, 467, 7883, 19937};
	for (const auto n: sizes)
	{
		data.assign(n, 'X');
		ss1.sendBytes(data.data(), static_cast<int>(data.size()));
		std::string received;
		while (received.size() < n)
		{
			rc = ss1.receiveBytes(buffer, sizeof(buffer));
			if (rc > 0)
			{
				received.append(buffer, rc);
			}
			else if (rc == 0)
			{
				break;
			}
		}
		assertTrue (received == data);
	}

	ss1.close();
}


void SecureStreamSocketTest::testPeek()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss(sa);
	
	int n = ss.sendBytes("hello, world!", 13);
	assertTrue (n == 13);
	char buffer[256];
	n = ss.receiveBytes(buffer, 5, MSG_PEEK);
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");
	n = ss.receiveBytes(buffer, sizeof(buffer), MSG_PEEK);
	assertTrue (n == 13);
	assertTrue (std::string(buffer, n) == "hello, world!");
	n = ss.receiveBytes(buffer, 7);
	assertTrue (n == 7);
	assertTrue (std::string(buffer, n) == "hello, ");
	n = ss.receiveBytes(buffer, 6);
	assertTrue (n == 6);
	assertTrue (std::string(buffer, n) == "world!");
	ss.close();
}


void SecureStreamSocketTest::testNB()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa);
	ss1.setBlocking(false);
	ss1.setSendBufferSize(32000);

	char buffer[8192];
	const std::vector<std::size_t> sizes = {67, 467, 7883, 19937};
	for (const auto s: sizes)
	{
		std::string data(s, 'X');
		ss1.sendBytes(data.data(), static_cast<int>(data.size()));

		int rc;
		do
		{
			rc = ss1.receiveBytes(buffer, sizeof(buffer), MSG_PEEK);
		}
		while (rc < 0);
		assertTrue (rc > 0 && rc <= s);
		assertTrue (data.compare(0, rc, buffer, rc) == 0);

		std::string received;
		while (received.size() < s)
		{
			rc = ss1.receiveBytes(buffer, sizeof(buffer));
			if (rc > 0)
			{
				received.append(buffer, rc);
			}
			else if (rc == 0)
			{
				break;
			}
		}
		assertTrue (received == data);
	}

	ss1.close();
}


void SecureStreamSocketTest::testSendFile()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<CopyToStringConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", srv.port()));

	std::string sentData = "Hello, world!";

	Poco::TemporaryFile file;
	Poco::FileOutputStream ostr(file.path());
	ostr.write(sentData.data(), sentData.size());
	ostr.close();
	
	Poco::FileInputStream istr(file.path());
	std::streamsize n = ss.sendFile(istr);
	assertTrue (n == file.getSize());

	istr.close();
	ss.close();

	Poco::Thread::sleep(200);
	while (srv.currentConnections() > 0) 
	{
		Poco::Thread::sleep(100);
	}
	srv.stop();

	assertTrue (CopyToStringConnection::data() == sentData);
}


void SecureStreamSocketTest::testSendFileLarge()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<CopyToStringConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", srv.port()));

	std::string sentData;

	Poco::TemporaryFile file;
	Poco::FileOutputStream ostr(file.path());
	std::string data("0123456789abcdef");
	for (int i = 0; i < 10000; i++)
	{
		ostr.write(data.data(), data.size());
		sentData += data;
	}
	ostr.close();
	
	Poco::FileInputStream istr(file.path());
	std::streamsize n = ss.sendFile(istr);
	assertTrue (n == file.getSize());

	istr.close();
	ss.close();

	Poco::Thread::sleep(200);
	while (srv.currentConnections() > 0) 
	{
		Poco::Thread::sleep(100);
	}
	srv.stop();

	assertTrue (CopyToStringConnection::data() == sentData);
}


void SecureStreamSocketTest::testSendFileRange()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<CopyToStringConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", srv.port()));

	std::string fileData;

	Poco::TemporaryFile file;
	Poco::FileOutputStream ostr(file.path());
	std::string data("0123456789abcdef");
	for (int i = 0; i < 1024; i++)
	{
		ostr.write(data.data(), data.size());
		fileData += data;
	}
	ostr.close();
	
	const std::streamoff offset = 4000;
	const std::streamsize count = 10000;

	Poco::FileInputStream istr(file.path());
	std::streamsize n = ss.sendFile(istr, offset, count);
	assertTrue (n == count);

	istr.close();
	ss.close();

	Poco::Thread::sleep(200);
	while (srv.currentConnections() > 0) 
	{
		Poco::Thread::sleep(100);
	}
	srv.stop();

	assertTrue (CopyToStringConnection::data() == fileData.substr(offset, count));
}


void SecureStreamSocketTest::setUp()
{
}


void SecureStreamSocketTest::tearDown()
{
}


CppUnit::Test* SecureStreamSocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SecureStreamSocketTest");

	CppUnit_addTest(pSuite, SecureStreamSocketTest, testSendReceive);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testPeek);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testNB);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testSendFile);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testSendFileLarge);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testSendFileRange);

	return pSuite;
}
