//
// PropertyFileConfigurationTest.h
//
// Definition of the PropertyFileConfigurationTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PropertyFileConfigurationTest_INCLUDED
#define PropertyFileConfigurationTest_INCLUDED


#include "AbstractConfigurationTest.h"
#include "Poco/Util/Util.h"


class PropertyFileConfigurationTest: public AbstractConfigurationTest
{
public:
	PropertyFileConfigurationTest(const std::string& name);
	virtual ~PropertyFileConfigurationTest();

	void testLoad();
   void testLoadEmpty();
   void testLoadWithPreserveComment();
	void testSave();
   void testLoadSaveWithPreserveComment();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	virtual Poco::Util::AbstractConfiguration::Ptr allocConfiguration() const;

   void testLoad(bool preserveComment);
};


#endif // PropertyFileConfigurationTest_INCLUDED
