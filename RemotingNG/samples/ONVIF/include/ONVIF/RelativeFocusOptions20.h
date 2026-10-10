// 
// RelativeFocusOptions20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RelativeFocusOptions20_INCLUDED
#define ONVIF_RelativeFocusOptions20_INCLUDED


#include "ONVIF/FloatRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class FloatRange;
} 


namespace ONVIF {


//@ name=RelativeFocusOptions20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RelativeFocusOptions20
{
public:
	RelativeFocusOptions20();

	RelativeFocusOptions20(
		const FloatRange& distance, 
		const Poco::SharedPtr<FloatRange>& speed);

	virtual ~RelativeFocusOptions20();

	const FloatRange& getDistance() const;

	const Poco::SharedPtr<FloatRange>& getSpeed() const;

	void setDistance(const FloatRange& val);

	void setDistance(FloatRange&& val);

	void setSpeed(const Poco::SharedPtr<FloatRange>& val);

	void setSpeed(Poco::SharedPtr<FloatRange>&& val);

private:
	//@ name=Distance
	//@ order=0
	FloatRange _distance;

	//@ mandatory=false
	//@ name=Speed
	//@ order=1
	Poco::SharedPtr<FloatRange> _speed;

};


inline const FloatRange& RelativeFocusOptions20::getDistance() const
{
	return _distance;
}


inline const Poco::SharedPtr<FloatRange>& RelativeFocusOptions20::getSpeed() const
{
	return _speed;
}


inline void RelativeFocusOptions20::setDistance(const FloatRange& val)
{
	_distance = val;
}


inline void RelativeFocusOptions20::setDistance(FloatRange&& val)
{
	_distance = std::move(val);
}


inline void RelativeFocusOptions20::setSpeed(const Poco::SharedPtr<FloatRange>& val)
{
	_speed = val;
}


inline void RelativeFocusOptions20::setSpeed(Poco::SharedPtr<FloatRange>&& val)
{
	_speed = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/FloatRange.h"


#endif // ONVIF_RelativeFocusOptions20_INCLUDED
