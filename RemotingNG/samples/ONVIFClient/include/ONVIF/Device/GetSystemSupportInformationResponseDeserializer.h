//
// GetSystemSupportInformationResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetSystemSupportInformationResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetSystemSupportInformationResponse_INCLUDED


#include "ONVIF/Device/GetSystemSupportInformationResponse.h"
#include "ONVIF/SupportInformationDeserializer.h"
#include "ONVIF/SupportInformationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetSystemSupportInformationResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetSystemSupportInformationResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetSystemSupportInformationResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SupportInformation"s};
		bool ret = false;
		ONVIF::SupportInformation gen_supportInformation;
		ret = TypeDeserializer<ONVIF::SupportInformation>::deserialize(REMOTING__NAMES[0], true, deser, gen_supportInformation);
		if (ret) value.setSupportInformation(gen_supportInformation);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetSystemSupportInformationResponse_INCLUDED

