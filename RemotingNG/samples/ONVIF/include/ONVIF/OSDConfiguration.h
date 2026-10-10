// 
// OSDConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_OSDConfiguration_INCLUDED
#define ONVIF_OSDConfiguration_INCLUDED


#include "ONVIF/DeviceEntity.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/OSDPosConfiguration.h"
#include "ONVIF/OSDReference.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class OSDConfigurationExtension;
class OSDImgConfiguration;
class OSDTextConfiguration;
} 


namespace ONVIF {


//@ name=OSDConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API OSDConfiguration: public DeviceEntity
{
public:
	OSDConfiguration();

	OSDConfiguration(
		const std::string& token, 
		const OSDReference& videoSourceConfigurationToken, 
		const std::string& type, 
		const OSDPosConfiguration& position, 
		const Poco::SharedPtr<OSDTextConfiguration>& textString, 
		const Poco::SharedPtr<OSDImgConfiguration>& image, 
		const Poco::SharedPtr<OSDConfigurationExtension>& extension);

	virtual ~OSDConfiguration();

	const Poco::SharedPtr<OSDConfigurationExtension>& getExtension() const;

	const Poco::SharedPtr<OSDImgConfiguration>& getImage() const;

	const OSDPosConfiguration& getPosition() const;

	const Poco::SharedPtr<OSDTextConfiguration>& getTextString() const;

	const std::string& getType() const;

	const OSDReference& getVideoSourceConfigurationToken() const;

	void setExtension(const Poco::SharedPtr<OSDConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<OSDConfigurationExtension>&& val);

	void setImage(const Poco::SharedPtr<OSDImgConfiguration>& val);

	void setImage(Poco::SharedPtr<OSDImgConfiguration>&& val);

	void setPosition(const OSDPosConfiguration& val);

	void setPosition(OSDPosConfiguration&& val);

	void setTextString(const Poco::SharedPtr<OSDTextConfiguration>& val);

	void setTextString(Poco::SharedPtr<OSDTextConfiguration>&& val);

	void setType(const std::string& val);

	void setType(std::string&& val);

	void setVideoSourceConfigurationToken(const OSDReference& val);

	void setVideoSourceConfigurationToken(OSDReference&& val);

private:
	//@ name=VideoSourceConfigurationToken
	//@ order=0
	OSDReference _videoSourceConfigurationToken;

	//@ name=Type
	//@ order=1
	std::string _type;

	//@ name=Position
	//@ order=2
	OSDPosConfiguration _position;

	//@ mandatory=false
	//@ name=TextString
	//@ order=3
	Poco::SharedPtr<OSDTextConfiguration> _textString;

	//@ mandatory=false
	//@ name=Image
	//@ order=4
	Poco::SharedPtr<OSDImgConfiguration> _image;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<OSDConfigurationExtension> _extension;

};


inline const Poco::SharedPtr<OSDConfigurationExtension>& OSDConfiguration::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<OSDImgConfiguration>& OSDConfiguration::getImage() const
{
	return _image;
}


inline const OSDPosConfiguration& OSDConfiguration::getPosition() const
{
	return _position;
}


inline const Poco::SharedPtr<OSDTextConfiguration>& OSDConfiguration::getTextString() const
{
	return _textString;
}


inline const std::string& OSDConfiguration::getType() const
{
	return _type;
}


inline const OSDReference& OSDConfiguration::getVideoSourceConfigurationToken() const
{
	return _videoSourceConfigurationToken;
}


inline void OSDConfiguration::setExtension(const Poco::SharedPtr<OSDConfigurationExtension>& val)
{
	_extension = val;
}


inline void OSDConfiguration::setExtension(Poco::SharedPtr<OSDConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void OSDConfiguration::setImage(const Poco::SharedPtr<OSDImgConfiguration>& val)
{
	_image = val;
}


inline void OSDConfiguration::setImage(Poco::SharedPtr<OSDImgConfiguration>&& val)
{
	_image = std::move(val);
}


inline void OSDConfiguration::setPosition(const OSDPosConfiguration& val)
{
	_position = val;
}


inline void OSDConfiguration::setPosition(OSDPosConfiguration&& val)
{
	_position = std::move(val);
}


inline void OSDConfiguration::setTextString(const Poco::SharedPtr<OSDTextConfiguration>& val)
{
	_textString = val;
}


inline void OSDConfiguration::setTextString(Poco::SharedPtr<OSDTextConfiguration>&& val)
{
	_textString = std::move(val);
}


inline void OSDConfiguration::setType(const std::string& val)
{
	_type = val;
}


inline void OSDConfiguration::setType(std::string&& val)
{
	_type = std::move(val);
}


inline void OSDConfiguration::setVideoSourceConfigurationToken(const OSDReference& val)
{
	_videoSourceConfigurationToken = val;
}


inline void OSDConfiguration::setVideoSourceConfigurationToken(OSDReference&& val)
{
	_videoSourceConfigurationToken = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/OSDConfigurationExtension.h"
#include "ONVIF/OSDImgConfiguration.h"
#include "ONVIF/OSDTextConfiguration.h"


#endif // ONVIF_OSDConfiguration_INCLUDED
