//
// SystemCapabilitiesExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_SystemCapabilitiesExtension_INCLUDED
#define TypeSerializer_ONVIF_SystemCapabilitiesExtension_INCLUDED


#include "ONVIF/SystemCapabilitiesExtension.h"
#include "ONVIF/SystemCapabilitiesExtension2Deserializer.h"
#include "ONVIF/SystemCapabilitiesExtension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::SystemCapabilitiesExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::SystemCapabilitiesExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::SystemCapabilitiesExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"HttpFirmwareUpgrade"s,"HttpSystemBackup"s,"HttpSystemLogging"s,"HttpSupportInformation"s,"Extension"s,""s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getHttpFirmwareUpgrade(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getHttpSystemBackup(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getHttpSystemLogging(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[3], value.getHttpSupportInformation(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SystemCapabilitiesExtension2>>::serialize(REMOTING__NAMES[4], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_SystemCapabilitiesExtension_INCLUDED

