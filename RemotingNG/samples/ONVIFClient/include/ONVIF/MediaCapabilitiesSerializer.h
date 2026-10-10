//
// MediaCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_MediaCapabilities_INCLUDED
#define TypeSerializer_ONVIF_MediaCapabilities_INCLUDED


#include "ONVIF/MediaCapabilities.h"
#include "ONVIF/MediaCapabilitiesExtensionDeserializer.h"
#include "ONVIF/MediaCapabilitiesExtensionSerializer.h"
#include "ONVIF/RealTimeStreamingCapabilitiesDeserializer.h"
#include "ONVIF/RealTimeStreamingCapabilitiesSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::MediaCapabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::MediaCapabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::MediaCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"StreamingCapabilities"s,"Extension"s,""s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getXAddr(), ser);
		TypeSerializer<ONVIF::RealTimeStreamingCapabilities>::serialize(REMOTING__NAMES[1], value.getStreamingCapabilities(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::MediaCapabilitiesExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_MediaCapabilities_INCLUDED

