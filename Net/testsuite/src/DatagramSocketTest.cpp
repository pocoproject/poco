//
// DatagramSocketTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DatagramSocketTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "UDPEchoServer.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/Buffer.h"
#include "Poco/Stopwatch.h"
#include "Poco/Thread.h"
#include <cstring>


using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
#ifdef POCO_NET_HAS_INTERFACE
	using Poco::Net::NetworkInterface;
#endif
using Poco::Timespan;
using Poco::Buffer;
using Poco::Stopwatch;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;
using Poco::IOException;
using Poco::Thread;


DatagramSocketTest::DatagramSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


DatagramSocketTest::~DatagramSocketTest()
{
}


void DatagramSocketTest::testEcho()
{
	UDPEchoServer echoServer;
	DatagramSocket ss;
	char buffer[256];
	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	int n = ss.sendBytes("hello", 5);
	assertTrue (n == 5);
	n = ss.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");
	ss.close();
}


void DatagramSocketTest::testMoveDatagramSocket()
{
	UDPEchoServer echoServer;
	DatagramSocket ss0 = DatagramSocket();
	char buffer[256];
	ss0.connect(SocketAddress("127.0.0.1", echoServer.port()));
	DatagramSocket ss(std::move(ss0));
#if POCO_NEW_STATE_ON_MOVE
	assertTrue (ss0.isNull());
#else
	assertFalse (ss0.isNull());
#endif
	int n = ss.sendBytes("hello", 5);
	assertTrue (n == 5);
	n = ss.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");

	std::memset(buffer, 0, sizeof(buffer));
	ss0 = ss;
	assertTrue (ss0.impl());
	assertTrue (ss.impl());
	assertTrue (ss0.impl() == ss.impl());
	ss = std::move(ss0);
#if POCO_NEW_STATE_ON_MOVE
	assertTrue (ss0.isNull());
#else
	assertFalse (ss0.isNull());
#endif
	assertTrue (ss.impl());
	n = ss.sendBytes("hello", 5);
	assertTrue (n == 5);
	n = ss.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");
	ss.close();
	ss0.close();
}


void DatagramSocketTest::testEchoBuffer()
{
	UDPEchoServer echoServer;
	DatagramSocket ss;
	Buffer<char> buffer(0);
	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	int n = ss.receiveBytes(buffer);
	assertTrue (n == 0);
	assertTrue (buffer.size() == 0);
	n = ss.sendBytes("hello", 5);
	assertTrue (n == 5);
	n = ss.receiveBytes(buffer);
	assertTrue (n == 5);
	assertTrue (buffer.size() == 5);
	assertTrue (std::string(buffer.begin(), n) == "hello");
	ss.close();
}


void DatagramSocketTest::testReceiveFromAvailable()
{
	UDPEchoServer echoServer(SocketAddress("127.0.0.1", 0));
	DatagramSocket ss(SocketAddress::IPv4);
	int n = ss.sendTo("hello", 5, SocketAddress("127.0.0.1", echoServer.port()));
	assertTrue (n == 5);
	Thread::sleep(100);
	char buffer[256];
	SocketAddress sa;
	assertTrue (ss.available() == 5);
	n = ss.receiveFrom(buffer, sizeof(buffer), sa);
	assertTrue (sa.host() == echoServer.address().host());
	assertTrue (sa.port() == echoServer.port());
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");
	ss.close();
}


void DatagramSocketTest::testSendToReceiveFrom()
{
	UDPEchoServer echoServer(SocketAddress("127.0.0.1", 0));
	DatagramSocket ss(SocketAddress::IPv4);
	int n = ss.sendTo("hello", 5, SocketAddress("127.0.0.1", echoServer.port()));
	assertTrue (n == 5);
	char buffer[256];
	SocketAddress sa;
	n = ss.receiveFrom(buffer, sizeof(buffer), sa);
	assertTrue (sa.host() == echoServer.address().host());
	assertTrue (sa.port() == echoServer.port());
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");
	ss.close();
}


