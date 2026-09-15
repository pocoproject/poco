//
// SystemCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_SystemCapabilities_INCLUDED
#define TypeSerializer_ONVIF_Device_SystemCapabilities_INCLUDED


#include "ONVIF/Device/SystemCapabilities.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::SystemCapabilities>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DiscoveryResolve"s,"DiscoveryBye"s,"RemoteDiscovery"s,"SystemBackup"s,"SystemLogging"s,"FirmwareUpgrade"s,"HttpFirmwareUpgrade"s,"HttpSystemBackup"s,"HttpSystemLogging"s,"HttpSupportInformation"s,"StorageConfiguration"s,"MaxStorageConfigurations"s,"GeoLocationEntries"s,"AutoGeo"s,"StorageTypesSupported"s,"DiscoveryNotSupported"s,"NetworkConfigNotSupported"s,"UserConfigNotSupported"s,"Addons"s,"http://www.onvif.org/ver10/device/wsdl"s};
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[3]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[4]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[5]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[6]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[7]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[8]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[9]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[10]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[11]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[12]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[13]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[14]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[15]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[16]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[17]);
		ser.pushAttribute(REMOTING__NAMES[19], REMOTING__NAMES[18]);
	}

	static void serialize(const std::string& name, const ONVIF::Device::SystemCapabilities& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::Device::SystemCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DiscoveryResolve"s,"DiscoveryBye"s,"RemoteDiscovery"s,"SystemBackup"s,"SystemLogging"s,"FirmwareUpgrade"s,"HttpFirmwareUpgrade"s,"HttpSystemBackup"s,"HttpSystemLogging"s,"HttpSupportInformation"s,"StorageConfiguration"s,"MaxStorageConfigurations"s,"GeoLocationEntries"s,"AutoGeo"s,"StorageTypesSupported"s,"DiscoveryNotSupported"s,"NetworkConfigNotSupported"s,"UserConfigNotSupported"s,"Addons"s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getDiscoveryResolve(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getDiscoveryBye(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getRemoteDiscovery(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[3], value.getSystemBackup(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[4], value.getSystemLogging(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[5], value.getFirmwareUpgrade(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[6], value.getHttpFirmwareUpgrade(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[7], value.getHttpSystemBackup(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[8], value.getHttpSystemLogging(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[9], value.getHttpSupportInformation(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[10], value.getStorageConfiguration(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[11], value.getMaxStorageConfigurations(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[12], value.getGeoLocationEntries(), ser);
		TypeSerializer<std::vector<std::vector<std::string>>>::serialize(REMOTING__NAMES[13], value.getAutoGeo(), ser);
		TypeSerializer<std::vector<std::vector<std::string>>>::serialize(REMOTING__NAMES[14], value.getStorageTypesSupported(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[15], value.getDiscoveryNotSupported(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[16], value.getNetworkConfigNotSupported(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[17], value.getUserConfigNotSupported(), ser);
		TypeSerializer<std::vector<std::vector<std::string>>>::serialize(REMOTING__NAMES[18], value.getAddons(), ser);
	}

	static void serializeImpl(const ONVIF::Device::SystemCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_SystemCapabilities_INCLUDED

