// 
// ImagingOptions20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingOptions20_INCLUDED
#define ONVIF_ImagingOptions20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class BacklightCompensationOptions20;
class ExposureOptions20;
class FloatRange;
class FocusOptions20;
class ImagingOptions20Extension;
class WhiteBalanceOptions20;
class WideDynamicRangeOptions20;
} 


namespace ONVIF {


//@ name=ImagingOptions20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingOptions20
{
public:
	ImagingOptions20();

	ImagingOptions20(
		const Poco::SharedPtr<BacklightCompensationOptions20>& backlightCompensation, 
		const Poco::SharedPtr<FloatRange>& brightness, 
		const Poco::SharedPtr<FloatRange>& colorSaturation, 
		const Poco::SharedPtr<FloatRange>& contrast, 
		const Poco::SharedPtr<ExposureOptions20>& exposure, 
		const Poco::SharedPtr<FocusOptions20>& focus, 
		const std::vector<std::string>& irCutFilterModes, 
		const Poco::SharedPtr<FloatRange>& sharpness, 
		const Poco::SharedPtr<WideDynamicRangeOptions20>& wideDynamicRange, 
		const Poco::SharedPtr<WhiteBalanceOptions20>& whiteBalance, 
		const Poco::SharedPtr<ImagingOptions20Extension>& extension);

	virtual ~ImagingOptions20();

	const Poco::SharedPtr<BacklightCompensationOptions20>& getBacklightCompensation() const;

	const Poco::SharedPtr<FloatRange>& getBrightness() const;

	const Poco::SharedPtr<FloatRange>& getColorSaturation() const;

	const Poco::SharedPtr<FloatRange>& getContrast() const;

	const Poco::SharedPtr<ExposureOptions20>& getExposure() const;

	const Poco::SharedPtr<ImagingOptions20Extension>& getExtension() const;

	const Poco::SharedPtr<FocusOptions20>& getFocus() const;

	const std::vector<std::string>& getIrCutFilterModes() const;

	std::vector<std::string>& getIrCutFilterModes();

	const Poco::SharedPtr<FloatRange>& getSharpness() const;

	const Poco::SharedPtr<WhiteBalanceOptions20>& getWhiteBalance() const;

	const Poco::SharedPtr<WideDynamicRangeOptions20>& getWideDynamicRange() const;

	void setBacklightCompensation(const Poco::SharedPtr<BacklightCompensationOptions20>& val);

	void setBacklightCompensation(Poco::SharedPtr<BacklightCompensationOptions20>&& val);

	void setBrightness(const Poco::SharedPtr<FloatRange>& val);

	void setBrightness(Poco::SharedPtr<FloatRange>&& val);

	void setColorSaturation(const Poco::SharedPtr<FloatRange>& val);

	void setColorSaturation(Poco::SharedPtr<FloatRange>&& val);

	void setContrast(const Poco::SharedPtr<FloatRange>& val);

	void setContrast(Poco::SharedPtr<FloatRange>&& val);

	void setExposure(const Poco::SharedPtr<ExposureOptions20>& val);

	void setExposure(Poco::SharedPtr<ExposureOptions20>&& val);

	void setExtension(const Poco::SharedPtr<ImagingOptions20Extension>& val);

	void setExtension(Poco::SharedPtr<ImagingOptions20Extension>&& val);

	void setFocus(const Poco::SharedPtr<FocusOptions20>& val);

	void setFocus(Poco::SharedPtr<FocusOptions20>&& val);

	void setIrCutFilterModes(const std::vector<std::string>& val);

	void setIrCutFilterModes(std::vector<std::string>&& val);

	void setSharpness(const Poco::SharedPtr<FloatRange>& val);

	void setSharpness(Poco::SharedPtr<FloatRange>&& val);

	void setWhiteBalance(const Poco::SharedPtr<WhiteBalanceOptions20>& val);

	void setWhiteBalance(Poco::SharedPtr<WhiteBalanceOptions20>&& val);

	void setWideDynamicRange(const Poco::SharedPtr<WideDynamicRangeOptions20>& val);

	void setWideDynamicRange(Poco::SharedPtr<WideDynamicRangeOptions20>&& val);

private:
	//@ mandatory=false
	//@ name=BacklightCompensation
	//@ order=0
	Poco::SharedPtr<BacklightCompensationOptions20> _backlightCompensation;

	//@ mandatory=false
	//@ name=Brightness
	//@ order=1
	Poco::SharedPtr<FloatRange> _brightness;

	//@ mandatory=false
	//@ name=ColorSaturation
	//@ order=2
	Poco::SharedPtr<FloatRange> _colorSaturation;

	//@ mandatory=false
	//@ name=Contrast
	//@ order=3
	Poco::SharedPtr<FloatRange> _contrast;

	//@ mandatory=false
	//@ name=Exposure
	//@ order=4
	Poco::SharedPtr<ExposureOptions20> _exposure;

	//@ mandatory=false
	//@ name=Focus
	//@ order=5
	Poco::SharedPtr<FocusOptions20> _focus;

	//@ mandatory=false
	//@ name=IrCutFilterModes
	//@ order=6
	std::vector<std::string> _irCutFilterModes;

	//@ mandatory=false
	//@ name=Sharpness
	//@ order=7
	Poco::SharedPtr<FloatRange> _sharpness;

	//@ mandatory=false
	//@ name=WideDynamicRange
	//@ order=8
	Poco::SharedPtr<WideDynamicRangeOptions20> _wideDynamicRange;

	//@ mandatory=false
	//@ name=WhiteBalance
	//@ order=9
	Poco::SharedPtr<WhiteBalanceOptions20> _whiteBalance;

	//@ mandatory=false
	//@ name=Extension
	//@ order=10
	Poco::SharedPtr<ImagingOptions20Extension> _extension;

};


inline const Poco::SharedPtr<BacklightCompensationOptions20>& ImagingOptions20::getBacklightCompensation() const
{
	return _backlightCompensation;
}


inline const Poco::SharedPtr<FloatRange>& ImagingOptions20::getBrightness() const
{
	return _brightness;
}


inline const Poco::SharedPtr<FloatRange>& ImagingOptions20::getColorSaturation() const
{
	return _colorSaturation;
}


inline const Poco::SharedPtr<FloatRange>& ImagingOptions20::getContrast() const
{
	return _contrast;
}


inline const Poco::SharedPtr<ExposureOptions20>& ImagingOptions20::getExposure() const
{
	return _exposure;
}


inline const Poco::SharedPtr<ImagingOptions20Extension>& ImagingOptions20::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<FocusOptions20>& ImagingOptions20::getFocus() const
{
	return _focus;
}


inline const std::vector<std::string>& ImagingOptions20::getIrCutFilterModes() const
{
	return _irCutFilterModes;
}


inline std::vector<std::string>& ImagingOptions20::getIrCutFilterModes()
{
	return _irCutFilterModes;
}


inline const Poco::SharedPtr<FloatRange>& ImagingOptions20::getSharpness() const
{
	return _sharpness;
}


inline const Poco::SharedPtr<WhiteBalanceOptions20>& ImagingOptions20::getWhiteBalance() const
{
	return _whiteBalance;
}


inline const Poco::SharedPtr<WideDynamicRangeOptions20>& ImagingOptions20::getWideDynamicRange() const
{
	return _wideDynamicRange;
}


inline void ImagingOptions20::setBacklightCompensation(const Poco::SharedPtr<BacklightCompensationOptions20>& val)
{
	_backlightCompensation = val;
}


inline void ImagingOptions20::setBacklightCompensation(Poco::SharedPtr<BacklightCompensationOptions20>&& val)
{
	_backlightCompensation = std::move(val);
}


inline void ImagingOptions20::setBrightness(const Poco::SharedPtr<FloatRange>& val)
{
	_brightness = val;
}


inline void ImagingOptions20::setBrightness(Poco::SharedPtr<FloatRange>&& val)
{
	_brightness = std::move(val);
}


inline void ImagingOptions20::setColorSaturation(const Poco::SharedPtr<FloatRange>& val)
{
	_colorSaturation = val;
}


inline void ImagingOptions20::setColorSaturation(Poco::SharedPtr<FloatRange>&& val)
{
	_colorSaturation = std::move(val);
}


inline void ImagingOptions20::setContrast(const Poco::SharedPtr<FloatRange>& val)
{
	_contrast = val;
}


inline void ImagingOptions20::setContrast(Poco::SharedPtr<FloatRange>&& val)
{
	_contrast = std::move(val);
}


inline void ImagingOptions20::setExposure(const Poco::SharedPtr<ExposureOptions20>& val)
{
	_exposure = val;
}


inline void ImagingOptions20::setExposure(Poco::SharedPtr<ExposureOptions20>&& val)
{
	_exposure = std::move(val);
}


inline void ImagingOptions20::setExtension(const Poco::SharedPtr<ImagingOptions20Extension>& val)
{
	_extension = val;
}


inline void ImagingOptions20::setExtension(Poco::SharedPtr<ImagingOptions20Extension>&& val)
{
	_extension = std::move(val);
}


inline void ImagingOptions20::setFocus(const Poco::SharedPtr<FocusOptions20>& val)
{
	_focus = val;
}


inline void ImagingOptions20::setFocus(Poco::SharedPtr<FocusOptions20>&& val)
{
	_focus = std::move(val);
}


inline void ImagingOptions20::setIrCutFilterModes(const std::vector<std::string>& val)
{
	_irCutFilterModes = val;
}


inline void ImagingOptions20::setIrCutFilterModes(std::vector<std::string>&& val)
{
	_irCutFilterModes = std::move(val);
}


inline void ImagingOptions20::setSharpness(const Poco::SharedPtr<FloatRange>& val)
{
	_sharpness = val;
}


inline void ImagingOptions20::setSharpness(Poco::SharedPtr<FloatRange>&& val)
{
	_sharpness = std::move(val);
}


inline void ImagingOptions20::setWhiteBalance(const Poco::SharedPtr<WhiteBalanceOptions20>& val)
{
	_whiteBalance = val;
}


inline void ImagingOptions20::setWhiteBalance(Poco::SharedPtr<WhiteBalanceOptions20>&& val)
{
	_whiteBalance = std::move(val);
}


inline void ImagingOptions20::setWideDynamicRange(const Poco::SharedPtr<WideDynamicRangeOptions20>& val)
{
	_wideDynamicRange = val;
}


inline void ImagingOptions20::setWideDynamicRange(Poco::SharedPtr<WideDynamicRangeOptions20>&& val)
{
	_wideDynamicRange = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/BacklightCompensationOptions20.h"
#include "ONVIF/ExposureOptions20.h"
#include "ONVIF/FloatRange.h"
#include "ONVIF/FocusOptions20.h"
#include "ONVIF/ImagingOptions20Extension.h"
#include "ONVIF/WhiteBalanceOptions20.h"
#include "ONVIF/WideDynamicRangeOptions20.h"


#endif // ONVIF_ImagingOptions20_INCLUDED
