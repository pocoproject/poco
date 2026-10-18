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
#include <algorithm>
#include <cmath>
#include <limits>


using Poco::FPEnvironment;


//
// The rhumb line calculations below are based on the formulas and scripts at
// http://www.movable-type.co.uk/scripts/latlong.html
// Copyright (c) 2002-2022 Chris Veness, MIT license.
//


namespace Poco::Geo {


namespace
{
	// Smallest difference of the stretched latitudes for which a rhumb line's
	// stretch factor can be formed as a quotient. Below it, the difference rounds
	// to zero on a due east or west course and for latitudes a few ulps apart,
	// which makes the quotient infinite.
	constexpr double MIN_DELTA_PHI = 1.0e-12;

	double wrapLongitude(double radians)
		/// Reduces a longitude or a longitude difference to (-PI, PI].
	{
		// std::remainder is exact, but yields [-PI, PI]. The antimeridian is named
		// 180 degrees east, so an exact half turn belongs at the positive end.
		const double wrapped = std::remainder(radians, 2.0*Angle::PI);
		return wrapped == -Angle::PI ? Angle::PI : wrapped;
	}

	bool samePosition(const LatLon& a, const LatLon& b)
		/// Returns true only for two positions that are exactly equal.
		///
		/// The comparison is deliberately exact. Two positions that differ at all,
		/// even by one unit in the last place, have a defined distance and azimuth
		/// and must reach the general formula, and a position with a NaN coordinate
		/// compares unequal so that the NaN propagates.
		///
		/// For greatCircleAzimuthTo() this is not merely a shortcut. The azimuth
		/// formula subtracts two products that cancel only in exact arithmetic, so
		/// for one position against itself it divides a zero by a small negative
		/// residue and yields 180 degrees rather than 0.
	{
		return a.latitude().radians() == b.latitude().radians()
			&& a.longitude().radians() == b.longitude().radians();
	}

	bool inDomain(const LatLon& position)
		/// Returns true if the position can take part in a computation.
		///
		/// The latitude must lie within [-90, 90], because a value beyond a pole
		/// names no point on the globe, and both coordinates must be finite. The
		/// longitude is not bounded: the formulas are periodic in it, so an
		/// unnormalized value names the position it is congruent to.
	{
		return std::isfinite(position.latitude().degrees())
			&& std::isfinite(position.longitude().degrees())
			&& std::abs(position.latitude().degrees()) <= 90.0;
	}

	Angle undefinedAngle()
		/// The result of a computation whose arguments are outside its domain.
	{
		return Angle::fromRadians(std::numeric_limits<double>::quiet_NaN());
	}

