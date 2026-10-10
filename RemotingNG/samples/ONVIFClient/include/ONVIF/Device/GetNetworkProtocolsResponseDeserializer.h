//
// GetNetworkProtocolsResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetNetworkProtocolsResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetNetworkProtocolsResponse_INCLUDED


#include "ONVIF/Device/GetNetworkProtocolsResponse.h"
#include "ONVIF/NetworkProtocolDeserializer.h"
#include "ONVIF/NetworkProtocolSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetNetworkProtocolsResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetNetworkProtocolsResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetNetworkProtocolsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"NetworkProtocols"s};
		bool ret = false;
		std::vector<ONVIF::NetworkProtocol> gen_networkProtocols;
		ret = TypeDeserializer<std::vector<ONVIF::NetworkProtocol>>::deserialize(REMOTING__NAMES[0], false, deser, gen_networkProtocols);
		if (ret) value.setNetworkProtocols(gen_networkProtocols);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetNetworkProtocolsResponse_INCLUDED

