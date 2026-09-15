// 
// FocusConfiguration20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FocusConfiguration20_INCLUDED
#define ONVIF_FocusConfiguration20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class FocusConfiguration20Extension;
} 


namespace ONVIF {


//@ name=FocusConfiguration20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FocusConfiguration20
{
public:
	FocusConfiguration20();

	FocusConfiguration20(
		const std::vector<std::vector<std::string>>& aFMode, 
		const std::string& autoFocusMode, 
		Poco::Optional<float> defaultSpeed, 
		Poco::Optional<float> nearLimit, 
		Poco::Optional<float> farLimit, 
		const Poco::SharedPtr<FocusConfiguration20Extension>& extension);

	virtual ~FocusConfiguration20();

	const std::vector<std::vector<std::string>>& getAFMode() const;

	std::vector<std::vector<std::string>>& getAFMode();

	const std::string& getAutoFocusMode() const;

	Poco::Optional<float> getDefaultSpeed() const;

	const Poco::SharedPtr<FocusConfiguration20Extension>& getExtension() const;

	Poco::Optional<float> getFarLimit() const;

	Poco::Optional<float> getNearLimit() const;

	void setAFMode(const std::vector<std::vector<std::string>>& val);

	void setAFMode(std::vector<std::vector<std::string>>&& val);

	void setAutoFocusMode(const std::string& val);

	void setAutoFocusMode(std::string&& val);

	void setDefaultSpeed(Poco::Optional<float> val);

	void setExtension(const Poco::SharedPtr<FocusConfiguration20Extension>& val);

	void setExtension(Poco::SharedPtr<FocusConfiguration20Extension>&& val);

	void setFarLimit(Poco::Optional<float> val);

	void setNearLimit(Poco::Optional<float> val);

private:
	//@ mandatory=false
	//@ name=AFMode
	//@ order=0
	//@ type=attr
	std::vector<std::vector<std::string>> _aFMode;

	//@ name=AutoFocusMode
	//@ order=1
	std::string _autoFocusMode;

	//@ mandatory=false
	//@ name=DefaultSpeed
	//@ order=2
	Poco::Optional<float> _defaultSpeed;

	//@ mandatory=false
	//@ name=NearLimit
	//@ order=3
	Poco::Optional<float> _nearLimit;

	//@ mandatory=false
	//@ name=FarLimit
	//@ order=4
	Poco::Optional<float> _farLimit;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<FocusConfiguration20Extension> _extension;

};


inline const std::vector<std::vector<std::string>>& FocusConfiguration20::getAFMode() const
{
	return _aFMode;
}


inline std::vector<std::vector<std::string>>& FocusConfiguration20::getAFMode()
{
	return _aFMode;
}


inline const std::string& FocusConfiguration20::getAutoFocusMode() const
{
	return _autoFocusMode;
}


inline Poco::Optional<float> FocusConfiguration20::getDefaultSpeed() const
{
	return _defaultSpeed;
}


inline const Poco::SharedPtr<FocusConfiguration20Extension>& FocusConfiguration20::getExtension() const
{
	return _extension;
}


inline Poco::Optional<float> FocusConfiguration20::getFarLimit() const
{
	return _farLimit;
}


inline Poco::Optional<float> FocusConfiguration20::getNearLimit() const
{
	return _nearLimit;
}


inline void FocusConfiguration20::setAFMode(const std::vector<std::vector<std::string>>& val)
{
	_aFMode = val;
}


inline void FocusConfiguration20::setAFMode(std::vector<std::vector<std::string>>&& val)
{
	_aFMode = std::move(val);
}


inline void FocusConfiguration20::setAutoFocusMode(const std::string& val)
{
	_autoFocusMode = val;
}


inline void FocusConfiguration20::setAutoFocusMode(std::string&& val)
{
	_autoFocusMode = std::move(val);
}


inline void FocusConfiguration20::setDefaultSpeed(Poco::Optional<float> val)
{
	_defaultSpeed = val;
}


inline void FocusConfiguration20::setExtension(const Poco::SharedPtr<FocusConfiguration20Extension>& val)
{
	_extension = val;
}


inline void FocusConfiguration20::setExtension(Poco::SharedPtr<FocusConfiguration20Extension>&& val)
{
	_extension = std::move(val);
}


inline void FocusConfiguration20::setFarLimit(Poco::Optional<float> val)
{
	_farLimit = val;
}


inline void FocusConfiguration20::setNearLimit(Poco::Optional<float> val)
{
	_nearLimit = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/FocusConfiguration20Extension.h"


#endif // ONVIF_FocusConfiguration20_INCLUDED
