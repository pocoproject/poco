//
// ActivityTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ActivityTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Activity.h"
#include "Poco/Thread.h"


using Poco::Activity;
using Poco::Thread;


namespace
{
	class ActiveObject
	{
	public:
		ActiveObject():
			_activity(this, &ActiveObject::run),
			_count(0)
		{
		}
		
		~ActiveObject()
		{
		}
		
		Activity<ActiveObject>& activity()
		{
			return _activity;
		}
		
		Poco::UInt64 count() const
		{
			return _count;
		}

	protected:
		void run()
		{
			while (!_activity.isStopped())
				++_count;
		}

	private:
		Activity<ActiveObject> _activity;
		Poco::UInt64           _count;
	};
}


ActivityTest::ActivityTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


ActivityTest::~ActivityTest()
{
}


void ActivityTest::testActivity()
{
	ActiveObject activeObj;
	assertTrue (activeObj.activity().isStopped());
	activeObj.activity().start();
	assertTrue (!activeObj.activity().isStopped());
	Thread::sleep(1000);
	assertTrue (activeObj.activity().isRunning());
	activeObj.activity().stop();
	activeObj.activity().wait();
	assertTrue (activeObj.count() > 0);
}


void ActivityTest::setUp()
{
}


void ActivityTest::tearDown()
{
}


CppUnit::Test* ActivityTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ActivityTest");

	CppUnit_addTest(pSuite, ActivityTest, testActivity);

	return pSuite;
}
