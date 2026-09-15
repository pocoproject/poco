//
// MediaAttributesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_MediaAttributes_INCLUDED
#define TypeSerializer_ONVIF_MediaAttributes_INCLUDED


#include "ONVIF/MediaAttributes.h"
#include "ONVIF/TrackAttributesDeserializer.h"
#include "ONVIF/TrackAttributesSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::MediaAttributes>
{
public:
	static void serialize(const std::string& name, const ONVIF::MediaAttributes& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::MediaAttributes& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"TrackAttributes"s,"From"s,"Until"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getRecordingToken(), ser);
		TypeSerializer<std::vector<ONVIF::TrackAttributes>>::serialize(REMOTING__NAMES[1], value.getTrackAttributes(), ser);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::DateTime>::serialize(REMOTING__NAMES[2], value.getFrom(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::DateTime>::serialize(REMOTING__NAMES[3], value.getUntil(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_MediaAttributes_INCLUDED

