//
// PropertyFileConfigurationTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PropertyFileConfigurationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include <sstream>
#include <algorithm>


using Poco::Util::PropertyFileConfiguration;
using Poco::Util::AbstractConfiguration;
using Poco::AutoPtr;
using Poco::NotFoundException;


PropertyFileConfigurationTest::PropertyFileConfigurationTest(const std::string& name): AbstractConfigurationTest(name)
{
}


PropertyFileConfigurationTest::~PropertyFileConfigurationTest()
{
}


void PropertyFileConfigurationTest::testLoad()
{
	static const std::string propFile =
		"! comment\n"
		"! comment\n"
		"prop1=value1\n"
		"prop2 = value2  \n"
		"prop3.prop31: value3\n"
		"# comment\n"
		"  prop3.prop32: value 4\n"
		"prop3.prop33: value5, value6, \\\n"
		"value7, value8, \\\r\n"
		"value9\n"
		"prop4 = escaped[\\t\\r\\n\\f]\n"
		"#prop4 = foo\n"
		"prop5:foo";

	std::istringstream istr(propFile);
	AutoPtr<PropertyFileConfiguration> pConf = new PropertyFileConfiguration(istr);

	assertTrue (pConf->getString("prop1") == "value1");
	assertTrue (pConf->getString("prop2") == "value2");
	assertTrue (pConf->getString("prop3.prop31") == "value3");
	assertTrue (pConf->getString("prop3.prop32") == "value 4");
	assertTrue (pConf->getString("prop3.prop33") == "value5, value6, value7, value8, value9");
	assertTrue (pConf->getString("prop4") == "escaped[\t\r\n\f]");
	assertTrue (pConf->getString("prop5") == "foo");

	AbstractConfiguration::Keys keys;
	pConf->keys(keys);
	assertTrue (keys.size() == 5);
	assertTrue (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop3") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop4") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop5") != keys.end());

	pConf->keys("prop3", keys);
	assertTrue (keys.size() == 3);
	assertTrue (std::find(keys.begin(), keys.end(), "prop31") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop32") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop33") != keys.end());

	try
	{
		std::string s = pConf->getString("foo");
		fail("No property - must throw");
	}
	catch (NotFoundException&)
	{
	}

	std::istringstream istr_err(propFile);
	istr_err.putback(std::ios_base::failbit);
	try
	{
		AutoPtr<PropertyFileConfiguration> pConf_err = new PropertyFileConfiguration(istr_err);
	}
	catch (Poco::IOException& exc)
	{
		std::string s(exc.message());
		assertTrue (s == "Broken input stream");
	}
}


void PropertyFileConfigurationTest::testSave()
{
	AutoPtr<PropertyFileConfiguration> pConf = new PropertyFileConfiguration;

	pConf->setString("prop1", "value1");
	pConf->setInt("prop2", 42);
	pConf->setString("prop3", "value\\1\txxx");

	std::ostringstream ostr;
	pConf->save(ostr);
	std::string propFile = ostr.str();
	assertTrue (propFile == "prop1: value1\n"
	                    "prop2: 42\n"
	                    "prop3: value\\\\1\\txxx\n");
}


AbstractConfiguration::Ptr PropertyFileConfigurationTest::allocConfiguration() const
{
	return new PropertyFileConfiguration;
}


void PropertyFileConfigurationTest::setUp()
{
}


void PropertyFileConfigurationTest::tearDown()
{
}


CppUnit::Test* PropertyFileConfigurationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PropertyFileConfigurationTest");

	AbstractConfigurationTest_addTests(pSuite, PropertyFileConfigurationTest);
	CppUnit_addTest(pSuite, PropertyFileConfigurationTest, testLoad);
	CppUnit_addTest(pSuite, PropertyFileConfigurationTest, testSave);

	return pSuite;
}
