//
// ConfigurationEventTest.h
//
// Definition of the ConfigurationEventTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ConfigurationEventTest_INCLUDED
#define ConfigurationEventTest_INCLUDED


#include "CppUnit/TestCase.h"
#include "Poco/BasicEvent.h"
#include <string>


class ConfigurationEventTest: public CppUnit::TestCase
{
public:
	ConfigurationEventTest() = delete;
	explicit ConfigurationEventTest(const std::string& name);
	~ConfigurationEventTest() override;

	void setUp() override;
	void tearDown() override;

	void testRemoveEventsWithStringEventInstantiatedFirst();

	static CppUnit::Test* suite();

private:
	void onRemoving(const void*, const std::string& key);
	void onRemoved(const void*, const std::string& key);
	void onKeyEvent(const void*, const std::string& key);

	// Instantiates the event type of AbstractConfiguration::propertyRemoved before
	// ConfigurationEventTest.cpp includes Poco/Util/AbstractConfiguration.h.
	Poco::BasicEvent<const std::string> _keyEvent;
	std::string _removingKey;
	std::string _removedKey;
	std::string _keyEventKey;
};


#endif // ConfigurationEventTest_INCLUDED
