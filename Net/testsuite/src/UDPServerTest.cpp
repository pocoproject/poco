//
// UDPServerTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "UDPServerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/UDPServer.h"
#include "Poco/Net/UDPServerParams.h"
#include "Poco/Net/UDPClient.h"
#include "Poco/Net/UDPHandler.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/AtomicCounter.h"
#include "Poco/StringTokenizer.h"
#include <cstring>
#include <iostream>


using Poco::Net::DatagramSocket;
using Poco::Net::UDPServer;
using Poco::Net::UDPClient;
using Poco::Net::UDPMultiServer;
using Poco::Net::UDPHandler;
using Poco::Net::Socket;
using Poco::Net::SocketBufVec;
using Poco::Net::SocketAddress;
using Poco::Timespan;
using Poco::Thread;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;
using Poco::AtomicCounter;
using Poco::StringTokenizer;


namespace
{
	struct TestUDPHandler : public Poco::Net::UDPHandler
	{
		TestUDPHandler() : counter(0), errCounter(0)
		{
			start();
		}

		void processData(char *buf) override
		{
			if (!addr.empty() && addr != address(buf).toString()) ++errors;
			addr = address(buf).toString();
			counter = counter.value() + static_cast<AtomicCounter::ValueType>(payload(buf, '\n').count());
			if (counter % 10)
			{
				if (payload(buf, '\n').count() == 0) ++errors;
				std::memset(buf, 0, blockSize());
			}
			else // fake error
			{
				errCounter = errCounter.value() + static_cast<AtomicCounter::ValueType>(payload(buf, '\n').count());
				setError(buf, "error");
				processError(buf);
			}
		}

		void processError(char *buf) override
		{
			if (std::string(error(buf)) != "error") ++errors;
			std::memset(buf, 0, blockSize());
		}

		AtomicCounter counter;
		AtomicCounter errCounter;
		static AtomicCounter errors;
		std::string addr;
	};

	AtomicCounter TestUDPHandler::errors;

	template<typename SRVT, typename CLTT, typename HNDLRT>
	bool server(int handlerCount, int clientCount, int reps, const typename SRVT::ServerParams& params)
	{
		Poco::Net::UDPHandler::List handlers;
		for (int i = 0; i < handlerCount; ++i)
			handlers.push_back(new HNDLRT());

		SRVT server(handlers, params);
		Poco::Thread::sleep(100);

		using ClientPtr = Poco::AutoPtr<CLTT>;
		std::vector<ClientPtr> clients;
		for (int i = 0; i < clientCount; i++)
			clients.push_back( new CLTT("127.0.0.1", server.port(), true) );

		Poco::Thread::sleep(10);

		std::vector<std::string> data;
		int i = 0;
		const char *str = "hello\n";
		for (; i < reps; ++i)
		{
			data.emplace_back(str);
			if (data.size() == 230 || i == reps - 1)
			{
				data.back().append(1, '\0');
				std::size_t sz = (data.size() * strlen(str)) + 1;
				int sent = clients[i % clientCount]->send(Poco::Net::Socket::makeBufVec(data));
				if (sz != sent)
				{
					std::cerr << "Send count mismatch, expected: " << sz
						<< ", sent: " << sent << std::endl;
					return false;
				}
				Poco::Thread::sleep(5);
				data.clear();
			}
		}

		int count = 0;
		int errCount = 0;
		do
		{
			count = 0;
			errCount = 0;
			Poco::Thread::sleep(10);
			for (const auto& h: handlers)
			{
				count += dynamic_cast<const HNDLRT &>(*h).counter.value();
				errCount += count / 10;
			}
		} while (count < i);
		if (reps != count)
		{
			std::cerr << "Response mismatch, expected: " << reps
					<< ", received: " << count << std::endl;
			return false;
		}
		const auto address = clients[0]->address().toString();
		for (const auto& he: handlers)
		{
			const auto &h = dynamic_cast<const HNDLRT &>(*he);
			count = h.counter.value();
			errCount = h.errCounter.value();
			if (errCount < count / 10)
			{
				std::cerr << "Error count mismatch, expected: <" << count / 10
					<< ", received: " << errCount << std::endl;
				return false;
			}

			if (clientCount == 1)
				if (h.counter && (h.addr.empty() || h.addr != address))
				{
					std::cerr << "Address mismatch, expected: " << address
						<< ", received: " << h.addr << std::endl;
					return false;
				}
		}
		return true;
	}
}


UDPServerTest::UDPServerTest(const std::string& name): CppUnit::TestCase(name)
{
}


