//
// GetCertificateInformationResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetCertificateInformationResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetCertificateInformationResponse_INCLUDED


#include "ONVIF/CertificateInformationDeserializer.h"
#include "ONVIF/CertificateInformationSerializer.h"
#include "ONVIF/Device/GetCertificateInformationResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetCertificateInformationResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetCertificateInformationResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetCertificateInformationResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CertificateInformation"s};
		bool ret = false;
		ONVIF::CertificateInformation gen_certificateInformation;
		ret = TypeDeserializer<ONVIF::CertificateInformation>::deserialize(REMOTING__NAMES[0], true, deser, gen_certificateInformation);
		if (ret) value.setCertificateInformation(gen_certificateInformation);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetCertificateInformationResponse_INCLUDED

