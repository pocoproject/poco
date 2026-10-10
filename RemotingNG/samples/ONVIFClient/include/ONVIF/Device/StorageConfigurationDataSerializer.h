//
// StorageConfigurationDataSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_StorageConfigurationData_INCLUDED
#define TypeSerializer_ONVIF_Device_StorageConfigurationData_INCLUDED


#include "ONVIF/Device/Extension_2Deserializer.h"
#include "ONVIF/Device/Extension_2Serializer.h"
#include "ONVIF/Device/StorageConfigurationData.h"
#include "ONVIF/Device/UserCredentialDeserializer.h"
#include "ONVIF/Device/UserCredentialSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::StorageConfigurationData>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"type"s,"Region"s,"http://www.onvif.org/ver10/device/wsdl"s};
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1]);
	}

	static void serialize(const std::string& name, const ONVIF::Device::StorageConfigurationData& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::Device::StorageConfigurationData& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"type"s,"Region"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getType(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getRegion(), ser);
	}

	static void serializeImpl(const ONVIF::Device::StorageConfigurationData& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"LocalPath"s,"StorageUri"s,"User"s,"Extension"s,""s};
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[0], value.getLocalPath(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[1], value.getStorageUri(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Device::UserCredential>>::serialize(REMOTING__NAMES[2], value.getUser(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Device::Extension_2>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_StorageConfigurationData_INCLUDED

