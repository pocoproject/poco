//
// CallbackMetricTest.cpp
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CallbackMetricTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Prometheus/CallbackMetric.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/TextExporter.h"
#include "Poco/Exception.h"
#include <sstream>


using namespace Poco::Prometheus;
using namespace std::string_literals;


CallbackMetricTest::CallbackMetricTest(const std::string& name):
	CppUnit::TestCase("CallbackMetricTest"s)
{
}


void CallbackMetricTest::testExport()
{
	CallbackIntCounter counter1("counter_1"s, []()
		{
			return 42;
		});
	counter1.help("A test counter"s);

	CallbackIntGauge gauge1("gauge_1"s, []()
		{
			return 2112;
		});
	gauge1.help("A test gauge"s);

	std::ostringstream stream;
	TextExporter exporter(stream);
	Registry::defaultRegistry().exportTo(exporter);

	const std::string text = stream.str();
	assertEqual(
		"# HELP counter_1 A test counter\n"
		"# TYPE counter_1 counter\n"
		"counter_1 42\n"
		"# HELP gauge_1 A test gauge\n"
		"# TYPE gauge_1 gauge\n"
		"gauge_1 2112\n"s,
		text);
}


void CallbackMetricTest::setUp()
{
	Registry::defaultRegistry().clear();
}


void CallbackMetricTest::tearDown()
{
}


CppUnit::Test* CallbackMetricTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CallbackMetricTest");

	CppUnit_addTest(pSuite, CallbackMetricTest, testExport);

	return pSuite;
}
