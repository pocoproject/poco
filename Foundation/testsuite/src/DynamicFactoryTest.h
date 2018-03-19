//
// DynamicFactoryTest.h
//
// Definition of the DynamicFactoryTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DynamicFactoryTest_INCLUDED
#define DynamicFactoryTest_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/CppUnit/TestCase.h"


class DynamicFactoryTest: public CppUnit::TestCase
{
public:
	DynamicFactoryTest(const std::string& name);
	~DynamicFactoryTest();

	void testDynamicFactoryPtr();
	void testDynamicFactoryAutoPtr();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // DynamicFactoryTest_INCLUDED
