//
// MetadataConfigurationOptionsExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_MetadataConfigurationOptionsExtension_INCLUDED
#define TypeSerializer_ONVIF_MetadataConfigurationOptionsExtension_INCLUDED


#include "ONVIF/MetadataConfigurationOptionsExtension.h"
#include "ONVIF/MetadataConfigurationOptionsExtension2Deserializer.h"
#include "ONVIF/MetadataConfigurationOptionsExtension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::MetadataConfigurationOptionsExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::MetadataConfigurationOptionsExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::MetadataConfigurationOptionsExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CompressionType"s,"Extension"s,""s};
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[0], value.getCompressionType(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::MetadataConfigurationOptionsExtension2>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_MetadataConfigurationOptionsExtension_INCLUDED

