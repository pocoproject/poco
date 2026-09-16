//
// RealTimeStreamingCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RealTimeStreamingCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_RealTimeStreamingCapabilities_INCLUDED


#include "ONVIF/RealTimeStreamingCapabilities.h"
#include "ONVIF/RealTimeStreamingCapabilitiesExtensionDeserializer.h"
#include "ONVIF/RealTimeStreamingCapabilitiesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RealTimeStreamingCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RealTimeStreamingCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::RealTimeStreamingCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RTPMulticast"s,"RTP_TCP"s,"RTP_RTSP_TCP"s,"Extension"s};
		bool ret = false;
		Poco::Optional<bool> gen_rTPMulticast;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_rTPMulticast);
		if (ret) value.setRTPMulticast(gen_rTPMulticast);
		Poco::Optional<bool> gen_rTP_TCP;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_rTP_TCP);
		if (ret) value.setRTP_TCP(gen_rTP_TCP);
		Poco::Optional<bool> gen_rTP_RTSP_TCP;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_rTP_RTSP_TCP);
		if (ret) value.setRTP_RTSP_TCP(gen_rTP_RTSP_TCP);
		Poco::SharedPtr<ONVIF::RealTimeStreamingCapabilitiesExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RealTimeStreamingCapabilitiesExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RealTimeStreamingCapabilities_INCLUDED

