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
#include <cstddef>


namespace Poco::Geo {


PolygonalGeofence::PolygonalGeofence(const std::vector<LatLon>& coordinates)
{
	poco_assert (coordinates.size() >= 3);

	// Averaging the raw longitudes places the reference of a polygon that straddles
	// the antimeridian on the opposite side of the globe, where the projection folds
	// the polygon through the reference meridian. Averaging the offsets from the
	// first vertex keeps the reference within the polygon.
	const double lon0 = coordinates.front().longitude().degrees();
	double cLat = 0;
	double cLon = 0;
	for (const auto& c: coordinates)
	{
		cLat += c.latitude().degrees();
		cLon += Angle::normalizedDegreesLongitude(c.longitude().degrees() - lon0);
	}
	cLat /= coordinates.size();
	cLon /= coordinates.size();
	_reference.assign(
		Angle::fromDegreesLatitude(cLat),
		Angle::fromDegreesLongitude(Angle::normalizedDegreesLongitude(lon0 + cLon))
	);
	for (const auto& c: coordinates)
	{
		_coordinates.push_back(c.toCartesian(_reference));
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
	//
	// NaN coordinates, from a position outside the projection's domain, compare false
	// everywhere and so read as outside.

	const Cartesian p = position.toCartesian(_reference);
	const std::size_t n = _coordinates.size();

	bool inside = false;
	std::size_t j = n - 1;
	for (std::size_t i = 0; i < n; ++i)
	{
		const Cartesian& a = _coordinates[j];
		const Cartesian& b = _coordinates[i];

		// Projecting the same position against the same reference yields the same bits,
		// so a position on a vertex is recognized exactly.
		if (p.x == b.x && p.y == b.y) return false;

		if ((b.y > p.y) != (a.y > p.y))
		{
			const double x = b.x + (p.y - b.y)*(a.x - b.x)/(a.y - b.y);
			if (p.x < x) inside = !inside;
		}

		j = i;
	}

	return inside;
}


} // namespace Poco::Geo
