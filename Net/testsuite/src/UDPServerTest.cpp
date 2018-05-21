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
#include "UDPEchoServer.h"
#include "Poco/Net/UDPServer.h"
#include "Poco/Net/UDPClient.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/AtomicCounter.h"
#include "Poco/Thread.h"
#include "Poco/StringTokenizer.h"


using Poco::Net::DatagramSocket;
using Poco::Net::UDPServer;
using Poco::Net::UDPClient;
using Poco::Net::UDPMultiServer;
using Poco::Net::UDPHandler;
using Poco::Net::Socket;
using Poco::Net::SocketBufVec;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
#ifdef POCO_NET_HAS_INTERFACE
	using Poco::Net::NetworkInterface;
#endif
using Poco::Timespan;
using Poco::Thread;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;
using Poco::AtomicCounter;
using Poco::StringTokenizer;
using Poco::IOException;


namespace
{
	struct TestUDPHandler: public UDPHandler
	{
		TestUDPHandler(): counter(0), errCounter(0) {}

		void processData(char* buf)
		{
			if (!addr.empty() && addr != address(buf).toString())
				throw Poco::InvalidArgumentException("Invalid address.");
			addr = address(buf).toString();
			counter = counter.value() + payload(buf, '\n').count();
			if (counter % 10)
			{
				if (payload(buf, '\n').count() == 0)
					throw Poco::InvalidArgumentException("Invalid error string.");
				std::memset(buf, 0, blockSize());
			}
			else // fake error
			{
				errCounter = errCounter.value() + payload(buf, '\n').count();
				setError(buf, "error");
				processError(buf);
			}
		}

		void processError(char* buf)
		{
			if (std::string(error(buf)) != "error")
				throw Poco::InvalidArgumentException("Invalid error string.");
			std::memset(buf, 0, blockSize());
		}

		AtomicCounter counter;
		AtomicCounter errCounter;
		std::string addr;
	};
}


UDPServerTest::UDPServerTest(const std::string& name): CppUnit::TestCase(name)
{
}


UDPServerTest::~UDPServerTest()
{
}


void UDPServerTest::testServer()
{
	testServer<UDPServer>(1, 10000);
	testServer<UDPMultiServer>(10, 10000, 22080);
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
