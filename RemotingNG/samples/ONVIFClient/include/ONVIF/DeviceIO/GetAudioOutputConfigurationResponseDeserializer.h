//
// GetAudioOutputConfigurationResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_GetAudioOutputConfigurationResponse_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_GetAudioOutputConfigurationResponse_INCLUDED


#include "ONVIF/AudioOutputConfigurationDeserializer.h"
#include "ONVIF/AudioOutputConfigurationSerializer.h"
#include "ONVIF/DeviceIO/GetAudioOutputConfigurationResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::GetAudioOutputConfigurationResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::GetAudioOutputConfigurationResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/deviceIO/wsdl"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::GetAudioOutputConfigurationResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AudioOutputConfiguration"s};
		bool ret = false;
		ONVIF::AudioOutputConfiguration gen_audioOutputConfiguration;
		ret = TypeDeserializer<ONVIF::AudioOutputConfiguration>::deserialize(REMOTING__NAMES[0], true, deser, gen_audioOutputConfiguration);
		if (ret) value.setAudioOutputConfiguration(gen_audioOutputConfiguration);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_GetAudioOutputConfigurationResponse_INCLUDED

