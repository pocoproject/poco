//
// CertificateInformationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_CertificateInformation_INCLUDED
#define TypeDeserializer_ONVIF_CertificateInformation_INCLUDED


#include "ONVIF/CertificateInformation.h"
#include "ONVIF/CertificateInformationExtensionDeserializer.h"
#include "ONVIF/CertificateInformationExtensionSerializer.h"
#include "ONVIF/CertificateUsageDeserializer.h"
#include "ONVIF/CertificateUsageSerializer.h"
#include "ONVIF/DateTimeRangeDeserializer.h"
#include "ONVIF/DateTimeRangeSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::CertificateInformation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::CertificateInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::CertificateInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CertificateID"s,"IssuerDN"s,"SubjectDN"s,"KeyUsage"s,"ExtendedKeyUsage"s,"KeyLength"s,"Version"s,"SerialNum"s,"SignatureAlgorithm"s,"Validity"s,"Extension"s};
		bool ret = false;
		std::string gen_certificateID;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_certificateID);
		if (ret) value.setCertificateID(gen_certificateID);
		Poco::Optional<std::string> gen_issuerDN;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_issuerDN);
		if (ret) value.setIssuerDN(gen_issuerDN);
		Poco::Optional<std::string> gen_subjectDN;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_subjectDN);
		if (ret) value.setSubjectDN(gen_subjectDN);
		Poco::SharedPtr<ONVIF::CertificateUsage> gen_keyUsage;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::CertificateUsage>>::deserialize(REMOTING__NAMES[3], false, deser, gen_keyUsage);
		if (ret) value.setKeyUsage(gen_keyUsage);
		Poco::SharedPtr<ONVIF::CertificateUsage> gen_extendedKeyUsage;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::CertificateUsage>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extendedKeyUsage);
		if (ret) value.setExtendedKeyUsage(gen_extendedKeyUsage);
		Poco::Optional<int> gen_keyLength;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[5], false, deser, gen_keyLength);
		if (ret) value.setKeyLength(gen_keyLength);
		Poco::Optional<std::string> gen_version;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[6], false, deser, gen_version);
		if (ret) value.setVersion(gen_version);
		Poco::Optional<std::string> gen_serialNum;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[7], false, deser, gen_serialNum);
		if (ret) value.setSerialNum(gen_serialNum);
		Poco::Optional<std::string> gen_signatureAlgorithm;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[8], false, deser, gen_signatureAlgorithm);
		if (ret) value.setSignatureAlgorithm(gen_signatureAlgorithm);
		Poco::SharedPtr<ONVIF::DateTimeRange> gen_validity;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DateTimeRange>>::deserialize(REMOTING__NAMES[9], false, deser, gen_validity);
		if (ret) value.setValidity(gen_validity);
		Poco::SharedPtr<ONVIF::CertificateInformationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::CertificateInformationExtension>>::deserialize(REMOTING__NAMES[10], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_CertificateInformation_INCLUDED

