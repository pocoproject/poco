//
// LayeredConfigurationTest.h
//
// Definition of the LayeredConfigurationTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef LayeredConfigurationTest_INCLUDED
#define LayeredConfigurationTest_INCLUDED


#include "AbstractConfigurationTest.h"
#include "Poco/Util/Util.h"


class LayeredConfigurationTest: public AbstractConfigurationTest
{
public:
	LayeredConfigurationTest(const std::string& name);
	virtual ~LayeredConfigurationTest();

	void testEmpty();
	void testOneLayer();
	void testTwoLayers();
	void testThreeLayers();
	void testRemove();
	void testFind();
	void testReplace();
		/// Checks that replace() swaps the first configuration with the label,
		/// applies the given priority and writeable flag, adds a configuration
		/// whose label is missing, and rejects an empty label and a null configuration.
	void testReplaceConcurrent();
		/// Checks that readers never miss a property of the labeled configuration
		/// while another thread replaces that configuration.

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	virtual Poco::Util::AbstractConfiguration::Ptr allocConfiguration() const;
};


#endif // LayeredConfigurationTest_INCLUDED
