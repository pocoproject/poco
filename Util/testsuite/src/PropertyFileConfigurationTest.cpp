//
// PropertyFileConfigurationTest.cpp
//
// $Id: //poco/1.4/Util/testsuite/src/PropertyFileConfigurationTest.cpp#1 $
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
	testLoad(false);
}


void PropertyFileConfigurationTest::testLoadEmpty()
{
	static const std::string propFile = " ";
		
	std::istringstream istr(propFile);
	AutoPtr<PropertyFileConfiguration> pConf = new PropertyFileConfiguration(istr);
	
	AbstractConfiguration::Keys keys;
	pConf->keys(keys);
	assert (keys.size() == 0);
}


void PropertyFileConfigurationTest::testLoadWithPreserveComment()
{
	testLoad(true);
}


void PropertyFileConfigurationTest::testLoad(bool preserveComment)
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
	AutoPtr<PropertyFileConfiguration> pConf = new PropertyFileConfiguration(istr, preserveComment);
	
	assert (pConf->getString("prop1") == "value1");
	assert (pConf->getString("prop2") == "value2");
	assert (pConf->getString("prop3.prop31") == "value3");
	assert (pConf->getString("prop3.prop32") == "value 4");
	assert (pConf->getString("prop3.prop33") == "value5, value6, value7, value8, value9");
	assert (pConf->getString("prop4") == "escaped[\t\r\n\f]");
	assert (pConf->getString("prop5") == "foo");
	
	AbstractConfiguration::Keys keys;
	pConf->keys(keys);
	assert (keys.size() == 5);
	assert (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "prop2") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "prop3") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "prop4") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "prop5") != keys.end());
	
	pConf->keys("prop3", keys);
	assert (keys.size() == 3);
	assert (std::find(keys.begin(), keys.end(), "prop31") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "prop32") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "prop33") != keys.end());
	
	try
	{
		std::string s = pConf->getString("foo");
		fail("No property - must throw");
	}
	catch (NotFoundException&)
	{
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
	assert (propFile == "prop1: value1\n"
	                    "prop2: 42\n"
	                    "prop3: value\\\\1\\txxx\n");
}

void PropertyFileConfigurationTest::testLoadSaveWithPreserveComment()
{
	std::string propFile = 
		"! comment #\n"
		"prop1=value1\n"
		"# comment #\n"
		"# comment !\n"
		"prop2 = value2  \n"
		"! comment !\n"
		"prop3:foo\n"
		"prop4";
		
	std::istringstream istr(propFile);
	AutoPtr<PropertyFileConfiguration> pConf = new PropertyFileConfiguration(istr, true);

	std::ostringstream ostr;
	pConf->save(ostr);
	assertEqual ("! comment #\n"
	             "prop1: value1\n"
	             "# comment #\n"
	             "# comment !\n"
					 "prop2: value2\n"
					 "! comment !\n"
					 "prop3: foo\n"
					 "prop4: \n",
					 ostr.str());

	pConf->setString("prop4", "value4");
	ostr.clear();
	ostr.str("");
	pConf->save(ostr);
	assertEqual ("! comment #\n"
	             "prop1: value1\n"
	             "# comment #\n"
	             "# comment !\n"
					 "prop2: value2\n"
					 "! comment !\n"
					 "prop3: foo\n"
					 "prop4: value4\n",
					 ostr.str());

	pConf->remove("prop2");
	ostr.clear();
	ostr.str("");
	pConf->save(ostr);
	assertEqual ("! comment #\n"
	             "prop1: value1\n"
	             "# comment #\n"
	             "# comment !\n"
					 "! comment !\n"
					 "prop3: foo\n"
					 "prop4: value4\n",
					 ostr.str());

	pConf->setString("prop4", "value5");
	ostr.clear();
	ostr.str("");
	pConf->save(ostr);
	assertEqual ("! comment #\n"
	             "prop1: value1\n"
	             "# comment #\n"
	             "# comment !\n"
					 "! comment !\n"
					 "prop3: foo\n"
					 "prop4: value5\n",
					 ostr.str());
}


AbstractConfiguration* PropertyFileConfigurationTest::allocConfiguration() const
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
	CppUnit_addTest(pSuite, PropertyFileConfigurationTest, testLoadEmpty);
	CppUnit_addTest(pSuite, PropertyFileConfigurationTest, testSave);
	CppUnit_addTest(pSuite, PropertyFileConfigurationTest, testLoadWithPreserveComment);
	CppUnit_addTest(pSuite, PropertyFileConfigurationTest, testLoadSaveWithPreserveComment);
	
	return pSuite;
}
