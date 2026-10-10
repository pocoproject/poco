//
// GetDiscoveryModeResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_GetDiscoveryModeResponse_INCLUDED
#define TypeSerializer_ONVIF_Device_GetDiscoveryModeResponse_INCLUDED


#include "ONVIF/Device/GetDiscoveryModeResponse.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::GetDiscoveryModeResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::GetDiscoveryModeResponse& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Device::GetDiscoveryModeResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DiscoveryMode"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getDiscoveryMode(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_GetDiscoveryModeResponse_INCLUDED

