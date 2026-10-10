// 
// GeoOrientation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_GeoOrientation_INCLUDED
#define ONVIF_GeoOrientation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=GeoOrientation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API GeoOrientation
{
public:
	GeoOrientation();

	GeoOrientation(
		Poco::Optional<float> roll, 
		Poco::Optional<float> pitch, 
		Poco::Optional<float> yaw);

	virtual ~GeoOrientation();

	Poco::Optional<float> getPitch() const;

	Poco::Optional<float> getRoll() const;

	Poco::Optional<float> getYaw() const;

	void setPitch(Poco::Optional<float> val);

	void setRoll(Poco::Optional<float> val);

	void setYaw(Poco::Optional<float> val);

private:
	//@ mandatory=false
	//@ name=roll
	//@ order=0
	//@ type=attr
	Poco::Optional<float> _roll;

	//@ mandatory=false
	//@ name=pitch
	//@ order=1
	//@ type=attr
	Poco::Optional<float> _pitch;

	//@ mandatory=false
	//@ name=yaw
	//@ order=2
	//@ type=attr
	Poco::Optional<float> _yaw;

};


inline Poco::Optional<float> GeoOrientation::getPitch() const
{
	return _pitch;
}


inline Poco::Optional<float> GeoOrientation::getRoll() const
{
	return _roll;
}


inline Poco::Optional<float> GeoOrientation::getYaw() const
{
	return _yaw;
}


inline void GeoOrientation::setPitch(Poco::Optional<float> val)
{
	_pitch = val;
}


inline void GeoOrientation::setRoll(Poco::Optional<float> val)
{
	_roll = val;
}


inline void GeoOrientation::setYaw(Poco::Optional<float> val)
{
	_yaw = val;
}


} // ONVIF


#endif // ONVIF_GeoOrientation_INCLUDED
