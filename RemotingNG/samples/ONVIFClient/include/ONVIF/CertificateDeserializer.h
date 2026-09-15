//
// CertificateDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Certificate_INCLUDED
#define TypeDeserializer_ONVIF_Certificate_INCLUDED


#include "ONVIF/BinaryDataDeserializer.h"
#include "ONVIF/BinaryDataSerializer.h"
#include "ONVIF/Certificate.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Certificate>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Certificate& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Certificate& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CertificateID"s,"Certificate"s};
		bool ret = false;
		std::string gen_certificateID;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_certificateID);
		if (ret) value.setCertificateID(gen_certificateID);
		ONVIF::BinaryData gen_certificate;
		ret = TypeDeserializer<ONVIF::BinaryData>::deserialize(REMOTING__NAMES[1], true, deser, gen_certificate);
		if (ret) value.setCertificate(gen_certificate);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Certificate_INCLUDED

