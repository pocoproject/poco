//
// GeofenceTest.cpp
//
// Copyright (c) 2025, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "GeofenceTest.h"
#include "Poco/Geo/CircularGeofence.h"
#include "Poco/Geo/Geofence.h"
#include "Poco/Geo/PolygonalGeofence.h"
#include "Poco/Exception.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"


using namespace Poco::Geo;
using Poco::AssertionViolationException;


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


void GeofenceTest::testPolygonalBoundary()
{
	// A position on the boundary is considered outside the geofence. The
	// polygon is the cartesian projection of the given coordinates, so only a
	// vertex can be named exactly: a position given in degrees between two
	// vertices projects beside the straight edge, not onto it.
	const std::vector<LatLon> square
	{
		LatLon::fromDegrees(46.59, 14.03),
		LatLon::fromDegrees(46.59, 14.05),
		LatLon::fromDegrees(46.61, 14.05),
		LatLon::fromDegrees(46.61, 14.03)
	};

	PolygonalGeofence fence(square);

	for (const auto& vertex: square)
	{
		assertFalse (fence.isInside(vertex));
	}

	// The center of the square is inside. The midpoint of the southern edge,
	// given in degrees, projects about 50 mm south of the straight edge
	// between the two southern vertices, hence outside.
	assertTrue (fence.isInside(LatLon::fromDegrees(46.60, 14.04)));
	assertFalse (fence.isInside(LatLon::fromDegrees(46.59, 14.04)));
}


void GeofenceTest::testPolygonalDegenerate()
{
	// A polygon needs at least three coordinates; fewer are rejected.
	const std::vector<std::vector<LatLon>> degenerate
	{
		{},
		{ LatLon::fromDegrees(46.60, 14.04) },
		{ LatLon::fromDegrees(46.60, 14.04), LatLon::fromDegrees(46.61, 14.05) }
	};

	for (const auto& coordinates: degenerate)
	{
		try
		{
			PolygonalGeofence fence(coordinates);
			fail ("a polygon with fewer than three coordinates must be rejected");
		}
		catch (AssertionViolationException&)
		{
		}
	}
}


void GeofenceTest::testPolygonalConcave()
{
	// An L-shaped polygon, with its arms to the south and to the west. The
	// notch in the northeast is within the bounding box of the polygon, but
	// outside the polygon itself.
	const std::vector<LatLon> lShape
	{
		LatLon::fromDegrees(46.60, 14.04),
		LatLon::fromDegrees(46.60, 14.06),
		LatLon::fromDegrees(46.61, 14.06),
		LatLon::fromDegrees(46.61, 14.05),
		LatLon::fromDegrees(46.62, 14.05),
		LatLon::fromDegrees(46.62, 14.04)
	};

	PolygonalGeofence fence(lShape);

	assertFalse (fence.isInside(LatLon::fromDegrees(46.615, 14.055)));
	assertTrue (fence.isInside(LatLon::fromDegrees(46.605, 14.055)));
	assertTrue (fence.isInside(LatLon::fromDegrees(46.615, 14.045)));
	assertFalse (fence.isInside(LatLon::fromDegrees(46.630, 14.070)));
}


void GeofenceTest::testCircularBoundary()
{
	// A circular geofence covers the positions closer to its center than the
	// radius. As with the polygonal geofence, a position on the boundary, that
	// is one exactly at the radius distance, counts as outside.
	const LatLon center = LatLon::fromDegrees(46.617527564222684, 14.041860704081785);
	const LatLon onBoundary = center.greatCircleEndPosition(
		Angle::fromDegrees(45), Angle::fromRadians(500.0/LatLon::EARTH_MEAN_RADIUS));
	const double radius = center.greatCircleDistanceTo(onBoundary).radians()*LatLon::EARTH_MEAN_RADIUS;

	// greatCircleEndPosition() and greatCircleDistanceTo() are inverse to within
	// rounding, so the distance measured back is the 500 m the position was
	// placed at.
	assertEqualDelta (500.0, radius, 0.001);

	CircularGeofence atRadius(center, radius);
	assertFalse (atRadius.isInside(onBoundary));

	CircularGeofence oneMeterWider(center, radius + 1.0);
	assertTrue (oneMeterWider.isInside(onBoundary));

	// The radius must be positive; a circle without an area is rejected.
	try
	{
		CircularGeofence degenerate(center, 0.0);
		fail ("a circular geofence with a zero radius must be rejected");
	}
	catch (AssertionViolationException&)
	{
	}
}


void GeofenceTest::testGeofenceBase()
{
	const LatLon center = LatLon::fromDegrees(46.60, 14.04);

	CircularGeofence circular(center, 500);

	const std::vector<LatLon> square
	{
		LatLon::fromDegrees(46.59, 14.03),
		LatLon::fromDegrees(46.59, 14.05),
		LatLon::fromDegrees(46.61, 14.05),
		LatLon::fromDegrees(46.61, 14.03)
	};
	PolygonalGeofence polygonal(square);

	// A position about 70 m from the center is inside both geofences, one
	// about 11 km away is outside both. Both are tested through the base class.
	const LatLon inside = LatLon::fromDegrees(46.6005, 14.0405);
	const LatLon outside = LatLon::fromDegrees(46.70, 14.20);

	const Geofence* fences[] = { &circular, &polygonal };
	for (const Geofence* pFence: fences)
	{
		assertTrue (pFence->isInside(inside));
		assertFalse (pFence->isInside(outside));
	}
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
	CppUnit_addTest(pSuite, GeofenceTest, testPolygonalBoundary);
	CppUnit_addTest(pSuite, GeofenceTest, testPolygonalDegenerate);
	CppUnit_addTest(pSuite, GeofenceTest, testPolygonalConcave);
	CppUnit_addTest(pSuite, GeofenceTest, testCircularBoundary);
	CppUnit_addTest(pSuite, GeofenceTest, testGeofenceBase);

	return pSuite;
}
