// 
// ContinuousFocus.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ContinuousFocus_INCLUDED
#define ONVIF_ContinuousFocus_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ContinuousFocus
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ContinuousFocus
{
public:
	ContinuousFocus();

	ContinuousFocus(float speed);

	virtual ~ContinuousFocus();

	float getSpeed() const;

	void setSpeed(float val);

private:
	//@ name=Speed
	//@ order=0
	float _speed;

};


inline float ContinuousFocus::getSpeed() const
{
	return _speed;
}


inline void ContinuousFocus::setSpeed(float val)
{
	_speed = val;
}


} // ONVIF


#endif // ONVIF_ContinuousFocus_INCLUDED
