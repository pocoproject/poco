//
// RecordingInformationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RecordingInformation_INCLUDED
#define TypeSerializer_ONVIF_RecordingInformation_INCLUDED


#include "ONVIF/RecordingInformation.h"
#include "ONVIF/RecordingSourceInformationDeserializer.h"
#include "ONVIF/RecordingSourceInformationSerializer.h"
#include "ONVIF/TrackInformationDeserializer.h"
#include "ONVIF/TrackInformationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RecordingInformation>
{
public:
	static void serialize(const std::string& name, const ONVIF::RecordingInformation& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::RecordingInformation& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"Source"s,"EarliestRecording"s,"LatestRecording"s,"Content"s,"Track"s,"RecordingStatus"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getRecordingToken(), ser);
		TypeSerializer<ONVIF::RecordingSourceInformation>::serialize(REMOTING__NAMES[1], value.getSource(), ser);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::Optional<Poco::DateTime>>::serialize(REMOTING__NAMES[2], value.getEarliestRecording(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::Optional<Poco::DateTime>>::serialize(REMOTING__NAMES[3], value.getLatestRecording(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[4], value.getContent(), ser);
		TypeSerializer<std::vector<ONVIF::TrackInformation>>::serialize(REMOTING__NAMES[5], value.getTrack(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[6], value.getRecordingStatus(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RecordingInformation_INCLUDED

