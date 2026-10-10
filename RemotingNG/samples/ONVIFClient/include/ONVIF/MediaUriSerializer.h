//
// MediaUriSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_MediaUri_INCLUDED
#define TypeSerializer_ONVIF_MediaUri_INCLUDED


#include "ONVIF/MediaUri.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::MediaUri>
{
public:
	static void serialize(const std::string& name, const ONVIF::MediaUri& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::MediaUri& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Uri"s,"InvalidAfterConnect"s,"InvalidAfterReboot"s,"Timeout"s,""s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getUri(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getInvalidAfterConnect(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[2], value.getInvalidAfterReboot(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[3], value.getTimeout(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_MediaUri_INCLUDED

