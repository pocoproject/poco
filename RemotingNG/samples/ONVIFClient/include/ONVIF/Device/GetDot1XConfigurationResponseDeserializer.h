//
// GetDot1XConfigurationResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetDot1XConfigurationResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetDot1XConfigurationResponse_INCLUDED


#include "ONVIF/Device/GetDot1XConfigurationResponse.h"
#include "ONVIF/Dot1XConfigurationDeserializer.h"
#include "ONVIF/Dot1XConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetDot1XConfigurationResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetDot1XConfigurationResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetDot1XConfigurationResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dot1XConfiguration"s};
		bool ret = false;
		ONVIF::Dot1XConfiguration gen_dot1XConfiguration;
		ret = TypeDeserializer<ONVIF::Dot1XConfiguration>::deserialize(REMOTING__NAMES[0], true, deser, gen_dot1XConfiguration);
		if (ret) value.setDot1XConfiguration(gen_dot1XConfiguration);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetDot1XConfigurationResponse_INCLUDED