void DatagramSocketTest::testUnbound()
{
	UDPEchoServer echoServer;
	DatagramSocket ss;
	char buffer[256];
	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	int n = ss.sendBytes("hello", 5);
	assertTrue (n == 5);
	n = ss.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");
	ss.close();
}


Poco::UInt16 DatagramSocketTest::getFreePort(SocketAddress::Family family, Poco::UInt16 port)
{
	bool failed = false;
	poco_assert_dbg(port > 0);
	--port;
	DatagramSocket sock(family);
	do
	{
		failed = false;
		SocketAddress sa(family, ++port);
		try
		{
			sock.bind(sa, false);
		}
		catch (Poco::Net::NetException&)
		{
			failed = true;
		}
	} while (failed && sock.getError() == POCO_EADDRINUSE);
	return port;
}


void DatagramSocketTest::testReuseAddressPortWildcard()
{
	Poco::UInt16 port = getFreePort(SocketAddress::IPv4, 1234);
	Poco::UInt16 port6 = getFreePort(SocketAddress::IPv6, 1234);
	assertTrue(port >= 1234);
	assertTrue(port6 >= 1234);

	// reuse
	{
		DatagramSocket ds1(SocketAddress::IPv4);
		ds1.bind(SocketAddress(port), true);
		assertTrue(ds1.getReuseAddress());
		DatagramSocket ds2;
		ds2.bind(SocketAddress(port), true);
		assertTrue(ds2.getReuseAddress());
#ifdef POCO_HAVE_IPv6
		DatagramSocket ds3(SocketAddress::IPv6);
		ds3.bind6(SocketAddress(SocketAddress::IPv6, port6), true, true, false);
		assertTrue(ds3.getReuseAddress());
#endif
	}

#ifdef POCO_HAVE_IPv6
	{
		DatagramSocket ds1(SocketAddress::IPv6);
		ds1.bind6(SocketAddress(SocketAddress::IPv6, port6), true, true, false);
		assertTrue(ds1.getReuseAddress());
		DatagramSocket ds2;
		ds2.bind6(SocketAddress(SocketAddress::IPv6, port6), true, true, false);
		assertTrue(ds2.getReuseAddress());
		DatagramSocket ds3;
		ds3.bind(SocketAddress(port), true, true);
		assertTrue(ds3.getReuseAddress());
	}
#endif

#ifdef POCO_HAVE_IPv6
	{
		DatagramSocket ds1(SocketAddress::IPv6);
		ds1.bind6(SocketAddress(SocketAddress::IPv6, port), true, true, true);
		assertTrue(ds1.getReuseAddress());
		DatagramSocket ds2;
		ds2.bind6(SocketAddress(SocketAddress::IPv6, port), true, true, true);
		assertTrue(ds2.getReuseAddress());
	}
#endif

	// not reuse
	{
		DatagramSocket ds1(SocketAddress::IPv4);
		ds1.bind(SocketAddress(port), false);
		assertTrue(!ds1.getReuseAddress());
		DatagramSocket ds2;
		try
		{
			ds2.bind(SocketAddress(port), false);
			fail("binding to non-reuse address must throw");
		}
		catch (Poco::IOException&) {}

#ifdef POCO_HAVE_IPv6
		{
			DatagramSocket ds1(SocketAddress::IPv6);
			ds1.bind6(SocketAddress(SocketAddress::IPv6, port), false, false, true);
			assertTrue(!ds1.getReuseAddress());
			DatagramSocket ds2(SocketAddress::IPv6);
			try
			{
				ds2.bind6(SocketAddress(SocketAddress::IPv6, port), false, false, true);
				fail("binding to non-reuse address must throw");
			}
			catch (Poco::IOException&) {}
		}
#endif
	}
}


