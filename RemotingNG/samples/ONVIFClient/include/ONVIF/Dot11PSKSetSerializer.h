//
// Dot11PSKSetSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Dot11PSKSet_INCLUDED
#define TypeSerializer_ONVIF_Dot11PSKSet_INCLUDED


#include "ONVIF/Dot11PSKSet.h"
#include "ONVIF/Dot11PSKSetExtensionDeserializer.h"
#include "ONVIF/Dot11PSKSetExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Dot11PSKSet>
{
public:
	static void serialize(const std::string& name, const ONVIF::Dot11PSKSet& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Dot11PSKSet& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Key"s,"Passphrase"s,"Extension"s,""s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getKey(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getPassphrase(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Dot11PSKSetExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Dot11PSKSet_INCLUDED

