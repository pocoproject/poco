//
// GetHostnameResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetHostnameResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetHostnameResponse_INCLUDED


#include "ONVIF/Device/GetHostnameResponse.h"
#include "ONVIF/HostnameInformationDeserializer.h"
#include "ONVIF/HostnameInformationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetHostnameResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetHostnameResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetHostnameResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"HostnameInformation"s};
		bool ret = false;
		ONVIF::HostnameInformation gen_hostnameInformation;
		ret = TypeDeserializer<ONVIF::HostnameInformation>::deserialize(REMOTING__NAMES[0], true, deser, gen_hostnameInformation);
		if (ret) value.setHostnameInformation(gen_hostnameInformation);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetHostnameResponse_INCLUDED

