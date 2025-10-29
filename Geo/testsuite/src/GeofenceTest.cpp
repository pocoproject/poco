//
// GeofenceTest.cpp
//
// Copyright (c) 2025, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "GeofenceTest.h"
#include "Poco/Geo/CircularGeofence.h"
#include "Poco/Geo/PolygonalGeofence.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"


using namespace Poco::Geo;


GeofenceTest::GeofenceTest(const std::string& name): CppUnit::TestCase(name)
{
}


GeofenceTest::~GeofenceTest()
{
}


void GeofenceTest::testCircular()
{
	LatLon center(
		Angle::fromDegreesLatitude(46.617527564222684),
		Angle::fromDegreesLongitude(14.041860704081785)
	);

	CircularGeofence fence(center, 500);

	LatLon inside(
		Angle::fromDegreesLatitude(46.6159971048821),
		Angle::fromDegreesLongitude(14.03940505544034)
	);
	assertTrue (fence.isInside(inside));

	LatLon outside(
		Angle::fromDegreesLatitude(46.611158906223295),
		Angle::fromDegreesLongitude(14.040488650824983)
	);
	assertFalse (fence.isInside(outside));
}


void GeofenceTest::testPolygonal()
{
	// Geofence polygon coordinates in GeoJSON format (lon, lat)
	std::vector<std::pair<double, double>> coordinates(
		{
            {
              14.033344303704354,
              46.61601559225525
			},
            {
              14.036256466299108,
              46.60568737905788
			},
            {
              14.052510397056068,
              46.608478982249466
			},
            {
              14.05257812176876,
              46.60945600937768
			},
            {
              14.045128403504435,
              46.60936295993457
			},
            {
              14.042622589178762,
              46.61387567381189
			},
            {
              14.049327335616255,
              46.61573647725416
			},
            {
              14.047363318983628,
              46.6195509244188
			},
            {
              14.033344303704354,
              46.61601559225525
			}
		}
	);

	std::vector<LatLon> polygon;
	for (const auto& c: coordinates)
	{
		polygon.push_back(
			LatLon(Angle::fromDegreesLatitude(c.second), Angle::fromDegreesLongitude(c.first))
		);
	}

	PolygonalGeofence fence(polygon);

	LatLon inside1(
		Angle::fromDegreesLatitude(46.618946212913244),
		Angle::fromDegreesLongitude(14.046956970714518)
	);
	assertTrue (fence.isInside(inside1));

	LatLon inside2(
		Angle::fromDegreesLatitude(46.614480441943925),
		Angle::fromDegreesLongitude(14.035172870914437)
	);
	assertTrue (fence.isInside(inside2));

	LatLon inside3(
		Angle::fromDegreesLatitude(46.611735670807974),
		Angle::fromDegreesLongitude(14.042487139756247)
	);
	assertTrue (fence.isInside(inside3));

	LatLon inside4(
		Angle::fromDegreesLatitude(46.608572033211146),
		Angle::fromDegreesLongitude(14.048176015521733)
	);
	assertTrue (fence.isInside(inside4));

	LatLon outside1(
		Angle::fromDegreesLatitude(46.618853179774305),
		Angle::fromDegreesLongitude(14.050410931000897)
	);
	assertFalse (fence.isInside(outside1));

	LatLon outside2(
		Angle::fromDegreesLatitude(46.61215437370177),
		Angle::fromDegreesLongitude(14.046821521292003)
	);
	assertFalse (fence.isInside(outside2));

	LatLon outside3(
		Angle::fromDegreesLatitude(46.60531515442932),
		Angle::fromDegreesLongitude(14.044451156388874)
	);
	assertFalse (fence.isInside(outside3));
}


void GeofenceTest::setUp()
{
}


void GeofenceTest::tearDown()
{
}


CppUnit::Test* GeofenceTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("GeofenceTest");

	CppUnit_addTest(pSuite, GeofenceTest, testCircular);
	CppUnit_addTest(pSuite, GeofenceTest, testPolygonal);

	return pSuite;
}
