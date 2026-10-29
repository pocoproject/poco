//
// JSONConfigurationTest.cpp
//
// Copyright (c) 2004-2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "JSONConfigurationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/JSONConfiguration.h"
#include "Poco/Util/ConfigurationMapper.h"
#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/Delegate.h"
#include <string>


using Poco::Util::JSONConfiguration;
using Poco::Util::AbstractConfiguration;
using Poco::Util::ConfigurationMapper;
using Poco::Util::LayeredConfiguration;
using Poco::AutoPtr;
using Poco::NotImplementedException;
using Poco::NotFoundException;
using Poco::JSON::JSONException;


JSONConfigurationTest::JSONConfigurationTest(const std::string& name) : AbstractConfigurationTest(name)
{
}


JSONConfigurationTest::~JSONConfigurationTest()
{
}


void JSONConfigurationTest::testLoad()
{
	JSONConfiguration config;

	std::string json = "{ \"config\" : "
							" { \"prop1\" : \"value1\", "
							" \"prop2\" : 10, "
							" \"prop3\" : [ \"element1\", \"element2\" ], "
							" \"prop4\" : { \"prop5\" : false, "
											" \"prop6\" : null } "
							" }"
						"}";

	std::istringstream iss(json);
	try
	{
		config.load(iss);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	std::string property1 = config.getString("config.prop1");
	assertTrue (property1.compare("value1") == 0);

	int property2 = config.getInt("config.prop2");
	assertTrue (property2 == 10);

	int nonExistingProperty = config.getInt("config.prop7", 5);
	assertTrue (nonExistingProperty == 5);

	std::string arrProperty = config.getString("config.prop3[1]");
	assertTrue (arrProperty.compare("element2") == 0);

	bool property35 = config.getBool("config.prop4.prop5");
	assertTrue (! property35);

	try
	{
		(void) config.getString("propertyUnknown");
		assertTrue (true);
	}
	catch(NotFoundException& nfe)
	{
	}
}


void JSONConfigurationTest::testSetArrayElement()
{
	JSONConfiguration config;

	std::string json = "{ \"config\" : "
							" { \"prop1\" : \"value1\", "
							" \"prop2\" : 10, "
							" \"prop3\" : [ \"element1\", \"element2\" ], "
							" \"prop4\" : { \"prop5\" : false, "
											" \"prop6\" : null } "
							" }"
						"}";

	std::istringstream iss(json);
	config.load(iss);

	// config.prop3[0] = "foo"
	config.setString("config.prop3[0]", "foo");
	assertTrue (config.getString("config.prop3[0]") == "foo");

	// config.prop3[1] = "bar"
	config.setString("config.prop3[1]", "bar");
	assertTrue (config.getString("config.prop3[1]") == "bar");

	// config.prop3[3] = "baz"
	config.setString("config.prop3[3]", "baz");
	assertTrue (config.getString("config.prop3[3]") == "baz");
}


void JSONConfigurationTest::testConfigurationView()
{
	std::string json = R"json({ "foo" : [ "bar" ] })json";
	Poco::Util::JSONConfiguration config;
	std::istringstream stream(json);
	config.load(stream);

	Poco::Util::AbstractConfiguration::Ptr pView = config.createView("foo");

	assertTrue (pView->getString("[0]") == "bar");

	try
	{
		(void) pView->getString("[1]");
		fail ("must throw on index out of bounds");
	}
	catch(Poco::NotFoundException&){}
}


void JSONConfigurationTest::testWrapperEvents()
{
	AbstractConfigurationTest* pTest = this;
	_changingCount = 0;
	_changedCount = 0;
	AutoPtr<JSONConfiguration> pJSON = new JSONConfiguration;
	pJSON->propertyChanging += Poco::delegate(pTest, &AbstractConfigurationTest::onPropertyChanging);
	pJSON->propertyChanged += Poco::delegate(pTest, &AbstractConfigurationTest::onPropertyChanged);

	AbstractConfiguration::Ptr pView = pJSON->createView("view");
	pView->propertyChanging += Poco::delegate(pTest, &AbstractConfigurationTest::onPropertyChanging);
	pView->propertyChanged += Poco::delegate(pTest, &AbstractConfigurationTest::onPropertyChanged);
	pView->setInt("int", 1);
	assertEqual (1, _changingCount);
	assertEqual (1, _changedCount);
	assertTrue (_changedKey == "int");
	assertEqual (1, pJSON->getInt("view.int"));

	_changingCount = 0;
	_changedCount = 0;
	AutoPtr<ConfigurationMapper> pMapper = new ConfigurationMapper("json", "mapped", pJSON);
	pMapper->propertyChanging += Poco::delegate(pTest, &AbstractConfigurationTest::onPropertyChanging);
	pMapper->propertyChanged += Poco::delegate(pTest, &AbstractConfigurationTest::onPropertyChanged);
	pMapper->setInt("mapped.int", 2);
	assertEqual (1, _changingCount);
	assertEqual (1, _changedCount);
	assertTrue (_changedKey == "mapped.int");
	assertEqual (2, pJSON->getInt("json.int"));

	_changingCount = 0;
	_changedCount = 0;
	AutoPtr<LayeredConfiguration> pLayered = new LayeredConfiguration;
	pLayered->addWriteable(pJSON, 0);
	pLayered->propertyChanging += Poco::delegate(pTest, &AbstractConfigurationTest::onPropertyChanging);
	pLayered->propertyChanged += Poco::delegate(pTest, &AbstractConfigurationTest::onPropertyChanged);
	pLayered->setInt("layered.int", 3);
	assertEqual (1, _changingCount);
	assertEqual (1, _changedCount);
	assertEqual (3, pJSON->getInt("layered.int"));
}


AbstractConfiguration::Ptr JSONConfigurationTest::allocConfiguration() const
{
	return new JSONConfiguration;
}


void JSONConfigurationTest::setUp()
{
}


void JSONConfigurationTest::tearDown()
{
}


CppUnit::Test* JSONConfigurationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JSONConfigurationTest");

	AbstractConfigurationTest_addTests(pSuite, JSONConfigurationTest);
	CppUnit_addTest(pSuite, JSONConfigurationTest, testLoad);
	CppUnit_addTest(pSuite, JSONConfigurationTest, testSetArrayElement);
	CppUnit_addTest(pSuite, JSONConfigurationTest, testConfigurationView);
	CppUnit_addTest(pSuite, JSONConfigurationTest, testWrapperEvents);

	return pSuite;
}
