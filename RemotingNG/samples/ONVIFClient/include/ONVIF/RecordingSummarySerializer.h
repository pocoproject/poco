//
// RecordingSummarySerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RecordingSummary_INCLUDED
#define TypeSerializer_ONVIF_RecordingSummary_INCLUDED


#include "ONVIF/RecordingSummary.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RecordingSummary>
{
public:
	static void serialize(const std::string& name, const ONVIF::RecordingSummary& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::RecordingSummary& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DataFrom"s,"DataUntil"s,"NumberRecordings"s,""s};
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::DateTime>::serialize(REMOTING__NAMES[0], value.getDataFrom(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::DateTime>::serialize(REMOTING__NAMES[1], value.getDataUntil(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
		TypeSerializer<int>::serialize(REMOTING__NAMES[2], value.getNumberRecordings(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RecordingSummary_INCLUDED

