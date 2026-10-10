// 
// LocalLocation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_LocalLocation_INCLUDED
#define ONVIF_LocalLocation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=LocalLocation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API LocalLocation
{
public:
	LocalLocation();

	LocalLocation(
		Poco::Optional<float> x, 
		Poco::Optional<float> y, 
		Poco::Optional<float> z);

	virtual ~LocalLocation();

	Poco::Optional<float> getX() const;

	Poco::Optional<float> getY() const;

	Poco::Optional<float> getZ() const;

	void setX(Poco::Optional<float> val);

	void setY(Poco::Optional<float> val);

	void setZ(Poco::Optional<float> val);

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

	//@ mandatory=false
	//@ name=z
	//@ order=2
	//@ type=attr
	Poco::Optional<float> _z;

};


inline Poco::Optional<float> LocalLocation::getX() const
{
	return _x;
}


inline Poco::Optional<float> LocalLocation::getY() const
{
	return _y;
}


inline Poco::Optional<float> LocalLocation::getZ() const
{
	return _z;
}


inline void LocalLocation::setX(Poco::Optional<float> val)
{
	_x = val;
}


inline void LocalLocation::setY(Poco::Optional<float> val)
{
	_y = val;
}


inline void LocalLocation::setZ(Poco::Optional<float> val)
{
	_z = val;
}


} // ONVIF


#endif // ONVIF_LocalLocation_INCLUDED
