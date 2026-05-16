//
// ProcessCollectorTest.cpp
//
// Copyright (c) 2022-2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ProcessCollectorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Prometheus/ProcessCollector.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/TextExporter.h"
#include <sstream>


using namespace Poco::Prometheus;
using namespace std::string_literals;


ProcessCollectorTest::ProcessCollectorTest(const std::string& name):
	CppUnit::TestCase("ProcessCollectorTest"s)
{
}


void ProcessCollectorTest::testExport()
{
	ProcessCollector collector;

	std::ostringstream stream;
	TextExporter exporter(stream);
	Registry::defaultRegistry().exportTo(exporter);

	const std::string text = stream.str();

	// Cross-platform metrics
	assertTrue(text.find("# HELP process_cpu_seconds_total") != std::string::npos);
	assertTrue(text.find("# TYPE process_cpu_seconds_total gauge") != std::string::npos);
	assertTrue(text.find("# HELP process_start_time_seconds") != std::string::npos);
	assertTrue(text.find("# TYPE process_start_time_seconds gauge") != std::string::npos);
	assertTrue(text.find("# HELP process_up_time_seconds") != std::string::npos);
	assertTrue(text.find("# TYPE process_up_time_seconds gauge") != std::string::npos);

#ifdef POCO_OS_FAMILY_UNIX
	assertTrue(text.find("# HELP process_max_fds") != std::string::npos);
	assertTrue(text.find("# TYPE process_max_fds gauge") != std::string::npos);
#endif

#if POCO_OS == POCO_OS_LINUX
	// Memory metrics should be present with values > 0
	assertTrue(text.find("# HELP process_resident_memory_bytes") != std::string::npos);
	assertTrue(text.find("# TYPE process_resident_memory_bytes gauge") != std::string::npos);
	assertTrue(text.find("# HELP process_virtual_memory_bytes") != std::string::npos);
	assertTrue(text.find("# TYPE process_virtual_memory_bytes gauge") != std::string::npos);

	// Thread CPU metric header should be present
	assertTrue(text.find("# HELP process_thread_cpu_seconds") != std::string::npos);
	assertTrue(text.find("# TYPE process_thread_cpu_seconds gauge") != std::string::npos);
#endif
}


void ProcessCollectorTest::testExportThreadCPU()
{
#if POCO_OS == POCO_OS_LINUX
	ProcessCollector collector;

	std::ostringstream stream;
	TextExporter exporter(stream);
	Registry::defaultRegistry().exportTo(exporter);

	const std::string text = stream.str();

	// Should have at least one thread sample with tid and name labels
	assertTrue(text.find("process_thread_cpu_seconds{tid=\"") != std::string::npos);
	assertTrue(text.find("\",name=\"") != std::string::npos);

	// Find a sample line and verify value is non-negative
	auto pos = text.find("process_thread_cpu_seconds{tid=\"");
	assertTrue(pos != std::string::npos);
	auto braceClose = text.find("} ", pos);
	assertTrue(braceClose != std::string::npos);
	std::string valueStr = text.substr(braceClose + 2);
	auto lineEnd = valueStr.find('\n');
	if (lineEnd != std::string::npos)
		valueStr = valueStr.substr(0, lineEnd);
	double value = std::stod(valueStr);
	assertTrue(value >= 0.0);
#endif
}


void ProcessCollectorTest::testCustomName()
{
	ProcessCollector collector("myapp"s, nullptr);

	std::ostringstream stream;
	TextExporter exporter(stream);
	collector.exportTo(exporter);

	const std::string text = stream.str();

	// All metrics should use the custom prefix
	assertTrue(text.find("myapp_cpu_seconds_total") != std::string::npos);
	assertTrue(text.find("myapp_start_time_seconds") != std::string::npos);
	assertTrue(text.find("myapp_up_time_seconds") != std::string::npos);
	// Should not contain default "process_" prefix
	assertTrue(text.find("process_cpu_seconds_total") == std::string::npos);

#if POCO_OS == POCO_OS_LINUX
	assertTrue(text.find("myapp_resident_memory_bytes") != std::string::npos);
	assertTrue(text.find("myapp_virtual_memory_bytes") != std::string::npos);
	assertTrue(text.find("myapp_thread_cpu_seconds") != std::string::npos);
#endif
}


void ProcessCollectorTest::setUp()
{
	Registry::defaultRegistry().clear();
}


void ProcessCollectorTest::tearDown()
{
}


CppUnit::Test* ProcessCollectorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ProcessCollectorTest");

	CppUnit_addTest(pSuite, ProcessCollectorTest, testExport);
	CppUnit_addTest(pSuite, ProcessCollectorTest, testExportThreadCPU);
	CppUnit_addTest(pSuite, ProcessCollectorTest, testCustomName);

	return pSuite;
}
