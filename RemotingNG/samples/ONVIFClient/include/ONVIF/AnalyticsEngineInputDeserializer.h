//
// AnalyticsEngineInputDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AnalyticsEngineInput_INCLUDED
#define TypeDeserializer_ONVIF_AnalyticsEngineInput_INCLUDED


#include "ONVIF/AnalyticsEngineInput.h"
#include "ONVIF/MetadataInputDeserializer.h"
#include "ONVIF/MetadataInputSerializer.h"
#include "ONVIF/SourceIdentificationDeserializer.h"
#include "ONVIF/SourceIdentificationSerializer.h"
#include "ONVIF/VideoEncoderConfigurationDeserializer.h"
#include "ONVIF/VideoEncoderConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AnalyticsEngineInput>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AnalyticsEngineInput& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::AnalyticsEngineInput& value)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s};
		bool ret = false;
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
		}
		
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::AnalyticsEngineInput& value)
	{
		using namespace std::string_literals;
		
		// ONVIF::ConfigurationEntity
		{
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"Name"s,"UseCount"s};
		bool ret = false;
		std::string gen_name;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], true, deser, gen_name);
		if (ret) value.setName(gen_name);
		int gen_useCount;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], true, deser, gen_useCount);
		if (ret) value.setUseCount(gen_useCount);
		}
		
		static const std::string REMOTING__NAMES[] = {"SourceIdentification"s,"VideoInput"s,"MetadataInput"s};
		bool ret = false;
		ONVIF::SourceIdentification gen_sourceIdentification;
		ret = TypeDeserializer<ONVIF::SourceIdentification>::deserialize(REMOTING__NAMES[0], true, deser, gen_sourceIdentification);
		if (ret) value.setSourceIdentification(gen_sourceIdentification);
		ONVIF::VideoEncoderConfiguration gen_videoInput;
		ret = TypeDeserializer<ONVIF::VideoEncoderConfiguration>::deserialize(REMOTING__NAMES[1], true, deser, gen_videoInput);
		if (ret) value.setVideoInput(gen_videoInput);
		ONVIF::MetadataInput gen_metadataInput;
		ret = TypeDeserializer<ONVIF::MetadataInput>::deserialize(REMOTING__NAMES[2], true, deser, gen_metadataInput);
		if (ret) value.setMetadataInput(gen_metadataInput);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], true);
		}
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AnalyticsEngineInput_INCLUDED

