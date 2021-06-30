//
// PollSetTest.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PollSetTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "EchoServer.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/PollSet.h"
#include "Poco/Stopwatch.h"


using Poco::Net::Socket;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::ConnectionRefusedException;
using Poco::Net::PollSet;
using Poco::Timespan;
using Poco::Stopwatch;
using Poco::Thread;


PollSetTest::PollSetTest(const std::string& name): CppUnit::TestCase(name)
{
}


PollSetTest::~PollSetTest()
{
}


void PollSetTest::testPoll()
{
	EchoServer echoServer1;
	EchoServer echoServer2;
	StreamSocket ss1;
	StreamSocket ss2;

	ss1.connect(SocketAddress("127.0.0.1", echoServer1.port()));
	ss2.connect(SocketAddress("127.0.0.1", echoServer2.port()));

	PollSet ps;
	assertTrue(ps.empty());
	ps.add(ss1, PollSet::POLL_READ);
	assertTrue(!ps.empty());
	assertTrue(ps.has(ss1));
	assertTrue(!ps.has(ss2));

	// nothing readable
	Stopwatch sw;
	sw.start();
	Timespan timeout(1000000);
	assertTrue (ps.poll(timeout).empty());
	assertTrue (sw.elapsed() >= 900000);
	sw.restart();

	ps.add(ss2, PollSet::POLL_READ);
	assertTrue(!ps.empty());
	assertTrue(ps.has(ss1));
	assertTrue(ps.has(ss2));

	// ss1 must be writable, if polled for
	ps.update(ss1, PollSet::POLL_READ | PollSet::POLL_WRITE);
	PollSet::SocketModeMap sm = ps.poll(timeout);
	assertTrue (sm.find(ss1) != sm.end());
	assertTrue (sm.find(ss2) == sm.end());
	assertTrue (sm.find(ss1)->second == PollSet::POLL_WRITE);
	assertTrue (sw.elapsed() < 1100000);

	ps.update(ss1, PollSet::POLL_READ);

	ss1.sendBytes("hello", 5);
	char buffer[256];
	sw.restart();
	sm = ps.poll(timeout);
	assertTrue (sm.find(ss1) != sm.end());
	assertTrue (sm.find(ss2) == sm.end());
	assertTrue (sm.find(ss1)->second == PollSet::POLL_READ);
	assertTrue (sw.elapsed() < 1100000);

	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");


	ss2.sendBytes("HELLO", 5);
	sw.restart();
	sm = ps.poll(timeout);
	assertTrue (sm.find(ss1) == sm.end());
	assertTrue (sm.find(ss2) != sm.end());
	assertTrue (sm.find(ss2)->second == PollSet::POLL_READ);
	assertTrue (sw.elapsed() < 1100000);

	n = ss2.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "HELLO");

	ps.remove(ss2);
	assertTrue(!ps.empty());
	assertTrue(ps.has(ss1));
	assertTrue(!ps.has(ss2));

	ss2.sendBytes("HELLO", 5);
	sw.restart();
	sm = ps.poll(timeout);
	assertTrue (sm.empty());

	n = ss2.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "HELLO");

	ss1.close();
	ss2.close();
}


void PollSetTest::testPollNoServer()
{
#ifndef POCO_OS_FAMILY_WINDOWS
	StreamSocket ss1;
	StreamSocket ss2;

	ss1.connectNB(SocketAddress("127.0.0.1", 0xFEFE));
	ss2.connectNB(SocketAddress("127.0.0.1", 0xFEFF));
	PollSet ps;
	assertTrue(ps.empty());
	ps.add(ss1, PollSet::POLL_READ);
	ps.add(ss2, PollSet::POLL_READ);
	assertTrue(!ps.empty());
	assertTrue(ps.has(ss1));
	assertTrue(ps.has(ss2));
	PollSet::SocketModeMap sm;
	Stopwatch sw; sw.start();
	do
	{
		sm = ps.poll(Timespan(1000000));
		if (sw.elapsedSeconds() > 10) fail();
	} while (sm.size() < 2);
	assertTrue(sm.size() == 2);
	for (auto s : sm)
		assertTrue(0 != (s.second | PollSet::POLL_ERROR));
#endif // POCO_OS_FAMILY_WINDOWS
}


void PollSetTest::testPollClosedServer()
{
#ifndef POCO_OS_FAMILY_WINDOWS
	EchoServer echoServer1;
	EchoServer echoServer2;
	StreamSocket ss1;
	StreamSocket ss2;

	ss1.connect(SocketAddress("127.0.0.1", echoServer1.port()));
	ss2.connect(SocketAddress("127.0.0.1", echoServer2.port()));

	PollSet ps;
	assertTrue(ps.empty());
	ps.add(ss1, PollSet::POLL_READ);
	ps.add(ss2, PollSet::POLL_READ);
	assertTrue(!ps.empty());
	assertTrue(ps.has(ss1));
	assertTrue(ps.has(ss2));

	echoServer1.stop();
	ss1.sendBytes("HELLO", 5);
	while (!echoServer1.done()) Thread::sleep(10);
	echoServer2.stop();
	ss2.sendBytes("HELLO", 5);
	while (!echoServer2.done()) Thread::sleep(10);
	PollSet::SocketModeMap sm;
	Stopwatch sw; sw.start();
	do
	{
		sm = ps.poll(Timespan(1000000));
		if (sw.elapsedSeconds() > 10) fail();
	} while (sm.size() < 2);
	assertTrue(sm.size() == 2);
	assertTrue(0 == ss1.receiveBytes(0, 0));
	assertTrue(0 == ss2.receiveBytes(0, 0));
#endif // POCO_OS_FAMILY_WINDOWS
}


void PollSetTest::setUp()
{
}


void PollSetTest::tearDown()
{
}


CppUnit::Test* PollSetTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PollSetTest");

	CppUnit_addTest(pSuite, PollSetTest, testPoll);
	CppUnit_addTest(pSuite, PollSetTest, testPollNoServer);
	CppUnit_addTest(pSuite, PollSetTest, testPollClosedServer);

	return pSuite;
}
