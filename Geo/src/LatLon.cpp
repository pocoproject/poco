//
// LatLon.cpp
//
// Library: Geo
// Package: Geo
// Module:  LatLon
//
// Copyright (c) 2010-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Geo/LatLon.h"
#include "Poco/FPEnvironment.h"
#include <cmath>


using Poco::FPEnvironment;


namespace Poco {
namespace Geo {


const LatLon LatLon::ZERO; 
const double LatLon::EARTH_EQUATORIAL_RADIUS(6378137.0); 
const double LatLon::EARTH_POLAR_RADIUS(6356752.3); 
const double LatLon::EARTH_MEAN_RADIUS(6371009.0);


LatLon::LatLon()
{
}

	
LatLon::LatLon(const Angle& latitude, const Angle& longitude):
	_latitude(latitude),
	_longitude(longitude)
{
}

	
LatLon::LatLon(const LatLon& other):
	_latitude(other._latitude),
	_longitude(other._longitude)
{
}

	
LatLon::~LatLon()
{
}


LatLon& LatLon::operator = (const LatLon& other)
{
	LatLon tmp(other);
	swap(tmp);
	return *this;
}


void LatLon::assign(const LatLon& other)
{
	LatLon tmp(other);
	swap(tmp);
}

	
void LatLon::assign(const Angle& latitude, const Angle& longitude)
{
	_latitude  = latitude;
	_longitude = longitude;
}		

	
void LatLon::swap(LatLon& other)
{
	_latitude.swap(other._latitude);
	_longitude.swap(other._longitude);
}


Angle LatLon::greatCircleDistanceTo(const LatLon& latLon) const
{
	double lat1 = _latitude.radians();
	double lon1 = _longitude.radians();
	double lat2 = latLon._latitude.radians();
	double lon2 = latLon._longitude.radians();
	
	if (lat1 == lat2 && lon1 == lon2)
		return Angle();
	
	// Taken from "Map Projections - A Working Manual", page 30, equation 5-3a.
	// The traditional d=2*asin(a) form has been replaced with d=2*atan2(sqrt(a), sqrt(1-a))
	// to reduce rounding errors with large distances.
	double a = std::sin((lat2 - lat1) / 2.0) * std::sin((lat2 - lat1) / 2.0)
			+ std::cos(lat1) * std::cos(lat2) * std::sin((lon2 - lon1) / 2.0) * std::sin((lon2 - lon1) / 2.0);
	double distanceRadians = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
	
	return FPEnvironment::isNaN(distanceRadians) ? Angle() : Angle::fromRadians(distanceRadians);
}


Angle LatLon::greatCircleAzimuthTo(const LatLon& latLon) const
{
	double lat1 = _latitude.radians();
	double lon1 = _longitude.radians();
	double lat2 = latLon._latitude.radians();
	double lon2 = latLon._longitude.radians();
	
	if (lat1 == lat2 && lon1 == lon2)
		return Angle();
	
	if (lon1 == lon2)
		return lat1 > lat2 ? Angle::POS180 : Angle();
	
	// Taken from "Map Projections - A Working Manual", page 30, equation 5-4b.
	// The atan2() function is used in place of the traditional atan(y/x) to simplify the case when x==0.
	double y = std::cos(lat2) * std::sin(lon2 - lon1);
	double x = std::cos(lat1) * std::sin(lat2) - std::sin(lat1) * std::cos(lat2) * std::cos(lon2 - lon1);
	double azimuthRadians = std::atan2(y, x);
	
	return FPEnvironment::isNaN(azimuthRadians) ? Angle() : Angle::fromRadians(azimuthRadians);
}


LatLon LatLon::greatCircleEndPosition(const Angle& greatCircleAzimuth, const Angle& arcDistance) const
{
	double lat = _latitude.radians();
	double lon = _longitude.radians();
	double azimuth = greatCircleAzimuth.radians();
	double distance = arcDistance.radians();
	
	if (distance == 0)
		return *this;
	
	// Taken from "Map Projections - A Working Manual", page 31, equation 5-5 and 5-6.
	double endLatRadians = std::asin(std::sin(lat) * std::cos(distance)
			+ std::cos(lat) * std::sin(distance) * std::cos(azimuth));
	double endLonRadians = lon + std::atan2(
			std::sin(distance) * std::sin(azimuth),
			std::cos(lat) * std::cos(distance) - std::sin(lat) * std::sin(distance) * std::cos(azimuth));
	
	if (FPEnvironment::isNaN(endLatRadians) || FPEnvironment::isNaN(endLonRadians))
		return *this;
	
	return LatLon(Angle::fromRadians(endLatRadians).normalizedLatitude(), Angle::fromRadians(endLonRadians).normalizedLongitude());
}


Angle LatLon::rhumbDistanceTo(const LatLon& latLon) const
{
	double lat1 = _latitude.radians();
	double lon1 = _longitude.radians();
	double lat2 = latLon._latitude.radians();
	double lon2 = latLon._longitude.radians();
	
	if (lat1 == lat2 && lon1 == lon2)
		return Angle();
	
	// Taken from http://www.movable-type.co.uk/scripts/latlong.html
	double dLat = lat2 - lat1;
	double dLon = lon2 - lon1;
	double dPhi = std::log(std::tan(lat2 / 2.0 + Angle::PI / 4.0) / std::tan(lat1 / 2.0 + Angle::PI / 4.0));
	double q = dLat / dPhi;
	if (FPEnvironment::isNaN(dPhi) || FPEnvironment::isNaN(q))
	{
		q = std::cos(lat1);
	}
	// If lonChange over 180 take shorter rhumb across 180 meridian.
	if (std::fabs(dLon) > Angle::PI)
	{
		dLon = dLon > 0 ? -(2 * Angle::PI - dLon) : (2 * Angle::PI + dLon);
	}
	
	double distanceRadians = std::sqrt(dLat * dLat + q * q * dLon * dLon);
	
	return FPEnvironment::isNaN(distanceRadians) ? Angle() : Angle::fromRadians(distanceRadians);
}


Angle LatLon::rhumbAzimuthTo(const LatLon& latLon) const
{
	double lat1 = _latitude.radians();
	double lon1 = _longitude.radians();
	double lat2 = latLon._latitude.radians();
	double lon2 = latLon._longitude.radians();
	
	if (lat1 == lat2 && lon1 == lon2)
		return Angle();
	
	// Taken from http://www.movable-type.co.uk/scripts/latlong.html
	double dLon = lon2 - lon1;
	double dPhi = std::log(std::tan(lat2 / 2.0 + Angle::PI / 4.0) / std::tan(lat1 / 2.0 + Angle::PI / 4.0));
	// If lonChange over 180 take shorter rhumb across 180 meridian.
	if (std::fabs(dLon) > Angle::PI)
	{
		dLon = dLon > 0 ? -(2 * Angle::PI - dLon) : (2 * Angle::PI + dLon);
	}

	double azimuthRadians = std::atan2(dLon, dPhi);

	return FPEnvironment::isNaN(azimuthRadians) ? Angle() : Angle::fromRadians(azimuthRadians);
}


LatLon LatLon::rhumbEndPosition(const Angle& rhumbAzimuth, const Angle& arcDistance) const
{
	double lat1 = _latitude.radians();
	double lon1 = _longitude.radians();
	double azimuth = rhumbAzimuth.radians();
	double distance = arcDistance.radians();
	
	if (distance == 0)
		return *this;
	
	// Taken from http://www.movable-type.co.uk/scripts/latlong.html
	double lat2 = lat1 + distance * std::cos(azimuth);
	double dPhi = std::log(std::tan(lat2 / 2.0 + Angle::PI / 4.0) / std::tan(lat1 / 2.0 + Angle::PI / 4.0));
	double q = (lat2 - lat1) / dPhi;
	if (FPEnvironment::isNaN(dPhi) || FPEnvironment::isNaN(q))
	{
		q = std::cos(lat1);
	}
	double dLon = distance * std::sin(azimuth) / q;
	// Handle latitude passing over either pole.
	if (std::fabs(lat2) > Angle::PI / 2.0)
	{
		lat2 = lat2 > 0 ? Angle::PI - lat2 : -Angle::PI - lat2;
	}
	double lon2 = std::fmod((lon1 + dLon + Angle::PI), (2 * Angle::PI)) - Angle::PI;
	
	if (FPEnvironment::isNaN(lat2) || FPEnvironment::isNaN(lon2))
		return *this;
	
	return fromRadians(lat2, lon2);
}


bool LatLon::equals(const LatLon& other, double delta) const
{
	double dLat = std::fabs(_latitude.degrees() - other._latitude.degrees());
	double dLon = std::fabs(_longitude.degrees() - other._longitude.degrees());
	return dLat <= delta && dLon <= delta;
}


} } // namespace Poco::Geo