void DatagramSocketTest::testReuseAddressPortSpecific()
{
	Poco::UInt16 port = getFreePort(SocketAddress::IPv4, 1234);
	assertTrue(port >= 1234);

	// reuse
	{
		DatagramSocket ds1(SocketAddress::IPv4);
		ds1.bind(SocketAddress(port), true);
		assertTrue(ds1.getReuseAddress());
		DatagramSocket ds2;
		ds2.bind(SocketAddress("127.0.0.1", port), true);
		assertTrue(ds2.getReuseAddress());
#ifdef POCO_HAVE_IPv6
		DatagramSocket ds3(SocketAddress::IPv6);
		ds3.bind6(SocketAddress("::1", port), true, true, false);
		assertTrue(ds3.getReuseAddress());
#endif
	}

	// not reuse
	{
		DatagramSocket ds1(SocketAddress::IPv4);
		ds1.bind(SocketAddress("0.0.0.0", port), false);
		assertTrue(!ds1.getReuseAddress());
		DatagramSocket ds2;
		try
		{
			ds2.bind(SocketAddress("127.0.0.1", port), false);
			fail("binding to non-reuse IPv4 address must throw");
		}
		catch (Poco::IOException&) {}

#ifdef POCO_HAVE_IPv6
		{
			DatagramSocket ds1(SocketAddress::IPv6);
			ds1.bind6(SocketAddress("::", port), false, false, true);
			assertTrue(!ds1.getReuseAddress());
			DatagramSocket ds2(SocketAddress::IPv6);
			try
			{
				ds2.bind6(SocketAddress("::1", port), false, false, true);
				fail("binding to non-reuse IPv6 address must throw");
			}
			catch (Poco::IOException&) {}
		}
#endif
	}
}


void DatagramSocketTest::testBroadcast()
{
	UDPEchoServer echoServer;
	DatagramSocket ss(IPAddress::IPv4);

#if defined(POCO_NET_HAS_INTERFACE) && (POCO_OS == POCO_OS_FREE_BSD)
	NetworkInterface ni = NetworkInterface::forName("em0");
	SocketAddress sa(ni.broadcastAddress(1), echoServer.port());
#else
	SocketAddress sa("255.255.255.255", echoServer.port());
#endif
	// not all socket implementations fail if broadcast option is not set
/*
	try
	{
		int n = ss.sendTo("hello", 5, sa);
		fail ("broadcast option not set - must throw");
	}
	catch (IOException&)
	{
	}
*/
	ss.setBroadcast(true);

#if (POCO_OS == POCO_OS_FREE_BSD)
	int opt = 1;
	poco_socklen_t len = sizeof(opt);
	ss.impl()->setRawOption(IPPROTO_IP, IP_ONESBCAST, (const char*) &opt, len);
	ss.impl()->getRawOption(IPPROTO_IP, IP_ONESBCAST, &opt, len);
	assertTrue (opt == 1);
#endif

	int n = ss.sendTo("hello", 5, sa);
	assertTrue (n == 5);
	char buffer[256] = { 0 };
	n = ss.receiveBytes(buffer, 5);
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");
	ss.close();
}


void DatagramSocketTest::testGatherScatterFixed()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	testGatherScatterFixedWin();
	testGatherScatterSTRFFixedWin();
#elif defined(POCO_OS_FAMILY_UNIX)
	testGatherScatterFixedUNIX();
	testGatherScatterSTRFFixedUNIX();
#endif
}


