//
// GetRecordingsResponseItemSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_GetRecordingsResponseItem_INCLUDED
#define TypeSerializer_ONVIF_GetRecordingsResponseItem_INCLUDED


#include "ONVIF/GetRecordingsResponseItem.h"
#include "ONVIF/GetTracksResponseListDeserializer.h"
#include "ONVIF/GetTracksResponseListSerializer.h"
#include "ONVIF/RecordingConfigurationDeserializer.h"
#include "ONVIF/RecordingConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::GetRecordingsResponseItem>
{
public:
	static void serialize(const std::string& name, const ONVIF::GetRecordingsResponseItem& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::GetRecordingsResponseItem& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"Configuration"s,"Tracks"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getRecordingToken(), ser);
		TypeSerializer<ONVIF::RecordingConfiguration>::serialize(REMOTING__NAMES[1], value.getConfiguration(), ser);
		TypeSerializer<ONVIF::GetTracksResponseList>::serialize(REMOTING__NAMES[2], value.getTracks(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_GetRecordingsResponseItem_INCLUDED

