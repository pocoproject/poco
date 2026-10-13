//
// AngleTest.cpp
//
// Copyright (c) 2011-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AngleTest.h"
#include "Poco/Geo/Angle.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include <cmath>


using namespace Poco::Geo;


AngleTest::AngleTest(const std::string& name): CppUnit::TestCase(name)
{
}


AngleTest::~AngleTest()
{
}


void AngleTest::testAngle()
{
	Angle a = Angle::fromDegrees(0);
	assert (a.degrees() == 0);
	assert (a.radians() == 0);

	a = Angle::fromDegrees(90);
	assert (a.degrees() == 90);
	assertEqualDelta (Angle::PI/2, a.radians(), 0.01);

	a = Angle::fromDegrees(180);
	assert (a.degrees() == 180);
	assertEqualDelta (Angle::PI, a.radians(), 0.01);
	
	a = Angle::fromDegrees(360);
	assert (a.normalized().degrees() == 0);

	a = Angle::fromDegrees(-10);
	assert (a.normalized().degrees() == 350);

	a = Angle::fromDegrees(370);
	assert (a.normalized().degrees() == 10);

	a = Angle::fromDegrees(40);
	assert (a.normalizedLatitude().degrees() == 40);

	a = Angle::fromDegrees(90);
	assert (a.normalizedLatitude().degrees() == 90);

	a = Angle::fromDegrees(100);
	assert (a.normalizedLatitude().degrees() == 80);

	a = Angle::fromDegrees(100);
	assert (a.normalizedLongitude().degrees() == 100);

	a = Angle::fromDegrees(180);
	assert (a.normalizedLongitude().degrees() == 180);

	a = Angle::fromDegrees(190);
	assert (a.normalizedLongitude().degrees() == -170);
	
	a = Angle::fromDegrees(10);
	a = a + Angle::fromDegrees(10);
	assert (a.degrees() == 20);
	
	a = a - Angle::fromDegrees(10);
	assert (a.degrees() == 10);
	
	a = a*2;
	assert (a.degrees() == 20);
}


void AngleTest::testCompass()
{
	Angle a = Angle::fromDegrees(0);
	assert (a.compassDirection() == "N");
	
	a = Angle::fromDegrees(22.5);
	assert (a.compassDirection() == "NNE");

	a = Angle::fromDegrees(45);
	assert (a.compassDirection() == "NE");

	a = Angle::fromDegrees(67.5);
	assert (a.compassDirection() == "ENE");

	a = Angle::fromDegrees(90);
	assert (a.compassDirection() == "E");

	a = Angle::fromDegrees(112.5);
	assert (a.compassDirection() == "ESE");

	a = Angle::fromDegrees(135);
	assert (a.compassDirection() == "SE");

	a = Angle::fromDegrees(157.5);
	assert (a.compassDirection() == "SSE");

	a = Angle::fromDegrees(180);
	assert (a.compassDirection() == "S");

	a = Angle::fromDegrees(202.5);
	assert (a.compassDirection() == "SSW");

	a = Angle::fromDegrees(225);
	assert (a.compassDirection() == "SW");

	a = Angle::fromDegrees(247.5);
	assert (a.compassDirection() == "WSW");

	a = Angle::fromDegrees(270);
	assert (a.compassDirection() == "W");

	a = Angle::fromDegrees(292.5);
	assert (a.compassDirection() == "WNW");
	
	a = Angle::fromDegrees(315);
	assert (a.compassDirection() == "NW");
	
	a = Angle::fromDegrees(337.5);
	assert (a.compassDirection() == "NNW");

	a = Angle::fromDegrees(360);
	assert (a.compassDirection() == "N");

	a = Angle::fromDegrees(382.5);
	assert (a.compassDirection() == "NNE");

	a = Angle::fromDegrees(-22.5);
	assert (a.compassDirection() == "NNW");
}


