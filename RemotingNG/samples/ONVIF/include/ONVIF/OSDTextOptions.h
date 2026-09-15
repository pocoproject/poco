// 
// OSDTextOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_OSDTextOptions_INCLUDED
#define ONVIF_OSDTextOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class IntRange;
class OSDColorOptions;
class OSDTextOptionsExtension;
} 


namespace ONVIF {


//@ name=OSDTextOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API OSDTextOptions
{
public:
	OSDTextOptions();

	OSDTextOptions(
		const std::vector<std::string>& type, 
		const Poco::SharedPtr<IntRange>& fontSizeRange, 
		const std::vector<std::string>& dateFormat, 
		const std::vector<std::string>& timeFormat, 
		const Poco::SharedPtr<OSDColorOptions>& fontColor, 
		const Poco::SharedPtr<OSDColorOptions>& backgroundColor, 
		const Poco::SharedPtr<OSDTextOptionsExtension>& extension);

	virtual ~OSDTextOptions();

	const Poco::SharedPtr<OSDColorOptions>& getBackgroundColor() const;

	const std::vector<std::string>& getDateFormat() const;

	std::vector<std::string>& getDateFormat();

	const Poco::SharedPtr<OSDTextOptionsExtension>& getExtension() const;

	const Poco::SharedPtr<OSDColorOptions>& getFontColor() const;

	const Poco::SharedPtr<IntRange>& getFontSizeRange() const;

	const std::vector<std::string>& getTimeFormat() const;

	std::vector<std::string>& getTimeFormat();

	const std::vector<std::string>& getType() const;

	std::vector<std::string>& getType();

	void setBackgroundColor(const Poco::SharedPtr<OSDColorOptions>& val);

	void setBackgroundColor(Poco::SharedPtr<OSDColorOptions>&& val);

	void setDateFormat(const std::vector<std::string>& val);

	void setDateFormat(std::vector<std::string>&& val);

	void setExtension(const Poco::SharedPtr<OSDTextOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<OSDTextOptionsExtension>&& val);

	void setFontColor(const Poco::SharedPtr<OSDColorOptions>& val);

	void setFontColor(Poco::SharedPtr<OSDColorOptions>&& val);

	void setFontSizeRange(const Poco::SharedPtr<IntRange>& val);

	void setFontSizeRange(Poco::SharedPtr<IntRange>&& val);

	void setTimeFormat(const std::vector<std::string>& val);

	void setTimeFormat(std::vector<std::string>&& val);

	void setType(const std::vector<std::string>& val);

	void setType(std::vector<std::string>&& val);

private:
	//@ name=Type
	//@ order=0
	std::vector<std::string> _type;

	//@ mandatory=false
	//@ name=FontSizeRange
	//@ order=1
	Poco::SharedPtr<IntRange> _fontSizeRange;

	//@ mandatory=false
	//@ name=DateFormat
	//@ order=2
	std::vector<std::string> _dateFormat;

	//@ mandatory=false
	//@ name=TimeFormat
	//@ order=3
	std::vector<std::string> _timeFormat;

	//@ mandatory=false
	//@ name=FontColor
	//@ order=4
	Poco::SharedPtr<OSDColorOptions> _fontColor;

	//@ mandatory=false
	//@ name=BackgroundColor
	//@ order=5
	Poco::SharedPtr<OSDColorOptions> _backgroundColor;

	//@ mandatory=false
	//@ name=Extension
	//@ order=6
	Poco::SharedPtr<OSDTextOptionsExtension> _extension;

};


inline const Poco::SharedPtr<OSDColorOptions>& OSDTextOptions::getBackgroundColor() const
{
	return _backgroundColor;
}


inline const std::vector<std::string>& OSDTextOptions::getDateFormat() const
{
	return _dateFormat;
}


inline std::vector<std::string>& OSDTextOptions::getDateFormat()
{
	return _dateFormat;
}


inline const Poco::SharedPtr<OSDTextOptionsExtension>& OSDTextOptions::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<OSDColorOptions>& OSDTextOptions::getFontColor() const
{
	return _fontColor;
}


inline const Poco::SharedPtr<IntRange>& OSDTextOptions::getFontSizeRange() const
{
	return _fontSizeRange;
}


inline const std::vector<std::string>& OSDTextOptions::getTimeFormat() const
{
	return _timeFormat;
}


inline std::vector<std::string>& OSDTextOptions::getTimeFormat()
{
	return _timeFormat;
}


inline const std::vector<std::string>& OSDTextOptions::getType() const
{
	return _type;
}


inline std::vector<std::string>& OSDTextOptions::getType()
{
	return _type;
}


inline void OSDTextOptions::setBackgroundColor(const Poco::SharedPtr<OSDColorOptions>& val)
{
	_backgroundColor = val;
}


inline void OSDTextOptions::setBackgroundColor(Poco::SharedPtr<OSDColorOptions>&& val)
{
	_backgroundColor = std::move(val);
}


inline void OSDTextOptions::setDateFormat(const std::vector<std::string>& val)
{
	_dateFormat = val;
}


inline void OSDTextOptions::setDateFormat(std::vector<std::string>&& val)
{
	_dateFormat = std::move(val);
}


inline void OSDTextOptions::setExtension(const Poco::SharedPtr<OSDTextOptionsExtension>& val)
{
	_extension = val;
}


inline void OSDTextOptions::setExtension(Poco::SharedPtr<OSDTextOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void OSDTextOptions::setFontColor(const Poco::SharedPtr<OSDColorOptions>& val)
{
	_fontColor = val;
}


inline void OSDTextOptions::setFontColor(Poco::SharedPtr<OSDColorOptions>&& val)
{
	_fontColor = std::move(val);
}


inline void OSDTextOptions::setFontSizeRange(const Poco::SharedPtr<IntRange>& val)
{
	_fontSizeRange = val;
}


inline void OSDTextOptions::setFontSizeRange(Poco::SharedPtr<IntRange>&& val)
{
	_fontSizeRange = std::move(val);
}


inline void OSDTextOptions::setTimeFormat(const std::vector<std::string>& val)
{
	_timeFormat = val;
}


inline void OSDTextOptions::setTimeFormat(std::vector<std::string>&& val)
{
	_timeFormat = std::move(val);
}


inline void OSDTextOptions::setType(const std::vector<std::string>& val)
{
	_type = val;
}


inline void OSDTextOptions::setType(std::vector<std::string>&& val)
{
	_type = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/IntRange.h"
#include "ONVIF/OSDColorOptions.h"
#include "ONVIF/OSDTextOptionsExtension.h"


#endif // ONVIF_OSDTextOptions_INCLUDED
