//
// IntCounterTest.cpp
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "IntCounterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Prometheus/IntCounter.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/TextExporter.h"
#include "Poco/Exception.h"
#include <sstream>


using namespace Poco::Prometheus;
using namespace std::string_literals;


IntCounterTest::IntCounterTest(const std::string& name):
	CppUnit::TestCase("IntCounterTest"s)
{
}


void IntCounterTest::testBasicBehavior()
{
	IntCounter counter("counter"s);

	assertEqual(0, counter.value());

	counter.inc();
	assertEqual(1, counter.value());

	counter.inc(2);
	assertEqual(3, counter.value());
}


void IntCounterTest::testExport()
{
	IntCounter counter1("counter_1"s);
	counter1.help("A test counter"s);

	IntCounter counter2("counter_2"s);
	counter2.help("Another test counter"s);

	counter2.inc(42);

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
		"counter_2 42\n"s,
		text);
}


void IntCounterTest::setUp()
{
	Registry::defaultRegistry().clear();
}


void IntCounterTest::tearDown()
{
}


CppUnit::Test* IntCounterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("IntCounterTest");

	CppUnit_addTest(pSuite, IntCounterTest, testBasicBehavior);
	CppUnit_addTest(pSuite, IntCounterTest, testExport);

	return pSuite;
}
