//
// RecordingJobStateSourceSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RecordingJobStateSource_INCLUDED
#define TypeSerializer_ONVIF_RecordingJobStateSource_INCLUDED


#include "ONVIF/RecordingJobStateSource.h"
#include "ONVIF/RecordingJobStateTracksDeserializer.h"
#include "ONVIF/RecordingJobStateTracksSerializer.h"
#include "ONVIF/SourceReferenceDeserializer.h"
#include "ONVIF/SourceReferenceSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RecordingJobStateSource>
{
public:
	static void serialize(const std::string& name, const ONVIF::RecordingJobStateSource& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::RecordingJobStateSource& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SourceToken"s,"State"s,"Tracks"s,""s};
		TypeSerializer<ONVIF::SourceReference>::serialize(REMOTING__NAMES[0], value.getSourceToken(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getState(), ser);
		TypeSerializer<ONVIF::RecordingJobStateTracks>::serialize(REMOTING__NAMES[2], value.getTracks(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RecordingJobStateSource_INCLUDED

