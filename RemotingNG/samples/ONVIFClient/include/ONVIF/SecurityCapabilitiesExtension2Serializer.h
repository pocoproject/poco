//
// SecurityCapabilitiesExtension2Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_SecurityCapabilitiesExtension2_INCLUDED
#define TypeSerializer_ONVIF_SecurityCapabilitiesExtension2_INCLUDED


#include "ONVIF/SecurityCapabilitiesExtension2.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::SecurityCapabilitiesExtension2>
{
public:
	static void serialize(const std::string& name, const ONVIF::SecurityCapabilitiesExtension2& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::SecurityCapabilitiesExtension2& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dot1X"s,"SupportedEAPMethod"s,"RemoteUserHandling"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getDot1X(), ser);
		TypeSerializer<std::vector<int>>::serialize(REMOTING__NAMES[1], value.getSupportedEAPMethod(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[2], value.getRemoteUserHandling(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_SecurityCapabilitiesExtension2_INCLUDED

