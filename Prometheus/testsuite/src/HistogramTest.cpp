//
// HistogramTest.cpp
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HistogramTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Prometheus/Histogram.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/TextExporter.h"
#include "Poco/Exception.h"
#include <sstream>


using namespace Poco::Prometheus;
using namespace std::string_literals;


HistogramTest::HistogramTest(const std::string& name):
	CppUnit::TestCase("HistogramTest"s)
{
}


void HistogramTest::testNoBuckets()
{
	Histogram histo("histo"s);

	histo.observe(1.0);
	histo.observe(2.0);

	const auto data = histo.data();
	assertEqual(0, data.bucketCounts.size());
	assertEqual(2, data.count);
	assertEqualDelta(3.0, data.sum, 0.0);
}


void HistogramTest::testBuckets()
{
	Histogram histo("histo"s);
	histo.buckets({1.0, 2.0, 3.0});

	histo.observe(1.1);
	histo.observe(2.0);
	histo.observe(2.5);
	histo.observe(4.0);

	const auto data = histo.data();
	assertEqual(3, data.bucketCounts.size());
	assertEqual(0, data.bucketCounts[0]);
	assertEqual(2, data.bucketCounts[1]);
	assertEqual(3, data.bucketCounts[2]);

	assertEqual(4, data.count);
	assertEqualDelta(9.6, data.sum, 0.001);
}


void HistogramTest::testLabels()
{
	Histogram histo("histo"s);
	histo
		.buckets({1.0, 2.0, 3.0})
		.labelNames({"label"s});

	histo.labels({"value1"s}).observe(1.1);
	histo.labels({"value1"s}).observe(2.0);
	histo.labels({"value1"s}).observe(2.5);
	histo.labels({"value1"s}).observe(4.0);

	histo.labels({"value2"s}).observe(3.0);
	histo.labels({"value2"s}).observe(5.0);

	const auto data1 = histo.labels({"value1"s}).data();
	assertEqual(3, data1.bucketCounts.size());
	assertEqual(0, data1.bucketCounts[0]);
	assertEqual(2, data1.bucketCounts[1]);
	assertEqual(3, data1.bucketCounts[2]);

	assertEqual(4, data1.count);
	assertEqualDelta(9.6, data1.sum, 0.001);

	const auto data2 = histo.labels({"value2"s}).data();
	assertEqual(3, data2.bucketCounts.size());
	assertEqual(0, data2.bucketCounts[0]);
	assertEqual(0, data2.bucketCounts[1]);
	assertEqual(1, data2.bucketCounts[2]);

	assertEqual(2, data2.count);
	assertEqualDelta(8.0, data2.sum, 0.001);
}


void HistogramTest::testExport()
{
	Histogram histo1("histo_1"s);

	histo1.observe(1.0);
	histo1.observe(2.0);

	Histogram histo2("histo_2"s, {
		/*.help =*/ "A histogram with buckets"s,
		/* .labelNames =*/ {},
		/*.buckets =*/ {1.0, 2.0, 3.0}
	});

	histo2.observe(1.1);
	histo2.observe(2.0);
	histo2.observe(2.5);
	histo2.observe(4.0);

	Histogram histo3("histo_3"s, {
		/*.help =*/ "A histogram with labels"s,
		/*.labelNames =*/ {"label"s},
		/*.buckets =*/ {1.0, 2.0}
	});

	histo3.labels({"value1"s}).observe(1.0);
	histo3.labels({"value1"s}).observe(2.0);

	std::ostringstream stream;
	TextExporter exporter(stream);
	Registry::defaultRegistry().exportTo(exporter);

	const std::string text = stream.str();
	assertEqual(
		"# TYPE histo_1 histogram\n"
		"histo_1_bucket{le=\"+Inf\"} 2\n"
		"histo_1_sum 3\n"
		"histo_1_count 2\n"
		"# HELP histo_2 A histogram with buckets\n"
		"# TYPE histo_2 histogram\n"
		"histo_2_bucket{le=\"1\"} 0\n"
		"histo_2_bucket{le=\"2\"} 2\n"
		"histo_2_bucket{le=\"3\"} 3\n"
		"histo_2_bucket{le=\"+Inf\"} 4\n"
		"histo_2_sum 9.6\n"
		"histo_2_count 4\n"
		"# HELP histo_3 A histogram with labels\n"
		"# TYPE histo_3 histogram\n"
		"histo_3_bucket{label=\"value1\",le=\"1\"} 1\n"
		"histo_3_bucket{label=\"value1\",le=\"2\"} 2\n"
		"histo_3_bucket{label=\"value1\",le=\"+Inf\"} 2\n"
		"histo_3_sum{label=\"value1\"} 3\n"
		"histo_3_count{label=\"value1\"} 2\n"s,
		text);
}


void HistogramTest::setUp()
{
	Registry::defaultRegistry().clear();
}


void HistogramTest::tearDown()
{
}


CppUnit::Test* HistogramTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HistogramTest");

	CppUnit_addTest(pSuite, HistogramTest, testNoBuckets);
	CppUnit_addTest(pSuite, HistogramTest, testBuckets);
	CppUnit_addTest(pSuite, HistogramTest, testLabels);
	CppUnit_addTest(pSuite, HistogramTest, testExport);

	return pSuite;
}