	LatLon undefinedPosition()
		/// The result of a computation whose arguments are outside its domain.
	{
		return LatLon(undefinedAngle(), undefinedAngle());
	}
}


const LatLon LatLon::ZERO; 


LatLon::LatLon(const Angle& latitude, const Angle& longitude):
	_latitude(latitude),
	_longitude(longitude)
{
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

	
void LatLon::swap(LatLon& other) noexcept
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
	
	if (!inDomain(*this) || !inDomain(latLon))
		return undefinedAngle();

	if (samePosition(*this, latLon))
		return Angle();
	
	// Taken from "Map Projections - A Working Manual", page 30, equation 5-3a.
	// The traditional d=2*asin(a) form has been replaced with d=2*atan2(sqrt(a), sqrt(1-a))
	// to reduce rounding errors with large distances.
	// The term rounds above 1 for antipodal positions, where the square root of
	// its complement would be NaN.
	double a = std::clamp(std::sin((lat2 - lat1) / 2.0) * std::sin((lat2 - lat1) / 2.0)
			+ std::cos(lat1) * std::cos(lat2) * std::sin((lon2 - lon1) / 2.0) * std::sin((lon2 - lon1) / 2.0),
			0.0, 1.0);
	double distanceRadians = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
	
	return Angle::fromRadians(distanceRadians);
}


Angle LatLon::greatCircleAzimuthTo(const LatLon& latLon) const
{
	double lat1 = _latitude.radians();
	double lon1 = _longitude.radians();
	double lat2 = latLon._latitude.radians();
	double lon2 = latLon._longitude.radians();
	
	if (!inDomain(*this) || !inDomain(latLon))
		return undefinedAngle();

	if (samePosition(*this, latLon))
		return Angle();
	
	// Exact again, as in samePosition(): on the same meridian the course is due
	// north or due south, and any difference at all has a defined azimuth.
	if (lon1 == lon2)
		return lat1 > lat2 ? Angle::POS180 : Angle();
	
	// Taken from "Map Projections - A Working Manual", page 30, equation 5-4b.
	// The atan2() function is used in place of the traditional atan(y/x) to simplify the case when x==0.
	double y = std::cos(lat2) * std::sin(lon2 - lon1);
	double x = std::cos(lat1) * std::sin(lat2) - std::sin(lat1) * std::cos(lat2) * std::cos(lon2 - lon1);
	double azimuthRadians = std::atan2(y, x);
	
	return Angle::fromRadians(azimuthRadians);
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
	// The argument of the arc sine rounds above 1 when the end position is a pole.
	double endLatRadians = std::asin(std::clamp(std::sin(lat) * std::cos(distance)
			+ std::cos(lat) * std::sin(distance) * std::cos(azimuth), -1.0, 1.0));
	double endLonRadians = lon + std::atan2(
			std::sin(distance) * std::sin(azimuth),
			std::cos(lat) * std::cos(distance) - std::sin(lat) * std::sin(distance) * std::cos(azimuth));
	
	if (FPEnvironment::isNaN(endLatRadians) || FPEnvironment::isNaN(endLonRadians))
		return undefinedPosition();
	
	return LatLon(Angle::fromRadians(endLatRadians).normalizedLatitude(), Angle::fromRadians(endLonRadians).normalizedLongitude());
}


Angle LatLon::rhumbDistanceTo(const LatLon& latLon) const
{
	double lat1 = _latitude.radians();
	double lon1 = _longitude.radians();
	double lat2 = latLon._latitude.radians();
	double lon2 = latLon._longitude.radians();
	
	if (!inDomain(*this) || !inDomain(latLon))
		return undefinedAngle();

	if (samePosition(*this, latLon))
		return Angle();
	
	// Taken from http://www.movable-type.co.uk/scripts/latlong.html
	double dLat = lat2 - lat1;
	double dLon = lon2 - lon1;
	double dPhi = std::log(std::tan(lat2 / 2.0 + Angle::PI / 4.0) / std::tan(lat1 / 2.0 + Angle::PI / 4.0));
	double q = std::fabs(dPhi) < MIN_DELTA_PHI ? std::cos(lat1) : dLat / dPhi;
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
	
	return Angle::fromRadians(distanceRadians);
}


Angle LatLon::rhumbAzimuthTo(const LatLon& latLon) const
{
	double lat1 = _latitude.radians();
	double lon1 = _longitude.radians();
	double lat2 = latLon._latitude.radians();
	double lon2 = latLon._longitude.radians();
	
	if (!inDomain(*this) || !inDomain(latLon))
		return undefinedAngle();

	if (samePosition(*this, latLon))
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

	return Angle::fromRadians(azimuthRadians);
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
	double q = std::fabs(dPhi) < MIN_DELTA_PHI ? std::cos(lat1) : (lat2 - lat1) / dPhi;
	if (FPEnvironment::isNaN(dPhi) || FPEnvironment::isNaN(q))
	{
		q = std::cos(lat1);
	}
	double dLon = distance * std::sin(azimuth) / q;
	
	if (FPEnvironment::isNaN(lat2) || FPEnvironment::isNaN(dLon))
		return undefinedPosition();
	
	// Normalizing reflects a latitude that passed over a pole and brings a longitude
	// that crossed the antimeridian back into range.
	return LatLon(Angle::fromRadians(lat2).normalizedLatitude(), Angle::fromRadians(lon1 + dLon).normalizedLongitude());
}


bool LatLon::equals(const LatLon& other, double delta) const
{
	// The angular distance wraps the difference, so that two positions on either
	// side of the antimeridian are as close to each other as they are on a globe.
	return _latitude.angularDistanceTo(other._latitude).degrees() <= delta
		&& _longitude.angularDistanceTo(other._longitude).degrees() <= delta;
}


LatLon LatLon::fromCartesian(const LatLon& reference, const Cartesian& xy)
{
	return fromCartesian(reference, xy.x, xy.y);
}


LatLon LatLon::fromCartesian(const LatLon& reference, double x, double y)
{
	// The projection has no simple closed-form inverse, but it is smooth, so
	// Newton's method on toCartesian() converges in a few steps. The Jacobian is
	// obtained by finite differences, which keeps the forward projection the single
	// definition of the mapping: a correction to it carries into the inverse.
	//
	// The residual is a distance in meters and the step is in degrees, so the
	// Jacobian is what converts between the two.

	// Residual in meters at which the position is accepted without a further step.
	constexpr double TOLERANCE = 1e-9;
	// Step in degrees for the finite differences, about 0.11 m.
	constexpr double DELTA = 1e-6;
	constexpr int MAX_ITERATIONS = 16;
	// A residual cannot fall below the spacing of the projected coordinates, so the
	// iteration also stops as soon as one no longer decreases. A residual far above
	// that spacing then means the solve failed instead of reaching that limit.
	constexpr double RESIDUAL_FLOOR = EARTH_EQUATORIAL_RADIUS*std::numeric_limits<double>::epsilon();
	constexpr double MAX_ACCEPTED_RESIDUAL = 1000.0*RESIDUAL_FLOOR;
	// The finite differences lose about ten digits, so a determinant below this
	// fraction of the product of the Jacobian's column norms carries no direction.
	constexpr double MIN_RELATIVE_DETERMINANT = 1e-8;
	// Meters per degree along a meridian, the local scale of the projection.
	constexpr double METERS_PER_DEGREE = Angle::DEGREES_TO_RADIANS*EARTH_EQUATORIAL_RADIUS;

	const auto [lon0, ml0] = referenceParameters(reference);

	// Starting from the local scale instead of from the reference saves one
	// iteration. It is a guess only, because the projection is not linear.
	double lat = std::clamp(reference.latitude().degrees() + y/METERS_PER_DEGREE, -90.0, 90.0);
	double lon = Angle::normalizedDegreesLongitude(reference.longitude().degrees()
			+ x/(METERS_PER_DEGREE*std::cos(reference.latitude().radians())));
	double residual = std::numeric_limits<double>::infinity();
	bool accepted = false;

	for (int i = 0; i < MAX_ITERATIONS; ++i)
	{
		const Cartesian f = toCartesian(lon0, ml0, fromDegrees(lat, lon));
		const double rx = f.x - x;
		const double ry = f.y - y;
		const double r = std::hypot(rx, ry);
		accepted = r <= MAX_ACCEPTED_RESIDUAL;
		if (r <= TOLERANCE || r >= residual) break;
		residual = r;

		const Cartesian fLat = toCartesian(lon0, ml0, fromDegrees(lat + DELTA, lon));
		const Cartesian fLon = toCartesian(lon0, ml0, fromDegrees(lat, lon + DELTA));
		const double dxdLat = (fLat.x - f.x)/DELTA;
		const double dxdLon = (fLon.x - f.x)/DELTA;
		const double dydLat = (fLat.y - f.y)/DELTA;
		const double dydLon = (fLon.y - f.y)/DELTA;

		const double determinant = dxdLat*dydLon - dxdLon*dydLat;
		if (std::abs(determinant) < MIN_RELATIVE_DETERMINANT*std::hypot(dxdLat, dydLat)*std::hypot(dxdLon, dydLon))
		{
			// Degenerate at a pole, where the forward projection is not invertible.
			break;
		}

		// The step itself does not respect the range of a position, so the iterate
		// is brought back into it.
		lat = std::clamp(lat - ( dydLon*rx - dxdLon*ry)/determinant, -90.0, 90.0);
		lon = Angle::normalizedDegreesLongitude(lon - (-dydLat*rx + dxdLat*ry)/determinant);
	}

	if (!accepted)
		return fromDegrees(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN());

	return fromDegrees(lat, lon);
}


namespace
{
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

