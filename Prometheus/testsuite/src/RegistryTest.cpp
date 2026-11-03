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
#include "Poco/Prometheus/CallbackMetric.h"
#include "Poco/Prometheus/Collector.h"
#include "Poco/Prometheus/Counter.h"
#include "Poco/Prometheus/Gauge.h"
#include "Poco/Prometheus/Histogram.h"
#include "Poco/Prometheus/IntCounter.h"
#include "Poco/Prometheus/IntGauge.h"
#include "Poco/Prometheus/Metric.h"
#include "Poco/Prometheus/ProcessCollector.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/TextExporter.h"
#include "Poco/Prometheus/ThreadPoolCollector.h"
#include "Poco/Event.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include <atomic>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


using namespace Poco::Prometheus;
using namespace std::string_literals;


namespace
{
	class HoldingExporter: public TextExporter
		/// Yields in every header while Registry::exportTo() holds the
		/// registry mutex, so that a thread that constructs or destroys
		/// a collector at the same time has to wait for the export.
	{
	public:
		HoldingExporter() = delete;

		explicit HoldingExporter(std::ostream& stream):
			TextExporter(stream)
		{
		}

		~HoldingExporter() override = default;

		HoldingExporter(const HoldingExporter&) = delete;
		HoldingExporter(HoldingExporter&&) = delete;
		HoldingExporter& operator = (const HoldingExporter&) = delete;
		HoldingExporter& operator = (HoldingExporter&&) = delete;

		void writeHeader(const Metric& metric) override
		{
			Poco::Thread::yield();
			TextExporter::writeHeader(metric);
		}
	};


	class ExportThread
		/// Exports a Registry through a HoldingExporter in a loop,
		/// from the first export pass until it is destroyed.
	{
	public:
		ExportThread() = delete;

		POCO_NODISCARD_CTOR explicit ExportThread(const Registry& registry):
			_registry(registry)
		{
			_thread.startFunc([this]()
			{
				run();
			});
			_exporting.wait();
		}

		~ExportThread()
		{
			_stop = true;
			_thread.join();
		}

		ExportThread(const ExportThread&) = delete;
		ExportThread(ExportThread&&) = delete;
		ExportThread& operator = (const ExportThread&) = delete;
		ExportThread& operator = (ExportThread&&) = delete;

	private:
		void run()
		{
			while (!_stop)
			{
				std::ostringstream stream;
				HoldingExporter exporter(stream);
				_registry.exportTo(exporter);
				_exporting.set();
				// The mutex is not fair on every platform; without a yield,
				// the thread under test can wait through many export passes.
				Poco::Thread::yield();
			}
		}

		const Registry& _registry;
		std::atomic<bool> _stop{false};
		Poco::Event _exporting;
		Poco::Thread _thread;
	};
}


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


void RegistryTest::testConcurrentExportAndConstruction()
{
	constexpr int ROUNDS = 10;

	Registry registry;
	std::vector<std::unique_ptr<Collector>> collectors;
	{
		ExportThread exportThread(registry);
		for (int i = 0; i < ROUNDS; ++i)
		{
			const std::string suffix = "_"s + std::to_string(i);
			collectors.push_back(std::make_unique<Counter>(
				"counter"s + suffix, Counter::Params{"A test counter"s, {"label"s}}, &registry));
			collectors.push_back(std::make_unique<Gauge>(
				"gauge"s + suffix, Gauge::Params{"A test gauge"s, {"label"s}}, &registry));
			collectors.push_back(std::make_unique<Histogram>(
				"histogram"s + suffix, Histogram::Params{"A test histogram"s, {"label"s}, {1.0, 2.0}}, &registry));
			collectors.push_back(std::make_unique<IntCounter>(
				"int_counter"s + suffix, IntCounter::Params{"A test counter"s}, &registry));
			collectors.push_back(std::make_unique<IntGauge>(
				"int_gauge"s + suffix, IntGauge::Params{"A test gauge"s}, &registry));
			collectors.push_back(std::make_unique<CallbackGauge>(
				"callback_gauge"s + suffix, "A test gauge"s, &registry, []()
				{
					return 1.0;
				}));
			collectors.push_back(std::make_unique<ProcessCollector>(
				"process"s + suffix, &registry));
			collectors.push_back(std::make_unique<ThreadPoolCollector>(
				"pool"s + suffix, Poco::ThreadPool::defaultPool(), &registry));
		}
	}

	for (const auto& pCollector: collectors)
	{
		assertTrue (registry.findCollector(pCollector->name()) == pCollector.get());
	}
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
	CppUnit_addTest(pSuite, RegistryTest, testConcurrentExportAndConstruction);

	return pSuite;
}
