//
// GetRelayOutputOptionsResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_GetRelayOutputOptionsResponse_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_GetRelayOutputOptionsResponse_INCLUDED


#include "ONVIF/DeviceIO/GetRelayOutputOptionsResponse.h"
#include "ONVIF/DeviceIO/RelayOutputOptionsDeserializer.h"
#include "ONVIF/DeviceIO/RelayOutputOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::GetRelayOutputOptionsResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::GetRelayOutputOptionsResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::GetRelayOutputOptionsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RelayOutputOptions"s};
		bool ret = false;
		std::vector<ONVIF::DeviceIO::RelayOutputOptions> gen_relayOutputOptions;
		ret = TypeDeserializer<std::vector<ONVIF::DeviceIO::RelayOutputOptions>>::deserialize(REMOTING__NAMES[0], false, deser, gen_relayOutputOptions);
		if (ret) value.setRelayOutputOptions(gen_relayOutputOptions);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_GetRelayOutputOptionsResponse_INCLUDED

