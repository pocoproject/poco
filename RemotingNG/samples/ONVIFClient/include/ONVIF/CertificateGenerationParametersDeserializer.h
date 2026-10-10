//
// CertificateGenerationParametersDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_CertificateGenerationParameters_INCLUDED
#define TypeDeserializer_ONVIF_CertificateGenerationParameters_INCLUDED


#include "ONVIF/CertificateGenerationParameters.h"
#include "ONVIF/CertificateGenerationParametersExtensionDeserializer.h"
#include "ONVIF/CertificateGenerationParametersExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::CertificateGenerationParameters>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::CertificateGenerationParameters& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::CertificateGenerationParameters& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CertificateID"s,"Subject"s,"ValidNotBefore"s,"ValidNotAfter"s,"Extension"s};
		bool ret = false;
		Poco::Optional<std::string> gen_certificateID;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_certificateID);
		if (ret) value.setCertificateID(gen_certificateID);
		Poco::Optional<std::string> gen_subject;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_subject);
		if (ret) value.setSubject(gen_subject);
		Poco::Optional<std::string> gen_validNotBefore;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_validNotBefore);
		if (ret) value.setValidNotBefore(gen_validNotBefore);
		Poco::Optional<std::string> gen_validNotAfter;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[3], false, deser, gen_validNotAfter);
		if (ret) value.setValidNotAfter(gen_validNotAfter);
		Poco::SharedPtr<ONVIF::CertificateGenerationParametersExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::CertificateGenerationParametersExtension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_CertificateGenerationParameters_INCLUDED

