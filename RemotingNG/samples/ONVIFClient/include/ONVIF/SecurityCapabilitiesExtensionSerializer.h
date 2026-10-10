//
// SecurityCapabilitiesExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_SecurityCapabilitiesExtension_INCLUDED
#define TypeSerializer_ONVIF_SecurityCapabilitiesExtension_INCLUDED


#include "ONVIF/SecurityCapabilitiesExtension.h"
#include "ONVIF/SecurityCapabilitiesExtension2Deserializer.h"
#include "ONVIF/SecurityCapabilitiesExtension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::SecurityCapabilitiesExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::SecurityCapabilitiesExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::SecurityCapabilitiesExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TLS1.0"s,"Extension"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getTLS1_0(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SecurityCapabilitiesExtension2>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_SecurityCapabilitiesExtension_INCLUDED

