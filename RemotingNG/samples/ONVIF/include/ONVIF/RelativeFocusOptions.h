// 
// RelativeFocusOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RelativeFocusOptions_INCLUDED
#define ONVIF_RelativeFocusOptions_INCLUDED


#include "ONVIF/FloatRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RelativeFocusOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RelativeFocusOptions
{
public:
	RelativeFocusOptions();

	RelativeFocusOptions(
		const FloatRange& distance, 
		const FloatRange& speed);

	virtual ~RelativeFocusOptions();

	const FloatRange& getDistance() const;

	const FloatRange& getSpeed() const;

	void setDistance(const FloatRange& val);

	void setDistance(FloatRange&& val);

	void setSpeed(const FloatRange& val);

	void setSpeed(FloatRange&& val);

private:
	//@ name=Distance
	//@ order=0
	FloatRange _distance;

	//@ name=Speed
	//@ order=1
	FloatRange _speed;

};


inline const FloatRange& RelativeFocusOptions::getDistance() const
{
	return _distance;
}


inline const FloatRange& RelativeFocusOptions::getSpeed() const
{
	return _speed;
}


inline void RelativeFocusOptions::setDistance(const FloatRange& val)
{
	_distance = val;
}


inline void RelativeFocusOptions::setDistance(FloatRange&& val)
{
	_distance = std::move(val);
}


inline void RelativeFocusOptions::setSpeed(const FloatRange& val)
{
	_speed = val;
}


inline void RelativeFocusOptions::setSpeed(FloatRange&& val)
{
	_speed = std::move(val);
}


} // ONVIF


#endif // ONVIF_RelativeFocusOptions_INCLUDED
