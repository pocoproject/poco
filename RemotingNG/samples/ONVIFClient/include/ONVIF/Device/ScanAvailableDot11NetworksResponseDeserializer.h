//
// ScanAvailableDot11NetworksResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_ScanAvailableDot11NetworksResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_ScanAvailableDot11NetworksResponse_INCLUDED


#include "ONVIF/Device/ScanAvailableDot11NetworksResponse.h"
#include "ONVIF/Dot11AvailableNetworksDeserializer.h"
#include "ONVIF/Dot11AvailableNetworksSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::ScanAvailableDot11NetworksResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::ScanAvailableDot11NetworksResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::ScanAvailableDot11NetworksResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Networks"s};
		bool ret = false;
		std::vector<ONVIF::Dot11AvailableNetworks> gen_networks;
		ret = TypeDeserializer<std::vector<ONVIF::Dot11AvailableNetworks>>::deserialize(REMOTING__NAMES[0], false, deser, gen_networks);
		if (ret) value.setNetworks(gen_networks);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_ScanAvailableDot11NetworksResponse_INCLUDED

