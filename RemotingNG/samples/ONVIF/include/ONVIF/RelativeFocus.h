// 
// RelativeFocus.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RelativeFocus_INCLUDED
#define ONVIF_RelativeFocus_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RelativeFocus
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RelativeFocus
{
public:
	RelativeFocus();

	RelativeFocus(
		float distance, 
		Poco::Optional<float> speed);

	virtual ~RelativeFocus();

	float getDistance() const;

	Poco::Optional<float> getSpeed() const;

	void setDistance(float val);

	void setSpeed(Poco::Optional<float> val);

private:
	//@ name=Distance
	//@ order=0
	float _distance;

	//@ mandatory=false
	//@ name=Speed
	//@ order=1
	Poco::Optional<float> _speed;

};


inline float RelativeFocus::getDistance() const
{
	return _distance;
}


inline Poco::Optional<float> RelativeFocus::getSpeed() const
{
	return _speed;
}


inline void RelativeFocus::setDistance(float val)
{
	_distance = val;
}


inline void RelativeFocus::setSpeed(Poco::Optional<float> val)
{
	_speed = val;
}


} // ONVIF


#endif // ONVIF_RelativeFocus_INCLUDED
