//
// RecordingJobConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingJobConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_RecordingJobConfiguration_INCLUDED


#include "ONVIF/RecordingEventFilterDeserializer.h"
#include "ONVIF/RecordingEventFilterSerializer.h"
#include "ONVIF/RecordingJobConfiguration.h"
#include "ONVIF/RecordingJobConfigurationExtensionDeserializer.h"
#include "ONVIF/RecordingJobConfigurationExtensionSerializer.h"
#include "ONVIF/RecordingJobSourceDeserializer.h"
#include "ONVIF/RecordingJobSourceSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingJobConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingJobConfiguration& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::RecordingJobConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ScheduleToken"s};
		bool ret = false;
		Poco::Optional<std::string> gen_scheduleToken;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_scheduleToken);
		if (ret) value.setScheduleToken(gen_scheduleToken);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingJobConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"Mode"s,"Priority"s,"Source"s,"Extension"s,"EventFilter"s};
		bool ret = false;
		std::string gen_recordingToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_recordingToken);
		if (ret) value.setRecordingToken(gen_recordingToken);
		std::string gen_mode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		int gen_priority;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_priority);
		if (ret) value.setPriority(gen_priority);
		std::vector<ONVIF::RecordingJobSource> gen_source;
		ret = TypeDeserializer<std::vector<ONVIF::RecordingJobSource>>::deserialize(REMOTING__NAMES[3], false, deser, gen_source);
		if (ret) value.setSource(gen_source);
		Poco::SharedPtr<ONVIF::RecordingJobConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RecordingJobConfigurationExtension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
		Poco::SharedPtr<ONVIF::RecordingEventFilter> gen_eventFilter;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RecordingEventFilter>>::deserialize(REMOTING__NAMES[5], false, deser, gen_eventFilter);
		if (ret) value.setEventFilter(gen_eventFilter);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ScheduleToken"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingJobConfiguration_INCLUDED

