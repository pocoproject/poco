//
// FilesystemConfigurationTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FilesystemConfigurationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/FilesystemConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/File.h"
#include <algorithm>


using Poco::Util::FilesystemConfiguration;
using Poco::Util::AbstractConfiguration;
using Poco::AutoPtr;


FilesystemConfigurationTest::FilesystemConfigurationTest(const std::string& name): AbstractConfigurationTest(name),
	_path("TestConfiguration")
{
}


FilesystemConfigurationTest::~FilesystemConfigurationTest()
{
}


void FilesystemConfigurationTest::testFilesystemConfiguration()
{
	AutoPtr<FilesystemConfiguration> config = new FilesystemConfiguration(_path.toString());

	config->setString("logging.loggers.root.channel.class", "ConsoleChannel");
	config->setString("logging.loggers.app.name", "Application");
	config->setString("logging.loggers.app.channel", "c1");
	config->setString("logging.formatters.f1.class", "PatternFormatter");
	config->setString("logging.formatters.f1.pattern", "[%p] %t");
	config->setString("logging.channels.c1.class", "ConsoleChannel");

	assertTrue (config->getString("logging.loggers.root.channel.class") == "ConsoleChannel");
	assertTrue (config->getString("logging.loggers.app.name") == "Application");
	assertTrue (config->getString("logging.loggers.app.channel") == "c1");
	assertTrue (config->getString("logging.formatters.f1.class") == "PatternFormatter");
	assertTrue (config->getString("logging.formatters.f1.pattern") == "[%p] %t");

	config->setString("logging.loggers.app.channel", "c2");
	assertTrue (config->getString("logging.loggers.app.channel") == "c2");

	AbstractConfiguration::Keys keys;
	config->keys(keys);
	assertTrue (keys.size() == 1);
	assertTrue (std::find(keys.begin(), keys.end(), "logging") != keys.end());

	config->keys("logging", keys);
	assertTrue (keys.size() == 3);
	assertTrue (std::find(keys.begin(), keys.end(), "loggers") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "formatters") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "channels") != keys.end());

	config->keys("logging.formatters", keys);
	assertTrue (keys.size() == 1);
	assertTrue (std::find(keys.begin(), keys.end(), "f1") != keys.end());

	config->keys("logging.formatters.f1", keys);
	assertTrue (keys.size() == 2);
	assertTrue (std::find(keys.begin(), keys.end(), "class") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "pattern") != keys.end());

	assertTrue (config->hasProperty("logging.loggers.root.channel.class"));
	config->clear();
	assertTrue (!config->hasProperty("logging.loggers.root.channel.class"));
}


AbstractConfiguration::Ptr FilesystemConfigurationTest::allocConfiguration() const
{
	return new FilesystemConfiguration(_path.toString());
}


void FilesystemConfigurationTest::setUp()
{
	Poco::File dir(_path);
	if (dir.exists()) {
		dir.remove(true);
	}
}


void FilesystemConfigurationTest::tearDown()
{
	Poco::File dir(_path);
	if (dir.exists()) {
		dir.remove(true);
	}
}


CppUnit::Test* FilesystemConfigurationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FilesystemConfigurationTest");

	AbstractConfigurationTest_addTests(pSuite, FilesystemConfigurationTest);
	CppUnit_addTest(pSuite, FilesystemConfigurationTest, testFilesystemConfiguration);

	return pSuite;
}
