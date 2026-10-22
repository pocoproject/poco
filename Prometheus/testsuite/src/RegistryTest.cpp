//
// RegistryTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "RegistryTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Prometheus/Collector.h"
#include "Poco/Prometheus/Counter.h"
#include "Poco/Prometheus/Histogram.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/TextExporter.h"
#include "Poco/Event.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include <atomic>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


using namespace Poco::Prometheus;
using namespace std::string_literals;


RegistryTest::RegistryTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


RegistryTest::~RegistryTest() = default;


void RegistryTest::testCollectorUnregistersWhenDestroyed()
{
	Registry registry;
	auto pCounter = std::make_unique<Counter>("counter"s, &registry);
	assertTrue (registry.findCollector("counter"s) == pCounter.get());

	pCounter.reset();
	assertNullPtr(registry.findCollector("counter"s));

	std::ostringstream stream;
	TextExporter exporter(stream);
	registry.exportTo(exporter);
	assertEqual(""s, stream.str());
}


void RegistryTest::testDestroyedCollectorKeepsReplacement()
{
	Registry registry;
	auto pFirst = std::make_unique<Counter>("counter"s, &registry);
	registry.clear();

	Counter second("counter"s, &registry);
	pFirst.reset();
	assertTrue (registry.findCollector("counter"s) == &second);
}


void RegistryTest::testUnregisterIgnoresOtherCollectorWithSameName()
{
	Registry registry;
	Counter registered("counter"s, &registry);
	Counter other("counter"s, nullptr);

	registry.unregisterCollector(&other);
	assertTrue (registry.findCollector("counter"s) == &registered);
}


void RegistryTest::testCollectorOutlivesRegistry()
{
	auto pRegistry = std::make_unique<Registry>();
	auto pCounter = std::make_unique<Counter>("counter"s, pRegistry.get());
	pRegistry.reset();

	Registry registry;
	registry.registerCollector(pCounter.get());
	assertTrue (registry.findCollector("counter"s) == pCounter.get());

	pCounter.reset();
	assertNullPtr(registry.findCollector("counter"s));
}


void RegistryTest::testUnregisteredCollectorCanRegisterElsewhere()
{
	auto pFirst = std::make_unique<Registry>();
	auto pByPointer = std::make_unique<Counter>("by_pointer"s, pFirst.get());
	auto pByName = std::make_unique<Counter>("by_name"s, pFirst.get());
	pFirst->unregisterCollector(pByPointer.get());
	pFirst->unregisterCollector("by_name"s);
	pFirst.reset();

	Registry second;
	second.registerCollector(pByPointer.get());
	second.registerCollector(pByName.get());
	assertTrue (second.findCollector("by_pointer"s) == pByPointer.get());
	assertTrue (second.findCollector("by_name"s) == pByName.get());

	pByPointer.reset();
	pByName.reset();
	assertNullPtr(second.findCollector("by_pointer"s));
	assertNullPtr(second.findCollector("by_name"s));
}


void RegistryTest::testRegisterWithSecondRegistryFails()
{
	Registry first;
	Registry second;
	Counter counter("counter"s, &first);

	try
	{
		second.registerCollector(&counter);
		fail("collector registered with another registry - must throw"s);
	}
	catch (Poco::ExistsException&)
	{
	}
	assertNullPtr(second.findCollector("counter"s));
	assertTrue (first.findCollector("counter"s) == &counter);
}


void RegistryTest::testConcurrentExportAndDestruction()
{
	constexpr int COLLECTOR_COUNT = 100;

	Registry registry;
	std::vector<std::unique_ptr<Collector>> collectors;
	for (int i = 0; i < COLLECTOR_COUNT; ++i)
	{
		auto pCounter = std::make_unique<Counter>("counter_"s + std::to_string(i), Counter::Params{"A test counter"s, {"label"s}}, &registry);
		pCounter->labels({"value"s}).inc();
		collectors.push_back(std::move(pCounter));

		auto pHistogram = std::make_unique<Histogram>("histogram_"s + std::to_string(i), Histogram::Params{"A test histogram"s, {"label"s}, {1.0, 2.0}}, &registry);
		pHistogram->labels({"value"s}).observe(1.5);
		collectors.push_back(std::move(pHistogram));
	}

	std::atomic<bool> stop{false};
	Poco::Event exporting;
	Poco::Thread exportThread;
	exportThread.startFunc([&registry, &stop, &exporting]()
	{
		while (!stop)
		{
			std::ostringstream stream;
			TextExporter exporter(stream);
			registry.exportTo(exporter);
			exporting.set();
			// The mutex is not fair on every platform; without a yield,
			// destroying collectors can wait through many export passes.
			Poco::Thread::yield();
		}
	});
	exporting.wait();

	collectors.clear();
	stop = true;
	exportThread.join();

	assertNullPtr(registry.findCollector("counter_0"s));
	assertNullPtr(registry.findCollector("histogram_0"s));

	std::ostringstream stream;
	TextExporter exporter(stream);
	registry.exportTo(exporter);
	assertEqual(""s, stream.str());
}


void RegistryTest::setUp()
{
}


void RegistryTest::tearDown()
{
}


CppUnit::Test* RegistryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RegistryTest");

	CppUnit_addTest(pSuite, RegistryTest, testCollectorUnregistersWhenDestroyed);
	CppUnit_addTest(pSuite, RegistryTest, testDestroyedCollectorKeepsReplacement);
	CppUnit_addTest(pSuite, RegistryTest, testUnregisterIgnoresOtherCollectorWithSameName);
	CppUnit_addTest(pSuite, RegistryTest, testCollectorOutlivesRegistry);
	CppUnit_addTest(pSuite, RegistryTest, testUnregisteredCollectorCanRegisterElsewhere);
	CppUnit_addTest(pSuite, RegistryTest, testRegisterWithSecondRegistryFails);
	CppUnit_addTest(pSuite, RegistryTest, testConcurrentExportAndDestruction);

	return pSuite;
}
