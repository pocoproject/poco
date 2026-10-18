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
	/// All coordinates, the polygon's and the position to test, are converted to
	/// cartesian coordinates in a local projection, where the even-odd rule decides
	/// containment. The edges are straight lines in that projection, so an edge given
	/// as a parallel or as a great circle is approximated, with an error that grows
	/// with the length of the edge. A position exactly on a vertex is outside the
	/// geofence; the rest of the boundary is half-open, so of two polygons sharing an
	/// edge a position on that edge is inside exactly one of them. That holds only for
	/// polygons that project against the same reference; each polygon derives its
	/// reference from its own vertices, so a shared edge becomes two slightly different
	/// chords, with a narrow band between them that is outside both polygons.
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
