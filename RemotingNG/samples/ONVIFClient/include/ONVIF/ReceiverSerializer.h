//
// ReceiverSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Receiver_INCLUDED
#define TypeSerializer_ONVIF_Receiver_INCLUDED


#include "ONVIF/Receiver.h"
#include "ONVIF/ReceiverConfigurationDeserializer.h"
#include "ONVIF/ReceiverConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Receiver>
{
public:
	static void serialize(const std::string& name, const ONVIF::Receiver& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Receiver& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Token"s,"Configuration"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getToken(), ser);
		TypeSerializer<ONVIF::ReceiverConfiguration>::serialize(REMOTING__NAMES[1], value.getConfiguration(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Receiver_INCLUDED

