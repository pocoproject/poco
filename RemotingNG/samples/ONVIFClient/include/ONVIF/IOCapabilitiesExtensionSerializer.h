//
// IOCapabilitiesExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_IOCapabilitiesExtension_INCLUDED
#define TypeSerializer_ONVIF_IOCapabilitiesExtension_INCLUDED


#include "ONVIF/IOCapabilitiesExtension.h"
#include "ONVIF/IOCapabilitiesExtension2Deserializer.h"
#include "ONVIF/IOCapabilitiesExtension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::IOCapabilitiesExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::IOCapabilitiesExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::IOCapabilitiesExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Auxiliary"s,"AuxiliaryCommands"s,"Extension"s,""s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getAuxiliary(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[1], value.getAuxiliaryCommands(), ser);
		TypeSerializer<ONVIF::IOCapabilitiesExtension2>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_IOCapabilitiesExtension_INCLUDED

