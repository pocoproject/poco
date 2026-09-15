//
// GetAudioSourceConfigurationResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_GetAudioSourceConfigurationResponse_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_GetAudioSourceConfigurationResponse_INCLUDED


#include "ONVIF/AudioSourceConfigurationDeserializer.h"
#include "ONVIF/AudioSourceConfigurationSerializer.h"
#include "ONVIF/DeviceIO/GetAudioSourceConfigurationResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::GetAudioSourceConfigurationResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::GetAudioSourceConfigurationResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::GetAudioSourceConfigurationResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AudioSourceConfiguration"s};
		bool ret = false;
		ONVIF::AudioSourceConfiguration gen_audioSourceConfiguration;
		ret = TypeDeserializer<ONVIF::AudioSourceConfiguration>::deserialize(REMOTING__NAMES[0], true, deser, gen_audioSourceConfiguration);
		if (ret) value.setAudioSourceConfiguration(gen_audioSourceConfiguration);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_GetAudioSourceConfigurationResponse_INCLUDED

