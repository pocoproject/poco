// 
// ImagingSettings20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingSettings20_INCLUDED
#define ONVIF_ImagingSettings20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class BacklightCompensation20;
class Exposure20;
class FocusConfiguration20;
class ImagingSettingsExtension20;
class WhiteBalance20;
class WideDynamicRange20;
} 


namespace ONVIF {


//@ name=ImagingSettings20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingSettings20
{
public:
	ImagingSettings20();

	ImagingSettings20(
		const Poco::SharedPtr<BacklightCompensation20>& backlightCompensation, 
		Poco::Optional<float> brightness, 
		Poco::Optional<float> colorSaturation, 
		Poco::Optional<float> contrast, 
		const Poco::SharedPtr<Exposure20>& exposure, 
		const Poco::SharedPtr<FocusConfiguration20>& focus, 
		const Poco::Optional<std::string>& irCutFilter, 
		Poco::Optional<float> sharpness, 
		const Poco::SharedPtr<WideDynamicRange20>& wideDynamicRange, 
		const Poco::SharedPtr<WhiteBalance20>& whiteBalance, 
		const Poco::SharedPtr<ImagingSettingsExtension20>& extension);

	virtual ~ImagingSettings20();

	const Poco::SharedPtr<BacklightCompensation20>& getBacklightCompensation() const;

	Poco::Optional<float> getBrightness() const;

	Poco::Optional<float> getColorSaturation() const;

	Poco::Optional<float> getContrast() const;

	const Poco::SharedPtr<Exposure20>& getExposure() const;

	const Poco::SharedPtr<ImagingSettingsExtension20>& getExtension() const;

	const Poco::SharedPtr<FocusConfiguration20>& getFocus() const;

	const Poco::Optional<std::string>& getIrCutFilter() const;

	Poco::Optional<float> getSharpness() const;

	const Poco::SharedPtr<WhiteBalance20>& getWhiteBalance() const;

	const Poco::SharedPtr<WideDynamicRange20>& getWideDynamicRange() const;

	void setBacklightCompensation(const Poco::SharedPtr<BacklightCompensation20>& val);

	void setBacklightCompensation(Poco::SharedPtr<BacklightCompensation20>&& val);

	void setBrightness(Poco::Optional<float> val);

	void setColorSaturation(Poco::Optional<float> val);

	void setContrast(Poco::Optional<float> val);

	void setExposure(const Poco::SharedPtr<Exposure20>& val);

	void setExposure(Poco::SharedPtr<Exposure20>&& val);

	void setExtension(const Poco::SharedPtr<ImagingSettingsExtension20>& val);

	void setExtension(Poco::SharedPtr<ImagingSettingsExtension20>&& val);

	void setFocus(const Poco::SharedPtr<FocusConfiguration20>& val);

	void setFocus(Poco::SharedPtr<FocusConfiguration20>&& val);

	void setIrCutFilter(const Poco::Optional<std::string>& val);

	void setIrCutFilter(Poco::Optional<std::string>&& val);

	void setSharpness(Poco::Optional<float> val);

	void setWhiteBalance(const Poco::SharedPtr<WhiteBalance20>& val);

	void setWhiteBalance(Poco::SharedPtr<WhiteBalance20>&& val);

	void setWideDynamicRange(const Poco::SharedPtr<WideDynamicRange20>& val);

	void setWideDynamicRange(Poco::SharedPtr<WideDynamicRange20>&& val);

private:
	//@ mandatory=false
	//@ name=BacklightCompensation
	//@ order=0
	Poco::SharedPtr<BacklightCompensation20> _backlightCompensation;

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
	Poco::SharedPtr<Exposure20> _exposure;

	//@ mandatory=false
	//@ name=Focus
	//@ order=5
	Poco::SharedPtr<FocusConfiguration20> _focus;

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
	Poco::SharedPtr<WideDynamicRange20> _wideDynamicRange;

	//@ mandatory=false
	//@ name=WhiteBalance
	//@ order=9
	Poco::SharedPtr<WhiteBalance20> _whiteBalance;

	//@ mandatory=false
	//@ name=Extension
	//@ order=10
	Poco::SharedPtr<ImagingSettingsExtension20> _extension;

};


inline const Poco::SharedPtr<BacklightCompensation20>& ImagingSettings20::getBacklightCompensation() const
{
	return _backlightCompensation;
}


inline Poco::Optional<float> ImagingSettings20::getBrightness() const
{
	return _brightness;
}


inline Poco::Optional<float> ImagingSettings20::getColorSaturation() const
{
	return _colorSaturation;
}


inline Poco::Optional<float> ImagingSettings20::getContrast() const
{
	return _contrast;
}


inline const Poco::SharedPtr<Exposure20>& ImagingSettings20::getExposure() const
{
	return _exposure;
}


inline const Poco::SharedPtr<ImagingSettingsExtension20>& ImagingSettings20::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<FocusConfiguration20>& ImagingSettings20::getFocus() const
{
	return _focus;
}


inline const Poco::Optional<std::string>& ImagingSettings20::getIrCutFilter() const
{
	return _irCutFilter;
}


inline Poco::Optional<float> ImagingSettings20::getSharpness() const
{
	return _sharpness;
}


inline const Poco::SharedPtr<WhiteBalance20>& ImagingSettings20::getWhiteBalance() const
{
	return _whiteBalance;
}


inline const Poco::SharedPtr<WideDynamicRange20>& ImagingSettings20::getWideDynamicRange() const
{
	return _wideDynamicRange;
}


inline void ImagingSettings20::setBacklightCompensation(const Poco::SharedPtr<BacklightCompensation20>& val)
{
	_backlightCompensation = val;
}


inline void ImagingSettings20::setBacklightCompensation(Poco::SharedPtr<BacklightCompensation20>&& val)
{
	_backlightCompensation = std::move(val);
}


inline void ImagingSettings20::setBrightness(Poco::Optional<float> val)
{
	_brightness = val;
}


inline void ImagingSettings20::setColorSaturation(Poco::Optional<float> val)
{
	_colorSaturation = val;
}


inline void ImagingSettings20::setContrast(Poco::Optional<float> val)
{
	_contrast = val;
}


inline void ImagingSettings20::setExposure(const Poco::SharedPtr<Exposure20>& val)
{
	_exposure = val;
}


inline void ImagingSettings20::setExposure(Poco::SharedPtr<Exposure20>&& val)
{
	_exposure = std::move(val);
}


inline void ImagingSettings20::setExtension(const Poco::SharedPtr<ImagingSettingsExtension20>& val)
{
	_extension = val;
}


inline void ImagingSettings20::setExtension(Poco::SharedPtr<ImagingSettingsExtension20>&& val)
{
	_extension = std::move(val);
}


inline void ImagingSettings20::setFocus(const Poco::SharedPtr<FocusConfiguration20>& val)
{
	_focus = val;
}


inline void ImagingSettings20::setFocus(Poco::SharedPtr<FocusConfiguration20>&& val)
{
	_focus = std::move(val);
}


inline void ImagingSettings20::setIrCutFilter(const Poco::Optional<std::string>& val)
{
	_irCutFilter = val;
}


inline void ImagingSettings20::setIrCutFilter(Poco::Optional<std::string>&& val)
{
	_irCutFilter = std::move(val);
}


inline void ImagingSettings20::setSharpness(Poco::Optional<float> val)
{
	_sharpness = val;
}


inline void ImagingSettings20::setWhiteBalance(const Poco::SharedPtr<WhiteBalance20>& val)
{
	_whiteBalance = val;
}


inline void ImagingSettings20::setWhiteBalance(Poco::SharedPtr<WhiteBalance20>&& val)
{
	_whiteBalance = std::move(val);
}


inline void ImagingSettings20::setWideDynamicRange(const Poco::SharedPtr<WideDynamicRange20>& val)
{
	_wideDynamicRange = val;
}


inline void ImagingSettings20::setWideDynamicRange(Poco::SharedPtr<WideDynamicRange20>&& val)
{
	_wideDynamicRange = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/BacklightCompensation20.h"
#include "ONVIF/Exposure20.h"
#include "ONVIF/FocusConfiguration20.h"
#include "ONVIF/ImagingSettingsExtension20.h"
#include "ONVIF/WhiteBalance20.h"
#include "ONVIF/WideDynamicRange20.h"


#endif // ONVIF_ImagingSettings20_INCLUDED
