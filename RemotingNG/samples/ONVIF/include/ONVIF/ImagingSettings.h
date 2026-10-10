// 
// ImagingSettings.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingSettings_INCLUDED
#define ONVIF_ImagingSettings_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class BacklightCompensation;
class Exposure;
class FocusConfiguration;
class ImagingSettingsExtension;
class WhiteBalance;
class WideDynamicRange;
} 


namespace ONVIF {


//@ name=ImagingSettings
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingSettings
{
public:
	ImagingSettings();

	ImagingSettings(
		const Poco::SharedPtr<BacklightCompensation>& backlightCompensation, 
		Poco::Optional<float> brightness, 
		Poco::Optional<float> colorSaturation, 
		Poco::Optional<float> contrast, 
		const Poco::SharedPtr<Exposure>& exposure, 
		const Poco::SharedPtr<FocusConfiguration>& focus, 
		const Poco::Optional<std::string>& irCutFilter, 
		Poco::Optional<float> sharpness, 
		const Poco::SharedPtr<WideDynamicRange>& wideDynamicRange, 
		const Poco::SharedPtr<WhiteBalance>& whiteBalance, 
		const Poco::SharedPtr<ImagingSettingsExtension>& extension);

	virtual ~ImagingSettings();

	const Poco::SharedPtr<BacklightCompensation>& getBacklightCompensation() const;

	Poco::Optional<float> getBrightness() const;

	Poco::Optional<float> getColorSaturation() const;

	Poco::Optional<float> getContrast() const;

	const Poco::SharedPtr<Exposure>& getExposure() const;

	const Poco::SharedPtr<ImagingSettingsExtension>& getExtension() const;

	const Poco::SharedPtr<FocusConfiguration>& getFocus() const;

	const Poco::Optional<std::string>& getIrCutFilter() const;

	Poco::Optional<float> getSharpness() const;

	const Poco::SharedPtr<WhiteBalance>& getWhiteBalance() const;

	const Poco::SharedPtr<WideDynamicRange>& getWideDynamicRange() const;

	void setBacklightCompensation(const Poco::SharedPtr<BacklightCompensation>& val);

	void setBacklightCompensation(Poco::SharedPtr<BacklightCompensation>&& val);

	void setBrightness(Poco::Optional<float> val);

	void setColorSaturation(Poco::Optional<float> val);

	void setContrast(Poco::Optional<float> val);

	void setExposure(const Poco::SharedPtr<Exposure>& val);

	void setExposure(Poco::SharedPtr<Exposure>&& val);

	void setExtension(const Poco::SharedPtr<ImagingSettingsExtension>& val);

	void setExtension(Poco::SharedPtr<ImagingSettingsExtension>&& val);

	void setFocus(const Poco::SharedPtr<FocusConfiguration>& val);

	void setFocus(Poco::SharedPtr<FocusConfiguration>&& val);

	void setIrCutFilter(const Poco::Optional<std::string>& val);

	void setIrCutFilter(Poco::Optional<std::string>&& val);

	void setSharpness(Poco::Optional<float> val);

	void setWhiteBalance(const Poco::SharedPtr<WhiteBalance>& val);

	void setWhiteBalance(Poco::SharedPtr<WhiteBalance>&& val);

	void setWideDynamicRange(const Poco::SharedPtr<WideDynamicRange>& val);

	void setWideDynamicRange(Poco::SharedPtr<WideDynamicRange>&& val);

private:
	//@ mandatory=false
	//@ name=BacklightCompensation
	//@ order=0
	Poco::SharedPtr<BacklightCompensation> _backlightCompensation;

	//@ mandatory=false
	//@ name=Brightness
	//@ order=1
	Poco::Optional<float> _brightness;

	//@ mandatory=false
	//@ name=ColorSaturation
	//@ order=2
	Poco::Optional<float> _colorSaturation;

	//@ mandatory=false
	//@ name=Contrast
	//@ order=3
	Poco::Optional<float> _contrast;

	//@ mandatory=false
	//@ name=Exposure
	//@ order=4
	Poco::SharedPtr<Exposure> _exposure;

	//@ mandatory=false
	//@ name=Focus
	//@ order=5
	Poco::SharedPtr<FocusConfiguration> _focus;

	//@ mandatory=false
	//@ name=IrCutFilter
	//@ order=6
	Poco::Optional<std::string> _irCutFilter;

	//@ mandatory=false
	//@ name=Sharpness
	//@ order=7
	Poco::Optional<float> _sharpness;

	//@ mandatory=false
	//@ name=WideDynamicRange
	//@ order=8
	Poco::SharedPtr<WideDynamicRange> _wideDynamicRange;

	//@ mandatory=false
	//@ name=WhiteBalance
	//@ order=9
	Poco::SharedPtr<WhiteBalance> _whiteBalance;

	//@ mandatory=false
	//@ name=Extension
	//@ order=10
	Poco::SharedPtr<ImagingSettingsExtension> _extension;

};


inline const Poco::SharedPtr<BacklightCompensation>& ImagingSettings::getBacklightCompensation() const
{
	return _backlightCompensation;
}


inline Poco::Optional<float> ImagingSettings::getBrightness() const
{
	return _brightness;
}


inline Poco::Optional<float> ImagingSettings::getColorSaturation() const
{
	return _colorSaturation;
}


inline Poco::Optional<float> ImagingSettings::getContrast() const
{
	return _contrast;
}


inline const Poco::SharedPtr<Exposure>& ImagingSettings::getExposure() const
{
	return _exposure;
}


inline const Poco::SharedPtr<ImagingSettingsExtension>& ImagingSettings::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<FocusConfiguration>& ImagingSettings::getFocus() const
{
	return _focus;
}


inline const Poco::Optional<std::string>& ImagingSettings::getIrCutFilter() const
{
	return _irCutFilter;
}


inline Poco::Optional<float> ImagingSettings::getSharpness() const
{
	return _sharpness;
}


inline const Poco::SharedPtr<WhiteBalance>& ImagingSettings::getWhiteBalance() const
{
	return _whiteBalance;
}


inline const Poco::SharedPtr<WideDynamicRange>& ImagingSettings::getWideDynamicRange() const
{
	return _wideDynamicRange;
}


inline void ImagingSettings::setBacklightCompensation(const Poco::SharedPtr<BacklightCompensation>& val)
{
	_backlightCompensation = val;
}


inline void ImagingSettings::setBacklightCompensation(Poco::SharedPtr<BacklightCompensation>&& val)
{
	_backlightCompensation = std::move(val);
}


inline void ImagingSettings::setBrightness(Poco::Optional<float> val)
{
	_brightness = val;
}


inline void ImagingSettings::setColorSaturation(Poco::Optional<float> val)
{
	_colorSaturation = val;
}


inline void ImagingSettings::setContrast(Poco::Optional<float> val)
{
	_contrast = val;
}


inline void ImagingSettings::setExposure(const Poco::SharedPtr<Exposure>& val)
{
	_exposure = val;
}


inline void ImagingSettings::setExposure(Poco::SharedPtr<Exposure>&& val)
{
	_exposure = std::move(val);
}


inline void ImagingSettings::setExtension(const Poco::SharedPtr<ImagingSettingsExtension>& val)
{
	_extension = val;
}


inline void ImagingSettings::setExtension(Poco::SharedPtr<ImagingSettingsExtension>&& val)
{
	_extension = std::move(val);
}


inline void ImagingSettings::setFocus(const Poco::SharedPtr<FocusConfiguration>& val)
{
	_focus = val;
}


inline void ImagingSettings::setFocus(Poco::SharedPtr<FocusConfiguration>&& val)
{
	_focus = std::move(val);
}


inline void ImagingSettings::setIrCutFilter(const Poco::Optional<std::string>& val)
{
	_irCutFilter = val;
}


inline void ImagingSettings::setIrCutFilter(Poco::Optional<std::string>&& val)
{
	_irCutFilter = std::move(val);
}


inline void ImagingSettings::setSharpness(Poco::Optional<float> val)
{
	_sharpness = val;
}


inline void ImagingSettings::setWhiteBalance(const Poco::SharedPtr<WhiteBalance>& val)
{
	_whiteBalance = val;
}


inline void ImagingSettings::setWhiteBalance(Poco::SharedPtr<WhiteBalance>&& val)
{
	_whiteBalance = std::move(val);
}


inline void ImagingSettings::setWideDynamicRange(const Poco::SharedPtr<WideDynamicRange>& val)
{
	_wideDynamicRange = val;
}


inline void ImagingSettings::setWideDynamicRange(Poco::SharedPtr<WideDynamicRange>&& val)
{
	_wideDynamicRange = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/BacklightCompensation.h"
#include "ONVIF/Exposure.h"
#include "ONVIF/FocusConfiguration.h"
#include "ONVIF/ImagingSettingsExtension.h"
#include "ONVIF/WhiteBalance.h"
#include "ONVIF/WideDynamicRange.h"


#endif // ONVIF_ImagingSettings_INCLUDED
