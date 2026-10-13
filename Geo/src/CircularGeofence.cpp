//
// CircularGeofence.cpp
//
// Library: Geo
// Package: Geofence
// Module:  CircularGeofence
//
// Copyright (c) 2025, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Geo/CircularGeofence.h"


namespace Poco::Geo {


CircularGeofence::CircularGeofence(const LatLon& center, double radius):
	_center(center),
	_radius(radius)
{
	poco_assert (_radius > 0);
}


bool CircularGeofence::isInside(const LatLon& position) const
{
	Angle a = _center.greatCircleDistanceTo(position);
	double distance = a.radians()*LatLon::EARTH_MEAN_RADIUS;
	return distance < _radius;
}


} // namespace Poco::Geo
