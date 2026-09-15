//
// MetadataInputDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MetadataInput_INCLUDED
#define TypeDeserializer_ONVIF_MetadataInput_INCLUDED


#include "ONVIF/ConfigDeserializer.h"
#include "ONVIF/ConfigSerializer.h"
#include "ONVIF/MetadataInput.h"
#include "ONVIF/MetadataInputExtensionDeserializer.h"
#include "ONVIF/MetadataInputExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MetadataInput>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MetadataInput& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::MetadataInput& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MetadataConfig"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::Config> gen_metadataConfig;
		ret = TypeDeserializer<std::vector<ONVIF::Config>>::deserialize(REMOTING__NAMES[0], false, deser, gen_metadataConfig);
		if (ret) value.setMetadataConfig(gen_metadataConfig);
		Poco::SharedPtr<ONVIF::MetadataInputExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MetadataInputExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MetadataInput_INCLUDED

