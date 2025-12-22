//
// IntGaugeTest.cpp
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "IntGaugeTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Prometheus/IntGauge.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/TextExporter.h"
#include "Poco/Exception.h"
#include <sstream>


using namespace Poco::Prometheus;
using namespace std::string_literals;


IntGaugeTest::IntGaugeTest(const std::string& name):
	CppUnit::TestCase("IntGaugeTest"s)
{
}


void IntGaugeTest::testBasicBehavior()
{
	IntGauge gauge("gauge"s);

	assertEqual(0, gauge.value());

	gauge.inc();
	assertEqual(1, gauge.value());

	gauge.inc(2);
	assertEqual(3, gauge.value());

	gauge.dec(2);
	assertEqual(1, gauge.value());

	gauge.set(5);
	assertEqual(5, gauge.value());
}


void IntGaugeTest::testExport()
{
	IntGauge gauge1("gauge_1"s);
	gauge1.help("A test gauge"s);

	IntGauge gauge2("gauge_2"s);
	gauge2.help("Another test gauge"s);

	gauge2.inc(42);

	std::ostringstream stream;
	TextExporter exporter(stream);
	Registry::defaultRegistry().exportTo(exporter);

	const std::string text = stream.str();
	assertEqual(
		"# HELP gauge_1 A test gauge\n"
		"# TYPE gauge_1 gauge\n"
		"gauge_1 0\n"
		"# HELP gauge_2 Another test gauge\n"
		"# TYPE gauge_2 gauge\n"
		"gauge_2 42\n"s,
		text);
}


void IntGaugeTest::setUp()
{
	Registry::defaultRegistry().clear();
}


void IntGaugeTest::tearDown()
{
}


CppUnit::Test* IntGaugeTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("IntGaugeTest");

	CppUnit_addTest(pSuite, IntGaugeTest, testBasicBehavior);
	CppUnit_addTest(pSuite, IntGaugeTest, testExport);

	return pSuite;
}
