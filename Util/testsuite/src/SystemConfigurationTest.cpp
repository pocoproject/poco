//
// SystemConfigurationTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SystemConfigurationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/SystemConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/Environment.h"
#include "Poco/Path.h"
#if !defined(POCO_VXWORKS)
#include "Poco/Process.h"
#endif
#include "Poco/NumberParser.h"
#include <algorithm>


using Poco::Util::SystemConfiguration;
using Poco::Util::AbstractConfiguration;
using Poco::AutoPtr;
using Poco::Environment;
using Poco::Path;
using Poco::InvalidAccessException;
using Poco::NotFoundException;


SystemConfigurationTest::SystemConfigurationTest(const std::string& name): CppUnit::TestCase(name)
{
}


SystemConfigurationTest::~SystemConfigurationTest()
{
}


void SystemConfigurationTest::testProperties()
{
	AutoPtr<SystemConfiguration> pConf = new SystemConfiguration;

	assertTrue (pConf->getString("system.osName") == Environment::osName());
	assertTrue (pConf->getString("system.osVersion") == Environment::osVersion());
	assertTrue (pConf->getString("system.osArchitecture") == Environment::osArchitecture());
	assertTrue (pConf->getString("system.nodeName") == Environment::nodeName());
	assertTrue (pConf->getString("system.currentDir") == Path::current());
	assertTrue (pConf->getString("system.homeDir") == Path::home());
	assertTrue (pConf->getString("system.tempDir") == Path::temp());

	std::string dateTime = pConf->getString("system.dateTime");
	assertTrue (dateTime.size() == 20);

#if !defined(POCO_VXWORKS)
	std::string pid = pConf->getString("system.pid");
	assertTrue (Poco::NumberParser::parse64(pid) == Poco::Process::id());
#endif

#if defined(POCO_OS_FAMILY_WINDOWS)
	std::string home = pConf->getString("system.env.HOMEPATH");
#else
	std::string home = pConf->getString("system.env.HOME");
#endif
	assertTrue (!home.empty());
}


void SystemConfigurationTest::testKeys()
{
	AutoPtr<SystemConfiguration> pConf = new SystemConfiguration;

	AbstractConfiguration::Keys keys;
	pConf->keys(keys);
	assertTrue (keys.size() == 1);
	assertTrue (std::find(keys.begin(), keys.end(), "system") != keys.end());

	pConf->keys("system", keys);
#if defined(POCO_VXWORKS)
	assertTrue (keys.size() == 10);
#else
	assertTrue (keys.size() == 16);
#endif
	assertTrue (std::find(keys.begin(), keys.end(), "osName") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "osVersion") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "osArchitecture") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "nodeName") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "nodeId") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "currentDir") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "homeDir") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "configHomeDir") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "cacheHomeDir") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "dataHomeDir") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "tempDir") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "dateTime") != keys.end());
#if !defined(POCO_VXWORKS)
	assertTrue (std::find(keys.begin(), keys.end(), "pid") != keys.end());
#endif
	assertTrue (std::find(keys.begin(), keys.end(), "env") != keys.end());
}


void SystemConfigurationTest::setUp()
{
}


void SystemConfigurationTest::tearDown()
{
}


CppUnit::Test* SystemConfigurationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SystemConfigurationTest");

	CppUnit_addTest(pSuite, SystemConfigurationTest, testProperties);
	CppUnit_addTest(pSuite, SystemConfigurationTest, testKeys);

	return pSuite;
}
