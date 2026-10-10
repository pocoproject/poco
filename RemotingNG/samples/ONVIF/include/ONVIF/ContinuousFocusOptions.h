// 
// ContinuousFocusOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ContinuousFocusOptions_INCLUDED
#define ONVIF_ContinuousFocusOptions_INCLUDED


#include "ONVIF/FloatRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ContinuousFocusOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ContinuousFocusOptions
{
public:
	ContinuousFocusOptions();

	ContinuousFocusOptions(const FloatRange& speed);

	virtual ~ContinuousFocusOptions();

	const FloatRange& getSpeed() const;

	void setSpeed(const FloatRange& val);

	void setSpeed(FloatRange&& val);

private:
	//@ name=Speed
	//@ order=0
	FloatRange _speed;

};


inline const FloatRange& ContinuousFocusOptions::getSpeed() const
{
	return _speed;
}


inline void ContinuousFocusOptions::setSpeed(const FloatRange& val)
{
	_speed = val;
}


inline void ContinuousFocusOptions::setSpeed(FloatRange&& val)
{
	_speed = std::move(val);
}


} // ONVIF


#endif // ONVIF_ContinuousFocusOptions_INCLUDED