void DatagramSocketTest::testGatherScatterFixedWin()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	UDPEchoServer echoServer;
	DatagramSocket ss;
	Socket::BufVec sbv = Socket::makeBufVec(3, 10);
	assertTrue (sbv.size() == 3);

	std::memcpy(sbv[0].buf, "1234567890", 10);
	std::memcpy(sbv[1].buf, "abcdefghij", 10);
	std::memcpy(sbv[2].buf, "helloworld", 10);

	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	int n = ss.sendBytes(sbv);
	assertTrue (n == 30);

	std::memset(sbv[0].buf, 0, 10);
	std::memset(sbv[1].buf, 0, 10);
	std::memset(sbv[2].buf, 0, 10);

	char empty[10] = {};
	assertTrue (0 == std::memcmp(sbv[0].buf, empty, 10));
	assertTrue (0 == std::memcmp(sbv[1].buf, empty, 10));
	assertTrue (0 == std::memcmp(sbv[2].buf, empty, 10));

	n = ss.receiveBytes(sbv);
	assertTrue (n == 30);

	assertTrue (0 == std::memcmp(sbv[0].buf, "1234567890", 10));
	assertTrue (0 == std::memcmp(sbv[1].buf, "abcdefghij", 10));
	assertTrue (0 == std::memcmp(sbv[2].buf, "helloworld", 10));

	Socket::destroyBufVec(sbv);

	ss.close();
#endif
}


void DatagramSocketTest::testGatherScatterSTRFFixedWin()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	UDPEchoServer echoServer(SocketAddress("127.0.0.1", 0));
	DatagramSocket ss;
	Socket::BufVec sbv = Socket::makeBufVec(3, 10);
	assertTrue (sbv.size() == 3);

	std::memcpy(sbv[0].buf, "1234567890", 10);
	std::memcpy(sbv[1].buf, "abcdefghij", 10);
	std::memcpy(sbv[2].buf, "helloworld", 10);

	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	int n = ss.sendTo(sbv, SocketAddress("127.0.0.1", echoServer.port()));
	assertTrue (n == 30);

	std::memset(sbv[0].buf, 0, 10);
	std::memset(sbv[1].buf, 0, 10);
	std::memset(sbv[2].buf, 0, 10);

	char empty[10] = {};
	assertTrue (0 == std::memcmp(sbv[0].buf, empty, 10));
	assertTrue (0 == std::memcmp(sbv[1].buf, empty, 10));
	assertTrue (0 == std::memcmp(sbv[2].buf, empty, 10));

	SocketAddress sa;
	n = ss.receiveFrom(sbv, sa);
	assertTrue (sa.host() == echoServer.address().host());
	assertTrue (sa.port() == echoServer.port());
	assertTrue (n == 30);

	assertTrue (0 == std::memcmp(sbv[0].buf, "1234567890", 10));
	assertTrue (0 == std::memcmp(sbv[1].buf, "abcdefghij", 10));
	assertTrue (0 == std::memcmp(sbv[2].buf, "helloworld", 10));

	Socket::destroyBufVec(sbv);

	ss.close();
#endif
}


void DatagramSocketTest::testGatherScatterFixedUNIX()
{
#if defined(POCO_OS_FAMILY_UNIX)
	UDPEchoServer echoServer;
	DatagramSocket ss;
	Socket::BufVec sbv = Socket::makeBufVec(3, 10);
	assertTrue (sbv.size() == 3);

	std::memcpy(sbv[0].iov_base, "1234567890", 10);
	std::memcpy(sbv[1].iov_base, "abcdefghij", 10);
	std::memcpy(sbv[2].iov_base, "helloworld", 10);

	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	int n = ss.sendBytes(sbv);
	assertTrue (n == 30);

	std::memset(sbv[0].iov_base, 0, 10);
	std::memset(sbv[1].iov_base, 0, 10);
	std::memset(sbv[2].iov_base, 0, 10);

	char empty[10] = {};
	assertTrue (0 == std::memcmp(sbv[0].iov_base, empty, 10));
	assertTrue (0 == std::memcmp(sbv[1].iov_base, empty, 10));
	assertTrue (0 == std::memcmp(sbv[2].iov_base, empty, 10));

	n = ss.receiveBytes(sbv);
	assertTrue (n == 30);

	assertTrue (0 == std::memcmp(sbv[0].iov_base, "1234567890", 10));
	assertTrue (0 == std::memcmp(sbv[1].iov_base, "abcdefghij", 10));
	assertTrue (0 == std::memcmp(sbv[2].iov_base, "helloworld", 10));

	Socket::destroyBufVec(sbv);

	ss.close();
#endif
}


