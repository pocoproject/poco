//
// ConfigurationMapperTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ConfigurationMapperTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/ConfigurationMapper.h"
#include "Poco/Util/MapConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include <algorithm>


using Poco::Util::AbstractConfiguration;
using Poco::Util::ConfigurationMapper;
using Poco::Util::MapConfiguration;
using Poco::AutoPtr;


ConfigurationMapperTest::ConfigurationMapperTest(const std::string& name): AbstractConfigurationTest(name)
{
}


ConfigurationMapperTest::~ConfigurationMapperTest()
{
}


void ConfigurationMapperTest::testMapper1()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();
	AutoPtr<AbstractConfiguration> pMapper = new ConfigurationMapper("", "", pConf);
	assertTrue (pMapper->hasProperty("prop5.string1"));
	assertTrue (pMapper->hasProperty("prop5.string1"));

	AbstractConfiguration::Keys keys;
	pMapper->keys(keys);
	assertTrue (keys.size() == 13);
	assertTrue (std::find(keys.begin(), keys.end(), "prop5") != keys.end());

	pMapper->keys("prop5", keys);
	assertTrue (keys.size() == 4);
	assertTrue (std::find(keys.begin(), keys.end(), "string1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "string2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "sub1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "sub2") != keys.end());
	
	assertTrue (pMapper->getString("prop5.string1") == "foo");
	assertTrue (pMapper->getString("prop5.sub1.string1") == "FOO");
	
	pMapper->setString("prop5.string3", "baz");
	assertTrue (pMapper->getString("prop5.string3") == "baz");
	assertTrue (pConf->getString("prop5.string3") == "baz");

	pMapper->remove("prop5.string3");
	assertTrue (!pMapper->hasProperty("prop5.string3"));
	assertTrue (!pConf->hasProperty("prop5.string3"));
}


void ConfigurationMapperTest::testMapper2()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();
	AutoPtr<AbstractConfiguration> pMapper = new ConfigurationMapper("prop5", "root.conf", pConf);

	assertTrue (pMapper->hasProperty("root.conf.string1"));
	assertTrue (pMapper->hasProperty("root.conf.string2"));

	AbstractConfiguration::Keys keys;
	pMapper->keys(keys);
	assertTrue (keys.size() == 1);
	assertTrue (std::find(keys.begin(), keys.end(), "root") != keys.end());

	pMapper->keys("root", keys);
	assertTrue (keys.size() == 1);
	assertTrue (std::find(keys.begin(), keys.end(), "conf") != keys.end());

	pMapper->keys("root.conf", keys);
	assertTrue (keys.size() == 4);
	assertTrue (std::find(keys.begin(), keys.end(), "string1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "string2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "sub1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "sub2") != keys.end());

	assertTrue (pMapper->getString("root.conf.string1") == "foo");
	assertTrue (pMapper->getString("root.conf.sub1.string1") == "FOO");
	
	pMapper->setString("root.conf.string3", "baz");
	assertTrue (pMapper->getString("root.conf.string3") == "baz");
	assertTrue (pConf->getString("prop5.string3") == "baz");

	pMapper->remove("root.conf.string3");
	assertTrue (!pMapper->hasProperty("root.conf.string3"));
	assertTrue (!pConf->hasProperty("prop5.string3"));
}


void ConfigurationMapperTest::testMapper3()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();
	AutoPtr<AbstractConfiguration> pMapper = new ConfigurationMapper("", "root", pConf);

	assertTrue (pMapper->hasProperty("root.prop5.string1"));
	assertTrue (pMapper->hasProperty("root.prop5.string2"));

	AbstractConfiguration::Keys keys;
	pMapper->keys(keys);
	assertTrue (keys.size() == 1);
	assertTrue (std::find(keys.begin(), keys.end(), "root") != keys.end());

	pMapper->keys("root", keys);
	assertTrue (keys.size() == 13);
	assertTrue (std::find(keys.begin(), keys.end(), "prop5") != keys.end());

	pMapper->keys("root.prop5", keys);
	assertTrue (keys.size() == 4);
	assertTrue (std::find(keys.begin(), keys.end(), "string1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "string2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "sub1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "sub2") != keys.end());
	
	assertTrue (pMapper->getString("root.prop5.string1") == "foo");
	assertTrue (pMapper->getString("root.prop5.sub1.string1") == "FOO");
	
	pMapper->setString("root.prop5.string3", "baz");
	assertTrue (pMapper->getString("root.prop5.string3") == "baz");
	assertTrue (pConf->getString("prop5.string3") == "baz");

	pMapper->remove("root.prop5.string3");
	assertTrue (!pMapper->hasProperty("root.prop5.string3"));
	assertTrue (!pConf->hasProperty("prop5.string3"));
}


void ConfigurationMapperTest::testMapper4()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();
	AutoPtr<AbstractConfiguration> pMapper = new ConfigurationMapper("prop5", "", pConf);

	assertTrue (pMapper->hasProperty("string1"));
	assertTrue (pMapper->hasProperty("string2"));

	AbstractConfiguration::Keys keys;
	pMapper->keys(keys);
	assertTrue (keys.size() == 4);
	assertTrue (std::find(keys.begin(), keys.end(), "string1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "string2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "sub1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "sub2") != keys.end());
	
	assertTrue (pMapper->getString("string1") == "foo");
	assertTrue (pMapper->getString("sub1.string1") == "FOO");
	
	pMapper->setString("string3", "baz");
	assertTrue (pMapper->getString("string3") == "baz");
	assertTrue (pConf->getString("prop5.string3") == "baz");

	pMapper->remove("string3");
	assertTrue (!pMapper->hasProperty("string3"));
	assertTrue (!pConf->hasProperty("prop5.string3"));
}


AbstractConfiguration::Ptr ConfigurationMapperTest::allocConfiguration() const
{
	return new MapConfiguration;
}


void ConfigurationMapperTest::setUp()
{
}


void ConfigurationMapperTest::tearDown()
{
}


CppUnit::Test* ConfigurationMapperTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ConfigurationMapperTest");

	AbstractConfigurationTest_addTests(pSuite, ConfigurationMapperTest);
	CppUnit_addTest(pSuite, ConfigurationMapperTest, testMapper1);
	CppUnit_addTest(pSuite, ConfigurationMapperTest, testMapper2);
	CppUnit_addTest(pSuite, ConfigurationMapperTest, testMapper3);
	CppUnit_addTest(pSuite, ConfigurationMapperTest, testMapper4);

	return pSuite;
}
