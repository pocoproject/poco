//
// FindPTZPositionResultSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_FindPTZPositionResult_INCLUDED
#define TypeSerializer_ONVIF_FindPTZPositionResult_INCLUDED


#include "ONVIF/FindPTZPositionResult.h"
#include "ONVIF/PTZVectorDeserializer.h"
#include "ONVIF/PTZVectorSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::FindPTZPositionResult>
{
public:
	static void serialize(const std::string& name, const ONVIF::FindPTZPositionResult& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::FindPTZPositionResult& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"TrackToken"s,"Time"s,"Position"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getRecordingToken(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getTrackToken(), ser);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::DateTime>::serialize(REMOTING__NAMES[2], value.getTime(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
		TypeSerializer<ONVIF::PTZVector>::serialize(REMOTING__NAMES[3], value.getPosition(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_FindPTZPositionResult_INCLUDED