void DatagramSocketTest::testGatherScatterSTRFFixedUNIX()
{
#if defined(POCO_OS_FAMILY_UNIX)
	UDPEchoServer echoServer(SocketAddress("127.0.0.1", 0));
	DatagramSocket ss;
	Socket::BufVec sbv = Socket::makeBufVec(3, 10);
	assertTrue (sbv.size() == 3);

	std::memcpy(sbv[0].iov_base, "1234567890", 10);
	std::memcpy(sbv[1].iov_base, "abcdefghij", 10);
	std::memcpy(sbv[2].iov_base, "helloworld", 10);

	int n = ss.sendTo(sbv, SocketAddress("127.0.0.1", echoServer.port()));
	assertTrue (n == 30);

	std::memset(sbv[0].iov_base, 0, 10);
	std::memset(sbv[1].iov_base, 0, 10);
	std::memset(sbv[2].iov_base, 0, 10);

	char empty[10] = {};
	assertTrue (0 == std::memcmp(sbv[0].iov_base, empty, 10));
	assertTrue (0 == std::memcmp(sbv[1].iov_base, empty, 10));
	assertTrue (0 == std::memcmp(sbv[2].iov_base, empty, 10));

	SocketAddress sa;
	n = ss.receiveFrom(sbv, sa);
	assertTrue (sa.host() == echoServer.address().host());
	assertTrue (sa.port() == echoServer.port());
	assertTrue (n == 30);

	assertTrue (0 == std::memcmp(sbv[0].iov_base, "1234567890", 10));
	assertTrue (0 == std::memcmp(sbv[1].iov_base, "abcdefghij", 10));
	assertTrue (0 == std::memcmp(sbv[2].iov_base, "helloworld", 10));

	Socket::destroyBufVec(sbv);

	ss.close();
#endif
}


void DatagramSocketTest::testGatherScatterVariable()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	testGatherScatterVariableWin();
	testGatherScatterSTRFVariableWin();
#elif defined(POCO_OS_FAMILY_UNIX)
	testGatherScatterVariableUNIX();
	testGatherScatterSTRFVariableUNIX();
#endif
}


