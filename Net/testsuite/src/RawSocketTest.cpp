//
// RawSocketTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "RawSocketTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/RawSocket.h"
#include "Poco/Net/RawSocketImpl.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/Stopwatch.h"
#include <iostream>


using Poco::Net::Socket;
using Poco::Net::RawSocket;
using Poco::Net::RawSocketImpl;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::Timespan;
using Poco::Stopwatch;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;
using Poco::IOException;


bool RawSocketTest::_probed = false;
bool RawSocketTest::_available = false;


RawSocketTest::RawSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


bool RawSocketTest::rawSocketsAvailable()
{
	if (!_probed)
	{
		_probed = true;
		try
		{
			RawSocket rs(IPAddress::IPv4);
			_available = true;
		}
		catch (Poco::Exception&)
		{
			std::cout << "Raw sockets not available (requires elevated privileges), tests skipped." << std::endl;
		}
	}
	return _available;
}


RawSocketTest::~RawSocketTest()
{
}


void RawSocketTest::testEchoIPv4()
{
	if (!rawSocketsAvailable()) return;

	SocketAddress sa("127.0.0.1", 0);
	RawSocket rs(IPAddress::IPv4);
	rs.connect(sa);

	int n = rs.sendBytes("hello", 5);
	assertTrue (5 == n);

	char buffer[256] = "";
	unsigned char* ptr = (unsigned char*) buffer;

	n = rs.receiveBytes(buffer, sizeof(buffer));
	int shift = ((buffer[0] & 0x0F) * 4);
	ptr += shift;

	assertTrue (5 == (n - shift));
	assertTrue ("hello" == std::string((char*)ptr, 5));

	rs.close();
}


void RawSocketTest::testSendToReceiveFromIPv4()
{
	if (!rawSocketsAvailable()) return;

	RawSocket rs(IPAddress::IPv4);

	int n = rs.sendTo("hello", 5, SocketAddress("127.0.0.1", 0));
	assertTrue (n == 5);

	char buffer[256] = "";
	unsigned char* ptr = (unsigned char*) buffer;
	SocketAddress sa;
	n = rs.receiveFrom(buffer, sizeof(buffer), sa);
	int shift = ((buffer[0] & 0x0F) * 4);
	ptr += shift;

	assertTrue ((n - shift) == 5);
	assertTrue ("hello" == std::string((char*)ptr, 5));
	rs.close();
}


void RawSocketTest::testEchoIPv4Move()
{
	if (!rawSocketsAvailable()) return;

	SocketAddress sa("127.0.0.1", 0);
	RawSocket rs0 = RawSocket(IPAddress::IPv4);
	rs0.connect(sa);

	RawSocket rs(std::move(rs0));
#ifdef POCO_NEW_STATE_ON_MOVE
	assertTrue (rs0.impl() == nullptr);
#endif // POCO_NEW_STATE_ON_MOVE

	int n = rs.sendBytes("hello", 5);
	assertTrue (5 == n);

	char buffer[256] = "";
	unsigned char* ptr = (unsigned char*) buffer;

	n = rs.receiveBytes(buffer, sizeof(buffer));
	int shift = ((buffer[0] & 0x0F) * 4);
	ptr += shift;

	assertTrue (5 == (n - shift));
	assertTrue ("hello" == std::string((char*)ptr, 5));

	rs.close();
}


void RawSocketTest::setUp()
{
}


void RawSocketTest::tearDown()
{
}


CppUnit::Test* RawSocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RawSocketTest");

	CppUnit_addTest(pSuite, RawSocketTest, testEchoIPv4);
	CppUnit_addTest(pSuite, RawSocketTest, testSendToReceiveFromIPv4);
	CppUnit_addTest(pSuite, RawSocketTest, testEchoIPv4Move);

	return pSuite;
}
