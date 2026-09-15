//
// SecurityCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_SecurityCapabilities_INCLUDED
#define TypeSerializer_ONVIF_SecurityCapabilities_INCLUDED


#include "ONVIF/SecurityCapabilities.h"
#include "ONVIF/SecurityCapabilitiesExtensionDeserializer.h"
#include "ONVIF/SecurityCapabilitiesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::SecurityCapabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::SecurityCapabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::SecurityCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TLS1.1"s,"TLS1.2"s,"OnboardKeyGeneration"s,"AccessPolicyConfig"s,"X.509Token"s,"SAMLToken"s,"KerberosToken"s,"RELToken"s,"Extension"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getTLS1_1(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getTLS1_2(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[2], value.getOnboardKeyGeneration(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[3], value.getAccessPolicyConfig(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[4], value.getX_509Token(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[5], value.getSAMLToken(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[6], value.getKerberosToken(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[7], value.getRELToken(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SecurityCapabilitiesExtension>>::serialize(REMOTING__NAMES[8], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_SecurityCapabilities_INCLUDED

