//
// CreateCertificateResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_CreateCertificateResponse_INCLUDED
#define TypeSerializer_ONVIF_Device_CreateCertificateResponse_INCLUDED


#include "ONVIF/CertificateDeserializer.h"
#include "ONVIF/CertificateSerializer.h"
#include "ONVIF/Device/CreateCertificateResponse.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::CreateCertificateResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::CreateCertificateResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::Device::CreateCertificateResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"NvtCertificate"s,""s};
		TypeSerializer<ONVIF::Certificate>::serialize(REMOTING__NAMES[0], value.getNvtCertificate(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_CreateCertificateResponse_INCLUDED

