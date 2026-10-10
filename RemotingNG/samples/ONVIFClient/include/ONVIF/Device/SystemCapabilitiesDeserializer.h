//
// SystemCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SystemCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_Device_SystemCapabilities_INCLUDED


#include "ONVIF/Device/SystemCapabilities.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SystemCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SystemCapabilities& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::Device::SystemCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DiscoveryResolve"s,"DiscoveryBye"s,"RemoteDiscovery"s,"SystemBackup"s,"SystemLogging"s,"FirmwareUpgrade"s,"HttpFirmwareUpgrade"s,"HttpSystemBackup"s,"HttpSystemLogging"s,"HttpSupportInformation"s,"StorageConfiguration"s,"MaxStorageConfigurations"s,"GeoLocationEntries"s,"AutoGeo"s,"StorageTypesSupported"s,"DiscoveryNotSupported"s,"NetworkConfigNotSupported"s,"UserConfigNotSupported"s,"Addons"s};
		bool ret = false;
		Poco::Optional<bool> gen_discoveryResolve;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_discoveryResolve);
		if (ret) value.setDiscoveryResolve(gen_discoveryResolve);
		Poco::Optional<bool> gen_discoveryBye;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_discoveryBye);
		if (ret) value.setDiscoveryBye(gen_discoveryBye);
		Poco::Optional<bool> gen_remoteDiscovery;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_remoteDiscovery);
		if (ret) value.setRemoteDiscovery(gen_remoteDiscovery);
		Poco::Optional<bool> gen_systemBackup;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[3], false, deser, gen_systemBackup);
		if (ret) value.setSystemBackup(gen_systemBackup);
		Poco::Optional<bool> gen_systemLogging;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[4], false, deser, gen_systemLogging);
		if (ret) value.setSystemLogging(gen_systemLogging);
		Poco::Optional<bool> gen_firmwareUpgrade;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[5], false, deser, gen_firmwareUpgrade);
		if (ret) value.setFirmwareUpgrade(gen_firmwareUpgrade);
		Poco::Optional<bool> gen_httpFirmwareUpgrade;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[6], false, deser, gen_httpFirmwareUpgrade);
		if (ret) value.setHttpFirmwareUpgrade(gen_httpFirmwareUpgrade);
		Poco::Optional<bool> gen_httpSystemBackup;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[7], false, deser, gen_httpSystemBackup);
		if (ret) value.setHttpSystemBackup(gen_httpSystemBackup);
		Poco::Optional<bool> gen_httpSystemLogging;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[8], false, deser, gen_httpSystemLogging);
		if (ret) value.setHttpSystemLogging(gen_httpSystemLogging);
		Poco::Optional<bool> gen_httpSupportInformation;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[9], false, deser, gen_httpSupportInformation);
		if (ret) value.setHttpSupportInformation(gen_httpSupportInformation);
		Poco::Optional<bool> gen_storageConfiguration;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[10], false, deser, gen_storageConfiguration);
		if (ret) value.setStorageConfiguration(gen_storageConfiguration);
		Poco::Optional<int> gen_maxStorageConfigurations;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[11], false, deser, gen_maxStorageConfigurations);
		if (ret) value.setMaxStorageConfigurations(gen_maxStorageConfigurations);
		Poco::Optional<int> gen_geoLocationEntries;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[12], false, deser, gen_geoLocationEntries);
		if (ret) value.setGeoLocationEntries(gen_geoLocationEntries);
		std::vector<std::vector<std::string>> gen_autoGeo;
		ret = TypeDeserializer<std::vector<std::vector<std::string>>>::deserialize(REMOTING__NAMES[13], false, deser, gen_autoGeo);
		if (ret) value.setAutoGeo(gen_autoGeo);
		std::vector<std::vector<std::string>> gen_storageTypesSupported;
		ret = TypeDeserializer<std::vector<std::vector<std::string>>>::deserialize(REMOTING__NAMES[14], false, deser, gen_storageTypesSupported);
		if (ret) value.setStorageTypesSupported(gen_storageTypesSupported);
		Poco::Optional<bool> gen_discoveryNotSupported;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[15], false, deser, gen_discoveryNotSupported);
		if (ret) value.setDiscoveryNotSupported(gen_discoveryNotSupported);
		Poco::Optional<bool> gen_networkConfigNotSupported;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[16], false, deser, gen_networkConfigNotSupported);
		if (ret) value.setNetworkConfigNotSupported(gen_networkConfigNotSupported);
		Poco::Optional<bool> gen_userConfigNotSupported;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[17], false, deser, gen_userConfigNotSupported);
		if (ret) value.setUserConfigNotSupported(gen_userConfigNotSupported);
		std::vector<std::vector<std::string>> gen_addons;
		ret = TypeDeserializer<std::vector<std::vector<std::string>>>::deserialize(REMOTING__NAMES[18], false, deser, gen_addons);
		if (ret) value.setAddons(gen_addons);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SystemCapabilities& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DiscoveryResolve"s,"DiscoveryBye"s,"RemoteDiscovery"s,"SystemBackup"s,"SystemLogging"s,"FirmwareUpgrade"s,"HttpFirmwareUpgrade"s,"HttpSystemBackup"s,"HttpSystemLogging"s,"HttpSupportInformation"s,"StorageConfiguration"s,"MaxStorageConfigurations"s,"GeoLocationEntries"s,"AutoGeo"s,"StorageTypesSupported"s,"DiscoveryNotSupported"s,"NetworkConfigNotSupported"s,"UserConfigNotSupported"s,"Addons"s,"http://www.onvif.org/ver10/device/wsdl"s};
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[2], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[3], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[4], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[5], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[6], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[7], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[8], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[9], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[10], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[11], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[12], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[13], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[14], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[15], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[16], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[17], false);
		deser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[18], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SystemCapabilities_INCLUDED

