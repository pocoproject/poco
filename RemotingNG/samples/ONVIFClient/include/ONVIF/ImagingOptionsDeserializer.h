//
// ImagingOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImagingOptions_INCLUDED
#define TypeDeserializer_ONVIF_ImagingOptions_INCLUDED


#include "ONVIF/BacklightCompensationOptionsDeserializer.h"
#include "ONVIF/BacklightCompensationOptionsSerializer.h"
#include "ONVIF/ExposureOptionsDeserializer.h"
#include "ONVIF/ExposureOptionsSerializer.h"
#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "ONVIF/FocusOptionsDeserializer.h"
#include "ONVIF/FocusOptionsSerializer.h"
#include "ONVIF/ImagingOptions.h"
#include "ONVIF/WhiteBalanceOptionsDeserializer.h"
#include "ONVIF/WhiteBalanceOptionsSerializer.h"
#include "ONVIF/WideDynamicRangeOptionsDeserializer.h"
#include "ONVIF/WideDynamicRangeOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImagingOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImagingOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::ImagingOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BacklightCompensation"s,"Brightness"s,"ColorSaturation"s,"Contrast"s,"Exposure"s,"Focus"s,"IrCutFilterModes"s,"Sharpness"s,"WideDynamicRange"s,"WhiteBalance"s};
		bool ret = false;
		ONVIF::BacklightCompensationOptions gen_backlightCompensation;
		ret = TypeDeserializer<ONVIF::BacklightCompensationOptions>::deserialize(REMOTING__NAMES[0], true, deser, gen_backlightCompensation);
		if (ret) value.setBacklightCompensation(gen_backlightCompensation);
		ONVIF::FloatRange gen_brightness;
		ret = TypeDeserializer<ONVIF::FloatRange>::deserialize(REMOTING__NAMES[1], true, deser, gen_brightness);
		if (ret) value.setBrightness(gen_brightness);
		ONVIF::FloatRange gen_colorSaturation;
		ret = TypeDeserializer<ONVIF::FloatRange>::deserialize(REMOTING__NAMES[2], true, deser, gen_colorSaturation);
		if (ret) value.setColorSaturation(gen_colorSaturation);
		ONVIF::FloatRange gen_contrast;
		ret = TypeDeserializer<ONVIF::FloatRange>::deserialize(REMOTING__NAMES[3], true, deser, gen_contrast);
		if (ret) value.setContrast(gen_contrast);
		ONVIF::ExposureOptions gen_exposure;
		ret = TypeDeserializer<ONVIF::ExposureOptions>::deserialize(REMOTING__NAMES[4], true, deser, gen_exposure);
		if (ret) value.setExposure(gen_exposure);
		ONVIF::FocusOptions gen_focus;
		ret = TypeDeserializer<ONVIF::FocusOptions>::deserialize(REMOTING__NAMES[5], true, deser, gen_focus);
		if (ret) value.setFocus(gen_focus);
		std::vector<std::string> gen_irCutFilterModes;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[6], true, deser, gen_irCutFilterModes);
		if (ret) value.setIrCutFilterModes(gen_irCutFilterModes);
		ONVIF::FloatRange gen_sharpness;
		ret = TypeDeserializer<ONVIF::FloatRange>::deserialize(REMOTING__NAMES[7], true, deser, gen_sharpness);
		if (ret) value.setSharpness(gen_sharpness);
		ONVIF::WideDynamicRangeOptions gen_wideDynamicRange;
		ret = TypeDeserializer<ONVIF::WideDynamicRangeOptions>::deserialize(REMOTING__NAMES[8], true, deser, gen_wideDynamicRange);
		if (ret) value.setWideDynamicRange(gen_wideDynamicRange);
		ONVIF::WhiteBalanceOptions gen_whiteBalance;
		ret = TypeDeserializer<ONVIF::WhiteBalanceOptions>::deserialize(REMOTING__NAMES[9], true, deser, gen_whiteBalance);
		if (ret) value.setWhiteBalance(gen_whiteBalance);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImagingOptions_INCLUDED

