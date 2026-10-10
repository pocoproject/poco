//
// CertificateInformationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_CertificateInformation_INCLUDED
#define TypeSerializer_ONVIF_CertificateInformation_INCLUDED


#include "ONVIF/CertificateInformation.h"
#include "ONVIF/CertificateInformationExtensionDeserializer.h"
#include "ONVIF/CertificateInformationExtensionSerializer.h"
#include "ONVIF/CertificateUsageDeserializer.h"
#include "ONVIF/CertificateUsageSerializer.h"
#include "ONVIF/DateTimeRangeDeserializer.h"
#include "ONVIF/DateTimeRangeSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::CertificateInformation>
{
public:
	static void serialize(const std::string& name, const ONVIF::CertificateInformation& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::CertificateInformation& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CertificateID"s,"IssuerDN"s,"SubjectDN"s,"KeyUsage"s,"ExtendedKeyUsage"s,"KeyLength"s,"Version"s,"SerialNum"s,"SignatureAlgorithm"s,"Validity"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getCertificateID(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getIssuerDN(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getSubjectDN(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::CertificateUsage>>::serialize(REMOTING__NAMES[3], value.getKeyUsage(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::CertificateUsage>>::serialize(REMOTING__NAMES[4], value.getExtendedKeyUsage(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[5], value.getKeyLength(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[6], value.getVersion(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[7], value.getSerialNum(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[8], value.getSignatureAlgorithm(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::DateTimeRange>>::serialize(REMOTING__NAMES[9], value.getValidity(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::CertificateInformationExtension>>::serialize(REMOTING__NAMES[10], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_CertificateInformation_INCLUDED

