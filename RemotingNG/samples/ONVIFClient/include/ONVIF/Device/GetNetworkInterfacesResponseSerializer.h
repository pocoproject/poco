//
// GetNetworkInterfacesResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_GetNetworkInterfacesResponse_INCLUDED
#define TypeSerializer_ONVIF_Device_GetNetworkInterfacesResponse_INCLUDED


#include "ONVIF/Device/GetNetworkInterfacesResponse.h"
#include "ONVIF/NetworkInterfaceDeserializer.h"
#include "ONVIF/NetworkInterfaceSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::GetNetworkInterfacesResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::GetNetworkInterfacesResponse& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Device::GetNetworkInterfacesResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"NetworkInterfaces"s,""s};
		TypeSerializer<std::vector<ONVIF::NetworkInterface>>::serialize(REMOTING__NAMES[0], value.getNetworkInterfaces(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_GetNetworkInterfacesResponse_INCLUDED

