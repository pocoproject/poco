//
// MetadataConfigurationOptionsExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MetadataConfigurationOptionsExtension_INCLUDED
#define TypeDeserializer_ONVIF_MetadataConfigurationOptionsExtension_INCLUDED


#include "ONVIF/MetadataConfigurationOptionsExtension.h"
#include "ONVIF/MetadataConfigurationOptionsExtension2Deserializer.h"
#include "ONVIF/MetadataConfigurationOptionsExtension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MetadataConfigurationOptionsExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MetadataConfigurationOptionsExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::MetadataConfigurationOptionsExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CompressionType"s,"Extension"s};
		bool ret = false;
		std::vector<std::string> gen_compressionType;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_compressionType);
		if (ret) value.setCompressionType(gen_compressionType);
		Poco::SharedPtr<ONVIF::MetadataConfigurationOptionsExtension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MetadataConfigurationOptionsExtension2>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MetadataConfigurationOptionsExtension_INCLUDED

