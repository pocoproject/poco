//
// LatLonTest.h
//
// Definition of the LatLonTest class.
//
// Copyright (c) 2011-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef LatLonTest_INCLUDED
#define LatLonTest_INCLUDED


#include "Poco/Geo/Geo.h"
#include "CppUnit/TestCase.h"


class LatLonTest: public CppUnit::TestCase
{
public:
	LatLonTest(const std::string& name);
	~LatLonTest();

	void testLatLon();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // GeoTest_INCLUDED
