//
// UDPServerTest.h
//
// Definition of the UDPServerTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef UDPServerTest_INCLUDED
#define UDPServerTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class UDPServerTest: public CppUnit::TestCase
{
public:
	UDPServerTest(const std::string& name);
	~UDPServerTest();

	void testServer();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	template <typename S>
	void testServer(int handlerCount, int reps, int port = 0)
	{
		UDPHandler::List handlers;
		for (int i = 0; i < handlerCount; ++i)
			handlers.push_back(new TestUDPHandler());

		S server(handlers, SocketAddress("127.0.0.1", port));
		Thread::sleep(100);

		UDPClient client("127.0.0.1", server.port(), true);
		Thread::sleep(10);

		std::vector<std::string> data;
		int i = 0;
		const char* str = "hello\n";
		for (; i < reps; ++i)
		{
			data.push_back(str);
			if (data.size() == 230 || i == reps - 1)
			{
				data.back().append(1, '\0');
				std::size_t sz = (data.size() * strlen(str)) + 1;
				assertTrue(sz == client.send(Socket::makeBufVec(data)));
				Thread::sleep(10);
				data.clear();
			}
		}

		int count = 0;
		int errCount = 0;
		do
		{
			count = 0;
			errCount = 0;
			Thread::sleep(10);
			UDPHandler::Iterator it = handlers.begin();
			UDPHandler::Iterator end = handlers.end();
			for (; it != end; ++it)
			{
				count += dynamic_cast<TestUDPHandler&>(*(*it)).counter.value();
				errCount += count/10;
			}
		} while (count < i);
		assertTrue(reps == count);
		UDPHandler::Iterator it = handlers.begin();
		UDPHandler::Iterator end = handlers.end();
		for (; it != end; ++it)
		{
			TestUDPHandler& h = dynamic_cast<TestUDPHandler&>(*(*it));
			count = h.counter.value();
			errCount = h.errCounter.value();
			assertTrue(errCount >= count/10);
			assertTrue (h.addr.empty() || h.addr == client.address().toString());
		}
	}
};


#endif // UDPServerTest_INCLUDED
