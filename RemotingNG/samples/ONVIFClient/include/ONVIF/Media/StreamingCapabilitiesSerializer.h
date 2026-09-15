//
// StreamingCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Media_StreamingCapabilities_INCLUDED
#define TypeSerializer_ONVIF_Media_StreamingCapabilities_INCLUDED


#include "ONVIF/Media/StreamingCapabilities.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Media::StreamingCapabilities>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RTPMulticast"s,"RTP_TCP"s,"RTP_RTSP_TCP"s,"NonAggregateControl"s,"NoRTSPStreaming"s,"http://www.onvif.org/ver10/media/wsdl"s};
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[3]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[4]);
	}

	static void serialize(const std::string& name, const ONVIF::Media::StreamingCapabilities& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::Media::StreamingCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RTPMulticast"s,"RTP_TCP"s,"RTP_RTSP_TCP"s,"NonAggregateControl"s,"NoRTSPStreaming"s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getRTPMulticast(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getRTP_TCP(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getRTP_RTSP_TCP(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[3], value.getNonAggregateControl(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[4], value.getNoRTSPStreaming(), ser);
	}

	static void serializeImpl(const ONVIF::Media::StreamingCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Media_StreamingCapabilities_INCLUDED

