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
#include <iostream>


using Poco::Net::Socket;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::ConnectionRefusedException;
using Poco::Net::PollSet;
using Poco::Timespan;
using Poco::Stopwatch;
using Poco::Thread;


namespace {

class Poller : public Poco::Runnable
{
public:
	Poller(PollSet& pollSet, const Timespan& timeout): _pollSet(pollSet),
		_timeout(timeout),
		_running(false)
	{
	}

	void run()
	{
		_running = true;
		_pollSet.poll(_timeout);
		_running = false;
	}

	bool isRunning()
	{
		return _running;
	}

private:
	PollSet& _pollSet;
	Timespan _timeout;
	std::atomic<bool> _running;
};


}


PollSetTest::PollSetTest(const std::string& name): CppUnit::TestCase(name)
{
}


PollSetTest::~PollSetTest()
{
}


void PollSetTest::testAddUpdate()
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
	ps.add(ss1, PollSet::POLL_WRITE);
	PollSet::SocketModeMap sm = ps.poll(timeout);
	assertTrue (sm.find(ss1) != sm.end());
	assertTrue (sm.find(ss2) == sm.end());
	assertTrue (sm.find(ss1)->second & PollSet::POLL_WRITE);
	assertTrue (sw.elapsed() < 1100000);

	ps.add(ss1, PollSet::POLL_READ);

	ss1.setBlocking(true);
	ss1.sendBytes("hello", 5);
	while (!ss1.poll(Timespan(0, 10000), Socket::SELECT_READ))
		Poco::Thread::sleep(10);
	sw.restart();
	sm = ps.poll(timeout);
	assertTrue (sm.find(ss1) != sm.end());
	assertTrue (sm.find(ss2) == sm.end());
	assertTrue (sm.find(ss1)->second & PollSet::POLL_READ);
	assertTrue (sw.elapsed() < 1100000);

	char buffer[256];
	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");

	ss2.setBlocking(true);
	ss2.sendBytes("HELLO", 5);
	sw.restart();
	ps.remove(ss1);
	sm = ps.poll(timeout);
	assertTrue (sm.find(ss1) == sm.end());
	assertTrue (sm.find(ss2) != sm.end());
	assertTrue (sm.find(ss2)->second & PollSet::POLL_READ);
	assertTrue (sw.elapsed() < 1100000);

	n = ss2.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "HELLO");

	ps.remove(ss2);
	assertTrue(ps.empty());
	assertTrue(!ps.has(ss1));
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


void PollSetTest::testTimeout()
{
	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	PollSet ps;
	ps.add(ss, PollSet::POLL_READ);
	Timespan timeout(1000000);
	Stopwatch sw; sw.start();
	PollSet::SocketModeMap sm = ps.poll(timeout);
	sw.stop();
	assertTrue(sm.empty());
	assertTrue(sw.elapsed() >= 900000);

	ss.sendBytes("hello", 5);
	sw.restart();
	sm = ps.poll(timeout);
	sw.stop();
	assertTrue(ps.poll(timeout).size() == 1);

	char buffer[5];
	ss.receiveBytes(buffer, sizeof(buffer));

	sw.restart();
	sm = ps.poll(timeout);
	sw.stop();
	assertTrue(sm.empty());
	assertTrue(sw.elapsed() >= 900000);
}


