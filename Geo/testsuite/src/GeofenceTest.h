//
// GeofenceTest.h
//
// Definition of the GeofenceTest class.
//
// Copyright (c) 2025, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef GeofenceTest_INCLUDED
#define GeofenceTest_INCLUDED


#include "Poco/Geo/Geo.h"
#include "CppUnit/TestCase.h"


class GeofenceTest: public CppUnit::TestCase
{
public:
	GeofenceTest(const std::string& name);
	~GeofenceTest();

	void testCircular();
	void testPolygonal();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // GeoTest_INCLUDED
