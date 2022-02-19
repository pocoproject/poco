//
// LocalConfigurationViewTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "LocalConfigurationViewTest.h"

#include <algorithm>

#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/Util/MapConfiguration.h"

using Poco::AutoPtr;
using Poco::Util::AbstractConfiguration;
using Poco::Util::MapConfiguration;

LocalConfigurationViewTest::LocalConfigurationViewTest(const std::string& name) : AbstractConfigurationTest(name) {
}

LocalConfigurationViewTest::~LocalConfigurationViewTest() {
}

void LocalConfigurationViewTest::testLocalView() {
    AbstractConfiguration::Ptr pConf = createConfiguration();
    AbstractConfiguration::Ptr pView = pConf->createLocalView("");
    assertTrue(pView->hasProperty("prop1"));
    assertTrue(pView->hasProperty("prop2"));

    AbstractConfiguration::Keys keys;
    pView->keys(keys);
    assertTrue(keys.size() == 13);
    assertTrue(std::find(keys.begin(), keys.end(), "prop1") != keys.end());
    assertTrue(std::find(keys.begin(), keys.end(), "prop2") != keys.end());
    assertTrue(std::find(keys.begin(), keys.end(), "prop3") != keys.end());
    assertTrue(std::find(keys.begin(), keys.end(), "prop4") != keys.end());

    assertTrue(pView->getString("prop1") == "foo");
    assertTrue(pView->getString("prop3.string1") == "foo");

    pView->setString("prop6", "foobar");
    assertTrue(pConf->getString("prop6") == "foobar");

    pView = pConf->createLocalView("prop1");
    pView->keys(keys);
    assertTrue(keys.empty());
    assertFalse(pView->hasProperty("prop1"));

    pView->setString("prop11", "foobar");
    assertTrue(pConf->getString("prop1.prop11") == "foobar");

    pView = pConf->createLocalView("prop3");
    pView->keys(keys);
    assertTrue(keys.size() == 2);
    assertTrue(std::find(keys.begin(), keys.end(), "string1") != keys.end());
    assertTrue(std::find(keys.begin(), keys.end(), "string2") != keys.end());

    assertTrue(pView->getString("string1") == "foo");
    assertTrue(pView->getString("string2") == "bar");

    pView->setString("string3", "foobar");
    assertTrue(pConf->getString("prop3.string3") == "foobar");

    pView = pConf->createLocalView("prop5");
    pView->keys(keys);
    assertTrue(keys.size() == 4);
    assertTrue(std::find(keys.begin(), keys.end(), "string1") != keys.end());
    assertTrue(std::find(keys.begin(), keys.end(), "string1") != keys.end());
    assertTrue(std::find(keys.begin(), keys.end(), "sub1") != keys.end());
    assertTrue(std::find(keys.begin(), keys.end(), "sub2") != keys.end());

    assertTrue(pView->getString("sub1.string1") == "FOO");
    assertTrue(pView->getString("sub2.string2") == "Bar");

    pView = pConf->createLocalView("prop5.sub1");
    pView->keys(keys);
    assertTrue(keys.size() == 2);
    assertTrue(std::find(keys.begin(), keys.end(), "string1") != keys.end());
    assertTrue(std::find(keys.begin(), keys.end(), "string2") != keys.end());

    assertTrue(pView->getString("string1") == "FOO");
    assertTrue(pView->getString("string2") == "BAR");

    pView->setString("string3", "foobar");
    assertTrue(pConf->getString("prop5.sub1.string3") == "foobar");

    pView->remove("string3");
    assertTrue(!pConf->hasProperty("prop5.sub1.string3"));
}

AbstractConfiguration::Ptr LocalConfigurationViewTest::allocConfiguration() const {
    return new MapConfiguration;
}

void LocalConfigurationViewTest::setUp() {
}

void LocalConfigurationViewTest::tearDown() {
}

CppUnit::Test* LocalConfigurationViewTest::suite() {
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LocalConfigurationViewTest");

    AbstractConfigurationTest_addTests(pSuite, LocalConfigurationViewTest);
    CppUnit_addTest(pSuite, LocalConfigurationViewTest, testLocalView);

    return pSuite;
}
