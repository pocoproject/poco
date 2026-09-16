//
// StartFirmwareUpgradeResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_StartFirmwareUpgradeResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_StartFirmwareUpgradeResponse_INCLUDED


#include "ONVIF/Device/StartFirmwareUpgradeResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::StartFirmwareUpgradeResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::StartFirmwareUpgradeResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::StartFirmwareUpgradeResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"UploadUri"s,"UploadDelay"s,"ExpectedDownTime"s};
		bool ret = false;
		Poco::URI gen_uploadUri;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_uploadUri);
		if (ret) value.setUploadUri(gen_uploadUri);
		std::string gen_uploadDelay;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_uploadDelay);
		if (ret) value.setUploadDelay(gen_uploadDelay);
		std::string gen_expectedDownTime;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[2], true, deser, gen_expectedDownTime);
		if (ret) value.setExpectedDownTime(gen_expectedDownTime);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_StartFirmwareUpgradeResponse_INCLUDED

