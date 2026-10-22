//
// ConfigurationEventTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ConfigurationEventTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
// ConfigurationEventTest.h instantiates BasicEvent<const std::string> first. If AbstractConfiguration.h declared an
// export attribute on that type, GCC would ignore it with only a warning; the pragma turns that warning into an error,
// so this test fails to build.
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wattributes"
#endif
#include "Poco/Util/MapConfiguration.h"
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#include "Poco/AutoPtr.h"
#include "Poco/Delegate.h"


using Poco::Util::MapConfiguration;
using Poco::AutoPtr;
using Poco::delegate;


ConfigurationEventTest::ConfigurationEventTest(const std::string& name): CppUnit::TestCase(name)
{
}


ConfigurationEventTest::~ConfigurationEventTest() = default;


void ConfigurationEventTest::testRemoveEventsWithStringEventInstantiatedFirst()
{
	AutoPtr<MapConfiguration> pConf = new MapConfiguration;
	pConf->setString("foo", "bar");

	pConf->propertyRemoving += delegate(this, &ConfigurationEventTest::onRemoving);
	pConf->propertyRemoved += delegate(this, &ConfigurationEventTest::onRemoved);
	pConf->remove("foo");
	pConf->propertyRemoving -= delegate(this, &ConfigurationEventTest::onRemoving);
	pConf->propertyRemoved -= delegate(this, &ConfigurationEventTest::onRemoved);

	assertEqual ("foo", _removingKey);
	assertEqual ("foo", _removedKey);

	_keyEvent += delegate(this, &ConfigurationEventTest::onKeyEvent);
	_keyEvent.notify(this, std::string("bar"));
	_keyEvent -= delegate(this, &ConfigurationEventTest::onKeyEvent);

	assertEqual ("bar", _keyEventKey);
}


void ConfigurationEventTest::onRemoving(const void*, const std::string& key)
{
	_removingKey = key;
}


void ConfigurationEventTest::onRemoved(const void*, const std::string& key)
{
	_removedKey = key;
}


void ConfigurationEventTest::onKeyEvent(const void*, const std::string& key)
{
	_keyEventKey = key;
}


void ConfigurationEventTest::setUp()
{
	_removingKey.clear();
	_removedKey.clear();
	_keyEventKey.clear();
}


void ConfigurationEventTest::tearDown()
{
}


CppUnit::Test* ConfigurationEventTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ConfigurationEventTest");

	CppUnit_addTest(pSuite, ConfigurationEventTest, testRemoveEventsWithStringEventInstantiatedFirst);

	return pSuite;
}
