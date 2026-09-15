//
// GetDigitalInputConfigurationOptionsResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_GetDigitalInputConfigurationOptionsResponse_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_GetDigitalInputConfigurationOptionsResponse_INCLUDED


#include "ONVIF/DeviceIO/DigitalInputConfigurationOptionsDeserializer.h"
#include "ONVIF/DeviceIO/DigitalInputConfigurationOptionsSerializer.h"
#include "ONVIF/DeviceIO/GetDigitalInputConfigurationOptionsResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::GetDigitalInputConfigurationOptionsResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::GetDigitalInputConfigurationOptionsResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::GetDigitalInputConfigurationOptionsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DigitalInputOptions"s};
		bool ret = false;
		ONVIF::DeviceIO::DigitalInputConfigurationOptions gen_digitalInputOptions;
		ret = TypeDeserializer<ONVIF::DeviceIO::DigitalInputConfigurationOptions>::deserialize(REMOTING__NAMES[0], true, deser, gen_digitalInputOptions);
		if (ret) value.setDigitalInputOptions(gen_digitalInputOptions);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_GetDigitalInputConfigurationOptionsResponse_INCLUDED

