//
// IDeviceBinding.h
//
// Package: Generated
// Module:  IDeviceBinding
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef IDeviceBinding_INCLUDED
#define IDeviceBinding_INCLUDED


#include "ONVIF/Device/DeviceBinding.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"


namespace ONVIF {
namespace Device {


class IDeviceBinding: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<IDeviceBinding>;

	IDeviceBinding();
		/// Creates a IDeviceBinding.

	virtual ~IDeviceBinding();
		/// Destroys the IDeviceBinding.

	virtual void addIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter) = 0;

	virtual void addScopes(const std::vector<Poco::URI>& scopeItem) = 0;

	virtual void createCertificate(const Poco::Optional<std::string>& certificateID, const Poco::Optional<std::string>& subject, const Poco::Optional<Poco::DateTime>& validNotBefore, const Poco::Optional<Poco::DateTime>& validNotAfter, ONVIF::Certificate& nvtCertificate) = 0;

	virtual void createDot1XConfiguration(const ONVIF::Dot1XConfiguration& dot1XConfiguration) = 0;

	virtual void createStorageConfiguration(const ONVIF::Device::StorageConfigurationData& storageConfiguration, std::string& token) = 0;

	virtual void createUsers(const std::vector<ONVIF::User>& user) = 0;

	virtual void deleteCertificates(const std::vector<std::string>& certificateID) = 0;

	virtual void deleteDot1XConfiguration(const std::vector<std::string>& dot1XConfigurationToken) = 0;

	virtual void deleteGeoLocation(const std::vector<ONVIF::LocationEntity>& location) = 0;

	virtual void deleteStorageConfiguration(const std::string& token) = 0;

	virtual void deleteUsers(const std::vector<std::string>& username) = 0;

	virtual void getAccessPolicy(ONVIF::BinaryData& policyFile) = 0;

	virtual void getAuthFailureWarningConfiguration(bool& enabled, int& monitorPeriod, int& maxAuthFailures) = 0;

	virtual void getAuthFailureWarningOptions(ONVIF::IntRange& monitorPeriodRange, ONVIF::IntRange& authFailureRange) = 0;

	virtual void getCACertificates(std::vector<ONVIF::Certificate>& cACertificate) = 0;

	virtual void getCapabilities(const std::vector<std::string>& category, ONVIF::Capabilities& capabilities) = 0;

	virtual void getCertificateInformation(const std::string& certificateID, ONVIF::CertificateInformation& certificateInformation) = 0;

	virtual void getCertificates(std::vector<ONVIF::Certificate>& nvtCertificate) = 0;

	virtual void getCertificatesStatus(std::vector<ONVIF::CertificateStatus>& certificateStatus) = 0;

	virtual void getClientCertificateMode(bool& enabled) = 0;

	virtual void getDNS(ONVIF::DNSInformation& dNSInformation) = 0;

	virtual void getDPAddresses(std::vector<ONVIF::NetworkHost>& dPAddress) = 0;

	virtual void getDeviceInformation(std::string& manufacturer, std::string& model, std::string& firmwareVersion, std::string& serialNumber, std::string& hardwareId) = 0;

	virtual void getDiscoveryMode(std::string& discoveryMode) = 0;

	virtual void getDot11Capabilities(ONVIF::Dot11Capabilities& capabilities) = 0;

	virtual void getDot11Status(const std::string& interfaceToken, ONVIF::Dot11Status& status) = 0;

	virtual void getDot1XConfiguration(const std::string& dot1XConfigurationToken, ONVIF::Dot1XConfiguration& dot1XConfiguration) = 0;

	virtual void getDot1XConfigurations(std::vector<ONVIF::Dot1XConfiguration>& dot1XConfiguration) = 0;

	virtual void getDynamicDNS(ONVIF::DynamicDNSInformation& dynamicDNSInformation) = 0;

	virtual void getEndpointReference(std::string& gUID) = 0;

	virtual void getGeoLocation(std::vector<ONVIF::LocationEntity>& location) = 0;

	virtual void getHostname(ONVIF::HostnameInformation& hostnameInformation) = 0;

	virtual void getIPAddressFilter(ONVIF::IPAddressFilter& iPAddressFilter) = 0;

	virtual void getNTP(ONVIF::NTPInformation& nTPInformation) = 0;

