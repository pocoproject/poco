//
// EAPMethodConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_EAPMethodConfiguration_INCLUDED
#define TypeSerializer_ONVIF_EAPMethodConfiguration_INCLUDED


#include "ONVIF/EAPMethodConfiguration.h"
#include "ONVIF/EapMethodExtensionDeserializer.h"
#include "ONVIF/EapMethodExtensionSerializer.h"
#include "ONVIF/TLSConfigurationDeserializer.h"
#include "ONVIF/TLSConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::EAPMethodConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::EAPMethodConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::EAPMethodConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TLSConfiguration"s,"Password"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::TLSConfiguration>>::serialize(REMOTING__NAMES[0], value.getTLSConfiguration(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getPassword(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::EapMethodExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_EAPMethodConfiguration_INCLUDED

