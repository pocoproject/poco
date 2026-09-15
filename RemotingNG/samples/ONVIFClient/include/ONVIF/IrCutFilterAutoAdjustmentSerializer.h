//
// IrCutFilterAutoAdjustmentSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_IrCutFilterAutoAdjustment_INCLUDED
#define TypeSerializer_ONVIF_IrCutFilterAutoAdjustment_INCLUDED


#include "ONVIF/IrCutFilterAutoAdjustment.h"
#include "ONVIF/IrCutFilterAutoAdjustmentExtensionDeserializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::IrCutFilterAutoAdjustment>
{
public:
	static void serialize(const std::string& name, const ONVIF::IrCutFilterAutoAdjustment& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::IrCutFilterAutoAdjustment& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BoundaryType"s,"BoundaryOffset"s,"ResponseTime"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getBoundaryType(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[1], value.getBoundaryOffset(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getResponseTime(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IrCutFilterAutoAdjustmentExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_IrCutFilterAutoAdjustment_INCLUDED

