// 
// LensProjection.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_LensProjection_INCLUDED
#define ONVIF_LensProjection_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=LensProjection
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API LensProjection
{
public:
	LensProjection();

	LensProjection(
		float angle, 
		float radius, 
		Poco::Optional<float> transmittance);

	virtual ~LensProjection();

	float getAngle() const;

	float getRadius() const;

	Poco::Optional<float> getTransmittance() const;

	void setAngle(float val);

	void setRadius(float val);

	void setTransmittance(Poco::Optional<float> val);

private:
	//@ name=Angle
	//@ order=0
	float _angle;

	//@ name=Radius
	//@ order=1
	float _radius;

	//@ mandatory=false
	//@ name=Transmittance
	//@ order=2
	Poco::Optional<float> _transmittance;

};


inline float LensProjection::getAngle() const
{
	return _angle;
}


inline float LensProjection::getRadius() const
{
	return _radius;
}


inline Poco::Optional<float> LensProjection::getTransmittance() const
{
	return _transmittance;
}


inline void LensProjection::setAngle(float val)
{
	_angle = val;
}


inline void LensProjection::setRadius(float val)
{
	_radius = val;
}


inline void LensProjection::setTransmittance(Poco::Optional<float> val)
{
	_transmittance = val;
}


} // ONVIF


#endif // ONVIF_LensProjection_INCLUDED
