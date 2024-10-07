//
// NamedEventTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NamedEventTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/NamedEvent.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"

#include <iostream>

#if POCO_NAMED_EVENT_USE_POSIX_SEMAPHORES
	#include <semaphore.h>
#endif

using Poco::NamedEvent;
using Poco::Thread;
using Poco::Runnable;
using Poco::Timestamp;

using namespace std::string_literals;

static NamedEvent testEvent("TestEvent");


namespace
{
	class TestEvent: public Runnable
	{
	public:
		void run()
		{

			testEvent.wait();
			_timestamp.update();
		}

		const Timestamp& timestamp() const
		{
			return _timestamp;
		}

	private:
		Timestamp _timestamp;
	};
}


NamedEventTest::NamedEventTest(const std::string& name): CppUnit::TestCase(name)
{
}


NamedEventTest::~NamedEventTest()
{
}


void NamedEventTest::testNamedEvent()
{
	Thread thr1;
	TestEvent te;
	thr1.start(te);
	Timestamp now;
	Thread::sleep(2000);
	try
	{
		testEvent.set();
	}
	catch(Poco::NotImplementedException& e)
	{
#if POCO_OS != POCO_OS_ANDROID
		throw e;
#endif
	}
	thr1.join();
#if POCO_OS != POCO_OS_ANDROID
	assertTrue (te.timestamp() > now);
#endif
	Thread thr2;
	thr2.start(te);
	now.update();
	Thread::sleep(2000);
	try
	{
		testEvent.set();
	}
	catch(Poco::NotImplementedException& e)
	{
#if POCO_OS != POCO_OS_ANDROID
		throw e;
#endif
	}
	thr2.join();
#if POCO_OS != POCO_OS_ANDROID
	assertTrue (te.timestamp() > now);
#endif
}


void NamedEventTest::testCreateManyNamedEvents()
{

	std::string name;
#if (POCO_OS == POCO_OS_MAC_OS_X)
	constexpr int testEventCount = 5000;
#else
	constexpr int testEventCount = 20000;
#endif

#if POCO_NAMED_EVENT_USE_POSIX_SEMAPHORES
	// Attempt to remove existing semaphores from previous run of the test suite.
	for (int i = 0; i < testEventCount; i++)
	{
		name = std::string("TestEvent ") + std::to_string(i);
		if (::sem_unlink(name.c_str()) != 0)
		{
			if (errno != ENOENT)
			{
				std::cout << "NamedEvent: Cleanup sem_unlink failed: " << errno << std::endl;
			}
		}
	}
#endif

	try
	{
		int i = 0;
		for (; i < testEventCount; i++)
		{
			name = std::string("TestEvent_") + std::to_string(i);
			auto* ne = new NamedEvent(name);
			delete ne;
		}
		assertEqual(i, testEventCount);
	}
	catch (const Poco::Exception& e)
	{
		fail ("Failed creating named event: "s  + name + " " + e.displayText());
	}
}


void NamedEventTest::setUp()
{
}


void NamedEventTest::tearDown()
{
}


CppUnit::Test* NamedEventTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NamedEventTest");

	CppUnit_addTest(pSuite, NamedEventTest, testNamedEvent);
	CppUnit_addTest(pSuite, NamedEventTest, testCreateManyNamedEvents);

	return pSuite;
}
