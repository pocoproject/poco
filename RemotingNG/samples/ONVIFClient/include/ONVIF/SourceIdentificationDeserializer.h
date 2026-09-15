//
// SourceIdentificationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SourceIdentification_INCLUDED
#define TypeDeserializer_ONVIF_SourceIdentification_INCLUDED


#include "ONVIF/SourceIdentification.h"
#include "ONVIF/SourceIdentificationExtensionDeserializer.h"
#include "ONVIF/SourceIdentificationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SourceIdentification>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SourceIdentification& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::SourceIdentification& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"Token"s,"Extension"s};
		bool ret = false;
		std::string gen_name;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_name);
		if (ret) value.setName(gen_name);
		std::vector<std::string> gen_token;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[1], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
		Poco::SharedPtr<ONVIF::SourceIdentificationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SourceIdentificationExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SourceIdentification_INCLUDED

