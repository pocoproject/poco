//
// ImagingSettingsExtension202Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ImagingSettingsExtension202_INCLUDED
#define TypeSerializer_ONVIF_ImagingSettingsExtension202_INCLUDED


#include "ONVIF/ImagingSettingsExtension202.h"
#include "ONVIF/ImagingSettingsExtension203Deserializer.h"
#include "ONVIF/ImagingSettingsExtension203Serializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentDeserializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ImagingSettingsExtension202>
{
public:
	static void serialize(const std::string& name, const ONVIF::ImagingSettingsExtension202& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ImagingSettingsExtension202& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IrCutFilterAutoAdjustment"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::IrCutFilterAutoAdjustment>>::serialize(REMOTING__NAMES[0], value.getIrCutFilterAutoAdjustment(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ImagingSettingsExtension203>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ImagingSettingsExtension202_INCLUDED

