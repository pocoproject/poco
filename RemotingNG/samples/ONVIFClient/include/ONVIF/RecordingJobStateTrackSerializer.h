//
// RecordingJobStateTrackSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RecordingJobStateTrack_INCLUDED
#define TypeSerializer_ONVIF_RecordingJobStateTrack_INCLUDED


#include "ONVIF/RecordingJobStateTrack.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RecordingJobStateTrack>
{
public:
	static void serialize(const std::string& name, const ONVIF::RecordingJobStateTrack& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::RecordingJobStateTrack& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SourceTag"s,"Destination"s,"Error"s,"State"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getSourceTag(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getDestination(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getError(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[3], value.getState(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RecordingJobStateTrack_INCLUDED

