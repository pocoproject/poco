// 
// SphericalCoordinate.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SphericalCoordinate_INCLUDED
#define ONVIF_SphericalCoordinate_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=SphericalCoordinate
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SphericalCoordinate
{
public:
	SphericalCoordinate();

	SphericalCoordinate(
		Poco::Optional<float> distance, 
		Poco::Optional<float> elevationAngle, 
		Poco::Optional<float> azimuthAngle);

	virtual ~SphericalCoordinate();

	Poco::Optional<float> getAzimuthAngle() const;

	Poco::Optional<float> getDistance() const;

	Poco::Optional<float> getElevationAngle() const;

	void setAzimuthAngle(Poco::Optional<float> val);

	void setDistance(Poco::Optional<float> val);

	void setElevationAngle(Poco::Optional<float> val);

private:
	//@ mandatory=false
	//@ name=Distance
	//@ order=0
	//@ type=attr
	Poco::Optional<float> _distance;

	//@ mandatory=false
	//@ name=ElevationAngle
	//@ order=1
	//@ type=attr
	Poco::Optional<float> _elevationAngle;

	//@ mandatory=false
	//@ name=AzimuthAngle
	//@ order=2
	//@ type=attr
	Poco::Optional<float> _azimuthAngle;

};


inline Poco::Optional<float> SphericalCoordinate::getAzimuthAngle() const
{
	return _azimuthAngle;
}


inline Poco::Optional<float> SphericalCoordinate::getDistance() const
{
	return _distance;
}


inline Poco::Optional<float> SphericalCoordinate::getElevationAngle() const
{
	return _elevationAngle;
}


inline void SphericalCoordinate::setAzimuthAngle(Poco::Optional<float> val)
{
	_azimuthAngle = val;
}


inline void SphericalCoordinate::setDistance(Poco::Optional<float> val)
{
	_distance = val;
}


inline void SphericalCoordinate::setElevationAngle(Poco::Optional<float> val)
{
	_elevationAngle = val;
}


} // ONVIF


#endif // ONVIF_SphericalCoordinate_INCLUDED
