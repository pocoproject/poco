//
// GetClientCertificateModeResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetClientCertificateModeResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetClientCertificateModeResponse_INCLUDED


#include "ONVIF/Device/GetClientCertificateModeResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetClientCertificateModeResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetClientCertificateModeResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetClientCertificateModeResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Enabled"s};
		bool ret = false;
		bool gen_enabled;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_enabled);
		if (ret) value.setEnabled(gen_enabled);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetClientCertificateModeResponse_INCLUDED

