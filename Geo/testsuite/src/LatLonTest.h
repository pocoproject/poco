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
	void testToCartesian();
	void testFromCartesian();
	void testCartesianRoundTrip();
	void testAssignAndSwap();
	void testComparison();
	void testFromRadians();
	void testAntipodalGreatCircleDistance();
	void testGreatCircleEndPositionAtPole();
	void testDistanceToNaNPosition();
	void testRhumbDueEastOnEquator();
	void testRhumbBetweenNearlyEqualLatitudes();
	void testRhumbWestAcrossAntimeridian();
	void testToCartesianAcrossAntimeridian();
	void testToCartesianOutOfDomain();
	void testToCartesianNonFiniteCoordinate();
	void testDegenerateAzimuth();
	void testOutOfDomainDistance();
	void testUnnormalizedLongitude();
	void testEndPositionUndefinedInput();
	void testToCartesianInvalidReference();
	void testFromCartesianNearPole();
	void testEqualsAcrossAntimeridian();
	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // LatLonTest_INCLUDED
