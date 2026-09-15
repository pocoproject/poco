//
// ColorDescriptorSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ColorDescriptor_INCLUDED
#define TypeSerializer_ONVIF_ColorDescriptor_INCLUDED


#include "ONVIF/ColorClusterDeserializer.h"
#include "ONVIF/ColorClusterSerializer.h"
#include "ONVIF/ColorDescriptor.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ColorDescriptor>
{
public:
	static void serialize(const std::string& name, const ONVIF::ColorDescriptor& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ColorDescriptor& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ColorCluster"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::ColorCluster>>::serialize(REMOTING__NAMES[0], value.getColorCluster(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ColorDescriptor_INCLUDED

