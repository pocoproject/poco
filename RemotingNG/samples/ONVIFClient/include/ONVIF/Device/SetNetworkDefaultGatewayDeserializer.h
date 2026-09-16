//
// SetNetworkDefaultGatewayDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SetNetworkDefaultGateway_INCLUDED
#define TypeDeserializer_ONVIF_Device_SetNetworkDefaultGateway_INCLUDED


#include "ONVIF/Device/SetNetworkDefaultGateway.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SetNetworkDefaultGateway>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SetNetworkDefaultGateway& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SetNetworkDefaultGateway& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IPv4Address"s,"IPv6Address"s};
		bool ret = false;
		std::vector<std::string> gen_iPv4Address;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_iPv4Address);
		if (ret) value.setIPv4Address(gen_iPv4Address);
		std::vector<std::string> gen_iPv6Address;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_iPv6Address);
		if (ret) value.setIPv6Address(gen_iPv6Address);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SetNetworkDefaultGateway_INCLUDED

