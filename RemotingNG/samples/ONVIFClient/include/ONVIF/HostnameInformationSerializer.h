//
// HostnameInformationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_HostnameInformation_INCLUDED
#define TypeSerializer_ONVIF_HostnameInformation_INCLUDED


#include "ONVIF/HostnameInformation.h"
#include "ONVIF/HostnameInformationExtensionDeserializer.h"
#include "ONVIF/HostnameInformationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::HostnameInformation>
{
public:
	static void serialize(const std::string& name, const ONVIF::HostnameInformation& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::HostnameInformation& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FromDHCP"s,"Name"s,"Extension"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getFromDHCP(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getName(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::HostnameInformationExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_HostnameInformation_INCLUDED

