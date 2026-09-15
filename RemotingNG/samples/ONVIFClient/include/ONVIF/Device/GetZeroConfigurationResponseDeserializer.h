//
// GetZeroConfigurationResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetZeroConfigurationResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetZeroConfigurationResponse_INCLUDED


#include "ONVIF/Device/GetZeroConfigurationResponse.h"
#include "ONVIF/NetworkZeroConfigurationDeserializer.h"
#include "ONVIF/NetworkZeroConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetZeroConfigurationResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetZeroConfigurationResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetZeroConfigurationResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ZeroConfiguration"s};
		bool ret = false;
		ONVIF::NetworkZeroConfiguration gen_zeroConfiguration;
		ret = TypeDeserializer<ONVIF::NetworkZeroConfiguration>::deserialize(REMOTING__NAMES[0], true, deser, gen_zeroConfiguration);
		if (ret) value.setZeroConfiguration(gen_zeroConfiguration);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetZeroConfigurationResponse_INCLUDED

