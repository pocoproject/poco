//
// EventDispatcherTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "EventDispatcherTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/EventDispatcher.h"
#include "Poco/AutoPtr.h"
#include "Poco/Event.h"
#include "Poco/Exception.h"
#include "Poco/Mutex.h"
#include <cstddef>
#include <thread>


namespace
{
	class Dispatcher: public Poco::RemotingNG::EventDispatcher
		/// An EventDispatcher that can be kept busy, as one is that
		/// delivers an event.
	{
	public:
		Dispatcher(): Poco::RemotingNG::EventDispatcher("test")
		{
		}

		void keepBusy(Poco::Event& busy, Poco::Event& done)
			/// Is busy until done is set.
		{
			Poco::FastMutex::ScopedLock lock(_mutex);

			busy.set();
			done.wait();
		}

		std::size_t subscribers()
		{
			Poco::FastMutex::ScopedLock lock(_mutex);

			return _subscribers.size();
		}
	};


	class Busy
		/// Keeps a Dispatcher busy on a thread of its own for as long
		/// as it exists.
	{
	public:
		explicit Busy(Dispatcher& dispatcher):
			_thread([this, &dispatcher]()
			{
				dispatcher.keepBusy(_busy, _done);
			})
		{
			_busy.wait();
		}

		~Busy()
		{
			_done.set();
			_thread.join();
		}

	private:
		Poco::Event _busy;
		Poco::Event _done;
		std::thread _thread;
	};
}


EventDispatcherTest::EventDispatcherTest(const std::string& name): CppUnit::TestCase(name)
{
}


EventDispatcherTest::~EventDispatcherTest()
{
}


void EventDispatcherTest::setUp()
{
}


void EventDispatcherTest::tearDown()
{
}


void EventDispatcherTest::testTrySubscribe()
{
	Poco::AutoPtr<Dispatcher> pDispatcher = new Dispatcher;

	assert (pDispatcher->trySubscribe("subscriber1", "endpoint1"));
	assert (pDispatcher->subscribers() == 1);

	// A busy dispatcher is not waited for, and nothing is changed.
	{
		Busy busy(*pDispatcher);
		assert (!pDispatcher->trySubscribe("subscriber2", "endpoint2"));
	}
	assert (pDispatcher->subscribers() == 1);

	assert (pDispatcher->trySubscribe("subscriber2", "endpoint2"));
	assert (pDispatcher->subscribers() == 2);

	// A subscription that exists is renewed.
	assert (pDispatcher->trySubscribe("subscriber2", "endpoint2"));
	assert (pDispatcher->subscribers() == 2);
}


void EventDispatcherTest::testTryUnsubscribe()
{
	Poco::AutoPtr<Dispatcher> pDispatcher = new Dispatcher;
	pDispatcher->subscribe("subscriber1", "endpoint1");
	pDispatcher->subscribe("subscriber2", "endpoint2");

	// A busy dispatcher is not waited for, and nothing is changed.
	{
		Busy busy(*pDispatcher);
		assert (!pDispatcher->tryUnsubscribe("subscriber1"));
	}
	assert (pDispatcher->subscribers() == 2);

	assert (pDispatcher->tryUnsubscribe("subscriber1"));
	assert (pDispatcher->subscribers() == 1);

	try
	{
		(void) pDispatcher->tryUnsubscribe("subscriber1");
		fail("a subscription that does not exist must be reported");
	}
	catch (Poco::NotFoundException&)
	{
	}
	assert (pDispatcher->subscribers() == 1);
}


CppUnit::Test* EventDispatcherTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("EventDispatcherTest");

	CppUnit_addTest(pSuite, EventDispatcherTest, testTrySubscribe);
	CppUnit_addTest(pSuite, EventDispatcherTest, testTryUnsubscribe);

	return pSuite;
}
