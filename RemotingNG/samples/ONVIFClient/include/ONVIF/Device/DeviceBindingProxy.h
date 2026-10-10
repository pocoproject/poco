//
// DeviceBindingProxy.h
//
// Package: Generated
// Module:  DeviceBindingProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef DeviceBindingProxy_INCLUDED
#define DeviceBindingProxy_INCLUDED


#include "ONVIF/Device/IDeviceBinding.h"
#include "Poco/RemotingNG/Proxy.h"


namespace ONVIF {
namespace Device {


class DeviceBindingProxy: public ONVIF::Device::IDeviceBinding, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<DeviceBindingProxy>;

	DeviceBindingProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a DeviceBindingProxy.

	virtual ~DeviceBindingProxy();
		/// Destroys the DeviceBindingProxy.

	virtual void addIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter);

	virtual void addScopes(const std::vector<Poco::URI>& scopeItem);

	virtual void createCertificate(const Poco::Optional<std::string>& certificateID, const Poco::Optional<std::string>& subject, const Poco::Optional<Poco::DateTime>& validNotBefore, const Poco::Optional<Poco::DateTime>& validNotAfter, ONVIF::Certificate& nvtCertificate);

	virtual void createDot1XConfiguration(const ONVIF::Dot1XConfiguration& dot1XConfiguration);

	virtual void createStorageConfiguration(const ONVIF::Device::StorageConfigurationData& storageConfiguration, std::string& token);

	virtual void createUsers(const std::vector<ONVIF::User>& user);

	virtual void deleteCertificates(const std::vector<std::string>& certificateID);

	virtual void deleteDot1XConfiguration(const std::vector<std::string>& dot1XConfigurationToken);

	virtual void deleteGeoLocation(const std::vector<ONVIF::LocationEntity>& location);

	virtual void deleteStorageConfiguration(const std::string& token);

	virtual void deleteUsers(const std::vector<std::string>& username);

	virtual void getAccessPolicy(ONVIF::BinaryData& policyFile);

	virtual void getAuthFailureWarningConfiguration(bool& enabled, int& monitorPeriod, int& maxAuthFailures);

	virtual void getAuthFailureWarningOptions(ONVIF::IntRange& monitorPeriodRange, ONVIF::IntRange& authFailureRange);

	virtual void getCACertificates(std::vector<ONVIF::Certificate>& cACertificate);

	virtual void getCapabilities(const std::vector<std::string>& category, ONVIF::Capabilities& capabilities);

	virtual void getCertificateInformation(const std::string& certificateID, ONVIF::CertificateInformation& certificateInformation);

	virtual void getCertificates(std::vector<ONVIF::Certificate>& nvtCertificate);

	virtual void getCertificatesStatus(std::vector<ONVIF::CertificateStatus>& certificateStatus);

	virtual void getClientCertificateMode(bool& enabled);

	virtual void getDNS(ONVIF::DNSInformation& dNSInformation);

	virtual void getDPAddresses(std::vector<ONVIF::NetworkHost>& dPAddress);

	virtual void getDeviceInformation(std::string& manufacturer, std::string& model, std::string& firmwareVersion, std::string& serialNumber, std::string& hardwareId);

	virtual void getDiscoveryMode(std::string& discoveryMode);

	virtual void getDot11Capabilities(ONVIF::Dot11Capabilities& capabilities);

	virtual void getDot11Status(const std::string& interfaceToken, ONVIF::Dot11Status& status);

	virtual void getDot1XConfiguration(const std::string& dot1XConfigurationToken, ONVIF::Dot1XConfiguration& dot1XConfiguration);

	virtual void getDot1XConfigurations(std::vector<ONVIF::Dot1XConfiguration>& dot1XConfiguration);

	virtual void getDynamicDNS(ONVIF::DynamicDNSInformation& dynamicDNSInformation);

	virtual void getEndpointReference(std::string& gUID);

	virtual void getGeoLocation(std::vector<ONVIF::LocationEntity>& location);

	virtual void getHostname(ONVIF::HostnameInformation& hostnameInformation);

