//
// GetRelayOutputsResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_GetRelayOutputsResponse_INCLUDED
#define TypeSerializer_ONVIF_Device_GetRelayOutputsResponse_INCLUDED


#include "ONVIF/Device/GetRelayOutputsResponse.h"
#include "ONVIF/RelayOutputDeserializer.h"
#include "ONVIF/RelayOutputSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::GetRelayOutputsResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::GetRelayOutputsResponse& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Device::GetRelayOutputsResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RelayOutputs"s,""s};
		TypeSerializer<std::vector<ONVIF::RelayOutput>>::serialize(REMOTING__NAMES[0], value.getRelayOutputs(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_GetRelayOutputsResponse_INCLUDED

