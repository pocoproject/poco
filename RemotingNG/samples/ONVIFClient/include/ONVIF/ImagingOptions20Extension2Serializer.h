//
// ImagingOptions20Extension2Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ImagingOptions20Extension2_INCLUDED
#define TypeSerializer_ONVIF_ImagingOptions20Extension2_INCLUDED


#include "ONVIF/ImagingOptions20Extension2.h"
#include "ONVIF/ImagingOptions20Extension3Deserializer.h"
#include "ONVIF/ImagingOptions20Extension3Serializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptionsDeserializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptionsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ImagingOptions20Extension2>
{
public:
	static void serialize(const std::string& name, const ONVIF::ImagingOptions20Extension2& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ImagingOptions20Extension2& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IrCutFilterAutoAdjustment"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::IrCutFilterAutoAdjustmentOptions>>::serialize(REMOTING__NAMES[0], value.getIrCutFilterAutoAdjustment(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ImagingOptions20Extension3>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ImagingOptions20Extension2_INCLUDED

