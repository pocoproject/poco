//
// GetDynamicDNSResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetDynamicDNSResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetDynamicDNSResponse_INCLUDED


#include "ONVIF/Device/GetDynamicDNSResponse.h"
#include "ONVIF/DynamicDNSInformationDeserializer.h"
#include "ONVIF/DynamicDNSInformationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetDynamicDNSResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetDynamicDNSResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetDynamicDNSResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DynamicDNSInformation"s};
		bool ret = false;
		ONVIF::DynamicDNSInformation gen_dynamicDNSInformation;
		ret = TypeDeserializer<ONVIF::DynamicDNSInformation>::deserialize(REMOTING__NAMES[0], true, deser, gen_dynamicDNSInformation);
		if (ret) value.setDynamicDNSInformation(gen_dynamicDNSInformation);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetDynamicDNSResponse_INCLUDED

