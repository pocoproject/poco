//
// LoadCertificatesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_LoadCertificates_INCLUDED
#define TypeDeserializer_ONVIF_Device_LoadCertificates_INCLUDED


#include "ONVIF/CertificateDeserializer.h"
#include "ONVIF/CertificateSerializer.h"
#include "ONVIF/Device/LoadCertificates.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::LoadCertificates>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::LoadCertificates& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::LoadCertificates& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"NVTCertificate"s};
		bool ret = false;
		std::vector<ONVIF::Certificate> gen_nVTCertificate;
		ret = TypeDeserializer<std::vector<ONVIF::Certificate>>::deserialize(REMOTING__NAMES[0], true, deser, gen_nVTCertificate);
		if (ret) value.setNVTCertificate(gen_nVTCertificate);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_LoadCertificates_INCLUDED

