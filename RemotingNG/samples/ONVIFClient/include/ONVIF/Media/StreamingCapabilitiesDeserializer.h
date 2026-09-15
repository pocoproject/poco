//
// StreamingCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_StreamingCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_Media_StreamingCapabilities_INCLUDED


#include "ONVIF/Media/StreamingCapabilities.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::StreamingCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::StreamingCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/media/wsdl"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::Media::StreamingCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RTPMulticast"s,"RTP_TCP"s,"RTP_RTSP_TCP"s,"NonAggregateControl"s,"NoRTSPStreaming"s};
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
		Poco::Optional<bool> gen_nonAggregateControl;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[3], false, deser, gen_nonAggregateControl);
		if (ret) value.setNonAggregateControl(gen_nonAggregateControl);
		Poco::Optional<bool> gen_noRTSPStreaming;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[4], false, deser, gen_noRTSPStreaming);
		if (ret) value.setNoRTSPStreaming(gen_noRTSPStreaming);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::StreamingCapabilities& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RTPMulticast"s,"RTP_TCP"s,"RTP_RTSP_TCP"s,"NonAggregateControl"s,"NoRTSPStreaming"s,"http://www.onvif.org/ver10/media/wsdl"s};
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[2], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[3], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[4], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_StreamingCapabilities_INCLUDED

