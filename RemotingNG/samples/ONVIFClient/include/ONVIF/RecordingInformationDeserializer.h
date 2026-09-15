//
// RecordingInformationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingInformation_INCLUDED
#define TypeDeserializer_ONVIF_RecordingInformation_INCLUDED


#include "ONVIF/RecordingInformation.h"
#include "ONVIF/RecordingSourceInformationDeserializer.h"
#include "ONVIF/RecordingSourceInformationSerializer.h"
#include "ONVIF/TrackInformationDeserializer.h"
#include "ONVIF/TrackInformationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingInformation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingInformation& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"Source"s,"EarliestRecording"s,"LatestRecording"s,"Content"s,"Track"s,"RecordingStatus"s};
		bool ret = false;
		std::string gen_recordingToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_recordingToken);
		if (ret) value.setRecordingToken(gen_recordingToken);
		ONVIF::RecordingSourceInformation gen_source;
		ret = TypeDeserializer<ONVIF::RecordingSourceInformation>::deserialize(REMOTING__NAMES[1], true, deser, gen_source);
		if (ret) value.setSource(gen_source);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::Optional<Poco::DateTime> gen_earliestRecording;
		ret = TypeDeserializer<Poco::Optional<Poco::DateTime>>::deserialize(REMOTING__NAMES[2], false, deser, gen_earliestRecording);
		if (ret) value.setEarliestRecording(gen_earliestRecording);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::Optional<Poco::DateTime> gen_latestRecording;
		ret = TypeDeserializer<Poco::Optional<Poco::DateTime>>::deserialize(REMOTING__NAMES[3], false, deser, gen_latestRecording);
		if (ret) value.setLatestRecording(gen_latestRecording);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		std::string gen_content;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[4], true, deser, gen_content);
		if (ret) value.setContent(gen_content);
		std::vector<ONVIF::TrackInformation> gen_track;
		ret = TypeDeserializer<std::vector<ONVIF::TrackInformation>>::deserialize(REMOTING__NAMES[5], false, deser, gen_track);
		if (ret) value.setTrack(gen_track);
		std::string gen_recordingStatus;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[6], true, deser, gen_recordingStatus);
		if (ret) value.setRecordingStatus(gen_recordingStatus);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingInformation_INCLUDED