void DatagramSocketTest::testGatherScatterVariableWin()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	UDPEchoServer echoServer(SocketAddress("127.0.0.1", 0));
	DatagramSocket ss;
	std::vector<std::string> strOut(3);
	strOut[0] = "123";
	strOut[1] = "abcdef";
	strOut[2] = "helloworld";
	Socket::BufVec sbvOut = Socket::makeBufVec(strOut);
	assertTrue (sbvOut.size() == 3);
	assertTrue (sbvOut[0].len == 3);
	assertTrue (sbvOut[1].len == 6);
	assertTrue (sbvOut[2].len == 10);
	assertTrue (0 == std::memcmp(sbvOut[0].buf, "123", 3));
	assertTrue (0 == std::memcmp(sbvOut[1].buf, "abcdef", 6));
	assertTrue (0 == std::memcmp(sbvOut[2].buf, "helloworld", 10));

	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	int n = ss.sendTo(sbvOut, SocketAddress("127.0.0.1", echoServer.port()));
	assertTrue (n == 19);

	std::vector<char*> strIn(3);
	strIn[0] = (char*) calloc(4, 1);
	strIn[1] = (char*) calloc(7, 1);
	strIn[2] = (char*) calloc(11, 1);
	std::memcpy(strIn[0], "321", 3);
	std::memcpy(strIn[1], "fedcba", 6);
	std::memcpy(strIn[2], "dlrowolleh", 10);
	Socket::BufVec sbvIn = Socket::makeBufVec(strIn);
	assertTrue (sbvIn.size() == 3);
	assertTrue (sbvIn[0].len == 3);
	assertTrue (sbvIn[1].len == 6);
	assertTrue (sbvIn[2].len == 10);
	assertTrue (0 == std::memcmp(sbvIn[0].buf, "321", 3));
	assertTrue (0 == std::memcmp(sbvIn[1].buf, "fedcba", 6));
	assertTrue (0 == std::memcmp(sbvIn[2].buf, "dlrowolleh", 10));

	SocketAddress sa;
	n = ss.receiveFrom(sbvIn, sa);
	assertTrue (sa.host() == echoServer.address().host());
	assertTrue (sa.port() == echoServer.port());
	assertTrue (n == 19);

	assertTrue (0 == std::memcmp(sbvIn[0].buf, "123", 3));
	assertTrue (0 == std::memcmp(sbvIn[1].buf, "abcdef", 6));
	assertTrue (0 == std::memcmp(sbvIn[2].buf, "helloworld", 10));

	n = ss.sendBytes(sbvOut);
	assertTrue (n == 19);

	std::reverse(sbvIn.begin(), sbvIn.end());
	n = ss.receiveBytes(sbvIn);
	assertTrue (n == 19);

	assertTrue (0 == std::memcmp(sbvIn[0].buf, "123abcdefh", 10));
	assertTrue (0 == std::memcmp(sbvIn[1].buf, "ellowo", 6));
	assertTrue (0 == std::memcmp(sbvIn[2].buf, "rld", 3));

	free(strIn[0]);
	free(strIn[1]);
	free(strIn[2]);

	ss.close();
#endif
}


void DatagramSocketTest::testGatherScatterSTRFVariableWin()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	UDPEchoServer echoServer;
	DatagramSocket ss;
	std::vector<std::string> strOut(3);
	strOut[0] = "123";
	strOut[1] = "abcdef";
	strOut[2] = "helloworld";
	Socket::BufVec sbvOut = Socket::makeBufVec(strOut);
	assertTrue (sbvOut.size() == 3);
	assertTrue (sbvOut[0].len == 3);
	assertTrue (sbvOut[1].len == 6);
	assertTrue (sbvOut[2].len == 10);
	assertTrue (0 == std::memcmp(sbvOut[0].buf, "123", 3));
	assertTrue (0 == std::memcmp(sbvOut[1].buf, "abcdef", 6));
	assertTrue (0 == std::memcmp(sbvOut[2].buf, "helloworld", 10));

	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	int n = ss.sendBytes(sbvOut);
	assertTrue (n == 19);

	std::vector<char*> strIn(3);
	strIn[0] = new char[4];
	strIn[1] = new char[7];
	strIn[2] = new char[11];
	std::memcpy(strIn[0], "321", 3); strIn[0][3] = '\0';
	std::memcpy(strIn[1], "fedcba", 6); strIn[1][6] = '\0';
	std::memcpy(strIn[2], "dlrowolleh", 10); strIn[2][10] = '\0';
	Socket::BufVec sbvIn = Socket::makeBufVec(strIn);
	assertTrue (sbvIn.size() == 3);
	assertTrue (sbvIn[0].len == 3);
	assertTrue (sbvIn[1].len == 6);
	assertTrue (sbvIn[2].len == 10);
	assertTrue (0 == std::memcmp(sbvIn[0].buf, "321", 3));
	assertTrue (0 == std::memcmp(sbvIn[1].buf, "fedcba", 6));
	assertTrue (0 == std::memcmp(sbvIn[2].buf, "dlrowolleh", 10));

	n = ss.receiveBytes(sbvIn);
	assertTrue (n == 19);

	assertTrue (0 == std::memcmp(sbvIn[0].buf, "123", 3));
	assertTrue (0 == std::memcmp(sbvIn[1].buf, "abcdef", 6));
	assertTrue (0 == std::memcmp(sbvIn[2].buf, "helloworld", 10));

	n = ss.sendBytes(sbvOut);
	assertTrue (n == 19);

	std::reverse(sbvIn.begin(), sbvIn.end());
	n = ss.receiveBytes(sbvIn);
	assertTrue (n == 19);

	assertTrue (0 == std::memcmp(sbvIn[0].buf, "123abcdefh", 10));
	assertTrue (0 == std::memcmp(sbvIn[1].buf, "ellowo", 6));
	assertTrue (0 == std::memcmp(sbvIn[2].buf, "rld", 3));

	delete [] strIn[0];
	delete [] strIn[1];
	delete [] strIn[2];

	ss.close();
