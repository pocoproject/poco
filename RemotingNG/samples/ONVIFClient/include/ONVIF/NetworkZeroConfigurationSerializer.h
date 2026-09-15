//
// NetworkZeroConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NetworkZeroConfiguration_INCLUDED
#define TypeSerializer_ONVIF_NetworkZeroConfiguration_INCLUDED


#include "ONVIF/NetworkZeroConfiguration.h"
#include "ONVIF/NetworkZeroConfigurationExtensionDeserializer.h"
#include "ONVIF/NetworkZeroConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NetworkZeroConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::NetworkZeroConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::NetworkZeroConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"InterfaceToken"s,"Enabled"s,"Addresses"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getInterfaceToken(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getEnabled(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[2], value.getAddresses(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkZeroConfigurationExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NetworkZeroConfiguration_INCLUDED

