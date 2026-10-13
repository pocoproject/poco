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
#include <algorithm>


namespace Poco {
namespace Geo {


const double Angle::PI(POCO_GEO_PI);
const double Angle::DEGREES_TO_RADIANS(POCO_GEO_PI/180.0);
const double Angle::RADIANS_TO_DEGREES(180.0/POCO_GEO_PI);
const double Angle::PI_OVER_2(POCO_GEO_PI/2);


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


Angle::Angle():
	_degrees(0.0),
	_radians(0.0)
{
}


Angle::Angle(double deg, double rad):
	_degrees(deg),
	_radians(rad)
{
}


Angle::Angle(const Angle& angle):
	_degrees(angle._degrees),
	_radians(angle._radians)
{
}


Angle::~Angle()
{
}


Angle& Angle::operator = (const Angle& angle)
{
	Angle tmp(angle);
	swap(tmp);
	return *this;
}


void Angle::swap(Angle& other)
{
	std::swap(_degrees, other._degrees);
	std::swap(_radians, other._radians);
}


Angle Angle::fromDegreesLatitude(double degrees)
{
	degrees = degrees < -90 ? -90 : degrees > 90 ? 90 : degrees;
	double radians = DEGREES_TO_RADIANS*degrees;
	radians = radians < -PI_OVER_2 ? -PI_OVER_2 : radians > PI_OVER_2 ? PI_OVER_2 : radians;
	
	return Angle(degrees, radians);
}


Angle Angle::fromRadiansLatitude(double radians)
{
	radians = radians < -PI_OVER_2 ? -PI_OVER_2 : radians > PI_OVER_2 ? PI_OVER_2 : radians;
	double degrees = RADIANS_TO_DEGREES*radians;
	degrees = degrees < -90 ? -90 : degrees > 90 ? 90 : degrees;
	
	return Angle(degrees, radians);
}


Angle Angle::fromDegreesLongitude(double degrees)
{
	degrees = degrees < -180 ? -180 : degrees > 180 ? 180 : degrees;
	double radians = DEGREES_TO_RADIANS*degrees;
	radians = radians < -PI ? -PI : radians > PI ? PI : radians;
	
	return Angle(degrees, radians);
}


Angle Angle::fromRadiansLongitude(double radians)
{
	radians = radians < -PI ? -PI : radians > PI ? PI : radians;
	double degrees = RADIANS_TO_DEGREES*radians;
	degrees = degrees < -180 ? -180 : degrees > 180 ? 180 : degrees;
	
	return Angle(degrees, radians);
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
	double lat = std::fmod(degrees, 180.0);
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


} } // namespace Poco::Geo
