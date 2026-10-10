//
// GetRecordingsResponseItemDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_GetRecordingsResponseItem_INCLUDED
#define TypeDeserializer_ONVIF_GetRecordingsResponseItem_INCLUDED


#include "ONVIF/GetRecordingsResponseItem.h"
#include "ONVIF/GetTracksResponseListDeserializer.h"
#include "ONVIF/GetTracksResponseListSerializer.h"
#include "ONVIF/RecordingConfigurationDeserializer.h"
#include "ONVIF/RecordingConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::GetRecordingsResponseItem>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::GetRecordingsResponseItem& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::GetRecordingsResponseItem& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"Configuration"s,"Tracks"s};
		bool ret = false;
		std::string gen_recordingToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_recordingToken);
		if (ret) value.setRecordingToken(gen_recordingToken);
		ONVIF::RecordingConfiguration gen_configuration;
		ret = TypeDeserializer<ONVIF::RecordingConfiguration>::deserialize(REMOTING__NAMES[1], true, deser, gen_configuration);
		if (ret) value.setConfiguration(gen_configuration);
		ONVIF::GetTracksResponseList gen_tracks;
		ret = TypeDeserializer<ONVIF::GetTracksResponseList>::deserialize(REMOTING__NAMES[2], true, deser, gen_tracks);
		if (ret) value.setTracks(gen_tracks);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_GetRecordingsResponseItem_INCLUDED

