//
// DeviceIOCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIOCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIOCapabilities_INCLUDED


#include "ONVIF/DeviceIOCapabilities.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIOCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIOCapabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIOCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"VideoSources"s,"VideoOutputs"s,"AudioSources"s,"AudioOutputs"s,"RelayOutputs"s};
		bool ret = false;
		Poco::URI gen_xAddr;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_xAddr);
		if (ret) value.setXAddr(gen_xAddr);
		int gen_videoSources;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_videoSources);
		if (ret) value.setVideoSources(gen_videoSources);
		int gen_videoOutputs;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_videoOutputs);
		if (ret) value.setVideoOutputs(gen_videoOutputs);
		int gen_audioSources;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[3], true, deser, gen_audioSources);
		if (ret) value.setAudioSources(gen_audioSources);
		int gen_audioOutputs;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[4], true, deser, gen_audioOutputs);
		if (ret) value.setAudioOutputs(gen_audioOutputs);
		int gen_relayOutputs;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[5], true, deser, gen_relayOutputs);
		if (ret) value.setRelayOutputs(gen_relayOutputs);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIOCapabilities_INCLUDED

