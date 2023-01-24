//
// MulticastSocketTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MulticastSocketTest.h"


#ifdef POCO_NET_HAS_INTERFACE


#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "MulticastEchoServer.h"
#include "Poco/Net/MulticastSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/Stopwatch.h"


using Poco::Net::Socket;
using Poco::Net::MulticastSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::Timespan;
using Poco::Stopwatch;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;
using Poco::IOException;


MulticastSocketTest::MulticastSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


MulticastSocketTest::~MulticastSocketTest()
{
}


void MulticastSocketTest::testMulticast()
{
	try
	{
		MulticastEchoServer echoServer;
		MulticastSocket ms(SocketAddress::IPv4);
		SocketAddress multicastAddress("234.2.2.2", 4040);
		ms.joinGroup(multicastAddress.host());
		ms.setReceiveTimeout(Poco::Timespan(5, 0));
		int n = ms.sendTo("hello", 5, echoServer.group());
		assertTrue (n == 5);
		char buffer[256];
		n = ms.receiveBytes(buffer, sizeof(buffer));
		assertTrue (n == 5);
		assertTrue (std::string(buffer, n) == "hello");
		ms.leaveGroup(multicastAddress.host());
		ms.close();
	}
	catch (Poco::NotImplementedException&)
	{
#if POCO_OS != POCO_OS_ANDROID
		throw;
#endif
	}
}


void MulticastSocketTest::setUp()
{
}


void MulticastSocketTest::tearDown()
{
}


CppUnit::Test* MulticastSocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MulticastSocketTest");
#if (POCO_OS != POCO_OS_FREE_BSD) // TODO
	CppUnit_addTest(pSuite, MulticastSocketTest, testMulticast);
#endif
	return pSuite;
}


#endif // POCO_NET_HAS_INTERFACE
