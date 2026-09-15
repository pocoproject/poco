//
// GetSystemUrisResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_GetSystemUrisResponse_INCLUDED
#define TypeSerializer_ONVIF_Device_GetSystemUrisResponse_INCLUDED


#include "ONVIF/Device/ExtensionDeserializer.h"
#include "ONVIF/Device/ExtensionSerializer.h"
#include "ONVIF/Device/GetSystemUrisResponse.h"
#include "ONVIF/SystemLogUriListDeserializer.h"
#include "ONVIF/SystemLogUriListSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::GetSystemUrisResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::GetSystemUrisResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::Device::GetSystemUrisResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SystemLogUris"s,"SupportInfoUri"s,"SystemBackupUri"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::SystemLogUriList>>::serialize(REMOTING__NAMES[0], value.getSystemLogUris(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[1], value.getSupportInfoUri(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[2], value.getSystemBackupUri(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Device::Extension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_GetSystemUrisResponse_INCLUDED

