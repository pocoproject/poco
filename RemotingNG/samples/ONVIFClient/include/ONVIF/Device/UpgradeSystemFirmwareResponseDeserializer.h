//
// UpgradeSystemFirmwareResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_UpgradeSystemFirmwareResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_UpgradeSystemFirmwareResponse_INCLUDED


#include "ONVIF/Device/UpgradeSystemFirmwareResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::UpgradeSystemFirmwareResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::UpgradeSystemFirmwareResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::UpgradeSystemFirmwareResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Message"s};
		bool ret = false;
		Poco::Optional<std::string> gen_message;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_message);
		if (ret) value.setMessage(gen_message);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_UpgradeSystemFirmwareResponse_INCLUDED

