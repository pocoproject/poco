//
// NetworkZeroConfigurationExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NetworkZeroConfigurationExtension_INCLUDED
#define TypeSerializer_ONVIF_NetworkZeroConfigurationExtension_INCLUDED


#include "ONVIF/NetworkZeroConfigurationDeserializer.h"
#include "ONVIF/NetworkZeroConfigurationExtension.h"
#include "ONVIF/NetworkZeroConfigurationExtension2Deserializer.h"
#include "ONVIF/NetworkZeroConfigurationExtension2Serializer.h"
#include "ONVIF/NetworkZeroConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NetworkZeroConfigurationExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::NetworkZeroConfigurationExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::NetworkZeroConfigurationExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Additional"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::NetworkZeroConfiguration>>::serialize(REMOTING__NAMES[0], value.getAdditional(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkZeroConfigurationExtension2>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NetworkZeroConfigurationExtension_INCLUDED

