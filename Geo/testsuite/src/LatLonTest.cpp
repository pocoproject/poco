//
// LatLonTest.cpp
//
// Copyright (c) 2011-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "LatLonTest.h"
#include "Poco/Geo/LatLon.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"


using namespace Poco::Geo;


LatLonTest::LatLonTest(const std::string& name): CppUnit::TestCase(name)
{
}


LatLonTest::~LatLonTest()
{
}


void LatLonTest::testLatLon()
{
	LatLon goldenGateSouthTower(LatLon::fromDegrees(37.813964, -122.478192));
	LatLon goldenGateNorthTower(LatLon::fromDegrees(37.825585, -122.479426));
	
	Angle a = goldenGateNorthTower.greatCircleDistanceTo(goldenGateSouthTower);
	double m = a.radians()*LatLon::EARTH_MEAN_RADIUS;
	
	assertEquals (1296.7, m, 0.1);
	
	a = goldenGateNorthTower.greatCircleAzimuthTo(goldenGateSouthTower);
	double d = a.degrees();
	
	assertEquals (175.2, d, 0.1);

	a = goldenGateNorthTower.rhumbDistanceTo(goldenGateSouthTower);
	m = a.radians()*LatLon::EARTH_MEAN_RADIUS;
	
	assertEquals (1296.7, m, 0.1);
	
	a = goldenGateNorthTower.rhumbAzimuthTo(goldenGateSouthTower);
	d = a.degrees();
	
	assertEquals (175.2, d, 0.1);

	LatLon latLon = goldenGateSouthTower.greatCircleEndPosition(Angle::fromDegrees(355.2), goldenGateNorthTower.greatCircleDistanceTo(goldenGateSouthTower));
	assert (latLon.equals(goldenGateNorthTower, 0.00001));

	latLon = goldenGateSouthTower.rhumbEndPosition(Angle::fromDegrees(355.2), goldenGateNorthTower.greatCircleDistanceTo(goldenGateSouthTower));
	assert (latLon.equals(goldenGateNorthTower, 0.00001));
	
	LatLon sanFrancisco(LatLon::fromDegrees(37.728965, -122.420151));
	LatLon newYork(LatLon::fromDegrees(40.712180, -73.995796));
	
	a = newYork.greatCircleDistanceTo(sanFrancisco);
	m = a.radians()*LatLon::EARTH_MEAN_RADIUS;
	
	assertEquals (4131767.3, m, 0.1);
	
	a = newYork.greatCircleAzimuthTo(sanFrancisco);
	d = a.normalized().degrees();
	
	assertEquals(281.6, d, 0.1);

	a = newYork.rhumbDistanceTo(sanFrancisco);
	m = a.radians()*LatLon::EARTH_MEAN_RADIUS;
	
	assertEquals (4183577.9, m, 0.1);
	
	a = newYork.rhumbAzimuthTo(sanFrancisco);
	d = a.normalized().degrees();
	
	assertEquals(265.4, d, 0.1);
}


void LatLonTest::setUp()
{
}


void LatLonTest::tearDown()
{
}


CppUnit::Test* LatLonTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LatLonTest");

	CppUnit_addTest(pSuite, LatLonTest, testLatLon);

	return pSuite;
}
