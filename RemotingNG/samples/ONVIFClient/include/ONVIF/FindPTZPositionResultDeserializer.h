//
// FindPTZPositionResultDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_FindPTZPositionResult_INCLUDED
#define TypeDeserializer_ONVIF_FindPTZPositionResult_INCLUDED


#include "ONVIF/FindPTZPositionResult.h"
#include "ONVIF/PTZVectorDeserializer.h"
#include "ONVIF/PTZVectorSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::FindPTZPositionResult>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::FindPTZPositionResult& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::FindPTZPositionResult& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"TrackToken"s,"Time"s,"Position"s};
		bool ret = false;
		std::string gen_recordingToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_recordingToken);
		if (ret) value.setRecordingToken(gen_recordingToken);
		std::string gen_trackToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_trackToken);
		if (ret) value.setTrackToken(gen_trackToken);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_time;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[2], true, deser, gen_time);
		if (ret) value.setTime(gen_time);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		ONVIF::PTZVector gen_position;
		ret = TypeDeserializer<ONVIF::PTZVector>::deserialize(REMOTING__NAMES[3], true, deser, gen_position);
		if (ret) value.setPosition(gen_position);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_FindPTZPositionResult_INCLUDED

