//
// MulticastConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_MulticastConfiguration_INCLUDED
#define TypeSerializer_ONVIF_MulticastConfiguration_INCLUDED


#include "ONVIF/IPAddressDeserializer.h"
#include "ONVIF/IPAddressSerializer.h"
#include "ONVIF/MulticastConfiguration.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::MulticastConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::MulticastConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::MulticastConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Address"s,"Port"s,"TTL"s,"AutoStart"s,""s};
		TypeSerializer<ONVIF::IPAddress>::serialize(REMOTING__NAMES[0], value.getAddress(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[1], value.getPort(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[2], value.getTTL(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[3], value.getAutoStart(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_MulticastConfiguration_INCLUDED

