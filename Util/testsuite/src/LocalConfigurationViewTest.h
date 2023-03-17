//
// LocalConfigurationViewTest.h
//
// Definition of the ConfigurationViewTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef LocalConfigurationViewTest_INCLUDED
#define LocalConfigurationViewTest_INCLUDED

#include "AbstractConfigurationTest.h"
#include "Poco/Util/Util.h"

class LocalConfigurationViewTest : public AbstractConfigurationTest {
   public:
    LocalConfigurationViewTest(const std::string& name);
    virtual ~LocalConfigurationViewTest();

    void testLocalView();

    void setUp();
    void tearDown();

    static CppUnit::Test* suite();

   private:
    virtual Poco::Util::AbstractConfiguration::Ptr allocConfiguration() const;
};

#endif  // LocalConfigurationViewTest_INCLUDED
