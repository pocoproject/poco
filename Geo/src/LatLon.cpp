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
#include <limits>


using Poco::FPEnvironment;


namespace Poco {
namespace Geo {


const LatLon LatLon::ZERO; 


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


LatLon LatLon::fromCartesian(const LatLon& reference, const Cartesian& xy)
{
	return fromCartesian(reference, xy.x, xy.y);
}


LatLon LatLon::fromCartesian(const LatLon& reference, double x, double y)
{
    constexpr double EPSILON10{1.0e-4};
    const int32_t signLon{(x < 0) ? -1 : 1};
    const int32_t signLat{(y < 0) ? -1 : 1};

    double approxLat(reference.latitude().degrees());
	double approxLon(reference.longitude().degrees());
    Cartesian cartesianResult{toCartesian(reference, reference)};

    double dPrev{(std::numeric_limits<double>::max)()};
    double d = std::abs(y - cartesianResult.y);
    double incLat{1e-6};
    while ((d < dPrev) && (d > EPSILON10)) 
	{
        incLat = std::max(1e-6 * d, static_cast<double>(1e-9));
        approxLat += signLat * incLat;
        cartesianResult = toCartesian(reference, LatLon::fromDegrees(approxLat, approxLon));
        dPrev = d;
        d = std::abs(y - cartesianResult.y);
    }

    dPrev = (std::numeric_limits<double>::max)();
    d = std::abs(x - cartesianResult.x);
    double incLon{1e-6};
    while ((d < dPrev) && (d > EPSILON10)) 
	{
        incLon = std::max(1e-6 * d, static_cast<double>(1e-9));
        approxLon += signLon * incLon;
        cartesianResult = toCartesian(reference, LatLon::fromDegrees(approxLat, approxLon));
        dPrev = d;
        d = std::abs(x - cartesianResult.x);
    }

    return LatLon::fromDegrees(approxLat, approxLon);
}


Cartesian LatLon::toCartesian(const LatLon& reference)
{
	return toCartesian(reference, *this);
}


Cartesian LatLon::toCartesian(const LatLon& reference, const LatLon& latLon)
{
#ifndef M_PI
	constexpr double M_PI = 3.141592653589793;
#endif
	constexpr double HALF_PI{M_PI / 2.0};
	constexpr double EPSILON10{1.0e-10};
	constexpr double EPSILON12{1.0e-12};

	constexpr double FLATTENING{1.0 / 298.257223563};
	constexpr double SQUARED_ECCENTRICITY{2.0 * FLATTENING - FLATTENING * FLATTENING};

	constexpr double C00{1.0};
	constexpr double C02{0.25};
	constexpr double C04{0.046875};
	constexpr double C06{0.01953125};
	constexpr double C08{0.01068115234375};
	constexpr double C22{0.75};
	constexpr double C44{0.46875};
	constexpr double C46{0.01302083333333333333};
	constexpr double C48{0.00712076822916666666};
	constexpr double C66{0.36458333333333333333};
	constexpr double C68{0.00569661458333333333};
	constexpr double C88{0.3076171875};

	constexpr double R0{C00 - SQUARED_ECCENTRICITY * (C02 + SQUARED_ECCENTRICITY * (C04 + SQUARED_ECCENTRICITY * (C06 + SQUARED_ECCENTRICITY * C08)))};
	constexpr double R1{SQUARED_ECCENTRICITY * (C22 - SQUARED_ECCENTRICITY * (C04 + SQUARED_ECCENTRICITY * (C06 + SQUARED_ECCENTRICITY * C08)))};
	constexpr double R2T{SQUARED_ECCENTRICITY * SQUARED_ECCENTRICITY};
	constexpr double R2{R2T * (C44 - SQUARED_ECCENTRICITY * (C46 + SQUARED_ECCENTRICITY * C48))};
	constexpr double R3T{R2T * SQUARED_ECCENTRICITY};
	constexpr double R3{R3T * (C66 - SQUARED_ECCENTRICITY * C68)};
	constexpr double R4{R3T * SQUARED_ECCENTRICITY * C88};

	auto mlfn = [&](const double &lat) 
	{
		const double sin_phi{std::sin(lat)};
		const double cos_phi{std::cos(lat) * sin_phi};
		const double squared_sin_phi = sin_phi * sin_phi;
		return (R0 * lat - cos_phi * (R1 + squared_sin_phi * (R2 + squared_sin_phi * (R3 + squared_sin_phi * R4))));
	};

	const double ML0{mlfn(reference.latitude().radians())};

	auto msfn = [&](double sinPhi, double cosPhi, double es) 
	{
		return (cosPhi / std::sqrt(1.0 - es * sinPhi * sinPhi));
	};

	auto project = [&](double lat, double lon) 
	{
		Cartesian xy{lon, -1.0 * ML0};
		if (!(std::abs(lat) < EPSILON10)) 
		{
			const double ms{(std::abs(std::sin(lat)) > EPSILON10) ? msfn(std::sin(lat), std::cos(lat), SQUARED_ECCENTRICITY) / std::sin(lat) : 0.0};
			xy.x = ms * std::sin(lon *= std::sin(lat));
			xy.y = (mlfn(lat) - ML0) + ms * (1.0 - std::cos(lon));
		}
		return xy;
	};

	auto fwd = [&](double lat, double lon) 
	{
		const double D = std::abs(lat) - HALF_PI;
		if ((D > EPSILON12) || (std::abs(lon) > 10.0)) 
		{
			return Cartesian{0.0, 0.0};
		}
		if (std::abs(D) < EPSILON12) 
		{
			lat = (lat < 0.0) ? -1.0 * HALF_PI : HALF_PI;
		}
		lon -= reference.longitude().radians();
		const auto projected{project(lat, lon)};
		return Cartesian{EARTH_EQUATORIAL_RADIUS * projected.x, EARTH_EQUATORIAL_RADIUS * projected.y};
	};

	return fwd(latLon.latitude().radians(), latLon.longitude().radians());
}


} } // namespace Poco::Geo
