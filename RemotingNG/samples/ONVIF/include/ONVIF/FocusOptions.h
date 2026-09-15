// 
// FocusOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FocusOptions_INCLUDED
#define ONVIF_FocusOptions_INCLUDED


#include "ONVIF/FloatRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FocusOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FocusOptions
{
public:
	FocusOptions();

	FocusOptions(
		const std::vector<std::string>& autoFocusModes, 
		const FloatRange& defaultSpeed, 
		const FloatRange& nearLimit, 
		const FloatRange& farLimit);

	virtual ~FocusOptions();

	const std::vector<std::string>& getAutoFocusModes() const;

	std::vector<std::string>& getAutoFocusModes();

	const FloatRange& getDefaultSpeed() const;

	const FloatRange& getFarLimit() const;

	const FloatRange& getNearLimit() const;

	void setAutoFocusModes(const std::vector<std::string>& val);

	void setAutoFocusModes(std::vector<std::string>&& val);

	void setDefaultSpeed(const FloatRange& val);

	void setDefaultSpeed(FloatRange&& val);

	void setFarLimit(const FloatRange& val);

	void setFarLimit(FloatRange&& val);

	void setNearLimit(const FloatRange& val);

	void setNearLimit(FloatRange&& val);

private:
	//@ mandatory=false
	//@ name=AutoFocusModes
	//@ order=0
	std::vector<std::string> _autoFocusModes;

	//@ name=DefaultSpeed
	//@ order=1
	FloatRange _defaultSpeed;

	//@ name=NearLimit
	//@ order=2
	FloatRange _nearLimit;

	//@ name=FarLimit
	//@ order=3
	FloatRange _farLimit;

};


inline const std::vector<std::string>& FocusOptions::getAutoFocusModes() const
{
	return _autoFocusModes;
}


inline std::vector<std::string>& FocusOptions::getAutoFocusModes()
{
	return _autoFocusModes;
}


inline const FloatRange& FocusOptions::getDefaultSpeed() const
{
	return _defaultSpeed;
}


inline const FloatRange& FocusOptions::getFarLimit() const
{
	return _farLimit;
}


inline const FloatRange& FocusOptions::getNearLimit() const
{
	return _nearLimit;
}


inline void FocusOptions::setAutoFocusModes(const std::vector<std::string>& val)
{
	_autoFocusModes = val;
}


inline void FocusOptions::setAutoFocusModes(std::vector<std::string>&& val)
{
	_autoFocusModes = std::move(val);
}


inline void FocusOptions::setDefaultSpeed(const FloatRange& val)
{
	_defaultSpeed = val;
}


inline void FocusOptions::setDefaultSpeed(FloatRange&& val)
{
	_defaultSpeed = std::move(val);
}


inline void FocusOptions::setFarLimit(const FloatRange& val)
{
	_farLimit = val;
}


inline void FocusOptions::setFarLimit(FloatRange&& val)
{
	_farLimit = std::move(val);
}


inline void FocusOptions::setNearLimit(const FloatRange& val)
{
	_nearLimit = val;
}


inline void FocusOptions::setNearLimit(FloatRange&& val)
{
	_nearLimit = std::move(val);
}


} // ONVIF


#endif // ONVIF_FocusOptions_INCLUDED
