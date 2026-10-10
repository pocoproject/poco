// 
// GeoLocation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_GeoLocation_INCLUDED
#define ONVIF_GeoLocation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=GeoLocation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API GeoLocation
{
public:
	GeoLocation();

	GeoLocation(
		Poco::Optional<double> lon, 
		Poco::Optional<double> lat, 
		Poco::Optional<float> elevation);

	virtual ~GeoLocation();

	Poco::Optional<float> getElevation() const;

	Poco::Optional<double> getLat() const;

	Poco::Optional<double> getLon() const;

	void setElevation(Poco::Optional<float> val);

	void setLat(Poco::Optional<double> val);

	void setLon(Poco::Optional<double> val);

private:
	//@ mandatory=false
	//@ name=lon
	//@ order=0
	//@ type=attr
	Poco::Optional<double> _lon;

	//@ mandatory=false
	//@ name=lat
	//@ order=1
	//@ type=attr
	Poco::Optional<double> _lat;

	//@ mandatory=false
	//@ name=elevation
	//@ order=2
	//@ type=attr
	Poco::Optional<float> _elevation;

};


inline Poco::Optional<float> GeoLocation::getElevation() const
{
	return _elevation;
}


inline Poco::Optional<double> GeoLocation::getLat() const
{
	return _lat;
}


inline Poco::Optional<double> GeoLocation::getLon() const
{
	return _lon;
}


inline void GeoLocation::setElevation(Poco::Optional<float> val)
{
	_elevation = val;
}


inline void GeoLocation::setLat(Poco::Optional<double> val)
{
	_lat = val;
}


inline void GeoLocation::setLon(Poco::Optional<double> val)
{
	_lon = val;
}


} // ONVIF


#endif // ONVIF_GeoLocation_INCLUDED
