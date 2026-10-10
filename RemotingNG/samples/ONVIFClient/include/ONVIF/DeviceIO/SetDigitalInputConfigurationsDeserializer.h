//
// SetDigitalInputConfigurationsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_SetDigitalInputConfigurations_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_SetDigitalInputConfigurations_INCLUDED


#include "ONVIF/DeviceIO/SetDigitalInputConfigurations.h"
#include "ONVIF/DigitalInputDeserializer.h"
#include "ONVIF/DigitalInputSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::SetDigitalInputConfigurations>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::SetDigitalInputConfigurations& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::SetDigitalInputConfigurations& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DigitalInputs"s};
		bool ret = false;
		std::vector<ONVIF::DigitalInput> gen_digitalInputs;
		ret = TypeDeserializer<std::vector<ONVIF::DigitalInput>>::deserialize(REMOTING__NAMES[0], true, deser, gen_digitalInputs);
		if (ret) value.setDigitalInputs(gen_digitalInputs);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_SetDigitalInputConfigurations_INCLUDED