	virtual void getNetworkDefaultGateway(ONVIF::NetworkGateway& networkGateway) = 0;

	virtual void getNetworkInterfaces(std::vector<ONVIF::NetworkInterface>& networkInterfaces) = 0;

	virtual void getNetworkProtocols(std::vector<ONVIF::NetworkProtocol>& networkProtocols) = 0;

	virtual void getPasswordComplexityConfiguration(Poco::Optional<int>& minLen, Poco::Optional<int>& uppercase, Poco::Optional<int>& number, Poco::Optional<int>& specialChars, Poco::Optional<bool>& blockUsernameOccurrence, Poco::Optional<bool>& policyConfigurationLocked) = 0;

	virtual void getPasswordComplexityOptions(Poco::SharedPtr<ONVIF::IntRange>& minLenRange, Poco::SharedPtr<ONVIF::IntRange>& uppercaseRange, Poco::SharedPtr<ONVIF::IntRange>& numberRange, Poco::SharedPtr<ONVIF::IntRange>& specialCharsRange, Poco::Optional<bool>& blockUsernameOccurrenceSupported, Poco::Optional<bool>& policyConfigurationLockSupported) = 0;

	virtual void getPasswordHistoryConfiguration(bool& enabled, int& length) = 0;

	virtual void getPkcs10Request(const std::string& certificateID, const Poco::Optional<std::string>& subject, const Poco::SharedPtr<ONVIF::BinaryData>& attributes, ONVIF::BinaryData& pkcs10Request) = 0;

	virtual void getRelayOutputs(std::vector<ONVIF::RelayOutput>& relayOutputs) = 0;

	virtual void getRemoteDiscoveryMode(std::string& remoteDiscoveryMode) = 0;

	virtual void getRemoteUser(Poco::SharedPtr<ONVIF::RemoteUser>& remoteUser) = 0;

	virtual void getScopes(std::vector<ONVIF::Scope>& scopes) = 0;

	virtual void getServiceCapabilities(ONVIF::Device::DeviceServiceCapabilities& capabilities) = 0;

	virtual void getServices(bool includeCapability, std::vector<ONVIF::Device::Service>& service) = 0;

	virtual void getStorageConfiguration(const std::string& token, ONVIF::Device::StorageConfiguration& storageConfiguration) = 0;

	virtual void getStorageConfigurations(std::vector<ONVIF::Device::StorageConfiguration>& storageConfigurations) = 0;

	virtual void getSystemBackup(std::vector<ONVIF::BackupFile>& backupFiles) = 0;

	virtual void getSystemDateAndTime(ONVIF::SystemDateTime& systemDateAndTime) = 0;

	virtual void getSystemLog(const ONVIF::SystemLog& logType, ONVIF::SystemLog& systemLog) = 0;

	virtual void getSystemSupportInformation(ONVIF::SupportInformation& supportInformation) = 0;

	virtual void getSystemUris(Poco::SharedPtr<ONVIF::SystemLogUriList>& systemLogUris, Poco::Optional<Poco::URI>& supportInfoUri, Poco::Optional<Poco::URI>& systemBackupUri, Poco::SharedPtr<ONVIF::Device::Extension>& extension) = 0;

	virtual void getUsers(std::vector<ONVIF::User>& user) = 0;

	virtual void getWsdlUrl(Poco::URI& wsdlUrl) = 0;

	virtual void getZeroConfiguration(ONVIF::NetworkZeroConfiguration& zeroConfiguration) = 0;

	virtual void loadCACertificates(const std::vector<ONVIF::Certificate>& cACertificate) = 0;

	virtual void loadCertificateWithPrivateKey(const std::vector<ONVIF::CertificateWithPrivateKey>& certificateWithPrivateKey) = 0;

	virtual void loadCertificates(const std::vector<ONVIF::Certificate>& nVTCertificate) = 0;

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

