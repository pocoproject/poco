//
// GetRecordingJobsResponseItemDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_GetRecordingJobsResponseItem_INCLUDED
#define TypeDeserializer_ONVIF_GetRecordingJobsResponseItem_INCLUDED


#include "ONVIF/GetRecordingJobsResponseItem.h"
#include "ONVIF/RecordingJobConfigurationDeserializer.h"
#include "ONVIF/RecordingJobConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::GetRecordingJobsResponseItem>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::GetRecordingJobsResponseItem& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::GetRecordingJobsResponseItem& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"JobToken"s,"JobConfiguration"s};
		bool ret = false;
		std::string gen_jobToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_jobToken);
		if (ret) value.setJobToken(gen_jobToken);
		ONVIF::RecordingJobConfiguration gen_jobConfiguration;
		ret = TypeDeserializer<ONVIF::RecordingJobConfiguration>::deserialize(REMOTING__NAMES[1], true, deser, gen_jobConfiguration);
		if (ret) value.setJobConfiguration(gen_jobConfiguration);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_GetRecordingJobsResponseItem_INCLUDED

