//
// CircularGeofence.h
//
// Library: Geo
// Package: Geofence
// Module:  CircularGeofence
//
// Definition of the CircularGeofence class.
//
// Copyright (c) 2025, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Geo_CircularGeofence_INCLUDED
#define Geo_CircularGeofence_INCLUDED


#include "Poco/Geo/Geofence.h"


namespace Poco::Geo {


class Geo_API CircularGeofence: public Geofence
	/// A circular geofence around a given center point
	/// with a given radius.
{
public:
	CircularGeofence(const LatLon& center, double radius);
		/// Creates a circular geofence with the given center
		/// and radius (in meters).

	// Geofence
	[[nodiscard]] bool isInside(const LatLon& position) const override;

private:
	LatLon _center;
	double _radius;
};


} // namespace Poco::Geo


#endif // Geo_CircularGeofence_INCLUDED
