//
// PropertyListConfigurationTest.h
//
// Definition of the PropertyListConfigurationTest class.
//
// Copyright (c) 2017-2018, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PropertyListConfigurationTest_INCLUDED
#define PropertyListConfigurationTest_INCLUDED


#include "AbstractConfigurationTest.h"
#include "Poco/Util/Util.h"


class PropertyListConfigurationTest: public AbstractConfigurationTest
{
public:
	PropertyListConfigurationTest(const std::string& name);
	virtual ~PropertyListConfigurationTest();

	void testLoad();
	void testSave();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	virtual Poco::Util::AbstractConfiguration::Ptr allocConfiguration() const;
};


#endif // PropertyListConfigurationTest_INCLUDED
