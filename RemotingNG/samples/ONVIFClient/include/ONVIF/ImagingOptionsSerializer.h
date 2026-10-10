//
// ImagingOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ImagingOptions_INCLUDED
#define TypeSerializer_ONVIF_ImagingOptions_INCLUDED


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
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ImagingOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::ImagingOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ImagingOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BacklightCompensation"s,"Brightness"s,"ColorSaturation"s,"Contrast"s,"Exposure"s,"Focus"s,"IrCutFilterModes"s,"Sharpness"s,"WideDynamicRange"s,"WhiteBalance"s,""s};
		TypeSerializer<ONVIF::BacklightCompensationOptions>::serialize(REMOTING__NAMES[0], value.getBacklightCompensation(), ser);
		TypeSerializer<ONVIF::FloatRange>::serialize(REMOTING__NAMES[1], value.getBrightness(), ser);
		TypeSerializer<ONVIF::FloatRange>::serialize(REMOTING__NAMES[2], value.getColorSaturation(), ser);
		TypeSerializer<ONVIF::FloatRange>::serialize(REMOTING__NAMES[3], value.getContrast(), ser);
		TypeSerializer<ONVIF::ExposureOptions>::serialize(REMOTING__NAMES[4], value.getExposure(), ser);
		TypeSerializer<ONVIF::FocusOptions>::serialize(REMOTING__NAMES[5], value.getFocus(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[6], value.getIrCutFilterModes(), ser);
		TypeSerializer<ONVIF::FloatRange>::serialize(REMOTING__NAMES[7], value.getSharpness(), ser);
		TypeSerializer<ONVIF::WideDynamicRangeOptions>::serialize(REMOTING__NAMES[8], value.getWideDynamicRange(), ser);
		TypeSerializer<ONVIF::WhiteBalanceOptions>::serialize(REMOTING__NAMES[9], value.getWhiteBalance(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ImagingOptions_INCLUDED

