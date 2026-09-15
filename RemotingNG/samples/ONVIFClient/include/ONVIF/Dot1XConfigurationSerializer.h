//
// Dot1XConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Dot1XConfiguration_INCLUDED
#define TypeSerializer_ONVIF_Dot1XConfiguration_INCLUDED


#include "ONVIF/Dot1XConfiguration.h"
#include "ONVIF/Dot1XConfigurationExtensionDeserializer.h"
#include "ONVIF/Dot1XConfigurationExtensionSerializer.h"
#include "ONVIF/EAPMethodConfigurationDeserializer.h"
#include "ONVIF/EAPMethodConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Dot1XConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::Dot1XConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Dot1XConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dot1XConfigurationToken"s,"Identity"s,"AnonymousID"s,"EAPMethod"s,"CACertificateID"s,"EAPMethodConfiguration"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getDot1XConfigurationToken(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getIdentity(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getAnonymousID(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[3], value.getEAPMethod(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[4], value.getCACertificateID(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::EAPMethodConfiguration>>::serialize(REMOTING__NAMES[5], value.getEAPMethodConfiguration(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Dot1XConfigurationExtension>>::serialize(REMOTING__NAMES[6], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Dot1XConfiguration_INCLUDED

