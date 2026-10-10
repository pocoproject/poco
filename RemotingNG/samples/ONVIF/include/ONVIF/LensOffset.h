// 
// LensOffset.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_LensOffset_INCLUDED
#define ONVIF_LensOffset_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=LensOffset
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API LensOffset
{
public:
	LensOffset();

	LensOffset(
		Poco::Optional<float> x, 
		Poco::Optional<float> y);

	virtual ~LensOffset();

	Poco::Optional<float> getX() const;

	Poco::Optional<float> getY() const;

	void setX(Poco::Optional<float> val);

	void setY(Poco::Optional<float> val);

private:
	//@ mandatory=false
	//@ name=x
	//@ order=0
	//@ type=attr
	Poco::Optional<float> _x;

	//@ mandatory=false
	//@ name=y
	//@ order=1
	//@ type=attr
	Poco::Optional<float> _y;

};


inline Poco::Optional<float> LensOffset::getX() const
{
	return _x;
}


inline Poco::Optional<float> LensOffset::getY() const
{
	return _y;
}


inline void LensOffset::setX(Poco::Optional<float> val)
{
	_x = val;
}


inline void LensOffset::setY(Poco::Optional<float> val)
{
	_y = val;
}


} // ONVIF


#endif // ONVIF_LensOffset_INCLUDED
