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

	return pSuite;
}
