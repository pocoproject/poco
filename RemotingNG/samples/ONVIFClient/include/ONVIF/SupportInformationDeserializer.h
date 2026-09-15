//
// SupportInformationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SupportInformation_INCLUDED
#define TypeDeserializer_ONVIF_SupportInformation_INCLUDED


#include "ONVIF/AttachmentDataDeserializer.h"
#include "ONVIF/AttachmentDataSerializer.h"
#include "ONVIF/SupportInformation.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SupportInformation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SupportInformation& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::SupportInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Binary"s,"String"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::AttachmentData> gen_binary;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AttachmentData>>::deserialize(REMOTING__NAMES[0], false, deser, gen_binary);
		if (ret) value.setBinary(gen_binary);
		Poco::Optional<std::string> gen_string;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_string);
		if (ret) value.setString(gen_string);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SupportInformation_INCLUDED

