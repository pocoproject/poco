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


namespace Poco {
namespace Geo {


class Geo_API PolygonalGeofence: public Geofence
	/// A geofence defined by a polygon.
	///
	/// A point that falls on the geofence boundary is considered
	/// outside the geofence.
	///
	/// Internally, all coordinates (polygon and point to test)
	/// are converted to cartesian, then the ray-casting algorithm
	/// is used to check whether the point is inside the polygon.
{
public:
	PolygonalGeofence(const std::vector<LatLon>& coordinates);
		/// Creates a polygon-shaped geofence with the 
		/// given polygon coordinates.

	// Geofence
	bool isInside(const LatLon& position) const;

private:
	LatLon _reference;
	std::vector<Cartesian> _coordinates;
};


} } // namespace Poco::Geo


#endif // Geo_PolygonalGeofence_INCLUDED
