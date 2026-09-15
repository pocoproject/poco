//
// RecordingJobSourceSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RecordingJobSource_INCLUDED
#define TypeSerializer_ONVIF_RecordingJobSource_INCLUDED


#include "ONVIF/RecordingJobSource.h"
#include "ONVIF/RecordingJobSourceExtensionDeserializer.h"
#include "ONVIF/RecordingJobSourceExtensionSerializer.h"
#include "ONVIF/RecordingJobTrackDeserializer.h"
#include "ONVIF/RecordingJobTrackSerializer.h"
#include "ONVIF/SourceReferenceDeserializer.h"
#include "ONVIF/SourceReferenceSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RecordingJobSource>
{
public:
	static void serialize(const std::string& name, const ONVIF::RecordingJobSource& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::RecordingJobSource& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SourceToken"s,"AutoCreateReceiver"s,"Tracks"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::SourceReference>>::serialize(REMOTING__NAMES[0], value.getSourceToken(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getAutoCreateReceiver(), ser);
		TypeSerializer<std::vector<ONVIF::RecordingJobTrack>>::serialize(REMOTING__NAMES[2], value.getTracks(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::RecordingJobSourceExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RecordingJobSource_INCLUDED

