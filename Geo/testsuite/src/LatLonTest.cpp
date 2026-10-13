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


void LatLonTest::testToCartesian()
{
	const LatLon ref = LatLon::fromDegrees(52.247041, 10.575830);

	LatLon pos = LatLon::fromDegrees(52.247041, 10.575830);

	auto result = pos.toCartesian(ref);

	assertEquals(0, result.x, 0);
	assertEquals(0, result.y, 0);


	pos = LatLon::fromDegrees(52.248091, 10.57417);
	
	result = pos.toCartesian(ref);

	assertEquals(-113.3742031902, result.x, 0.0001);
	assertEquals(116.8369533306, result.y, 0.0001);


	pos = LatLon::fromDegrees(52.251011, 10.573568);

	result = pos.toCartesian(ref);

	assertEquals(-154.48, result.x, 0.01);
	assertEquals(441.75, result.y, 0.01);
}


void LatLonTest::testFromCartesian()
{
	const LatLon ref = LatLon::fromDegrees(52.247041, 10.575830);

	auto result = LatLon::fromCartesian(ref, -154.48, 441.75);

	assertEquals(52.251011, result.latitude().degrees(), 0.000001);
	assertEquals(10.573568, result.longitude().degrees(), 0.000001);


	result = LatLon::fromCartesian(ref, -208.57, 431.07);

	assertEquals(52.250915, result.latitude().degrees(), 0.000001);
	assertEquals(10.572776, result.longitude().degrees(), 0.000001);
}


void LatLonTest::testCartesianRoundTrip()
{
	// fromCartesian() solves for the position with Newton's method on the forward
	// projection and accepts a residual of 1.0e-9 meters. The round trip error is
	// independent of the offset from the reference: it stays at a few nanometers
	// from 10 m out to 100 km, at every bearing and in every latitude band. The
	// tolerance below is one micrometer, which is three orders of magnitude above
	// the measured error and still three orders below the millimeter that an
	// application of this library could notice.
	constexpr double TOLERANCE = 1e-6;

	const LatLon ref = LatLon::fromDegrees(52.247041, 10.575830);

	// About 150 m from the reference.
	LatLon pos = LatLon::fromDegrees(52.248091, 10.574170);
	LatLon back = LatLon::fromCartesian(ref, pos.toCartesian(ref));
	double error = pos.greatCircleDistanceTo(back).radians()*LatLon::EARTH_MEAN_RADIUS;
	assertEquals(0.0, error, TOLERANCE);

	// About 470 m from the reference.
	pos = LatLon::fromDegrees(52.251011, 10.573568);
	back = LatLon::fromCartesian(ref, pos.toCartesian(ref));
	error = pos.greatCircleDistanceTo(back).radians()*LatLon::EARTH_MEAN_RADIUS;
	assertEquals(0.0, error, TOLERANCE);

	// About 4.5 km from the reference.
	pos = LatLon::fromDegrees(52.280000, 10.620000);
	back = LatLon::fromCartesian(ref, pos.toCartesian(ref));
	error = pos.greatCircleDistanceTo(back).radians()*LatLon::EARTH_MEAN_RADIUS;
	assertEquals(0.0, error, TOLERANCE);

	// A reference in the southern hemisphere, at another latitude band, with a
	// position about 1.5 km away.
	const LatLon southernRef = LatLon::fromDegrees(-33.856800, 151.215200);

	pos = LatLon::fromDegrees(-33.850000, 151.230000);
	back = LatLon::fromCartesian(southernRef, pos.toCartesian(southernRef));
	error = pos.greatCircleDistanceTo(back).radians()*LatLon::EARTH_MEAN_RADIUS;
	assertEquals(0.0, error, TOLERANCE);
}


void LatLonTest::testCartesianOverloads()
{
	const LatLon ref = LatLon::fromDegrees(52.247041, 10.575830);
	const LatLon pos = LatLon::fromDegrees(52.251011, 10.573568);

	// The member and the static toCartesian(), and the two fromCartesian()
	// overloads, perform the same computation, so they agree exactly.
	const Cartesian fromMember = pos.toCartesian(ref);
	const Cartesian fromStatic = LatLon::toCartesian(ref, pos);
	assertEquals(fromMember.x, fromStatic.x, 0);
	assertEquals(fromMember.y, fromStatic.y, 0);

	const LatLon fromStruct = LatLon::fromCartesian(ref, fromMember);
	const LatLon fromCoordinates = LatLon::fromCartesian(ref, fromMember.x, fromMember.y);
	assertTrue (fromStruct == fromCoordinates);

	assertEquals(52.251011, fromStruct.latitude().degrees(), 0.000001);
	assertEquals(10.573568, fromStruct.longitude().degrees(), 0.000001);
}


void LatLonTest::testAssignAndSwap()
{
	const LatLon southTower = LatLon::fromDegrees(37.813964, -122.478192);
	const LatLon northTower = LatLon::fromDegrees(37.825585, -122.479426);

	LatLon copy(southTower);
	assertTrue (copy == southTower);

	copy = northTower;
	assertTrue (copy == northTower);

	copy.assign(southTower);
	assertTrue (copy == southTower);

	copy.assign(Angle::fromDegrees(10), Angle::fromDegrees(20));
	assertEquals(10.0, copy.latitude().degrees(), 0);
	assertEquals(20.0, copy.longitude().degrees(), 0);

	LatLon other(northTower);
	copy.swap(other);
	assertTrue (copy == northTower);
	assertEquals(10.0, other.latitude().degrees(), 0);
	assertEquals(20.0, other.longitude().degrees(), 0);
}


void LatLonTest::testComparison()
{
	const LatLon southTower = LatLon::fromDegrees(37.813964, -122.478192);
	const LatLon sameTower = LatLon::fromDegrees(37.813964, -122.478192);
	const LatLon northTower = LatLon::fromDegrees(37.825585, -122.479426);

	assertTrue (southTower == sameTower);
	assertFalse (southTower != sameTower);
	assertTrue (southTower != northTower);
	assertFalse (southTower == northTower);

	// equals() compares latitude and longitude separately, with the delta given
	// in degrees. The two towers are 0.011621 degrees of latitude apart.
	assertTrue (southTower.equals(northTower, 0.02));
	assertFalse (southTower.equals(northTower, 0.01));
	assertTrue (southTower.equals(sameTower, 0));
}


void LatLonTest::testFromRadians()
{
	// PI/4 radians are 45 degrees, PI/2 radians are 90 degrees.
	LatLon latLon = LatLon::fromRadians(Angle::PI/4, Angle::PI/2);
	assertTrue (latLon.equals(LatLon::fromDegrees(45, 90), 1e-12));

	latLon = LatLon::fromRadians(-Angle::PI/6, -Angle::PI/3);
	assertTrue (latLon.equals(LatLon::fromDegrees(-30, -60), 1e-12));

	latLon = LatLon::fromRadians(0, 0);
	assertTrue (latLon == LatLon::ZERO);
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
	CppUnit_addTest(pSuite, LatLonTest, testToCartesian);
	CppUnit_addTest(pSuite, LatLonTest, testFromCartesian);
	CppUnit_addTest(pSuite, LatLonTest, testCartesianRoundTrip);
	CppUnit_addTest(pSuite, LatLonTest, testCartesianOverloads);
	CppUnit_addTest(pSuite, LatLonTest, testAssignAndSwap);
	CppUnit_addTest(pSuite, LatLonTest, testComparison);
	CppUnit_addTest(pSuite, LatLonTest, testFromRadians);

	return pSuite;
}
