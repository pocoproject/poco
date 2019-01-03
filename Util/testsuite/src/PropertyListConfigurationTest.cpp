//
// PropertyListConfigurationTest.cpp
//
// Copyright (c) 2017-2018, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PropertyListConfigurationTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Util/PropertyListConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include <sstream>


using Poco::Util::PropertyListConfiguration;
using Poco::Util::AbstractConfiguration;
using Poco::AutoPtr;
using Poco::NotImplementedException;
using Poco::NotFoundException;


PropertyListConfigurationTest::PropertyListConfigurationTest(const std::string& name): AbstractConfigurationTest(name)
{
}


PropertyListConfigurationTest::~PropertyListConfigurationTest()
{
}


void PropertyListConfigurationTest::testLoad()
{
	static const std::string pfileFile =
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
		"<plist version=\"1.0\">\n"
		"<dict>\n"
		"  <key>prop1</key>\n"
		"  <string>value1</string>\r\n"
		"  <key>prop2</key>\r\n"
		"  <integer>8080</integer>\r\n"
		"  <key>tree1</key>\n"
		"  <dict>\n"
		"	 <key>date1</key>\r\n"
		"	 <date>2017-06-21T09:42:33Z</date>\r\n"
		"	 <key>prop3</key>\r\n"
		"	 <false/>\r\n"
		"	 <key>prop4</key>\r\n"
		"	 <data>2g==</data>\r\n"
		"  </dict>\r\n"
		"</dict>\r\n"
		"</plist>\r\n";

	std::istringstream istr(pfileFile);
	AutoPtr<PropertyListConfiguration> pConf = new PropertyListConfiguration(istr);

	assertTrue (pConf->getString("prop1") == "value1");
	assertTrue (pConf->getString("prop2") == "8080");
	assertTrue (pConf->getString("tree1.date1") == "2017-06-21T09:42:33Z");
	assertTrue (pConf->getString("tree1.prop3") == "false");
	std::stringstream buf;
	pConf->getData("tree1.prop4", buf);
	assertTrue (buf.str() == "\xda");

	AbstractConfiguration::Keys keys;
	pConf->keys(keys);
	assertTrue (keys.size() == 3);
	assertTrue (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "tree1") != keys.end());

	pConf->keys("tree1", keys);
	assertTrue (keys.size() == 3);
	assertTrue (std::find(keys.begin(), keys.end(), "date1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop3") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop4") != keys.end());

	try
	{
		std::string s = pConf->getString("foo");
		fail("No property - must throw");
	}
	catch (NotFoundException&)
	{
	}
}

void PropertyListConfigurationTest::testSave()
{
	AutoPtr<PropertyListConfiguration> pConf = new PropertyListConfiguration;

	pConf->setString("prop1", "value1");
	pConf->setInt("prop2", 42);
	pConf->setString("prop3", "value\\1\txxx<");
	pConf->setBool("prop4", true);
	std::stringstream istr;
	istr.write("#0", 2);
	pConf->setData("prop5", istr);
	pConf->setDouble("prop6", 3.14);
	pConf->setString("prop7.prop8", "nestedvalue");

	std::ostringstream ostr;
	pConf->save(ostr);
	std::string propFile = ostr.str();

	assertTrue (propFile == "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
		"<plist version=\"1.0\">\n"
		"\t<dict>\n"
		"\t\t<key>prop1</key>\n"
		"\t\t<string>value1</string>\n"
		"\t\t<key>prop2</key>\n"
		"\t\t<integer>42</integer>\n"
		"\t\t<key>prop3</key>\n"
		"\t\t<string>value\\1\txxx&lt;</string>\n"
		"\t\t<key>prop4</key>\n"
		"\t\t<true/>\n"
		"\t\t<key>prop5</key>\n"
		"\t\t<data>IzA=</data>\n"
		"\t\t<key>prop6</key>\n"
		"\t\t<real>3.14</real>\n"
		"\t\t<key>prop7</key>\n"
		"\t\t<dict>\n"
		"\t\t\t<key>prop8</key>\n"
		"\t\t\t<string>nestedvalue</string>\n"
		"\t\t</dict>\n"
		"\t</dict>\n"
		"</plist>\n");
}

AbstractConfiguration::Ptr PropertyListConfigurationTest::allocConfiguration() const
{
	return new PropertyListConfiguration;
}


void PropertyListConfigurationTest::setUp()
{
}


void PropertyListConfigurationTest::tearDown()
{
}


CppUnit::Test* PropertyListConfigurationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PropertyListConfigurationTest");

	AbstractConfigurationTest_addTests(pSuite, PropertyListConfigurationTest);
	CppUnit_addTest(pSuite, PropertyListConfigurationTest, testLoad);
	//CppUnit_addTest(pSuite, PropertyListConfigurationTest, testSave);

	return pSuite;
}
