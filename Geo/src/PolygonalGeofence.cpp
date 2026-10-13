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


namespace
{
	constexpr double BOUNDARY_TOLERANCE = 1e-6;
		/// A position within one micrometer of an edge is considered to be on it.
		/// The cartesian coordinates are in meters, so this is an absolute tolerance.


	bool isOnEdge(const Cartesian& p, const Cartesian& a, const Cartesian& b)
		/// Returns true iff p lies on the line segment from a to b,
		/// within BOUNDARY_TOLERANCE.
	{
		const double dx = b.x - a.x;
		const double dy = b.y - a.y;
		const double length = std::hypot(dx, dy);
		if (length <= BOUNDARY_TOLERANCE)
		{
			return std::hypot(p.x - a.x, p.y - a.y) <= BOUNDARY_TOLERANCE;
		}

		// Distance of p from the infinite line through a and b.
		if (std::abs(dx*(p.y - a.y) - dy*(p.x - a.x))/length > BOUNDARY_TOLERANCE) return false;

		// Position of the projection of p on the segment, as a fraction of its length.
		const double t = (dx*(p.x - a.x) + dy*(p.y - a.y))/(length*length);
		const double slack = BOUNDARY_TOLERANCE/length;
		return t >= -slack && t <= 1.0 + slack;
	}
}


bool PolygonalGeofence::isInside(const LatLon& position) const
{
	// Even-odd rule: count how often the ray that runs from the position in the
	// direction of increasing x crosses an edge of the polygon. An odd number of
	// crossings means the position is inside.
	//
	// An edge is considered for a crossing when exactly one of its endpoints lies
	// above the ray. That half-open condition counts a crossing through a vertex
	// for one of the two adjacent edges only, and never counts an edge that is
	// parallel to the ray, so neither case needs to be handled separately.

	const Cartesian p = position.toCartesian(_reference);
	const std::size_t n = _coordinates.size();

	bool inside = false;
	for (std::size_t i = 0, j = n - 1; i < n; j = i++)
	{
		const Cartesian& a = _coordinates[j];
		const Cartesian& b = _coordinates[i];

		if (isOnEdge(p, a, b)) return false;

		if ((b.y > p.y) != (a.y > p.y))
		{
			const double x = b.x + (p.y - b.y)*(a.x - b.x)/(a.y - b.y);
			if (p.x < x) inside = !inside;
		}
	}

	return inside;
}

} } // namespace Poco::Geo
