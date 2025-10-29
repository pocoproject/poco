//
// Geofence.h
//
// Library: Geo
// Package: Geofence
// Module:  Geofence
//
// Definition of the Geofence class.
//
// Copyright (c) 2025, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Geo_Geofence_INCLUDED
#define Geo_Geofence_INCLUDED


#include "Poco/Geo/LatLon.h"


namespace Poco {
namespace Geo {


class Geo_API Geofence
	/// Base class for different kindes of geofences.
{
public:
	virtual ~Geofence() = default;

	virtual bool isInside(const LatLon& position) const = 0;
		/// Returns true iff the given position is
		/// inside of the geofence, otherwise false.
};


} } // namespace Poco::Geo


#endif // Geo_Geofence_INCLUDED
