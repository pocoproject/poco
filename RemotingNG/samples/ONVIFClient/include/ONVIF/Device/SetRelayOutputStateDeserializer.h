//
// SetRelayOutputStateDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SetRelayOutputState_INCLUDED
#define TypeDeserializer_ONVIF_Device_SetRelayOutputState_INCLUDED


#include "ONVIF/Device/SetRelayOutputState.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SetRelayOutputState>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SetRelayOutputState& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SetRelayOutputState& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RelayOutputToken"s,"LogicalState"s};
		bool ret = false;
		std::string gen_relayOutputToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_relayOutputToken);
		if (ret) value.setRelayOutputToken(gen_relayOutputToken);
		std::string gen_logicalState;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_logicalState);
		if (ret) value.setLogicalState(gen_logicalState);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SetRelayOutputState_INCLUDED