void PollSetTest::testPollNB()
{
	EchoServer echoServer1;
	StreamSocket ss1;

	ss1.connectNB(SocketAddress("127.0.0.1", echoServer1.port()));

	PollSet ps;
	assertTrue(ps.empty());
	ps.add(ss1, PollSet::POLL_READ);
	ps.add(ss1, PollSet::POLL_WRITE);
	assertTrue(!ps.empty());
	assertTrue(ps.has(ss1));

	while (!ss1.poll(Timespan(0, 10000), Socket::SELECT_WRITE))
		Poco::Thread::sleep(10);

	Timespan timeout(1000000);
	PollSet::SocketModeMap sm;
	while (sm.empty()) sm = ps.poll(timeout);
	assertTrue(sm.find(ss1) != sm.end());
	assertTrue(sm.find(ss1)->second & PollSet::POLL_WRITE);

	ss1.setBlocking(true);
	ss1.sendBytes("hello", 5);
	while (!ss1.poll(Timespan(0, 10000), Socket::SELECT_READ))
		Poco::Thread::sleep(10);
	char buffer[256];
	sm = ps.poll(timeout);
	assertTrue(sm.find(ss1) != sm.end());
	assertTrue(sm.find(ss1)->second & PollSet::POLL_READ);

	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue(n == 5);
	assertTrue(std::string(buffer, n) == "hello");
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
	assertTrue (sm.find(ss1)->second | PollSet::POLL_WRITE);
	assertTrue (sw.elapsed() < 1100000);

	ps.update(ss1, PollSet::POLL_READ);

	ss1.setBlocking(true);
	ss1.sendBytes("hello", 5);
	while (!ss1.poll(Timespan(0, 10000), Socket::SELECT_READ))
		Poco::Thread::sleep(10);
	char buffer[256];
	sw.restart();
	sm = ps.poll(timeout);
	assertTrue (sm.find(ss1) != sm.end());
	assertTrue (sm.find(ss2) == sm.end());
	assertTrue (sm.find(ss1)->second & PollSet::POLL_READ);
	assertTrue (sw.elapsed() < 1100000);

	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");

	ss2.setBlocking(true);
	ss2.sendBytes("HELLO", 5);
	while (!ss2.poll(Timespan(0, 10000), Socket::SELECT_READ))
		Poco::Thread::sleep(10);
	sw.restart();
	sm = ps.poll(timeout);
	assertTrue (sm.find(ss1) == sm.end());
	assertTrue (sm.find(ss2) != sm.end());
	assertTrue (sm.find(ss2)->second & PollSet::POLL_READ);
	assertTrue (sw.elapsed() < 1100000);

	n = ss2.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "HELLO");

	ps.remove(ss2);
	ps.update(ss1, Socket::SELECT_READ);
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
	StreamSocket ss1(SocketAddress::IPv4);
	StreamSocket ss2(SocketAddress::IPv4);

	PollSet ps;
	assertTrue(ps.empty());
	ps.add(ss1, PollSet::POLL_READ | PollSet::POLL_WRITE | PollSet::POLL_ERROR);
	ps.add(ss2, PollSet::POLL_READ | PollSet::POLL_WRITE | PollSet::POLL_ERROR);
	assertTrue(!ps.empty());
	assertTrue(ps.has(ss1));
	assertTrue(ps.has(ss2));

	// should be like this, but Linux epoll disagrees ...
	//assertEqual(0, static_cast<int>(ps.poll(Timespan(100)).size()));

	ss1.setBlocking(true);
	ss2.setBlocking(true);
	try
	{
		ss1.connect(SocketAddress("127.0.0.1", 0xFFFF));
		fail("connection must fail", __LINE__, __FILE__);
	}
	catch (Poco::Net::ConnectionRefusedException&) {}
	catch (Poco::Net::NetException&) {}
	catch (Poco::Exception&) {}

	try
	{
		ss2.connect(SocketAddress("127.0.0.1", 0xFFFF));
		fail("connection must fail", __LINE__, __FILE__);
	}
	catch (Poco::Net::ConnectionRefusedException&) {}
	catch (Poco::Net::NetException&) {}
	catch (Poco::Exception&) {}

	assertEqual(2, ps.poll(Timespan(1000000)).size());
}


