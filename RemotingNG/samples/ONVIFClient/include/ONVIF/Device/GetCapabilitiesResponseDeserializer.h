//
// GetCapabilitiesResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetCapabilitiesResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetCapabilitiesResponse_INCLUDED


#include "ONVIF/CapabilitiesDeserializer.h"
#include "ONVIF/CapabilitiesSerializer.h"
#include "ONVIF/Device/GetCapabilitiesResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetCapabilitiesResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetCapabilitiesResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetCapabilitiesResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Capabilities"s};
		bool ret = false;
		ONVIF::Capabilities gen_capabilities;
		ret = TypeDeserializer<ONVIF::Capabilities>::deserialize(REMOTING__NAMES[0], true, deser, gen_capabilities);
		if (ret) value.setCapabilities(gen_capabilities);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetCapabilitiesResponse_INCLUDED

