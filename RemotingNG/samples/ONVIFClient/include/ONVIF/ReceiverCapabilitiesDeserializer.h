//
// ReceiverCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ReceiverCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_ReceiverCapabilities_INCLUDED


#include "ONVIF/ReceiverCapabilities.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ReceiverCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ReceiverCapabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ReceiverCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"RTP_Multicast"s,"RTP_TCP"s,"RTP_RTSP_TCP"s,"SupportedReceivers"s,"MaximumRTSPURILength"s};
		bool ret = false;
		Poco::URI gen_xAddr;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_xAddr);
		if (ret) value.setXAddr(gen_xAddr);
		bool gen_rTP_Multicast;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_rTP_Multicast);
		if (ret) value.setRTP_Multicast(gen_rTP_Multicast);
		bool gen_rTP_TCP;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_rTP_TCP);
		if (ret) value.setRTP_TCP(gen_rTP_TCP);
		bool gen_rTP_RTSP_TCP;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[3], true, deser, gen_rTP_RTSP_TCP);
		if (ret) value.setRTP_RTSP_TCP(gen_rTP_RTSP_TCP);
		int gen_supportedReceivers;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[4], true, deser, gen_supportedReceivers);
		if (ret) value.setSupportedReceivers(gen_supportedReceivers);
		int gen_maximumRTSPURILength;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[5], true, deser, gen_maximumRTSPURILength);
		if (ret) value.setMaximumRTSPURILength(gen_maximumRTSPURILength);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ReceiverCapabilities_INCLUDED