	virtual void getIPAddressFilter(ONVIF::IPAddressFilter& iPAddressFilter);

	virtual void getNTP(ONVIF::NTPInformation& nTPInformation);

	virtual void getNetworkDefaultGateway(ONVIF::NetworkGateway& networkGateway);

	virtual void getNetworkInterfaces(std::vector<ONVIF::NetworkInterface>& networkInterfaces);

	virtual void getNetworkProtocols(std::vector<ONVIF::NetworkProtocol>& networkProtocols);

	virtual void getPasswordComplexityConfiguration(Poco::Optional<int>& minLen, Poco::Optional<int>& uppercase, Poco::Optional<int>& number, Poco::Optional<int>& specialChars, Poco::Optional<bool>& blockUsernameOccurrence, Poco::Optional<bool>& policyConfigurationLocked);

	virtual void getPasswordComplexityOptions(Poco::SharedPtr<ONVIF::IntRange>& minLenRange, Poco::SharedPtr<ONVIF::IntRange>& uppercaseRange, Poco::SharedPtr<ONVIF::IntRange>& numberRange, Poco::SharedPtr<ONVIF::IntRange>& specialCharsRange, Poco::Optional<bool>& blockUsernameOccurrenceSupported, Poco::Optional<bool>& policyConfigurationLockSupported);

	virtual void getPasswordHistoryConfiguration(bool& enabled, int& length);

	virtual void getPkcs10Request(const std::string& certificateID, const Poco::Optional<std::string>& subject, const Poco::SharedPtr<ONVIF::BinaryData>& attributes, ONVIF::BinaryData& pkcs10Request);

	virtual void getRelayOutputs(std::vector<ONVIF::RelayOutput>& relayOutputs);

	virtual void getRemoteDiscoveryMode(std::string& remoteDiscoveryMode);

	virtual void getRemoteUser(Poco::SharedPtr<ONVIF::RemoteUser>& remoteUser);

	virtual void getScopes(std::vector<ONVIF::Scope>& scopes);

	virtual void getServiceCapabilities(ONVIF::Device::DeviceServiceCapabilities& capabilities);

	virtual void getServices(bool includeCapability, std::vector<ONVIF::Device::Service>& service);

	virtual void getStorageConfiguration(const std::string& token, ONVIF::Device::StorageConfiguration& storageConfiguration);

	virtual void getStorageConfigurations(std::vector<ONVIF::Device::StorageConfiguration>& storageConfigurations);

	virtual void getSystemBackup(std::vector<ONVIF::BackupFile>& backupFiles);

	virtual void getSystemDateAndTime(ONVIF::SystemDateTime& systemDateAndTime);

	virtual void getSystemLog(const ONVIF::SystemLog& logType, ONVIF::SystemLog& systemLog);

	virtual void getSystemSupportInformation(ONVIF::SupportInformation& supportInformation);

	virtual void getSystemUris(Poco::SharedPtr<ONVIF::SystemLogUriList>& systemLogUris, Poco::Optional<Poco::URI>& supportInfoUri, Poco::Optional<Poco::URI>& systemBackupUri, Poco::SharedPtr<ONVIF::Device::Extension>& extension);

	virtual void getUsers(std::vector<ONVIF::User>& user);

	virtual void getWsdlUrl(Poco::URI& wsdlUrl);

	virtual void getZeroConfiguration(ONVIF::NetworkZeroConfiguration& zeroConfiguration);

	virtual void loadCACertificates(const std::vector<ONVIF::Certificate>& cACertificate);

	virtual void loadCertificateWithPrivateKey(const std::vector<ONVIF::CertificateWithPrivateKey>& certificateWithPrivateKey);

