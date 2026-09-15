//
// StorageConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_StorageConfiguration_INCLUDED
#define TypeSerializer_ONVIF_Device_StorageConfiguration_INCLUDED


#include "ONVIF/Device/StorageConfiguration.h"
#include "ONVIF/Device/StorageConfigurationDataDeserializer.h"
#include "ONVIF/Device/StorageConfigurationDataSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::StorageConfiguration>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES__ONVIF__DEVICEENTITY[1], REMOTING__NAMES__ONVIF__DEVICEENTITY[0]);
		}
		
	}

	static void serialize(const std::string& name, const ONVIF::Device::StorageConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::Device::StorageConfiguration& value, Serializer& ser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__DEVICEENTITY[0], value.getToken(), ser);
		}
		
	}

	static void serializeImpl(const ONVIF::Device::StorageConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::DeviceEntity
		{
		}
		
		static const std::string REMOTING__NAMES[] = {"Data"s,""s};
		TypeSerializer<ONVIF::Device::StorageConfigurationData>::serialize(REMOTING__NAMES[0], value.getData(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_StorageConfiguration_INCLUDED