void AngleTest::testFromRadiansAndXY()
{
	// 45 degrees is PI/4 radians.
	Angle a = Angle::fromRadians(Angle::PI/4);
	assertEqualDelta (45.0, a.degrees(), 1e-12);
	assertEqualDelta (Angle::PI/4, a.radians(), 1e-15);

	// A latitude in radians is clamped to [-PI/2, PI/2].
	a = Angle::fromRadiansLatitude(Angle::PI/6);
	assertEqualDelta (30.0, a.degrees(), 1e-12);

	a = Angle::fromRadiansLatitude(Angle::PI);
	assertEqualDelta (90.0, a.degrees(), 1e-12);
	assertEqualDelta (Angle::PI_OVER_2, a.radians(), 1e-15);

	a = Angle::fromRadiansLatitude(-Angle::PI);
	assertEqualDelta (-90.0, a.degrees(), 1e-12);

	// A longitude in radians is clamped to [-PI, PI].
	a = Angle::fromRadiansLongitude(Angle::PI/2);
	assertEqualDelta (90.0, a.degrees(), 1e-12);

	a = Angle::fromRadiansLongitude(2*Angle::PI);
	assertEqualDelta (180.0, a.degrees(), 1e-12);
	assertEqualDelta (Angle::PI, a.radians(), 1e-15);

	a = Angle::fromRadiansLongitude(-2*Angle::PI);
	assertEqualDelta (-180.0, a.degrees(), 1e-12);

	// fromXY() returns the angle of the point, measured counterclockwise
	// from the positive x axis, as atan2(y, x) does.
	assertEqualDelta (0.0, Angle::fromXY(1, 0).degrees(), 1e-12);
	assertEqualDelta (45.0, Angle::fromXY(1, 1).degrees(), 1e-12);
	assertEqualDelta (90.0, Angle::fromXY(0, 1).degrees(), 1e-12);
	assertEqualDelta (135.0, Angle::fromXY(-1, 1).degrees(), 1e-12);
	assertEqualDelta (180.0, Angle::fromXY(-1, 0).degrees(), 1e-12);
	assertEqualDelta (-90.0, Angle::fromXY(0, -1).degrees(), 1e-12);
	assertEqualDelta (-135.0, Angle::fromXY(-1, -1).degrees(), 1e-12);
}


void AngleTest::testClamping()
{
	// A latitude beyond a pole is clamped to the pole, in degrees and in radians.
	Angle a = Angle::fromDegreesLatitude(100);
	assertEqualDelta (90.0, a.degrees(), 1e-12);
	assertEqualDelta (Angle::PI_OVER_2, a.radians(), 1e-15);

	a = Angle::fromDegreesLatitude(-100);
	assertEqualDelta (-90.0, a.degrees(), 1e-12);
	assertEqualDelta (-Angle::PI_OVER_2, a.radians(), 1e-15);

	a = Angle::fromDegreesLatitude(45);
	assertEqualDelta (45.0, a.degrees(), 1e-12);

	// A longitude beyond the antimeridian is clamped to it, not wrapped around.
	a = Angle::fromDegreesLongitude(200);
	assertEqualDelta (180.0, a.degrees(), 1e-12);
	assertEqualDelta (Angle::PI, a.radians(), 1e-15);

	a = Angle::fromDegreesLongitude(-200);
	assertEqualDelta (-180.0, a.degrees(), 1e-12);
	assertEqualDelta (-Angle::PI, a.radians(), 1e-15);

	a = Angle::fromDegreesLongitude(-90);
	assertEqualDelta (-90.0, a.degrees(), 1e-12);
}


void AngleTest::testNormalization()
{
	// normalizedDegrees() maps an angle to [0, 360).
	assertEqualDelta (45.0, Angle::normalizedDegrees(45), 1e-12);
	assertEqualDelta (350.0, Angle::normalizedDegrees(-370), 1e-12);
	assertEqualDelta (0.0, Angle::normalizedDegrees(720), 1e-12);

	// normalizedDegreesLatitude() reflects a latitude beyond a pole back.
	assertEqualDelta (-45.0, Angle::normalizedDegreesLatitude(-45), 1e-12);
	assertEqualDelta (85.0, Angle::normalizedDegreesLatitude(95), 1e-12);
	assertEqualDelta (-80.0, Angle::normalizedDegreesLatitude(-100), 1e-12);

	// Beyond 180 degrees the reflection changes hemisphere: going north from the
	// equator, 90 is the north pole, 180 is the equator on the far side, 270 is
	// the south pole and 360 is the starting point again.
	assertEqualDelta (-10.0, Angle::normalizedDegreesLatitude(190), 1e-12);
	assertEqualDelta (-90.0, Angle::normalizedDegreesLatitude(270), 1e-12);
	assertEqualDelta (0.0, Angle::normalizedDegreesLatitude(360), 1e-12);
	assertEqualDelta (10.0, Angle::normalizedDegreesLatitude(-190), 1e-12);
	assertEqualDelta (90.0, Angle::normalizedDegreesLatitude(-270), 1e-12);

	// normalizedDegreesLongitude() wraps a longitude at the antimeridian.
	assertEqualDelta (-90.0, Angle::normalizedDegreesLongitude(-90), 1e-12);
	assertEqualDelta (-160.0, Angle::normalizedDegreesLongitude(200), 1e-12);
	assertEqualDelta (160.0, Angle::normalizedDegreesLongitude(-200), 1e-12);
	assertEqualDelta (180.0, Angle::normalizedDegreesLongitude(540), 1e-12);

	// The members normalize the Angle by the same rules.
	assertEqualDelta (350.0, Angle::fromDegrees(-370).normalized().degrees(), 1e-12);
	assertEqualDelta (-80.0, Angle::fromDegrees(-100).normalizedLatitude().degrees(), 1e-12);
	assertEqualDelta (-160.0, Angle::fromDegrees(200).normalizedLongitude().degrees(), 1e-12);
}