#endif
}


void DatagramSocketTest::testGatherScatterSTRFVariableUNIX()
{
#if defined(POCO_OS_FAMILY_UNIX)
	UDPEchoServer echoServer;
	DatagramSocket ss;
	std::vector<std::string> strOut(3);
	strOut[0] = "123";
	strOut[1] = "abcdef";
	strOut[2] = "helloworld";
	Socket::BufVec sbvOut = Socket::makeBufVec(strOut);
	assertTrue (sbvOut.size() == 3);
	assertTrue (sbvOut[0].iov_len == 3);
	assertTrue (sbvOut[1].iov_len == 6);
	assertTrue (sbvOut[2].iov_len == 10);
	assertTrue (0 == std::memcmp(sbvOut[0].iov_base, "123", 3));
	assertTrue (0 == std::memcmp(sbvOut[1].iov_base, "abcdef", 6));
	assertTrue (0 == std::memcmp(sbvOut[2].iov_base, "helloworld", 10));

	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	int n = ss.sendBytes(sbvOut);
	assertTrue (n == 19);

	std::vector<char*> strIn(3);
	strIn[0] = new char[4];
	strIn[1] = new char[7];
	strIn[2] = new char[11];
	std::memcpy(strIn[0], "321", 3); strIn[0][3] = '\0';
	std::memcpy(strIn[1], "fedcba", 6); strIn[1][6] = '\0';
	std::memcpy(strIn[2], "dlrowolleh", 10); strIn[2][10] = '\0';
	Socket::BufVec sbvIn = Socket::makeBufVec(strIn);
	assertTrue (sbvIn.size() == 3);
	assertTrue (sbvIn[0].iov_len == 3);
	assertTrue (sbvIn[1].iov_len == 6);
	assertTrue (sbvIn[2].iov_len == 10);
	assertTrue (0 == std::memcmp(sbvIn[0].iov_base, "321", 3));
	assertTrue (0 == std::memcmp(sbvIn[1].iov_base, "fedcba", 6));
	assertTrue (0 == std::memcmp(sbvIn[2].iov_base, "dlrowolleh", 10));

	n = ss.receiveBytes(sbvIn);
	assertTrue (n == 19);

	assertTrue (0 == std::memcmp(sbvIn[0].iov_base, "123", 3));
	assertTrue (0 == std::memcmp(sbvIn[1].iov_base, "abcdef", 6));
	assertTrue (0 == std::memcmp(sbvIn[2].iov_base, "helloworld", 10));

	n = ss.sendBytes(sbvOut);
	assertTrue (n == 19);

	std::reverse(sbvIn.begin(), sbvIn.end());
	n = ss.receiveBytes(sbvIn);
	assertTrue (n == 19);

	assertTrue (0 == std::memcmp(sbvIn[0].iov_base, "123abcdefh", 10));
	assertTrue (0 == std::memcmp(sbvIn[1].iov_base, "ellowo", 6));
	assertTrue (0 == std::memcmp(sbvIn[2].iov_base, "rld", 3));

	delete [] strIn[0];
	delete [] strIn[1];
	delete [] strIn[2];

	ss.close();
#endif
}


