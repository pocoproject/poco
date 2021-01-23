//
// ICMPSocketTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ICMPSocketTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "UDPEchoServer.h"
#include "Poco/Net/ICMPSocket.h"
#include "Poco/Net/ICMPPacketImpl.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/Stopwatch.h"
#include <iostream>


using Poco::Net::Socket;
using Poco::Net::ICMPSocket;
using Poco::Net::ICMPPacketImpl;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::Net::ICMPException;
using Poco::Timespan;
using Poco::Stopwatch;
using Poco::TimeoutException;
using Poco::Net::NetException;
using Poco::Net::ICMPException;
using Poco::Exception;
using Poco::TimeoutException;


ICMPSocketTest::ICMPSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


ICMPSocketTest::~ICMPSocketTest()
{
}


void ICMPSocketTest::testAssign()
{
	ICMPSocket s1(IPAddress::IPv4);
	ICMPSocket s2(s1);
}

void ICMPSocketTest::testSendToReceiveFrom()
{
	ICMPSocket ss(IPAddress::IPv4);

	SocketAddress sa("www.appinf.com", 0);
	SocketAddress sr(sa);

	try
	{
		ss.receiveFrom(sa);
		fail("must throw");
	}
	catch (ICMPException&) { }
	catch (TimeoutException&) { }
	catch (Exception&) { }

	ss.sendTo(sa);
	ss.receiveFrom(sa);

	assertTrue (sr.host().toString() == sa.host().toString());
	ss.close();
}


void ICMPSocketTest::testMTU()
{
	try
	{
		Poco::UInt16 sz = ICMPPacketImpl::MAX_PAYLOAD_SIZE + 1;
		SocketAddress addr("127.0.0.1:0");
		Poco::UInt16 mtu = ICMPSocket::mtu(addr, sz);
		std::cout << addr.toString() << " : MTU=" << mtu << std::endl;
		assertTrue (mtu != 0 && mtu <= ICMPPacketImpl::MAX_PAYLOAD_SIZE);
		sz = ICMPPacketImpl::MAX_PAYLOAD_SIZE;
		mtu = ICMPSocket::mtu(addr, sz);
		std::cout << addr.toString() << " : MTU=" << mtu << std::endl;
		assertTrue (mtu != 0);
		sz = 1500;
		addr = SocketAddress("www.appinf.com:0");
		mtu = ICMPSocket::mtu(addr, sz);
		std::cout << addr.toString() << " : MTU=" << mtu << std::endl;
		assertTrue (mtu != 0 && mtu <= sz);
	}
	catch (Poco::NotImplementedException& ex)
	{
		std::cerr << ex.displayText() << std::endl;
	}
}


void ICMPSocketTest::setUp()
{
}


void ICMPSocketTest::tearDown()
{
}


CppUnit::Test* ICMPSocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ICMPSocketTest");

	CppUnit_addTest(pSuite, ICMPSocketTest, testSendToReceiveFrom);
	CppUnit_addTest(pSuite, ICMPSocketTest, testAssign);
	CppUnit_addTest(pSuite, ICMPSocketTest, testMTU);

	return pSuite;
}
