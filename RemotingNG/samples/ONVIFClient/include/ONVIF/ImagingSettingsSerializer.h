//
// ImagingSettingsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ImagingSettings_INCLUDED
#define TypeSerializer_ONVIF_ImagingSettings_INCLUDED


#include "ONVIF/BacklightCompensationDeserializer.h"
#include "ONVIF/BacklightCompensationSerializer.h"
#include "ONVIF/ExposureDeserializer.h"
#include "ONVIF/ExposureSerializer.h"
#include "ONVIF/FocusConfigurationDeserializer.h"
#include "ONVIF/FocusConfigurationSerializer.h"
#include "ONVIF/ImagingSettings.h"
#include "ONVIF/ImagingSettingsExtensionDeserializer.h"
#include "ONVIF/ImagingSettingsExtensionSerializer.h"
#include "ONVIF/WhiteBalanceDeserializer.h"
#include "ONVIF/WhiteBalanceSerializer.h"
#include "ONVIF/WideDynamicRangeDeserializer.h"
#include "ONVIF/WideDynamicRangeSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ImagingSettings>
{
public:
	static void serialize(const std::string& name, const ONVIF::ImagingSettings& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ImagingSettings& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BacklightCompensation"s,"Brightness"s,"ColorSaturation"s,"Contrast"s,"Exposure"s,"Focus"s,"IrCutFilter"s,"Sharpness"s,"WideDynamicRange"s,"WhiteBalance"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::BacklightCompensation>>::serialize(REMOTING__NAMES[0], value.getBacklightCompensation(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[1], value.getBrightness(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[2], value.getColorSaturation(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[3], value.getContrast(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Exposure>>::serialize(REMOTING__NAMES[4], value.getExposure(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FocusConfiguration>>::serialize(REMOTING__NAMES[5], value.getFocus(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[6], value.getIrCutFilter(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[7], value.getSharpness(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::WideDynamicRange>>::serialize(REMOTING__NAMES[8], value.getWideDynamicRange(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::WhiteBalance>>::serialize(REMOTING__NAMES[9], value.getWhiteBalance(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ImagingSettingsExtension>>::serialize(REMOTING__NAMES[10], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ImagingSettings_INCLUDED

