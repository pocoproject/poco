//
// ImagingOptions20Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ImagingOptions20_INCLUDED
#define TypeSerializer_ONVIF_ImagingOptions20_INCLUDED


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
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ImagingOptions20>
{
public:
	static void serialize(const std::string& name, const ONVIF::ImagingOptions20& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ImagingOptions20& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BacklightCompensation"s,"Brightness"s,"ColorSaturation"s,"Contrast"s,"Exposure"s,"Focus"s,"IrCutFilterModes"s,"Sharpness"s,"WideDynamicRange"s,"WhiteBalance"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::BacklightCompensationOptions20>>::serialize(REMOTING__NAMES[0], value.getBacklightCompensation(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[1], value.getBrightness(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[2], value.getColorSaturation(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[3], value.getContrast(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ExposureOptions20>>::serialize(REMOTING__NAMES[4], value.getExposure(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FocusOptions20>>::serialize(REMOTING__NAMES[5], value.getFocus(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[6], value.getIrCutFilterModes(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[7], value.getSharpness(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::WideDynamicRangeOptions20>>::serialize(REMOTING__NAMES[8], value.getWideDynamicRange(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::WhiteBalanceOptions20>>::serialize(REMOTING__NAMES[9], value.getWhiteBalance(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ImagingOptions20Extension>>::serialize(REMOTING__NAMES[10], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ImagingOptions20_INCLUDED

