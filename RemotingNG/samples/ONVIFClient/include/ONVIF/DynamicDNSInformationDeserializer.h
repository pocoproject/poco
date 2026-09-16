//
// DynamicDNSInformationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DynamicDNSInformation_INCLUDED
#define TypeDeserializer_ONVIF_DynamicDNSInformation_INCLUDED


#include "ONVIF/DynamicDNSInformation.h"
#include "ONVIF/DynamicDNSInformationExtensionDeserializer.h"
#include "ONVIF/DynamicDNSInformationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DynamicDNSInformation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DynamicDNSInformation& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DynamicDNSInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"Name"s,"TTL"s,"Extension"s};
		bool ret = false;
		std::string gen_type;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_type);
		if (ret) value.setType(gen_type);
		Poco::Optional<std::string> gen_name;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_name);
		if (ret) value.setName(gen_name);
		Poco::Optional<std::string> gen_tTL;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_tTL);
		if (ret) value.setTTL(gen_tTL);
		Poco::SharedPtr<ONVIF::DynamicDNSInformationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DynamicDNSInformationExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DynamicDNSInformation_INCLUDED

