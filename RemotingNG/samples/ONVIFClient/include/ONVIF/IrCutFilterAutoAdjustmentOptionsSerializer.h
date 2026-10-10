//
// IrCutFilterAutoAdjustmentOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_IrCutFilterAutoAdjustmentOptions_INCLUDED
#define TypeSerializer_ONVIF_IrCutFilterAutoAdjustmentOptions_INCLUDED


#include "ONVIF/DurationRangeDeserializer.h"
#include "ONVIF/DurationRangeSerializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptions.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptionsExtensionDeserializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::IrCutFilterAutoAdjustmentOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::IrCutFilterAutoAdjustmentOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::IrCutFilterAutoAdjustmentOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BoundaryType"s,"BoundaryOffset"s,"ResponseTimeRange"s,"Extension"s,""s};
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[0], value.getBoundaryType(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getBoundaryOffset(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::DurationRange>>::serialize(REMOTING__NAMES[2], value.getResponseTimeRange(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IrCutFilterAutoAdjustmentOptionsExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_IrCutFilterAutoAdjustmentOptions_INCLUDED

