//
// RecordingJobStateSourceDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingJobStateSource_INCLUDED
#define TypeDeserializer_ONVIF_RecordingJobStateSource_INCLUDED


#include "ONVIF/RecordingJobStateSource.h"
#include "ONVIF/RecordingJobStateTracksDeserializer.h"
#include "ONVIF/RecordingJobStateTracksSerializer.h"
#include "ONVIF/SourceReferenceDeserializer.h"
#include "ONVIF/SourceReferenceSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingJobStateSource>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingJobStateSource& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingJobStateSource& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SourceToken"s,"State"s,"Tracks"s};
		bool ret = false;
		ONVIF::SourceReference gen_sourceToken;
		ret = TypeDeserializer<ONVIF::SourceReference>::deserialize(REMOTING__NAMES[0], true, deser, gen_sourceToken);
		if (ret) value.setSourceToken(gen_sourceToken);
		std::string gen_state;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_state);
		if (ret) value.setState(gen_state);
		ONVIF::RecordingJobStateTracks gen_tracks;
		ret = TypeDeserializer<ONVIF::RecordingJobStateTracks>::deserialize(REMOTING__NAMES[2], true, deser, gen_tracks);
		if (ret) value.setTracks(gen_tracks);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingJobStateSource_INCLUDED

