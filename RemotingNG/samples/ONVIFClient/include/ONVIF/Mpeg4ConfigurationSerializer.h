//
// Mpeg4ConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Mpeg4Configuration_INCLUDED
#define TypeSerializer_ONVIF_Mpeg4Configuration_INCLUDED


#include "ONVIF/Mpeg4Configuration.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Mpeg4Configuration>
{
public:
	static void serialize(const std::string& name, const ONVIF::Mpeg4Configuration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Mpeg4Configuration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GovLength"s,"Mpeg4Profile"s,""s};
		TypeSerializer<int>::serialize(REMOTING__NAMES[0], value.getGovLength(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getMpeg4Profile(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Mpeg4Configuration_INCLUDED

