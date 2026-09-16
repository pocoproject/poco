//
// GetDNSResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetDNSResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetDNSResponse_INCLUDED


#include "ONVIF/DNSInformationDeserializer.h"
#include "ONVIF/DNSInformationSerializer.h"
#include "ONVIF/Device/GetDNSResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetDNSResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetDNSResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetDNSResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DNSInformation"s};
		bool ret = false;
		ONVIF::DNSInformation gen_dNSInformation;
		ret = TypeDeserializer<ONVIF::DNSInformation>::deserialize(REMOTING__NAMES[0], true, deser, gen_dNSInformation);
		if (ret) value.setDNSInformation(gen_dNSInformation);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetDNSResponse_INCLUDED

