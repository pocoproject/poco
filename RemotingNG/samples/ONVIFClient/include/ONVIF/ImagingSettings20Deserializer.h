//
// ImagingSettings20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImagingSettings20_INCLUDED
#define TypeDeserializer_ONVIF_ImagingSettings20_INCLUDED


#include "ONVIF/BacklightCompensation20Deserializer.h"
#include "ONVIF/BacklightCompensation20Serializer.h"
#include "ONVIF/Exposure20Deserializer.h"
#include "ONVIF/Exposure20Serializer.h"
#include "ONVIF/FocusConfiguration20Deserializer.h"
#include "ONVIF/FocusConfiguration20Serializer.h"
#include "ONVIF/ImagingSettings20.h"
#include "ONVIF/ImagingSettingsExtension20Deserializer.h"
#include "ONVIF/ImagingSettingsExtension20Serializer.h"
#include "ONVIF/WhiteBalance20Deserializer.h"
#include "ONVIF/WhiteBalance20Serializer.h"
#include "ONVIF/WideDynamicRange20Deserializer.h"
#include "ONVIF/WideDynamicRange20Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImagingSettings20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImagingSettings20& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ImagingSettings20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BacklightCompensation"s,"Brightness"s,"ColorSaturation"s,"Contrast"s,"Exposure"s,"Focus"s,"IrCutFilter"s,"Sharpness"s,"WideDynamicRange"s,"WhiteBalance"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::BacklightCompensation20> gen_backlightCompensation;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::BacklightCompensation20>>::deserialize(REMOTING__NAMES[0], false, deser, gen_backlightCompensation);
		if (ret) value.setBacklightCompensation(gen_backlightCompensation);
		Poco::Optional<float> gen_brightness;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[1], false, deser, gen_brightness);
		if (ret) value.setBrightness(gen_brightness);
		Poco::Optional<float> gen_colorSaturation;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[2], false, deser, gen_colorSaturation);
		if (ret) value.setColorSaturation(gen_colorSaturation);
		Poco::Optional<float> gen_contrast;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[3], false, deser, gen_contrast);
		if (ret) value.setContrast(gen_contrast);
		Poco::SharedPtr<ONVIF::Exposure20> gen_exposure;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Exposure20>>::deserialize(REMOTING__NAMES[4], false, deser, gen_exposure);
		if (ret) value.setExposure(gen_exposure);
		Poco::SharedPtr<ONVIF::FocusConfiguration20> gen_focus;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FocusConfiguration20>>::deserialize(REMOTING__NAMES[5], false, deser, gen_focus);
		if (ret) value.setFocus(gen_focus);
		Poco::Optional<std::string> gen_irCutFilter;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[6], false, deser, gen_irCutFilter);
		if (ret) value.setIrCutFilter(gen_irCutFilter);
		Poco::Optional<float> gen_sharpness;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[7], false, deser, gen_sharpness);
		if (ret) value.setSharpness(gen_sharpness);
		Poco::SharedPtr<ONVIF::WideDynamicRange20> gen_wideDynamicRange;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::WideDynamicRange20>>::deserialize(REMOTING__NAMES[8], false, deser, gen_wideDynamicRange);
		if (ret) value.setWideDynamicRange(gen_wideDynamicRange);
		Poco::SharedPtr<ONVIF::WhiteBalance20> gen_whiteBalance;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::WhiteBalance20>>::deserialize(REMOTING__NAMES[9], false, deser, gen_whiteBalance);
		if (ret) value.setWhiteBalance(gen_whiteBalance);
		Poco::SharedPtr<ONVIF::ImagingSettingsExtension20> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImagingSettingsExtension20>>::deserialize(REMOTING__NAMES[10], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImagingSettings20_INCLUDED

