//
// WinConfigurationTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "WinConfigurationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/WinRegistryConfiguration.h"
#include "Poco/Util/WinRegistryKey.h"
#include "Poco/Environment.h"
#include "Poco/AutoPtr.h"
#include "Poco/Types.h"
#undef min
#undef max
#include <limits>


using Poco::Util::WinRegistryConfiguration;
using Poco::Util::WinRegistryKey;
using Poco::Environment;
using Poco::AutoPtr;
using Poco::Int64;
using Poco::UInt64;


WinConfigurationTest::WinConfigurationTest(const std::string& name): CppUnit::TestCase(name)
{
}


WinConfigurationTest::~WinConfigurationTest()
{
}


void WinConfigurationTest::testConfiguration()
{
	WinRegistryKey regKey("HKEY_CURRENT_USER\\Software\\Applied Informatics\\Test");
	if (regKey.exists()) regKey.deleteKey();
	assertTrue (!regKey.exists());

	AutoPtr<WinRegistryConfiguration> pReg = new WinRegistryConfiguration("HKEY_CURRENT_USER\\Software\\Applied Informatics\\Test");
	pReg->setString("name1", "value1");
	assertTrue (pReg->getString("name1") == "value1");
	pReg->setInt("name1", 1); // overwrite should also change type
	assertTrue (pReg->getInt("name1") == 1);
	pReg->setString("name2", "value2");
	assertTrue (pReg->getString("name2") == "value2");
#if defined(POCO_HAVE_INT64)
	pReg->setUInt64("name2", std::numeric_limits<UInt64>::max()); // overwrite should also change type
	assertTrue (pReg->getUInt64("name2") == std::numeric_limits<UInt64>::max());
	pReg->setInt64("name2", std::numeric_limits<Int64>::min());
	assertTrue (pReg->getInt64("name2") == std::numeric_limits<Int64>::min());

	/// write real int64 value type
	regKey.setInt64("name3", std::numeric_limits<Int64>::max());
	assertTrue (pReg->getInt64("name3") == std::numeric_limits<Int64>::max());
#endif

	/// create fake binary data
	const int dataSize = 127;
	std::vector<char> data(dataSize);
	for (int i = 0; i < dataSize; ++i)
		data[i] = rand() % 256;

	regKey.setBinary("name4", data);
	assertTrue (pReg->getString("name4") == std::string(data.begin(), data.end()));


	assertTrue (pReg->hasProperty("name1"));
	assertTrue (pReg->hasProperty("name2"));

	std::string dfl = pReg->getString("nonexistent", "default");
	assertTrue (dfl == "default");

	AutoPtr<Poco::Util::AbstractConfiguration> pView = pReg->createView("config");
	dfl = pView->getString("sub.foo", "default");
	assertTrue (dfl == "default");

	pView->setString("sub.foo", "bar");
	assertTrue (pView->getString("sub.foo", "default") == "bar");

	std::string value;
	assertTrue (pReg->convertToRegFormat("A.B.C", value) == "A\\B");
	assertTrue (value == "C");

	Poco::Util::AbstractConfiguration::Keys keys;
	pReg->keys(keys);
	assertTrue (keys.size() == 5);
	assertTrue (std::find(keys.begin(), keys.end(), "name1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "name2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "name3") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "name4") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "config") != keys.end());

	pReg->keys("config", keys);
	assertTrue (keys.size() == 1);
	assertTrue (std::find(keys.begin(), keys.end(), "sub") != keys.end());

	AutoPtr<WinRegistryConfiguration> pRootReg = new WinRegistryConfiguration("");

	assertTrue (pRootReg->getInt("HKEY_CURRENT_USER.Software.Applied Informatics.Test.name1") == 1);

	pRootReg->keys(keys);

	assertTrue (keys.size() == 6);
	assertTrue (std::find(keys.begin(), keys.end(), "HKEY_CLASSES_ROOT") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "HKEY_CURRENT_CONFIG") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "HKEY_CURRENT_USER") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "HKEY_LOCAL_MACHINE") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "HKEY_PERFORMANCE_DATA") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "HKEY_USERS") != keys.end());

	pRootReg->keys("HKEY_CURRENT_USER.Software.Applied Informatics.Test", keys);
	assertTrue (keys.size() == 5);
	assertTrue (std::find(keys.begin(), keys.end(), "name1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "name2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "name3") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "name4") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "config") != keys.end());
}


void WinConfigurationTest::setUp()
{
}


void WinConfigurationTest::tearDown()
{
}


CppUnit::Test* WinConfigurationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("WinConfigurationTest");

	CppUnit_addTest(pSuite, WinConfigurationTest, testConfiguration);

	return pSuite;
}
