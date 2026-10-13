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


void Angle::swap(Angle& other)
{
	std::swap(_degrees, other._degrees);
	std::swap(_radians, other._radians);
}


Angle Angle::fromXY(double x, double y)
{
	double radians = std::atan2(y, x);
	return Angle(RADIANS_TO_DEGREES*radians, radians);
}


Angle Angle::angularDistanceTo(const Angle& angle) const
{
	double differenceDegrees = (angle - *this)._degrees;
	if (differenceDegrees < -180)
		differenceDegrees += 360;
	else if (differenceDegrees > 180)
		differenceDegrees -= 360;
	
	double absAngle = std::abs(differenceDegrees);
	return fromDegrees(absAngle);
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
	// Reduce to (-180, 180] first. Taking the remainder modulo 180 loses the
	// hemisphere: 190 degrees is 10 degrees south on the far side of the north
	// pole, not 10 degrees north.
	double lat = std::fmod(degrees, 360.0);
	if (lat > 180) lat -= 360;
	else if (lat < -180) lat += 360;
	return lat > 90 ? 180 - lat : lat < -90 ? -180 - lat : lat;
}

	
double Angle::normalizedDegreesLongitude(double degrees)
{
	double lon = std::fmod(degrees, 360.0);
	return lon > 180 ? lon - 360 : lon < -180 ? 360 + lon : lon;
}


double Angle::normalizedDegrees(double degrees)
{
	double deg = std::fmod(degrees, 360.0);
	return deg < 0 ? 360 + deg : deg;
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


const std::string& Angle::compassDirectionFromDegrees(double degrees)
{
	degrees = normalizedDegrees(degrees);

	if (degrees >= 15*22.5 + 11.25)
		return COMPASS_N;
	else if (degrees >= 14*22.5 + 11.25)
		return COMPASS_NNW;
	else if (degrees >= 13*22.5 + 11.25)
		return COMPASS_NW;
	else if (degrees >= 12*22.5 + 11.25)
		return COMPASS_WNW;
	else if (degrees >= 11*22.5 + 11.25)
		return COMPASS_W;
	else if (degrees >= 10*22.5 + 11.25)
		return COMPASS_WSW;
	else if (degrees >= 9*22.5 + 11.25)
		return COMPASS_SW;
	else if (degrees >= 8*22.5 + 11.25)
		return COMPASS_SSW;
	else if (degrees >= 7*22.5 + 11.25)
		return COMPASS_S;
	else if (degrees >= 6*22.5 + 11.25)
		return COMPASS_SSE;
	else if (degrees >= 5*22.5 + 11.25)
		return COMPASS_SE;
	else if (degrees >= 4*22.5 + 11.25)
		return COMPASS_ESE;
	else if (degrees >= 3*22.5 + 11.25)
		return COMPASS_E;
	else if (degrees >= 2*22.5 + 11.25)
		return COMPASS_ENE;
	else if (degrees >= 1*22.5 + 11.25)
		return COMPASS_NE;
	else if (degrees >= 11.25)
		return COMPASS_NNE;
	else
		return COMPASS_N;
}


const std::string& Angle::compassDirection() const
{
	return compassDirectionFromDegrees(_degrees);
}


} // namespace Poco::Geo
