//
// ICMPClientTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ICMPClientTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/ICMPSocket.h"
#include "Poco/Net/ICMPClient.h"
#include "Poco/Net/ICMPEventArgs.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/AutoPtr.h"
#include "Poco/Delegate.h"
#include <sstream>
#include <iostream>


using Poco::Net::ICMPSocket;
using Poco::Net::ICMPClient;
using Poco::Net::ICMPEventArgs;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::Net::HostNotFoundException;
using Poco::Delegate;
using Poco::AutoPtr;


Poco::FastMutex ICMPClientTest::_mutex;


ICMPClientTest::ICMPClientTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


ICMPClientTest::~ICMPClientTest()
{
}


void ICMPClientTest::testPing()
{
	assertTrue (ICMPClient::pingIPv4("127.0.0.1") > 0);

	Poco::Net::ICMPClient icmpClient(IPAddress::IPv4);

	registerDelegates(icmpClient);

	assertTrue (icmpClient.ping("127.0.0.1") > 0);
#if POCO_OS == POCO_OS_ANDROID
	assertTrue (icmpClient.ping("10.0.2.15", 4) > 0);
	assertTrue (icmpClient.ping("10.0.2.2", 4) > 0);
#else
	assertTrue (icmpClient.ping("github.com", 4) > 0);

	// warning: may fail depending on the existence of the addresses at test site
	// if so, adjust accordingly (i.e. specify non-existent or unreachable IP addresses)
	assertTrue (0 == icmpClient.ping("192.168.243.1"));
	assertTrue (0 == icmpClient.ping("10.11.12.13"));
#endif

	unregisterDelegates(icmpClient);
	// wait for delegates to finish printing
	Poco::FastMutex::ScopedLock l(_mutex);
}


void ICMPClientTest::testBigPing()
{
	assertTrue (ICMPClient::pingIPv4("127.0.0.1", 1, 96) > 0);

	Poco::Net::ICMPClient icmpClient(IPAddress::IPv4, 96);

	registerDelegates(icmpClient);

	assertTrue (icmpClient.ping("127.0.0.1", 1) > 0);
#if POCO_OS == POCO_OS_ANDROID
	assertTrue (icmpClient.ping("10.0.2.15", 4) > 0);
	assertTrue (icmpClient.ping("10.0.2.2", 4) > 0);
#else
	assertTrue (icmpClient.ping("github.com", 4) > 0);

	// warning: may fail depending on the existence of the addresses at test site
	// if so, adjust accordingly (i.e. specify non-existent or unreachable IP addresses)
	assertTrue (0 == icmpClient.ping("192.168.243.1"));
	assertTrue (0 == icmpClient.ping("10.11.12.13"));
#endif

	unregisterDelegates(icmpClient);
	// wait for delegates to finish printing
	Poco::FastMutex::ScopedLock l(_mutex);
}


void ICMPClientTest::registerDelegates(const ICMPClient& icmpClient)
{
	icmpClient.pingBegin += Delegate<ICMPClientTest, ICMPEventArgs>(this, &ICMPClientTest::onBegin);
	icmpClient.pingReply += Delegate<ICMPClientTest, ICMPEventArgs>(this, &ICMPClientTest::onReply);
	icmpClient.pingError += Delegate<ICMPClientTest, ICMPEventArgs>(this, &ICMPClientTest::onError);
	icmpClient.pingEnd += Delegate<ICMPClientTest, ICMPEventArgs>(this, &ICMPClientTest::onEnd);
}


void ICMPClientTest::unregisterDelegates(const ICMPClient& icmpClient)
{
	icmpClient.pingBegin -= Delegate<ICMPClientTest, ICMPEventArgs>(this, &ICMPClientTest::onBegin);
	icmpClient.pingReply -= Delegate<ICMPClientTest, ICMPEventArgs>(this, &ICMPClientTest::onReply);
	icmpClient.pingError -= Delegate<ICMPClientTest, ICMPEventArgs>(this, &ICMPClientTest::onError);
	icmpClient.pingEnd -= Delegate<ICMPClientTest, ICMPEventArgs>(this, &ICMPClientTest::onEnd);
}


void ICMPClientTest::setUp()
{
}


void ICMPClientTest::tearDown()
{
}


void ICMPClientTest::onBegin(const void* pSender, ICMPEventArgs& args)
{
	Poco::FastMutex::ScopedLock l(_mutex);
	std::ostringstream os;
	os << std::endl << "Pinging " << args.hostName() << " [" << args.hostAddress() << "] with "
		<< args.dataSize() << " bytes of data:"
		<< std::endl << "-------------------------------------------------------" << std::endl;
	std::cout << os.str() << std::endl;
}


void ICMPClientTest::onReply(const void* pSender, ICMPEventArgs& args)
{
	Poco::FastMutex::ScopedLock l(_mutex);
	std::ostringstream os;
	os << "Reply from " << args.hostAddress()
		<< " bytes=" << args.dataSize()
		<< " time=" << args.replyTime() << "ms"
		<< " TTL=" << args.ttl();
	std::cout << os.str() << std::endl;
}


void ICMPClientTest::onError(const void* pSender, ICMPEventArgs& args)
{
	Poco::FastMutex::ScopedLock l(_mutex);
	std::ostringstream os;
	os << args.error();
	std::cerr << os.str() << std::endl;
}


void ICMPClientTest::onEnd(const void* pSender, ICMPEventArgs& args)
{
	Poco::FastMutex::ScopedLock l(_mutex);
	std::ostringstream os;
	int received = args.received();
	os << std::endl << "--- Ping statistics for " << args.hostAddress() << " ---"
		<< std::endl << "Packets: Sent=" << args.sent() << ", Received=" << received
		<< " Lost=" << args.repetitions() - received << " (" << 100.0 - args.percent() << "% loss),"
		<< std::endl << "Approximate round trip times in milliseconds: " << std::endl
		<< "Minimum=" << args.minRTT() << "ms, Maximum=" << args.maxRTT()
		<< "ms, Average=" << args.avgRTT() << "ms"
		<< std::endl << "-----------------------------------------------" << std::endl;
	std::cout << os.str() << std::endl;
}


CppUnit::Test* ICMPClientTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ICMPClientTest");

	CppUnit_addTest(pSuite, ICMPClientTest, testPing);
	CppUnit_addTest(pSuite, ICMPClientTest, testBigPing);

	return pSuite;
}
