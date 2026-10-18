//
// Angle.cpp
//
// Library: Geo
// Package: Geo
// Module:  Angle
//
// Copyright (c) 2010-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Geo/Angle.h"
#include <array>
#include <cmath>
#include <utility>


namespace Poco::Geo {


const Angle Angle::ZERO;
const Angle Angle::POS90(Angle::fromDegrees(90));
const Angle Angle::NEG90(Angle::fromDegrees(-90));
const Angle Angle::POS180(Angle::fromDegrees(180));
const Angle Angle::NEG180(Angle::fromDegrees(-180));
const Angle Angle::POS360(Angle::fromDegrees(360));
const std::string Angle::COMPASS_N("N");
const std::string Angle::COMPASS_NNE("NNE");
const std::string Angle::COMPASS_NE("NE");
const std::string Angle::COMPASS_ENE("ENE");
const std::string Angle::COMPASS_E("E");
const std::string Angle::COMPASS_ESE("ESE");
const std::string Angle::COMPASS_SE("SE");
const std::string Angle::COMPASS_SSE("SSE");
const std::string Angle::COMPASS_S("S");
const std::string Angle::COMPASS_SSW("SSW");
const std::string Angle::COMPASS_SW("SW");
const std::string Angle::COMPASS_WSW("WSW");
const std::string Angle::COMPASS_W("W");
const std::string Angle::COMPASS_WNW("WNW");
const std::string Angle::COMPASS_NW("NW");
const std::string Angle::COMPASS_NNW("NNW");


void Angle::swap(Angle& other) noexcept
{
	std::swap(_degrees, other._degrees);
	std::swap(_radians, other._radians);
}


Angle Angle::fromXY(double x, double y)
{
	return fromRadians(std::atan2(y, x));
}


Angle Angle::angularDistanceTo(const Angle& angle) const
{
	return fromDegrees(std::abs(normalizedDegreesLongitude(angle._degrees - _degrees)));
}


double Angle::sin() const
{
	return std::sin(_radians);
}


double Angle::cos() const
{
	return std::cos(_radians);
}


double Angle::normalizedDegreesLatitude(double degrees)
{
	// Reducing modulo 180 would lose the hemisphere: 190 degrees is 10 degrees
	// south on the far side of the north pole, not 10 degrees north.
	const double lat = normalizedDegreesLongitude(degrees);
	return lat > 90 ? 180 - lat : lat < -90 ? -180 - lat : lat;
}

	
double Angle::normalizedDegreesLongitude(double degrees)
{
	double lon = std::fmod(degrees, 360.0);
	return lon > 180 ? lon - 360 : lon < -180 ? 360 + lon : lon;
}


double Angle::normalizedDegrees(double degrees)
{
	// std::remainder reduces exactly, to [-180, 180]. The outer fmod keeps the
	// interval half-open, where adding 360 to a tiny negative value rounds up to 360.
	return std::fmod(std::remainder(degrees, 360.0) + 360.0, 360.0);
}


Angle Angle::normalizedLatitude() const
{
	return fromDegrees(normalizedDegreesLatitude(_degrees));
}

	
Angle Angle::normalizedLongitude() const
{
	return fromDegrees(normalizedDegreesLongitude(_degrees));
}


Angle Angle::normalized() const
{
	return fromDegrees(normalizedDegrees(_degrees));
}


namespace
{
	const std::array<const std::string*, 16> DIRECTIONS =
	{
		&Angle::COMPASS_N,
		&Angle::COMPASS_NNE,
		&Angle::COMPASS_NE,
		&Angle::COMPASS_ENE,
		&Angle::COMPASS_E,
		&Angle::COMPASS_ESE,
		&Angle::COMPASS_SE,
		&Angle::COMPASS_SSE,
		&Angle::COMPASS_S,
		&Angle::COMPASS_SSW,
		&Angle::COMPASS_SW,
		&Angle::COMPASS_WSW,
		&Angle::COMPASS_W,
		&Angle::COMPASS_WNW,
		&Angle::COMPASS_NW,
		&Angle::COMPASS_NNW
	};
		/// Indexed by compass sector, clockwise from north.
}


const std::string& Angle::compassDirectionFromDegrees(double degrees)
{
	degrees = normalizedDegrees(degrees);
	int sector = 16;
	while (sector > 0 && degrees < sector*22.5 - 11.25) --sector;

	// The north sector straddles zero, so its upper half comes out as sector 16.
	return *DIRECTIONS[sector % 16];
}


const std::string& Angle::compassDirection() const
{
	return compassDirectionFromDegrees(_degrees);
}


} // namespace Poco::Geo
