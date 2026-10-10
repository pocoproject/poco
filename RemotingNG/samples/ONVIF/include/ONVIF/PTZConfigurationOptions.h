// 
// PTZConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZConfigurationOptions_INCLUDED
#define ONVIF_PTZConfigurationOptions_INCLUDED


#include "ONVIF/DurationRange.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/PTZSpaces.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTControlDirectionOptions;
class PTZConfigurationOptions2;
} 


namespace ONVIF {


//@ name=PTZConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZConfigurationOptions
{
public:
	PTZConfigurationOptions();

	PTZConfigurationOptions(
		const std::vector<std::vector<int>>& pTZRamps, 
		const PTZSpaces& spaces, 
		const DurationRange& pTZTimeout, 
		const Poco::SharedPtr<PTControlDirectionOptions>& pTControlDirection, 
		const Poco::SharedPtr<PTZConfigurationOptions2>& extension);

	virtual ~PTZConfigurationOptions();

	const Poco::SharedPtr<PTZConfigurationOptions2>& getExtension() const;

	const Poco::SharedPtr<PTControlDirectionOptions>& getPTControlDirection() const;

	const std::vector<std::vector<int>>& getPTZRamps() const;

	std::vector<std::vector<int>>& getPTZRamps();

	const DurationRange& getPTZTimeout() const;

	const PTZSpaces& getSpaces() const;

	void setExtension(const Poco::SharedPtr<PTZConfigurationOptions2>& val);

	void setExtension(Poco::SharedPtr<PTZConfigurationOptions2>&& val);

	void setPTControlDirection(const Poco::SharedPtr<PTControlDirectionOptions>& val);

	void setPTControlDirection(Poco::SharedPtr<PTControlDirectionOptions>&& val);

	void setPTZRamps(const std::vector<std::vector<int>>& val);

	void setPTZRamps(std::vector<std::vector<int>>&& val);

	void setPTZTimeout(const DurationRange& val);

	void setPTZTimeout(DurationRange&& val);

	void setSpaces(const PTZSpaces& val);

	void setSpaces(PTZSpaces&& val);

private:
	//@ mandatory=false
	//@ name=PTZRamps
	//@ order=0
	//@ type=attr
	std::vector<std::vector<int>> _pTZRamps;

	//@ name=Spaces
	//@ order=1
	PTZSpaces _spaces;

	//@ name=PTZTimeout
	//@ order=2
	DurationRange _pTZTimeout;

	//@ mandatory=false
	//@ name=PTControlDirection
	//@ order=3
	Poco::SharedPtr<PTControlDirectionOptions> _pTControlDirection;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<PTZConfigurationOptions2> _extension;

};


inline const Poco::SharedPtr<PTZConfigurationOptions2>& PTZConfigurationOptions::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<PTControlDirectionOptions>& PTZConfigurationOptions::getPTControlDirection() const
{
	return _pTControlDirection;
}


inline const std::vector<std::vector<int>>& PTZConfigurationOptions::getPTZRamps() const
{
	return _pTZRamps;
}


inline std::vector<std::vector<int>>& PTZConfigurationOptions::getPTZRamps()
{
	return _pTZRamps;
}


inline const DurationRange& PTZConfigurationOptions::getPTZTimeout() const
{
	return _pTZTimeout;
}


inline const PTZSpaces& PTZConfigurationOptions::getSpaces() const
{
	return _spaces;
}


inline void PTZConfigurationOptions::setExtension(const Poco::SharedPtr<PTZConfigurationOptions2>& val)
{
	_extension = val;
}


inline void PTZConfigurationOptions::setExtension(Poco::SharedPtr<PTZConfigurationOptions2>&& val)
{
	_extension = std::move(val);
}


inline void PTZConfigurationOptions::setPTControlDirection(const Poco::SharedPtr<PTControlDirectionOptions>& val)
{
	_pTControlDirection = val;
}


inline void PTZConfigurationOptions::setPTControlDirection(Poco::SharedPtr<PTControlDirectionOptions>&& val)
{
	_pTControlDirection = std::move(val);
}


inline void PTZConfigurationOptions::setPTZRamps(const std::vector<std::vector<int>>& val)
{
	_pTZRamps = val;
}


inline void PTZConfigurationOptions::setPTZRamps(std::vector<std::vector<int>>&& val)
{
	_pTZRamps = std::move(val);
}


inline void PTZConfigurationOptions::setPTZTimeout(const DurationRange& val)
{
	_pTZTimeout = val;
}


inline void PTZConfigurationOptions::setPTZTimeout(DurationRange&& val)
{
	_pTZTimeout = std::move(val);
}


inline void PTZConfigurationOptions::setSpaces(const PTZSpaces& val)
{
	_spaces = val;
}


inline void PTZConfigurationOptions::setSpaces(PTZSpaces&& val)
{
	_spaces = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTControlDirectionOptions.h"
#include "ONVIF/PTZConfigurationOptions2.h"


#endif // ONVIF_PTZConfigurationOptions_INCLUDED
