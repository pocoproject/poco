// 
// DeviceBinding.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_DeviceBinding_INCLUDED
#define ONVIF_Device_DeviceBinding_INCLUDED


#include "ONVIF/AttachmentData.h"
#include "ONVIF/BackupFile.h"
#include "ONVIF/BinaryData.h"
#include "ONVIF/Capabilities.h"
#include "ONVIF/Certificate.h"
#include "ONVIF/CertificateInformation.h"
#include "ONVIF/CertificateStatus.h"
#include "ONVIF/CertificateWithPrivateKey.h"
#include "ONVIF/DNSInformation.h"
#include "ONVIF/Device/DeviceServiceCapabilities.h"
#include "ONVIF/Device/Service.h"
#include "ONVIF/Device/StorageConfiguration.h"
#include "ONVIF/Device/StorageConfigurationData.h"
#include "ONVIF/Dot11AvailableNetworks.h"
#include "ONVIF/Dot11Capabilities.h"
#include "ONVIF/Dot11Status.h"
#include "ONVIF/Dot1XConfiguration.h"
#include "ONVIF/DynamicDNSInformation.h"
#include "ONVIF/HostnameInformation.h"
#include "ONVIF/IPAddress.h"
#include "ONVIF/IPAddressFilter.h"
#include "ONVIF/IntRange.h"
#include "ONVIF/LocationEntity.h"
#include "ONVIF/NTPInformation.h"
#include "ONVIF/NetworkGateway.h"
#include "ONVIF/NetworkHost.h"
#include "ONVIF/NetworkInterface.h"
#include "ONVIF/NetworkInterfaceSetConfiguration.h"
#include "ONVIF/NetworkProtocol.h"
#include "ONVIF/NetworkZeroConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/RelayOutput.h"
#include "ONVIF/RelayOutputSettings.h"
#include "ONVIF/Scope.h"
#include "ONVIF/SupportInformation.h"
#include "ONVIF/SystemDateTime.h"
#include "ONVIF/SystemLog.h"
#include "ONVIF/User.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
class BinaryData;
class DateTime;
} 
namespace ONVIF {
namespace Device {
class Extension;
} } 
namespace ONVIF {
class IntRange;
class RemoteUser;
class SystemLogUriList;
class TimeZone;
} 


namespace ONVIF {
namespace Device {


//@ name=DeviceBinding
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ remote
class ONVIF_API DeviceBinding
{
public:
	virtual ~DeviceBinding();

