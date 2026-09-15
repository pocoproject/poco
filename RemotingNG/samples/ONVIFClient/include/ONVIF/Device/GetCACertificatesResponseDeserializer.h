//
// GetCACertificatesResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetCACertificatesResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetCACertificatesResponse_INCLUDED


#include "ONVIF/CertificateDeserializer.h"
#include "ONVIF/CertificateSerializer.h"
#include "ONVIF/Device/GetCACertificatesResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetCACertificatesResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetCACertificatesResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetCACertificatesResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CACertificate"s};
		bool ret = false;
		std::vector<ONVIF::Certificate> gen_cACertificate;
		ret = TypeDeserializer<std::vector<ONVIF::Certificate>>::deserialize(REMOTING__NAMES[0], false, deser, gen_cACertificate);
		if (ret) value.setCACertificate(gen_cACertificate);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetCACertificatesResponse_INCLUDED

