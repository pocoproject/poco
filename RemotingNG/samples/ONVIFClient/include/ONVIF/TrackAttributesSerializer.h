//
// TrackAttributesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_TrackAttributes_INCLUDED
#define TypeSerializer_ONVIF_TrackAttributes_INCLUDED


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
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::TrackAttributes>
{
public:
	static void serialize(const std::string& name, const ONVIF::TrackAttributes& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::TrackAttributes& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TrackInformation"s,"VideoAttributes"s,"AudioAttributes"s,"MetadataAttributes"s,"Extension"s,""s};
		TypeSerializer<ONVIF::TrackInformation>::serialize(REMOTING__NAMES[0], value.getTrackInformation(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoAttributes>>::serialize(REMOTING__NAMES[1], value.getVideoAttributes(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AudioAttributes>>::serialize(REMOTING__NAMES[2], value.getAudioAttributes(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::MetadataAttributes>>::serialize(REMOTING__NAMES[3], value.getMetadataAttributes(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::TrackAttributesExtension>>::serialize(REMOTING__NAMES[4], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_TrackAttributes_INCLUDED

