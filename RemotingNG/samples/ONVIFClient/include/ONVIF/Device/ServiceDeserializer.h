//
// ServiceDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_Service_INCLUDED
#define TypeDeserializer_ONVIF_Device_Service_INCLUDED


#include "ONVIF/Device/CapabilitiesDeserializer.h"
#include "ONVIF/Device/CapabilitiesSerializer.h"
#include "ONVIF/Device/Service.h"
#include "ONVIF/OnvifVersionDeserializer.h"
#include "ONVIF/OnvifVersionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::Service>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::Service& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::Service& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Namespace"s,"XAddr"s,"Capabilities"s,"Version"s};
		bool ret = false;
		Poco::URI gen_namespace;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_namespace);
		if (ret) value.setNamespace(gen_namespace);
		Poco::URI gen_xAddr;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[1], true, deser, gen_xAddr);
		if (ret) value.setXAddr(gen_xAddr);
		Poco::SharedPtr<ONVIF::Device::Capabilities> gen_capabilities;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Device::Capabilities>>::deserialize(REMOTING__NAMES[2], false, deser, gen_capabilities);
		if (ret) value.setCapabilities(gen_capabilities);
		ONVIF::OnvifVersion gen_version;
		ret = TypeDeserializer<ONVIF::OnvifVersion>::deserialize(REMOTING__NAMES[3], true, deser, gen_version);
		if (ret) value.setVersion(gen_version);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_Service_INCLUDED

