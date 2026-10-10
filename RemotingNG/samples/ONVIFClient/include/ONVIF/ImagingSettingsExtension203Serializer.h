//
// ImagingSettingsExtension203Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ImagingSettingsExtension203_INCLUDED
#define TypeSerializer_ONVIF_ImagingSettingsExtension203_INCLUDED


#include "ONVIF/DefoggingDeserializer.h"
#include "ONVIF/DefoggingSerializer.h"
#include "ONVIF/ImagingSettingsExtension203.h"
#include "ONVIF/ImagingSettingsExtension204Deserializer.h"
#include "ONVIF/ImagingSettingsExtension204Serializer.h"
#include "ONVIF/NoiseReductionDeserializer.h"
#include "ONVIF/NoiseReductionSerializer.h"
#include "ONVIF/ToneCompensationDeserializer.h"
#include "ONVIF/ToneCompensationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ImagingSettingsExtension203>
{
public:
	static void serialize(const std::string& name, const ONVIF::ImagingSettingsExtension203& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ImagingSettingsExtension203& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ToneCompensation"s,"Defogging"s,"NoiseReduction"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::ToneCompensation>>::serialize(REMOTING__NAMES[0], value.getToneCompensation(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Defogging>>::serialize(REMOTING__NAMES[1], value.getDefogging(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NoiseReduction>>::serialize(REMOTING__NAMES[2], value.getNoiseReduction(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ImagingSettingsExtension204>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ImagingSettingsExtension203_INCLUDED

