//
// ULIDGeneratorTest.h
//
// Definition of the ULIDGeneratorTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ULIDGeneratorTest_INCLUDED
#define ULIDGeneratorTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class ULIDGeneratorTest: public CppUnit::TestCase
{
public:
	ULIDGeneratorTest(const std::string& name);
	~ULIDGeneratorTest();

	void testCreate();
	void testCreateTime();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // ULIDGeneratorTest_INCLUDED
