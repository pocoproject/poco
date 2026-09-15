//
// ImagingSettings20Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ImagingSettings20_INCLUDED
#define TypeSerializer_ONVIF_ImagingSettings20_INCLUDED


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
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ImagingSettings20>
{
public:
	static void serialize(const std::string& name, const ONVIF::ImagingSettings20& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::ImagingSettings20& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BacklightCompensation"s,"Brightness"s,"ColorSaturation"s,"Contrast"s,"Exposure"s,"Focus"s,"IrCutFilter"s,"Sharpness"s,"WideDynamicRange"s,"WhiteBalance"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::BacklightCompensation20>>::serialize(REMOTING__NAMES[0], value.getBacklightCompensation(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[1], value.getBrightness(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[2], value.getColorSaturation(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[3], value.getContrast(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Exposure20>>::serialize(REMOTING__NAMES[4], value.getExposure(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FocusConfiguration20>>::serialize(REMOTING__NAMES[5], value.getFocus(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[6], value.getIrCutFilter(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[7], value.getSharpness(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::WideDynamicRange20>>::serialize(REMOTING__NAMES[8], value.getWideDynamicRange(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::WhiteBalance20>>::serialize(REMOTING__NAMES[9], value.getWhiteBalance(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ImagingSettingsExtension20>>::serialize(REMOTING__NAMES[10], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ImagingSettings20_INCLUDED

