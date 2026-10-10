//
// RecordingSourceInformationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RecordingSourceInformation_INCLUDED
#define TypeSerializer_ONVIF_RecordingSourceInformation_INCLUDED


#include "ONVIF/RecordingSourceInformation.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RecordingSourceInformation>
{
public:
	static void serialize(const std::string& name, const ONVIF::RecordingSourceInformation& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::RecordingSourceInformation& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SourceId"s,"Name"s,"Location"s,"Description"s,"Address"s,""s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getSourceId(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getName(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[2], value.getLocation(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[3], value.getDescription(), ser);
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[4], value.getAddress(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RecordingSourceInformation_INCLUDED

