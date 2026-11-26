//
// MapConfigurationTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MapConfigurationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/MapConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include <string>


using Poco::Util::AbstractConfiguration;
using Poco::Util::MapConfiguration;
using Poco::AutoPtr;
using Poco::CircularReferenceException;


namespace
{
	const std::string REENTRANT_PREFIX("reentrant.");

	class ReentrantConfiguration: public MapConfiguration
		/// getRaw() of a key "reentrant.<key>" returns getString("<key>")
		/// of the same configuration.
	{
	protected:
		~ReentrantConfiguration() override = default;

		[[nodiscard]] bool getRaw(const std::string& key, std::string& value) const override
		{
			if (key.starts_with(REENTRANT_PREFIX))
			{
				value = getString(key.substr(REENTRANT_PREFIX.size()));
				return true;
			}
			return MapConfiguration::getRaw(key, value);
		}
	};
}


MapConfigurationTest::MapConfigurationTest(const std::string& name): AbstractConfigurationTest(name)
{
}


MapConfigurationTest::~MapConfigurationTest()
{
}


void MapConfigurationTest::testClear()
{
	AutoPtr<MapConfiguration> pConf = new MapConfiguration;

	pConf->setString("foo", "bar");
	assertTrue (pConf->hasProperty("foo"));

	pConf->clear();
	assertTrue (!pConf->hasProperty("foo"));
}


void MapConfigurationTest::testReentrantGetRaw()
{
	AutoPtr<ReentrantConfiguration> pConf = new ReentrantConfiguration;

	pConf->setString("loop", "${reentrant.loop}");
	try
	{
		(void) pConf->getString("loop");
		fail("circular reference - must throw");
	}
	catch (CircularReferenceException&)
	{
	}

	// Each nested getString() call counts toward the depth limit: reading
	// chain9 nests ten getString() calls (chain9 to chain0) and works,
	// reading chain10 nests eleven and throws.
	pConf->setString("chain0", "x");
	for (int i = 0; i < MAX_REFERENCE_DEPTH; ++i)
	{
		pConf->setString("chain" + std::to_string(i + 1), "${reentrant.chain" + std::to_string(i) + "}");
	}
	assertTrue (pConf->getString("chain" + std::to_string(MAX_REFERENCE_DEPTH - 1)) == "x");
	try
	{
		(void) pConf->getString("chain" + std::to_string(MAX_REFERENCE_DEPTH));
		fail("too many nested references - must throw");
	}
	catch (CircularReferenceException&)
	{
	}
}


AbstractConfiguration::Ptr MapConfigurationTest::allocConfiguration() const
{
	return new MapConfiguration;
}


void MapConfigurationTest::setUp()
{
}


void MapConfigurationTest::tearDown()
{
}


CppUnit::Test* MapConfigurationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MapConfigurationTest");

	AbstractConfigurationTest_addTests(pSuite, MapConfigurationTest);
	CppUnit_addTest(pSuite, MapConfigurationTest, testClear);
	CppUnit_addTest(pSuite, MapConfigurationTest, testReentrantGetRaw);

	return pSuite;
}
