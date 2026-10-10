// 
// ImagingOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingOptions_INCLUDED
#define ONVIF_ImagingOptions_INCLUDED


#include "ONVIF/BacklightCompensationOptions.h"
#include "ONVIF/ExposureOptions.h"
#include "ONVIF/FloatRange.h"
#include "ONVIF/FocusOptions.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/WhiteBalanceOptions.h"
#include "ONVIF/WideDynamicRangeOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ImagingOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingOptions
{
public:
	ImagingOptions();

	ImagingOptions(
		const BacklightCompensationOptions& backlightCompensation, 
		const FloatRange& brightness, 
		const FloatRange& colorSaturation, 
		const FloatRange& contrast, 
		const ExposureOptions& exposure, 
		const FocusOptions& focus, 
		const std::vector<std::string>& irCutFilterModes, 
		const FloatRange& sharpness, 
		const WideDynamicRangeOptions& wideDynamicRange, 
		const WhiteBalanceOptions& whiteBalance);

	virtual ~ImagingOptions();

	const BacklightCompensationOptions& getBacklightCompensation() const;

	const FloatRange& getBrightness() const;

	const FloatRange& getColorSaturation() const;

	const FloatRange& getContrast() const;

	const ExposureOptions& getExposure() const;

	const FocusOptions& getFocus() const;

	const std::vector<std::string>& getIrCutFilterModes() const;

	std::vector<std::string>& getIrCutFilterModes();

	const FloatRange& getSharpness() const;

	const WhiteBalanceOptions& getWhiteBalance() const;

	const WideDynamicRangeOptions& getWideDynamicRange() const;

	void setBacklightCompensation(const BacklightCompensationOptions& val);

	void setBacklightCompensation(BacklightCompensationOptions&& val);

	void setBrightness(const FloatRange& val);

	void setBrightness(FloatRange&& val);

	void setColorSaturation(const FloatRange& val);

	void setColorSaturation(FloatRange&& val);

	void setContrast(const FloatRange& val);

	void setContrast(FloatRange&& val);

	void setExposure(const ExposureOptions& val);

	void setExposure(ExposureOptions&& val);

	void setFocus(const FocusOptions& val);

	void setFocus(FocusOptions&& val);

	void setIrCutFilterModes(const std::vector<std::string>& val);

	void setIrCutFilterModes(std::vector<std::string>&& val);

	void setSharpness(const FloatRange& val);

	void setSharpness(FloatRange&& val);

	void setWhiteBalance(const WhiteBalanceOptions& val);

	void setWhiteBalance(WhiteBalanceOptions&& val);

	void setWideDynamicRange(const WideDynamicRangeOptions& val);

	void setWideDynamicRange(WideDynamicRangeOptions&& val);

private:
	//@ name=BacklightCompensation
	//@ order=0
	BacklightCompensationOptions _backlightCompensation;

	//@ name=Brightness
	//@ order=1
	FloatRange _brightness;

	//@ name=ColorSaturation
	//@ order=2
	FloatRange _colorSaturation;

	//@ name=Contrast
	//@ order=3
	FloatRange _contrast;

	//@ name=Exposure
	//@ order=4
	ExposureOptions _exposure;

	//@ name=Focus
	//@ order=5
	FocusOptions _focus;

	//@ name=IrCutFilterModes
	//@ order=6
	std::vector<std::string> _irCutFilterModes;

	//@ name=Sharpness
	//@ order=7
	FloatRange _sharpness;

	//@ name=WideDynamicRange
	//@ order=8
	WideDynamicRangeOptions _wideDynamicRange;

	//@ name=WhiteBalance
	//@ order=9
	WhiteBalanceOptions _whiteBalance;

};


inline const BacklightCompensationOptions& ImagingOptions::getBacklightCompensation() const
{
	return _backlightCompensation;
}


inline const FloatRange& ImagingOptions::getBrightness() const
{
	return _brightness;
}


inline const FloatRange& ImagingOptions::getColorSaturation() const
{
	return _colorSaturation;
}


inline const FloatRange& ImagingOptions::getContrast() const
{
	return _contrast;
}


inline const ExposureOptions& ImagingOptions::getExposure() const
{
	return _exposure;
}


inline const FocusOptions& ImagingOptions::getFocus() const
{
	return _focus;
}


inline const std::vector<std::string>& ImagingOptions::getIrCutFilterModes() const
{
	return _irCutFilterModes;
}


inline std::vector<std::string>& ImagingOptions::getIrCutFilterModes()
{
	return _irCutFilterModes;
}


inline const FloatRange& ImagingOptions::getSharpness() const
{
	return _sharpness;
}


inline const WhiteBalanceOptions& ImagingOptions::getWhiteBalance() const
{
	return _whiteBalance;
}


inline const WideDynamicRangeOptions& ImagingOptions::getWideDynamicRange() const
{
	return _wideDynamicRange;
}


inline void ImagingOptions::setBacklightCompensation(const BacklightCompensationOptions& val)
{
	_backlightCompensation = val;
}


inline void ImagingOptions::setBacklightCompensation(BacklightCompensationOptions&& val)
{
	_backlightCompensation = std::move(val);
}


inline void ImagingOptions::setBrightness(const FloatRange& val)
{
	_brightness = val;
}


inline void ImagingOptions::setBrightness(FloatRange&& val)
{
	_brightness = std::move(val);
}


inline void ImagingOptions::setColorSaturation(const FloatRange& val)
{
	_colorSaturation = val;
}


inline void ImagingOptions::setColorSaturation(FloatRange&& val)
{
	_colorSaturation = std::move(val);
}


inline void ImagingOptions::setContrast(const FloatRange& val)
{
	_contrast = val;
}


inline void ImagingOptions::setContrast(FloatRange&& val)
{
	_contrast = std::move(val);
}


inline void ImagingOptions::setExposure(const ExposureOptions& val)
{
	_exposure = val;
}


inline void ImagingOptions::setExposure(ExposureOptions&& val)
{
	_exposure = std::move(val);
}


inline void ImagingOptions::setFocus(const FocusOptions& val)
{
	_focus = val;
}


inline void ImagingOptions::setFocus(FocusOptions&& val)
{
	_focus = std::move(val);
}


inline void ImagingOptions::setIrCutFilterModes(const std::vector<std::string>& val)
{
	_irCutFilterModes = val;
}


inline void ImagingOptions::setIrCutFilterModes(std::vector<std::string>&& val)
{
	_irCutFilterModes = std::move(val);
}


inline void ImagingOptions::setSharpness(const FloatRange& val)
{
	_sharpness = val;
}


inline void ImagingOptions::setSharpness(FloatRange&& val)
{
	_sharpness = std::move(val);
}


inline void ImagingOptions::setWhiteBalance(const WhiteBalanceOptions& val)
{
	_whiteBalance = val;
}


inline void ImagingOptions::setWhiteBalance(WhiteBalanceOptions&& val)
{
	_whiteBalance = std::move(val);
}


inline void ImagingOptions::setWideDynamicRange(const WideDynamicRangeOptions& val)
{
	_wideDynamicRange = val;
}


inline void ImagingOptions::setWideDynamicRange(WideDynamicRangeOptions&& val)
{
	_wideDynamicRange = std::move(val);
}


} // ONVIF


#endif // ONVIF_ImagingOptions_INCLUDED
