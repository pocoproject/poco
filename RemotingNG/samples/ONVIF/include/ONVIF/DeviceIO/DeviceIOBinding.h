// 
// DeviceIOBinding.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_DeviceIOBinding_INCLUDED
#define ONVIF_DeviceIO_DeviceIOBinding_INCLUDED


#include "ONVIF/AudioOutputConfiguration.h"
#include "ONVIF/AudioOutputConfigurationOptions.h"
#include "ONVIF/AudioSourceConfiguration.h"
#include "ONVIF/AudioSourceConfigurationOptions.h"
#include "ONVIF/DeviceIO/Capabilities.h"
#include "ONVIF/DeviceIO/DigitalInputConfigurationOptions.h"
#include "ONVIF/DeviceIO/RelayOutputOptions.h"
#include "ONVIF/DeviceIO/SerialPort.h"
#include "ONVIF/DeviceIO/SerialPortConfiguration.h"
#include "ONVIF/DeviceIO/SerialPortConfigurationOptions.h"
#include "ONVIF/DigitalInput.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/RelayOutput.h"
#include "ONVIF/VideoOutput.h"
#include "ONVIF/VideoOutputConfiguration.h"
#include "ONVIF/VideoOutputConfigurationOptions.h"
#include "ONVIF/VideoSourceConfiguration.h"
#include "ONVIF/VideoSourceConfigurationOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {
class SerialData;
} } 


