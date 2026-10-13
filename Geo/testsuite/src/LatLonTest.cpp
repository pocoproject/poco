//
// LatLonTest.cpp
//
// Copyright (c) 2011-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "LatLonTest.h"
#include "Poco/Geo/LatLon.h"
#include "Poco/FPEnvironment.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include <cmath>
#include <limits>


using namespace Poco::Geo;
using Poco::FPEnvironment;


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
	
	assertEqualDelta (1296.7, m, 0.1);
	
	a = goldenGateNorthTower.greatCircleAzimuthTo(goldenGateSouthTower);
	double d = a.degrees();
	
	assertEqualDelta (175.2, d, 0.1);

	a = goldenGateNorthTower.rhumbDistanceTo(goldenGateSouthTower);
	m = a.radians()*LatLon::EARTH_MEAN_RADIUS;
	
	assertEqualDelta (1296.7, m, 0.1);
	
	a = goldenGateNorthTower.rhumbAzimuthTo(goldenGateSouthTower);
	d = a.degrees();
	
	assertEqualDelta (175.2, d, 0.1);

	LatLon latLon = goldenGateSouthTower.greatCircleEndPosition(Angle::fromDegrees(355.2), goldenGateNorthTower.greatCircleDistanceTo(goldenGateSouthTower));
	assert (latLon.equals(goldenGateNorthTower, 0.00001));

	latLon = goldenGateSouthTower.rhumbEndPosition(Angle::fromDegrees(355.2), goldenGateNorthTower.greatCircleDistanceTo(goldenGateSouthTower));
	assert (latLon.equals(goldenGateNorthTower, 0.00001));
	
	LatLon sanFrancisco(LatLon::fromDegrees(37.728965, -122.420151));
	LatLon newYork(LatLon::fromDegrees(40.712180, -73.995796));
	
	a = newYork.greatCircleDistanceTo(sanFrancisco);
	m = a.radians()*LatLon::EARTH_MEAN_RADIUS;
	
	assertEqualDelta (4131767.3, m, 0.1);
	
	a = newYork.greatCircleAzimuthTo(sanFrancisco);
	d = a.normalized().degrees();
	
	assertEqualDelta (281.6, d, 0.1);

	a = newYork.rhumbDistanceTo(sanFrancisco);
	m = a.radians()*LatLon::EARTH_MEAN_RADIUS;
	
	assertEqualDelta (4183577.9, m, 0.1);
	
	a = newYork.rhumbAzimuthTo(sanFrancisco);
	d = a.normalized().degrees();
	
	assertEqualDelta (265.4, d, 0.1);
}


void LatLonTest::testToCartesian()
{
	const LatLon ref = LatLon::fromDegrees(52.247041, 10.575830);

	LatLon pos = LatLon::fromDegrees(52.247041, 10.575830);

	auto result = pos.toCartesian(ref);

	// The position is the reference, so it projects onto the origin with no rounding
	// left to tolerate.
	assertEqual (0.0, result.x);
	assertEqual (0.0, result.y);


	pos = LatLon::fromDegrees(52.248091, 10.57417);
	
	result = pos.toCartesian(ref);

	assertEqualDelta (-113.3742031902, result.x, 0.0001);
	assertEqualDelta (116.8369533306, result.y, 0.0001);


	pos = LatLon::fromDegrees(52.251011, 10.573568);

	result = pos.toCartesian(ref);

	assertEqualDelta (-154.48, result.x, 0.01);
	assertEqualDelta (441.75, result.y, 0.01);
}


void LatLonTest::testFromCartesian()
{
	const LatLon ref = LatLon::fromDegrees(52.247041, 10.575830);

	auto result = LatLon::fromCartesian(ref, -154.48, 441.75);

	assertEqualDelta (52.251011, result.latitude().degrees(), 0.000001);
	assertEqualDelta (10.573568, result.longitude().degrees(), 0.000001);


	result = LatLon::fromCartesian(ref, -208.57, 431.07);

	assertEqualDelta (52.250915, result.latitude().degrees(), 0.000001);
	assertEqualDelta (10.572776, result.longitude().degrees(), 0.000001);
}


