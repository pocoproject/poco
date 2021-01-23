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
		TestUDPHandler() : counter(0), errCounter(0) {}

		void processData(char *buf)
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

		void processError(char *buf)
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

	template<typename S>
	bool server(int handlerCount, int reps, int port = 0)
	{
		Poco::Net::UDPHandler::List handlers;
		for (int i = 0; i < handlerCount; ++i)
			handlers.push_back(new TestUDPHandler());

		S server(handlers, Poco::Net::SocketAddress("127.0.0.1", port));
		Poco::Thread::sleep(100);

		Poco::Net::UDPClient client("127.0.0.1", server.port(), true);
		Poco::Thread::sleep(10);

		std::vector<std::string> data;
		int i = 0;
		const char *str = "hello\n";
		for (; i < reps; ++i)
		{
			data.push_back(str);
			if (data.size() == 230 || i == reps - 1)
			{
				data.back().append(1, '\0');
				std::size_t sz = (data.size() * strlen(str)) + 1;
				int sent = client.send(Poco::Net::Socket::makeBufVec(data));
				if (sz != sent)
				{
					std::cerr << "Send count mismatch, expected: " << sz
						<< ", sent: " << sent << std::endl;
					return false;
				}
				Poco::Thread::sleep(10);
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
			Poco::Net::UDPHandler::Iterator it = handlers.begin();
			Poco::Net::UDPHandler::Iterator end = handlers.end();
			for (; it != end; ++it)
			{
				count += dynamic_cast<TestUDPHandler &>(*(*it)).counter.value();
				errCount += count / 10;
			}
		} while (count < i);
		if (reps != count)
		{
			std::cerr << "Response mismatch, expected: " << reps
					<< ", received: " << count << std::endl;
			return false;
		}
		Poco::Net::UDPHandler::Iterator it = handlers.begin();
		Poco::Net::UDPHandler::Iterator end = handlers.end();
		for (; it != end; ++it)
		{
			TestUDPHandler &h = dynamic_cast<TestUDPHandler &>(*(*it));
			count = h.counter.value();
			errCount = h.errCounter.value();
			if (errCount < count / 10)
			{
				std::cerr << "Error count mismatch, expected: <" << count / 10
					<< ", received: " << errCount << std::endl;
				return false;
			}

			if (h.counter && (h.addr.empty() || h.addr != client.address().toString()))
			{
				std::cerr << "Address mismatch, expected: " << client.address().toString()
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


void UDPServerTest::testServer()
{
	int msgs = 10000;
	assertTrue (server<Poco::Net::UDPServer>(1, msgs));
	assertTrue (server<Poco::Net::UDPMultiServer>(10, msgs, 22080));
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

	CppUnit_addTest(pSuite, UDPServerTest, testServer);

	return pSuite;
}
