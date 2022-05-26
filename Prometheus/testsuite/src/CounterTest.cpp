//
// CounterTest.cpp
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CounterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Prometheus/Counter.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/TextExporter.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Exception.h"
#include <sstream>


using namespace Poco::Prometheus;
using namespace std::string_literals;


CounterTest::CounterTest(const std::string& name):
	CppUnit::TestCase("CounterTest"s)
{
}


void CounterTest::testBasicBehavior()
{
	Counter counter("counter"s);

	assertEqualDelta(0.0, counter.value(), 0.0);

	counter.inc();
	assertEqualDelta(1.0, counter.value(), 0.0);

	counter.inc(2);
	assertEqualDelta(3.0, counter.value(), 0.0);
}


void CounterTest::testInvalidName()
{
	try
	{
		Counter counter("invalid.name"s);
		fail("invalid name - must throw"s);
	}
	catch (Poco::SyntaxException&)
	{
	}
}


void CounterTest::testLabels()
{
	const std::vector<std::string> labelNames{"label1"s, "label2"};

	Counter counter("counter"s);
	counter.labelNames(labelNames);

	assertTrue(labelNames == counter.labelNames());

	counter.labels({"value11"s, "value21"s}).inc();
	counter.labels({"value12"s, "value22"s}).inc(2);

	assertEqualDelta(1.0, counter.labels({"value11"s, "value21"s}).value(), 0.0);
	assertEqualDelta(2.0, counter.labels({"value12"s, "value22"s}).value(), 0.0);

	assertEqual(2, counter.sampleCount());

	counter.remove({"value12"s, "value22"s});
	assertEqual(1, counter.sampleCount());

	counter.clear();
	assertEqual(0, counter.sampleCount());

	try
	{
		counter.labels({"value11"s}).inc();
		fail("missing label values - must throw"s);
	}
	catch (Poco::InvalidArgumentException&)
	{
	}

	try
	{
		counter.labels({"value11"s, "value21"s, "value31"s}).inc();
		fail("too many label values - must throw"s);
	}
	catch (Poco::InvalidArgumentException&)
	{
	}

	try
	{
		counter.labelNames({"label3"s, "label4"s});
		fail("cannot change label values - must throw");
	}
	catch (Poco::IllegalStateException&)
	{
	}

	try
	{
		Counter counter2("counter2"s);
		counter2.labelNames({"invalid.value"s});
		fail("invalid label name - must throw"s);
	}
	catch (Poco::SyntaxException&)
	{
	}
}


void CounterTest::testConcurrency()
{
	class R: public Poco::Runnable
	{
	public:
		R(Counter& c, int n):
			_c(c),
			_n(n)
		{
		}

		void run()
		{
			for (int i = 0; i < _n; i++)
			{
				_c.inc();
			}
		}

	private:
		Counter& _c;
		int _n;
	};

	Poco::Thread t1;
	Poco::Thread t2;
	Poco::Thread t3;
	Poco::Thread t4;

	Counter counter("counter1"s);
	const int n = 1000000;

	R r(counter, n);
	t1.start(r);
	t2.start(r);
	t3.start(r);
	t4.start(r);
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	assertEqualDelta(4.0*n, counter.value(), 0.0);
}


void CounterTest::testExport()
{
	Counter counter1("counter_1"s);
	counter1.help("A test counter"s);

	Counter counter2("counter_2"s);
	counter2.help("Another test counter"s);

	Counter counter3("counter_3"s, {
		/*.help =*/ "A test counter with one label"s,
		/*.labelNames =*/ {"label1"s}
	});

	Counter counter4("counter_4"s, {
		/*.help =*/ "A test counter with two labels"s,
		/*.labelNames =*/ {"label1"s, "label2"s}
	});

	counter2.inc();

	counter3.labels({"value11"}).inc(2);
	counter3.labels({"value12"}).inc(3);

	counter4.labels({"value11"s, "value21"s}).inc(4);
	counter4.labels({"value12"s, "value22"s}).inc(5);

	std::ostringstream stream;
	TextExporter exporter(stream);
	Registry::defaultRegistry().exportTo(exporter);

	const std::string text = stream.str();
	assertEqual(
		"# HELP counter_1 A test counter\n"
		"# TYPE counter_1 counter\n"
		"counter_1 0\n"
		"# HELP counter_2 Another test counter\n"
		"# TYPE counter_2 counter\n"
		"counter_2 1\n"
		"# HELP counter_3 A test counter with one label\n"
		"# TYPE counter_3 counter\n"
		"counter_3{label1=\"value11\"} 2\n"
		"counter_3{label1=\"value12\"} 3\n"
		"# HELP counter_4 A test counter with two labels\n"
		"# TYPE counter_4 counter\n"
		"counter_4{label1=\"value11\",label2=\"value21\"} 4\n"
		"counter_4{label1=\"value12\",label2=\"value22\"} 5\n"s,
		text);
}


void CounterTest::setUp()
{
	Registry::defaultRegistry().clear();
}


void CounterTest::tearDown()
{
}


CppUnit::Test* CounterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CounterTest");

	CppUnit_addTest(pSuite, CounterTest, testBasicBehavior);
	CppUnit_addTest(pSuite, CounterTest, testInvalidName);
	CppUnit_addTest(pSuite, CounterTest, testLabels);
	CppUnit_addTest(pSuite, CounterTest, testConcurrency);
	CppUnit_addTest(pSuite, CounterTest, testExport);

	return pSuite;
}