	virtual void loadCertificates(const std::vector<ONVIF::Certificate>& nVTCertificate);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	virtual void removeIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter);

	virtual void removeScopes(const std::vector<Poco::URI>& scopeItem, std::vector<Poco::URI>& scopeItem2);

	virtual void restoreSystem(const std::vector<ONVIF::BackupFile>& backupFiles);

	virtual void scanAvailableDot11Networks(const std::string& interfaceToken, std::vector<ONVIF::Dot11AvailableNetworks>& networks);

	virtual void sendAuxiliaryCommand(const std::string& auxiliaryCommand, Poco::Optional<std::string>& auxiliaryCommandResponse);

	virtual void setAccessPolicy(const ONVIF::BinaryData& policyFile);

	virtual void setAuthFailureWarningConfiguration(bool enabled, int monitorPeriod, int maxAuthFailures);

	virtual void setCertificatesStatus(const std::vector<ONVIF::CertificateStatus>& certificateStatus);

	virtual void setClientCertificateMode(bool enabled);

	virtual void setDNS(bool fromDHCP, const std::vector<std::string>& searchDomain, const std::vector<ONVIF::IPAddress>& dNSManual);

	virtual void setDPAddresses(const std::vector<ONVIF::NetworkHost>& dPAddress);

	virtual void setDiscoveryMode(const std::string& discoveryMode);

	virtual void setDot1XConfiguration(const ONVIF::Dot1XConfiguration& dot1XConfiguration);

	virtual void setDynamicDNS(const std::string& type, const Poco::Optional<std::string>& name, const Poco::Optional<std::string>& tTL);

	virtual void setGeoLocation(const std::vector<ONVIF::LocationEntity>& location);

	virtual void setHashingAlgorithm(const std::vector<std::string>& algorithm);

	virtual void setHostname(const std::string& name);

	virtual void setHostnameFromDHCP(bool fromDHCP, bool& rebootNeeded);

	virtual void setIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter);

	virtual void setNTP(bool fromDHCP, const std::vector<ONVIF::NetworkHost>& nTPManual);

	virtual void setNetworkDefaultGateway(const std::vector<std::string>& iPv4Address, const std::vector<std::string>& iPv6Address);

	virtual void setNetworkInterfaces(const std::string& interfaceToken, const ONVIF::NetworkInterfaceSetConfiguration& networkInterface, bool& rebootNeeded);

	virtual void setNetworkProtocols(const std::vector<ONVIF::NetworkProtocol>& networkProtocols);

	virtual void setPasswordComplexityConfiguration(Poco::Optional<int> minLen, Poco::Optional<int> uppercase, Poco::Optional<int> number, Poco::Optional<int> specialChars, Poco::Optional<bool> blockUsernameOccurrence, Poco::Optional<bool> policyConfigurationLocked);

	virtual void setPasswordHistoryConfiguration(bool enabled, int length);

	virtual void setRelayOutputSettings(const std::string& relayOutputToken, const ONVIF::RelayOutputSettings& properties);

	virtual void setRelayOutputState(const std::string& relayOutputToken, const std::string& logicalState);

	virtual void setRemoteDiscoveryMode(const std::string& remoteDiscoveryMode);

	virtual void setRemoteUser(const Poco::SharedPtr<ONVIF::RemoteUser>& remoteUser);

	virtual void setScopes(const std::vector<Poco::URI>& scopes);

	virtual void setStorageConfiguration(const ONVIF::Device::StorageConfiguration& storageConfiguration);

	virtual void setSystemDateAndTime(const std::string& dateTimeType, bool daylightSavings, const Poco::SharedPtr<ONVIF::TimeZone>& timeZone, const Poco::SharedPtr<ONVIF::DateTime>& uTCDateTime);

	virtual void setSystemFactoryDefault(const std::string& factoryDefault);

	virtual void setUser(const std::vector<ONVIF::User>& user);

	virtual void setZeroConfiguration(const std::string& interfaceToken, bool enabled);

	virtual void startFirmwareUpgrade(Poco::URI& uploadUri, std::string& uploadDelay, std::string& expectedDownTime);

	virtual void startSystemRestore(Poco::URI& uploadUri, std::string& expectedDownTime);

	virtual void systemReboot(std::string& message);

	virtual void upgradeSystemFirmware(const ONVIF::AttachmentData& firmware, Poco::Optional<std::string>& message);

private:
	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& DeviceBindingProxy::remoting__typeId() const
{
	return IDeviceBinding::remoting__typeId();
}


} // namespace Device
} // namespace ONVIF


#endif // DeviceBindingProxy_INCLUDED

