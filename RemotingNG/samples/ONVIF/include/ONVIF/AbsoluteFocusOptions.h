// 
// AbsoluteFocusOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AbsoluteFocusOptions_INCLUDED
#define ONVIF_AbsoluteFocusOptions_INCLUDED


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


//@ name=AbsoluteFocusOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AbsoluteFocusOptions
{
public:
	AbsoluteFocusOptions();

	AbsoluteFocusOptions(
		const FloatRange& position, 
		const Poco::SharedPtr<FloatRange>& speed);

	virtual ~AbsoluteFocusOptions();

	const FloatRange& getPosition() const;

	const Poco::SharedPtr<FloatRange>& getSpeed() const;

	void setPosition(const FloatRange& val);

	void setPosition(FloatRange&& val);

	void setSpeed(const Poco::SharedPtr<FloatRange>& val);

	void setSpeed(Poco::SharedPtr<FloatRange>&& val);

private:
	//@ name=Position
	//@ order=0
	FloatRange _position;

	//@ mandatory=false
	//@ name=Speed
	//@ order=1
	Poco::SharedPtr<FloatRange> _speed;

};


inline const FloatRange& AbsoluteFocusOptions::getPosition() const
{
	return _position;
}


inline const Poco::SharedPtr<FloatRange>& AbsoluteFocusOptions::getSpeed() const
{
	return _speed;
}


inline void AbsoluteFocusOptions::setPosition(const FloatRange& val)
{
	_position = val;
}


inline void AbsoluteFocusOptions::setPosition(FloatRange&& val)
{
	_position = std::move(val);
}


inline void AbsoluteFocusOptions::setSpeed(const Poco::SharedPtr<FloatRange>& val)
{
	_speed = val;
}


inline void AbsoluteFocusOptions::setSpeed(Poco::SharedPtr<FloatRange>&& val)
{
	_speed = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/FloatRange.h"


#endif // ONVIF_AbsoluteFocusOptions_INCLUDED
