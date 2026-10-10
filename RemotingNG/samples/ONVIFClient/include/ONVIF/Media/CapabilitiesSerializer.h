//
// CapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Media_Capabilities_INCLUDED
#define TypeSerializer_ONVIF_Media_Capabilities_INCLUDED


#include "ONVIF/Media/Capabilities.h"
#include "ONVIF/Media/ProfileCapabilitiesDeserializer.h"
#include "ONVIF/Media/ProfileCapabilitiesSerializer.h"
#include "ONVIF/Media/StreamingCapabilitiesDeserializer.h"
#include "ONVIF/Media/StreamingCapabilitiesSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Media::Capabilities>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SnapshotUri"s,"Rotation"s,"VideoSourceMode"s,"OSD"s,"TemporaryOSDText"s,"EXICompression"s,"http://www.onvif.org/ver10/media/wsdl"s};
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[3]);
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[4]);
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[5]);
	}

	static void serialize(const std::string& name, const ONVIF::Media::Capabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/media/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::Media::Capabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SnapshotUri"s,"Rotation"s,"VideoSourceMode"s,"OSD"s,"TemporaryOSDText"s,"EXICompression"s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getSnapshotUri(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getRotation(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getVideoSourceMode(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[3], value.getOSD(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[4], value.getTemporaryOSDText(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[5], value.getEXICompression(), ser);
	}

	static void serializeImpl(const ONVIF::Media::Capabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ProfileCapabilities"s,"StreamingCapabilities"s,""s};
		TypeSerializer<ONVIF::Media::ProfileCapabilities>::serialize(REMOTING__NAMES[0], value.getProfileCapabilities(), ser);
		TypeSerializer<ONVIF::Media::StreamingCapabilities>::serialize(REMOTING__NAMES[1], value.getStreamingCapabilities(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Media_Capabilities_INCLUDED

