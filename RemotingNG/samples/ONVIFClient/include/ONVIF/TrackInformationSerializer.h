//
// TrackInformationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_TrackInformation_INCLUDED
#define TypeSerializer_ONVIF_TrackInformation_INCLUDED


#include "ONVIF/TrackInformation.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::TrackInformation>
{
public:
	static void serialize(const std::string& name, const ONVIF::TrackInformation& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::TrackInformation& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TrackToken"s,"TrackType"s,"Description"s,"DataFrom"s,"DataTo"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getTrackToken(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getTrackType(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[2], value.getDescription(), ser);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::DateTime>::serialize(REMOTING__NAMES[3], value.getDataFrom(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::DateTime>::serialize(REMOTING__NAMES[4], value.getDataTo(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_TrackInformation_INCLUDED

