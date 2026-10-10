// 
// FocusConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FocusConfiguration_INCLUDED
#define ONVIF_FocusConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FocusConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FocusConfiguration
{
public:
	FocusConfiguration();

	FocusConfiguration(
		const std::string& autoFocusMode, 
		float defaultSpeed, 
		float nearLimit, 
		float farLimit);

	virtual ~FocusConfiguration();

	const std::string& getAutoFocusMode() const;

	float getDefaultSpeed() const;

	float getFarLimit() const;

	float getNearLimit() const;

	void setAutoFocusMode(const std::string& val);

	void setAutoFocusMode(std::string&& val);

	void setDefaultSpeed(float val);

	void setFarLimit(float val);

	void setNearLimit(float val);

private:
	//@ name=AutoFocusMode
	//@ order=0
	std::string _autoFocusMode;

	//@ name=DefaultSpeed
	//@ order=1
	float _defaultSpeed;

	//@ name=NearLimit
	//@ order=2
	float _nearLimit;

	//@ name=FarLimit
	//@ order=3
	float _farLimit;

};


inline const std::string& FocusConfiguration::getAutoFocusMode() const
{
	return _autoFocusMode;
}


inline float FocusConfiguration::getDefaultSpeed() const
{
	return _defaultSpeed;
}


inline float FocusConfiguration::getFarLimit() const
{
	return _farLimit;
}


inline float FocusConfiguration::getNearLimit() const
{
	return _nearLimit;
}


inline void FocusConfiguration::setAutoFocusMode(const std::string& val)
{
	_autoFocusMode = val;
}


inline void FocusConfiguration::setAutoFocusMode(std::string&& val)
{
	_autoFocusMode = std::move(val);
}


inline void FocusConfiguration::setDefaultSpeed(float val)
{
	_defaultSpeed = val;
}


inline void FocusConfiguration::setFarLimit(float val)
{
	_farLimit = val;
}


inline void FocusConfiguration::setNearLimit(float val)
{
	_nearLimit = val;
}


} // ONVIF


#endif // ONVIF_FocusConfiguration_INCLUDED
