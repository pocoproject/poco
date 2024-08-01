//
// AnyTest.h
//
// Tests for Any types
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef AnyTest_INCLUDED
#define AnyTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class AnyTest: public CppUnit::TestCase
{
public:
	class Dummy{};

	AnyTest(const std::string& name);
	~AnyTest();

	void testAnyConvertingCtor();
	void testAnyDefaultCtor();
	void testAnyCopyCtor();
	void testAnyCopyAssign();
	void testAnyConvertingAssign();
	void testAnyBadCast();
	void testAnySwap();
	void testAnyEmptyCopy();
	void testAnyCastToReference();

	void testAnyPointer();
	void testAnyInt();
	void testAnyComplexType();
	void testAnyVector();

	void testPlaceholder();

	void setUp();
	void tearDown();
	static CppUnit::Test* suite();

private:
	int _dummy = 0;
	Dummy _dummyObject;
};


#endif // AnyTest_INCLUDED
