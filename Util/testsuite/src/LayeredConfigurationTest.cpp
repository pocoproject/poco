//
// LayeredConfigurationTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "LayeredConfigurationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/Util/MapConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <exception>
#include <string>
#include <thread>
#include <vector>


using Poco::Util::AbstractConfiguration;
using Poco::Util::LayeredConfiguration;
using Poco::Util::MapConfiguration;
using Poco::AutoPtr;
using Poco::InvalidArgumentException;
using Poco::NotFoundException;
using Poco::NullPointerException;
using Poco::RuntimeException;


namespace
{
	const std::string REPLACED_LABEL("replaced");
	const std::string OTHER_LABEL("other");
	const std::string REPLACED_KEY("replaced.key");
	// A lower priority value takes precedence.
	constexpr int HIGHEST_PRIORITY = -2;
	constexpr int HIGH_PRIORITY = -1;
	constexpr int BASE_PRIORITY = 0;
	constexpr int LOW_PRIORITY = 1;
	constexpr int READER_COUNT = 4;
	constexpr int REPLACE_COUNT = 20000;
	constexpr auto READER_START_TIMEOUT = std::chrono::seconds(10);
}


LayeredConfigurationTest::LayeredConfigurationTest(const std::string& name): AbstractConfigurationTest(name)
{
}


LayeredConfigurationTest::~LayeredConfigurationTest()
{
}


void LayeredConfigurationTest::testEmpty()
{
	AutoPtr<LayeredConfiguration> pLC = new LayeredConfiguration;

	AbstractConfiguration::Keys keys;
	pLC->keys(keys);
	assertTrue (keys.empty());

	assertTrue (!pLC->hasProperty("foo"));
	try
	{
		pLC->setString("foo", "bar");
		fail("empty LayeredConfiguration - must throw");
	}
	catch (RuntimeException&)
	{
	}

	try
	{
		std::string s = pLC->getString("foo");
		fail("empty LayeredConfiguration - must throw");
	}
	catch (NotFoundException&)
	{
	}
}


