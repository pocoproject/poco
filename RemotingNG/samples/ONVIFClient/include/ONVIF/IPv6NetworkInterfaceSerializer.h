//
// IPv6NetworkInterfaceSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_IPv6NetworkInterface_INCLUDED
#define TypeSerializer_ONVIF_IPv6NetworkInterface_INCLUDED


#include "ONVIF/IPv6ConfigurationDeserializer.h"
#include "ONVIF/IPv6ConfigurationSerializer.h"
#include "ONVIF/IPv6NetworkInterface.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::IPv6NetworkInterface>
{
public:
	static void serialize(const std::string& name, const ONVIF::IPv6NetworkInterface& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::IPv6NetworkInterface& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Enabled"s,"Config"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getEnabled(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IPv6Configuration>>::serialize(REMOTING__NAMES[1], value.getConfig(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_IPv6NetworkInterface_INCLUDED

