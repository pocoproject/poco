//
// CapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_Capabilities_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_Capabilities_INCLUDED


#include "ONVIF/DeviceIO/Capabilities.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::Capabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::Capabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/deviceIO/wsdl"s);
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::DeviceIO::Capabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"VideoSources"s,"VideoOutputs"s,"AudioSources"s,"AudioOutputs"s,"RelayOutputs"s,"SerialPorts"s,"DigitalInputs"s,"DigitalInputOptions"s};
		bool ret = false;
		Poco::Optional<int> gen_videoSources;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_videoSources);
		if (ret) value.setVideoSources(gen_videoSources);
		Poco::Optional<int> gen_videoOutputs;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[1], false, deser, gen_videoOutputs);
		if (ret) value.setVideoOutputs(gen_videoOutputs);
		Poco::Optional<int> gen_audioSources;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[2], false, deser, gen_audioSources);
		if (ret) value.setAudioSources(gen_audioSources);
		Poco::Optional<int> gen_audioOutputs;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[3], false, deser, gen_audioOutputs);
		if (ret) value.setAudioOutputs(gen_audioOutputs);
		Poco::Optional<int> gen_relayOutputs;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[4], false, deser, gen_relayOutputs);
		if (ret) value.setRelayOutputs(gen_relayOutputs);
		Poco::Optional<int> gen_serialPorts;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[5], false, deser, gen_serialPorts);
		if (ret) value.setSerialPorts(gen_serialPorts);
		Poco::Optional<int> gen_digitalInputs;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[6], false, deser, gen_digitalInputs);
		if (ret) value.setDigitalInputs(gen_digitalInputs);
		Poco::Optional<bool> gen_digitalInputOptions;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[7], false, deser, gen_digitalInputOptions);
		if (ret) value.setDigitalInputOptions(gen_digitalInputOptions);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::Capabilities& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"VideoSources"s,"VideoOutputs"s,"AudioSources"s,"AudioOutputs"s,"RelayOutputs"s,"SerialPorts"s,"DigitalInputs"s,"DigitalInputOptions"s,"http://www.onvif.org/ver10/deviceIO/wsdl"s};
		deser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[2], false);
		deser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[3], false);
		deser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[4], false);
		deser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[5], false);
		deser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[6], false);
		deser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[7], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_Capabilities_INCLUDED