void DatagramSocketTest::testGatherScatterVariableUNIX()
{
#if defined(POCO_OS_FAMILY_UNIX)
	UDPEchoServer echoServer;
	DatagramSocket ss;
	std::vector<std::string> strOut(3);
	strOut[0] = "123";
	strOut[1] = "abcdef";
	strOut[2] = "helloworld";
	Socket::BufVec sbvOut = Socket::makeBufVec(strOut);
	assertTrue (sbvOut.size() == 3);
	assertTrue (sbvOut[0].iov_len == 3);
	assertTrue (sbvOut[1].iov_len == 6);
	assertTrue (sbvOut[2].iov_len == 10);
	assertTrue (0 == std::memcmp(sbvOut[0].iov_base, "123", 3));
	assertTrue (0 == std::memcmp(sbvOut[1].iov_base, "abcdef", 6));
	assertTrue (0 == std::memcmp(sbvOut[2].iov_base, "helloworld", 10));

	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	int n = ss.sendBytes(sbvOut);
	assertTrue (n == 19);

	std::vector<char*> strIn(3);
	strIn[0] = new char[4];
	strIn[1] = new char[7];
	strIn[2] = new char[11];
	std::memcpy(strIn[0], "321", 3); strIn[0][3] = '\0';
	std::memcpy(strIn[1], "fedcba", 6); strIn[1][6] = '\0';
	std::memcpy(strIn[2], "dlrowolleh", 10); strIn[2][10] = '\0';
	Socket::BufVec sbvIn = Socket::makeBufVec(strIn);
	assertTrue (sbvIn.size() == 3);
	assertTrue (sbvIn[0].iov_len == 3);
	assertTrue (sbvIn[1].iov_len == 6);
	assertTrue (sbvIn[2].iov_len == 10);
	assertTrue (0 == std::memcmp(sbvIn[0].iov_base, "321", 3));
	assertTrue (0 == std::memcmp(sbvIn[1].iov_base, "fedcba", 6));
	assertTrue (0 == std::memcmp(sbvIn[2].iov_base, "dlrowolleh", 10));

	n = ss.receiveBytes(sbvIn);
	assertTrue (n == 19);

	assertTrue (0 == std::memcmp(sbvIn[0].iov_base, "123", 3));
	assertTrue (0 == std::memcmp(sbvIn[1].iov_base, "abcdef", 6));
	assertTrue (0 == std::memcmp(sbvIn[2].iov_base, "helloworld", 10));

	delete [] strIn[0];
	delete [] strIn[1];
	delete [] strIn[2];

	ss.close();
#endif
}


void DatagramSocketTest::setUp()
{
}


void DatagramSocketTest::tearDown()
{
}


CppUnit::Test* DatagramSocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DatagramSocketTest");

	CppUnit_addTest(pSuite, DatagramSocketTest, testEcho);
	CppUnit_addTest(pSuite, DatagramSocketTest, testMoveDatagramSocket);
	CppUnit_addTest(pSuite, DatagramSocketTest, testEchoBuffer);
	CppUnit_addTest(pSuite, DatagramSocketTest, testReceiveFromAvailable);
	CppUnit_addTest(pSuite, DatagramSocketTest, testSendToReceiveFrom);
	CppUnit_addTest(pSuite, DatagramSocketTest, testUnbound);
	CppUnit_addTest(pSuite, DatagramSocketTest, testReuseAddressPortWildcard);
	CppUnit_addTest(pSuite, DatagramSocketTest, testReuseAddressPortSpecific);
#if (POCO_OS != POCO_OS_FREE_BSD) // works only with local net bcast and very randomly
	CppUnit_addTest(pSuite, DatagramSocketTest, testBroadcast);
#endif
	CppUnit_addTest(pSuite, DatagramSocketTest, testGatherScatterFixed);
	CppUnit_addTest(pSuite, DatagramSocketTest, testGatherScatterVariable);

	return pSuite;
}
