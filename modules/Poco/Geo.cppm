//
// Geo.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_GEO
#include "Poco/Geo/Angle.h"
#include "Poco/Geo/CircularGeofence.h"
#include "Poco/Geo/Geofence.h"
#include "Poco/Geo/Geo.h"
#include "Poco/Geo/LatLon.h"
#include "Poco/Geo/PolygonalGeofence.h"
#endif

export module Poco:Geo;

export namespace Poco::Geo {
	#ifdef ENABLE_GEO
	using Poco::Geo::Angle;
	using Poco::Geo::Cartesian;
	using Poco::Geo::CircularGeofence;
	using Poco::Geo::Geofence;
	using Poco::Geo::LatLon;
	using Poco::Geo::PolygonalGeofence;

	using Poco::Geo::swap;
	#endif
}