void PollSetTest::testPollClosedServer()
{
	EchoServer echoServer1;
	EchoServer echoServer2;

	StreamSocket ss1(SocketAddress::IPv4);
	StreamSocket ss2(SocketAddress::IPv4);

	PollSet ps;
	assertTrue(ps.empty());
	ps.add(ss1, PollSet::POLL_READ);
	ps.add(ss2, PollSet::POLL_READ);
	assertTrue(!ps.empty());
	assertTrue(ps.has(ss1));
	assertTrue(ps.has(ss2));

	//assertEqual(0, static_cast<int>(ps.poll(Timespan(100)).size()));

	ss1.connect(SocketAddress("127.0.0.1", echoServer1.port()));
	ss2.connect(SocketAddress("127.0.0.1", echoServer2.port()));

	std::string str = "HELLO";
	int len = static_cast<int>(str.length());

	echoServer1.stop();
	// echoServer is blocked waiting for data, send some
	assertTrue (len == ss1.sendBytes(str.data(), len));
	// the stop flag should kick in, wait for it ...
	Stopwatch sw; sw.start();
	while (!echoServer1.done())
	{
		Thread::sleep(10);
		int secs = sw.elapsedSeconds();
		if (secs > 10)
		{
			fail(Poco::format("testPollClosedServer(1) timed out "
				"waiting on server after %ds", secs), __LINE__);
		}
	}
	char buffer[5];
	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue(n == 0);
	auto smm = ps.poll(Timespan(1000000));
	assertEqual(1, smm.size());
	assertTrue(ss1 == smm.begin()->first);
	ps.remove(ss1);
	assertTrue(!ps.empty());
	assertTrue(!ps.has(ss1));
	assertTrue(ps.has(ss2));
	echoServer2.stop();
	assertTrue (len == ss2.sendBytes(str.data(), len));
	sw.restart();
	while (!echoServer2.done())
	{
		Thread::sleep(10);
		int secs = sw.elapsedSeconds();
		if (secs > 10)
		{
			fail(Poco::format("testPollClosedServer(2) timed out "
				"waiting on server after %ds", secs), __LINE__);
		}
	}
	n = ss2.receiveBytes(buffer, sizeof(buffer));
	assertTrue(n == 0);
	smm = ps.poll(Timespan(1000000));
	assertEqual(1, smm.size());
	assertTrue(ss2 == smm.begin()->first);

	// socket closed or error
	assertTrue(0 >= ss1.receiveBytes(0, 0));
	assertTrue(0 >= ss2.receiveBytes(0, 0));
}


void PollSetTest::testPollSetWakeUp()
{
#if defined(POCO_HAVE_FD_EPOLL) || defined (POCO_HAVE_FD_POLL)
	PollSet ps;
	Timespan timeout(100000000); // 100 seconds
	Poller poller(ps, timeout);
	Thread t;
	Stopwatch sw;
	sw.start();
	t.start(poller);
	while (!poller.isRunning()) Thread::sleep(100);
	ps.wakeUp();
	t.join();
	sw.stop();
	assertFalse (poller.isRunning());
	assertTrue(sw.elapsedSeconds() < 1);
#else // TODO: other implementations
	std::cout << "not implemented";
#endif // POCO_HAVE_FD_EPOLL || POCO_HAVE_FD_EPOLL
}


void PollSetTest::testClear()
{
	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", echoServer.port()));
	PollSet ps;

	ps.add(ss, PollSet::POLL_READ);
	PollSet::SocketModeMap sm = ps.poll(0);
	assertTrue(sm.empty());

	ss.sendBytes("hello", 5);
	assertTrue(ps.poll(100000).size() == 1);

	char buffer[5];
	ss.receiveBytes(buffer, sizeof(buffer));

	ps.clear();
	ps.add(ss, PollSet::POLL_READ);
	sm = ps.poll(0);
	assertTrue(sm.empty());

	ss.sendBytes(buffer, 5);
	assertTrue(ps.poll(100000).size() == 1);

	ss.receiveBytes(buffer, sizeof(buffer));
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

	CppUnit_addTest(pSuite, PollSetTest, testAddUpdate);
	CppUnit_addTest(pSuite, PollSetTest, testTimeout);
	CppUnit_addTest(pSuite, PollSetTest, testPollNB);
	CppUnit_addTest(pSuite, PollSetTest, testPoll);
	CppUnit_addTest(pSuite, PollSetTest, testPollNoServer);
	CppUnit_addTest(pSuite, PollSetTest, testPollClosedServer);
	CppUnit_addTest(pSuite, PollSetTest, testPollSetWakeUp);
	CppUnit_addTest(pSuite, PollSetTest, testClear);

	return pSuite;
}
