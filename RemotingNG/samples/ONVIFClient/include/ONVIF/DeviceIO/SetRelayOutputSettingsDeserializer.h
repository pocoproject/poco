//
// SetRelayOutputSettingsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_SetRelayOutputSettings_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_SetRelayOutputSettings_INCLUDED


#include "ONVIF/DeviceIO/SetRelayOutputSettings.h"
#include "ONVIF/RelayOutputDeserializer.h"
#include "ONVIF/RelayOutputSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::SetRelayOutputSettings>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::SetRelayOutputSettings& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::SetRelayOutputSettings& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RelayOutput"s};
		bool ret = false;
		ONVIF::RelayOutput gen_relayOutput;
		ret = TypeDeserializer<ONVIF::RelayOutput>::deserialize(REMOTING__NAMES[0], true, deser, gen_relayOutput);
		if (ret) value.setRelayOutput(gen_relayOutput);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_SetRelayOutputSettings_INCLUDED