namespace ONVIF {
namespace DeviceIO {


//@ name=DeviceIOBinding
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ remote
class ONVIF_API DeviceIOBinding
{
public:
	virtual ~DeviceIOBinding();

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetAudioOutputConfiguration"
	//@ name=GetAudioOutputConfiguration
	//@ replyName=GetAudioOutputConfigurationResponse
	//@ $audioOutputToken={direction=in, name=AudioOutputToken}
	//@ $audioOutputConfiguration={direction=out, name=AudioOutputConfiguration}
	virtual void getAudioOutputConfiguration(
		const std::string& audioOutputToken, 
		ONVIF::AudioOutputConfiguration& audioOutputConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetAudioOutputConfigurationOptions"
	//@ name=GetAudioOutputConfigurationOptions
	//@ replyName=GetAudioOutputConfigurationOptionsResponse
	//@ $audioOutputToken={direction=in, name=AudioOutputToken}
	//@ $audioOutputOptions={direction=out, name=AudioOutputOptions}
	virtual void getAudioOutputConfigurationOptions(
		const std::string& audioOutputToken, 
		ONVIF::AudioOutputConfigurationOptions& audioOutputOptions) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetAudioOutputs"
	//@ name=GetAudioOutputs
	//@ replyName=GetAudioOutputsResponse
	//@ $token={direction=out, name=Token}
	virtual void getAudioOutputs(std::vector<std::string>& token) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetAudioSourceConfiguration"
	//@ name=GetAudioSourceConfiguration
	//@ replyName=GetAudioSourceConfigurationResponse
	//@ $audioSourceToken={direction=in, name=AudioSourceToken}
	//@ $audioSourceConfiguration={direction=out, name=AudioSourceConfiguration}
	virtual void getAudioSourceConfiguration(
		const std::string& audioSourceToken, 
		ONVIF::AudioSourceConfiguration& audioSourceConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetAudioSourceConfigurationOptions"
	//@ name=GetAudioSourceConfigurationOptions
	//@ replyName=GetAudioSourceConfigurationOptionsResponse
	//@ $audioSourceToken={direction=in, name=AudioSourceToken}
	//@ $audioSourceOptions={direction=out, name=AudioSourceOptions}
	virtual void getAudioSourceConfigurationOptions(
		const std::string& audioSourceToken, 
		ONVIF::AudioSourceConfigurationOptions& audioSourceOptions) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetAudioSources"
	//@ name=GetAudioSources
	//@ replyName=GetAudioSourcesResponse
	//@ $token={direction=out, name=Token}
	virtual void getAudioSources(std::vector<std::string>& token) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetDigitalInputConfigurationOptions"
	//@ name=GetDigitalInputConfigurationOptions
	//@ replyName=GetDigitalInputConfigurationOptionsResponse
	//@ $token={direction=in, name=Token}
	//@ $digitalInputOptions={direction=out, name=DigitalInputOptions}
	virtual void getDigitalInputConfigurationOptions(
		const Poco::Optional<std::string>& token, 
		DigitalInputConfigurationOptions& digitalInputOptions) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetDigitalInputs"
	//@ name=GetDigitalInputs
	//@ replyName=GetDigitalInputsResponse
	//@ $digitalInputs={direction=out, name=DigitalInputs}
	virtual void getDigitalInputs(std::vector<ONVIF::DigitalInput>& digitalInputs) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetRelayOutputOptions"
	//@ name=GetRelayOutputOptions
	//@ replyName=GetRelayOutputOptionsResponse
	//@ $relayOutputToken={direction=in, name=RelayOutputToken}
	//@ $relayOutputOptions={direction=out, name=RelayOutputOptions}
	virtual void getRelayOutputOptions(
		const Poco::Optional<std::string>& relayOutputToken, 
		std::vector<RelayOutputOptions>& relayOutputOptions) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetRelayOutputs"
	//@ name=GetRelayOutputs
	//@ namespace="http://www.onvif.org/ver10/device/wsdl"
	//@ replyName=GetRelayOutputsResponse
	//@ $relayOutputs={direction=out, name=RelayOutputs}
	virtual void getRelayOutputs(std::vector<ONVIF::RelayOutput>& relayOutputs) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetSerialPortConfigurations"
	//@ name=GetSerialPortConfiguration
	//@ replyName=GetSerialPortConfigurationResponse
	//@ $serialPortToken={direction=in, name=SerialPortToken}
	//@ $serialPortConfiguration={direction=out, name=SerialPortConfiguration}
	virtual void getSerialPortConfiguration(
		const std::string& serialPortToken, 
		SerialPortConfiguration& serialPortConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetSerialPortConfigurationOptions"
	//@ name=GetSerialPortConfigurationOptions
	//@ replyName=GetSerialPortConfigurationOptionsResponse
	//@ $serialPortToken={direction=in, name=SerialPortToken}
	//@ $serialPortOptions={direction=out, name=SerialPortOptions}
	virtual void getSerialPortConfigurationOptions(
		const std::string& serialPortToken, 
		SerialPortConfigurationOptions& serialPortOptions) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetSerialPorts"
	//@ name=GetSerialPorts
	//@ replyName=GetSerialPortsResponse
	//@ $serialPort={direction=out, name=SerialPort}
	virtual void getSerialPorts(std::vector<SerialPort>& serialPort) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetServiceCapabilities"
	//@ name=GetServiceCapabilities
	//@ replyName=GetServiceCapabilitiesResponse
	//@ $capabilities={direction=out, name=Capabilities}
	virtual void getServiceCapabilities(Capabilities& capabilities) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetVideoOutputConfiguration"
	//@ name=GetVideoOutputConfiguration
	//@ replyName=GetVideoOutputConfigurationResponse
	//@ $videoOutputToken={direction=in, name=VideoOutputToken}
	//@ $videoOutputConfiguration={direction=out, name=VideoOutputConfiguration}
	virtual void getVideoOutputConfiguration(
		const std::string& videoOutputToken, 
		ONVIF::VideoOutputConfiguration& videoOutputConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetVideoOutputConfigurationOptions"
	//@ name=GetVideoOutputConfigurationOptions
	//@ replyName=GetVideoOutputConfigurationOptionsResponse
	//@ $videoOutputToken={direction=in, name=VideoOutputToken}
	//@ $videoOutputConfigurationOptions={direction=out, name=VideoOutputConfigurationOptions}
	virtual void getVideoOutputConfigurationOptions(
		const std::string& videoOutputToken, 
		ONVIF::VideoOutputConfigurationOptions& videoOutputConfigurationOptions) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetVideoOutputs"
	//@ name=GetVideoOutputs
	//@ replyName=GetVideoOutputsResponse
	//@ $videoOutputs={direction=out, name=VideoOutputs}
	virtual void getVideoOutputs(std::vector<ONVIF::VideoOutput>& videoOutputs) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetVideoSourceConfiguration"
	//@ name=GetVideoSourceConfiguration
	//@ replyName=GetVideoSourceConfigurationResponse
	//@ $videoSourceToken={direction=in, name=VideoSourceToken}
	//@ $videoSourceConfiguration={direction=out, name=VideoSourceConfiguration}
	virtual void getVideoSourceConfiguration(
		const std::string& videoSourceToken, 
		ONVIF::VideoSourceConfiguration& videoSourceConfiguration) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetVideoSourceConfigurationOptions"
	//@ name=GetVideoSourceConfigurationOptions
	//@ replyName=GetVideoSourceConfigurationOptionsResponse
	//@ $videoSourceToken={direction=in, name=VideoSourceToken}
	//@ $videoSourceConfigurationOptions={direction=out, name=VideoSourceConfigurationOptions}
	virtual void getVideoSourceConfigurationOptions(
		const std::string& videoSourceToken, 
		ONVIF::VideoSourceConfigurationOptions& videoSourceConfigurationOptions) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/GetVideoSources"
	//@ name=GetVideoSources
	//@ replyName=GetVideoSourcesResponse
	//@ $token={direction=out, name=Token}
	virtual void getVideoSources(std::vector<std::string>& token) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/SendReceiveSerialCommand"
	//@ name=SendReceiveSerialCommand
	//@ replyName=SendReceiveSerialCommandResponse
	//@ $token={direction=in, name=Token}
	//@ $serialData={direction=in, name=SerialData}
	//@ $timeOut={direction=in, name=TimeOut}
	//@ $dataLength={direction=in, name=DataLength}
	//@ $delimiter={direction=in, name=Delimiter}
	//@ $serialData2={direction=out, name=SerialData}
	virtual void sendReceiveSerialCommand(
		const Poco::Optional<std::string>& token, 
		const Poco::SharedPtr<SerialData>& serialData, 
		const Poco::Optional<std::string>& timeOut, 
		Poco::Optional<int> dataLength, 
		const Poco::Optional<std::string>& delimiter, 
		Poco::SharedPtr<SerialData>& serialData2) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/SetAudioOutputConfiguration"
	//@ name=SetAudioOutputConfiguration
	//@ replyName=SetAudioOutputConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setAudioOutputConfiguration(
		const ONVIF::AudioOutputConfiguration& configuration, 
		bool forcePersistence) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/SetAudioSourceConfiguration"
	//@ name=SetAudioSourceConfiguration
	//@ replyName=SetAudioSourceConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setAudioSourceConfiguration(
		const ONVIF::AudioSourceConfiguration& configuration, 
		bool forcePersistence) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/SetDigitalInputConfigurations"
	//@ name=SetDigitalInputConfigurations
	//@ replyName=SetDigitalInputConfigurationsResponse
	//@ $digitalInputs={direction=in, name=DigitalInputs}
	virtual void setDigitalInputConfigurations(const std::vector<ONVIF::DigitalInput>& digitalInputs) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/SetRelayOutputSettings"
	//@ name=SetRelayOutputSettings
	//@ replyName=SetRelayOutputSettingsResponse
	//@ $relayOutput={direction=in, name=RelayOutput}
	virtual void setRelayOutputSettings(const ONVIF::RelayOutput& relayOutput) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/SetRelayOutputState"
	//@ name=SetRelayOutputState
	//@ namespace="http://www.onvif.org/ver10/device/wsdl"
	//@ replyName=SetRelayOutputStateResponse
	//@ $relayOutputToken={direction=in, name=RelayOutputToken}
	//@ $logicalState={direction=in, name=LogicalState}
	virtual void setRelayOutputState(
		const std::string& relayOutputToken, 
		const std::string& logicalState) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/SetSerialPortConfiguration"
	//@ name=SetSerialPortConfiguration
	//@ replyName=SetSerialPortConfigurationResponse
	//@ $serialPortConfiguration={direction=in, name=SerialPortConfiguration}
	//@ $forcePersistance={direction=in, name=ForcePersistance}
	virtual void setSerialPortConfiguration(
		const SerialPortConfiguration& serialPortConfiguration, 
		bool forcePersistance) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/SetVideoOutputConfiguration"
	//@ name=SetVideoOutputConfiguration
	//@ replyName=SetVideoOutputConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setVideoOutputConfiguration(
		const ONVIF::VideoOutputConfiguration& configuration, 
		bool forcePersistence) = 0;

	//@ action="http://www.onvif.org/ver10/deviceio/wsdl/SetVideoSourceConfiguration"
	//@ name=SetVideoSourceConfiguration
	//@ replyName=SetVideoSourceConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setVideoSourceConfiguration(
		const ONVIF::VideoSourceConfiguration& configuration, 
		bool forcePersistence) = 0;
};


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_DeviceIOBinding_INCLUDED
