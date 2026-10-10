//
// ServiceSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_Service_INCLUDED
#define TypeSerializer_ONVIF_Device_Service_INCLUDED


#include "ONVIF/Device/CapabilitiesDeserializer.h"
#include "ONVIF/Device/CapabilitiesSerializer.h"
#include "ONVIF/Device/Service.h"
#include "ONVIF/OnvifVersionDeserializer.h"
#include "ONVIF/OnvifVersionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::Service>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::Service& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::Device::Service& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Namespace"s,"XAddr"s,"Capabilities"s,"Version"s,""s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getNamespace(), ser);
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[1], value.getXAddr(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Device::Capabilities>>::serialize(REMOTING__NAMES[2], value.getCapabilities(), ser);
		TypeSerializer<ONVIF::OnvifVersion>::serialize(REMOTING__NAMES[3], value.getVersion(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_Service_INCLUDED

