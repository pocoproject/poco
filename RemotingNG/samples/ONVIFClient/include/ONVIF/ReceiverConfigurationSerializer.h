//
// ReceiverConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ReceiverConfiguration_INCLUDED
#define TypeSerializer_ONVIF_ReceiverConfiguration_INCLUDED


#include "ONVIF/ReceiverConfiguration.h"
#include "ONVIF/StreamSetupDeserializer.h"
#include "ONVIF/StreamSetupSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ReceiverConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::ReceiverConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ReceiverConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"MediaUri"s,"StreamSetup"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getMode(), ser);
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[1], value.getMediaUri(), ser);
		TypeSerializer<ONVIF::StreamSetup>::serialize(REMOTING__NAMES[2], value.getStreamSetup(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ReceiverConfiguration_INCLUDED

