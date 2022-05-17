//
// GaugeTest.cpp
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "GaugeTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Prometheus/Gauge.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/TextExporter.h"
#include "Poco/Exception.h"
#include <sstream>
#include <cmath>


using namespace Poco::Prometheus;
using namespace std::string_literals;


GaugeTest::GaugeTest(const std::string& name):
	CppUnit::TestCase("GaugeTest"s)
{
}


void GaugeTest::testBasicBehavior()
{
	Gauge gauge("gauge"s);

	assertEqualDelta(0.0, gauge.value(), 0.0);

	gauge.inc();
	assertEqualDelta(1.0, gauge.value(), 0.0);

	gauge.inc(2.0);
	assertEqualDelta(3.0, gauge.value(), 0.0);

	gauge.dec(2.0);
	assertEqualDelta(1.0, gauge.value(), 0.0);

	gauge.set(5.0);
	assertEqualDelta(5.0, gauge.value(), 0.0);
}


void GaugeTest::testInvalidName()
{
	try
	{
		Gauge gauge("invalid.name"s);
		fail("invalid name - must throw"s);
	}
	catch (Poco::SyntaxException&)
	{
	}
}


void GaugeTest::testLabels()
{
	const std::vector<std::string> labelNames{"label1"s, "label2"};

	Gauge gauge("gauge"s);
	gauge.labelNames(labelNames);

	assertTrue(labelNames == gauge.labelNames());

	gauge.labels({"value11"s, "value21"s}).inc();
	gauge.labels({"value12"s, "value22"s}).inc(2);

	assertEqualDelta(1.0, gauge.labels({"value11"s, "value21"s}).value(), 0.0);
	assertEqualDelta(2.0, gauge.labels({"value12"s, "value22"s}).value(), 0.0);

	try
	{
		gauge.labels({"value11"s}).inc();
		fail("no label values - must throw"s);
	}
	catch (Poco::InvalidArgumentException&)
	{
	}

	try
	{
		gauge.labels({"value11"s, "value21"s, "value31"s}).inc();
		fail("too many label values - must throw"s);
	}
	catch (Poco::InvalidArgumentException&)
	{
	}

	try
	{
		gauge.labelNames({"label3"s, "label4"s});
		fail("cannot change label values - must throw");
	}
	catch (Poco::IllegalStateException&)
	{
	}

	try
	{
		Gauge gauge2("Gauge2"s);
		gauge2.labelNames({"invalid.value"s});
		fail("invalid label name - must throw"s);
	}
	catch (Poco::SyntaxException&)
	{
	}
}


void GaugeTest::testExport()
{
	Gauge gauge1("gauge_1"s);
	gauge1.help("A test gauge"s);

	Gauge gauge2("gauge_2"s);
	gauge2.help("Another test gauge"s);

	Gauge gauge3("gauge_3"s, {
		/*.help =*/ "A test gauge with one label"s,
		/*.labelNames =*/ {"label1"s}
	});

	Gauge gauge4("gauge_4"s, {
		/*.help =*/ "A test gauge with two labels"s,
		/*.labelNames =*/ {"label1"s, "label2"s}
	});

	gauge2.inc();

	gauge3.labels({"value11"}).inc(2);
	gauge3.labels({"value12"}).inc(3);

	gauge4.labels({"value11"s, "value21"s}).inc(4);
	gauge4.labels({"value12"s, "value22"s}).inc(5);

	Gauge gauge5("gauge_5"s, {
		/*.help =*/ "A test for special values"s,
		/*.labelNames =*/ {"test"s}
	});

	gauge5.labels({"+Inf"}).set(INFINITY);
	gauge5.labels({"-Inf"}).set(-INFINITY);
	gauge5.labels({"NaN"}).set(NAN);

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
		"gauge_2 1\n"
		"# HELP gauge_3 A test gauge with one label\n"
		"# TYPE gauge_3 gauge\n"
		"gauge_3{label1=\"value11\"} 2\n"
		"gauge_3{label1=\"value12\"} 3\n"
		"# HELP gauge_4 A test gauge with two labels\n"
		"# TYPE gauge_4 gauge\n"
		"gauge_4{label1=\"value11\",label2=\"value21\"} 4\n"
		"gauge_4{label1=\"value12\",label2=\"value22\"} 5\n"
		"# HELP gauge_5 A test for special values\n"
		"# TYPE gauge_5 gauge\n"
		"gauge_5{test=\"+Inf\"} +Inf\n"
		"gauge_5{test=\"-Inf\"} -Inf\n"
		"gauge_5{test=\"NaN\"} NaN\n"s,
		text);
}


void GaugeTest::setUp()
{
	Registry::defaultRegistry().clear();
}


void GaugeTest::tearDown()
{
}


CppUnit::Test* GaugeTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("GaugeTest");

	CppUnit_addTest(pSuite, GaugeTest, testBasicBehavior);
	CppUnit_addTest(pSuite, GaugeTest, testInvalidName);
	CppUnit_addTest(pSuite, GaugeTest, testLabels);
	CppUnit_addTest(pSuite, GaugeTest, testExport);

	return pSuite;
}
