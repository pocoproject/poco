//
// RemoteUserSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RemoteUser_INCLUDED
#define TypeSerializer_ONVIF_RemoteUser_INCLUDED


#include "ONVIF/RemoteUser.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RemoteUser>
{
public:
	static void serialize(const std::string& name, const ONVIF::RemoteUser& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::RemoteUser& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Username"s,"Password"s,"UseDerivedPassword"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getUsername(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getPassword(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[2], value.getUseDerivedPassword(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RemoteUser_INCLUDED

