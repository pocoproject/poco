//
// DeviceCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_DeviceCapabilities_INCLUDED


#include "ONVIF/DeviceCapabilities.h"
#include "ONVIF/DeviceCapabilitiesExtensionDeserializer.h"
#include "ONVIF/DeviceCapabilitiesExtensionSerializer.h"
#include "ONVIF/IOCapabilitiesDeserializer.h"
#include "ONVIF/IOCapabilitiesSerializer.h"
#include "ONVIF/NetworkCapabilitiesDeserializer.h"
#include "ONVIF/NetworkCapabilitiesSerializer.h"
#include "ONVIF/SecurityCapabilitiesDeserializer.h"
#include "ONVIF/SecurityCapabilitiesSerializer.h"
#include "ONVIF/SystemCapabilitiesDeserializer.h"
#include "ONVIF/SystemCapabilitiesSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"Network"s,"System"s,"IO"s,"Security"s,"Extension"s};
		bool ret = false;
		Poco::URI gen_xAddr;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_xAddr);
		if (ret) value.setXAddr(gen_xAddr);
		Poco::SharedPtr<ONVIF::NetworkCapabilities> gen_network;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkCapabilities>>::deserialize(REMOTING__NAMES[1], false, deser, gen_network);
		if (ret) value.setNetwork(gen_network);
		Poco::SharedPtr<ONVIF::SystemCapabilities> gen_system;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SystemCapabilities>>::deserialize(REMOTING__NAMES[2], false, deser, gen_system);
		if (ret) value.setSystem(gen_system);
		Poco::SharedPtr<ONVIF::IOCapabilities> gen_iO;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IOCapabilities>>::deserialize(REMOTING__NAMES[3], false, deser, gen_iO);
		if (ret) value.setIO(gen_iO);
		Poco::SharedPtr<ONVIF::SecurityCapabilities> gen_security;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SecurityCapabilities>>::deserialize(REMOTING__NAMES[4], false, deser, gen_security);
		if (ret) value.setSecurity(gen_security);
		Poco::SharedPtr<ONVIF::DeviceCapabilitiesExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DeviceCapabilitiesExtension>>::deserialize(REMOTING__NAMES[5], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceCapabilities_INCLUDED

