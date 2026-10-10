// 
// FocusOptions20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FocusOptions20_INCLUDED
#define ONVIF_FocusOptions20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class FloatRange;
class FocusOptions20Extension;
} 


namespace ONVIF {


//@ name=FocusOptions20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FocusOptions20
{
public:
	FocusOptions20();

	FocusOptions20(
		const std::vector<std::string>& autoFocusModes, 
		const Poco::SharedPtr<FloatRange>& defaultSpeed, 
		const Poco::SharedPtr<FloatRange>& nearLimit, 
		const Poco::SharedPtr<FloatRange>& farLimit, 
		const Poco::SharedPtr<FocusOptions20Extension>& extension);

	virtual ~FocusOptions20();

	const std::vector<std::string>& getAutoFocusModes() const;

	std::vector<std::string>& getAutoFocusModes();

	const Poco::SharedPtr<FloatRange>& getDefaultSpeed() const;

	const Poco::SharedPtr<FocusOptions20Extension>& getExtension() const;

	const Poco::SharedPtr<FloatRange>& getFarLimit() const;

	const Poco::SharedPtr<FloatRange>& getNearLimit() const;

	void setAutoFocusModes(const std::vector<std::string>& val);

	void setAutoFocusModes(std::vector<std::string>&& val);

	void setDefaultSpeed(const Poco::SharedPtr<FloatRange>& val);

	void setDefaultSpeed(Poco::SharedPtr<FloatRange>&& val);

	void setExtension(const Poco::SharedPtr<FocusOptions20Extension>& val);

	void setExtension(Poco::SharedPtr<FocusOptions20Extension>&& val);

	void setFarLimit(const Poco::SharedPtr<FloatRange>& val);

	void setFarLimit(Poco::SharedPtr<FloatRange>&& val);

	void setNearLimit(const Poco::SharedPtr<FloatRange>& val);

	void setNearLimit(Poco::SharedPtr<FloatRange>&& val);

private:
	//@ mandatory=false
	//@ name=AutoFocusModes
	//@ order=0
	std::vector<std::string> _autoFocusModes;

	//@ mandatory=false
	//@ name=DefaultSpeed
	//@ order=1
	Poco::SharedPtr<FloatRange> _defaultSpeed;

	//@ mandatory=false
	//@ name=NearLimit
	//@ order=2
	Poco::SharedPtr<FloatRange> _nearLimit;

	//@ mandatory=false
	//@ name=FarLimit
	//@ order=3
	Poco::SharedPtr<FloatRange> _farLimit;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<FocusOptions20Extension> _extension;

};


inline const std::vector<std::string>& FocusOptions20::getAutoFocusModes() const
{
	return _autoFocusModes;
}


inline std::vector<std::string>& FocusOptions20::getAutoFocusModes()
{
	return _autoFocusModes;
}


inline const Poco::SharedPtr<FloatRange>& FocusOptions20::getDefaultSpeed() const
{
	return _defaultSpeed;
}


inline const Poco::SharedPtr<FocusOptions20Extension>& FocusOptions20::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<FloatRange>& FocusOptions20::getFarLimit() const
{
	return _farLimit;
}


inline const Poco::SharedPtr<FloatRange>& FocusOptions20::getNearLimit() const
{
	return _nearLimit;
}


inline void FocusOptions20::setAutoFocusModes(const std::vector<std::string>& val)
{
	_autoFocusModes = val;
}


inline void FocusOptions20::setAutoFocusModes(std::vector<std::string>&& val)
{
	_autoFocusModes = std::move(val);
}


inline void FocusOptions20::setDefaultSpeed(const Poco::SharedPtr<FloatRange>& val)
{
	_defaultSpeed = val;
}


inline void FocusOptions20::setDefaultSpeed(Poco::SharedPtr<FloatRange>&& val)
{
	_defaultSpeed = std::move(val);
}


inline void FocusOptions20::setExtension(const Poco::SharedPtr<FocusOptions20Extension>& val)
{
	_extension = val;
}


inline void FocusOptions20::setExtension(Poco::SharedPtr<FocusOptions20Extension>&& val)
{
	_extension = std::move(val);
}


inline void FocusOptions20::setFarLimit(const Poco::SharedPtr<FloatRange>& val)
{
	_farLimit = val;
}


inline void FocusOptions20::setFarLimit(Poco::SharedPtr<FloatRange>&& val)
{
	_farLimit = std::move(val);
}


inline void FocusOptions20::setNearLimit(const Poco::SharedPtr<FloatRange>& val)
{
	_nearLimit = val;
}


inline void FocusOptions20::setNearLimit(Poco::SharedPtr<FloatRange>&& val)
{
	_nearLimit = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/FloatRange.h"
#include "ONVIF/FocusOptions20Extension.h"


#endif // ONVIF_FocusOptions20_INCLUDED
