// 
// LocalOrientation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_LocalOrientation_INCLUDED
#define ONVIF_LocalOrientation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=LocalOrientation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API LocalOrientation
{
public:
	LocalOrientation();

	LocalOrientation(
		Poco::Optional<float> pan, 
		Poco::Optional<float> tilt, 
		Poco::Optional<float> roll);

	virtual ~LocalOrientation();

	Poco::Optional<float> getPan() const;

	Poco::Optional<float> getRoll() const;

	Poco::Optional<float> getTilt() const;

	void setPan(Poco::Optional<float> val);

	void setRoll(Poco::Optional<float> val);

	void setTilt(Poco::Optional<float> val);

private:
	//@ mandatory=false
	//@ name=pan
	//@ order=0
	//@ type=attr
	Poco::Optional<float> _pan;

	//@ mandatory=false
	//@ name=tilt
	//@ order=1
	//@ type=attr
	Poco::Optional<float> _tilt;

	//@ mandatory=false
	//@ name=roll
	//@ order=2
	//@ type=attr
	Poco::Optional<float> _roll;

};


inline Poco::Optional<float> LocalOrientation::getPan() const
{
	return _pan;
}


inline Poco::Optional<float> LocalOrientation::getRoll() const
{
	return _roll;
}


inline Poco::Optional<float> LocalOrientation::getTilt() const
{
	return _tilt;
}


inline void LocalOrientation::setPan(Poco::Optional<float> val)
{
	_pan = val;
}


inline void LocalOrientation::setRoll(Poco::Optional<float> val)
{
	_roll = val;
}


inline void LocalOrientation::setTilt(Poco::Optional<float> val)
{
	_tilt = val;
}


} // ONVIF


#endif // ONVIF_LocalOrientation_INCLUDED
