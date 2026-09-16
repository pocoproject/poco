//
// RecordingJobSourceDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingJobSource_INCLUDED
#define TypeDeserializer_ONVIF_RecordingJobSource_INCLUDED


#include "ONVIF/RecordingJobSource.h"
#include "ONVIF/RecordingJobSourceExtensionDeserializer.h"
#include "ONVIF/RecordingJobSourceExtensionSerializer.h"
#include "ONVIF/RecordingJobTrackDeserializer.h"
#include "ONVIF/RecordingJobTrackSerializer.h"
#include "ONVIF/SourceReferenceDeserializer.h"
#include "ONVIF/SourceReferenceSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingJobSource>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingJobSource& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingJobSource& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SourceToken"s,"AutoCreateReceiver"s,"Tracks"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::SourceReference> gen_sourceToken;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SourceReference>>::deserialize(REMOTING__NAMES[0], false, deser, gen_sourceToken);
		if (ret) value.setSourceToken(gen_sourceToken);
		Poco::Optional<bool> gen_autoCreateReceiver;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_autoCreateReceiver);
		if (ret) value.setAutoCreateReceiver(gen_autoCreateReceiver);
		std::vector<ONVIF::RecordingJobTrack> gen_tracks;
		ret = TypeDeserializer<std::vector<ONVIF::RecordingJobTrack>>::deserialize(REMOTING__NAMES[2], false, deser, gen_tracks);
		if (ret) value.setTracks(gen_tracks);
		Poco::SharedPtr<ONVIF::RecordingJobSourceExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RecordingJobSourceExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingJobSource_INCLUDED

