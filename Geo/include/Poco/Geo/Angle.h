//
// Angle.h
//
// Library: Geo
// Package: Geo
// Module:  Angle
//
// Definition of the Angle class.
//
// Copyright (c) 2010-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Geo_Angle_INCLUDED
#define Geo_Angle_INCLUDED


#include "Poco/Geo/Geo.h"


#define POCO_GEO_PI 3.141592653589793238462643383279502884197


namespace Poco {
namespace Geo {


class Geo_API Angle
	/// This class represents a geometric angle.
	///
	/// Based on code from the Angle (gov.nasa.worldwind.geom.Angle)  
	/// class from the NASA World Wind Java source code released
	/// under the NOSA license.
	///
	/// Note: when comparing Angles for equality, the usual
	/// issues when comparing floating-point numbers should 
	/// be considered.
{
public:
	static const double PI;
	static const double DEGREES_TO_RADIANS;
	static const double RADIANS_TO_DEGREES;
	static const double PI_OVER_2;
	
	static const Angle ZERO;   /// An Angle of zero degrees.
	static const Angle POS90;  /// An Angle of positive 90 degrees.
	static const Angle NEG90;  /// An Angle of negative 90 degrees.
	static const Angle POS180; /// An Angle of positive 180 degrees.
	static const Angle NEG180; /// An Angle of negative 180 degrees.
	static const Angle POS360; /// An Angle of positive 360 degrees. 
	
	Angle();
		/// Creates a zero Angle.
		
	Angle(const Angle& angle);
		/// Creates an Angle by copying another one.
		
	~Angle();
		/// Destroys the Angle.
		
	Angle& operator = (const Angle& angle);
		/// Assignment operator.
		
	void swap(Angle& other);
		/// Swaps the Angle with another one.
	
	static Angle fromDegrees(double deg);
		/// Creates an Angle from a number of degrees.
		
	static Angle fromRadians(double rad);
		/// Creates an Angle from a number of radians.
		
	static Angle fromDegreesLatitude(double degrees);
		/// Creates an Angle from a latitude in degrees.
		///
		/// The latitude must be in the interval [-90, 90].

	static Angle fromRadiansLatitude(double radians);
		/// Creates an Angle from a latitude in radians.
		///
		/// The latitude must be in the interval [-PI/2, PI/2].

	static Angle fromDegreesLongitude(double degrees);
		/// Creates an Angle from a longitude in degrees.
		///
		/// The latitude must be in the interval [-180, 180].

	static Angle fromRadiansLongitude(double radians);
		/// Creates an Angle from a longitude in radians.
		///
		/// The latitude must be in the interval [-PI, PI].
		
	static Angle fromXY(double x, double y);
		/// Creates an Angle from rectangular coordinates.
		///   - x is the abscissa coordinate
		///   - y is the ordinate coordinate
	
	double degrees() const;
		/// Returns the Angle expressed in degrees.
		
	double radians() const;
		/// Returns the Angle expressed in radians.
		
	Angle angularDistanceTo(const Angle& angle) const;
		/// Returns the shortest distance between this and the 
		/// given Angle.
		
	Angle operator + (const Angle& angle) const;
		/// Adds another Angle and returns the result.
		
	Angle operator - (const Angle& angle) const;
		/// Subtracts another Angle and returns the result.
		
	Angle operator * (double multiplier) const;
		/// Multiplies the Angle by the given multiplier
		/// and returns the result.
		
	Angle operator / (double divisor) const;
		/// Divides the Angle by the given divisor
		/// and returns the result.
		
	double operator / (const Angle& angle) const;
		/// Divices the Angle by the given angle
		/// and returns the result.
		
	double sin() const;
		/// Returns the sine of this Angle.
		
	double cos() const;
		/// Returns the cosine of this Angle.
		
	static double normalizedDegreesLatitude(double degrees);
		/// Normalizes the given angle in degrees to a
		/// latitude in the interval [-90, 90] and returns
		/// the result.
		
	static double normalizedDegreesLongitude(double degrees);
		/// Normalizes the given angle in degrees to a
		/// longitude in the interval [-180, 180] and returns
		/// the result.

	static double normalizedDegrees(double degrees);
		/// Normalizes the given angle in degrees to
		/// the interval [0, 360) and returns
		/// the result.
	
	Angle normalizedLatitude() const;
		/// Returns a normalized latitudinal Angle.
		
	Angle normalizedLongitude() const;
		/// Returns a normalized longitudinal Angle.
		
	Angle normalized() const;
		/// Returns a normalized Angle.
		
	static const std::string& compassDirectionFromDegrees(double degrees);
		/// Returns the corresponding compass direction (N, NNW, NW, etc.)
		/// for the given angle.
		
	const std::string& compassDirection() const;
		/// Returns the corresponding compass direction (N, NNW, NW, etc.)
		/// for the Angle.
	
	// Comparison
	bool operator == (const Angle& angle) const;
	bool operator != (const Angle& angle) const;
	bool operator >  (const Angle& angle) const;
	bool operator >= (const Angle& angle) const;
	bool operator <  (const Angle& angle) const;
	bool operator <= (const Angle& angle) const;

	static const std::string COMPASS_N;
	static const std::string COMPASS_NNE;
	static const std::string COMPASS_NE;
	static const std::string COMPASS_ENE;
	static const std::string COMPASS_E;
	static const std::string COMPASS_ESE;
	static const std::string COMPASS_SE;
	static const std::string COMPASS_SSE;
	static const std::string COMPASS_S;
	static const std::string COMPASS_SSW;
	static const std::string COMPASS_SW;
	static const std::string COMPASS_WSW;
	static const std::string COMPASS_W;
	static const std::string COMPASS_WNW;
	static const std::string COMPASS_NW;
	static const std::string COMPASS_NNW;
	
protected:
	Angle(double deg, double rad);
		/// Creates an Angle.
	
private:
	double _degrees;
	double _radians;
};


//
// inlines
//
inline Angle Angle::fromDegrees(double deg)
{
	return Angle(deg, DEGREES_TO_RADIANS*deg);
}


inline Angle Angle::fromRadians(double rad)
{
	return Angle(RADIANS_TO_DEGREES*rad, rad);
}


inline double Angle::degrees() const
{
	return _degrees;
}


inline double Angle::radians() const
{
	return _radians;
}


inline bool Angle::operator == (const Angle& angle) const
{
	return _degrees == angle._degrees;
}


inline bool Angle::operator != (const Angle& angle) const
{
	return _degrees != angle._degrees;
}


inline bool Angle::operator > (const Angle& angle) const
{
	return _degrees > angle._degrees;
}


inline bool Angle::operator >= (const Angle& angle) const
{
	return _degrees >= angle._degrees;
}


inline bool Angle::operator < (const Angle& angle) const
{
	return _degrees < angle._degrees;
}


inline bool Angle::operator <= (const Angle& angle) const
{
	return _degrees <= angle._degrees;
}


inline Angle Angle::operator + (const Angle& angle) const
{
	return fromDegrees(_degrees + angle._degrees);
}


inline Angle Angle::operator - (const Angle& angle) const
{
	return fromDegrees(_degrees - angle._degrees);
}


inline Angle Angle::operator * (double multiplier) const
{
	return fromDegrees(_degrees*multiplier);
}


inline Angle Angle::operator / (double divisor) const
{
	return fromDegrees(_degrees/divisor);
}


inline double Angle::operator / (const Angle& angle) const
{
	return _degrees/angle._degrees;
}


inline void swap(Angle& a1, Angle& a2)
{
	a1.swap(a2);
}


} } // namespace Poco::Geo


#endif // Geo_Angle_INCLUDED
