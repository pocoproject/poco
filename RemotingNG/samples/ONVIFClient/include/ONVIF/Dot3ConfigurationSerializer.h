//
// Dot3ConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Dot3Configuration_INCLUDED
#define TypeSerializer_ONVIF_Dot3Configuration_INCLUDED


#include "ONVIF/Dot3Configuration.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Dot3Configuration>
{
public:
	static void serialize(const std::string& name, const ONVIF::Dot3Configuration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Dot3Configuration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Dot3Configuration_INCLUDED