void LatLonTest::testCartesianRoundTrip()
{
	// The tolerance of 1e-11 degrees, about one micrometer, keeps the round trip well
	// below the millimeter that an application of this library could notice.
	constexpr double TOLERANCE = 1e-11;

	// equals() compares with <=, so a NaN coordinate fails the comparison.
	const auto roundTrips = [](const LatLon& reference, const LatLon& pos)
	{
		return LatLon::fromCartesian(reference, pos.toCartesian(reference)).equals(pos, TOLERANCE);
	};

	const LatLon ref = LatLon::fromDegrees(52.247041, 10.575830);

	// About 150 m, 470 m and 4.5 km from the reference.
	assertTrue (roundTrips(ref, LatLon::fromDegrees(52.248091, 10.574170)));
	assertTrue (roundTrips(ref, LatLon::fromDegrees(52.251011, 10.573568)));
	assertTrue (roundTrips(ref, LatLon::fromDegrees(52.280000, 10.620000)));

	// A reference in the southern hemisphere, at another latitude band, with a
	// position about 1.5 km away.
	const LatLon southernRef = LatLon::fromDegrees(-33.856800, 151.215200);

	assertTrue (roundTrips(southernRef, LatLon::fromDegrees(-33.850000, 151.230000)));
}


void LatLonTest::testAssignAndSwap()
{
	const LatLon southTower = LatLon::fromDegrees(37.813964, -122.478192);
	const LatLon northTower = LatLon::fromDegrees(37.825585, -122.479426);

	LatLon copy;
	copy.assign(southTower);
	assertTrue (copy == southTower);

	copy.assign(Angle::fromDegrees(10), Angle::fromDegrees(20));
	assertEqual (10.0, copy.latitude().degrees());
	assertEqual (20.0, copy.longitude().degrees());

	LatLon other(northTower);
	copy.swap(other);
	assertTrue (copy == northTower);
	assertEqual (10.0, other.latitude().degrees());
	assertEqual (20.0, other.longitude().degrees());
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


void LatLonTest::testAntipodalGreatCircleDistance()
{
	// For an antipodal pair the haversine term rounds above 1, where the square root
	// of its complement is NaN and the distance would read as zero.
	const LatLon pos = LatLon::fromDegrees(0.015, 20.0);
	const LatLon antipode = LatLon::fromDegrees(-0.015, -160.0);
	const double m = pos.greatCircleDistanceTo(antipode).radians()*LatLon::EARTH_MEAN_RADIUS;

	assertEqualDelta (Angle::PI*LatLon::EARTH_MEAN_RADIUS, m, 0.001);
}


void LatLonTest::testGreatCircleEndPositionAtPole()
{
	// The argument of the arc sine rounds above 1 when the arc ends exactly on a pole,
	// where the formula is NaN and the start position would be returned unchanged.
	const LatLon start = LatLon::fromDegrees(-56.25, 10);
	const LatLon end = start.greatCircleEndPosition(Angle::fromDegrees(0), Angle::fromDegrees(146.25));

	assertEqualDelta (90.0, end.latitude().degrees(), 1e-12);
}


void LatLonTest::testDistanceToNaNPosition()
{
	// A NaN coordinate must carry through to the distance; a zero distance would put
	// the two positions on top of each other.
	const LatLon pos = LatLon::fromDegrees(46.6, 14.04);
	const LatLon undefined = LatLon::fromDegrees(std::numeric_limits<double>::quiet_NaN(), 10.0);

	assertTrue (FPEnvironment::isNaN(pos.greatCircleDistanceTo(undefined).radians()));
}


void LatLonTest::testRhumbDueEastOnEquator()
{
	// On a due east or west course the stretched latitudes are equal, so the factor
	// that carries the course cannot be their quotient and the position would not move.
	const LatLon end = LatLon::fromDegrees(0, 0).rhumbEndPosition(Angle::fromDegrees(90), Angle::fromDegrees(1));

	assertEqualDelta (0.0, end.latitude().degrees(), 1e-12);
	assertEqualDelta (1.0, end.longitude().degrees(), 1e-12);
}


void LatLonTest::testRhumbBetweenNearlyEqualLatitudes()
{
	// A few ulps of latitude round the difference of the stretched latitudes to zero,
	// where the quotient that carries the course is infinite and the distance with it.
	const auto rhumbMeters = [](double latitude1, double latitude2)
	{
		const LatLon from = LatLon::fromDegrees(latitude1, 10.0);
		const LatLon to = LatLon::fromDegrees(latitude2, 11.0);
		return from.rhumbDistanceTo(to).radians()*LatLon::EARTH_MEAN_RADIUS;
	};

	const double oneUlp = std::nextafter(45.0, 90.0);
	const double twoUlps = std::nextafter(oneUlp, 90.0);
	const double threeUlps = std::nextafter(twoUlps, 90.0);

	assertEqualDelta (78626.8, rhumbMeters(45.0, oneUlp), 1.0);
	assertEqualDelta (78626.8, rhumbMeters(45.0, twoUlps), 1.0);
	assertEqualDelta (78626.8, rhumbMeters(45.0, threeUlps), 1.0);
}


void LatLonTest::testRhumbWestAcrossAntimeridian()
{
	// The course crosses the antimeridian, where a longitude that is not wrapped comes
	// out beyond the range of a longitude.
	const LatLon start = LatLon::fromDegrees(10, -179);
	const LatLon end = start.rhumbEndPosition(Angle::fromDegrees(270), Angle::fromDegrees(2));

	assertEqualDelta (10.0, end.latitude().degrees(), 1e-9);
	assertEqualDelta (178.969, end.longitude().degrees(), 0.001);
}


void LatLonTest::testToCartesianAcrossAntimeridian()
{
	// Taken without wrapping, the difference of the two longitudes puts a position and
	// a reference on either side of the antimeridian a whole turn apart.
	const LatLon ref = LatLon::fromDegrees(0, 179);
	const Cartesian result = LatLon::fromDegrees(0, -179).toCartesian(ref);

	assertEqualDelta (222638.98, result.x, 0.01);
	assertEqualDelta (0.0, result.y, 1e-9);
}


void LatLonTest::testToCartesianOutOfDomain()
{
	// A latitude beyond a pole is outside the domain of the projection. The origin
	// would be the projection of the reference position itself.
	const LatLon ref = LatLon::fromDegrees(52.247041, 10.575830);
	const Cartesian result = LatLon::toCartesian(ref, LatLon::fromDegrees(95, 10));

	assertTrue (FPEnvironment::isNaN(result.x));
	assertTrue (FPEnvironment::isNaN(result.y));
}


void LatLonTest::testFromCartesianNearPole()
{
	// Next to a pole the projection is not invertible. The solve reports that, rather
	// than hand back the position its last step happened to reach, which can be out
	// of range in both coordinates.
	const LatLon ref = LatLon::fromDegrees(89.999, 0);
	const LatLon result = LatLon::fromCartesian(ref, 1000.0, 1000.0);

	if (FPEnvironment::isNaN(result.latitude().degrees()))
	{
		assertTrue (FPEnvironment::isNaN(result.longitude().degrees()));
	}
	else
	{
		assertTrue (std::abs(result.latitude().degrees()) <= 90.0);
		assertTrue (std::abs(result.longitude().degrees()) <= 180.0);

		// A solved position reproduces the coordinates it was solved for to within
		// one micrometer.
		const Cartesian back = result.toCartesian(ref);
		assertEqualDelta (1000.0, back.x, 1e-6);
		assertEqualDelta (1000.0, back.y, 1e-6);
	}
}


void LatLonTest::testEqualsAcrossAntimeridian()
{
	// Taken without wrapping, the difference of the two longitudes makes positions on
	// either side of the antimeridian look a whole turn apart.
	const LatLon east = LatLon::fromDegrees(0, 179.9999);
	const LatLon west = LatLon::fromDegrees(0, -179.9999);

	assertTrue (east.equals(west, 0.001));
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
	CppUnit_addTest(pSuite, LatLonTest, testAssignAndSwap);
	CppUnit_addTest(pSuite, LatLonTest, testComparison);
	CppUnit_addTest(pSuite, LatLonTest, testFromRadians);
	CppUnit_addTest(pSuite, LatLonTest, testAntipodalGreatCircleDistance);
	CppUnit_addTest(pSuite, LatLonTest, testGreatCircleEndPositionAtPole);
	CppUnit_addTest(pSuite, LatLonTest, testDistanceToNaNPosition);
	CppUnit_addTest(pSuite, LatLonTest, testRhumbDueEastOnEquator);
	CppUnit_addTest(pSuite, LatLonTest, testRhumbBetweenNearlyEqualLatitudes);
	CppUnit_addTest(pSuite, LatLonTest, testRhumbWestAcrossAntimeridian);
	CppUnit_addTest(pSuite, LatLonTest, testToCartesianAcrossAntimeridian);
	CppUnit_addTest(pSuite, LatLonTest, testToCartesianOutOfDomain);
	CppUnit_addTest(pSuite, LatLonTest, testFromCartesianNearPole);
	CppUnit_addTest(pSuite, LatLonTest, testEqualsAcrossAntimeridian);

	return pSuite;
}
