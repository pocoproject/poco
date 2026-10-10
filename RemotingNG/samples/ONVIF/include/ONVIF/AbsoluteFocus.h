// 
// AbsoluteFocus.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AbsoluteFocus_INCLUDED
#define ONVIF_AbsoluteFocus_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AbsoluteFocus
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AbsoluteFocus
{
public:
	AbsoluteFocus();

	AbsoluteFocus(
		float position, 
		Poco::Optional<float> speed);

	virtual ~AbsoluteFocus();

	float getPosition() const;

	Poco::Optional<float> getSpeed() const;

	void setPosition(float val);

	void setSpeed(Poco::Optional<float> val);

private:
	//@ name=Position
	//@ order=0
	float _position;

	//@ mandatory=false
	//@ name=Speed
	//@ order=1
	Poco::Optional<float> _speed;

};


inline float AbsoluteFocus::getPosition() const
{
	return _position;
}


inline Poco::Optional<float> AbsoluteFocus::getSpeed() const
{
	return _speed;
}


inline void AbsoluteFocus::setPosition(float val)
{
	_position = val;
}


inline void AbsoluteFocus::setSpeed(Poco::Optional<float> val)
{
	_speed = val;
}


} // ONVIF


#endif // ONVIF_AbsoluteFocus_INCLUDED
