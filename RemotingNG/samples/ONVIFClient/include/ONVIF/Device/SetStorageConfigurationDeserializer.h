//
// SetStorageConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SetStorageConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_Device_SetStorageConfiguration_INCLUDED


#include "ONVIF/Device/SetStorageConfiguration.h"
#include "ONVIF/Device/StorageConfigurationDeserializer.h"
#include "ONVIF/Device/StorageConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SetStorageConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SetStorageConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SetStorageConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"StorageConfiguration"s};
		bool ret = false;
		ONVIF::Device::StorageConfiguration gen_storageConfiguration;
		ret = TypeDeserializer<ONVIF::Device::StorageConfiguration>::deserialize(REMOTING__NAMES[0], true, deser, gen_storageConfiguration);
		if (ret) value.setStorageConfiguration(gen_storageConfiguration);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SetStorageConfiguration_INCLUDED

