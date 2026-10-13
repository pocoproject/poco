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
#include <limits>
#include <vector>


using namespace Poco::Geo;
using Poco::AssertionViolationException;


namespace
{
	std::vector<LatLon> squareCoordinates(double southLatitude, double northLatitude)
		/// Returns the coordinates of a square between the given latitudes and the
		/// meridians 14.03 and 14.05 degrees east.
	{
		return
		{
			LatLon::fromDegrees(southLatitude, 14.03),
			LatLon::fromDegrees(southLatitude, 14.05),
			LatLon::fromDegrees(northLatitude, 14.05),
			LatLon::fromDegrees(northLatitude, 14.03)
		};
	}
}


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
	// The last vertex repeats the first, so the polygon also carries a closing edge
	// of zero length.
	const std::vector<LatLon> polygon
	{
		LatLon::fromDegrees(46.61601559225525, 14.033344303704354),
		LatLon::fromDegrees(46.60568737905788, 14.036256466299108),
		LatLon::fromDegrees(46.608478982249466, 14.052510397056068),
		LatLon::fromDegrees(46.60945600937768, 14.05257812176876),
		LatLon::fromDegrees(46.60936295993457, 14.045128403504435),
		LatLon::fromDegrees(46.61387567381189, 14.042622589178762),
		LatLon::fromDegrees(46.61573647725416, 14.049327335616255),
		LatLon::fromDegrees(46.6195509244188, 14.047363318983628),
		LatLon::fromDegrees(46.61601559225525, 14.033344303704354)
	};

	PolygonalGeofence fence(polygon);

	assertTrue (fence.isInside(LatLon::fromDegrees(46.618946212913244, 14.046956970714518)));
	assertTrue (fence.isInside(LatLon::fromDegrees(46.614480441943925, 14.035172870914437)));
	assertTrue (fence.isInside(LatLon::fromDegrees(46.611735670807974, 14.042487139756247)));
	assertTrue (fence.isInside(LatLon::fromDegrees(46.608572033211146, 14.048176015521733)));

	assertFalse (fence.isInside(LatLon::fromDegrees(46.618853179774305, 14.050410931000897)));
	assertFalse (fence.isInside(LatLon::fromDegrees(46.61215437370177, 14.046821521292003)));
	assertFalse (fence.isInside(LatLon::fromDegrees(46.60531515442932, 14.044451156388874)));
}


void GeofenceTest::testPolygonalBoundary()
{
	// Containment follows the even-odd rule in a local projection, where the edges are
	// straight chords between the projected vertices and not parallels or great
	// circles. A position exactly on a vertex is outside; of two polygons that share
	// an edge, a position on that edge is inside exactly one.
	const std::vector<LatLon> north = squareCoordinates(46.59, 46.61);
	const std::vector<LatLon> south = squareCoordinates(46.57, 46.59);

	PolygonalGeofence northFence(north);
	PolygonalGeofence southFence(south);

	for (const auto& vertex: north)
	{
		assertFalse (northFence.isInside(vertex));
	}

	assertTrue (northFence.isInside(LatLon::fromDegrees(46.60, 14.04)));
	assertFalse (northFence.isInside(LatLon::fromDegrees(46.70, 14.20)));

	// The two squares share the edge between the vertices at 46.59 degrees north. A
	// position on the chord between their projections, and not the midpoint in degrees
	// which lies about 50 mm off it, is obtained through the inverse conversion.
	const LatLon edgeReference = LatLon::fromDegrees(46.59, 14.04);
	const Cartesian west = LatLon::fromDegrees(46.59, 14.03).toCartesian(edgeReference);
	const Cartesian east = LatLon::fromDegrees(46.59, 14.05).toCartesian(edgeReference);
	const LatLon onEdge = LatLon::fromCartesian(edgeReference,
		Cartesian{(west.x + east.x)/2, (west.y + east.y)/2});

	assertTrue (northFence.isInside(onEdge) != southFence.isInside(onEdge));
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


void GeofenceTest::testPolygonalAcrossAntimeridian()
{
	// Averaging the raw longitudes places the reference of the projection on the
	// opposite side of the globe, where the polygon folds through the reference
	// meridian and covers a quarter of the globe instead of the box.
	const std::vector<LatLon> box
	{
		LatLon::fromDegrees(-1, 179),
		LatLon::fromDegrees(-1, -179),
		LatLon::fromDegrees(1, -179),
		LatLon::fromDegrees(1, 179)
	};

	PolygonalGeofence fence(box);

	assertTrue (fence.isInside(LatLon::fromDegrees(0, 180)));
	assertTrue (fence.isInside(LatLon::fromDegrees(0, -179.5)));
	assertTrue (fence.isInside(LatLon::fromDegrees(0, 179.5)));
	assertFalse (fence.isInside(LatLon::fromDegrees(0, 0)));
}


void GeofenceTest::testPolygonalOutOfRangePosition()
{
	// A latitude beyond a pole is outside the domain of the projection, which answers
	// with NaN. An answer of the origin would put the position on the polygon's own
	// reference, inside it.
	PolygonalGeofence fence(squareCoordinates(46.59, 46.61));

	assertFalse (fence.isInside(LatLon::fromDegrees(95, 10)));
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


void GeofenceTest::testNaNPositionOutside()
{
	// A NaN coordinate must neither become a zero distance nor the origin of the
	// projection, each of which lies inside the geofence.
	const LatLon undefined = LatLon::fromDegrees(std::numeric_limits<double>::quiet_NaN(),
		std::numeric_limits<double>::quiet_NaN());

	CircularGeofence circular(LatLon::fromDegrees(46.60, 14.04), 500);
	assertFalse (circular.isInside(undefined));

	PolygonalGeofence polygonal(squareCoordinates(46.59, 46.61));
	assertFalse (polygonal.isInside(undefined));
}


void GeofenceTest::testGeofenceBase()
{
	const LatLon center = LatLon::fromDegrees(46.60, 14.04);

	CircularGeofence circular(center, 500);

	PolygonalGeofence polygonal(squareCoordinates(46.59, 46.61));

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
	CppUnit_addTest(pSuite, GeofenceTest, testPolygonalAcrossAntimeridian);
	CppUnit_addTest(pSuite, GeofenceTest, testPolygonalOutOfRangePosition);
	CppUnit_addTest(pSuite, GeofenceTest, testCircularBoundary);
	CppUnit_addTest(pSuite, GeofenceTest, testNaNPositionOutside);
	CppUnit_addTest(pSuite, GeofenceTest, testGeofenceBase);

	return pSuite;
}