	virtual void removeIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter) = 0;

	virtual void removeScopes(const std::vector<Poco::URI>& scopeItem, std::vector<Poco::URI>& scopeItem2) = 0;

	virtual void restoreSystem(const std::vector<ONVIF::BackupFile>& backupFiles) = 0;

	virtual void scanAvailableDot11Networks(const std::string& interfaceToken, std::vector<ONVIF::Dot11AvailableNetworks>& networks) = 0;

	virtual void sendAuxiliaryCommand(const std::string& auxiliaryCommand, Poco::Optional<std::string>& auxiliaryCommandResponse) = 0;

	virtual void setAccessPolicy(const ONVIF::BinaryData& policyFile) = 0;

	virtual void setAuthFailureWarningConfiguration(bool enabled, int monitorPeriod, int maxAuthFailures) = 0;

	virtual void setCertificatesStatus(const std::vector<ONVIF::CertificateStatus>& certificateStatus) = 0;

	virtual void setClientCertificateMode(bool enabled) = 0;

	virtual void setDNS(bool fromDHCP, const std::vector<std::string>& searchDomain, const std::vector<ONVIF::IPAddress>& dNSManual) = 0;

	virtual void setDPAddresses(const std::vector<ONVIF::NetworkHost>& dPAddress) = 0;

	virtual void setDiscoveryMode(const std::string& discoveryMode) = 0;

	virtual void setDot1XConfiguration(const ONVIF::Dot1XConfiguration& dot1XConfiguration) = 0;

	virtual void setDynamicDNS(const std::string& type, const Poco::Optional<std::string>& name, const Poco::Optional<std::string>& tTL) = 0;

	virtual void setGeoLocation(const std::vector<ONVIF::LocationEntity>& location) = 0;

	virtual void setHashingAlgorithm(const std::vector<std::string>& algorithm) = 0;

	virtual void setHostname(const std::string& name) = 0;

	virtual void setHostnameFromDHCP(bool fromDHCP, bool& rebootNeeded) = 0;

	virtual void setIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter) = 0;

	virtual void setNTP(bool fromDHCP, const std::vector<ONVIF::NetworkHost>& nTPManual) = 0;

	virtual void setNetworkDefaultGateway(const std::vector<std::string>& iPv4Address, const std::vector<std::string>& iPv6Address) = 0;

	virtual void setNetworkInterfaces(const std::string& interfaceToken, const ONVIF::NetworkInterfaceSetConfiguration& networkInterface, bool& rebootNeeded) = 0;

	virtual void setNetworkProtocols(const std::vector<ONVIF::NetworkProtocol>& networkProtocols) = 0;

	virtual void setPasswordComplexityConfiguration(Poco::Optional<int> minLen, Poco::Optional<int> uppercase, Poco::Optional<int> number, Poco::Optional<int> specialChars, Poco::Optional<bool> blockUsernameOccurrence, Poco::Optional<bool> policyConfigurationLocked) = 0;

	virtual void setPasswordHistoryConfiguration(bool enabled, int length) = 0;

	virtual void setRelayOutputSettings(const std::string& relayOutputToken, const ONVIF::RelayOutputSettings& properties) = 0;

	virtual void setRelayOutputState(const std::string& relayOutputToken, const std::string& logicalState) = 0;

	virtual void setRemoteDiscoveryMode(const std::string& remoteDiscoveryMode) = 0;

	virtual void setRemoteUser(const Poco::SharedPtr<ONVIF::RemoteUser>& remoteUser) = 0;

	virtual void setScopes(const std::vector<Poco::URI>& scopes) = 0;

	virtual void setStorageConfiguration(const ONVIF::Device::StorageConfiguration& storageConfiguration) = 0;

	virtual void setSystemDateAndTime(const std::string& dateTimeType, bool daylightSavings, const Poco::SharedPtr<ONVIF::TimeZone>& timeZone, const Poco::SharedPtr<ONVIF::DateTime>& uTCDateTime) = 0;

	virtual void setSystemFactoryDefault(const std::string& factoryDefault) = 0;

	virtual void setUser(const std::vector<ONVIF::User>& user) = 0;

	virtual void setZeroConfiguration(const std::string& interfaceToken, bool enabled) = 0;

	virtual void startFirmwareUpgrade(Poco::URI& uploadUri, std::string& uploadDelay, std::string& expectedDownTime) = 0;

	virtual void startSystemRestore(Poco::URI& uploadUri, std::string& expectedDownTime) = 0;

	virtual void systemReboot(std::string& message) = 0;

	virtual void upgradeSystemFirmware(const ONVIF::AttachmentData& firmware, Poco::Optional<std::string>& message) = 0;

};


} // namespace Device
} // namespace ONVIF


#endif // IDeviceBinding_INCLUDED