	//@ action="http://www.onvif.org/ver10/device/wsdl/AddIPAddressFilter"
	//@ name=AddIPAddressFilter
	//@ replyName=AddIPAddressFilterResponse
	//@ $iPAddressFilter={direction=in, name=IPAddressFilter}
	virtual void addIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/AddScopes"
	//@ name=AddScopes
	//@ replyName=AddScopesResponse
	//@ $scopeItem={direction=in, name=ScopeItem}
	virtual void addScopes(const std::vector<Poco::URI>& scopeItem) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/CreateCertificate"
	//@ name=CreateCertificate
	//@ replyName=CreateCertificateResponse
	//@ $certificateID={direction=in, name=CertificateID}
	//@ $subject={direction=in, name=Subject}
	//@ $validNotBefore={direction=in, name=ValidNotBefore, xsdType=dateTime}
	//@ $validNotAfter={direction=in, name=ValidNotAfter, xsdType=dateTime}
	//@ $nvtCertificate={direction=out, name=NvtCertificate}
	virtual void createCertificate(
		const Poco::Optional<std::string>& certificateID, 
		const Poco::Optional<std::string>& subject, 
		const Poco::Optional<Poco::DateTime>& validNotBefore, 
		const Poco::Optional<Poco::DateTime>& validNotAfter, 
		ONVIF::Certificate& nvtCertificate) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/CreateDot1XConfiguration"
	//@ name=CreateDot1XConfiguration
	//@ replyName=CreateDot1XConfigurationResponse
	//@ $dot1XConfiguration={direction=in, name=Dot1XConfiguration}
	virtual void createDot1XConfiguration(const ONVIF::Dot1XConfiguration& dot1XConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/CreateStorageConfiguration"
	//@ name=CreateStorageConfiguration
	//@ replyName=CreateStorageConfigurationResponse
	//@ $storageConfiguration={direction=in, name=StorageConfiguration}
	//@ $token={direction=out, name=Token}
	virtual void createStorageConfiguration(
		const StorageConfigurationData& storageConfiguration, 
		std::string& token) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/CreateUsers"
	//@ name=CreateUsers
	//@ replyName=CreateUsersResponse
	//@ $user={direction=in, name=User}
	virtual void createUsers(const std::vector<ONVIF::User>& user) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/DeleteCertificates"
	//@ name=DeleteCertificates
	//@ replyName=DeleteCertificatesResponse
	//@ $certificateID={direction=in, name=CertificateID}
	virtual void deleteCertificates(const std::vector<std::string>& certificateID) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/DeleteDot1XConfiguration"
	//@ name=DeleteDot1XConfiguration
	//@ replyName=DeleteDot1XConfigurationResponse
	//@ $dot1XConfigurationToken={direction=in, name=Dot1XConfigurationToken}
	virtual void deleteDot1XConfiguration(const std::vector<std::string>& dot1XConfigurationToken) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/DeleteGeoLocation"
	//@ name=DeleteGeoLocation
	//@ replyName=DeleteGeoLocationResponse
	//@ $location={direction=in, name=Location}
	virtual void deleteGeoLocation(const std::vector<ONVIF::LocationEntity>& location) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/DeleteStorageConfiguration"
	//@ name=DeleteStorageConfiguration
	//@ replyName=DeleteStorageConfigurationResponse
	//@ $token={direction=in, name=Token}
	virtual void deleteStorageConfiguration(const std::string& token) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/DeleteUsers"
	//@ name=DeleteUsers
	//@ replyName=DeleteUsersResponse
	//@ $username={direction=in, name=Username}
	virtual void deleteUsers(const std::vector<std::string>& username) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetAccessPolicy"
	//@ name=GetAccessPolicy
	//@ replyName=GetAccessPolicyResponse
	//@ $policyFile={direction=out, name=PolicyFile}
	virtual void getAccessPolicy(ONVIF::BinaryData& policyFile) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetAuthFailureWarningConfiguration"
	//@ name=GetAuthFailureWarningConfiguration
	//@ replyName=GetAuthFailureWarningConfigurationResponse
	//@ $enabled={direction=out, name=Enabled}
	//@ $monitorPeriod={direction=out, name=MonitorPeriod}
	//@ $maxAuthFailures={direction=out, name=MaxAuthFailures}
	virtual void getAuthFailureWarningConfiguration(
		bool& enabled, 
		int& monitorPeriod, 
		int& maxAuthFailures) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetAuthFailureWarningOptions"
	//@ name=GetAuthFailureWarningOptions
	//@ replyName=GetAuthFailureWarningOptionsResponse
	//@ $monitorPeriodRange={direction=out, name=MonitorPeriodRange}
	//@ $authFailureRange={direction=out, name=AuthFailureRange}
	virtual void getAuthFailureWarningOptions(
		ONVIF::IntRange& monitorPeriodRange, 
		ONVIF::IntRange& authFailureRange) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetCACertificates"
	//@ name=GetCACertificates
	//@ replyName=GetCACertificatesResponse
	//@ $cACertificate={direction=out, name=CACertificate}
	virtual void getCACertificates(std::vector<ONVIF::Certificate>& cACertificate) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetCapabilities"
	//@ name=GetCapabilities
	//@ replyName=GetCapabilitiesResponse
	//@ $category={direction=in, name=Category}
	//@ $capabilities={direction=out, name=Capabilities}
	virtual void getCapabilities(
		const std::vector<std::string>& category, 
		ONVIF::Capabilities& capabilities) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetCertificateInformation"
	//@ name=GetCertificateInformation
	//@ replyName=GetCertificateInformationResponse
	//@ $certificateID={direction=in, name=CertificateID}
	//@ $certificateInformation={direction=out, name=CertificateInformation}
	virtual void getCertificateInformation(
		const std::string& certificateID, 
		ONVIF::CertificateInformation& certificateInformation) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetCertificates"
	//@ name=GetCertificates
	//@ replyName=GetCertificatesResponse
	//@ $nvtCertificate={direction=out, name=NvtCertificate}
	virtual void getCertificates(std::vector<ONVIF::Certificate>& nvtCertificate) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetCertificatesStatus"
	//@ name=GetCertificatesStatus
	//@ replyName=GetCertificatesStatusResponse
	//@ $certificateStatus={direction=out, name=CertificateStatus}
	virtual void getCertificatesStatus(std::vector<ONVIF::CertificateStatus>& certificateStatus) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetClientCertificateMode"
	//@ name=GetClientCertificateMode
	//@ replyName=GetClientCertificateModeResponse
	//@ $enabled={direction=out, name=Enabled}
	virtual void getClientCertificateMode(bool& enabled) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetDNS"
	//@ name=GetDNS
	//@ replyName=GetDNSResponse
	//@ $dNSInformation={direction=out, name=DNSInformation}
	virtual void getDNS(ONVIF::DNSInformation& dNSInformation) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetDPAddresses"
	//@ name=GetDPAddresses
	//@ replyName=GetDPAddressesResponse
	//@ $dPAddress={direction=out, name=DPAddress}
	virtual void getDPAddresses(std::vector<ONVIF::NetworkHost>& dPAddress) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation"
	//@ name=GetDeviceInformation
	//@ replyName=GetDeviceInformationResponse
	//@ $manufacturer={direction=out, name=Manufacturer}
	//@ $model={direction=out, name=Model}
	//@ $firmwareVersion={direction=out, name=FirmwareVersion}
	//@ $serialNumber={direction=out, name=SerialNumber}
	//@ $hardwareId={direction=out, name=HardwareId}
	virtual void getDeviceInformation(
		std::string& manufacturer, 
		std::string& model, 
		std::string& firmwareVersion, 
		std::string& serialNumber, 
		std::string& hardwareId) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetDiscoveryMode"
	//@ name=GetDiscoveryMode
	//@ replyName=GetDiscoveryModeResponse
	//@ $discoveryMode={direction=out, name=DiscoveryMode}
	virtual void getDiscoveryMode(std::string& discoveryMode) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetDot11Capabilities"
	//@ name=GetDot11Capabilities
	//@ replyName=GetDot11CapabilitiesResponse
	//@ $capabilities={direction=out, name=Capabilities}
	virtual void getDot11Capabilities(ONVIF::Dot11Capabilities& capabilities) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetDot11Status"
	//@ name=GetDot11Status
	//@ replyName=GetDot11StatusResponse
	//@ $interfaceToken={direction=in, name=InterfaceToken}
	//@ $status={direction=out, name=Status}
	virtual void getDot11Status(
		const std::string& interfaceToken, 
		ONVIF::Dot11Status& status) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetDot1XConfiguration"
	//@ name=GetDot1XConfiguration
	//@ replyName=GetDot1XConfigurationResponse
	//@ $dot1XConfigurationToken={direction=in, name=Dot1XConfigurationToken}
	//@ $dot1XConfiguration={direction=out, name=Dot1XConfiguration}
	virtual void getDot1XConfiguration(
		const std::string& dot1XConfigurationToken, 
		ONVIF::Dot1XConfiguration& dot1XConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetDot1XConfigurations"
	//@ name=GetDot1XConfigurations
	//@ replyName=GetDot1XConfigurationsResponse
	//@ $dot1XConfiguration={direction=out, name=Dot1XConfiguration}
	virtual void getDot1XConfigurations(std::vector<ONVIF::Dot1XConfiguration>& dot1XConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetDynamicDNS"
	//@ name=GetDynamicDNS
	//@ replyName=GetDynamicDNSResponse
	//@ $dynamicDNSInformation={direction=out, name=DynamicDNSInformation}
	virtual void getDynamicDNS(ONVIF::DynamicDNSInformation& dynamicDNSInformation) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetEndpointReference"
	//@ name=GetEndpointReference
	//@ replyName=GetEndpointReferenceResponse
	//@ $gUID={direction=out, name=GUID}
	virtual void getEndpointReference(std::string& gUID) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetGeoLocation"
	//@ name=GetGeoLocation
	//@ replyName=GetGeoLocationResponse
	//@ $location={direction=out, name=Location}
	virtual void getGeoLocation(std::vector<ONVIF::LocationEntity>& location) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetHostname"
	//@ name=GetHostname
	//@ replyName=GetHostnameResponse
	//@ $hostnameInformation={direction=out, name=HostnameInformation}
	virtual void getHostname(ONVIF::HostnameInformation& hostnameInformation) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetIPAddressFilter"
	//@ name=GetIPAddressFilter
	//@ replyName=GetIPAddressFilterResponse
	//@ $iPAddressFilter={direction=out, name=IPAddressFilter}
	virtual void getIPAddressFilter(ONVIF::IPAddressFilter& iPAddressFilter) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetNTP"
	//@ name=GetNTP
	//@ replyName=GetNTPResponse
	//@ $nTPInformation={direction=out, name=NTPInformation}
	virtual void getNTP(ONVIF::NTPInformation& nTPInformation) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetNetworkDefaultGateway"
	//@ name=GetNetworkDefaultGateway
	//@ replyName=GetNetworkDefaultGatewayResponse
	//@ $networkGateway={direction=out, name=NetworkGateway}
	virtual void getNetworkDefaultGateway(ONVIF::NetworkGateway& networkGateway) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetNetworkInterfaces"
	//@ name=GetNetworkInterfaces
	//@ replyName=GetNetworkInterfacesResponse
	//@ $networkInterfaces={direction=out, name=NetworkInterfaces}
	virtual void getNetworkInterfaces(std::vector<ONVIF::NetworkInterface>& networkInterfaces) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetNetworkProtocols"
	//@ name=GetNetworkProtocols
	//@ replyName=GetNetworkProtocolsResponse
	//@ $networkProtocols={direction=out, name=NetworkProtocols}
	virtual void getNetworkProtocols(std::vector<ONVIF::NetworkProtocol>& networkProtocols) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetPasswordComplexityConfiguration"
	//@ name=GetPasswordComplexityConfiguration
	//@ replyName=GetPasswordComplexityConfigurationResponse
	//@ $minLen={direction=out, name=MinLen}
	//@ $uppercase={direction=out, name=Uppercase}
	//@ $number={direction=out, name=Number}
	//@ $specialChars={direction=out, name=SpecialChars}
	//@ $blockUsernameOccurrence={direction=out, name=BlockUsernameOccurrence}
	//@ $policyConfigurationLocked={direction=out, name=PolicyConfigurationLocked}
	virtual void getPasswordComplexityConfiguration(
		Poco::Optional<int>& minLen, 
		Poco::Optional<int>& uppercase, 
		Poco::Optional<int>& number, 
		Poco::Optional<int>& specialChars, 
		Poco::Optional<bool>& blockUsernameOccurrence, 
		Poco::Optional<bool>& policyConfigurationLocked) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetPasswordComplexityOptions"
	//@ name=GetPasswordComplexityOptions
	//@ replyName=GetPasswordComplexityOptionsResponse
	//@ $minLenRange={direction=out, name=MinLenRange}
	//@ $uppercaseRange={direction=out, name=UppercaseRange}
	//@ $numberRange={direction=out, name=NumberRange}
	//@ $specialCharsRange={direction=out, name=SpecialCharsRange}
	//@ $blockUsernameOccurrenceSupported={direction=out, name=BlockUsernameOccurrenceSupported}
	//@ $policyConfigurationLockSupported={direction=out, name=PolicyConfigurationLockSupported}
	virtual void getPasswordComplexityOptions(
		Poco::SharedPtr<ONVIF::IntRange>& minLenRange, 
		Poco::SharedPtr<ONVIF::IntRange>& uppercaseRange, 
		Poco::SharedPtr<ONVIF::IntRange>& numberRange, 
		Poco::SharedPtr<ONVIF::IntRange>& specialCharsRange, 
		Poco::Optional<bool>& blockUsernameOccurrenceSupported, 
		Poco::Optional<bool>& policyConfigurationLockSupported) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetPasswordHistoryConfiguration"
	//@ name=GetPasswordHistoryConfiguration
	//@ replyName=GetPasswordHistoryConfigurationResponse
	//@ $enabled={direction=out, name=Enabled}
	//@ $length={direction=out, name=Length}
	virtual void getPasswordHistoryConfiguration(
		bool& enabled, 
		int& length) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetPkcs10Request"
	//@ name=GetPkcs10Request
	//@ replyName=GetPkcs10RequestResponse
	//@ $certificateID={direction=in, name=CertificateID}
	//@ $subject={direction=in, name=Subject}
	//@ $attributes={direction=in, name=Attributes}
	//@ $pkcs10Request={direction=out, name=Pkcs10Request}
	virtual void getPkcs10Request(
		const std::string& certificateID, 
		const Poco::Optional<std::string>& subject, 
		const Poco::SharedPtr<ONVIF::BinaryData>& attributes, 
		ONVIF::BinaryData& pkcs10Request) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetRelayOutputs"
	//@ name=GetRelayOutputs
	//@ replyName=GetRelayOutputsResponse
	//@ $relayOutputs={direction=out, name=RelayOutputs}
	virtual void getRelayOutputs(std::vector<ONVIF::RelayOutput>& relayOutputs) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetRemoteDiscoveryMode"
	//@ name=GetRemoteDiscoveryMode
	//@ replyName=GetRemoteDiscoveryModeResponse
	//@ $remoteDiscoveryMode={direction=out, name=RemoteDiscoveryMode}
	virtual void getRemoteDiscoveryMode(std::string& remoteDiscoveryMode) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetRemoteUser"
	//@ name=GetRemoteUser
	//@ replyName=GetRemoteUserResponse
	//@ $remoteUser={direction=out, name=RemoteUser}
	virtual void getRemoteUser(Poco::SharedPtr<ONVIF::RemoteUser>& remoteUser) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetScopes"
	//@ name=GetScopes
	//@ replyName=GetScopesResponse
	//@ $scopes={direction=out, name=Scopes}
	virtual void getScopes(std::vector<ONVIF::Scope>& scopes) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetServiceCapabilities"
	//@ name=GetServiceCapabilities
	//@ replyName=GetServiceCapabilitiesResponse
	//@ $capabilities={direction=out, name=Capabilities}
	virtual void getServiceCapabilities(DeviceServiceCapabilities& capabilities) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetServices"
	//@ name=GetServices
	//@ replyName=GetServicesResponse
	//@ $includeCapability={direction=in, name=IncludeCapability}
	//@ $service={direction=out, name=Service}
	virtual void getServices(
		bool includeCapability, 
		std::vector<Service>& service) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetStorageConfiguration"
	//@ name=GetStorageConfiguration
	//@ replyName=GetStorageConfigurationResponse
	//@ $token={direction=in, name=Token}
	//@ $storageConfiguration={direction=out, name=StorageConfiguration}
	virtual void getStorageConfiguration(
		const std::string& token, 
		StorageConfiguration& storageConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetStorageConfigurations"
	//@ name=GetStorageConfigurations
	//@ replyName=GetStorageConfigurationsResponse
	//@ $storageConfigurations={direction=out, name=StorageConfigurations}
	virtual void getStorageConfigurations(std::vector<StorageConfiguration>& storageConfigurations) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetSystemBackup"
	//@ name=GetSystemBackup
	//@ replyName=GetSystemBackupResponse
	//@ $backupFiles={direction=out, name=BackupFiles}
	virtual void getSystemBackup(std::vector<ONVIF::BackupFile>& backupFiles) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetSystemDateAndTime"
	//@ name=GetSystemDateAndTime
	//@ replyName=GetSystemDateAndTimeResponse
	//@ $systemDateAndTime={direction=out, name=SystemDateAndTime}
	virtual void getSystemDateAndTime(ONVIF::SystemDateTime& systemDateAndTime) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetSystemLog"
	//@ name=GetSystemLog
	//@ replyName=GetSystemLogResponse
	//@ $logType={direction=in, name=LogType}
	//@ $systemLog={direction=out, name=SystemLog}
	virtual void getSystemLog(
		const ONVIF::SystemLog& logType, 
		ONVIF::SystemLog& systemLog) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetSystemSupportInformation"
	//@ name=GetSystemSupportInformation
	//@ replyName=GetSystemSupportInformationResponse
	//@ $supportInformation={direction=out, name=SupportInformation}
	virtual void getSystemSupportInformation(ONVIF::SupportInformation& supportInformation) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetSystemUris"
	//@ name=GetSystemUris
	//@ replyName=GetSystemUrisResponse
	//@ $systemLogUris={direction=out, name=SystemLogUris}
	//@ $supportInfoUri={direction=out, name=SupportInfoUri}
	//@ $systemBackupUri={direction=out, name=SystemBackupUri}
	//@ $extension={direction=out, name=Extension}
	virtual void getSystemUris(
		Poco::SharedPtr<ONVIF::SystemLogUriList>& systemLogUris, 
		Poco::Optional<Poco::URI>& supportInfoUri, 
		Poco::Optional<Poco::URI>& systemBackupUri, 
		Poco::SharedPtr<Extension>& extension) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetUsers"
	//@ name=GetUsers
	//@ replyName=GetUsersResponse
	//@ $user={direction=out, name=User}
	virtual void getUsers(std::vector<ONVIF::User>& user) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetWsdlUrl"
	//@ name=GetWsdlUrl
	//@ replyName=GetWsdlUrlResponse
	//@ $wsdlUrl={direction=out, name=WsdlUrl}
	virtual void getWsdlUrl(Poco::URI& wsdlUrl) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/GetZeroConfiguration"
	//@ name=GetZeroConfiguration
	//@ replyName=GetZeroConfigurationResponse
	//@ $zeroConfiguration={direction=out, name=ZeroConfiguration}
	virtual void getZeroConfiguration(ONVIF::NetworkZeroConfiguration& zeroConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/LoadCACertificates"
	//@ name=LoadCACertificates
	//@ replyName=LoadCACertificatesResponse
	//@ $cACertificate={direction=in, name=CACertificate}
	virtual void loadCACertificates(const std::vector<ONVIF::Certificate>& cACertificate) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/LoadCertificateWithPrivateKey"
	//@ name=LoadCertificateWithPrivateKey
	//@ replyName=LoadCertificateWithPrivateKeyResponse
	//@ $certificateWithPrivateKey={direction=in, name=CertificateWithPrivateKey}
	virtual void loadCertificateWithPrivateKey(const std::vector<ONVIF::CertificateWithPrivateKey>& certificateWithPrivateKey) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/LoadCertificates"
	//@ name=LoadCertificates
	//@ replyName=LoadCertificatesResponse
	//@ $nVTCertificate={direction=in, name=NVTCertificate}
	virtual void loadCertificates(const std::vector<ONVIF::Certificate>& nVTCertificate) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/RemoveIPAddressFilter"
	//@ name=RemoveIPAddressFilter
	//@ replyName=RemoveIPAddressFilterResponse
	//@ $iPAddressFilter={direction=in, name=IPAddressFilter}
	virtual void removeIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/RemoveScopes"
	//@ name=RemoveScopes
	//@ replyName=RemoveScopesResponse
	//@ $scopeItem={direction=in, name=ScopeItem}
	//@ $scopeItem2={direction=out, name=ScopeItem}
	virtual void removeScopes(
		const std::vector<Poco::URI>& scopeItem, 
		std::vector<Poco::URI>& scopeItem2) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/RestoreSystem"
	//@ name=RestoreSystem
	//@ replyName=RestoreSystemResponse
	//@ $backupFiles={direction=in, name=BackupFiles}
	virtual void restoreSystem(const std::vector<ONVIF::BackupFile>& backupFiles) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/ScanAvailableDot11Networks"
	//@ name=ScanAvailableDot11Networks
	//@ replyName=ScanAvailableDot11NetworksResponse
	//@ $interfaceToken={direction=in, name=InterfaceToken}
	//@ $networks={direction=out, name=Networks}
	virtual void scanAvailableDot11Networks(
		const std::string& interfaceToken, 
		std::vector<ONVIF::Dot11AvailableNetworks>& networks) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SendAuxiliaryCommand"
	//@ name=SendAuxiliaryCommand
	//@ replyName=SendAuxiliaryCommandResponse
	//@ $auxiliaryCommand={direction=in, name=AuxiliaryCommand}
	//@ $auxiliaryCommandResponse={direction=out, name=AuxiliaryCommandResponse}
	virtual void sendAuxiliaryCommand(
		const std::string& auxiliaryCommand, 
		Poco::Optional<std::string>& auxiliaryCommandResponse) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetAccessPolicy"
	//@ name=SetAccessPolicy
	//@ replyName=SetAccessPolicyResponse
	//@ $policyFile={direction=in, name=PolicyFile}
	virtual void setAccessPolicy(const ONVIF::BinaryData& policyFile) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetAuthFailureWarningConfiguration"
	//@ name=SetAuthFailureWarningConfiguration
	//@ replyName=SetAuthFailureWarningConfigurationResponse
	//@ $enabled={direction=in, name=Enabled}
	//@ $monitorPeriod={direction=in, name=MonitorPeriod}
	//@ $maxAuthFailures={direction=in, name=MaxAuthFailures}
	virtual void setAuthFailureWarningConfiguration(
		bool enabled, 
		int monitorPeriod, 
		int maxAuthFailures) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetCertificatesStatus"
	//@ name=SetCertificatesStatus
	//@ replyName=SetCertificatesStatusResponse
	//@ $certificateStatus={direction=in, name=CertificateStatus}
	virtual void setCertificatesStatus(const std::vector<ONVIF::CertificateStatus>& certificateStatus) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetClientCertificateMode"
	//@ name=SetClientCertificateMode
	//@ replyName=SetClientCertificateModeResponse
	//@ $enabled={direction=in, name=Enabled}
	virtual void setClientCertificateMode(bool enabled) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetDNS"
	//@ name=SetDNS
	//@ replyName=SetDNSResponse
	//@ $fromDHCP={direction=in, name=FromDHCP}
	//@ $searchDomain={direction=in, name=SearchDomain}
	//@ $dNSManual={direction=in, name=DNSManual}
	virtual void setDNS(
		bool fromDHCP, 
		const std::vector<std::string>& searchDomain, 
		const std::vector<ONVIF::IPAddress>& dNSManual) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetDPAddresses"
	//@ name=SetDPAddresses
	//@ replyName=SetDPAddressesResponse
	//@ $dPAddress={direction=in, name=DPAddress}
	virtual void setDPAddresses(const std::vector<ONVIF::NetworkHost>& dPAddress) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetDiscoveryMode"
	//@ name=SetDiscoveryMode
	//@ replyName=SetDiscoveryModeResponse
	//@ $discoveryMode={direction=in, name=DiscoveryMode}
	virtual void setDiscoveryMode(const std::string& discoveryMode) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetDot1XConfiguration"
	//@ name=SetDot1XConfiguration
	//@ replyName=SetDot1XConfigurationResponse
	//@ $dot1XConfiguration={direction=in, name=Dot1XConfiguration}
	virtual void setDot1XConfiguration(const ONVIF::Dot1XConfiguration& dot1XConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetDynamicDNS"
	//@ name=SetDynamicDNS
	//@ replyName=SetDynamicDNSResponse
	//@ $type={direction=in, name=Type}
	//@ $name={direction=in, name=Name}
	//@ $tTL={direction=in, name=TTL}
	virtual void setDynamicDNS(
		const std::string& type, 
		const Poco::Optional<std::string>& name, 
		const Poco::Optional<std::string>& tTL) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetGeoLocation"
	//@ name=SetGeoLocation
	//@ replyName=SetGeoLocationResponse
	//@ $location={direction=in, name=Location}
	virtual void setGeoLocation(const std::vector<ONVIF::LocationEntity>& location) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetHashingAlgorithm"
	//@ name=SetHashingAlgorithm
	//@ replyName=SetHashingAlgorithmResponse
	//@ $algorithm={direction=in, name=Algorithm}
	virtual void setHashingAlgorithm(const std::vector<std::string>& algorithm) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetHostname"
	//@ name=SetHostname
	//@ replyName=SetHostnameResponse
	//@ $name={direction=in, name=Name}
	virtual void setHostname(const std::string& name) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetHostnameFromDHCP"
	//@ name=SetHostnameFromDHCP
	//@ replyName=SetHostnameFromDHCPResponse
	//@ $fromDHCP={direction=in, name=FromDHCP}
	//@ $rebootNeeded={direction=out, name=RebootNeeded}
	virtual void setHostnameFromDHCP(
		bool fromDHCP, 
		bool& rebootNeeded) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetIPAddressFilter"
	//@ name=SetIPAddressFilter
	//@ replyName=SetIPAddressFilterResponse
	//@ $iPAddressFilter={direction=in, name=IPAddressFilter}
	virtual void setIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetNTP"
	//@ name=SetNTP
	//@ replyName=SetNTPResponse
	//@ $fromDHCP={direction=in, name=FromDHCP}
	//@ $nTPManual={direction=in, name=NTPManual}
	virtual void setNTP(
		bool fromDHCP, 
		const std::vector<ONVIF::NetworkHost>& nTPManual) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetNetworkDefaultGateway"
	//@ name=SetNetworkDefaultGateway
	//@ replyName=SetNetworkDefaultGatewayResponse
	//@ $iPv4Address={direction=in, name=IPv4Address}
	//@ $iPv6Address={direction=in, name=IPv6Address}
	virtual void setNetworkDefaultGateway(
		const std::vector<std::string>& iPv4Address, 
		const std::vector<std::string>& iPv6Address) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetNetworkInterfaces"
	//@ name=SetNetworkInterfaces
	//@ replyName=SetNetworkInterfacesResponse
	//@ $interfaceToken={direction=in, name=InterfaceToken}
	//@ $networkInterface={direction=in, name=NetworkInterface}
	//@ $rebootNeeded={direction=out, name=RebootNeeded}
	virtual void setNetworkInterfaces(
		const std::string& interfaceToken, 
		const ONVIF::NetworkInterfaceSetConfiguration& networkInterface, 
		bool& rebootNeeded) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetNetworkProtocols"
	//@ name=SetNetworkProtocols
	//@ replyName=SetNetworkProtocolsResponse
	//@ $networkProtocols={direction=in, name=NetworkProtocols}
	virtual void setNetworkProtocols(const std::vector<ONVIF::NetworkProtocol>& networkProtocols) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetPasswordComplexityConfiguration"
	//@ name=SetPasswordComplexityConfiguration
	//@ replyName=SetPasswordComplexityConfigurationResponse
	//@ $minLen={direction=in, name=MinLen}
	//@ $uppercase={direction=in, name=Uppercase}
	//@ $number={direction=in, name=Number}
	//@ $specialChars={direction=in, name=SpecialChars}
	//@ $blockUsernameOccurrence={direction=in, name=BlockUsernameOccurrence}
	//@ $policyConfigurationLocked={direction=in, name=PolicyConfigurationLocked}
	virtual void setPasswordComplexityConfiguration(
		Poco::Optional<int> minLen, 
		Poco::Optional<int> uppercase, 
		Poco::Optional<int> number, 
		Poco::Optional<int> specialChars, 
		Poco::Optional<bool> blockUsernameOccurrence, 
		Poco::Optional<bool> policyConfigurationLocked) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetPasswordHistoryConfiguration"
	//@ name=SetPasswordHistoryConfiguration
	//@ replyName=SetPasswordHistoryConfigurationResponse
	//@ $enabled={direction=in, name=Enabled}
	//@ $length={direction=in, name=Length}
	virtual void setPasswordHistoryConfiguration(
		bool enabled, 
		int length) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetRelayOutputSettings"
	//@ name=SetRelayOutputSettings
	//@ replyName=SetRelayOutputSettingsResponse
	//@ $relayOutputToken={direction=in, name=RelayOutputToken}
	//@ $properties={direction=in, name=Properties}
	virtual void setRelayOutputSettings(
		const std::string& relayOutputToken, 
		const ONVIF::RelayOutputSettings& properties) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetRelayOutputState"
	//@ name=SetRelayOutputState
	//@ replyName=SetRelayOutputStateResponse
	//@ $relayOutputToken={direction=in, name=RelayOutputToken}
	//@ $logicalState={direction=in, name=LogicalState}
	virtual void setRelayOutputState(
		const std::string& relayOutputToken, 
		const std::string& logicalState) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetRemoteDiscoveryMode"
	//@ name=SetRemoteDiscoveryMode
	//@ replyName=SetRemoteDiscoveryModeResponse
	//@ $remoteDiscoveryMode={direction=in, name=RemoteDiscoveryMode}
	virtual void setRemoteDiscoveryMode(const std::string& remoteDiscoveryMode) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetRemoteUser"
	//@ name=SetRemoteUser
	//@ replyName=SetRemoteUserResponse
	//@ $remoteUser={direction=in, name=RemoteUser}
	virtual void setRemoteUser(const Poco::SharedPtr<ONVIF::RemoteUser>& remoteUser) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetScopes"
	//@ name=SetScopes
	//@ replyName=SetScopesResponse
	//@ $scopes={direction=in, name=Scopes}
	virtual void setScopes(const std::vector<Poco::URI>& scopes) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetStorageConfiguration"
	//@ name=SetStorageConfiguration
	//@ replyName=SetStorageConfigurationResponse
	//@ $storageConfiguration={direction=in, name=StorageConfiguration}
	virtual void setStorageConfiguration(const StorageConfiguration& storageConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetSystemDateAndTime"
	//@ name=SetSystemDateAndTime
	//@ replyName=SetSystemDateAndTimeResponse
	//@ $dateTimeType={direction=in, name=DateTimeType}
	//@ $daylightSavings={direction=in, name=DaylightSavings}
	//@ $timeZone={direction=in, name=TimeZone}
	//@ $uTCDateTime={direction=in, name=UTCDateTime}
	virtual void setSystemDateAndTime(
		const std::string& dateTimeType, 
		bool daylightSavings, 
		const Poco::SharedPtr<ONVIF::TimeZone>& timeZone, 
		const Poco::SharedPtr<ONVIF::DateTime>& uTCDateTime) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetSystemFactoryDefault"
	//@ name=SetSystemFactoryDefault
	//@ replyName=SetSystemFactoryDefaultResponse
	//@ $factoryDefault={direction=in, name=FactoryDefault}
	virtual void setSystemFactoryDefault(const std::string& factoryDefault) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetUser"
	//@ name=SetUser
	//@ replyName=SetUserResponse
	//@ $user={direction=in, name=User}
	virtual void setUser(const std::vector<ONVIF::User>& user) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SetZeroConfiguration"
	//@ name=SetZeroConfiguration
	//@ replyName=SetZeroConfigurationResponse
	//@ $interfaceToken={direction=in, name=InterfaceToken}
	//@ $enabled={direction=in, name=Enabled}
	virtual void setZeroConfiguration(
		const std::string& interfaceToken, 
		bool enabled) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/StartFirmwareUpgrade"
	//@ name=StartFirmwareUpgrade
	//@ replyName=StartFirmwareUpgradeResponse
	//@ $uploadUri={direction=out, name=UploadUri}
	//@ $uploadDelay={direction=out, name=UploadDelay}
	//@ $expectedDownTime={direction=out, name=ExpectedDownTime}
	virtual void startFirmwareUpgrade(
		Poco::URI& uploadUri, 
		std::string& uploadDelay, 
		std::string& expectedDownTime) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/StartSystemRestore"
	//@ name=StartSystemRestore
	//@ replyName=StartSystemRestoreResponse
	//@ $uploadUri={direction=out, name=UploadUri}
	//@ $expectedDownTime={direction=out, name=ExpectedDownTime}
	virtual void startSystemRestore(
		Poco::URI& uploadUri, 
		std::string& expectedDownTime) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/SystemReboot"
	//@ name=SystemReboot
	//@ replyName=SystemRebootResponse
	//@ $message={direction=out, name=Message}
	virtual void systemReboot(std::string& message) = 0;

	//@ action="http://www.onvif.org/ver10/device/wsdl/UpgradeSystemFirmware"
	//@ name=UpgradeSystemFirmware
	//@ replyName=UpgradeSystemFirmwareResponse
	//@ $firmware={direction=in, name=Firmware}
	//@ $message={direction=out, name=Message}
	virtual void upgradeSystemFirmware(
		const ONVIF::AttachmentData& firmware, 
		Poco::Optional<std::string>& message) = 0;
};


} } // ONVIF::Device


#endif // ONVIF_Device_DeviceBinding_INCLUDED
