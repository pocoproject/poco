//
// LatLon.h
//
// Library: Geo
// Package: Geo
// Module:  LatLon
//
// Definition of the LatLon class.
//
// Copyright (c) 2010-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Geo_LatLon_INCLUDED
#define Geo_LatLon_INCLUDED


#include "Poco/Geo/Geo.h"
#include "Poco/Geo/Angle.h"
#include <utility>


namespace Poco::Geo {


struct Cartesian
	/// Cartesian coordinates, given in meters.
{
	double x = 0.0;
	double y = 0.0;
};


class Geo_API LatLon
	/// LatLon represents a point on the two-dimensional surface of a globe. 
	/// Latitude is the degrees North and ranges between [-90, 90]
	/// while longitude refers to degrees East, and ranges between [-180, 180].
	/// The range is not enforced; Angle::normalizedLatitude() and
	/// Angle::normalizedLongitude() reduce a position to it.
	///
	/// A position outside the domain of a computation, and a position with a NaN
	/// coordinate, yield NaN coordinates and NaN distances. Every comparison against
	/// a NaN is false, so a geofence reports such a position as outside.
	///
	/// Based on code from the LatLon (gov.nasa.worldwind.geom.LatLon)  
	/// class from the NASA WorldWind Java source code released
	/// under the Apache License 2.0.
	///
	/// toCartesian() is based on WGS84toCartesian.hpp by Christian Berger,
	/// partially based on the work from Gerald I. Evenden on proj 4.3 released under public domain terms.
	///
	/// Note: when comparing LatLons for equality, the usual
	/// issues when comparing floating-point numbers should 
	/// be considered.
{
public:
	static const LatLon ZERO; /// Zero LatLon (for Earth, the crossing of equator and Greenwich meridian)
	static constexpr double EARTH_EQUATORIAL_RADIUS = 6378137.0; /// Earth equatorial radius in meters (6378137.0 m, derived from WGS-84 ellipsoid)
	static constexpr double EARTH_POLAR_RADIUS = 6356752.3; /// Earth polar radius in meters (6356752.3 m, derived from WGS-84 ellipsoid)
	static constexpr double EARTH_MEAN_RADIUS = 6371009.0; /// Earth mean radius according to IUGG in meters (6371009 m)
	
	constexpr LatLon() = default;
		/// Creates a zero LatLon.
		
	LatLon(const Angle& latitude, const Angle& longitude);
		/// Creates a LatLon with the given latitude and longitude.
		
	LatLon(const LatLon& other) = default;
		/// Creates a LatLon by copying another one.
		
	~LatLon() = default;
		/// Destroys the LatLon.

	LatLon& operator = (const LatLon& other) = default;
		/// Assignment operator.
		
	void assign(const LatLon& other);
		/// Assigns another LatLon.
		
	void assign(const Angle& latitude, const Angle& longitude);
		/// Assigns another latitude and longitude.
		
	void swap(LatLon& other) noexcept;
		/// Swaps the LatLon with another one.
		
	[[nodiscard]] static LatLon fromDegrees(double latitude, double longitude);
		/// Creates a LatLon from the given latitude and longitude in degrees.

	[[nodiscard]] static LatLon fromRadians(double latitude, double longitude);
		/// Creates a LatLon from the given latitude and longitude in radians.

	[[nodiscard]] static LatLon fromCartesian(const LatLon& reference, const Cartesian& xy);
		/// Creates a LatLon from the given cartesian coordinates (in meters),
		/// relative to the given reference position.

	[[nodiscard]] static LatLon fromCartesian(const LatLon& reference, double x, double y);
		/// Creates a LatLon from the given cartesian coordinates (in meters),
		/// relative to the given reference position.

	[[nodiscard]] Cartesian toCartesian(const LatLon& reference) const;
		/// Converts the LatLon to cartesian coordinates (in meters),
		/// relative to the given reference position.

	[[nodiscard]] static Cartesian toCartesian(const LatLon& reference, const LatLon& latLon);
		/// Converts the LatLon to cartesian coordinates (in meters),
		/// relative to the given reference position.

	[[nodiscard]] constexpr const Angle& latitude() const;
		/// Returns the latitude.
		
	[[nodiscard]] constexpr const Angle& longitude() const;
		/// Returns the longitude.

	[[nodiscard]] Angle greatCircleDistanceTo(const LatLon& latLon) const;
		/// Computes the great circle angular distance between two locations using
		/// the haversine formula.
		///
		/// The return value gives the distance as the angle between the two positions 
		/// on the PI radius circle. In radians, this angle is also the arc length of the
		/// segment between the two positions on that circle. To compute a distance in 
		/// meters from this value, multiply it by the radius of the globe.
		///
		/// Example: compute the distance (in meters) between New York and San Francisco:
		///
		///     LatLon sanFrancisco(LatLon::fromDegrees(37.728965, -122.420151));
		///     LatLon newYork(LatLon::fromDegrees(40.712180, -73.995796));
		///     Angle a = newYork.greatCircleDistanceTo(sanFrancisco);
		///     double m = a.radians()*LatLon::EARTH_MEAN_RADIUS;

	[[nodiscard]] Angle greatCircleAzimuthTo(const LatLon& latLon) const;
		/// Computes the azimuth angle (clockwise from North) that points from this
		/// location to the second location using the haversine formula.
		///
		/// This angle can be used as the starting azimuth for a great circle arc 
		/// that begins at the first location, and passes through the second location.

	[[nodiscard]] LatLon greatCircleEndPosition(const Angle& greatCircleAzimuth, const Angle& arcDistance) const;
		/// Computes the location on a great circle arc with this as starting location, 
		/// the given azimuth, and arc distance.
		///
		/// To compute the arcDistance (in radians) from a distance in meters, divide the 
		/// distance in meters by the radius of the globe.
		///
		/// Example: travel from New York to San Francisco (distance: 4131767 meters, heading: 281.6 degrees)
		///
		///     LatLon newYork(LatLon::fromDegrees(40.712180, -73.995796));
		///     LatLon destination = newYork.greatCircleEndPosition(Angle::fromDegrees(281.6), 
		///                              Angle::fromRadians(4131767.0/LatLon::EARTH_MEAN_RADIUS));

	[[nodiscard]] Angle rhumbDistanceTo(const LatLon& latLon) const;
		/// Computes the length of the rhumb line (a line of constant heading) between two locations.
		///
		/// The return value gives the distance as the angular distance between the 
		/// two positions on the PI radius circle. In radians, this angle is also the arc length
		/// of the segment between the two positions on that circle. 
		/// To compute a distance in meters from this value, multiply it by the radius of the globe.

	[[nodiscard]] Angle rhumbAzimuthTo(const LatLon& latLon) const;
		/// Computes the azimuth angle (clockwise from North) of a rhumb line 
		/// (a line of constant heading) between two locations.
		
	[[nodiscard]] LatLon rhumbEndPosition(const Angle& rhumbAzimuth, const Angle& arcDistance) const;
		/// Computes the location on a rhumb line with the given starting location, 
		/// rhumb azimuth, and arc distance along the line.
		///
		/// To compute the arcDistance (in radians) from a distance in meters, divide 
		/// the distance in meters by the radius of the globe.
		
	[[nodiscard]] bool operator == (const LatLon& other) const;
	[[nodiscard]] bool operator != (const LatLon& other) const;
	
	[[nodiscard]] bool equals(const LatLon& other, double delta) const;
		/// Returns true if the other position is within the given
		/// delta of this position. Delta is given in degrees.

private:
	[[nodiscard]] static std::pair<double, double> referenceParameters(const LatLon& reference);
		/// Returns the reference longitude in radians and the meridional distance of the
		/// reference latitude, the two quantities of the cartesian projection that depend
		/// on the reference alone.

	[[nodiscard]] static Cartesian toCartesian(double lon0, double ml0, const LatLon& latLon);
		/// Converts the given position to cartesian coordinates, with the reference
		/// parameters computed once by referenceParameters(), so that a sequence of
		/// conversions against one reference does not recompute them.

	Angle _latitude;
	Angle _longitude;
};


//
// inlines
//
inline LatLon LatLon::fromDegrees(double latitude, double longitude)
{
	return LatLon(Angle::fromDegrees(latitude), Angle::fromDegrees(longitude));
}


inline LatLon LatLon::fromRadians(double latitude, double longitude)
{
	return LatLon(Angle::fromRadians(latitude), Angle::fromRadians(longitude));
}


constexpr const Angle& LatLon::latitude() const
{
	return _latitude;
}


constexpr const Angle& LatLon::longitude() const
{
	return _longitude;
}


inline bool LatLon::operator == (const LatLon& other) const
{
	return _latitude == other._latitude && _longitude == other._longitude;
}


inline bool LatLon::operator != (const LatLon& other) const
{
	return _latitude != other._latitude || _longitude != other._longitude;
}


inline void swap(LatLon& l1, LatLon& l2) noexcept
{
	l1.swap(l2);
}


} // namespace Poco::Geo


#endif // Geo_LatLon_INCLUDED
