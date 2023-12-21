//
// SocketStreamTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SocketStreamTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "EchoServer.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/Stopwatch.h"
#include "Poco/FileStream.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include <iostream>

using Poco::Net::Socket;
using Poco::Net::SocketStream;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::ConnectionRefusedException;
using Poco::Timespan;
using Poco::Stopwatch;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;
using Poco::FileInputStream;
using Poco::FileOutputStream;
using Poco::File;

SocketStreamTest::SocketStreamTest(const std::string& name): CppUnit::TestCase(name)
{
}


SocketStreamTest::~SocketStreamTest()
{
}


void SocketStreamTest::testStreamEcho()
{
	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	SocketStream str(ss);
	str << "hello";
	assertTrue (str.good());
	str.flush();
	assertTrue (str.good());
	ss.shutdownSend();

	char buffer[5];
	str.read(buffer, sizeof(buffer));
	assertTrue (str.good());
	assertTrue (str.gcount() == 5);
	assertTrue (std::string(buffer, 5) == "hello");

	ss.close();
}


void SocketStreamTest::testLargeStreamEcho()
{
	const int msgSize = 64000;
	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	SocketStream str(ss);
	ss.setSendBufferSize(msgSize);
	ss.setReceiveBufferSize(msgSize);
	std::string payload(msgSize, 'x');
	str << payload;
	assertTrue (str.good());
	str.flush();
	assertTrue (str.good());
	ss.shutdownSend();

	assertTrue (str.gcount() == 0);
	char buffer[msgSize];
	str.read(buffer, sizeof(buffer));
	assertTrue (str.good());
	assertTrue (str.gcount() == msgSize);

	ss.close();
}


void SocketStreamTest::testEOF()
{
	StreamSocket ss;
	SocketStream str(ss);
	{
		EchoServer echoServer;

		ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
		str << "hello";
		assertTrue (str.good());
		str.flush();
		assertTrue (str.good());
		ss.shutdownSend();

		char buffer[5];
		str.read(buffer, sizeof(buffer));
		assertTrue (str.good());
		assertTrue (str.gcount() == 5);
		assertTrue (std::string(buffer, 5) == "hello");
	}

	int c = str.get();
	assertTrue (c == -1);
	assertTrue (str.eof());

	ss.close();
}

void SocketStreamTest::testSendFile()
{
	const int fileSize = 64000;
	std::string payload(fileSize, 'x');
	Poco::Path testFilePath = Poco::Path::temp().append("test.sendfile.txt");
	const std::string fileName = testFilePath.toString();
	{
		File f(fileName);
		if (f.exists())
		{
			f.remove();
		}
	}
	FileOutputStream fout(fileName);
	fout << payload;
	fout.close();
	FileInputStream fin(fileName);
	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	
	SocketStream str(ss);

	Poco::UInt64 offset = 0;
	Poco::Int64 sent = 0;
	try
	{
	sent = ss.sendFile(fin);
	}
	catch (Poco::NotImplementedException &)
	{
		std::cout << "[NOT IMPLEMENTED]\n";
		return;
	}
	assertTrue(sent >= 0);
	while (sent < fileSize)
	{
		offset = sent;
		sent = ss.sendFile(fin, offset);
		assertTrue(sent >= 0);
	}
	str.flush();
	assertTrue (str.good());
	ss.shutdownSend();

	assertTrue (str.gcount() == 0);
	char buffer[fileSize];
	str.read(buffer, sizeof(buffer));
	assertTrue (str.good());
	assertTrue (str.gcount() == fileSize);

	ss.close();
	fin.close();
	File f(fileName);
	f.remove();
}

void SocketStreamTest::setUp()
{
}


void SocketStreamTest::tearDown()
{
}


CppUnit::Test* SocketStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SocketStreamTest");

	CppUnit_addTest(pSuite, SocketStreamTest, testStreamEcho);
	CppUnit_addTest(pSuite, SocketStreamTest, testLargeStreamEcho);
	CppUnit_addTest(pSuite, SocketStreamTest, testEOF);
	CppUnit_addTest(pSuite, SocketStreamTest, testSendFile);

	return pSuite;
}
