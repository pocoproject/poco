//
// StorageConfigurationDataDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_StorageConfigurationData_INCLUDED
#define TypeDeserializer_ONVIF_Device_StorageConfigurationData_INCLUDED


#include "ONVIF/Device/Extension_2Deserializer.h"
#include "ONVIF/Device/Extension_2Serializer.h"
#include "ONVIF/Device/StorageConfigurationData.h"
#include "ONVIF/Device/UserCredentialDeserializer.h"
#include "ONVIF/Device/UserCredentialSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::StorageConfigurationData>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::StorageConfigurationData& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::Device::StorageConfigurationData& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"type"s,"Region"s};
		bool ret = false;
		std::string gen_type;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_type);
		if (ret) value.setType(gen_type);
		Poco::Optional<std::string> gen_region;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_region);
		if (ret) value.setRegion(gen_region);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::StorageConfigurationData& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"LocalPath"s,"StorageUri"s,"User"s,"Extension"s};
		bool ret = false;
		Poco::Optional<Poco::URI> gen_localPath;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[0], false, deser, gen_localPath);
		if (ret) value.setLocalPath(gen_localPath);
		Poco::Optional<Poco::URI> gen_storageUri;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[1], false, deser, gen_storageUri);
		if (ret) value.setStorageUri(gen_storageUri);
		Poco::SharedPtr<ONVIF::Device::UserCredential> gen_user;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Device::UserCredential>>::deserialize(REMOTING__NAMES[2], false, deser, gen_user);
		if (ret) value.setUser(gen_user);
		Poco::SharedPtr<ONVIF::Device::Extension_2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Device::Extension_2>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"type"s,"Region"s,"http://www.onvif.org/ver10/device/wsdl"s};
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0], true);
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_StorageConfigurationData_INCLUDED