	double mlfn(double lat)
	{
		const double sin_phi{std::sin(lat)};
		const double cos_phi{std::cos(lat) * sin_phi};
		const double squared_sin_phi = sin_phi * sin_phi;
		return (R0 * lat - cos_phi * (R1 + squared_sin_phi * (R2 + squared_sin_phi * (R3 + squared_sin_phi * R4))));
	}

	double msfn(double sinPhi, double cosPhi, double es)
	{
		return (cosPhi / std::sqrt(1.0 - es * sinPhi * sinPhi));
	}
}


std::pair<double, double> LatLon::referenceParameters(const LatLon& reference)
{
	// Validated here so that both the forward and the inverse conversion inherit
	// the check: a non-finite parameter makes the projection yield NaN below.
	if (!inDomain(reference))
	{
		const double undefined = std::numeric_limits<double>::quiet_NaN();
		return { undefined, undefined };
	}

	return { reference.longitude().radians(), mlfn(reference.latitude().radians()) };
}


Cartesian LatLon::toCartesian(const LatLon& reference) const
{
	return toCartesian(reference, *this);
}


Cartesian LatLon::toCartesian(const LatLon& reference, const LatLon& latLon)
{
	const auto [lon0, ml0] = referenceParameters(reference);
	return toCartesian(lon0, ml0, latLon);
}


Cartesian LatLon::toCartesian(double lon0, double ml0, const LatLon& latLon)
{
	double lat = latLon.latitude().radians();
	// On the equator the longitude does not enter the y coordinate, so a position with
	// a NaN longitude would otherwise come out with a finite y. The reference arrives
	// as lon0 and ml0, which referenceParameters() leaves non-finite when it is itself
	// outside the domain.
	if (!std::isfinite(lat) || !std::isfinite(latLon.longitude().radians())
		|| !std::isfinite(lon0) || !std::isfinite(ml0))
	{
		return Cartesian{std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
	}

	const double D = std::abs(lat) - Angle::PI_OVER_2;
	if (D > EPSILON12)
	{
		return Cartesian{std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
	}
	if (std::abs(D) < EPSILON12)
	{
		lat = (lat < 0.0) ? -1.0 * Angle::PI_OVER_2 : Angle::PI_OVER_2;
	}

	// Without wrapping the difference, a position and a reference on either side of
	// the antimeridian come out a whole turn apart.
	double lon = wrapLongitude(latLon.longitude().radians() - lon0);

	Cartesian xy{lon, -1.0 * ml0};
	if (!(std::abs(lat) < EPSILON10))
	{
		const double ms{(std::abs(std::sin(lat)) > EPSILON10) ? msfn(std::sin(lat), std::cos(lat), SQUARED_ECCENTRICITY) / std::sin(lat) : 0.0};
		xy.x = ms * std::sin(lon *= std::sin(lat));
		xy.y = (mlfn(lat) - ml0) + ms * (1.0 - std::cos(lon));
	}

	return Cartesian{EARTH_EQUATORIAL_RADIUS * xy.x, EARTH_EQUATORIAL_RADIUS * xy.y};
}


} // namespace Poco::Geo
