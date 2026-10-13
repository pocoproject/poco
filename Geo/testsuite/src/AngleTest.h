//
// AngleTest.h
//
// Definition of the AngleTest class.
//
// Copyright (c) 2011-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AngleTest_INCLUDED
#define AngleTest_INCLUDED


#include "Poco/Geo/Geo.h"
#include "CppUnit/TestCase.h"


class AngleTest: public CppUnit::TestCase
{
public:
	AngleTest(const std::string& name);
	~AngleTest();

	void testAngle();
	void testCompass();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // GeoTest_INCLUDED
