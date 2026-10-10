//
// DateSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Date_INCLUDED
#define TypeSerializer_ONVIF_Date_INCLUDED


#include "ONVIF/Date.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Date>
{
public:
	static void serialize(const std::string& name, const ONVIF::Date& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Date& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Year"s,"Month"s,"Day"s,""s};
		TypeSerializer<int>::serialize(REMOTING__NAMES[0], value.getYear(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[1], value.getMonth(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[2], value.getDay(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Date_INCLUDED