void AngleTest::testArithmetic()
{
	Angle a = Angle::fromDegrees(30) + Angle::fromDegrees(45);
	assertEqualDelta (75.0, a.degrees(), 1e-12);

	a = Angle::fromDegrees(45) - Angle::fromDegrees(30);
	assertEqualDelta (15.0, a.degrees(), 1e-12);

	a = Angle::fromDegrees(30)*3;
	assertEqualDelta (90.0, a.degrees(), 1e-12);
	assertEqualDelta (Angle::PI_OVER_2, a.radians(), 1e-15);

	a = Angle::fromDegrees(90)/3;
	assertEqualDelta (30.0, a.degrees(), 1e-12);

	assertEqualDelta (3.0, Angle::fromDegrees(90)/Angle::fromDegrees(30), 1e-12);

	// angularDistanceTo() returns the shorter way round, in both directions
	// and also for a pair that is separated by the 180 degrees boundary.
	assertEqualDelta (90.0, Angle::fromDegrees(0).angularDistanceTo(Angle::fromDegrees(90)).degrees(), 1e-12);
	assertEqualDelta (20.0, Angle::fromDegrees(10).angularDistanceTo(Angle::fromDegrees(350)).degrees(), 1e-12);
	assertEqualDelta (20.0, Angle::fromDegrees(170).angularDistanceTo(Angle::fromDegrees(-170)).degrees(), 1e-12);
	assertEqualDelta (20.0, Angle::fromDegrees(-170).angularDistanceTo(Angle::fromDegrees(170)).degrees(), 1e-12);
}


void AngleTest::testComparisonOperators()
{
	const Angle a = Angle::fromDegrees(30);
	const Angle b = Angle::fromDegrees(30);
	const Angle c = Angle::fromDegrees(45);

	assertTrue (a == b);
	assertFalse (a != b);
	assertTrue (a != c);
	assertFalse (a == c);

	assertTrue (a < c);
	assertFalse (c < a);
	assertFalse (a < b);

	assertTrue (a <= c);
	assertTrue (a <= b);
	assertFalse (c <= a);

	assertTrue (c > a);
	assertFalse (a > c);
	assertFalse (a > b);

	assertTrue (c >= a);
	assertTrue (a >= b);
	assertFalse (a >= c);
}


void AngleTest::testSinCos()
{
	// Exact values: sin(30) = cos(60) = 1/2, sin(45) = cos(45) = sqrt(2)/2,
	// sin(60) = cos(30) = sqrt(3)/2.
	const double sqrt2Half = std::sqrt(2.0)/2;
	const double sqrt3Half = std::sqrt(3.0)/2;

	assertEqualDelta (0.0, Angle::fromDegrees(0).sin(), 1e-12);
	assertEqualDelta (1.0, Angle::fromDegrees(0).cos(), 1e-12);

	assertEqualDelta (0.5, Angle::fromDegrees(30).sin(), 1e-12);
	assertEqualDelta (sqrt3Half, Angle::fromDegrees(30).cos(), 1e-12);

	assertEqualDelta (sqrt2Half, Angle::fromDegrees(45).sin(), 1e-12);
	assertEqualDelta (sqrt2Half, Angle::fromDegrees(45).cos(), 1e-12);

	assertEqualDelta (sqrt3Half, Angle::fromDegrees(60).sin(), 1e-12);
	assertEqualDelta (0.5, Angle::fromDegrees(60).cos(), 1e-12);

	assertEqualDelta (1.0, Angle::fromDegrees(90).sin(), 1e-12);
	assertEqualDelta (0.0, Angle::fromDegrees(90).cos(), 1e-12);
}


void AngleTest::setUp()
{
}


void AngleTest::tearDown()
{
}


CppUnit::Test* AngleTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AngleTest");

	CppUnit_addTest(pSuite, AngleTest, testAngle);
	CppUnit_addTest(pSuite, AngleTest, testCompass);
	CppUnit_addTest(pSuite, AngleTest, testFromRadiansAndXY);
	CppUnit_addTest(pSuite, AngleTest, testClamping);
	CppUnit_addTest(pSuite, AngleTest, testNormalization);
	CppUnit_addTest(pSuite, AngleTest, testArithmetic);
	CppUnit_addTest(pSuite, AngleTest, testComparisonOperators);
	CppUnit_addTest(pSuite, AngleTest, testSinCos);

	return pSuite;
}
