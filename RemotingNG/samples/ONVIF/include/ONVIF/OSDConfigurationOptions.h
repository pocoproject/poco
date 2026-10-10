// 
// OSDConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_OSDConfigurationOptions_INCLUDED
#define ONVIF_OSDConfigurationOptions_INCLUDED


#include "ONVIF/MaximumNumberOfOSDs.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class OSDConfigurationOptionsExtension;
class OSDImgOptions;
class OSDTextOptions;
} 


namespace ONVIF {


//@ name=OSDConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API OSDConfigurationOptions
{
public:
	OSDConfigurationOptions();

	OSDConfigurationOptions(
		const MaximumNumberOfOSDs& maximumNumberOfOSDs, 
		const std::vector<std::string>& type, 
		const std::vector<std::string>& positionOption, 
		const Poco::SharedPtr<OSDTextOptions>& textOption, 
		const Poco::SharedPtr<OSDImgOptions>& imageOption, 
		const Poco::SharedPtr<OSDConfigurationOptionsExtension>& extension);

	virtual ~OSDConfigurationOptions();

	const Poco::SharedPtr<OSDConfigurationOptionsExtension>& getExtension() const;

	const Poco::SharedPtr<OSDImgOptions>& getImageOption() const;

	const MaximumNumberOfOSDs& getMaximumNumberOfOSDs() const;

	const std::vector<std::string>& getPositionOption() const;

	std::vector<std::string>& getPositionOption();

	const Poco::SharedPtr<OSDTextOptions>& getTextOption() const;

	const std::vector<std::string>& getType() const;

	std::vector<std::string>& getType();

	void setExtension(const Poco::SharedPtr<OSDConfigurationOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<OSDConfigurationOptionsExtension>&& val);

	void setImageOption(const Poco::SharedPtr<OSDImgOptions>& val);

	void setImageOption(Poco::SharedPtr<OSDImgOptions>&& val);

	void setMaximumNumberOfOSDs(const MaximumNumberOfOSDs& val);

	void setMaximumNumberOfOSDs(MaximumNumberOfOSDs&& val);

	void setPositionOption(const std::vector<std::string>& val);

	void setPositionOption(std::vector<std::string>&& val);

	void setTextOption(const Poco::SharedPtr<OSDTextOptions>& val);

	void setTextOption(Poco::SharedPtr<OSDTextOptions>&& val);

	void setType(const std::vector<std::string>& val);

	void setType(std::vector<std::string>&& val);

private:
	//@ name=MaximumNumberOfOSDs
	//@ order=0
	MaximumNumberOfOSDs _maximumNumberOfOSDs;

	//@ name=Type
	//@ order=1
	std::vector<std::string> _type;

	//@ name=PositionOption
	//@ order=2
	std::vector<std::string> _positionOption;

	//@ mandatory=false
	//@ name=TextOption
	//@ order=3
	Poco::SharedPtr<OSDTextOptions> _textOption;

	//@ mandatory=false
	//@ name=ImageOption
	//@ order=4
	Poco::SharedPtr<OSDImgOptions> _imageOption;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<OSDConfigurationOptionsExtension> _extension;

};


inline const Poco::SharedPtr<OSDConfigurationOptionsExtension>& OSDConfigurationOptions::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<OSDImgOptions>& OSDConfigurationOptions::getImageOption() const
{
	return _imageOption;
}


inline const MaximumNumberOfOSDs& OSDConfigurationOptions::getMaximumNumberOfOSDs() const
{
	return _maximumNumberOfOSDs;
}


inline const std::vector<std::string>& OSDConfigurationOptions::getPositionOption() const
{
	return _positionOption;
}


inline std::vector<std::string>& OSDConfigurationOptions::getPositionOption()
{
	return _positionOption;
}


inline const Poco::SharedPtr<OSDTextOptions>& OSDConfigurationOptions::getTextOption() const
{
	return _textOption;
}


inline const std::vector<std::string>& OSDConfigurationOptions::getType() const
{
	return _type;
}


inline std::vector<std::string>& OSDConfigurationOptions::getType()
{
	return _type;
}


inline void OSDConfigurationOptions::setExtension(const Poco::SharedPtr<OSDConfigurationOptionsExtension>& val)
{
	_extension = val;
}


inline void OSDConfigurationOptions::setExtension(Poco::SharedPtr<OSDConfigurationOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void OSDConfigurationOptions::setImageOption(const Poco::SharedPtr<OSDImgOptions>& val)
{
	_imageOption = val;
}


inline void OSDConfigurationOptions::setImageOption(Poco::SharedPtr<OSDImgOptions>&& val)
{
	_imageOption = std::move(val);
}


inline void OSDConfigurationOptions::setMaximumNumberOfOSDs(const MaximumNumberOfOSDs& val)
{
	_maximumNumberOfOSDs = val;
}


inline void OSDConfigurationOptions::setMaximumNumberOfOSDs(MaximumNumberOfOSDs&& val)
{
	_maximumNumberOfOSDs = std::move(val);
}


inline void OSDConfigurationOptions::setPositionOption(const std::vector<std::string>& val)
{
	_positionOption = val;
}


inline void OSDConfigurationOptions::setPositionOption(std::vector<std::string>&& val)
{
	_positionOption = std::move(val);
}


inline void OSDConfigurationOptions::setTextOption(const Poco::SharedPtr<OSDTextOptions>& val)
{
	_textOption = val;
}


inline void OSDConfigurationOptions::setTextOption(Poco::SharedPtr<OSDTextOptions>&& val)
{
	_textOption = std::move(val);
}


inline void OSDConfigurationOptions::setType(const std::vector<std::string>& val)
{
	_type = val;
}


inline void OSDConfigurationOptions::setType(std::vector<std::string>&& val)
{
	_type = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/OSDConfigurationOptionsExtension.h"
#include "ONVIF/OSDImgOptions.h"
#include "ONVIF/OSDTextOptions.h"


#endif // ONVIF_OSDConfigurationOptions_INCLUDED
