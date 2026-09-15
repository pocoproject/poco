// 
// OSDTextConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_OSDTextConfiguration_INCLUDED
#define ONVIF_OSDTextConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class OSDColor;
class OSDTextConfigurationExtension;
} 


namespace ONVIF {


//@ name=OSDTextConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API OSDTextConfiguration
{
public:
	OSDTextConfiguration();

	OSDTextConfiguration(
		Poco::Optional<bool> isPersistentText, 
		const std::string& type, 
		const Poco::Optional<std::string>& dateFormat, 
		const Poco::Optional<std::string>& timeFormat, 
		Poco::Optional<int> fontSize, 
		const Poco::SharedPtr<OSDColor>& fontColor, 
		const Poco::SharedPtr<OSDColor>& backgroundColor, 
		const Poco::Optional<std::string>& plainText, 
		const Poco::SharedPtr<OSDTextConfigurationExtension>& extension);

	virtual ~OSDTextConfiguration();

	const Poco::SharedPtr<OSDColor>& getBackgroundColor() const;

	const Poco::Optional<std::string>& getDateFormat() const;

	const Poco::SharedPtr<OSDTextConfigurationExtension>& getExtension() const;

	const Poco::SharedPtr<OSDColor>& getFontColor() const;

	Poco::Optional<int> getFontSize() const;

	Poco::Optional<bool> getIsPersistentText() const;

	const Poco::Optional<std::string>& getPlainText() const;

	const Poco::Optional<std::string>& getTimeFormat() const;

	const std::string& getType() const;

	void setBackgroundColor(const Poco::SharedPtr<OSDColor>& val);

	void setBackgroundColor(Poco::SharedPtr<OSDColor>&& val);

	void setDateFormat(const Poco::Optional<std::string>& val);

	void setDateFormat(Poco::Optional<std::string>&& val);

	void setExtension(const Poco::SharedPtr<OSDTextConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<OSDTextConfigurationExtension>&& val);

	void setFontColor(const Poco::SharedPtr<OSDColor>& val);

	void setFontColor(Poco::SharedPtr<OSDColor>&& val);

	void setFontSize(Poco::Optional<int> val);

	void setIsPersistentText(Poco::Optional<bool> val);

	void setPlainText(const Poco::Optional<std::string>& val);

	void setPlainText(Poco::Optional<std::string>&& val);

	void setTimeFormat(const Poco::Optional<std::string>& val);

	void setTimeFormat(Poco::Optional<std::string>&& val);

	void setType(const std::string& val);

	void setType(std::string&& val);

private:
	//@ mandatory=false
	//@ name=IsPersistentText
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _isPersistentText;

	//@ name=Type
	//@ order=1
	std::string _type;

	//@ mandatory=false
	//@ name=DateFormat
	//@ order=2
	Poco::Optional<std::string> _dateFormat;

	//@ mandatory=false
	//@ name=TimeFormat
	//@ order=3
	Poco::Optional<std::string> _timeFormat;

	//@ mandatory=false
	//@ name=FontSize
	//@ order=4
	Poco::Optional<int> _fontSize;

	//@ mandatory=false
	//@ name=FontColor
	//@ order=5
	Poco::SharedPtr<OSDColor> _fontColor;

	//@ mandatory=false
	//@ name=BackgroundColor
	//@ order=6
	Poco::SharedPtr<OSDColor> _backgroundColor;

	//@ mandatory=false
	//@ name=PlainText
	//@ order=7
	Poco::Optional<std::string> _plainText;

	//@ mandatory=false
	//@ name=Extension
	//@ order=8
	Poco::SharedPtr<OSDTextConfigurationExtension> _extension;

};


inline const Poco::SharedPtr<OSDColor>& OSDTextConfiguration::getBackgroundColor() const
{
	return _backgroundColor;
}


inline const Poco::Optional<std::string>& OSDTextConfiguration::getDateFormat() const
{
	return _dateFormat;
}


inline const Poco::SharedPtr<OSDTextConfigurationExtension>& OSDTextConfiguration::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<OSDColor>& OSDTextConfiguration::getFontColor() const
{
	return _fontColor;
}


inline Poco::Optional<int> OSDTextConfiguration::getFontSize() const
{
	return _fontSize;
}


inline Poco::Optional<bool> OSDTextConfiguration::getIsPersistentText() const
{
	return _isPersistentText;
}


inline const Poco::Optional<std::string>& OSDTextConfiguration::getPlainText() const
{
	return _plainText;
}


inline const Poco::Optional<std::string>& OSDTextConfiguration::getTimeFormat() const
{
	return _timeFormat;
}


inline const std::string& OSDTextConfiguration::getType() const
{
	return _type;
}


inline void OSDTextConfiguration::setBackgroundColor(const Poco::SharedPtr<OSDColor>& val)
{
	_backgroundColor = val;
}


inline void OSDTextConfiguration::setBackgroundColor(Poco::SharedPtr<OSDColor>&& val)
{
	_backgroundColor = std::move(val);
}


inline void OSDTextConfiguration::setDateFormat(const Poco::Optional<std::string>& val)
{
	_dateFormat = val;
}


inline void OSDTextConfiguration::setDateFormat(Poco::Optional<std::string>&& val)
{
	_dateFormat = std::move(val);
}


inline void OSDTextConfiguration::setExtension(const Poco::SharedPtr<OSDTextConfigurationExtension>& val)
{
	_extension = val;
}


inline void OSDTextConfiguration::setExtension(Poco::SharedPtr<OSDTextConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void OSDTextConfiguration::setFontColor(const Poco::SharedPtr<OSDColor>& val)
{
	_fontColor = val;
}


inline void OSDTextConfiguration::setFontColor(Poco::SharedPtr<OSDColor>&& val)
{
	_fontColor = std::move(val);
}


inline void OSDTextConfiguration::setFontSize(Poco::Optional<int> val)
{
	_fontSize = val;
}


inline void OSDTextConfiguration::setIsPersistentText(Poco::Optional<bool> val)
{
	_isPersistentText = val;
}


inline void OSDTextConfiguration::setPlainText(const Poco::Optional<std::string>& val)
{
	_plainText = val;
}


inline void OSDTextConfiguration::setPlainText(Poco::Optional<std::string>&& val)
{
	_plainText = std::move(val);
}


inline void OSDTextConfiguration::setTimeFormat(const Poco::Optional<std::string>& val)
{
	_timeFormat = val;
}


inline void OSDTextConfiguration::setTimeFormat(Poco::Optional<std::string>&& val)
{
	_timeFormat = std::move(val);
}


inline void OSDTextConfiguration::setType(const std::string& val)
{
	_type = val;
}


inline void OSDTextConfiguration::setType(std::string&& val)
{
	_type = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/OSDColor.h"
#include "ONVIF/OSDTextConfigurationExtension.h"


#endif // ONVIF_OSDTextConfiguration_INCLUDED
