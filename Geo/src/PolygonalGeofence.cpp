//
// PolygonalGeofence.cpp
//
// Library: Geo
// Package: Geofence
// Module:  PolygonalGeofence
//
// Copyright (c) 2025, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Geo/PolygonalGeofence.h"
#include <algorithm>
#include <cmath>


namespace Poco {
namespace Geo {


PolygonalGeofence::PolygonalGeofence(const std::vector<LatLon>& coordinates)
{
	poco_assert (coordinates.size() >= 3);

	double cLat = 0;
	double cLon = 0;
	for (const auto& c: coordinates)
	{
		cLat += c.latitude().degrees();
		cLon += c.longitude().degrees();
	}
	cLat /= coordinates.size();
	cLon /= coordinates.size();
	_reference.assign(
		Angle::fromDegreesLatitude(cLat),
		Angle::fromDegreesLongitude(cLon)
	);
	for (const auto& c: coordinates)
	{
		_coordinates.push_back(c.toCartesian(_reference));
	}
}


bool PolygonalGeofence::isInside(const LatLon& position) const
{
	// Adapted from: https://paulbourke.net/geometry/polygonmesh/ and
	// https://paulbourke.net/geometry/polygonmesh/InsidePolygonWithBounds.cpp

	const Cartesian p = position.toCartesian(_reference);

	// cross points count of x
    int count = 0;

    // neighbour bound vertices
    Cartesian p1;
	Cartesian p2;

    // left vertex
    p1 = _coordinates[0];

    // check all rays
	std::size_t n = _coordinates.size();
    for (std::size_t i = 1; i <= n; ++i)
    {
        // point is an vertex
        if (p.x == p1.x && p.y == p1.y) return false;

        // right vertex
        p2 = _coordinates[i % n];

        // ray is outside of our interests
        if (p.y < std::min(p1.y, p2.y) || p.y > std::max(p1.y, p2.y))
        {
            // next ray left point
            p1 = p2; continue;
        }

        // ray is crossing over by the algorithm (common part of)
        if (p.y > std::min(p1.y, p2.y) && p.y < std::max(p1.y, p2.y))
        {
            // x is before of ray
            if (p.x <= std::max(p1.x, p2.x))
            {
                // overlies on a horizontal ray
                if (p1.y == p2.y && p.x >= std::min(p1.x, p2.x)) return false;

                // ray is vertical
                if (p1.x == p2.x)
                {
                    // overlies on a ray
                    if (p1.x == p.x) return false;
                    // before ray
                    else ++count;
                }
                else // cross point on the left side
                {
                    // cross point of x
                    double xinters = (p.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;

                    // overlies on a ray
                    if (std::abs(p.x - xinters) < __DBL_EPSILON__) return false;

                    // before ray
                    if (p.x < xinters) ++count;
                }
            }
        }
        else // special case when ray is crossing through the vertex
        {
            // p crossing over p2
            if (p.y == p2.y && p.x <= p2.x)
            {
                // next vertex
                const Cartesian& p3 = _coordinates[(i+1) % n];

                // p.y lies between p1.y & p3.y
                if (p.y >= std::min(p1.y, p3.y) && p.y <= std::max(p1.y, p3.y))
                {
                    ++count;
                }
                else
                {
                    count += 2;
                }
            }
        }
        // next ray left point
        p1 = p2;
    }

	return count % 2 != 0;
}


} } // namespace Poco::Geo