UDPServerTest::~UDPServerTest()
{
}


void UDPServerTest::testUDPSingleSocket()
{
	TestUDPHandler::errors = 0;
	int msgs = 10000;	
	Poco::Net::UDPServerParams params(Poco::Net::SocketAddress("127.0.0.1", 0));

	auto tf = server<Poco::Net::UDPServer, Poco::Net::UDPClient, TestUDPHandler>;
	assertTrue( tf(1, 1, msgs, params) );
	assertTrue (TestUDPHandler::errors == 0);
}


void UDPServerTest::testUDPMultiSocket()
{
	TestUDPHandler::errors = 0;
	int msgs = 10000;
	Poco::Net::UDPServerParams params(Poco::Net::SocketAddress("127.0.0.1", 22080));

	auto tf = server<Poco::Net::UDPMultiServer, Poco::Net::UDPClient, TestUDPHandler>;
	assertTrue( tf(1, 1, msgs, params) );
	assertTrue (TestUDPHandler::errors == 0);
}


void UDPServerTest::testUDPSingleSocketMultipleHandlers()
{
	TestUDPHandler::errors = 0;
	int msgs = 10000;
	Poco::Net::UDPServerParams params(Poco::Net::SocketAddress("127.0.0.1", 0));

	auto tf = server<Poco::Net::UDPServer, Poco::Net::UDPClient, TestUDPHandler>;
	assertTrue( tf(10, 1, msgs, params) );
	assertTrue (TestUDPHandler::errors == 0);
}


void UDPServerTest::testUDPMultiSocketMultipleHandlers()
{
	TestUDPHandler::errors = 0;
	int msgs = 10000;
	Poco::Net::UDPServerParams params(Poco::Net::SocketAddress("127.0.0.1", 22080));

	auto tf = server<Poco::Net::UDPMultiServer, Poco::Net::UDPClient, TestUDPHandler>;
	assertTrue( tf(10, 1, msgs, params) );
	assertTrue (TestUDPHandler::errors == 0);
}


void UDPServerTest::testUDPMultiSocketMultipleHandlersLessSockets()
{
	TestUDPHandler::errors = 0;
	int msgs = 10000;
	Poco::Net::UDPServerParams params(
		Poco::Net::SocketAddress("127.0.0.1", 22080),
		2
	);

	auto tf = server<Poco::Net::UDPMultiServer, Poco::Net::UDPClient, TestUDPHandler>;
	assertTrue( tf(10, 1, msgs, params) );
	assertTrue (TestUDPHandler::errors == 0);
}


void UDPServerTest::testUDPMultiSocketMultipleHandlersMoreSockets()
{
	TestUDPHandler::errors = 0;
	int msgs = 10000;
	Poco::Net::UDPServerParams params(
		Poco::Net::SocketAddress("127.0.0.1", 22080),
		10
	);

	auto tf = server<Poco::Net::UDPMultiServer, Poco::Net::UDPClient, TestUDPHandler>;
	assertTrue( tf(2, 1, msgs, params) );
	assertTrue (TestUDPHandler::errors == 0);
}


void UDPServerTest::testUDPMultiSocketMultipleHandlersMultipleClients()
{
	TestUDPHandler::errors = 0;
	int msgs = 50000;
	Poco::Net::UDPServerParams params(Poco::Net::SocketAddress("127.0.0.1", 0));

	auto tf = server<Poco::Net::UDPServer, Poco::Net::UDPClient, TestUDPHandler>;
	assertTrue( tf(10, 5, msgs, params) );
	assertTrue (TestUDPHandler::errors == 0);
}


void UDPServerTest::setUp()
{
}


void UDPServerTest::tearDown()
{
}


CppUnit::Test* UDPServerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("UDPServerTest");

	CppUnit_addTest(pSuite, UDPServerTest, testUDPSingleSocket);
	CppUnit_addTest(pSuite, UDPServerTest, testUDPMultiSocket);
	CppUnit_addTest(pSuite, UDPServerTest, testUDPSingleSocketMultipleHandlers);
	CppUnit_addTest(pSuite, UDPServerTest, testUDPMultiSocketMultipleHandlers);
	CppUnit_addTest(pSuite, UDPServerTest, testUDPMultiSocketMultipleHandlersLessSockets);
	CppUnit_addTest(pSuite, UDPServerTest, testUDPMultiSocketMultipleHandlersMoreSockets);
	CppUnit_addTest(pSuite, UDPServerTest, testUDPMultiSocketMultipleHandlersMultipleClients);

	return pSuite;
}
