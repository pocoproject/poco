//
// PolygonalGeofence.h
//
// Library: Geo
// Package: Geofence
// Module:  PolygonalGeofence
//
// Definition of the PolygonalGeofence class.
//
// Copyright (c) 2025, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Geo_PolygonalGeofence_INCLUDED
#define Geo_PolygonalGeofence_INCLUDED


#include "Poco/Geo/Geofence.h"
#include <vector>


namespace Poco::Geo {


class Geo_API PolygonalGeofence: public Geofence
	/// A geofence defined by a polygon.
	///
	/// A point that falls on the geofence boundary is considered
	/// outside the geofence. A point within one micrometer of an
	/// edge is taken to be on the boundary.
	///
	/// Internally, all coordinates (polygon and point to test)
	/// are converted to cartesian, then the even-odd rule is used
	/// to check whether the point is inside the polygon.
{
public:
	explicit PolygonalGeofence(const std::vector<LatLon>& coordinates);
		/// Creates a polygon-shaped geofence with the 
		/// given polygon coordinates.

	// Geofence
	[[nodiscard]] bool isInside(const LatLon& position) const override;

private:
	LatLon _reference;
	std::vector<Cartesian> _coordinates;
};


} // namespace Poco::Geo


#endif // Geo_PolygonalGeofence_INCLUDED
