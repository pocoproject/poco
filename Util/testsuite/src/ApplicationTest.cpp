//
// ApplicationTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ApplicationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/Application.h"
#include "Poco/AutoPtr.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/TemporaryFile.h"
#include <fstream>


using Poco::AutoPtr;
using Poco::File;
using Poco::Path;
using Poco::TemporaryFile;
using Poco::Util::Application;


using namespace std::string_literals;


namespace
{
	class ConfigDirTestApplication: public Application
		/// Finds its default configuration file at the path given to the constructor.
	{
	public:
		explicit ConfigDirTestApplication(const Path& defaultConfigPath):
			_defaultConfigPath(defaultConfigPath)
		{
		}

	protected:
		~ConfigDirTestApplication() override = default;

		bool findAppConfigFile(const std::string&, const std::string& extension, Path& path) const override
		{
			if (extension != "properties"s) return false;
			path = _defaultConfigPath;
			return true;
		}

	private:
		Path _defaultConfigPath;
	};
}


ApplicationTest::ApplicationTest(const std::string& name): CppUnit::TestCase(name)
{
}


ApplicationTest::~ApplicationTest() = default;


void ApplicationTest::testConfigDirFollowsFirstLoadedFile()
{
	TemporaryFile tempDir;
	Path explicitDir(tempDir.path());
	explicitDir.makeDirectory().pushDirectory("explicit"s);
	Path defaultDir(tempDir.path());
	defaultDir.makeDirectory().pushDirectory("default"s);
	File(explicitDir).createDirectories();
	File(defaultDir).createDirectories();
	const Path explicitConfig(explicitDir, "explicit.properties"s);
	const Path defaultConfig(defaultDir, "default.properties"s);
	std::ofstream(explicitConfig.toString()) << "source = explicit\n";
	std::ofstream(defaultConfig.toString()) << "source = default\n";

	AutoPtr<ConfigDirTestApplication> pApp = new ConfigDirTestApplication(defaultConfig);
	pApp->init(Application::ArgVec{"ConfigDirTest"s});
	const std::string userConfigDir = Path::configHome() + pApp->getApplicationPath().getBaseName() + Path::separator();
	assertEqual (userConfigDir, pApp->config().getString("application.configDir"s));

	pApp->loadConfiguration(explicitConfig.toString());
	assertEqual (explicitDir.toString(), pApp->config().getString("application.configDir"s));

	assertEqual (1, pApp->loadConfiguration());
	assertEqual (explicitDir.toString(), pApp->config().getString("application.configDir"s));
}


void ApplicationTest::setUp()
{
}


void ApplicationTest::tearDown()
{
}


CppUnit::Test* ApplicationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ApplicationTest");

	CppUnit_addTest(pSuite, ApplicationTest, testConfigDirFollowsFirstLoadedFile);

	return pSuite;
}
