//
// RecordingConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_RecordingConfiguration_INCLUDED


#include "ONVIF/RecordingConfiguration.h"
#include "ONVIF/RecordingSourceInformationDeserializer.h"
#include "ONVIF/RecordingSourceInformationSerializer.h"
#include "ONVIF/RecordingTargetConfigurationDeserializer.h"
#include "ONVIF/RecordingTargetConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Source"s,"Content"s,"MaximumRetentionTime"s,"Target"s};
		bool ret = false;
		ONVIF::RecordingSourceInformation gen_source;
		ret = TypeDeserializer<ONVIF::RecordingSourceInformation>::deserialize(REMOTING__NAMES[0], true, deser, gen_source);
		if (ret) value.setSource(gen_source);
		std::string gen_content;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_content);
		if (ret) value.setContent(gen_content);
		std::string gen_maximumRetentionTime;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[2], true, deser, gen_maximumRetentionTime);
		if (ret) value.setMaximumRetentionTime(gen_maximumRetentionTime);
		Poco::SharedPtr<ONVIF::RecordingTargetConfiguration> gen_target;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RecordingTargetConfiguration>>::deserialize(REMOTING__NAMES[3], false, deser, gen_target);
		if (ret) value.setTarget(gen_target);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingConfiguration_INCLUDED

