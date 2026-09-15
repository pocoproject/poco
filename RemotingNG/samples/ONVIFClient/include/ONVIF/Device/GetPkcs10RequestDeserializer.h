//
// GetPkcs10RequestDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetPkcs10Request_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetPkcs10Request_INCLUDED


#include "ONVIF/BinaryDataDeserializer.h"
#include "ONVIF/BinaryDataSerializer.h"
#include "ONVIF/Device/GetPkcs10Request.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetPkcs10Request>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetPkcs10Request& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetPkcs10Request& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CertificateID"s,"Subject"s,"Attributes"s};
		bool ret = false;
		std::string gen_certificateID;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_certificateID);
		if (ret) value.setCertificateID(gen_certificateID);
		Poco::Optional<std::string> gen_subject;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_subject);
		if (ret) value.setSubject(gen_subject);
		Poco::SharedPtr<ONVIF::BinaryData> gen_attributes;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::BinaryData>>::deserialize(REMOTING__NAMES[2], false, deser, gen_attributes);
		if (ret) value.setAttributes(gen_attributes);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetPkcs10Request_INCLUDED