void LayeredConfigurationTest::testOneLayer()
{
	AutoPtr<LayeredConfiguration> pLC = new LayeredConfiguration;
	AutoPtr<MapConfiguration> pMC = new MapConfiguration;

	pMC->setString("prop1", "value1");
	pMC->setString("prop2", "value2");

	pLC->addWriteable(pMC, 0);

	AbstractConfiguration::Keys keys;
	pLC->keys(keys);
	assertTrue (keys.size() == 2);
	assertTrue (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop2") != keys.end());

	assertTrue (pLC->getString("prop1") == "value1");
	assertTrue (pLC->getString("prop2") == "value2");

	pLC->setString("prop3", "value3");
	assertTrue (pLC->getString("prop3") == "value3");

	pLC->remove("prop3");
	assertTrue (!pLC->hasProperty("prop3"));
}


void LayeredConfigurationTest::testTwoLayers()
{
	AutoPtr<LayeredConfiguration> pLC = new LayeredConfiguration;
	AutoPtr<MapConfiguration> pMC1 = new MapConfiguration;
	AutoPtr<MapConfiguration> pMC2 = new MapConfiguration;

	pMC1->setString("prop1", "value1");
	pMC1->setString("prop2", "value2");
	pMC2->setString("prop2", "value3");
	pMC2->setString("prop3", "value4");

	pLC->add(pMC1, 0);
	pLC->addWriteable(pMC2, 1);

	AbstractConfiguration::Keys keys;
	pLC->keys(keys);
	assertTrue (keys.size() == 3);
	assertTrue (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop3") != keys.end());

	assertTrue (pLC->getString("prop1") == "value1");
	assertTrue (pLC->getString("prop2") == "value2");
	assertTrue (pLC->getString("prop3") == "value4");

	pLC->setString("prop4", "value4");
	assertTrue (pLC->getString("prop4") == "value4");

	assertTrue (!pMC1->hasProperty("prop4"));
	assertTrue (pMC2->hasProperty("prop4"));

	pLC->setString("prop1", "value11");
	assertTrue (pLC->getString("prop1") == "value1");
	assertTrue (pMC2->getString("prop1") == "value11");

	pLC->remove("prop1");
	assertTrue (pLC->getString("prop1") == "value1");
	assertTrue (!pMC2->hasProperty("prop1"));
}


void LayeredConfigurationTest::testThreeLayers()
{
	AutoPtr<LayeredConfiguration> pLC = new LayeredConfiguration;
	AutoPtr<MapConfiguration> pMC1 = new MapConfiguration;
	AutoPtr<MapConfiguration> pMC2 = new MapConfiguration;
	AutoPtr<MapConfiguration> pMC3 = new MapConfiguration;

	pMC1->setString("prop1", "value1");
	pMC1->setString("prop2", "value2");
	pMC1->setString("prop3", "value3");
	pMC2->setString("prop2", "value4");
	pMC2->setString("prop4", "value5");
	pMC3->setString("prop5", "value6");
	pMC3->setString("prop1", "value7");

	pLC->add(pMC1, 0);
	pLC->add(pMC2, 1);
	pLC->add(pMC3, -1);

	assertTrue (pLC->getString("prop1") == "value7");
	assertTrue (pLC->getString("prop2") == "value2");
	assertTrue (pLC->getString("prop3") == "value3");
	assertTrue (pLC->getString("prop4") == "value5");
	assertTrue (pLC->getString("prop5") == "value6");
}


void LayeredConfigurationTest::testRemove()
{
	AutoPtr<LayeredConfiguration> pLC = new LayeredConfiguration;
	AutoPtr<MapConfiguration> pMC1 = new MapConfiguration;
	AutoPtr<MapConfiguration> pMC2 = new MapConfiguration;

	pMC1->setString("prop1", "value1");
	pMC1->setString("prop2", "value2");
	pMC2->setString("prop2", "value3");
	pMC2->setString("prop3", "value4");

	pLC->add(pMC1, 0);
	pLC->add(pMC2, -1);

	AbstractConfiguration::Keys keys;
	pLC->keys(keys);

	assertTrue (keys.size() == 3);
	assertTrue (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop3") != keys.end());

	assertTrue (pLC->getString("prop1") == "value1");
	assertTrue (pLC->getString("prop2") == "value3");
	assertTrue (pLC->getString("prop3") == "value4");

	pLC->removeConfiguration(pMC2);
	keys.clear();
	pLC->keys(keys);

	assertTrue (keys.size() == 2);
	assertTrue (pLC->getString("prop1") == "value1");
	assertTrue (pLC->getString("prop2") == "value2");
}


void LayeredConfigurationTest::testFind()
{
	LayeredConfiguration::Ptr pLC = new LayeredConfiguration;
	AbstractConfiguration::Ptr pMC1 = new MapConfiguration;
	AbstractConfiguration::Ptr pMC2 = new MapConfiguration;

	pLC->add(pMC1, 0);
	pLC->add(pMC2, "label", -1);

	AbstractConfiguration::Ptr pFound = pLC->find("label");
	assertTrue (pFound == pMC2);

	pFound = pLC->find("notfound");
	assertTrue (pFound.isNull());
}


void LayeredConfigurationTest::testReplace()
{
	LayeredConfiguration::Ptr pLC = new LayeredConfiguration;
	AbstractConfiguration::Ptr pBase = new MapConfiguration;
	AbstractConfiguration::Ptr pOld = new MapConfiguration;
	AbstractConfiguration::Ptr pNew = new MapConfiguration;
	AbstractConfiguration::Ptr pOther = new MapConfiguration;

	pBase->setString("prop1", "base1");
	pBase->setString("prop2", "base2");
	pOld->setString("prop1", "old1");
	pOld->setString("prop3", "old3");
	pNew->setString("prop1", "new1");
	pNew->setString("prop4", "new4");
	pOther->setString("prop2", "other2");

	pLC->addWriteable(pBase, BASE_PRIORITY);
	pLC->add(pOld, REPLACED_LABEL, HIGH_PRIORITY);

	pLC->replace(pNew, REPLACED_LABEL, HIGH_PRIORITY);
	assertTrue (pLC->find(REPLACED_LABEL) == pNew);
	assertEqual ("new1", pLC->getString("prop1"));
	assertEqual ("new4", pLC->getString("prop4"));
	assertTrue (!pLC->hasProperty("prop3"));

	pLC->replace(pOld, REPLACED_LABEL, LOW_PRIORITY);
	assertTrue (pLC->find(REPLACED_LABEL) == pOld);
	assertEqual ("base1", pLC->getString("prop1"));
	assertEqual ("old3", pLC->getString("prop3"));
	assertTrue (!pLC->hasProperty("prop4"));

	pLC->replace(pOther, OTHER_LABEL, HIGHEST_PRIORITY);
	assertTrue (pLC->find(OTHER_LABEL) == pOther);
	assertTrue (pLC->find(REPLACED_LABEL) == pOld);
	assertEqual ("other2", pLC->getString("prop2"));

	pLC->replace(pNew, REPLACED_LABEL, HIGHEST_PRIORITY, true);
	pLC->setString("prop5", "value5");
	assertEqual ("value5", pNew->getString("prop5"));
	assertTrue (!pBase->hasProperty("prop5"));
	assertEqual ("new1", pLC->getString("prop1"));

	try
	{
		pLC->replace(pOld, "", HIGH_PRIORITY);
		fail("empty label - must throw");
	}
	catch (InvalidArgumentException&)
	{
	}

	try
	{
		pLC->replace(nullptr, REPLACED_LABEL, HIGH_PRIORITY);
		fail("null configuration - must throw");
	}
	catch (NullPointerException&)
	{
	}
	assertTrue (pLC->find(REPLACED_LABEL) == pNew);

	LayeredConfiguration::Ptr pSameLabelLC = new LayeredConfiguration;
	AbstractConfiguration::Ptr pFirstLabeled = new MapConfiguration;
	AbstractConfiguration::Ptr pSecondLabeled = new MapConfiguration;
	AbstractConfiguration::Ptr pReplacement = new MapConfiguration;
	pFirstLabeled->setString("a", "1");
	pSecondLabeled->setString("b", "2");
	pReplacement->setString("a", "3");
	pSameLabelLC->add(pFirstLabeled, REPLACED_LABEL, BASE_PRIORITY);
	pSameLabelLC->add(pSecondLabeled, REPLACED_LABEL, LOW_PRIORITY);

	pSameLabelLC->replace(pReplacement, REPLACED_LABEL, BASE_PRIORITY);
	assertTrue (pSameLabelLC->find(REPLACED_LABEL) == pReplacement);
	assertEqual ("3", pSameLabelLC->getString("a"));
	assertEqual ("2", pSameLabelLC->getString("b"));
}


void LayeredConfigurationTest::testReplaceConcurrent()
{
	LayeredConfiguration::Ptr pLC = new LayeredConfiguration;
	AbstractConfiguration::Ptr pBase = new MapConfiguration;
	AbstractConfiguration::Ptr pFirst = new MapConfiguration;
	AbstractConfiguration::Ptr pSecond = new MapConfiguration;

	pBase->setString("prop1", "base1");
	pFirst->setString(REPLACED_KEY, "first");
	pSecond->setString(REPLACED_KEY, "second");

	pLC->add(pBase, BASE_PRIORITY);
	pLC->add(pFirst, REPLACED_LABEL, HIGH_PRIORITY);

	std::atomic<bool> writerDone(false);
	std::atomic<int> readersStarted(0);
	std::atomic<int> misses(0);
	bool allReadersStarted = false;
	std::string writerError;

	const auto read = [&pLC, &writerDone, &readersStarted, &misses]()
	{
		++readersStarted;
		while (!writerDone)
		{
			if (!pLC->hasProperty(REPLACED_KEY)) ++misses;
		}
	};
	const auto write = [&pLC, &pFirst, &pSecond, &writerDone, &readersStarted, &allReadersStarted, &writerError]()
	{
		try
		{
			// Start replacing only after all readers have started, so that the replacements overlap with reads.
			const auto deadline = std::chrono::steady_clock::now() + READER_START_TIMEOUT;
			while (readersStarted < READER_COUNT && std::chrono::steady_clock::now() < deadline)
			{
				std::this_thread::yield();
			}
			allReadersStarted = (readersStarted == READER_COUNT);
			for (int i = 0; allReadersStarted && i < REPLACE_COUNT; ++i)
			{
				pLC->replace((i % 2 == 0) ? pSecond : pFirst, REPLACED_LABEL, HIGH_PRIORITY);
			}
		}
		catch (const Poco::Exception& exc)
		{
			writerError = exc.displayText();
		}
		catch (const std::exception& exc)
		{
			writerError = exc.what();
		}
		writerDone = true;
	};

	std::vector<std::thread> threads;
	try
	{
		for (int i = 0; i < READER_COUNT; ++i) threads.emplace_back(read);
		threads.emplace_back(write);
	}
	catch (...)
	{
		// The readers end only when the writer is done.
		writerDone = true;
		for (auto& thread: threads) thread.join();
		throw;
	}
	for (auto& thread: threads) thread.join();

	if (!writerError.empty()) failmsg(writerError);
	assertTrue (allReadersStarted);
	assertEqual (0, misses.load());
}


AbstractConfiguration::Ptr LayeredConfigurationTest::allocConfiguration() const
{
	LayeredConfiguration* pLC = new LayeredConfiguration;
	AutoPtr<MapConfiguration> pMC1 = new MapConfiguration;
	AutoPtr<MapConfiguration> pMC2 = new MapConfiguration;

	pLC->add(pMC1, 0);
	pLC->addWriteable(pMC2, 1);

	return pLC;
}


void LayeredConfigurationTest::setUp()
{
}


void LayeredConfigurationTest::tearDown()
{
}


CppUnit::Test* LayeredConfigurationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LayeredConfigurationTest");

	AbstractConfigurationTest_addTests(pSuite, LayeredConfigurationTest);
	CppUnit_addTest(pSuite, LayeredConfigurationTest, testEmpty);
	CppUnit_addTest(pSuite, LayeredConfigurationTest, testOneLayer);
	CppUnit_addTest(pSuite, LayeredConfigurationTest, testTwoLayers);
	CppUnit_addTest(pSuite, LayeredConfigurationTest, testThreeLayers);
	CppUnit_addTest(pSuite, LayeredConfigurationTest, testRemove);
	CppUnit_addTest(pSuite, LayeredConfigurationTest, testFind);
	CppUnit_addTest(pSuite, LayeredConfigurationTest, testReplace);
	CppUnit_addTest(pSuite, LayeredConfigurationTest, testReplaceConcurrent);

	return pSuite;
}
