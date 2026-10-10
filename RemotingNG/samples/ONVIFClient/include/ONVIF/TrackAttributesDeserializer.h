//
// TrackAttributesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_TrackAttributes_INCLUDED
#define TypeDeserializer_ONVIF_TrackAttributes_INCLUDED


#include "ONVIF/AudioAttributesDeserializer.h"
#include "ONVIF/AudioAttributesSerializer.h"
#include "ONVIF/MetadataAttributesDeserializer.h"
#include "ONVIF/MetadataAttributesSerializer.h"
#include "ONVIF/TrackAttributes.h"
#include "ONVIF/TrackAttributesExtensionDeserializer.h"
#include "ONVIF/TrackAttributesExtensionSerializer.h"
#include "ONVIF/TrackInformationDeserializer.h"
#include "ONVIF/TrackInformationSerializer.h"
#include "ONVIF/VideoAttributesDeserializer.h"
#include "ONVIF/VideoAttributesSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::TrackAttributes>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::TrackAttributes& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::TrackAttributes& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TrackInformation"s,"VideoAttributes"s,"AudioAttributes"s,"MetadataAttributes"s,"Extension"s};
		bool ret = false;
		ONVIF::TrackInformation gen_trackInformation;
		ret = TypeDeserializer<ONVIF::TrackInformation>::deserialize(REMOTING__NAMES[0], true, deser, gen_trackInformation);
		if (ret) value.setTrackInformation(gen_trackInformation);
		Poco::SharedPtr<ONVIF::VideoAttributes> gen_videoAttributes;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoAttributes>>::deserialize(REMOTING__NAMES[1], false, deser, gen_videoAttributes);
		if (ret) value.setVideoAttributes(gen_videoAttributes);
		Poco::SharedPtr<ONVIF::AudioAttributes> gen_audioAttributes;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AudioAttributes>>::deserialize(REMOTING__NAMES[2], false, deser, gen_audioAttributes);
		if (ret) value.setAudioAttributes(gen_audioAttributes);
		Poco::SharedPtr<ONVIF::MetadataAttributes> gen_metadataAttributes;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MetadataAttributes>>::deserialize(REMOTING__NAMES[3], false, deser, gen_metadataAttributes);
		if (ret) value.setMetadataAttributes(gen_metadataAttributes);
		Poco::SharedPtr<ONVIF::TrackAttributesExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::TrackAttributesExtension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_TrackAttributes_INCLUDED

