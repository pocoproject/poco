//
// IniFileConfigurationTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "IniFileConfigurationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include <sstream>
#include <algorithm>


using Poco::Util::IniFileConfiguration;
using Poco::Util::AbstractConfiguration;
using Poco::AutoPtr;
using Poco::NotImplementedException;
using Poco::NotFoundException;


IniFileConfigurationTest::IniFileConfigurationTest(const std::string& name): AbstractConfigurationTest(name)
{
}


IniFileConfigurationTest::~IniFileConfigurationTest()
{
}


void IniFileConfigurationTest::testLoad()
{
	static const std::string iniFile =
		"; comment\n"
		"  ; comment  \n"
		"prop1=value1\n"
		"  prop2 = value2  \n"
		"[section1]\n"
		"prop1 = value3\r\n"
		"\tprop2=value4\r\n"
		";prop3=value7\r\n"
		"\n"
		"  [ section 2 ]\n"
		"prop1 = value 5\n"
		"\t   \n"
		"Prop2 = value6";

	std::istringstream istr(iniFile);
	AutoPtr<IniFileConfiguration> pConf = new IniFileConfiguration(istr);

	assertTrue (pConf->getString("prop1") == "value1");
	assertTrue (pConf->getString("prop2") == "value2");
	assertTrue (pConf->getString("section1.prop1") == "value3");
	assertTrue (pConf->getString("Section1.Prop2") == "value4");
	assertTrue (pConf->getString("section 2.prop1") == "value 5");
	assertTrue (pConf->getString("section 2.prop2") == "value6");
	assertTrue (pConf->getString("SECTION 2.PROP2") == "value6");

	AbstractConfiguration::Keys keys;
	pConf->keys(keys);
	assertTrue (keys.size() == 4);
	assertTrue (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "section1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "section 2") != keys.end());

	pConf->keys("Section1", keys);
	assertTrue (keys.size() == 2);
	assertTrue (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop2") != keys.end());

	pConf->setString("prop1", "value11");
	assertTrue (pConf->getString("PROP1") == "value11");
	pConf->setString("Prop1", "value12");
	assertTrue (pConf->getString("prop1") == "value12");
	pConf->keys(keys);
	assertTrue (keys.size() == 4);
	assertTrue (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "section1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "section 2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "Prop1") == keys.end());

	std::istringstream istr_err(iniFile);
	istr_err.putback(std::ios_base::failbit);
	try
	{
		AutoPtr<IniFileConfiguration> pConf_err = new IniFileConfiguration(istr_err);
	}
	catch (Poco::IOException& exc)
	{
		std::string s(exc.message());
		assertTrue (s == "Broken input stream");
	}
}


void IniFileConfigurationTest::testCaseInsensitiveRemove()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();
	AbstractConfiguration::Keys keys;

	assertTrue (pConf->hasProperty("Prop1"));
	assertTrue (pConf->hasProperty("prop4.BOOL1"));
	assertTrue (pConf->hasProperty("Prop4.bool2"));
	assertTrue (pConf->hasProperty("PROP4.Bool3"));
	pConf->keys(keys);
	assertTrue (keys.size() == 13);
	pConf->keys("prop4", keys);
	assertTrue (keys.size() == 19);

	pConf->remove("PROP4.Bool1");
	assertTrue (pConf->hasProperty("Prop1"));
	assertTrue (!pConf->hasProperty("prop4.BOOL1"));
	assertTrue (pConf->hasProperty("Prop4.bool2"));
	assertTrue (pConf->hasProperty("PROP4.Bool3"));
	pConf->keys(keys);
	assertTrue (keys.size() == 13);
	pConf->keys("PROP4", keys);
	assertTrue (keys.size() == 18);

	pConf->remove("Prop4");
	assertTrue (pConf->hasProperty("Prop1"));
	assertTrue (!pConf->hasProperty("prop4.BOOL1"));
	assertTrue (!pConf->hasProperty("Prop4.bool2"));
	assertTrue (!pConf->hasProperty("PROP4.Bool3"));
	pConf->keys(keys);
	assertTrue (keys.size() == 12);
	pConf->keys("prop4", keys);
	assertTrue (keys.size() == 0);
}


AbstractConfiguration::Ptr IniFileConfigurationTest::allocConfiguration() const
{
	return new IniFileConfiguration;
}


void IniFileConfigurationTest::setUp()
{
}


void IniFileConfigurationTest::tearDown()
{
}


CppUnit::Test* IniFileConfigurationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("IniFileConfigurationTest");

	AbstractConfigurationTest_addTests(pSuite, IniFileConfigurationTest);
	CppUnit_addTest(pSuite, IniFileConfigurationTest, testLoad);
	CppUnit_addTest(pSuite, IniFileConfigurationTest, testCaseInsensitiveRemove);

	return pSuite;
}
