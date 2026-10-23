//
// CancellationTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CancellationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/Cancellation.h"
#include "Poco/Exception.h"
#include <memory>


using Poco::AI::Cancellation;


CancellationTest::CancellationTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


CancellationTest::~CancellationTest()
{
}


void CancellationTest::testCancelCallsEveryAbort()
{
	// The aborts of the model call and of a tool that run at the same time.
	Cancellation cancellation;
	int first = 0;
	int second = 0;
	Cancellation::Request call(cancellation, [&first]() { ++first; });
	Cancellation::Request tool(cancellation, [&second]() { ++second; });
	assertTrue(!cancellation.cancelled());
	cancellation.cancel();
	assertTrue(cancellation.cancelled());
	assertEqual(1, first);
	assertEqual(1, second);
}


void CancellationTest::testAbortAfterCancel()
{
	// An operation that starts after the cancel is aborted at once.
	Cancellation cancellation;
	cancellation.cancel();
	int aborts = 0;
	Cancellation::Request late(cancellation, [&aborts]() { ++aborts; });
	assertEqual(1, aborts);
}


void CancellationTest::testUnregistered()
{
	// Once its Request is gone, an abort is not called.
	Cancellation cancellation;
	int aborts = 0;
	{
		Cancellation::Request done(cancellation, [&aborts]() { ++aborts; });
	}
	cancellation.cancel();
	assertEqual(0, aborts);
}


void CancellationTest::testThrowingAbort()
{
	// An abort that throws keeps neither the others from running nor the
	// cancel from returning.
	Cancellation cancellation;
	int aborts = 0;
	Cancellation::Request throwing(cancellation, []() { throw Poco::IOException("abort failed"); });
	Cancellation::Request other(cancellation, [&aborts]() { ++aborts; });
	cancellation.cancel();
	assertTrue(cancellation.cancelled());
	assertEqual(1, aborts);
}


void CancellationTest::testThrowingAbortInConstructor()
{
	// The abort the constructor calls throws: it is not left registered, so
	// a later cancel does not call what it captured.
	Cancellation cancellation;
	cancellation.cancel();
	auto pCalls = std::make_shared<int>(0);
	try
	{
		Cancellation::Request request(cancellation, [pCalls]()
		{
			++*pCalls;
			throw Poco::IOException("abort failed");
		});
		fail("the exception of the abort must reach the caller");
	}
	catch (const Poco::IOException&)
	{
	}
	assertEqual(1, *pCalls);
	cancellation.cancel();
	assertEqual(1, *pCalls);
}


void CancellationTest::setUp()
{
}


void CancellationTest::tearDown()
{
}


CppUnit::Test* CancellationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CancellationTest");

	CppUnit_addTest(pSuite, CancellationTest, testCancelCallsEveryAbort);
	CppUnit_addTest(pSuite, CancellationTest, testAbortAfterCancel);
	CppUnit_addTest(pSuite, CancellationTest, testUnregistered);
	CppUnit_addTest(pSuite, CancellationTest, testThrowingAbort);
	CppUnit_addTest(pSuite, CancellationTest, testThrowingAbortInConstructor);

	return pSuite;
}
