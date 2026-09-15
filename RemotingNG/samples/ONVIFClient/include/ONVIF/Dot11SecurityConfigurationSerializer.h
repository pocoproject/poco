//
// Dot11SecurityConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Dot11SecurityConfiguration_INCLUDED
#define TypeSerializer_ONVIF_Dot11SecurityConfiguration_INCLUDED


#include "ONVIF/Dot11PSKSetDeserializer.h"
#include "ONVIF/Dot11PSKSetSerializer.h"
#include "ONVIF/Dot11SecurityConfiguration.h"
#include "ONVIF/Dot11SecurityConfigurationExtensionDeserializer.h"
#include "ONVIF/Dot11SecurityConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Dot11SecurityConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::Dot11SecurityConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Dot11SecurityConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Algorithm"s,"PSK"s,"Dot1X"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getMode(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getAlgorithm(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Dot11PSKSet>>::serialize(REMOTING__NAMES[2], value.getPSK(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[3], value.getDot1X(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Dot11SecurityConfigurationExtension>>::serialize(REMOTING__NAMES[4], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Dot11SecurityConfiguration_INCLUDED

