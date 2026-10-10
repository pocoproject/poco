//
// ImagingOptions20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImagingOptions20_INCLUDED
#define TypeDeserializer_ONVIF_ImagingOptions20_INCLUDED


#include "ONVIF/BacklightCompensationOptions20Deserializer.h"
#include "ONVIF/BacklightCompensationOptions20Serializer.h"
#include "ONVIF/ExposureOptions20Deserializer.h"
#include "ONVIF/ExposureOptions20Serializer.h"
#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "ONVIF/FocusOptions20Deserializer.h"
#include "ONVIF/FocusOptions20Serializer.h"
#include "ONVIF/ImagingOptions20.h"
#include "ONVIF/ImagingOptions20ExtensionDeserializer.h"
#include "ONVIF/ImagingOptions20ExtensionSerializer.h"
#include "ONVIF/WhiteBalanceOptions20Deserializer.h"
#include "ONVIF/WhiteBalanceOptions20Serializer.h"
#include "ONVIF/WideDynamicRangeOptions20Deserializer.h"
#include "ONVIF/WideDynamicRangeOptions20Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImagingOptions20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImagingOptions20& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ImagingOptions20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BacklightCompensation"s,"Brightness"s,"ColorSaturation"s,"Contrast"s,"Exposure"s,"Focus"s,"IrCutFilterModes"s,"Sharpness"s,"WideDynamicRange"s,"WhiteBalance"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::BacklightCompensationOptions20> gen_backlightCompensation;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::BacklightCompensationOptions20>>::deserialize(REMOTING__NAMES[0], false, deser, gen_backlightCompensation);
		if (ret) value.setBacklightCompensation(gen_backlightCompensation);
		Poco::SharedPtr<ONVIF::FloatRange> gen_brightness;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[1], false, deser, gen_brightness);
		if (ret) value.setBrightness(gen_brightness);
		Poco::SharedPtr<ONVIF::FloatRange> gen_colorSaturation;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[2], false, deser, gen_colorSaturation);
		if (ret) value.setColorSaturation(gen_colorSaturation);
		Poco::SharedPtr<ONVIF::FloatRange> gen_contrast;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[3], false, deser, gen_contrast);
		if (ret) value.setContrast(gen_contrast);
		Poco::SharedPtr<ONVIF::ExposureOptions20> gen_exposure;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ExposureOptions20>>::deserialize(REMOTING__NAMES[4], false, deser, gen_exposure);
		if (ret) value.setExposure(gen_exposure);
		Poco::SharedPtr<ONVIF::FocusOptions20> gen_focus;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FocusOptions20>>::deserialize(REMOTING__NAMES[5], false, deser, gen_focus);
		if (ret) value.setFocus(gen_focus);
		std::vector<std::string> gen_irCutFilterModes;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[6], false, deser, gen_irCutFilterModes);
		if (ret) value.setIrCutFilterModes(gen_irCutFilterModes);
		Poco::SharedPtr<ONVIF::FloatRange> gen_sharpness;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[7], false, deser, gen_sharpness);
		if (ret) value.setSharpness(gen_sharpness);
		Poco::SharedPtr<ONVIF::WideDynamicRangeOptions20> gen_wideDynamicRange;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::WideDynamicRangeOptions20>>::deserialize(REMOTING__NAMES[8], false, deser, gen_wideDynamicRange);
		if (ret) value.setWideDynamicRange(gen_wideDynamicRange);
		Poco::SharedPtr<ONVIF::WhiteBalanceOptions20> gen_whiteBalance;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::WhiteBalanceOptions20>>::deserialize(REMOTING__NAMES[9], false, deser, gen_whiteBalance);
		if (ret) value.setWhiteBalance(gen_whiteBalance);
		Poco::SharedPtr<ONVIF::ImagingOptions20Extension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImagingOptions20Extension>>::deserialize(REMOTING__NAMES[10], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImagingOptions20_INCLUDED

